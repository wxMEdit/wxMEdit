///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxmedit_options_dialog.cpp
// Description: Options Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_options_dialog.h"

#include "../xm/cxx11.h"
#include "../mad_utils.h"
#include "../wxmedit/wxmedit_command.h"
#include "../wxmedit/wxmedit.h"
#include "../wxmedit_frame.h"
#include "../xm/encoding/encoding.h"
#include "../wxm/update.h"
#include "../wxm/def.h"
#include "../wxm/utils.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*InternalHeaders(WXMEditOptionsDialog)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <wx/aui/auibook.h>
#include <wx/fileconf.h>
#include <wx/config.h>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include <algorithm>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

const wchar_t * LocalizeText(const wchar_t* txt);

WXMEditOptionsDialog *g_OptionsDialog=nullptr;

extern const wxChar *g_LanguageString[];
extern const size_t g_LanguageCount;

TreeItemData *g_SelectedCommandItem=nullptr;
int g_SelectedKeyId=-1;
TreeItemData *g_CommandItemOfNewKey=nullptr;


class KeyTextCtrl : public wxTextCtrl
{
	DECLARE_EVENT_TABLE()
public:
	KeyTextCtrl()
	{
	}
	KeyTextCtrl(wxWindow *parent, wxWindowID id,
			   const wxString& value = wxEmptyString,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize,
			   long style = 0,
			   const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTextCtrlNameStr)
			   :wxTextCtrl(parent, id, value, pos, size, style, validator, name)
	{
	}

	void OnKeyDown(wxKeyEvent& evt)
	{
		int flags=wxACCEL_NORMAL;
		int key=evt.GetKeyCode();

		if(evt.ControlDown()) flags|=wxACCEL_CTRL;
		if(evt.AltDown())     flags|=wxACCEL_ALT;
		if(evt.ShiftDown())   flags|=wxACCEL_SHIFT;

		MadEditShortCut sc=ShortCut(flags, key);
		static MadEditShortCut prevsc=0;

		if(sc==prevsc) return;
		prevsc=sc;

		wxString scstr=ShortCutToString(sc);
		SetValue(scstr);

		g_OptionsDialog->UpdateKeyHint();
		//evt.Skip();
	}
	void OnSetFocus(wxFocusEvent &evt)
	{
		g_OptionsDialog->DisableTabTraversal();
		evt.Skip();
	}
	void OnKillFocus(wxFocusEvent &evt)
	{
		g_OptionsDialog->EnableTabTraversal();
		evt.Skip();
	}
};

BEGIN_EVENT_TABLE(KeyTextCtrl,wxTextCtrl)
	EVT_KEY_DOWN(KeyTextCtrl::OnKeyDown)
	EVT_SET_FOCUS(KeyTextCtrl::OnSetFocus)
	EVT_KILL_FOCUS (KeyTextCtrl::OnKillFocus)
END_EVENT_TABLE()

// filter '&' and '.'
wxString FilterChar(const wxChar *ws)
{
	wxString str;
	wxChar wc;

	while((wc=*ws) != wxT('\0'))
	{
		if(wc!=wxT('&') && wc!=wxT('.'))
		{
			str << wc;
		}
		++ws;
	}

	return str;
}


//(*IdInit(WXMEditOptionsDialog)
const long WXMEditOptionsDialog::ID_STATICTEXT17 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCOMBOBOXUPDATESCHECKINGPERIOD = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONCHECKNOW = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXCHECKPRERELEASE = wxNewId();
const long WXMEditOptionsDialog::ID_WXCOMBOBOXLANGUAGE = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT16 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCOMBOBOXFORCEENCODING = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXFORCEENCODING = wxNewId();
const long WXMEditOptionsDialog::ID_WXCOMBOBOXENCODING = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT13 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXSINGLEINSTANCE = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXRELOADFILES = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXRECORDCARETMOVEMENTS = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXRESTORECARETPOS = wxNewId();
const long WXMEditOptionsDialog::ID_PANEL1 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITMAXLINELENGTH = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT3 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITMAXCOLUMNS = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT4 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITTABCOLUMNS = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT5 = wxNewId();
const long WXMEditOptionsDialog::IID_WXEDITINDENTCOLUMNS = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT6 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXTABORSPACES = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXAUTOINDENT = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXAUTOCOMPLETEPAIR = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXMOUSESELECTTOCOPY = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXWHENPRESSCTRLKEY = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXMIDDLEMOUSETOPASTE = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT18 = wxNewId();
const long WXMEditOptionsDialog::ID_COMBOBOXBEHAVIORCOPYINHEXAREA = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT19 = wxNewId();
const long WXMEditOptionsDialog::ID_COMBOBOXPASTEASHEXSTRING = wxNewId();
const long WXMEditOptionsDialog::ID_PANEL2 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTSYNTAX = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTLINENUMBER = wxNewId();
const long WXMEditOptionsDialog::ID_CHECKBOXPRINTBOOKMARK = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTENDOFLINE = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTTABCHAR = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTSPACECHAR = wxNewId();
const long WXMEditOptionsDialog::ID_WXRADIOBOXPRINTOFFSET = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTPAGEHEADER = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT7 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITHEADERLEFT = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON1 = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT8 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITHEADERCENTER = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON2 = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT9 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITHEADERRIGHT = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON3 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXPRINTPAGEFOOTER = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT10 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITFOOTERLEFT = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON4 = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT11 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITFOOTERLEFTCENTER = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON5 = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT12 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITFOOTERRIGHT = wxNewId();
const long WXMEditOptionsDialog::ID_BUTTON6 = wxNewId();
const long WXMEditOptionsDialog::ID_PANEL3 = wxNewId();
const long WXMEditOptionsDialog::ID_TREECTRL1 = wxNewId();
const long WXMEditOptionsDialog::ID_WXSTATICTEXTCOMMANDHINT = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITHINT = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT14 = wxNewId();
const long WXMEditOptionsDialog::ID_WXLISTBOXKEYS = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT15 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITKEY = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITKEYHINT = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONADDKEY = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONDELETEKEY = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONSHOWINMENU = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXRESETALLKEYS = wxNewId();
const long WXMEditOptionsDialog::ID_PANEL4 = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXDONOTSAVESETTINGS = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITMAXTEXTFILESIZE = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT2 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITMAXSIZETOLOAD = wxNewId();
const long WXMEditOptionsDialog::ID_STATICTEXT1 = wxNewId();
const long WXMEditOptionsDialog::ID_WXEDITDATETIME = wxNewId();
const long WXMEditOptionsDialog::ID_WXSTATICTEXTDATETIME = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONDATETIME = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXDATETIMEINENGLISH = wxNewId();
const long WXMEditOptionsDialog::ID_WXCHECKBOXDOTMATCHNEWLINE = wxNewId();
const long WXMEditOptionsDialog::ID_PANEL5 = wxNewId();
const long WXMEditOptionsDialog::ID_AUINOTEBOOK1 = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONOK = wxNewId();
const long WXMEditOptionsDialog::ID_WXBUTTONCANCEL = wxNewId();
//*)

const long WXMEditOptionsDialog:: ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___D__M__Y_24_02_2007_1192 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU_MENUITEM1_1170 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___A_FULLWEEKDAYNAME_1171 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___B_ABBREVIATEDMONTHNAME_1172 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___B_FULLMONTHNAME_1173 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___Z_TIME_ZONENAME_1193 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___Z_TIME_ZONEABBREVIATION_1194 = wxNewId();

const long WXMEditOptionsDialog:: ID_MNU_MENUITEM1_1110 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___P__PATHNAME_1111 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU_MENUITEM3_1112 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___N_PAGE_NUMBER_1113 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___S__TOTALPAGES_1114 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU_MENUITEM6_1115 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___D__DATE_1116 = wxNewId();
const long WXMEditOptionsDialog:: ID_MNU___T__TIME_1117 = wxNewId();

BEGIN_EVENT_TABLE(WXMEditOptionsDialog,wxDialog)
	//(*EventTable(WXMEditOptionsDialog)
	//*)
	EVT_UPDATE_UI(WXMEditOptionsDialog::ID_WXBUTTONCHECKNOW, WXMEditOptionsDialog::OnUpdateUI_DialogOptions_CheckNow)

	EVT_ACTIVATE(WXMEditOptionsDialog::WXMOptionsDialogActivate)

	EVT_BUTTON(ID_BUTTON1,WXMEditOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_BUTTON2,WXMEditOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_BUTTON3,WXMEditOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_BUTTON4,WXMEditOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_BUTTON5,WXMEditOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_BUTTON6,WXMEditOptionsDialog::PrintMarkButtonClick)

	EVT_MENU(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D__M__Y_24_02_2007_1192 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU_MENUITEM1_1170 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A_FULLWEEKDAYNAME_1171 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_ABBREVIATEDMONTHNAME_1172 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_FULLMONTHNAME_1173 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONENAME_1193 , WXMEditOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONEABBREVIATION_1194 , WXMEditOptionsDialog::DateTimeMarkClick)

	EVT_MENU(ID_MNU_MENUITEM1_1110 , WXMEditOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___P__PATHNAME_1111 , WXMEditOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___N_PAGE_NUMBER_1113 , WXMEditOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___S__TOTALPAGES_1114 , WXMEditOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___D__DATE_1116 , WXMEditOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___T__TIME_1117 , WXMEditOptionsDialog::PrintMarkClick)

END_EVENT_TABLE()

WXMEditOptionsDialog::WXMEditOptionsDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(WXMEditOptionsDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer12;
	wxBoxSizer* BoxSizer13;
	wxBoxSizer* BoxSizer14;
	wxBoxSizer* BoxSizer15;
	wxBoxSizer* BoxSizer16;
	wxBoxSizer* BoxSizer17;
	wxBoxSizer* BoxSizer18;
	wxBoxSizer* BoxSizer19;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer20;
	wxBoxSizer* BoxSizer21;
	wxBoxSizer* BoxSizer22;
	wxBoxSizer* BoxSizer23;
	wxBoxSizer* BoxSizer24;
	wxBoxSizer* BoxSizer25;
	wxBoxSizer* BoxSizer26;
	wxBoxSizer* BoxSizer27;
	wxBoxSizer* BoxSizer28;
	wxBoxSizer* BoxSizer29;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer30;
	wxBoxSizer* BoxSizer31;
	wxBoxSizer* BoxSizer32;
	wxBoxSizer* BoxSizer33;
	wxBoxSizer* BoxSizer34;
	wxBoxSizer* BoxSizer35;
	wxBoxSizer* BoxSizer36;
	wxBoxSizer* BoxSizer37;
	wxBoxSizer* BoxSizer38;
	wxBoxSizer* BoxSizer39;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer40;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;

	Create(parent, wxID_ANY, _("Options"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer31 = new wxBoxSizer(wxVERTICAL);
	AuiNotebook1 = new wxAuiNotebook(this, ID_AUINOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
	Panel1 = new wxPanel(AuiNotebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer35 = new wxBoxSizer(wxVERTICAL);
	BoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
	StaticText17 = new wxStaticText(Panel1, ID_STATICTEXT17, _("Automatically check for new versions: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	BoxSizer34->Add(StaticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	WxComboBoxUpdatesCheckingPeriod = new wxComboBox(Panel1, ID_WXCOMBOBOXUPDATESCHECKINGPERIOD, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXUPDATESCHECKINGPERIOD"));
	BoxSizer34->Add(WxComboBoxUpdatesCheckingPeriod, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer34->Add(20,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxButtonCheckNow = new wxButton(Panel1, ID_WXBUTTONCHECKNOW, _("Check &now"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCHECKNOW"));
	BoxSizer34->Add(WxButtonCheckNow, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer35->Add(BoxSizer34, 0, wxALL|wxALIGN_LEFT, 0);
	WxCheckBoxCheckPrerelease = new wxCheckBox(Panel1, ID_WXCHECKBOXCHECKPRERELEASE, _("Check for new versions including &prereleases"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXCHECKPRERELEASE"));
	WxCheckBoxCheckPrerelease->SetValue(false);
	BoxSizer35->Add(WxCheckBoxCheckPrerelease, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer3->Add(BoxSizer35, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer27->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxLanguage = new wxComboBox(Panel1, ID_WXCOMBOBOXLANGUAGE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXLANGUAGE"));
	BoxSizer27->Add(WxComboBoxLanguage, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText16 = new wxStaticText(Panel1, ID_STATICTEXT16, _("Language of User Interface (must restart wxMEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	BoxSizer27->Add(StaticText16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer27, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer40->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxForceEncoding = new wxComboBox(Panel1, ID_WXCOMBOBOXFORCEENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXFORCEENCODING"));
	WxComboBoxForceEncoding->Disable();
	BoxSizer40->Add(WxComboBoxForceEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBoxForceEncoding = new wxCheckBox(Panel1, ID_WXCHECKBOXFORCEENCODING, _("Always use this encoding to open file (without encoding detection)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXFORCEENCODING"));
	WxCheckBoxForceEncoding->SetValue(false);
	BoxSizer40->Add(WxCheckBoxForceEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3->Add(BoxSizer40, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxEncoding = new wxComboBox(Panel1, ID_WXCOMBOBOXENCODING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_WXCOMBOBOXENCODING"));
	BoxSizer4->Add(WxComboBoxEncoding, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, _("Use this encoding to create new file or when wxMEdit cannot determine the encoding of old file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	BoxSizer4->Add(StaticText13, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer4, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxSingleInstance = new wxCheckBox(Panel1, ID_WXCHECKBOXSINGLEINSTANCE, _("Single Instance (must restart wxMEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXSINGLEINSTANCE"));
	WxCheckBoxSingleInstance->SetValue(false);
	BoxSizer7->Add(WxCheckBoxSingleInstance, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxReloadFiles = new wxCheckBox(Panel1, ID_WXCHECKBOXRELOADFILES, _("Reload files previously open on startup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRELOADFILES"));
	WxCheckBoxReloadFiles->SetValue(false);
	BoxSizer7->Add(WxCheckBoxReloadFiles, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRecordCaretMovements = new wxCheckBox(Panel1, ID_WXCHECKBOXRECORDCARETMOVEMENTS, _("Record caret movements in undo list"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRECORDCARETMOVEMENTS"));
	WxCheckBoxRecordCaretMovements->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRecordCaretMovements, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxRestoreCaretPos = new wxCheckBox(Panel1, ID_WXCHECKBOXRESTORECARETPOS, _("Restore previous caret position when files are opened"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRESTORECARETPOS"));
	WxCheckBoxRestoreCaretPos->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRestoreCaretPos, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer3->Add(BoxSizer7, 0, wxALL|wxALIGN_LEFT, 5);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
	Panel2 = new wxPanel(AuiNotebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer9->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxLineLength = new wxTextCtrl(Panel2, ID_WXEDITMAXLINELENGTH, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXLINELENGTH"));
	BoxSizer9->Add(WxEditMaxLineLength, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Max line length before Line-Wrap (must restart wxMEdit)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer9->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(BoxSizer9, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer10->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxColumns = new wxTextCtrl(Panel2, ID_WXEDITMAXCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXCOLUMNS"));
	BoxSizer10->Add(WxEditMaxColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT4, _("Columns of Wrap-By-Column"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer10->Add(StaticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(BoxSizer10, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer11->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditTabColumns = new wxTextCtrl(Panel2, ID_WXEDITTABCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITTABCOLUMNS"));
	BoxSizer11->Add(WxEditTabColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText5 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Columns of Tab"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	BoxSizer11->Add(StaticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(BoxSizer11, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer13->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditIndentColumns = new wxTextCtrl(Panel2, IID_WXEDITINDENTCOLUMNS, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("IID_WXEDITINDENTCOLUMNS"));
	BoxSizer13->Add(WxEditIndentColumns, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText6 = new wxStaticText(Panel2, ID_STATICTEXT6, _("Columns of Indent"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	BoxSizer13->Add(StaticText6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(BoxSizer13, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer12 = new wxBoxSizer(wxVERTICAL);
	WxCheckBoxTabOrSpaces = new wxCheckBox(Panel2, ID_WXCHECKBOXTABORSPACES, _("Insert Space char instead of Tab char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXTABORSPACES"));
	WxCheckBoxTabOrSpaces->SetValue(false);
	BoxSizer12->Add(WxCheckBoxTabOrSpaces, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxAutoIndent = new wxCheckBox(Panel2, ID_WXCHECKBOXAUTOINDENT, _("Auto Indent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXAUTOINDENT"));
	WxCheckBoxAutoIndent->SetValue(false);
	BoxSizer12->Add(WxCheckBoxAutoIndent, 0, wxALL|wxALIGN_LEFT, 2);
	WxCheckBoxAutoCompletePair = new wxCheckBox(Panel2, ID_WXCHECKBOXAUTOCOMPLETEPAIR, _("Auto complete character pair"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXAUTOCOMPLETEPAIR"));
	WxCheckBoxAutoCompletePair->SetValue(false);
	BoxSizer12->Add(WxCheckBoxAutoCompletePair, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxMouseSelectToCopy = new wxCheckBox(Panel2, ID_WXCHECKBOXMOUSESELECTTOCOPY, _("Auto copy the mouse-selected text to clipboard"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMOUSESELECTTOCOPY"));
	WxCheckBoxMouseSelectToCopy->SetValue(false);
	BoxSizer28->Add(WxCheckBoxMouseSelectToCopy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	WxCheckBoxWhenPressCtrlKey = new wxCheckBox(Panel2, ID_WXCHECKBOXWHENPRESSCTRLKEY, _("when pressing Ctrl key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXWHENPRESSCTRLKEY"));
	WxCheckBoxWhenPressCtrlKey->SetValue(false);
	BoxSizer28->Add(WxCheckBoxWhenPressCtrlKey, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer12->Add(BoxSizer28, 0, wxALL|wxALIGN_LEFT, 0);
	WxCheckBoxMiddleMouseToPaste = new wxCheckBox(Panel2, ID_WXCHECKBOXMIDDLEMOUSETOPASTE, _("Paste text from clipboard when pressing middle mouse button"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXMIDDLEMOUSETOPASTE"));
	WxCheckBoxMiddleMouseToPaste->SetValue(false);
	BoxSizer12->Add(WxCheckBoxMiddleMouseToPaste, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText18 = new wxStaticText(Panel2, ID_STATICTEXT18, _("Behavior of copying in hex editing area:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	BoxSizer6->Add(StaticText18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxBehaviorCopyInHexArea = new wxComboBox(Panel2, ID_COMBOBOXBEHAVIORCOPYINHEXAREA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXBEHAVIORCOPYINHEXAREA"));
	BoxSizer6->Add(WxComboBoxBehaviorCopyInHexArea, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer12->Add(BoxSizer6, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	StaticText19 = new wxStaticText(Panel2, ID_STATICTEXT19, _("Treat clipboard text as Hex String when pasting in hex area:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	BoxSizer23->Add(StaticText19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxComboBoxPasteAsHexString = new wxComboBox(Panel2, ID_COMBOBOXPASTEASHEXSTRING, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY|wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOXPASTEASHEXSTRING"));
	BoxSizer23->Add(WxComboBoxPasteAsHexString, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer12->Add(BoxSizer23, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer8->Add(BoxSizer12, 0, wxALL|wxALIGN_LEFT, 5);
	Panel2->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel2);
	BoxSizer8->SetSizeHints(Panel2);
	Panel3 = new wxPanel(AuiNotebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer15 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Text Mode"));
	WxCheckBoxPrintSyntax = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTSYNTAX, _("Print Syntax Highlighter"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTSYNTAX"));
	WxCheckBoxPrintSyntax->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintSyntax, 0, wxALL|wxALIGN_LEFT, 1);
	WxCheckBoxPrintLineNumber = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTLINENUMBER, _("Print Line Number"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTLINENUMBER"));
	WxCheckBoxPrintLineNumber->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintLineNumber, 0, wxALL|wxALIGN_LEFT, 1);
	WxCheckBoxPrintBookmark = new wxCheckBox(Panel3, ID_CHECKBOXPRINTBOOKMARK, _("Print Bookmark"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOXPRINTBOOKMARK"));
	WxCheckBoxPrintBookmark->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintBookmark, 0, wxALL|wxALIGN_LEFT, 1);
	WxCheckBoxPrintEndOfLine = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTENDOFLINE, _("Print End of Line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTENDOFLINE"));
	WxCheckBoxPrintEndOfLine->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintEndOfLine, 0, wxALL|wxALIGN_LEFT, 1);
	WxCheckBoxPrintTabChar = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTTABCHAR, _("Print Tab Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTTABCHAR"));
	WxCheckBoxPrintTabChar->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintTabChar, 0, wxALL|wxALIGN_LEFT, 1);
	WxCheckBoxPrintSpaceChar = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTSPACECHAR, _("Print Space Char"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTSPACECHAR"));
	WxCheckBoxPrintSpaceChar->SetValue(false);
	StaticBoxSizer1->Add(WxCheckBoxPrintSpaceChar, 0, wxALL|wxALIGN_LEFT, 1);
	BoxSizer15->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Hex Mode"));
	wxString __wxRadioBoxChoices_1[3] =
	{
	_("None"),
	_("First Page Only"),
	_("Every Page")
	};
	WxRadioBoxPrintOffset = new wxRadioBox(Panel3, ID_WXRADIOBOXPRINTOFFSET, _("Print Offset Header"), wxDefaultPosition, wxDefaultSize, 3, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _T("ID_WXRADIOBOXPRINTOFFSET"));
	StaticBoxSizer2->Add(WxRadioBoxPrintOffset, 0, wxALL|wxALIGN_LEFT, 1);
	BoxSizer15->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer15, 0, wxALL|wxALIGN_TOP, 5);
	BoxSizer16 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Header"));
	WxCheckBoxPrintPageHeader = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTPAGEHEADER, _("Print Page Header"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTPAGEHEADER"));
	WxCheckBoxPrintPageHeader->SetValue(false);
	StaticBoxSizer3->Add(WxCheckBoxPrintPageHeader, 0, wxALL|wxALIGN_LEFT, 1);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText7 = new wxStaticText(Panel3, ID_STATICTEXT7, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditHeaderLeft = new wxTextCtrl(Panel3, ID_WXEDITHEADERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERLEFT"));
	FlexGridSizer1->Add(WxEditHeaderLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button1 = new wxButton(Panel3, ID_BUTTON1, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer1->Add(Button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText8 = new wxStaticText(Panel3, ID_STATICTEXT8, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditHeaderCenter = new wxTextCtrl(Panel3, ID_WXEDITHEADERCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERCENTER"));
	FlexGridSizer1->Add(WxEditHeaderCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button2 = new wxButton(Panel3, ID_BUTTON2, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	FlexGridSizer1->Add(Button2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText9 = new wxStaticText(Panel3, ID_STATICTEXT9, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditHeaderRight = new wxTextCtrl(Panel3, ID_WXEDITHEADERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITHEADERRIGHT"));
	FlexGridSizer1->Add(WxEditHeaderRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button3 = new wxButton(Panel3, ID_BUTTON3, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	FlexGridSizer1->Add(Button3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer3->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 4);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxVERTICAL, Panel3, _("Page Footer"));
	WxCheckBoxPrintPageFooter = new wxCheckBox(Panel3, ID_WXCHECKBOXPRINTPAGEFOOTER, _("Print Page Footer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXPRINTPAGEFOOTER"));
	WxCheckBoxPrintPageFooter->SetValue(false);
	StaticBoxSizer4->Add(WxCheckBoxPrintPageFooter, 0, wxALL|wxALIGN_LEFT, 1);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText10 = new wxStaticText(Panel3, ID_STATICTEXT10, _("Left:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(StaticText10, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFooterLeft = new wxTextCtrl(Panel3, ID_WXEDITFOOTERLEFT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERLEFT"));
	FlexGridSizer2->Add(WxEditFooterLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button4 = new wxButton(Panel3, ID_BUTTON4, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	FlexGridSizer2->Add(Button4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText11 = new wxStaticText(Panel3, ID_STATICTEXT11, _("Center:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer2->Add(StaticText11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFooterCenter = new wxTextCtrl(Panel3, ID_WXEDITFOOTERLEFTCENTER, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERLEFTCENTER"));
	FlexGridSizer2->Add(WxEditFooterCenter, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button5 = new wxButton(Panel3, ID_BUTTON5, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	FlexGridSizer2->Add(Button5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticText12 = new wxStaticText(Panel3, ID_STATICTEXT12, _("Right:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer2->Add(StaticText12, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
	WxEditFooterRight = new wxTextCtrl(Panel3, ID_WXEDITFOOTERRIGHT, wxEmptyString, wxDefaultPosition, wxSize(240,-1), 0, wxDefaultValidator, _T("ID_WXEDITFOOTERRIGHT"));
	FlexGridSizer2->Add(WxEditFooterRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Button6 = new wxButton(Panel3, ID_BUTTON6, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	FlexGridSizer2->Add(Button6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);
	StaticBoxSizer4->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer16->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 4);
	BoxSizer14->Add(BoxSizer16, 0, wxALL|wxALIGN_TOP, 2);
	Panel3->SetSizer(BoxSizer14);
	BoxSizer14->Fit(Panel3);
	BoxSizer14->SetSizeHints(Panel3);
	Panel4 = new wxPanel(AuiNotebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	TreeCtrl1 = new wxTreeCtrl(Panel4, ID_TREECTRL1, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
	BoxSizer19->Add(TreeCtrl1, 1, wxALL|wxEXPAND, 3);
	BoxSizer18->Add(BoxSizer19, 2, wxALL|wxEXPAND, 4);
	BoxSizer20 = new wxBoxSizer(wxVERTICAL);
	BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	WxStaticTextCommandHint = new wxStaticText(Panel4, ID_WXSTATICTEXTCOMMANDHINT, _("Hint:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTCOMMANDHINT"));
	BoxSizer24->Add(WxStaticTextCommandHint, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	WxEditCommandHint = new wxTextCtrl(Panel4, ID_WXEDITHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_WXEDITHINT"));
	WxEditCommandHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer24->Add(WxEditCommandHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer20->Add(BoxSizer24, 0, wxALL|wxEXPAND, 2);
	BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer22 = new wxBoxSizer(wxVERTICAL);
	StaticText14 = new wxStaticText(Panel4, ID_STATICTEXT14, _("Assigned Keys:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	BoxSizer22->Add(StaticText14, 0, wxALL|wxALIGN_LEFT, 5);
	WxListBoxKeys = new wxListBox(Panel4, ID_WXLISTBOXKEYS, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("ID_WXLISTBOXKEYS"));
	BoxSizer22->Add(WxListBoxKeys, 1, wxALL|wxEXPAND, 2);
	BoxSizer21->Add(BoxSizer22, 0, wxALL|wxEXPAND, 2);
	BoxSizer25 = new wxBoxSizer(wxVERTICAL);
	StaticText15 = new wxStaticText(Panel4, ID_STATICTEXT15, _("New Key:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	BoxSizer25->Add(StaticText15, 0, wxALL|wxALIGN_LEFT, 5);
	WxEditKey = new KeyTextCtrl(Panel4, ID_WXEDITKEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxWANTS_CHARS, wxDefaultValidator, _T("ID_WXEDITKEY"));
	BoxSizer25->Add(WxEditKey, 0, wxALL|wxEXPAND, 5);
	WxEditKeyHint = new wxTextCtrl(Panel4, ID_WXEDITKEYHINT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_WXEDITKEYHINT"));
	WxEditKeyHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	BoxSizer25->Add(WxEditKeyHint, 1, wxALL|wxEXPAND, 5);
	BoxSizer29 = new wxBoxSizer(wxVERTICAL);
	WxButtonAddKey = new wxButton(Panel4, ID_WXBUTTONADDKEY, _("<== Add Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONADDKEY"));
	BoxSizer29->Add(WxButtonAddKey, 0, wxALL|wxEXPAND, 5);
	WxButtonDeleteKey = new wxButton(Panel4, ID_WXBUTTONDELETEKEY, _("==> Delete Key"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONDELETEKEY"));
	BoxSizer29->Add(WxButtonDeleteKey, 0, wxALL|wxEXPAND, 5);
	WxButtonShowInMenu = new wxButton(Panel4, ID_WXBUTTONSHOWINMENU, _("Show the Key in Menu"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONSHOWINMENU"));
	BoxSizer29->Add(WxButtonShowInMenu, 0, wxALL|wxEXPAND, 5);
	BoxSizer25->Add(BoxSizer29, 0, wxALL|wxALIGN_LEFT, 0);
	BoxSizer21->Add(BoxSizer25, 5, wxALL|wxEXPAND, 1);
	BoxSizer20->Add(BoxSizer21, 1, wxALL|wxEXPAND, 2);
	BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxResetAllKeys = new wxCheckBox(Panel4, ID_WXCHECKBOXRESETALLKEYS, _("Reset all keys to default (must restart wxMEdit)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXRESETALLKEYS"));
	WxCheckBoxResetAllKeys->SetValue(false);
	BoxSizer26->Add(WxCheckBoxResetAllKeys, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer20->Add(BoxSizer26, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer18->Add(BoxSizer20, 3, wxALL|wxEXPAND, 2);
	Panel4->SetSizer(BoxSizer18);
	BoxSizer18->Fit(Panel4);
	BoxSizer18->SetSizeHints(Panel4);
	Panel5 = new wxPanel(AuiNotebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	BoxSizer36 = new wxBoxSizer(wxVERTICAL);
	BoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxDoNotSaveSettings = new wxCheckBox(Panel5, ID_WXCHECKBOXDONOTSAVESETTINGS, _("Do not save settings to wxmedit.cfg when wxMEdit closed (this session only)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXDONOTSAVESETTINGS"));
	WxCheckBoxDoNotSaveSettings->SetValue(false);
	BoxSizer37->Add(WxCheckBoxDoNotSaveSettings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(BoxSizer37, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer17->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxTextFileSize = new wxTextCtrl(Panel5, ID_WXEDITMAXTEXTFILESIZE, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXTEXTFILESIZE"));
	BoxSizer17->Add(WxEditMaxTextFileSize, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText2 = new wxStaticText(Panel5, ID_STATICTEXT2, _("Max file size to load as a text file"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer17->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer36->Add(BoxSizer17, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer38 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer38->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditMaxSizeToLoad = new wxTextCtrl(Panel5, ID_WXEDITMAXSIZETOLOAD, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXEDITMAXSIZETOLOAD"));
	BoxSizer38->Add(WxEditMaxSizeToLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText1 = new wxStaticText(Panel5, ID_STATICTEXT1, _("Max file size to load whole file into memory"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer38->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer36->Add(BoxSizer38, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxEditDateTime = new wxTextCtrl(Panel5, ID_WXEDITDATETIME, wxEmptyString, wxDefaultPosition, wxSize(160,-1), 0, wxDefaultValidator, _T("ID_WXEDITDATETIME"));
	BoxSizer5->Add(WxEditDateTime, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxStaticTextDateTime = new wxStaticText(Panel5, ID_WXSTATICTEXTDATETIME, _("Format of Date/Time"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_WXSTATICTEXTDATETIME"));
	BoxSizer5->Add(WxStaticTextDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(3,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	WxButtonDateTime = new wxButton(Panel5, ID_WXBUTTONDATETIME, _(">>"), wxDefaultPosition, wxSize(40,-1), 0, wxDefaultValidator, _T("ID_WXBUTTONDATETIME"));
	BoxSizer5->Add(WxButtonDateTime, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
	WxCheckBoxDateTimeInEnglish = new wxCheckBox(Panel5, ID_WXCHECKBOXDATETIMEINENGLISH, _("Use English instead of current locale"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXDATETIMEINENGLISH"));
	WxCheckBoxDateTimeInEnglish->SetValue(false);
	BoxSizer5->Add(WxCheckBoxDateTimeInEnglish, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(BoxSizer5, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
	WxCheckBoxDotMatchNewline = new wxCheckBox(Panel5, ID_WXCHECKBOXDOTMATCHNEWLINE, _("Enable dot matching newline in regular expression"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXCHECKBOXDOTMATCHNEWLINE"));
	WxCheckBoxDotMatchNewline->SetValue(false);
	BoxSizer39->Add(WxCheckBoxDotMatchNewline, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer36->Add(BoxSizer39, 0, wxALL|wxALIGN_LEFT, 5);
	Panel5->SetSizer(BoxSizer36);
	BoxSizer36->Fit(Panel5);
	BoxSizer36->SetSizeHints(Panel5);
	AuiNotebook1->AddPage(Panel1, _("General"));
	AuiNotebook1->AddPage(Panel2, _("Edit"));
	AuiNotebook1->AddPage(Panel3, _("Print"));
	AuiNotebook1->AddPage(Panel4, _("Keys"));
	AuiNotebook1->AddPage(Panel5, _("Misc"));
	BoxSizer31->Add(AuiNotebook1, 1, wxALL|wxEXPAND, 0);
	BoxSizer32 = new wxBoxSizer(wxVERTICAL);
	BoxSizer32->Add(450,0,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer31->Add(BoxSizer32, 0, wxALL|wxEXPAND, 0);
	BoxSizer30->Add(BoxSizer31, 1, wxALL|wxEXPAND, 0);
	BoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer33->Add(0,260,0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer30->Add(BoxSizer33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer1->Add(BoxSizer30, 1, wxALL|wxEXPAND, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxButtonOK = new wxButton(this, ID_WXBUTTONOK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONOK"));
	BoxSizer2->Add(WxButtonOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	WxButtonCancel = new wxButton(this, ID_WXBUTTONCANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_WXBUTTONCANCEL"));
	BoxSizer2->Add(WxButtonCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_WXBUTTONCHECKNOW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonCheckNowClick);
	Connect(ID_WXCHECKBOXFORCEENCODING,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxCheckBoxForceEncodingClick);
	Connect(ID_WXCHECKBOXMOUSESELECTTOCOPY,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxCheckBoxMouseSelectToCopyClick);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxTreeCtrl1SelChanged);
	Connect(ID_WXLISTBOXKEYS,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxListBoxKeysSelected);
	Connect(ID_WXBUTTONADDKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonAddKeyClick);
	Connect(ID_WXBUTTONDELETEKEY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonDeleteKeyClick);
	Connect(ID_WXBUTTONSHOWINMENU,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonShowInMenuClick);
	Connect(ID_WXBUTTONDATETIME,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonDateTimeClick);
	Connect(ID_WXBUTTONOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonOKClick);
	Connect(ID_WXBUTTONCANCEL,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WXMEditOptionsDialog::WxButtonCancelClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&WXMEditOptionsDialog::WXMEditOptionsDialogClose);
	//*)

	InitButtonRelativeEdit();

	WxPopupMenuPrintMark.Append(ID_MNU_MENUITEM1_1110, _("[%f] &File Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___P__PATHNAME_1111, _("[%p] &Path Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.AppendSeparator();
	WxPopupMenuPrintMark.Append(ID_MNU___N_PAGE_NUMBER_1113, _("[%n] Page &Number"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___S__TOTALPAGES_1114, _("[%s] Total Page&s"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.AppendSeparator();
	WxPopupMenuPrintMark.Append(ID_MNU___D__DATE_1116, _("[%d] &Date"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark.Append(ID_MNU___T__TIME_1117, _("[%t] &Time"), _(""), wxITEM_NORMAL);

	WxPopupMenuDateTimeMark.Append(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, _("[%Y/%m/%d %I:%M:%S %p] 2007/02/24 08:30:55 AM"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___D__M__Y_24_02_2007_1192, _("[%d/%m/%Y] 24/02/2007"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, _("[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.AppendSeparator();
	WxPopupMenuDateTimeMark.Append(ID_MNU_MENUITEM1_1170, _("[%a] Abbreviated weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___A_FULLWEEKDAYNAME_1171, _("[%A] Full weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___B_ABBREVIATEDMONTHNAME_1172, _("[%b] Abbreviated month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___B_FULLMONTHNAME_1173, _("[%B] Full month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, _("[%c] Date and time representation appropriate for locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, _("[%d] Day of month as decimal number (01 - 31)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, _("[%H] Hour in 24-hour format (00 - 23)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, _("[%I] Hour in 12-hour format (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, _("[%j] Day of year as decimal number (001 - 366)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, _("[%m] Month as decimal number (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, _("[%M] Minute as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, _("[%p] Current locale's A.M./P.M. indicator for 12-hour clock"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, _("[%S] Second as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, _("[%U] Week of year as decimal number, with Sunday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, _("[%w] Weekday as decimal number (0 - 6; Sunday is 0)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, _("[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, _("[%x] Date representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, _("[%X] Time representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, _("[%y] Year without century, as decimal number (00 - 99)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, _("[%Y] Year with century, as decimal number"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONENAME_1193, _("[%z] Time-zone name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark.Append(ID_MNU___Z_TIME_ZONEABBREVIATION_1194, _("[%Z] Time-zone abbreviation"), _(""), wxITEM_NORMAL);

	wxString systemenc(_("System Default"));
	WxComboBoxEncoding->Append(systemenc);
	WxComboBoxForceEncoding->Append(systemenc);
	size_t cnt=xm::EncodingManager::Instance().GetEncodingsCount();
	size_t i;
	for(i=0; i<cnt; i++)
	{
		std::wstring enc = xm::EncodingManager::Instance().GetEncodingNameWithAliases(i);
		WxComboBoxEncoding->Append(enc.c_str());
		WxComboBoxForceEncoding->Append(enc.c_str());
	}
	WxComboBoxEncoding->SetValue(systemenc);
	WxComboBoxForceEncoding->SetValue(systemenc);

	wxString systemlang(_("System Default"));
	WxComboBoxLanguage->Append(systemlang);
	for(i=1; i<g_LanguageCount; i++)
	{
		WxComboBoxLanguage->Append(wxString(g_LanguageString[i]));
	}
	WxComboBoxLanguage->SetValue(systemlang);

	BOOST_FOREACH(wxString peroid, wxm::UpdatePeriods::Instance().GetTitles())
		WxComboBoxUpdatesCheckingPeriod->Append(peroid);
	WxComboBoxUpdatesCheckingPeriod->SetValue(wxm::UpdatePeriods::Instance().GetDefaultTitle());

	BOOST_FOREACH(wxString behavior, wxm::HexAreaClipboardCopyProxy::Instance().GetTitles())
		WxComboBoxBehaviorCopyInHexArea->Append(behavior);
	WxComboBoxBehaviorCopyInHexArea->SetValue(wxm::HexAreaClipboardCopyProxy::Instance().GetDefaultTitle());

	BOOST_FOREACH(wxString behavior, wxm::HexAreaClipboardPasteProxy::Instance().GetTitles())
		WxComboBoxPasteAsHexString->Append(behavior);
	WxComboBoxPasteAsHexString->SetValue(wxm::HexAreaClipboardPasteProxy::Instance().GetDefaultTitle());

#ifdef __WXMSW__
	WxCheckBoxRightClickMenu = new wxCheckBox(Panel1, -1, _("Add wxMEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)"),
	                                          wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("WxCheckBoxRightClickMenu"));
	WxCheckBoxRightClickMenu->SetValue(false);

	WxCheckBoxConfigInUserHome = new wxCheckBox(Panel1, -1, _("Place configuration files into application data directory of current user (%APPDATA%\\wxmedit) rather than wxMEdit executable directory"),
	                                             wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("WxCheckBoxConfigInUserHome"));
	WxCheckBoxConfigInUserHome->SetValue(false);
	BoxSizer7->Add(WxCheckBoxRightClickMenu, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer7->Add(WxCheckBoxConfigInUserHome, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);

	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
#endif

	wxSize sz1 = BoxSizer3->CalcMin();
	wxSize sz2 = BoxSizer8->CalcMin();
	wxSize sz3 = BoxSizer14->CalcMin();
	wxSize sz4 = BoxSizer18->CalcMin();

	int ww = std::max(std::max(sz1.x, sz2.x), std::max(sz3.x, sz4.x));
	int hh = std::max(std::max(sz1.y, sz2.y), std::max(sz3.y, sz4.y));

	BoxSizer33->Add(0, hh + AuiNotebook1->GetTabCtrlHeight() + 2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer32->Add(ww + 2, 0, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);

	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	// build Command Tree
	TreeCtrl1->SetImageList(g_MainFrame->m_ImageList);
	wxTreeItemId root=TreeCtrl1->AddRoot(_("Commands"));

	wxTreeItemId menuRoot=TreeCtrl1->AppendItem(root, _("Menu"));
	wxTreeItemId editorRoot=TreeCtrl1->AppendItem(root, _("Editor"));

	list<wxTreeItemId> tree_stack;
	CommandData *cd = &CommandTable[0];
	wxTreeItemId tid;

	// Menu command
	do
	{
		if(cd->menu_level==0)
		{
			tid=TreeCtrl1->AppendItem(menuRoot, FilterChar(LocalizeText(cd->text)));
			tree_stack.clear();
			tree_stack.push_back(tid);
			++cd;
			continue;
		}
		else if(cd->menu_level < int(tree_stack.size()))
		{
			do
			{
				tree_stack.pop_back();
			}
			while(cd->menu_level < int(tree_stack.size()));
		}

		if(cd->menu_ptr != 0)
		{
			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(LocalizeText(cd->text)));
			tree_stack.push_back(tid);
		}
		else if(cd->kind != wxITEM_SEPARATOR)
		{
			TreeItemData *data=new TreeItemData;
			data->cmddata=cd;
			TreeItemDataList.push_back(data);

			tid=TreeCtrl1->AppendItem(tree_stack.back(), FilterChar(LocalizeText(cd->text)), cd->image_idx, cd->image_idx, data);
			TreeCtrl1->SetItemBold(tid, true);
		}

		++cd;
	}
	while(cd->menu_level>=0);

	// Editor command
	do
	{
		TreeItemData *data=new TreeItemData;
		data->cmddata=cd;
		TreeItemDataList.push_back(data);

		tid=TreeCtrl1->AppendItem(editorRoot, MadKeyBindings::CommandToText(cd->command), cd->image_idx, cd->image_idx, data);
		TreeCtrl1->SetItemBold(tid, true);
		cd++;
	}
	while(cd->command > 0);

	AuiNotebook1->SetWindowStyleFlag(wxAUI_NB_TOP|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS);

	WxButtonCancel->SetId(wxID_CANCEL);
}

WXMEditOptionsDialog::~WXMEditOptionsDialog()
{
	//(*Destroy(WXMEditOptionsDialog)
	//*)
}

wxString WXMEditOptionsDialog::GetSelectedLanguage()
{
	wxString wxs = WxComboBoxLanguage->GetValue();
	if (wxs == _("System Default"))
		return wxString();
	return wxs;
}

wxString WXMEditOptionsDialog::GetSelectedForceEncoding()
{
	wxString wxs = WxComboBoxForceEncoding->GetValue();
	if (wxs == _("System Default"))
		return wxString();
	return xm::EncodingManager::ExtractEncodingName(wxs.wc_str()).c_str();
}

wxString WXMEditOptionsDialog::GetSelectedEncoding()
{
	wxString wxs = WxComboBoxEncoding->GetValue();
	if (wxs == _("System Default"))
		return wxString();
	return xm::EncodingManager::ExtractEncodingName(wxs.wc_str()).c_str();
}

wxString WXMEditOptionsDialog::GetSelectedUpdatePeroid()
{
	wxString title = WxComboBoxUpdatesCheckingPeriod->GetValue();
	return wxm::UpdatePeriods::Instance().TitleToConfig(title);
}

wxString WXMEditOptionsDialog::GetSelectedBehaviorCopyInHexArea()
{
	wxString title = WxComboBoxBehaviorCopyInHexArea->GetValue();
	return wxm::HexAreaClipboardCopyProxy::Instance().TitleToConfig(title);
}

wxString WXMEditOptionsDialog::GetSelectedConditionPasteAsHexInHexArea()
{
	wxString title = WxComboBoxPasteAsHexString->GetValue();
	return wxm::HexAreaClipboardPasteProxy::Instance().TitleToConfig(title);
}

void WXMEditOptionsDialog::WXMEditOptionsDialogClose(wxCloseEvent& event)
{
	if(event.CanVeto())
	{
		event.Veto();
		Show(false);
		return;
	}

	g_OptionsDialog=nullptr;
	Destroy();
}

void WXMEditOptionsDialog::WXMOptionsDialogActivate(wxActivateEvent& event)
{
	if (event.GetActive())
	{
		EnableTabTraversal();

		if (FindFocus() == nullptr)
		{
			SetReturnCode(wxID_CANCEL);
			WxButtonCancel->SetFocus();
		}
	}

	event.Skip();
}

void WXMEditOptionsDialog::LoadOptions(void)
{
	wxConfigBase *cfg=wxConfigBase::Get(false);
	wxString oldpath=cfg->GetPath();
	cfg->SetPath(wxT("/wxMEdit"));

	long ll;
	bool bb;
	wxString ss;

	// General page
	cfg->Read(wxT("UpdatesCheckingPeriod"), &ss);
	WxComboBoxUpdatesCheckingPeriod->SetValue(wxm::UpdatePeriods::Instance().ConfigToTitle(ss));

	WxCheckBoxCheckPrerelease->SetValue(wxm::g_check_prerelease);

	cfg->Read(wxT("Language"), &ss);
	if (ss.IsEmpty())
		ss=_("System Default");
	WxComboBoxLanguage->SetValue(ss);

	cfg->Read(wxT("UseForceEncoding"), &bb, false);
	WxCheckBoxForceEncoding->SetValue(bb);
	SetUseForceEncoding(bb);

	ss.Clear();
	cfg->Read(wxT("ForceEncoding"), &ss);
	ss = ss.IsEmpty() ? wxString(_("System Default")) : wxString(xm::EncodingManager::Instance().ExpandEncodingAliases(ss.wc_str()).c_str());
	WxComboBoxForceEncoding->SetValue(ss);

	ss.Clear();
	cfg->Read(wxT("DefaultEncoding"), &ss);
	ss = ss.IsEmpty()? wxString(_("System Default")): wxString(xm::EncodingManager::Instance().ExpandEncodingAliases(ss.wc_str()).c_str());
	WxComboBoxEncoding->SetValue(ss);

	cfg->Read(wxT("SingleInstance"), &bb);
	WxCheckBoxSingleInstance->SetValue(bb);

	cfg->Read(wxT("ReloadFiles"), &bb, true);
	WxCheckBoxReloadFiles->SetValue(bb);

	cfg->Read(wxT("RecordCaretMovements"), &bb);
	WxCheckBoxRecordCaretMovements->SetValue(bb);

	cfg->Read(wxT("RestoreCaretPos"), &bb, true);
	WxCheckBoxRestoreCaretPos->SetValue(bb);

#ifdef __WXMSW__
	boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(wxm::s_wxsRegkeyClasses + wxT("*\\shell\\wxMEdit\\command")) );
	if(pRegKey->Exists())
	{
		wxString str;
		if(pRegKey->QueryValue(wxEmptyString, str))
		{
			wxString exepath=GetExecutablePath();
			WxCheckBoxRightClickMenu->SetValue( str.Upper().Find(exepath.Upper())>=0 );
		}
	}
	WxCheckBoxConfigInUserHome->SetValue(wxm::AppPath::Instance().ConfigWillBeInUserHome());
	WxCheckBoxConfigInUserHome->Enable(wxm::AppPath::Instance().AppDirWritable());
#endif

	// Edit page
	cfg->Read(wxT("MaxLineLength"), &ll);
	WxEditMaxLineLength->SetValue(wxString()<<ll);

	cfg->Read(wxT("MaxColumns"), &ll);
	WxEditMaxColumns->SetValue(wxString()<<ll);

	cfg->Read(wxT("TabColumns"), &ll);
	WxEditTabColumns->SetValue(wxString()<<ll);

	cfg->Read(wxT("IndentColumns"), &ll);
	WxEditIndentColumns->SetValue(wxString()<<ll);

	cfg->Read(wxT("InsertSpacesInsteadOfTab"), &bb);
	WxCheckBoxTabOrSpaces->SetValue(bb);

	cfg->Read(wxT("AutoIndent"), &bb);
	WxCheckBoxAutoIndent->SetValue(bb);

	cfg->Read(wxT("AutoCompletePair"), &bb);
	WxCheckBoxAutoCompletePair->SetValue(bb);

	cfg->Read(wxT("MouseSelectToCopy"), &bb);
	WxCheckBoxMouseSelectToCopy->SetValue(bb);
	WxCheckBoxWhenPressCtrlKey->Enable(bb);

	cfg->Read(wxT("MouseSelectToCopyWithCtrlKey"), &bb);
	WxCheckBoxWhenPressCtrlKey->SetValue(bb);

	cfg->Read(wxT("MiddleMouseToPaste"), &bb);
	WxCheckBoxMiddleMouseToPaste->SetValue(bb);

	WxComboBoxBehaviorCopyInHexArea->SetValue(
		wxm::HexAreaClipboardCopyProxy::Instance().GetSelectedCopierTitle());

	WxComboBoxPasteAsHexString->SetValue(
		wxm::HexAreaClipboardPasteProxy::Instance().GetSelectedConditionTitle());

	// Print page
	cfg->Read(wxT("PrintSyntax"), &bb);
	WxCheckBoxPrintSyntax->SetValue(bb);

	cfg->Read(wxT("PrintLineNumber"), &bb);
	WxCheckBoxPrintLineNumber->SetValue(bb);

	cfg->Read(wxT("PrintBookmark"), &bb);
	WxCheckBoxPrintBookmark->SetValue(bb);

	cfg->Read(wxT("PrintEndOfLine"), &bb);
	WxCheckBoxPrintEndOfLine->SetValue(bb);

	cfg->Read(wxT("PrintTabChar"), &bb);
	WxCheckBoxPrintTabChar->SetValue(bb);

	cfg->Read(wxT("PrintSpaceChar"), &bb);
	WxCheckBoxPrintSpaceChar->SetValue(bb);

	cfg->Read(wxT("PrintOffsetHeader"), &ll);
	WxRadioBoxPrintOffset->SetSelection(ll);

	cfg->Read(wxT("PrintPageHeader"), &bb);
	WxCheckBoxPrintPageHeader->SetValue(bb);

	cfg->Read(wxT("PageHeaderLeft"), &ss);
	WxEditHeaderLeft->SetValue(ss);
	cfg->Read(wxT("PageHeaderCenter"), &ss);
	WxEditHeaderCenter->SetValue(ss);
	cfg->Read(wxT("PageHeaderRight"), &ss);
	WxEditHeaderRight->SetValue(ss);

	cfg->Read(wxT("PrintPageFooter"), &bb);
	WxCheckBoxPrintPageFooter->SetValue(bb);

	cfg->Read(wxT("PageFooterLeft"), &ss);
	WxEditFooterLeft->SetValue(ss);
	cfg->Read(wxT("PageFooterCenter"), &ss);
	WxEditFooterCenter->SetValue(ss);
	cfg->Read(wxT("PageFooterRight"), &ss);
	WxEditFooterRight->SetValue(ss);

	// Keys page
	extern bool g_ResetAllKeys;
	WxCheckBoxResetAllKeys->SetValue(g_ResetAllKeys);

	ChangedTreeItemDataList.clear();

	// get all keys of each command and srore them in TreeItemDataList
	list<TreeItemData*>::iterator tidit=TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend=TreeItemDataList.end();
	do
	{
		CommandData *cd=(*tidit)->cmddata;
		(*tidit)->keys.Clear();
		MadEdit::ms_KeyBindings.GetKeys(cd->menu_id, cd->command, (*tidit)->keys);
	}
	while(++tidit != tiditend);

	g_SelectedCommandItem=nullptr;
	g_SelectedKeyId=-1;
	WxEditCommandHint->Clear();
	WxListBoxKeys->Clear();
	UpdateKeyHint();

	wxTreeItemId selid=TreeCtrl1->GetSelection();
	if(selid.IsOk())
	{
		TreeCtrl1->Unselect();
		TreeCtrl1->SelectItem(selid, true);
	}

	// Misc page
	extern bool g_DoNotSaveSettings;
	WxCheckBoxDoNotSaveSettings->SetValue(g_DoNotSaveSettings);

	cfg->Read(wxT("MaxSizeToLoad"), &ll);
	WxEditMaxSizeToLoad->SetValue(wxString()<<ll);

	cfg->Read(wxT("MaxTextFileSize"), &ll);
	WxEditMaxTextFileSize->SetValue(wxString()<<ll);

	cfg->Read(wxT("DateTimeFormat"), &ss, wxT("%c"));
	WxEditDateTime->SetValue(ss);

	cfg->Read(wxT("DateTimeInEnglish"), &bb, false);
	WxCheckBoxDateTimeInEnglish->SetValue(bb);

	extern bool g_regex_dot_match_newline;
	WxCheckBoxDotMatchNewline->SetValue(g_regex_dot_match_newline);

	cfg->SetPath(oldpath);

}

void WXMEditOptionsDialog::EnableTabTraversal()
{
	SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
	AuiNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->AuiNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL);
	Panel4->SetWindowStyleFlag(g_OptionsDialog->Panel4->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
	WxButtonCancel->SetId(wxID_CANCEL);
}

void WXMEditOptionsDialog::DisableTabTraversal()
{
	SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
	AuiNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->AuiNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
	Panel4->SetWindowStyleFlag(g_OptionsDialog->Panel4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
	WxButtonCancel->SetId(WXMEditOptionsDialog::ID_WXBUTTONCANCEL);
}

void WXMEditOptionsDialog::WxButtonOKClick(wxCommandEvent& event)
{
	long lo;
	bool error=false;
	wxString errtext(_("Invalid value of \"%s\""));

	if(!WxEditMaxSizeToLoad->GetValue().ToLong(&lo) || lo<0)
	{
		wxLogError(errtext, StaticText1->GetLabel().c_str());
		error=true;
	}

	if(!WxEditMaxTextFileSize->GetValue().ToLong(&lo) || lo<0)
	{
		wxLogError(errtext, StaticText2->GetLabel().c_str());
		error=true;
	}

	if(!WxEditMaxLineLength->GetValue().ToLong(&lo) ||
		lo<wxm::MINVAL_MAXLINELEN || lo>wxm::MAXVAL_MAXLINELEN)
	{
		wxLogError(errtext, StaticText3->GetLabel().c_str());
		error=true;
	}

	if(!WxEditMaxColumns->GetValue().ToLong(&lo) || lo<=0)
	{
		wxLogError(errtext, StaticText4->GetLabel().c_str());
		error=true;
	}

	if(!WxEditTabColumns->GetValue().ToLong(&lo) || lo<=0)
	{
		wxLogError(errtext, StaticText5->GetLabel().c_str());
		error=true;
	}

	if(!WxEditIndentColumns->GetValue().ToLong(&lo) || lo<=0)
	{
		wxLogError(errtext, StaticText6->GetLabel().c_str());
		error=true;
	}

	if(!error) EndModal(wxID_OK);
}

void WXMEditOptionsDialog::WxButtonCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void WXMEditOptionsDialog::PrintMarkButtonClick(wxCommandEvent& event)
{
	ButtonID=event.GetId();
	PopupMenu(&WxPopupMenuPrintMark);
}

void WXMEditOptionsDialog::InitButtonRelativeEdit()
{
	m_btnid_edit_map[ID_BUTTON1] = WxEditHeaderLeft;
	m_btnid_edit_map[ID_BUTTON2] = WxEditHeaderCenter;
	m_btnid_edit_map[ID_BUTTON3] = WxEditHeaderRight;
	m_btnid_edit_map[ID_BUTTON4] = WxEditFooterLeft;
	m_btnid_edit_map[ID_BUTTON5] = WxEditFooterCenter;
	m_btnid_edit_map[ID_BUTTON6] = WxEditFooterRight;
}

wxTextCtrl* WXMEditOptionsDialog::GetButtonRelativeEdit(long btnid)
{
	return xm::wrap_map(m_btnid_edit_map).get(btnid, nullptr);
}

void WXMEditOptionsDialog::PrintMarkClick(wxCommandEvent& event)
{
	wxString str=WxPopupMenuPrintMark.GetLabel(event.GetId());
	wxTextCtrl *edit = GetButtonRelativeEdit(ButtonID);

	if(edit!=nullptr && str[0]==wxT('[') && str[3]==wxT(']'))
	{
		wxString text=edit->GetValue();
		edit->SetValue(text+ str.Mid(1, 2));
	}
	ButtonID=0;
}

void WXMEditOptionsDialog::WxTreeCtrl1SelChanged(wxTreeEvent& event)
{
	wxTreeItemId id=event.GetItem();
	if(!id.IsOk()) return;

	g_SelectedCommandItem=(TreeItemData*)TreeCtrl1->GetItemData(id);
	g_SelectedKeyId=-1;

	if(g_SelectedCommandItem==nullptr)
	{
		WxListBoxKeys->Clear();
		WxEditCommandHint->SetValue(_("Cannot assign key to this item"));
	}
	else
	{
		WxListBoxKeys->Set(g_SelectedCommandItem->keys);
		wxString hint=wxT('[');
		if (g_SelectedCommandItem->cmddata->menuid_name != nullptr)
		{
			hint+= wxString(g_SelectedCommandItem->cmddata->menuid_name);
		}
		if (g_SelectedCommandItem->cmddata->command > 0)
		{
			wxString text=MadKeyBindings::CommandToText(g_SelectedCommandItem->cmddata->command);
			if(!text.IsEmpty())
			{
				if(hint.Length()>1)
				{
					hint+= wxT(',');
				}
				hint+= text;
			}
		}
		hint+= wxT("] ");
		hint+= wxString(wxGetTranslation(g_SelectedCommandItem->cmddata->hint));

		WxEditCommandHint->SetValue(hint);
	}
}

void WXMEditOptionsDialog::WxListBoxKeysSelected(wxCommandEvent& event)
{
	g_SelectedKeyId=event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* WXMEditOptionsDialog::FindKeyInList(const wxString &key)
{
	list<TreeItemData*>::iterator tidit=TreeItemDataList.begin();
	list<TreeItemData*>::iterator tiditend=TreeItemDataList.end();

	wxString lkey=key.Lower();

	while(tidit!=tiditend)
	{
		TreeItemData *tid = (*tidit);
		if(!tid->keys.IsEmpty())
		{
			size_t count=tid->keys.GetCount();
			size_t idx=0;
			for(;idx<count;idx++)
			{
				if(tid->keys[idx].Lower() == lkey)
				{
					return tid;
				}
			}
		}

		++tidit;
	}
	return nullptr;
}

bool WXMEditOptionsDialog::FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist)
{
	list<TreeItemData*>::const_iterator tidit=tlist.begin();
	list<TreeItemData*>::const_iterator tiditend=tlist.end();

	while(tidit!=tiditend)
	{
		if((*tidit) == tid)
		{
			return true;
		}
		++tidit;
	}
	return false;
}

void WXMEditOptionsDialog::UpdateKeyHint()
{
	wxString scstr=WxEditKey->GetValue();
	g_CommandItemOfNewKey=nullptr;

	if(scstr.IsEmpty())
	{
		g_OptionsDialog->WxEditKeyHint->Clear();
	}
	else
	{
		// find the key is assigned to which command
		TreeItemData *tid = g_OptionsDialog->FindKeyInList(scstr);
		if(tid==nullptr)
		{
			g_OptionsDialog->WxEditKeyHint->SetValue(_("This key is not assigned"));
		}
		else
		{
			g_CommandItemOfNewKey=tid;
			wxString cmd;

			if (tid->cmddata->menuid_name != nullptr)
			{
				cmd+= wxString(tid->cmddata->menuid_name);
			}
			if (tid->cmddata->command > 0)
			{
				wxString text=MadKeyBindings::CommandToText(tid->cmddata->command);
				if(!text.IsEmpty())
				{
					if(cmd.Length()>1)
					{
						cmd+= wxT(',');
					}
					cmd+= text;
				}
			}

			wxString str=wxString::Format(_("This key is assigned to [%s]"), cmd.c_str());
			g_OptionsDialog->WxEditKeyHint->SetValue(str);
		}
	}
}

void WXMEditOptionsDialog::WxButtonAddKeyClick(wxCommandEvent& event)
{
	if(g_SelectedCommandItem!=nullptr && g_SelectedCommandItem!=g_CommandItemOfNewKey)
	{
		wxString key=WxEditKey->GetValue();
		if(g_CommandItemOfNewKey!=nullptr) // new key is assigned to another command
		{
			/*
			wxMessageDialog dlg(this, key +wxT(": ") +WxEditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
				wxT("wxMEdit"), wxYES_NO|wxICON_QUESTION );
			if(dlg.ShowModal()!=wxID_YES)
			{
				return;
			}
			*/

			//delete the key from g_CommandItemOfNewKey
			int idx=g_CommandItemOfNewKey->keys.Index(key.c_str(), false);
			wxASSERT(idx>=0);
			g_CommandItemOfNewKey->keys.RemoveAt(idx);
			if(FindItemInList(g_CommandItemOfNewKey, ChangedTreeItemDataList)==false)
			{
				ChangedTreeItemDataList.push_back(g_CommandItemOfNewKey);
			}
		}

		// add the key to g_SelectedCommandItem
		g_SelectedCommandItem->keys.Add(key);

		WxListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void WXMEditOptionsDialog::WxButtonDeleteKeyClick(wxCommandEvent& event)
{
	if(g_SelectedCommandItem!=nullptr && g_SelectedKeyId >= 0)
	{
		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);

		WxListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
		UpdateKeyHint();
	}
}

void WXMEditOptionsDialog::WxButtonShowInMenuClick(wxCommandEvent& event)
{
	// move the selected key to first element of WxListBoxKeys
	if(g_SelectedCommandItem!=nullptr && g_SelectedCommandItem->cmddata->menu_id>0 && g_SelectedKeyId > 0)
	{
		wxString key=g_SelectedCommandItem->keys[g_SelectedKeyId];

		g_SelectedCommandItem->keys.RemoveAt(g_SelectedKeyId);
		g_SelectedCommandItem->keys.Insert(key, 0);

		WxListBoxKeys->Set(g_SelectedCommandItem->keys);
		if(FindItemInList(g_SelectedCommandItem, ChangedTreeItemDataList)==false)
		{
			ChangedTreeItemDataList.push_back(g_SelectedCommandItem);
		}

		g_SelectedKeyId=-1;
	}
}

void WXMEditOptionsDialog::WxButtonDateTimeClick(wxCommandEvent& event)
{
	PopupMenu(&WxPopupMenuDateTimeMark);
}

void WXMEditOptionsDialog::DateTimeMarkClick(wxCommandEvent& event)
{
	wxString str=WxPopupMenuDateTimeMark.GetLabel(event.GetId());
	int pos=str.Find(wxT(']'));
	if(pos != wxNOT_FOUND && str[0]==wxT('['))
	{
		str = str.Mid(1, pos-1);
	}

	wxString text=WxEditDateTime->GetValue();
	WxEditDateTime->SetValue(text+str);
}


void WXMEditOptionsDialog::WxButtonCheckNowClick(wxCommandEvent& event)
{
	wxm::ManualCheckUpdates();
}

void WXMEditOptionsDialog::OnUpdateUI_DialogOptions_CheckNow(wxUpdateUIEvent& event)
{
	event.Enable(!wxm::g_update_checking);
}

void WXMEditOptionsDialog::WxCheckBoxMouseSelectToCopyClick(wxCommandEvent& event)
{
	WxCheckBoxWhenPressCtrlKey->Enable(event.IsChecked());
}

void WXMEditOptionsDialog::WxCheckBoxForceEncodingClick(wxCommandEvent& event)
{
	SetUseForceEncoding(event.IsChecked());
}
