///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/searcher.h
// Description: wxMEdit Searching and Replacing Functions
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_SEARCHER_H_
#define _WXM_SEARCHER_H_

#include "../wxmedit/ucs4_t.h"
#include "../xm/cxx11.h"

#include <wx/string.h>
#include <wx/filefn.h>
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

typedef std::basic_string<ucs4_t> ucs4string;

namespace wxm
{

	struct WXMSearcher
	{
		WXMSearcher(MadEdit* edit) : m_edit(edit) {}

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		MadSearchResult FindTextNext(const wxString& text,
			bool bRegex, bool bCaseSensitive, bool bWholeWord,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);
		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		MadSearchResult FindTextPrevious(const wxString& text,
			bool bRegex, bool bCaseSensitive, bool bWholeWord,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);

		// search in [rangeFrom, rangeTo], default in [CaretPos, EndOfDoc]
		MadSearchResult FindHexNext(const wxString& hexstr,
			wxFileOffset rangeFrom = -1,
			wxFileOffset rangeTo = -1);
		// search in [rangeFrom, rangeTo], rangeFrom > rangeTo, default in [CaretPos, BeginOfDoc]
		MadSearchResult FindHexPrevious(const wxString& hexstr,
			wxFileOffset rangeFrom = -1,
			wxFileOffset rangeTo = -1);

		// replace the selected text that must match expr
		MadReplaceResult ReplaceTextOnce(const wxString& expr, const wxString& fmt,
			bool bRegex, bool bCaseSensitive, bool bWholeWord,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);
		MadReplaceResult ReplaceHexOnce(const wxString& expr, const wxString& fmt,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);

		// return the replaced count or SR_EXPR_ERROR
		int ReplaceTextAll(const wxString& expr, const wxString& fmt,
			bool bRegex, bool bCaseSensitive, bool bWholeWord,
			std::vector<wxFileOffset>* pbegpos = nullptr, std::vector<wxFileOffset>* pendpos = nullptr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);
		int ReplaceHexAll(const wxString &expr, const wxString &fmt,
			std::vector<wxFileOffset>* pbegpos = nullptr, std::vector<wxFileOffset>* pendpos = nullptr,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);

		// list the matched data to pbegpos & pendpos
		// return the found count or SR_EXPR_ERROR
		int FindTextAll(const wxString& expr,
			bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bFirstOnly,
			std::vector<wxFileOffset>* pbegpos, std::vector<wxFileOffset>* pendpos,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);
		int FindHexAll(const wxString &expr, bool bFirstOnly,
			std::vector<wxFileOffset>* pbegpos, std::vector<wxFileOffset>* pendpos,
			wxFileOffset rangeFrom = -1, wxFileOffset rangeTo = -1);

	private:
		MadSearchResult Search(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
			const wxString& text, bool bRegex, bool bCaseSensitive, bool bWholeWord);

		MadSearchResult SearchHex(/*IN_OUT*/MadCaretPos& beginpos, /*IN_OUT*/MadCaretPos& endpos,
			const std::vector<wxByte>& hex);

		MadSearchResult Replace(/*OUT*/ucs4string& out, const MadCaretPos& beginpos, const MadCaretPos& endpos,
			const wxString& expr, const wxString& fmt,
			bool bRegex, bool bCaseSensitive, bool bWholeWord);

		void UpdateWXMEditCaret(MadCaretPos& cp);
		void UpdateWXMEditCaret(wxFileOffset& pos, MadCaretPos& cp);
		void AssignTextFileBegin(MadCaretPos& cp);
		void AssignHexFileBegin(MadCaretPos& cp);
		void AssignFileEnd(MadCaretPos& cp);
		void AssignCaretPos(wxFileOffset& pos, MadCaretPos& cp);

		bool IsDelimiterChar(ucs4_t uc);
		bool IsWordBoundary(UCIterator& ucit);
		bool IsWordBoundary(UCIterator& ucit1, UCIterator& ucit2);

		bool NextRegexSearchingPos(MadCaretPos& cp, const wxString &expr);

		MadEdit* m_edit;
	};

} //namespace wxm

#endif //_WXM_SEARCHER_H_
