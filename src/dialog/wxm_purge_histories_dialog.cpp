///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_sort_dialog.h
// Description: Purge Histories Dialog
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_purge_histories_dialog.h"
#include "../wxmedit_frame.h"
#include "wxm_find_in_files_dialog.h"
#include "wxm_search_replace_dialog.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMPurgeHistoriesDialog)
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

//(*IdInit(WXMPurgeHistoriesDialog)
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFILES = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFONTS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTENCODINGS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFINDTEXTS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTREPLACETEXTS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFINDDIRS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFINDFILTERS = wxNewId();
const long WXMPurgeHistoriesDialog::ID_CHECKBOXRECENTFINDEXCLUDES = wxNewId();
const long WXMPurgeHistoriesDialog::ID_BUTTONPURGE = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMPurgeHistoriesDialog,wxDialog)
	//(*EventTable(WXMPurgeHistoriesDialog)
	//*)
END_EVENT_TABLE()

WXMPurgeHistoriesDialog::WXMPurgeHistoriesDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMPurgeHistoriesDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Purge Histories"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Select histories to purge"));
	WxCheckBoxRecentFiles = new wxCheckBox(this, ID_CHECKBOXRECENTFILES, _("Recent &Files"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFILES"));
	WxCheckBoxRecentFiles->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFiles, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentFonts = new wxCheckBox(this, ID_CHECKBOXRECENTFONTS, _("Recent Fon&ts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFONTS"));
	WxCheckBoxRecentFonts->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFonts, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentEncodings = new wxCheckBox(this, ID_CHECKBOXRECENTENCODINGS, _("Recent &Encodings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTENCODINGS"));
	WxCheckBoxRecentEncodings->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentEncodings, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentFindTexts = new wxCheckBox(this, ID_CHECKBOXRECENTFINDTEXTS, _("Recent &Searched Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFINDTEXTS"));
	WxCheckBoxRecentFindTexts->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFindTexts, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentReplaceTexts = new wxCheckBox(this, ID_CHECKBOXRECENTREPLACETEXTS, _("Recent &Replaced Texts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTREPLACETEXTS"));
	WxCheckBoxRecentReplaceTexts->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentReplaceTexts, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentFilesFindDirs = new wxCheckBox(this, ID_CHECKBOXRECENTFINDDIRS, _("Recent Searched &Directories"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFINDDIRS"));
	WxCheckBoxRecentFilesFindDirs->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFilesFindDirs, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentFindFilters = new wxCheckBox(this, ID_CHECKBOXRECENTFINDFILTERS, _("Recent Searched File Fi&lters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFINDFILTERS"));
	WxCheckBoxRecentFindFilters->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFindFilters, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecentFindExcludes = new wxCheckBox(this, ID_CHECKBOXRECENTFINDEXCLUDES, _("Recent Searched E&xclude Filters"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXRECENTFINDEXCLUDES"));
	WxCheckBoxRecentFindExcludes->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxRecentFindExcludes, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonPurge = new wxButton(this, ID_BUTTONPURGE, _("&Purge"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTONPURGE"));
	BoxSizer2->Add(WxButtonPurge, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	WxButtonCancel->SetDefault();
	BoxSizer2->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_BUTTONPURGE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMPurgeHistoriesDialog::WxButtonPurgeClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMPurgeHistoriesDialog::WXMPurgeHistoriesDialogClose);
	//*)
}

WXMPurgeHistoriesDialog::~WXMPurgeHistoriesDialog()
{
	//(*Destroy(WXMPurgeHistoriesDialog)
	//*)
}


void WXMPurgeHistoriesDialog::WXMPurgeHistoriesDialogClose(wxCloseEvent& event)
{
	Destroy();
}

void WXMPurgeHistoriesDialog::WxButtonPurgeClick(wxCommandEvent& event)
{
	if (WxCheckBoxRecentFiles->IsChecked())
		g_MainFrame->PurgeRecentFiles();

	if (WxCheckBoxRecentFonts->IsChecked())
		g_MainFrame->PurgeRecentFonts();

	if (WxCheckBoxRecentEncodings->IsChecked())
		g_MainFrame->PurgeRecentEncodings();

	if (WxCheckBoxRecentFindTexts->IsChecked())
		GetSearchReplaceDialog(g_MainFrame).PurgeRecentFindTexts();

	if (WxCheckBoxRecentReplaceTexts->IsChecked())
		GetSearchReplaceDialog(g_MainFrame).PurgeRecentReplaceTexts();

	if (WxCheckBoxRecentFilesFindDirs->IsChecked())
		GetFindInFilesDialog(g_MainFrame).PurgeRecentFindDirs();

	if (WxCheckBoxRecentFindFilters->IsChecked())
		GetFindInFilesDialog(g_MainFrame).PurgeRecentFindFilters();

	if (WxCheckBoxRecentFindExcludes->IsChecked())
		GetFindInFilesDialog(g_MainFrame).PurgeRecentFindExcludes();

	Close();
}
