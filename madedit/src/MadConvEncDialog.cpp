///////////////////////////////////////////////////////////////////////////////
// Name:        MadConvEncDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadConvEncDialog.h"
#include "MadEdit/MadEncoding.h"
#include <wx/config.h>

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

MadConvEncDialog *g_ConvEncDialog=NULL;

//----------------------------------------------------------------------------
// MadConvEncDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadConvEncDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadConvEncDialog::MadConvEncDialogClose)
END_EVENT_TABLE()
////Event Table End

MadConvEncDialog::MadConvEncDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
}

MadConvEncDialog::~MadConvEncDialog() {} 

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

void MadConvEncDialog::CreateGUIControls(void)
{
    //do not set FontName, it is not exist on all platforms
    #define wxFont(p0,p1,p2,p3,p4,p5) wxFont(wxDEFAULT,wxDEFAULT,p2,p3,p4)

    //Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("New Encoding:"), wxPoint(5, 7), wxDefaultSize, 0, _("WxStaticText1"));
	WxStaticText1->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, _("MS Sans Serif")));
	WxBoxSizer2->Add(WxStaticText1,0,wxALIGN_CENTER | wxALL,5);

	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox(this, ID_WXCOMBOBOXENCODING, _("WxComboBoxEncoding"), wxPoint(92, 5), wxSize(150, 21), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxEncoding"));
	WxComboBoxEncoding->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, _("MS Sans Serif")));
	WxBoxSizer2->Add(WxComboBoxEncoding,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_CENTER | wxALL, 5);

	wxArrayString arrayStringFor_WxRadioBoxOption;
	arrayStringFor_WxRadioBoxOption.Add(_("None"));
	arrayStringFor_WxRadioBoxOption.Add(_("Simplified Chinese => Traditional Chinese"));
	arrayStringFor_WxRadioBoxOption.Add(_("Traditional Chinese => Simplified Chinese"));
	arrayStringFor_WxRadioBoxOption.Add(_("Japanese Kanji => Traditional Chinese"));
	arrayStringFor_WxRadioBoxOption.Add(_("Japanese Kanji => Simplified Chinese"));
	arrayStringFor_WxRadioBoxOption.Add(_("Chinese => Japanese Kanji"));
	WxRadioBoxOption = new wxRadioBox(this, ID_WXRADIOBOXOPTION, _("Addtional Option"), wxPoint(5, 5), wxSize(250, 155), arrayStringFor_WxRadioBoxOption, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _("WxRadioBoxOption"));
	WxRadioBoxOption->SetSelection(0);
	WxRadioBoxOption->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, _("MS Sans Serif")));
	WxBoxSizer3->Add(WxRadioBoxOption,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 5);

	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxPoint(11, 5), wxSize(91, 30), 0, wxDefaultValidator, _("WxButtonOK"));
	WxButtonOK->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, _("MS Sans Serif")));
	WxBoxSizer4->Add(WxButtonOK,0,wxALIGN_CENTER | wxALL,5);

	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(106, 5), wxSize(90, 30), 0, wxDefaultValidator, _("WxButtonCancel"));
	WxButtonCancel->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, _("MS Sans Serif")));
	WxBoxSizer4->Add(WxButtonCancel,0,wxALIGN_CENTER | wxALL,5);

	SetTitle(_("Convert Encoding"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End

    //restore wxFont
    #undef wxFont

    size_t cnt=MadEncoding::GetEncodingsCount();
    for(size_t i=0;i<cnt;i++)
    {
        WxComboBoxEncoding->Append(MadEncoding::GetEncodingName(i));//enc+des);
    }
    wxString convenc=MadEncoding::GetEncodingName(0);
    wxConfigBase *cfg=wxConfigBase::Get(false);
    wxString oldpath=cfg->GetPath();
    cfg->Read(wxT("/MadEdit/ConvertEncoding"), &convenc);
    cfg->SetPath(oldpath);
    WxComboBoxEncoding->SetValue(convenc);

    ResizeItem(WxBoxSizer2, WxStaticText1, 2, 2);

    int strx=0, stry=0;
    wxString str=WxRadioBoxOption->GetLabel();
    WxRadioBoxOption->GetTextExtent(str, &strx, &stry);
    for(size_t i=0;i<WxRadioBoxOption->GetCount();i++)
    {
        int x;
        str=WxRadioBoxOption->GetString((unsigned int)i);
        WxRadioBoxOption->GetTextExtent(str, &x, &stry);
        if(x>strx) strx=x;
    }
    stry = (stry *((int)WxRadioBoxOption->GetCount()+2))*4/3;
    WxRadioBoxOption->SetSize(strx+=35, stry);
    WxBoxSizer3->SetItemMinSize(WxRadioBoxOption, strx, stry);

    WxButtonCancel->SetFocus();
}

void MadConvEncDialog::MadConvEncDialogClose(wxCloseEvent& event)
{
    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }
    
    g_ConvEncDialog=NULL;
    Destroy();
}
 
