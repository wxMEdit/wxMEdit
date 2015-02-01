///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/unicode.cpp
// Description: Define the Unicode Encodings Supported by wxMEdit
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "unicode.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

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

bool WXMEncodingUTF8::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	int cnt= (rest<4)? rest: 4;

	if(buf[0]<=0x7F)
	{
		mapper.MoveUChar32Bytes(ucqueue, buf[0], 1);
		return true;
	}

	ucs4_t uc;
	if(buf[0]<=0xDF)
	{
		if(cnt>=2 && (buf[1] & 0xC0) == 0x80)     // valid 2 bytes
		{
			if((uc= ((ucs4_t(buf[0] & 0x1F)<<6) | (buf[1] & 0x3F) )) >= 0x80)
			{
				mapper.MoveUChar32Bytes(ucqueue, uc, 2);
				return true;
			}
		}
		mapper.MoveUChar32Bytes(ucqueue, buf[0], 1);
		return true;
	}

	if(buf[0]<=0xEF)
	{
		if(cnt>=3 && (buf[1] & 0xC0)==0x80 && (buf[2] & 0xC0)==0x80)     // valid 3 bytes
		{
			uc= (ucs4_t(buf[0] & 0x0F)<<12) | (ucs4_t(buf[1] & 0x3F) << 6) | (buf[2] & 0x3F);
			if(uc>=0x800 )//&& !(uc>=0xD800 && uc<=0xDFFF))
			{
				mapper.MoveUChar32Bytes(ucqueue, uc, 3);
				return true;
			}
		}
		mapper.MoveUChar32Bytes(ucqueue, buf[0], 1);
		return true;
	}

	if((buf[0]&0xF0) == 0xF0)
	{
		if(cnt>=4 && (buf[1] & 0xC0)==0x80 && (buf[2] & 0xC0)==0x80 && (buf[3] & 0xC0)==0x80)     // valid 4 bytes
		{
			uc= (ucs4_t(buf[0] & 0x07)<<18) | (ucs4_t(buf[1] & 0x3F) << 12) | (ucs4_t(buf[2] & 0x3F) << 6) | (buf[3] & 0x3F);
			if(uc>=0x10000 && uc<=0x10FFFF)
			{
				mapper.MoveUChar32Bytes(ucqueue, uc, 4);
				return true;
			}
		}
	}

	mapper.MoveUChar32Bytes(ucqueue, buf[0], 1);
	return true;
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
bool WXMEncodingUTF16LE::IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
{
	// odd m_Size is invalid
	if((len & 1) != 0)
		return false;

	wxByte buf[2];
	dumper.Dump(buf, 2);
	return buf[0]==0x0A && buf[1]==0;
}
ucs4_t WXMEncodingUTF16LE::PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
{
	// odd m_Size is invalid
	if((len & 1) != 0)
		return 0;

	wxByte buf[2];
	dumper.Dump(buf, 2);
	if ( (buf[0]==0x0A || buf[0]==0x0D) && buf[1]==0)
		return (ucs4_t)buf[0];
	return 0;
}
bool WXMEncodingUTF16LE::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	if(rest >= 2)
	{
		ucs4_t uc;
		uc=(ucs4_t(buf[1])<<8) | buf[0];

		//utf16 surrogates
		if(uc>=0xD800 && uc<=0xDBFF && rest >= 4)
		{
			buf+=2;
			ucs4_t uc1=(ucs4_t(buf[1])<<8) | buf[0];

			if(uc1>=0xDC00 && uc1<=0xDFFF)
			{
				//ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
				ucs4_t uc2 = ((uc-0xD800)<<10) + (uc1-0xDC00) + 0x10000;
				mapper.MoveUChar32Bytes(ucqueue, uc2, 4);
				return true;
			}
		}

		mapper.MoveUChar32Bytes(ucqueue, uc, 2);
		return true;
	}

	/***
	ucs4_t uc;
	if(m_MadEdit->editMode_!=emHexMode)
		uc=wxT('?');
	else
		uc=m_NextUChar_LineIter->Get(m_NextUChar_Pos);
	***/

	mapper.MoveUChar32Bytes(ucqueue, (ucs4_t)'?', 1);
	return true;
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
bool WXMEncodingUTF16BE::IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
{
	// odd m_Size is invalid
	if((len & 1) != 0)
		return false;

	wxByte buf[2];
	dumper.Dump(buf, 2);
	return buf[1]==0x0A && buf[0]==0;
}
ucs4_t WXMEncodingUTF16BE::PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
{
	// odd m_Size is invalid
	if((len & 1) != 0)
		return 0;

	wxByte buf[2];
	dumper.Dump(buf, 2);
	if ((buf[1]==0x0A || buf[1]==0x0D) && buf[0]==0)
		return (ucs4_t)buf[1];
	return 0;
}
bool WXMEncodingUTF16BE::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	if(rest >= 2)
	{
		ucs4_t uc;
		uc=(ucs4_t(buf[0])<<8) | buf[1];

		if(uc>=0xD800 && uc<=0xDBFF && rest>=4)
		{
			buf+=2;
			ucs4_t uc1=(ucs4_t(buf[0])<<8) | buf[1];

			if(uc1>=0xDC00 && uc1<=0xDFFF)
			{
				ucs4_t uc2 = ((uc-0xD800)<<10) + (uc1-0xDC00) + 0x10000;
				mapper.MoveUChar32Bytes(ucqueue, uc2, 4);
				return true;
			}
		}

		mapper.MoveUChar32Bytes(ucqueue, uc, 2);
		return true;
	}

	mapper.MoveUChar32Bytes(ucqueue, (ucs4_t)'?', 1);
	return true;
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
bool WXMEncodingUTF32LE::IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
{
	// m_Size must be multiple of 4
	if((len & 3) != 0)
		return false;

	wxByte buf[4];
	dumper.Dump(buf, 4);
	return buf[0]==0x0A && buf[1]==0 && buf[2]==0 && buf[3]==0;
}
ucs4_t WXMEncodingUTF32LE::PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
{
	// m_Size must be multiple of 4
	if((len & 3) != 0)
		return 0;

	wxByte buf[4];
	dumper.Dump(buf, 4);
	if ((buf[0]==0x0A || buf[0]==0x0D) && buf[1]==0 && buf[2]==0 && buf[3]==0)
		return (ucs4_t)buf[0];
	return 0;
}
bool WXMEncodingUTF32LE::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	if(rest>=4)
	{
		ucs4_t ucs4=wxUINT32_SWAP_ON_BE(*((ucs4_t*)buf));
		if(ucs4>0x10FFFF || ucs4<0)
		{
			ucs4='?'; // not a valid ucs4 char
		}

		mapper.MoveUChar32Bytes(ucqueue, ucs4, 4);
		return true;
	}

	mapper.MoveUChar32Bytes(ucqueue, (ucs4_t)'?', rest);
	return true;
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
bool WXMEncodingUTF32BE::IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
{
	// m_Size must be multiple of 4
	if((len & 3) != 0)
		return false;

	wxByte buf[4];
	dumper.Dump(buf, 4);
	return buf[3]==0x0A && buf[2]==0 && buf[1]==0 && buf[0]==0;
}
ucs4_t WXMEncodingUTF32BE::PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
{
	// m_Size must be multiple of 4
	if((len & 3) != 0)
		return 0;

	wxByte buf[4];
	dumper.Dump(buf, 4);
	if ((buf[3]==0x0A || buf[3]==0x0D) && buf[2]==0 && buf[1]==0 && buf[0]==0)
		return (ucs4_t)buf[3];
	return 0;
}
bool WXMEncodingUTF32BE::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	if(rest>=4)
	{
		ucs4_t ucs4=wxUINT32_SWAP_ON_LE(*((ucs4_t*)buf));
		if(ucs4>0x10FFFF || ucs4<0)
		{
			ucs4='?'; // not a valid ucs4 char
		}

		mapper.MoveUChar32Bytes(ucqueue, ucs4, 4);
		return true;
	}

	mapper.MoveUChar32Bytes(ucqueue, (ucs4_t)'?', rest);
	return true;
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
