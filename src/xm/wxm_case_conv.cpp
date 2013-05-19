///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_wx_icu.cpp
// Description: Case conversion for wxWidgets objects
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_case_conv.h"
#include <boost/scoped_array.hpp>
#include <boost/static_assert.hpp>
#include <limits>

namespace wxm
{

wxString WxStrToLower(const wxString& wxs)
{
	UnicodeString us = WxStrToICU(wxs);
	us.toLower();

	wxString tmp = ICUStrToWx(us);
	if (tmp.IsEmpty())
		return wxs;
	return tmp;
}

wxString WxStrToUpper(const wxString& wxs)
{
	UnicodeString us = WxStrToICU(wxs);
	us.toUpper();

	wxString tmp = ICUStrToWx(us);
	if (tmp.IsEmpty())
		return wxs;
	return tmp;
}

void WxCaseConverter::Convert()
{
	while(!Finished())
	{
		UChar32 c = PeekChar();
		UChar32 nc = c;
		nc = ConvertCase(c);
		if(nc != c)
		{
			SaveChar(nc);
			m_modified = true;
		}
		NextChar();
	}
}

#ifdef __WXMSW__
	BOOST_STATIC_ASSERT(sizeof(wxChar) == sizeof(UChar));
#else
	BOOST_STATIC_ASSERT(sizeof(wxChar) == sizeof(UChar32));
#endif

UChar32 WxCaseConverter::PeekChar()
{
#ifndef __WXMSW__
	return m_txt[m_idx];
#else
	UChar32 ch = m_txt[m_idx];
	if (ch < 0xD800 || ch > 0xDBFF)
		return ch;

	if (m_idx+1 >= m_txt.Length() || m_txt[m_idx+1] < 0xDC00 ||  m_txt[m_idx+1] > 0xDFFF)
		return ch; // should'n t be here

	m_in_surrogate = true;
	return 0x10000 + ( ((ch & 0x3FF) << 10) | (m_txt[m_idx+1] & 0x3FF) );
#endif
}

void WxCaseConverter::NextChar()
{
	++m_idx;
#ifdef __WXMSW__
	if (m_in_surrogate)
	{
		m_in_surrogate = false;
		++m_idx;
	}
#endif
}

void WxCaseConverter::SaveChar(UChar32 ch)
{
#ifndef __WXMSW__
	m_txt[m_idx] = ch;
#else
	if (!m_in_surrogate)
	{
		m_txt[m_idx] = ch;
		return;
	}

	UChar32 tmp = ch - 0x10000;
	m_txt[m_idx] = 0xD800 | ((tmp >> 10) & 0x3FF);
	m_txt[m_idx+1] = 0xDC00 | (tmp & 0x3FF);
#endif
}

} // namespace wxm
