///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.cpp
// Description: define the Multi-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding_multibyte.h"
#include <wx/config.h>
#include <boost/scoped_array.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

extern "C"
{
	void error_callback(const void *context, UConverterFromUnicodeArgs *args, const UChar *codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode *pErrorCode)
	{
		*pErrorCode = U_INVALID_CHAR_FOUND;
	}
}

ICUConverter::ICUConverter(const std::string& encname): m_ucnv(NULL)
{
	UErrorCode err = U_ZERO_ERROR;

	m_ucnv = ucnv_open(encname.c_str(), &err);

	ucnv_setFallback(m_ucnv, FALSE);
	ucnv_setFromUCallBack(m_ucnv, error_callback, NULL, NULL, NULL, &err);
}

ICUConverter::~ICUConverter()
{
	ucnv_close(m_ucnv);
	m_ucnv = NULL;
}

size_t ICUConverter::MB2WC(UChar32& ch, const char* src, size_t src_len)
{
	UChar dest[2];
	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_toUChars(m_ucnv, dest, 2, src, src_len, &err);

	if (n == 1)
	{
		if (dest[0]==0xFFFD)
			return 0;

		ch = dest[0];
		return 1;
	}

	if (n<1 || n>=2 && (dest[0]<0xD800 || dest[0]>0xDBFF || dest[1]<0xDC00 || dest[1]>0xDFFF))
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
		src[0] = (ch >> 10) | 0xD800;
		src[1] = (ch & 0x3FF) | 0xDC00;
	}

	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_fromUChars(m_ucnv, dest, dest_len, src, src_len, &err);

	if (n<=0)
		return 0;

	return ((size_t)n>dest_len)? dest_len: (size_t)n;
}

WXConverter::WXConverter(const std::string& encname, wxFontEncoding enc)
{
#if defined(__WXGTK__)
	wxString wxencname = wxString(encname.c_str(), wxConvUTF8);
	m_wxcnv=new wxCSConv(wxencname.c_str());
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
	m_wxcnv=new wxCSConv(enc);
#endif
}

size_t WXConverter::MB2WC(UChar32& ch, const char* src, size_t src_len)
{
	boost::scoped_array<wchar_t> wc(new wchar_t[src_len+1]);

	boost::scoped_array<char> arr(new char[src_len + 1]);
	strncpy(arr.get(), src, src_len);
	arr[src_len] = '\0';

	size_t n = m_wxcnv->MB2WC(wc.get(), arr.get(), src_len+1);
	ch = wc[0]; // FIXME: no non-BMP support on Windows
	return n;
}

size_t WXConverter::WC2MB(char* dest, size_t dest_len, const UChar32& ch)
{
	if (ch > 0x10000) // FIXME: no non-BMP support (but only for buggy EUC-JP now)
		return 0;

	wchar_t wc[2]={ch, 0};
	size_t len = m_wxcnv->WC2MB(dest, wc, dest_len);
	return (len==(size_t)-1)? 0: len;
}

void WXMEncodingMultiByte::Create(ssize_t idx)
{
	WXMEncoding::Create(idx);

	MultiByteInit();
}

};// namespace wxm
