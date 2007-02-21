///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditCommand.cpp
// Description: define the Edit Commands and KeyBindings
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEditCommand.h"

#include <wx/confbase.h>
#include <wx/tokenzr.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

//---------------------------------------------------------------------------

#if !wxCHECK_VERSION(2,7,0)
    #define WXK_PAGEUP          WXK_PRIOR
    #define WXK_PAGEDOWN        WXK_NEXT
    #define WXK_NUMPAD_PAGEUP   WXK_NUMPAD_PRIOR
    #define WXK_NUMPAD_PAGEDOWN WXK_NUMPAD_NEXT
#endif

MadCommandTextMap *MadKeyBindings::ms_CommandTextMap=NULL;
MadTextCommandMap *MadKeyBindings::ms_TextCommandMap=NULL;
MadCommandTextMap *MadKeyBindings::ms_MenuIdTextMap=NULL;
MadTextCommandMap *MadKeyBindings::ms_TextMenuIdMap=NULL;
MadMenuCommandMap *MadKeyBindings::ms_MenuIdCommandMap=NULL;

#define INSERT_COMMANDTEXT(cmd) \
    ms_CommandTextMap->insert(MadCommandTextMap::value_type(cmd, wxT(#cmd)));\
    ms_TextCommandMap->insert(MadTextCommandMap::value_type(wxT(#cmd), cmd))

void MadKeyBindings::InitCommandTextMap()
{
    ms_CommandTextMap = new MadCommandTextMap();
    ms_TextCommandMap = new MadTextCommandMap();
    ms_MenuIdTextMap = new MadCommandTextMap();
    ms_TextMenuIdMap = new MadTextCommandMap();
    ms_MenuIdCommandMap = new MadMenuCommandMap();

    INSERT_COMMANDTEXT(ecLeft);
    INSERT_COMMANDTEXT(ecUp);
    INSERT_COMMANDTEXT(ecRight);
    INSERT_COMMANDTEXT(ecDown);
    INSERT_COMMANDTEXT(ecBeginLine);
    INSERT_COMMANDTEXT(ecEndLine);
    INSERT_COMMANDTEXT(ecBeginDoc);
    INSERT_COMMANDTEXT(ecEndDoc);
    INSERT_COMMANDTEXT(ecPrevPage);
    INSERT_COMMANDTEXT(ecNextPage);
    INSERT_COMMANDTEXT(ecPrevWord);
    INSERT_COMMANDTEXT(ecNextWord);
    INSERT_COMMANDTEXT(ecLeftBrace);
    INSERT_COMMANDTEXT(ecRightBrace);

    INSERT_COMMANDTEXT(ecSelLeft);
    INSERT_COMMANDTEXT(ecSelRight);
    INSERT_COMMANDTEXT(ecSelUp);
    INSERT_COMMANDTEXT(ecSelDown);
    INSERT_COMMANDTEXT(ecSelBeginLine);
    INSERT_COMMANDTEXT(ecSelEndLine);
    INSERT_COMMANDTEXT(ecSelBeginDoc);
    INSERT_COMMANDTEXT(ecSelEndDoc);
    INSERT_COMMANDTEXT(ecSelPrevPage);
    INSERT_COMMANDTEXT(ecSelNextPage);
    INSERT_COMMANDTEXT(ecSelPrevWord);
    INSERT_COMMANDTEXT(ecSelNextWord);
    INSERT_COMMANDTEXT(ecSelLeftBrace);
    INSERT_COMMANDTEXT(ecSelRightBrace);

    INSERT_COMMANDTEXT(ecScrollLineUp);
    INSERT_COMMANDTEXT(ecScrollLineDown);
    INSERT_COMMANDTEXT(ecScrollPageUp);
    INSERT_COMMANDTEXT(ecScrollPageDown);

    INSERT_COMMANDTEXT(ecScrollLeft);
    INSERT_COMMANDTEXT(ecScrollRight);

    INSERT_COMMANDTEXT(ecSelectAll);

    INSERT_COMMANDTEXT(ecReturn);
    INSERT_COMMANDTEXT(ecReturnNoIndent);
    INSERT_COMMANDTEXT(ecTab);
    INSERT_COMMANDTEXT(ecInsertTabChar);
    INSERT_COMMANDTEXT(ecToggleWindow);
    INSERT_COMMANDTEXT(ecDelete);
    INSERT_COMMANDTEXT(ecBackSpace);

    INSERT_COMMANDTEXT(ecCutLine);
    INSERT_COMMANDTEXT(ecDeleteLine);

    INSERT_COMMANDTEXT(ecCut);
    INSERT_COMMANDTEXT(ecCopy);
    INSERT_COMMANDTEXT(ecPaste);

    INSERT_COMMANDTEXT(ecToggleInsertMode);
    INSERT_COMMANDTEXT(ecTextMode);
    INSERT_COMMANDTEXT(ecColumnMode);
    INSERT_COMMANDTEXT(ecHexMode);

    INSERT_COMMANDTEXT(ecNoWrap);
    INSERT_COMMANDTEXT(ecWrapByWindow);
    INSERT_COMMANDTEXT(ecWrapByColumn);

    INSERT_COMMANDTEXT(ecUndo);
    INSERT_COMMANDTEXT(ecRedo);

    INSERT_COMMANDTEXT(ecIncreaseIndent);
    INSERT_COMMANDTEXT(ecDecreaseIndent);

    INSERT_COMMANDTEXT(ecComment);
    INSERT_COMMANDTEXT(ecUncomment);

    INSERT_COMMANDTEXT(ecToUpperCase);
    INSERT_COMMANDTEXT(ecToLowerCase);
    INSERT_COMMANDTEXT(ecInvertCase);

    INSERT_COMMANDTEXT(ecToHalfWidth);
    INSERT_COMMANDTEXT(ecToFullWidth);

    INSERT_COMMANDTEXT(ecInsertDateTime);
}

void MadKeyBindings::FreeCommandTextMap()
{
    ms_CommandTextMap->clear();
    ms_TextCommandMap->clear();
    ms_MenuIdTextMap->clear();
    ms_TextMenuIdMap->clear();
    ms_MenuIdCommandMap->clear();
    delete ms_CommandTextMap;
    delete ms_TextCommandMap;
    delete ms_MenuIdTextMap;
    delete ms_TextMenuIdMap;
    delete ms_MenuIdCommandMap;
}

wxString MadKeyBindings::CommandToText(MadEditCommand cmd)
{
    MadCommandTextMap::iterator textit;
    if((textit=ms_CommandTextMap->find(cmd)) != ms_CommandTextMap->end())
    {
        return textit->second;
    }
    return wxEmptyString;
}

wxString MadKeyBindings::MenuIdToText(int menuid)
{
    MadCommandTextMap::iterator textit;
    if((textit=ms_MenuIdTextMap->find(menuid)) != ms_MenuIdTextMap->end())
    {
        return textit->second;
    }
    return wxEmptyString;
}

int MadKeyBindings::TextToMenuId(const wxString &text)
{
    MadTextCommandMap::iterator it;
    if((it=ms_TextMenuIdMap->find(text)) != ms_TextMenuIdMap->end())
    {
        return it->second;
    }
    return 0;
}

void MadKeyBindings::AddMenuTextCommand(int menuid, const wxString &text, MadEditCommand cmd)
{
    ms_MenuIdTextMap->insert(MadCommandTextMap::value_type(menuid, text));
    ms_TextMenuIdMap->insert(MadTextCommandMap::value_type(text, menuid));
    ms_MenuIdCommandMap->insert(MadMenuCommandMap::value_type(menuid, cmd));
}

MadEditCommand MadKeyBindings::GetCommandFromMenuId(int menuid)
{
    MadMenuCommandMap::iterator it=ms_MenuIdCommandMap->find(menuid);
    if(it != ms_MenuIdCommandMap->end())
    {
        return it->second;
    }
    return 0;
}

//---------------------------------------------------------------------------

MadEditShortCut StringToShortCut(const wxString &text)
{
    // wxPrintf( wxT("text %s\n"), text.c_str() );

    // check for accelerators: they are given after '\t'
    //int posTab = text.Find(wxT('\t'));
    //if ( posTab != wxNOT_FOUND ) {
        // parse the accelerator string
        int keyCode = 0;
        int accelFlags = wxACCEL_NORMAL;
        wxString current;
        for ( size_t n = 0; n < text.Len(); n++ ) {
            if ( (text[n] == '+') || (text[n] == '-') ) {
                if ( current == wxT("ctrl") )
                    accelFlags |= wxACCEL_CTRL;
                else if ( current == wxT("alt") )
                    accelFlags |= wxACCEL_ALT;
                else if ( current == wxT("shift") )
                    accelFlags |= wxACCEL_SHIFT;
                else {
                    // we may have "Ctrl-+", for example, but we still want to
                    // catch typos like "Crtl-A" so only give the warning if we
                    // have something before the current '+' or '-', else take
                    // it as a literal symbol
                    if ( current.empty() )
                    {
                        current += text[n];

                        // skip clearing it below
                        continue;
                    }
                    else
                    {
                        wxLogDebug(wxT("Unknown accel modifier: '%s'"),
                                   current.c_str());
                        //wxASSERT(false);
                    }
                }

                current.clear();
            }
            else {
                current += (wxChar) wxTolower(text[n]);
            }
        }

        if ( current.empty() ) {
            wxLogDebug(wxT("No accel key found, accel string ignored."));
        }
        else {
            if ( current.Len() == 1 ) {
                // it's a letter
                keyCode = current[0U];

                // Only call wxToupper if control, alt, or shift is held down,
                // otherwise lower case accelerators won't work.
                if (accelFlags != wxACCEL_NORMAL) {
                    keyCode = wxToupper(keyCode);
                }
            }
            else {
                // is it a function key?
                if ( current[0U] == 'f' && wxIsdigit(current[1U]) &&
                     (current.Len() == 2 ||
                     (current.Len() == 3 && wxIsdigit(current[2U]))) ) {
                    keyCode = WXK_F1 + wxAtoi(current.c_str() + 1) - 1;
                }
                else {
                    // several special cases
                    current.MakeUpper();
                    if ( current == wxT("DEL") )
                        keyCode = WXK_DELETE;
                    else if ( current == wxT("DELETE") )
                        keyCode = WXK_DELETE;
                    else if ( current == wxT("BACK") )
                        keyCode = WXK_BACK;
                    else if ( current == wxT("INS") )
                        keyCode = WXK_INSERT;
                    else if ( current == wxT("INSERT") )
                        keyCode = WXK_INSERT;
                    else if ( current == wxT("ENTER") || current == wxT("RETURN") )
                        keyCode = WXK_RETURN;
                    else if ( current == wxT("LEFT") )
                        keyCode = WXK_LEFT;
                    else if ( current == wxT("RIGHT") )
                        keyCode = WXK_RIGHT;
                    else if ( current == wxT("UP") )
                        keyCode = WXK_UP;
                    else if ( current == wxT("DOWN") )
                        keyCode = WXK_DOWN;
                    else if ( current == wxT("HOME") )
                        keyCode = WXK_HOME;
                    else if ( current == wxT("END") )
                        keyCode = WXK_END;
                    else if ( current == wxT("SPACE") )
                        keyCode = WXK_SPACE;
                    else if ( current == wxT("TAB") )
                        keyCode = WXK_TAB;
                    else if ( current == wxT("ESC") || current == wxT("ESCAPE") )
                        keyCode = WXK_ESCAPE;
                    else if ( current == wxT("CANCEL") )
                        keyCode = WXK_CANCEL;
                    else if ( current == wxT("CLEAR") )
                        keyCode = WXK_CLEAR;
                    else if ( current == wxT("MENU") )
                        keyCode = WXK_MENU;
                    else if ( current == wxT("PAUSE") )
                        keyCode = WXK_PAUSE;
                    else if ( current == wxT("CAPITAL") )
                        keyCode = WXK_CAPITAL;
                    else if ( current == wxT("SELECT") )
                        keyCode = WXK_SELECT;
                    else if ( current == wxT("PRINT") )
                        keyCode = WXK_PRINT;
                    else if ( current == wxT("EXECUTE") )
                        keyCode = WXK_EXECUTE;
                    else if ( current == wxT("SNAPSHOT") )
                        keyCode = WXK_SNAPSHOT;
                    else if ( current == wxT("HELP") )
                        keyCode = WXK_HELP;
                    else if ( current == wxT("ADD") )
                        keyCode = WXK_ADD;
                    else if ( current == wxT("SEPARATOR") )
                        keyCode = WXK_SEPARATOR;
                    else if ( current == wxT("SUBTRACT") )
                        keyCode = WXK_SUBTRACT;
                    else if ( current == wxT("DECIMAL") )
                        keyCode = WXK_DECIMAL;
                    else if ( current == wxT("DIVIDE") )
                        keyCode = WXK_DIVIDE;
                    else if ( current == wxT("NUM_LOCK") )
                        keyCode = WXK_NUMLOCK;
                    else if ( current == wxT("SCROLL_LOCK") )
                        keyCode = WXK_SCROLL;
                    else if ( current == wxT("PGUP") )
                        keyCode = WXK_PAGEUP;
                    else if ( current == wxT("PGDN") )
                        keyCode = WXK_PAGEDOWN;
                    else if ( current == wxT("KP_SPACE") )
                        keyCode = WXK_NUMPAD_SPACE;
                    else if ( current == wxT("KP_TAB") )
                        keyCode = WXK_NUMPAD_TAB;
                    else if ( current == wxT("KP_ENTER") )
                        keyCode = WXK_NUMPAD_ENTER;
                    else if ( current == wxT("KP_HOME") )
                        keyCode = WXK_NUMPAD_HOME;
                    else if ( current == wxT("KP_LEFT") )
                        keyCode = WXK_NUMPAD_LEFT;
                    else if ( current == wxT("KP_UP") )
                        keyCode = WXK_NUMPAD_UP;
                    else if ( current == wxT("KP_RIGHT") )
                        keyCode = WXK_NUMPAD_RIGHT;
                    else if ( current == wxT("KP_DOWN") )
                        keyCode = WXK_NUMPAD_DOWN;
                    else if ( current == wxT("KP_PGUP") )
                        keyCode = WXK_NUMPAD_PAGEUP;
                    else if ( current == wxT("KP_PGDN") )
                        keyCode = WXK_NUMPAD_PAGEDOWN;
                    else if ( current == wxT("KP_END") )
                        keyCode = WXK_NUMPAD_END;
                    else if ( current == wxT("KP_BEGIN") )
                        keyCode = WXK_NUMPAD_BEGIN;
                    else if ( current == wxT("KP_INSERT") )
                        keyCode = WXK_NUMPAD_INSERT;
                    else if ( current == wxT("KP_DELETE") )
                        keyCode = WXK_NUMPAD_DELETE;
                    else if ( current == wxT("KP_EQUAL") )
                        keyCode = WXK_NUMPAD_EQUAL;
                    else if ( current == wxT("KP_MULTIPLY") )
                        keyCode = WXK_NUMPAD_MULTIPLY;
                    else if ( current == wxT("KP_ADD") )
                        keyCode = WXK_NUMPAD_ADD;
                    else if ( current == wxT("KP_SEPARATOR") )
                        keyCode = WXK_NUMPAD_SEPARATOR;
                    else if ( current == wxT("KP_SUBTRACT") )
                        keyCode = WXK_NUMPAD_SUBTRACT;
                    else if ( current == wxT("KP_DECIMAL") )
                        keyCode = WXK_NUMPAD_DECIMAL;
                    else if ( current == wxT("KP_DIVIDE") )
                        keyCode = WXK_NUMPAD_DIVIDE;
                    else if ( current == wxT("WINDOWS_LEFT") )
                        keyCode = WXK_WINDOWS_LEFT;
                    else if ( current == wxT("WINDOWS_RIGHT") )
                        keyCode = WXK_WINDOWS_RIGHT;
                    else if ( current == wxT("WINDOWS_MENU") )
                        keyCode = WXK_WINDOWS_MENU;
                    else if ( current == wxT("COMMAND") )
                        keyCode = WXK_COMMAND;
                    else if ( current.Left(3) == wxT("KP_") && wxIsdigit(current[3U]) )
                        keyCode = WXK_NUMPAD0 + wxAtoi(current.c_str() + 3);
                    else if ( current.Left(7) == wxT("SPECIAL") && wxIsdigit(current[7U]) )
                        keyCode = WXK_SPECIAL1 + wxAtoi(current.c_str() + 7) - 1;
                    else
                    {
                        wxLogDebug(wxT("Unrecognized accel key '%s', accel string ignored."),
                                   current.c_str());
                        return 0;
                    }
                }
            }
        }

        if ( keyCode ) {
            // we do have something
            return ShortCut(accelFlags, keyCode);
        }
    //}

    return 0;
}

wxString ShortCutToString(MadEditShortCut shortcut)
{
    wxString text;

    int flags = shortcut>>16;
    if ( flags & wxACCEL_CTRL )
        text += wxT("Ctrl-");
    if ( flags & wxACCEL_SHIFT )
        text += wxT("Shift-");
    if ( flags & wxACCEL_ALT )
        text += wxT("Alt-");

    int code = shortcut&0xFFFF;
    switch ( code )
    {
        case WXK_F1:
        case WXK_F2:
        case WXK_F3:
        case WXK_F4:
        case WXK_F5:
        case WXK_F6:
        case WXK_F7:
        case WXK_F8:
        case WXK_F9:
        case WXK_F10:
        case WXK_F11:
        case WXK_F12:
            text << wxT('F') << code - WXK_F1 + 1;
            break;

        // if there are any other keys wxGetAccelFromString() may return,
        // we should process them here
        case WXK_DELETE:
            text << wxT("DEL");
            break;
        case WXK_BACK:
            text << wxT("BACK");
            break;
        case WXK_INSERT:
            text << wxT("INS");
            break;
        case WXK_RETURN:
            text << wxT("ENTER");;
            break;
        case WXK_LEFT:
            text << wxT("LEFT");
            break;
        case WXK_RIGHT:
            text << wxT("RIGHT");
            break;
        case WXK_UP:
            text << wxT("UP");
            break;
        case WXK_DOWN:
            text << wxT("DOWN");
            break;
        case WXK_HOME:
            text << wxT("HOME");
            break;
        case WXK_END:
            text << wxT("END");
            break;
        case WXK_SPACE:
            text << wxT("SPACE");
            break;
        case WXK_TAB:
            text << wxT("TAB");
            break;
        case WXK_ESCAPE:
            text << wxT("ESC");
            break;
        case WXK_CANCEL:
            text << wxT("CANCEL");
            break;
        case WXK_CLEAR:
            text << wxT("CLEAR");
            break;
        case WXK_MENU:
            text << wxT("MENU");
            break;
        case WXK_PAUSE:
            text << wxT("PAUSE");
            break;
        case WXK_CAPITAL:
            text << wxT("CAPITAL");
            break;
        case WXK_SELECT:
            text << wxT("SELECT");
            break;
        case WXK_PRINT:
            text << wxT("PRINT");
            break;
        case WXK_EXECUTE:
            text << wxT("EXECUTE");
            break;
        case WXK_SNAPSHOT:
            text << wxT("SNAPSHOT");
            break;
        case WXK_HELP:
            text << wxT("HELP");
            break;
        case WXK_ADD:
            text << wxT("ADD");
            break;
        case WXK_SEPARATOR:
            text << wxT("SEPARATOR");
            break;
        case WXK_SUBTRACT:
            text << wxT("SUBTRACT");
            break;
        case WXK_DECIMAL:
            text << wxT("DECIMAL");
            break;
        case WXK_DIVIDE:
            text << wxT("DIVIDE");
            break;
        case WXK_NUMLOCK:
            text << wxT("NUM_LOCK");
            break;
        case WXK_SCROLL:
            text << wxT("SCROLL_LOCK");
            break;
        case WXK_PAGEUP:
            text << wxT("PGUP");
            break;
        case WXK_PAGEDOWN:
            text << wxT("PGDN");
            break;
        case WXK_NUMPAD_SPACE:
            text << wxT("KP_SPACE");
            break;
        case WXK_NUMPAD_TAB:
            text << wxT("KP_TAB");
            break;
        case WXK_NUMPAD_ENTER:
            text << wxT("KP_ENTER");
            break;
        case WXK_NUMPAD_HOME:
            text << wxT("KP_HOME");
            break;
        case WXK_NUMPAD_LEFT:
            text << wxT("KP_LEFT");
            break;
        case WXK_NUMPAD_UP:
            text << wxT("KP_UP");
            break;
        case WXK_NUMPAD_RIGHT:
            text << wxT("KP_RIGHT");
            break;
        case WXK_NUMPAD_DOWN:
            text << wxT("KP_DOWN");
            break;
        case WXK_NUMPAD_PAGEUP:
            text << wxT("KP_PGUP");
            break;
        case WXK_NUMPAD_PAGEDOWN:
            text << wxT("KP_PGDN");
            break;
        case WXK_NUMPAD_END:
            text << wxT("KP_END");
            break;
        case WXK_NUMPAD_BEGIN:
            text << wxT("KP_BEGIN");
            break;
        case WXK_NUMPAD_INSERT:
            text << wxT("KP_INSERT");
            break;
        case WXK_NUMPAD_DELETE:
            text << wxT("KP_DELETE");
            break;
        case WXK_NUMPAD_EQUAL:
            text << wxT("KP_EQUAL");
            break;
        case WXK_NUMPAD_MULTIPLY:
            text << wxT("KP_MULTIPLY");
            break;
        case WXK_NUMPAD_ADD:
            text << wxT("KP_ADD");
            break;
        case WXK_NUMPAD_SEPARATOR:
            text << wxT("KP_SEPARATOR");
            break;
        case WXK_NUMPAD_SUBTRACT:
            text << wxT("KP_SUBTRACT");
            break;
        case WXK_NUMPAD_DECIMAL:
            text << wxT("KP_DECIMAL");
            break;
        case WXK_NUMPAD_DIVIDE:
            text << wxT("KP_DIVIDE");
            break;
        case WXK_WINDOWS_LEFT:
            text << wxT("WINDOWS_LEFT");
            break;
        case WXK_WINDOWS_RIGHT:
            text << wxT("WINDOWS_RIGHT");
            break;
        case WXK_WINDOWS_MENU:
            text << wxT("WINDOWS_MENU");
            break;
        case WXK_COMMAND:
            text << wxT("COMMAND");
            break;

        default:
            if(code<WXK_DELETE && code>WXK_SPACE) //wxIsalnum(code) )
            {
                text << (wxChar)code;
                break;
            }
            if(code>=WXK_NUMPAD0 && code<=WXK_NUMPAD9)
            {
                text << wxT("KP_") << wxChar(wxT('0') + code - WXK_NUMPAD0);
                break;
            }
            if(code>=WXK_SPECIAL1 && code<=WXK_SPECIAL20)
            {
                text << wxT("SPECIAL") << code-WXK_SPECIAL1+1;
                break;
            }

            return wxEmptyString;
            //wxFAIL_MSG( wxT("unknown keyboard accel") );
    }

    return text;
}

//---------------------------------------------------------------------------

MadKeyBindings::MadKeyBindings()
{
    if(ms_CommandTextMap==NULL)
        InitCommandTextMap();

    m_KeyBindings=new MadKeyBindingList();
    m_MenuIdMap=new MadKeyBindingMap;
    m_EditCommandMap=new MadKeyBindingMap;
    m_ShortCutMap=new MadKeyBindingMap;
}

MadKeyBindings::~MadKeyBindings()
{
    MadKeyBindingList::iterator it=m_KeyBindings->begin();
    while(it != m_KeyBindings->end())
    { 
        delete *it;
        ++it;
    }
    m_KeyBindings->clear();
    delete m_KeyBindings;
    delete m_MenuIdMap;
    delete m_EditCommandMap;
    delete m_ShortCutMap;
}

void MadKeyBindings::AddDefaultBindings(bool overwrite)
{
    Add(ShortCut(wxACCEL_NORMAL, WXK_LEFT),     ecLeft, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_RIGHT),    ecRight, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_UP),       ecUp, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_DOWN),     ecDown, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_HOME),     ecBeginLine, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_END),      ecEndLine, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   WXK_HOME),     ecBeginDoc, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   WXK_END),      ecEndDoc, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_PAGEUP),   ecPrevPage, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_PAGEDOWN), ecNextPage, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   WXK_LEFT),     ecPrevWord, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   WXK_RIGHT),    ecNextWord, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   '['),          ecLeftBrace, overwrite);
    Add(ShortCut(wxACCEL_CTRL,   ']'),          ecRightBrace, overwrite);

    Add(ShortCut(wxACCEL_SHIFT, WXK_LEFT),  ecSelLeft, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_RIGHT), ecSelRight, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_UP),    ecSelUp, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_DOWN),  ecSelDown, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_HOME),  ecSelBeginLine, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_END),   ecSelEndLine, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_HOME),   ecSelBeginDoc, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_END),    ecSelEndDoc, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_PAGEUP),                ecSelPrevPage, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_PAGEDOWN),              ecSelNextPage, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_LEFT),   ecSelPrevWord, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_RIGHT),  ecSelNextWord, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, '['),        ecSelLeftBrace, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, ']'),        ecSelRightBrace, overwrite);

    Add(ShortCut(wxACCEL_CTRL, WXK_UP),                     ecScrollLineUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL, WXK_DOWN),                   ecScrollLineDown, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_UP),     ecScrollLineUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_DOWN),   ecScrollLineDown, overwrite);
    Add(ShortCut(wxACCEL_ALT, WXK_UP),                      ecScrollLineUp, overwrite);
    Add(ShortCut(wxACCEL_ALT, WXK_DOWN),                    ecScrollLineDown, overwrite);
    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_UP),      ecScrollLineUp, overwrite);
    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_DOWN),    ecScrollLineDown, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_ALT, WXK_UP),       ecScrollLineUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_ALT, WXK_DOWN),     ecScrollLineDown, overwrite);

    Add(ShortCut(wxACCEL_CTRL, WXK_PAGEUP),                   ecScrollPageUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL, WXK_PAGEDOWN),                 ecScrollPageDown, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_PAGEUP),   ecScrollPageUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_PAGEDOWN), ecScrollPageDown, overwrite);
    Add(ShortCut(wxACCEL_ALT, WXK_PAGEUP),                    ecScrollPageUp, overwrite);
    Add(ShortCut(wxACCEL_ALT, WXK_PAGEDOWN),                  ecScrollPageDown, overwrite);
    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_PAGEUP),    ecScrollPageUp, overwrite);
    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_PAGEDOWN),  ecScrollPageDown, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_ALT, WXK_PAGEUP),     ecScrollPageUp, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_ALT, WXK_PAGEDOWN),   ecScrollPageDown, overwrite);

    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_LEFT),    ecScrollLeft, overwrite);
    Add(ShortCut(wxACCEL_ALT | wxACCEL_SHIFT, WXK_RIGHT),   ecScrollRight, overwrite);


    Add(ShortCut(wxACCEL_CTRL, 'A'),            ecSelectAll, overwrite);

    Add(ShortCut(wxACCEL_NORMAL, WXK_RETURN),       ecReturn, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_NUMPAD_ENTER), ecReturn, overwrite);

    Add(ShortCut(wxACCEL_SHIFT, WXK_RETURN),       ecReturnNoIndent, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_NUMPAD_ENTER), ecReturnNoIndent, overwrite);

    Add(ShortCut(wxACCEL_NORMAL, WXK_TAB),          ecTab, overwrite);
    Add(ShortCut(wxACCEL_CTRL, WXK_TAB),            ecToggleWindow, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, WXK_TAB),            ecToggleWindow, overwrite);

#ifdef __WXMSW__
    Add(ShortCut(wxACCEL_CTRL, '~'),            ecInsertTabChar, overwrite);
#else
    Add(ShortCut(wxACCEL_CTRL, '`'),            ecInsertTabChar, overwrite);
#endif

    Add(ShortCut(wxACCEL_NORMAL, WXK_DELETE),   ecDelete, overwrite);
    Add(ShortCut(wxACCEL_NORMAL, WXK_BACK),     ecBackSpace, overwrite);

    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, 'L'),  ecCutLine, overwrite);
    Add(ShortCut(wxACCEL_CTRL, 'L'),  ecDeleteLine, overwrite);
#ifndef __WXMSW__
    Add(ShortCut(wxACCEL_CTRL, 'Y'),  ecDeleteLine, overwrite);
#endif

    Add(ShortCut(wxACCEL_CTRL, 'X'),            ecCut, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_DELETE),    ecCut, overwrite);
    Add(ShortCut(wxACCEL_CTRL, 'C'),            ecCopy, overwrite);
    Add(ShortCut(wxACCEL_CTRL, WXK_INSERT),     ecCopy, overwrite);
    Add(ShortCut(wxACCEL_CTRL, 'V'),            ecPaste, overwrite);
    Add(ShortCut(wxACCEL_SHIFT, WXK_INSERT),    ecPaste, overwrite);

    Add(ShortCut(wxACCEL_NORMAL, WXK_INSERT),   ecToggleInsertMode, overwrite);

    Add(ShortCut(wxACCEL_ALT, '1'), ecTextMode,     overwrite);
    Add(ShortCut(wxACCEL_ALT, '2'), ecColumnMode,   overwrite);
    Add(ShortCut(wxACCEL_ALT, '3'), ecHexMode,      overwrite);

    Add(ShortCut(wxACCEL_CTRL, 'Q'), ecNoWrap,       overwrite);
    Add(ShortCut(wxACCEL_CTRL, 'W'), ecWrapByWindow, overwrite);
    Add(ShortCut(wxACCEL_CTRL, 'E'), ecWrapByColumn, overwrite);

    Add(ShortCut(wxACCEL_CTRL, 'Z'),                 ecUndo, overwrite);
    Add(ShortCut(wxACCEL_ALT, WXK_BACK),             ecUndo, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, 'Z'), ecRedo, overwrite);
#ifdef __WXMSW__
    Add(ShortCut(wxACCEL_CTRL, 'Y'),                 ecRedo, overwrite);
#endif

    Add(ShortCut(wxACCEL_SHIFT, WXK_TAB),       ecDecreaseIndent, overwrite);

    Add(ShortCut(wxACCEL_CTRL, 'M'),                    ecComment, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, 'M'),    ecUncomment, overwrite);

    Add(ShortCut(wxACCEL_CTRL, 'U'), ecToUpperCase, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_SHIFT, 'U'), ecToLowerCase, overwrite);
    Add(ShortCut(wxACCEL_CTRL | wxACCEL_ALT  , 'U'), ecInvertCase, overwrite);

    Add(ShortCut(wxACCEL_NORMAL, WXK_F7), ecInsertDateTime, overwrite);

    //ecToHalfWidth
    //ecToFullWidth
}

void MadKeyBindings::RemoveByCommand(MadEditCommand cmd)
{
    MadKeyBindingMap::iterator ecit = m_EditCommandMap->find(cmd);
    if(ecit==m_EditCommandMap->end()) return;

    MadKeyBinding *kb = ecit->second;

    MadKeyBindingList::iterator it=m_KeyBindings->begin();
    MadKeyBindingList::iterator itend=m_KeyBindings->end();
    while(it!=itend)
    {
        if(*it == kb)
        {
            m_KeyBindings->erase(it);
            break;
        }
        ++it;
    }

    m_EditCommandMap->erase(cmd);
    if(kb->menuid!=0) m_MenuIdMap->erase(kb->menuid);

    if(kb->firstsc!=0)
    {
        m_ShortCutMap->erase(kb->firstsc);

        MadShortCutSet::iterator sit=kb->shortcuts.begin();
        MadShortCutSet::iterator sitend=kb->shortcuts.end();
        while(sit!=sitend)
        {
            m_ShortCutMap->erase(*sit);
            ++sit;
        }
    }
    delete kb;
}

void MadKeyBindings::RemoveByMenuId(int menuid)
{
    MadKeyBindingMap::iterator mit = m_MenuIdMap->find(menuid);
    if(mit==m_MenuIdMap->end()) return;

    MadKeyBinding *kb = mit->second;

    MadKeyBindingList::iterator it=m_KeyBindings->begin();
    MadKeyBindingList::iterator itend=m_KeyBindings->end();
    while(it!=itend)
    {
        if(*it == kb)
        {
            m_KeyBindings->erase(it);
            break;
        }
        ++it;
    }

    m_MenuIdMap->erase(menuid);
    if(kb->editcmd!=0) m_EditCommandMap->erase(kb->editcmd);

    if(kb->firstsc!=0)
    {
        m_ShortCutMap->erase(kb->firstsc);

        MadShortCutSet::iterator sit=kb->shortcuts.begin();
        MadShortCutSet::iterator sitend=kb->shortcuts.end();
        while(sit!=sitend)
        {
            m_ShortCutMap->erase(*sit);
            ++sit;
        }
    }
    delete kb;
}

wxString MadKeyBindings::GetKeyByMenuText(const wxString &text)
{
    int menuid=TextToMenuId(text);
    if(menuid!=0)
    {
        MadKeyBindingMap::iterator mit = m_MenuIdMap->find(menuid);
        if(mit != m_MenuIdMap->end())
        {
            MadKeyBinding *kb=mit->second;
            if(kb->firstsc != 0)
            {
                return ShortCutToString(kb->firstsc);
            }
        }
    }
    return wxEmptyString;
}

bool MadKeyBindings::KeyIsAssigned(const wxString &key)
{
    MadEditShortCut sc=StringToShortCut(key);
    if(sc != 0)
    {
        MadKeyBindingMap::iterator scit = m_ShortCutMap->find(sc);
        if(scit != m_ShortCutMap->end())
        {
            return true;
        }
    }
    return false;
}

void MadKeyBindings::GetKeys(int menuid, MadEditCommand editcmd, wxArrayString &keys)
{
    MadKeyBinding *kb=NULL;
    if(menuid!=0)
    {
        MadKeyBindingMap::iterator mit = m_MenuIdMap->find(menuid);
        if(mit != m_MenuIdMap->end())
        {
            kb = mit->second;
        }
    }
    else if(editcmd!=0)
    {
        MadKeyBindingMap::iterator ecit = m_EditCommandMap->find(editcmd);
        if(ecit != m_EditCommandMap->end())
        {
            kb = ecit->second;
        }
    }
    if(kb!=NULL && kb->firstsc!=0)
    {
        wxString key=ShortCutToString(kb->firstsc);
        if(!key.IsEmpty()) keys.Add(key);

        MadShortCutSet::iterator sit=kb->shortcuts.begin();
        MadShortCutSet::iterator sitend=kb->shortcuts.end();
        while(sit!=sitend)
        {
            key=ShortCutToString(*sit);
            if(!key.IsEmpty()) keys.Add(key);
            ++sit;
        }
    }
}

void MadKeyBindings::BuildAccelEntries(bool includeFirstSC, vector<wxAcceleratorEntry> &entries)
{
    MadKeyBindingList::iterator it=m_KeyBindings->begin();
    while(it!=m_KeyBindings->end())
    {
        MadKeyBinding *kb = (*it);
        MadEditShortCut sc = kb->firstsc;
        if(kb->menuid!=0 && kb->editcmd==0 && sc!=0)
        {
            if(includeFirstSC)
            {
                entries.push_back(wxAcceleratorEntry( sc>>16, sc&0xFFFF, kb->menuid ));
            }

            MadShortCutSet::iterator sit=kb->shortcuts.begin();
            MadShortCutSet::iterator sitend=kb->shortcuts.end();
            while(sit!=sitend)
            {
                sc = *sit;
                entries.push_back(wxAcceleratorEntry( sc>>16, sc&0xFFFF, kb->menuid ));
                ++sit;
            }
        }

        ++it;
    }
}

void MadKeyBindings::LoadFromConfig(wxConfigBase *config)
{
    MadEditShortCut sc;
    MadTextCommandMap::iterator cmdit;

    wxString key, text;
    long idx=0;
    bool kcont=config->GetNextEntry(key, idx);
    while(kcont)
    {
        config->Read(key, &text);

        if((sc=StringToShortCut(key))!=0)
        {
            if((cmdit=ms_TextCommandMap->find(text))!=ms_TextCommandMap->end())
            {
                Add(sc, cmdit->second, true);
            }
        }

        kcont=config->GetNextEntry(key, idx);
    }
}

void MadKeyBindings::LoadFromConfig_New(wxConfigBase *config)
{
    MadEditShortCut sc;
    MadTextCommandMap::iterator tcit;

    wxString key, text;
    long idx=0;
    bool first;
    bool kcont=config->GetNextEntry(key, idx);
    while(kcont)
    {
        config->Read(key, &text);

        if((sc=StringToShortCut(key))!=0)
        {
            first=false;
            wxStringTokenizer tkz(text);
            text=tkz.GetNextToken();
            while(!text.IsEmpty())
            {
                const wxChar ch = text[0];
                if(ch == wxT('*'))
                {
                    first=true;
                }
                else if(ch == wxT('m'))// menuXXX
                {
                    if((tcit=ms_TextMenuIdMap->find(text))!=ms_TextMenuIdMap->end())
                    {
                        Add(sc, first, tcit->second, true);
                    }
                }
                else if(ch == wxT('e'))// ecXXX
                {
                    if((tcit=ms_TextCommandMap->find(text))!=ms_TextCommandMap->end())
                    {
                        Add(sc, tcit->second, true, first);
                    }
                }
                text=tkz.GetNextToken();
            }
        }

        kcont=config->GetNextEntry(key, idx);
    }
}

void MadKeyBindings::SaveToConfig_New(wxConfigBase *config)
{
    // record all keys in config
    wxArrayString keys;
    wxString key;
    long idx=0;
    bool kcont=config->GetNextEntry(key, idx);
    while(kcont)
    {
        keys.Add(key);
        kcont=config->GetNextEntry(key, idx);
    }
    
    MadKeyBindingList::iterator it=m_KeyBindings->begin();
    while(it!=m_KeyBindings->end())
    {
        MadKeyBinding *kb= (*it);

        wxString text;
        if(kb->menuid != 0)
        {
            text = wxT(' ');
            text += MenuIdToText(kb->menuid);
        }
        if(kb->editcmd != 0)
        {
            wxString s = CommandToText(kb->editcmd);
            if(!s.IsEmpty())
            {
                text += wxT(' ');
                text += s;
            }
        }

        if(!text.IsEmpty() && kb->firstsc!=0)
        {
            key=ShortCutToString(kb->firstsc);
            config->Write(key, wxString(wxT(" *"))+text);

            int idx=keys.Index(key.c_str(), false);
            if(idx>=0) { keys.RemoveAt(idx); }

            MadShortCutSet::iterator sit=kb->shortcuts.begin();
            MadShortCutSet::iterator sitend=kb->shortcuts.end();
            while(sit!=sitend)
            {
                key=ShortCutToString(*sit);
                config->Write(key, text);
                int idx=keys.Index(key.c_str(), false);
                if(idx>=0) { keys.RemoveAt(idx); }
                ++sit;
            }
        }

        ++it;
    }

    //delete unused keys
    for(size_t i=0; i<keys.GetCount(); i++)
    {
        config->DeleteEntry(keys[i]);
    }
}

