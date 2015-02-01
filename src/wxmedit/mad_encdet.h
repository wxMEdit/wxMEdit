///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4 expandtab
// Name:        wxmedit/mad_encdet.h
// Description: MadEdit Encoding Detector
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAD_ENCDET_H_
#define _MAD_ENCDET_H_

#include "../wxm/encoding/encoding_def.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/defs.h>
#endif

bool IsBinaryData(const wxByte *data, size_t len);
void DetectChineseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc);
void DetectJapaneseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc);

#endif //_MAD_ENCDET_H_
