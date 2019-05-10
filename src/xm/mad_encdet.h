///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/mad_encdet.h
// Description: MadEdit Encoding Detector
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAD_ENCDET_H_
#define _MAD_ENCDET_H_

#include "encoding/encoding_def.h"

bool IsBinaryData(const ubyte* data, size_t len);
void DetectChineseEncoding(const ubyte * text, size_t len, xm::EncodingID &enc);
void DetectJapaneseEncoding(const ubyte * text, size_t len, xm::EncodingID &enc);

#endif //_MAD_ENCDET_H_
