///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_wx_icu.cpp
// Description: wxWidgets/ICU type adapter
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_wx_icu.h"
#include <boost/scoped_array.hpp>
#include <limits>

namespace wxm
{

UnicodeString WxStrToICU(const wxString& wxs)
{
#ifdef __WXMSW__
	return UnicodeString(wxs.c_str(), wxs.Length());
#else
	return UnicodeString::fromUTF32((const UChar32*)wxs.c_str(), wxs.Length());
#endif
}

wxString ICUStrToWx(const UnicodeString& us)
{
	size_t cap = us.length() + 1;
	boost::scoped_array<wchar_t> wbuff(new wchar_t[cap]);
	UErrorCode errcode = U_ZERO_ERROR;

#ifdef __WXMSW__
	int32_t len = us.extract(wbuff.get(), cap, errcode);
#else
	int32_t len = us.toUTF32((UChar32 *)wbuff.get(), cap, errcode);
#endif

	if(errcode == U_ZERO_ERROR)
		return wxString((wxChar*)wbuff.get(), len);

	return wxString();
}

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
