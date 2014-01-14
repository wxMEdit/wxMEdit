///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_about_dialog.cpp
// Description:
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_about_dialog.h"
#include "../wxm_utils.h"

//(*InternalHeaders(WXMAboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)


#define static static const
#include "../../images/wxmedit.xpm"
#include "../../images/madedit.xpm"
#undef static

//(*IdInit(WXMAboutDialog)
const long WXMAboutDialog::ID_STATICBITMAP1 = wxNewId();
const long WXMAboutDialog::ID_STATICBITMAP2 = wxNewId();
const long WXMAboutDialog::ID_TEXTCTRL1 = wxNewId();
const long WXMAboutDialog::ID_PANEL1 = wxNewId();
const long WXMAboutDialog::ID_TEXTCTRL2 = wxNewId();
const long WXMAboutDialog::ID_PANEL2 = wxNewId();
const long WXMAboutDialog::ID_TEXTCTRL3 = wxNewId();
const long WXMAboutDialog::ID_PANEL3 = wxNewId();
const long WXMAboutDialog::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMAboutDialog,wxDialog)
	//(*EventTable(WXMAboutDialog)
	//*)
END_EVENT_TABLE()

WXMAboutDialog::WXMAboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMAboutDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
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
	Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
	AoubtTab = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	TxtAbout = new wxTextCtrl(AoubtTab, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(450,180), wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer5->Add(TxtAbout, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	AoubtTab->SetSizer(BoxSizer5);
	BoxSizer5->Fit(AoubtTab);
	BoxSizer5->SetSizeHints(AoubtTab);
	LicenseTab = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	TxtLicense = new wxTextCtrl(LicenseTab, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer6->Add(TxtLicense, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	LicenseTab->SetSizer(BoxSizer6);
	BoxSizer6->Fit(LicenseTab);
	BoxSizer6->SetSizeHints(LicenseTab);
	CreditsTab = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	TxtCredits = new wxTextCtrl(CreditsTab, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	BoxSizer7->Add(TxtCredits, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	CreditsTab->SetSizer(BoxSizer7);
	BoxSizer7->Fit(CreditsTab);
	BoxSizer7->SetSizeHints(CreditsTab);
	Notebook1->AddPage(AoubtTab, _("About"), false);
	Notebook1->AddPage(LicenseTab, _("License"), false);
	Notebook1->AddPage(CreditsTab, _("Credits"), false);
	BoxSizer2->Add(Notebook1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer3->Add(WxButtonOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	WxButtonCancel->SetDefault();
	BoxSizer3->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&WXMAboutDialog::OnNotebook1PageChanged);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMAboutDialog::WXMAboutDialogClose);
	//*)

	SetDefaultMonoFont(TxtCredits);

	StaticBitmap1->SetBitmap(wxBitmap(wxmedit_xpm));
	StaticBitmap1->Enable(true);
	StaticBitmap2->SetBitmap(wxBitmap(madedit_xpm));
	StaticBitmap2->Enable(true);

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

void WXMAboutDialog::OnNotebook1PageChanged(wxNotebookEvent& event)
{
	if (event.GetSelection() == 0/* AboutTab */)
	{
		g_wxMEdit_About_URL = g_wxMEdit_Homepage_URL;
		return;
	}

	if (event.GetSelection() == 1/* LicenseTab */)
	{
		g_wxMEdit_About_URL = g_wxMEdit_License_URL;
		return;
	}

	g_wxMEdit_About_URL.clear();
}
