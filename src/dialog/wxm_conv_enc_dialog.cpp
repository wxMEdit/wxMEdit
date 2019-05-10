///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_conv_enc_dialog.cpp
// Description: Encoding Conversion Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_conv_enc_dialog.h"
#include "../xm/cxx11.h"

#include "../xm/encoding/encoding.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/config.h>

//(*InternalHeaders(WXMConvEncDialog)
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

WXMConvEncDialog *g_ConvEncDialog=nullptr;
//(*IdInit(WXMConvEncDialog)
const long WXMConvEncDialog::ID_STATICTEXT1 = wxNewId();
const long WXMConvEncDialog::ID_WXCOMBOBOXENCODING = wxNewId();
const long WXMConvEncDialog::ID_WXRADIOBOXOPTION = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMConvEncDialog,wxDialog)
	//(*EventTable(WXMConvEncDialog)
	//*)
END_EVENT_TABLE()

WXMConvEncDialog::WXMConvEncDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMConvEncDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;

	Create(parent, id, _("Convert Encoding"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("New Encoding:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXENCODING"));
	BoxSizer2->Add(WxComboBoxEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxString __wxRadioBoxChoices_1[6] =
	{
	_("None"),
	_("Simplified Chinese => Traditional Chinese"),
	_("Traditional Chinese => Simplified Chinese"),
	_("Japanese Kanji => Traditional Chinese"),
	_("Japanese Kanji => Simplified Chinese"),
	_("Chinese => Japanese Kanji")
	};
	WxRadioBoxOption = new wxRadioBox(this, ID_WXRADIOBOXOPTION, _("Addtional Option"), wxDefaultPosition, wxDefaultSize, 6, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXOPTION"));
	BoxSizer3->Add(WxRadioBoxOption, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer4->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer4->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMConvEncDialog::WXMConvEncDialogClose);
	//*)

	size_t cnt=xm::EncodingManager::Instance().GetEncodingsCount();
	for(size_t i=0;i<cnt;i++)
	{
		WxComboBoxEncoding->Append(xm::EncodingManager::Instance().GetEncodingNameWithAliases(i).c_str());
	}
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	xm::EncodingManager& encmgr = xm::EncodingManager::Instance();
	wxString convenc = encmgr.GetEncodingName(0).c_str();
	wxConfigBase *cfg = wxConfigBase::Get(false);
	wxString oldpath = cfg->GetPath();
	cfg->Read(wxT("/wxMEdit/ConvertEncoding"), &convenc);
	cfg->SetPath(oldpath);
	WxComboBoxEncoding->SetValue(encmgr.ExpandEncodingAliases(convenc.wc_str()).c_str());

	WxButtonCancel->SetFocus();
}

WXMConvEncDialog::~WXMConvEncDialog()
{
	//(*Destroy(WXMConvEncDialog)
	//*)
}


void WXMConvEncDialog::WXMConvEncDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_ConvEncDialog=nullptr;
	Destroy();
}

std::wstring WXMConvEncDialog::GetEncoding() const
{
	return xm::EncodingManager::ExtractEncodingName(WxComboBoxEncoding->GetValue().wc_str());
}
