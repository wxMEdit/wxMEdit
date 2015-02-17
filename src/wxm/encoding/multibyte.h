///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/multibyte.h
// Description: Define the Multi-byte Encodings Supported by wxMEdit
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_MULTIBYTE_H_
#define _WXM_ENCODING_MULTIBYTE_H_

#include "../../xm/cxx11.h"
#include "encoding.h"

#include <unicode/ucnv.h>

namespace wxm
{

struct WXMEncodingMultiByte: public WXMEncoding
{
	virtual void MultiByteInit() = 0;
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf) override = 0;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) override = 0;
	virtual bool IsSingleByteEncoding() override = 0;
	virtual bool IsDoubleByteEncoding() override = 0;
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper) override = 0;
	virtual void Create(ssize_t idx) override;

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

	virtual size_t MB2WC(UChar32& ch, const char* src, size_t src_len) override;
	virtual size_t WC2MB(char* dest, size_t dest_len, const UChar32& ch) override;
private:
	UConverter* m_ucnv;
};

};// namespace wxm

#endif // _WXM_ENCODING_MULTIBYTE_H_
