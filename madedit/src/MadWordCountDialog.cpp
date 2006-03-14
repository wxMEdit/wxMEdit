///////////////////////////////////////////////////////////////////////////////
// Name:        MadWordCountDialog.cpp
// Description: WordCount Dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadWordCountDialog.h"
#include "MadEdit/MadEdit.h"


//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// MadWordCountDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadWordCountDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadWordCountDialog::MadWordCountDialogClose)
END_EVENT_TABLE()
////Event Table End

MadWordCountDialog::MadWordCountDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
}

MadWordCountDialog::~MadWordCountDialog() {} 

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


void MadWordCountDialog::CreateGUIControls(void)
{
    //Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxBoxSizer* WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(TRUE);

	wxFlexGridSizer* WxFlexGridSizer1 = new wxFlexGridSizer(5,2,0,60);
	WxBoxSizer1->Add(WxFlexGridSizer1,0,wxALIGN_CENTER_HORIZONTAL | wxALL,4);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Words"), wxPoint(46,4), wxSize(35,17), 0, _("WxStaticText1"));
	WxFlexGridSizer1->Add(WxStaticText1,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticTextWordCount = new wxStaticText(this, ID_WXSTATICTEXTWORDCOUNT, _("999999"), wxPoint(132,4), wxSize(40,17), 0, _("WxStaticTextWordCount"));
	WxFlexGridSizer1->Add(WxStaticTextWordCount,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _("Characters (no spaces)"), wxPoint(7,29), wxSize(113,17), 0, _("WxStaticText2"));
	WxFlexGridSizer1->Add(WxStaticText2,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticTextCharCount = new wxStaticText(this, ID_WXSTATICTEXTCHARCOUNT, _("999999"), wxPoint(132,29), wxSize(40,17), 0, _("WxStaticTextCharCount"));
	WxFlexGridSizer1->Add(WxStaticTextCharCount,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("Characters (with spaces)"), wxPoint(4,54), wxSize(120,17), 0, _("WxStaticText3"));
	WxFlexGridSizer1->Add(WxStaticText3,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticTextSpaceCount = new wxStaticText(this, ID_WXSTATICTEXTSPACECOUNT, _("999999"), wxPoint(132,54), wxSize(40,17), 0, _("WxStaticTextSpaceCount"));
	WxFlexGridSizer1->Add(WxStaticTextSpaceCount,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Halfwidth Words"), wxPoint(23,79), wxSize(82,17), 0, _("WxStaticText4"));
	WxFlexGridSizer1->Add(WxStaticText4,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticTextHalfwidthCount = new wxStaticText(this, ID_WXSTATICTEXTHALFWIDTHCOUNT, _("999999"), wxPoint(132,79), wxSize(40,17), 0, _("WxStaticTextHalfwidthCount"));
	WxFlexGridSizer1->Add(WxStaticTextHalfwidthCount,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticText5 = new wxStaticText(this, ID_WXSTATICTEXT5, _("Fullwidth Words"), wxPoint(24,104), wxSize(79,17), 0, _("WxStaticText5"));
	WxFlexGridSizer1->Add(WxStaticText5,0,wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticTextFullwidthCount = new wxStaticText(this, ID_WXSTATICTEXTFULLWIDTHCOUNT, _("999999"), wxPoint(132,104), wxSize(40,17), 0, _("WxStaticTextFullwidthCount"));
	WxFlexGridSizer1->Add(WxStaticTextFullwidthCount,0,wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL,5);

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(90,140), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer1->Add(WxStaticLine1,0,wxGROW | wxALL,2);

	WxStaticText6 = new wxStaticText(this, ID_WXSTATICTEXT6, _("Detail Information of Characters:"), wxPoint(87,157), wxSize(155,17), 0, _("WxStaticText6"));
	WxBoxSizer1->Add(WxStaticText6,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, _(""), wxPoint(5,184), wxSize(320,89), wxTE_READONLY | wxTE_DONTWRAP | wxTE_MULTILINE, wxDefaultValidator, _("WxMemo1"));
	WxMemo1->SetMaxLength(0);
	WxMemo1->SetFocus();
	WxMemo1->SetInsertionPointEnd();
	WxBoxSizer1->Add(WxMemo1,1,wxGROW | wxALL,4);

	WxButton1 = new wxButton(this, wxID_OK, _("&Close"), wxPoint(127,283), wxSize(75,30), 0, wxDefaultValidator, _("WxButton1"));
	WxBoxSizer1->Add(WxButton1,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	this->SetTitle(_("Word Count"));
	this->Center();
	this->SetIcon(wxNullIcon);
	
	////GUI Items Creation End

    extern MadEdit *g_ActiveMadEdit;
    int words, chars, spaces, halfwidths, fullwidths;
    wxArrayString detail;
    g_ActiveMadEdit->WordCount(g_ActiveMadEdit->IsSelected(), words, chars, spaces, halfwidths, fullwidths, &detail);

    WxStaticTextWordCount->SetLabel(wxString()<<words);
    WxStaticTextCharCount->SetLabel(wxString()<<chars);
    WxStaticTextSpaceCount->SetLabel(wxString()<<(chars+spaces));
    WxStaticTextHalfwidthCount->SetLabel(wxString()<<halfwidths);
    WxStaticTextFullwidthCount->SetLabel(wxString()<<fullwidths);
    wxString str;
    for(size_t i=0;i<detail.Count();i++)
    {
        str<<detail[i]<<wxT("\n");
    }
    WxMemo1->SetValue(str);

    ResizeItem(WxBoxSizer1, WxStaticText1, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticText2, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticText3, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticText4, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticText5, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticTextWordCount, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticTextCharCount, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticTextSpaceCount, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticTextHalfwidthCount, 2, 2);
    ResizeItem(WxBoxSizer1, WxStaticTextFullwidthCount, 2, 2);

    ResizeItem(WxBoxSizer1, WxStaticText6, 2, 2);
}

void MadWordCountDialog::MadWordCountDialogClose(wxCloseEvent& event)
{
    Destroy();
}
 
