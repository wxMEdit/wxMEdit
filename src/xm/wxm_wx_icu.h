///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_wx_icu.h
// Description: wxWidgets/ICU type adapter
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_WX_ICU_H_
#define _WXM_WX_ICU_H_

#include <wx/string.h>
#include <unicode/unistr.h>

namespace wxm
{
	UnicodeString WxStrToICU(const wxString& wxs);
	wxString ICUStrToWx(const UnicodeString& us);
} //namespace wxm

#endif //_WXM_WX_ICU_H_
