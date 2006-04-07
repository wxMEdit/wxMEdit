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
	WxBoxSizer1->Add(WxBoxSizer2,0,wxGROW | wxALL,0);

	wxBoxSizer* WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4,0,wxALIGN_TOP | wxALL,5);

	wxBoxSizer* WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer6,0,wxALIGN_LEFT | wxALL,5);

	wxBoxSizer* WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer7,0,wxALIGN_LEFT | wxALL,5);

	wxBoxSizer* WxBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer4->Add(WxBoxSizer8,0,wxALIGN_LEFT | wxALL,5);

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

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(164,213), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine1,0,wxGROW | wxALL,2);

	wxBoxSizer* WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3,0,wxGROW | wxALL,0);

	WxRadioButton1 = new wxRadioButton(this, ID_WXRADIOBUTTON1, _("In The Opened Files"), wxPoint(177,0), wxSize(113,20), 0, wxDefaultValidator, _("WxRadioButton1"));
	WxBoxSizer3->Add(WxRadioButton1,0,wxALIGN_LEFT | wxALL,3);

	wxFlexGridSizer* WxFlexGridSizer1 = new wxFlexGridSizer(4,3,0,0);
	WxBoxSizer3->Add(WxFlexGridSizer1,0,wxGROW | wxALL,0);

	WxRadioButton2 = new wxRadioButton(this, ID_WXRADIOBUTTON2, _("Selected Directory:"), wxPoint(0,0), wxSize(113,20), 0, wxDefaultValidator, _("WxRadioButton2"));
	WxFlexGridSizer1->Add(WxRadioButton2,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,3);

	wxArrayString arrayStringFor_WxComboBoxDir;
	WxComboBoxDir = new wxComboBox(this, ID_WXCOMBOBOXDIR, _(""), wxPoint(113,0), wxSize(330,21), arrayStringFor_WxComboBoxDir, 0, wxDefaultValidator, _("WxComboBoxDir"));
	WxFlexGridSizer1->Add(WxComboBoxDir,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxButtonDir = new wxButton(this, ID_WXBUTTONDIR, _("..."), wxPoint(443,0), wxSize(25,21), 0, wxDefaultValidator, _("WxButtonDir"));
	WxFlexGridSizer1->Add(WxButtonDir,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("File Filters:"), wxPoint(30,21), wxSize(53,20), 0, _("WxStaticText1"));
	WxFlexGridSizer1->Add(WxStaticText1,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,3);

	wxArrayString arrayStringFor_WxComboBoxFilter;
	WxComboBoxFilter = new wxComboBox(this, ID_WXCOMBOBOXFILTER, _(""), wxPoint(113,21), wxSize(330,21), arrayStringFor_WxComboBoxFilter, 0, wxDefaultValidator, _("WxComboBoxFilter"));
	WxFlexGridSizer1->Add(WxComboBoxFilter,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _(""), wxPoint(453,29), wxSize(4,4), 0, _("WxStaticText2"));
	WxFlexGridSizer1->Add(WxStaticText2,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("File Encoding:"), wxPoint(21,42), wxSize(71,20), 0, _("WxStaticText3"));
	WxFlexGridSizer1->Add(WxStaticText3,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,3);

	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, _(""), wxPoint(178,42), wxSize(200,21), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxEncoding"));
	WxFlexGridSizer1->Add(WxComboBoxEncoding,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxCheckBoxSubDir = new wxCheckBox(this, ID_WXCHECKBOXSUBDIR, _("Include Subdirectories"), wxPoint(84,83), wxSize(300,20), 0, wxDefaultValidator, _("WxCheckBoxSubDir"));
	WxBoxSizer3->Add(WxCheckBoxSubDir,0,wxALIGN_LEFT | wxALL,3);

	WxStaticLine2 = new wxStaticLine(this, ID_WXSTATICLINE2, wxPoint(164,343), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine2,0,wxGROW | wxALL,2);

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(178,360), wxSize(121,160), arrayStringFor_WxListBox1, wxLB_SINGLE);
	WxBoxSizer1->Add(WxListBox1,1,wxGROW | wxALL,2);

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

    ResizeItem(WxBoxSizer3, WxRadioButton1, 25, 4);
    ResizeItem(WxBoxSizer3, WxRadioButton2, 25, 4);
    ResizeItem(WxBoxSizer3, WxStaticText1, 2, 2);
    ResizeItem(WxBoxSizer3, WxStaticText3, 2, 2);
    ResizeItem(WxBoxSizer3, WxCheckBoxSubDir, 25, 4);

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
