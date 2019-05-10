///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_enumeration_dialog.cpp
// Description: Insert Ordered Sequence Dialog
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_enumeration_dialog.h"
#include "../wxm/utils.h"
#include "../wxm/wx_icu.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMEnumerationDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/msgdlg.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/locid.h>
#include <unicode/uversion.h>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4819 )
#endif
#include <boost/format.hpp>
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <boost/lexical_cast.hpp>

#include <limits>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
{

struct NSII64Limit: public virtual NSILimit
{
	virtual int64_t Max() override { return std::numeric_limits<int64_t>::max(); }
};

struct NSIDecNoCase: public virtual NSIBaseAndCase
{
	virtual bool DefaultLower() override { return false; }
	virtual bool HasCase() override { return false; }
	virtual void SetLowercase(bool lower = true) override {}

	virtual void SetBase(char base) override {}
	virtual bool OnlyDec() override { return true; }
protected:
	virtual bool NonDecFormat(UnicodeString& us, int64_t n) override { return false; }
};

struct NSILatinBaseAndCase: public virtual NSIBaseAndCase
{
	NSILatinBaseAndCase() : m_lower(false), m_base('d') {}

	virtual bool DefaultLower() override { return false; }

	virtual bool HasCase() override
	{
		return m_base == 'x' || m_base == 'X';
	}

	virtual void SetLowercase(bool lower = true) override
	{
		m_lower = lower;
		m_base = char(lower ? tolower(m_base) : toupper(m_base));
	}

	virtual void SetBase(char base) override
	{
		if (m_lower && base == 'X')
		{
			m_base = 'x';
			return;
		}

		m_base = base;
	}

	virtual bool OnlyDec() override { return false; }

protected:
	virtual bool NonDecFormat(UnicodeString& us, int64_t n) override
	{
		return HexOctFormat(us, n) || BinFormat(us, n);
	}

private:
	bool HexOctFormat(UnicodeString& us, int64_t n)
	{
		if (m_base != 'X' && m_base != 'x' && m_base != 'o')
			return false;
		std::string fmt = "%";
		fmt.push_back(m_base);
		us = UnicodeString((boost::format(fmt) % n).str().c_str());
		return true;
	}

	bool BinFormat(UnicodeString& us, int64_t n)
	{
		if (m_base != 'b')
			return false;

		if (n == 0)
		{
			us = UnicodeString("0");
			return true;
		}

		int64_t i = 63;
		for (; i >= 0; --i)
		{
			if ((n & (uint64_t(2) << i)) != 0)
				break;
		}
		for (; i >= 0; --i)
		{
			unsigned char d = (n & (uint64_t(2) << i)) != 0 ? '1' : '0';
			us += UChar32(d);
		}
		return true;
	}

	bool m_lower;
	char m_base;
};

NSIWidth::NSIWidth()
{
	UErrorCode uerr = U_ZERO_ERROR;
	UnicodeString wcv("halfwidth-fullwidth");
	m_fwtr.reset(Transliterator::createInstance(wcv, UTRANS_FORWARD, uerr));
}

struct NSIHalfWidth: public virtual NSIWidth
{
	virtual bool DefaultHalfwidth() override { return true; }
	virtual bool HasFullwidth() override { return false; }
	virtual bool HasHalfwidth() override { return true; }
protected:
	virtual void ToHalfWidth(UnicodeString& us) override {}
	virtual void ToFullWidth(UnicodeString& us) override {}
};

struct NSIFullWidth: public virtual NSIWidth
{
	virtual bool DefaultHalfwidth() override { return false; }
	virtual bool HasFullwidth() override { return true; }
	virtual bool HasHalfwidth() override { return false; }
protected:
	virtual void ToHalfWidth(UnicodeString& us) override {}
	virtual void ToFullWidth(UnicodeString& us) override { m_fwtr->transliterate(us); }
};

struct NSIDualWidthLatin: public virtual NSIWidth
{
	virtual bool DefaultHalfwidth() override { return true; }
	virtual bool HasFullwidth() override { return true; }
	virtual bool HasHalfwidth() override { return true; }
protected:
	virtual void ToHalfWidth(UnicodeString& us) override {}
	virtual void ToFullWidth(UnicodeString& us) override { m_fwtr->transliterate(us); }
};

UnicodeString NSIPadding::PadWith(const UnicodeString& us, size_t len, bool alignleft, UChar32 ch)
{
	size_t ulen = size_t(us.countChar32());
	if (ulen >= len)
		return us;

	UnicodeString padding;
	for (size_t i = 0; i<len - ulen; ++i)
		padding += ch;

	if (alignleft)
		return us + padding;

	return padding + us;
}

struct NSIZeroPaddable: public virtual NSIPadding
{
	virtual bool ZeroPaddable() override { return true; }
protected:
	virtual UnicodeString Pad(const UnicodeString& us, size_t len, bool alignleft, bool padzero) override
	{
		return PadWith(us, len, alignleft, (padzero? L'0' : L' '));
	}
};

struct NSINonZeroPaddable: public virtual NSIPadding
{
	virtual bool ZeroPaddable() override { return false; }
protected:
	virtual UnicodeString Pad(const UnicodeString& us, size_t len, bool alignleft, bool) override
	{
		return PadWith(us, len, alignleft, L' ');
	}
};

struct NSIGroupable: public virtual NSIGroupablity
{
	virtual bool Groupable() override { return true; }
};

struct NSINonGroupable: public virtual NSIGroupablity
{
	virtual bool Groupable() override { return false; }
};

struct NumSysLatin: public NumSysBase
	, public NSII64Limit, public NSILatinBaseAndCase, public NSIDualWidthLatin
	, public NSIZeroPaddable, public NSIGroupable
{
	NumSysLatin(const std::string& id): NumSysBase(id) {}
};

struct NumSysArabicLike : public NumSysBase
	, public NSII64Limit, public NSIDecNoCase, public NSINonZeroPaddable, public NSIGroupable
{
	NumSysArabicLike(const std::string& id) : NumSysBase(id) {}
};

struct NumSysArabic : public NumSysArabicLike, public NSIHalfWidth
{
	NumSysArabic(const std::string& id) : NumSysArabicLike(id) {}
};

struct NumSysHaniDec: public NumSysArabicLike, public NSIDualWidthLatin
{
	NumSysHaniDec(const std::string& id) : NumSysArabicLike(id) {}
};

struct NumSys10000Based: public NumSysBase
	, public NSII64Limit, public NSIDecNoCase, public NSINonZeroPaddable, public NSINonGroupable
{
	NumSys10000Based(const std::string& id) : NumSysBase(id) {}

protected:
	virtual UnicodeString TenPetaAndRemainder(int64_t n) = 0;

private:
	virtual UnicodeString DecFormat(int64_t n, bool) override
	{
		const int64_t ten_peta = int64_t(10000)*int64_t(10000)*int64_t(10000)*int64_t(10000);
		if (n < 2*ten_peta)
			return NumSysBase::DecFormat(n, false);

		return NumSysBase::DecFormat(n/ten_peta, false) + TenPetaAndRemainder(ten_peta+n%ten_peta);;
	}
};

struct NumSysChinese: public NumSys10000Based, public NSIFullWidth
{
	NumSysChinese(const std::string& id) : NumSys10000Based(id) {}
private:
	virtual UnicodeString TenPetaAndRemainder(int64_t n) override
	{
		UnicodeString us = NumSysBase::DecFormat(n, false);
		return UnicodeString(us.getBuffer()+1, us.length()-1);
	}
};

struct NumSysEthiopic: public NumSys10000Based, public NSIHalfWidth
{
	NumSysEthiopic(const std::string& id) : NumSys10000Based(id) {}
private:
	virtual UnicodeString TenPetaAndRemainder(int64_t n) override
	{
		return NumSysBase::DecFormat(n, false);
	}
};

struct NumSysLimited: public NumSysBase
	, public NSIHalfWidth, public NSIDecNoCase, public NSINonZeroPaddable, public NSINonGroupable
{
	NumSysLimited(const std::string& id, int64_t max) : NumSysBase(id), m_max(max) {}
	virtual int64_t Max() override { return m_max; }
private:
	int64_t m_max;
};

NumSysBase::NumSysBase(const std::string& id): m_len(0), m_grouping(false)
	, m_fullwidth(false), m_alignleft(false), m_padzero(false)
{
	UErrorCode uerr = U_ZERO_ERROR;
	Locale loc = Locale(("C@numbers=" + id).c_str());
	m_icufmt.reset(NumberFormat::createInstance(loc, uerr));
}

UnicodeString NumSysBase::DecFormat(int64_t n, bool groupping)
{
	UnicodeString us;
	m_icufmt->setGroupingUsed(groupping);
	m_icufmt->format(n, us);
	return us;
}

}// namespace wxm

//(*IdInit(WXMEnumerationDialog)
const long WXMEnumerationDialog::ID_STATICTEXT1 = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT3 = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT2 = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT10 = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXTFINALNUM = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT11 = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLINITIALNUM = wxNewId();
const long WXMEnumerationDialog::ID_CHOICESTEPOP = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLSTEPPARAM = wxNewId();
const long WXMEnumerationDialog::ID_CHOICEFINALCMP = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLFINALNUM = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLPREVIEW = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT6 = wxNewId();
const long WXMEnumerationDialog::ID_CHOICENUMSYS = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONHEX = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONDEC = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONOCT = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONBIN = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONUPPER = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONLOWER = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONHALFWIDTH = wxNewId();
const long WXMEnumerationDialog::ID_RADIOBUTTONFULLWIDTH = wxNewId();
const long WXMEnumerationDialog::ID_CHECKBOXGRPSEP = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT7 = wxNewId();
const long WXMEnumerationDialog::ID_CHOICELENGTH = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT9 = wxNewId();
const long WXMEnumerationDialog::ID_CHOICEALIGN = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT8 = wxNewId();
const long WXMEnumerationDialog::ID_CHOICEPADDING = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT4 = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLPREFIX = wxNewId();
const long WXMEnumerationDialog::ID_STATICTEXT5 = wxNewId();
const long WXMEnumerationDialog::ID_TEXTCTRLPOSTFIX = wxNewId();
//*)

BEGIN_EVENT_TABLE(WXMEnumerationDialog,wxDialog)
	//(*EventTable(WXMEnumerationDialog)
	//*)
END_EVENT_TABLE()

WXMEnumerationDialog::WXMEnumerationDialog(std::vector<ucs4_t>& seq, size_t& seqrows, wxWindow* parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size)
	: m_sequence(seq), m_seqrows(seqrows), m_selrows(1), m_degressive(false), m_exponential(false)
{
	//(*Initialize(WXMEnumerationDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer13;
	wxBoxSizer* BoxSizer14;
	wxBoxSizer* BoxSizer15;
	wxBoxSizer* BoxSizer16;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer19;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer20;
	wxBoxSizer* BoxSizer21;
	wxBoxSizer* BoxSizer22;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;

	Create(parent, wxID_ANY, _("Insert Ordered Sequence"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Sequence"));
	BoxSizer13 = new wxBoxSizer(wxVERTICAL);
	BoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
	StaticTextInitalNum = new wxStaticText(this, ID_STATICTEXT1, _("&Initial Number"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer15->Add(StaticTextInitalNum, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT3, _(":"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer15->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer13->Add(BoxSizer15, 1, wxALL|wxEXPAND, 2);
	BoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
	StaticTextStep = new wxStaticText(this, ID_STATICTEXT2, _("Step"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer16->Add(StaticTextStep, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT10, _(":"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	BoxSizer16->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer13->Add(BoxSizer16, 1, wxALL|wxEXPAND, 2);
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	StaticTextFinalNum = new wxStaticText(this, ID_STATICTEXTFINALNUM, _("Final Number"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXTFINALNUM"));
	BoxSizer17->Add(StaticTextFinalNum, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT11, _(":"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	BoxSizer17->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer13->Add(BoxSizer17, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer1->Add(BoxSizer13, 0, wxALL|wxEXPAND, 2);
	BoxSizer14 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlInitialNum = new wxTextCtrl(this, ID_TEXTCTRLINITIALNUM, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLINITIALNUM"));
	TextCtrlInitialNum->SetMaxLength(66);
	BoxSizer2->Add(TextCtrlInitialNum, 1, wxALL|wxEXPAND, 2);
	BoxSizer14->Add(BoxSizer2, 1, wxALL|wxEXPAND, 2);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ChoiceStepOp = new wxChoice(this, ID_CHOICESTEPOP, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICESTEPOP"));
	ChoiceStepOp->SetSelection( ChoiceStepOp->Append(_("+")) );
	ChoiceStepOp->Append(_("-"));
	ChoiceStepOp->Append(_("*"));
	ChoiceStepOp->Append(_("/"));
	BoxSizer3->Add(ChoiceStepOp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrlStepParam = new wxTextCtrl(this, ID_TEXTCTRLSTEPPARAM, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLSTEPPARAM"));
	TextCtrlStepParam->SetMaxLength(66);
	BoxSizer3->Add(TextCtrlStepParam, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer14->Add(BoxSizer3, 1, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	ChoiceFinalCmp = new wxChoice(this, ID_CHOICEFINALCMP, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEFINALCMP"));
	ChoiceFinalCmp->SetSelection( ChoiceFinalCmp->Append(_("(Automatic)")) );
	ChoiceFinalCmp->Append(_("<="));
	ChoiceFinalCmp->Append(_("<"));
	BoxSizer4->Add(ChoiceFinalCmp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrlFinalNum = new wxTextCtrl(this, ID_TEXTCTRLFINALNUM, _("1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLFINALNUM"));
	TextCtrlFinalNum->SetMaxLength(66);
	TextCtrlFinalNum->SetMinSize(wxSize(64,-1));
	TextCtrlFinalNum->Disable();
	BoxSizer4->Add(TextCtrlFinalNum, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 2);
	BoxSizer14->Add(BoxSizer4, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer1->Add(BoxSizer14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Preview of First 10000 Lines"));
	TextCtrlPreview = new wxTextCtrl(this, ID_TEXTCTRLPREVIEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRLPREVIEW"));
	StaticBoxSizer3->Add(TextCtrlPreview, 1, wxALL|wxEXPAND, 2);
	BoxSizer12->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer12, 1, wxALL|wxEXPAND, 0);
	BoxSizer18 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Format"));
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("&Numbering System:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer5->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ChoiceNumSys = new wxChoice(this, ID_CHOICENUMSYS, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICENUMSYS"));
	BoxSizer5->Add(ChoiceNumSys, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer5, 1, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	RadioButtonHex = new wxRadioButton(this, ID_RADIOBUTTONHEX, _("He&x"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTONHEX"));
	BoxSizer6->Add(RadioButtonHex, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	RadioButtonDec = new wxRadioButton(this, ID_RADIOBUTTONDEC, _("&Dec"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONDEC"));
	RadioButtonDec->SetValue(true);
	BoxSizer6->Add(RadioButtonDec, 0, wxALL|wxEXPAND, 2);
	RadioButtonOct = new wxRadioButton(this, ID_RADIOBUTTONOCT, _("Oc&t"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONOCT"));
	BoxSizer6->Add(RadioButtonOct, 0, wxALL|wxEXPAND, 2);
	RadioButtonBin = new wxRadioButton(this, ID_RADIOBUTTONBIN, _("&Bin"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONBIN"));
	BoxSizer6->Add(RadioButtonBin, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer2->Add(BoxSizer6, 1, wxALL|wxEXPAND, 2);
	BoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
	RadioButtonUpper = new wxRadioButton(this, ID_RADIOBUTTONUPPER, _("&Uppercase"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTONUPPER"));
	RadioButtonUpper->SetValue(true);
	RadioButtonUpper->Disable();
	BoxSizer22->Add(RadioButtonUpper, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	RadioButtonLower = new wxRadioButton(this, ID_RADIOBUTTONLOWER, _("Lowercas&e"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONLOWER"));
	RadioButtonLower->Disable();
	BoxSizer22->Add(RadioButtonLower, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer22, 1, wxALL|wxEXPAND, 2);
	BoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
	RadioButtonHalfWidth = new wxRadioButton(this, ID_RADIOBUTTONHALFWIDTH, _("&Halfwidth"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _T("ID_RADIOBUTTONHALFWIDTH"));
	RadioButtonHalfWidth->SetValue(true);
	BoxSizer20->Add(RadioButtonHalfWidth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	RadioButtonFullWidth = new wxRadioButton(this, ID_RADIOBUTTONFULLWIDTH, _("&Fullwidth"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTONFULLWIDTH"));
	BoxSizer20->Add(RadioButtonFullWidth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer20, 1, wxALL|wxEXPAND, 2);
	BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	CheckBoxGrpSep = new wxCheckBox(this, ID_CHECKBOXGRPSEP, _("Use &Grouping Separator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXGRPSEP"));
	CheckBoxGrpSep->SetValue(false);
	BoxSizer21->Add(CheckBoxGrpSep, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer21, 1, wxALL|wxEXPAND, 2);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("&Length (Characters):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer9->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ChoiceLength = new wxChoice(this, ID_CHOICELENGTH, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICELENGTH"));
	ChoiceLength->SetSelection( ChoiceLength->Append(_("(Automatic)")) );
	BoxSizer9->Add(ChoiceLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer9, 1, wxALL|wxEXPAND, 2);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("&Alignment:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	BoxSizer10->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ChoiceAlign = new wxChoice(this, ID_CHOICEALIGN, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEALIGN"));
	ChoiceAlign->SetSelection( ChoiceAlign->Append(_("Right")) );
	ChoiceAlign->Append(_("Left"));
	ChoiceAlign->Disable();
	BoxSizer10->Add(ChoiceAlign, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer10, 1, wxALL|wxEXPAND, 2);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("&Padding:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	BoxSizer11->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	ChoicePadding = new wxChoice(this, ID_CHOICEPADDING, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICEPADDING"));
	ChoicePadding->SetSelection( ChoicePadding->Append(_("Blank Space")) );
	ChoicePadding->Append(_("0"));
	ChoicePadding->Disable();
	BoxSizer11->Add(ChoicePadding, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer11, 1, wxALL|wxEXPAND, 2);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("P&refix:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer7->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrlPrefix = new wxTextCtrl(this, ID_TEXTCTRLPREFIX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLPREFIX"));
	TextCtrlPrefix->SetMaxLength(32);
	BoxSizer7->Add(TextCtrlPrefix, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer7, 1, wxALL|wxEXPAND, 2);
	BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Po&stfix:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer8->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrlPostfix = new wxTextCtrl(this, ID_TEXTCTRLPOSTFIX, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRLPOSTFIX"));
	TextCtrlPostfix->SetMaxLength(32);
	BoxSizer8->Add(TextCtrlPostfix, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer8, 1, wxALL|wxEXPAND, 2);
	BoxSizer18->Add(StaticBoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	ButtonOK = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	ButtonOK->SetDefault();
	BoxSizer19->Add(ButtonOK, 1, wxALL|wxALIGN_BOTTOM, 5);
	ButtonCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_CANCEL"));
	BoxSizer19->Add(ButtonCancel, 1, wxALL|wxALIGN_BOTTOM, 5);
	BoxSizer18->Add(BoxSizer19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 2);
	BoxSizer1->Add(BoxSizer18, 0, wxALL|wxEXPAND, 0);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_TEXTCTRLINITIALNUM,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_CHOICESTEPOP,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnChoiceStepOpSelect);
	Connect(ID_TEXTCTRLSTEPPARAM,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_CHOICEFINALCMP,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnChoiceFinalCmpSelect);
	Connect(ID_TEXTCTRLFINALNUM,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_CHOICENUMSYS,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnChoiceNumSysSelect);
	Connect(ID_RADIOBUTTONHEX,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonHexSelect);
	Connect(ID_RADIOBUTTONDEC,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonDecSelect);
	Connect(ID_RADIOBUTTONOCT,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonOctSelect);
	Connect(ID_RADIOBUTTONBIN,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonBinSelect);
	Connect(ID_RADIOBUTTONUPPER,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonUpperSelect);
	Connect(ID_RADIOBUTTONLOWER,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnRadioButtonLowerSelect);
	Connect(ID_RADIOBUTTONHALFWIDTH,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_RADIOBUTTONFULLWIDTH,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_CHECKBOXGRPSEP,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_CHOICELENGTH,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnChoiceLengthSelect);
	Connect(ID_CHOICEALIGN,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnChoiceAlignSelect);
	Connect(ID_CHOICEPADDING,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_TEXTCTRLPREFIX,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(ID_TEXTCTRLPOSTFIX,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&WXMEnumerationDialog::OnResultChange);
	Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEnumerationDialog::OnButtonOKClick);
	//*)

	std::vector<wxString> ns_titles(wxm::NumberingSystemConfig::Instance().GetTitles());
	BOOST_FOREACH(const wxString& nst, ns_titles)
		ChoiceNumSys->Append(nst);
	ChoiceNumSys->SetSelection(0);
	m_numsys = wxm::NumberingSystemConfig::Instance().IndexToVal(ChoiceNumSys->GetSelection());
	m_numsys->SetLowercase(false);
	m_numsys->SetBase('d');

	for (size_t i = 1; i <= 64; ++i)
		ChoiceLength->Append(wxString() << i);

	m_cmps[_("<=")] = std::less_equal<int64_t>();
	m_cmps[_("<")]  = std::less<int64_t>();
	m_cmps[_(">=")] = std::greater_equal<int64_t>();
	m_cmps[_(">")]  = std::greater<int64_t>();

	m_ops[_("+")] = std::plus<int64_t>();
	m_ops[_("-")] = std::minus<int64_t>();
	m_ops[_("*")] = std::multiplies<int64_t>();
	m_ops[_("/")] = std::divides<int64_t>();

	wxm::SetDefaultMonoFont(TextCtrlPreview);

	Preview();
}

WXMEnumerationDialog::~WXMEnumerationDialog()
{
	//(*Destroy(WXMEnumerationDialog)
	//*)
}

namespace wxm
{

void NumberingSystemConfig::DoInit()
{

#if U_ICU_VERSION_MAJOR_NUM > 50
	const int64_t greekmax = 999999;
#else
	const int64_t greekmax = 9999;
#endif

	int i = 0;
	AddData(i++,    wxT("latn"),    _("Default (Latin)"),               NumSysSharedPtr(new NumSysLatin("latn")));
	AddData(i++,    wxT("arab"),    _("Arabic"),                        NumSysSharedPtr(new NumSysArabic("arab")));
	AddData(i++,    wxT("armn"),    _("Armenian"),                      NumSysSharedPtr(new NumSysLimited("armn", 9999)));
	AddData(i++,    wxT("beng"),    _("Bengali"),                       NumSysSharedPtr(new NumSysArabic("beng")));
	AddData(i++,    wxT("hanidec"), _("Chinese Digits"),                NumSysSharedPtr(new NumSysHaniDec("hanidec")));
	AddData(i++,    wxT("hans"),    _("Chinese Simplified"),            NumSysSharedPtr(new NumSysChinese("hans")));
	AddData(i++,    wxT("hansfin"), _("Chinese Simplified Financial"),  NumSysSharedPtr(new NumSysChinese("hansfin")));
	AddData(i++,    wxT("hant"),    _("Chinese Traditional"),           NumSysSharedPtr(new NumSysChinese("hant")));
	AddData(i++,    wxT("hantfin"), _("Chinese Traditional Financial"), NumSysSharedPtr(new NumSysChinese("hantfin")));
	AddData(i++,    wxT("deva"),    _("Devanagari"),                    NumSysSharedPtr(new NumSysArabic("deva")));
	AddData(i++,    wxT("ethi"),    _("Ethiopic"),                      NumSysSharedPtr(new NumSysEthiopic("ethi")));
	AddData(i++,    wxT("geor"),    _("Georgian"),                      NumSysSharedPtr(new NumSysLimited("geor", 19999)));
	AddData(i++,    wxT("grek"),    _("Greek"),                         NumSysSharedPtr(new NumSysLimited("grek", greekmax)));
	AddData(i++,    wxT("gujr"),    _("Gujarati"),                      NumSysSharedPtr(new NumSysArabic("gujr")));
	AddData(i++,    wxT("guru"),    _("Gurmukhi"),                      NumSysSharedPtr(new NumSysArabic("guru")));
	AddData(i++,    wxT("hebr"),    _("Hebrew"),                        NumSysSharedPtr(new NumSysLimited("hebr", 1000000)));
	AddData(i++,    wxT("jpan"),    _("Japanese"),                      NumSysSharedPtr(new NumSysChinese("jpan")));
	AddData(i++,    wxT("jpanfin"), _("Japanese Financial"),            NumSysSharedPtr(new NumSysChinese("jpanfin")));
	AddData(i++,    wxT("knda"),    _("Kannada"),                       NumSysSharedPtr(new NumSysArabic("knda")));
	AddData(i++,    wxT("khmr"),    _("Khmer"),                         NumSysSharedPtr(new NumSysArabic("khmr")));
	AddData(i++,    wxT("laoo"),    _("Lao"),                           NumSysSharedPtr(new NumSysArabic("laoo")));
	AddData(i++,    wxT("mlym"),    _("Malayalam"),                     NumSysSharedPtr(new NumSysArabic("mlym")));
	AddData(i++,    wxT("mymr"),    _("Myanmar"),                       NumSysSharedPtr(new NumSysArabic("mymr")));
	AddData(i++,    wxT("orya"),    _("Oriya"),                         NumSysSharedPtr(new NumSysArabic("orya")));
	AddData(i++,    wxT("arabext"), _("Perso-Arabic"),                  NumSysSharedPtr(new NumSysArabic("arabext")));
	AddData(i++,    wxT("taml"),    _("Tamil"),                         NumSysSharedPtr(new NumSysLimited("taml", 99999999)));
	AddData(i++,    wxT("tamldec"), _("Tamil Digits"),                  NumSysSharedPtr(new NumSysArabic("tamldec")));
	AddData(i++,    wxT("telu"),    _("Telugu"),                        NumSysSharedPtr(new NumSysArabic("telu")));
	AddData(i++,    wxT("tibt"),    _("Tibetan"),                       NumSysSharedPtr(new NumSysArabic("tibt")));
	AddData(i++,    wxT("thai"),    _("Thai"),                          NumSysSharedPtr(new NumSysArabic("thai")));
	AddData(i++,    wxT("vaii"),    _("Vai"),                           NumSysSharedPtr(new NumSysArabic("vaii")));
}

bool CheckTextNumMin(const wxStaticText* label, const wxTextCtrl* txtctrl, int64_t minval, wxString& errmsg)
{
	wxString name = label->GetLabelText();
	wxString txtnum = txtctrl->GetValue();
	int64_t n = -1;
	try
	{
		n = boost::lexical_cast<int64_t>(std::wstring(txtnum.wc_str()));
	}
	catch (boost::bad_lexical_cast&)
	{
		errmsg = wxString::Format(_("%s is Not a Number."), name.wc_str());
		return false;
	}

	if (n < minval)
	{
		errmsg = wxString::Format(_("%s should not less than %d."), name.wc_str(), minval);
		return false;
	}
	return true;
}

wxString GetSelectWXStr(wxChoice* choice)
{
	return choice->GetString(choice->GetSelection());
}

int64_t GetI64FromTextCtrl(const wxTextCtrl* txtctrl, int64_t defaultval=0)
{
	try
	{
		return boost::lexical_cast<int64_t>(std::wstring(txtctrl->GetValue().wc_str()));
	}
	catch (boost::bad_lexical_cast&)
	{}

	return defaultval;
}

}// namespace wxm

static const int64_t i64max = std::numeric_limits<int64_t>::max();
static const size_t sizemax = std::numeric_limits<size_t>::max();

bool WXMEnumerationDialog::NumValid(wxString& errmsg)
{
	return wxm::CheckTextNumMin(StaticTextInitalNum, TextCtrlInitialNum, 1, errmsg)
		&& wxm::CheckTextNumMin(StaticTextStep, TextCtrlStepParam, MinStep(), errmsg)
		&& (ChoiceFinalCmp->GetSelection() == 0 ||
			wxm::CheckTextNumMin(StaticTextFinalNum, TextCtrlFinalNum, 1, errmsg));
}

size_t WXMEnumerationDialog::Enumerate(UnicodeString& text, bool preview)
{
	m_numsys->SetLength(size_t(ChoiceLength->GetSelection()));
	m_numsys->SetFullWidth(RadioButtonFullWidth->GetValue());
	m_numsys->SetGrouping(CheckBoxGrpSep->GetValue());
	m_numsys->SetAlignLeft(ChoiceAlign->GetSelection() != 0);
	m_numsys->SetPadZero(ChoicePadding->GetSelection() != 0);

	int64_t initalnum = wxm::GetI64FromTextCtrl(TextCtrlInitialNum);
	int64_t step = wxm::GetI64FromTextCtrl(TextCtrlStepParam);

	bool autofinal = (ChoiceFinalCmp->GetSelection() == 0);

	int64_t finalnum = (m_degressive)? 1: i64max;
	if (!autofinal)
		finalnum = wxm::GetI64FromTextCtrl(TextCtrlFinalNum);

	size_t maxtimes = preview? 10000: sizemax;
	size_t times = autofinal? m_selrows: maxtimes;

	unsigned int cmpidx = ChoiceFinalCmp->GetSelection() == 0? 1: ChoiceFinalCmp->GetSelection();
	CmpFunc cmp = m_cmps[ChoiceFinalCmp->GetString(cmpidx)];
	OpFunc op = m_ops[wxm::GetSelectWXStr(ChoiceStepOp)];

	UnicodeString prefix = wxm::WxStrToICU(TextCtrlPrefix->GetValue());
	UnicodeString postfix = wxm::WxStrToICU(TextCtrlPostfix->GetValue());
	size_t t = 0;
	for (int64_t i = initalnum; cmp(i, finalnum) && t < times; i = op(i, step), ++t)
		text += prefix + m_numsys->Format(i) + postfix + UChar32('\n');
	return t;
}

void WXMEnumerationDialog::Preview()
{
	wxString errmsg;
	bool numvalid = NumValid(errmsg);

	if (!numvalid)
	{
		TextCtrlPreview->SetValue(errmsg);
		return;
	}

	UnicodeString text;
	Enumerate(text, true);
	TextCtrlPreview->SetValue(wxm::ICUStrToWx(text));
}

void WXMEnumerationDialog::OnButtonOKClick(wxCommandEvent& event)
{
	wxString errmsg;
	bool numvalid = NumValid(errmsg);

	if (!numvalid)
	{
		wxMessageBox(errmsg, _("Wrong Input"), wxICON_ERROR);
		event.Skip(false);
		return;
	}

	TextCtrlPreview->SetValue(wxEmptyString);

	UnicodeString us;
	m_seqrows = Enumerate(us);

	m_sequence.clear();
	int32_t ulen = us.countChar32();
	for (int32_t i=0; i<ulen; i=us.moveIndex32(i, 1))
		m_sequence.push_back(us.char32At(i));

	event.Skip(true);
}

void WXMEnumerationDialog::OnChoiceNumSysSelect(wxCommandEvent& event)
{
	wxm::NumSysSharedPtr ns = wxm::NumberingSystemConfig::Instance().IndexToVal(ChoiceNumSys->GetSelection());
	if (m_numsys == ns)
		return;
	m_numsys = ns;

	bool nondec_support = !ns->OnlyDec();
	ns->SetBase('d');
	RadioButtonDec->SetValue(true);
	RadioButtonHex->Enable(nondec_support);
	RadioButtonOct->Enable(nondec_support);
	RadioButtonBin->Enable(nondec_support);

	bool hascase = ns->HasCase();
	RadioButtonUpper->Enable(hascase);
	RadioButtonLower->Enable(hascase);

	RadioButtonHalfWidth->SetValue(ns->DefaultHalfwidth());
	RadioButtonFullWidth->SetValue(!ns->DefaultHalfwidth());
	RadioButtonHalfWidth->Enable(ns->HasHalfwidth());
	RadioButtonFullWidth->Enable(ns->HasFullwidth());

	ChoicePadding->Select(0);
	AdjustChoicePadding(true);

	CheckBoxGrpSep->SetValue(false);
	CheckBoxGrpSep->Enable(ns->Groupable());

	if (ns->Max() < std::numeric_limits<int64_t>::max())
	{
		wxString note0 = wxString::Format(_("Note: This numbering system can only represent the range of 1-%d.\n"), ns->Max());
		wxString note1 = _("Numbers out of range will be represent in Latin numbering system with grouping separator.");
		wxMessageBox(note0 + note1, wxT("wxMEdit"), wxICON_INFORMATION);
	}

	Preview();
}

void WXMEnumerationDialog::OnChoiceFinalCmpSelect(wxCommandEvent& event)
{
	TextCtrlFinalNum->Enable(ChoiceFinalCmp->GetSelection() != 0);

	Preview();
}

void WXMEnumerationDialog::OnChoiceStepOpSelect(wxCommandEvent& event)
{
	bool exponential = (ChoiceStepOp->GetSelection() >= 2);
	if (m_exponential != exponential)
	{
		m_exponential = exponential;
		if (wxm::GetI64FromTextCtrl(TextCtrlStepParam) < MinStep())
			TextCtrlStepParam->SetValue(wxString() << MinStep());
	}

	bool degressive = (ChoiceStepOp->GetSelection() % 2 != 0);
	if (m_degressive != degressive)
	{
		m_degressive = degressive;

		int sel = ChoiceFinalCmp->GetSelection();
		if (degressive)
		{
			ChoiceFinalCmp->SetString(1, _(">="));
			ChoiceFinalCmp->SetString(2, _(">"));
		}
		else
		{
			ChoiceFinalCmp->SetString(1, _("<="));
			ChoiceFinalCmp->SetString(2, _("<"));
		}
		ChoiceFinalCmp->SetSelection(sel);
	}

	Preview();
}

void WXMEnumerationDialog::OnResultChange(wxCommandEvent& event)
{
	Preview();
}

void WXMEnumerationDialog::OnBaseSelect(char base)
{
	m_numsys->SetBase(base);

	if (base != 'd')
		CheckBoxGrpSep->SetValue(false);
	CheckBoxGrpSep->Enable(base == 'd');

	bool hascase = m_numsys->HasCase();
	RadioButtonUpper->Enable(hascase);
	RadioButtonLower->Enable(hascase);

	Preview();
}

void WXMEnumerationDialog::AdjustChoicePadding(bool alignable)
{
	bool paddable = alignable && m_numsys->ZeroPaddable() && (ChoiceAlign->GetSelection() == 0);
	if (!paddable)
		ChoicePadding->SetSelection(0);
	ChoicePadding->Enable(paddable);

	Preview();
}

void WXMEnumerationDialog::OnChoiceLengthSelect(wxCommandEvent& event)
{
	bool alignable = (ChoiceLength->GetSelection() != 0);
	ChoiceAlign->Enable(alignable);
	AdjustChoicePadding(alignable);

	Preview();
}

void WXMEnumerationDialog::OnChoiceAlignSelect(wxCommandEvent& event)
{
	AdjustChoicePadding(true);

	Preview();
}

void WXMEnumerationDialog::OnRadioButtonHexSelect(wxCommandEvent& event)
{
	OnBaseSelect('X');
}

void WXMEnumerationDialog::OnRadioButtonDecSelect(wxCommandEvent& event)
{
	OnBaseSelect('d');
}

void WXMEnumerationDialog::OnRadioButtonOctSelect(wxCommandEvent& event)
{
	OnBaseSelect('o');
}

void WXMEnumerationDialog::OnRadioButtonBinSelect(wxCommandEvent& event)
{
	OnBaseSelect('b');
}

void WXMEnumerationDialog::OnRadioButtonUpperSelect(wxCommandEvent& event)
{
	m_numsys->SetLowercase(false);

	Preview();
}

void WXMEnumerationDialog::OnRadioButtonLowerSelect(wxCommandEvent& event)
{
	m_numsys->SetLowercase(true);

	Preview();
}

#ifdef _MSC_VER
# pragma warning( pop )
#endif
