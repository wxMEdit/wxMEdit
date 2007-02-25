///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditApp.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEditApp.h"
#include "MadEditFrame.h"
#include "MadOptionsDialog.h"
#include "MadUtils.h"

#include "MadEdit/MadEdit.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>

IMPLEMENT_APP(MadEditApp)


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#ifdef __WXMSW__
HANDLE g_Mutex=NULL;
#endif

wxLocale g_Locale;

wxString g_MadEditAppDir;
wxString g_MadEditHomeDir;
wxString g_MadEditConfigName;
bool g_DoNotSaveSettings=false;
bool g_ResetAllKeys=false;

wxChar *g_LanguageString[]=
{
    wxT("System Default"),
    wxT("Chinese Simplified"),
    wxT("Chinese Traditional"),
    wxT("English"),
    wxT("Italian"),
};
int g_LanguageValue[]=
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_CHINESE_SIMPLIFIED,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_ENGLISH_US,
    wxLANGUAGE_ITALIAN,
};
extern const size_t g_LanguageCount = sizeof(g_LanguageValue)/sizeof(int);

#ifdef __WXGTK__

// the codes of SingleInstance checking and 
// SendMessage to previous instance under GTK+
// are from gcin (http://www.csie.nctu.edu.tw/~cp76/gcin/)

#include <X11/Xatom.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <wx/gtk/win_gtk.h>

Atom g_MadEdit_atom;
Display *g_Display=NULL;

static GdkFilterReturn my_gdk_filter(GdkXEvent *xevent,
                                     GdkEvent *event,
                                     gpointer data)
{
    XEvent *xeve = (XEvent *)xevent;

    if (xeve->type == PropertyNotify) 
    {
        XPropertyEvent *xprop = &xeve->xproperty;

        if (xprop->atom == g_MadEdit_atom) 
        {
            Atom actual_type;
            int actual_format;
            u_long nitems,bytes_after;
            wxChar *message;

            if (XGetWindowProperty(g_Display, xprop->window, g_MadEdit_atom, 0, 1024*16,
                False, AnyPropertyType, &actual_type, &actual_format,
                &nitems,&bytes_after,(unsigned char**)&message) != Success) 
            {
                //dbg("err prop");
                return GDK_FILTER_REMOVE;
            }

            OnReceiveMessage(message, nitems*sizeof(wxChar));

            XFree(message);
            return GDK_FILTER_REMOVE;
        }
    }

    //if (XFilterEvent(xeve, None) == True)
    //    return GDK_FILTER_REMOVE;

    return GDK_FILTER_CONTINUE;
}

void send_message(Window madedit_win, const wxChar *msg, size_t len)
{
    Window mwin = XCreateSimpleWindow(g_Display, DefaultRootWindow(g_Display),
                    0,0,90,90,1,0,0);

    XChangeProperty(g_Display, mwin , g_MadEdit_atom, XA_STRING, sizeof(wxChar)*8,
        PropModeReplace, (unsigned char*)msg, len);

    XPropertyEvent eve;

    eve.type=PropertyNotify;
    eve.window=mwin;
    eve.state=PropertyNewValue;
    eve.atom=g_MadEdit_atom;
    XSendEvent(g_Display, madedit_win, False, 0, (XEvent *)&eve);
    XSync(g_Display,0);
    sleep(1);

    XDestroyWindow(g_Display, mwin);
}

#endif


void DeleteConfig()
{
    if(g_DoNotSaveSettings==false)
    {
        wxFileConfig *cfg=(wxFileConfig *)wxFileConfig::Get(false);

        if(g_ResetAllKeys==false)
        {
            // save MadEdit::KeyBindings
            cfg->SetPath(wxT("/KeyBindings"));
            MadEdit::ms_KeyBindings.SaveToConfig_New(cfg);
        }
        else
        {
            cfg->DeleteGroup(wxT("/KeyBindings"));
        }
        cfg->DeleteGroup(wxT("/EditKeys"));
        cfg->DeleteGroup(wxT("/MenuKeys"));

        delete cfg;
    }

    MadEdit::ms_KeyBindings.FreeCommandTextMap();

    FontWidthManager::Save();
    FontWidthManager::FreeMem();

    wxFileConfig::Set(NULL);
}

bool MadEditApp::OnInit()
{
    wxFileName filename(GetExecutablePath());
    filename.MakeAbsolute();
    g_MadEditAppDir=filename.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);

#ifdef __WXMSW__
    g_MadEditHomeDir=g_MadEditAppDir;
#else //linux: ~/.madedit
    g_MadEditHomeDir=wxStandardPaths::Get().GetUserDataDir() +wxFILE_SEP_PATH;
    if(!wxDirExists(g_MadEditHomeDir))
    {
        wxLogNull nolog; // no error message
        wxMkdir(g_MadEditHomeDir);
    }
#endif

    //wxLogMessage(g_MadEditAppDir);
    //wxLogMessage(g_MadEditHomeDir);


    // parse commandline to filenames, every file is with a trailing char '|', ex: filename1|filename2|
    wxString filenames;
    for(int i=1;i<argc;i++)
    {
        wxFileName filename(argv[i]);
        filename.MakeAbsolute();
        filenames += filename.GetFullPath() + wxT('|');
    }


    // init wxConfig
    g_MadEditConfigName=g_MadEditHomeDir+ GetAppName()+ wxT(".cfg");

    wxFileConfig *cfg=new wxFileConfig(wxEmptyString, wxEmptyString, g_MadEditConfigName, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
    cfg->SetExpandEnvVars(false);
    cfg->SetRecordDefaults(true);
    wxFileConfig::Set(cfg);

    bool bSingleInstance=true;
    cfg->Read(wxT("/MadEdit/SingleInstance"), &bSingleInstance, true);

    if(bSingleInstance)
    {
        // check SingleInstance and send filenames to previous MadEdit
#ifdef __WXMSW__
        g_Mutex = CreateMutex(NULL, true, wxT("MadEdit_App"));
        if(GetLastError() == ERROR_ALREADY_EXISTS)
        {
            extern const wxChar *wxCanvasClassNameNR;    // class name of MadEditFrame
            wxChar title[256]={0};
            HWND prevapp = ::FindWindowEx(NULL, NULL, wxCanvasClassNameNR, NULL);
            for(;;)                // find wxCanvasClassNameNR 
            {
                if(prevapp)
                {
                    int len = ::GetWindowText(prevapp, title, 256);
                    if(len>=8 && title[len-1]==wxT(' '))    // last wchar is space?
                    {
                        title[7]=0;
                        if(lstrcmp(title, wxT("MadEdit"))==0) // compare first 7 wchars
                            break;
                    }
                }
                else
                {
                    Sleep(50);
                }
                prevapp =::FindWindowEx(NULL, prevapp, wxCanvasClassNameNR, NULL);
            }

            if(prevapp != NULL)   // send msg to the previous instance 
            {
                WINDOWPLACEMENT wp;
                wp.length=sizeof(WINDOWPLACEMENT);
                GetWindowPlacement(prevapp, &wp);
                if(wp.showCmd!=SW_SHOWMINIMIZED)
                {
                    ::SetForegroundWindow(prevapp);
                }

                COPYDATASTRUCT cds =
                { 
                    (ULONG_PTR)prevapp, 
                    DWORD((filenames.length()+1)*sizeof(wxChar)), 
                    (PVOID)filenames.c_str() 
                };

                ::SendMessage(prevapp, WM_COPYDATA, 0, (LPARAM) &cds);

                DeleteConfig();
                return false;
            }
        }
#elif defined(__WXGTK__)
        g_Display=GDK_DISPLAY();
        g_MadEdit_atom = XInternAtom(g_Display, "g_MadEdit_atom", False);
        Window madedit_win;
        if ((madedit_win=XGetSelectionOwner(g_Display, g_MadEdit_atom))!=None) 
        {
            send_message(madedit_win, filenames.c_str(), filenames.length()+1);

            DeleteConfig();
            return false;
        }
#endif
    }


#ifdef __WXGTK__
    bool bDisableWarningMessage = true;
    cfg->Read(wxT("/MadEdit/DisableWarningMessage"), &bDisableWarningMessage, true);
    if(bDisableWarningMessage)
    {
        // redirect "IPP request failed" message to /dev/null
        int fdnull = open ("/dev/null", O_WRONLY, 0);
        if(fdnull >= 0)
        {
            dup2(fdnull, STDERR_FILENO);
        }
    }
#endif


    // init locale
    wxString strlang;
    cfg->Read(wxT("/MadEdit/Language"), &strlang);
    int lang=g_LanguageValue[0];
    if(!strlang.IsEmpty())
    {
        strlang.MakeLower();
        for(size_t idx=1; idx<g_LanguageCount; idx++)
        {
            if(strlang == wxString(g_LanguageString[idx]).Lower())
            {
                lang=g_LanguageValue[idx];
                break;
            }
        }
    }

    g_Locale.Init(lang);
    g_Locale.AddCatalogLookupPathPrefix(wxT("./locale/"));
    g_Locale.AddCatalogLookupPathPrefix(g_MadEditAppDir+wxT("locale/"));
#ifndef __WXMSW__
    g_Locale.AddCatalogLookupPathPrefix(g_MadEditHomeDir+wxT("locale/"));
#   if defined (DATA_DIR)
    g_Locale.AddCatalogLookupPathPrefix(wxT(DATA_DIR"/locale/"));
#   endif

#endif
    g_Locale.AddCatalog(wxT("madedit"));

    // set colors
    SetHtmlColors();

    bool maximize=false;
#ifdef __WXMSW__
    cfg->Read(wxT("/MadEdit/WindowMaximize"), &maximize, false);
#endif
    wxPoint pos=wxDefaultPosition;
    wxSize size=wxDefaultSize;

    if(!maximize)
    {
        long x=0,y=0,w=0,h=0;
        cfg->Read(wxT("/MadEdit/WindowLeft"), &x);
        cfg->Read(wxT("/MadEdit/WindowTop"), &y);
        cfg->Read(wxT("/MadEdit/WindowWidth"), &w);
        cfg->Read(wxT("/MadEdit/WindowHeight"), &h);

        if(x+w>0 && y+h>0)
        //if(w>0 && h>0)
        {
            size.x=w;
            size.y=h;

            pos.x=x;
            pos.y=y;
        }
    }


    // load FontWidth buffers
    cfg->Read(wxT("/MadEdit/FontWidthBufferMaxCount"), &FontWidthManager::MaxCount, 10);
    if(FontWidthManager::MaxCount < 4) FontWidthManager::MaxCount=4;
    else if(FontWidthManager::MaxCount>40) FontWidthManager::MaxCount=40;
    FontWidthManager::Init(g_MadEditHomeDir);


    // create the main frame
    MadEditFrame *myFrame = new MadEditFrame(NULL, 1 , wxEmptyString, pos, size);
    SetTopWindow(myFrame);
    
#ifdef __WXMSW__
    if(maximize)
    {
        WINDOWPLACEMENT wp;
        wp.length=sizeof(WINDOWPLACEMENT);
        GetWindowPlacement((HWND)myFrame->GetHWND(), &wp);
        wp.showCmd=SW_SHOWMAXIMIZED;
        SetWindowPlacement((HWND)myFrame->GetHWND(), &wp);
    }
#endif

    myFrame->Show(TRUE);  


#if defined(__WXGTK__)
    if(bSingleInstance)
    {
        GtkPizza *pizza = GTK_PIZZA(myFrame->m_mainWidget);
        Window win=GDK_WINDOW_XWINDOW(pizza->bin_window);
        XSetSelectionOwner(g_Display, g_MadEdit_atom, win, CurrentTime);
        gdk_window_add_filter(NULL, my_gdk_filter, NULL);
    }
#endif

    // reload files previously opened
    wxString files;
    cfg->Read(wxT("/MadEdit/ReloadFilesList"), &files);
    if(!files.IsEmpty())
    {
        filenames = files + filenames;
    }

    if(!filenames.IsEmpty())
    {
        // use OnReceiveMessage() to open the files
        OnReceiveMessage(filenames.c_str(), (filenames.size()+1)*sizeof(wxChar));
    }

    if(myFrame->OpenedFileCount()==0)
    {
        myFrame->OpenFile(wxEmptyString, false);
    }

    return TRUE;
}

int MadEditApp::OnExit()
{
    // save settings in FrameClose();
    return 0;
}
