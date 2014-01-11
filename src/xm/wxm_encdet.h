///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_encdet.h
// Description: wxMEdit encoding detector
// Author:      wxmedit@gmail.com
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
