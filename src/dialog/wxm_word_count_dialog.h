///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_word_count_dialog.h
// Description: WordCount Dialog
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef WXM_WORD_COUNT_DIALOG_H
#define WXM_WORD_COUNT_DIALOG_H

//(*Headers(WXMWordCountDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class WXMWordCountDialog: public wxDialog
{
	public:

		WXMWordCountDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMWordCountDialog();

		//(*Declarations(WXMWordCountDialog)
		wxStaticText* StaticText10;
		wxStaticText* StaticText9;
		wxStaticText* WxStaticTextCharCountNoSPNoCtrl;
		wxStaticText* WxStaticTextPureFullwidthCount;
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxStaticText* StaticText6;
		wxStaticText* WxStaticTextByteCount;
		wxStaticText* StaticText8;
		wxStaticText* WxStaticTextCharCountAll;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticText* WxStaticTextWordCount;
		wxStaticText* WxStaticTextLineCount;
		wxStaticText* WxStaticTextFullwidthCount;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* WxStaticTextControlCount;
		wxStaticLine* StaticLine1;
		wxTextCtrl* WxMemo1;
		wxStaticText* WxStaticTextSpaceCount;
		wxStaticText* StaticText4;
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
