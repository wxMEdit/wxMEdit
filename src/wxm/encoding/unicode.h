///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/unicode.h
// Description: Define the Unicode Encodings Supported by wxMEdit
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_UNICODE_H_
#define _WXM_ENCODING_UNICODE_H_

#include "encoding.h"

#include <unicode/ucnv.h>
#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
{

struct WXMEncodingUTF8: public WXMEncoding, WXMEncodingDecoderISO646
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);

private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingUTF8(){}
	~WXMEncodingUTF8(){}
};

struct WXMEncodingUTF16LE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return len>=2 && buf[0]==0x0A && buf[1]==0;
	}

private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingUTF16LE(){}
	~WXMEncodingUTF16LE(){}
};

struct WXMEncodingUTF16BE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return len>=2 && buf[1]==0x0A && buf[0]==0;
	}

private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingUTF16BE(){}
	~WXMEncodingUTF16BE(){}
};

struct WXMEncodingUTF32LE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return len>=4 && buf[0]==0x0A && buf[1]==0 && buf[2]==0 && buf[3]==0;
	}

private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingUTF32LE(){}
	~WXMEncodingUTF32LE(){}
};

struct WXMEncodingUTF32BE: public WXMEncoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len);
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return len>=4 && buf[3]==0x0A && buf[2]==0 && buf[1]==0 && buf[0]==0;
	}

private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingUTF32BE(){}
	~WXMEncodingUTF32BE(){}
};

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf);

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_UNICODE_H_
