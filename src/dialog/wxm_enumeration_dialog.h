///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_enumeration_dialog.h
// Description: Insert Ordered Sequence Dialog
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_ENUMERATION_DIALOG_H
#define WXM_ENUMERATION_DIALOG_H

#include "../xm/cxx11.h"
#include "../wxm/choice_map.hpp"
#include "../xm/encoding/encoding_def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMEnumerationDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/numfmt.h>
using namespace U_ICU_NAMESPACE;

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <limits>
#include <vector>

namespace wxm
{
	struct NSILimit
	{
		virtual int64_t Max() = 0;
		virtual ~NSILimit() {}
	};

	struct NSIBaseAndCase
	{
		virtual bool DefaultLower() = 0;
		virtual bool HasCase() = 0;
		virtual void SetLowercase(bool lower = true) = 0;
		virtual void SetBase(char base) = 0;
		virtual bool OnlyDec() = 0;
		virtual ~NSIBaseAndCase() {}
	protected:
		virtual bool NonDecFormat(UnicodeString& us, int64_t n) = 0;
	};

	struct NSIWidth
	{
		NSIWidth();
		virtual bool DefaultHalfwidth() = 0;
		virtual bool HasFullwidth() = 0;
		virtual bool HasHalfwidth() = 0;
		virtual ~NSIWidth() {}
	protected:
		virtual void ToHalfWidth(UnicodeString& us) = 0;
		virtual void ToFullWidth(UnicodeString& us) = 0;
		boost::scoped_ptr<Transliterator> m_fwtr;
	};

	struct NSIPadding
	{
		virtual bool ZeroPaddable() = 0;
		virtual ~NSIPadding() {}
	protected:
		virtual UnicodeString Pad(const UnicodeString& us, size_t len, bool alignleft, bool padzero) = 0;
		UnicodeString PadWith(const UnicodeString& us, size_t len, bool alignleft, UChar32 ch);
	};

	struct NSIGroupablity
	{
		virtual bool Groupable() = 0;
		virtual ~NSIGroupablity() {}
	};

	struct NumSysBase : public virtual NSILimit, public virtual NSIBaseAndCase, public virtual NSIWidth
		, public virtual NSIPadding, public virtual NSIGroupablity
	{
		NumSysBase(const std::string& id);

		void SetLength(size_t len) { m_len = len; }
		void SetFullWidth(bool fullwidth = true){ m_fullwidth = fullwidth; }
		void SetGrouping(bool grouping = true) { m_grouping = grouping; }
		void SetAlignLeft(bool alignleft = true){ m_alignleft = alignleft; }
		void SetPadZero(bool padzero = true){ m_padzero = padzero; }
		UnicodeString Format(int64_t n)
		{
			UnicodeString usnum = BaseFormat(n, m_grouping);
			UnicodeString us = Pad(usnum, m_len, m_alignleft, m_padzero);
			if (m_fullwidth)
				ToFullWidth(us);
			return us;
		}

	protected:
		virtual UnicodeString DecFormat(int64_t n, bool groupping);

	private:
		UnicodeString BaseFormat(int64_t n, bool groupping)
		{
			UnicodeString us;
			if (NonDecFormat(us, n))
				return us;
			return DecFormat(n, groupping);
		}

		boost::scoped_ptr <NumberFormat> m_icufmt;
		size_t m_len;
		bool m_grouping;
		bool m_fullwidth;
		bool m_alignleft;
		bool m_padzero;
	};

	typedef boost::shared_ptr<NumSysBase> NumSysSharedPtr;
	class NumberingSystemConfig: public ChoiceMap <NumberingSystemConfig, NumSysSharedPtr>
	{
		friend class ChoiceMap <NumberingSystemConfig, NumSysSharedPtr>;
		NumberingSystemConfig() {}

		virtual void DoInit() override;
		static const int default_idx = 0;
	};

}// namespace wxm

class WXMEnumerationDialog: public wxDialog
{
	public:

		WXMEnumerationDialog(std::vector<ucs4_t>& seq, size_t& seqlines, wxWindow* parent,
			wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		virtual ~WXMEnumerationDialog();

		//(*Declarations(WXMEnumerationDialog)
		wxButton* ButtonCancel;
		wxButton* ButtonOK;
		wxCheckBox* CheckBoxGrpSep;
		wxChoice* ChoiceAlign;
		wxChoice* ChoiceFinalCmp;
		wxChoice* ChoiceLength;
		wxChoice* ChoiceNumSys;
		wxChoice* ChoicePadding;
		wxChoice* ChoiceStepOp;
		wxRadioButton* RadioButtonBin;
		wxRadioButton* RadioButtonDec;
		wxRadioButton* RadioButtonFullWidth;
		wxRadioButton* RadioButtonHalfWidth;
		wxRadioButton* RadioButtonHex;
		wxRadioButton* RadioButtonLower;
		wxRadioButton* RadioButtonOct;
		wxRadioButton* RadioButtonUpper;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* StaticTextFinalNum;
		wxStaticText* StaticTextInitalNum;
		wxStaticText* StaticTextStep;
		wxTextCtrl* TextCtrlFinalNum;
		wxTextCtrl* TextCtrlInitialNum;
		wxTextCtrl* TextCtrlPostfix;
		wxTextCtrl* TextCtrlPrefix;
		wxTextCtrl* TextCtrlPreview;
		wxTextCtrl* TextCtrlStepParam;
		//*)

		void SetSelectedRows(size_t n) { m_selrows = n; Preview(); }

	protected:

		//(*Identifiers(WXMEnumerationDialog)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXTFINALNUM;
		static const long ID_STATICTEXT11;
		static const long ID_TEXTCTRLINITIALNUM;
		static const long ID_CHOICESTEPOP;
		static const long ID_TEXTCTRLSTEPPARAM;
		static const long ID_CHOICEFINALCMP;
		static const long ID_TEXTCTRLFINALNUM;
		static const long ID_TEXTCTRLPREVIEW;
		static const long ID_STATICTEXT6;
		static const long ID_CHOICENUMSYS;
		static const long ID_RADIOBUTTONHEX;
		static const long ID_RADIOBUTTONDEC;
		static const long ID_RADIOBUTTONOCT;
		static const long ID_RADIOBUTTONBIN;
		static const long ID_RADIOBUTTONUPPER;
		static const long ID_RADIOBUTTONLOWER;
		static const long ID_RADIOBUTTONHALFWIDTH;
		static const long ID_RADIOBUTTONFULLWIDTH;
		static const long ID_CHECKBOXGRPSEP;
		static const long ID_STATICTEXT7;
		static const long ID_CHOICELENGTH;
		static const long ID_STATICTEXT9;
		static const long ID_CHOICEALIGN;
		static const long ID_STATICTEXT8;
		static const long ID_CHOICEPADDING;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRLPREFIX;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRLPOSTFIX;
		//*)

	private:

		//(*Handlers(WXMEnumerationDialog)
		void OnButtonOKClick(wxCommandEvent& event);
		void OnChoiceNumSysSelect(wxCommandEvent& event);
		void OnChoiceFinalCmpSelect(wxCommandEvent& event);
		void OnChoiceStepOpSelect(wxCommandEvent& event);
		void OnResultChange(wxCommandEvent& event);
		void OnChoiceLengthSelect(wxCommandEvent& event);
		void OnChoiceAlignSelect(wxCommandEvent& event);
		void OnRadioButtonHexSelect(wxCommandEvent& event);
		void OnRadioButtonDecSelect(wxCommandEvent& event);
		void OnRadioButtonOctSelect(wxCommandEvent& event);
		void OnRadioButtonBinSelect(wxCommandEvent& event);
		void OnRadioButtonUpperSelect(wxCommandEvent& event);
		void OnRadioButtonLowerSelect(wxCommandEvent& event);
		//*)

		bool NumValid(wxString& errmsg);
		size_t Enumerate(UnicodeString& text, bool preview=false);
		void Preview();
		void AdjustChoicePadding(bool alignable);
		void OnBaseSelect(char base);

		int64_t MinStep()
		{
			return m_exponential ? 2 : 1;
		}

		std::vector<ucs4_t>& m_sequence;
		size_t& m_seqrows;
		size_t m_selrows;

		wxm::NumSysSharedPtr m_numsys;

		bool m_degressive;
		bool m_exponential;

		typedef boost::function<bool(int64_t, int64_t)> CmpFunc;
		typedef boost::function<int64_t(int64_t, int64_t)> OpFunc;
		std::map<wxString, CmpFunc> m_cmps;
		std::map<wxString, OpFunc> m_ops;

		DECLARE_EVENT_TABLE()
};

#endif
