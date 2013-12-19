///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_search_dialog.cpp
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_search_dialog.h"
#include "wxm_replace_dialog.h"

#include "../wxmedit/wxmedit.h"

//(*InternalHeaders(WXMSearchDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#define static static const
#include "../../images/down.xpm"
#undef static

WXMSearchDialog *g_SearchDialog=NULL;

//(*IdInit(WXMSearchDialog)
const long WXMSearchDialog::ID_WXCHECKBOXMOVEFOCUS = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXWRAPAROUND = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXCASESENSITIVE = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXWHOLEWORD = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXREGEX = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXFINDHEX = wxNewId();
const long WXMSearchDialog::ID_WXCHECKBOXSEARCHINSELECTION = wxNewId();
const long WXMSearchDialog::ID_WXSTATICTEXTFROM = wxNewId();
const long WXMSearchDialog::ID_WXEDITFROM = wxNewId();
const long WXMSearchDialog::ID_WXSTATICTEXTTO = wxNewId();
const long WXMSearchDialog::ID_WXEDITTO = wxNewId();
const long WXMSearchDialog::ID_WXBUTTONFINDNEXT = wxNewId();
const long WXMSearchDialog::ID_WXBUTTONFINDPREV = wxNewId();
const long WXMSearchDialog::ID_WXBUTTONCOUNT = wxNewId();
const long WXMSearchDialog::ID_WXBUTTONREPLACE = wxNewId();
const long WXMSearchDialog::ID_WXBUTTONCLOSE = wxNewId();
//*)

const long WXMSearchDialog::ID_MADEDIT = wxNewId();
const long WXMSearchDialog::ID_WXBITMAPBUTTONRECENTFINDTEXT = wxNewId();

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

BEGIN_EVENT_TABLE(WXMSearchDialog,wxDialog)
	//(*EventTable(WXMSearchDialog)
	//*)
	EVT_ACTIVATE(WXMSearchDialog::WXMSearchDialogActivate)

	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, WXMSearchDialog::WxBitmapButtonRecentFindTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20, WXMSearchDialog::OnRecentFindText)
END_EVENT_TABLE()

WXMSearchDialog::WXMSearchDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WXMSearchDialog)
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;

	Create(parent, id, _("Search"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxMoveFocus = new wxCheckBox(this, ID_WXCHECKBOXMOVEFOCUS, _("&Move Focus to Editor Window"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMOVEFOCUS"));
	WxCheckBoxMoveFocus->SetValue(false);
	BoxSizer5->Add(WxCheckBoxMoveFocus, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWrapAround = new wxCheckBox(this, ID_WXCHECKBOXWRAPAROUND, _("Wrap ar&ound"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWRAPAROUND"));
	WxCheckBoxWrapAround->SetValue(true);
	BoxSizer5->Add(WxCheckBoxWrapAround, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCASESENSITIVE"));
	WxCheckBoxCaseSensitive->SetValue(false);
	BoxSizer5->Add(WxCheckBoxCaseSensitive, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWHOLEWORD"));
	WxCheckBoxWholeWord->SetValue(false);
	BoxSizer5->Add(WxCheckBoxWholeWord, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXREGEX"));
	WxCheckBoxRegex->SetValue(false);
	BoxSizer5->Add(WxCheckBoxRegex, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXFINDHEX"));
	WxCheckBoxFindHex->SetValue(false);
	BoxSizer5->Add(WxCheckBoxFindHex, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxSearchInSelection = new wxCheckBox(this, ID_WXCHECKBOXSEARCHINSELECTION, _("Search In &Selection"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSEARCHINSELECTION"));
	WxCheckBoxSearchInSelection->SetValue(false);
	BoxSizer6->Add(WxCheckBoxSearchInSelection, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextFrom = new wxStaticText(this, ID_WXSTATICTEXTFROM, _("From:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFROM"));
	BoxSizer6->Add(WxStaticTextFrom, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFrom = new wxTextCtrl(this, ID_WXEDITFROM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITFROM"));
	BoxSizer6->Add(WxEditFrom, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxStaticTextTo = new wxStaticText(this, ID_WXSTATICTEXTTO, _("To:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTTO"));
	BoxSizer6->Add(WxStaticTextTo, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditTo = new wxTextCtrl(this, ID_WXEDITTO, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITTO"));
	BoxSizer6->Add(WxEditTo, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5->Add(BoxSizer6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer2->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxButtonFindNext = new wxButton(this, ID_WXBUTTONFINDNEXT, _("Find &Next"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDNEXT"));
	WxButtonFindNext->SetDefault();
	BoxSizer3->Add(WxButtonFindNext, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonFindPrev = new wxButton(this, ID_WXBUTTONFINDPREV, _("Find &Previous"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFINDPREV"));
	BoxSizer3->Add(WxButtonFindPrev, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonCount = new wxButton(this, ID_WXBUTTONCOUNT, _("C&ount"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCOUNT"));
	BoxSizer3->Add(WxButtonCount, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace >>"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONREPLACE"));
	BoxSizer3->Add(WxButtonReplace, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonClose = new wxButton(this, ID_WXBUTTONCLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCLOSE"));
	BoxSizer3->Add(WxButtonClose, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_WXCHECKBOXFINDHEX,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxCheckBoxFindHexClick);
	Connect(ID_WXCHECKBOXSEARCHINSELECTION,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxCheckBoxSearchInSelectionClick);
	Connect(ID_WXBUTTONFINDNEXT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxButtonFindNextClick);
	Connect(ID_WXBUTTONFINDPREV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxButtonFindPrevClick);
	Connect(ID_WXBUTTONCOUNT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxButtonCountClick);
	Connect(ID_WXBUTTONREPLACE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxButtonReplaceClick);
	Connect(ID_WXBUTTONCLOSE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMSearchDialog::WxButtonCloseClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMSearchDialog::WXMSearchDialogClose);
	Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&WXMSearchDialog::WXMSearchDialogKeyDown);
	//*)

	this->SetPosition(wxPoint(300,100));

	int bw, bh;
	WxButtonFindNext->GetSize(&bw, &bh);

	m_FindText=new MadEdit(this, ID_MADEDIT, wxPoint(0, 0), wxSize(400, bh));
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

	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	// connect to KeyDown event handler
	m_FindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxBitmapButtonRecentFindText->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxMoveFocus->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxCaseSensitive->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxWholeWord->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxRegex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxFindHex->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxCheckBoxSearchInSelection->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxEditFrom->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxEditTo->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxButtonFindNext->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxButtonFindPrev->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxButtonCount->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));
	WxButtonClose->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(WXMSearchDialog::WXMSearchDialogKeyDown));


	m_RecentFindText=new wxRecentList(true, 20, ID_RECENTFINDTEXT1);
	m_RecentFindText->UseMenu(&WxPopupMenuRecentFindText);

	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();
	m_Config->SetPath(wxT("/RecentFindText"));
	m_RecentFindText->Load(*m_Config);
	m_Config->SetPath(oldpath);

	if(m_RecentFindText->GetCount()>0)
	{
		wxString text=m_RecentFindText->GetHistoryFile(0);
		if(!text.IsEmpty())
		{
			m_FindText->SetText(text);
		}
	}

}

WXMSearchDialog::~WXMSearchDialog()
{
	//(*Destroy(WXMSearchDialog)
	//*)
}


void WXMSearchDialog::WXMSearchDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_SearchDialog=NULL;
	Destroy();
}

void WXMSearchDialog::WxButtonCloseClick(wxCommandEvent& event)
{
	Show(false);
}

void WXMSearchDialog::WxButtonFindNextClick(wxCommandEvent& event)
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
			if(caretpos <= from || caretpos > to)
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

void WXMSearchDialog::WxButtonFindPrevClick(wxCommandEvent& event)
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
			if(caretpos < from || caretpos >= to)
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

void WXMSearchDialog::WXMSearchDialogKeyDown(wxKeyEvent& event)
{
	int key=event.GetKeyCode();

	//g_SearchDialog->SetTitle(wxString()<<key);

	switch(key)
	{
	case WXK_ESCAPE:
		g_SearchDialog->Show(false);
		return;
	case WXK_RETURN:
	case WXK_NUMPAD_ENTER:
		if((wxButton*)this!=g_SearchDialog->WxButtonFindNext &&
		   (wxButton*)this!=g_SearchDialog->WxButtonFindPrev &&
		   (wxButton*)this!=g_SearchDialog->WxButtonClose)
		{
			wxCommandEvent e;
			g_SearchDialog->WxButtonFindNextClick(e);
			return; // no skip
		}
		break;
	case WXK_DOWN:
		if((MadEdit*)this==g_SearchDialog->m_FindText)
		{
			int x,y,w,h;
			g_SearchDialog->m_FindText->GetPosition(&x, &y);
			g_SearchDialog->m_FindText->GetSize(&w, &h);
			g_SearchDialog->PopupMenu(&g_SearchDialog->WxPopupMenuRecentFindText, x, y+h);
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
		g_SearchDialog->WxButtonFindNextClick(e);
		return; // no skip
	}

	if(g_AccelFindPrev.GetKeyCode()==key && g_AccelFindPrev.GetFlags()==flags)
	{
		wxCommandEvent e;
		g_SearchDialog->WxButtonFindPrevClick(e);
		return; // no skip
	}

	event.Skip();
}



void WXMSearchDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
	PopupMenu(&WxPopupMenuRecentFindText);
}

void WXMSearchDialog::OnRecentFindText(wxCommandEvent& event)
{
	int idx=event.GetId()-ID_RECENTFINDTEXT1;
	wxString text=m_RecentFindText->GetHistoryFile(idx);
	if(!text.IsEmpty())
	{
		m_FindText->SetText(text);
		m_FindText->SetFocus();
	}
}

void WXMSearchDialog::ReadWriteSettings(bool bRead)
{
	wxConfigBase *m_Config=wxConfigBase::Get(false);
	wxString oldpath=m_Config->GetPath();

	if(bRead)
	{
		bool bb;
		m_Config->Read(wxT("/wxMEdit/SearchMoveFocus"), &bb, false);
		WxCheckBoxMoveFocus->SetValue(bb);

		m_Config->Read(wxT("/wxMEdit/SearchWrapAround"), &bb, false);
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

void WXMSearchDialog::UpdateCheckBoxByCBHex(bool check)
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

void WXMSearchDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
	UpdateCheckBoxByCBHex(event.IsChecked());
}

void WXMSearchDialog::WXMSearchDialogActivate(wxActivateEvent& event)
{
	ReadWriteSettings(event.GetActive());
}

void WXMSearchDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
	wxString fname;
	int fsize;
	m_FindText->GetFont(fname, fsize);
	g_ReplaceDialog->m_FindText->SetFont(fname, 14);
	g_ReplaceDialog->m_ReplaceText->SetFont(fname, 14);

	wxString text;
	m_FindText->GetText(text, true);
	g_ReplaceDialog->m_FindText->SetText(text);

	this->Show(false);
	g_ReplaceDialog->Show();
	g_ReplaceDialog->SetFocus();
	g_ReplaceDialog->Raise();

	g_ReplaceDialog->UpdateCheckBoxByCBHex(g_ReplaceDialog->WxCheckBoxFindHex->GetValue());

	g_ReplaceDialog->m_FindText->SelectAll();
	g_ReplaceDialog->m_FindText->SetFocus();

}

void WXMSearchDialog::UpdateSearchInSelection(bool check)
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

void WXMSearchDialog::WxCheckBoxSearchInSelectionClick(wxCommandEvent& event)
{
	UpdateSearchInSelection(event.IsChecked());
}

void WXMSearchDialog::WxButtonCountClick(wxCommandEvent& event)
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

