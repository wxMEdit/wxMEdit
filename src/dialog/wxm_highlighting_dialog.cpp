///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_highlighting_dialog.cpp
// Description: Syntax Highlight Settings Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2006-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_highlighting_dialog.h"

#include "../xm/cxx11.h"
#include "../mad_utils.h"
#include "../wxmedit/wxm_syntax.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMHighlightingDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern void ApplySyntaxAttributes(MadSyntax *syn);

WXMHighlightingDialog *g_HighlightingDialog=nullptr;

void SetItemColour(wxListCtrl* listctrl, long item, const wxColour& fc, const wxColour& bc);
void SetItemTextColour(wxListCtrl* listctrl, long item, const wxColour& fc);
void SetItemBackgroundColour(wxListCtrl* listctrl, long item, const wxColour& bc);
namespace wxm
{

inline const wxColor& NormalColor(const wxColor& color, const wxString& colorname)
{
	return (colorname == _("(Automatic)")) ? wxNullColour : color;
}

struct HighlightingBGOnly : public HighlightingItem
{
	HighlightingBGOnly(WXMHighlightingDialog* dlg, long itemidx, wxColor& bc)
		: HighlightingItem(dlg, itemidx), m_bgcolor(bc)
	{}

private:
	virtual wxColor DoInitDialogControls() override
	{
		m_dlg->DisableFontConfig();
		m_dlg->DisableFGColorConfig();
		return m_dlg->EnableBGColorConfig(m_bgcolor);
	}
	virtual void SetFGColor(const wxColor& color, const wxString& colorname) override {}
	virtual void SetBGColor(const wxColor& color, const wxString& colorname) override
	{
		m_bgcolor = NormalColor(color, colorname);
	}
	virtual wxColor GetFGColor() override
	{
		return wxNullColour;
	}
	virtual wxColor GetBGColor() override
	{
		return m_bgcolor;
	}
	virtual void SetListItemFontStyle(long itemidx) override {}
	virtual void EnableFontStyle(const MadFontStyles& style, bool enable) override {}

	wxColor& m_bgcolor;
};

struct HighlightingFGOnly : public HighlightingItem
{
	HighlightingFGOnly(WXMHighlightingDialog* dlg, long itemidx, wxColor& fc)
		: HighlightingItem(dlg, itemidx), m_fgcolor(fc)
	{}

private:
	virtual wxColor DoInitDialogControls() override
	{
		m_dlg->DisableFontConfig();
		m_dlg->EnableFGColorConfig(m_fgcolor);
		return m_dlg->DisableBGColorConfig();
	}
	virtual void SetFGColor(const wxColor& color, const wxString& colorname) override
	{
		m_fgcolor = NormalColor(color, colorname);
	}
	virtual void SetBGColor(const wxColor& color, const wxString& colorname) override {}
	virtual wxColor GetFGColor() override
	{
		return m_fgcolor;
	}
	virtual wxColor GetBGColor() override
	{
		return wxNullColour;
	}
	virtual void SetListItemFontStyle(long itemidx) override {}
	virtual void EnableFontStyle(const MadFontStyles& style, bool enable) override {}

	wxColor& m_fgcolor;
};

struct HighlightingColor : public HighlightingItem
{
	HighlightingColor(WXMHighlightingDialog* dlg, long itemidx, wxColor& fc, wxColor& bc)
		: HighlightingItem(dlg, itemidx), m_fgcolor(fc), m_bgcolor(bc)
	{}

private:
	virtual wxColor DoInitDialogControls() override
	{
		m_dlg->DisableFontConfig();
		m_dlg->EnableFGColorConfig(m_fgcolor);
		return m_dlg->EnableBGColorConfig(m_bgcolor);
	}
	virtual void SetFGColor(const wxColor& color, const wxString& colorname) override
	{
		m_fgcolor = NormalColor(color, colorname);
	}
	virtual void SetBGColor(const wxColor& color, const wxString& colorname) override
	{
		m_bgcolor = NormalColor(color, colorname);
	}
	virtual wxColor GetFGColor() override
	{
		return m_fgcolor;
	}
	virtual wxColor GetBGColor() override
	{
		return m_bgcolor;
	}
	virtual void SetListItemFontStyle(long itemidx) override {}
	virtual void EnableFontStyle(const MadFontStyles& style, bool enable) override {}

	wxColor& m_fgcolor;
	wxColor& m_bgcolor;
};

void SetItemFontStyle(wxListCtrl* listctrl, long item, const MadFontStyles& style);
struct HighlightingKeyword : public HighlightingItem
{
	HighlightingKeyword(WXMHighlightingDialog* dlg, long itemidx, MadAttributes& attr)
		: HighlightingItem(dlg, itemidx), m_attr(attr)
	{}

private:
	virtual wxColor DoInitDialogControls() override
	{
		m_dlg->EnableFontConfig(m_attr.style);
		m_dlg->EnableFGColorConfig(m_attr.color);
		return m_dlg->EnableBGColorConfig(m_attr.bgcolor);
	}
	virtual void SetFGColor(const wxColor& color, const wxString& colorname) override
	{
		m_attr.color = NormalColor(color, colorname);
	}
	virtual void SetBGColor(const wxColor& color, const wxString& colorname) override
	{
		m_attr.bgcolor = NormalColor(color, colorname);
	}
	virtual wxColor GetFGColor() override
	{
		return m_attr.color;
	}
	virtual wxColor GetBGColor() override
	{
		return m_attr.bgcolor;
	}
	virtual void SetListItemFontStyle(long itemidx) override
	{
		SetItemFontStyle(m_dlg->WxListCtrlKeyword, itemidx, m_attr.style);
	}
	virtual void EnableFontStyle(const MadFontStyles& style, bool enable) override;

	MadAttributes& m_attr;
};

void HighlightingKeyword::EnableFontStyle(const MadFontStyles& sytle, bool enable)
{
	if (enable)
		m_attr.style |= sytle;
	else
		m_attr.style &= (~sytle);
	SetItemFontStyle(m_dlg->WxListCtrlKeyword, m_itemidx, m_attr.style);
}

wxColor HighlightingItem::InitDialogControls(const wxColor& bc0)
{
	wxColor bgc = DoInitDialogControls();
	return (bgc != wxNullColour)? bgc : bc0;
}

void HighlightingItem::RepaintListItem(long itemidx, const wxColor& fc0, const wxColor& bc0)
{
	SetListItemFontStyle(itemidx);
	wxColor fc = GetFGColor();
	wxColor bc = GetBGColor();
	if (fc == wxNullColour) fc = fc0;
	if (bc == wxNullColour) bc = bc0;
	SetItemColour(m_dlg->WxListCtrlKeyword, itemidx, fc, bc);
}

void HighlightingItemManager::PaintListItems()
{
	const wxColor& fc0 = GetTextFGColor();
	const wxColor& bc0 = GetTextBGColor();
	for (long idx = 0; (size_t)idx < m_table.size(); ++idx)
		m_table[idx].RepaintListItem(idx, fc0, bc0);
}

void HighlightingItemManager::SetCurItemFGColor(const wxColor& color, const wxString& colorname)
{
	wxm::HighlightingItem &kinfo = m_table[m_index];
	wxString name = colorname;
	if (m_index == 0)
	{
		SetItemTextColour(m_dlg->WxListCtrlFC, 0, color);
		name.clear();
	}

	kinfo.SetFGColor(color, name);
}

void HighlightingItemManager::SetCurItemBGColor(const wxColor& color, const wxString& colorname)
{
	wxm::HighlightingItem &kinfo = m_table[m_index];
	wxString name = colorname;
	if (m_index == 0)
	{
		SetItemBackgroundColour(m_dlg->WxListCtrlBC, 0, color);
		m_dlg->WxListCtrlKeyword->SetBackgroundColour(color);
		name.clear();
	}

	kinfo.SetBGColor(color, name);
}

void HighlightingItemManager::SetTextColor()
{
	if (m_index != 0)
		return;

	// set (Automatic) colors
	SetItemTextColour(m_dlg->WxListCtrlFC, 0, GetTextFGColor());

	wxColor bgc = GetTextBGColor();

	unsigned char c = (bgc.Red() + bgc.Green() + bgc.Blue()) / 3;
	wxColor fgc = (c >= 128)?  *wxBLACK: *wxWHITE;

	SetItemColour(m_dlg->WxListCtrlBC, 0, fgc, GetTextBGColor());
}

void HighlightingItemManager::InitItems(MadSyntax* syn)
{
	m_table.clear();
	long index = 0;

	// system attributes
	for (int ae = aeText; ae<aeNone; ae++)
	{
		long item = m_dlg->WxListCtrlKeyword->InsertItem(index++, MadSyntax::GetAttributeName((MadAttributeElement)ae));
		MadAttributes* pattr = syn->GetAttributes((MadAttributeElement)ae);
		if (ae == aeActiveLine)
			m_table.push_back(new HighlightingFGOnly(m_dlg, item, pattr->color));
		else if (ae == aeBookmark)
			m_table.push_back(new HighlightingColor(m_dlg, item, pattr->color, pattr->bgcolor));
		else
			m_table.push_back(new HighlightingKeyword(m_dlg, item, *pattr));

		if (ae == aeText)
			m_dlg->WxListCtrlKeyword->SetBackgroundColour(pattr->bgcolor);

		SetItemColour(m_dlg->WxListCtrlKeyword, item, pattr->color, pattr->bgcolor);
		wxm::SetItemFontStyle(m_dlg->WxListCtrlKeyword, item, pattr->style);
	}

	// custom ranges
	size_t i;
	for (i = 0; i < syn->m_CustomRange.size(); ++i)
	{
		wxString text;
		text.Printf(wxT("Range %s %s"), syn->m_CustomRange[i].begin.c_str(), syn->m_CustomRange[i].end.c_str());
		long item = m_dlg->WxListCtrlKeyword->InsertItem(index++, text);
		wxColour& bc = syn->m_CustomRange[i].bgcolor;
		m_table.push_back(new HighlightingBGOnly(m_dlg, item, bc));
		SetItemColour(m_dlg->WxListCtrlKeyword, item, GetTextFGColor(), bc);
	}

	// custom keywords
	for (i = 0; i < syn->m_CustomKeyword.size(); ++i)
	{
		long item = m_dlg->WxListCtrlKeyword->InsertItem(index++, syn->m_CustomKeyword[i].m_Name);
		MadAttributes& attr = syn->m_CustomKeyword[i].m_Attr;
		m_table.push_back(new HighlightingKeyword(m_dlg, item, attr));

		SetItemColour(m_dlg->WxListCtrlKeyword, item, attr.color, attr.bgcolor);
		wxm::SetItemFontStyle(m_dlg->WxListCtrlKeyword, item, attr.style);
	}
}

} // namespace wxm

int g_DefaultFontSize;

wxColour WXMHighlightingDialog::GetColourFromUser(const wxColour& colInit, const wxString& caption)
{
    if(m_ColourDialog==nullptr)
    {
        wxColourData *data=new wxColourData;
        data->SetChooseFull(true);
        if ( colInit.Ok() )
        {
            data->SetColour((wxColour &)colInit); // const_cast
        }
        m_ColourDialog = new wxColourDialog(g_HighlightingDialog, data);
    }
    else
    {
        if ( colInit.Ok() )
        {
            m_ColourDialog->GetColourData().SetColour(colInit);
        }
    }

    if (!caption.IsEmpty())
        m_ColourDialog->SetTitle(caption);

    wxColour colRet;
    if ( m_ColourDialog->ShowModal() == wxID_OK )
    {
        colRet = m_ColourDialog->GetColourData().GetColour();
    }

    return colRet;
}

void SetItemColour(wxListCtrl* listctrl, long item, const wxColour& fc, const wxColour& bc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetTextColour(fc);
    it.SetBackgroundColour(bc);
    listctrl->SetItem(it);
}

void SetItemTextColour(wxListCtrl* listctrl, long item, const wxColour& fc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetTextColour(fc);
    listctrl->SetItem(it);
}
void SetItemBackgroundColour(wxListCtrl* listctrl, long item, const wxColour& bc)
{
    wxListItem it;
    it.SetId(item);
    listctrl->GetItem(it);
    it.SetBackgroundColour(bc);
    listctrl->SetItem(it);
}

void SetFontStyle(wxFont &font, MadFontStyles fs)
{
    if((fs&fsBold)!=0)  font.SetWeight(wxFONTWEIGHT_BOLD);
    else                font.SetWeight(wxFONTWEIGHT_NORMAL);
    if((fs&fsItalic)!=0) font.SetStyle(wxFONTSTYLE_ITALIC);
    else                 font.SetStyle(wxFONTSTYLE_NORMAL);
    font.SetUnderlined((fs&fsUnderline)!=0);
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
    font.SetPointSize(g_DefaultFontSize);
    it.SetFont(font);
    listctrl->SetItem(it);
}

//(*IdInit(WXMHighlightingDialog)
const long WXMHighlightingDialog::ID_WXLISTBOXSYNTAX = wxNewId();
const long WXMHighlightingDialog::ID_STATICTEXT1 = wxNewId();
const long WXMHighlightingDialog::ID_WXCOMBOBOXSCHEME = wxNewId();
const long WXMHighlightingDialog::ID_STATICTEXT2 = wxNewId();
const long WXMHighlightingDialog::ID_WXBUTTONLOAD = wxNewId();
const long WXMHighlightingDialog::ID_WXBUTTONSAVE = wxNewId();
const long WXMHighlightingDialog::ID_WXBUTTONDELETE = wxNewId();
const long WXMHighlightingDialog::ID_STATICLINE1 = wxNewId();
const long WXMHighlightingDialog::ID_WXLISTCTRLKEYWORD = wxNewId();
const long WXMHighlightingDialog::ID_WXCHECKBOXBOLD = wxNewId();
const long WXMHighlightingDialog::ID_WXCHECKBOXITALIC = wxNewId();
const long WXMHighlightingDialog::ID_WXCHECKBOXUNDERLINE = wxNewId();
const long WXMHighlightingDialog::ID_STATICLINE3 = wxNewId();
const long WXMHighlightingDialog::ID_STATICTEXT3 = wxNewId();
const long WXMHighlightingDialog::ID_WXSTATICTEXTFCNAME = wxNewId();
const long WXMHighlightingDialog::ID_WXPANELFC = wxNewId();
const long WXMHighlightingDialog::ID_WXLISTCTRLFC = wxNewId();
const long WXMHighlightingDialog::ID_WXBUTTONFC = wxNewId();
const long WXMHighlightingDialog::ID_STATICLINE2 = wxNewId();
const long WXMHighlightingDialog::ID_STATICTEXT4 = wxNewId();
const long WXMHighlightingDialog::ID_WXSTATICTEXTBCNAME = wxNewId();
const long WXMHighlightingDialog::ID_WXPANELBC = wxNewId();
const long WXMHighlightingDialog::ID_WXLISTCTRLBC = wxNewId();
const long WXMHighlightingDialog::ID_WXBUTTONBC = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMHighlightingDialog,wxDialog)
	//(*EventTable(WXMHighlightingDialog)
	//*)
	EVT_ACTIVATE(WXMHighlightingDialog::WXMHighlightingDialogActivate)
END_EVENT_TABLE()

WXMHighlightingDialog::WXMHighlightingDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
	: m_Syntax(nullptr), m_cur_syn(nullptr), m_ColourDialog(nullptr), m_himgr(this)
{
	//(*Initialize(WXMHighlightingDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;

	Create(parent, wxID_ANY, _("Syntax Highlighting Settings"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	WxListBoxSyntax = new wxListBox(this, ID_WXLISTBOXSYNTAX, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE|wxLB_HSCROLL, wxDefaultValidator, _T("ID_WXLISTBOXSYNTAX"));
	BoxSizer1->Add(WxListBoxSyntax, 1, wxALL|wxEXPAND, 4);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Scheme:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer4->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxComboBoxScheme = new wxComboBox(this, ID_WXCOMBOBOXSCHEME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_WXCOMBOBOXSCHEME"));
	BoxSizer4->Add(WxComboBoxScheme, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("You cannot modify the scheme with * sign."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer4->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonLoad = new wxButton(this, ID_WXBUTTONLOAD, _("&Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONLOAD"));
	BoxSizer6->Add(WxButtonLoad, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonSave = new wxButton(this, ID_WXBUTTONSAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONSAVE"));
	BoxSizer6->Add(WxButtonSave, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxButtonDelete = new wxButton(this, ID_WXBUTTONDELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONDELETE"));
	BoxSizer6->Add(WxButtonDelete, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer2->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND, 1);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxListCtrlKeyword = new wxListCtrl(this, ID_WXLISTCTRLKEYWORD, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_WXLISTCTRLKEYWORD"));
	BoxSizer5->Add(WxListCtrlKeyword, 1, wxALL|wxEXPAND, 2);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxBold = new wxCheckBox(this, ID_WXCHECKBOXBOLD, _("Bold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXBOLD"));
	WxCheckBoxBold->SetValue(false);
	BoxSizer9->Add(WxCheckBoxBold, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxItalic = new wxCheckBox(this, ID_WXCHECKBOXITALIC, _("Italic"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXITALIC"));
	WxCheckBoxItalic->SetValue(false);
	BoxSizer9->Add(WxCheckBoxItalic, 0, wxALL|wxEXPAND, 2);
	WxCheckBoxUnderline = new wxCheckBox(this, ID_WXCHECKBOXUNDERLINE, _("Underline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXUNDERLINE"));
	WxCheckBoxUnderline->SetValue(false);
	BoxSizer9->Add(WxCheckBoxUnderline, 0, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxALIGN_LEFT, 2);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	BoxSizer8->Add(StaticLine3, 0, wxALL|wxEXPAND, 1);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11 = new wxBoxSizer(wxVERTICAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Foreground/Text Color"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer11->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxStaticTextFCName = new wxStaticText(this, ID_WXSTATICTEXTFCNAME, _("WxStaticTextFCName"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTFCNAME"));
	BoxSizer11->Add(WxStaticTextFCName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxPanelFC = new wxPanel(this, ID_WXPANELFC, wxDefaultPosition, wxSize(85,20), wxSIMPLE_BORDER, _T("ID_WXPANELFC"));
	BoxSizer11->Add(WxPanelFC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxListCtrlFC = new wxListCtrl(this, ID_WXLISTCTRLFC, wxDefaultPosition, wxSize(160,140), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_WXLISTCTRLFC"));
	BoxSizer11->Add(WxListCtrlFC, 1, wxALL|wxEXPAND, 2);
	WxButtonFC = new wxButton(this, ID_WXBUTTONFC, _("Other Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONFC"));
	BoxSizer11->Add(WxButtonFC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer10->Add(BoxSizer11, 1, wxALL|wxEXPAND, 2);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(-1,-1), wxLI_VERTICAL, _T("ID_STATICLINE2"));
	BoxSizer10->Add(StaticLine2, 0, wxALL|wxEXPAND, 1);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Background Color"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer12->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxStaticTextBCName = new wxStaticText(this, ID_WXSTATICTEXTBCNAME, _("WxStaticTextBCName"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTBCNAME"));
	BoxSizer12->Add(WxStaticTextBCName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxPanelBC = new wxPanel(this, ID_WXPANELBC, wxDefaultPosition, wxSize(85,20), wxSIMPLE_BORDER, _T("ID_WXPANELBC"));
	BoxSizer12->Add(WxPanelBC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxListCtrlBC = new wxListCtrl(this, ID_WXLISTCTRLBC, wxDefaultPosition, wxSize(160,140), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_WXLISTCTRLBC"));
	BoxSizer12->Add(WxListCtrlBC, 1, wxALL|wxEXPAND, 2);
	WxButtonBC = new wxButton(this, ID_WXBUTTONBC, _("Other Color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONBC"));
	BoxSizer12->Add(WxButtonBC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer10->Add(BoxSizer12, 1, wxALL|wxEXPAND, 2);
	BoxSizer8->Add(BoxSizer10, 1, wxALL|wxEXPAND, 2);
	BoxSizer5->Add(BoxSizer8, 2, wxALL|wxEXPAND, 0);
	BoxSizer2->Add(BoxSizer5, 1, wxALL|wxEXPAND, 0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonClose = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	BoxSizer7->Add(WxButtonClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	WxButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer7->Add(WxButtonCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	BoxSizer2->Add(BoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 3, wxALL|wxEXPAND, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_WXLISTBOXSYNTAX,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&WXMHighlightingDialog::WxListBoxSyntaxSelected);
	Connect(ID_WXBUTTONLOAD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxButtonLoadClick);
	Connect(ID_WXBUTTONSAVE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxButtonSaveClick);
	Connect(ID_WXBUTTONDELETE,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxButtonDeleteClick);
	Connect(ID_WXLISTCTRLKEYWORD,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&WXMHighlightingDialog::WxListCtrlKeywordSelected);
	Connect(ID_WXCHECKBOXBOLD,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxCheckBoxBoldClick);
	Connect(ID_WXCHECKBOXITALIC,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxCheckBoxItalicClick);
	Connect(ID_WXCHECKBOXUNDERLINE,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxCheckBoxUnderlineClick);
	Connect(ID_WXLISTCTRLFC,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&WXMHighlightingDialog::WxListCtrlFCSelected);
	Connect(ID_WXBUTTONFC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxButtonFCClick);
	Connect(ID_WXLISTCTRLBC,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&WXMHighlightingDialog::WxListCtrlBCSelected);
	Connect(ID_WXBUTTONBC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMHighlightingDialog::WxButtonBCClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMHighlightingDialog::WXMHighlightingDialogClose);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&WXMHighlightingDialog::WXMHighlightingDialogResize);
	//*)

#ifdef __WXMSW__
	WxListCtrlKeyword->SetWindowStyle(WxListCtrlKeyword->GetWindowStyle() & ~wxALWAYS_SHOW_SB);
#endif

    {   // build scheme list
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;i++)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
            if(i==0) WxComboBoxScheme->SetValue(name);
        }
    }

    {   // build syntax type list
        size_t cnt=MadSyntax::GetSyntaxCount();
        for(size_t i=0;i<cnt;i++)
        {
            WxListBoxSyntax->Append(MadSyntax::GetSyntaxTitle(i));
        }
    }

    g_DefaultFontSize=WxListCtrlKeyword->GetFont().GetPointSize();
    WxListCtrlKeyword->InsertColumn(0, wxT("first column"));

    // build color list
    WxListCtrlFC->Hide();
    WxListCtrlBC->Hide();
    WxListCtrlFC->InsertColumn(0, wxT("first column"));
    WxListCtrlBC->InsertColumn(0, wxT("first column"));

    wxListItem it;
    it.SetColumn(0);
    it.SetId(0);
    it.SetText(_("(Automatic)"));
    it.SetTextColour(wxT("Red"));
    WxListCtrlFC->InsertItem(it);
    WxListCtrlBC->InsertItem(it);

    HtmlColor *hc=HtmlColorTable;
    for(int i=0; i<HtmlColorTableCount; ++i, ++hc)
    {
        it.SetText(wxGetTranslation(hc->name));
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

WXMHighlightingDialog::~WXMHighlightingDialog()
{
	//(*Destroy(WXMHighlightingDialog)
	//*)
}


void WXMHighlightingDialog::WXMHighlightingDialogClose(wxCloseEvent& event)
{
    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }

    FreeSyntax(false);
    g_HighlightingDialog=nullptr;
    if(m_ColourDialog) delete m_ColourDialog;
    Destroy();
}

void WXMHighlightingDialog::WxListBoxSyntaxSelected(wxCommandEvent& event)
{
    wxString title = WxListBoxSyntax->GetString(event.GetSelection());
    m_cur_syn = GetSyntax(title);

    // build keyword list
    WxListCtrlKeyword->Freeze();
    WxListCtrlKeyword->DeleteAllItems();

    m_himgr.InitItems(m_cur_syn);

    OnListCtrlKeywordResize();
    WxListCtrlKeyword->Thaw();

    OnListCtrlFCResize();
    OnListCtrlBCResize();

    m_himgr.ResetIndex();
    wxListEvent e;
    e.m_itemIndex=0;
    WxListCtrlKeywordSelected(e);
}


void WXMHighlightingDialog::SetPanelFC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextFCName->SetLabel(_("(Automatic)"));
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
        else
        {
            cname = g_color_l10n_map[cname.Upper()];
        }
        WxStaticTextFCName->SetLabel(cname);
        WxPanelFC->SetBackgroundColour(color);
        WxPanelFC->ClearBackground();
    }
}

void WXMHighlightingDialog::SetPanelBC(const wxColor &color)
{
    if(color==wxNullColour)
    {
        WxStaticTextBCName->SetLabel(_("(Automatic)"));
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
        else
        {
            cname = g_color_l10n_map[cname.Upper()];
        }
        WxStaticTextBCName->SetLabel(cname);
        WxPanelBC->SetBackgroundColour(color);
        WxPanelBC->ClearBackground();
    }
}

void WXMHighlightingDialog::EnableFontConfig(const MadFontStyles& style)
{
	WxCheckBoxBold->Enable();
	WxCheckBoxItalic->Enable();
	WxCheckBoxUnderline->Enable();
	WxCheckBoxBold->SetValue((style&fsBold) != 0);
	WxCheckBoxItalic->SetValue((style&fsItalic) != 0);
	WxCheckBoxUnderline->SetValue((style&fsUnderline) != 0);
}

void WXMHighlightingDialog::DisableFontConfig()
{
	WxCheckBoxBold->SetValue(false);
	WxCheckBoxItalic->SetValue(false);
	WxCheckBoxUnderline->SetValue(false);
	WxCheckBoxBold->Disable();
	WxCheckBoxItalic->Disable();
	WxCheckBoxUnderline->Disable();
}

void WXMHighlightingDialog::EnableFGColorConfig(const wxColor& color)
{
	SetPanelFC(color);
	WxListCtrlFC->Enable();
	WxButtonFC->Enable();
}

void WXMHighlightingDialog::DisableFGColorConfig()
{
	SetPanelFC(wxNullColour);
	WxListCtrlFC->Disable();
	WxButtonFC->Disable();
}

wxColor WXMHighlightingDialog::EnableBGColorConfig(const wxColor& bgcolor)
{
	SetPanelBC(bgcolor);
	WxListCtrlBC->Enable();
	WxButtonBC->Enable();
	return bgcolor;
}

wxColor WXMHighlightingDialog::DisableBGColorConfig()
{
	SetPanelBC(wxNullColour);
	WxListCtrlBC->Disable();
	WxButtonBC->Disable();
	return wxNullColour;
}

int GetListCtrlRealClientSize(const wxListCtrl* listctrl)
{
#ifdef __WXMSW__
	return listctrl->GetClientSize().x - 4;
#else
	return listctrl->GetClientSize().x - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
#endif
}

void ListCtrlFitColumn(wxListCtrl* listctrl)
{
	listctrl->SetColumnWidth(0, GetListCtrlRealClientSize(listctrl));
}

void WXMHighlightingDialog::OnListCtrlKeywordResize()
{
	ListCtrlFitColumn(WxListCtrlKeyword);
}

void WXMHighlightingDialog::OnListCtrlFCResize()
{
	ListCtrlFitColumn(WxListCtrlFC);
}

void WXMHighlightingDialog::OnListCtrlBCResize()
{
	ListCtrlFitColumn(WxListCtrlBC);
}

void WXMHighlightingDialog::WxListCtrlKeywordSelected(wxListEvent& event)
{
    long oldIndex = m_himgr.GetIndex();
    m_himgr.SetIndex(event.m_itemIndex);

    WxListCtrlKeyword->Freeze();
    WxListCtrlFC->Freeze();

    if (oldIndex != m_himgr.GetIndex())
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

        str = WxListCtrlKeyword->GetItemText(m_himgr.GetIndex());
        WxListCtrlKeyword->SetItemText(m_himgr.GetIndex(), wxString(wxT('*')) + str);

        OnListCtrlKeywordResize();
    }

    wxm::HighlightingItem &kinfo = m_himgr.GetCurItem();
    m_himgr.SetTextColor();

    wxColor bgc = kinfo.InitDialogControls(m_himgr.GetTextBGColor());
    WxListCtrlFC->SetBackgroundColour(bgc);
    SetItemBackgroundColour(WxListCtrlFC, 0, bgc);
    this->Layout();

    WxListCtrlKeyword->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
    WxListCtrlKeyword->Thaw();
    WxListCtrlFC->Thaw();
}

void WXMHighlightingDialog::WxListCtrlFCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlFC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlFC->GetItemTextColour(event.m_itemIndex);
    WxStaticTextFCName->SetLabel(colorname);
    this->Layout();

    WxPanelFC->SetBackgroundColour(color);
    WxPanelFC->ClearBackground();

    SetAttrFC(color, colorname);
    SetToModifiedSyntax();

    WxListCtrlFC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

void WXMHighlightingDialog::WxListCtrlBCSelected(wxListEvent& event)
{
    wxString colorname=WxListCtrlBC->GetItemText(event.m_itemIndex);
    wxColor color=WxListCtrlBC->GetItemBackgroundColour(event.m_itemIndex);
    WxStaticTextBCName->SetLabel(colorname);
    this->Layout();

    WxPanelBC->SetBackgroundColour(color);
    WxPanelBC->ClearBackground();

    SetAttrBC(color, colorname);
    SetToModifiedSyntax();

    WxListCtrlBC->SetItemState(event.m_itemIndex, 0, wxLIST_STATE_SELECTED);
}

void WXMHighlightingDialog::WxCheckBoxBoldClick(wxCommandEvent& event)
{
    m_himgr.GetCurItem().EnableFontStyle(fsBold, event.IsChecked());
    OnListCtrlKeywordResize();
    SetToModifiedSyntax();
}

void WXMHighlightingDialog::WxCheckBoxItalicClick(wxCommandEvent& event)
{
    m_himgr.GetCurItem().EnableFontStyle(fsItalic, event.IsChecked());
    OnListCtrlKeywordResize();
    SetToModifiedSyntax();
}

void WXMHighlightingDialog::WxCheckBoxUnderlineClick(wxCommandEvent& event)
{
    m_himgr.GetCurItem().EnableFontStyle(fsUnderline, event.IsChecked());
    OnListCtrlKeywordResize();
    SetToModifiedSyntax();
}

void WXMHighlightingDialog::WxButtonFCClick(wxCommandEvent& event)
{
    wxColour color = GetColourFromUser(WxListCtrlKeyword->GetItemTextColour(m_himgr.GetIndex()), StaticText3->GetLabel());
    if (!color.Ok())
        return;

    SetPanelFC(color);
    this->Layout();
    wxString colorname=WxStaticTextFCName->GetLabel();
    SetAttrFC(color, colorname);
    SetToModifiedSyntax();
}

void WXMHighlightingDialog::WxButtonBCClick(wxCommandEvent& event)
{
    wxColour color=GetColourFromUser(WxPanelBC->GetBackgroundColour(), StaticText4->GetLabel());
    if (!color.Ok())
        return;

    SetPanelBC(color);
    this->Layout();
    wxString colorname=WxStaticTextBCName->GetLabel();
    SetAttrBC(color, colorname);
    SetToModifiedSyntax();
}
void WXMHighlightingDialog::WXMHighlightingDialogActivate(wxActivateEvent& event)
{
    if (m_InitSetting.IsEmpty() || !event.GetActive())
        return;

    m_himgr.ResetIndex();

    int i=WxListBoxSyntax->FindString(m_InitSetting);
    if(i==wxNOT_FOUND)
    {
        i=WxListBoxSyntax->GetSelection();
        if(i==wxNOT_FOUND) i=0;
    }
    WxListBoxSyntax->SetSelection(i);

    wxCommandEvent e;
    e.SetInt(i);
    WxListBoxSyntaxSelected(e);

    m_InitSetting.Empty();

    if(FindFocus()==nullptr)
    {
        WxButtonCancel->SetFocus();
    }
}

MadSyntax *WXMHighlightingDialog::GetSyntax(const wxString &title)
{
    if (m_Syntax && m_Syntax->GetTitle().CmpNoCase(title)==0)
        return m_Syntax;

    for (size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        if (m_ModifiedSyntax[i]->GetTitle().CmpNoCase(title) == 0)
            return m_ModifiedSyntax[i];
    }

    if(m_Syntax) delete m_Syntax;
    m_Syntax=MadSyntax::GetSyntaxByTitle(title);
    return m_Syntax;
}

void WXMHighlightingDialog::SetToModifiedSyntax()
{
    if (m_cur_syn == m_Syntax)
    {
        m_ModifiedSyntax.push_back(m_cur_syn);
        m_Syntax = nullptr;
    }

    // apply syntax attributes to editor
    ApplySyntaxAttributes(m_cur_syn);
}

void WXMHighlightingDialog::SetAttrFC(const wxColor &color, const wxString &colorname)
{
    m_himgr.SetCurItemFGColor(color, colorname);
    RepaintKeyword();
}

void WXMHighlightingDialog::SetAttrBC(const wxColor& color, const wxString& colorname)
{
    m_himgr.SetCurItemBGColor(color, colorname);
    RepaintKeyword();

    SetItemBackgroundColour(WxListCtrlFC, 0, color);
    WxListCtrlFC->SetBackgroundColour(color);
    WxListCtrlFC->Refresh();
}

namespace wxm
{
	void SetItemFontStyle(wxListCtrl* listctrl, long item, const MadFontStyles& style)
	{
		wxFont font = GetItemFont(listctrl, item);
		SetFontStyle(font, style);
		SetItemFont(listctrl, item, font);
	}
} // namespace wxm

void WXMHighlightingDialog::RepaintKeyword()
{
    WxListCtrlKeyword->Freeze();
    m_himgr.PaintListItems();
    OnListCtrlKeywordResize();
    WxListCtrlKeyword->Thaw();
}

void WXMHighlightingDialog::FreeSyntax(bool restore)
{
    if (restore) // restore the original syntax
    {
        for (size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            MadSyntax *syn = MadSyntax::GetSyntaxByTitle(m_ModifiedSyntax[i]->GetTitle());
            ApplySyntaxAttributes(syn);
            delete syn;
        }
    }
    else // write the modified syntax back
    {
        for (size_t i=0; i<m_ModifiedSyntax.size(); ++i)
        {
            m_ModifiedSyntax[i]->SaveAttributes();
        }
    }

    if (m_Syntax)
    {
        delete m_Syntax;
        m_Syntax=nullptr;
    }

    for (size_t i=0; i<m_ModifiedSyntax.size(); ++i)
    {
        delete m_ModifiedSyntax[i];
    }
    m_ModifiedSyntax.clear();
}

void WXMHighlightingDialog::WxButtonLoadClick(wxCommandEvent& event)
{
    if(MadSyntax::LoadScheme(WxComboBoxScheme->GetValue(), m_cur_syn))
    {
        WxListCtrlKeyword->SetBackgroundColour(m_himgr.GetTextBGColor());
        RepaintKeyword();
        wxListEvent e;
		e.m_itemIndex = m_himgr.GetIndex();
        WxListCtrlKeywordSelected(e);

        SetToModifiedSyntax();
    }
    else
    {
        wxMessageBox(_("Cannot load this scheme."), wxT("wxMEdit"), wxICON_WARNING|wxOK);
    }
}

void WXMHighlightingDialog::WxButtonSaveClick(wxCommandEvent& event)
{
    wxString schname = WxComboBoxScheme->GetValue();
    if(MadSyntax::SaveScheme(schname, m_cur_syn))
    {
        WxComboBoxScheme->Clear();
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;i++)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
        }
        WxComboBoxScheme->SetValue(schname);
    }
    else
    {
        wxMessageBox(_("Cannot save to the scheme."), wxT("wxMEdit"), wxICON_WARNING|wxOK);
    }
}

void WXMHighlightingDialog::WxButtonDeleteClick(wxCommandEvent& event)
{
    if(MadSyntax::DeleteScheme(WxComboBoxScheme->GetValue()))
    {
        WxComboBoxScheme->Clear();
        size_t cnt=MadSyntax::GetSchemeCount();
        for(size_t i=0;i<cnt;i++)
        {
            wxString name=MadSyntax::GetSchemeName(i);
            WxComboBoxScheme->Append(name);
            if(i==0) WxComboBoxScheme->SetValue(name);
        }
    }
    else
    {
        wxMessageBox(_("Cannot delete this scheme."), wxT("wxMEdit"), wxICON_WARNING|wxOK);
    }
}

void WXMHighlightingDialog::WXMHighlightingDialogResize(wxSizeEvent& event)
{
	OnListCtrlKeywordResize();
	OnListCtrlFCResize();
	OnListCtrlBCResize();
	event.Skip();
}
