///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding_singlebyte.cpp
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

void ISO8859_16TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	toutab[0xA1] = 0x000104; fromutab[0x000104] = 0xA1; fromutab.erase(0x0000A1);
	toutab[0xA2] = 0x000105; fromutab[0x000105] = 0xA2; fromutab.erase(0x0000A2);
	toutab[0xA3] = 0x000141; fromutab[0x000141] = 0xA3; fromutab.erase(0x0000A3);
	toutab[0xA5] = 0x00201E; fromutab[0x00201E] = 0xA5; fromutab.erase(0x0000A5);
	toutab[0xAA] = 0x000218; fromutab[0x000218] = 0xAA; fromutab.erase(0x0000AA);
	toutab[0xAC] = 0x000179; fromutab[0x000179] = 0xAC; fromutab.erase(0x0000AC);
	toutab[0xAE] = 0x00017A; fromutab[0x00017A] = 0xAE; fromutab.erase(0x0000AE);
	toutab[0xAF] = 0x00017B; fromutab[0x00017B] = 0xAF; fromutab.erase(0x0000AF);

	toutab[0xB2] = 0x00010C; fromutab[0x00010C] = 0xB2; fromutab.erase(0x0000B2);
	toutab[0xB3] = 0x000142; fromutab[0x000142] = 0xB3; fromutab.erase(0x0000B3);
	toutab[0xB5] = 0x00201D; fromutab[0x00201D] = 0xB5; fromutab.erase(0x0000B5);
	toutab[0xB9] = 0x00010D; fromutab[0x00010D] = 0xB9; fromutab.erase(0x0000B9);
	toutab[0xBA] = 0x000219; fromutab[0x000219] = 0xBA; fromutab.erase(0x0000BA);
	toutab[0xBF] = 0x00017C; fromutab[0x00017C] = 0xBF; fromutab.erase(0x0000BF);

	toutab[0xC3] = 0x000102; fromutab[0x000102] = 0xC3; fromutab.erase(0x0000C3);
	toutab[0xC5] = 0x000106; fromutab[0x000106] = 0xC5; fromutab.erase(0x0000C5);

	toutab[0xD0] = 0x000110; fromutab[0x000110] = 0xD0; fromutab.erase(0x0000D0);
	toutab[0xD1] = 0x000143; fromutab[0x000143] = 0xD1; fromutab.erase(0x0000D1);
	toutab[0xD5] = 0x000150; fromutab[0x000150] = 0xD5; fromutab.erase(0x0000D5);
	toutab[0xD7] = 0x00015A; fromutab[0x00015A] = 0xD7; fromutab.erase(0x0000D7);
	toutab[0xD8] = 0x000170; fromutab[0x000170] = 0xD8; fromutab.erase(0x0000D8);
	toutab[0xDD] = 0x000118; fromutab[0x000118] = 0xDD; fromutab.erase(0x0000DD);
	toutab[0xDE] = 0x00021A; fromutab[0x00021A] = 0xDE; fromutab.erase(0x0000DE);

	toutab[0xE3] = 0x000103; fromutab[0x000103] = 0xE3; fromutab.erase(0x0000E3);
	toutab[0xE5] = 0x000107; fromutab[0x000107] = 0xE5; fromutab.erase(0x0000E5);

	toutab[0xF0] = 0x000111; fromutab[0x000111] = 0xF0; fromutab.erase(0x0000F0);
	toutab[0xF1] = 0x000144; fromutab[0x000144] = 0xF1; fromutab.erase(0x0000F1);
	toutab[0xF5] = 0x000151; fromutab[0x000151] = 0xF5; fromutab.erase(0x0000F5);
	toutab[0xF7] = 0x00015B; fromutab[0x00015B] = 0xF7; fromutab.erase(0x0000F7);
	toutab[0xF8] = 0x000171; fromutab[0x000171] = 0xF8; fromutab.erase(0x0000F8);
	toutab[0xFD] = 0x000119; fromutab[0x000119] = 0xFD; fromutab.erase(0x0000FD);
	toutab[0xFE] = 0x00021B; fromutab[0x00021B] = 0xFE; fromutab.erase(0x0000FE);
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
	if (m_name == wxT("ISO-8859-16"))
		return new ISO8859_16TableFixer();
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
