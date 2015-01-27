///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/single_line_wxmedit.h
// Description: Single Line wxMEdit Control
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _SINGLE_LINE_WXMEDIT_H_
#define _SINGLE_LINE_WXMEDIT_H_

#include "simple_wxmedit.h"

namespace wxm
{

class SingleLineWXMEdit: public SimpleTextWXMEdit
{
	virtual void UpdateScrollBarPos();
	virtual bool AdjustInsertCount(const ucs4_t* ucs, size_t& count);
	virtual bool AdjustStringLength(const wxString& ws, size_t& size);
	virtual void ProcessReturnCommand(MadEditCommand command) {}
	virtual void OnGetFocusByClickAsControl(const wxMouseEvent &evt);
	virtual bool NeedNotProcessMouseLeftDown(wxMouseEvent &evt);
	virtual void OnWXMEditSetFocus();
	virtual void OnWXMEditKillFocus();
	virtual void OnMouseWheel(wxMouseEvent &evt) {}
	virtual void SetEditMode(MadEditMode mode) {}
	virtual void SetClientSizeData(int w, int h);

	int m_LeftClickX, m_LeftClickY;     // for singleline & setfocus
public:
	SingleLineWXMEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER | wxWANTS_CHARS);
};

} //namespace wxm

#endif //_SINGLE_LINE_WXMEDIT_H_
