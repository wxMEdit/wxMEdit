///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_encdet.h
// Description: wxMEdit encoding detector
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCDET_H_
#define _WXM_ENCDET_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

bool IsTextUTF32LE(wxByte *text, int size);
bool IsTextUTF32BE(wxByte *text, int size);
bool IsTextUTF16LE(wxByte *text, int size);
bool IsTextUTF16BE(wxByte *text, int size);
bool IsTextUTF8(wxByte *text, int size);
bool IsBinaryData(wxByte *data, int size);
void DetectChineseEncoding(const wxByte *text, int count, wxFontEncoding &enc);
void DetectJapaneseEncoding(const wxByte *text, int count, wxFontEncoding &enc);
void DetectEncoding(const wxByte *text, int count, wxFontEncoding &enc);

#endif //_WXM_ENCDET_H_
