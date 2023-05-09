///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxmedit_app.cpp
// Description:
// Copyright:   2013-2023  JiaYanwei   <wxmedit@gmail.com>
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

#ifdef __WXGTK__
# include <wx/log.h>
# include <gio/gio.h>
#endif // __WXGTK__


IMPLEMENT_APP(MadEditApp)


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#ifdef __WXMSW__
HANDLE g_Mutex=NULL;
#endif

wxLocale* g_locale = new wxLocale();

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

GApplication *g_app;

void on_activate(GApplication* application)
{
    wxLogTrace(wxT("wxMEdit activated%s\n"), wxT(""));
}

void on_files_open(GApplication* application, GFile** files, gint n_files, const gchar *hint)
{
    wxString wxs;
    for (gint i = 0; i < n_files; i++)
    {
        gchar *path = g_file_get_path(files[i]);
        wxs += (const wxChar*) wxConvUTF8.cMB2WX(path);
        wxs += wxT('|');
        g_free (path);
    }

    wxLogTrace(wxT("wxMEdit receive open files: %s\n"), wxs);
    OnReceiveMessage(wxs.data(), wxs.length());
}

#endif // __WXGTK__


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

bool OpenFilesInPrevInst(const wxString& flist, int argc, wxChar** argv)
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
    g_app = g_application_new("io.github.wxmedit", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(g_app, "activate", G_CALLBACK (on_activate), nullptr);
    g_signal_connect(g_app, "open", G_CALLBACK (on_files_open), nullptr);

    GError* error = nullptr;
    gboolean r0 = g_application_register(g_app, nullptr, &error);
    if (error != nullptr)
        g_printerr("g_application_register error: %s\n", error->message);
    gboolean r1 = g_application_get_is_remote(g_app);

    if (error == nullptr && r1 == FALSE) {
        return false;
    }

    std::vector<wxCharBuffer> buffers;
    std::vector<const char*> mb_argv;
    for (int i=0; i<argc; ++i)
    {
        buffers.push_back(wxConvUTF8.cWX2MB(argv[i]));
        mb_argv.push_back((const char*) buffers[i]);
    }
    g_application_run(g_app, argc, const_cast<char**>(mb_argv.data()));

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
    if(bSingleInstance && OpenFilesInPrevInst(filelist.String(), argc, argv))
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
    g_locale->Init(lang);
    g_locale->AddCatalogLookupPathPrefix(wxT("./locale/"));
    g_locale->AddCatalogLookupPathPrefix(path.AppDir() + wxT("locale/"));
    if (path.AppDir() != path.HomeDir())
        g_locale->AddCatalogLookupPathPrefix(path.HomeDir() + wxT("locale/"));
#ifndef __WXMSW__
# ifdef DATA_DIR
    g_locale->AddCatalogLookupPathPrefix(wxT(DATA_DIR"/locale/"));
# endif
#endif

    g_locale->AddCatalog(wxT("wxmedit"));

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
