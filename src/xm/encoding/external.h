///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/external.h
// Description: Declaration of External Dependencies
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#ifndef _XM_ENCODING_EXTERNAL_H_
#define _XM_ENCODING_EXTERNAL_H_
#include <string>

std::wstring GetMSCPFontName(const std::wstring & mscp);
std::wstring GetASCIIArtFontName();
const wchar_t * LocalizeText(const wchar_t* txt);

#endif // _XM_ENCODING_EXTERNAL_H_
