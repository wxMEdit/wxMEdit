///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxmedit_about_dialog.h
// Description: wxMEdit About Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXMEDIT_ABOUT_DIALOG_H
#define WXMEDIT_ABOUT_DIALOG_H

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMEditAboutDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifndef wxTE_AUTO_SCROLL
#define wxTE_AUTO_SCROLL 0
#endif

namespace wxm
{
	extern const wxString g_wxMEdit_Homepage_URL;
	extern const wxString g_wxMEdit_License_URL;
	extern wxString g_wxMEdit_About_URL;
} // namespace wxm

class WXMEditAboutDialog: public wxDialog
{
	public:

		WXMEditAboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMEditAboutDialog();

		//(*Declarations(WXMEditAboutDialog)
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		wxNotebook* Notebook1;
		wxPanel* AoubtTab;
		wxPanel* CreditsTab;
		wxPanel* LicenseTab;
		wxStaticBitmap* StaticBitmap1;
		wxStaticBitmap* StaticBitmap2;
		wxTextCtrl* TxtAbout;
		wxTextCtrl* TxtCredits;
		wxTextCtrl* TxtLicense;
		//*)

	protected:

		//(*Identifiers(WXMEditAboutDialog)
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

		//(*Handlers(WXMEditAboutDialog)
		void WXMEditAboutDialogClose(wxCloseEvent& event);
		void OnNotebook1PageChanged(wxNotebookEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
