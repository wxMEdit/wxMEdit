///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_statusbar.h
// Description: Status Bar Functions
// Copyright:   2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_STATUSBAR_H_
#define _WXM_STATUSBAR_H_

#include <wx/statusbr.h>

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

	WXMStatusBar() : m_frame(NULL), m_statusbar(NULL) {}
private:
	MadEditFrame* m_frame;
	wxStatusBar* m_statusbar;
};

WXMStatusBar& GetFrameStatusBar();

} // namespace wxm

#endif //_WXM_STATUSBAR_H_
