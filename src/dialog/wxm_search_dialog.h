///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_search_dialog.h
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_SEARCH_DIALOG_H
#define WXM_SEARCH_DIALOG_H

#include "../xm/wx_recent_list.h"

//(*Headers(WXMSearchDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/fileconf.h>
#include <wx/bmpbuttn.h>

extern const long ID_RECENTFINDTEXT1;
extern const long ID_RECENTFINDTEXT20;
extern const long ID_RECENTREPLACETEXT1;
extern const long ID_RECENTREPLACETEXT20;

class MadEdit;

class WXMSearchDialog: public wxDialog
{
	public:

		WXMSearchDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMSearchDialog();

		//(*Declarations(WXMSearchDialog)
		wxButton* WxButtonReplace;
		wxButton* WxButtonFindPrev;
		wxButton* WxButtonFindNext;
		wxCheckBox* WxCheckBoxMoveFocus;
		wxStaticText* WxStaticTextTo;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxSearchInSelection;
		wxMenu WxPopupMenuRecentFindText;
		wxTextCtrl* WxEditTo;
		wxButton* WxButtonClose;
		wxTextCtrl* WxEditFrom;
		wxCheckBox* WxCheckBoxFindHex;
		wxStaticText* WxStaticTextFrom;
		wxCheckBox* WxCheckBoxWholeWord;
		wxButton* WxButtonCount;
		wxCheckBox* WxCheckBoxRegex;
		//*)

	protected:

		//(*Identifiers(WXMSearchDialog)
		static const long ID_WXCHECKBOXMOVEFOCUS;
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
		static const long ID_WXBUTTONCLOSE;
		//*)

		static const long ID_MADEDIT;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;

	public:
		MadEdit *m_FindText;
		wxBitmapButton *WxBitmapButtonRecentFindText;
		wxRecentList *m_RecentFindText;
		
		void ReadWriteSettings(bool bRead);
		void UpdateCheckBoxByCBHex(bool check);
		void UpdateSearchInSelection(bool check);

	//private:
	public:
		//(*Handlers(WXMSearchDialog)
		void WXMSearchDialogClose(wxCloseEvent& event);
		void WXMSearchDialogKeyDown(wxKeyEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
		void WxButtonFindNextClick(wxCommandEvent& event);
		void WxButtonFindPrevClick(wxCommandEvent& event);
		void WxButtonCountClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxButtonCloseClick(wxCommandEvent& event);
		//*)

		void WXMSearchDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

extern WXMSearchDialog *g_SearchDialog;

#endif
