///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/simple.h
// Description: Simple wxMEdit Implementations
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_SIMPLE_H_
#define _WXMEDIT_SIMPLE_H_

#include "../../xm/cxx11.h"
#include "../../wxmedit/wxmedit.h"
#include "../searcher.h"

namespace wxm
{

struct DummyConfigWriter: public ConfigWriter
{
	virtual void Record(const wxString& key, long val) override {}
	virtual void Record(const wxString& key, double val) override {}
	virtual void Record(const wxString& key, const wxString& val) override {}

	virtual void SetConfig(wxConfigBase* cfg) override {}
};

struct SimpleWXMEdit: public MadEdit
{
	SimpleWXMEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER | wxWANTS_CHARS)
		: MadEdit(new DummyConfigWriter(),  parent, id, pos, size, style)
	{
	}
private:
	virtual int CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount) override { return 0; }
	virtual int GetLineNumberAreaWidth(int number) override { return 0; }
	virtual void OnPaintInPrinting(wxPaintDC& dc, wxMemoryDC& memdc) override {}
	virtual LineNumberList SaveBookmarkLineNumberList() override { return LineNumberList(); }
	virtual void RestoreBookmarkByLineNumberList(const LineNumberList& linenums) override {}
	virtual int CachedLineNumberAreaWidth() override { return 0; }
	virtual void CacheLineNumberAreaWidth(int width) override {}
	virtual void PaintLineNumberArea(const wxColor & bgcolor, wxDC * dc, const wxRect& rect, bool is_trailing_subrow,
		MadLineIterator lineiter, int lineid, int text_top) override {}
	virtual WXMSearcher* Searcher(bool inhex, bool use_regex) override { return nullptr; }
	virtual bool ManuallyCancelHexToText() override { return false; }
};

struct InFrameWXMEdit;
struct HexPrintingWXMEdit : public SimpleWXMEdit
{
	friend struct InFrameWXMEdit;
	HexPrintingWXMEdit(wxWindow* parent, const wxFont* font);
private:
	virtual void SetWordWrapMode(MadWordWrapMode mode) override {}
	virtual MadWordWrapMode GetWordWrapMode() override { return wwmWrapByColumn; }
};

struct SimpleTextWXMEdit: public SimpleWXMEdit
{
	SimpleTextWXMEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER | wxWANTS_CHARS)
		: SimpleWXMEdit(parent, id, pos, size, style)
	{
		InitTextFont();
		SetFixedWidthMode(false);
	}
private:
	virtual void SetWordWrapMode(MadWordWrapMode mode) override {}
	virtual MadWordWrapMode GetWordWrapMode() override { return wwmNoWrap; }
};

struct SearchingWXMEdit : public SimpleTextWXMEdit
{
	SearchingWXMEdit(wxWindow* parent, bool bSearchWholeWord)
		: SimpleTextWXMEdit(parent, wxID_ANY, wxPoint(-1024, -1024)), m_auto_searcher(this)
	{
		InitHexFont();
		StopRepaint();
		SetSearchOptions(true, bSearchWholeWord);
	}

	virtual WXMSearcher* Searcher(bool inhex, bool use_regex) override
	{
		return m_auto_searcher.Searcher(inhex, use_regex);
	}
private:
	AutoSearcher m_auto_searcher;
};

} //namespace wxm

#endif //_WXMEDIT_SIMPLE_H_
