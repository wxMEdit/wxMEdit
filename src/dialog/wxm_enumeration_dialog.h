///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_enumeration_dialog.h
// Description: Insert Ordered Sequence Dialog
// Copyright:   2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_ENUMERATION_DIALOG_H
#define WXM_ENUMERATION_DIALOG_H

#include "../xm/cxx11.h"
#include "../wxm/choice_map.hpp"
#include "../wxmedit/ucs4_t.h"

//(*Headers(WXMEnumerationDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/numfmt.h>

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

	struct NSIWidth
	{
		NSIWidth();
		virtual bool DefaultHalfwidth() = 0;
		virtual bool HasFullwidth() = 0;
		virtual bool HasHalfwidth() = 0;
	protected:
		virtual void ToHalfWidth(UnicodeString& us) = 0;
		virtual void ToFullWidth(UnicodeString& us) = 0;
		boost::scoped_ptr<icu::Transliterator> m_fwtr;
		virtual ~NSIWidth() {}
	};

	struct NSIFormat
	{
		virtual bool OnlyDec() = 0;
		virtual bool ZeroPaddable() = 0;
		virtual ~NSIFormat() {}
	protected:
		virtual bool NonDecFormat(UnicodeString& us, int64_t n, char base) = 0;
		virtual UnicodeString Pad(const UnicodeString& us, size_t len, bool alignleft, bool padzero) = 0;
		UnicodeString PadWith(const UnicodeString& us, size_t len, bool alignleft, UChar32 ch);
	};

	struct NSIGroupablity
	{
		virtual bool Groupable() = 0;
		virtual ~NSIGroupablity() {}
	};

	struct NumSysBase : public virtual NSILimit, public virtual NSIWidth
		, public virtual NSIFormat, public virtual NSIGroupablity
	{
		NumSysBase(const std::string& id);

		void SetLength(size_t len) { m_len = len; }
		void SetBase(char base) { m_base = base; }
		void SetGrouping(bool grouping = true) { m_grouping = grouping; }
		void SetFullWidth(bool fullwidth = true){ m_fullwidth = fullwidth; }
		void SetAlignLeft(bool alignleft = true){ m_alignleft = alignleft; }
		void SetPadZero(bool padzero = true){ m_padzero = padzero; }
		UnicodeString Format(int64_t n)
		{
			UnicodeString usnum = BaseFormat(n, m_base, m_grouping);
			UnicodeString us = Pad(usnum, m_len, m_alignleft, m_padzero);
			if (m_fullwidth)
				ToFullWidth(us);
			return us;
		}

	protected:
		virtual UnicodeString DecFormat(int64_t n, bool groupping);

	private:
		UnicodeString BaseFormat(int64_t n, int base, bool groupping)
		{
			UnicodeString us;
			if (NonDecFormat(us, n, base))
				return us;
			return DecFormat(n, groupping);
		}

		boost::scoped_ptr <icu::NumberFormat> m_icufmt;
		size_t m_len;
		char m_base;
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
		wxRadioButton* RadioButtonHalfWidth;
		wxStaticText* StaticText9;
		wxRadioButton* RadioButtonBin;
		wxTextCtrl* TextCtrlFinalNum;
		wxStaticText* StaticText2;
		wxStaticText* StaticTextInitalNum;
		wxStaticText* StaticText6;
		wxRadioButton* RadioButtonLowerHex;
		wxButton* ButtonOK;
		wxChoice* ChoiceFinalCmp;
		wxChoice* ChoiceStepOp;
		wxStaticText* StaticText8;
		wxTextCtrl* TextCtrlPreview;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxRadioButton* RadioButtonUpperHex;
		wxButton* ButtonCancel;
		wxChoice* ChoicePadding;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxRadioButton* RadioButtonOct;
		wxStaticText* StaticTextStep;
		wxRadioButton* RadioButtonDec;
		wxChoice* ChoiceAlign;
		wxTextCtrl* TextCtrlStepParam;
		wxStaticText* StaticTextFinalNum;
		wxChoice* ChoiceLength;
		wxTextCtrl* TextCtrlPostfix;
		wxStaticText* StaticText4;
		wxRadioButton* RadioButtonFullWidth;
		wxChoice* ChoiceNumSys;
		wxTextCtrl* TextCtrlPrefix;
		wxCheckBox* CheckBoxGrpSep;
		wxTextCtrl* TextCtrlInitialNum;
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
		static const long ID_RADIOBUTTONLOWERHEX;
		static const long ID_RADIOBUTTONDEC;
		static const long ID_RADIOBUTTONOCT;
		static const long ID_RADIOBUTTONBIN;
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
		void OnRadioButtonUpperHexSelect(wxCommandEvent& event);
		void OnRadioButtonLowerHexSelect(wxCommandEvent& event);
		void OnRadioButtonDecSelect(wxCommandEvent& event);
		void OnRadioButtonOctSelect(wxCommandEvent& event);
		void OnRadioButtonBinSelect(wxCommandEvent& event);
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
		size_t m_seqrows;
		size_t m_selrows;

		wxm::NumSysSharedPtr m_numsys;

		bool m_degressive;
		bool m_exponential;
		char m_base;

		typedef boost::function<bool(int64_t, int64_t)> CmpFunc;
		typedef boost::function<int64_t(int64_t, int64_t)> OpFunc;
		std::map<wxString, CmpFunc> m_cmps;
		std::map<wxString, OpFunc> m_ops;

		DECLARE_EVENT_TABLE()
};

#endif
