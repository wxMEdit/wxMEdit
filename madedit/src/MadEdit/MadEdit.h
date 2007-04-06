///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEdit.h
// Description: main Edit component of MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDIT_H_
#define _MADEDIT_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <wx/confbase.h>
#include <string>

#include "MadLines.h"
#include "MadEditCommand.h"
#include "MadEncoding.h"
#include "MadSyntax.h"
#include "MadUndo.h"
#include "ucs4_t.h"


enum { ID_VSCROLLBAR=19876, ID_HSCROLLBAR };

class MadEdit;

typedef void (*OnSelectionChangedPtr)(MadEdit *madedit);
typedef void (*OnStatusChangedPtr)(MadEdit *madedit);
typedef void (*OnToggleWindowPtr)(MadEdit *madedit);
typedef void (*OnMouseRightUpPtr)(MadEdit *madedit);
typedef void (*OnActivatePtr)(MadEdit *madedit);

//==============================================================================
// A Manager of FontWidth Buffer
//==============================================================================
class FontWidthManager
{
public:
    struct FontWidthBuffer
    {
        wchar_t fontname[128];
        int fontsize;
        wxUint16 *widths;

        FontWidthBuffer() {}
        FontWidthBuffer(const wxString &fn, int fs, wxUint16 *ws)
            :fontsize(fs), widths(ws)
        {
            size_t count=fn.Length();
            if(count>127) count=127;
            memcpy(fontname, fn.c_str(), count*sizeof(wchar_t));
            memset(fontname+count, 0, (128-count)*sizeof(wchar_t));
        }
    };

    static int MaxCount;
    static wxString DataDir;
    typedef list<FontWidthBuffer> FontWidthBuffers;
    static vector<FontWidthBuffers> FontWidthBuffersVector; //0~16

    typedef pair<wxUint16 *, bool> VerifiedFlag;
    static list<VerifiedFlag> VerifiedFlagList; // indicate FontWidthBuffer is verified or not

private:
    static bool VerifyFontWidths(wxUint16 *widths, const wxString &fontname, int fontsize, wxWindow *win);
    static void ClearBuffer_1_16(const wxString &fontname, int fontsize); //clear FontWidthBuffersVector[1~16] with the same name,size
    static bool LoadFromFile(const wxString &filename, FontWidthBuffers &fwbuffers, bool verify);

public:
    static void Init(const wxString &datadir); // where to load data file
    static wxUint16 *GetFontWidths(int index, const wxString &fontname, int fontsize, wxWindow *win);
    static void Save(); // save all FontWidthBuffer to file in datadir
    static void FreeMem();
};

//==================================================
enum MadWordWrapMode
{ wwmNoWrap, wwmWrapByWindow, wwmWrapByColumn };

enum MadEditMode
{ emTextMode, emColumnMode, emHexMode };

enum MadCaretType
{ ctVerticalLine, ctHorizontalLine, ctBlock };

enum MadNewLineType
{ nltDefault /*Depends on Platform*/, nltDOS /*0D0A*/ , nltUNIX /*0A*/ , nltMAC /*0D*/  };

enum MadConvertEncodingFlag
{
  cefNone,
  cefSC2TC, // Simplified Chinese  ==> Traditional Chinese
  cefTC2SC, // Traditional Chinese ==> Simplified Chinese
  cefJK2TC, // Japanese Kanji      ==> Traditional Chinese
  cefJK2SC, // Japanese Kanji      ==> Simplified Chinese
  cefC2JK   // Trad&Simp Chinese   ==> Japanese Kanji
};

// flags for SortLines
typedef int MadSortFlags;
enum
{ sfAscending=0, sfDescending=1, sfCaseSensitive=2, sfRemoveDuplicate=4, sfNumericSort=8 };

//==================================================
typedef std::basic_string<ucs4_t> ucs4string;

class MadMouseMotionTimer;

struct MadCaretPos
{
    wxFileOffset pos;     // position of whole file
    int rowid;            // row-id. of whole file

    wxFileOffset linepos; // position of this line
    MadLineIterator iter; // line iterator
    int lineid;           // id. of iterator in m_Lines->m_LineList
    int subrowid;         // sub row-id. in this line

    int xpos;
    int extraspaces;      // in ColumnMode ,use it when caret beyond EndOfLine/Row

    // begin must be m_Lines->m_LineList.begin()
    void Reset(const MadLineIterator & begin)
    {
        iter = begin;
        pos = iter->m_RowIndices.front().m_Start;
        linepos = pos;

        rowid = 0;
        lineid = 0;
        subrowid = 0;

        xpos = 0;
        extraspaces = 0;
    }

    /*
    void operator=(const MadCaretPos &cp)
    {
        pos = cp.pos;
        rowid = cp.rowid;
        linepos = cp.linepos;
        iter = cp.iter;
        lineid = cp.lineid;
        subrowid = cp.subrowid;
        xpos = cp.xpos;
        extraspaces = cp.extraspaces;
    }
    */

};

struct UCIterator;
enum MadSearchResult 
{ SR_EXPR_ERROR=-2, SR_YES=-1, SR_NO=0 };  // returned state of Search & Replace

#define MadEditSuperClass wxWindow //wxScrolledWindow//wxPanel//wxControl//

class MadEdit: public MadEditSuperClass
{
    DECLARE_EVENT_TABLE()
public:
    static MadKeyBindings ms_KeyBindings;

private:
    friend class MadSyntax;
    friend class MadEncoding;
    friend class MadLines;
    friend class MadMouseMotionTimer;

    static int      ms_Count; // the count of MadEdit

    wxScrollBar     *m_VScrollBar, *m_HScrollBar;
    int             m_VSBWidth, m_VSBHeight, m_HSBWidth, m_HSBHeight;
    int             m_ClientWidth, m_ClientHeight; // client size exclude scrollbar

    wxConfigBase    *m_Config;

    MadLines        *m_Lines;
    MadSyntax       *m_Syntax;
    MadEncoding     *m_Encoding;

    MadUndoBuffer   *m_UndoBuffer;
    MadUndo         *m_SavePoint;
    bool            m_RecordCaretMovements;

    ucs4_t          *m_WordBuffer;
    int             *m_WidthBuffer;
    wchar_t         *m_WCWordBuffer;
    int             *m_WCWidthBuffer;

    wxBitmap        *m_ClientBitmap, *m_MarkBitmap;
    int             m_LastPaintBitmap;// 0:client, 1:mark

    wxPoint         m_Space_Points[4], m_EOF_Points[4];
    wxPoint         m_CR_Points[10], m_LF_Points[10], m_CRLF_Points[10];

    MadCaretPos     m_CaretPos;
    MadUCQueue      m_ActiveRowUChars;  // ucs4 char cache of active row
    vector<int>     m_ActiveRowWidths;  // width cache of active row
    int             m_CaretRowUCharPos; // ucs4 char pos of active row
    int             m_LastCaretXPos;    // when move caret up/down, use this to calc the nearest xpos

    // generally m_ValidPos==TopRow, but sometimes not
    MadLineIterator m_ValidPos_iter;    // line iterator
    int             m_ValidPos_lineid;  // line-id. of iterator
    int             m_ValidPos_rowid;   // row-id. of iter
    wxFileOffset    m_ValidPos_pos;     // position of line in whole file
    int             m_UpdateValidPos;   // ==0: no update; <0 update always; >0 update if newpos<oldpos

    bool            m_Selection;
    MadCaretPos     m_SelectionPos1, m_SelectionPos2;
    MadCaretPos     *m_SelectionBegin, *m_SelectionEnd;

    int             m_SelFirstRow, m_SelLastRow;        // for repaint
    int             m_SelLeftXPos, m_SelRightXPos;      // for ColumnMode

    wxFont          *m_TextFont; // readonly, set it at SetTextFont()
    int             m_TextFontHeight;
    int             m_TextFontAveCharWidth;
    int             m_TextFontMaxDigitWidth;

    wxFont          *m_HexFont; // readonly, set it at SetHexFont()
    int             m_HexFontHeight;
    int             m_HexFontMaxDigitWidth;

    wxUint16        *m_TextFontWidths[17], *m_HexFontWidths[17];

    bool            m_SingleLineMode;
    bool            m_StorePropertiesToGlobalConfig;

    bool            m_FixedWidthMode;

    bool            m_RepaintAll;
    bool            m_RepaintSelection;


    MadEditMode     m_EditMode;
    int             m_MaxColumnRowWidth;

    bool            m_DoRecountLineWidth;
    int             m_OldWidth, m_OldHeight;

    MadWordWrapMode m_WordWrapMode;

    bool            m_Modified;
    time_t          m_ModificationTime;
    bool            m_ReadOnly;

    int             m_TopRow, m_TextTopRow, m_HexTopRow;
    int             m_DrawingXPos;

    int             m_CompleteRowCount;
    int             m_VisibleRowCount;
    int             m_PageRowCount;

    int             m_RowHeight;
    long            m_LineSpacing;
    long            m_MaxColumns;

    int             m_LineNumberAreaWidth;
    int             m_LeftMarginWidth;
    int             m_RightMarginWidth;

    // for OnSize()
    int             m_MaxWidth, m_MaxHeight;


    MadNewLineType  m_NewLineType, m_InsertNewLineType;
    long            m_MaxLineLength;
    bool            m_HasTab;
    long            m_TabColumns;
    long            m_IndentColumns;
    bool            m_InsertSpacesInsteadOfTab;
    bool            m_WantTab;

    bool            m_AutoIndent;

    bool            m_DisplayLineNumber;
    bool            m_ShowEndOfLine, m_ShowSpaceChar, m_ShowTabChar, m_MarkActiveLine;
    bool            m_MarkBracePair;

    bool            m_InsertMode;
    MadCaretType    m_CaretType;

    bool            m_MouseLeftDown, m_MouseLeftDoubleClick;
    int             m_DoubleClickX, m_DoubleClickY;
    int             m_LeftClickX, m_LeftClickY;     // for singleline & setfocus
    bool            m_MouseAtHexTextArea;

    MadMouseMotionTimer *m_MouseMotionTimer;

    bool            m_MouseSelectToCopy;
    bool            m_MouseSelectToCopyWithCtrlKey; // enable or disable when pressing Ctrl key
    bool            m_MiddleMouseToPaste;

    wxBitmap        *m_HexDigitBitmap;
    wxDC            *m_HexDigitDC;    // temp dc, must set it on OnPaint()
    bool            m_CaretAtHexArea;
    bool            m_CaretAtHalfByte;
    int             m_HexRowCount;
    vector<wxFileOffset> m_HexRowIndex;     // HexMode row index
    int             m_TextAreaXPos;         // in HexMode, the xpos of caret in TextArea
    int             m_LastTextAreaXPos;     // for Up,Down,PageUp,PageDown...

    // if brace_rowid < 0 indicate the brace is invalid
    int             m_LeftBrace_rowid, m_RightBrace_rowid;
    BracePairIndex  m_LeftBrace, m_RightBrace;

    bool            m_AutoCompletePair;
    ucs4_t          m_AutoCompleteRightChar;
    wxFileOffset    m_AutoCompletePos;

    bool            m_LoadingFile;
    bool            m_Painted;

    // speed-up flags for FindInFiles
    bool            m_UseDefaultSyntax;
    bool            m_SearchWholeWord;

    OnSelectionChangedPtr m_OnSelectionChanged;
    OnStatusChangedPtr    m_OnStatusChanged;
    OnToggleWindowPtr     m_OnToggleWindow;
    OnMouseRightUpPtr     m_OnMouseRightUp;
    OnActivatePtr         m_OnActivate;

#ifdef __WXMSW__
    bool m_IsWin98;
    int  m_Win98LeadByte; // fixed that input DBCS char under win98
    bool m_ProcessWin98LeadByte;
#endif

protected:

    // GetLineByXXX() will get the wanted line from the nearest position(begin of lines, end of lines, or m_ValidPos)
    // IN:  rowid in whole file
    // OUT: lit of wanted line
    // OUT: rowid of the line
    // OUT: pos of the line
    // return: lineid of the line
    int GetLineByRow(/*OUT*/ MadLineIterator &lit, /*OUT*/ wxFileOffset &pos, /*IN_OUT*/ int &rowid);
    // IN:  pos in whole file
    // OUT: lit of wanted line
    // OUT: rowid of the line
    // OUT: pos of the line
    // return: lineid of the line
    int GetLineByPos(/*OUT*/ MadLineIterator &lit, /*IN_OUT*/ wxFileOffset &pos, /*OUT*/ int &rowid);
    // IN:  lineid in whole file
    // OUT: lit of wanted line
    // OUT: pos of the line
    // return: rowid of the line
    int GetLineByLine(/*OUT*/ MadLineIterator &lit, /*OUT*/ wxFileOffset &pos, /*IN*/ int lineid);

    // IN: caretPos.iter linepos subrowid ; not used: pos lineid rowid
    // UPDATE:caretPos.xpos extraspaces and ucharQueue widthArray ucharPos
    void UpdateCaret(MadCaretPos &caretPos,
                     MadUCQueue &ucharQueue, vector<int> &widthArray,
                     int &ucharPos);

    // IN: caretPos.pos
    // UPDATE: caretPos.all and ucharQueue widthArray ucharPos
    void UpdateCaretByPos(MadCaretPos &caretPos,
                          MadUCQueue &ucharQueue, vector<int> &widthArray,
                          int &ucharPos);

    // caretPos.pos linepos must be begin of row
    // IN: caretPos.iter subrowid ; not used: lineid rowid
    // UPDATE:caretPos.xpos pos linepos extraspaces and ucharQueue widthArray ucharPos
    void UpdateCaretByXPos(int xPos, MadCaretPos &caretPos,
                           MadUCQueue &ucharQueue, vector<int> &widthArray,
                           int &ucharPos);

    // update the two SelectionPos by their .pos
    void UpdateSelectionPos();

    void PaintText(wxDC *dc, int x, int y, const ucs4_t *text, const int *width, int count, int minleft, int maxright);
    void PaintTextLines(wxDC *dc, const wxRect &rect, int toprow, int rowcount, const wxColor &bgcolor);

    void PaintHexDigit(wxDC *dc, int x, int y, const ucs4_t *hexdigit, const int *width, int count);
    void PaintHexOffset(wxDC *dc, int x, int y, const ucs4_t *hexdigit, const int *width, int count);
    void PaintHexLines(wxDC *dc, wxRect &rect, int toprow, int rowcount, bool painthead);

    int GetVisibleHexRowCount();
    void PrepareHexRowIndex(int toprow, int count);
    void UpdateTextAreaXPos();
    void ToggleHexTextArea();
    void AppearHexRow(wxFileOffset pos);
    void UpdateHexPosByXPos(int row, int xpos);

    void RecountLineWidth(bool bForceRecount);
    void ReformatAll();

    void UpdateAppearance();
    void UpdateScrollBarPos();

    void BeginUpdateSelection();
    void EndUpdateSelection(bool bSelection);

    void SetSelection(wxFileOffset beginpos, wxFileOffset endpos);

    wxFileOffset GetColumnSelection(wxString *ws);

    // if(ws==NULL) SelectWord only;
    // else GetWord to ws;
    void SelectWordFromCaretPos(wxString *ws);

    bool PutTextToClipboard(const wxString &ws);
    bool PutColumnDataToClipboard(const wxString &ws, int linecount);
    bool PutHexDataToClipboard(const char *cs, size_t length);

    // return linecount
    int TranslateText(const wxChar *pwcs, size_t count, vector<ucs4_t> *ucs, bool passNewLine);
    // return linecount
    int GetTextFromClipboard(vector <ucs4_t> *ucs);
    // return linecount
    int GetColumnDataFromClipboard(vector <ucs4_t> *ucs);
    void GetHexDataFromClipboard(vector <char> *cs);

    void HexModeToTextMode(MadEditMode mode);

    void AppearCaret(bool middle = false); // make the caret showing within the client area
    void ShowCaret(bool show);
    void DisplayCaret(bool moveonly); // show caret immediately

    void CopyFileDataToMem(MadBlockIterator begin, MadBlockIterator end);

    // return the line-iterator and and lineid (if it is not NULL) by the pos
    MadLineIterator DeleteInsertData(wxFileOffset pos,
                                     wxFileOffset delsize, /*OUT*/ MadBlockVector *deldata,
                                     wxFileOffset inssize, /*IN*/  MadBlockVector *insdata,
                                     /*OUT*/ int *lineid = NULL);

    void UCStoBlock(const ucs4_t *ucs, size_t count, MadBlock & block);
    void InsertString(const ucs4_t *ucs, size_t count, bool bColumnEditing, bool moveCaret, bool bSelText);

    wxFileOffset GetRowposXPos(int &xpos, MadLineIterator & lit,
                               int subrowid, wxFileOffset inssize);
    void InsertColumnString(const ucs4_t *ucs, size_t count, int linecount, bool bColumnEditing, bool bSelText);

    void GetIndentSpaces(int lineid, MadLineIterator lit, vector <ucs4_t> &spaces, bool autoIndent, bool unindentChar);

    size_t CalcColumns(vector <ucs4_t> &spaces);
    void ConvertColumns(size_t columns, vector <ucs4_t> &spaces);
    void IncreaseIndentSpaces(vector <ucs4_t> &spaces);
    void DecreaseIndentSpaces(vector <ucs4_t> &spaces);

    MadUndo *DeleteSelection(bool bCorrectCaretPos, vector <int> *rpos, bool bColumnEditing);

    void InsertHexChar(int hc);
    void InsertHexData(wxByte *hex, size_t count);

    MadSearchResult Search(/*IN_OUT*/MadCaretPos &beginpos, /*IN_OUT*/MadCaretPos &endpos,
                           const wxString &text, bool bRegex, bool bCaseSensitive, bool bWholeWord);

    MadSearchResult SearchHex(/*IN_OUT*/MadCaretPos &beginpos, /*IN_OUT*/MadCaretPos &endpos,
                              const wxByte *hex, size_t count);

    MadSearchResult Replace(/*OUT*/ucs4string &out, const MadCaretPos &beginpos, const MadCaretPos &endpos,
                            const wxString &expr, const wxString &fmt,
                            bool bRegex, bool bCaseSensitive, bool bWholeWord);

    void OverwriteDataSingle(vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
                             vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
                             vector<wxFileOffset> &ins_len);

    void OverwriteDataMulti(vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
                            vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
                            vector<wxFileOffset> &ins_len);

    // FindLeft/RightBrace()
    // IN: rowid of begin of lit
    // OUT: rowid of bpi, or -1: not found
    void FindLeftBrace(/*IN_OUT*/int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi);
    void FindRightBrace(/*IN_OUT*/int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi);
    void FindBracePairUnderCaretPos();

    // update mouse cursor by the mouse position
    void UpdateCursor(int mouse_x, int mouse_y);

protected:
    void OnChar(wxKeyEvent &evt);
    void OnKeyDown(wxKeyEvent &evt);

    void OnMouseLeftDown(wxMouseEvent &evt);
    void OnMouseLeftUp(wxMouseEvent &evt);
    void OnMouseLeftDClick(wxMouseEvent &evt);
    void OnMouseMotion(wxMouseEvent &evt);
    void OnMouseRightUp(wxMouseEvent &evt);
    void OnMouseMiddleUp(wxMouseEvent &evt);

    void OnSetFocus(wxFocusEvent &evt);
    void OnKillFocus(wxFocusEvent &evt);

    void OnSize(wxSizeEvent &evt);
    void OnVScroll(wxScrollEvent &evt);
    void OnHScroll(wxScrollEvent &evt);
    void OnMouseWheel(wxMouseEvent &evt);
    void OnMouseEnterWindow(wxMouseEvent &evt);
    void OnMouseLeaveWindow(wxMouseEvent &evt);

    void OnEraseBackground(wxEraseEvent &evt);
    void OnPaint(wxPaintEvent &evt);

    void DoSelectionChanged();
    void DoStatusChanged();
    void DoToggleWindow();
    void DoMouseRightUp();
    void DoActivate();

#ifdef __WXMSW__
    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

#ifdef __WXGTK__
    void ConnectToFixedKeyPressHandler();
#endif

public:
    MadEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER|wxWANTS_CHARS);//|wxTAB_TRAVERSAL);
    ~MadEdit();

    void SetStorePropertiesToGlobalConfig(bool value)
    {
        m_StorePropertiesToGlobalConfig=value;
    }

    void StopRepaint()
    {
        m_Printing=true;
        m_LastPaintBitmap=-1;
    }

    void ProcessCommand(MadEditCommand command);

    void SetSyntax(const wxString &title);
    wxString GetSyntaxTitle() { return m_Syntax->m_Title; }
    void ApplySyntaxAttributes(MadSyntax *syn, bool matchTitle);
    void LoadDefaultSyntaxScheme() { MadSyntax::LoadScheme(wxT("Default"), m_Syntax); }

    void SetEncoding(const wxString &encname);
    wxString GetEncodingName() { return m_Encoding->GetName(); }
    wxString GetEncodingDescription() { return m_Encoding->GetDescription(); }
    MadEncodingType GetEncodingType() { return m_Encoding->GetType(); }
    void ConvertEncoding(const wxString &newenc, MadConvertEncodingFlag flag);
    void ConvertChinese(MadConvertEncodingFlag flag);

    bool GetRecordCaretMovements() { return m_RecordCaretMovements; }
    void SetRecordCaretMovements(bool value);

    bool HasBOM()
    {
        return m_Lines->m_LineList.begin()->m_RowIndices[0].m_Start != 0;
    }
    void ToggleBOM();

    void SetTextFont(const wxString &name, int size, bool forceReset);
    void SetHexFont(const wxString &name, int size, bool forceReset);

    void SetFont(const wxString &name, int size)
    {
        if(m_EditMode==emHexMode)
        {
            SetHexFont(name, size, false);
        }
        else
        {
            SetTextFont(name, size, false);
        }
    }

    wxFont GetFont()
    {
        if(m_EditMode==emHexMode)
        {
            return *m_HexFont;
        }
        else
        {
            return *m_TextFont;
        }
    }

    void GetFont(wxString &name, int &size)
    {
        if(m_EditMode==emHexMode)
        {
            name=m_HexFont->GetFaceName();
            size=m_HexFont->GetPointSize();
        }
        else
        {
            name=m_TextFont->GetFaceName();
            size=m_TextFont->GetPointSize();
        }
    }

    virtual bool SetFont(const wxFont& font)
    {
        SetFont(font.GetFaceName(), font.GetPointSize());
        return true;
    }

    void SetFixedWidthMode(bool mode);
    bool GetFixedWidthMode() {return m_FixedWidthMode; }

    void SetLineSpacing(int percent);
    long GetLineSpacing() { return m_LineSpacing; }

    void SetEditMode(MadEditMode mode);
    MadEditMode GetEditMode() {return m_EditMode; }

    void SetSingleLineMode(bool mode);

    void SetTabColumns(long value);
    long GetTabColumns() { return m_TabColumns; }

    void SetIndentColumns(long value)
    {
        if(value>=1 && value<=80)
            m_IndentColumns=value;
    }
    long GetIndentColumns() { return m_IndentColumns; }

    void SetInsertSpacesInsteadOfTab(bool value);
    bool GetInsertSpacesInsteadOfTab() { return m_InsertSpacesInsteadOfTab; }
    void SetWantTab(bool value) { m_WantTab=value; }
    bool GetWantTab() { return m_WantTab; }

    void SetWordWrapMode(MadWordWrapMode mode);
    MadWordWrapMode GetWordWrapMode() { return m_WordWrapMode; }

    void SetDisplayLineNumber(bool value);
    void SetShowEndOfLine(bool value);
    void SetShowTabChar(bool value);
    void SetShowSpaceChar(bool value);
    void SetMarkActiveLine(bool value);

    bool GetDisplayLineNumber() { return m_DisplayLineNumber; }
    bool GetShowEndOfLine() { return m_ShowEndOfLine; }
    bool GetShowTabChar() { return m_ShowTabChar; }
    bool GetShowSpaceChar() { return m_ShowSpaceChar; }
    bool GetMarkActiveLine() { return m_MarkActiveLine; }

    void SetMarkBracePair(bool value);
    bool GetMarkBracePair() { return m_MarkBracePair; }

    long GetMaxColumns() { return m_MaxColumns; }
    void SetMaxColumns(long cols);

    bool GetAutoIndent() { return m_AutoIndent; }
    void SetAutoIndent(bool value) { m_AutoIndent=value; }

    void SetAutoCompletePair(bool value) { m_AutoCompletePair=value; }
    bool GetAutoCompletePair() { return m_AutoCompletePair; }

    void SetInsertMode(bool mode);  // true: insert, false: overwrite
    bool GetInsertMode()
    {
        if(m_EditMode==emHexMode)
            return false;
        return m_InsertMode;
    }

    void SetCaretType(MadCaretType type);

    bool GetMouseSelectToCopy() { return m_MouseSelectToCopy; }
    void SetMouseSelectToCopy(bool value) { m_MouseSelectToCopy=value; }
    bool GetMouseSelectToCopyWithCtrlKey() { return m_MouseSelectToCopyWithCtrlKey; }
    void SetMouseSelectToCopyWithCtrlKey(bool value) { m_MouseSelectToCopyWithCtrlKey=value; }

    bool GetMiddleMouseToPaste() { return m_MiddleMouseToPaste; }
    void SetMiddleMouseToPaste(bool value) { m_MiddleMouseToPaste=value; }

    int CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount);
    int GetLineNumberAreaWidth(int number);
    int GetMaxWordWrapWidth();

    int GetUCharWidth(ucs4_t uc);
    int GetHexUCharWidth(ucs4_t uc);
    int GetUCharType(ucs4_t uc);

    // all are zero-based
    void GetCaretPosition(int &line, int &subrow, wxFileOffset &column);

    wxString GetFileName() { return m_Lines->m_Name; }
    wxFileOffset GetCharPosition() { return m_CaretPos.pos; }
    wxFileOffset GetFileSize() { return m_Lines->m_Size; }

    bool IsSelected() { return m_Selection; }
    wxFileOffset GetSelectionSize();

    // return -1 for no selection
    void GetSelectionLineId(int &beginline, int &endline);

    int GetLineCount() { return int(m_Lines->m_LineCount); }

    void ConvertNewLineType(MadNewLineType type);
    void SetInsertNewLineType(MadNewLineType type)
    {
        m_InsertNewLineType=type;
    }

    MadNewLineType GetNewLineType()
    {
        if(m_NewLineType==nltDefault)
        {
#ifdef __WXMSW__
            return nltDOS;
#else
            return nltUNIX;
#endif
        }
        return m_NewLineType;
    }

    MadNewLineType GetInsertNewLineType()
    {
        if(m_InsertNewLineType==nltDefault)
        {
#ifdef __WXMSW__
            return nltDOS;
#else
            return nltUNIX;
#endif
        }
        return m_InsertNewLineType;
    }

    bool IsModified() { return m_Modified; }
    time_t GetModificationTime() { return m_ModificationTime; }

    void SetReadOnly(bool value) { m_ReadOnly=value; }
    bool IsReadOnly() { return m_ReadOnly||m_Lines->m_ReadOnly; }
    bool IsTextFile() { return m_Lines->m_MaxLineWidth>=0; }

    void GetSelText(wxString &ws);
    void GetText(wxString &ws, bool ignoreBOM = true);
    void SetText(const wxString &ws);

    // line: zero based
    // return true for full line, false for partial line
    bool GetLine(wxString &ws, int line, size_t maxlen = 0, bool ignoreBOM = true);
    int GetLineByPos(const wxFileOffset &pos);

    void GetSelHexString(wxString &ws, bool withSpace);

    void GetWordFromCaretPos(wxString &ws)
    { SelectWordFromCaretPos(&ws); }

    void Delete() { ProcessCommand(ecDelete); }
    void CutLine() { ProcessCommand(ecCutLine); }
    void DeleteLine() { ProcessCommand(ecDeleteLine); }
    void InsertTabChar() { ProcessCommand(ecInsertTabChar); }
    void InsertDateTime() { ProcessCommand(ecInsertDateTime); }

    void IncreaseDecreaseIndent(bool incIndent);
    bool HasLineComment() { return !m_Syntax->m_LineComment.empty(); }
    void CommentUncomment(bool comment);

    void ToUpperCase();
    void ToLowerCase();
    void InvertCase();
    void ToHalfWidth();
    void ToFullWidth();

    void TrimTrailingSpaces();

    // startline<0 : sort all lines; otherwise sort [beginline, endline]
    void SortLines(MadSortFlags flags, int beginline, int endline);

    void SelectAll();
    void CutToClipboard();
    void CopyToClipboard();
    void PasteFromClipboard();
    bool CanPaste();
    void CopyAsHexString(bool withSpace);

    bool CanUndo()
    {
        return m_UndoBuffer->CanUndo(!m_RecordCaretMovements);
    }
    bool CanRedo()
    {
        return m_UndoBuffer->CanRedo(!m_RecordCaretMovements);
    }
    void Undo();
    void Redo();

    void GoToLine(int line);
    void SetCaretPosition(wxFileOffset pos, wxFileOffset selbeg=-1, wxFileOffset selend=-1);

    bool HasBracePair() { return m_Syntax->m_LeftBrace.size()!=0; }
    void GoToLeftBrace() { ProcessCommand(ecLeftBrace); }
    void GoToRightBrace() { ProcessCommand(ecRightBrace); }

    // search from caretpos or SelectionEnd if IsSelected().
    MadSearchResult FindTextNext(const wxString &text, bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bFromDocBegin);
    // search from caretpos or SelectionBegin if IsSelected().
    MadSearchResult FindTextPrevious(const wxString &text, bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bFromDocEnd);

    // search from caretpos or SelectionEnd if IsSelected().
    MadSearchResult FindHexNext(const wxString &hexstr, bool bFromDocBegin);
    // search from caretpos or SelectionBegin if IsSelected().
    MadSearchResult FindHexPrevious(const wxString &hexstr, bool bFromDocEnd);

    // replace the selected text that must match expr
    bool ReplaceText(const wxString &expr, const wxString &fmt,
                     bool bRegex, bool bCaseSensitive, bool bWholeWord);
    bool ReplaceHex(const wxString &expr, const wxString &fmt);

    // return the replaced count or SR_EXPR_ERROR
    int ReplaceTextAll(const wxString &expr, const wxString &fmt,
                       bool bRegex, bool bCaseSensitive, bool bWholeWord,
                       vector<wxFileOffset> *pbegpos=NULL,
                       vector<wxFileOffset> *pendpos=NULL);
    int ReplaceHexAll(const wxString &expr, const wxString &fmt,
                      vector<wxFileOffset> *pbegpos=NULL,
                      vector<wxFileOffset> *pendpos=NULL);

    // list the matched data to pbegpos & pendpos
    // return the found count or SR_EXPR_ERROR
    int FindTextAll(const wxString &expr,
                    bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bFirstOnly,
                    vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos);
    int FindHexAll(const wxString &expr, bool bFirstOnly,
                   vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos);

    bool LoadFromFile(const wxString &filename, const wxString &encoding = wxEmptyString);
    bool SaveToFile(const wxString &filename);
    bool Reload();
    // if the file is modified by another app, reload it.
    bool ReloadByModificationTime();
    // restore pos in Reload(), ConvertEncoding()
    void RestorePosition(wxFileOffset pos, int toprow);

    // write back to the original FileName;
    // if FileName is empty, ask the user to get filename
    // return wxID_YES(Saved), wxID_NO(Not Saved), or wxID_CANCEL
    int Save(bool ask, const wxString &title, bool saveas);

    void WordCount(bool selection, int &wordCount, int &charCount, int &spaceCount,
                   int &halfWidthCount, int &fullWidthCount, int &lineCount,
                   wxArrayString *detail);

    void SetOnSelectionChanged(OnSelectionChangedPtr func)
    {
        m_OnSelectionChanged=func;
    }
    void SetOnStatusChanged(OnStatusChangedPtr func)
    {
        m_OnStatusChanged=func;
    }
    void SetOnToggleWindow(OnToggleWindowPtr func)
    {
        m_OnToggleWindow=func;
    }
    void SetOnMouseRightUp(OnMouseRightUpPtr func)
    {
        m_OnMouseRightUp=func;
    }
    void SetOnActivate(OnActivatePtr func)
    {
        m_OnActivate=func;
    }

    void SetSearchOptions(bool bUseDefaultSyntax, bool bSearchWholeWord)
    {
        m_UseDefaultSyntax = bUseDefaultSyntax;
        m_SearchWholeWord = bSearchWholeWord;
    }

private: // Printing functions
    int m_Printing;     // 0: no, <0: Text, >0: Hex
    bool TextPrinting() { return m_Printing<0; }
    bool HexPrinting()  { return m_Printing>0; }
    bool InPrinting()   { return m_Printing!=0; }

    int             m_old_ClientWidth;
    int             m_old_ClientHeight;
    MadWordWrapMode m_old_WordWrapMode;
    bool            m_old_Selection;
    bool            m_old_DisplayLineNumber;
    bool            m_old_ShowEndOfLine, m_old_ShowSpaceChar, m_old_ShowTabChar;
    int             m_old_LeftMarginWidth;
    int             m_old_DrawingXPos;

    wxRect m_PrintRect;
    int m_PrintPageCount;

    int m_RowCountPerPage;
    bool m_PrintSyntax;

    bool m_old_CaretAtHexArea;

    int m_PrintOffsetHeader;
    int m_RowCountPerHexLine;
    int m_HexLineCountPerPage;
    int m_PrintTotalHexLineCount;
    MadEdit *m_PrintHexEdit;    // use a temporary MadEdit to print Hex-Data

public: // printing functions
    void BeginPrint(const wxRect &printRect);
    int  GetPageCount() { return m_PrintPageCount; }
    bool PrintPage(wxDC *dc, int pageNum);
    void EndPrint();

public: // fix wxDC.Blit(wxINVERT) not work on some old versions of VMWare
    typedef void (MadEdit::*InvertRectPtr)(wxDC *dc, int x, int y, int w, int h);
    InvertRectPtr InvertRect;
    void InvertRectNormal(wxDC *dc, int x, int y, int w, int h);
    void InvertRectManual(wxDC *dc, int x, int y, int w, int h);

};

wxString FixUTF8ToWCS(const wxString &str);

#endif
