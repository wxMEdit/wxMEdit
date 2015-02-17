///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/singlebyte.h
// Description: Define the Single-byte Encodings Supported by wxMEdit
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_SINGLEBYTE_H_
#define _WXM_ENCODING_SINGLEBYTE_H_

#include "../../xm/cxx11.h"
#include "multibyte.h"

#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
{

typedef boost::array<ucs4_t, 256> ByteUnicodeArr;
typedef std::map<ucs4_t, wxByte> UnicodeByteMap;

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

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
	virtual void MultiByteInit() override;
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf) override;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) override;
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper) override;

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
	WXMEncodingSingleByte(): m_icucnv(nullptr)
	{
	}
	~WXMEncodingSingleByte()
	{
		delete m_icucnv; m_icucnv = nullptr;
	}

	virtual SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer();
};

struct WXMEncodingSingleByteISO646Compatible: public WXMEncodingSingleByte, public WXMEncodingDecoderISO646
{
private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingSingleByteISO646Compatible(): WXMEncodingSingleByte(), WXMEncodingDecoderISO646() {}
	~WXMEncodingSingleByteISO646Compatible(){}
};

struct WXMEncodingSingleByteNonISO646Compatible: public WXMEncodingSingleByte
{
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len) override
	{
		return MultiBytetoUCS4(buf) == (ucs4_t)0x00000A;
	}
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len) override
	{
		wxByte b;
		dumper.Dump(&b, 1);
		return MultiBytetoUCS4(&b) == (ucs4_t)0x00000A;
	}
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len) override
	{
		wxByte b;
		dumper.Dump(&b, 1);
		ucs4_t u = MultiBytetoUCS4(&b);
		if (u == (ucs4_t)0x00000A || u == (ucs4_t)0x00000D)
			return u;
		return 0;
	}

protected:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingSingleByteNonISO646Compatible(): WXMEncodingSingleByte() {}
	~WXMEncodingSingleByteNonISO646Compatible() {}
};

struct WXMEncodingCP437Art: public WXMEncodingSingleByteNonISO646Compatible
{
	virtual SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer() override
	{
		return new CP437ArtTableFixer();
	}
private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingCP437Art(): WXMEncodingSingleByteNonISO646Compatible() {}
	~WXMEncodingCP437Art() {}
};

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_SINGLEBYTE_H_
