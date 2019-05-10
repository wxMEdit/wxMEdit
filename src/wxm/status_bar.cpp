///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/status_bar.cpp
// Description: Status Bar Functions
// Copyright:   2016-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "status_bar.h"
#include "../wxmedit_frame.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

void WXMStatusBar::Init(MadEditFrame* frame, wxWindowID id)
{
	m_frame = frame;
	m_statusbar = new wxStatusBar(m_frame, id);

#if defined(__WXGTK__)
	m_statusbar->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
#endif
}

void WXMStatusBar::Resize()
{
#ifdef __WXMSW__
	static int status_width_1_6 =     (220+ 305+ 135+ 155+ 65+ (50 + 10));
	static int status_widths[7] = { 0, 220, 305, 135, 155, 65, (50 + 10) };
#else
	static int status_width_1_6 =     (220+ 305+ 135+ 155+ 65+ (50 + 0));
	static int status_widths[7] = { 0, 220, 305, 135, 155, 65, (50 + 0) };
#endif

	// status bar field widths
	int width0 = m_frame->GetClientSize().GetWidth() - status_width_1_6;
	status_widths[0] = (width0 < 0) ? 0 : width0;
	m_statusbar->SetFieldsCount(7, status_widths);
}

void WXMStatusBar::Attach()
{
	m_frame->SetStatusBar(m_statusbar);
}

void WXMStatusBar::ConnectSizeEvent(wxObjectEventFunction func)
{
	m_statusbar->Connect(wxEVT_SIZE, func);
}

WXMStatusBar& GetFrameStatusBar()
{
	return g_MainFrame->m_wxmstatusbar;
}

} //namespace wxm
