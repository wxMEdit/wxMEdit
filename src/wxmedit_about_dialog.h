///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_about_dialog.h
// Description:
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef WXMEDIT_ABOUT_DIALOG_H
#define WXMEDIT_ABOUT_DIALOG_H

//(*Headers(WXMAboutDialog)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class WXMAboutDialog: public wxDialog
{
	public:

		WXMAboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMAboutDialog();

		//(*Declarations(WXMAboutDialog)
		wxStaticBitmap* StaticBitmap2;
		wxStaticBitmap* StaticBitmap1;
		wxButton* WxButtonCancel;
		wxTextCtrl* WxMemo1;
		wxButton* WxButtonOK;
		//*)

	protected:

		//(*Identifiers(WXMAboutDialog)
		static const long ID_STATICBITMAP1;
		static const long ID_STATICBITMAP2;
		static const long ID_WXMEMO1;
		//*)

	private:

		//(*Handlers(WXMAboutDialog)
		void WXMAboutDialogClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
