///////////////////////////////////////////////////////////////////////////////
// Name:        MadWordCountDialog.h
// Description: WordCount Dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADWORDCOUNTDIALOG_H_
#define _MADWORDCOUNTDIALOG_H_

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
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>
////Header Include End

//Compatibility for 2.4 code
#ifndef wxCLOSE_BOX
	#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
	#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadWordCountDialog_STYLE
#define MadWordCountDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class MadWordCountDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadWordCountDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Word Count"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadWordCountDialog_STYLE);
		virtual ~MadWordCountDialog();
	
	public:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton1;
		wxTextCtrl *WxMemo1;
		wxStaticText *WxStaticText6;
		wxStaticLine *WxStaticLine1;
		wxStaticText *WxStaticTextFullwidthCount;
		wxStaticText *WxStaticText5;
		wxStaticText *WxStaticTextHalfwidthCount;
		wxStaticText *WxStaticText4;
		wxStaticText *WxStaticTextSpaceCount;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticTextCharCount;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticTextWordCount;
		wxStaticText *WxStaticText1;
		wxFlexGridSizer *WxFlexGridSizer1;
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
			ID_WXMEMO1 = 1019,
			ID_WXSTATICTEXT6 = 1018,
			ID_WXSTATICLINE1 = 1017,
			ID_WXSTATICTEXTFULLWIDTHCOUNT = 1013,
			ID_WXSTATICTEXT5 = 1012,
			ID_WXSTATICTEXTHALFWIDTHCOUNT = 1011,
			ID_WXSTATICTEXT4 = 1010,
			ID_WXSTATICTEXTSPACECOUNT = 1009,
			ID_WXSTATICTEXT3 = 1008,
			ID_WXSTATICTEXTCHARCOUNT = 1007,
			ID_WXSTATICTEXT2 = 1006,
			ID_WXSTATICTEXTWORDCOUNT = 1005,
			ID_WXSTATICTEXT1 = 1004,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		}; //End of Enum
	
	public:
		void MadWordCountDialogClose(wxCloseEvent& event);
		void CreateGUIControls(void);
};

#endif
