///////////////////////////////////////////////////////////////////////////////
// Name:        MadSortDialog.cpp
// Description: Sort Options Dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadSortDialog.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MadSortDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadSortDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadSortDialog::MadSortDialogClose)
END_EVENT_TABLE()
////Event Table End

MadSortDialog::MadSortDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
}

MadSortDialog::~MadSortDialog() {} 


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

void MadSortDialog::CreateGUIControls(void)
{
    //Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	wxArrayString arrayStringFor_WxRadioBoxOrder;
	arrayStringFor_WxRadioBoxOrder.Add(_("&Ascending"));
	arrayStringFor_WxRadioBoxOrder.Add(_("&Descending"));
	WxRadioBoxOrder = new wxRadioBox(this, ID_WXRADIOBOXORDER, _("Sort Order"), wxPoint(30,5), wxSize(150,75), arrayStringFor_WxRadioBoxOrder, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _("WxRadioBoxOrder"));
	WxRadioBoxOrder->SetSelection(0);
	WxBoxSizer1->Add(WxRadioBoxOrder,0,wxALIGN_LEFT | wxALL,5);

	WxCheckBoxCase = new wxCheckBox(this, ID_WXCHECKBOXCASE, _("&Case Sensitive"), wxPoint(30,90), wxSize(150,20), 0, wxDefaultValidator, _("WxCheckBoxCase"));
	WxBoxSizer1->Add(WxCheckBoxCase,0,wxALIGN_LEFT | wxALL,5);

	WxCheckBoxNumeric = new wxCheckBox(this, ID_WXCHECKBOXNUMERIC, _("&Numeric Sort"), wxPoint(30,120), wxSize(150,20), 0, wxDefaultValidator, _("WxCheckBoxNumeric"));
	WxBoxSizer1->Add(WxCheckBoxNumeric,0,wxALIGN_LEFT | wxALL,5);

	WxCheckBoxRemoveDup = new wxCheckBox(this, ID_WXCHECKBOXREMOVEDUP, _("&Remove Duplicates"), wxPoint(30,150), wxSize(150,20), 0, wxDefaultValidator, _("WxCheckBoxRemoveDup"));
	WxBoxSizer1->Add(WxCheckBoxRemoveDup,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_LEFT | wxALL, 5);

	WxButtonSort = new wxButton(this, wxID_OK, _("&Sort"), wxPoint(5,5), wxSize(90,30), 0, wxDefaultValidator, _("WxButtonSort"));
	WxBoxSizer2->Add(WxButtonSort,0,wxALIGN_CENTER | wxALL,5);

	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(105,5), wxSize(90,30), 0, wxDefaultValidator, _("WxButtonCancel"));
	WxBoxSizer2->Add(WxButtonCancel,0,wxALIGN_CENTER | wxALL,5);

	SetTitle(_("Sort Options"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End

	size_t i;
    int strx=0, stry=0;
    wxString str=WxRadioBoxOrder->GetLabel();
    WxRadioBoxOrder->GetTextExtent(str, &strx, &stry);
    for(i=0; i<WxRadioBoxOrder->GetCount(); i++)
    {
        int x;
        str=WxRadioBoxOrder->GetString((unsigned int)i);
        WxRadioBoxOrder->GetTextExtent(str, &x, &stry);
        if(x>strx) strx=x;
    }
    stry = (stry *((int)WxRadioBoxOrder->GetCount()+2))*4/3;
    WxRadioBoxOrder->SetSize(strx+=25, stry);

    ResizeItem(WxBoxSizer1, WxCheckBoxCase, 25, 4);
    ResizeItem(WxBoxSizer1, WxCheckBoxNumeric, 25, 4);
    ResizeItem(WxBoxSizer1, WxCheckBoxRemoveDup, 25, 4);

    WxButtonSort->SetFocus();
}

void MadSortDialog::MadSortDialogClose(wxCloseEvent& event)
{
    Destroy();
}
 
