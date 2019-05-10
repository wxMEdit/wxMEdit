///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/singlebyte.h
// Description: Define the Single-byte Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_SINGLEBYTE_H_
#define _XM_ENCODING_SINGLEBYTE_H_

#include "../cxx11.h"
#include "multibyte.h"

#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace xm
{

typedef boost::array<ucs4_t, 256> ByteUnicodeArr;
typedef std::map<ucs4_t, ubyte> UnicodeByteMap;

struct SingleByteEncodingTableFixer
{
	virtual ~SingleByteEncodingTableFixer() {}
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) {}
};

struct OEMTableFixer: public SingleByteEncodingTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct CP437TableFixer: public OEMTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct CP437ArtTableFixer: public CP437TableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct CP852TableFixer: public OEMTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct Windows874TableFixer: public SingleByteEncodingTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct ISO8859_16TableFixer: public SingleByteEncodingTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab) override;
};

struct SingleByteEncoding: public MultiByteEncoding
{
	virtual void MultiByteInit() override;
	virtual ucs4_t MultiBytetoUCS4(const ubyte* buf) override;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;

	virtual bool IsSingleByteEncoding() override
	{
		return true;
	}
	virtual bool IsDoubleByteEncoding() override
	{
		return false;
	}

private:
	ICUConverter* m_icucnv;
	ByteUnicodeArr m_tounicode;
	UnicodeByteMap m_fromunicode;

protected:
	SingleByteEncoding(): m_icucnv(nullptr)
	{
	}
	~SingleByteEncoding()
	{
		delete m_icucnv; m_icucnv = nullptr;
	}

	virtual SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer();
};

struct SingleByteEncodingISO646Compatible: public SingleByteEncoding, public EncodingDecoderISO646
{
private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	SingleByteEncodingISO646Compatible(): SingleByteEncoding(), EncodingDecoderISO646() {}
	~SingleByteEncodingISO646Compatible(){}
};

struct SingleByteEncodingNonISO646Compatible: public SingleByteEncoding
{
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override
	{
		return MultiBytetoUCS4(buf) == (ucs4_t)0x00000A;
	}
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override
	{
		ubyte b;
		dumper.Dump(&b, 1);
		return MultiBytetoUCS4(&b) == (ucs4_t)0x00000A;
	}
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override
	{
		ubyte b;
		dumper.Dump(&b, 1);
		ucs4_t u = MultiBytetoUCS4(&b);
		if (u == (ucs4_t)0x00000A || u == (ucs4_t)0x00000D)
			return u;
		return 0;
	}

protected:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	SingleByteEncodingNonISO646Compatible(): SingleByteEncoding() {}
	~SingleByteEncodingNonISO646Compatible() {}
};

struct CP437ArtEncoding: public SingleByteEncodingNonISO646Compatible
{
	virtual SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer() override
	{
		return new CP437ArtTableFixer();
	}
private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	CP437ArtEncoding(): SingleByteEncodingNonISO646Compatible() {}
	~CP437ArtEncoding() {}
};

};// namespace xm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _XM_ENCODING_SINGLEBYTE_H_
