///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_about_dialog.cpp
// Description:
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_about_dialog.h"

//(*InternalHeaders(WXMAboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "../images/wxmedit.xpm"
#include "../images/madedit.xpm"

//(*IdInit(WXMAboutDialog)
const long WXMAboutDialog::ID_STATICBITMAP1 = wxNewId();
const long WXMAboutDialog::ID_STATICBITMAP2 = wxNewId();
const long WXMAboutDialog::ID_WXMEMO1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMAboutDialog,wxDialog)
	//(*EventTable(WXMAboutDialog)
	//*)
END_EVENT_TABLE()

WXMAboutDialog::WXMAboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMAboutDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, _("About wxMEdit"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	StaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP1"));
	BoxSizer4->Add(StaticBitmap1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBitmap2 = new wxStaticBitmap(this, ID_STATICBITMAP2, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP2"));
	BoxSizer4->Add(StaticBitmap2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxDefaultPosition, wxSize(350,150), wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL, wxDefaultValidator, _T("ID_WXMEMO1"));
	BoxSizer2->Add(WxMemo1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(WxButtonOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer3->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMAboutDialog::WXMAboutDialogClose);
	//*)

	StaticBitmap1->SetBitmap(wxBitmap(wxmedit_xpm));
	StaticBitmap1->Enable(true);
	StaticBitmap2->SetBitmap(wxBitmap(madedit_xpm));
	StaticBitmap2->Enable(true);

	SetDefaultItem(WxButtonCancel);
	WxButtonCancel->SetFocus();
}

WXMAboutDialog::~WXMAboutDialog()
{
	//(*Destroy(WXMAboutDialog)
	//*)
}

void WXMAboutDialog::WXMAboutDialogClose(wxCloseEvent& event)
{
	Destroy();
}
