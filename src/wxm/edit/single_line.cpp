///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/single_line.cpp
// Description: Single Line wxMEdit Control
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "single_line.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

void SingleLineWXMEdit::UpdateScrollBarPos()
{
	ResetTextModeScrollBarPos();
}

template <typename _CHAR>
bool AdjustCharCount(const _CHAR* cs, size_t& count, wxFileOffset filesize, long maxlen)
{
	long cnt = 0;
	const wxFileOffset CHAR_BYTES = 4;

	while (cnt < long(count) && *cs != 0x0D && *cs != 0x0A)
	{
		++cnt;
		++cs;
	}

	if (filesize + cnt*CHAR_BYTES > maxlen)
		cnt = long( (maxlen - filesize) / CHAR_BYTES );

	if (cnt <= 0)
		return false;

	count = size_t(cnt);
	return true;
}

bool SingleLineWXMEdit::AdjustInsertCount(const ucs4_t *ucs, size_t& count)
{
	return AdjustCharCount(ucs, count, GetFileSize(), MaxLineLength());
}

bool SingleLineWXMEdit::AdjustStringLength(const wxString& ws, size_t& size)
{
	return AdjustCharCount(ws.wc_str(), size, GetFileSize(), MaxLineLength());
}

bool SingleLineWXMEdit::NeedNotProcessMouseLeftDown(wxMouseEvent &evt)
{
	if (evt.m_x != m_LeftClickX || evt.m_y != m_LeftClickY)
		return false;

	m_LeftClickX = m_LeftClickY = INT_MIN;
	evt.Skip();
	return true;
}

void SingleLineWXMEdit::OnGetFocusByClickAsControl(const wxMouseEvent &evt)
{
	if (FindFocus() == this)
		return;

	m_LeftClickX = evt.m_x;
	m_LeftClickY = evt.m_y;
}

void SingleLineWXMEdit::OnWXMEditSetFocus()
{
	SelectAll();
}

void SingleLineWXMEdit::OnWXMEditKillFocus()
{
	if (!IsSelected())
		return;

	Deselect();
	SetRepaintAll(true);
	Refresh();
}

void SingleLineWXMEdit::SetClientSizeData(int w, int h)
{
	SetClientAreaSize(w, h);
}

SingleLineWXMEdit::SingleLineWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: SimpleTextWXMEdit(parent, id, pos, size, style), m_LeftClickX(INT_MIN), m_LeftClickY(INT_MIN)
{
	SetMarkActiveLine(false);

	HideScrollBars();

	SetEncoding(wxT("UTF-32LE"));
	SetRecordCaretMovements(false);
	SetInsertSpacesInsteadOfTab(false);
	SetWantTab(false);
	LoadDefaultSyntaxScheme();

	wxSizeEvent evt;
	OnSize(evt);
}

} //namespace wxm
