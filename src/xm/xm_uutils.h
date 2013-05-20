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

struct WordCounter
{
	virtual void operator+=(UChar32 ch) = 0;
	virtual void operator+=(const UnicodeString& ustr) = 0;
	virtual size_t GetWordCount() = 0;
	virtual size_t GetWordCountNoCtrlNoSP() = 0;

	virtual ~WordCounter(){}
};

struct SimpleWordCounter: public WordCounter
{
	virtual size_t GetWordCount();
	virtual size_t GetWordCountNoCtrlNoSP();

	virtual void operator+=(UChar32 ch)
	{
		m_ustr += ch;
	}
	virtual void operator+=(const UnicodeString& ustr)
	{
		m_ustr += ustr;
	}

	virtual ~SimpleWordCounter(){}
private:
	UnicodeString m_ustr;
};

}; // namespace xm
#endif //_XM_UUTILS_H_
