///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_sort_dialog.h
// Description: Sort Options Dialog
// Author:      wxmedit@gmail.com
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_SORT_DIALOG_H
#define WXM_SORT_DIALOG_H

//(*Headers(WXMSortDialog)
#include <wx/sizer.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class WXMSortDialog: public wxDialog
{
	public:

		WXMSortDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMSortDialog();

		//(*Declarations(WXMSortDialog)
		wxCheckBox* WxCheckBoxNumeric;
		wxButton* WxButtonSort;
		wxCheckBox* WxCheckBoxCase;
		wxCheckBox* WxCheckBoxRemoveDup;
		wxButton* WxButtonCancel;
		wxRadioBox* WxRadioBoxOrder;
		//*)

	protected:

		//(*Identifiers(WXMSortDialog)
		static const long ID_WXRADIOBOXORDER;
		static const long ID_WXCHECKBOXCASE;
		static const long ID_WXCHECKBOXNUMERIC;
		static const long ID_WXCHECKBOXREMOVEDUP;
		//*)

	private:

		//(*Handlers(WXMSortDialog)
		void WXMSortDialogClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
