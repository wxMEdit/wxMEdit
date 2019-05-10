///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxmedit/wxmedit.h
// Description: Main Edit Component of wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_H_
#define _WXMEDIT_H_

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
#endif
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include "wxm_lines.h"
#include "wxmedit_command.h"
#include "../xm/encoding/encoding.h"
#include "wxm_syntax.h"
#include "wxm_undo.h"

#include <wx/confbase.h>

#include <unicode/brkiter.h>
using U_ICU_NAMESPACE::BreakIterator;

#include <boost/shared_ptr.hpp>
#include <string>
#include <algorithm>

enum { ID_VSCROLLBAR=19876, ID_HSCROLLBAR };

class MadEdit;

namespace wxm
{
    struct WordCountData
    {
        size_t bytes;
		size_t words;
		size_t chars;
		size_t spaces;
		size_t controls;
		size_t fullwidths;
		size_t ambws;
		size_t lines;
        wxArrayString detail;

        WordCountData()
            : bytes(0), words(0), chars(0), spaces(0)
            , controls(0), fullwidths(0), ambws(0), lines(0)
        {}
    };
} // namespace wxm

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

class MadMouseMotionTimer;

struct WXMCharIterator;

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

    void AssignWith(const WXMCharIterator& cit);
};

struct WXMCharIterator
{
    wxFileOffset    pos;
    MadLineIterator lit;
    wxFileOffset    linepos;

    WXMCharIterator() {}
    WXMCharIterator(const MadCaretPos& cp)
        :pos(cp.pos), lit(cp.iter), linepos(cp.linepos)
    {}
};

struct UCIterator;


#define MadEditSuperClass wxWindow //wxScrolledWindow//wxPanel//wxControl//

namespace wxm
{
    struct MouseCapturer;
    struct TextSearcher;
    struct HexSearcher;

    struct ConfigWriter
    {
        void Record(const wxString& key, bool val) { return Record(key, (long)val); }
        void Record(const wxString& key, int val) { return Record(key, (long)val); }

        virtual void Record(const wxString& key, long val) = 0;
        virtual void Record(const wxString& key, double val) = 0;
        virtual void Record(const wxString& key, const wxString& val) = 0;

        virtual void SetConfig(wxConfigBase* cfg) = 0;
        virtual ~ConfigWriter() {}
    };
}

class MadEdit: public MadEditSuperClass
{
    DECLARE_EVENT_TABLE()
public:
    static MadKeyBindings ms_KeyBindings;

private:
    friend class MadSyntax;
    friend struct xm::Encoding;
    friend class MadLines;
    friend class MadMouseMotionTimer;
    friend struct wxm::WXMSearcher;
    friend struct wxm::TextSearcher;
    friend struct wxm::HexSearcher;
    friend struct wxm::InFrameWXMEdit;

    static int      ms_Count; // the count of MadEdit

    wxScrollBar     *m_VScrollBar, *m_HScrollBar;
    int             m_VSBWidth, m_VSBHeight, m_HSBWidth, m_HSBHeight;
protected:
    int             m_ClientWidth, m_ClientHeight; // client size exclude scrollbar

    wxConfigBase    *m_Config;

    MadLines        *m_Lines;
private:
    MadSyntax       *m_Syntax;
    xm::Encoding *m_Encoding;

    MadUndoBuffer   *m_UndoBuffer;
    MadUndo         *m_SavePoint;
    bool            m_RecordCaretMovements;

    ucs4_t          *m_WordBuffer;
    int             *m_WidthBuffer;
    wchar_t         *m_WCWordBuffer;
    int             *m_WCWidthBuffer;

    wxBitmap        *m_ClientBitmap, *m_MarkBitmap;
    int             m_LastPaintBitmap;// 0:client, 1:mark

    std::vector<wxPoint> m_space_points;
public:
    std::vector<wxPoint> m_cr_points, m_lf_points, m_crlf_points, m_eof_points;

protected:
    MadCaretPos     m_CaretPos;
private:
    xm::UCQueue      m_ActiveRowUChars;  // ucs4 char cache of active row
    vector<int>     m_ActiveRowWidths;  // width cache of active row
    int             m_CaretRowUCharPos; // ucs4 char pos of active row
    int             m_LastCaretXPos;    // when move caret up/down, use this to calc the nearest xpos

    // generally m_ValidPos==TopRow, but sometimes not
    MadLineIterator m_ValidPos_iter;    // line iterator
    int             m_ValidPos_lineid;  // line-id. of iterator
    int             m_ValidPos_rowid;   // row-id. of iter
    wxFileOffset    m_ValidPos_pos;     // position of line in whole file
    int             m_UpdateValidPos;   // ==0: no update; <0 update always; >0 update if newpos<oldpos

protected:
    bool            m_Selection;

private:
    MadCaretPos     m_SelectionPos1, m_SelectionPos2;
    MadCaretPos     *m_SelectionBegin, *m_SelectionEnd;

    int             m_SelFirstRow, m_SelLastRow;        // for repaint
    int             m_SelLeftXPos, m_SelRightXPos;      // for ColumnMode

public:
    int GetSpaceCharFontWidth()
    {
        return m_TextFontSpaceCharWidth;
    }

    int GetTabMaxCharFontWidth()
    {
        return m_TabColumns * GetSpaceCharFontWidth();
    }

    int CalcTabWidth(int rowwidth, int xpos)
    {
        const int tabwidth = GetTabMaxCharFontWidth();
        const int normalwidth = tabwidth - (xpos % tabwidth);
        const int fitwidth = std::max(rowwidth - xpos, GetSpaceCharFontWidth());
        return std::min(normalwidth, fitwidth);
    }

    int GetUCharTextFontWidth(ucs4_t uc, int rowwidth, int nowxpos)
    {
        return (uc == 0x09)? CalcTabWidth(rowwidth, nowxpos) : GetUCharWidth(uc);
    }

private:
    wxFont          *m_TextFont; // readonly, set it at SetTextFont()
    int             m_TextFontHeight;
    int             m_TextFontAveCharWidth;
    int             m_TextFontSpaceCharWidth;
protected:
    int             m_TextFontMaxDigitWidth;

    wxFont          *m_HexFont; // readonly, set it at SetHexFont()
private:
    int             m_HexFontHeight;
    int             m_HexFontMaxDigitWidth;

    wxUint16        *m_TextFontWidths[17], *m_HexFontWidths[17];

protected:
    wxm::ConfigWriter* m_cfg_writer;
private:

    bool            m_FixedWidthMode;

protected:
    bool            m_RepaintAll;
private:
    bool            m_RepaintSelection;


    MadEditMode     m_EditMode;
    int             m_MaxColumnRowWidth;

protected:
    bool            m_DoRecountLineWidth;
private:
    int             m_OldWidth, m_OldHeight;

    bool            m_Modified;
    time_t          m_ModificationTime;
    bool            m_ReadOnly;

    int             m_TopRow, m_TextTopRow, m_HexTopRow;
protected:
    int             m_DrawingXPos;
private:

    int             m_CompleteRowCount;
    int             m_VisibleRowCount;
    int             m_PageRowCount;

protected:
    int             m_RowHeight;
private:
    long            m_LineSpacing;
    long            m_MaxColumns;

protected:
    int             m_LeftMarginWidth;
    int             m_RightMarginWidth;

private:
    // for OnSize()
    int             m_MaxWidth, m_MaxHeight;

    const wxm::NewLineChar* m_newline;
    const wxm::NewLineChar* m_newline_for_insert;

    long            m_MaxLineLength;
    bool            m_HasTab;
    long            m_TabColumns;
    long            m_IndentColumns;
    bool            m_InsertSpacesInsteadOfTab;
    bool            m_WantTab;

    bool            m_AutoIndent;

protected:
    bool            m_ShowEndOfLine, m_ShowSpaceChar, m_ShowTabChar;
private:
    bool            m_MarkActiveLine;
    bool            m_MarkBracePair;

    bool            m_InsertMode;
    MadCaretType    m_CaretType;

    bool            m_MouseLeftDown, m_MouseLeftDoubleClick;
    int             m_DoubleClickX, m_DoubleClickY;
    bool            m_MouseAtHexTextArea;

    MadMouseMotionTimer *m_MouseMotionTimer;

    bool            m_MouseSelectToCopy;
    bool            m_MouseSelectToCopyWithCtrlKey; // enable or disable when pressing Ctrl key
    bool            m_MiddleMouseToPaste;

    wxBitmap        *m_HexDigitBitmap;
    wxDC            *m_HexDigitDC;    // temp dc, must set it on OnPaint()
protected:
    bool            m_CaretAtHexArea;
private:
    bool            m_CaretAtHalfByte;
    int             m_HexRowCount;
protected:
    vector<wxFileOffset> m_HexRowIndex;     // HexMode row index
private:
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

    wxMilliClock_t m_lastDoubleClick;

    wxm::MouseCapturer* m_mouse_capturer;

    bool m_mouse_in_window;

    UErrorCode m_word_bi_status;
    boost::shared_ptr<BreakIterator> m_word_bi;

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
                     xm::UCQueue &ucharQueue, vector<int> &widthArray,
                     int &ucharPos);

    // IN: caretPos.pos
    // UPDATE: caretPos.all and ucharQueue widthArray ucharPos
    void UpdateCaretByPos(MadCaretPos &caretPos,
                          xm::UCQueue &ucharQueue, vector<int> &widthArray,
                          int &ucharPos);

    // caretPos.pos linepos must be begin of row
    // IN: caretPos.iter subrowid ; not used: lineid rowid
    // UPDATE:caretPos.xpos pos linepos extraspaces and ucharQueue widthArray ucharPos
    void UpdateCaretByXPos(int xPos, MadCaretPos &caretPos,
                           xm::UCQueue &ucharQueue, vector<int> &widthArray,
                           int &ucharPos);

    // update the two SelectionPos by their .pos
    void UpdateSelectionPos();

    void PaintText(wxDC *dc, int x, int y, const ucs4_t *text, const int *width, int count, int minleft, int maxright);
    void PaintTextLines(wxDC *dc, const wxRect &rect, int toprow, int rowcount, const wxColor &bgcolor);

    virtual int CachedLineNumberAreaWidth() = 0;
    virtual void CacheLineNumberAreaWidth(int width) = 0;
    virtual void PaintLineNumberArea(const wxColor & bgcolor, wxDC * dc, const wxRect& rect, bool is_trailing_subrow, MadLineIterator lineiter, int lineid, int text_top) = 0;

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
    void ResetTextModeScrollBarPos();
    virtual void UpdateScrollBarPos();

    void BeginUpdateSelection();
    void EndUpdateSelection(bool bSelection);

    void SetSelection(wxFileOffset beginpos, wxFileOffset endpos, bool bCaretAtBeginPos = false);

    wxFileOffset GetColumnSelection(wxString *ws);

    // if(ws==nullptr) SelectWord only;
    // else GetWord to ws;
    void SelectWordFromCaretPos(wxString *ws);
    void SelectLineFromCaretPos(wxString *ws=nullptr);

    // return linecount
    int TranslateText(const wxChar* pwcs, size_t count, vector<ucs4_t>& ucs, bool passNewLine);
    // return linecount
    int GetTextFromClipboard(vector<ucs4_t>& ucs);
    // return linecount
    int GetColumnDataFromClipboard(vector<ucs4_t>& ucs);

    static bool PutColumnDataToClipboard(const wxString &ws, int linecount);
    static bool PutRawBytesToClipboard(const std::string& cs);

public:
    static bool PutTextToClipboard(const wxString &ws);

    bool GetRawBytesFromClipboardDirectly(vector<char>& cs);
    void ConvertToRawBytesFromUnicodeText(vector<char>& cs, const vector<ucs4_t>& ucs);

protected:
    void GetRawBytesFromClipboard(vector<char>& cs);

    void HexModeToTextMode(MadEditMode mode);
    virtual bool ManuallyCancelHexToText() = 0;

    void AppearCaret(bool middle = false); // make the caret showing within the client area
    void ShowCaret(bool show);
    void DisplayCaret(bool moveonly); // show caret immediately

    void CopyFileDataToMem(xm::BlockIterator begin, xm::BlockIterator end);

    // return the line-iterator and and lineid (if it is not nullptr) by the pos
    MadLineIterator DeleteInsertData(wxFileOffset pos,
                                     wxFileOffset delsize, /*OUT*/ xm::BlockVector *deldata,
                                     wxFileOffset inssize, /*IN*/  xm::BlockVector *insdata,
                                     /*OUT*/ int *lineid = nullptr);

    void UCStoBlock(const ucs4_t *ucs, size_t count, MadBlock & block);
    void NewLineToBlock(const wxm::NewLineChar& nl, MadBlock & block)
    {
        UCStoBlock(nl.Value().data(), nl.Value().size(), block);
    }
    void NewLineToBlock(MadBlock & block)
    {
        NewLineToBlock(*m_newline_for_insert, block);
    }

    long MaxLineLength() { return m_MaxLineLength; }
    virtual bool AdjustInsertCount(const ucs4_t *ucs, size_t& count) { return true; }
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
    void InsertRawBytes(wxByte *bytes, size_t count, bool overwirte);

    void OverwriteDataSingle(vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
                             vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
                             vector<wxFileOffset> &ins_len);

    void OverwriteDataMultiple(vector<wxFileOffset> &del_bpos, vector<wxFileOffset> &del_epos,
                               vector<const ucs4_t*> *ins_ucs, vector<wxByte*> *ins_data,
                               vector<wxFileOffset> &ins_len);

    // FindLeft/RightBrace()
    // IN: rowid of begin of lit
    // OUT: rowid of bpi, or -1: not found
    void FindLeftBrace(/*IN_OUT*/int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi);
    void FindRightBrace(/*IN_OUT*/int &rowid, MadLineIterator lit, wxFileOffset linepos, BracePairIndex &bpi);
    void FindBracePairUnderCaretPos();

    void CaretPosUpdate();
    void ForwardNthUChars(MadLineIterator& lit, int32_t n, wxFileOffset lineendpos);
    void DoPrevWord();
    void DoNextWord();

    // update mouse cursor by the mouse position
    void UpdateCursor(int mouse_x, int mouse_y);

    virtual int CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount) = 0;
    virtual int GetLineNumberAreaWidth(int number) = 0;

    void InsertColumnText(vector<ucs4_t>& ucs, int lines);
    void InsertRegularText(vector<ucs4_t>& ucs);
    void PasteColumnText();
    void PasteRegularText();
    void PasteRawBytes(bool overwirte);
    void CopyColumnText();
public:
    void CopyRegularText();
    void CopyRawBytes();

protected:
    void OnChar(wxKeyEvent &evt);
    void OnKeyDown(wxKeyEvent &evt);

    void LogicMouseLeftUp();
    virtual void OnGetFocusByClickAsControl(const wxMouseEvent &evt) {}
    virtual bool NeedNotProcessMouseLeftDown(wxMouseEvent &evt) { return false; }

    void OnMouseLeftDown(wxMouseEvent &evt);
    void OnMouseLeftUp(wxMouseEvent &evt);
    void OnMouseLeftDClick(wxMouseEvent &evt);
    void OnMouseMotion(wxMouseEvent &evt);
    void OnMouseRightUp(wxMouseEvent &evt);
    void OnMouseMiddleUp(wxMouseEvent &evt);

    void OnMouseCaptureLost(wxMouseCaptureLostEvent &evt);

    virtual void OnWXMEditSetFocus() {}
    virtual void OnWXMEditKillFocus();
    void Deselect() { m_Selection = false; }
    void SetRepaintAll(bool val) { m_RepaintAll = val; }

    void OnSetFocus(wxFocusEvent &evt);
    void OnKillFocus(wxFocusEvent &evt);

    void OnSize(wxSizeEvent &evt);
    void OnVScroll(wxScrollEvent &evt);
    void OnHScroll(wxScrollEvent &evt);
    virtual void OnMouseWheel(wxMouseEvent &evt);
    void OnMouseEnterWindow(wxMouseEvent &evt);
    void OnMouseLeaveWindow(wxMouseEvent &evt);

    void OnEraseBackground(wxEraseEvent &evt);
    void OnPaint(wxPaintEvent &evt);

    virtual void OnPaintInPrinting(wxPaintDC& dc, wxMemoryDC& memdc) = 0;

public:
    void OnSelectionAndStatusChanged()
    {
        DoSelectionChanged();
        DoStatusChanged();
    }

protected:
    virtual void DoSelectionChanged() {}
    virtual void DoStatusChanged() {}
    virtual void DoToggleWindow() {}
    virtual void DoMouseRightUp() {}

#ifdef __WXMSW__
    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

#if defined(__WXGTK__) && wxMAJOR_VERSION == 2
    void ConnectToFixedKeyPressHandler();
#endif

public:
    MadEdit(wxm::ConfigWriter* cfg_writer, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER|wxWANTS_CHARS);
    virtual ~MadEdit();

    void StopRepaint()
    {
        m_Printing=true;
        m_LastPaintBitmap=-1;
    }

    virtual void ProcessReturnCommand(MadEditCommand command);
    void ProcessCommand(MadEditCommand command);

protected:
    void BeginSyntaxPrint(bool printSyntax)
    {
        m_Syntax->BeginPrint(printSyntax);
    }
    void EndSyntaxPrint()
    {
        m_Syntax->EndPrint();
    }
    void InitTextFont();
    void InitHexFont();

    void CalcEOLMarkPoints(std::vector<wxPoint>& dest, const std::vector<wxPoint>& src, const wxSize& charsz);
    void CalcSpaceMarkPoints(const wxSize& charsz);
    void CalcEOFMarkPoints(const wxSize& charsz);
    void CalcTabMarkPoints(std::vector<wxPoint>& dest, const wxSize& charsz);

public: // basic functions
    void SetSyntax(const wxString &title, bool manual=false);
    MadSyntax* GetSyntax() { return m_Syntax; }
    wxString GetSyntaxTitle() { return m_Syntax->GetTitle(); }
    void ApplySyntaxAttributes(MadSyntax *syn, bool matchTitle);
    void LoadDefaultSyntaxScheme() { MadSyntax::LoadScheme(wxT("Default"), m_Syntax); }

    void SetEncoding(const std::wstring & encname);
    wxString GetEncodingName() { return m_Encoding->GetName().c_str(); }
    wxString GetEncodingDescription() { return m_Encoding->GetDescription().c_str(); }
    bool IsUnicodeFile() { return m_Encoding->IsUnicodeEncoding(); }

    bool GetRecordCaretMovements() { return m_RecordCaretMovements; }
    void SetRecordCaretMovements(bool value);

    void SetTextFont(const wxString &name, int size, bool forceReset);
    void SetHexFont(const wxString &name, int size, bool forceReset);

    void GetTextFont(wxString &name, int &size)
    {
        name=m_TextFont->GetFaceName();
        size=m_TextFont->GetPointSize();
    }
    void GetHexFont(wxString &name, int &size)
    {
        name=m_HexFont->GetFaceName();
        size=m_HexFont->GetPointSize();
    }

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
            GetHexFont(name, size);
        }
        else
        {
            GetTextFont(name, size);
        }
    }

    virtual bool SetFont(const wxFont& font) override
    {
        SetFont(font.GetFaceName(), font.GetPointSize());
        return true;
    }

    void SetFixedWidthMode(bool mode);
    bool GetFixedWidthMode() {return m_FixedWidthMode; }

    void SetLineSpacing(int percent);
    long GetLineSpacing() { return m_LineSpacing; }

    virtual void SetEditMode(MadEditMode mode);
    MadEditMode GetEditMode() {return m_EditMode; }

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

    virtual void SetWordWrapMode(MadWordWrapMode mode) = 0;
	virtual MadWordWrapMode GetWordWrapMode() = 0;

    void SetShowEndOfLine(bool value);
    void SetShowTabChar(bool value);
    void SetShowSpaceChar(bool value);
    void SetMarkActiveLine(bool value);

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
    bool IsInsertMode()
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

    int GetMaxWordWrapWidth();
    int GetUCharWidth(ucs4_t uc);
    int GetHexUCharWidth(ucs4_t uc);

    // all are zero-based
    void GetCaretPosition(int &line, int &subrow, wxFileOffset &column);
    wxFileOffset GetCaretPosition() { return m_CaretPos.pos; }

    wxString GetFileName() { return m_Lines->m_Name; }
    wxFileOffset GetFileSize() { return m_Lines->m_Size; }

    bool IsSelected() { return m_Selection; }
    wxFileOffset GetSelectionSize();
    wxFileOffset GetSelectionBeginPos() { return m_Selection ? m_SelectionBegin->pos : -1; }
    wxFileOffset GetSelectionEndPos() { return m_Selection ? m_SelectionEnd->pos : -1; }

    // return -1 for no selection
    void GetSelectionLineId(int &beginline, int &endline);

    int GetLineCount() { return int(m_Lines->m_LineCount); }

    void AppendNewLine(vector<ucs4_t>& newtext, const MadLineIterator& lit, size_t firstrow, size_t lastrow, size_t subrowid);
    void ConvertNewLine(const wxm::NewLineChar& nl);
    void SetInsertNewLine(const wxm::NewLineChar& nl)
    {
        m_newline_for_insert = &nl;
    }

    const wxm::NewLineChar& GetNewLine()
    {
        return *m_newline;
    }

    const wxm::NewLineChar& GetNewLine4Insert()
    {
        return *m_newline_for_insert;
    }

    bool IsModified() { return m_Modified; }
    time_t GetModificationTime() { return m_ModificationTime; }

    void SetReadOnly(bool value) { m_ReadOnly=value; }
    bool IsReadOnly() { return m_ReadOnly||m_Lines->m_ReadOnly; }
    bool IsTextFile() { return m_Lines->m_MaxLineWidth>=0; }

    virtual bool AdjustStringLength(const wxString& ws, size_t& size) { return true; }
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

    void SelectAll();
    void CutToClipboard();
    void CopyToClipboard();
    void PasteFromClipboard(bool overwirte);
    bool CanPaste();

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

    virtual wxm::WXMSearcher* Searcher(bool inhex, bool use_regex) = 0;

    bool LoadFromFile(const wxString& filename, const std::wstring& encoding=wxEmptyString, bool hexmode=false);
    bool SaveToFile(const wxString& filename);

    struct WXMLocations
    {
        wxFileOffset pos;
        int toprow;
        LineNumberList bmklinenums;
    };

    // save & restore caret position and bookmarks in Reload(), ConvertEncoding()
    WXMLocations SaveLocations();
    void RestoreLocations(const WXMLocations& loc);

    // write back to the original FileName;
    // if FileName is empty, ask the user to get filename
    // return wxID_YES(Saved), wxID_NO(Not Saved), or wxID_CANCEL
    int Save(bool ask, const wxString &title, bool saveas);

public: // advanced functions
    void ConvertEncoding(const std::wstring & newenc, MadConvertEncodingFlag flag);
    void ConvertChinese(MadConvertEncodingFlag flag);

    bool HasBOM()
    {
        return m_Lines->m_LineList.begin()->m_RowIndices[0].m_Start != 0;
    }
    void ToggleBOM();

    bool EditingInHexAera()
    {
        return m_EditMode == emHexMode && m_CaretAtHexArea;
    }

    void IncreaseDecreaseIndent(bool incIndent);
    bool HasLineComment() { return !m_Syntax->m_LineComment.empty(); }
    void CommentUncomment(bool comment);

    void ToUpperCase();
    void ToLowerCase();
    void InvertCase();
    void ToHalfWidth(bool ascii=true, bool japanese=true, bool korean=true, bool other=true);
    void ToFullWidth(bool ascii=true, bool japanese=true, bool korean=true, bool other=true);

    // startline<0 : sort all lines; otherwise sort [beginline, endline]
    void SortLines(MadSortFlags flags, int beginline, int endline);

    // convert WordWraps to NewLine-chars in the SelText or whole file
    void ConvertWordWrapToNewLine();
    // convert NewLine-chars to WordWraps in the SelText
    void ConvertNewLineToWordWrap();

    void ConvertSpaceToTab();
    void ConvertTabToSpace();

    void CopyAsHexString(bool withSpace);


    void WordCount(bool selection, wxm::WordCountData& data);

    virtual LineNumberList SaveBookmarkLineNumberList() = 0;
    virtual void RestoreBookmarkByLineNumberList(const LineNumberList& linenums) = 0;

public:
    void SetSearchOptions(bool bUseDefaultSyntax, bool bSearchWholeWord)
    {
        m_UseDefaultSyntax = bUseDefaultSyntax;
        m_SearchWholeWord = bSearchWholeWord;
    }

protected:
    void SetClientAreaSize(int w, int h) { m_ClientWidth = w; m_ClientHeight = h; }
    void HideScrollBars();

protected: // Printing functions
    void SetHexPrinting() { m_Printing = 1; }
    void SetTextPrinting() { m_Printing = -1; }
    void SetNoPrinting() { m_Printing = 0; }
    int m_Printing;     // 0: no, <0: Text, >0: Hex
    bool TextPrinting() { return m_Printing<0; }
    bool HexPrinting()  { return m_Printing>0; }
    bool InPrinting()   { return m_Printing!=0; }

private:

    virtual void SetClientSizeData(int w, int h);
    void UpdateClientBitmap();

    static wxMilliClock_t GetTripleClickInterval();

public: // fix wxDC.Blit(wxINVERT) not work on some old versions of VMWare
    typedef void (MadEdit::*InvertRectPtr)(wxDC *dc, int x, int y, int w, int h);
    InvertRectPtr InvertRect;
    void InvertRectNormal(wxDC *dc, int x, int y, int w, int h);
    void InvertRectManual(wxDC *dc, int x, int y, int w, int h);
};

wxString FixUTF8ToWCS(const wxString &str);
bool StrToInt64(wxString str, wxInt64 &i64);

#endif
