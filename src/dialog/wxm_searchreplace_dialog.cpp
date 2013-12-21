///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_searchreplace_dialog.cpp
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_searchreplace_dialog.h"

#include "../wxmedit/wxmedit.h"

//(*InternalHeaders(WXMSearchReplaceDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#define static static const
#include "../../images/down.xpm"
#undef static

WXMSearchReplaceDialog *g_SearchReplaceDialog=NULL;

//(*IdInit(WXMSearchReplaceDialog)
const long WXMSearchReplaceDialog::ID_WXCHECKBOXMOVEFOCUS = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXWRAPAROUND = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXCASESENSITIVE = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXWHOLEWORD = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXREGEX = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXFINDHEX = wxNewId();
const long WXMSearchReplaceDialog::ID_WXCHECKBOXSEARCHINSELECTION = wxNewId();
const long WXMSearchReplaceDialog::ID_WXSTATICTEXTFROM = wxNewId();
const long WXMSearchReplaceDialog::ID_WXEDITFROM = wxNewId();
const long WXMSearchReplaceDialog::ID_WXSTATICTEXTTO = wxNewId();
const long WXMSearchReplaceDialog::ID_WXEDITTO = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONFINDNEXT = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONFINDPREV = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONCOUNT = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONREPLACE = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONREPLACEALL = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONREPLACEEXPAND = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBUTTONCLOSE = wxNewId();
//*)

const long WXMSearchReplaceDialog::ID_MADEDIT1 = wxNewId();
const long WXMSearchReplaceDialog::ID_MADEDIT2 = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBITMAPBUTTONRECENTFINDTEXT = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBITMAPBUTTONRECENTREPLACETEXT = wxNewId();

static long WXMNewID(long begin, size_t count)
{
	for(size_t i=1; i<count; ++i)
		wxRegisterId(begin + i);
	return begin + count - 1;
}

static long fid1 = wxNewId();
const long ID_RECENTFINDTEXT1 = fid1;
const long ID_RECENTFINDTEXT20 = WXMNewID(fid1, 20-1);
static long rid1 = wxNewId();
const long ID_RECENTREPLACETEXT1 = rid1;
const long ID_RECENTREPLACETEXT20 = WXMNewID(rid1, 20-1);

BEGIN_EVENT_TABLE(WXMSearchReplaceDialog,wxDialog)
	//(*EventTable(WXMSearchReplaceDialog)
	//*)
	EVT_ACTIVATE(WXMSearchReplaceDialog::WXMSearchReplaceDialogActivate)

	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, WXMSearchReplaceDialog::WxBitmapButtonRecentFindTextClick)
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTREPLACETEXT, WXMSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20, WXMSearchReplaceDialog::OnRecentFindText)
	EVT_MENU_RANGE(ID_RECENTREPLACETEXT1, ID_RECENTREPLACETEXT20, WXMSearchReplaceDialog::OnRecentReplaceText)
END_EVENT_TABLE()

WXMSearchReplaceDialog::WXMSearchReplaceDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMSearchReplaceDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxDIALOG_NO_PARENT, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
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
	WxButtonFindNext->SetDefault();
	BoxSizer3->Add(WxButtonFindNext, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonFindPrev = new wxButton(this, ID_WXBUTTONFINDPREV, _("Find &Previous"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDPREV"));
	BoxSizer3->Add(WxButtonFindPrev, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonCount = new wxButton(this, ID_WXBUTTONCOUNT, _("C&ount"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCOUNT"));
	BoxSizer3->Add(WxButtonCount, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACE"));
	BoxSizer3->Add(WxButtonReplace, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplaceAll = new wxButton(this, ID_WXBUTTONREPLACEALL, _("Replace &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACEALL"));
	BoxSizer3->Add(WxButtonReplaceAll, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplaceExpand = new wxButton(this, ID_WXBUTTONREPLACEEXPAND, _("&Replace >>"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACEEXPAND"));
	BoxSizer3->Add(WxButtonReplaceExpand, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonClose = new wxButton(this, ID_WXBUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCLOSE"));
	BoxSizer3->Add(WxButtonClose, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_WXCHECKBOXFINDHEX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxCheckBoxFindHexClick);
	Connect(ID_WXCHECKBOXSEARCHINSELECTION,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxCheckBoxSearchInSelectionClick);
	Connect(ID_WXBUTTONFINDNEXT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonFindNextClick);
	Connect(ID_WXBUTTONFINDPREV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonFindPrevClick);
	Connect(ID_WXBUTTONCOUNT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonCountClick);
	Connect(ID_WXBUTTONREPLACE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonReplaceClick);
	Connect(ID_WXBUTTONREPLACEALL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonReplaceAllClick);
	Connect(ID_WXBUTTONREPLACEEXPAND,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonReplaceExpandClick);
	Connect(ID_WXBUTTONCLOSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchReplaceDialog::WxButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMSearchReplaceDialog::WXMSearchReplaceDialogClose);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown);
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
	m_FindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	m_ReplaceText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxBitmapButtonRecentFindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxBitmapButtonRecentReplaceText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxMoveFocus->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxCaseSensitive->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxWholeWord->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxRegex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxFindHex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxCheckBoxSearchInSelection->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxStaticTextFrom->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxStaticTextTo->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxButtonFindNext->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxButtonReplace->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxButtonReplaceAll->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));
	WxButtonClose->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown));


	m_RecentFindText=new wxRecentList(true, 20, ID_RECENTFINDTEXT1);
	m_RecentFindText->UseMenu(&WxPopupMenuRecentFindText);
	m_RecentReplaceText=new wxRecentList(true, 20, ID_RECENTREPLACETEXT1);
	m_RecentReplaceText->UseMenu(&WxPopupMenuRecentReplaceText);

	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();

	m_Config->SetPath(wxT("/RecentFindText"));
	m_RecentFindText->Load(*m_Config);

	m_Config->SetPath(wxT("/RecentReplaceText"));
	m_RecentReplaceText->Load(*m_Config);

	m_Config->SetPath(oldpath);

	if(m_RecentFindText->GetCount()>0)
	{
		wxString text=m_RecentFindText->GetHistoryFile(0);
		if(!text.IsEmpty())
		{
			m_FindText->SetText(text);
		}
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

WXMSearchReplaceDialog::~WXMSearchReplaceDialog()
{
	//(*Destroy(WXMSearchReplaceDialog)
	//*)
}


void WXMSearchReplaceDialog::WXMSearchReplaceDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_SearchReplaceDialog=NULL;
	Destroy();
}

void WXMSearchReplaceDialog::WxButtonCloseClick(wxCommandEvent& event)
{
	Show(false);
}

void WXMSearchReplaceDialog::WxButtonFindNextClick(wxCommandEvent& event)
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
		m_RecentFindText->AddItemToHistory(text);

		MadSearchResult sr;
		wxFileOffset selend = g_ActiveMadEdit->GetSelectionEndPos();
		wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();

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
			if(caretpos <= from || caretpos > to || (caretpos==to && WxCheckBoxWrapAround->IsChecked()))
				rangeFrom = from;
		}

		for(;;)
		{
			if(WxCheckBoxFindHex->GetValue())
			{
				sr=g_ActiveMadEdit->FindHexNext(text, rangeFrom, rangeTo);
			}
			else
			{
				sr=g_ActiveMadEdit->FindTextNext(text,
					WxCheckBoxRegex->GetValue(),
					WxCheckBoxCaseSensitive->GetValue(),
					WxCheckBoxWholeWord->GetValue(),
					rangeFrom, rangeTo);
			}

			if(sr != SR_NO)
			{
				if(sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selend)
				{
					selend = -1;
					continue;
				}
				break;
			}

			if (WxCheckBoxWrapAround->IsChecked() && rangeTo != caretpos)
			{
				rangeTo = caretpos;
				rangeFrom = WxCheckBoxSearchInSelection->IsChecked()? from : 0;
				continue;
			}

			wxString msg(_("Cannot find the matched string."));
			wxMessageBox(msg, _("Find Next"), wxOK|wxICON_WARNING);
			break;
		}
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_ActiveMadEdit->SetFocus();
	}
}

void WXMSearchReplaceDialog::WxButtonFindPrevClick(wxCommandEvent& event)
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
		m_RecentFindText->AddItemToHistory(text);

		MadSearchResult sr;
		wxFileOffset selbeg = g_ActiveMadEdit->GetSelectionBeginPos();
		wxFileOffset caretpos = g_ActiveMadEdit->GetCaretPosition();

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

			rangeFrom = from;
			if(caretpos < from || caretpos >= to || (caretpos==from && WxCheckBoxWrapAround->IsChecked()))
				rangeTo = to;
		}

		for(;;)
		{
			if(WxCheckBoxFindHex->GetValue())
			{
				sr=g_ActiveMadEdit->FindHexPrevious(text, rangeTo, rangeFrom);
			}
			else
			{
				sr=g_ActiveMadEdit->FindTextPrevious(text,
					WxCheckBoxRegex->GetValue(),
					WxCheckBoxCaseSensitive->GetValue(),
					WxCheckBoxWholeWord->GetValue(),
					rangeTo, rangeFrom);
			}

			if(sr!=SR_NO)
			{
				if(sr == SR_YES && g_ActiveMadEdit->GetCaretPosition() == selbeg)
				{
					selbeg = -1;
					continue;
				}
				break;
			}

			if (WxCheckBoxWrapAround->IsChecked() && rangeFrom != caretpos)
			{
				rangeTo = WxCheckBoxSearchInSelection->IsChecked()? to: g_ActiveMadEdit->GetFileSize();
				rangeFrom = caretpos;
				continue;
			}

			wxString msg(_("Cannot find the matched string."));
			wxMessageBox(msg, _("Find Previous"), wxOK|wxICON_WARNING);
			break;
		}
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_ActiveMadEdit->SetFocus();
	}
}

void WXMSearchReplaceDialog::ReadWriteSettings(bool bRead)
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

void WXMSearchReplaceDialog::UpdateCheckBoxByCBHex(bool check)
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

void WXMSearchReplaceDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
	UpdateCheckBoxByCBHex(event.IsChecked());
}

void WXMSearchReplaceDialog::WXMSearchReplaceDialogKeyDown(wxKeyEvent& event)
{
	int key=event.GetKeyCode();

	//g_SearchReplaceDialog->SetTitle(wxString()<<key);

	switch(key)
	{
	case WXK_ESCAPE:
		g_SearchReplaceDialog->Show(false);
		return;
	case WXK_RETURN:
	case WXK_NUMPAD_ENTER:
		if(this->GetClassInfo()->GetClassName()!=wxString(wxT("wxButton")))
		{
			wxCommandEvent e;
			wxButton* default_btn = static_cast<wxButton*>(g_SearchReplaceDialog->GetDefaultItem());
			if(default_btn == g_SearchReplaceDialog->WxButtonReplace)
				return g_SearchReplaceDialog->WxButtonReplaceClick(e); // no skip

			return g_SearchReplaceDialog->WxButtonFindNextClick(e); // no skip
		}
		break;
	case WXK_DOWN:
		if((MadEdit*)this==g_SearchReplaceDialog->m_FindText)
		{
			int x,y,w,h;
			g_SearchReplaceDialog->m_FindText->GetPosition(&x, &y);
			g_SearchReplaceDialog->m_FindText->GetSize(&w, &h);
			g_SearchReplaceDialog->PopupMenu(&g_SearchReplaceDialog->WxPopupMenuRecentFindText, x, y+h);
			return;
		}
		else if((MadEdit*)this==g_SearchReplaceDialog->m_ReplaceText)
		{
			int x,y,w,h;
			g_SearchReplaceDialog->m_ReplaceText->GetPosition(&x, &y);
			g_SearchReplaceDialog->m_ReplaceText->GetSize(&w, &h);
			g_SearchReplaceDialog->PopupMenu(&g_SearchReplaceDialog->WxPopupMenuRecentReplaceText, x, y+h);
			return;
		}
		break;
	}

	extern wxAcceleratorEntry g_AccelFindNext, g_AccelFindPrev;
	int flags=wxACCEL_NORMAL;
	if(event.m_altDown) flags|=wxACCEL_ALT;
	if(event.m_shiftDown) flags|=wxACCEL_SHIFT;
	if(event.m_controlDown) flags|=wxACCEL_CTRL;

	if(g_AccelFindNext.GetKeyCode()==key && g_AccelFindNext.GetFlags()==flags)
	{
		wxCommandEvent e;
		g_SearchReplaceDialog->WxButtonFindNextClick(e);
		return; // no skip
	}

	if(g_AccelFindPrev.GetKeyCode()==key && g_AccelFindPrev.GetFlags()==flags)
	{
		wxCommandEvent e;
		g_SearchReplaceDialog->WxButtonFindPrevClick(e);
		return; // no skip
	}

	event.Skip();
}

void WXMSearchReplaceDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
	PopupMenu(&WxPopupMenuRecentFindText);
}

void WXMSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event)
{
	PopupMenu(&WxPopupMenuRecentReplaceText);
}

void WXMSearchReplaceDialog::OnRecentFindText(wxCommandEvent& event)
{
	int idx=event.GetId()-ID_RECENTFINDTEXT1;
	wxString text=m_RecentFindText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_FindText->SetText(text);
		m_FindText->SetFocus();
	}
}

void WXMSearchReplaceDialog::OnRecentReplaceText(wxCommandEvent& event)
{
	int idx=event.GetId()-ID_RECENTREPLACETEXT1;
	wxString text=m_RecentReplaceText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_ReplaceText->SetText(text);
		m_ReplaceText->SetFocus();
	}
}

void WXMSearchReplaceDialog::WxButtonReplaceExpandClick(wxCommandEvent& event)
{
	g_SearchReplaceDialog->ShowWithReplaceFunc();
	g_SearchReplaceDialog->SetFocus();
	g_SearchReplaceDialog->Raise();

	g_SearchReplaceDialog->UpdateCheckBoxByCBHex(g_SearchReplaceDialog->WxCheckBoxFindHex->GetValue());

	g_SearchReplaceDialog->m_FindText->SelectAll();
	g_SearchReplaceDialog->m_FindText->SetFocus();
}

void WXMSearchReplaceDialog::WxButtonReplaceClick(wxCommandEvent& event)
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

	m_RecentFindText->AddItemToHistory(text);

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

void WXMSearchReplaceDialog::WxButtonCountClick(wxCommandEvent& event)
{
	extern MadEdit *g_ActiveMadEdit;

	if(g_ActiveMadEdit==NULL)
		return;

	int count = 0;
	wxString text;
	m_FindText->GetText(text, true);

	if(text.Len()>0)
	{
		m_RecentFindText->AddItemToHistory(text);

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

		if(WxCheckBoxFindHex->GetValue())
		{
			count=g_ActiveMadEdit->FindHexAll(text, false, NULL, NULL, rangeFrom, rangeTo);
		}
		else
		{
			count=g_ActiveMadEdit->FindTextAll(text,
				WxCheckBoxRegex->GetValue(),
				WxCheckBoxCaseSensitive->GetValue(),
				WxCheckBoxWholeWord->GetValue(),
				false,
				NULL, NULL,
				rangeFrom, rangeTo);
		}
	}

	if(count >= 0)
	{
		wxString msg;
		msg.Printf(_("'%s' was found %d times."), text.c_str(), count);
		wxMessageBox(msg, wxT("wxMEdit"), wxOK);
	}
}

void WXMSearchReplaceDialog::WxButtonReplaceAllClick(wxCommandEvent& event)
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

		m_RecentFindText->AddItemToHistory(text);

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

void WXMSearchReplaceDialog::WXMSearchReplaceDialogActivate(wxActivateEvent& event)
{
	ReadWriteSettings(event.GetActive());
}

void WXMSearchReplaceDialog::UpdateSearchInSelection(bool check)
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

void WXMSearchReplaceDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
	UpdateSearchInSelection(event.IsChecked());
}

bool WXMSearchReplaceDialog::ShowOnlyFindFunc()
{
	m_ReplaceText->Show(false);
	WxBitmapButtonRecentReplaceText->Show(false);
	WxButtonReplace->Show(false);
	WxButtonReplaceAll->Show(false);

	WxButtonReplaceExpand->Show(true);

	WxButtonFindNext->SetDefault();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	SetTitle(_("Search"));

	return Show(true);
}

bool WXMSearchReplaceDialog::ShowWithReplaceFunc()
{
	m_ReplaceText->Show(true);
	WxBitmapButtonRecentReplaceText->Show(true);
	WxButtonReplace->Show(true);
	WxButtonReplaceAll->Show(true);

	WxButtonReplaceExpand->Show(false);

	WxButtonReplace->SetDefault();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	SetTitle(_("Replace"));

	return Show(true);
}
