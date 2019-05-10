///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/searcher.cpp
// Description: wxMEdit Searching and Replacing Functions
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "searcher.h"
#include "../xm/cxx11.h"
#include "../wxmedit/wxmedit.h"
#include "case_conv.h"
#include "../xm/utils.hpp"
#include "../mad_utils.h"

#include <unicode/uchar.h>
#include <iostream>
#include <string>

//#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/xpressive/traits/null_regex_traits.hpp>
#include <boost/xpressive/traits/cpp_regex_traits.hpp>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost::xpressive;

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

template<typename char_type>
inline char_type NormalCase(char_type ch, bool sensitive)
{
	return sensitive ? ch : (char_type)u_tolower((UChar32)ch);
}

template<>
inline wxByte NormalCase(wxByte ch, bool sensitive)
{
	return ch;
}

class JumpTable_Hex
{
private:
	int m_Table[256];
	std::vector<wxByte> m_Pattern;
public:
	void Build(const std::vector<wxByte>& pat)
	{
		if (m_Pattern == pat)
			return;
		m_Pattern = pat;
		size_t len = pat.size();

		for (size_t i = 0; i<256; ++i) m_Table[i] = (int)len + 1;
		for (size_t i = 0; i<len; ++i) m_Table[pat[i]] = (int)(len - i);
	}
	int GetValue(const wxByte ch) const
	{
		return m_Table[ch];
	}
};

typedef boost::unordered_map<unsigned int, int> UCS4_Map;

class JumpTable_UCS4
{
private:
	int m_UCS2_Table[65536];
	UCS4_Map m_Table;
	ucs4string m_Pattern;
	int m_Len_1;
public:
	void Build(const ucs4string& ucs)
	{
		if (m_Pattern == ucs)
			return;
		m_Pattern = ucs;

		const int len1 = m_Len_1 = (int)ucs.size() + 1;
		int *ptab = m_UCS2_Table;
		for (size_t i = 0; i<65536; ++i, ++ptab) *ptab = len1;

		m_Table.clear();
		size_t len = ucs.size();
		for (size_t i = 0; i<ucs.size(); ++i)
		{
			const unsigned int idx = (unsigned int)ucs[i];
			if (idx <= 0xFFFF)
				m_UCS2_Table[idx] = (int)(len - i);
			else
				m_Table[idx] = (int)(len - i);
		}
	}
	int GetValue(const ucs4_t ch) const
	{
		if (ch <= 0xFFFF && ch >= 0)
		{
			return m_UCS2_Table[(unsigned int)ch];
		}

		return xm::wrap_map(m_Table).get((unsigned int)ch, m_Len_1);
	}
};

template <typename CharIter, typename JumpTable, typename Seq>
bool Search(CharIter &begin, CharIter &end, const Seq& pattern,
	const JumpTable &jump_table, bool bCaseSensitive)
	// if (!bCaseSensitive) the content of 'pattern' must be normal case.
{
	size_t pat_len = pattern.size();
	wxASSERT(pat_len != 0);

	if (begin == end)
		return false;

	typename Seq::const_iterator p = pattern.begin();
	typename Seq::value_type c1;
	size_t idx = 0;
	CharIter beginpos = begin;

	for (;;)
	{
		c1 = NormalCase(*begin, bCaseSensitive);

		if (c1 == *p)
		{
			if (idx == 0)
				beginpos = begin;
			++idx;
			++p;

			if (idx == pat_len) // found
			{
				end = begin;
				++end;
				begin = beginpos;
				return true;
			}

			// compare next char
			if (++begin == end)
				return false;

			continue;
		}

		// c1 != *p
		// jump by the jump_table
		CharIter it = begin;
		int i = (int)(pat_len - idx);
		do
		{
			if (++it == end)
				return false;
		} while (--i > 0);

		c1 = NormalCase(*it, bCaseSensitive);

		int jv = jump_table.GetValue(c1);
		if (jv >= (int)pat_len)
		{
			begin = it;
			jv -= (int)pat_len;
			idx = 0;
			p = pattern.begin();
		}
		else if (idx != 0)
		{
			begin = beginpos;
			idx = 0;
			p = pattern.begin();
		}

		if (jv > 0)
		{
			do
			{
				if (++begin == end)
					return false;
			} while (--jv > 0);
		}
	}

	return false;
}


#ifdef __WXMSW__
namespace boost { namespace xpressive { namespace detail
{
	template<>
	struct string_type<ucs4_t>  // defined in xpressive/detail/detail_fwd.hpp
	{
		typedef ucs4string type;
	};

	template<>
	struct string_literal<ucs4_t>
	{
		static ucs4string inter_str;

		static ucs4_t const *pick(char const *, wchar_t const *cstr)
		{
			inter_str.clear();
			while (*cstr != 0)
			{
				inter_str.push_back(ucs4_t(*cstr));
				++cstr;
			}

			return inter_str.c_str();
		}

		static ucs4_t pick(char, wchar_t ch)
		{
			return ch;
		}
	};

	ucs4string string_literal<ucs4_t>::inter_str;

}}}


struct ucs4_regex_traits : public null_regex_traits<ucs4_t>
{
	typedef ucs4_t char_type;
	typedef detail::umaskex_t char_class_type;
	typedef ucs4string string_type;
	typedef std::locale locale_type;

	template<typename char_type2>
	static char_type2 tolower(char_type2 ch)
	{
		return (char_type2)u_tolower((UChar32)ch);
	}

	template<typename char_type2>
	static char_type2 toupper(char_type2 ch)
	{
		return (char_type2)u_toupper((UChar32)ch);
	}

	static char_type widen(char ch)
	{
		return char_type(ch);
	}

	char_type translate_nocase(char_type ch) const
	{
		return this->tolower(ch);
	}

	bool in_range_nocase(char_type first, char_type last, char_type ch) const
	{
		return this->in_range(first, last, ch)
			|| this->in_range(first, last, this->toupper(ch))
			|| this->in_range(first, last, this->tolower(ch));
	}

	template<typename FwdIter>
	static char_class_type lookup_classname(FwdIter begin, FwdIter end, bool icase)
	{
		cpp_regex_traits<char> cpptraits;
		char_class_type c = cpptraits.lookup_classname(begin, end, icase);
		return c;
	}

	static bool isctype(char_type ch, char_class_type mask)
	{
		cpp_regex_traits<char> cpptraits;
		int i = ch;
		if (i<0 || i>0xFF) ch = 0x0;//?
		return cpptraits.isctype(char(ch), mask);
	}

	static int value(char_type ch, int radix)
	{
		switch (radix)
		{
		case 8:
			if (ch >= '0' && ch <= '7') return ch - '0';
			break;
		case 10:
			if (ch >= '0' && ch <= '9') return ch - '0';
			break;
		case 16:
			if (ch >= '0' && ch <= '9') return ch - '0';
			if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
			if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
			break;
		}
		return -1;
	}
};

#else // __WXGTK__

typedef cpp_regex_traits<ucs4_t> ucs4_regex_traits;

#endif


struct UCQueueSet
{
	xm::UCQueue  ucq;
	int          lock;
};

struct UCIterator : public WXMCharIterator   // ucs4_t widechar iterator
{
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef ucs4_t value_type;
	typedef wxFileOffset difference_type;
	typedef const value_type *pointer;
	typedef const value_type &reference;

	static MadLines         *s_lines;
	static wxFileOffset     s_endpos;
	static list<UCQueueSet> s_ucqueues;

	static void Init(MadLines *lines, const wxFileOffset &endpos)
	{
		wxASSERT(endpos >= 0 && endpos <= lines->GetSize());

		s_lines = lines;
		s_endpos = endpos;
		s_ucqueues.clear();
	}

	typedef list<UCQueueSet>::iterator UCQIterator;
#define UCQ_MAXSIZE (10 * 1024)
#define BUF_MAXSIZE (5 * 1024)

	UCQIterator     ucqit;
	int             ucqidx;

	UCIterator() :ucqidx(-1) {}

	~UCIterator()
	{
		if (ucqidx >= 0 && --ucqit->lock == 0)
		{
			s_ucqueues.erase(ucqit);
		}
	}

	//UCIterator(wxFileOffset pos0):ucqidx(-1), pos(pos0) {}

	UCIterator(const UCIterator &ucit) :ucqidx(-1)
	{
		this->operator =(ucit);
	}

	UCIterator(const MadCaretPos& cp)
		:WXMCharIterator(cp)
	{
		if (linepos == lit->m_Size && pos<s_lines->GetSize())
		{
			++lit;
			linepos = 0;
		}

		s_ucqueues.push_back(UCQueueSet());
		ucqit = s_ucqueues.end();
		--ucqit;

		ucqit->lock = 1;            // lock this ucqueue

		ucqidx = 0;

		if (pos >= s_lines->GetSize())
			return;

		xm::UCQueue& ucqueue = ucqit->ucq;
		s_lines->InitNextUChar(lit, linepos);
		int i = BUF_MAXSIZE;

		if (pos >= s_endpos) i = 10;

		while (--i>0 && s_lines->NextUChar(ucqueue))
		{
			//ucqit->size += ucqueue.back().second;
		}
	}

	UCIterator & operator=(const UCIterator & it)
	{
		if (ucqidx >= 0 && --ucqit->lock == 0)
		{
			wxASSERT(ucqit != it.ucqit);
			s_ucqueues.erase(ucqit);
		}

		pos = it.pos;
		lit = it.lit;
		linepos = it.linepos;
		ucqit = it.ucqit;
		ucqidx = it.ucqidx;

		if (ucqidx >= 0) ucqit->lock++;

		return *this;
	}

	const value_type operator*() const
	{
		wxASSERT(ucqidx >= 0 && ucqidx < int(ucqit->ucq.size()));

		return ucqit->ucq[ucqidx].ucs4();
	}

	/***
	ucs4_t *operator->() const
	{
	return _ws_ + pos;
	}
	***/

	// pre-increment operator
	UCIterator & operator++()
	{
		wxASSERT(ucqidx >= 0 && ucqidx < int(ucqit->ucq.size()));

		xm::UCQueue *ucqueue = &(ucqit->ucq);

		int len = (*ucqueue)[ucqidx].nbytes();
		pos += len;
		linepos += len;

		if (linepos == lit->m_Size)
		{
			if (pos == s_endpos)
				return *this; // end

			++lit;
			linepos = 0;
		}

		++ucqidx;

		if (ucqidx == (int)(*ucqueue).size())
		{
			wxASSERT(pos <= s_endpos);

			if (ucqidx >= UCQ_MAXSIZE)
			{
				if (--ucqit->lock == 0)
				{
					s_ucqueues.erase(ucqit);
				}

				s_ucqueues.push_back(UCQueueSet());
				ucqit = s_ucqueues.end();
				--ucqit;

				ucqit->lock = 1;            // lock this ucqueue

				ucqidx = 0;

				ucqueue = &(ucqit->ucq);
			}

			s_lines->InitNextUChar(lit, linepos);
			int i = BUF_MAXSIZE;
			while (--i>0 && s_lines->NextUChar(*ucqueue))
			{
				//ucqit->size += ucqueue->back().second;
			}

		}

		return *this;
	}

	/***
	// post-increment operator
	UCIterator operator++(int)
	{
	UCIterator tmp = *this;
	++*this;
	return tmp;
	}
	***/

	//***
	// pre-decrement operator
	UCIterator & operator--()
	{
		wxASSERT(pos>0 && ucqidx >= 0 && ucqidx <= int(ucqit->ucq.size()));

		if (ucqidx == 0)                   //rarely happen
		{
			if (--ucqit->lock == 0)
			{
				s_ucqueues.erase(ucqit);
			}

			s_ucqueues.push_back(UCQueueSet());
			ucqit = s_ucqueues.end();
			--ucqit;

			ucqit->lock = 1;              // lock this ucqueue

			ucqidx = 0;

			xm::CharUnit cu = s_lines->PreviousUChar(lit, linepos);

			wxASSERT(cu.nbytes() != 0);

			pos -= cu.nbytes();
			ucqit->ucq.push_back(cu);

			return *this;
		}

		--ucqidx;
		int len = ucqit->ucq[ucqidx].nbytes();
		pos -= len;

		if (linepos == 0)
		{
			--lit;
			linepos = lit->m_Size;
		}
		linepos -= len;

		return *this;
	}
	//***/

	/***
	// post-decrement operator
	UCIterator operator--(int)
	{
	UCIterator tmp = *this;
	--*this;
	return tmp;
	}
	***/

	bool operator==(const UCIterator & it) const
	{
		if (pos == it.pos)
			return true;
		return (pos >= s_endpos && it.pos >= s_endpos);
	}

	bool operator!=(const UCIterator & it) const
	{
		return !(this->operator==(it));
	}

};

MadLines *UCIterator::s_lines = nullptr;
wxFileOffset UCIterator::s_endpos = 0;
list<UCQueueSet> UCIterator::s_ucqueues;

void MadCaretPos::AssignWith(const WXMCharIterator& ucit)
{
	pos = ucit.pos;
	iter = ucit.lit;
	linepos = ucit.linepos;
}

// convert escape char to literal char
ucs4string ConvertEscape(const ucs4string &str)
{
	ucs4string out;
	detail::escape_value<ucs4_t, ucs4_regex_traits::char_class_type> esc;

	ucs4string::const_iterator begin = str.begin();
	ucs4string::const_iterator end = str.end();
	compiler_traits<ucs4_regex_traits> ucs4traits;

	while (begin != end)
	{
		if (*begin != '\\')
		{
			out += *begin;
			++begin;
			continue;
		}

		// last char is '\'
		if (++begin == end)
			throw regex_error(regex_constants::error_escape);

		esc = detail::parse_escape(begin, end, ucs4traits);
		out += esc.ch_;
	}

	return out;
}

// Hex Search
struct ByteIterator : public WXMCharIterator
{
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef wxByte value_type;
	typedef wxFileOffset difference_type;
	typedef const value_type *pointer;
	typedef const value_type &reference;

	static MadLines         *s_lines;
	static wxFileOffset     s_endpos;

	static void Init(MadLines *lines, const wxFileOffset &endpos)
	{
		wxASSERT(endpos >= 0 && endpos <= lines->GetSize());

		s_lines = lines;
		s_endpos = endpos;
	}

	ByteIterator() {}

	ByteIterator(const ByteIterator &it)
	{
		this->operator =(it);
	}

	ByteIterator(const MadCaretPos& cp)
		:WXMCharIterator(cp)
	{
		if (linepos == lit->m_Size && pos<s_lines->GetSize())
		{
			++lit;
			linepos = 0;
		}
	}

	ByteIterator & operator=(const ByteIterator & it)
	{
		pos = it.pos;
		lit = it.lit;
		linepos = it.linepos;
		return *this;
	}

	const value_type operator*()
	{
		wxASSERT(linepos < lit->m_Size);
		return lit->Get(linepos);
	}

	/***
	ucs4_t *operator->() const
	{
	return _ws_ + pos;
	}
	***/

	// pre-increment operator
	ByteIterator & operator++()
	{
		++pos;
		++linepos;

		if (linepos == lit->m_Size)
		{
			if (pos == s_endpos)
				return *this; // end

			++lit;
			linepos = 0;
		}

		return *this;
	}

	/***
	// post-increment operator
	ByteIterator operator++(int)
	{
	ByteIterator tmp = *this;
	++*this;
	return tmp;
	}
	***/

	//***
	// pre-decrement operator
	ByteIterator & operator--()
	{
		wxASSERT(pos>0);

		--pos;

		if (linepos == 0)
		{
			--lit;
			linepos = lit->m_Size;
		}
		--linepos;

		return *this;
	}
	//***/

	/***
	// post-decrement operator
	ByteIterator operator--(int)
	{
	ByteIterator tmp = *this;
	--*this;
	return tmp;
	}
	***/

	bool operator==(const ByteIterator & it) const
	{
		if (pos == it.pos)
			return true;
		return (pos >= s_endpos && it.pos >= s_endpos);
	}

	bool operator!=(const ByteIterator & it) const
	{
		return !(this->operator==(it));
	}

};

MadLines *ByteIterator::s_lines = nullptr;
wxFileOffset ByteIterator::s_endpos = 0;

extern bool g_regex_dot_match_newline;

namespace wxm
{

ucs4string WXMSearcher::from_wxString(const wxString& wxs)
{
#ifdef __WXMSW__
	vector<ucs4_t> ucs;
	m_edit->TranslateText(wxs.c_str(), wxs.Len(), ucs, true);
	return ucs4string(ucs.begin(), ucs.end());
#else
	const ucs4_t *puc = wxs.c_str();
	size_t len = wxs.Len();
	return ucs4string(puc, puc + len);
#endif
}

void WXMSearcher::AssignTextFileBegin(MadCaretPos& cp)
{
	cp.iter = m_edit->m_Lines->m_LineList.begin();
	cp.pos = cp.iter->m_RowIndices[0].m_Start;
	if (m_edit->m_CaretPos.pos < cp.pos || m_edit->m_EditMode == emHexMode)
		cp.pos = 0;
	cp.linepos = cp.pos;
}

void WXMSearcher::AssignHexFileBegin(MadCaretPos& cp)
{
	cp.pos = 0;
	cp.iter = m_edit->m_Lines->m_LineList.begin();
	cp.linepos = 0;
}

void WXMSearcher::AssignFileEnd(MadCaretPos& cp)
{
	cp.pos = m_edit->m_Lines->m_Size;
	cp.iter = m_edit->m_Lines->m_LineList.end();
	--cp.iter;
	cp.linepos = cp.iter->m_Size;
}

void WXMSearcher::AssignCaretPos(wxFileOffset& pos, MadCaretPos& cp)
{
	if (pos > m_edit->GetFileSize())
		pos = m_edit->GetFileSize();
	cp.pos = pos;
	cp.linepos = pos;
	m_edit->GetLineByPos(cp.iter, cp.linepos, cp.rowid);
	cp.linepos = pos - cp.linepos;
}

bool TextSearcher::IsDelimiterChar(ucs4_t uc)
{
	return (uc <= 0x20 || m_edit->m_Syntax->IsDelimiter(uc) || uc == 0x3000);
}

bool TextSearcher::IsWordBoundary(UCIterator& ucit)
{
	ucs4_t uc = *ucit;
	if (IsDelimiterChar(uc))
		return true;

	// check prev-uchar of ucit
	--ucit;
	uc = *ucit;
	if (!IsDelimiterChar(uc))
		return false;

	return true;
}

bool TextSearcher::IsWordBoundary(UCIterator ucit1, UCIterator ucit2)
{
	if (ucit1.pos >= ucit2.pos)
		return true;

	// check ucit2
	if (ucit2.linepos > ucit2.lit->m_RowIndices[0].m_Start &&     // not at begin/end of line
		ucit2.linepos < (ucit2.lit->m_Size - ucit2.lit->m_NewLineSize) &&
		!IsWordBoundary(ucit2))
	{
		return false;
	}

	// check ucit1
	wxFileOffset lpos = ucit1.lit->m_RowIndices[0].m_Start;
	if (ucit1.linepos > lpos &&        // not at begin of line
		!IsWordBoundary(ucit1))
	{
		return false;
	}

	return true;
}
bool TextSearcher::NextRegexSearchingPos(MadCaretPos& cp, const wxString &expr)
{
	if (expr.find_first_of(wxT('^')) != wxString::npos || expr.find_last_of(wxT('$')) != wxString::npos)
	{
		wxFileOffset len = cp.iter->m_Size - cp.linepos - 1;
		cp.pos += len;
		cp.linepos += len;
	}

	if (cp.pos == UCIterator::s_endpos)
		return false;

	UCIterator it(cp);
	++it;
	cp.AssignWith(it);
	return true;
}

MadSearchResult TextSearcher::Search(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
	const wxString &text)
{
	if (beginpos.pos >= endpos.pos || text.IsEmpty())
		return SR_NO;

	const wxString *text_ptr = &text;
	if (!m_case_sensitive)
	{
		static wxString text_ncase;
		text_ncase = wxm::WxStrToNormalCase(text);
		text_ptr = &text_ncase;
	}

	ucs4string exprstr(from_wxString(*text_ptr));

	UCIterator::Init(m_edit->m_Lines, endpos.pos);

	UCIterator start(beginpos);
	UCIterator end(endpos);
	bool found;

	UCIterator fbegin, fend;

	if (!SearchingPrepare(exprstr, text))
		return SR_EXPR_ERROR;

	for (;;)
	{
		if (!DoSearch(found, fbegin, fend, start, end, exprstr))
			return SR_EXPR_ERROR;


		if (!found)
			break;

		if (m_whole_word)
			found = IsWordBoundary(fbegin, fend);

		if (found)
		{
			beginpos.AssignWith(fbegin);
			endpos.AssignWith(fend);
			return SR_YES;
		}

		// not found, repeat...
		start = fend;
	}

	return SR_NO;
}

MadSearchResult TextSearcher::FindNext(const wxString &text,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	MadCaretPos bpos, epos;

	if (rangeFrom < 0)
		bpos = m_edit->m_CaretPos;
	else
		UpdateWXMEditCaret(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		UpdateWXMEditCaret(rangeTo, epos);

	MadSearchResult state = Search(bpos, epos, text);
	if (state == SR_YES)
		m_edit->SetSelection(bpos.pos, epos.pos);

	return state;
}

MadSearchResult TextSearcher::FindPrevious(const wxString &text,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	MadCaretPos bpos, epos;

	if (rangeFrom < 0)
		epos = m_edit->m_CaretPos;
	else
		UpdateWXMEditCaret(rangeFrom, epos);

	if (rangeTo < 0) rangeTo = 0;
	else if (rangeTo > m_edit->GetFileSize()) rangeTo = m_edit->GetFileSize();

	if (epos.pos <= rangeTo)
		return SR_NO;

	size_t searchsize = 50 * 1024;
	if (searchsize< text.Len() * 8) searchsize = text.Len() * 8;

	wxFileOffset startpos = epos.pos;

	for (;;)
	{
		startpos -= searchsize;
		if (startpos<rangeTo)
		{
			startpos = rangeTo;
		}
		else
		{
			size_t s = size_t(startpos);
			s = s % 4;
			startpos -= s;
		}

		wxFileOffset len = epos.pos - startpos;
		if (epos.linepos >= len)
		{
			bpos.pos = epos.pos - len;
			bpos.iter = epos.iter;
			bpos.linepos = epos.linepos - len;
		}
		else
		{
			bpos.pos = startpos;
			m_edit->GetLineByPos(bpos.iter, bpos.pos, bpos.rowid);
			bpos.linepos = 0;
			startpos = bpos.pos;
		}

		MadCaretPos bpos1 = bpos, epos1 = epos;
		int state = Search(bpos1, epos1, text);
		if (state == SR_EXPR_ERROR)
			return SR_EXPR_ERROR;

		if (state == SR_YES) // found
		{
			xm::UCQueue ucq;

			MadCaretPos bp, ep;
			do
			{
				bp = bpos1;
				ep = epos1;

				// advance one uchar
				m_edit->m_Lines->InitNextUChar(bpos1.iter, bpos1.linepos);
				if (!m_edit->m_Lines->NextUChar(ucq))
				{
					++bpos1.iter;
					bpos1.linepos = 0;
					m_edit->m_Lines->InitNextUChar(bpos1.iter, 0);
					m_edit->m_Lines->NextUChar(ucq);
				}
				bpos1.pos += ucq.back().nbytes();
				bpos1.linepos += ucq.back().nbytes();

				epos1 = epos;
			} while (Search(bpos1, epos1, text));

			m_edit->SetSelection(bp.pos, ep.pos, true);
			return SR_YES;
		}

		// not found
		if (startpos <= rangeTo)
			break;

		epos = bpos;
		size_t s = text.Len() * 4;

		if (wxFileOffset(epos.pos + s) > epos1.pos)
		{
			s = text.Len();
			if (wxFileOffset(epos.pos + s) > epos1.pos)
				s = 0;
		}

		if (s != 0)
		{
			epos.pos += s;
			do
			{
				len = epos.iter->m_Size - epos.linepos;
				if (len>(wxFileOffset)s) len = s;

				if ((epos.linepos += len) == epos.iter->m_Size)
				{
					epos.linepos = 0;
					epos.iter++;
				}
			} while ((s -= size_t(len)) > 0);
		}
	}

	return SR_NO;
}

MadReplaceResult TextSearcher::ReplaceOnce(const wxString &expr, const wxString &fmt,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return RR_NREP_NNEXT;

	bool selok = false;

	MadCaretPos bpos = *m_edit->m_SelectionBegin;
	MadCaretPos epos = *m_edit->m_SelectionEnd;

	if (m_edit->m_Selection) // test the selection is wanted text
	{
		int state = Search(bpos, epos, expr);
		if (state == SR_EXPR_ERROR)
			return RR_EXPR_ERROR;

		if (state == SR_YES && bpos.pos == m_edit->m_SelectionBegin->pos && epos.pos == m_edit->m_SelectionEnd->pos)
			selok = true;
	}

	if (!selok)  // just find next
	{
		switch (FindNext(expr, rangeFrom, rangeTo))
		{
		case SR_EXPR_ERROR: return RR_EXPR_ERROR;
		case SR_YES:        return RR_NREP_NEXT;
		case SR_NO:         return RR_NREP_NNEXT;
		}
	}

	ucs4string out;

	// replace the selected text
	int state = Replace(out, bpos, epos, expr, fmt);
	if (state == SR_EXPR_ERROR)
		return RR_EXPR_ERROR;

	if (out.length() == 0)
		m_edit->DeleteSelection(true, nullptr, false);
	else
		m_edit->InsertString(out.c_str(), out.length(), false, true, false);

	if (SR_NO == FindNext(expr, -1, rangeTo))
		return RR_REP_NNEXT;

	return RR_REP_NEXT;
}

int TextSearcher::FindAll(const wxString &expr, bool bFirstOnly,
	vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return 0;

	MadCaretPos bpos, epos, endpos;

	if (rangeFrom <= 0)
		AssignTextFileBegin(bpos);
	else
		UpdateWXMEditCaret(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		UpdateWXMEditCaret(rangeTo, epos);

	if (bpos.pos >= epos.pos)
		return 0;

	endpos = epos;
	int count = 0;
	int state;

	while ((state = Search(bpos, epos, expr)) == SR_YES)
	{
		if (pbegpos) pbegpos->push_back(bpos.pos);
		if (pendpos) pendpos->push_back(epos.pos);
		++count;
		if (bFirstOnly)
			break;

		if (bpos.pos == epos.pos && !NextRegexSearchingPos(epos, expr))
			break;

		bpos = epos;
		epos = endpos;
	}

	if (state == SR_EXPR_ERROR)
		return SR_EXPR_ERROR;

	return count;
}

int TextSearcher::ReplaceAll(const wxString &expr, const wxString &fmt,
	vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return 0;

	MadCaretPos bpos, epos, endpos;

	if (rangeFrom <= 0)
		AssignTextFileBegin(bpos);
	else
		UpdateWXMEditCaret(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		UpdateWXMEditCaret(rangeTo, epos);

	if (bpos.pos >= epos.pos)
		return 0;

	endpos = epos;
	int multi = 0;

	vector<wxFileOffset> del_bpos;
	vector<wxFileOffset> del_epos;
	vector<const ucs4_t*> ins_ucs;
	vector<wxFileOffset> ins_len;

	list<ucs4string> outs;

	int state;
	while ((state = Search(bpos, epos, expr)) == SR_YES)
	{
		ucs4string out;
		int state = Replace(out, bpos, epos, expr, fmt);
		if (state == SR_EXPR_ERROR)
			return SR_EXPR_ERROR;

		del_bpos.push_back(bpos.pos);
		del_epos.push_back(epos.pos);

		outs.push_back(out);
		ucs4string &str = outs.back();
		ins_ucs.push_back(str.c_str());
		ins_len.push_back(str.length());

		if (bpos.iter != epos.iter)
			++multi;

		if (bpos.pos == epos.pos && !NextRegexSearchingPos(epos, expr))
			break;

		bpos = epos;
		epos = endpos;
	}

	if (state == SR_EXPR_ERROR)
		return SR_EXPR_ERROR;

	int count = int(del_bpos.size());

	if (count>0)
	{
		wxFileOffset size = del_epos.back() - del_bpos.front();
		if ((size <= 2 * 1024 * 1024) || (multi >= 40 && size <= 10 * 1024 * 1024))
			m_edit->OverwriteDataSingle(del_bpos, del_epos, &ins_ucs, nullptr, ins_len);
		else
			m_edit->OverwriteDataMultiple(del_bpos, del_epos, &ins_ucs, nullptr, ins_len);

		if (pbegpos != 0 && pendpos != 0)
		{
			pbegpos->resize(count);
			pendpos->resize(count);

			wxFileOffset diff = 0, b, e, l;
			for (int i = 0; i<count; i++)
			{
				b = del_bpos[i];
				e = del_epos[i];
				l = ins_len[i];
				size = b + diff;
				(*pbegpos)[i] = size;
				(*pendpos)[i] = size + l;
				diff += (l - (e - b));
			}
		}
	}

	return count;
}

void TextSearcher::UpdateWXMEditCaret(MadCaretPos& cp)
{
	xm::UCQueue dummyUCQueue;
	vector<int> dummyWidthArray;
	int dummyUCPos;

	m_edit->UpdateCaretByPos(cp, dummyUCQueue, dummyWidthArray, dummyUCPos);
}

void TextSearcher::UpdateWXMEditCaret(wxFileOffset& pos, MadCaretPos& cp)
{
	if (pos > m_edit->GetFileSize())
		pos = m_edit->GetFileSize();
	cp.pos = pos;

	UpdateWXMEditCaret(cp);
}

static JumpTable_UCS4 sg_jtab;
bool StringSearcher::SearchingPrepare(const ucs4string& exprstr, const wxString& text)
{
	sg_jtab.Build(exprstr);
	return true;
}

bool StringSearcher::DoSearch(bool& found, UCIterator& fbegin, UCIterator& fend,
	const UCIterator& start, const UCIterator& end, const ucs4string& exprstr)
{
	fbegin = start;
	fend = end;
	found = ::Search(fbegin, fend, exprstr, sg_jtab, m_case_sensitive);
	return true;
}

static basic_regex<UCIterator> sg_expression;
bool RegexSearcher::SearchingPrepare(const ucs4string& exprstr, const wxString& text)
{
	try
	{
		regex_constants::syntax_option_type opt = regex_constants::ECMAScript;
		if (!m_case_sensitive)
			opt = opt | regex_constants::icase;
		if (!g_regex_dot_match_newline)
			opt = opt | regex_constants::not_dot_newline;
		regex_compiler<UCIterator, ucs4_regex_traits > ucs4comp;
		sg_expression = ucs4comp.compile(exprstr, opt);
	}
	catch (regex_error)
	{
		wxMessageDialog dlg(m_edit, wxString::Format(_("'%s' is not a valid regular expression."), text.c_str()),
			wxT("wxMEdit"), wxOK | wxICON_ERROR);
		dlg.ShowModal();
		return false;
	}

	return true;
}

bool RegexSearcher::DoSearch(bool& found, UCIterator& fbegin, UCIterator& fend,
	const UCIterator& start, const UCIterator& end, const ucs4string& exprstr)
{
	match_results<UCIterator> what;
	try
	{
		found = regex_search(start, end, what, sg_expression);
	}
	catch (regex_error)
	{
		wxMessageDialog dlg(m_edit, _("Catched a exception of 'regex_error'.\nMaybe the regular expression is invalid."),
			wxT("wxMEdit"), wxOK | wxICON_ERROR);
		dlg.ShowModal();
		return false;
	}

	if (found)
	{
		fbegin = what[0].first;
		fend = what[0].second;
	}

	return true;
}

MadSearchResult StringSearcher::Replace(ucs4string &out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
	const wxString &expr, const wxString &fmt)
{
	if (expr.empty())
		return SR_NO;

	// fmt is the wanted string
	out += from_wxString(fmt);

	return SR_YES;
}

MadSearchResult RegexSearcher::Replace(ucs4string &out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
	const wxString &expr, const wxString &fmt)
{
	ucs4string exprstr(from_wxString(expr));

	if (!SearchingPrepare(exprstr, expr))
		return SR_EXPR_ERROR;

	ucs4string fmtstr(from_wxString(fmt));

	UCIterator begin(beginpos);
	UCIterator end(endpos);

	try
	{
		std::back_insert_iterator<ucs4string> oi(out);
		regex_replace(oi, begin, end, sg_expression, fmtstr);
		out = ConvertEscape(out);
	}
	catch (regex_error)
	{
		wxMessageDialog dlg(m_edit, wxString::Format(_("The format of '%s' is invalid."), fmt.c_str()),
			wxT("wxMEdit"), wxOK | wxICON_ERROR);
		dlg.ShowModal();
		return SR_EXPR_ERROR;
	}

	return SR_YES;
}

MadSearchResult HexSearcher::SearchHex(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
	const std::vector<wxByte>& hex)
{
	if (beginpos.pos >= endpos.pos || hex.empty())
		return SR_NO;

	ByteIterator::Init(m_edit->m_Lines, endpos.pos);
	ByteIterator start(beginpos);
	ByteIterator end(endpos);

	static JumpTable_Hex jtab;
	jtab.Build(hex);

	if (!::Search(start, end, hex, jtab, true))
		return SR_NO;

	beginpos.AssignWith(start);
	endpos.AssignWith(end);

	return SR_YES;
}

MadSearchResult HexSearcher::FindNext(const wxString &hexstr,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	vector<wxByte> hex;
	if (!StringToHex(hexstr, hex))
		return SR_EXPR_ERROR;

	MadCaretPos bpos, epos;

	if (rangeFrom < 0)
		bpos = m_edit->m_CaretPos;
	else
		AssignCaretPos(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		AssignCaretPos(rangeTo, epos);

	if (SearchHex(bpos, epos, hex) != SR_YES)
		return SR_NO;

	m_edit->SetSelection(bpos.pos, epos.pos);
	return SR_YES;
}

MadSearchResult HexSearcher::FindPrevious(const wxString &hexstr,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	vector<wxByte> hex;
	if (!StringToHex(hexstr, hex))
		return SR_EXPR_ERROR;

	MadCaretPos bpos, epos;

	if (rangeFrom < 0)
		epos = m_edit->m_CaretPos;
	else
		AssignCaretPos(rangeFrom, epos);

	if (rangeTo < 0) rangeTo = 0;
	else if (rangeTo > m_edit->GetFileSize()) rangeTo = m_edit->GetFileSize();

	if (epos.pos <= rangeTo)
		return SR_NO;

	size_t searchsize = 50 * 1024;
	if (searchsize< hexstr.Len() * 8) searchsize = hexstr.Len() * 8;

	wxFileOffset startpos = epos.pos;

	for (;;)
	{
		startpos -= searchsize;
		if (startpos<rangeTo)
		{
			startpos = rangeTo;
		}
		else
		{
			size_t s = size_t(startpos);
			s = s % 4;
			startpos -= s;
		}

		wxFileOffset len = epos.pos - startpos;
		if (epos.linepos >= len)
		{
			bpos.pos = epos.pos - len;
			bpos.iter = epos.iter;
			bpos.linepos = epos.linepos - len;
		}
		else
		{
			bpos.pos = startpos;
			m_edit->GetLineByPos(bpos.iter, bpos.pos, bpos.rowid);
			bpos.linepos = 0;
			startpos = bpos.pos;
		}

		MadCaretPos bpos1 = bpos, epos1 = epos;
		if (SR_YES == SearchHex(bpos1, epos1, hex)) // found
		{
			MadCaretPos bp, ep;
			do
			{
				bp = bpos1;
				ep = epos1;

				// advance one byte
				if (bpos1.linepos == bpos1.iter->m_Size)
				{
					++bpos1.iter;
					bpos1.linepos = 0;
				}
				bpos1.pos++;
				bpos1.linepos++;

				epos1 = epos;
			} while (SearchHex(bpos1, epos1, hex));

			m_edit->SetSelection(bp.pos, ep.pos, true);
			return SR_YES;
		}

		// not found
		if (startpos <= rangeTo)
			break;

		epos = bpos;
		size_t s = hexstr.Len() * 4;

		if (wxFileOffset(epos.pos + s) > epos1.pos)
		{
			s = hexstr.Len();
			if (wxFileOffset(epos.pos + s) > epos1.pos)
				s = 0;
		}

		if (s != 0)
		{
			epos.pos += s;
			do
			{
				len = epos.iter->m_Size - epos.linepos;
				if (len>(wxFileOffset)s) len = s;

				if ((epos.linepos += len) == epos.iter->m_Size)
				{
					epos.linepos = 0;
					epos.iter++;
				}
			} while ((s -= size_t(len)) > 0);
		}
	}

	return SR_NO;
}

MadReplaceResult HexSearcher::ReplaceOnce(const wxString &expr, const wxString &fmt,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return RR_NREP_NNEXT;

	bool selok = false;

	vector<wxByte> hex;
	if (!StringToHex(expr, hex))
		return RR_EXPR_ERROR;

	vector<wxByte> fmthex;
	if (!StringToHex(fmt, fmthex))
		return RR_EXPR_ERROR;


	MadCaretPos bpos = *m_edit->m_SelectionBegin;
	MadCaretPos epos = *m_edit->m_SelectionEnd;

	if (m_edit->m_Selection) // test the selection is wanted text
	{
		int state = SearchHex(bpos, epos, hex);
		if (state == SR_EXPR_ERROR)
			return RR_EXPR_ERROR;

		if (state == SR_YES && bpos.pos == m_edit->m_SelectionBegin->pos && epos.pos == m_edit->m_SelectionEnd->pos)
		{
			selok = true;
		}
	}

	if (!selok)  // just find next
	{
		switch (FindNext(expr, rangeFrom, rangeTo))
		{
		case SR_EXPR_ERROR: return RR_EXPR_ERROR;
		case SR_YES:        return RR_NREP_NEXT;
		case SR_NO:         return RR_NREP_NNEXT;
		}
	}

	if (fmthex.size() == 0)
		m_edit->DeleteSelection(true, nullptr, false);
	else
		m_edit->InsertRawBytes(&fmthex[0], fmthex.size(), false);

	if (SR_NO == FindNext(expr, -1, rangeTo))
		return RR_REP_NNEXT;

	return RR_REP_NEXT;
}

int HexSearcher::ReplaceAll(const wxString &expr, const wxString &fmt,
	vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return 0;

	vector<wxByte> hex;
	if (!StringToHex(expr, hex))
		return SR_EXPR_ERROR;

	vector<wxByte> fmthex;
	if (!StringToHex(fmt, fmthex))
		return SR_EXPR_ERROR;

	vector<wxFileOffset> del_bpos;
	vector<wxFileOffset> del_epos;
	vector<wxByte*> ins_data;
	vector<wxFileOffset> ins_len;

	MadCaretPos bpos, epos, endpos;

	if (rangeFrom <= 0)
		AssignHexFileBegin(bpos);
	else
		AssignCaretPos(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		AssignCaretPos(rangeTo, epos);

	if (bpos.pos >= epos.pos)
		return 0;

	endpos = epos;
	int multi = 0;

	while (SearchHex(bpos, epos, hex) == SR_YES)
	{
		del_bpos.push_back(bpos.pos);
		del_epos.push_back(epos.pos);
		ins_data.push_back(fmthex.empty() ? nullptr : &fmthex[0]);
		ins_len.push_back(fmthex.size());

		if (bpos.iter != epos.iter)
			++multi;

		bpos = epos;
		epos = endpos;
	}

	int count = int(del_bpos.size());

	if (count>0)
	{
		wxFileOffset size = del_epos.back() - del_bpos.front();
		if (m_edit->IsTextFile() && ((size <= 2 * 1024 * 1024) || (multi >= 40 && size <= 10 * 1024 * 1024)))
			m_edit->OverwriteDataSingle(del_bpos, del_epos, nullptr, &ins_data, ins_len);
		else
			m_edit->OverwriteDataMultiple(del_bpos, del_epos, nullptr, &ins_data, ins_len);

		if (pbegpos != 0 && pendpos != 0)
		{
			pbegpos->resize(count);
			pendpos->resize(count);

			wxFileOffset diff = 0, b, e, l;
			for (int i = 0; i<count; i++)
			{
				b = del_bpos[i];
				e = del_epos[i];
				l = ins_len[i];
				size = b + diff;
				(*pbegpos)[i] = size;
				(*pendpos)[i] = size + l;
				diff += (l - (e - b));
			}
		}
	}

	return count;
}

int HexSearcher::FindAll(const wxString &expr, bool bFirstOnly,
	vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos,
	wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
	if (expr.empty())
		return 0;

	vector<wxByte> hex;
	if (!StringToHex(expr, hex))
		return SR_EXPR_ERROR;

	MadCaretPos bpos, epos, endpos;

	if (rangeFrom <= 0)
		AssignHexFileBegin(bpos);
	else
		AssignCaretPos(rangeFrom, bpos);

	if (rangeTo < 0)
		AssignFileEnd(epos);
	else
		AssignCaretPos(rangeTo, epos);

	if (bpos.pos >= epos.pos)
		return 0;

	endpos = epos;
	int count = 0;

	while (SearchHex(bpos, epos, hex) == SR_YES)
	{
		if (pbegpos) pbegpos->push_back(bpos.pos);
		if (pendpos) pendpos->push_back(epos.pos);
		++count;
		if (bFirstOnly)
			break;

		bpos = epos;
		epos = endpos;
	}

	return count;
}

} // namespace wxm
