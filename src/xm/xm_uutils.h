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
#include <stddef.h>

namespace xm
{

size_t GetWordCount(const UnicodeString& ustr);
size_t GetWordCountNoCtrlNoSP(const UnicodeString& ustr);

}; // namespace xm
#endif //_XM_UUTILS_H_
