///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxmedit_command.h
// Description: Define the Edit Commands and KeyBindings
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_COMMAND_H_
#define _WXMEDIT_COMMAND_H_

#include "../xm/cxx11.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/wxprec.h>

#ifdef __BORLANDC__
# pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
# include <wx/wx.h>
# include <wx/accel.h>
#endif
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
using boost::unordered_map;
using boost::unordered_set;

#include <vector>
#include <list>
using std::vector;
using std::list;

enum //MadEditCommand
{
    ecNone = 0,

    /*== UCS4 Range ==*/
    ecCharFirst = 0x0020, ecCharLast = 0x10FFFF,

    /*== Caret Movement Commands ==*/
    ecCaretCommandFirst = 0x800000,

    ecLeft, ecRight, ecUp, ecDown,
    ecBeginLine, ecEndLine,
    ecBeginDoc, ecEndDoc,
    ecPrevPage, ecNextPage,
    ecPrevWord, ecNextWord,

    ecLeftBrace, ecRightBrace,

    ecCaretCommandLast,

    /*== Caret Movement & Selection Commands ==*/
    ecSelCommandFirst,

    ecSelLeft, ecSelRight, ecSelUp, ecSelDown,
    ecSelBeginLine, ecSelEndLine,
    ecSelBeginDoc, ecSelEndDoc,
    ecSelPrevPage, ecSelNextPage,
    ecSelPrevWord, ecSelNextWord,

    ecSelLeftBrace, ecSelRightBrace,

    ecSelCommandLast,
    /*============================================*/

    ecSelectAll,

    ecScrollLineUp, ecScrollLineDown,
    ecScrollPageUp, ecScrollPageDown,
    ecScrollLeft, ecScrollRight,

    ecReturn, ecReturnNoIndent,
    ecTab,
    ecInsertTabChar, // useful when set InsertSpacesIsteadOfTab=true but want to insert a actual Tab char
    ecDelete,
    ecBackSpace,

    ecDelPrevWord, ecDelNextWord,

    ecCutLine,
    ecDeleteLine,

    ecUndo, ecRedo,
    ecCut, ecCopy, ecPaste, ecPasteOvr,

    ecToggleInsertMode,
    ecTextMode, ecColumnMode, ecHexMode,

    ecNoWrap, ecWrapByWindow, ecWrapByColumn,

    ecToggleWindow, // default: Ctrl-Tab

    ecIncreaseIndent, ecDecreaseIndent,
    ecComment, ecUncomment,

    ecToUpperCase, ecToLowerCase, ecInvertCase,
    ecToHalfWidth, ecToFullWidth,
    ecInsertDateTime,

    ecMouseNotify // just a notification for MouseEvent
};

//---------------------------------------------------------------------------

// MadEditShortCut:
// high word: wxACCEL_ALT | wxACCEL_CTRL | wxACCEL_SHIFT
// low  word: wxKeyCode
typedef int MadEditShortCut;
typedef int MadEditCommand;

typedef unordered_map<MadEditShortCut, wxString> MadCommandTextMap;
typedef unordered_map<wxString, MadEditShortCut, wxStringHash> MadTextCommandMap;

typedef unordered_map<int, MadEditShortCut> MadMenuCommandMap;

//---------------------------------------------------------------------------

typedef unordered_set<MadEditShortCut> MadShortCutSet;

struct MadKeyBinding
{
    int             menuid;
    MadEditCommand  editcmd;

    MadEditShortCut firstsc;
    MadShortCutSet  shortcuts; // the rest shortcuts

    MadKeyBinding(): menuid(0), editcmd(0), firstsc(0) {}

    void Add(int mid, MadEditCommand cmd, MadEditShortCut sc)
    {
        if(mid!=0) menuid=mid;
        if(cmd!=0) editcmd=cmd;
        if(firstsc==0) firstsc=sc;
        else shortcuts.insert(sc);
    }
    void AddFirst(int mid, MadEditCommand cmd, MadEditShortCut sc)
    {
        if(mid!=0) menuid=mid;
        if(cmd!=0) editcmd=cmd;
        if(firstsc==0) firstsc=sc;
        else if(firstsc!=sc)
        {
            shortcuts.insert(firstsc);
            firstsc=sc;
        }
    }

    void RemoveShortCut(MadEditShortCut sc)
    {
        if(firstsc!=0)
        {
            if(firstsc==sc)
            {
                if(shortcuts.begin() != shortcuts.end())
                {
                    firstsc = *shortcuts.begin();
                    shortcuts.erase(shortcuts.begin());
                }
                else
                {
                    firstsc = 0;
                }
            }
            else
            {
                shortcuts.erase(sc);
            }
        }
    }

    bool HasShortCut(MadEditShortCut sc)
    {
        return (sc==firstsc) || (shortcuts.find(sc)!=shortcuts.end()) ;
    }
};

typedef list<MadKeyBinding*> MadKeyBindingList;

// menuid or editcmd or shortcut => MadKeyBinding*
typedef unordered_map<int, MadKeyBinding*> MadKeyBindingMap;

//---------------------------------------------------------------------------

inline MadEditShortCut ShortCut(int flags, int keyCode)
{
    return (flags<<16) | keyCode;
}

MadEditShortCut StringToShortCut(const wxString &text);
wxString ShortCutToString(MadEditShortCut shortcut);

//---------------------------------------------------------------------------

class wxConfigBase;

class MadKeyBindings
{
private:
    // editcommand
    static MadCommandTextMap *ms_CommandTextMap;
    static MadTextCommandMap *ms_TextCommandMap;
    // menuid
    static MadCommandTextMap *ms_MenuIdTextMap;
    static MadTextCommandMap *ms_TextMenuIdMap;
    static MadMenuCommandMap *ms_MenuIdCommandMap;

    MadKeyBindingList *m_KeyBindings;
    MadKeyBindingMap *m_MenuIdMap;
    MadKeyBindingMap *m_EditCommandMap;
    MadKeyBindingMap *m_ShortCutMap;

public:
    static void InitCommandTextMap();
    static void FreeCommandTextMap();

    static wxString CommandToText(MadEditCommand cmd);
    static wxString MenuIdToText(int menuid);
    static int TextToMenuId(const wxString &text);

    static void AddMenuTextCommand(int menuid, const wxString &text, MadEditCommand cmd);
    static MadEditCommand GetCommandFromMenuId(int menuid);

public:
    MadKeyBindings();
    ~MadKeyBindings();

    void AddDefaultBindings(bool overwrite);
    void Add(MadEditShortCut shortcut, bool first, int menuid, bool overwrite)
    {
        MadKeyBindingMap::iterator scit = m_ShortCutMap->find(shortcut);
        if(scit == m_ShortCutMap->end())
        {
            overwrite=true;
        }
        else if(overwrite)
        {
            //remove old corelation
            scit->second->RemoveShortCut(shortcut);
        }
        if(overwrite)
        {
            MadKeyBinding *kb;
            MadKeyBindingMap::iterator mit = m_MenuIdMap->find(menuid);
            if(mit != m_MenuIdMap->end())
            {
                kb = mit->second;
            }
            else
            {
                kb = new MadKeyBinding;
                m_KeyBindings->push_back(kb);
            }

            MadEditCommand cmd = GetCommandFromMenuId(menuid);
            if(first) kb->AddFirst(menuid, cmd, shortcut);
            else      kb->Add(menuid, cmd, shortcut);

            m_MenuIdMap->insert(MadKeyBindingMap::value_type(menuid, kb));
            if(cmd!=0) m_EditCommandMap->insert(MadKeyBindingMap::value_type(cmd, kb));
            m_ShortCutMap->insert(MadKeyBindingMap::value_type(shortcut, kb));
        }
    }
    void Add(MadEditShortCut shortcut, MadEditCommand cmd, bool overwrite, bool first = false)
    {
        MadKeyBindingMap::iterator scit = m_ShortCutMap->find(shortcut);
        if(scit == m_ShortCutMap->end())
        {
            overwrite=true;
        }
        else if(overwrite)
        {
            //remove old corelation
            scit->second->RemoveShortCut(shortcut);
        }
        if(overwrite)
        {
            MadKeyBinding *kb=nullptr;
            MadKeyBindingMap::iterator ecit = m_EditCommandMap->find(cmd);
            if(ecit != m_EditCommandMap->end())
            {
                kb = ecit->second;
            }
            else
            {
                kb = new MadKeyBinding;
                m_KeyBindings->push_back(kb);
            }

            if(first) kb->AddFirst(0, cmd, shortcut);
            else      kb->Add(0, cmd, shortcut);
            m_EditCommandMap->insert(MadKeyBindingMap::value_type(cmd, kb));
            m_ShortCutMap->insert(MadKeyBindingMap::value_type(shortcut, kb));
        }
    }

    void RemoveByCommand(MadEditCommand cmd); // remove all keys of cmd
    void RemoveByMenuId(int menuid); // remove all keys of menuid

    wxString GetKeyByMenuText(const wxString &text);
    bool KeyIsAssigned(const wxString &key);
    void GetKeys(int menuid, MadEditCommand editcmd, wxArrayString &keys);

    // for menuid only
    void BuildAccelEntries(bool includeFirstSC, vector<wxAcceleratorEntry> &entries);

    bool IsEmpty()
    {
        return m_KeyBindings->empty();
    }

    MadEditCommand FindCommand(int flags, int keyCode)
    {
        return FindCommand(ShortCut(flags, keyCode));
    }
    MadEditCommand FindCommand(MadEditShortCut shortcut)
    {
        MadKeyBindingMap::iterator scit = m_ShortCutMap->find(shortcut);
        if(scit != m_ShortCutMap->end())
        {
            return scit->second->editcmd;
        }
        return ecNone;
    }

    // must config->SetPath() before call these functions
    void LoadFromConfig(wxConfigBase *config);
    void SaveToConfig(wxConfigBase *config);
};

#endif

