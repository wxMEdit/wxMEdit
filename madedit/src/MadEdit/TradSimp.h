///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/TradSimp.h
// Description: Traditional Chinese <==> Simplified Chinese <==> Japanese Kanji
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _TRADSIMP_H_
#define _TRADSIMP_H_

#include "ucs4_t.h"

// Trad: Traditional Chinese characters
// Simp: Simplified Chinese characters
// Kanji: Chinese characters used in Japanese form of writing
enum MadConvertChineseFlag
{ 
    ccfTrad2Simp, ccfSimp2Trad,
    ccfKanji2Trad, ccfKanji2Simp,
    ccfChinese2Kanji,
};

int ConvertChinese(const wxChar *in, wxChar *out, size_t count, MadConvertChineseFlag flag);
void ConvertChineseInClipboard(MadConvertChineseFlag flag);
void FreeConvertChineseTable();

#endif
