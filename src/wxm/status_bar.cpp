///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/status_bar.cpp
// Description: Status Bar Functions
// Copyright:   2016-2023  JiaYanwei   <wxmedit@gmail.com>
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
	m_statusbar->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
}

void WXMStatusBar::Resize()
{
	// width-height ratios for fields
#ifdef __WXMSW__
	static const float W_H_RATIOS[7] = { 0.0f, 8.6f, 13.5f, 6.6f, 5.5f, 2.7f, 2.0f };
#else
	static const float W_H_RATIOS[7] = { 0.0f, 9.8f, 15.6f, 7.8f, 6.7f, 3.0f, 2.5f };
#endif

	int h = m_statusbar->GetClientSize().GetHeight();

#if defined(__WXMSW__) && wxMAJOR_VERSION > 2
	static const int MARGIN = 5;
#else
	static const int MARGIN = 1;
#endif

	// calculate status bar field widths
	int width_1to6 = MARGIN;
	int widths[7] = { 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 7; ++i)
	{
		int w = int(W_H_RATIOS[i] * h);
		widths[i] = w;
		width_1to6 += w + MARGIN*2;
	}
	width_1to6 += int(0.8f * h);

	int width_0 = m_statusbar->GetClientSize().GetWidth() - width_1to6;
	widths[0] = (width_0 < 0) ? 0 : width_0;

	m_statusbar->SetFieldsCount(7, widths);
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
