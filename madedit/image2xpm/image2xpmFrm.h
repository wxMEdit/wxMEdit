//---------------------------------------------------------------------------
//
// Name:        image2xpmFrm.h
// Author:      madedit@gmail.com
// Created:     2005/8/3 ¤U¤È 09:37:20
//
//---------------------------------------------------------------------------
#ifndef __image2xpmFrm_HPP_
#define __image2xpmFrm_HPP_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif


class MyPanel;


//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/panel.h>
////Header Include End

#include <wx/frame.h>

////Dialog Style Start
	#undef image2xpmFrm_STYLE
	#define image2xpmFrm_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class image2xpmFrm : public wxFrame
{
private:
    DECLARE_EVENT_TABLE()
public:
    image2xpmFrm( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("image2xpm"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = image2xpmFrm_STYLE);
    virtual ~image2xpmFrm();
public:
  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
	wxStaticText *WxStaticText1;
	wxButton *WxButton2;
	wxButton *WxButton1;
	MyPanel *WxPanel1;
	wxBoxSizer *WxBoxSizer1;
  ////GUI Control Declaration End
public:
	//Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
	//It may replace a numeric value in the enums names.
	enum {
////GUI Enum Control ID Start
ID_WXSTATICTEXT1 = 1006,
ID_WXBUTTON2 = 1005,
ID_WXBUTTON1 = 1004,
ID_WXPANEL1 = 1003,
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum
public:
    void image2xpmFrmClose(wxCloseEvent& event);
    void CreateGUIControls(void);
	void WxButton1Click(wxCommandEvent& event);
	void WxButton2Click(wxCommandEvent& event);

};

#endif
 
 
 
 
