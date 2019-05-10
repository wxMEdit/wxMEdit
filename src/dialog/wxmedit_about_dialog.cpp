///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxmedit_about_dialog.cpp
// Description: wxMEdit About Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_about_dialog.h"
#include "../wxm/utils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMEditAboutDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#define static static const
#include "../../images/wxmedit.xpm"
#include "../../images/wxmedit0.xpm"
#undef static

//(*IdInit(WXMEditAboutDialog)
const long WXMEditAboutDialog::ID_STATICBITMAP1 = wxNewId();
const long WXMEditAboutDialog::ID_STATICBITMAP2 = wxNewId();
const long WXMEditAboutDialog::ID_TEXTCTRL1 = wxNewId();
const long WXMEditAboutDialog::ID_PANEL1 = wxNewId();
const long WXMEditAboutDialog::ID_TEXTCTRL2 = wxNewId();
const long WXMEditAboutDialog::ID_PANEL2 = wxNewId();
const long WXMEditAboutDialog::ID_TEXTCTRL3 = wxNewId();
const long WXMEditAboutDialog::ID_PANEL3 = wxNewId();
const long WXMEditAboutDialog::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMEditAboutDialog,wxDialog)
	//(*EventTable(WXMEditAboutDialog)
	//*)
END_EVENT_TABLE()

WXMEditAboutDialog::WXMEditAboutDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMEditAboutDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;

	Create(parent, wxID_ANY, _("About wxMEdit"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	StaticBitmap1 = new wxStaticBitmap(this, ID_STATICBITMAP1, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP1"));
	BoxSizer4->Add(StaticBitmap1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBitmap2 = new wxStaticBitmap(this, ID_STATICBITMAP2, wxNullBitmap, wxDefaultPosition, wxSize(48,48), 0, _T("ID_STATICBITMAP2"));
	BoxSizer4->Add(StaticBitmap2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_TOP, 5);
	Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
	AoubtTab = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	TxtAbout = new wxTextCtrl(AoubtTab, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(472,240), wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer5->Add(TxtAbout, 1, wxALL|wxEXPAND, 0);
	AoubtTab->SetSizer(BoxSizer5);
	BoxSizer5->Fit(AoubtTab);
	BoxSizer5->SetSizeHints(AoubtTab);
	LicenseTab = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	TxtLicense = new wxTextCtrl(LicenseTab, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL|wxTE_WORDWRAP|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	BoxSizer6->Add(TxtLicense, 1, wxALL|wxEXPAND, 0);
	LicenseTab->SetSizer(BoxSizer6);
	BoxSizer6->Fit(LicenseTab);
	BoxSizer6->SetSizeHints(LicenseTab);
	CreditsTab = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	TxtCredits = new wxTextCtrl(CreditsTab, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxSTATIC_BORDER, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	BoxSizer7->Add(TxtCredits, 1, wxALL|wxEXPAND, 0);
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

	Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&WXMEditAboutDialog::OnNotebook1PageChanged);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMEditAboutDialog::WXMEditAboutDialogClose);
	//*)

	wxm::SetDefaultMonoFont(TxtCredits);

	StaticBitmap1->SetBitmap(wxBitmap(wxmedit_xpm));
	StaticBitmap1->Enable(true);
	StaticBitmap2->SetBitmap(wxBitmap(wxmedit0_xpm));
	StaticBitmap2->Enable(true);

	WxButtonCancel->SetFocus();
}

WXMEditAboutDialog::~WXMEditAboutDialog()
{
	//(*Destroy(WXMEditAboutDialog)
	//*)
}

void WXMEditAboutDialog::WXMEditAboutDialogClose(wxCloseEvent& event)
{
	Destroy();
}

void WXMEditAboutDialog::OnNotebook1PageChanged(wxNotebookEvent& event)
{
	if (event.GetSelection() == 0/* AboutTab */)
	{
		wxm::g_wxMEdit_About_URL = wxm::g_wxMEdit_Homepage_URL;
		return;
	}

	if (event.GetSelection() == 1/* LicenseTab */)
	{
		wxm::g_wxMEdit_About_URL = wxm::g_wxMEdit_License_URL;
		return;
	}

	wxm::g_wxMEdit_About_URL.clear();
}
