///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/encoding_def.h
// Description: Encoding Relative Definitions of wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#ifndef _XM_ENCODING_DEF_H_
#define _XM_ENCODING_DEF_H_

#include <unicode/umachine.h>

typedef uint8_t ubyte;
typedef uint16_t ucs2_t;

#if defined(U_WINDOWS) || defined(U_PF_WINDOWS) && U_PLATFORM == U_PF_WINDOWS || defined(__MINGW32__)
	typedef int32_t ucs4_t;
#else
	typedef wchar_t ucs4_t;
#endif

#ifdef _MSC_VER
	typedef ptrdiff_t ssize_t;
#endif

namespace xm
{

enum EncodingID
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
	ENC_CP437_ART,
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

enum EncodingGroupID
{
	ENCG_DEFAULT = 0,
	ENCG_WESTERNEUROPE,
	ENCG_CENTRALEUROPE,
	ENCG_SOUTHEUROPE,
	ENCG_NORTHEUROPE,
	ENCG_CYRILLIC,
	ENCG_ARABIC,
	ENCG_GREEK,
	ENCG_HEBREW,
	ENCG_TURKISH,
	ENCG_BALTIC,
	ENCG_EASTASIA,
	ENCG_SOUTHEASTASIA,
	ENCG_UNICODE,
	ENCG_ISO8859,
	ENCG_WINDOWS,
	ENCG_OEM,
};

};// namespace xm

#endif // _XM_ENCODING_DEF_H_
