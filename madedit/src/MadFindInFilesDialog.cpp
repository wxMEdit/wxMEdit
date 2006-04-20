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
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

#include <wx/progdlg.h>
#include <wx/dir.h>
#include <wx/tokenzr.h>

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
	
	EVT_LISTBOX_DCLICK(ID_WXLISTBOXFILES,MadFindInFilesDialog::WxListBoxFilesDoubleClicked)
	EVT_BUTTON(ID_WXBUTTONDIR,MadFindInFilesDialog::WxButtonDirClick)
	EVT_CHECKBOX(ID_WXCHECKBOXNOREPLACE,MadFindInFilesDialog::WxCheckBoxNoReplaceClick)
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
	WxBoxSizer8->Add(WxCheckBoxCaseSensitive,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxWholeWord = new wxCheckBox(this, ID_WXCHECKBOXWHOLEWORD, _("&Whole Word Only"), wxPoint(5,37), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxWholeWord"));
	WxBoxSizer8->Add(WxCheckBoxWholeWord,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxRegex = new wxCheckBox(this, ID_WXCHECKBOXREGEX, _("Use Regular E&xpressions"), wxPoint(5,69), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxRegex"));
	WxBoxSizer8->Add(WxCheckBoxRegex,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxFindHex = new wxCheckBox(this, ID_WXCHECKBOXFINDHEX, _("Find &Hex String (Example: BE 00 3A or BE003A)"), wxPoint(5,101), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxFindHex"));
	WxBoxSizer8->Add(WxCheckBoxFindHex,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxListFirstOnly = new wxCheckBox(this, ID_WXCHECKBOXLISTFIRSTONLY, _("&List the First Found Item Only"), wxPoint(5,133), wxSize(300,22), 0, wxDefaultValidator, _("WxCheckBoxListFirstOnly"));
	WxBoxSizer8->Add(WxCheckBoxListFirstOnly,0,wxALIGN_LEFT | wxALL,2);

	wxBoxSizer* WxBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer5,0,wxALIGN_TOP | wxALL,5);

	WxButtonFind = new wxButton(this, ID_WXBUTTONFIND, _("&Find"), wxPoint(5,5), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonFind"));
	WxBoxSizer5->Add(WxButtonFind,0,wxALIGN_LEFT | wxALL,5);

	WxButtonReplace = new wxButton(this, ID_WXBUTTONREPLACE, _("&Replace"), wxPoint(5,43), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonReplace"));
	WxBoxSizer5->Add(WxButtonReplace,0,wxALIGN_LEFT | wxALL,5);

	WxCheckBoxNoReplace = new wxCheckBox(this, ID_WXCHECKBOXNOREPLACE, _("&No 'Replace'"), wxPoint(5,81), wxSize(100,22), 0, wxDefaultValidator, _("WxCheckBoxNoReplace"));
	WxBoxSizer5->Add(WxCheckBoxNoReplace,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	WxButtonClose = new wxButton(this, wxID_CANCEL, _("Close"), wxPoint(5,108), wxSize(100,28), 0, wxDefaultValidator, _("WxButtonClose"));
	WxBoxSizer5->Add(WxButtonClose,0,wxALIGN_LEFT | wxALL,5);

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(166,245), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine1,0,wxGROW | wxALL,2);

	wxBoxSizer* WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer3,0,wxGROW | wxALL,0);

	WxRadioButtonOpenedFiles = new wxRadioButton(this, ID_WXRADIOBUTTONOPENEDFILES, _("Opened Files in Editor"), wxPoint(180,0), wxSize(113,20), 0, wxDefaultValidator, _("WxRadioButtonOpenedFiles"));
	WxBoxSizer3->Add(WxRadioButtonOpenedFiles,0,wxALIGN_LEFT | wxALL,2);

	wxFlexGridSizer* WxFlexGridSizer1 = new wxFlexGridSizer(4,3,0,0);
	WxBoxSizer3->Add(WxFlexGridSizer1,0,wxGROW | wxALL,0);

	WxRadioButtonDir = new wxRadioButton(this, ID_WXRADIOBUTTONDIR, _("Selected Directory:"), wxPoint(0,1), wxSize(113,20), 0, wxDefaultValidator, _("WxRadioButtonDir"));
	WxFlexGridSizer1->Add(WxRadioButtonDir,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxDir;
	WxComboBoxDir = new wxComboBox(this, ID_WXCOMBOBOXDIR, _(""), wxPoint(113,1), wxSize(330,21), arrayStringFor_WxComboBoxDir, 0, wxDefaultValidator, _("WxComboBoxDir"));
	WxFlexGridSizer1->Add(WxComboBoxDir,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxButtonDir = new wxButton(this, ID_WXBUTTONDIR, _("..."), wxPoint(443,0), wxSize(30,23), 0, wxDefaultValidator, _("WxButtonDir"));
	WxFlexGridSizer1->Add(WxButtonDir,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("File Filters:"), wxPoint(30,25), wxSize(53,17), 0, _("WxStaticText1"));
	WxFlexGridSizer1->Add(WxStaticText1,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxFilter;
	WxComboBoxFilter = new wxComboBox(this, ID_WXCOMBOBOXFILTER, _(""), wxPoint(113,23), wxSize(330,21), arrayStringFor_WxComboBoxFilter, 0, wxDefaultValidator, _("WxComboBoxFilter"));
	WxFlexGridSizer1->Add(WxComboBoxFilter,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _(""), wxPoint(456,31), wxSize(4,4), 0, _("WxStaticText2"));
	WxFlexGridSizer1->Add(WxStaticText2,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("File Encoding:"), wxPoint(21,46), wxSize(71,17), 0, _("WxStaticText3"));
	WxFlexGridSizer1->Add(WxStaticText3,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,2);

	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, _(""), wxPoint(178,44), wxSize(200,21), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxEncoding"));
	WxFlexGridSizer1->Add(WxComboBoxEncoding,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxCheckBoxSubDir = new wxCheckBox(this, ID_WXCHECKBOXSUBDIR, _("Include Subdirectories"), wxPoint(86,85), wxSize(300,20), 0, wxDefaultValidator, _("WxCheckBoxSubDir"));
	WxBoxSizer3->Add(WxCheckBoxSubDir,0,wxALIGN_LEFT | wxALL,2);

	WxStaticLine2 = new wxStaticLine(this, ID_WXSTATICLINE2, wxPoint(166,377), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine2,0,wxGROW | wxALL,2);

	wxArrayString arrayStringFor_WxListBoxFiles;
	WxListBoxFiles = new wxListBox(this, ID_WXLISTBOXFILES, wxPoint(122,394), wxSize(238,130), arrayStringFor_WxListBoxFiles, wxLB_SINGLE | wxLB_HSCROLL);
	WxBoxSizer1->Add(WxListBoxFiles,1,wxGROW | wxALL,2);

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
    m_FindText->LoadDefaultSyntaxScheme();

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
    m_ReplaceText->LoadDefaultSyntaxScheme();

    WxBoxSizer7->Add(m_ReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);
    WxBoxSizer7->SetItemMinSize(m_ReplaceText, 400, bh);

    WxBitmapButtonRecentReplaceText = new wxBitmapButton(this, ID_WXBITMAPBUTTONRECENTREPLACETEXT, WxBitmapButtonRecentFindText_BITMAP, wxPoint(0,0), wxSize(bh,bh), wxBU_AUTODRAW, wxDefaultValidator, _("WxBitmapButtonRecentReplaceText"));
    WxBoxSizer7->Add(WxBitmapButtonRecentReplaceText,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);

    // build encoding list
    wxString systemenc(_("System Default"));
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
    wxConfigBase *m_Config=wxConfigBase::Get(false);
    wxString oldpath=m_Config->GetPath();
    m_Config->SetPath(wxT("/RecentFindDir"));
    m_RecentFindDir->Load(*m_Config);
    m_Config->SetPath(wxT("/RecentFindFilter"));
    m_RecentFindFilter->Load(*m_Config);
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
    ResizeItem(WxBoxSizer3, WxCheckBoxSubDir, 25, 4);

    ResizeItem(WxBoxSizer5, WxCheckBoxNoReplace, 25, 4);

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
    if(/*WxCheckBoxFindHex->IsShown() &&*/ check)
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
        WxCheckBoxNoReplace->SetValue(true);

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
list<wxString> g_FileNameList; // the filenames matched the filename filter


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
        g_FileNameList.push_back(filename);
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

    WxListBoxFiles->Clear();
    g_Continue=true;
    g_Time=wxGetLocalTimeMillis();

    size_t totalfiles;
    if(WxRadioButtonOpenedFiles->GetValue())
    {
        totalfiles=g_MainFrame->m_Notebook->GetPageCount();
    }
    else
    {
        // test the existence of the dir
        wxString str=WxComboBoxDir->GetValue();
        if(!wxDirExists(str))
        {
            dialog.Update(max);
            wxMessageBox(_T("The selected directory does not exist."), wxT("MadEdit"), wxOK|wxICON_ERROR);
            return;
        }
        m_RecentFindDir->AddFileToHistory(str);
        if(WxComboBoxDir->GetCount()==0 || WxComboBoxDir->GetString(0)!=str)
        {
            WxComboBoxDir->Insert(str, 0);
        }
        
        // get the filename filters
        str=WxComboBoxFilter->GetValue();
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
            madedit=tempedit=new MadEdit(this, -1, wxPoint(-999,-999));
            tempedit->SetStorePropertiesToGlobalConfig(false);
        }
        
        wxString fmt(_("Processing %d of %d files..."));
        vector<wxFileOffset> begpos, endpos;
        list<wxString>::iterator fnit=g_FileNameList.begin();
        bool cont = true;
        for(size_t i = 0; i < totalfiles && cont; i++)
        {
            // prepare madedit
            if(WxRadioButtonOpenedFiles->GetValue())
            {
                madedit=(MadEdit*)g_MainFrame->m_Notebook->GetPage(i);
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
                int id = g_MainFrame->m_Notebook->GetPageIndex(madedit);
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
                    pid=g_MainFrame->m_Notebook->GetPageIndex(madedit);
                    if(pid>=0)
                    {
                        expr=g_MainFrame->m_Notebook->GetPageText(pid);
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
                    WxListBoxFiles->Freeze();
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
                            loc.Printf(_(": Line(%d): "), line+1);
                        }
                        else
                        {
                            loc.Printf(_(": Offset(%s): "), wxLongLong(begpos[idx]).ToString().c_str());
                            linetext = _("Binary file matches");
                        }

                        fmt = expr +loc +linetext;
                        WxListBoxFiles->Append(fmt, new CaretPosData(expr, pid, begpos[idx], endpos[idx]));
                    }
                    while(++idx < count);
                    WxListBoxFiles->Append(wxT("--------------------------------------------------"));
                    WxListBoxFiles->Thaw();
                }
            }
        }
        
        if(tempedit) delete tempedit;
    }

    dialog.Update(max);
    g_ProgressDialog=NULL;
    g_FileNameList.clear();
}

/*
 * WxCheckBoxNoReplaceClick
 */
void MadFindInFilesDialog::WxCheckBoxNoReplaceClick(wxCommandEvent& event)
{
    WxButtonReplace->Enable(!event.IsChecked());
}

/*
 * WxListBoxFilesDoubleClicked
 */
void MadFindInFilesDialog::WxListBoxFilesDoubleClicked(wxCommandEvent& event)
{
    int id=event.GetSelection();
    CaretPosData *cpdata = (CaretPosData*)WxListBoxFiles->GetClientObject(id);
    if(cpdata)
    {
        extern MadEdit *g_ActiveMadEdit;

        MadEdit *madedit=NULL;
        int count = int(g_MainFrame->m_Notebook->GetPageCount());
        if(cpdata->pageid>=0 && cpdata->pageid<count)
        {
            wxString title=g_MainFrame->m_Notebook->GetPageText(cpdata->pageid);
            if(title[title.Len()-1]==wxT('*'))
                title.Truncate(title.Len()-1);
            if(title==cpdata->filename)
            {
                g_MainFrame->SetPageFocus(cpdata->pageid);
                madedit = g_ActiveMadEdit;
            }
        }
        else
        {
            g_MainFrame->OpenFile(cpdata->filename, true);

#ifdef __WXMSW__
            if(g_ActiveMadEdit->GetFileName().Lower()==cpdata->filename.Lower())
#else
            if(g_ActiveMadEdit->GetFileName()==cpdata->filename)
#endif
            {
                madedit = g_ActiveMadEdit;
            }
        }

        if(madedit)
        {
            madedit->SetCaretPosition(cpdata->epos, cpdata->bpos, cpdata->epos);
        }
    }
}
