///////////////////////////////////////////////////////////////////////////////
// Name:        MadFileAssociationDialog.h
// Description: File Type Association Dialog for MSWindows
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADFILEASSOCIATIONDIALOG_H_
#define _MADFILEASSOCIATIONDIALOG_H_

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

#ifdef __WXMSW__

//Do not add custom headers between
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
////Header Include End

////Dialog Style Start
#undef MadFileAssociationDialog_STYLE
#define MadFileAssociationDialog_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMAXIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class MadFileAssociationDialog : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();

	public:
		MadFileAssociationDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("File Type Associations"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = MadFileAssociationDialog_STYLE);
		virtual ~MadFileAssociationDialog();
		void WxListBoxAssociatedDoubleClicked(wxCommandEvent& event);
		void WxListBoxPredefinedDoubleClicked(wxCommandEvent& event);
		void WxButtonAddClick(wxCommandEvent& event);
		void WxButtonRemoveClick(wxCommandEvent& event);
		void WxButtonAddCustomClick(wxCommandEvent& event);
		void WxButtonOKClick(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButtonCancel;
		wxButton *WxButtonOK;
		wxBoxSizer *WxBoxSizer3;
		wxButton *WxButtonAddCustom;
		wxTextCtrl *WxEditCustomType;
		wxBoxSizer *WxBoxSizer5;
		wxListBox *WxListBoxPredefined;
		wxStaticText *WxStaticText2;
		wxBoxSizer *WxBoxSizer8;
		wxButton *WxButtonRemove;
		wxButton *WxButtonAdd;
		wxBoxSizer *WxBoxSizer7;
		wxListBox *WxListBoxAssociated;
		wxStaticText *WxStaticText1;
		wxBoxSizer *WxBoxSizer6;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		////GUI Control Declaration End

	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTONOK = 1005,
			ID_WXBUTTONADDCUSTOM = 1020,
			ID_WXEDITCUSTOMTYPE = 1018,
			ID_WXLISTBOXPREDEFINED = 1015,
			ID_WXSTATICTEXT2 = 1014,
			ID_WXBUTTONREMOVE = 1017,
			ID_WXBUTTONADD = 1016,
			ID_WXLISTBOXASSOCIATED = 1013,
			ID_WXSTATICTEXT1 = 1012,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif //__WXMSW__

#endif
