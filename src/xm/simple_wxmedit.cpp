///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/simple_wxmedit.cpp
// Description: Simple wxMEdit Implementations
// Copyright:   2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "simple_wxmedit.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

HexPrintingWXMEdit::HexPrintingWXMEdit(wxWindow* parent, const wxFont* font)
	: InvisibleWXMEdit(parent)
{
	SetHexPrinting();
	SetEncoding(wxT("UTF-32LE"));
	SetLineSpacing(100);
	SetDisplayLineNumber(false);
	SetShowEndOfLine(false);
	SetShowSpaceChar(false);
	SetShowTabChar(false);

	SetFixedWidthMode(true);
	SetTextFont(font->GetFaceName(), font->GetPointSize(), true);
	SetWordWrapMode(wwmWrapByColumn);

	BeginSyntaxPrint(false);

	m_LineNumberAreaWidth = 0;
	m_LeftMarginWidth = 0;
	m_RightMarginWidth = 0;
}

} //namespace wxm
