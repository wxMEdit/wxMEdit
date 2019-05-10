///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/unicode.h
// Description: Define the Unicode Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_UNICODE_H_
#define _XM_ENCODING_UNICODE_H_

#include "../cxx11.h"
#include "encoding.h"

#include <unicode/ucnv.h>
#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace xm
{

struct UTF8Encoding: public Encoding, EncodingDecoderISO646
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	UTF8Encoding(){}
	~UTF8Encoding(){}
};

struct UTF16LE_Encoding: public Encoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override
	{
		return len>=2 && buf[0]==0x0A && buf[1]==0;
	}

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	UTF16LE_Encoding(){}
	~UTF16LE_Encoding(){}
};

struct UTF16BE_Encoding: public Encoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override
	{
		return len>=2 && buf[1]==0x0A && buf[0]==0;
	}

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	UTF16BE_Encoding(){}
	~UTF16BE_Encoding(){}
};

struct UTF32LE_Encoding: public Encoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override
	{
		return len>=4 && buf[0]==0x0A && buf[1]==0 && buf[2]==0 && buf[3]==0;
	}

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	UTF32LE_Encoding(){}
	~UTF32LE_Encoding(){}
};

struct UTF32BE_Encoding: public Encoding
{
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override;
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override
	{
		return len>=4 && buf[3]==0x0A && buf[2]==0 && buf[1]==0 && buf[0]==0;
	}

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	UTF32BE_Encoding(){}
	~UTF32BE_Encoding(){}
};

};// namespace xm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _XM_ENCODING_UNICODE_H_
