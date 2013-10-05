///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_replace_dialog.h
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_REPLACE_DIALOG_H
#define WXM_REPLACE_DIALOG_H

#include "xm/wx_recent_list.h"

//(*Headers(WXMReplaceDialog)
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


class MadEdit;

class WXMReplaceDialog: public wxDialog
{
	public:

		WXMReplaceDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMReplaceDialog();

		//(*Declarations(WXMReplaceDialog)
		wxButton* WxButtonReplace;
		wxButton* WxButtonFindNext;
		wxButton* WxButtonReplaceAll;
		wxCheckBox* WxCheckBoxMoveFocus;
		wxStaticText* WxStaticTextTo;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxSearchInSelection;
		wxTextCtrl* WxEditTo;
		wxButton* WxButtonClose;
		wxTextCtrl* WxEditFrom;
		wxMenu WxPopupMenuRecentReplaceText;
		wxCheckBox* WxCheckBoxFindHex;
		wxStaticText* WxStaticTextFrom;
		wxCheckBox* WxCheckBoxWholeWord;
		wxCheckBox* WxCheckBoxRegex;
		//*)

	protected:

		//(*Identifiers(WXMReplaceDialog)
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
		static const long ID_WXBUTTONREPLACE;
		static const long ID_WXBUTTONREPLACEALL;
		static const long ID_WXBUTTONCLOSE;
		//*)

		static const long ID_MADEDIT1;
		static const long ID_MADEDIT2;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
		static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;

	public:
		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		wxRecentList *m_RecentReplaceText;

		void ReadWriteSettings(bool bRead);
		void UpdateCheckBoxByCBHex(bool check);
		void UpdateSearchInSelection(bool check);

	//private:
	public:
		//(*Handlers(WXMReplaceDialog)
		void WXMReplaceDialogClose(wxCloseEvent& event);
		void WXMReplaceDialogKeyDown(wxKeyEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
		void WxButtonFindNextClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxButtonReplaceAllClick(wxCommandEvent& event);
		void WxButtonCloseClick(wxCommandEvent& event);
		//*)

		void WXMReplaceDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

extern WXMReplaceDialog *g_ReplaceDialog;

#endif
