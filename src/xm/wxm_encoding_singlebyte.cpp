///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.cpp
// Description: define the Single-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding_singlebyte.h"
#include <boost/scoped_ptr.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

void OEMTableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	toutab[0x1A] = 0x00001A;
	toutab[0x1C] = 0x00001C;
	toutab[0x7f] = 0x00007f;

	fromutab[0x00001A] = 0x1A;
	fromutab[0x00001C] = 0x1C;
	fromutab[0x00007F] = 0x7F;
}

void CP437TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	OEMTableFixer::fix(toutab, fromutab);

	toutab[0xE6] = 0x0000B5;

	fromutab.erase(0x0003BC);
	fromutab[0x0000B5] = 0xE6;
}

void CP852TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	OEMTableFixer::fix(toutab, fromutab);

	toutab[0xAA] = 0x0000AC;

	fromutab.erase(0x0000AA);
	fromutab[0x0000AC] = 0xAA;
}

void Windows874TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	toutab[0xDB] = 0x0000DB;
	toutab[0xDC] = 0x0000DC;
	toutab[0xDD] = 0x0000DD;
	toutab[0xDE] = 0x0000DE;
	toutab[0xFC] = 0x0000FC;
	toutab[0xFD] = 0x0000FD;
	toutab[0xFE] = 0x0000FE;
	toutab[0xFF] = 0x0000FF;

	fromutab[0x0000DB] = 0xDB;
	fromutab[0x0000DC] = 0xDC;
	fromutab[0x0000DD] = 0xDD;
	fromutab[0x0000DE] = 0xDE;
	fromutab[0x0000FC] = 0xFC;
	fromutab[0x0000FD] = 0xFD;
	fromutab[0x0000FE] = 0xFE;
	fromutab[0x0000FF] = 0xFF;
}

SingleByteEncodingTableFixer* WXMEncodingSingleByte::CreateSingleByteEncodingTableFixer()
{
	if (m_name == wxT("CP437"))
		return new CP437TableFixer();
	if (m_name == wxT("CP850") || m_name == wxT("CP855") || m_name == wxT("CP866"))
		return new OEMTableFixer();
	if (m_name == wxT("CP852"))
		return new CP852TableFixer();
	if (m_name == wxT("Windows-874"))
		return new Windows874TableFixer();
	return new SingleByteEncodingTableFixer();
}

void WXMEncodingSingleByte::MultiByteInit()
{
	m_icucnv = new ICUConverter(m_innername);

	boost::scoped_ptr<SingleByteEncodingTableFixer>enc_fix(CreateSingleByteEncodingTableFixer());

	char singlebyte[1];
	UChar32 ch;
	for (size_t i=0; i<256; ++i)
	{
		singlebyte[0] = i;
		if (m_icucnv->MB2WC(ch, singlebyte, 1) == 1)
		{
			m_tounicode[i] = ch;
		}
		else
		{
			m_tounicode[i] = i;
			m_fromunicode[i] = i;
		}
	}

	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		if (i>=0xD800 && i<=0xF8FF) // skip PUA and surrogates
			continue;

		ch = i;
		if (m_icucnv->WC2MB(singlebyte, 1, ch) == 1)
		{
			m_fromunicode[i] = singlebyte[0];
		}
	}

	enc_fix->fix(m_tounicode, m_fromunicode);
}

ucs4_t WXMEncodingSingleByte::MultiBytetoUCS4(wxByte* buf)
{
	return m_tounicode[*buf];
}

size_t WXMEncodingSingleByte::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	UnicodeByteMap::const_iterator it = m_fromunicode.find(ucs4);
	if (it == m_fromunicode.end() || it->second == 0)
		return 0;

	buf[0] = it->second;
	return 1;
}

};// namespace wxm
