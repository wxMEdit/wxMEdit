///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/single_line.h
// Description: Single Line wxMEdit Control
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_SINGLE_LINE_H_
#define _WXMEDIT_SINGLE_LINE_H_

#include "simple.h"

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

#endif //_WXMEDIT_SINGLE_LINE_H_
