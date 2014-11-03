///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/single_line_wxmedit.cpp
// Description: Single Line wxMEdit Control
// Author:      wxmedit@gmail.com
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "single_line_wxmedit.h"

namespace wxm
{

void SingleLineWXMEdit::UpdateScrollBarPos()
{
	ResetTextModeScrollBarPos();
}

bool SingleLineWXMEdit::AdjustInsertCount(const ucs4_t *ucs, size_t& count)
{
	const ucs4_t *ucs1 = ucs;
	int sss = 0;

	while (sss < int(count) && *ucs1 != 0x0D && *ucs1 != 0x0A)
	{
		++sss;
		++ucs1;
	}

	int maxlen = MaxLineLength() - 100;

	if (GetFileSize() + sss > maxlen)
		sss = maxlen - int(GetFileSize());

	if (sss <= 0)
		return false;

	count = size_t(sss);
	return true;
}

void SingleLineWXMEdit::AdjustStringLength(const wxString ws, size_t& size)
{
	const wxChar *wcs = ws.c_str();
	size_t sss = 0;

	while (sss < size && *wcs != 0x0D && *wcs != 0x0A)
	{
		++sss;
		++wcs;
	}

	long maxlen = MaxLineLength() - 100;
	if (wxFileOffset(GetFileSize() + sss) > maxlen)
		sss = maxlen - long(GetFileSize());

	size = sss;
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

void SingleLineWXMEdit::OnSetFocusAsControl()
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

void SingleLineWXMEdit::SingleLineWXMEditInit()
{
	SetStorePropertiesToGlobalConfig(false);
	SetDisplayLineNumber(false);
	//SetWordWrapMode(wwmNoWrap);
	//SetEditMode(emTextMode);
	SetMarkActiveLine(false);

	HideScrollBars();

	SetEncoding(wxT("UTF-32LE"));
	SetFixedWidthMode(false);
	SetRecordCaretMovements(false);
	SetInsertSpacesInsteadOfTab(false);
	SetWantTab(false);
	LoadDefaultSyntaxScheme();

	wxSizeEvent evt;
	OnSize(evt);
}

} //namespace wxm
