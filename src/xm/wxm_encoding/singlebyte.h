///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/singlebyte.h
// Description: define the Single-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_SINGLEBYTE_H_
#define _WXM_ENCODING_SINGLEBYTE_H_

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
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab);
};

struct CP437TableFixer: public OEMTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab);
};

struct CP852TableFixer: public OEMTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab);
};

struct Windows874TableFixer: public SingleByteEncodingTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab);
};

struct ISO8859_16TableFixer: public SingleByteEncodingTableFixer
{
	virtual void fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab);
};

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
	virtual void MultiByteInit();
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf);
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);

	virtual bool IsSingleByteEncoding()
	{
		return true;
	}
	virtual bool IsDoubleByteEncoding()
	{
		return false;
	}

private:
	ICUConverter* m_icucnv;
	ByteUnicodeArr m_tounicode;
	UnicodeByteMap m_fromunicode;

protected:
	WXMEncodingSingleByte(): m_icucnv(NULL)
	{
	}
	~WXMEncodingSingleByte()
	{
		delete m_icucnv; m_icucnv = NULL;
	}

	SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer();
};

struct WXMEncodingSingleByteISO646Compatible: public WXMEncodingSingleByte, public WXMEncodingDecoderISO646
{
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingSingleByteISO646Compatible(): WXMEncodingSingleByte(), WXMEncodingDecoderISO646() {}
	~WXMEncodingSingleByteISO646Compatible(){}
};

struct WXMEncodingSingleByteNonISO646Compatible: public WXMEncodingSingleByte
{
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return MultiBytetoUCS4(buf) == (ucs4_t)0x00000A;
	}
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
	{
		wxByte b;
		dumper.Dump(&b, 1);
		return MultiBytetoUCS4(&b) == (ucs4_t)0x00000A;
	}
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
	{
		wxByte b;
		dumper.Dump(&b, 1);
		ucs4_t u = MultiBytetoUCS4(&b);
		if (u == (ucs4_t)0x00000A || u == (ucs4_t)0x00000D)
			return u;
		return 0;
	}

private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingSingleByteNonISO646Compatible(): WXMEncodingSingleByte() {}
	~WXMEncodingSingleByteNonISO646Compatible() {}
};

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_SINGLEBYTE_H_
