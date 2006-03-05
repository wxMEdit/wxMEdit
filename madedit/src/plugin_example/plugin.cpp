
#include "../plugin.h"


WIN32_DLL_ENTRY();

int g_PluginID=0;
EditorProc_Proc EditorProc;

wchar_t name[]=L"TestPlugin";
#include "wxwin16x16.xpm"


PLUGIN_EXPORT int PluginExecute()
{
    int ver;
    EditorProc(g_PluginID, ED_GET_VERSION, &ver);
    return 1;
}

PLUGIN_EXPORT int PluginOnEvents(int PluginID, int nEvents, void* pParam)
{
    return 0;
}

PLUGIN_EXPORT int PluginProc(int PluginID, int nMsg, void* pParam)
{
    switch(nMsg)
    {
    case PL_INITIALIZE:
        g_PluginID=PluginID;
        EditorProc=(EditorProc_Proc)pParam;
        break;
    case PL_GET_NAME:
        *((wchar_t**)pParam)=name;
        break;
    case PL_GET_XPM:
        *((char***)pParam)=wxwin16x16_xpm;
        break;
    }
    return 1;
}

