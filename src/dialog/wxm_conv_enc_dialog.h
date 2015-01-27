///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_conv_enc_dialog.h
// Description: Encoding Conversion Dialog
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_CONV_ENC_DIALOG_H
#define WXM_CONV_ENC_DIALOG_H

#include "../xm/wx_avoid_wxmsw_bug4373.h"

//(*Headers(WXMConvEncDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
//*)


class WXMConvEncDialog: public wxDialogWrapper
{
	public:

		WXMConvEncDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMConvEncDialog();

		//(*Declarations(WXMConvEncDialog)
		wxRadioBox* WxRadioBoxOption;
		wxStaticText* StaticText1;
		wxComboBox* WxComboBoxEncoding;
		wxButton* WxButtonCancel;
		wxButton* WxButtonOK;
		//*)

		wxString GetEncoding() const;
	protected:

		//(*Identifiers(WXMConvEncDialog)
		static const long ID_STATICTEXT1;
		static const long ID_WXCOMBOBOXENCODING;
		static const long ID_WXRADIOBOXOPTION;
		//*)

	private:

		//(*Handlers(WXMConvEncDialog)
		void WXMConvEncDialogClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

extern WXMConvEncDialog *g_ConvEncDialog;

#endif
