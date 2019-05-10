///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_search_replace_dialog.cpp
// Description: Search/Replace Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_search_replace_dialog.h"

#include "../xm/cxx11.h"
#include "../wxm/edit/single_line.h"
#include "../wxm/edit/inframe.h"
#include "../wxm/status_bar.h"
#include "../xm/utils.hpp"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMSearchReplaceDialog)
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
#include "../../images/down.xpm"
#undef static

extern wxm::InFrameWXMEdit* g_active_wxmedit;

WXMSearchReplaceDialog *g_SearchReplaceDialog=nullptr;

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
const long WXMSearchReplaceDialog::ID_STATICLINE1 = wxNewId();
const long WXMSearchReplaceDialog::ID_STATICTEXTSTATUS = wxNewId();
//*)

const long WXMSearchReplaceDialog::ID_MADEDIT1 = wxNewId();
const long WXMSearchReplaceDialog::ID_MADEDIT2 = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBITMAPBUTTONRECENTFINDTEXT = wxNewId();
const long WXMSearchReplaceDialog::ID_WXBITMAPBUTTONRECENTREPLACETEXT = wxNewId();

static wxm::WXMControlIDReserver& s_idrsvr = wxm::WXMControlIDReserver::Instance();
BEGIN_EVENT_TABLE(WXMSearchReplaceDialog,wxDialog)
	//(*EventTable(WXMSearchReplaceDialog)
	//*)
	EVT_ACTIVATE(WXMSearchReplaceDialog::WXMSearchReplaceDialogActivate)

	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, WXMSearchReplaceDialog::WxBitmapButtonRecentFindTextClick)
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTREPLACETEXT, WXMSearchReplaceDialog::WxBitmapButtonRecentReplaceTextClick)
	EVT_MENU_RANGE(s_idrsvr.RecentFindTextID1(), s_idrsvr.RecentFindTextID20(), WXMSearchReplaceDialog::OnRecentFindText)
	EVT_MENU_RANGE(s_idrsvr.RecentReplaceTextID1(), s_idrsvr.RecentReplaceTextID20(), WXMSearchReplaceDialog::OnRecentReplaceText)
END_EVENT_TABLE()

WXMSearchReplaceDialog::WXMSearchReplaceDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMSearchReplaceDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND, 0);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizer5, 1, wxALL|wxEXPAND, 0);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxMoveFocus = new wxCheckBox(this, ID_WXCHECKBOXMOVEFOCUS, _("&Move Focus to Editor Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMOVEFOCUS"));
	WxCheckBoxMoveFocus->SetValue(false);
	BoxSizer6->Add(WxCheckBoxMoveFocus, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxWrapAround = new wxCheckBox(this, ID_WXCHECKBOXWRAPAROUND, _("Wrap aroun&d"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWRAPAROUND"));
	WxCheckBoxWrapAround->SetValue(true);
	BoxSizer6->Add(WxCheckBoxWrapAround, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCASESENSITIVE"));
	WxCheckBoxCaseSensitive->SetValue(false);
	BoxSizer6->Add(WxCheckBoxCaseSensitive, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWHOLEWORD"));
	WxCheckBoxWholeWord->SetValue(false);
	BoxSizer6->Add(WxCheckBoxWholeWord, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREGEX"));
	WxCheckBoxRegex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxRegex, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXFINDHEX"));
	WxCheckBoxFindHex->SetValue(false);
	BoxSizer6->Add(WxCheckBoxFindHex, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxSearchInSelection = new wxCheckBox(this, ID_WXCHECKBOXSEARCHINSELECTION, _("Search In &Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSEARCHINSELECTION"));
	WxCheckBoxSearchInSelection->SetValue(false);
	BoxSizer7->Add(WxCheckBoxSearchInSelection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextFrom = new wxStaticText(this, ID_WXSTATICTEXTFROM, _("From:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFROM"));
	BoxSizer7->Add(WxStaticTextFrom, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFrom = new wxTextCtrl(this, ID_WXEDITFROM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITFROM"));
	BoxSizer7->Add(WxEditFrom, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextTo = new wxStaticText(this, ID_WXSTATICTEXTTO, _("To:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTTO"));
	BoxSizer7->Add(WxStaticTextTo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxEditTo = new wxTextCtrl(this, ID_WXEDITTO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITTO"));
	BoxSizer7->Add(WxEditTo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer2->Add(BoxSizer6, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer8->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP, 0);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxButtonFindNext = new wxButton(this, ID_WXBUTTONFINDNEXT, _("Find &Next"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDNEXT"));
	WxButtonFindNext->SetDefault();
	BoxSizer3->Add(WxButtonFindNext, 0, wxALL|wxEXPAND, 2);
	WxButtonFindPrev = new wxButton(this, ID_WXBUTTONFINDPREV, _("Find &Previous"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDPREV"));
	BoxSizer3->Add(WxButtonFindPrev, 0, wxALL|wxEXPAND, 2);
	WxButtonCount = new wxButton(this, ID_WXBUTTONCOUNT, _("C&ount"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCOUNT"));
	BoxSizer3->Add(WxButtonCount, 0, wxALL|wxEXPAND, 2);
	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACE"));
	BoxSizer3->Add(WxButtonReplace, 0, wxALL|wxEXPAND, 2);
	WxButtonReplaceAll = new wxButton(this, ID_WXBUTTONREPLACEALL, _("Replace &All"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACEALL"));
	BoxSizer3->Add(WxButtonReplaceAll, 0, wxALL|wxEXPAND, 2);
	WxButtonReplaceExpand = new wxButton(this, ID_WXBUTTONREPLACEEXPAND, _("&Replace >>"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACEEXPAND"));
	BoxSizer3->Add(WxButtonReplaceExpand, 0, wxALL|wxEXPAND, 2);
	WxButtonClose = new wxButton(this, ID_WXBUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCLOSE"));
	BoxSizer3->Add(WxButtonClose, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer3, 0, wxALL|wxALIGN_TOP, 0);
	BoxSizer1->Add(BoxSizer8, 1, wxALL|wxEXPAND, 0);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 0, wxTOP|wxEXPAND, 2);
	StaticTextStatus = new wxStaticText(this, ID_STATICTEXTSTATUS, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTSTATUS"));
	BoxSizer1->Add(StaticTextStatus, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

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

	int bw, bh;
	// find
	WxButtonFindNext->GetSize(&bw, &bh);

	m_FindText = new wxm::SingleLineWXMEdit(this, ID_MADEDIT1, wxPoint(0, 0), wxSize(400, bh));

	BoxSizer4->Add(m_FindText,1,wxEXPAND|wxALIGN_LEFT | wxALL,2);
	BoxSizer4->SetItemMinSize(m_FindText, 400, bh);

	wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
	WxBitmapButtonRecentFindText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentFindText"));
	BoxSizer4->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	// replace
	WxButtonReplace->GetSize(&bw, &bh);

	m_ReplaceText = new wxm::SingleLineWXMEdit(this, ID_MADEDIT2, wxPoint(0, 0), wxSize(400, bh));

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


	m_RecentFindText = new wxm::wxSimpleRecentList(20, s_idrsvr.RecentFindTextID1());
	m_RecentFindText->UseMenu(&WxPopupMenuRecentFindText);
	m_RecentReplaceText = new wxm::wxSimpleRecentList(20, s_idrsvr.RecentReplaceTextID1());
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

	m_msgtypecolor_map[SMT_INFORMATION] = wxColor(wxT("Green"));
	m_msgtypecolor_map[SMT_NOTICE] = wxColor(wxT("Blue"));
	m_msgtypecolor_map[SMT_WARNING] = wxColor(wxT("Red"));
}

WXMSearchReplaceDialog::~WXMSearchReplaceDialog()
{
	//(*Destroy(WXMSearchReplaceDialog)
	//*)
	delete m_RecentReplaceText;
	delete m_RecentFindText;
}


void WXMSearchReplaceDialog::WXMSearchReplaceDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_SearchReplaceDialog=nullptr;
	Destroy();
}

void WXMSearchReplaceDialog::WxButtonCloseClick(wxCommandEvent& event)
{
	Show(false);
}

void WXMSearchReplaceDialog::WxButtonFindNextClick(wxCommandEvent& event)
{
	ResetMessage();

	if (g_active_wxmedit == nullptr)
		return;

	wxString text;
	m_FindText->GetText(text, true);

	if(WxCheckBoxFindHex->GetValue())
		text.Trim();

	if(text.Len()>0)
	{
		m_RecentFindText->AddItemToHistory(text);

		MadSearchResult sr;
		wxFileOffset selend = g_active_wxmedit->GetSelectionEndPos();
		wxFileOffset caretpos = g_active_wxmedit->GetCaretPosition();

		wxString msg_selection_end(_("End of selection reached, search from beginning."));
		wxString msg_file_end(_("End of file reached, search from beginning."));

		wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			if(!StrToInt64(WxEditFrom->GetValue(), from))
			{
				ShowMessage(_("The value of 'From' is incorrect."), SMT_WARNING);
				return;
			}
			if(!StrToInt64(WxEditTo->GetValue(), to))
			{
				ShowMessage(_("The value of 'To' is incorrect."), SMT_WARNING);
				return;
			}

			rangeTo = to;
			if (caretpos <= from || caretpos > to)
			{
				rangeFrom = from;
			}
			else if (caretpos==to && WxCheckBoxWrapAround->IsChecked())
			{
				ShowMessage(msg_selection_end);
				rangeFrom = from;
			}
		}

		wxm::WXMSearcher* searcher = g_active_wxmedit->Searcher(WxCheckBoxFindHex->GetValue(), WxCheckBoxRegex->GetValue());
		searcher->SetOption(WxCheckBoxCaseSensitive->GetValue(), WxCheckBoxWholeWord->GetValue());
		for (;;)
		{
			sr = searcher->FindNext(text, rangeFrom, rangeTo);

			if(sr != SR_NO)
			{
				if(sr == SR_YES && g_active_wxmedit->GetCaretPosition() == selend)
				{
					selend = -1;
					continue;
				}
				break;
			}

			if (WxCheckBoxWrapAround->IsChecked() && rangeTo != caretpos)
			{
				bool in_selection = WxCheckBoxSearchInSelection->IsChecked();

				ShowMessage(in_selection? msg_selection_end: msg_file_end);

				rangeTo = caretpos;
				rangeFrom = in_selection? from : 0;
				continue;
			}

			wxString msg(_("Cannot find the matched string."));
			ShowMessage(msg, SMT_WARNING);
			break;
		}
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_active_wxmedit->SetFocus();
	}
}

void WXMSearchReplaceDialog::WxButtonFindPrevClick(wxCommandEvent& event)
{
	ResetMessage();

	if (g_active_wxmedit == nullptr)
		return;

	wxString text;
	m_FindText->GetText(text, true);

	if(WxCheckBoxFindHex->GetValue())
		text.Trim();

	if(text.Len()>0)
	{
		m_RecentFindText->AddItemToHistory(text);

		MadSearchResult sr;
		wxFileOffset selbeg = g_active_wxmedit->GetSelectionBeginPos();
		wxFileOffset caretpos = g_active_wxmedit->GetCaretPosition();

		wxString msg_selection_begin(_("Beginning of selection reached, search from end."));
		wxString msg_file_begin(_("Beginning of file reached, search from end."));

		wxInt64 from = 0, to = 0;
		wxFileOffset rangeFrom = -1, rangeTo = -1;
		if(WxCheckBoxSearchInSelection->IsChecked())
		{
			if(!StrToInt64(WxEditFrom->GetValue(), from))
			{
				ShowMessage(_("The value of 'From' is incorrect."), SMT_WARNING);
				return;
			}
			if(!StrToInt64(WxEditTo->GetValue(), to))
			{
				ShowMessage(_("The value of 'To' is incorrect."), SMT_WARNING);
				return;
			}

			rangeFrom = from;
			if (caretpos < from || caretpos >= to)
			{
				rangeTo = to;
			}
			else if (caretpos==from && WxCheckBoxWrapAround->IsChecked())
			{
				ShowMessage(msg_selection_begin);
				rangeTo = to;
			}
		}

		wxm::WXMSearcher* searcher = g_active_wxmedit->Searcher(WxCheckBoxFindHex->GetValue(), WxCheckBoxRegex->GetValue());
		searcher->SetOption(WxCheckBoxCaseSensitive->GetValue(), WxCheckBoxWholeWord->GetValue());
		for (;;)
		{
			sr = searcher->FindPrevious(text, rangeTo, rangeFrom);

			if(sr!=SR_NO)
			{
				if(sr == SR_YES && g_active_wxmedit->GetCaretPosition() == selbeg)
				{
					selbeg = -1;
					continue;
				}
				break;
			}

			if (WxCheckBoxWrapAround->IsChecked() && rangeFrom != caretpos)
			{
				bool in_selection = WxCheckBoxSearchInSelection->IsChecked();

				ShowMessage(in_selection? msg_selection_begin: msg_file_begin);

				rangeTo = in_selection? to: g_active_wxmedit->GetFileSize();
				rangeFrom = caretpos;
				continue;
			}

			wxString msg(_("Cannot find the matched string."));
			ShowMessage(msg, SMT_WARNING);
			break;
		}
	}

	if(WxCheckBoxMoveFocus->GetValue())
	{
		((wxFrame*)wxTheApp->GetTopWindow())->Raise();
		g_active_wxmedit->SetFocus();
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
	int idx = event.GetId() - s_idrsvr.RecentFindTextID1();
	wxString text=m_RecentFindText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_FindText->SetText(text);
		m_FindText->SetFocus();
	}
}

void WXMSearchReplaceDialog::OnRecentReplaceText(wxCommandEvent& event)
{
	int idx = event.GetId() - s_idrsvr.RecentReplaceTextID1();
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

	g_SearchReplaceDialog->UpdateCheckBoxByCBHex();

	g_SearchReplaceDialog->m_FindText->SelectAll();
	g_SearchReplaceDialog->m_FindText->SetFocus();
}

void WXMSearchReplaceDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
	ResetMessage();

	if (g_active_wxmedit == nullptr)
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

	wxString msg_selection_end(_("End of selection reached, replace from beginning."));
	wxString msg_file_end(_("End of file reached, replace from beginning."));

	wxInt64 from = 0, to = 0;
	wxFileOffset rangeFrom = -1, rangeTo = -1;
	wxFileOffset caretpos = g_active_wxmedit->GetCaretPosition();
	if(WxCheckBoxSearchInSelection->IsChecked())
	{
		if(!StrToInt64(WxEditFrom->GetValue(), from))
		{
			ShowMessage(_("The value of 'From' is incorrect."), SMT_WARNING);
			return;
		}
		if(!StrToInt64(WxEditTo->GetValue(), to))
		{
			ShowMessage(_("The value of 'To' is incorrect."), SMT_WARNING);
			return;
		}

		rangeTo = to;
		if (caretpos <= from || caretpos > to)
		{
			rangeFrom = from;
		}
		else if (caretpos==to && WxCheckBoxWrapAround->IsChecked())
		{
			ShowMessage(msg_selection_end);
			rangeFrom = from;
		}
	}

	MadReplaceResult ret=RR_EXPR_ERROR;

	wxm::WXMSearcher* searcher = g_active_wxmedit->Searcher(WxCheckBoxFindHex->GetValue(), WxCheckBoxRegex->GetValue());
	searcher->SetOption(WxCheckBoxCaseSensitive->GetValue(), WxCheckBoxWholeWord->GetValue());
	for (;;)
	{
		ret = searcher->ReplaceOnce(text, reptext, rangeFrom, rangeTo);

		if ((ret==RR_REP_NNEXT || ret==RR_NREP_NNEXT) &&
			WxCheckBoxWrapAround->IsChecked() && rangeTo != caretpos)
		{
			bool in_selection = WxCheckBoxSearchInSelection->IsChecked();

			ShowMessage(in_selection? msg_selection_end: msg_file_end);

			rangeTo = caretpos;
			rangeFrom = in_selection? from : 0;
			continue;
		}

		break;
	}

	switch(ret)
	{
	case RR_REP_NNEXT:
		{
			ShowMessage(_("Cannot find the matched string. Replace is finished."), SMT_NOTICE);
			m_FindText->SetFocus();
		}
		break;
	case RR_NREP_NNEXT:
		{
			ShowMessage(_("Cannot find the matched string."), SMT_WARNING);
			m_FindText->SetFocus();
		}
		break;
	case RR_REP_NEXT:
	case RR_NREP_NEXT:
		if(WxCheckBoxMoveFocus->GetValue())
		{
			((wxFrame*)wxTheApp->GetTopWindow())->Raise();
			g_active_wxmedit->SetFocus();
		}
		break;
	default:
		break;
	}
}

void WXMSearchReplaceDialog::WxButtonCountClick(wxCommandEvent& event)
{
	ResetMessage();

	if (g_active_wxmedit == nullptr)
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
				ShowMessage(_("The value of 'From' is incorrect."), SMT_WARNING);
				return;
			}
			if(!StrToInt64(WxEditTo->GetValue(), to))
			{
				ShowMessage(_("The value of 'To' is incorrect."), SMT_WARNING);
				return;
			}

			rangeTo = to;
			rangeFrom = from;
		}

		wxm::WXMSearcher* searcher = g_active_wxmedit->Searcher(WxCheckBoxFindHex->GetValue(), WxCheckBoxRegex->GetValue());
		searcher->SetOption(WxCheckBoxCaseSensitive->GetValue(), WxCheckBoxWholeWord->GetValue());

		count = searcher->FindAll(text, false, nullptr, nullptr, rangeFrom, rangeTo);
	}

	if(count >= 0)
	{
		wxString msg;
		msg.Printf(_("'%s' was found %d times."), text.c_str(), count);
		ShowMessage(msg, SMT_NOTICE);
	}
}

void WXMSearchReplaceDialog::WxButtonReplaceAllClick(wxCommandEvent& event)
{
	ResetMessage();

	if( g_active_wxmedit == nullptr)
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
				ShowMessage(_("The value of 'From' is incorrect."), SMT_WARNING);
				return;
			}
			if(!StrToInt64(WxEditTo->GetValue(), to))
			{
				ShowMessage(_("The value of 'To' is incorrect."), SMT_WARNING);
				return;
			}

			rangeTo = to;
			rangeFrom = from;
		}

		int count=0;
		wxm::WXMSearcher* searcher = g_active_wxmedit->Searcher(WxCheckBoxFindHex->GetValue(), WxCheckBoxRegex->GetValue());
		searcher->SetOption(WxCheckBoxCaseSensitive->GetValue(), WxCheckBoxWholeWord->GetValue());

		count = searcher->ReplaceAll(text, reptext, nullptr, nullptr, rangeFrom, rangeTo);

		if(count>=0)
		{
			wxString msg;
			if(count==0)
				ShowMessage(_("Cannot find any matched string."), SMT_WARNING);
			else
				ShowMessage(wxString::Format(_("%d string(s) were replaced."), count), SMT_NOTICE);
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

	if (check && g_active_wxmedit!=nullptr)
	{
		WxEditFrom->SetValue(wxLongLong(g_active_wxmedit->GetSelectionBeginPos()).ToString());
		WxEditTo->SetValue(wxLongLong(g_active_wxmedit->GetSelectionEndPos()).ToString());
	}
}

void WXMSearchReplaceDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
	UpdateSearchInSelection(event.IsChecked());
}

void WXMSearchReplaceDialog::ShowOnlyFindFunc()
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

	Show(true);
	SetFocus();
	Raise();
}

void WXMSearchReplaceDialog::ShowWithReplaceFunc()
{
	m_ReplaceText->SetFont(m_FindText->GetFont());
	m_ReplaceText->Show(true);
	WxBitmapButtonRecentReplaceText->Show(true);
	WxButtonReplace->Show(true);
	WxButtonReplaceAll->Show(true);

	WxButtonReplaceExpand->Show(false);

	WxButtonReplace->SetDefault();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	SetTitle(_("Replace"));

	Show(true);
	SetFocus();
	Raise();
}

wxColor WXMSearchReplaceDialog::GetMessageColor(WXMSearchReplaceDialog::SearchMsgType type)
{
	return xm::wrap_map(m_msgtypecolor_map).get(type, wxColor());
}

void WXMSearchReplaceDialog::ResetMessage()
{
	StaticTextStatus->SetLabel(wxString());

	wxm::GetFrameStatusBar().SetField(wxm::STBF_HELP, wxEmptyString);
}

void WXMSearchReplaceDialog::ShowMessage(const wxString &msg, WXMSearchReplaceDialog::SearchMsgType type)
{
	if (type != SMT_INFORMATION)
		wxBell();
	StaticTextStatus->SetForegroundColour(GetMessageColor(type));
	StaticTextStatus->SetLabel(msg);

	if (!this->IsShown())
		wxm::GetFrameStatusBar().SetField(wxm::STBF_HELP, msg);
}

void WXMSearchReplaceDialog::PurgeRecentFindTexts()
{
	int n = (int) m_RecentFindText->GetCount();
	for(int i=n-1; i>=0; --i)
		m_RecentFindText->RemoveFileFromHistory((size_t)i);
}

void WXMSearchReplaceDialog::PurgeRecentReplaceTexts()
{
	int n = (int) m_RecentReplaceText->GetCount();
	for(int i=n-1; i>=0; --i)
		m_RecentReplaceText->RemoveFileFromHistory((size_t)i);
}

WXMSearchReplaceDialog& GetSearchReplaceDialog(wxWindow* parent)
{
	if (g_SearchReplaceDialog == nullptr)
		g_SearchReplaceDialog = new WXMSearchReplaceDialog(parent);
	return *g_SearchReplaceDialog;
}
