///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_highlighting_dialog.h
// Description: Syntax Highlight Settings Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2006-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_HIGHLIGHTING_DIALOG_H
#define WXM_HIGHLIGHTING_DIALOG_H


#include "../wxm/wx_avoid_wxmsw_bug4373.h"
#include "../wxmedit/wxm_syntax.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMHighlightingDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/colordlg.h>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

#include <vector>
#include <map>

class MadSyntax;
class WXMHighlightingDialog;

namespace wxm
{
	struct HighlightingItemManager;
	struct HighlightingItem: private boost::noncopyable
	{
		HighlightingItem(WXMHighlightingDialog* dlg, long itemidx)
			: m_dlg(dlg), m_itemidx(itemidx)
		{}

		virtual ~HighlightingItem() {}

		wxColor InitDialogControls(const wxColor& bc0);

		virtual wxColor DoInitDialogControls() = 0;

		virtual void SetFGColor(const wxColor& color, const wxString& colorname) = 0;
		virtual void SetBGColor(const wxColor& color, const wxString& colorname) = 0;

		virtual wxColor GetFGColor() = 0;
		virtual wxColor GetBGColor() = 0;

		virtual void SetListItemFontStyle(long itemidx) = 0;
		virtual void EnableFontStyle(const MadFontStyles& style, bool enable) = 0;

		void RepaintListItem(long itemidx, const wxColor& fc0, const wxColor& bc0);

	protected:
		WXMHighlightingDialog* m_dlg;
		long m_itemidx;
	};

	struct HighlightingItemManager: private boost::noncopyable
	{
		void InitItems(MadSyntax* syn);

		wxColor GetTextFGColor() { return m_table[0].GetFGColor(); }
		wxColor GetTextBGColor() { return m_table[0].GetBGColor(); }

		long GetIndex() { return m_index; }
		void SetIndex(long idx) { m_index = idx; }
		void ResetIndex() { SetIndex(-1); }

		HighlightingItem& GetCurItem() { return m_table[m_index]; }

		void PaintListItems();

		void SetTextColor();
		void SetCurItemFGColor(const wxColor& color, const wxString& colorname);
		void SetCurItemBGColor(const wxColor& color, const wxString& colorname);

		HighlightingItemManager(WXMHighlightingDialog* dlg) : m_dlg(dlg), m_index(-1) {}

	private:
		WXMHighlightingDialog* m_dlg;
		long m_index;
		boost::ptr_vector<wxm::HighlightingItem> m_table;
	};
} // namespace wxm

class WXMHighlightingDialog: public wxDialogWrapper
{
	public:

		WXMHighlightingDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMHighlightingDialog();

		//(*Declarations(WXMHighlightingDialog)
		wxButton* WxButtonBC;
		wxButton* WxButtonCancel;
		wxButton* WxButtonClose;
		wxButton* WxButtonDelete;
		wxButton* WxButtonFC;
		wxButton* WxButtonLoad;
		wxButton* WxButtonSave;
		wxCheckBox* WxCheckBoxBold;
		wxCheckBox* WxCheckBoxItalic;
		wxCheckBox* WxCheckBoxUnderline;
		wxComboBox* WxComboBoxScheme;
		wxListBox* WxListBoxSyntax;
		wxListCtrl* WxListCtrlBC;
		wxListCtrl* WxListCtrlFC;
		wxListCtrl* WxListCtrlKeyword;
		wxPanel* WxPanelBC;
		wxPanel* WxPanelFC;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* WxStaticTextBCName;
		wxStaticText* WxStaticTextFCName;
		//*)

	protected:

		//(*Identifiers(WXMHighlightingDialog)
		static const long ID_WXLISTBOXSYNTAX;
		static const long ID_STATICTEXT1;
		static const long ID_WXCOMBOBOXSCHEME;
		static const long ID_STATICTEXT2;
		static const long ID_WXBUTTONLOAD;
		static const long ID_WXBUTTONSAVE;
		static const long ID_WXBUTTONDELETE;
		static const long ID_STATICLINE1;
		static const long ID_WXLISTCTRLKEYWORD;
		static const long ID_WXCHECKBOXBOLD;
		static const long ID_WXCHECKBOXITALIC;
		static const long ID_WXCHECKBOXUNDERLINE;
		static const long ID_STATICLINE3;
		static const long ID_STATICTEXT3;
		static const long ID_WXSTATICTEXTFCNAME;
		static const long ID_WXPANELFC;
		static const long ID_WXLISTCTRLFC;
		static const long ID_WXBUTTONFC;
		static const long ID_STATICLINE2;
		static const long ID_STATICTEXT4;
		static const long ID_WXSTATICTEXTBCNAME;
		static const long ID_WXPANELBC;
		static const long ID_WXLISTCTRLBC;
		static const long ID_WXBUTTONBC;
		//*)

	public:
		wxString m_InitSetting; // the init title of madsyntax
		MadSyntax *GetSyntax(const wxString &title);
		void SetToModifiedSyntax();
		void SetAttrFC(const wxColor& color, const wxString& colorname);
		void SetAttrBC(const wxColor& color, const wxString& colorname);
		void RepaintKeyword();
		void FreeSyntax(bool restore);
		void SetPanelFC(const wxColor& color);
		void SetPanelBC(const wxColor& color);

		void EnableFontConfig(const MadFontStyles& style);
		void DisableFontConfig();
		void EnableFGColorConfig(const wxColor& color);
		void DisableFGColorConfig();
		wxColor EnableBGColorConfig(const wxColor& bgcolor);
		wxColor DisableBGColorConfig();
	private:
		wxColour GetColourFromUser(const wxColour& colInit, const wxString& caption);
		void OnListCtrlKeywordResize();
		void OnListCtrlFCResize();
		void OnListCtrlBCResize();

		MadSyntax* m_Syntax;
		MadSyntax* m_cur_syn;
		std::vector<MadSyntax*> m_ModifiedSyntax;
		wxColourDialog* m_ColourDialog;

		wxm::HighlightingItemManager m_himgr;
	private:

		//(*Handlers(WXMHighlightingDialog)
		void WXMHighlightingDialogClose(wxCloseEvent& event);
		void WxListBoxSyntaxSelected(wxCommandEvent& event);
		void WxButtonLoadClick(wxCommandEvent& event);
		void WxButtonSaveClick(wxCommandEvent& event);
		void WxButtonDeleteClick(wxCommandEvent& event);
		void WxCheckBoxBoldClick(wxCommandEvent& event);
		void WxCheckBoxItalicClick(wxCommandEvent& event);
		void WxCheckBoxUnderlineClick(wxCommandEvent& event);
		void WxListCtrlKeywordSelected(wxListEvent& event);
		void WxListCtrlFCSelected(wxListEvent& event);
		void WxListCtrlBCSelected(wxListEvent& event);
		void WxButtonFCClick(wxCommandEvent& event);
		void WxButtonBCClick(wxCommandEvent& event);
		void WXMHighlightingDialogResize(wxSizeEvent& event);
		//*)

		void WXMHighlightingDialogActivate(wxActivateEvent& event);

		DECLARE_EVENT_TABLE()
};

extern WXMHighlightingDialog *g_HighlightingDialog;

#endif
