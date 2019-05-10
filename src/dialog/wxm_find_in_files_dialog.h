///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_find_in_files_dialog.h
// Description: find or replace in files dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2006-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_FIND_IN_FILES_DIALOG_H
#define WXM_FIND_IN_FILES_DIALOG_H

#include "../wxm/recent_list.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMFindInFilesDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/fileconf.h>
#include <wx/bmpbuttn.h>


class MadEdit;

class WXMFindInFilesDialog: public wxDialog
{
	public:

		WXMFindInFilesDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMFindInFilesDialog();

		//(*Declarations(WXMFindInFilesDialog)
		wxButton* WxButtonActiveDir;
		wxButton* WxButtonClose;
		wxButton* WxButtonDir;
		wxButton* WxButtonFind;
		wxButton* WxButtonReplace;
		wxCheckBox* WxCheckBoxCaseSensitive;
		wxCheckBox* WxCheckBoxEnableReplace;
		wxCheckBox* WxCheckBoxFindHex;
		wxCheckBox* WxCheckBoxListFirstOnly;
		wxCheckBox* WxCheckBoxRegex;
		wxCheckBox* WxCheckBoxSubDir;
		wxCheckBox* WxCheckBoxWholeWord;
		wxComboBox* WxComboBoxDir;
		wxComboBox* WxComboBoxEncoding;
		wxComboBox* WxComboBoxExclude;
		wxComboBox* WxComboBoxFilter;
		wxRadioButton* WxRadioButtonDir;
		wxRadioButton* WxRadioButtonOpenedFiles;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		//*)

	protected:

		//(*Identifiers(WXMFindInFilesDialog)
		static const long ID_WXCHECKBOXCASESENSITIVE;
		static const long ID_WXCHECKBOXWHOLEWORD;
		static const long ID_WXCHECKBOXREGEX;
		static const long ID_WXCHECKBOXFINDHEX;
		static const long ID_WXCHECKBOXLISTFIRSTONLY;
		static const long ID_WXBUTTONFIND;
		static const long ID_WXBUTTONREPLACE;
		static const long ID_WXCHECKBOXENABLEREPLACE;
		static const long ID_STATICLINE1;
		static const long ID_WXRADIOBUTTONOPENEDFILES;
		static const long ID_WXRADIOBUTTONDIR;
		static const long ID_COMBOBOX1;
		static const long ID_WXBUTTONDIR;
		static const long ID_WXBUTTONACTIVEDIR;
		static const long ID_STATICTEXT1;
		static const long ID_WXCOMBOBOXFILTER;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_WXCOMBOBOXEXCLUDE;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_WXCOMBOBOXENCODING;
		static const long ID_WXCHECKBOXSUBDIR;
		//*)

		static const long ID_MADEDIT1;
		static const long ID_MADEDIT2;
		static const long ID_WXBITMAPBUTTONRECENTFINDTEXT;
		static const long ID_WXBITMAPBUTTONRECENTREPLACETEXT;

	public:
		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		wxm::wxRecentList *m_RecentFindDir;
		wxm::wxRecentList *m_RecentFindFilter, *m_RecentFindExclude;
		void UpdateCheckBoxByCBHex(bool check);
		void FindReplaceInFiles(bool bReplace);//false: find; true: replace

	private:
		wxString m_systemenc;

	public:
		//(*Handlers(WXMFindInFilesDialog)
		void WXMFindInFilesDialogClose(wxCloseEvent& event);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxButtonFindClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxCheckBoxEnableReplaceClick(wxCommandEvent& event);
		void WxButtonDirClick(wxCommandEvent& event);
		void WxButtonActiveDirClick(wxCommandEvent& event);
		//*)

		void WXMFindInFilesDialogActivate(wxActivateEvent& event);

		void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
		void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
		void OnRecentFindText(wxCommandEvent& event);
		void OnRecentReplaceText(wxCommandEvent& event);

		void PurgeRecentFindDirs();
		void PurgeRecentFindFilters();
		void PurgeRecentFindExcludes();

		DECLARE_EVENT_TABLE()
};

extern WXMFindInFilesDialog *g_FindInFilesDialog;
WXMFindInFilesDialog& GetFindInFilesDialog(wxWindow* parent);

#endif
