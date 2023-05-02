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

#include <gtk/gtk.h>
#if wxMAJOR_VERSION == 2
# include <wx/gtk/win_gtk.h>
#endif

GdkAtom g_MadEdit_atom;
GdkDisplay *g_Display=nullptr;

static GdkFilterReturn my_gdk_filter(GdkXEvent *xevent,
                                     GdkEvent *event,
                                     gpointer data)
{

    if (event->type == GDK_PROPERTY_NOTIFY)
    {
        GdkEventProperty *pevent = (GdkEventProperty*)event;

        if (pevent->atom == g_MadEdit_atom)
        {
            GdkAtom actual_type;
            int actual_format;
            gint nitems;
            char *message;

            if( gdk_property_get(pevent->window, g_MadEdit_atom, GDK_NONE, 0, 1024 * 16,
                FALSE, &actual_type, &actual_format,
                &nitems,(unsigned char**)&message) != TRUE)
            {
                //dbg("err prop");
                return GDK_FILTER_REMOVE;
            }

            const wxWCharBuffer wcstr = wxConvUTF8.cMB2WX(message);
            size_t datalen = wcslen((const wchar_t *)wcstr);

            OnReceiveMessage((const wchar_t *)wcstr, datalen*sizeof(wchar_t));

            g_free(message);
            return GDK_FILTER_REMOVE;
        }
    }

    //if (XFilterEvent(xeve, None) == True)
    //    return GDK_FILTER_REMOVE;

    return GDK_FILTER_CONTINUE;
}

void send_message(GdkWindow *madedit_win, const wxString &msg)
{
    GdkScreen *defscreen = gdk_screen_get_default();
    GdkWindow *rootwin = gdk_screen_get_root_window(defscreen);
    GdkWindowAttr wattr;

    memset(&wattr, 0, sizeof(GdkWindowAttr));
    wattr.x = 0;
    wattr.y=0;
    wattr.width=90;
    wattr.height=90;
    wattr.event_mask=0;
    wattr.wclass = GDK_INPUT_OUTPUT;
    wattr.visual = gdk_screen_get_system_visual(defscreen);
    wattr.window_type = GDK_WINDOW_TOPLEVEL;
    wattr.cursor = NULL;
    wattr.type_hint = GDK_WINDOW_TYPE_HINT_NORMAL;

    GdkWindow *mwin = gdk_window_new(rootwin, &wattr,GDK_WA_X | GDK_WA_Y | GDK_WA_TYPE_HINT);

    const wxCharBuffer data_utf8 = wxConvUTF8.cWX2MB( msg.wc_str() );
    size_t datalen_utf8 = strlen(data_utf8);

    gdk_property_change(mwin, g_MadEdit_atom, GDK_NONE, 8,
        GDK_PROP_MODE_REPLACE,(unsigned char*)(const char*)data_utf8, datalen_utf8 + 1);

    GdkEventProperty eve;

    eve.type=GDK_PROPERTY_NOTIFY;
    eve.window=mwin;
    eve.send_event=FALSE;
    eve.state=GDK_PROPERTY_NEW_VALUE;
    eve.atom=g_MadEdit_atom;
    eve.time = 0;

    gdk_display_put_event(g_Display, (GdkEvent *)&eve);
    gdk_display_sync(g_Display);
    sleep(1);

    gdk_window_destroy(mwin);
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
    g_Display= gdk_display_get_default();
    g_MadEdit_atom = gdk_atom_intern("g_wxMEdit_atom", FALSE);
    GdkWindow* madedit_win;

    if ((madedit_win = gdk_selection_owner_get_for_display(g_Display, g_MadEdit_atom)) == nullptr)
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
        GtkWidget* widget = myFrame->GetHandle();
        GdkWindow* gwin = gtk_widget_get_window(widget);
        gdk_selection_owner_set_for_display(g_Display, gwin, g_MadEdit_atom, GDK_CURRENT_TIME, FALSE);
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
