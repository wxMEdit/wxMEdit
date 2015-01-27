///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_about_dialog.h
// Description: wxMEdit About Dialog
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXMEDIT_ABOUT_DIALOG_H
#define WXMEDIT_ABOUT_DIALOG_H

//(*Headers(WXMAboutDialog)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

namespace wxm
{
	extern const wxString g_wxMEdit_Homepage_URL;
	extern const wxString g_wxMEdit_License_URL;
	extern wxString g_wxMEdit_About_URL;
} // namespace wxm

class WXMAboutDialog: public wxDialog
{
	public:

		WXMAboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMAboutDialog();

		//(*Declarations(WXMAboutDialog)
		wxStaticBitmap* StaticBitmap2;
		wxNotebook* Notebook1;
		wxStaticBitmap* StaticBitmap1;
		wxPanel* LicenseTab;
		wxTextCtrl* TxtAbout;
		wxPanel* AoubtTab;
		wxButton* WxButtonCancel;
		wxTextCtrl* TxtCredits;
		wxButton* WxButtonOK;
		wxTextCtrl* TxtLicense;
		wxPanel* CreditsTab;
		//*)

	protected:

		//(*Identifiers(WXMAboutDialog)
		static const long ID_STATICBITMAP1;
		static const long ID_STATICBITMAP2;
		static const long ID_TEXTCTRL1;
		static const long ID_PANEL1;
		static const long ID_TEXTCTRL2;
		static const long ID_PANEL2;
		static const long ID_TEXTCTRL3;
		static const long ID_PANEL3;
		static const long ID_NOTEBOOK1;
		//*)

	private:

		//(*Handlers(WXMAboutDialog)
		void WXMAboutDialogClose(wxCloseEvent& event);
		void OnNotebook1PageChanged(wxNotebookEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
