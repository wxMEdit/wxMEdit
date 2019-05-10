///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/wx_icu.cpp
// Description: wxWidgets/ICU Type Adapter
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wx_icu.h"
#include <boost/scoped_array.hpp>
#include <boost/static_assert.hpp>
#include <limits>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

#ifdef __WXMSW__
	BOOST_STATIC_ASSERT(sizeof(wxChar) == sizeof(UChar));
#else
	BOOST_STATIC_ASSERT(sizeof(wxChar) == sizeof(UChar32));
#endif

UnicodeString WxStrToICU(const wxString& wxs)
{
#ifdef __WXMSW__
	return UnicodeString(wxs.wc_str(), (int32_t)wxs.Length());
#else
	return UnicodeString::fromUTF32((const UChar32*)wxs.wc_str(), wxs.Length());
#endif
}

wxString ICUStrToWx(const UnicodeString& us)
{
	int32_t cap = us.length() + 1;
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

} // namespace wxm
