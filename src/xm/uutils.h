///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_uutils.h
// Description: Unicode Utilities
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UUTILS_H_
#define _XM_UUTILS_H_

#include "cxx11.h"

#include <unicode/unistr.h>
#include <unicode/uchar.h>
#include <string>
#include <stddef.h>

namespace xm
{

inline bool IsAmbiguousWidth(UChar32 ch)
{
	return u_getIntPropertyValue(ch, UCHAR_EAST_ASIAN_WIDTH) == U_EA_AMBIGUOUS;
}

inline bool IsWideWidthEastAsian(UChar32 ch)
{
	int x = u_getIntPropertyValue(ch, UCHAR_EAST_ASIAN_WIDTH);
	return (x==U_EA_FULLWIDTH || x==U_EA_WIDE || x==U_EA_AMBIGUOUS);
}

inline bool IsWideWidthEverywhere(UChar32 ch)
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
	virtual size_t GetWordCount() override;
	virtual size_t GetWordCountNoCtrlNoSP() override;

	virtual void operator+=(UChar32 ch) override
	{
		m_ustr += ch;
	}
	virtual void operator+=(const UnicodeString& ustr) override
	{
		m_ustr += ustr;
	}

	virtual ~SimpleWordCounter(){}
private:
	UnicodeString m_ustr;
};

struct AccumulativeWordCounter: public WordCounter
{
	virtual void operator+=(UChar32 ch) override;
	virtual void operator+=(const UnicodeString& ustr) override;
	virtual size_t GetWordCount() override;
	virtual size_t GetWordCountNoCtrlNoSP() override;

	AccumulativeWordCounter(int32_t capacity)
		: m_capacity(capacity), m_size(0)
		, m_cnt(0), m_ctrl_cnt(0), m_sp_cnt(0)
	{
	}
private:
	void PiecewiseCount(UChar32 ch);
	void PiecewiseClear();


	UnicodeString m_ustr;
	int32_t m_capacity;
	int32_t m_size;
	size_t m_cnt;
	size_t m_ctrl_cnt;
	size_t m_sp_cnt;
};


void NonBMPtoUTF16(UChar32 ch, UChar* buf);
size_t NonBMPtoUTF16LE(UChar32 ch, uint8_t* buf);
size_t NonBMPtoUTF16BE(UChar32 ch, uint8_t* buf);

inline uint16_t UIntSwap(uint16_t val)
{
	return ((val & 0x00FFu) << 8) | ((val & 0xFF00u) >> 8);
}
inline uint32_t UIntSwap(uint32_t val)
{
	return ((val & 0x000000FFu) << 24) | ((val & 0x0000FF00u) <<  8) |
		   ((val & 0x00FF0000u) >>  8) | ((val & 0xFF000000u) >> 24);
}

#if U_IS_BIG_ENDIAN == 0
// little endian
template <typename U>
inline U ToLE(U u) { return u; }
template <typename U>
inline U ToBE(U u) { return UIntSwap(u); }
#else
// big endian
template <typename U>
inline U ToLE(U u) { return UIntSwap(u); }
template <typename U>
inline U ToBE(U u) { return u; }
#endif

}; // namespace xm
#endif //_XM_UUTILS_H_
