///////////////////////////////////////////////////////////////////////////////
// Name:        dialog/wxm_word_count_dialog.h
// Description: WordCount Dialog
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_WORD_COUNT_DIALOG_H
#define WXM_WORD_COUNT_DIALOG_H

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
//(*Headers(WXMWordCountDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class WXMWordCountDialog: public wxDialog
{
	public:

		WXMWordCountDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMWordCountDialog();

		//(*Declarations(WXMWordCountDialog)
		wxButton* Button1;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText10;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* WxStaticTextByteCount;
		wxStaticText* WxStaticTextCharCountAll;
		wxStaticText* WxStaticTextCharCountNoSPNoCtrl;
		wxStaticText* WxStaticTextControlCount;
		wxStaticText* WxStaticTextFullwidthCount;
		wxStaticText* WxStaticTextLineCount;
		wxStaticText* WxStaticTextPureFullwidthCount;
		wxStaticText* WxStaticTextSpaceCount;
		wxStaticText* WxStaticTextWordCount;
		wxTextCtrl* WxMemo1;
		//*)

	protected:

		//(*Identifiers(WXMWordCountDialog)
		static const long ID_STATICTEXT8;
		static const long ID_WXSTATICTEXTLINECOUNT;
		static const long ID_STATICTEXT1;
		static const long ID_WXSTATICTEXTWORDCOUNT;
		static const long ID_STATICTEXT9;
		static const long ID_WXSTATICTEXTBYTECOUNT;
		static const long ID_STATICTEXT3;
		static const long ID_WXSTATICTEXTCHARCOUNTALL;
		static const long ID_STATICTEXT2;
		static const long ID_WXSTATICTEXTCHARCOUNTNOSPNOCTRL;
		static const long ID_STATICTEXT10;
		static const long ID_WXSTATICTEXTSPACECOUNT;
		static const long ID_STATICTEXT4;
		static const long ID_WXSTATICTEXTCONTROLCOUNT;
		static const long ID_STATICTEXT5;
		static const long ID_WXSTATICTEXTFULLWIDTHCOUNT;
		static const long ID_STATICTEXT6;
		static const long ID_WXSTATICTEXTPUREFULLWIDTHCOUNT;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT7;
		static const long ID_WXMEMO1;
		//*)

	private:

		//(*Handlers(WXMWordCountDialog)
		void WXMWordCountDialogClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
