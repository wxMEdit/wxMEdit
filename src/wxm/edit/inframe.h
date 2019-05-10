///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/inframe.h
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_INFRAME_H_
#define _WXMEDIT_INFRAME_H_

#include "simple.h"

#include <unicode/numfmt.h>
#include <boost/scoped_ptr.hpp>

namespace wxm
{

struct InFrameWXMEdit : public MadEdit
{
	virtual void SetWordWrapMode(MadWordWrapMode mode) override;
	virtual MadWordWrapMode GetWordWrapMode() override { return m_WordWrapMode; }

	void TrimTrailingSpaces();
	void InsertEnumeration();
	void ColumnAlign();
	void ColumnPaste();

	virtual bool BookmarkVisible() { return m_bookmark_visible; }
	virtual void SetBookmarkVisible(bool visible);

	virtual LineNumberList SaveBookmarkLineNumberList() override;
	virtual void RestoreBookmarkByLineNumberList(const LineNumberList& linenums) override;

	void BeginPrint(const wxRect &printRect);
	bool PrintPage(wxDC *dc, int pageNum);
	void EndPrint();
	int  GetPrintPageCount() { return m_PrintPageCount; }

	bool LineNumberVisible() { return m_linenum_visible; }
	void SetLineNumberVisible(bool value);

	void ToggleBookmark();
	void GotoNextBookmark();
	void GotoPreviousBookmark();
	void ClearAllBookmarks();
	bool BookmarkExist() { return m_Lines->m_LineList.BookmarkExist(); }

	virtual WXMSearcher* Searcher(bool inhex, bool use_regex) override
	{
		return m_auto_searcher.Searcher(inhex, use_regex);
	}


	bool Reload();
	// if the file is modified by another app, reload it.
	bool ReloadByModificationTime();

	InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
private:
	virtual void DoSelectionChanged() override;
	virtual void DoStatusChanged() override;
	virtual void DoToggleWindow() override;
	virtual void DoMouseRightUp() override;

	virtual int CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount) override;
	virtual int GetLineNumberAreaWidth(int number) override;
	virtual int CachedLineNumberAreaWidth() override { return m_LineNumberAreaWidth; }
	virtual void CacheLineNumberAreaWidth(int width) override { m_LineNumberAreaWidth = width; }

	virtual void PaintLineNumberArea(const wxColor & bgcolor, wxDC * dc, const wxRect& rect, bool is_trailing_subrow, MadLineIterator lineiter, int lineid, int text_top) override;

	virtual void OnPaintInPrinting(wxPaintDC& dc, wxMemoryDC& memdc) override;
	virtual bool ManuallyCancelHexToText() override;

	void BeginTextPrinting();
	void BeginHexPrinting();
	void PrintTextPage(wxDC *dc, int pageNum);
	void PrintHexPage(wxDC *dc, int pageNum);
	void EndTextPrinting();
	void EndHexPrinting();

	bool LineNumberAreaVisible() { return m_linenum_visible || m_bookmark_visible; }

	wxString FormattedNumber(int64_t num);

	int             m_LineNumberAreaWidth;
	bool            m_linenum_visible;
	bool            m_bookmark_visible;
	MadWordWrapMode m_WordWrapMode;

	int             m_old_ClientWidth;
	int             m_old_ClientHeight;
	MadWordWrapMode m_old_WordWrapMode;
	bool            m_old_Selection;
	bool            m_old_linenum_visible;
	bool            m_old_ShowEndOfLine, m_old_ShowSpaceChar, m_old_ShowTabChar;
	int             m_old_LeftMarginWidth;
	int             m_old_DrawingXPos;
	bool            m_old_CaretAtHexArea;

	// temporary wxMEdit to print Hex-Data
	HexPrintingWXMEdit* m_HexPrintWXMEdit;

	int m_PrintPageCount;
	wxRect m_PrintRect;
	bool m_PrintSyntax;
	int m_RowCountPerPage;
	int m_RowCountPerHexLine;
	int m_HexLineCountPerPage;
	int m_PrintOffsetHeader;
	int m_PrintTotalHexLineCount;

	AutoSearcher m_auto_searcher;
	boost::scoped_ptr <icu::NumberFormat> m_numfmt;
};

} //namespace wxm

#endif //_WXMEDIT_INFRAME_H_
