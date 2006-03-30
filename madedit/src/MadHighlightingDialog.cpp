///////////////////////////////////////////////////////////////////////////////
// Name:        MadHighlightingDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadHighlightingDialog.h"
#include "MadUtils.h"
#include "MadEdit/MadSyntax.h"
#include <wx/colordlg.h>


//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

extern void ApplySyntaxAttributes(MadSyntax *syn);

MadHighlightingDialog *g_HighlightingDialog=NULL;

enum
{
    kindSysAttr1, kindSysAttr2/*aeActiveLine, aeBookmark*/, kindRange, kindKeyword
};

struct KeywordInfo
{
    int kind;
    MadAttributes *attr; // for kindSysAttr1, kindSysArrt2, kindKeyword
    wxColour *range_bgcolor; // for kindRange
    KeywordInfo(): kind(0), attr(0), range_bgcolor(0)
    {}
    KeywordInfo(int k, MadAttributes *a, wxColour *bg): kind(k), attr(a), range_bgcolor(bg)
    {}
};
vector<KeywordInfo> g_KeywordInfoTable;
long g_Index=-1;
MadSyntax *g_Syntax=NULL;
wxColourDialog *g_ColourDialog=NULL;


wxColour GetColourFromUser(const wxColour& colInit, const wxString& caption)
{
    if(g_ColourDialog==NULL)
    {
        wxColourData *data=new wxColourData;
        data->SetChooseFull(true);
        if ( colInit.Ok() )
        {
            data->SetColour((wxColour &)colInit); // const_cast
        }
        g_ColourDialog = new wxColourDialog(g_HighlightingDialog, data);
    }
    else
    {
        if ( colInit.Ok() )
        {
            g_ColourDialog->GetColourData().SetColour(colInit);
        }
    }

    if (!caption.IsEmpty())
        g_ColourDialog->SetTitle(caption);

    wxColour colRet;
    if ( g_ColourDialog->ShowModal() == wxID_OK )
    {
        colRet = g_ColourDialog->GetColourData().GetColour();
    }

    return colRet;
}

wxFont GetItemFont(wxListCtrl *listctrl, long item)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    return it.GetFont();
}

void SetItemFont(wxListCtrl *listctrl, long item, wxFont &font)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetFont(font);
    listctrl->SetItem(it);
}

//----------------------------------------------------------------------------
// MadHighlightingDialog
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(MadHighlightingDialog,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(MadHighlightingDialog::MadHighlightingDialogClose)
	EVT_ACTIVATE(MadHighlightingDialog::MadHighlightingDialogActivate)
	EVT_BUTTON(ID_WXBUTTONBC,MadHighlightingDialog::WxButtonBCClick)
	
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRLBC,MadHighlightingDialog::WxListCtrlBCSelected)
	EVT_BUTTON(ID_WXBUTTONFC,MadHighlightingDialog::WxButtonFCClick)
	
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRLFC,MadHighlightingDialog::WxListCtrlFCSelected)
	EVT_CHECKBOX(ID_WXCHECKBOXUNDERLINE,MadHighlightingDialog::WxCheckBoxUnderlineClick)
	EVT_CHECKBOX(ID_WXCHECKBOXITALIC,MadHighlightingDialog::WxCheckBoxItalicClick)
	EVT_CHECKBOX(ID_WXCHECKBOXBOLD,MadHighlightingDialog::WxCheckBoxBoldClick)
	
	EVT_LIST_ITEM_SELECTED(ID_WXLISTCTRLKEYWORD,MadHighlightingDialog::WxListCtrlKeywordSelected)
	EVT_LISTBOX(ID_WXLISTBOXSYNTAX,MadHighlightingDialog::WxListBoxSyntaxSelected)
END_EVENT_TABLE()
////Event Table End

MadHighlightingDialog::MadHighlightingDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    m_Syntax=NULL;
    m_InitSetting=true;
    CreateGUIControls();
}

MadHighlightingDialog::~MadHighlightingDialog() {} 

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


void MadHighlightingDialog::CreateGUIControls(void)
{
    //Do not add custom code here
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxBoxSizer* WxBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(TRUE);

	wxArrayString arrayStringFor_WxListBoxSyntax;
	WxListBoxSyntax = new wxListBox(this, ID_WXLISTBOXSYNTAX, wxPoint(3,40), wxSize(145,380), arrayStringFor_WxListBoxSyntax, wxLB_SINGLE | wxLB_HSCROLL);
	WxBoxSizer1->Add(WxListBoxSyntax,1,wxGROW | wxALL,5);

	wxBoxSizer* WxBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer1->Add(WxBoxSizer2,3,wxGROW | wxALL,2);

	wxBoxSizer* WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer3,0,wxALIGN_CENTER_HORIZONTAL | wxALL,0);

	wxBoxSizer* WxBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer4,0,wxALIGN_CENTER_HORIZONTAL | wxALL,4);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Schema:"), wxPoint(4,6), wxSize(46,17), 0, _("WxStaticText1"));
	WxBoxSizer4->Add(WxStaticText1,0,wxALIGN_CENTER_VERTICAL | wxALL,4);

	wxArrayString arrayStringFor_WxComboBox1;
	WxComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, _(""), wxPoint(58,4), wxSize(145,21), arrayStringFor_WxComboBox1, 0, wxDefaultValidator, _("WxComboBox1"));
	WxBoxSizer4->Add(WxComboBox1,0,wxALIGN_CENTER_VERTICAL | wxALL,4);

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _("You cannot modify the schemas with * sign."), wxPoint(211,6), wxSize(209,17), 0, _("WxStaticText2"));
	WxBoxSizer4->Add(WxStaticText2,0,wxALIGN_CENTER_VERTICAL | wxALL,4);

	wxBoxSizer* WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer6,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	WxButtonLoad = new wxButton(this, ID_WXBUTTONLOAD, _("&Load"), wxPoint(2,2), wxSize(75,25), 0, wxDefaultValidator, _("WxButtonLoad"));
	WxBoxSizer6->Add(WxButtonLoad,0,wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxButtonSave = new wxButton(this, ID_WXBUTTONSAVE, _("&Save"), wxPoint(81,2), wxSize(75,25), 0, wxDefaultValidator, _("WxButtonSave"));
	WxBoxSizer6->Add(WxButtonSave,0,wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxButtonDelete = new wxButton(this, ID_WXBUTTONDELETE, _("&Delete"), wxPoint(160,2), wxSize(75,25), 0, wxDefaultValidator, _("WxButtonDelete"));
	WxBoxSizer6->Add(WxButtonDelete,0,wxALIGN_CENTER_VERTICAL | wxALL,2);

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(173,64), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer2->Add(WxStaticLine1,0,wxGROW | wxALL,1);

	wxBoxSizer* WxBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer5,1,wxGROW | wxALL,0);

	WxListCtrlKeyword = new wxListCtrl(this, ID_WXLISTCTRLKEYWORD, wxPoint(0,38), wxSize(145,259), wxLC_REPORT  | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	WxBoxSizer5->Add(WxListCtrlKeyword,1,wxGROW | wxALL,2);

	wxBoxSizer* WxBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer5->Add(WxBoxSizer8,2,wxGROW | wxALL,0);

	wxBoxSizer* WxBoxSizer9 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer8->Add(WxBoxSizer9,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxBold = new wxCheckBox(this, ID_WXCHECKBOXBOLD, _("Bold"), wxPoint(2,2), wxSize(97,17), 0, wxDefaultValidator, _("WxCheckBoxBold"));
	WxBoxSizer9->Add(WxCheckBoxBold,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxItalic = new wxCheckBox(this, ID_WXCHECKBOXITALIC, _("Italic"), wxPoint(2,23), wxSize(97,17), 0, wxDefaultValidator, _("WxCheckBoxItalic"));
	WxBoxSizer9->Add(WxCheckBoxItalic,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxUnderline = new wxCheckBox(this, ID_WXCHECKBOXUNDERLINE, _("Underline"), wxPoint(2,44), wxSize(97,17), 0, wxDefaultValidator, _("WxCheckBoxUnderline"));
	WxBoxSizer9->Add(WxCheckBoxUnderline,0,wxALIGN_LEFT | wxALL,2);

	WxStaticLine3 = new wxStaticLine(this, ID_WXSTATICLINE3, wxPoint(98,63), wxSize(150,-1), wxLI_HORIZONTAL);
	WxBoxSizer8->Add(WxStaticLine3,0,wxGROW | wxALL,1);

	wxBoxSizer* WxBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer10,1,wxGROW | wxALL,2);

	wxBoxSizer* WxBoxSizer11 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer10->Add(WxBoxSizer11,1,wxGROW | wxALL,3);

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("Foreground/Text Color"), wxPoint(27,3), wxSize(111,17), 0, _("WxStaticText3"));
	WxBoxSizer11->Add(WxStaticText3,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxStaticTextFCName = new wxStaticText(this, ID_WXSTATICTEXTFCNAME, _("WxStaticTextFCName"), wxPoint(28,26), wxSize(109,17), wxALIGN_CENTRE, _("WxStaticTextFCName"));
	WxBoxSizer11->Add(WxStaticTextFCName,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxPanelFC = new wxPanel(this, ID_WXPANELFC, wxPoint(40,49), wxSize(85,20), wxSIMPLE_BORDER);
	WxBoxSizer11->Add(WxPanelFC,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxListCtrlFC = new wxListCtrl(this, ID_WXLISTCTRLFC, wxPoint(3,75), wxSize(160,150), wxLC_REPORT  | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	WxBoxSizer11->Add(WxListCtrlFC,1,wxGROW | wxALL,2);

	WxButtonFC = new wxButton(this, ID_WXBUTTONFC, _("Other Color"), wxPoint(38,231), wxSize(90,28), 0, wxDefaultValidator, _("WxButtonFC"));
	WxBoxSizer11->Add(WxButtonFC,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	WxStaticLine2 = new wxStaticLine(this, ID_WXSTATICLINE2, wxPoint(172,58), wxSize(-1,150), wxLI_VERTICAL);
	WxBoxSizer10->Add(WxStaticLine2,0,wxGROW | wxALL,1);

	wxBoxSizer* WxBoxSizer12 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer10->Add(WxBoxSizer12,1,wxGROW | wxALL,3);

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Background Color"), wxPoint(38,3), wxSize(89,17), 0, _("WxStaticText4"));
	WxBoxSizer12->Add(WxStaticText4,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxStaticTextBCName = new wxStaticText(this, ID_WXSTATICTEXTBCNAME, _("WxStaticTextBCName"), wxPoint(28,26), wxSize(110,17), wxALIGN_CENTRE, _("WxStaticTextBCName"));
	WxBoxSizer12->Add(WxStaticTextBCName,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxPanelBC = new wxPanel(this, ID_WXPANELBC, wxPoint(40,49), wxSize(85,20), wxSIMPLE_BORDER);
	WxBoxSizer12->Add(WxPanelBC,0,wxALIGN_CENTER_HORIZONTAL | wxALL,3);

	WxListCtrlBC = new wxListCtrl(this, ID_WXLISTCTRLBC, wxPoint(3,75), wxSize(160,150), wxLC_REPORT  | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
	WxBoxSizer12->Add(WxListCtrlBC,1,wxGROW | wxALL,2);

	WxButtonBC = new wxButton(this, ID_WXBUTTONBC, _("Other Color"), wxPoint(38,231), wxSize(90,28), 0, wxDefaultValidator, _("WxButtonBC"));
	WxBoxSizer12->Add(WxButtonBC,0,wxALIGN_CENTER_HORIZONTAL | wxALL,2);

	wxBoxSizer* WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer2->Add(WxBoxSizer7,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	WxButtonClose = new wxButton(this, wxID_OK, _("&OK"), wxPoint(5,5), wxSize(90,27), 0, wxDefaultValidator, _("WxButtonClose"));
	WxBoxSizer7->Add(WxButtonClose,0,wxALIGN_CENTER_VERTICAL | wxALL,3);

	WxButton1 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(105,5), wxSize(90,27), 0, wxDefaultValidator, _("WxButton1"));
	WxBoxSizer7->Add(WxButton1,0,wxALIGN_CENTER_VERTICAL | wxALL,3);

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	this->SetTitle(_("Syntax Highlighting Settings"));
	this->Center();
	this->SetIcon(wxNullIcon);
	
	////GUI Items Creation End

    ResizeItem(WxBoxSizer4, WxStaticText1, 2, 2);
    ResizeItem(WxBoxSizer4, WxStaticText2, 2, 2);

    ResizeItem(WxBoxSizer9, WxCheckBoxBold, 25, 4);
    ResizeItem(WxBoxSizer9, WxCheckBoxItalic, 25, 4);
    ResizeItem(WxBoxSizer9, WxCheckBoxUnderline, 25, 4);

    ResizeItem(WxBoxSizer11, WxStaticText3, 2, 2);
    ResizeItem(WxBoxSizer11, WxStaticTextFCName, 2, 2);

    ResizeItem(WxBoxSizer12, WxStaticText4, 2, 2);
    ResizeItem(WxBoxSizer12, WxStaticTextBCName, 2, 2);

    {   // build syntax type list
        size_t cnt=MadSyntax::GetSyntaxCount();
        for(size_t i=0;i<cnt;i++)
        {
            wxString title=MadSyntax::GetSyntaxTitle(i);
            WxListBoxSyntax->Append(title);//, new wxStringClientData(MadSyntax::GetColorFileByTitle(title)));
        }
    }

    WxListCtrlKeyword->InsertColumn(0, wxT("first column"));

    // build color list
    WxListCtrlFC->Hide();
    WxListCtrlBC->Hide();
    WxListCtrlFC->InsertColumn(0, wxT("first column"));
    WxListCtrlBC->InsertColumn(0, wxT("first column"));

    wxListItem it;
    it.SetColumn(0);
    it.SetId(0);
    it.SetText(wxT("(Automatic)"));
    it.SetTextColour(wxT("Red"));
    WxListCtrlFC->InsertItem(it);
    WxListCtrlBC->InsertItem(it);

    HtmlColor *hc=HtmlColorTable;
    for(int i=0; i<HtmlColorTableCount; ++i, ++hc)
    {
        it.SetText(hc->name);
        it.SetId(i+1);

        wxColor c(wxColor(hc->red, hc->green, hc->blue));
        it.SetTextColour(c);
        it.SetBackgroundColour(wxNullColour);
        WxListCtrlFC->InsertItem(it);

        if((hc->red+hc->green+hc->blue)/3 >= 128)
            it.SetTextColour(*wxBLACK);
        else
            it.SetTextColour(*wxWHITE);
        it.SetBackgroundColour(c);
        WxListCtrlBC->InsertItem(it);
    }

    WxListCtrlFC->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    WxListCtrlBC->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    WxListCtrlFC->Show();
    WxListCtrlBC->Show();
}

void MadHighlightingDialog::MadHighlightingDialogClose(wxCloseEvent& event)
{
    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }

    FreeSyntax(false);
    g_HighlightingDialog=NULL;
    if(g_ColourDialog) delete g_ColourDialog;
    Destroy();
}

/*
 * WxListBoxSyntaxSelected
 */
void MadHighlightingDialog::WxListBoxSyntaxSelected(wxCommandEvent& event)
{
    wxString title=WxListBoxSyntax->GetString(event.GetSelection());
    g_Syntax=GetSyntax(title);

    // build keyword list
    WxListCtrlKeyword->Freeze();
    WxListCtrlKeyword->DeleteAllItems();
    g_KeywordInfoTable.clear();
    int index=0;

    // system attributes
    for(int ae=aeText; ae<aeNone; ae++)
    {
        long item = WxListCtrlKeyword->InsertItem(index++, MadSyntax::GetAttributeName((MadAttributeElement)ae));
        long data = (long)g_KeywordInfoTable.size();
        MadAttributes *attr = g_Syntax->GetAttributes((MadAttributeElement)ae);
        int kind=kindSysAttr1;
        if(ae==aeActiveLine || ae==aeBookmark) kind=kindSysAttr2;
        g_KeywordInfoTable.push_back(KeywordInfo(kind, attr, NULL));
        WxListCtrlKeyword->SetItemData(item, data);

        if(ae==aeText)
        {
            WxListCtrlKeyword->SetBackgroundColour(attr->bgcolor);
        }

        WxListCtrlKeyword->SetItemTextColour(item, attr->color);
        WxListCtrlKeyword->SetItemBackgroundColour(item, attr->bgcolor);
        wxFont font=GetItemFont(WxListCtrlKeyword, item);
        if(attr->style!=fsNone)
        {
            if(attr->style&fsBold) font.SetWeight(wxFONTWEIGHT_BOLD);
            if(attr->style&fsItalic) font.SetStyle(wxFONTSTYLE_ITALIC);
            if(attr->style&fsUnderline) font.SetUnderlined(true);
            SetItemFont(WxListCtrlKeyword, item, font);
        }
    }
    
    // custom ranges
    size_t i;
    for(i=0; i<g_Syntax->m_CustomRange.size(); ++i)
    {
        wxString text;
        text.Printf(wxT("Range %s %s"), g_Syntax->m_CustomRange[i].begin.c_str(), g_Syntax->m_CustomRange[i].end.c_str());
        long item = WxListCtrlKeyword->InsertItem(index++, text);
        long data = (long)g_KeywordInfoTable.size();
        wxColour *bg = &(g_Syntax->m_CustomRange[i].bgcolor);
        g_KeywordInfoTable.push_back( KeywordInfo(kindRange, NULL, bg) );
        WxListCtrlKeyword->SetItemData(item, data);
        WxListCtrlKeyword->SetItemTextColour(item, g_KeywordInfoTable[0].attr->color);
        WxListCtrlKeyword->SetItemBackgroundColour(item, *bg);
    }
    
    // custom keywords
    for(i=0; i<g_Syntax->m_CustomKeyword.size(); ++i)
    {
        long item = WxListCtrlKeyword->InsertItem(index++, g_Syntax->m_CustomKeyword[i].m_Name);
        long data = (long)g_KeywordInfoTable.size();
        MadAttributes *attr = &(g_Syntax->m_CustomKeyword[i].m_Attr);
        g_KeywordInfoTable.push_back(KeywordInfo(kindKeyword, attr, NULL));
        WxListCtrlKeyword->SetItemData(item, data);

        WxListCtrlKeyword->SetItemTextColour(item, attr->color);
        WxListCtrlKeyword->SetItemBackgroundColour(item, attr->bgcolor);
        wxFont font=GetItemFont(WxListCtrlKeyword, item);
        if(attr->style!=fsNone)
        {
            if(attr->style&fsBold) font.SetWeight(wxFONTWEIGHT_BOLD);
            if(attr->style&fsItalic) font.SetStyle(wxFONTSTYLE_ITALIC);
            if(attr->style&fsUnderline) font.SetUnderlined(true);
            SetItemFont(WxListCtrlKeyword, item, font);
        }
    }

    WxListCtrlKeyword->SetColumnWidth( 0, wxLIST_AUTOSIZE );
    WxListCtrlKeyword->Thaw();

    g_Index=-1;
    wxListEvent event(0);
    event.m_itemIndex=0;
    WxListCtrlKeywordSelected(event);
}


void MadHighlightingDialog::SetPanelFC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextFCName->SetLabel(wxT("(Automatic)"));
        WxPanelFC->SetBackgroundColour(WxListCtrlFC->GetItemTextColour(0));
        WxPanelFC->ClearBackground();
    }
    else
    {
        wxString cname=wxTheColourDatabase->FindName(color);
        if(cname.IsEmpty())
        {
            cname.Printf(wxT("#%02X%02X%02X"), color.Red(), color.Green(), color.Blue());
        }
        WxStaticTextFCName->SetLabel(cname);
        WxPanelFC->SetBackgroundColour(color);
        WxPanelFC->ClearBackground();
    }
}

void MadHighlightingDialog::SetPanelBC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextBCName->SetLabel(wxT("(Automatic)"));
        WxPanelBC->SetBackgroundColour(WxListCtrlBC->GetItemBackgroundColour(0));
        WxPanelBC->ClearBackground();
    }
    else
    {
        wxString cname=wxTheColourDatabase->FindName(color);
        if(cname.IsEmpty())
        {
            cname.Printf(wxT("#%02X%02X%02X"), color.Red(), color.Green(), color.Blue());
        }
        WxStaticTextBCName->SetLabel(cname);
        WxPanelBC->SetBackgroundColour(color);
        WxPanelBC->ClearBackground();
    }
}

/*
 * WxListCtrlKeywordSelected
 */
void MadHighlightingDialog::WxListCtrlKeywordSelected(wxListEvent& event)
{
    long oldIndex=g_Index;
    g_Index = WxListCtrlKeyword->GetItemData(event.m_itemIndex);

    WxListCtrlKeyword->Freeze();
    WxListCtrlFC->Freeze();

    if(oldIndex!=g_Index)
    {
        wxString str;
        if(oldIndex>=0)
        {
            str = WxListCtrlKeyword->GetItemText(oldIndex);
            if(str[0]==wxT('*')) 
            {
                WxListCtrlKeyword->SetItemText(oldIndex, str.Right(str.Len()-1));
            }
        }

        str = WxListCtrlKeyword->GetItemText(g_Index);
        WxListCtrlKeyword->SetItemText(g_Index, wxString(wxT('*'))+str);

        WxListCtrlKeyword->SetColumnWidth(0, wxLIST_AUTOSIZE);
    }

    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0) // set (Automatic) colors
    {
        WxListCtrlFC->SetItemTextColour(0, kinfo.attr->color);
        //WxListCtrlFC->SetItemBackgroundColour(0, kinfo.attr->bgcolor);
        //WxListCtrlFC->SetBackgroundColour(kinfo.attr->bgcolor);
        //WxListCtrlFC->Refresh();
        
        WxListCtrlBC->SetItemBackgroundColour(0, kinfo.attr->bgcolor);
        int c = (kinfo.attr->bgcolor.Red()+kinfo.attr->bgcolor.Green()+kinfo.attr->bgcolor.Blue())/3;
        if(c >= 128)
        {
            WxListCtrlBC->SetItemTextColour(0, *wxBLACK);
        }
        else
        {
            WxListCtrlBC->SetItemTextColour(0, *wxWHITE);
        }
    }

    wxColour bgc;
    switch(kinfo.kind)
    {
    case kindSysAttr1:
    case kindKeyword:
        WxCheckBoxBold->Enable();
        WxCheckBoxItalic->Enable();
        WxCheckBoxUnderline->Enable();
        WxCheckBoxBold->SetValue((kinfo.attr->style&fsBold)!=0);
        WxCheckBoxItalic->SetValue((kinfo.attr->style&fsItalic)!=0);
        WxCheckBoxUnderline->SetValue((kinfo.attr->style&fsUnderline)!=0);
        SetPanelFC(kinfo.attr->color);
        SetPanelBC(kinfo.attr->bgcolor);
        WxListCtrlFC->Enable();
        WxButtonFC->Enable();
        WxListCtrlBC->Enable();
        WxButtonBC->Enable();
        bgc=kinfo.attr->bgcolor;
        break;
    case kindSysAttr2:
        WxCheckBoxBold->SetValue(false);
        WxCheckBoxItalic->SetValue(false);
        WxCheckBoxUnderline->SetValue(false);
        WxCheckBoxBold->Disable();
        WxCheckBoxItalic->Disable();
        WxCheckBoxUnderline->Disable();
        SetPanelFC(kinfo.attr->color);
        SetPanelBC(wxNullColour);
        WxListCtrlFC->Enable();
        WxButtonFC->Enable();
        WxListCtrlBC->Disable();
        WxButtonBC->Disable();
        bgc==wxNullColour;
        break;
    case kindRange:
        WxCheckBoxBold->SetValue(false);
        WxCheckBoxItalic->SetValue(false);
        WxCheckBoxUnderline->SetValue(false);
        WxCheckBoxBold->Disable();
        WxCheckBoxItalic->Disable();
        WxCheckBoxUnderline->Disable();
        SetPanelFC(wxNullColour);
        SetPanelBC(*kinfo.range_bgcolor);
        WxListCtrlFC->Disable();
        WxButtonFC->Disable();
        WxListCtrlBC->Enable();
        WxButtonBC->Enable();
        bgc=*kinfo.range_bgcolor;
        break;
    }
    if(bgc==wxNullColour) bgc=g_KeywordInfoTable[0].attr->bgcolor;
    WxListCtrlFC->SetBackgroundColour(bgc);
    WxListCtrlFC->SetItemBackgroundColour(0, bgc);
    this->Layout();

    WxListCtrlKeyword->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
    WxListCtrlKeyword->Thaw();
    WxListCtrlFC->Thaw();
}

/*
 * WxListCtrlFCSelected
 */
void MadHighlightingDialog::WxListCtrlFCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlFC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlFC->GetItemTextColour(event.m_itemIndex);
    WxStaticTextFCName->SetLabel(colorname);
    this->Layout();

    WxPanelFC->SetBackgroundColour(color);
    WxPanelFC->ClearBackground();

    SetAttrFC(color, colorname);
    SetToModifiedSyntax(g_Syntax);

    WxListCtrlFC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

/*
 * WxListCtrlBCSelected
 */
void MadHighlightingDialog::WxListCtrlBCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlBC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlBC->GetItemBackgroundColour(event.m_itemIndex);
    WxStaticTextBCName->SetLabel(colorname);
    this->Layout();

    WxPanelBC->SetBackgroundColour(color);
    WxPanelBC->ClearBackground();

    SetAttrBC(color, colorname);
    SetToModifiedSyntax(g_Syntax);

    WxListCtrlBC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

/*
 * WxCheckBoxBoldClick
 */
void MadHighlightingDialog::WxCheckBoxBoldClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsBold;
        font.SetWeight(wxFONTWEIGHT_BOLD);
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsBold);
        font.SetWeight(wxFONTWEIGHT_NORMAL);
    }
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, wxLIST_AUTOSIZE);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxCheckBoxItalicClick
 */
void MadHighlightingDialog::WxCheckBoxItalicClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsItalic;
        font.SetStyle(wxFONTSTYLE_ITALIC);
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsItalic);
        font.SetStyle(wxFONTSTYLE_NORMAL);
    }
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, wxLIST_AUTOSIZE);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxCheckBoxUnderlineClick
 */
void MadHighlightingDialog::WxCheckBoxUnderlineClick(wxCommandEvent& event)
{
    wxFont font=GetItemFont(WxListCtrlKeyword, g_Index);
    if(event.IsChecked()) 
    {
        g_KeywordInfoTable[g_Index].attr->style |= fsUnderline;
        font.SetUnderlined(true);
    }
    else
    {
        g_KeywordInfoTable[g_Index].attr->style &= (~fsUnderline);
        font.SetUnderlined(false);
    }
    SetItemFont(WxListCtrlKeyword, g_Index, font);
    WxListCtrlKeyword->SetColumnWidth(0, wxLIST_AUTOSIZE);
    SetToModifiedSyntax(g_Syntax);
}

/*
 * WxButtonFCClick
 */
void MadHighlightingDialog::WxButtonFCClick(wxCommandEvent& event)
{
    wxColour color=GetColourFromUser(WxListCtrlKeyword->GetItemTextColour(g_Index), WxStaticText3->GetLabel());
    if(color.Ok())
    {
        SetPanelFC(color);
        this->Layout();
        wxString colorname=WxStaticTextFCName->GetLabel();
        SetAttrFC(color, colorname);
        SetToModifiedSyntax(g_Syntax);
    }
}

/*
 * WxButtonBCClick
 */
void MadHighlightingDialog::WxButtonBCClick(wxCommandEvent& event)
{
    wxColour color=GetColourFromUser(WxPanelBC->GetBackgroundColour(), WxStaticText4->GetLabel());
    if(color.Ok())
    {
        SetPanelBC(color);
        this->Layout();
        wxString colorname=WxStaticTextBCName->GetLabel();
        SetAttrBC(color, colorname);
        SetToModifiedSyntax(g_Syntax);
    }
}

/*
 * MadHighlightingDialogActivate
 */
void MadHighlightingDialog::MadHighlightingDialogActivate(wxActivateEvent& event)
{
    if(m_InitSetting && event.GetActive())
    {
        m_InitSetting=false;
        g_Index=-1;

        int i=WxListBoxSyntax->GetSelection();
        if(i==wxNOT_FOUND) i=0;
        WxListBoxSyntax->SetSelection(i);

        wxCommandEvent e;
        e.SetInt(i);
        WxListBoxSyntaxSelected(e);
    }
}

MadSyntax *MadHighlightingDialog::GetSyntax(const wxString &title)
{
    if(m_Syntax && m_Syntax->m_Title.CmpNoCase(title)==0)
        return m_Syntax;

    for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        if(m_ModifiedSyntax[i]->m_Title.CmpNoCase(title)==0)
            return m_ModifiedSyntax[i];
    }

    if(m_Syntax) delete m_Syntax;
    m_Syntax=MadSyntax::GetSyntaxByTitle(title);
    return m_Syntax;
}

void MadHighlightingDialog::SetToModifiedSyntax(MadSyntax *syn)
{
    if(syn==m_Syntax)
    {
        m_ModifiedSyntax.push_back(syn);
        m_Syntax=NULL;
    }
    ApplySyntaxAttributes(syn);// apply syntax attributes to editor
}

void MadHighlightingDialog::SetAttrFC(const wxColor &color, const wxString &colorname)
{
    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0)
    {
        WxListCtrlFC->SetItemTextColour(0, color);
        kinfo.attr->color=color;
    }
    else
    {
        wxASSERT(kinfo.kind!=kindRange);
        if(colorname==wxT("(Automatic)"))
        {
            kinfo.attr->color=wxNullColour;
        }
        else
        {
            kinfo.attr->color=color;
        }
    }
    RepaintKeyword();
}

void MadHighlightingDialog::SetAttrBC(const wxColor &color, const wxString &colorname)
{
    KeywordInfo &kinfo=g_KeywordInfoTable[g_Index];
    if(g_Index==0)
    {
        WxListCtrlBC->SetItemBackgroundColour(0, color);
        WxListCtrlKeyword->SetBackgroundColour(color);
        kinfo.attr->bgcolor=color;
    }
    else
    {
        wxASSERT(kinfo.kind!=kindSysAttr2);
        switch(kinfo.kind)
        {
        case kindSysAttr1:
        case kindKeyword:
            if(colorname==wxT("(Automatic)")) kinfo.attr->bgcolor=wxNullColour;
            else                              kinfo.attr->bgcolor=color;
            break;
        case kindRange:
            *kinfo.range_bgcolor=color;
            break;
        }
    }
    RepaintKeyword();

    WxListCtrlFC->SetItemBackgroundColour(0, color);
    WxListCtrlFC->SetBackgroundColour(color);
    WxListCtrlFC->Refresh();
}

void MadHighlightingDialog::RepaintKeyword()
{
    vector<KeywordInfo>::iterator it=g_KeywordInfoTable.begin();
    vector<KeywordInfo>::iterator itend=g_KeywordInfoTable.end();
    long idx=0;
    wxColour &fc0=it->attr->color;
    wxColour &bc0=it->attr->bgcolor;
    wxColour fc, bc;
    WxListCtrlKeyword->Freeze();
    do
    {
        switch(it->kind)
        {
        case kindSysAttr1:
        case kindSysAttr2:
        case kindKeyword:
            fc=it->attr->color;
            bc=it->attr->bgcolor;
            break;
        case kindRange:
            fc=wxNullColour;
            bc=*it->range_bgcolor;
            break;
        }
        if(fc==wxNullColour) fc=fc0;
        if(bc==wxNullColour) bc=bc0;
        WxListCtrlKeyword->SetItemTextColour(idx, fc);
        WxListCtrlKeyword->SetItemBackgroundColour(idx, bc);
        ++idx;
    }
    while(++it != itend);
    WxListCtrlKeyword->Thaw();
}

void MadHighlightingDialog::FreeSyntax(bool restore)
{
    if(restore) // restore the original syntax
    {
        for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            MadSyntax *syn=MadSyntax::GetSyntaxByTitle(m_ModifiedSyntax[i]->m_Title);
            ApplySyntaxAttributes(syn);
            delete syn;
        }
    }
    else // write the modified syntax back
    {
        for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            m_ModifiedSyntax[i]->SaveAttributes();
        }
    }

    if(m_Syntax) 
    {
        delete m_Syntax;
        m_Syntax=NULL;
    }

    for(size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        delete m_ModifiedSyntax[i];
    }
    m_ModifiedSyntax.clear();
}

