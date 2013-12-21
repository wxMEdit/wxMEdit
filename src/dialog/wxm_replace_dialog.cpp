///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_replace_dialog.cpp
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_replace_dialog.h"
#include "wxm_search_dialog.h"

#include "../wxmedit/wxmedit.h"

//(*InternalHeaders(WXMReplaceDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#define static static const
#include "../../images/down.xpm"
#undef static

WXMReplaceDialog *g_ReplaceDialog=NULL;

//(*IdInit(WXMReplaceDialog)
const long WXMReplaceDialog::ID_WXCHECKBOXMOVEFOCUS = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXWRAPAROUND = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXCASESENSITIVE = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXWHOLEWORD = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXREGEX = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXFINDHEX = wxNewId();
const long WXMReplaceDialog::ID_WXCHECKBOXSEARCHINSELECTION = wxNewId();
const long WXMReplaceDialog::ID_WXSTATICTEXTFROM = wxNewId();
const long WXMReplaceDialog::ID_WXEDITFROM = wxNewId();
const long WXMReplaceDialog::ID_WXSTATICTEXTTO = wxNewId();
const long WXMReplaceDialog::ID_WXEDITTO = wxNewId();
const long WXMReplaceDialog::ID_WXBUTTONFINDNEXT = wxNewId();
const long WXMReplaceDialog::ID_WXBUTTONREPLACE = wxNewId();
const long WXMReplaceDialog::ID_WXBUTTONREPLACEALL = wxNewId();
const long WXMReplaceDialog::ID_WXBUTTONCLOSE = wxNewId();
//*)

const long WXMReplaceDialog::ID_MADEDIT1 = wxNewId();
const long WXMReplaceDialog::ID_MADEDIT2 = wxNewId();
const long WXMReplaceDialog::ID_WXBITMAPBUTTONRECENTFINDTEXT = wxNewId();
const long WXMReplaceDialog::ID_WXBITMAPBUTTONRECENTREPLACETEXT = wxNewId();

BEGIN_EVENT_TABLE(WXMReplaceDialog,wxDialog)
	//(*EventTable(WXMReplaceDialog)
	//*)
	EVT_ACTIVATE(WXMReplaceDialog::WXMReplaceDialogActivate)

	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, WXMReplaceDialog::WxBitmapButtonRecentFindTextClick)
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTREPLACETEXT, WXMReplaceDialog::WxBitmapButtonRecentReplaceTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20, WXMReplaceDialog::OnRecentFindText)
	EVT_MENU_RANGE(ID_RECENTREPLACETEXT1, ID_RECENTREPLACETEXT20, WXMReplaceDialog::OnRecentReplaceText)
END_EVENT_TABLE()

WXMReplaceDialog::WXMReplaceDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMReplaceDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, wxID_ANY, _("Replace"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxMoveFocus = new wxCheckBox(this, ID_WXCHECKBOXMOVEFOCUS, _("&Move Focus to Editor Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMOVEFOCUS"));
	WxCheckBoxMoveFocus->SetValue(false);
	BoxSizer6->Add(WxCheckBoxMoveFocus, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWrapAround = new wxCheckBox(this, ID_WXCHECKBOXWRAPAROUND, _("Wrap ar&ound"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWRAPAROUND"));
	WxCheckBoxWrapAround->SetValue(true);
	BoxSizer6->Add(WxCheckBoxWrapAround, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCASESENSITIVE"));
	WxCheckBoxCaseSensitive->SetValue(false);
	BoxSizer6->Add(WxCheckBoxCaseSensitive, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWHOLEWORD"));
	WxCheckBoxWholeWord->SetValue(false);
	BoxSizer6->Add(WxCheckBoxWholeWord, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREGEX"));
	WxCheckBoxRegex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxRegex, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXFINDHEX"));
	WxCheckBoxFindHex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxFindHex, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxSearchInSelection = new wxCheckBox(this, ID_WXCHECKBOXSEARCHINSELECTION, _("Search In &Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSEARCHINSELECTION"));
	WxCheckBoxSearchInSelection->SetValue(false);
	BoxSizer7->Add(WxCheckBoxSearchInSelection, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextFrom = new wxStaticText(this, ID_WXSTATICTEXTFROM, _("From:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFROM"));
	BoxSizer7->Add(WxStaticTextFrom, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFrom = new wxTextCtrl(this, ID_WXEDITFROM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITFROM"));
	BoxSizer7->Add(WxEditFrom, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextTo = new wxStaticText(this, ID_WXSTATICTEXTTO, _("To:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTTO"));
	BoxSizer7->Add(WxStaticTextTo, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditTo = new wxTextCtrl(this, ID_WXEDITTO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITTO"));
	BoxSizer7->Add(WxEditTo, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer2->Add(BoxSizer6, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxButtonFindNext = new wxButton(this, ID_WXBUTTONFINDNEXT, _("Find &Next"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDNEXT"));
	BoxSizer3->Add(WxButtonFindNext, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACE"));
	WxButtonReplace->SetDefault();
	BoxSizer3->Add(WxButtonReplace, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplaceAll = new wxButton(this, ID_WXBUTTONREPLACEALL, _("Replace &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACEALL"));
	BoxSizer3->Add(WxButtonReplaceAll, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonClose = new wxButton(this, ID_WXBUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCLOSE"));
	BoxSizer3->Add(WxButtonClose, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_WXCHECKBOXFINDHEX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxCheckBoxFindHexClick);
	Connect(ID_WXCHECKBOXSEARCHINSELECTION,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxCheckBoxSearchInSelectionClick);
	Connect(ID_WXBUTTONFINDNEXT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxButtonFindNextClick);
	Connect(ID_WXBUTTONREPLACE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxButtonReplaceClick);
	Connect(ID_WXBUTTONREPLACEALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxButtonReplaceAllClick);
	Connect(ID_WXBUTTONCLOSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMReplaceDialog::WxButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMReplaceDialog::WXMReplaceDialogClose);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&WXMReplaceDialog::WXMReplaceDialogKeyDown);
	//*)

	this->SetPosition(wxPoint(300,100));

	int bw, bh;
	// find
	WxButtonFindNext->GetSize(&bw, &bh);

	m_FindText=new MadEdit(this, ID_MADEDIT1, wxPoint(0, 0), wxSize(400, bh));
	m_FindText->SetSingleLineMode(true);
	m_FindText->SetEncoding(wxT("UTF-32LE"));
	m_FindText->SetFixedWidthMode(false);
	m_FindText->SetRecordCaretMovements(false);
	m_FindText->SetInsertSpacesInsteadOfTab(false);
	m_FindText->SetWantTab(false);
	m_FindText->LoadDefaultSyntaxScheme();

	BoxSizer4->Add(m_FindText,1,wxEXPAND|wxALIGN_LEFT | wxALL,2);
	BoxSizer4->SetItemMinSize(m_FindText, 400, bh);

	wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
	WxBitmapButtonRecentFindText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentFindText"));
	BoxSizer4->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	// replace
	WxButtonReplace->GetSize(&bw, &bh);

	m_ReplaceText=new MadEdit(this, ID_MADEDIT2, wxPoint(0, 0), wxSize(400, bh));
	m_ReplaceText->SetSingleLineMode(true);
	m_ReplaceText->SetEncoding(wxT("UTF-32LE"));
	m_ReplaceText->SetFixedWidthMode(false);
	m_ReplaceText->SetRecordCaretMovements(false);
	m_ReplaceText->SetInsertSpacesInsteadOfTab(false);
	m_ReplaceText->SetWantTab(false);
	m_ReplaceText->LoadDefaultSyntaxScheme();

	BoxSizer5->Add(m_ReplaceText,1,wxEXPAND|wxALIGN_LEFT | wxALL,2);
	BoxSizer5->SetItemMinSize(m_ReplaceText, 400, bh);

	WxBitmapButtonRecentReplaceText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTREPLACETEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentReplaceText"));
	BoxSizer5->Add(WxBitmapButtonRecentReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	// connect to KeyDown event handler
	m_FindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	m_ReplaceText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxBitmapButtonRecentFindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxBitmapButtonRecentReplaceText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxMoveFocus->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxCaseSensitive->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxWholeWord->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxRegex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxFindHex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxCheckBoxSearchInSelection->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxStaticTextFrom->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxStaticTextTo->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxButtonFindNext->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxButtonReplace->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxButtonReplaceAll->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));
	WxButtonClose->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMReplaceDialog::WXMReplaceDialogKeyDown));


	m_RecentReplaceText=new wxRecentList(true, 20, ID_RECENTREPLACETEXT1);
	m_RecentReplaceText->UseMenu(&WxPopupMenuRecentReplaceText);

	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();
	m_Config->SetPath(wxT("/RecentReplaceText"));
	m_RecentReplaceText->Load(*m_Config);
	m_Config->SetPath(oldpath);


	if(g_SearchDialog!=NULL)
	{
		wxString text;
		g_SearchDialog->m_FindText->GetText(text, true);
		m_FindText->SetText(text);
	}

	if(m_RecentReplaceText->GetCount()>0)
	{
		wxString text=m_RecentReplaceText->GetHistoryFile(0);
		if(!text.IsEmpty())
		{
			m_ReplaceText->SetText(text);
		}
	}
}

WXMReplaceDialog::~WXMReplaceDialog()
{
	//(*Destroy(WXMReplaceDialog)
	//*)
}


void WXMReplaceDialog::WXMReplaceDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_ReplaceDialog=NULL;
	Destroy();
}

void WXMReplaceDialog::WxButtonCloseClick(wxCommandEvent& event)
{
	Show(false);
}

void WXMReplaceDialog::WxButtonFindNextClick(wxCommandEvent& event)
{
	this->ReadWriteSettings(false);
	g_SearchDialog->ReadWriteSettings(true);

	wxString text;
	m_FindText->GetText(text, true);
	g_SearchDialog->m_FindText->SetText(text);

	wxCommandEvent e;
	g_SearchDialog->WxButtonFindNextClick(e);
}


void WXMReplaceDialog::ReadWriteSettings(bool bRead)
{
	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();

	if(bRead)
	{
		bool bb;
		m_Config->Read(wxT("/wxMEdit/SearchMoveFocus"), &bb, false);
		WxCheckBoxMoveFocus->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchWrapAround"), &bb, true);
		WxCheckBoxWrapAround->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchCaseSensitive"), &bb, false);
		WxCheckBoxCaseSensitive->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchWholeWord"), &bb, false);
		WxCheckBoxWholeWord->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchRegex"), &bb, false);
		WxCheckBoxRegex->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchHex"), &bb, false);
		WxCheckBoxFindHex->SetValue(bb);
		UpdateCheckBoxByCBHex(bb);

		m_Config->Read(wxT("/wxMEdit/SearchInSelection"), &bb, false);
		WxCheckBoxSearchInSelection->SetValue(bb);
		UpdateSearchInSelection(bb);

		wxString str;
		m_Config->Read(wxT("/wxMEdit/SearchFrom"), &str, wxEmptyString);
		WxEditFrom->SetValue(str);
		m_Config->Read(wxT("/wxMEdit/SearchTo"), &str, wxEmptyString);
		WxEditTo->SetValue(str);
	}
	else
	{
		m_Config->Write(wxT("/wxMEdit/SearchMoveFocus"), WxCheckBoxMoveFocus->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchWrapAround"), WxCheckBoxWrapAround->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchCaseSensitive"), WxCheckBoxCaseSensitive->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchWholeWord"), WxCheckBoxWholeWord->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchRegex"), WxCheckBoxRegex->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchHex"), WxCheckBoxFindHex->GetValue());

		m_Config->Write(wxT("/wxMEdit/SearchInSelection"), WxCheckBoxSearchInSelection->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchFrom"), WxEditFrom->GetValue());
		m_Config->Write(wxT("/wxMEdit/SearchTo"), WxEditTo->GetValue());
	}

	m_Config->SetPath(oldpath);
}

void WXMReplaceDialog::UpdateCheckBoxByCBHex(bool check)
{
	if(check)
	{
		WxCheckBoxCaseSensitive->Disable();
		WxCheckBoxWholeWord->Disable();
		WxCheckBoxRegex->Disable();
	}
	else
	{
		WxCheckBoxCaseSensitive->Enable();
		WxCheckBoxWholeWord->Enable();
		WxCheckBoxRegex->Enable();
	}
}

void WXMReplaceDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
	UpdateCheckBoxByCBHex(event.IsChecked());
}

void WXMReplaceDialog::WXMReplaceDialogKeyDown(wxKeyEvent& event)
{
	int key=event.GetKeyCode();

	//g_SearchDialog->SetTitle(wxString()<<key);

	switch(key)
	{
	case WXK_ESCAPE:
		g_ReplaceDialog->Show(false);
		return;
	case WXK_RETURN:
	case WXK_NUMPAD_ENTER:
		if((wxButton*)this!=g_ReplaceDialog->WxButtonFindNext &&
		   (wxButton*)this!=g_ReplaceDialog->WxButtonReplace &&
		   (wxButton*)this!=g_ReplaceDialog->WxButtonReplaceAll &&
		   (wxButton*)this!=g_ReplaceDialog->WxButtonClose)
		{
			wxCommandEvent e;
			g_ReplaceDialog->WxButtonReplaceClick(e);
			return; // no skip
		}
		break;
	case WXK_DOWN:
		if((MadEdit*)this==g_ReplaceDialog->m_FindText)
		{
			int x,y,w,h;
			g_ReplaceDialog->m_FindText->GetPosition(&x, &y);
			g_ReplaceDialog->m_FindText->GetSize(&w, &h);
			g_ReplaceDialog->PopupMenu(&g_SearchDialog->WxPopupMenuRecentFindText, x, y+h);
			return;
		}
		else if((MadEdit*)this==g_ReplaceDialog->m_ReplaceText)
		{
			int x,y,w,h;
			g_ReplaceDialog->m_ReplaceText->GetPosition(&x, &y);
			g_ReplaceDialog->m_ReplaceText->GetSize(&w, &h);
			g_ReplaceDialog->PopupMenu(&g_ReplaceDialog->WxPopupMenuRecentReplaceText, x, y+h);
			return;
		}
		break;
	}

	event.Skip();
}

void WXMReplaceDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
	PopupMenu(&g_SearchDialog->WxPopupMenuRecentFindText);
}

void WXMReplaceDialog::WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event)
{
	PopupMenu(&WxPopupMenuRecentReplaceText);
}

void WXMReplaceDialog::OnRecentFindText(wxCommandEvent& event)
{
	int idx=event.GetId()-ID_RECENTFINDTEXT1;
	wxString text=g_SearchDialog->m_RecentFindText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_FindText->SetText(text);
		m_FindText->SetFocus();
	}
}

void WXMReplaceDialog::OnRecentReplaceText(wxCommandEvent& event)
{
	int idx=event.GetId()-ID_RECENTREPLACETEXT1;
	wxString text=m_RecentReplaceText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_ReplaceText->SetText(text);
		m_ReplaceText->SetFocus();
	}
}

void WXMReplaceDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit==NULL)
		return;

	wxString text;
	m_FindText->GetText(text, true);

	if(WxCheckBoxFindHex->GetValue())
		text.Trim();

	if(text.Len() <= 0)
		return;

	wxString reptext;
	m_ReplaceText->GetText(reptext, true);

	g_SearchDialog->m_RecentFindText->AddItemToHistory(text);

	if(reptext.Len()>0)
	{
		m_RecentReplaceText->AddItemToHistory(reptext);
	}

	wxInt64 from = 0, to = 0;
	wxFileOffset rangeFrom = -1, rangeTo = -1;
	wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();
	if(WxCheckBoxSearchInSelection->IsChecked())
	{
		if(!StrToInt64(WxEditFrom->GetValue(), from))
		{
			wxMessageBox(_("The value of 'From' is incorrect."), wxT("wxMEdit"), wxOK|wxICON_WARNING);
			return;
		}
		if(!StrToInt64(WxEditTo->GetValue(), to))
		{
			wxMessageBox(_("The value of 'To' is incorrect."), wxT("wxMEdit"), wxOK|wxICON_WARNING);
			return;
		}

		rangeTo = to;
		if(caretpos <= from || caretpos > to || (caretpos==to && WxCheckBoxWrapAround->IsChecked()))
			rangeFrom = from;
	}

	MadReplaceResult ret=RR_EXPR_ERROR;

	for(;;)
	{
		if(WxCheckBoxFindHex->GetValue())
		{
			ret=g_ActiveMadEdit->ReplaceHex(text, reptext, rangeFrom, rangeTo);
		}
		else
		{
			ret=g_ActiveMadEdit->ReplaceText(text, reptext,
				WxCheckBoxRegex->GetValue(),
				WxCheckBoxCaseSensitive->GetValue(),
				WxCheckBoxWholeWord->GetValue(),
				rangeFrom, rangeTo);
		}

		if ((ret==RR_REP_NNEXT || ret==RR_NREP_NNEXT) &&
			WxCheckBoxWrapAround->IsChecked() && rangeTo != caretpos)
		{
			rangeTo = caretpos;
			rangeFrom = WxCheckBoxSearchInSelection->IsChecked()? from : 0;
			continue;
		}

		break;
	}

	switch(ret)
	{
	case RR_REP_NNEXT:
	case RR_NREP_NNEXT:
		{
			wxMessageDialog dlg(this, _("Cannot find the matched string.\nReplace is finished."), wxT("wxMEdit"));
			dlg.ShowModal();
			m_FindText->SetFocus();
		}
		break;
	case RR_REP_NEXT:
	case RR_NREP_NEXT:
		if(WxCheckBoxMoveFocus->GetValue())
		{
			((wxFrame*)wxTheApp->GetTopWindow())->Raise();
			g_ActiveMadEdit->SetFocus();
		}
		break;
	default:
		break;
	}
}

void WXMReplaceDialog::WxButtonReplaceAllClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit==NULL)
		return;

	wxString text;
	m_FindText->GetText(text, true);

	if(WxCheckBoxFindHex->GetValue())
		text.Trim();

	if(text.Len()>0)
	{
		wxString reptext;
		m_ReplaceText->GetText(reptext, true);

		g_SearchDialog->m_RecentFindText->AddItemToHistory(text);

		if(reptext.Len()>0)
		{
			m_RecentReplaceText->AddItemToHistory(reptext);
		}

		wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			if(!StrToInt64(WxEditFrom->GetValue(), from))
			{
				wxMessageBox(_("The value of 'From' is incorrect."), wxT("wxMEdit"), wxOK|wxICON_WARNING);
				return;
			}
			if(!StrToInt64(WxEditTo->GetValue(), to))
			{
				wxMessageBox(_("The value of 'To' is incorrect."), wxT("wxMEdit"), wxOK|wxICON_WARNING);
				return;
			}

			rangeTo = to;
			rangeFrom = from;
		}

		int count=0;
		if(WxCheckBoxFindHex->GetValue())
		{
			count=g_ActiveMadEdit->ReplaceHexAll(text, reptext, NULL, NULL, rangeFrom, rangeTo);
		}
		else
		{
			count=g_ActiveMadEdit->ReplaceTextAll(text, reptext,
				WxCheckBoxRegex->GetValue(),
				WxCheckBoxCaseSensitive->GetValue(),
				WxCheckBoxWholeWord->GetValue(),
				NULL, NULL, rangeFrom, rangeTo);
		}

		if(count>=0)
		{
			wxString msg;
			if(count==0)
				msg=wxString(_("Cannot find any matched string."));
			else
				msg=wxString::Format(_("%d string(s) were replaced."), count);

			wxMessageDialog dlg(this, msg, wxT("wxMEdit"));
			dlg.ShowModal();
		}

		m_FindText->SetFocus();
	}
}

void WXMReplaceDialog::WXMReplaceDialogActivate(wxActivateEvent& event)
{
	ReadWriteSettings(event.GetActive());
}

void WXMReplaceDialog::UpdateSearchInSelection(bool check)
{
	WxEditFrom->Enable(check);
	WxEditTo->Enable(check);

	extern MadEdit *g_ActiveMadEdit;
	if(check && g_ActiveMadEdit!=NULL)
	{
		WxEditFrom->SetValue(wxLongLong(g_ActiveMadEdit->GetSelectionBeginPos()).ToString());
		WxEditTo->SetValue(wxLongLong(g_ActiveMadEdit->GetSelectionEndPos()).ToString());
	}
}

void WXMReplaceDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
	UpdateSearchInSelection(event.IsChecked());
}

