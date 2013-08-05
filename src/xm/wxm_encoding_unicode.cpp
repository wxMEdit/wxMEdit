///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.cpp
// Description: define the Unicode Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding_unicode.h"

namespace wxm
{

size_t WXMEncodingUTF8::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	/***  from rfc3629
	  Char. number range  |        UTF-8 octet sequence
	  (hexadecimal)       |              (binary)
	  --------------------+---------------------------------------------
	  0000 0000-0000 007F | 0xxxxxxx
	  0000 0080-0000 07FF | 110xxxxx 10xxxxxx
	  0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
	  0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

	  UTF8-octets = *( UTF8-char )
	  UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
	  UTF8-1      = %x00-7F
	  UTF8-2      = %xC2-DF UTF8-tail
	  UTF8-3      = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
	  %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
	  UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
	  %xF4 %x80-8F 2( UTF8-tail )
	  UTF8-tail   = %x80-BF
	 ***/

	if(ucs4<0)
		return 0;

	if(ucs4<=0x7F)
	{
		buf[0]=ucs4;
		return 1;
	}
	if(ucs4<=0x7FF)
	{
		buf[0]= 0xC0 | (ucs4>>6);
		buf[1]= 0x80 | (ucs4&0x3F);
		return 2;
	}
	if(ucs4<=0xFFFF)
	{
		buf[0]= 0xE0 | (ucs4>>12);
		buf[1]= 0x80 | ((ucs4>>6)&0x3F);
		buf[2]= 0x80 | (ucs4&0x3F);
		return 3;
	}
	if(ucs4<=0x10FFFF)
	{
		buf[0]= 0xF0 | (ucs4>>18);
		buf[1]= 0x80 | ((ucs4>>12)&0x3F);
		buf[2]= 0x80 | ((ucs4>>6)&0x3F);
		buf[3]= 0x80 | (ucs4&0x3F);
		return 4;
	}

	return 0;
}

size_t WXMEncodingUTF16LE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if(ucs4>=0x10000)// to unicode surrogates
	{
		if(ucs4>0x10FFFF) return 0;
		return UCS4toUTF16LE_U10000(ucs4, buf);
	}

	buf[0]=ucs4;
	buf[1]=ucs4>>8;

	return 2;
}
size_t WXMEncodingUTF16BE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if(ucs4>=0x10000)// to unicode surrogates
	{
		if(ucs4>0x10FFFF) return 0;

		ucs4-=0x10000;
		ucs2_t high=(ucs4>>10)+0xD800;    // high surrogate
		ucs2_t low=(ucs4&0x3FF)+0xDC00;    // low surrogate

		buf[0]=high>>8;
		buf[1]=high;
		buf[2]=low>>8;
		buf[3]=low;

		return 4;
	}

	buf[0]=ucs4>>8;
	buf[1]=ucs4;

	return 2;
}

size_t WXMEncodingUTF32LE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	wxASSERT(ucs4>=0 && ucs4<=0x10FFFF);

#if wxBYTE_ORDER == wxBIG_ENDIAN
	wxByte *p = (wxByte*)&ucs4;
	buf[0]=p[3];
	buf[1]=p[2];
	buf[2]=p[1];
	buf[3]=p[0];
#else
	*((ucs4_t*)buf) = ucs4;
#endif
	return 4;
}
size_t WXMEncodingUTF32BE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	wxASSERT(ucs4>=0 && ucs4<=0x10FFFF);

#if wxBYTE_ORDER == wxBIG_ENDIAN
	*((ucs4_t*)buf) = ucs4;
#else
	wxByte *p = (wxByte*)&ucs4;
	buf[0]=p[3];
	buf[1]=p[2];
	buf[2]=p[1];
	buf[3]=p[0];
#endif
	return 4;
}

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf)
{
	//ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
	//if(ucs4>0x10FFFF) return 0;

	//wxASSERT(ucs4>=0x10000 && ucs4<=0x10FFFF);

	ucs4-=0x10000;
	ucs2_t high=(ucs4>>10)+0xD800;    // high surrogate
	ucs2_t low=(ucs4&0x3FF)+0xDC00;    // low surrogate

	buf[0]=high;
	buf[1]=high>>8;
	buf[2]=low;
	buf[3]=low>>8;

	return 4;
}

};// namespace wxm
