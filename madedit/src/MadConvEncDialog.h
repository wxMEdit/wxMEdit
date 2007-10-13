///////////////////////////////////////////////////////////////////////////////
// Name:        MadConvEncDialog.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADCONVENCDIALOG_H_
#define _MADCONVENCDIALOG_H_

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
#include <wx/radiobox.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
////Header Include End

//Compatibility for 2.4 code
#ifndef wxCLOSE_BOX
	#define wxCLOSE_BOX 0x1000
#endif
#ifndef wxFIXED_MINSIZE
	#define wxFIXED_MINSIZE 0
#endif

////Dialog Style Start
#undef MadConvEncDialog_STYLE
#define MadConvEncDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class MadConvEncDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadConvEncDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Convert Encoding"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadConvEncDialog_STYLE);
		virtual ~MadConvEncDialog();
	
	public:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer4;
		wxRadioBox *WxRadioBoxOption;
		wxBoxSizer *WxBoxSizer3;
		wxComboBox *WxComboBoxEncoding;
		wxStaticText *WxStaticText1;
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
			ID_WXRADIOBOXOPTION = 1006,
			ID_WXCOMBOBOXENCODING = 1004,
			ID_WXSTATICTEXT1 = 1003,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		}; //End of Enum
	
	public:
		void MadConvEncDialogClose(wxCloseEvent& event);
		void CreateGUIControls(void);
};

extern MadConvEncDialog *g_ConvEncDialog;

#endif
