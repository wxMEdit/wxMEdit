///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxmedit_app.cpp
// Description:
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_app.h"
#include "xm/cxx11.h"
#include "wxmedit_frame.h"
#include "mad_utils.h"
#include "wxm/utils.h"
#include "wxm/update.h"
#include "xm/remote.h"
#include "dialog/wxmedit_options_dialog.h"

#include "wxmedit/wxmedit.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/filename.h>
#include <wx/fileconf.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

IMPLEMENT_APP(MadEditApp)


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#ifdef __WXMSW__
HANDLE g_Mutex=NULL;
#endif

wxLocale g_Locale;

bool g_DoNotSaveSettings = false;
bool g_regex_dot_match_newline = false;
bool g_ResetAllKeys = false;

const wxChar *g_LanguageString[]=
{
    wxT("System Default"),
    wxT("\u7B80\u4F53\u4E2D\u6587 (Chinese Simplified)"),
    wxT("\u6B63\u9AD4\u4E2D\u6587 (Chinese Traditional)"),
    wxT("English"),
    wxT("Fran\u00E7ais (French)"),
    wxT("Deutsch (German)"),
    wxT("Italiano (Italian)"),
    wxT("\u65E5\u672C\u8A9E (Japanese)"),
    wxT("polski (Polish)"),
    wxT("\u0420\u0443\u0441\u0441\u043a\u0438\u0439 (Russian)"),
    wxT("Espa\u00F1ol (Spanish)"),
};
int g_LanguageValue[]=
{
    wxLANGUAGE_DEFAULT,
    wxLANGUAGE_CHINESE_SIMPLIFIED,
    wxLANGUAGE_CHINESE_TRADITIONAL,
    wxLANGUAGE_ENGLISH_US,
	wxLANGUAGE_FRENCH,
    wxLANGUAGE_GERMAN,
    wxLANGUAGE_ITALIAN,
    wxLANGUAGE_JAPANESE,
    wxLANGUAGE_POLISH,
    wxLANGUAGE_RUSSIAN,
    wxLANGUAGE_SPANISH,
};
extern const size_t g_LanguageCount = sizeof(g_LanguageValue)/sizeof(int);

#ifdef __WXGTK__

// the codes of SingleInstance checking and
// SendMessage to previous instance under GTK+
// are from gcin (http://www.csie.nctu.edu.tw/~cp76/gcin/)

#include <X11/Xatom.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#if wxMAJOR_VERSION == 2
# include <wx/gtk/win_gtk.h>
#endif

Atom g_MadEdit_atom;
Display *g_Display=nullptr;

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

    const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( msg.wc_str() );
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
    if(!g_DoNotSaveSettings)
    {
        wxFileConfig *cfg=(wxFileConfig *)wxFileConfig::Get(false);

        if(g_ResetAllKeys)
        {
            cfg->DeleteGroup(wxT("/KeyBindings"));
        }
        else
        {
            // save MadEdit::KeyBindings
            cfg->SetPath(wxT("/KeyBindings"));
            MadEdit::ms_KeyBindings.SaveToConfig(cfg);
        }

        delete cfg;
    }

    MadEdit::ms_KeyBindings.FreeCommandTextMap();

    FontWidthManager::Save();
    FontWidthManager::FreeMem();

    wxFileConfig::Set(nullptr);
}

bool OpenFilesInPrevInst(const wxString& flist)
{
#ifdef __WXMSW__
    g_Mutex = CreateMutex(NULL, true, wxT("wxMEdit_App"));
    if(GetLastError() != ERROR_ALREADY_EXISTS)
        return false;

    const wxChar wxCanvasClassNameNR[] = wxT("wxWindowClassNR"); // class name of MadEditFrame
    wxChar title[256]={0};
    HWND prevapp = ::FindWindowEx(NULL, NULL, wxCanvasClassNameNR, nullptr);
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
        prevapp =::FindWindowEx(NULL, prevapp, wxCanvasClassNameNR, nullptr);
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
    g_Display= gdk_x11_get_default_xdisplay();
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
    xm::EncodingManager::PreInit();

    xm::RemoteAccessInit();

    wxm::AppPath::Instance().Init(GetAppName());

    // parse commandline to filelist
    wxm::FileList filelist;
    for(int i=1; i<argc; i++)
    {
        wxFileName filename(argv[i]);
        filename.MakeAbsolute();
        filelist.Append(filename.GetFullPath());
    }

    // init wxConfig
    wxFileConfig *cfg=new wxFileConfig(wxEmptyString, wxEmptyString, wxm::AppPath::Instance().ConfigPath(), 
                                       wxEmptyString, wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
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

    wxm::AppPath& path = wxm::AppPath::Instance();
    g_Locale.Init(lang);
    g_Locale.AddCatalogLookupPathPrefix(wxT("./locale/"));
    g_Locale.AddCatalogLookupPathPrefix(path.AppDir() + wxT("locale/"));
    if (path.AppDir() != path.HomeDir())
        g_Locale.AddCatalogLookupPathPrefix(path.HomeDir() + wxT("locale/"));
#ifndef __WXMSW__
# ifdef DATA_DIR
    g_Locale.AddCatalogLookupPathPrefix(wxT(DATA_DIR"/locale/"));
# endif
#endif

    g_Locale.AddCatalog(wxT("wxmedit"));

    // set colors
    wxm::SetL10nHtmlColors();

    wxm::UpdatePeriods::Instance().Initialize();

#if defined(__WXMSW__) || defined(__WXGTK__)
    bool maximize=false;
    cfg->Read(wxT("/wxMEdit/WindowMaximize"), &maximize, false);
#endif
    wxPoint pos=wxDefaultPosition;
    wxSize size;
    wxRect rect = wxGetClientDisplayRect(); // FIXME: multi-monitor
    size.x = std::min(rect.width, wxm::DEFAULT_WINDOW_WIDTH);
    size.y = std::min(rect.height, wxm::DEFAULT_WINDOW_HEIGHT);

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
    FontWidthManager::Init(wxm::AppPath::Instance().HomeDir());


    // create the main frame
    MadEditFrame *myFrame = new MadEditFrame(nullptr, 1 , wxEmptyString, pos, size);
    SetTopWindow(myFrame);

#if defined(__WXMSW__) || defined(__WXGTK__)
    if (maximize)
        myFrame->Maximize(true);
#endif

    myFrame->Show(true);


#if defined(__WXGTK__)
    if(bSingleInstance)
    {
# if wxMAJOR_VERSION == 2

        GtkPizza* pizza = GTK_PIZZA(myFrame->m_mainWidget);
        GdkWindow* gwin = pizza->bin_window;
# else
        GtkWidget* widget = myFrame->GetHandle();
        GdkWindow* gwin = gtk_widget_get_window(widget);
# endif
        Window win = GDK_WINDOW_XID(gwin);
        XSetSelectionOwner(g_Display, g_MadEdit_atom, win, CurrentTime);
        gdk_window_add_filter(nullptr, my_gdk_filter, nullptr);
    }
#endif

    wxm::AutoCheckUpdates(cfg);

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

    return true;
}

int MadEditApp::OnExit()
{
    xm::RemoteAccessCleanup();

    // save settings in FrameClose();
    return 0;
}
