///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxmedit_app.cpp
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_app.h"
#include "wxmedit_frame.h"
#include "wxm_utils.h"
#include "dialog/wxm_options_dialog.h"
#include "xm/wxm_update.h"

#include "wxmedit/wxmedit.h"

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

const wxChar *g_LanguageString[]=
{
    wxT("System Default"),
    wxT("\u7B80\u4F53\u4E2D\u6587(Chinese Simplified)"),
    wxT("\u6B63\u9AD4\u4E2D\u6587(Chinese Traditional)"),
    wxT("English"),
    wxT("Italiano(Italian)"),
    wxT("\u65E5\u6587(Japanese)"),
    wxT("Espa\u00F1ol(Spanish)"),
    wxT("\u0440\u0443\u0441\u0441\u043a\u0438\u0439(Russian)"),
};
int g_LanguageValue[]=
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_CHINESE_SIMPLIFIED,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_ENGLISH_US,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_JAPANESE,
    wxLANGUAGE_SPANISH,
    wxLANGUAGE_RUSSIAN,
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
            char *message;

            if (XGetWindowProperty(g_Display, xprop->window, g_MadEdit_atom, 0, 1024*16,
                False, AnyPropertyType, &actual_type, &actual_format,
                &nitems,&bytes_after,(unsigned char**)&message) != Success)
            {
                //dbg("err prop");
                return GDK_FILTER_REMOVE;
            }

            const wxWCharBuffer wcstr = wxConvUTF8.cMB2WX(message);
            size_t datalen = wcslen((const wchar_t *)wcstr);

            OnReceiveMessage((const wchar_t *)wcstr, datalen*sizeof(wchar_t));

            XFree(message);
            return GDK_FILTER_REMOVE;
        }
    }

    //if (XFilterEvent(xeve, None) == True)
    //    return GDK_FILTER_REMOVE;

    return GDK_FILTER_CONTINUE;
}

void send_message(Window madedit_win, const wxString &msg)
{
    Window mwin = XCreateSimpleWindow(g_Display, DefaultRootWindow(g_Display),
                    0,0,90,90,1,0,0);

    const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( msg );
    size_t datalen_utf8 = strlen(data_utf8);

    XChangeProperty(g_Display, mwin , g_MadEdit_atom, XA_STRING, 8,
        PropModeReplace, (unsigned char*)(const char*)data_utf8, datalen_utf8 + 1);

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

bool OpenFilesInPrevInst(const wxString& flist)
{
#ifdef __WXMSW__
    g_Mutex = CreateMutex(NULL, true, wxT("wxMEdit_App"));
    if(GetLastError() != ERROR_ALREADY_EXISTS)
        return false;

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
                if(lstrcmp(title, wxT("wxMEdit"))==0) // compare first 7 wchars
                    break;
            }
        }
        else
        {
            Sleep(50);
        }
        prevapp =::FindWindowEx(NULL, prevapp, wxCanvasClassNameNR, NULL);
    }

    if(prevapp == NULL)
        return false;

   // send msg to the previous instance
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
        DWORD((flist.length()+1)*sizeof(wxChar)),
        (PVOID)(const wxChar*)flist.c_str()
    };

    ::SendMessage(prevapp, WM_COPYDATA, 0, (LPARAM) &cds);

    g_DoNotSaveSettings = true;
    DeleteConfig();

	return true;
#elif defined(__WXGTK__)
    g_Display=GDK_DISPLAY();
    g_MadEdit_atom = XInternAtom(g_Display, "g_wxMEdit_atom", False);
    Window madedit_win;

    if ((madedit_win=XGetSelectionOwner(g_Display, g_MadEdit_atom)) == None)
        return false;

	send_message(madedit_win, flist);

    g_DoNotSaveSettings = true;
    DeleteConfig();
    return true;
#else // not supported
    return false;
#endif
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


    // parse commandline to filelist
    wxm::FileList filelist;
    for(int i=1; i<argc; i++)
    {
        wxFileName filename(argv[i]);
        filename.MakeAbsolute();
        filelist.Append(filename.GetFullPath());
    }


    // init wxConfig
    g_MadEditConfigName=g_MadEditHomeDir+ GetAppName()+ wxT(".cfg");

    wxFileConfig *cfg=new wxFileConfig(wxEmptyString, wxEmptyString, g_MadEditConfigName, wxEmptyString, wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
    cfg->SetExpandEnvVars(false);
    cfg->SetRecordDefaults(true);
    wxFileConfig::Set(cfg);

    bool bSingleInstance=true;
    cfg->Read(wxT("/wxMEdit/SingleInstance"), &bSingleInstance, true);

    // check SingleInstance and send filelist to previous instance
    if(bSingleInstance && OpenFilesInPrevInst(filelist.String()))
        return false;

#ifdef __WXGTK__
    bool bDisableWarningMessage = true;
    cfg->Read(wxT("/wxMEdit/DisableWarningMessage"), &bDisableWarningMessage, true);
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
    cfg->Read(wxT("/wxMEdit/Language"), &strlang);
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
    g_Locale.AddCatalog(wxT("wxmedit"));

    // set colors
    wxm::SetL10nHtmlColors();

    wxm::UpdatePeriods::Instance().Initialize();

    wxString behav;
    cfg->Read(wxT("/wxMEdit/BehaviorCopyingInHexArea"), &behav);
    wxm::HexAreaClipboardCopyProxy::Instance().SelectCopierByConfig(behav);

#ifdef __WXMSW__
    bool maximize=false;
    cfg->Read(wxT("/wxMEdit/WindowMaximize"), &maximize, false);
#endif
    wxPoint pos=wxDefaultPosition;
    wxSize size=wxDefaultSize;

    long x=0,y=0,w=0,h=0;
    cfg->Read(wxT("/wxMEdit/WindowLeft"), &x);
    cfg->Read(wxT("/wxMEdit/WindowTop"), &y);
    cfg->Read(wxT("/wxMEdit/WindowWidth"), &w);
    cfg->Read(wxT("/wxMEdit/WindowHeight"), &h);

    if(x+w>0 && y+h>0)
    //if(w>0 && h>0)
    {
        size.x=w;
        size.y=h;

        pos.x=x;
        pos.y=y;
    }

    // load FontWidth buffers
    cfg->Read(wxT("/wxMEdit/FontWidthBufferMaxCount"), &FontWidthManager::MaxCount, 10);
    if(FontWidthManager::MaxCount < 4) FontWidthManager::MaxCount=4;
    else if(FontWidthManager::MaxCount>40) FontWidthManager::MaxCount=40;
    FontWidthManager::Init(g_MadEditHomeDir);


    // create the main frame
    MadEditFrame *myFrame = new MadEditFrame(NULL, 1 , wxEmptyString, pos, size);
    SetTopWindow(myFrame);

#ifdef __WXMSW__
    WINDOWPLACEMENT wp;
    wp.length=sizeof(WINDOWPLACEMENT);
    GetWindowPlacement((HWND)myFrame->GetHWND(), &wp);
    wp.showCmd = maximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
    SetWindowPlacement((HWND)myFrame->GetHWND(), &wp);
#endif

    myFrame->Show(true);


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
    cfg->Read(wxT("/wxMEdit/ReloadFilesList"), &files);
    files += filelist.String();

    if(!files.IsEmpty())
    {
        // use OnReceiveMessage() to open the files
        OnReceiveMessage(files.c_str(), (files.size()+1)*sizeof(wxChar));
    }

    if(myFrame->OpenedFileCount()==0)
    {
        myFrame->OpenFile(wxEmptyString, false);
    }

    wxm::AutoCheckUpdates(cfg);

    return true;
}

int MadEditApp::OnExit()
{
    // save settings in FrameClose();
    return 0;
}
