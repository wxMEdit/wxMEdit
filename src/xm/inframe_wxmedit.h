///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/inframe_wxmedit.h
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _INFRAME_WXMEDIT_H_
#define _INFRAME_WXMEDIT_H_

#include "simple_wxmedit.h"

namespace wxm
{

struct InFrameWXMEdit : public MadEdit
{
	virtual void SetWordWrapMode(MadWordWrapMode mode);
	virtual MadWordWrapMode GetWordWrapMode() { return m_WordWrapMode; }

	virtual bool HasLineNumber() { return m_has_linenum; }
	virtual void SetShowLineNumber(bool value);

	virtual LineNumberList SaveBookmarkLineNumberList();
	virtual void RestoreBookmarkByLineNumberList(const LineNumberList& linenums);

	virtual void BeginPrint(const wxRect &printRect);
	virtual bool PrintPage(wxDC *dc, int pageNum);
	virtual void EndPrint();

	void ToggleBookmark();
	void GotoNextBookmark();
	void GotoPreviousBookmark();
	void ClearAllBookmarks();
	bool BookmarkExist() { return m_Lines->m_LineList.BookmarkExist(); }

	InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
private:
	virtual void DoSelectionChanged();
	virtual void DoStatusChanged();
	virtual void DoToggleWindow();
	virtual void DoMouseRightUp();

	virtual int CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount);
	virtual int GetLineNumberAreaWidth(int number);

	virtual void OnPaintInPrinting(wxPaintDC& dc, wxMemoryDC& memdc);

	void BeginTextPrinting();
	void BeginHexPrinting();
	void PrintTextPage(wxDC *dc, int pageNum);
	void PrintHexPage(wxDC *dc, int pageNum);
	void EndTextPrinting();
	void EndHexPrinting();

	bool            m_has_linenum;
	MadWordWrapMode m_WordWrapMode;

	int             m_old_ClientWidth;
	int             m_old_ClientHeight;
	MadWordWrapMode m_old_WordWrapMode;
	bool            m_old_Selection;
	bool            m_old_has_linenum;
	bool            m_old_ShowEndOfLine, m_old_ShowSpaceChar, m_old_ShowTabChar;
	int             m_old_LeftMarginWidth;
	int             m_old_DrawingXPos;
	bool            m_old_CaretAtHexArea;

	// temporary wxMEdit to print Hex-Data
	HexPrintingWXMEdit* m_HexPrintWXMEdit;

	wxRect m_PrintRect;
	bool m_PrintSyntax;
	int m_RowCountPerPage;
	int m_RowCountPerHexLine;
	int m_HexLineCountPerPage;
	int m_PrintOffsetHeader;
	int m_PrintTotalHexLineCount;
};

} //namespace wxm

#endif //_INFRAME_WXMEDIT_H_
