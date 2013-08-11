///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding_singlebyte.h
// Description: define the Single-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_SINGLEBYTE_H_
#define _WXM_ENCODING_SINGLEBYTE_H_

#include "wxm_encoding_multibyte.h"

#include <boost/array.hpp>

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

	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
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

struct WXMEncodingSingleByteISO646Compatible: public WXMEncodingSingleByte
{
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len)
	{
		return m_dec.IsUChar32_LineFeed(buf, len);
	}
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
	{
		return m_dec.IsUChar32_LineFeed(dumper, len);
	}
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
	{
		return m_dec.PeekUChar32_Newline(dumper, len);
	}

private:
	WXMEncodingDecoderISO646 m_dec;
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
};

};// namespace wxm

#endif // _WXM_ENCODING_SINGLEBYTE_H_
