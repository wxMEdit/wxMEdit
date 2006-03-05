///////////////////////////////////////////////////////////////////////////////
// Name:        MadAboutDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadAboutDialog.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

#include "../images/Mad.xpm"

//----------------------------------------------------------------------------
// MadAboutDialog
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp 
    ////Event Table Start
BEGIN_EVENT_TABLE(MadAboutDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadAboutDialog::MadAboutDialogClose)
END_EVENT_TABLE()
    ////Event Table End



MadAboutDialog::MadAboutDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style)
{
    CreateGUIControls();
}

MadAboutDialog::~MadAboutDialog() {} 

void MadAboutDialog::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	wxBoxSizer* WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(TRUE);

	wxBoxSizer* WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(3,54), wxSize(48,48) );
	WxStaticBitmap1->Enable(false);
	WxBoxSizer2->Add(WxStaticBitmap1,0,wxALIGN_TOP | wxALL,5);

	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, _(""), wxPoint(41,3), wxSize(350,150), wxTE_READONLY | wxTE_AUTO_URL | wxTE_MULTILINE, wxDefaultValidator, _("WxMemo1"));
	WxMemo1->SetMaxLength(0);
	WxMemo1->SetFocus();
	WxMemo1->SetInsertionPointEnd();
	WxBoxSizer2->Add(WxMemo1,0,wxALIGN_CENTER_VERTICAL | wxALL,2);

	wxBoxSizer* WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3,0,wxALIGN_RIGHT | wxALL,3);

	WxButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxPoint(3,3), wxSize(90,30), 0, wxDefaultValidator, _("WxButtonOK"));
	WxBoxSizer3->Add(WxButtonOK,0,wxALIGN_CENTER_VERTICAL | wxALL,4);

	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(99,3), wxSize(90,30), 0, wxDefaultValidator, _("WxButtonCancel"));
	WxBoxSizer3->Add(WxButtonCancel,0,wxALIGN_CENTER_VERTICAL | wxALL,4);

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	this->SetTitle(_("About MadEdit"));
	this->Center();
	this->SetIcon(wxNullIcon);
	
    ////GUI Items Creation End
    
    WxStaticBitmap1->SetBitmap(wxBitmap(Mad_xpm));
    WxStaticBitmap1->Enable(true);

    SetDefaultItem(WxButtonCancel);
    WxButtonCancel->SetFocus();
}

void MadAboutDialog::MadAboutDialogClose(wxCloseEvent& event)
{
    // --> Don't use Close with a wxDialog,
    // use Destroy instead.
    Destroy();
}

