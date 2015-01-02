///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/inframe_wxmedit.h
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _INFRAME_WXMEDIT_H_
#define _INFRAME_WXMEDIT_H_

#include "../wxmedit/wxmedit.h"

namespace wxm
{

class InFrameWXMEdit : public MadEdit
{
	virtual void DoSelectionChanged();
	virtual void DoStatusChanged();
	virtual void DoToggleWindow();
	virtual void DoMouseRightUp();
public:
	InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
};

} //namespace wxm

#endif //_INFRAME_WXMEDIT_H_
