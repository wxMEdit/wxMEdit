#ifndef WXM_PURGE_HISTORIES_DIALOG_H
#define WXM_PURGE_HISTORIES_DIALOG_H

//(*Headers(WXMPurgeHistoriesDialog)
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class WXMPurgeHistoriesDialog: public wxDialog
{
	public:

		WXMPurgeHistoriesDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WXMPurgeHistoriesDialog();

		//(*Declarations(WXMPurgeHistoriesDialog)
		wxButton* WxButtonPurge;
		wxCheckBox* WxCheckBoxRecentFindFilters;
		wxCheckBox* WxCheckBoxRecentFilesFindDirs;
		wxCheckBox* WxCheckBoxRecentFonts;
		wxButton* WxButtonCancel;
		wxCheckBox* WxCheckBoxRecentFindTexts;
		wxCheckBox* WxCheckBoxRecentReplaceTexts;
		wxCheckBox* WxCheckBoxRecentFindExcludes;
		wxCheckBox* WxCheckBoxRecentEncodings;
		wxCheckBox* WxCheckBoxRecentFiles;
		//*)

	protected:

		//(*Identifiers(WXMPurgeHistoriesDialog)
		static const long ID_CHECKBOXRECENTFILES;
		static const long ID_CHECKBOXRECENTFONTS;
		static const long ID_CHECKBOXRECENTENCODINGS;
		static const long ID_CHECKBOXRECENTFINDTEXTS;
		static const long ID_CHECKBOXRECENTREPLACETEXTS;
		static const long ID_CHECKBOXRECENTFINDDIRS;
		static const long ID_CHECKBOXRECENTFINDFILTERS;
		static const long ID_CHECKBOXRECENTFINDEXCLUDES;
		static const long ID_BUTTONPURGE;
		//*)

	private:

		//(*Handlers(WXMPurgeHistoriesDialog)
		void WXMPurgeHistoriesDialogClose(wxCloseEvent& event);
		void WxButtonPurgeClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
