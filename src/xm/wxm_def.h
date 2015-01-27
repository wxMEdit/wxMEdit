///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_def.h
// Description: wxMEdit Common Definitions
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_DEF_H_
#define _WXM_DEF_H_

#include <wx/string.h>
#include <wx/version.h>
#include <vector>

#define WXMEDIT_VERSION	"2.9.9"

#if wxMAJOR_VERSION==2 && wxMINOR_VERSION==6
# define wxOS_WINDOWS_9X wxWIN95
# include <wx/longlong.h>
  typedef wxLongLong wxMilliClock_t;
# define EVT_MOUSE_CAPTURE_LOST(x) 
# include <wx/event.h>
  typedef wxMouseEvent wxMouseCaptureLostEvent;
# define wxDD_DIR_MUST_EXIST 0x0200
#endif

typedef std::vector<size_t> LineNumberList;

namespace wxm
{
	static const wxString MonoFontName =
#ifdef __WXMSW__
		wxT("Courier New")
#elif defined(__APPLE__) && defined(__MACH__)
		wxT("Monaco")
#else
		wxT("Monospace")
#endif
	;

	static const wxString ASCIIArtFontName =
#ifdef __WXMSW__
		wxT("Courier New")
#elif defined(__APPLE__) && defined(__MACH__)
		wxT("Andale Mono")
#else
		wxT("Monospace")
#endif
	;

	static const long MINVAL_MAXLINELEN = 80;
	static const long MAXVAL_MAXLINELEN = 65536;

	static const size_t FIRSTLINE_SYNTAXPATTEN_MAXLEN = 16;

	static const int DEFAULT_WINDOW_WIDTH = 990;
	static const int DEFAULT_WINDOW_HEIGHT = 660;

}; // namespace wxm

#endif //_WXM_DEF_H_
