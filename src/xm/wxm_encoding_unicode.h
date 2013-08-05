///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.h
// Description: define the Unicode Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_UNICODE_H_
#define _WXM_ENCODING_UNICODE_H_

#include "wxm_encoding.h"

#include <unicode/ucnv.h>
#include <boost/array.hpp>

namespace wxm
{

struct WXMEncodingUTF8: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF8(){}
	~WXMEncodingUTF8(){}
};

struct WXMEncodingUTF16LE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16LE(){}
	~WXMEncodingUTF16LE(){}
};

struct WXMEncodingUTF16BE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16BE(){}
	~WXMEncodingUTF16BE(){}
};

struct WXMEncodingUTF32LE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32LE(){}
	~WXMEncodingUTF32LE(){}
};

struct WXMEncodingUTF32BE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32BE(){}
	~WXMEncodingUTF32BE(){}
};

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf);

};// namespace wxm

#endif // _WXM_ENCODING_UNICODE_H_
