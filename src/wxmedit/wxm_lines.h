///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_lines.h
// Description: The Text/Data Buffer of wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_LINES_H_
#define _WXM_LINES_H_

#include "../xm/cxx11.h"
#include "../xm/line_enc_adapter.h"
#include "../xm/encoding/encoding.h"
#include "../wxm/def.h"

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

#include <wx/file.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/unistr.h>
using namespace U_ICU_NAMESPACE;

#include <boost/scoped_ptr.hpp>

#include <vector>
#include <list>
#include <deque>
#include <utility>
#include <exception>

class MadEdit;

namespace wxm
{
	struct WXMEncoding;
	struct InFrameWXMEdit;
	struct WXMSearcher;
	struct BraceXPosAdjustor;
	struct NoWrapData;

	struct NewLineChar
	{
		NewLineChar() {}
		virtual bool IsDefault() const { return false; }
		virtual wxString Name() const = 0;
		virtual const wxString& Description() const = 0;
		virtual wxString wxValue() const = 0;
		virtual const ucs4string& Value() const = 0;
		virtual void ValueAppendTo(std::vector<ucs4_t>& vec) const = 0;
		virtual void Convert0x0D(ucs4_t& ch, std::vector<ucs4_t>& ucs) const = 0;
		virtual void Convert0x0A(ucs4_t& ch, std::vector<ucs4_t>& ucs) const = 0;
		virtual std::vector<wxPoint>& PatternPoints(MadEdit* edit) const = 0;

		virtual ~NewLineChar() {}
	protected:
		static const wxString MACDescription;
		static const wxString UNIXDescription;
		static const wxString DOSDescription;
		static const wxString NoneDescription;

		static const ucs4string MACValue;
		static const ucs4string UNIXValue;
		static const ucs4string DOSValue;
		static const ucs4string NoneValue;
	};

	struct NewLineDOS: public NewLineChar
	{
		NewLineDOS() {}
	private:
		virtual wxString Name() const override { return wxT("DOS"); }
		virtual const wxString& Description() const override { return DOSDescription; }
		virtual const ucs4string& Value() const override { return DOSValue; }
		virtual void Convert0x0D(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override { ucs.push_back(ch); ch = 0x0A; }
		virtual void Convert0x0A(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override { ucs.push_back(0x0D); }
		virtual std::vector<wxPoint>& PatternPoints(MadEdit* edit) const override;
	public:
		virtual wxString wxValue() const override { return wxT("\r\n"); }
		virtual void ValueAppendTo(std::vector<ucs4_t>& v) const override { v.push_back(0x0D); v.push_back(0x0A); }
	};

	struct NewLineUNIX: public NewLineChar
	{
		NewLineUNIX() {}
	private:
		virtual wxString Name() const override { return wxT("UNIX"); }
		virtual const wxString& Description() const override { return UNIXDescription; }
		virtual const ucs4string& Value() const override { return UNIXValue; }
		virtual void Convert0x0D(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override { ch = 0x0A; }
		virtual void Convert0x0A(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override {}
		virtual std::vector<wxPoint>& PatternPoints(MadEdit* edit) const override;
	public:
		virtual wxString wxValue() const override { return wxT("\n"); }
		virtual void ValueAppendTo(std::vector<ucs4_t>& v) const override { v.push_back(0x0A); }
	};

	struct NewLineDefault : public
#ifdef __WXMSW__
		NewLineDOS
#else
		NewLineUNIX
#endif
	{
		NewLineDefault() {}
	private:
		virtual bool IsDefault() const override { return true; }
	};

	struct NewLineMAC: public NewLineChar
	{
		NewLineMAC() {}
	private:
		virtual wxString Name() const override { return wxT("MAC"); }
		virtual const wxString& Description() const override { return MACDescription; }
		virtual const ucs4string& Value() const override { return MACValue; }
		virtual void Convert0x0D(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override {}
		virtual void Convert0x0A(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override { ch = 0x0D; }
		virtual std::vector<wxPoint>& PatternPoints(MadEdit* edit) const override;
	public:
		virtual wxString wxValue() const override { return wxT("\r"); }
		virtual void ValueAppendTo(std::vector<ucs4_t>& v) const override { v.push_back(0x0D); }
	};

	struct NewLineNone: public NewLineChar
	{
		NewLineNone() {}
	private:
		virtual wxString Name() const override { return wxT("NoEOL"); }
		virtual const wxString& Description() const override { return NoneDescription; }
		virtual const ucs4string& Value() const override { return NoneValue; }
		virtual void Convert0x0D(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override {}
		virtual void Convert0x0A(ucs4_t& ch, std::vector<ucs4_t>& ucs) const override { ch = 0; }
		virtual wxString wxValue() const override { return wxT(""); }
		virtual void ValueAppendTo(std::vector<ucs4_t>& v) const override {}
		virtual std::vector<wxPoint>& PatternPoints(MadEdit* edit) const override;
	};

	extern const NewLineDefault g_nl_default;
	extern const NewLineDOS     g_nl_dos;
	extern const NewLineUNIX    g_nl_unix;
	extern const NewLineMAC     g_nl_mac;
	extern const NewLineNone    g_nl_none;

	struct ExtUCQueue
	{
		xm::UCQueue q;

		ExtUCQueue() : u16idx(0) {}
		void IncIndex()
		{
			u16idx += (q.back().ucs4() > 0x10000) ? 2 : 1;
		}
		int32_t U16Index() { return u16idx; }
		void pop_front() { q.pop_front(); }
		xm::CharUnit& back() { return q.back(); }
		xm::CharUnit& front() { return q.front(); }
		xm::CharUnit& operator[] (size_t idx) { return q[idx]; }
		bool empty() { return q.empty(); }
		size_t size() { return q.size(); }
		xm::UCQueue::iterator begin() { return q.begin(); }
		void clear()
		{
			u16idx = 0;
			q.clear();
		}
	private:
		int32_t u16idx;
	};
} // namespace wxm

using std::vector;
using std::list;
using std::deque;
using std::pair;

//===========================================================================
// MadFileNameIsUTF8, MadDirExists, MadConvFileName_WC2MB_UseLibc
// for Testing/Converting of FileName Encoding under Linux
//===========================================================================

// return 0: filename is not found, 1:yes, -1:no
int MadFileNameIsUTF8(const wxString& filename);

// return 0: name is not a dir or not found, 1:yes(utf8), -1:yes(libc)
int MadDirExists(const wxString& name);

extern bool g_WC2MB_2_utf8;
extern bool g_MB2WC_is_utf8;
extern bool g_MB2WC_check_dir_filename;//check dir, filename separately

class MadConvFileName_WC2MB_UseLibc
{
public:
    MadConvFileName_WC2MB_UseLibc(bool uselibc)
    {
        g_WC2MB_2_utf8 = !uselibc;
    }
    ~MadConvFileName_WC2MB_UseLibc()
    {
        g_WC2MB_2_utf8=true;
    }
};

#ifndef __WXMSW__
class MadConvFileName : public wxMBConv
{
public:
    virtual size_t MB2WC(wchar_t *outputBuf, const char *psz, size_t outputSize) const override;
    virtual size_t WC2MB(char *outputBuf, const wchar_t *psz, size_t outputSize) const override;
    virtual wxMBConv *Clone() const override { return new MadConvFileName; }
};
#endif

//==================================================
class MadInData
{
protected:
    wxFileOffset m_Size;
public:
    virtual ~MadInData() {}
    virtual wxByte Get(const wxFileOffset &pos) = 0;
    virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size) = 0;
    wxFileOffset GetSize() { return m_Size; }
};

class MadOutData
{
public:
    virtual ~MadOutData() {}
    virtual wxFileOffset Put(wxByte *buffer, size_t size) = 0;
};


class MadMemData : public MadInData, public MadOutData
{
private:
    friend class MadLines;
    friend class MadEdit;

    vector <wxByte*> m_Buffers;
    void Reset() { m_Size = 0; }

public:
    MadMemData();
    virtual ~MadMemData();

    virtual wxByte Get(const wxFileOffset &pos) override;
    virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size) override;
    virtual wxFileOffset Put(wxByte *buffer, size_t size) override;
};

class MadFileData : public MadInData, public MadOutData
{
private:
    friend class MadLines;

    wxFile m_File;
    bool m_OpenSuccess;
    wxString m_Name;
    bool m_ReadOnly;

    wxByte *m_Buffer1,*m_Buffer2;
    wxFileOffset m_Buf1Pos,m_Buf2Pos;

    wxFileOffset m_SavePos; // for Put()

    bool OpenFile();
    bool Rename(const wxString &name);

public:
    MadFileData(const wxString &name);
    virtual ~MadFileData();
    virtual wxByte Get(const wxFileOffset &pos) override;
    virtual void Get(const wxFileOffset &pos, wxByte *buffer, size_t size) override;

    virtual wxFileOffset Put(wxByte *buffer, size_t size) override;
    void Read(const wxFileOffset &pos, wxByte *buffer, size_t size)
    {
        m_File.Seek(pos);
        m_File.Read(buffer, size);
    }

    bool OpenSuccess() { return m_OpenSuccess; }
    bool IsReadOnly() { return m_ReadOnly; }
};

//==================================================

struct MadBlock//:public Loki::SmallObject <>
{
    MadInData   *m_Data;
    wxFileOffset m_Pos;
    wxFileOffset m_Size;

    MadBlock():m_Data(nullptr), m_Pos(0), m_Size(0)
    {
    }
    MadBlock(MadInData *data, const wxFileOffset &pos, const wxFileOffset &size)
        :m_Data(data), m_Pos(pos), m_Size(size)
    {
    }
    void Reset()
    {
        m_Data = nullptr;
        m_Pos = 0;
        m_Size = 0;
    }
    wxByte Get(const wxFileOffset &pos)
    {
        wxASSERT(pos >= 0 && pos < m_Size);
        return m_Data->Get(m_Pos + pos);
    }
    void Get(const wxFileOffset &pos, wxByte *buffer, size_t size)
    {
        wxASSERT(pos >= 0 && pos < m_Size);
        m_Data->Get(m_Pos + pos, buffer, size);
    }
};

struct MadRowIndex    // wrapped-line index
{
    wxFileOffset m_Start;                   // start position
    int m_Width;                            // pixel-width of this row

    MadRowIndex():m_Start(0), m_Width(0)
    {
    }
    MadRowIndex(const wxFileOffset &start, int width):m_Start(start), m_Width(width)
    {
    }
    void Reset()
    {
        m_Start = 0;
        m_Width = 0;
    }
};

struct MadLineState
{
    wxByte RangeId;     // 0: Global Range, 1..255: User Defined Range
    wxByte CommentId;   // 0: not in blockcomment, 1..255: User Defined BlockComment
    wxByte CommentOff;  // contains a BlockCommentOff string in this line, for speed-up of parsing
    wxByte StringId;    // 0: not in string, 1..255: in string
    wxByte LineComment; // 0: not in linecomment, 1..255: in linecomment
    wxByte Directive;   // 0: not in directive, 1..255: in directive

    MadLineState():RangeId(0),CommentId(0),CommentOff(0),StringId(0),LineComment(0),Directive(0)
    {
    }
    bool operator==(const MadLineState &ls) const
    {
        return RangeId == ls.RangeId && CommentId == ls.CommentId &&
            CommentOff == ls.CommentOff && StringId == ls.StringId &&
            LineComment == ls.LineComment && Directive == ls.Directive;
    }
};

struct BracePairIndex
{
    int          XPos;       // pixel-width pos
    wxUint16     Width;
    wxFileOffset LinePos;    // linepos
    wxUint16     Length;     // byte-length
    wxByte       LeftPair;   // non-zero: leftpair, zero: rightpair
    char         BraceIndex; //

    BracePairIndex()
    {}

    BracePairIndex(int xpos, wxUint16 wid, wxFileOffset lpos, wxUint16 len, wxByte lp, char bi)
        :XPos(xpos), Width(wid), LinePos(lpos), Length(len), LeftPair(lp), BraceIndex(bi)
    {}
};

typedef vector < MadRowIndex >::iterator MadRowIndexIterator;

struct MadLine
{
    xm::BlockVector         m_Blocks;
    vector <MadRowIndex>    m_RowIndices;
    wxFileOffset            m_Size;         // data size, include m_NewLineSize
    wxByte                  m_NewLineSize;
    const wxm::NewLineChar* m_nl;

    MadLineState            m_State;

    vector <BracePairIndex> m_BracePairIndices;

    MadLine():m_Size(0), m_NewLineSize(0), m_nl(&wxm::g_nl_none)
    {
    }
    void Reset();
    void Empty();
    bool IsEmpty() { return m_Size == 0; }

    // if false return 0 ; else return 0x0D or 0x0A
    ucs4_t LastUCharIsNewLine(xm::Encoding *encoding);
    bool FirstUCharIs0x0A(xm::Encoding *encoding);

    wxByte Get(wxFileOffset pos)                            // get 1 byte
    {
        wxASSERT(pos >= 0 && pos < m_Size);

        xm::BlockIterator biter = m_Blocks.begin();
        if(pos >= biter->m_Size)
            do
            {
                pos -= biter->m_Size;
                ++biter;
            }
            while(pos >= biter->m_Size);

        return biter->Get(pos);
    }

    void Get(wxFileOffset pos, wxByte *buf, size_t size)    // get n bytes
    {
        wxASSERT(pos >= 0 && pos + (wxFileOffset)size <= m_Size);
        if (size == 0)
            return;

        xm::BlockIterator biter = m_Blocks.begin();
        if(pos >= biter->m_Size)
        {
            do
            {
                pos -= biter->m_Size;
            }
            while(pos >= (++biter)->m_Size);
        }

        size_t cnt;
        while(true)
        {
            cnt=size;
            if(wxFileOffset(pos+cnt) > biter->m_Size) cnt=(size_t)(biter->m_Size-pos);
            biter->Get(pos, buf, cnt);
            if((size-=cnt)==0) break;

            pos=0;
            buf+=cnt;
            ++biter;
        }
    }


    size_t RowCount()
    {
        return m_RowIndices.size() - 1;
    }
};

//==================================================

typedef xm::UCQueue::iterator      UCQueueIterator;
typedef list<MadLine>::iterator    MadLineIterator;
typedef vector<wxString>::iterator MadStringIterator;

class MadLineList : public list <MadLine>
{
    list<MadLineIterator> m_BookmarkList;

public:
    MadLineList();

    void ToggleBookmark( MadLineIterator position );      // toggle bookmark from given position
    int  GetNextBookmark( MadLineIterator position );     // return line number, or -1 if no bookmars
    int  GetPreviousBookmark( MadLineIterator position ); // return line number from the end to the beginning, or -1
    bool Bookmarked( MadLineIterator position );
    void ClearAllBookmarks();
    bool BookmarkExist() { return !m_BookmarkList.empty(); }

    LineNumberList SaveBookmarkLineNumberList() const;
    void RestoreBookmarkByLineNumberList(const LineNumberList& linenums);

    MadLineIterator erase( MadLineIterator position );

private:
    // it is private because bookmarks removal is not implemented yet for it
    // in this way if someone try to use it, he will get compiler error
    MadLineIterator erase( MadLineIterator first, MadLineIterator last )
        { return list<MadLine>::erase(first, last); }
};
typedef list<MadLineIterator>::iterator  MadBookmarkIterator;

class MadEdit;
class MadSyntax;
struct MadSyntaxRange;

class MadLines: public xm::UChar32BytesMapper
{
private:
    friend class MadEdit;
    friend struct wxm::InFrameWXMEdit;
    friend class MadSyntax;
    friend struct wxm::WXMSearcher;

    MadEdit       *m_MadEdit;
    MadSyntax     *m_Syntax;
    xm::Encoding  *m_Encoding;

    MadLineList m_LineList;
    size_t m_LineCount, m_RowCount;         // line counts
    wxFileOffset m_Size;                    // total size

    MadLineState m_EndState;

    MadFileData *m_FileData;
    MadFileData *m_TmpFileData;

    MadMemData *m_MemData;

    wxString    m_Name;
    bool        m_ReadOnly;
    int         m_MaxLineWidth;             //max pixel width of line/row

    wxByte          *m_WriteBuffer;
    vector<wxByte>  m_WriteBufferVector;

private:

    void Empty(bool freeAll);
    void Clear(bool freeAll);

    size_t ReformatCount;
    // reformat single line, return the state of line-end
    MadLineState Reformat(MadLineIterator iter);
    // reformat lines in [first,last]
    size_t Reformat(MadLineIterator first, MadLineIterator last);

    void DoCheckState(MadLineIterator iter, wxm::ExtUCQueue& ucqueue, xm::CharUnit& cu, ucs4_t prevuc, ucs4_t& lastuc, int& notSpaceCount, size_t& eatUCharCount, int& index, size_t& length, size_t bracepos, int*& bracexpos, int& bracexpos_count, MadLineState& state, MadStringIterator& sit, MadStringIterator& sitend, bool BeginOfLine, MadSyntaxRange* srange);

    // Recount all lines' width
    void RecountLineWidth(void);

    void DoWordWrap(MadLineIterator iter, wxm::BraceXPosAdjustor& brxpos_adj, bool word_canmove, bool canbreak, MadRowIndex& rowidx, size_t& rowlen, size_t& rowidx_idx, wxm::NoWrapData& nowrap);
    void NoWrapAccumulate(wxm::NoWrapData& nowrap, int ucwidth, size_t uclen, bool canbreak);
    UnicodeString DumpUTF16String(MadLineIterator iter);

    // append lit2 after lit1
    void Append(const MadLineIterator &lit1, const MadLineIterator &lit2);

    // write to fd or file if which one isn't Null
    void WriteBlockToData(MadOutData *fd, const xm::BlockIterator &bit);
    void WriteToFile(wxFile &file, MadFileData *oldfd, MadFileData *newfd);

    wxFileOffset GetMaxTempSize(const wxString &filename);

    void InitFileSyntax();

public:
    MadLines(MadEdit *madedit);
    virtual ~MadLines();

    bool LoadFromFile(const wxString& filename, const std::wstring& encoding=wxEmptyString, bool hexmode=false);
    bool SaveToFile(const wxString& filename, const wxString& tempdir);
    wxFileOffset GetSize() { return m_Size; }

private:  // NextUChar()
    wxByte          *m_NextUChar_Buffer;
    size_t          m_NextUChar_BufferStart;
    size_t          m_NextUChar_BufferSize;
    wxFileOffset    m_NextUChar_BufferNextPos;
    bool            m_NextUChar_BufferLoadNew;
    void LoadNewBuffer();


    MadLineIterator m_NextUChar_LineIter;
    wxFileOffset    m_NextUChar_LineSize;
    wxFileOffset    m_NextUChar_Pos;

    bool m_manual;

    UErrorCode m_line_bi_status;
    boost::scoped_ptr<BreakIterator> m_line_bi;

    virtual void MoveUChar32Bytes(xm::UCQueue &ucqueue, ucs4_t uc, size_t len) override;
    virtual ubyte* BufferLoadBytes(int64_t& rest, size_t buf_len) override;

    bool NextUCharIs0x0A(void);

    void DetectSyntax(const wxString &filename);

    bool PresetFileEncoding(const std::wstring& encoding, const wxByte* buf, size_t sz);
    void SetFileEncoding(const std::wstring& encoding, const std::wstring& defaultenc,
                         const wxByte* buf, size_t sz, bool skip_utf8);

    int FindStringCase(wxm::ExtUCQueue &ucqueue, MadStringIterator begin,
                   const MadStringIterator &end, size_t &len);

    // the [begin,end) iter must be lower case
    int FindStringNoCase(wxm::ExtUCQueue &ucqueue, MadStringIterator begin,
                   const MadStringIterator &end, size_t &len);

    typedef int (MadLines::*FindStringPtr)(wxm::ExtUCQueue &ucqueue,
                MadStringIterator begin, const MadStringIterator &end, size_t &len);

    FindStringPtr FindString;

public:
    void SetManual(bool manual) { m_manual = true; }
    void SetEncoding(xm::Encoding *encoding);

    void InitNextUChar(const MadLineIterator &iter, const wxFileOffset pos);

    bool NextUChar(xm::UCQueue &ucqueue);
    bool NextUChar(wxm::ExtUCQueue &ucq)
    {
        bool r = NextUChar(ucq.q);
        if (r)
            ucq.IncIndex();
        return r;
    }

    // should not frequently use this, it's slowly
    // if no, return xm::CharUnit(0, 0)
    xm::CharUnit PreviousUChar(/*IN_OUT*/MadLineIterator &lit, /*IN_OUT*/wxFileOffset &linepos);
};


#endif
