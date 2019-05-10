///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/status_bar.h
// Description: Status Bar Functions
// Copyright:   2016-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_STATUS_BAR_H_
#define _WXM_STATUS_BAR_H_

#include "../xm/cxx11.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/statusbr.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class MadEditFrame;
namespace wxm
{

enum WXMSTB_FIELDS { STBF_HELP = 0, STBF_ROWCOL, STBF_CHARPOS, STBF_SELECTION, STBF_ENCFMT, STBF_READONLY, STBF_INSOVR };
struct WXMStatusBar
{
	void Init(MadEditFrame* frame, wxWindowID id);
	void Resize();
	void Attach();
	void ConnectSizeEvent(wxObjectEventFunction func);

	void Update()
	{
		m_statusbar->Update();
	}

	void SetField(WXMSTB_FIELDS field, const wxString& s)
	{
		m_statusbar->SetStatusText(s, (int)field);
	}

	WXMStatusBar() : m_frame(nullptr), m_statusbar(nullptr) {}
private:
	MadEditFrame* m_frame;
	wxStatusBar* m_statusbar;
};

WXMStatusBar& GetFrameStatusBar();

} // namespace wxm

#endif //_WXM_STATUS_BAR_H_
