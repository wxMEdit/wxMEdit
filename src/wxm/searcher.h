///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/searcher.h
// Description: wxMEdit Searching and Replacing Functions
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_SEARCHER_H_
#define _WXM_SEARCHER_H_

#include "../xm/cxx11.h"
#include "def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/string.h>
#include <wx/filefn.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <vector>
#include <string>

class MadEdit;
struct MadCaretPos;
struct UCIterator;

// returned state of Search & Replace
enum MadSearchResult
{
	SR_EXPR_ERROR = -2, SR_YES = -1, SR_NO = 0
};

enum MadReplaceResult
{
	RR_EXPR_ERROR = -1,
	RR_NREP_NNEXT = 0, // not replaced, not found next
	RR_REP_NNEXT = 1,  // replaced, not found next
	RR_NREP_NEXT = 2,  // not replaced, found next
	RR_REP_NEXT = 3    // replaced, found next
};

namespace wxm
{

	struct WXMSearcher
	{
		WXMSearcher(MadEdit* edit, bool use_regex): m_edit(edit), m_use_regex(use_regex)
			, m_case_sensitive(true), m_whole_word(false)
		{}
		virtual ~WXMSearcher() {}

		virtual void SetOption(bool case_sensitive, bool whole_word)
		{
			m_case_sensitive = case_sensitive;
			m_whole_word = whole_word;
		}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		virtual MadSearchResult FindNext(const wxString& text,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) = 0;
		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		virtual MadSearchResult FindPrevious(const wxString& text,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) = 0;

		// replace the selected text that must match expr
		virtual MadReplaceResult ReplaceOnce(const wxString& expr, const wxString& fmt,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) = 0;

		// return the replaced count or SR_EXPR_ERROR
		virtual int ReplaceAll(const wxString& expr, const wxString& fmt,
			std::vector<wxFileOffset>* pbegpos = nullptr, std::vector<wxFileOffset>* pendpos = nullptr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) = 0;

		// list the matched data to pbegpos & pendpos
		// return the found count or SR_EXPR_ERROR
		virtual int FindAll(const wxString& expr, bool bFirstOnly,
			std::vector<wxFileOffset>* pbegpos, std::vector<wxFileOffset>* pendpos,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) = 0;

	protected:
		ucs4string from_wxString(const wxString& wxs);

		void AssignTextFileBegin(MadCaretPos& cp);
		void AssignHexFileBegin(MadCaretPos& cp);
		void AssignFileEnd(MadCaretPos& cp);
		void AssignCaretPos(wxFileOffset& pos, MadCaretPos& cp);

		MadEdit* m_edit;
		bool m_use_regex;
		bool m_case_sensitive;
		bool m_whole_word;
	};

	struct TextSearcher : public WXMSearcher
	{
		TextSearcher(MadEdit* edit, bool use_regex) : WXMSearcher(edit, use_regex) {}
	protected:
		virtual MadSearchResult FindNext(const wxString& hexstr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual MadSearchResult FindPrevious(const wxString& hexstr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual MadReplaceResult ReplaceOnce(const wxString& expr, const wxString& fmt,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual int ReplaceAll(const wxString& expr, const wxString& fmt,
			std::vector<wxFileOffset>* pbegpos = nullptr, std::vector<wxFileOffset>* pendpos = nullptr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual int FindAll(const wxString& expr, bool bFirstOnly,
			std::vector<wxFileOffset>* pbegpos, std::vector<wxFileOffset>* pendpos,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;

		void UpdateWXMEditCaret(MadCaretPos& cp);
		void UpdateWXMEditCaret(wxFileOffset& pos, MadCaretPos& cp);

		virtual bool SearchingPrepare(const ucs4string& exprstr, const wxString& text) = 0;
		virtual bool DoSearch(bool& found, UCIterator& fbegin, UCIterator& fend,
			const UCIterator& start, const UCIterator& end, const ucs4string& exprstr) = 0;

		MadSearchResult Search(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
			const wxString& text);

		virtual MadSearchResult Replace(/*OUT*/ucs4string& out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
			const wxString& expr, const wxString& fmt) = 0;

		bool IsDelimiterChar(ucs4_t uc);
		bool IsWordBoundary(UCIterator& ucit);
		bool IsWordBoundary(UCIterator ucit1, UCIterator ucit2);

		bool NextRegexSearchingPos(MadCaretPos& cp, const wxString &expr);
	};

	struct HexSearcher: public WXMSearcher
	{
		HexSearcher(MadEdit* edit) : WXMSearcher(edit, false) {}
	private:
		MadSearchResult SearchHex(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
			const std::vector<wxByte>& hex);

		virtual MadSearchResult FindNext(const wxString& hexstr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual MadSearchResult FindPrevious(const wxString& hexstr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual MadReplaceResult ReplaceOnce(const wxString& expr, const wxString& fmt,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual int ReplaceAll(const wxString& expr, const wxString& fmt,
			std::vector<wxFileOffset>* pbegpos = nullptr, std::vector<wxFileOffset>* pendpos = nullptr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;
		virtual int FindAll(const wxString& expr, bool bFirstOnly,
			std::vector<wxFileOffset>* pbegpos, std::vector<wxFileOffset>* pendpos,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1) override;

		virtual void SetOption(bool case_sensitive, bool whole_word) override {}
	};

	struct StringSearcher : public TextSearcher
	{
		StringSearcher(MadEdit* edit) : TextSearcher(edit, false) {}
	private:
		virtual MadSearchResult Replace(/*OUT*/ucs4string& out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
			const wxString& expr, const wxString& fmt) override;
		virtual bool SearchingPrepare(const ucs4string& exprstr, const wxString& text) override;
		virtual bool DoSearch(bool& found, UCIterator& fbegin, UCIterator& fend,
			const UCIterator& start, const UCIterator& end, const ucs4string& exprstr) override;
	};

	struct RegexSearcher : public TextSearcher
	{
		RegexSearcher(MadEdit* edit) : TextSearcher(edit, true) {}
	private:
		virtual MadSearchResult Replace(/*OUT*/ucs4string& out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
			const wxString& expr, const wxString& fmt) override;
		virtual bool SearchingPrepare(const ucs4string& exprstr, const wxString& text) override;
		virtual bool DoSearch(bool& found, UCIterator& fbegin, UCIterator& fend,
			const UCIterator& start, const UCIterator& end, const ucs4string& exprstr) override;
	};

	struct AutoSearcher
	{
		WXMSearcher* Searcher(bool inhex, bool use_regex)
		{
			if (inhex)
				return reinterpret_cast<WXMSearcher*>(&m_hex_searcher);

			if (use_regex)
				return reinterpret_cast<WXMSearcher*>(&m_regex_searcher);

			return reinterpret_cast<WXMSearcher*>(&m_str_searcher);
		}

		AutoSearcher(MadEdit* edit) : m_str_searcher(edit), m_regex_searcher(edit), m_hex_searcher(edit) {}
	private:
		StringSearcher m_str_searcher;
		RegexSearcher m_regex_searcher;
		HexSearcher m_hex_searcher;
	};
} //namespace wxm

#endif //_WXM_SEARCHER_H_
