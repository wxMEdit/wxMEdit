///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_def.h
// Description: wxMEdit common definitions
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_DEF_H_
#define _WXM_DEF_H_

#include <wx/string.h>
#include <wx/version.h>
#include <vector>

#define WXMEDIT_VERSION	"2.9.6"

#if defined(__WXMSW__) && wxMAJOR_VERSION==2 && wxMINOR_VERSION==6
# define wxOS_WINDOWS_9X wxWIN95
# include <wx/longlong.h>
  typedef wxLongLong wxMilliClock_t;
#endif

typedef std::vector<size_t> LineNumberList;

namespace wxm
{
	const wxString MonoFontName =
#ifdef __WXMSW__
		wxT("Courier New")
#elif defined(__APPLE__) && defined(__MACH__)
		wxT("Andale Mono")
#else
		wxT("Monospace")
#endif
	;

	const long MINVAL_MAXLINELEN = 80;
	const long MAXVAL_MAXLINELEN = 65536;

	const size_t FIRSTLINE_SYNTAXPATTEN_MAXLEN = 16;

}; // namespace wxm

#endif //_WXM_DEF_H_
