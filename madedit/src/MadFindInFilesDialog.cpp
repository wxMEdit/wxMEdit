///////////////////////////////////////////////////////////////////////////////
// Name:        MadFindInFilesDialog.cpp
// Description: find or replace in files dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadFindInFilesDialog.h"

#include "MadEdit/MadEdit.h"
#include <wx/progdlg.h>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

#include "../images/down.xpm"

MadFindInFilesDialog *g_FindInFilesDialog=NULL;

list<wxString> g_FileNameList; // the filenames matched the filename filter

//----------------------------------------------------------------------------
// MadFindInFilesDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadFindInFilesDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadFindInFilesDialog::MadFindInFilesDialogClose)
	EVT_BUTTON(ID_WXBUTTONREPLACE,MadFindInFilesDialog::WxButtonReplaceClick)
	EVT_BUTTON(ID_WXBUTTONFIND,MadFindInFilesDialog::WxButtonFindClick)
	EVT_CHECKBOX(ID_WXCHECKBOXFINDHEX,MadFindInFilesDialog::WxCheckBoxFindHexClick)
END_EVENT_TABLE()
////Event Table End

MadFindInFilesDialog::MadFindInFilesDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
}

MadFindInFilesDialog::~MadFindInFilesDialog() {} 

//static int gs_MinX=0;

static void ResizeItem(wxBoxSizer* sizer, wxWindow *item, int ax, int ay)
{
    int x, y;
    wxString str=item->GetLabel();
    item->GetTextExtent(str, &x, &y);
    item->SetSize(x+=ax, y+=ay);
    sizer->SetItemMinSize(item, x, y);
    
    //wxPoint pos=item->GetPosition();
    //if(pos.x + x > gs_MinX) gs_MinX = pos.x + x;
}

void MadFindInFilesDialog::CreateGUIControls(void)
{
    //Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxBoxSizer* WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(TRUE);

	wxBoxSizer* WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2,1,wxGROW | wxALL,5);

	wxBoxSizer* WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4,1,wxALIGN_TOP | wxALL,5);

	wxBoxSizer* WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer6,0,wxALIGN_LEFT | wxALL,5);

	wxBoxSizer* WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer7,0,wxALIGN_LEFT | wxALL,5);

	wxBoxSizer* WxBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer4->Add(WxBoxSizer8,1,wxALIGN_LEFT | wxALL,5);

	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxPoint(5,5), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxCaseSensitive"));
	WxBoxSizer8->Add(WxCheckBoxCaseSensitive,0,wxALIGN_LEFT | wxALL,3);

	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxPoint(5,37), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxWholeWord"));
	WxBoxSizer8->Add(WxCheckBoxWholeWord,0,wxALIGN_LEFT | wxALL,3);

	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxPoint(5,69), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxRegex"));
	WxBoxSizer8->Add(WxCheckBoxRegex,0,wxALIGN_LEFT | wxALL,3);

	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxPoint(5,101), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxFindHex"));
	WxBoxSizer8->Add(WxCheckBoxFindHex,0,wxALIGN_LEFT | wxALL,3);

	wxBoxSizer* WxBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer5,0,wxALIGN_TOP | wxALL,5);

	WxButtonFind = new wxButton(this, ID_WXBUTTONFIND, _("&Find"), wxPoint(5,5), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonFind"));
	WxBoxSizer5->Add(WxButtonFind,0,wxALIGN_LEFT | wxALL,5);

	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxPoint(5,43), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonReplace"));
	WxBoxSizer5->Add(WxButtonReplace,0,wxALIGN_LEFT | wxALL,5);

	WxButtonClose = new wxButton(this, wxID_CANCEL, _("Close"), wxPoint(5,81), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonClose"));
	WxBoxSizer5->Add(WxButtonClose,0,wxALIGN_LEFT | wxALL,5);

	wxBoxSizer* WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	this->SetTitle(_("Find/Replace In Files"));
	this->Center();
	this->SetIcon(wxNullIcon);
	
	////GUI Items Creation End

    int bw, bh;
    // find
    WxButtonFind->GetSize(&bw, &bh);

    m_FindText=new MadEdit(this, ID_MADEDIT1, wxPoint(0, 0), wxSize(400, bh));
    m_FindText->SetSingleLineMode(true);
    m_FindText->SetEncoding(wxT("UTF-32LE"));
    m_FindText->SetFixedWidthMode(false);
    m_FindText->SetRecordCaretMovements(false);
    m_FindText->SetInsertSpacesInsteadOfTab(false);
    m_FindText->SetWantTab(false);
    
    WxBoxSizer6->Add(m_FindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);
    WxBoxSizer6->SetItemMinSize(m_FindText, 400, bh);
    
    wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
	WxBitmapButtonRecentFindText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentFindText"));
    WxBoxSizer6->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);

    // replace
    WxButtonReplace->GetSize(&bw, &bh);
    
    m_ReplaceText=new MadEdit(this, ID_MADEDIT2, wxPoint(0, 0), wxSize(400, bh));
    m_ReplaceText->SetSingleLineMode(true);
    m_ReplaceText->SetEncoding(wxT("UTF-32LE"));
    m_ReplaceText->SetFixedWidthMode(false);
    m_ReplaceText->SetRecordCaretMovements(false);
    m_ReplaceText->SetInsertSpacesInsteadOfTab(false);
    m_ReplaceText->SetWantTab(false);
    WxBoxSizer7->Add(m_ReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);
    WxBoxSizer7->SetItemMinSize(m_ReplaceText, 400, bh);
    
    WxBitmapButtonRecentReplaceText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTREPLACETEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentReplaceText"));
    WxBoxSizer7->Add(WxBitmapButtonRecentReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);

    // resize checkbox
    ResizeItem(WxBoxSizer8, WxCheckBoxCaseSensitive, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxWholeWord, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxRegex, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxFindHex, 25, 4);

    GetSizer()->Fit(this);
    

    WxButtonClose->SetFocus();
}

void MadFindInFilesDialog::MadFindInFilesDialogClose(wxCloseEvent& event)
{
    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }
    
    g_FindInFilesDialog=NULL;
    Destroy();
}


/*
 * WxCheckBoxFindHexClick
 */
void MadFindInFilesDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
	// insert your code here
}

/*
 * WxButtonFindClick
 */
void MadFindInFilesDialog::WxButtonFindClick(wxCommandEvent& event)
{
    int max=10;
    wxProgressDialog dialog(_T("Progress dialog example"),
                            _T("An informative message"),
                            max,    // range
                            this,   // parent
                            wxPD_CAN_ABORT |
                            wxPD_AUTO_HIDE |
                            wxPD_APP_MODAL);


    bool cont = true;
    for ( int i = 0; i <= max && cont; i++ )
    {
        wxSleep(1);
        if ( i == max )
        {
            cont = dialog.Update(i, _T("That's all, folks!"));
        }
        else if ( i == max / 2 )
        {
            cont = dialog.Update(i, _T("Only a half left (very long message)!"));
        }
        else
        {
            cont = dialog.Update(i);
        }
    }
}

/*
 * WxButtonReplaceClick
 */
void MadFindInFilesDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
	// insert your code here
}
