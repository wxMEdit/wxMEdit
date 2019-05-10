///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/single_line.h
// Description: Single Line wxMEdit Control
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_SINGLE_LINE_H_
#define _WXMEDIT_SINGLE_LINE_H_

#include "../../xm/cxx11.h"
#include "simple.h"

namespace wxm
{

class SingleLineWXMEdit: public SimpleTextWXMEdit
{
	virtual void UpdateScrollBarPos() override;
	virtual bool AdjustInsertCount(const ucs4_t* ucs, size_t& count) override;
	virtual bool AdjustStringLength(const wxString& ws, size_t& size) override;
	virtual void ProcessReturnCommand(MadEditCommand command) override {}
	virtual void OnGetFocusByClickAsControl(const wxMouseEvent &evt) override;
	virtual bool NeedNotProcessMouseLeftDown(wxMouseEvent &evt) override;
	virtual void OnWXMEditSetFocus() override;
	virtual void OnWXMEditKillFocus() override;
	virtual void OnMouseWheel(wxMouseEvent &evt) override {}
	virtual void SetEditMode(MadEditMode mode) override {}
	virtual void SetClientSizeData(int w, int h) override;

	int m_LeftClickX, m_LeftClickY;     // for singleline & setfocus
public:
	SingleLineWXMEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER | wxWANTS_CHARS);
};

} //namespace wxm

#endif //_WXMEDIT_SINGLE_LINE_H_
