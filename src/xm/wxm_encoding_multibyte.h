///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding_multibyte.h
// Description: define the Multi-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_MULTIBYTE_H_
#define _WXM_ENCODING_MULTIBYTE_H_

#include "wxm_encoding.h"

#include <unicode/ucnv.h>

namespace wxm
{

struct WXMEncodingMultiByte: public WXMEncoding
{
	virtual void MultiByteInit() = 0;
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf) = 0;
	virtual void Create(ssize_t idx);

protected:
	WXMEncodingMultiByte(){}
	~WXMEncodingMultiByte(){}
};

struct MBConverter
{
	virtual size_t MB2WC(UChar32& ch, const char* src, size_t src_len) = 0;
	virtual size_t WC2MB(char* dest, size_t dest_len, const UChar32& ch) = 0;

	virtual ~MBConverter() {}
};

struct ICUConverter: public MBConverter
{
	ICUConverter(const std::string& encname);
	~ICUConverter();

	virtual size_t MB2WC(UChar32& ch, const char* src, size_t src_len);
	virtual size_t WC2MB(char* dest, size_t dest_len, const UChar32& ch);
private:
	UConverter* m_ucnv;
};

};// namespace wxm

#endif // _WXM_ENCODING_MULTIBYTE_H_
