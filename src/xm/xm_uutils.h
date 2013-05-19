///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_uutils.h
// Description: Unicode Utilities
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UUTILS_H_
#define _XM_UUTILS_H_

#include <unicode/unistr.h>
#include <unicode/uchar.h>
#include <stddef.h>

namespace xm
{

size_t GetWordCount(const UnicodeString& ustr);
size_t GetWordCountNoCtrlNoSP(const UnicodeString& ustr);

inline bool IsWideWidthEastAsian(UChar32 ch)
{
	int x = u_getIntPropertyValue(ch, UCHAR_EAST_ASIAN_WIDTH);
	return (x==U_EA_FULLWIDTH || x==U_EA_WIDE || x==U_EA_AMBIGUOUS);
}

inline bool IsWideWidthOther(UChar32 ch)
{
	int x = u_getIntPropertyValue(ch, UCHAR_EAST_ASIAN_WIDTH);
	return (x==U_EA_FULLWIDTH || x==U_EA_WIDE);
}

}; // namespace xm
#endif //_XM_UUTILS_H_
