///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_word_count_dialog.cpp
// Description: WordCount Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_word_count_dialog.h"
#include "../wxm/edit/inframe.h"
#include "../wxm/utils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMWordCountDialog)
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

extern wxm::InFrameWXMEdit* g_active_wxmedit;

//(*IdInit(WXMWordCountDialog)
const long WXMWordCountDialog::ID_STATICTEXT8 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTLINECOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT1 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTWORDCOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT9 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTBYTECOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT3 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTCHARCOUNTALL = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT2 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTCHARCOUNTNOSPNOCTRL = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT10 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTSPACECOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT4 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTCONTROLCOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT5 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTFULLWIDTHCOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT6 = wxNewId();
const long WXMWordCountDialog::ID_WXSTATICTEXTPUREFULLWIDTHCOUNT = wxNewId();
const long WXMWordCountDialog::ID_STATICLINE1 = wxNewId();
const long WXMWordCountDialog::ID_STATICTEXT7 = wxNewId();
const long WXMWordCountDialog::ID_WXMEMO1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMWordCountDialog,wxDialog)
	//(*EventTable(WXMWordCountDialog)
	//*)
END_EVENT_TABLE()

WXMWordCountDialog::WXMWordCountDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMWordCountDialog)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, _("Word Count"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 60);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Lines"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextLineCount = new wxStaticText(this, ID_WXSTATICTEXTLINECOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTLINECOUNT"));
	FlexGridSizer1->Add(WxStaticTextLineCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Words"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextWordCount = new wxStaticText(this, ID_WXSTATICTEXTWORDCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTWORDCOUNT"));
	FlexGridSizer1->Add(WxStaticTextWordCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Bytes"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextByteCount = new wxStaticText(this, ID_WXSTATICTEXTBYTECOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTBYTECOUNT"));
	FlexGridSizer1->Add(WxStaticTextByteCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Characters (all)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextCharCountAll = new wxStaticText(this, ID_WXSTATICTEXTCHARCOUNTALL, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCHARCOUNTALL"));
	FlexGridSizer1->Add(WxStaticTextCharCountAll, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Characters (no controls/spaces)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextCharCountNoSPNoCtrl = new wxStaticText(this, ID_WXSTATICTEXTCHARCOUNTNOSPNOCTRL, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCHARCOUNTNOSPNOCTRL"));
	FlexGridSizer1->Add(WxStaticTextCharCountNoSPNoCtrl, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Space Characters"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer1->Add(StaticText10, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextSpaceCount = new wxStaticText(this, ID_WXSTATICTEXTSPACECOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTSPACECOUNT"));
	FlexGridSizer1->Add(WxStaticTextSpaceCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Control Characters"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextControlCount = new wxStaticText(this, ID_WXSTATICTEXTCONTROLCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCONTROLCOUNT"));
	FlexGridSizer1->Add(WxStaticTextControlCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Fullwidth Characters (with East Asian Ambiguous Width Characters)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextFullwidthCount = new wxStaticText(this, ID_WXSTATICTEXTFULLWIDTHCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFULLWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextFullwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Fullwidth Characters (without East Asian Ambiguous Width Characters)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	WxStaticTextPureFullwidthCount = new wxStaticText(this, ID_WXSTATICTEXTPUREFULLWIDTHCOUNT, _("999999"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTPUREFULLWIDTHCOUNT"));
	FlexGridSizer1->Add(WxStaticTextPureFullwidthCount, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Detail Information of Characters:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer1->Add(StaticText7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxDefaultPosition, wxSize(440,160), wxTE_MULTILINE|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_WXMEMO1"));
	BoxSizer1->Add(WxMemo1, 1, wxALL|wxEXPAND, 4);
	Button1 = new wxButton(this, wxID_CANCEL, _("&Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMWordCountDialog::WXMWordCountDialogClose);
	//*)

	wxm::SetDefaultMonoFont(WxMemo1);

	bool selection = g_active_wxmedit->IsSelected();

	wxm::WordCountData data;
	g_active_wxmedit->WordCount(selection, data);
	if (selection)
		this->SetTitle(_("Word Count (Selected Text)"));

	WxStaticTextLineCount->SetLabel(wxString() << data.lines);
	WxStaticTextWordCount->SetLabel(wxString() << data.words);
	WxStaticTextByteCount->SetLabel(wxString() << data.bytes);
	WxStaticTextCharCountAll->SetLabel(wxString() << data.chars);
	WxStaticTextCharCountNoSPNoCtrl->SetLabel(wxString() << (data.chars - data.spaces - data.controls));
	WxStaticTextSpaceCount->SetLabel(wxString() << data.spaces);
	WxStaticTextControlCount->SetLabel(wxString() << data.controls);
	WxStaticTextFullwidthCount->SetLabel(wxString() << (data.fullwidths + data.ambws));
	WxStaticTextPureFullwidthCount->SetLabel(wxString() << (data.fullwidths));
	wxString str;
	for(size_t i=0; i<data.detail.Count(); ++i)
		str << data.detail[i] << wxT("\n");
	WxMemo1->SetValue(str);
	Button1->SetFocus();
}

WXMWordCountDialog::~WXMWordCountDialog()
{
	//(*Destroy(WXMWordCountDialog)
	//*)
}


void WXMWordCountDialog::WXMWordCountDialogClose(wxCloseEvent& event)
{
	Destroy();
}
