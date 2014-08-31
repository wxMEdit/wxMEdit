///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/trad_simp.h
// Description: Traditional Chinese <==> Simplified Chinese <==> Japanese Kanji
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _TRAD_SIMP_H_
#define _TRAD_SIMP_H_

#include "ucs4_t.h"

// Trad: Traditional Chinese characters
// Simp: Simplified Chinese characters
// Kanji: Chinese characters used in Japanese form of writing
enum MadConvertChineseFlag
{ 
    ccfSimp2Trad, ccfTrad2Simp,
    ccfKanji2Trad, ccfKanji2Simp,
    ccfChinese2Kanji,
};

int ConvertChinese(const wxChar *in, wxChar *out, size_t count, MadConvertChineseFlag flag);
void ConvertChineseInClipboard(MadConvertChineseFlag flag);
void FreeConvertChineseTable();

#endif
