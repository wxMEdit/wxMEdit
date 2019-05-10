///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/encdet.h
// Description: wxMEdit Encoding Detector
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCDET_H_
#define _XM_ENCDET_H_

#include "encoding/encoding_def.h"

#include <string>

namespace xm
{
	bool IsUTF8(const ubyte* text, size_t len);
	bool MatchEncoding(std::string& enc, const ubyte* text, size_t len);
	void DetectEncoding(const ubyte* text, size_t len, EncodingID& enc, bool skip_utf8);
} //namespace xm

#endif //_XM_ENCDET_H_
