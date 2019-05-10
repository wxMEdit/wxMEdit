///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/wx_icu.h
// Description: wxWidgets/ICU Type Adapter
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_WX_ICU_H_
#define _WXM_WX_ICU_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/string.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/unistr.h>
using U_ICU_NAMESPACE::UnicodeString;

namespace wxm
{
	UnicodeString WxStrToICU(const wxString& wxs);
	wxString ICUStrToWx(const UnicodeString& us);
} //namespace wxm

#endif //_WXM_WX_ICU_H_
