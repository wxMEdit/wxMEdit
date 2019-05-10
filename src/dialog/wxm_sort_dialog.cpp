///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_sort_dialog.cpp
// Description: Sort Options Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_sort_dialog.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMSortDialog)
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

//(*IdInit(WXMSortDialog)
const long WXMSortDialog::ID_WXRADIOBOXORDER = wxNewId();
const long WXMSortDialog::ID_WXCHECKBOXCASE = wxNewId();
const long WXMSortDialog::ID_WXCHECKBOXNUMERIC = wxNewId();
const long WXMSortDialog::ID_WXCHECKBOXREMOVEDUP = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMSortDialog,wxDialog)
	//(*EventTable(WXMSortDialog)
	//*)
END_EVENT_TABLE()

WXMSortDialog::WXMSortDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMSortDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;

	Create(parent, id, _("Sort Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	wxString __wxRadioBoxChoices_1[2] =
	{
	_("&Ascending"),
	_("&Descending")
	};
	WxRadioBoxOrder = new wxRadioBox(this, ID_WXRADIOBOXORDER, _("Sort Order"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXORDER"));
	BoxSizer1->Add(WxRadioBoxOrder, 0, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxCase = new wxCheckBox(this, ID_WXCHECKBOXCASE, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCASE"));
	WxCheckBoxCase->SetValue(false);
	BoxSizer1->Add(WxCheckBoxCase, 0, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxNumeric = new wxCheckBox(this, ID_WXCHECKBOXNUMERIC, _("&Numeric Sort"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXNUMERIC"));
	WxCheckBoxNumeric->SetValue(false);
	BoxSizer1->Add(WxCheckBoxNumeric, 0, wxALL|wxALIGN_LEFT, 5);
	WxCheckBoxRemoveDup = new wxCheckBox(this, ID_WXCHECKBOXREMOVEDUP, _("&Remove Duplicates"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREMOVEDUP"));
	WxCheckBoxRemoveDup->SetValue(false);
	BoxSizer1->Add(WxCheckBoxRemoveDup, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonSort = new wxButton(this, wxID_OK, _("&Sort"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer2->Add(WxButtonSort, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer2->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_LEFT, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMSortDialog::WXMSortDialogClose);
	//*)

	WxButtonSort->SetFocus();
}

WXMSortDialog::~WXMSortDialog()
{
	//(*Destroy(WXMSortDialog)
	//*)
}


void WXMSortDialog::WXMSortDialogClose(wxCloseEvent& event)
{
	Destroy();
}
