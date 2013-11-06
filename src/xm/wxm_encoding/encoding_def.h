///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/encoding_def.h
// Description: Encoding relative definitions of wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////
#ifndef _WXM_ENCODING_DEF_H_
#define _WXM_ENCODING_DEF_H_

namespace wxm
{

enum WXMEncodingID
{
	ENC_DEFAULT = 0,
	ENC_ISO_646,
	ENC_ISO_8859_1,
	ENC_ISO_8859_2,
	ENC_ISO_8859_3,
	ENC_ISO_8859_4,
	ENC_ISO_8859_5,
	ENC_ISO_8859_6,
	ENC_ISO_8859_7,
	ENC_ISO_8859_8,
	ENC_ISO_8859_9,
	ENC_ISO_8859_10,
	ENC_ISO_8859_11,
	ENC_ISO_8859_12, /* no use */
	ENC_ISO_8859_13,
	ENC_ISO_8859_14,
	ENC_ISO_8859_15,
	ENC_ISO_8859_16,
	ENC_Windows_874,
	ENC_Windows_1250,
	ENC_Windows_1251,
	ENC_Windows_1252,
	ENC_Windows_1253,
	ENC_Windows_1254,
	ENC_Windows_1255,
	ENC_Windows_1256,
	ENC_Windows_1257,
	ENC_Windows_1258,
	ENC_CP437,
	ENC_CP850,
	ENC_CP852,
	ENC_CP855,
	ENC_CP866,
	ENC_KOI8_R,
	ENC_KOI8_U,
	ENC_MS932,
	ENC_MS936,
	ENC_MS949,
	ENC_MS950,
	ENC_CP20932,
	ENC_GB18030,
	ENC_UTF_8,
	ENC_UTF_16LE,
	ENC_UTF_16BE,
	ENC_UTF_32LE,
	ENC_UTF_32BE,
	ENC_MAX
};

};// namespace wxm

#endif // _WXM_ENCODING_DEF_H_
