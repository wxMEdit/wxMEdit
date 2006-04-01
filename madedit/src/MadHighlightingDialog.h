///////////////////////////////////////////////////////////////////////////////
// Name:        MadHighlightingDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADHIGHLIGHTINGDIALOG_H_
#define _MADHIGHLIGHTINGDIALOG_H_

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
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
////Header Include End

#include <vector>
using std::vector;

//Compatibility for 2.4 code
#ifndef wxCLOSE_BOX
	#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
	#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadHighlightingDialog_STYLE
#define MadHighlightingDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class MadSyntax;

class MadHighlightingDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadHighlightingDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Syntax Highlighting Schema"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadHighlightingDialog_STYLE);
		virtual ~MadHighlightingDialog();
	
	public:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButtonCancel;
		wxButton *WxButtonClose;
		wxBoxSizer *WxBoxSizer7;
		wxButton *WxButtonBC;
		wxListCtrl *WxListCtrlBC;
		wxPanel *WxPanelBC;
		wxStaticText *WxStaticTextBCName;
		wxStaticText *WxStaticText4;
		wxBoxSizer *WxBoxSizer12;
		wxStaticLine *WxStaticLine2;
		wxButton *WxButtonFC;
		wxListCtrl *WxListCtrlFC;
		wxPanel *WxPanelFC;
		wxStaticText *WxStaticTextFCName;
		wxStaticText *WxStaticText3;
		wxBoxSizer *WxBoxSizer11;
		wxBoxSizer *WxBoxSizer10;
		wxStaticLine *WxStaticLine3;
		wxCheckBox *WxCheckBoxUnderline;
		wxCheckBox *WxCheckBoxItalic;
		wxCheckBox *WxCheckBoxBold;
		wxBoxSizer *WxBoxSizer9;
		wxBoxSizer *WxBoxSizer8;
		wxListCtrl *WxListCtrlKeyword;
		wxBoxSizer *WxBoxSizer5;
		wxStaticLine *WxStaticLine1;
		wxButton *WxButtonDelete;
		wxButton *WxButtonSave;
		wxButton *WxButtonLoad;
		wxBoxSizer *WxBoxSizer6;
		wxStaticText *WxStaticText2;
		wxComboBox *WxComboBoxSchema;
		wxStaticText *WxStaticText1;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer3;
		wxBoxSizer *WxBoxSizer2;
		wxListBox *WxListBoxSyntax;
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
			ID_WXBUTTONBC = 1042,
			ID_WXLISTCTRLBC = 1041,
			ID_WXPANELBC = 1039,
			ID_WXSTATICTEXTBCNAME = 1038,
			ID_WXSTATICTEXT4 = 1037,
			ID_WXSTATICLINE2 = 1032,
			ID_WXBUTTONFC = 1036,
			ID_WXLISTCTRLFC = 1035,
			ID_WXPANELFC = 1034,
			ID_WXSTATICTEXTFCNAME = 1033,
			ID_WXSTATICTEXT3 = 1031,
			ID_WXSTATICLINE3 = 1044,
			ID_WXCHECKBOXUNDERLINE = 1030,
			ID_WXCHECKBOXITALIC = 1029,
			ID_WXCHECKBOXBOLD = 1028,
			ID_WXLISTCTRLKEYWORD = 1043,
			ID_WXSTATICLINE1 = 1006,
			ID_WXBUTTONDELETE = 1016,
			ID_WXBUTTONSAVE = 1014,
			ID_WXBUTTONLOAD = 1015,
			ID_WXSTATICTEXT2 = 1017,
			ID_WXCOMBOBOXSCHEMA = 1010,
			ID_WXSTATICTEXT1 = 1009,
			ID_WXLISTBOXSYNTAX = 1021,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		}; //End of Enum
	
	private:
		MadSyntax *m_Syntax;
		vector<MadSyntax*> m_ModifiedSyntax;
	public:
		wxString m_InitSetting; // the init title of madsyntax
		MadSyntax *GetSyntax(const wxString &title);
		void SetToModifiedSyntax(MadSyntax *syn);
		void SetAttrFC(const wxColor &color, const wxString &colorname);
		void SetAttrBC(const wxColor &color, const wxString &colorname);
		void RepaintKeyword();
		void FreeSyntax(bool restore);
		void SetPanelFC(const wxColor &color);
		void SetPanelBC(const wxColor &color);
	public:
		void MadHighlightingDialogClose(wxCloseEvent& event);
		void CreateGUIControls(void);
		void WxListBoxSyntaxSelected(wxCommandEvent& event);
		void WxListCtrlFCSelected(wxListEvent& event);
		void WxListCtrlBCSelected(wxListEvent& event);
		void WxListCtrlKeywordSelected(wxListEvent& event);
		void WxCheckBoxBoldClick(wxCommandEvent& event);
		void WxCheckBoxItalicClick(wxCommandEvent& event);
		void WxCheckBoxUnderlineClick(wxCommandEvent& event);
		void WxButtonFCClick(wxCommandEvent& event);
		void WxButtonBCClick(wxCommandEvent& event);
		void MadHighlightingDialogActivate(wxActivateEvent& event);
		void WxButtonLoadClick(wxCommandEvent& event);
		void WxButtonSaveClick(wxCommandEvent& event);
		void WxButtonDeleteClick(wxCommandEvent& event);
};

extern MadHighlightingDialog *g_HighlightingDialog;

#endif
