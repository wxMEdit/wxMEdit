///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wx_avoid_wxmsw_bug4373.h
// Description: To avoid the bug 4373 of wxMSW-2.8.12 or lower
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_AVOID_WXMSW_BUG4373_H
#define _WX_AVOID_WXMSW_BUG4373_H

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#ifndef WX_PRECOMP
# include <wx/wx.h>
# include <wx/dialog.h>
#else
# include <wx/wxprec.h>
#endif

#if !defined(__WXMSW__) || wxMAJOR_VERSION == 3
# define wxDialogWrapper wxDialog
#else
# include "../xm/cxx11.h"
# include <wx/msw/private.h>

class wxDialogWrapper: public wxDialog
{
public:
	virtual int ShowModal() override
	{
		wxWindow* oldFocus = FindFocus();
		HWND hwndOldFocus = oldFocus ? GetHwndOf(oldFocus) : nullptr;

		int ret = wxDialog::ShowModal();

		if ( oldFocus && (oldFocus != this) && ::IsWindow(hwndOldFocus))
		{
			if (wxFindWinFromHandle((WXHWND) hwndOldFocus) == oldFocus)
				oldFocus->SetFocus();
		}

		return ret;
	}

	wxDialogWrapper()
		: wxDialog()
	{}

	wxDialogWrapper(wxWindow *parent, wxWindowID id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE,
			const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{}
};
#endif //__WXMSW__
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif //_WX_AVOID_WXMSW_BUG4373_H
