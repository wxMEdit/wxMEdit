///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_def.h
// Description: wxWidgets relative common definitions
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_DEF_H_
#define _WXM_DEF_H_

#include <wx/string.h>

namespace wxm
{
	const wxString MonoFontName =
#ifdef __WXMSW__
		wxT("Courier New")
#elif defined(__APPLE__) && defined(__MACH__)
		wxT("Monaco")
#else
		wxT("Monospace")
#endif
	;

}; // namespace wxm
#endif //_WXM_DEF_H_
