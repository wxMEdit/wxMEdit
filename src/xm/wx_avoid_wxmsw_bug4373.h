///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wx_avoid_wxmsw_bug4373.h
// Description: To avoid the bug 4373 of wxMSW-2.8.12 or lower
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_AVOID_WXMSW_BUG4373_H
#define _WX_AVOID_WXMSW_BUG4373_H

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

#ifndef __WXMSW__
# define wxDialogWrapper wxDialog
#else
# include <wx/msw/private.h>
class wxDialogWrapper: public wxDialog
{
public:
	virtual int ShowModal()
	{
		wxWindow* oldFocus = FindFocus();
		HWND hwndOldFocus = oldFocus ? GetHwndOf(oldFocus) : NULL;

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
#endif

#endif //_WX_AVOID_WXMSW_BUG4373_H
