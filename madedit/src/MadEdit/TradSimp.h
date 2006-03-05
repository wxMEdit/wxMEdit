///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/TradSimp.h
// Description: Traditional Chinese <==> Simplified Chinese
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _TRADSIMP_H_
#define _TRADSIMP_H_

#include "ucs4_t.h"

int ConvertChinese(const wxChar *in, wxChar *out, size_t count, bool bTrad2Simp);
void ConvertChineseInClipboard(bool bTrad2Simp);
void FreeConvertChineseTable();

#endif
