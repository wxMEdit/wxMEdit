///////////////////////////////////////////////////////////////////////////////
// Name:        MadFindInFilesDialog.cpp
// Description: find or replace in files dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadFindInFilesDialog.h"
#include "MadEditFrame.h"
#include "MadReplaceDialog.h"
#include "MadSearchDialog.h"
#include "MadEdit/MadEdit.h"

#include <wx/progdlg.h>
#include <wx/dir.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif


#include "../images/down.xpm"

MadFindInFilesDialog *g_FindInFilesDialog=NULL;

//----------------------------------------------------------------------------
// MadFindInFilesDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadFindInFilesDialog,wxDialog)
	////Manual Code Start
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTFINDTEXT, MadFindInFilesDialog::WxBitmapButtonRecentFindTextClick)
	EVT_BUTTON(ID_WXBITMAPBUTTONRECENTREPLACETEXT, MadFindInFilesDialog::WxBitmapButtonRecentReplaceTextClick)
	EVT_MENU_RANGE(ID_RECENTFINDTEXT1, ID_RECENTFINDTEXT20, MadFindInFilesDialog::OnRecentFindText)
	EVT_MENU_RANGE(ID_RECENTREPLACETEXT1, ID_RECENTREPLACETEXT20, MadFindInFilesDialog::OnRecentReplaceText)
	////Manual Code End
	
	EVT_CLOSE(MadFindInFilesDialog::MadFindInFilesDialogClose)
	EVT_ACTIVATE(MadFindInFilesDialog::MadFindInFilesDialogActivate)
	EVT_BUTTON(ID_WXBUTTONACTIVEDIR,MadFindInFilesDialog::WxButtonActiveDirClick)
	EVT_BUTTON(ID_WXBUTTONDIR,MadFindInFilesDialog::WxButtonDirClick)
	EVT_CHECKBOX(ID_WXCHECKBOXENABLEREPLACE,MadFindInFilesDialog::WxCheckBoxEnableReplaceClick)
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

MadFindInFilesDialog::~MadFindInFilesDialog()
{
    delete m_RecentFindDir;
    delete m_RecentFindFilter;
    delete m_RecentFindExclude;
} 

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

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer6, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer7, 0, wxALIGN_CENTER | wxALL, 0);

	WxBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer4->Add(WxBoxSizer8, 0, wxALIGN_LEFT | wxALIGN_TOP | wxALL, 0);

	WxCheckBoxCaseSensitive = new wxCheckBox(this, ID_WXCHECKBOXCASESENSITIVE, _("&Case Sensitive"), wxPoint(1,1), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxCaseSensitive"));
	WxBoxSizer8->Add(WxCheckBoxCaseSensitive,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxPoint(1,25), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxWholeWord"));
	WxBoxSizer8->Add(WxCheckBoxWholeWord,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxPoint(1,49), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxRegex"));
	WxBoxSizer8->Add(WxCheckBoxRegex,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxPoint(1,73), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxFindHex"));
	WxBoxSizer8->Add(WxCheckBoxFindHex,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxListFirstOnly = new wxCheckBox(this, ID_WXCHECKBOXLISTFIRSTONLY, _("&List the First Found Item Only"), wxPoint(1,97), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxListFirstOnly"));
	WxBoxSizer8->Add(WxCheckBoxListFirstOnly,0,wxALIGN_LEFT | wxALL,1);

	WxBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer5, 0, wxALIGN_TOP | wxALL, 0);

	WxButtonFind = new wxButton(this, ID_WXBUTTONFIND, _("&Find"), wxPoint(2,2), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonFind"));
	WxBoxSizer5->Add(WxButtonFind,0,wxALIGN_CENTER | wxALL,2);

	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxPoint(2,34), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonReplace"));
	WxBoxSizer5->Add(WxButtonReplace,0,wxALIGN_CENTER | wxALL,2);

	WxCheckBoxEnableReplace = new wxCheckBox(this, ID_WXCHECKBOXENABLEREPLACE, _("&Enable Replace"), wxPoint(2,64), wxSize(100,25), 0, wxDefaultValidator, _("WxCheckBoxEnableReplace"));
	WxBoxSizer5->Add(WxCheckBoxEnableReplace,0,wxALIGN_CENTER | wxALL,0);

	WxButtonClose = new wxButton(this, wxID_CANCEL, _("Close"), wxPoint(2,91), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonClose"));
	WxBoxSizer5->Add(WxButtonClose,0,wxALIGN_CENTER | wxALL,2);

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(184,162), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine1,0,wxEXPAND | wxALL,2);

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_LEFT | wxALL, 0);

	WxRadioButtonOpenedFiles = new wxRadioButton(this, ID_WXRADIOBUTTONOPENEDFILES, _("Opened Files in Editor"), wxPoint(164,2), wxSize(190,20), 0, wxDefaultValidator, _("WxRadioButtonOpenedFiles"));
	WxBoxSizer3->Add(WxRadioButtonOpenedFiles,0,wxALIGN_LEFT | wxALL,2);

	WxFlexGridSizer1 = new wxFlexGridSizer(0, 4, 0, 0);
	WxBoxSizer3->Add(WxFlexGridSizer1, 0, wxALIGN_LEFT | wxALL, 0);

	WxRadioButtonDir = new wxRadioButton(this, ID_WXRADIOBUTTONDIR, _("Selected Directory:"), wxPoint(2,4), wxSize(113,20), 0, wxDefaultValidator, _("WxRadioButtonDir"));
	WxFlexGridSizer1->Add(WxRadioButtonDir,0,wxALIGN_LEFT | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxDir;
	WxComboBoxDir = new wxComboBox(this, ID_WXCOMBOBOXDIR, _(""), wxPoint(119,4), wxSize(330,21), arrayStringFor_WxComboBoxDir, 0, wxDefaultValidator, _("WxComboBoxDir"));
	WxFlexGridSizer1->Add(WxComboBoxDir,0,wxALIGN_LEFT | wxALL,2);

	WxButtonDir = new wxButton(this, ID_WXBUTTONDIR, _("..."), wxPoint(453,2), wxSize(30,25), 0, wxDefaultValidator, _("WxButtonDir"));
	WxFlexGridSizer1->Add(WxButtonDir,0,wxALIGN_CENTER | wxALL,2);

	WxButtonActiveDir = new wxButton(this, ID_WXBUTTONACTIVEDIR, _("<<"), wxPoint(487,2), wxSize(30,25), 0, wxDefaultValidator, _("WxButtonActiveDir"));
	WxFlexGridSizer1->Add(WxButtonActiveDir,0,wxALIGN_CENTER | wxALL,2);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("File Filters:"), wxPoint(32,33), wxDefaultSize, 0, _("WxStaticText1"));
	WxFlexGridSizer1->Add(WxStaticText1,0,wxALIGN_RIGHT | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxFilter;
	WxComboBoxFilter = new wxComboBox(this, ID_WXCOMBOBOXFILTER, _(""), wxPoint(119,31), wxSize(330,21), arrayStringFor_WxComboBoxFilter, 0, wxDefaultValidator, _("WxComboBoxFilter"));
	WxFlexGridSizer1->Add(WxComboBoxFilter,0,wxALIGN_LEFT | wxALL,2);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _(""), wxPoint(466,39), wxDefaultSize, 0, _("WxStaticText2"));
	WxFlexGridSizer1->Add(WxStaticText2,0,wxALIGN_CENTER | wxALL,2);

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, _(""), wxPoint(500,39), wxDefaultSize, 0, _("WxStaticText5"));
	WxFlexGridSizer1->Add(WxStaticText5,0,wxALIGN_CENTER | wxALL,2);

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Exclude Filters:"), wxPoint(21,58), wxDefaultSize, 0, _("WxStaticText4"));
	WxFlexGridSizer1->Add(WxStaticText4,0,wxALIGN_RIGHT | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxExclude;
	WxComboBoxExclude = new wxComboBox(this, ID_WXCOMBOBOXEXCLUDE, _(""), wxPoint(119,56), wxSize(330,21), arrayStringFor_WxComboBoxExclude, 0, wxDefaultValidator, _("WxComboBoxExclude"));
	WxFlexGridSizer1->Add(WxComboBoxExclude,0,wxALIGN_LEFT | wxALL,2);

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, _(""), wxPoint(466,64), wxDefaultSize, 0, _("WxStaticText6"));
	WxFlexGridSizer1->Add(WxStaticText6,0,wxALIGN_CENTER | wxALL,2);

	WxStaticText7 = new wxStaticText(this, ID_WXSTATICTEXT7, _(""), wxPoint(500,64), wxDefaultSize, 0, _("WxStaticText7"));
	WxFlexGridSizer1->Add(WxStaticText7,0,wxALIGN_CENTER | wxALL,2);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("File Encoding:"), wxPoint(23,83), wxDefaultSize, 0, _("WxStaticText3"));
	WxFlexGridSizer1->Add(WxStaticText3,0,wxALIGN_RIGHT | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, _(""), wxPoint(184,81), wxSize(200,21), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxEncoding"));
	WxFlexGridSizer1->Add(WxComboBoxEncoding,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxSubDir = new wxCheckBox(this, ID_WXCHECKBOXSUBDIR, _("Include Subdirectories"), wxPoint(108,128), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxSubDir"));
	WxBoxSizer3->Add(WxCheckBoxSubDir,0,wxALIGN_LEFT | wxLEFT,10);

	SetTitle(_("Find/Replace In Files"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
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
    m_FindText->LoadDefaultSyntaxScheme();

    WxBoxSizer6->Add(m_FindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);
    WxBoxSizer6->SetItemMinSize(m_FindText, 400, bh);

    wxBitmap WxBitmapButtonRecentFindText_BITMAP (down_xpm);
	WxBitmapButtonRecentFindText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTFINDTEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentFindText"));
    WxBoxSizer6->Add(WxBitmapButtonRecentFindText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

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

    WxBoxSizer7->Add(m_ReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);
    WxBoxSizer7->SetItemMinSize(m_ReplaceText, 400, bh);

    WxBitmapButtonRecentReplaceText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTREPLACETEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentReplaceText"));
    WxBoxSizer7->Add(WxBitmapButtonRecentReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

    // build encoding list
    wxString systemenc(_("Automatic Detection"));
    WxComboBoxEncoding->Append(systemenc);
    size_t cnt=MadEncoding::GetEncodingsCount();
    for(size_t i=0;i<cnt;i++)
    {
        WxComboBoxEncoding->Append(MadEncoding::GetEncodingName(i));//enc+des);
    }
    WxComboBoxEncoding->SetValue(systemenc);

    //
    m_RecentFindDir = new wxFileHistory();
    m_RecentFindFilter = new wxFileHistory();
    m_RecentFindExclude = new wxFileHistory();
    wxConfigBase *m_Config=wxConfigBase::Get(false);
    wxString oldpath=m_Config->GetPath();
    m_Config->SetPath(wxT("/RecentFindDir"));
    m_RecentFindDir->Load(*m_Config);
    m_Config->SetPath(wxT("/RecentFindFilter"));
    m_RecentFindFilter->Load(*m_Config);
    m_Config->SetPath(wxT("/RecentFindExcludeFilter"));
    m_RecentFindExclude->Load(*m_Config);
    m_Config->SetPath(oldpath);

    size_t count=m_RecentFindDir->GetCount();
    if(count>0)
    {
        wxString text=m_RecentFindDir->GetHistoryFile(0);
        WxComboBoxDir->SetValue(text);
        WxComboBoxDir->Append(text);
        for(size_t i=1; i<count; ++i) WxComboBoxDir->Append(m_RecentFindDir->GetHistoryFile(i));
    }
    count=m_RecentFindFilter->GetCount();
    if(count>0)
    {
        wxString text=m_RecentFindFilter->GetHistoryFile(0);
        WxComboBoxFilter->SetValue(text);
        WxComboBoxFilter->Append(text);
        for(size_t i=1; i<count; ++i) WxComboBoxFilter->Append(m_RecentFindFilter->GetHistoryFile(i));
    }
    count=m_RecentFindExclude->GetCount();
    if(count>0)
    {
        wxString text=m_RecentFindExclude->GetHistoryFile(0);
        //WxComboBoxExclude->SetValue(text);
        WxComboBoxExclude->Append(text);
        for(size_t i=1; i<count; ++i) WxComboBoxExclude->Append(m_RecentFindExclude->GetHistoryFile(i));
    }

    // resize checkbox
    ResizeItem(WxBoxSizer8, WxCheckBoxCaseSensitive, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxWholeWord, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxRegex, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxFindHex, 25, 4);
    ResizeItem(WxBoxSizer8, WxCheckBoxListFirstOnly, 25, 4);

    ResizeItem(WxBoxSizer3, WxRadioButtonOpenedFiles, 25, 4);
    ResizeItem(WxBoxSizer3, WxRadioButtonDir, 25, 4);
    ResizeItem(WxBoxSizer3, WxStaticText1, 4, 2);
    ResizeItem(WxBoxSizer3, WxStaticText3, 4, 2);
    ResizeItem(WxBoxSizer3, WxStaticText4, 4, 2);
    ResizeItem(WxBoxSizer3, WxCheckBoxSubDir, 25, 4);

    ResizeItem(WxBoxSizer5, WxCheckBoxEnableReplace, 25, 4);

    GetSizer()->Fit(this);

    //
    if(g_SearchDialog->m_RecentFindText->GetCount()>0)
    {
        m_FindText->SetText(g_SearchDialog->m_RecentFindText->GetHistoryFile(0));
    }
    if(g_ReplaceDialog->m_RecentReplaceText->GetCount()>0)
    {
        m_ReplaceText->SetText(g_ReplaceDialog->m_RecentReplaceText->GetHistoryFile(0));
    }

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

void MadFindInFilesDialog::UpdateCheckBoxByCBHex(bool check)
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

void MadFindInFilesDialog::WxBitmapButtonRecentFindTextClick(wxCommandEvent& event)
{
    PopupMenu(g_SearchDialog->WxPopupMenuRecentFindText);
}

void MadFindInFilesDialog::WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event)
{
    PopupMenu(g_ReplaceDialog->WxPopupMenuRecentReplaceText);
}

void MadFindInFilesDialog::OnRecentFindText(wxCommandEvent& event)
{
    int idx=event.GetId()-ID_RECENTFINDTEXT1;
    wxString text=g_SearchDialog->m_RecentFindText->GetHistoryFile(idx);
    if(!text.IsEmpty())
    {
        m_FindText->SetText(text);
        m_FindText->SetFocus();
    }
}

void MadFindInFilesDialog::OnRecentReplaceText(wxCommandEvent& event)
{
    int idx=event.GetId()-ID_RECENTREPLACETEXT1;
    wxString text=g_ReplaceDialog->m_RecentReplaceText->GetHistoryFile(idx);
    if(!text.IsEmpty())
    {
        m_ReplaceText->SetText(text);
        m_ReplaceText->SetFocus();
    }
}

/*
 * WxCheckBoxFindHexClick
 */
void MadFindInFilesDialog::WxCheckBoxFindHexClick(wxCommandEvent& event)
{
    UpdateCheckBoxByCBHex(event.IsChecked());
}

/*
 * WxButtonFindClick
 */
void MadFindInFilesDialog::WxButtonFindClick(wxCommandEvent& event)
{
    wxString text;
    m_FindText->GetText(text);
    if(!text.IsEmpty())
    {
        g_SearchDialog->m_RecentFindText->AddFileToHistory(text);
        FindReplaceInFiles(false);
    }
}

/*
 * WxButtonReplaceClick
 */
void MadFindInFilesDialog::WxButtonReplaceClick(wxCommandEvent& event)
{
    wxString text, text2;
    m_FindText->GetText(text);
    m_ReplaceText->GetText(text2);
    if(!text.IsEmpty())
    {
        g_SearchDialog->m_RecentFindText->AddFileToHistory(text);
        if(!text2.IsEmpty())
        {
            g_ReplaceDialog->m_RecentReplaceText->AddFileToHistory(text2);
        }
        FindReplaceInFiles(true);
    }
}

/*
 * WxButtonDirClick
 */
void MadFindInFilesDialog::WxButtonDirClick(wxCommandEvent& event)
{
    wxString defdir=WxComboBoxDir->GetValue();
    if(defdir.IsEmpty()) defdir=wxGetCwd();
    wxDirDialog dlg(this, _("Choose a directory"), defdir);
    if(dlg.ShowModal()==wxID_OK)
    {
        WxComboBoxDir->SetValue(dlg.GetPath());
    }
}

/*
 * MadFindInFilesDialogActivate
 */
void MadFindInFilesDialog::MadFindInFilesDialogActivate(wxActivateEvent& event)
{
    wxConfigBase *m_Config=wxConfigBase::Get(false);
    wxString oldpath=m_Config->GetPath();

    if(event.GetActive())
    {
        WxButtonReplace->Disable();
        WxCheckBoxEnableReplace->SetValue(false);

        bool bb;
        m_Config->Read(wxT("/MadEdit/SearchCaseSensitive"), &bb, false);
        WxCheckBoxCaseSensitive->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchWholeWord"), &bb, false);
        WxCheckBoxWholeWord->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchRegex"), &bb, false);
        WxCheckBoxRegex->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/SearchHex"), &bb, false);
        WxCheckBoxFindHex->SetValue(bb);
        UpdateCheckBoxByCBHex(bb);

        m_Config->Read(wxT("/MadEdit/SearchListFileOnly"), &bb, false);
        WxCheckBoxListFirstOnly->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/FindListFileOnly"), &bb, false);
        WxCheckBoxListFirstOnly->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/FindOpenedFiles"), &bb, true);
        WxRadioButtonOpenedFiles->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/FindDir"), &bb, false);
        WxRadioButtonDir->SetValue(bb);

        m_Config->Read(wxT("/MadEdit/FindSubDir"), &bb, true);
        WxCheckBoxSubDir->SetValue(bb);
    }
    else
    {
        m_Config->Write(wxT("/MadEdit/SearchCaseSensitive"), WxCheckBoxCaseSensitive->GetValue());
        m_Config->Write(wxT("/MadEdit/SearchWholeWord"), WxCheckBoxWholeWord->GetValue());
        m_Config->Write(wxT("/MadEdit/SearchRegex"), WxCheckBoxRegex->GetValue());
        m_Config->Write(wxT("/MadEdit/SearchHex"), WxCheckBoxFindHex->GetValue());
        m_Config->Write(wxT("/MadEdit/FindListFileOnly"), WxCheckBoxListFirstOnly->GetValue());
        m_Config->Write(wxT("/MadEdit/FindOpenedFiles"), WxRadioButtonOpenedFiles->GetValue());
        m_Config->Write(wxT("/MadEdit/FindDir"), WxRadioButtonDir->GetValue());
        m_Config->Write(wxT("/MadEdit/FindSubDir"), WxCheckBoxSubDir->GetValue());
    }

    m_Config->SetPath(oldpath);
}

//=========================================================

wxProgressDialog *g_ProgressDialog=NULL;
bool g_Continue;
wxLongLong g_Time;

wxString fmtmsg1;

WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, MadFileNameList );
MadFileNameList g_FileNameList; // the filenames matched the filename filter

vector<wxString> g_ExcludeFilters;

class DirTraverser : public wxDirTraverser
{
public:
    wxDirTraverseResult Continue()
    {
        wxLongLong t=wxGetLocalTimeMillis();
        wxLongLong delta=t-g_Time;
        if(delta.ToLong()>=350)
        {
            g_Time=t;
            g_Continue=g_ProgressDialog->Update(0, wxString::Format(fmtmsg1, g_FileNameList.size()));
            if(!g_Continue) return wxDIR_STOP;
        }
        return wxDIR_CONTINUE;
    }
    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        bool exclude=false;
        if(!g_ExcludeFilters.empty())
        {
            wxFileName fn;
            size_t count=g_ExcludeFilters.size();
            for(size_t i=0; i<count; i++)
            {
                fn.Assign(filename);
#ifdef __WXMSW__
                if(fn.GetFullName().Lower().Matches(g_ExcludeFilters[i].c_str()))
#else
                if(fn.GetFullName().Matches(g_ExcludeFilters[i].c_str()))
#endif
                {
                    exclude=true;
                    break;
                }
            }
        }
        if(!exclude && g_FileNameList.find(filename) == g_FileNameList.end())
        {
            g_FileNameList.insert(filename);
        }
        return Continue();
    }
    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
    {
        return Continue();
    }
};

class CaretPosData: public wxClientData
{
public:
    wxString filename;
    int pageid; // >=0 for 'NoName'
    wxFileOffset bpos, epos;
    CaretPosData(const wxString &fn, int pid, const wxFileOffset &b, wxFileOffset &e)
        : filename(fn), pageid(pid), bpos(b), epos(e) {}
};

void MadFindInFilesDialog::FindReplaceInFiles(bool bReplace)
{
    //wxLogNull nolog;

#ifdef SHOW_RESULT_COUNT
    int ResultCount=0;
#endif

    const int max=1000;
    fmtmsg1 = _("Found %d file(s) matched the filters...");
    fmtmsg1 += wxT("                                        \n");
    wxProgressDialog dialog(this->GetTitle(),
                            wxString::Format(fmtmsg1, 0),
                            max,    // range
                            this,   // parent
                            wxPD_CAN_ABORT |
                            wxPD_AUTO_HIDE |
                            wxPD_APP_MODAL);
    g_ProgressDialog=&dialog;

    g_MainFrame->ResetFindInFilesResults();
    g_MainFrame->m_AuiManager.GetPane(g_MainFrame->m_InfoNotebook).Show();
    g_MainFrame->m_AuiManager.Update();

    g_Continue=true;
    g_Time=wxGetLocalTimeMillis();

    size_t totalfiles;
    if(WxRadioButtonOpenedFiles->GetValue())
    {
        totalfiles= ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageCount();
    }
    else
    {
        // test the existence of the dir
        wxString str=WxComboBoxDir->GetValue();
        if(!wxDirExists(str))
        {
            dialog.Update(max);
            wxMessageBox(_("The selected directory does not exist."), wxT("MadEdit"), wxOK|wxICON_ERROR);
            return;
        }
        m_RecentFindDir->AddFileToHistory(str);
        if(WxComboBoxDir->GetCount()==0 || WxComboBoxDir->GetString(0)!=str)
        {
            WxComboBoxDir->Insert(str, 0);
        }
        
        // get the filename filters
        str=WxComboBoxFilter->GetValue();
        str.Trim(true);
        str.Trim(false);
        wxStringTokenizer tkz(str, wxT(" \t;"));
        vector<wxString> filters;
        wxString tok;
        for(;;)
        {
            tok=tkz.GetNextToken();
            if(tok.IsEmpty()) break;
            filters.push_back(tok);
        }
        if(!filters.empty())
        {
            m_RecentFindFilter->AddFileToHistory(str);
            if(WxComboBoxFilter->GetCount()==0 || WxComboBoxFilter->GetString(0)!=str)
            {
                WxComboBoxFilter->Insert(str, 0);
            }
        }
        str=WxComboBoxExclude->GetValue();
        str.Trim(true);
        str.Trim(false);
        wxStringTokenizer tkz2(str, wxT(" \t;"));
        g_ExcludeFilters.clear();
        for(;;)
        {
            tok=tkz2.GetNextToken();
            if(tok.IsEmpty()) break;
#ifdef __WXMSW__
            tok.MakeLower();
#endif
            g_ExcludeFilters.push_back(tok);
        }
        if(!g_ExcludeFilters.empty())
        {
            m_RecentFindExclude->AddFileToHistory(str);
            if(WxComboBoxExclude->GetCount()==0 || WxComboBoxExclude->GetString(0)!=str)
            {
                WxComboBoxExclude->Insert(str, 0);
            }
        }

        // get the files matched the filters
        g_FileNameList.clear();

        DirTraverser traverser;
        wxDir dir(WxComboBoxDir->GetValue());
        int flags=wxDIR_FILES|wxDIR_HIDDEN;
        if(WxCheckBoxSubDir->GetValue()) flags|=wxDIR_DIRS;

        if(filters.empty())
        {
            dir.Traverse(traverser, wxEmptyString, flags);
        }
        else
        {
            for(size_t i=0; i<filters.size(); i++)
            {
                dir.Traverse(traverser, filters[i], flags);
            }
        }
        
        totalfiles=g_FileNameList.size();
    }

    if(g_Continue)
    {
        MadEdit *madedit, *tempedit=NULL;
        if(WxRadioButtonDir->GetValue())
        {
            madedit=tempedit=new MadEdit(this, -1, wxPoint(-1024,-1024));
            tempedit->StopRepaint();
            tempedit->SetStorePropertiesToGlobalConfig(false);
            tempedit->SetFixedWidthMode(false);
            tempedit->SetWordWrapMode(wwmNoWrap);
            tempedit->SetSearchOptions(true, WxCheckBoxWholeWord->GetValue());
        }

        wxString fmt(_("Processing %d of %d files..."));
        vector<wxFileOffset> begpos, endpos;
        MadFileNameList::iterator fnit=g_FileNameList.begin();
        bool cont = true;
        for(size_t i = 0; i < totalfiles && cont; i++)
        {
            // prepare madedit
            if(WxRadioButtonOpenedFiles->GetValue())
            {
                madedit=(MadEdit*) ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPage(i);
            }
            else
            {
                int id;
                madedit=g_MainFrame->GetEditByFileName(*fnit, id);

                if(madedit==NULL)
                {
                    madedit=tempedit;
                    wxString enc=WxComboBoxEncoding->GetValue();
                    if(enc==WxComboBoxEncoding->GetString(0))
                    {
                        enc.Clear();
                    }
                    madedit->LoadFromFile(*fnit, enc);
                }
                ++fnit;
            }

            wxLongLong t=wxGetLocalTimeMillis();
            wxLongLong delta=t-g_Time;
            if(delta.ToLong()>=350)
            {
                g_Time=t;
                int idx = int(i*max / totalfiles);
                wxString str=wxString::Format(fmt, i, totalfiles);
                wxString fn=madedit->GetFileName();
                if(!fn.IsEmpty())
                {
                    str+= wxT('\n');
                    str+=fn;
                }
                cont = dialog.Update(idx, str);
                if(!cont) break;
            }

            // get all matched data in madedit
            begpos.clear();
            endpos.clear();
            wxString expr, fmt;
            int ok;
            if(bReplace)
            {
                m_FindText->GetText(expr);
                m_ReplaceText->GetText(fmt);
                if(WxCheckBoxFindHex->GetValue())
                {
                    ok = madedit->ReplaceHexAll(expr, fmt, &begpos, &endpos);
                }
                else
                {
                    ok = madedit->ReplaceTextAll(expr, fmt, 
                        WxCheckBoxRegex->GetValue(),
                        WxCheckBoxCaseSensitive->GetValue(),
                        WxCheckBoxWholeWord->GetValue(), 
                        &begpos, &endpos);
                }

                if(ok<0) break;

                expr=madedit->GetFileName();
                int id = ((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageIndex(madedit);
                if(madedit->IsModified() && !expr.IsEmpty() && id<0)
                {
                    madedit->SaveToFile(expr);
                }
            }
            else
            {
                m_FindText->GetText(expr);
                if(WxCheckBoxFindHex->GetValue())
                {
                    ok = madedit->FindHexAll(expr, WxCheckBoxListFirstOnly->GetValue(),
                        &begpos, &endpos);
                }
                else
                {
                    ok = madedit->FindTextAll(expr,
                        WxCheckBoxRegex->GetValue(),
                        WxCheckBoxCaseSensitive->GetValue(),
                        WxCheckBoxWholeWord->GetValue(),
                        WxCheckBoxListFirstOnly->GetValue(),
                        &begpos, &endpos);
                }

                if(ok<0) break;
            }

            if(begpos.size()>0) // found data
            {
                int pid=-1;
                expr=madedit->GetFileName();
                if(expr.IsEmpty())
                {
                    pid=((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageIndex(madedit);
                    if(pid>=0)
                    {
                        expr=((wxAuiNotebook*)g_MainFrame->m_Notebook)->GetPageText(pid);
                        if(expr[expr.Len()-1]==wxT('*'))
                            expr.Truncate(expr.Len()-1);
                    }
                }
                if(!expr.IsEmpty())
                {
                    size_t count=begpos.size(), idx=0;
                    if(WxCheckBoxListFirstOnly->GetValue()) count=1;
                    int line=-1, oldline;
                    wxString linetext, loc;
                    g_MainFrame->m_FindInFilesResults->Freeze();
                    do
                    {
                        if(madedit->IsTextFile())
                        {
                            oldline=line;
                            line=madedit->GetLineByPos(begpos[idx]);
                            if(line!=oldline)
                            {
                                linetext.Empty();
                                madedit->GetLine(linetext, line, 512);
                            }
                            loc.Printf(_("Line(%d): "), line+1);
                        }
                        else
                        {
                            loc.Printf(_("Offset(%s): "), wxLongLong(begpos[idx]).ToString().c_str());
                            linetext = _("Binary file matches");
                        }

                        fmt = loc +linetext;
                        g_MainFrame->AddItemToFindInFilesResults(fmt, idx, expr, pid, begpos[idx], endpos[idx]);
#ifdef SHOW_RESULT_COUNT
                        ++ResultCount;
#endif
                    }
                    while(++idx < count);
                    g_MainFrame->m_FindInFilesResults->Thaw();
                }
            }
        }
        
        if(tempedit) delete tempedit;
    }

    dialog.Update(max);
    g_ProgressDialog=NULL;
    g_FileNameList.clear();

#ifdef SHOW_RESULT_COUNT
    wxString smsg;
    smsg.Printf(_("%d results"), ResultCount);
    wxMessageBox(smsg.c_str(), wxT("MadEdit"), wxOK);
#endif
}

/*
 * WxCheckBoxEnableReplaceClick
 */
void MadFindInFilesDialog::WxCheckBoxEnableReplaceClick(wxCommandEvent& event)
{
    WxButtonReplace->Enable(event.IsChecked());
}

/*
 * WxButtonActiveDirClick
 */
void MadFindInFilesDialog::WxButtonActiveDirClick(wxCommandEvent& event)
{
    extern MadEdit *g_ActiveMadEdit;
    if(g_ActiveMadEdit!=NULL)
    {
        wxString str=g_ActiveMadEdit->GetFileName();
        if(!str.IsEmpty())
        {
            wxFileName fn(str);
            WxComboBoxDir->SetValue(fn.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR));
        }
    }
}
