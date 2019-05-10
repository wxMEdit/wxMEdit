///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/multibyte.cpp
// Description: Define the Multi-byte Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "multibyte.h"
#include "../cxx11.h"

#include <boost/scoped_array.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

extern "C"
{
	void error_callback(const void *context, UConverterFromUnicodeArgs *args, const UChar *codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode *pErrorCode)
	{
		*pErrorCode = U_INVALID_CHAR_FOUND;
	}
}

ICUConverter::ICUConverter(const std::string& encname): m_ucnv(nullptr)
{
	UErrorCode err = U_ZERO_ERROR;

	m_ucnv = ucnv_open(encname.c_str(), &err);

	ucnv_setFallback(m_ucnv, FALSE);
	ucnv_setFromUCallBack(m_ucnv, error_callback, nullptr, nullptr, nullptr, &err);
}

ICUConverter::~ICUConverter()
{
	ucnv_close(m_ucnv);
	m_ucnv = nullptr;
}

size_t ICUConverter::MB2WC(UChar32& ch, const char* src, size_t src_len)
{
	UChar dest[2];
	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_toUChars(m_ucnv, dest, 2, src, (int32_t)src_len, &err);

	if (n == 1)
	{
		if (dest[0]==0xFFFD)
			return 0;

		ch = dest[0];
		return 1;
	}

	if (n<1 || (n>=2 && (dest[0]<0xD800 || dest[0]>0xDBFF || dest[1]<0xDC00 || dest[1]>0xDFFF)))
		return 0;

	ch = 0x10000 + (((dest[0] & 0x03FF) << 10) | (dest[1] & 0x03FF));

	return 1;
}

size_t ICUConverter::WC2MB(char* dest, size_t dest_len, const UChar32& ch)
{
	UChar src[2] = {0, 0};
	size_t src_len = 1;
	if (ch <= 0xFFFF)
	{
		src[0] = ch;
	}
	else
	{
		UChar32 tmp = ch - 0x10000;
		src[0] = (tmp >> 10) | 0xD800;
		src[1] = (tmp & 0x3FF) | 0xDC00;
	}

	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_fromUChars(m_ucnv, dest, (int32_t)dest_len, src, (int32_t)src_len, &err);

	if (n<=0)
		return 0;

	return ((size_t)n>dest_len)? dest_len: (size_t)n;
}

void MultiByteEncoding::Create(ssize_t idx)
{
	Encoding::Create(idx);

	MultiByteInit();
}

};// namespace xm
