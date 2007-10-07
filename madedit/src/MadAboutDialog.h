///////////////////////////////////////////////////////////////////////////////
// Name:        MadAboutDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADABOUTDIALOG_H_
#define _MADABOUTDIALOG_H_

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
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
////Header Include End

#include <wx/dialog.h>

//Compatibility for 2.4.x code
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadAboutDialog_STYLE
#define MadAboutDialog_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class MadAboutDialog : public wxDialog
{
public:
    MadAboutDialog( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("About MadEdit"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = MadAboutDialog_STYLE);
    virtual ~MadAboutDialog();

public:

  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
		wxButton *WxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer3;
		wxTextCtrl *WxMemo1;
		wxStaticBitmap *WxStaticBitmap2;
		wxStaticBitmap *WxStaticBitmap1;
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
			ID_WXMEMO1 = 1011,
			ID_WXSTATICBITMAP2 = 1015,
			ID_WXSTATICBITMAP1 = 1016,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum

public:
    void MadAboutDialogClose(wxCloseEvent& event);
    void CreateGUIControls(void);

};


#endif
 
 
 
 
