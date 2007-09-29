///////////////////////////////////////////////////////////////////////////////
// Name:        MadReplaceDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADREPLACEDIALOG_H_
#define _MADREPLACEDIALOG_H_

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
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
////Header Include End

#include <wx/dialog.h>
#include <wx/docview.h>
#include <wx/fileconf.h>

//Compatibility for 2.4.x code
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadReplaceDialog_STYLE
#define MadReplaceDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class MadEdit;

class MadReplaceDialog : public wxDialog
{
public:
    MadReplaceDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Replace"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadReplaceDialog_STYLE);
    virtual ~MadReplaceDialog();

public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxTextCtrl *WxEditTo;
		wxStaticText *WxStaticTextTo;
		wxTextCtrl *WxEditFrom;
		wxStaticText *WxStaticTextFrom;
		wxCheckBox *WxCheckBoxSearchInSelection;
		wxBoxSizer *WxBoxSizer7;
		wxMenu *WxPopupMenuRecentReplaceText;
		wxButton *WxButtonClose;
		wxButton *WxButtonReplaceAll;
		wxButton *WxButtonReplace;
		wxButton *WxButtonFindNext;
		wxBoxSizer *WxBoxSizer3;
		wxCheckBox *WxCheckBoxFindHex;
		wxCheckBox *WxCheckBoxRegex;
		wxCheckBox *WxCheckBoxWholeWord;
		wxCheckBox *WxCheckBoxCaseSensitive;
		wxCheckBox *WxCheckBoxMoveFocus;
		wxBoxSizer *WxBoxSizer6;
		wxBoxSizer *WxBoxSizer5;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer2;
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
			ID_WXEDITTO = 22,
			ID_WXSTATICTEXTTO = 21,
			ID_WXEDITFROM = 20,
			ID_WXSTATICTEXTFROM = 19,
			ID_WXCHECKBOXSEARCHINSELECTION = 18,
			ID_WXBUTTONCLOSE = 1028,
			ID_WXBUTTONREPLACEALL = 1027,
			ID_WXBUTTONREPLACE = 1026,
			ID_WXBUTTONFINDNEXT = 1025,
			ID_WXCHECKBOXFINDHEX = 1024,
			ID_WXCHECKBOXREGEX = 1023,
			ID_WXCHECKBOXWHOLEWORD = 1022,
			ID_WXCHECKBOXCASESENSITIVE = 1021,
			ID_WXCHECKBOXMOVEFOCUS = 1029,
////GUI Enum Control ID End
            ID_MADEDIT1 = 1400,
            ID_MADEDIT2 = 1401,
            ID_WXBITMAPBUTTONRECENTFINDTEXT=1402,
            ID_WXBITMAPBUTTONRECENTREPLACETEXT=1403,

            ID_RECENTFINDTEXT1=1501,    // must be the same with MadSearchDialog
            ID_RECENTFINDTEXT20=1520,
            ID_RECENTREPLACETEXT1=ID_RECENTFINDTEXT20+1,
            ID_RECENTREPLACETEXT20=ID_RECENTREPLACETEXT1+19,
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    void MadReplaceDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);

    MadEdit *m_FindText, *m_ReplaceText;
    wxBitmapButton *WxBitmapButtonRecentFindText, *WxBitmapButtonRecentReplaceText;
    wxFileHistory *m_RecentReplaceText;

    void WxButtonCloseClick(wxCommandEvent& event);
    void WxButtonFindNextClick(wxCommandEvent& event);

    void ReadWriteSettings(bool bRead);
    void UpdateCheckBoxByCBHex(bool check);
    void UpdateSearchInSelection(bool check);
	void WxCheckBoxFindHexClick(wxCommandEvent& event);
	
    void MadReplaceDialogKeyDown(wxKeyEvent& event);
	
	void WxBitmapButtonRecentFindTextClick(wxCommandEvent& event);
	void WxBitmapButtonRecentReplaceTextClick(wxCommandEvent& event);
	void OnRecentFindText(wxCommandEvent& event);
	void OnRecentReplaceText(wxCommandEvent& event);
	
	void WxButtonReplaceClick(wxCommandEvent& event);
	void MadReplaceDialogActivate(wxActivateEvent& event);
	void WxButtonReplaceAllClick(wxCommandEvent& event);
	void WxCheckBoxSearchInSelectionClick(wxCommandEvent& event);
};

extern MadReplaceDialog *g_ReplaceDialog;

#endif
 
 
 
 
