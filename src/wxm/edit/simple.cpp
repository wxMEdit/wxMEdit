///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/simple.cpp
// Description: Simple wxMEdit Implementations
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "simple.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

HexPrintingWXMEdit::HexPrintingWXMEdit(wxWindow* parent, const wxFont* font)
	: SimpleWXMEdit(parent, wxID_ANY, wxPoint(-1024, -1024))
{
	SetHexPrinting();
	SetTextFont(font->GetFaceName(), font->GetPointSize(), true);
	SetEncoding(wxT("UTF-32LE"));
	SetLineSpacing(100);
	SetShowEndOfLine(false);
	SetShowSpaceChar(false);
	SetShowTabChar(false);

	SetFixedWidthMode(true);

	BeginSyntaxPrint(false);

	m_LeftMarginWidth = 0;
	m_RightMarginWidth = 0;
}

} //namespace wxm
