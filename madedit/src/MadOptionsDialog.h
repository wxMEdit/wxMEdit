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
#include <wx/statbox.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
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
class wxFlatNotebook;

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
		wxMenu *WxPopupMenuPrintMark;
		wxMenu *WxPopupMenuDateTimeMark;
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
		wxStaticText *WxStaticText17;
		wxRadioButton *WxRadioButtonDisable;
		wxRadioButton *WxRadioButtonEnable;
		wxCheckBox *WxCheckBoxMouseSelectToCopy;
		wxBoxSizer *WxBoxSizer28;
		wxCheckBox *WxCheckBoxAutoCompletePair;
		wxCheckBox *WxCheckBoxAutoIndent;
		wxCheckBox *WxCheckBoxTabOrSpaces;
		wxBoxSizer *WxBoxSizer12;
		wxCheckBox *WxCheckBoxDateTimeInEnglish;
		wxButton *WxButtonDateTime;
		wxStaticText *WxStaticTextDateTime;
		wxTextCtrl *WxEditDateTime;
		wxBoxSizer *WxBoxSizer23;
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
		wxCheckBox *WxCheckBoxRestoreCaretPos;
		wxCheckBox *WxCheckBoxReloadFiles;
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
		wxStaticText *WxStaticText16;
		wxComboBox *WxComboBoxLanguage;
		wxBoxSizer *WxBoxSizer27;
		wxBoxSizer *WxBoxSizer3;
		wxPanel *WxNoteBookPage1;
		wxFlatNotebook *WxNotebook1;
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
			ID_MNU_MENUITEM1_1110 = 1118,
			ID_MNU___P__PATHNAME_1111 = 1119,
			ID_MNU_MENUITEM3_1112 = 1112,
			ID_MNU___N_PAGE_NUMBER_1113 = 1113,
			ID_MNU___S__TOTALPAGES_1114 = 1114,
			ID_MNU_MENUITEM6_1115 = 1115,
			ID_MNU___D__DATE_1116 = 1116,
			ID_MNU___T__TIME_1117 = 1117,
			
			ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 = 1191,
			ID_MNU___D__M__Y_24_02_2007_1192 = 1192,
			ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 = 1195,
			ID_MNU_MENUITEM1_1170 = 1170,
			ID_MNU___A_FULLWEEKDAYNAME_1171 = 1171,
			ID_MNU___B_ABBREVIATEDMONTHNAME_1172 = 1172,
			ID_MNU___B_FULLMONTHNAME_1173 = 1173,
			ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 = 1174,
			ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 = 1175,
			ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 = 1176,
			ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 = 1177,
			ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 = 1178,
			ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 = 1179,
			ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 = 1180,
			ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 = 1181,
			ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 = 1182,
			ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 = 1183,
			ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 = 1184,
			ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 = 1185,
			ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 = 1186,
			ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 = 1187,
			ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 = 1188,
			ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 = 1189,
			ID_MNU___Z_TIME_ZONENAME_1193 = 1193,
			ID_MNU___Z_TIME_ZONEABBREVIATION_1194 = 1194,
			
			ID_WXBUTTONCANCEL = 1007,
			ID_WXBUTTONOK = 1006,
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
			ID_WXSTATICTEXT17 = 1162,
			ID_WXRADIOBUTTONDISABLE = 1161,
			ID_WXRADIOBUTTONENABLE = 1160,
			ID_WXCHECKBOXMOUSESELECTTOCOPY = 1159,
			ID_WXCHECKBOXAUTOCOMPLETEPAIR = 1154,
			ID_WXCHECKBOXAUTOINDENT = 1060,
			ID_WXCHECKBOXTABORSPACES = 1059,
			ID_WXCHECKBOXDATETIMEINENGLISH = 1169,
			ID_WXBUTTONDATETIME = 1167,
			ID_WXSTATICTEXTDATETIME = 1168,
			ID_WXEDITDATETIME = 1166,
			ID_WXSTATICTEXT5 = 1057,
			ID_WXEDITTABCOLUMNS = 1056,
			ID_WXSTATICTEXT4 = 1054,
			ID_WXEDITMAXCOLUMNS = 1053,
			ID_WXSTATICTEXT3 = 1051,
			ID_WXEDITMAXLINELENGTH = 1049,
			ID_WXNOTEBOOKPAGE2 = 1014,
			ID_WXCHECKBOXDONOTSAVESETTINGS = 1045,
			ID_WXCHECKBOXRESTORECARETPOS = 1198,
			ID_WXCHECKBOXRELOADFILES = 1197,
			ID_WXSTATICTEXT13 = 1121,
			ID_WXCOMBOBOXENCODING = 1120,
			ID_WXSTATICTEXT2 = 1043,
			ID_WXEDITMAXTEXTFILESIZE = 1042,
			ID_WXSTATICTEXT1 = 1040,
			ID_WXEDITMAXSIZETOLOAD = 1039,
			ID_WXCHECKBOXRECORDCARETMOVEMENTS = 1037,
			ID_WXCHECKBOXSINGLEINSTANCE = 1034,
			ID_WXSTATICTEXT16 = 1157,
			ID_WXCOMBOBOXLANGUAGE = 1156,
			ID_WXNOTEBOOKPAGE1 = 1003,
			ID_WXNOTEBOOK1 = 1002,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    int ButtonID;
    list<TreeItemData*> TreeItemDataList; // list of all TreeItemData
    list<TreeItemData*> ChangedTreeItemDataList;
    
    TreeItemData* FindKeyInList(const wxString &key); // find key in TreeItemDataList
    bool FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist);
    void UpdateKeyHint();

#ifdef __WXMSW__
    wxCheckBox *WxCheckBoxRightClickMenu;
#endif

public:
    void MadOptionsDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);
	void MadOptionsDialogActivate(wxActivateEvent& event);
	void LoadOptions(void);
	void WxButtonOKClick(wxCommandEvent& event);
	void WxButtonCancelClick(wxCommandEvent& event);
	void PrintMarkClick(wxCommandEvent& event);
	void PrintMarkButtonClick(wxCommandEvent& event);
	void DateTimeMarkClick(wxCommandEvent& event);
	
	void WxTreeCtrl1SelChanged(wxTreeEvent& event);
	void WxButtonAddKeyClick(wxCommandEvent& event);
	void WxListBoxKeysSelected(wxCommandEvent& event);
	void WxButtonDeleteKeyClick(wxCommandEvent& event);
	void WxButtonDateTimeClick(wxCommandEvent& event);
	void WxButtonShowInMenuClick(wxCommandEvent& event);
};


extern MadOptionsDialog *g_OptionsDialog;

#endif
 
 
 
 
