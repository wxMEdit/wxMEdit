///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadSyntax.h
// Description: syntax parsing and syntax file management
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADSYNTAX_H_
#define _MADSYNTAX_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <wx/hashset.h>
#include <wx/colour.h>
#include <wx/string.h>

#include "ucs4_t.h"
#include "MadLines.h"


WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, MadKeywordSet );


enum MadFontStyle { fsNone=0, fsBold=1, fsItalic=2, fsUnderline=4, fsStrikeOut=8 };
typedef int MadFontStyles;

struct MadAttributes    // font attributes
{
    wxColour color;        // text color
    wxColour bgcolor;      // background color
    MadFontStyles style;   // style
    MadAttributes():color(wxNullColour),bgcolor(wxNullColour),style(fsNone)
    {}
};

struct MadSyntaxRange
{
    unsigned long id;
    wxString begin;
    wxString end;
    wxColour bgcolor;
    MadSyntaxRange():id(0),bgcolor(wxNullColour)
    {}
};

struct MadSyntaxKeyword
{
    wxString m_Name;
    MadAttributes m_Attr;
    vector < int > m_InRange;
    MadKeywordSet m_Keywords;
    bool m_CaseSensitive;

    MadSyntaxKeyword() : m_CaseSensitive(false)
    {}
};

enum MadAttributeElement
{
    aeText, aeDelimiter, aeSpace, aeNumber, aeString, aeComment, aeDirective,
    aeSpecialWord, aeLineNumber, aeActiveLine, aeBookmark, aeNone
};

struct MadState
{
    wxByte rangeid;
    wxByte blkcmtid;
    wxByte linecmt;
    wxByte stringid;
    wxByte directive;
    void Reset()
    {
        rangeid=blkcmtid=stringid=linecmt=directive=0;
    }
};


class MadEdit;
class MadEncoding;
class wxFileConfig;

class MadSyntax
{
private: // syntax files manager
    static wxString s_AttributeFilePath;
    static bool s_Loaded;
    static void LoadSyntaxFiles();
public:
    static void SetAttributeFilePath(const wxString &path) // where to load/save .clr files
    {
        s_AttributeFilePath=path;
    }
    static void AddSyntaxFilesPath(const wxString &path);
    static size_t GetSyntaxCount();
    static wxString GetSyntaxTitle(size_t index); //index must < GetSyntaxCount()
    static wxString GetSyntaxFile(size_t index); //index must < GetSyntaxCount()
    static wxString GetSyntaxFileByTitle(const wxString &title);
    static wxString GetAttributeFileByTitle(const wxString &title);

    // if title was not found it will return a default MadSyntax obj
    static MadSyntax* GetSyntaxByTitle(const wxString &title);

    // below functions will return NULL if not found
    static MadSyntax* GetSyntaxByExt(const wxString &ext);
    static MadSyntax* GetSyntaxByFirstLine(wxByte *data, int size);
    static MadSyntax* GetSyntaxByFileName(const wxString &filename);

    // .sch files management functions
    static size_t GetSchemeCount();
    static wxString GetSchemeName(size_t index);
    static wxString GetSchemeFileByName(const wxString &schname, MadSyntax *default_syn, bool &star);
    static bool LoadScheme(const wxString &schname, MadSyntax *syn); // apply scheme to syn
    static bool SaveScheme(const wxString &schname, MadSyntax *syn); // save schma from syn
    static bool DeleteScheme(const wxString &schname);

private:
    friend class MadEdit;
    friend class MadLines;
    MadAttributes m_SystemAttributes[aeNone];

    void  ParseSyntax(const wxString &filename);
    MadAttributes *GetAttributes(const wxString &name);
    MadSyntaxKeyword *GetCustomKeyword(const wxString &name);

    void  SetColor(const wxString &value, wxColour &c);
    void  SetStyle(const wxString &value, MadFontStyles &fs);
    void  SetInRange(const wxString &value, vector < int > &ir);

public:
    wxString            m_Title;
    bool                m_CaseSensitive;
    wxString            m_Delimiter;
    vector < wxString > m_LineComment;
    vector < wxString > m_BlockCommentOn;
    vector < wxString > m_BlockCommentOff;
    wxString            m_EscapeChar;
    wxString            m_StringChar;
    wxString            m_DirectiveLeading;
    wxString            m_KeywordPrefix;
    wxString            m_SpecialWordPrefix;
    wxString            m_IndentChar;
    wxString            m_UnindentChar;
    vector < wxString > m_LeftBrace;
    vector < wxString > m_RightBrace;
    wxString            m_AutoCompleteLeftChar;
    wxString            m_AutoCompleteRightChar;
    wxString            m_Encoding;

    vector < int >      m_StringInRange;
    vector < int >      m_LineCommentInRange;
    vector < std::vector < int > >m_BlockCommentInRange;

    bool                m_LineCommentAtBOL; // for diff/patch syntax
    bool                m_DirectiveLeadingAtBOL;

    vector < MadSyntaxRange > m_CustomRange; // user defined ranges
    vector < wxString >       m_RangeBeginString;

    vector < MadSyntaxKeyword > m_CustomKeyword; // user defined keywords

    bool m_CheckState;

public:
    MadSyntax(const wxString &filename, bool loadAttr = true);
    MadSyntax(bool loadAttr = true);
    ~MadSyntax();

    void LoadFromFile(const wxString &filename);
    void Reset();
    void LoadAttributes(const wxString &file = wxEmptyString); // if file IsEmpty, load from default .att file
    void SaveAttributes(const wxString &file = wxEmptyString); // if file IsEmpty, save to default .att file
    void AssignAttributes(MadSyntax *syn, bool add = false); // assign attributes from syn

    bool IsInRange(int range, vector < int >&InRangeVector);
    MadSyntaxRange *GetSyntaxRange(int rangeid);

    MadAttributes *GetAttributes(MadAttributeElement ae)
    {
        return m_SystemAttributes + ae;
    }
    static wxString GetAttributeName(MadAttributeElement ae);

    bool IsSpace(ucs4_t uc)
    {
        return (uc == 0x20 || (uc <= 0x0D && uc >= 0x09));
    }

    bool IsDelimiter(ucs4_t uc)
    {
        return (uc < 0x100 && m_Delimiter.Find(uc)>=0);
    }
    bool IsNotDelimiter(ucs4_t uc)
    {
        return (uc < 0x100 && m_Delimiter.Find(uc)<0 && !IsSpace(uc));
    }

    void SetAttributes(MadAttributeElement ae)
    {
        SetAttributes(m_SystemAttributes+ae);
    }
    void SetAttributes(MadAttributes *attr);

private: // for NextWord()
    MadLines    *nw_MadLines;
    MadEdit     *nw_MadEdit;
    MadEncoding *nw_Encoding;
    ucs4_t      *nw_Word;
    int         *nw_Widths;
    wxString    nw_FontName;
    int         nw_FontSize;
    int         nw_FontFamily;

    size_t      nw_MaxKeywordLen;

    MadState nw_State, nw_NextState;

    size_t nw_NotSpaceCount;
    bool nw_ContainCommentOff;
    bool nw_CommentUntilEOL;
    bool nw_BeginOfLine;

    MadSyntaxRange *nw_SynRange;

    ucs4_t nw_StringChar, nw_EscapeChar;

    MadLineIterator nw_LineIter;
    MadRowIndexIterator nw_RowIndexIter;

    int nw_LineWidth;
    size_t nw_FirstIndex;
    size_t nw_RestCount;
    size_t nw_MaxLength;
    MadUCQueue nw_ucqueue;

    bool nw_EndOfLine;
    wxColor nw_Color, nw_BgColor, nw_CurrentBgColor;
    wxFont *nw_Font;

    int FindStringCase(MadUCQueue & ucqueue, size_t first,
                   MadStringIterator begin, const MadStringIterator & end,
                   size_t & len);
    int FindStringNoCase(MadUCQueue & ucqueue, size_t first,
                   MadStringIterator begin, const MadStringIterator & end,
                   size_t & len);

    typedef int (MadSyntax::*FindStringPtr)(MadUCQueue & ucqueue, size_t first,
                   MadStringIterator begin, const MadStringIterator & end,
                   size_t & len);
    FindStringPtr FindString;

public:
    void InitNextWord1(MadLines *madlines, ucs4_t *word, int *widths, const wxString &fontname, int fontsize, int fontfamily);
    void InitNextWord2(MadLineIterator &lit, size_t row);
    void SetEncoding(MadEncoding *encoding);

    // return wordlength
    int NextWord(int &wordwidth);

private: // for Printing
    bool m_PrintSyntax;
    MadAttributes m_old_SystemAttributes[aeNone];
    vector < wxColour > m_CustomRangeBgColor;
    vector < MadAttributes > m_CustomKeywordColor;
    void BeginPrint(bool printSyntax);
    void EndPrint();

};

extern const wxString MadPlainTextTitle;

#endif
