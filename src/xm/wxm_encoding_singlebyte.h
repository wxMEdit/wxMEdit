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

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
	virtual void MultiByteInit();
	virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	ICUConverter* m_icucnv;
	ByteUnicodeArr m_tounicode;
	UnicodeByteMap m_fromunicode;

	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingSingleByte(): m_icucnv(NULL)
	{
	}
	~WXMEncodingSingleByte()
	{
		delete m_icucnv; m_icucnv = NULL;
	}

	SingleByteEncodingTableFixer* CreateSingleByteEncodingTableFixer();
};

};// namespace wxm

#endif // _WXM_ENCODING_SINGLEBYTE_H_
