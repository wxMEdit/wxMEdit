///////////////////////////////////////////////////////////////////////////////
// Name:        plugin.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADPLUGIN_H_
#define _MADPLUGIN_H_

/***************************************************************

Export Functions:

// 
PLUGIN_EXPORT int PluginExecute();

// if the plugin registers Events and the Events had been triggered, 
// Editor will call PluginOnEvents()
PLUGIN_EXPORT int PluginOnEvents(int PluginID, int nEvents, void* pParam);

// Editor send nMsg to the plugin for get or set data.
PLUGIN_EXPORT int PluginProc(int PluginID, int nMsg, void* pParam);



ChangeLogs:
v0.000009: not work yet.

***************************************************************/

#pragma pack(1)


#if defined(__GNUC__)
#   define PLUGIN_EXPORT    extern "C"
#elif defined(__MWERKS__)
#   define PLUGIN_EXPORT    extern "C" __declspec(export)
#else // __VISUALC__ __BORLANDC__ __WATCOMC__
#   define PLUGIN_EXPORT    extern "C" __declspec(dllexport)
#endif

#if !defined(PLUGIN_EXPORT)
#   define PLUGIN_EXPORT
#endif

#if defined(WIN32)
#   include <windows.h>
#   define WIN32_DLL_ENTRY()                                    \
static HINSTANCE gs_hInstance=NULL;                             \
BOOL WINAPI                                                     \
DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved)  \
{                                                               \
    if(fdwReason == DLL_PROCESS_ATTACH ){                       \
        gs_hInstance = hModule;                                 \
    }                                                           \
    return TRUE;                                                \
}
#else
#   define WIN32_DLL_ENTRY()
#endif

typedef int (*EditorProc_Proc)(int PluginID, int nMsg, void *pParam);

// Messages to Editor for EditorProc()
#define ED_GET_VERSION         1
#define ED_REGISTER_EVENTS      


// Messages to Plugin for PluginProc()

#define PL_INITIALIZE          1
// pParam = (EditorProc_Proc)

#define PL_GET_INFORMATION     2
// pParam = (PluginInformation*)
struct PluginInformation
{
    wchar_t* name;
    wchar_t* version;
    wchar_t* menutext;
    wchar_t* hint;
    char**   xpm;
    int      checkable;
};

#define PL_GET_NAME            2
#define PL_GET_VERSION         3
#define PL_GET_MENU_TEXT       4
#define PL_GET_HINT            5
#define PL_GET_XPM             6
#define PL_GET_CHECKABLE       7


// Events
#define EVENT_FIRST            0


#pragma pack()

#endif //_MADPLUGIN_H_
