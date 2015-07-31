///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_file_association_dialog.h
// Description: Find/Replace in Files Dialog
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2007-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_FILE_ASSOCIATION_DIALOG_H
#define WXM_FILE_ASSOCIATION_DIALOG_H

#include <wx/platform.h>
#ifdef __WXMSW__

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMFileAssociationDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class WXMFileAssociationDialog: public wxDialog
{
	public:

		WXMFileAssociationDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMFileAssociationDialog();

		//(*Declarations(WXMFileAssociationDialog)
		wxButton* WxButtonAddCustom;
		wxButton* WxButtonAdd;
		wxTextCtrl* WxEditCustomType;
		wxListBox* WxListBoxPredefined;
		wxStaticText* StaticText2;
		wxButton* WxButtonRemove;
		wxListBox* WxListBoxAssociated;
		wxStaticText* StaticText1;
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		//*)

	protected:

		//(*Identifiers(WXMFileAssociationDialog)
		static const long ID_STATICTEXT1;
		static const long ID_WXLISTBOXASSOCIATED;
		static const long ID_WXBUTTONADD;
		static const long ID_WXBUTTONREMOVE;
		static const long ID_STATICTEXT2;
		static const long ID_WXLISTBOXPREDEFINED;
		static const long ID_WXEDITCUSTOMTYPE;
		static const long ID_WXBUTTONADDCUSTOM;
		static const long ID_WXBUTTONOK;
		//*)

	private:

		//(*Handlers(WXMFileAssociationDialog)
		void WXMFileAssociationDialogClose(wxCloseEvent& event);
		void WxButtonAddClick(wxCommandEvent& event);
		void WxButtonRemoveClick(wxCommandEvent& event);
		void WxButtonAddCustomClick(wxCommandEvent& event);
		void WxButtonOKClick(wxCommandEvent& event);
		void WxListBoxAssociatedDoubleClicked(wxCommandEvent& event);
		void WxListBoxPredefinedDoubleClicked(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif //__WXMSW__

#endif
