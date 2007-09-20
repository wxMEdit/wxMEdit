///////////////////////////////////////////////////////////////////////////////
// Name:        MadOptionsDialog.cpp
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadOptionsDialog.h"
#include <wx/fileconf.h>
#include <wx/config.h>
#include "MadEdit/MadEncoding.h"
#include "MadEdit/MadEditCommand.h"
#include "MadEdit/MadEdit.h"
#include "MadEditFrame.h"
#include "MadUtils.h"
#include "wx/aui/auibook.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

MadOptionsDialog *g_OptionsDialog=NULL;

extern wxChar *g_LanguageString[];
extern const size_t g_LanguageCount;

TreeItemData *g_SelectedCommandItem=NULL;
int g_SelectedKeyId=-1;
TreeItemData *g_CommandItemOfNewKey=NULL;


class KeyTextCtrl : public wxTextCtrl
{
    DECLARE_EVENT_TABLE()
public:
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
    {   // for getting Ctrl-Tab
        g_OptionsDialog->SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
        g_OptionsDialog->WxNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->WxNotebook1->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
        g_OptionsDialog->WxNoteBookPage4->SetWindowStyleFlag(g_OptionsDialog->WxNoteBookPage4->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
        g_OptionsDialog->WxButtonCancel->SetId(MadOptionsDialog::ID_WXBUTTONCANCEL);
        evt.Skip();
    }
    void OnKillFocus(wxFocusEvent &evt)
    {   // restore wxTAB_TRAVERSAL
        g_OptionsDialog->SetWindowStyleFlag(g_OptionsDialog->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
        g_OptionsDialog->WxNotebook1->wxControl::SetWindowStyleFlag(g_OptionsDialog->WxNotebook1->wxControl::GetWindowStyleFlag() | wxTAB_TRAVERSAL);
        g_OptionsDialog->WxNoteBookPage4->SetWindowStyleFlag(g_OptionsDialog->WxNoteBookPage4->GetWindowStyleFlag() | wxTAB_TRAVERSAL);
        g_OptionsDialog->WxButtonCancel->SetId(wxID_CANCEL);
        evt.Skip();
    }
};

BEGIN_EVENT_TABLE(KeyTextCtrl,wxTextCtrl)
    EVT_KEY_DOWN(KeyTextCtrl::OnKeyDown)
    EVT_SET_FOCUS(KeyTextCtrl::OnSetFocus)
    EVT_KILL_FOCUS (KeyTextCtrl::OnKillFocus)
END_EVENT_TABLE()


//----------------------------------------------------------------------------
// MadOptionsDialog
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp
    ////Event Table Start
BEGIN_EVENT_TABLE(MadOptionsDialog,wxDialog)
	////Manual Code Start
	EVT_BUTTON(ID_WXBUTTON1,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON2,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON3,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON4,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON5,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(ID_WXBUTTON6,MadOptionsDialog::PrintMarkButtonClick)
	EVT_BUTTON(wxID_CANCEL,MadOptionsDialog::WxButtonCancelClick)
	////Manual Code End

	EVT_CLOSE(MadOptionsDialog::MadOptionsDialogClose)
	EVT_ACTIVATE(MadOptionsDialog::MadOptionsDialogActivate)
	EVT_MENU(ID_MNU_MENUITEM1_1110 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___P__PATHNAME_1111 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___N_PAGE_NUMBER_1113 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___S__TOTALPAGES_1114 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___D__DATE_1116 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___T__TIME_1117 , MadOptionsDialog::PrintMarkClick)
	EVT_MENU(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D__M__Y_24_02_2007_1192 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU_MENUITEM1_1170 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___A_FULLWEEKDAYNAME_1171 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_ABBREVIATEDMONTHNAME_1172 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___B_FULLMONTHNAME_1173 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONENAME_1193 , MadOptionsDialog::DateTimeMarkClick)
	EVT_MENU(ID_MNU___Z_TIME_ZONEABBREVIATION_1194 , MadOptionsDialog::DateTimeMarkClick)
	EVT_BUTTON(ID_WXBUTTONCANCEL,MadOptionsDialog::WxButtonCancelClick)
	EVT_BUTTON(ID_WXBUTTONOK,MadOptionsDialog::WxButtonOKClick)
	EVT_BUTTON(ID_WXBUTTONSHOWINMENU,MadOptionsDialog::WxButtonShowInMenuClick)
	EVT_BUTTON(ID_WXBUTTONDELETEKEY,MadOptionsDialog::WxButtonDeleteKeyClick)
	EVT_BUTTON(ID_WXBUTTONADDKEY,MadOptionsDialog::WxButtonAddKeyClick)
	EVT_LISTBOX(ID_WXLISTBOXKEYS,MadOptionsDialog::WxListBoxKeysSelected)

	EVT_TREE_SEL_CHANGED(ID_WXTREECTRL1,MadOptionsDialog::WxTreeCtrl1SelChanged)
	EVT_BUTTON(ID_WXBUTTONDATETIME,MadOptionsDialog::WxButtonDateTimeClick)
END_EVENT_TABLE()
    ////Event Table End



MadOptionsDialog::MadOptionsDialog( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style)
{
    ButtonID=0;
    CreateGUIControls();
}

MadOptionsDialog::~MadOptionsDialog()
{
    delete WxPopupMenuPrintMark;
}

static void ResizeItem(wxSizer* sizer, wxWindow *item, int ax, int ay)
{
    int x, y;
    if(ax!=0)
    {
        wxString str=item->GetLabel();
        item->GetTextExtent(str, &x, &y);
    }
    else
    {
        item->GetSize(&x, &y);
    }
    item->SetSize(x+=ax, y+=ay);
    sizer->SetItemMinSize(item, x, y);
}

inline void UpdateSize(wxSize &s1, const wxSize &s2)
{
    if(s2.x > s1.x) s1.x=s2.x;
    if(s2.y > s1.y) s1.y=s2.y;
}

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

void MadOptionsDialog::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxNotebook1 = new wxAuiNotebook(this, ID_WXNOTEBOOK1, wxPoint(0,0),wxSize(681,342));
	WxBoxSizer1->Add(WxNotebook1,1,wxEXPAND | wxALL,0);

	WxNoteBookPage1 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE1, wxPoint(4,24), wxSize(673,314));
	WxNotebook1->AddPage(WxNoteBookPage1, _("General"));

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxNoteBookPage1->SetSizer(WxBoxSizer3);
	WxNoteBookPage1->SetAutoLayout(true);

	WxBoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer27, 0, wxALIGN_LEFT | wxALL, 2);

	wxArrayString arrayStringFor_WxComboBoxLanguage;
	WxComboBoxLanguage = new wxComboBox(WxNoteBookPage1, ID_WXCOMBOBOXLANGUAGE, _(""), wxPoint(0,3), wxSize(140,21), arrayStringFor_WxComboBoxLanguage, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxLanguage"));
	WxBoxSizer27->Add(WxComboBoxLanguage,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText16 = new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT16, _("Language of User Interface (must restart MadEdit)"), wxPoint(145,5), wxDefaultSize, 0, _("WxStaticText16"));
	WxBoxSizer27->Add(WxStaticText16,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer3->Add(WxBoxSizer4, 0, wxALIGN_LEFT | wxALL, 5);

	WxCheckBoxSingleInstance = new wxCheckBox(WxNoteBookPage1, ID_WXCHECKBOXSINGLEINSTANCE, _("Single Instance (must restart MadEdit)"), wxPoint(2,2), wxSize(300,20), 0, wxDefaultValidator, _("WxCheckBoxSingleInstance"));
	WxBoxSizer4->Add(WxCheckBoxSingleInstance,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxRecordCaretMovements = new wxCheckBox(WxNoteBookPage1, ID_WXCHECKBOXRECORDCARETMOVEMENTS, _("Record caret movements in undo list"), wxPoint(2,26), wxSize(300,20), 0, wxDefaultValidator, _("WxCheckBoxRecordCaretMovements"));
	WxBoxSizer4->Add(WxCheckBoxRecordCaretMovements,0,wxALIGN_LEFT | wxALL,2);

	WxBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer5, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditMaxSizeToLoad = new wxTextCtrl(WxNoteBookPage1, ID_WXEDITMAXSIZETOLOAD, _("0"), wxPoint(0,3), wxSize(80,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditMaxSizeToLoad"));
	WxBoxSizer5->Add(WxEditMaxSizeToLoad,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText1 = new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT1, _("Max file size to load whole file into memory"), wxPoint(85,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText1"));
	WxBoxSizer5->Add(WxStaticText1,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer6, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditMaxTextFileSize = new wxTextCtrl(WxNoteBookPage1, ID_WXEDITMAXTEXTFILESIZE, _("0"), wxPoint(0,3), wxSize(80,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditMaxTextFileSize"));
	WxBoxSizer6->Add(WxEditMaxTextFileSize,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText2 = new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT2, _("Max file size to load as a text file"), wxPoint(85,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText2"));
	WxBoxSizer6->Add(WxStaticText2,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer3->Add(WxBoxSizer17, 0, wxALIGN_LEFT | wxALL, 2);

	wxArrayString arrayStringFor_WxComboBoxEncoding;
	WxComboBoxEncoding = new wxComboBox(WxNoteBookPage1, ID_WXCOMBOBOXENCODING, _(""), wxPoint(0,3), wxSize(140,21), arrayStringFor_WxComboBoxEncoding, wxCB_DROPDOWN | wxCB_READONLY, wxDefaultValidator, _("WxComboBoxEncoding"));
	WxBoxSizer17->Add(WxComboBoxEncoding,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText13 = new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT13, _("Use this encoding to create new file or when MadEdit cannot determine the encoding of old file"), wxPoint(145,5), wxDefaultSize, 0, _("WxStaticText13"));
	WxBoxSizer17->Add(WxStaticText13,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer7 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer3->Add(WxBoxSizer7, 0, wxALIGN_LEFT | wxALL, 5);

	WxCheckBoxReloadFiles = new wxCheckBox(WxNoteBookPage1, ID_WXCHECKBOXRELOADFILES, _("Reload files previously open on startup"), wxPoint(2,2), wxSize(400,20), 0, wxDefaultValidator, _("WxCheckBoxReloadFiles"));
	WxBoxSizer7->Add(WxCheckBoxReloadFiles,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxRestoreCaretPos = new wxCheckBox(WxNoteBookPage1, ID_WXCHECKBOXRESTORECARETPOS, _("Restore previous caret position when files are opened"), wxPoint(2,26), wxSize(400,20), 0, wxDefaultValidator, _("WxCheckBoxRestoreCaretPos"));
	WxBoxSizer7->Add(WxCheckBoxRestoreCaretPos,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxDoNotSaveSettings = new wxCheckBox(WxNoteBookPage1, ID_WXCHECKBOXDONOTSAVESETTINGS, _("Do not save settings to MadEdit.cfg when MadEdit closed (this session only)"), wxPoint(2,50), wxSize(400,20), 0, wxDefaultValidator, _("WxCheckBoxDoNotSaveSettings"));
	WxBoxSizer7->Add(WxCheckBoxDoNotSaveSettings,0,wxALIGN_LEFT | wxALL,2);

	WxNoteBookPage2 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE2, wxPoint(4,24), wxSize(673,314));
	WxNotebook1->AddPage(WxNoteBookPage2, _("Edit"));

	WxBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	WxNoteBookPage2->SetSizer(WxBoxSizer8);
	WxNoteBookPage2->SetAutoLayout(true);

	WxBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer9, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditMaxLineLength = new wxTextCtrl(WxNoteBookPage2, ID_WXEDITMAXLINELENGTH, _("0"), wxPoint(0,3), wxSize(60,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditMaxLineLength"));
	WxBoxSizer9->Add(WxEditMaxLineLength,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText3 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT3, _("Max line length before Line-Wrap (must restart MadEdit)"), wxPoint(65,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText3"));
	WxBoxSizer9->Add(WxStaticText3,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer10, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditMaxColumns = new wxTextCtrl(WxNoteBookPage2, ID_WXEDITMAXCOLUMNS, _("0"), wxPoint(0,3), wxSize(60,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditMaxColumns"));
	WxBoxSizer10->Add(WxEditMaxColumns,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText4 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT4, _("Columns of Wrap-By-Column"), wxPoint(65,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText4"));
	WxBoxSizer10->Add(WxStaticText4,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer11, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditTabColumns = new wxTextCtrl(WxNoteBookPage2, ID_WXEDITTABCOLUMNS, _("0"), wxPoint(0,3), wxSize(60,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditTabColumns"));
	WxBoxSizer11->Add(WxEditTabColumns,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText5 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT5, _("Columns of Tab"), wxPoint(65,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText5"));
	WxBoxSizer11->Add(WxStaticText5,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer13, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditIndentColumns = new wxTextCtrl(WxNoteBookPage2, ID_WXEDITTABCOLUMNS, _("0"), wxPoint(0,3), wxSize(60,21), 0, wxTextValidator(wxFILTER_NUMERIC), _("WxEditIndentColumns"));
	WxBoxSizer13->Add(WxEditIndentColumns,0,wxALIGN_LEFT | wxALL,0);

	WxStaticText6 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT5, _("Columns of Indent"), wxPoint(65,5), wxSize(300, 17), wxST_NO_AUTORESIZE, _("WxStaticText6"));
	WxBoxSizer13->Add(WxStaticText6,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer8->Add(WxBoxSizer23, 0, wxALIGN_LEFT | wxALL, 2);

	WxEditDateTime = new wxTextCtrl(WxNoteBookPage2, ID_WXEDITDATETIME, _(""), wxPoint(0,3), wxSize(160,21), 0, wxDefaultValidator, _("WxEditDateTime"));
	WxBoxSizer23->Add(WxEditDateTime,0,wxALIGN_LEFT | wxALL,0);

	WxStaticTextDateTime = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXTDATETIME, _("Format of Date/Time"), wxPoint(165,5), wxDefaultSize, 0, _("WxStaticTextDateTime"));
	WxBoxSizer23->Add(WxStaticTextDateTime,0,wxALIGN_CENTER | wxALL,5);

	WxButtonDateTime = new wxButton(WxNoteBookPage2, ID_WXBUTTONDATETIME, _(">>"), wxPoint(273,1), wxSize(25,25), 0, wxDefaultValidator, _("WxButtonDateTime"));
	WxBoxSizer23->Add(WxButtonDateTime,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxDateTimeInEnglish = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXDATETIMEINENGLISH, _("Use English instead of current locale"), wxPoint(301,3), wxSize(200,20), 0, wxDefaultValidator, _("WxCheckBoxDateTimeInEnglish"));
	WxBoxSizer23->Add(WxCheckBoxDateTimeInEnglish,0,wxALIGN_LEFT | wxALL,2);

	WxBoxSizer12 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer8->Add(WxBoxSizer12, 0, wxALIGN_LEFT | wxALL, 5);

	WxCheckBoxTabOrSpaces = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXTABORSPACES, _("Insert Space char instead of Tab char"), wxPoint(24,2), wxSize(480,20), 0, wxDefaultValidator, _("WxCheckBoxTabOrSpaces"));
	WxBoxSizer12->Add(WxCheckBoxTabOrSpaces,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxAutoIndent = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXAUTOINDENT, _("Auto Indent"), wxPoint(24,26), wxSize(480,20), 0, wxDefaultValidator, _("WxCheckBoxAutoIndent"));
	WxBoxSizer12->Add(WxCheckBoxAutoIndent,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxAutoCompletePair = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXAUTOCOMPLETEPAIR, _("Auto complete character pair"), wxPoint(24,50), wxSize(480,20), 0, wxDefaultValidator, _("WxCheckBoxAutoCompletePair"));
	WxBoxSizer12->Add(WxCheckBoxAutoCompletePair,0,wxALIGN_LEFT | wxALL,2);

	WxBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer12->Add(WxBoxSizer28, 0, wxALIGN_LEFT | wxALL, 0);

	WxCheckBoxMouseSelectToCopy = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXMOUSESELECTTOCOPY, _("Auto copy the mouse-selected text to clipboard  ("), wxPoint(2,2), wxSize(260,20), 0, wxDefaultValidator, _("WxCheckBoxMouseSelectToCopy"));
	WxBoxSizer28->Add(WxCheckBoxMouseSelectToCopy,0,wxALIGN_LEFT | wxALL,2);

	WxRadioButtonEnable = new wxRadioButton(WxNoteBookPage2, ID_WXRADIOBUTTONENABLE, _("Enable"), wxPoint(266,2), wxSize(70,20), 0, wxDefaultValidator, _("WxRadioButtonEnable"));
	WxBoxSizer28->Add(WxRadioButtonEnable,0,wxALIGN_LEFT | wxALL,2);

	WxRadioButtonDisable = new wxRadioButton(WxNoteBookPage2, ID_WXRADIOBUTTONDISABLE, _("Disable"), wxPoint(340,2), wxSize(70,20), 0, wxDefaultValidator, _("WxRadioButtonDisable"));
	WxBoxSizer28->Add(WxRadioButtonDisable,0,wxALIGN_LEFT | wxALL,2);

	WxStaticText17 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT17, _("when pressing Ctrl key)"), wxPoint(414,3), wxDefaultSize, 0, _("WxStaticText17"));
	WxBoxSizer28->Add(WxStaticText17,0,wxALIGN_LEFT | wxALL,2);

	WxCheckBoxMiddleMouseToPaste = new wxCheckBox(WxNoteBookPage2, ID_WXCHECKBOXMIDDLEMOUSETOPASTE, _("Paste text from clipboard when pressing middle mouse button"), wxPoint(24,98), wxSize(480,20), 0, wxDefaultValidator, _("WxCheckBoxMiddleMouseToPaste"));
	WxBoxSizer12->Add(WxCheckBoxMiddleMouseToPaste,0,wxALIGN_LEFT | wxALL,2);

	WxNoteBookPage3 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint(4,24), wxSize(673,314));
	WxNotebook1->AddPage(WxNoteBookPage3, _("Print"));

	WxBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	WxNoteBookPage3->SetSizer(WxBoxSizer14);
	WxNoteBookPage3->SetAutoLayout(true);

	WxBoxSizer15 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer14->Add(WxBoxSizer15, 0, wxALIGN_LEFT | wxALL, 2);

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(WxNoteBookPage3, wxID_ANY, _("Text Mode"));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxVERTICAL);
	WxBoxSizer15->Add(WxStaticBoxSizer1, 0, wxALIGN_LEFT | wxALL, 4);

	WxCheckBoxPrintSyntax = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTSYNTAX, _("Print Syntax Highlighter"), wxPoint(6,16), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintSyntax"));
	WxStaticBoxSizer1->Add(WxCheckBoxPrintSyntax,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxPrintLineNumber = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTLINENUMBER, _("Print Line Number"), wxPoint(6,35), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintLineNumber"));
	WxStaticBoxSizer1->Add(WxCheckBoxPrintLineNumber,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxPrintEndOfLine = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTENDOFLINE, _("Print End of Line"), wxPoint(6,54), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintEndOfLine"));
	WxStaticBoxSizer1->Add(WxCheckBoxPrintEndOfLine,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxPrintTabChar = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTTABCHAR, _("Print Tab Char"), wxPoint(6,73), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintTabChar"));
	WxStaticBoxSizer1->Add(WxCheckBoxPrintTabChar,0,wxALIGN_LEFT | wxALL,1);

	WxCheckBoxPrintSpaceChar = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTSPACECHAR, _("Print Space Char"), wxPoint(6,92), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintSpaceChar"));
	WxStaticBoxSizer1->Add(WxCheckBoxPrintSpaceChar,0,wxALIGN_LEFT | wxALL,1);

	wxStaticBox* WxStaticBoxSizer2_StaticBoxObj = new wxStaticBox(WxNoteBookPage3, wxID_ANY, _("Hex Mode"));
	WxStaticBoxSizer2 = new wxStaticBoxSizer(WxStaticBoxSizer2_StaticBoxObj, wxHORIZONTAL);
	WxBoxSizer15->Add(WxStaticBoxSizer2, 0, wxALIGN_LEFT | wxALL, 4);

	wxArrayString arrayStringFor_WxRadioBoxPrintOffset;
	arrayStringFor_WxRadioBoxPrintOffset.Add(_("None"));
	arrayStringFor_WxRadioBoxPrintOffset.Add(_("First Page Only"));
	arrayStringFor_WxRadioBoxPrintOffset.Add(_("Every Page"));
	WxRadioBoxPrintOffset = new wxRadioBox(WxNoteBookPage3, ID_WXRADIOBOXPRINTOFFSET, _("Print Offset Header"), wxPoint(6,16), wxSize(150,130), arrayStringFor_WxRadioBoxPrintOffset, 1, wxRA_SPECIFY_COLS, wxDefaultValidator, _("WxRadioBoxPrintOffset"));
	WxRadioBoxPrintOffset->SetSelection(0);
	WxStaticBoxSizer2->Add(WxRadioBoxPrintOffset,1,wxALIGN_LEFT | wxALL,1);

	WxBoxSizer16 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer14->Add(WxBoxSizer16, 1, wxEXPAND | wxALL, 2);

	wxStaticBox* WxStaticBoxSizer3_StaticBoxObj = new wxStaticBox(WxNoteBookPage3, wxID_ANY, _("Page Header"));
	WxStaticBoxSizer3 = new wxStaticBoxSizer(WxStaticBoxSizer3_StaticBoxObj, wxVERTICAL);
	WxBoxSizer16->Add(WxStaticBoxSizer3, 1, wxEXPAND | wxALL, 4);

	WxCheckBoxPrintPageHeader = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTPAGEHEADER, _("Print Page Header"), wxPoint(86,16), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintPageHeader"));
	WxStaticBoxSizer3->Add(WxCheckBoxPrintPageHeader,0,wxALIGN_LEFT | wxALL,1);

	WxFlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	WxStaticBoxSizer3->Add(WxFlexGridSizer1, 1, wxEXPAND | wxALL, 2);

	WxStaticText7 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT7, _("Left:"), wxPoint(8,5), wxDefaultSize, 0, _("WxStaticText7"));
	WxFlexGridSizer1->Add(WxStaticText7,0,wxALIGN_RIGHT | wxALL,2);

	WxEditHeaderLeft = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERLEFT, _(""), wxPoint(44,3), wxSize(240,21), 0, wxDefaultValidator, _("WxEditHeaderLeft"));
	WxFlexGridSizer1->Add(WxEditHeaderLeft,0,0 | wxALL,2);

	WxButton1 = new wxButton(WxNoteBookPage3, ID_WXBUTTON1, _(">>"), wxPoint(287,1), wxSize(25,25), 0, wxDefaultValidator, _("WxButton1"));
	WxFlexGridSizer1->Add(WxButton1,0,wxALIGN_LEFT | wxALL,1);

	WxStaticText8 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT8, _("Center:"), wxPoint(2,32), wxDefaultSize, 0, _("WxStaticText8"));
	WxFlexGridSizer1->Add(WxStaticText8,0,wxALIGN_RIGHT | wxALL,2);

	WxEditHeaderCenter = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERCENTER, _(""), wxPoint(38,25), wxSize(240,21), 0, wxDefaultValidator, _("WxEditHeaderCenter"));
	WxFlexGridSizer1->Add(WxEditHeaderCenter,0,0 | wxALL,2);

	WxButton2 = new wxButton(WxNoteBookPage3, ID_WXBUTTON2, _(">>"), wxPoint(287,28), wxSize(25,25), 0, wxDefaultValidator, _("WxButton2"));
	WxFlexGridSizer1->Add(WxButton2,0,wxALIGN_LEFT | wxALL,1);

	WxStaticText9 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT9, _("Right:"), wxPoint(5,59), wxDefaultSize, 0, _("WxStaticText9"));
	WxFlexGridSizer1->Add(WxStaticText9,0,wxALIGN_RIGHT | wxALL,2);

	WxEditHeaderRight = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERRIGHT, _(""), wxPoint(44,57), wxSize(240,21), 0, wxDefaultValidator, _("WxEditHeaderRight"));
	WxFlexGridSizer1->Add(WxEditHeaderRight,0,0 | wxALL,2);

	WxButton3 = new wxButton(WxNoteBookPage3, ID_WXBUTTON3, _(">>"), wxPoint(287,55), wxSize(25,25), 0, wxDefaultValidator, _("WxButton3"));
	WxFlexGridSizer1->Add(WxButton3,0,wxALIGN_LEFT | wxALL,1);

	wxStaticBox* WxStaticBoxSizer4_StaticBoxObj = new wxStaticBox(WxNoteBookPage3, wxID_ANY, _("Page Footer"));
	WxStaticBoxSizer4 = new wxStaticBoxSizer(WxStaticBoxSizer4_StaticBoxObj, wxVERTICAL);
	WxBoxSizer16->Add(WxStaticBoxSizer4, 1, wxEXPAND | wxALL, 4);

	WxCheckBoxPrintPageFooter = new wxCheckBox(WxNoteBookPage3, ID_WXCHECKBOXPRINTPAGEFOOTER, _("Print Page Footer"), wxPoint(86,16), wxSize(150,17), 0, wxDefaultValidator, _("WxCheckBoxPrintPageFooter"));
	WxStaticBoxSizer4->Add(WxCheckBoxPrintPageFooter,0,wxALIGN_LEFT | wxALL,1);

	WxFlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	WxStaticBoxSizer4->Add(WxFlexGridSizer2, 1, wxEXPAND | wxALL, 2);

	WxStaticText10 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT7, _("Left:"), wxPoint(8,5), wxDefaultSize, 0, _("WxStaticText10"));
	WxFlexGridSizer2->Add(WxStaticText10,0,wxALIGN_RIGHT | wxALL,2);

	WxEditFooterLeft = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERLEFT, _(""), wxPoint(44,3), wxSize(240,21), 0, wxDefaultValidator, _("WxEditFooterLeft"));
	WxFlexGridSizer2->Add(WxEditFooterLeft,0,0 | wxALL,2);

	WxButton4 = new wxButton(WxNoteBookPage3, ID_WXBUTTON4, _(">>"), wxPoint(287,1), wxSize(25,25), 0, wxDefaultValidator, _("WxButton4"));
	WxFlexGridSizer2->Add(WxButton4,0,wxALIGN_LEFT | wxALL,1);

	WxStaticText11 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT8, _("Center:"), wxPoint(2,32), wxDefaultSize, 0, _("WxStaticText11"));
	WxFlexGridSizer2->Add(WxStaticText11,0,wxALIGN_RIGHT | wxALL,2);

	WxEditFooterCenter = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERCENTER, _(""), wxPoint(44,30), wxSize(240,21), 0, wxDefaultValidator, _("WxEditFooterCenter"));
	WxFlexGridSizer2->Add(WxEditFooterCenter,0,0 | wxALL,2);

	WxButton5 = new wxButton(WxNoteBookPage3, ID_WXBUTTON5, _(">>"), wxPoint(287,28), wxSize(25,25), 0, wxDefaultValidator, _("WxButton5"));
	WxFlexGridSizer2->Add(WxButton5,0,wxALIGN_LEFT | wxALL,1);

	WxStaticText12 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT9, _("Right:"), wxPoint(5,59), wxDefaultSize, 0, _("WxStaticText12"));
	WxFlexGridSizer2->Add(WxStaticText12,0,wxALIGN_RIGHT | wxALL,2);

	WxEditFooterRight = new wxTextCtrl(WxNoteBookPage3, ID_WXEDITHEADERRIGHT, _(""), wxPoint(44,57), wxSize(240,21), 0, wxDefaultValidator, _("WxEditFooterRight"));
	WxFlexGridSizer2->Add(WxEditFooterRight,0,0 | wxALL,2);

	WxButton6 = new wxButton(WxNoteBookPage3, ID_WXBUTTON6, _(">>"), wxPoint(287,55), wxSize(25,25), 0, wxDefaultValidator, _("WxButton6"));
	WxFlexGridSizer2->Add(WxButton6,0,wxALIGN_LEFT | wxALL,1);

	WxNoteBookPage4 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE4, wxPoint(4,24), wxSize(673,314));
	WxNotebook1->AddPage(WxNoteBookPage4, _("Keys"));

	WxBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	WxNoteBookPage4->SetSizer(WxBoxSizer18);
	WxNoteBookPage4->SetAutoLayout(true);

	WxBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer18->Add(WxBoxSizer19, 2, wxEXPAND | wxALL, 4);

	WxTreeCtrl1 = new wxTreeCtrl(WxNoteBookPage4, ID_WXTREECTRL1, wxPoint(3,3), wxSize(250,240), wxTR_HAS_BUTTONS | wxTR_DEFAULT_STYLE);
	WxBoxSizer19->Add(WxTreeCtrl1,1,wxEXPAND | wxALL,3);

	WxBoxSizer20 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer18->Add(WxBoxSizer20, 3, wxEXPAND | wxALL, 2);

	WxBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer20->Add(WxBoxSizer24, 0, wxEXPAND | wxALL, 2);

	WxStaticTextCommandHint = new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXTCOMMANDHINT, _("Hint:"), wxPoint(5,7), wxDefaultSize, 0, _("WxStaticTextCommandHint"));
	WxBoxSizer24->Add(WxStaticTextCommandHint,0,wxALIGN_LEFT | wxALL,5);

	WxEditCommandHint = new wxTextCtrl(WxNoteBookPage4, ID_WXEDITHINT, _(""), wxPoint(41,5), wxSize(300,21), wxTE_READONLY, wxDefaultValidator, _("WxEditCommandHint"));
	WxEditCommandHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	WxBoxSizer24->Add(WxEditCommandHint,1,wxEXPAND | wxALL,5);

	WxBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer20->Add(WxBoxSizer21, 1, wxEXPAND | wxALL, 2);

	WxBoxSizer22 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer21->Add(WxBoxSizer22, 0, wxEXPAND | wxALL, 2);

	WxStaticText14 = new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT14, _("Assigned Keys:"), wxPoint(34,5), wxDefaultSize, 0, _("WxStaticText14"));
	WxBoxSizer22->Add(WxStaticText14,0,wxALIGN_LEFT | wxALL,5);

	wxArrayString arrayStringFor_WxListBoxKeys;
	WxListBoxKeys = new wxListBox(WxNoteBookPage4, ID_WXLISTBOXKEYS, wxPoint(2,29), wxSize(140,200), arrayStringFor_WxListBoxKeys, wxLB_SINGLE);
	WxBoxSizer22->Add(WxListBoxKeys,1,wxEXPAND | wxALL,2);

	WxBoxSizer25 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer21->Add(WxBoxSizer25, 5, wxEXPAND | wxALL, 1);

	WxStaticText15 = new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT15, _("New Key:"), wxPoint(70,5), wxDefaultSize, 0, _("WxStaticText15"));
	WxBoxSizer25->Add(WxStaticText15,0,wxALIGN_LEFT | wxALL,5);

	WxEditKey = new KeyTextCtrl(WxNoteBookPage4, ID_WXEDITKEY, _(""), wxPoint(5,32), wxSize(180,21), wxWANTS_CHARS | wxTE_PROCESS_TAB, wxDefaultValidator, _("WxEditKey"));
	WxBoxSizer25->Add(WxEditKey,0,wxEXPAND | wxALL,5);

	WxEditKeyHint = new wxTextCtrl(WxNoteBookPage4, ID_WXEDITKEYHINT, _(""), wxPoint(5,63), wxSize(180,21), wxTE_READONLY, wxDefaultValidator, _("WxEditKeyHint"));
	WxEditKeyHint->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	WxBoxSizer25->Add(WxEditKeyHint,1,wxEXPAND | wxALL,5);

	WxButtonAddKey = new wxButton(WxNoteBookPage4, ID_WXBUTTONADDKEY, _("<== Add Key"), wxPoint(15,94), wxSize(160,28), 0, wxDefaultValidator, _("WxButtonAddKey"));
	WxBoxSizer25->Add(WxButtonAddKey,0,wxALIGN_LEFT | wxALL,5);

	WxButtonDeleteKey = new wxButton(WxNoteBookPage4, ID_WXBUTTONDELETEKEY, _("==> Delete Key"), wxPoint(15,132), wxSize(160,28), 0, wxDefaultValidator, _("WxButtonDeleteKey"));
	WxBoxSizer25->Add(WxButtonDeleteKey,0,wxALIGN_LEFT | wxALL,5);

	WxButtonShowInMenu = new wxButton(WxNoteBookPage4, ID_WXBUTTONSHOWINMENU, _("Show the Key in Menu"), wxPoint(15,170), wxSize(160,28), 0, wxDefaultValidator, _("WxButtonShowInMenu"));
	WxBoxSizer25->Add(WxButtonShowInMenu,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer20->Add(WxBoxSizer26, 0, wxALIGN_LEFT | wxALL, 2);

	WxCheckBoxResetAllKeys = new wxCheckBox(WxNoteBookPage4, ID_WXCHECKBOXRESETALLKEYS, _("Reset all keys to default (must restart MadEdit)"), wxPoint(5,5), wxSize(250,17), 0, wxDefaultValidator, _("WxCheckBoxResetAllKeys"));
	WxBoxSizer26->Add(WxCheckBoxResetAllKeys,0,wxALIGN_LEFT | wxALL,5);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	WxButtonOK = new wxButton(this, ID_WXBUTTONOK, _("&OK"), wxPoint(5,5), wxSize(85,30), 0, wxDefaultValidator, _("WxButtonOK"));
	WxBoxSizer2->Add(WxButtonOK,0,wxALIGN_CENTER | wxALL,5);

	WxButtonCancel = new wxButton(this, ID_WXBUTTONCANCEL, _("&Cancel"), wxPoint(100,5), wxSize(90,30), 0, wxDefaultValidator, _("WxButtonCancel"));
	WxBoxSizer2->Add(WxButtonCancel,0,wxALIGN_CENTER | wxALL,5);

	WxPopupMenuDateTimeMark = new wxMenu(_(""));WxPopupMenuDateTimeMark->Append(ID_MNU___Y__M__D_I__M__S_P_2007_02_2408_30_55AM_1191, _("[%Y/%m/%d %I:%M:%S %p] 2007/02/24 08:30:55 AM"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___D__M__Y_24_02_2007_1192, _("[%d/%m/%Y] 24/02/2007"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___A__D_B_Y_H__M__S_Z_RFC822TIMESTAMP_1195, _("[%a, %d %b %Y %H:%M:%S %z] RFC822 timestamp"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->AppendSeparator();
	WxPopupMenuDateTimeMark->Append(ID_MNU_MENUITEM1_1170, _("[%a] Abbreviated weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___A_FULLWEEKDAYNAME_1171, _("[%A] Full weekday name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___B_ABBREVIATEDMONTHNAME_1172, _("[%b] Abbreviated month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___B_FULLMONTHNAME_1173, _("[%B] Full month name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___C_DATEANDTIMEREPRESENTATIONAPPROPRIATEFORLOCALE_1174, _("[%c] Date and time representation appropriate for locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___D_DAYOFMONTHASDECIMALNUMBER_01_31__1175, _("[%d] Day of month as decimal number (01 - 31)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___H_HOURIN24_HOURFORMAT_00_23__1176, _("[%H] Hour in 24-hour format (00 - 23)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___I_HOURIN12_HOURFORMAT_01_12__1177, _("[%I] Hour in 12-hour format (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___J_DAYOFYEARASDECIMALNUMBER_001_366__1178, _("[%j] Day of year as decimal number (001 - 366)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___M_MONTHASDECIMALNUMBER_01_12__1179, _("[%m] Month as decimal number (01 - 12)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___M_MINUTEASDECIMALNUMBER_00_59__1180, _("[%M] Minute as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___P_CURRENTLOCALESA_M__P_M_INDICATORFOR12_HOURCLOCK_1181, _("[%p] Current locale's A.M./P.M. indicator for 12-hour clock"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___S_SECONDASDECIMALNUMBER_00_59__1182, _("[%S] Second as decimal number (00 - 59)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___U_WEEKOFYEARASDECIMALNUMBER_WITHSUNDAYASFIRSTDAYOFWEEK_00_53__1183, _("[%U] Week of year as decimal number, with Sunday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___W_WEEKDAYASDECIMALNUMBER_0_6_SUNDAYIS0__1184, _("[%w] Weekday as decimal number (0 - 6; Sunday is 0)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___W_WEEKOFYEARASDECIMALNUMBER_WITHMONDAYASFIRSTDAYOFWEEK_00_53__1185, _("[%W] Week of year as decimal number, with Monday as first day of week (00 - 53)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___X_DATEREPRESENTATIONFORCURRENTLOCALE_1186, _("[%x] Date representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___X_TIMEREPRESENTATIONFORCURRENTLOCALE_1187, _("[%X] Time representation for current locale"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___Y_YEARWITHOUTCENTURY_ASDECIMALNUMBER_00_99__1188, _("[%y] Year without century, as decimal number (00 - 99)"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___Y_YEARWITHCENTURY_ASDECIMALNUMBER_1189, _("[%Y] Year with century, as decimal number"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___Z_TIME_ZONENAME_1193, _("[%z] Time-zone name"), _(""), wxITEM_NORMAL);
	WxPopupMenuDateTimeMark->Append(ID_MNU___Z_TIME_ZONEABBREVIATION_1194, _("[%Z] Time-zone abbreviation"), _(""), wxITEM_NORMAL);

	WxPopupMenuPrintMark = new wxMenu(_(""));WxPopupMenuPrintMark->Append(ID_MNU_MENUITEM1_1110, _("[%f] &File Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark->Append(ID_MNU___P__PATHNAME_1111, _("[%p] &Path Name"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark->AppendSeparator();
	WxPopupMenuPrintMark->Append(ID_MNU___N_PAGE_NUMBER_1113, _("[%n] Page &Number"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark->Append(ID_MNU___S__TOTALPAGES_1114, _("[%s] Total Page&s"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark->AppendSeparator();
	WxPopupMenuPrintMark->Append(ID_MNU___D__DATE_1116, _("[%d] &Date"), _(""), wxITEM_NORMAL);
	WxPopupMenuPrintMark->Append(ID_MNU___T__TIME_1117, _("[%t] &Time"), _(""), wxITEM_NORMAL);

	SetTitle(_("Options"));
	SetIcon(wxNullIcon);

	GetSizer()->Layout();
	GetSizer()->Fit(this);

    ////GUI Items Creation End

    wxString systemenc(_("System Default"));
    WxComboBoxEncoding->Append(systemenc);
    size_t cnt=MadEncoding::GetEncodingsCount();
    size_t i;
    for(i=0; i<cnt; i++)
    {
        WxComboBoxEncoding->Append(MadEncoding::GetEncodingName(i));//enc+des);
    }
    WxComboBoxEncoding->SetValue(systemenc);

    for(i=0; i<g_LanguageCount; i++)
    {
        WxComboBoxLanguage->Append(wxString(g_LanguageString[i]));
    }
    WxComboBoxLanguage->SetValue(wxString(g_LanguageString[0]));

    ResizeItem(WxBoxSizer27, WxStaticText16, 2, 2);
    ResizeItem(WxBoxSizer4, WxCheckBoxSingleInstance, 25, 4);
    ResizeItem(WxBoxSizer4, WxCheckBoxRecordCaretMovements, 25, 4);
    ResizeItem(WxBoxSizer5, WxStaticText1, 2, 2);
    ResizeItem(WxBoxSizer6, WxStaticText2, 2, 2);
    ResizeItem(WxBoxSizer7, WxCheckBoxDoNotSaveSettings, 25, 4);
    ResizeItem(WxBoxSizer7, WxCheckBoxReloadFiles, 25, 4);
    ResizeItem(WxBoxSizer7, WxCheckBoxRestoreCaretPos, 25, 4);
    ResizeItem(WxBoxSizer17, WxStaticText13, 2, 2);

    ResizeItem(WxBoxSizer9, WxStaticText3, 2, 2);
    ResizeItem(WxBoxSizer10, WxStaticText4, 2, 2);
    ResizeItem(WxBoxSizer11, WxStaticText5, 2, 2);
    ResizeItem(WxBoxSizer13, WxStaticText6, 2, 2);
    ResizeItem(WxBoxSizer12, WxCheckBoxTabOrSpaces, 25, 4);
    ResizeItem(WxBoxSizer12, WxCheckBoxAutoIndent, 25, 4);
    ResizeItem(WxBoxSizer12, WxCheckBoxAutoCompletePair, 25, 4);
    ResizeItem(WxBoxSizer28, WxCheckBoxMouseSelectToCopy, 25, 4);
    ResizeItem(WxBoxSizer28, WxRadioButtonEnable, 25, 4);
    ResizeItem(WxBoxSizer28, WxRadioButtonDisable, 25, 4);
    ResizeItem(WxBoxSizer28, WxStaticText17, 2, 2);
    ResizeItem(WxBoxSizer12, WxCheckBoxMiddleMouseToPaste, 25, 4);
    ResizeItem(WxBoxSizer23, WxStaticTextDateTime, 2, 2);
    ResizeItem(WxBoxSizer23, WxCheckBoxDateTimeInEnglish, 25, 4);

    ResizeItem(WxStaticBoxSizer1, WxCheckBoxPrintSyntax, 25, 4);
    ResizeItem(WxStaticBoxSizer1, WxCheckBoxPrintLineNumber, 25, 4);
    ResizeItem(WxStaticBoxSizer1, WxCheckBoxPrintEndOfLine, 25, 4);
    ResizeItem(WxStaticBoxSizer1, WxCheckBoxPrintTabChar, 25, 4);
    ResizeItem(WxStaticBoxSizer1, WxCheckBoxPrintSpaceChar, 25, 4);

    int strx=0, stry=0;
    wxString str=WxRadioBoxPrintOffset->GetLabel();
    WxRadioBoxPrintOffset->GetTextExtent(str, &strx, &stry);
    for(i=0;i<WxRadioBoxPrintOffset->GetCount();i++)
    {
        int x;
        str=WxRadioBoxPrintOffset->GetString((unsigned int)i);
        WxRadioBoxPrintOffset->GetTextExtent(str, &x, &stry);
        if(x>strx) strx=x;
    }
    stry = (stry *((int)WxRadioBoxPrintOffset->GetCount()+2))*4/3;
    WxRadioBoxPrintOffset->SetSize(strx+=25, stry);
    WxStaticBoxSizer2->SetItemMinSize(WxRadioBoxPrintOffset, strx, stry);

    ResizeItem(WxStaticBoxSizer3, WxCheckBoxPrintPageHeader, 25, 4);
    ResizeItem(WxStaticBoxSizer3, WxStaticText7, 2, 2);
    ResizeItem(WxStaticBoxSizer3, WxStaticText8, 2, 2);
    ResizeItem(WxStaticBoxSizer3, WxStaticText9, 2, 2);
    ResizeItem(WxStaticBoxSizer3, WxEditHeaderLeft, 0, 2);
    ResizeItem(WxStaticBoxSizer3, WxEditHeaderCenter, 0, 2);
    ResizeItem(WxStaticBoxSizer3, WxEditHeaderRight, 0, 2);

    ResizeItem(WxStaticBoxSizer4, WxCheckBoxPrintPageFooter, 25, 4);
    ResizeItem(WxStaticBoxSizer4, WxStaticText10, 2, 2);
    ResizeItem(WxStaticBoxSizer4, WxStaticText11, 2, 2);
    ResizeItem(WxStaticBoxSizer4, WxStaticText12, 2, 2);
    ResizeItem(WxStaticBoxSizer4, WxEditFooterLeft, 0, 2);
    ResizeItem(WxStaticBoxSizer4, WxEditFooterCenter, 0, 2);
    ResizeItem(WxStaticBoxSizer4, WxEditFooterRight, 0, 2);

    ResizeItem(WxBoxSizer22, WxStaticText14, 2, 2);
    ResizeItem(WxBoxSizer25, WxStaticText15, 2, 2);
    ResizeItem(WxBoxSizer24, WxStaticTextCommandHint, 2, 2);

    ResizeItem(WxBoxSizer26, WxCheckBoxResetAllKeys, 25, 4);


#ifdef __WXMSW__
    WxCheckBoxRightClickMenu = new wxCheckBox(WxNoteBookPage1, -1, _("Add MadEdit to the RightClickMenu of Explorer(Deselect to Remove the Entry from Windows Registry)"), wxPoint(5,5), wxSize(400,20), 0, wxDefaultValidator, _T("WxCheckBoxRightClickMenu"));
    WxBoxSizer7->Add(WxCheckBoxRightClickMenu,0,wxALIGN_LEFT | wxALL,2);
    ResizeItem(WxBoxSizer7, WxCheckBoxRightClickMenu, 25, 4);
#endif

    wxSize size=WxBoxSizer1->GetMinSize();
    wxSize size1=WxBoxSizer3->GetMinSize();
    wxSize size2=WxBoxSizer8->GetMinSize();
    wxSize size3=WxBoxSizer14->GetMinSize();
    wxSize size4=WxBoxSizer18->GetMinSize();

    UpdateSize(size, size1);
    UpdateSize(size, size2);
    UpdateSize(size, size3);
    UpdateSize(size, size4);

    wxSize size5=WxBoxSizer2->GetMinSize();
    size.y += (size5.y + 30);

    this->SetClientSize(size);
    Center();

    // build Command Tree
    WxTreeCtrl1->SetImageList(g_MainFrame->m_ImageList);
    wxTreeItemId root=WxTreeCtrl1->AddRoot(_("Commands"));

    wxTreeItemId menuRoot=WxTreeCtrl1->AppendItem(root, _("Menu"));
    wxTreeItemId editorRoot=WxTreeCtrl1->AppendItem(root, _("Editor"));

    list<wxTreeItemId> tree_stack;
    CommandData *cd = &CommandTable[0];
    wxTreeItemId tid;

    // Menu command
    do
    {
        if(cd->menu_level==0)
        {
            tid=WxTreeCtrl1->AppendItem(menuRoot, FilterChar(wxGetTranslation(cd->text)));
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
            tid=WxTreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)));
            tree_stack.push_back(tid);
        }
        else if(cd->kind != wxITEM_SEPARATOR)
        {
            TreeItemData *data=new TreeItemData;
            data->cmddata=cd;
            TreeItemDataList.push_back(data);

            tid=WxTreeCtrl1->AppendItem(tree_stack.back(), FilterChar(wxGetTranslation(cd->text)), cd->image_idx, cd->image_idx, data);
            WxTreeCtrl1->SetItemBold(tid, true);
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

        tid=WxTreeCtrl1->AppendItem(editorRoot, MadKeyBindings::CommandToText(cd->command), cd->image_idx, cd->image_idx, data);
        WxTreeCtrl1->SetItemBold(tid, true);
        cd++;
    }
    while(cd->command > 0);

    WxNotebook1->SetWindowStyleFlag(wxAUI_NB_TOP|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS);

    WxButtonCancel->SetId(wxID_CANCEL);
}

void MadOptionsDialog::MadOptionsDialogClose(wxCloseEvent& event)
{
    // --> Don't use Close with a wxDialog,
    // use Destroy instead.

    if(event.CanVeto())
    {
        event.Veto();
        Show(false);
        return;
    }

    g_OptionsDialog=NULL;
    Destroy();
}


/*
 * MadOptionsDialogActivate
 */
void MadOptionsDialog::MadOptionsDialogActivate(wxActivateEvent& event)
{
    if(event.GetActive())
    {
        if(FindFocus()==NULL)
        {
            SetReturnCode(wxID_CANCEL);
            WxButtonCancel->SetFocus();
        }
    }

    event.Skip();
}

void MadOptionsDialog::LoadOptions(void)
{
    wxConfigBase *cfg=wxConfigBase::Get(false);
    wxString oldpath=cfg->GetPath();
    cfg->SetPath(wxT("/MadEdit"));

    long ll;
    bool bb;
    wxString ss;

    // General page
    ss=g_LanguageString[0];
    cfg->Read(wxT("Language"), &ss);
    WxComboBoxLanguage->SetValue(ss);

    cfg->Read(wxT("SingleInstance"), &bb);
    WxCheckBoxSingleInstance->SetValue(bb);

    cfg->Read(wxT("RecordCaretMovements"), &bb);
    WxCheckBoxRecordCaretMovements->SetValue(bb);

    cfg->Read(wxT("MaxSizeToLoad"), &ll);
    WxEditMaxSizeToLoad->SetValue(wxString()<<ll);

    cfg->Read(wxT("MaxTextFileSize"), &ll);
    WxEditMaxTextFileSize->SetValue(wxString()<<ll);

    ss=_("System Default");
    cfg->Read(wxT("DefaultEncoding"), &ss);
    WxComboBoxEncoding->SetValue(ss);

#ifdef __WXMSW__
    wxRegKey *pRegKey = new wxRegKey(wxT("HKEY_CLASSES_ROOT\\*\\shell\\MadEdit\\command"));
    if(pRegKey->Exists())
    {
        wxString str;
        if(pRegKey->QueryValue(wxEmptyString, str))
        {
            wxString exepath=GetExecutablePath();
            WxCheckBoxRightClickMenu->SetValue( str.Upper().Find(exepath.Upper())>=0 );
        }
    }
    delete pRegKey;
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

    cfg->Read(wxT("DateTimeFormat"), &ss, wxT("%c"));
    WxEditDateTime->SetValue(ss);

    cfg->Read(wxT("DateTimeInEnglish"), &bb, false);
    WxCheckBoxDateTimeInEnglish->SetValue(bb);

    cfg->Read(wxT("InsertSpacesInsteadOfTab"), &bb);
    WxCheckBoxTabOrSpaces->SetValue(bb);

    cfg->Read(wxT("AutoIndent"), &bb);
    WxCheckBoxAutoIndent->SetValue(bb);

    cfg->Read(wxT("AutoCompletePair"), &bb);
    WxCheckBoxAutoCompletePair->SetValue(bb);

    cfg->Read(wxT("MouseSelectToCopy"), &bb);
    WxCheckBoxMouseSelectToCopy->SetValue(bb);

    cfg->Read(wxT("MouseSelectToCopyWithCtrlKey"), &bb);
    WxRadioButtonEnable->SetValue(bb);
    WxRadioButtonDisable->SetValue(!bb);

    cfg->Read(wxT("MiddleMouseToPaste"), &bb);
    WxCheckBoxMiddleMouseToPaste->SetValue(bb);

    extern bool g_DoNotSaveSettings;
    WxCheckBoxDoNotSaveSettings->SetValue(g_DoNotSaveSettings);

    cfg->Read(wxT("ReloadFiles"), &bb, true);
    WxCheckBoxReloadFiles->SetValue(bb);

    cfg->Read(wxT("RestoreCaretPos"), &bb, true);
    WxCheckBoxRestoreCaretPos->SetValue(bb);

    // Print page
    cfg->Read(wxT("PrintSyntax"), &bb);
    WxCheckBoxPrintSyntax->SetValue(bb);

    cfg->Read(wxT("PrintLineNumber"), &bb);
    WxCheckBoxPrintLineNumber->SetValue(bb);

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

    g_SelectedCommandItem=NULL;
    g_SelectedKeyId=-1;
    WxEditCommandHint->Clear();
    WxListBoxKeys->Clear();
    UpdateKeyHint();

    wxTreeItemId selid=WxTreeCtrl1->GetSelection();
    if(selid.IsOk())
    {
        WxTreeCtrl1->Unselect();
        WxTreeCtrl1->SelectItem(selid, true);
    }

    cfg->SetPath(oldpath);

}

/*
 * WxButtonOKClick
 */
void MadOptionsDialog::WxButtonOKClick(wxCommandEvent& event)
{
    long lo;
    bool error=false;
    wxString errtext(_("Invalid value of \"%s\""));

    if(!WxEditMaxSizeToLoad->GetValue().ToLong(&lo) || lo<0)
    {
        wxLogError(errtext, WxStaticText1->GetLabel().c_str());
        error=true;
    }

    if(!WxEditMaxTextFileSize->GetValue().ToLong(&lo) || lo<0)
    {
        wxLogError(errtext, WxStaticText2->GetLabel().c_str());
        error=true;
    }

    if(!WxEditMaxLineLength->GetValue().ToLong(&lo) || lo<80)
    {
        wxLogError(errtext, WxStaticText3->GetLabel().c_str());
        error=true;
    }

    if(!WxEditMaxColumns->GetValue().ToLong(&lo) || lo<=0)
    {
        wxLogError(errtext, WxStaticText4->GetLabel().c_str());
        error=true;
    }

    if(!WxEditTabColumns->GetValue().ToLong(&lo) || lo<=0)
    {
        wxLogError(errtext, WxStaticText5->GetLabel().c_str());
        error=true;
    }

    if(!WxEditIndentColumns->GetValue().ToLong(&lo) || lo<=0)
    {
        wxLogError(errtext, WxStaticText6->GetLabel().c_str());
        error=true;
    }

    if(!error) EndModal(wxID_OK);
}

/*
 * WxButtonCancelClick
 */
void MadOptionsDialog::WxButtonCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void MadOptionsDialog::PrintMarkButtonClick(wxCommandEvent& event)
{
    ButtonID=event.GetId();
    PopupMenu(WxPopupMenuPrintMark);
}

void MadOptionsDialog::PrintMarkClick(wxCommandEvent& event)
{
    wxString str=WxPopupMenuPrintMark->GetLabel(event.GetId());
    wxTextCtrl *edit=NULL;
    switch(ButtonID)
    {
    case ID_WXBUTTON1: edit=WxEditHeaderLeft; break;
    case ID_WXBUTTON2: edit=WxEditHeaderCenter; break;
    case ID_WXBUTTON3: edit=WxEditHeaderRight; break;
    case ID_WXBUTTON4: edit=WxEditFooterLeft; break;
    case ID_WXBUTTON5: edit=WxEditFooterCenter; break;
    case ID_WXBUTTON6: edit=WxEditFooterRight; break;
    }

    if(edit!=NULL && str[0]==wxT('[') && str[3]==wxT(']'))
    {
        wxString text=edit->GetValue();
        edit->SetValue(text+ str.Mid(1, 2));
    }
    ButtonID=0;
}


/*
 * WxTreeCtrl1SelChanged
 */
void MadOptionsDialog::WxTreeCtrl1SelChanged(wxTreeEvent& event)
{
    wxTreeItemId id=event.GetItem();
    if(!id.IsOk()) return;

    g_SelectedCommandItem=(TreeItemData*)WxTreeCtrl1->GetItemData(id);
    g_SelectedKeyId=-1;

    if(g_SelectedCommandItem==NULL)
    {
        WxListBoxKeys->Clear();
        WxEditCommandHint->SetValue(_("Cannot assign key to this item"));
    }
    else
    {
        WxListBoxKeys->Set(g_SelectedCommandItem->keys);
        wxString hint=wxT('[');
        if(g_SelectedCommandItem->cmddata->menuid_name > 0)
        {
            hint+= wxString(g_SelectedCommandItem->cmddata->menuid_name);
        }
        if(g_SelectedCommandItem->cmddata->command > 0)
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

/*
 * WxListBoxKeysSelected
 */
void MadOptionsDialog::WxListBoxKeysSelected(wxCommandEvent& event)
{
    g_SelectedKeyId=event.GetSelection();
}

// find key in TreeItemDataList
TreeItemData* MadOptionsDialog::FindKeyInList(const wxString &key)
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
    return NULL;
}

bool MadOptionsDialog::FindItemInList(TreeItemData* tid, const list<TreeItemData*> &tlist)
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

void MadOptionsDialog::UpdateKeyHint()
{
    wxString scstr=WxEditKey->GetValue();
    g_CommandItemOfNewKey=NULL;

    if(scstr.IsEmpty())
    {
        g_OptionsDialog->WxEditKeyHint->Clear();
    }
    else
    {
        // find the key is assigned to which command
        TreeItemData *tid = g_OptionsDialog->FindKeyInList(scstr);
        if(tid==NULL)
        {
            g_OptionsDialog->WxEditKeyHint->SetValue(_("This key is not assigned"));
        }
        else
        {
            g_CommandItemOfNewKey=tid;
            wxString cmd;

            if(tid->cmddata->menuid_name > 0)
            {
                cmd+= wxString(tid->cmddata->menuid_name);
            }
            if(tid->cmddata->command > 0)
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

/*
 * WxButtonAddKeyClick
 */
void MadOptionsDialog::WxButtonAddKeyClick(wxCommandEvent& event)
{
    if(g_SelectedCommandItem!=NULL && g_SelectedCommandItem!=g_CommandItemOfNewKey)
    {
        wxString key=WxEditKey->GetValue();
        if(g_CommandItemOfNewKey!=NULL) // new key is assigned to another command
        {
            /*
            wxMessageDialog dlg(this, key +wxT(": ") +WxEditKeyHint->GetValue() +wxT("\n\n") + wxString(_("Do you want to reassign this key?")),
                wxT("MadEdit"), wxYES_NO|wxICON_QUESTION );
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

/*
 * WxButtonDeleteKeyClick
 */
void MadOptionsDialog::WxButtonDeleteKeyClick(wxCommandEvent& event)
{
    if(g_SelectedCommandItem!=NULL && g_SelectedKeyId >= 0)
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

/*
 * WxButtonShowInMenuClick
 */
void MadOptionsDialog::WxButtonShowInMenuClick(wxCommandEvent& event)
{
    // move the selected key to first element of WxListBoxKeys
    if(g_SelectedCommandItem!=NULL && g_SelectedCommandItem->cmddata->menu_id>0 && g_SelectedKeyId > 0)
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


/*
 * WxButtonDateTimeClick
 */
void MadOptionsDialog::WxButtonDateTimeClick(wxCommandEvent& event)
{
    PopupMenu(WxPopupMenuDateTimeMark);
}

void MadOptionsDialog::DateTimeMarkClick(wxCommandEvent& event)
{
    wxString str=WxPopupMenuDateTimeMark->GetLabel(event.GetId());
    int pos=str.Find(wxT(']'));
    if(pos != wxNOT_FOUND && str[0]==wxT('['))
    {
        str = str.Mid(1, pos-1);
    }

    wxString text=WxEditDateTime->GetValue();
    WxEditDateTime->SetValue(text+str);
}
