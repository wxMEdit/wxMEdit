///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADOPTIONSDIALOG_H_
#define _MADOPTIONSDIALOG_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/menu.h>
#include <wx/listbox.h>
#include <wx/treectrl.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
////Header Include End

#include <wx/dialog.h>

#include "MadCommand.h"
#include <list>
using std::list;


//Compatibility for 2.4.x code
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadOptionsDialog_STYLE
#define MadOptionsDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End


class TreeItemData: public wxTreeItemData
{
public:
    CommandData *cmddata;
    wxArrayString keys;
};

class KeyTextCtrl;


class MadOptionsDialog : public wxDialog
{
public:
    MadOptionsDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Options"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadOptionsDialog_STYLE);
    virtual ~MadOptionsDialog();

public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxMenu *WxPopupMenu1;
		wxButton *WxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer2;
		wxCheckBox *WxCheckBoxResetAllKeys;
		wxBoxSizer *WxBoxSizer26;
		wxButton *WxButtonShowInMenu;
		wxButton *WxButtonDeleteKey;
		wxButton *WxButtonAddKey;
		wxTextCtrl *WxEditKeyHint;
		KeyTextCtrl *WxEditKey;
		wxStaticText *WxStaticText15;
		wxBoxSizer *WxBoxSizer25;
		wxBoxSizer *WxBoxSizer23;
		wxListBox *WxListBoxKeys;
		wxStaticText *WxStaticText14;
		wxBoxSizer *WxBoxSizer22;
		wxBoxSizer *WxBoxSizer21;
		wxTextCtrl *WxEditCommandHint;
		wxStaticText *WxStaticTextCommandHint;
		wxBoxSizer *WxBoxSizer24;
		wxBoxSizer *WxBoxSizer20;
		wxTreeCtrl *WxTreeCtrl1;
		wxBoxSizer *WxBoxSizer19;
		wxBoxSizer *WxBoxSizer18;
		wxPanel *WxNoteBookPage4;
		wxButton *WxButton6;
		wxTextCtrl *WxEditFooterRight;
		wxStaticText *WxStaticText12;
		wxButton *WxButton5;
		wxTextCtrl *WxEditFooterCenter;
		wxStaticText *WxStaticText11;
		wxButton *WxButton4;
		wxTextCtrl *WxEditFooterLeft;
		wxStaticText *WxStaticText10;
		wxFlexGridSizer *WxFlexGridSizer2;
		wxCheckBox *WxCheckBoxPrintPageFooter;
		wxStaticBoxSizer *WxStaticBoxSizer4;
		wxButton *WxButton3;
		wxTextCtrl *WxEditHeaderRight;
		wxStaticText *WxStaticText9;
		wxButton *WxButton2;
		wxTextCtrl *WxEditHeaderCenter;
		wxStaticText *WxStaticText8;
		wxButton *WxButton1;
		wxTextCtrl *WxEditHeaderLeft;
		wxStaticText *WxStaticText7;
		wxFlexGridSizer *WxFlexGridSizer1;
		wxCheckBox *WxCheckBoxPrintPageHeader;
		wxStaticBoxSizer *WxStaticBoxSizer3;
		wxBoxSizer *WxBoxSizer16;
		wxRadioBox *WxRadioBoxPrintOffset;
		wxStaticBoxSizer *WxStaticBoxSizer2;
		wxCheckBox *WxCheckBoxPrintSpaceChar;
		wxCheckBox *WxCheckBoxPrintTabChar;
		wxCheckBox *WxCheckBoxPrintEndOfLine;
		wxCheckBox *WxCheckBoxPrintLineNumber;
		wxCheckBox *WxCheckBoxPrintSyntax;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		wxBoxSizer *WxBoxSizer15;
		wxBoxSizer *WxBoxSizer14;
		wxPanel *WxNoteBookPage3;
		wxCheckBox *WxCheckBoxMiddleMouseToPaste;
		wxCheckBox *WxCheckBoxMouseSelectToCopy;
		wxCheckBox *WxCheckBoxAutoCompletePair;
		wxCheckBox *WxCheckBoxAutoIndent;
		wxCheckBox *WxCheckBoxTabOrSpaces;
		wxBoxSizer *WxBoxSizer12;
		wxStaticText *WxStaticText6;
		wxTextCtrl *WxEditIndentColumns;
		wxBoxSizer *WxBoxSizer13;
		wxStaticText *WxStaticText5;
		wxTextCtrl *WxEditTabColumns;
		wxBoxSizer *WxBoxSizer11;
		wxStaticText *WxStaticText4;
		wxTextCtrl *WxEditMaxColumns;
		wxBoxSizer *WxBoxSizer10;
		wxStaticText *WxStaticText3;
		wxTextCtrl *WxEditMaxLineLength;
		wxBoxSizer *WxBoxSizer9;
		wxBoxSizer *WxBoxSizer8;
		wxPanel *WxNoteBookPage2;
		wxCheckBox *WxCheckBoxDoNotSaveSettings;
		wxBoxSizer *WxBoxSizer7;
		wxStaticText *WxStaticText13;
		wxComboBox *WxComboBoxEncoding;
		wxBoxSizer *WxBoxSizer17;
		wxStaticText *WxStaticText2;
		wxTextCtrl *WxEditMaxTextFileSize;
		wxBoxSizer *WxBoxSizer6;
		wxStaticText *WxStaticText1;
		wxTextCtrl *WxEditMaxSizeToLoad;
		wxBoxSizer *WxBoxSizer5;
		wxCheckBox *WxCheckBoxRecordCaretMovements;
		wxCheckBox *WxCheckBoxSingleInstance;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer3;
		wxPanel *WxNoteBookPage1;
		wxNotebook *WxNotebook1;
		wxBoxSizer *WxBoxSizer1;
  ////GUI Control Declaration End

private:
    DECLARE_EVENT_TABLE()

public:
    //Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
    //#defines may replace a numeric value for the enums names.
    //Try copy pasting the below block in your old Form header Files.
	enum {
////GUI Enum Control ID Start
			ID_MNU_MENUITEM1_1110 = 1118 ,
			ID_MNU___P__PATHNAME_1111 = 1119 ,
			ID_MNU_MENUITEM3_1112 = 1112 ,
			ID_MNU___N_PAGE_NUMBER_1113 = 1113 ,
			ID_MNU___S__TOTALPAGES_1114 = 1114 ,
			ID_MNU_MENUITEM6_1115 = 1115 ,
			ID_MNU___D__DATE_1116 = 1116 ,
			ID_MNU___T__TIME_1117 = 1117 ,
			
			ID_WXCHECKBOXRESETALLKEYS = 1153,
			ID_WXBUTTONSHOWINMENU = 1144,
			ID_WXBUTTONDELETEKEY = 1143,
			ID_WXBUTTONADDKEY = 1138,
			ID_WXEDITKEYHINT = 1142,
			ID_WXEDITKEY = 1137,
			ID_WXSTATICTEXT15 = 1136,
			ID_WXLISTBOXKEYS = 1133,
			ID_WXSTATICTEXT14 = 1132,
			ID_WXEDITHINT = 1141,
			ID_WXSTATICTEXTCOMMANDHINT = 1140,
			ID_WXTREECTRL1 = 1125,
			ID_WXNOTEBOOKPAGE4 = 1122,
			ID_WXBUTTON6 = 1111,
			ID_WXBUTTON5 = 1110,
			ID_WXBUTTON4 = 1109,
			ID_WXCHECKBOXPRINTPAGEFOOTER = 1108,
			ID_WXBUTTON3 = 1107,
			ID_WXEDITHEADERRIGHT = 1106,
			ID_WXSTATICTEXT9 = 1105,
			ID_WXBUTTON2 = 1104,
			ID_WXEDITHEADERCENTER = 1103,
			ID_WXSTATICTEXT8 = 1102,
			ID_WXBUTTON1 = 1101,
			ID_WXEDITHEADERLEFT = 1100,
			ID_WXSTATICTEXT7 = 1099,
			ID_WXCHECKBOXPRINTPAGEHEADER = 1093,
			ID_WXRADIOBOXPRINTOFFSET = 1091,
			ID_WXCHECKBOXPRINTSPACECHAR = 1090,
			ID_WXCHECKBOXPRINTTABCHAR = 1089,
			ID_WXCHECKBOXPRINTENDOFLINE = 1088,
			ID_WXCHECKBOXPRINTLINENUMBER = 1087,
			ID_WXCHECKBOXPRINTSYNTAX = 1084,
			ID_WXNOTEBOOKPAGE3 = 1062,
			ID_WXCHECKBOXMIDDLEMOUSETOPASTE = 1127,
			ID_WXCHECKBOXMOUSESELECTTOCOPY = 1126,
			ID_WXCHECKBOXAUTOCOMPLETEPAIR = 1154,
			ID_WXCHECKBOXAUTOINDENT = 1060,
			ID_WXCHECKBOXTABORSPACES = 1059,
			ID_WXSTATICTEXT5 = 1057,
			ID_WXEDITTABCOLUMNS = 1056,
			ID_WXSTATICTEXT4 = 1054,
			ID_WXEDITMAXCOLUMNS = 1053,
			ID_WXSTATICTEXT3 = 1051,
			ID_WXEDITMAXLINELENGTH = 1049,
			ID_WXNOTEBOOKPAGE2 = 1014,
			ID_WXCHECKBOXDONOTSAVESETTINGS = 1045,
			ID_WXSTATICTEXT13 = 1121,
			ID_WXCOMBOBOXENCODING = 1120,
			ID_WXSTATICTEXT2 = 1043,
			ID_WXEDITMAXTEXTFILESIZE = 1042,
			ID_WXSTATICTEXT1 = 1040,
			ID_WXEDITMAXSIZETOLOAD = 1039,
			ID_WXCHECKBOXRECORDCARETMOVEMENTS = 1037,
			ID_WXCHECKBOXSINGLEINSTANCE = 1034,
			ID_WXNOTEBOOKPAGE1 = 1003,
			ID_WXNOTEBOOK1 = 1002,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    bool InitOptions;
    int LocID;
    list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
    list<TreeItemData*> ChangedTreeItemDataList;
    //wxArrayString ChangedKeys;
    
    TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
    bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
    //void AddKeyToChangedKeys(const wxString &key);
    void UpdateKeyHint();

#ifdef __WXMSW__
    wxCheckBox *WxCheckBoxRightClickMenu;
#endif

public:
    void MadOptionsDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);
	void MadOptionsDialogActivate(wxActivateEvent& event);
	void WxButtonOKClick(wxCommandEvent& event);
	void WxButtonCancelClick(wxCommandEvent& event);
	void PrintMarkClick(wxCommandEvent& event);
	void PrintMarkButtonClick(wxCommandEvent& event);
	
	void WxTreeCtrl1SelChanged(wxTreeEvent& event);
	void WxButtonAddKeyClick(wxCommandEvent& event);
	void WxListBoxKeysSelected(wxCommandEvent& event);
	void WxButtonDeleteKeyClick(wxCommandEvent& event);
	void WxButtonShowInMenuClick(wxCommandEvent& event);
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
 
 
 
 
