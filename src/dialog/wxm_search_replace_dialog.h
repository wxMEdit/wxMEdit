///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_search_replace_dialog.h
// Description: Search/Replace Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_REPLACE_DIALOG_H
#define WXM_REPLACE_DIALOG_H

#include "../xm/cxx11.h"
#include "../wxm/recent_list.h"
#include "../wxm/utils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMSearchReplaceDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/fileconf.h>
#include <wx/bmpbuttn.h>

#include <map>

class MadEdit;

class WXMSearchReplaceDialog: public wxDialog
{
	public:

		WXMSearchReplaceDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMSearchReplaceDialog();

		//(*Declarations(WXMSearchReplaceDialog)
		wxButton* WxButtonClose;
		wxButton* WxButtonCount;
		wxButton* WxButtonFindNext;
		wxButton* WxButtonFindPrev;
		wxButton* WxButtonReplace;
		wxButton* WxButtonReplaceAll;
		wxButton* WxButtonReplaceExpand;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxFindHex;
		wxCheckBox* WxCheckBoxMoveFocus;
		wxCheckBox* WxCheckBoxRegex;
		wxCheckBox* WxCheckBoxSearchInSelection;
		wxCheckBox* WxCheckBoxWholeWord;
		wxCheckBox* WxCheckBoxWrapAround;
		wxMenu WxPopupMenuRecentFindText;
		wxMenu WxPopupMenuRecentReplaceText;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticTextStatus;
		wxStaticText* WxStaticTextFrom;
		wxStaticText* WxStaticTextTo;
		wxTextCtrl* WxEditFrom;
		wxTextCtrl* WxEditTo;
		//*)

	protected:

		//(*Identifiers(WXMSearchReplaceDialog)
		static const long ID_WXCHECKBOXMOVEFOCUS;
		static const long ID_WXCHECKBOXWRAPAROUND;
		static const long ID_WXCHECKBOXCASESENSITIVE;
		static const long ID_WXCHECKBOXWHOLEWORD;
		static const long ID_WXCHECKBOXREGEX;
		static const long ID_WXCHECKBOXFINDHEX;
		static const long ID_WXCHECKBOXSEARCHINSELECTION;
		static const long ID_WXSTATICTEXTFROM;
		static const long ID_WXEDITFROM;
		static const long ID_WXSTATICTEXTTO;
		static const long ID_WXEDITTO;
		static const long ID_WXBUTTONFINDNEXT;
		static const long ID_WXBUTTONFINDPREV;
		static const long ID_WXBUTTONCOUNT;
		static const long ID_WXBUTTONREPLACE;
		static const long ID_WXBUTTONREPLACEALL;
		static const long ID_WXBUTTONREPLACEEXPAND;
		static const long ID_WXBUTTONCLOSE;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXTSTATUS;
		//*)

		static const long ID_MADEDIT1;
		static const long ID_MADEDIT2;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
		static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;

	public:
		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		wxm::wxRecentList *m_RecentFindText;
		wxm::wxRecentList *m_RecentReplaceText;

		void ReadWriteSettings(bool bRead);
		void UpdateCheckBoxByCBHex(bool check);
		void UpdateCheckBoxByCBHex() { UpdateCheckBoxByCBHex(WxCheckBoxFindHex->GetValue()); }
		void UpdateSearchInSelection(bool check);

		void ShowOnlyFindFunc();
		void ShowWithReplaceFunc();

		virtual bool Show(bool show) override
		{
			if (!show)
				ResetMessage();

			return wxDialog::Show(show);
		}
	private:
		enum SearchMsgType
		{
			SMT_INFORMATION,
			SMT_NOTICE,
			SMT_WARNING,
		};
		std::map<int, wxColor> m_msgtypecolor_map;
		wxColor GetMessageColor(SearchMsgType type);
		void ResetMessage();
		void ShowMessage(const wxString& msg, SearchMsgType type=SMT_INFORMATION);
	//private:
	public:
		//(*Handlers(WXMSearchReplaceDialog)
		void WXMSearchReplaceDialogClose(wxCloseEvent& event);
		void WXMSearchReplaceDialogKeyDown(wxKeyEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
		void WxButtonFindNextClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxButtonReplaceAllClick(wxCommandEvent& event);
		void WxButtonFindPrevClick(wxCommandEvent& event);
		void WxButtonCountClick(wxCommandEvent& event);
		void WxButtonCloseClick(wxCommandEvent& event);
		void WxButtonReplaceExpandClick(wxCommandEvent& event);
		//*)

		void WXMSearchReplaceDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindTexts();
		void PurgeRecentReplaceTexts();

		DECLARE_EVENT_TABLE()
};

extern WXMSearchReplaceDialog *g_SearchReplaceDialog;
WXMSearchReplaceDialog& GetSearchReplaceDialog(wxWindow* paren);

#endif
