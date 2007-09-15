///////////////////////////////////////////////////////////////////////////////
// Name:        MadSortDialog.h
// Description: Sort Options Dialog
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADSORTDIALOG_H_
#define _MADSORTDIALOG_H_

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
#include <wx/checkbox.h>
#include <wx/radiobox.h>
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
#undef MadSortDialog_STYLE
#define MadSortDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class MadSortDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		MadSortDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Sort Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadSortDialog_STYLE);
		virtual ~MadSortDialog();
	
	public:
		//Do not add custom control declarations
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButtonCancel;
		wxButton *WxButtonSort;
		wxBoxSizer *WxBoxSizer2;
		wxCheckBox *WxCheckBoxRemoveDup;
		wxCheckBox *WxCheckBoxNumeric;
		wxCheckBox *WxCheckBoxCase;
		wxRadioBox *WxRadioBoxOrder;
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
			ID_WXCHECKBOXREMOVEDUP = 1007,
			ID_WXCHECKBOXNUMERIC = 1006,
			ID_WXCHECKBOXCASE = 1004,
			ID_WXRADIOBOXORDER = 1003,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		}; //End of Enum
	
	public:
		void MadSortDialogClose(wxCloseEvent& event);
		void CreateGUIControls(void);
};

#endif
