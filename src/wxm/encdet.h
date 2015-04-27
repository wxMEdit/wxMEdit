///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/encdet.h
// Description: wxMEdit Encoding Detector
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCDET_H_
#define _WXM_ENCDET_H_

#include "encoding/encoding_def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/wxprec.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/defs.h>
#include <wx/string.h>
#endif

namespace wxm
{
	bool IsUTF8(const wxByte *text, size_t len);
	bool MatchWXMEncoding(wxString& enc, const wxByte *text, size_t len);
	void DetectEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc, bool skip_utf8);
} //namespace wxm

#endif //_WXM_ENCDET_H_
