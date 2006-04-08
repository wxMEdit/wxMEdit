///////////////////////////////////////////////////////////////////////////////
// Name:        MadFindInFilesDialog.h
// Description: find or replace in files dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADFINDINFILESDIALOG_H_
#define _MADFINDINFILESDIALOG_H_

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/checkbox.h>
////Header Include End

#include <wx/docview.h>
#include <wx/fileconf.h>

//Compatibility for 2.4 code
#ifndef wxCLOSE_BOX
	#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
	#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadFindInFilesDialog_STYLE
#define MadFindInFilesDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class MadEdit;

class MadFindInFilesDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadFindInFilesDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Find/Replace in Files"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadFindInFilesDialog_STYLE);
		virtual ~MadFindInFilesDialog();
	
	public:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxListBox *WxListBoxFiles;
		wxStaticLine *WxStaticLine2;
		wxCheckBox *WxCheckBoxSubDir;
		wxComboBox *WxComboBoxEncoding;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxComboBox *WxComboBoxFilter;
		wxStaticText *WxStaticText1;
		wxButton *WxButtonDir;
		wxComboBox *WxComboBoxDir;
		wxRadioButton *WxRadioButtonDir;
		wxFlexGridSizer *WxFlexGridSizer1;
		wxRadioButton *WxRadioButtonOpenedFiles;
		wxBoxSizer *WxBoxSizer3;
		wxStaticLine *WxStaticLine1;
		wxButton *WxButtonClose;
		wxButton *WxButtonReplace;
		wxButton *WxButtonFind;
		wxBoxSizer *WxBoxSizer5;
		wxCheckBox *WxCheckBoxListFile;
		wxCheckBox *WxCheckBoxFindHex;
		wxCheckBox *WxCheckBoxRegex;
		wxCheckBox *WxCheckBoxWholeWord;
		wxCheckBox *WxCheckBoxCaseSensitive;
		wxBoxSizer *WxBoxSizer8;
		wxBoxSizer *WxBoxSizer7;
		wxBoxSizer *WxBoxSizer6;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		////GUI Control Declaration End
		
	public:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXLISTBOXFILES = 1028,
			ID_WXSTATICLINE2 = 1030,
			ID_WXCHECKBOXSUBDIR = 1038,
			ID_WXCOMBOBOXENCODING = 1042,
			ID_WXSTATICTEXT3 = 1041,
			ID_WXSTATICTEXT2 = 1040,
			ID_WXCOMBOBOXFILTER = 1037,
			ID_WXSTATICTEXT1 = 1036,
			ID_WXBUTTONDIR = 1035,
			ID_WXCOMBOBOXDIR = 1034,
			ID_WXRADIOBUTTONDIR = 1033,
			ID_WXRADIOBUTTONOPENEDFILES = 1031,
			ID_WXSTATICLINE1 = 1029,
			ID_WXBUTTONREPLACE = 1023,
			ID_WXBUTTONFIND = 1022,
			ID_WXCHECKBOXLISTFILE = 1043,
			ID_WXCHECKBOXFINDHEX = 1018,
			ID_WXCHECKBOXREGEX = 1017,
			ID_WXCHECKBOXWHOLEWORD = 1016,
			ID_WXCHECKBOXCASESENSITIVE = 1015,
			////GUI Enum Control ID End
			ID_MADEDIT1 = 1400,
			ID_MADEDIT2 = 1401,
			ID_WXBITMAPBUTTONRECENTFINDTEXT=1402,
			ID_WXBITMAPBUTTONRECENTREPLACETEXT=1403,
			ID_RECENTFINDTEXT1=1501,    // must be the same with MadSearchDialog
			ID_RECENTFINDTEXT20=1520,
			ID_RECENTREPLACETEXT1=ID_RECENTFINDTEXT20+1,
			ID_RECENTREPLACETEXT20=ID_RECENTREPLACETEXT1+19,

			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		}; //End of Enum
	
	public:
		MadEdit *m_FindText, *m_ReplaceText;
		wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
		wxFileHistory *m_RecentReplaceText;
	public:
		void MadFindInFilesDialogClose(wxCloseEvent& event);
		void CreateGUIControls(void);
		void WxCheckBoxFindHexClick(wxCommandEvent& event);
		void WxButtonFindClick(wxCommandEvent& event);
		void WxButtonReplaceClick(wxCommandEvent& event);
		void WxButtonDirClick(wxCommandEvent& event);
};

extern MadFindInFilesDialog *g_FindInFilesDialog;

#endif
