///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/def.h
// Description: wxMEdit Common Definitions
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_DEF_H_
#define _WXM_DEF_H_

#include "../xm/encoding/encoding_def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/string.h>
#include <wx/version.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <vector>
#include <string>

#define WXMEDIT_VERSION	"3.1"

typedef std::basic_string<ucs4_t> ucs4string;

#if wxMAJOR_VERSION==2
  typedef int wxPrintOrientation;
# define wxGetSelectedChoices wxGetMultipleChoices
# define wxPENSTYLE_SOLID wxSOLID
#endif

typedef std::vector<size_t> LineNumberList;

namespace wxm
{
	static const std::wstring MonoFontName =
#ifdef __WXMSW__
		L"Courier New"
#elif defined(__APPLE__) && defined(__MACH__)
		L"Monaco"
#else
		L"Monospace"
#endif
	;

	static const std::wstring ASCIIArtFontName =
#ifdef __WXMSW__
		L"Courier New"
#elif defined(__APPLE__) && defined(__MACH__)
		L"Andale Mono"
#else
		L"Monospace"
#endif
	;

	static const long MINVAL_MAXLINELEN = 80;
	static const long MAXVAL_MAXLINELEN = 65536;

	static const size_t FIRSTLINE_SYNTAXPATTEN_MAXLEN = 16;

	static const int DEFAULT_WINDOW_WIDTH = 990;
	static const int DEFAULT_WINDOW_HEIGHT = 660;

}; // namespace wxm

#endif //_WXM_DEF_H_
