///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_encdet.h
// Description: wxMEdit encoding detector
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCDET_H_
#define _WXM_ENCDET_H_

#include "../xm/wxm_encoding/encoding_def.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

bool IsUTF32LE(const wxByte *text, size_t len);
bool IsUTF32BE(const wxByte *text, size_t len);
bool IsUTF16LE(const wxByte *text, size_t len);
bool IsUTF16BE(const wxByte *text, size_t len);
bool IsUTF8(const wxByte *text, size_t len);
bool MatchSimpleUnicode(wxString& enc, const wxByte *text, size_t len);
bool IsBinaryData(const wxByte *data, size_t len);
void DetectChineseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc);
void DetectJapaneseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc);
void DetectEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc);

#endif //_WXM_ENCDET_H_
