///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditCommand.h
// Description: define the Edit Commands and KeyBindings
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITCOMMAND_H_
#define _MADEDITCOMMAND_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <wx/accel.h>
#include <wx/hashmap.h>
#include <list>
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

    ecCutLine,
    ecDeleteLine,

    ecUndo, ecRedo,
    ecCut, ecCopy, ecPaste,

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


WX_DECLARE_HASH_MAP( MadEditShortCut,
                     MadEditCommand,
                     wxIntegerHash,
                     wxIntegerEqual,
                     MadKeyBindingsMap );

WX_DECLARE_HASH_MAP( MadEditShortCut,
                     wxString,
                     wxIntegerHash,
                     wxIntegerEqual,
                     MadCommandTextMap );

WX_DECLARE_HASH_MAP( wxString,
                     MadEditShortCut,
                     wxStringHash,
                     wxStringEqual,
                     MadTextCommandMap );

//---------------------------------------------------------------------------

struct MadCommandKeys
{
    MadEditCommand cmd;
    wxArrayString keys;
};
typedef list<MadCommandKeys> MadCommandKeysList;
extern MadCommandKeysList::iterator FindCmdKeys(MadEditCommand cmd, MadCommandKeysList &list);

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
    MadKeyBindingsMap *m_KeyBindings;
    static MadCommandTextMap *ms_CommandTextMap;
    static MadTextCommandMap *ms_TextCommandMap;

public:
    static void InitCommandTextMap();
    static void FreeCommandTextMap();
    static wxString CommandToText(MadEditCommand cmd);

public:
    MadKeyBindings();
    ~MadKeyBindings();

    void AddDefaultBindings(bool overwrite);
    void Add(MadEditShortCut shortcut, MadEditCommand cmd, bool overwrite)
    {
        if(overwrite || m_KeyBindings->find(shortcut)==m_KeyBindings->end())
            m_KeyBindings->insert(MadKeyBindingsMap::value_type(shortcut, cmd));
    }
    void Remove(MadEditCommand cmd); // remove all keys of cmd

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
        MadKeyBindingsMap::iterator it = m_KeyBindings->find(shortcut);
        if(it != m_KeyBindings->end())
            return it->second;
        return ecNone;
    }
    
    void BuildCommandKeysList(MadCommandKeysList &list);

    // must config->SetPath() before call those two
    void LoadFromConfig(wxConfigBase *config);
    void SaveToConfig(wxConfigBase *config);
};

#endif

