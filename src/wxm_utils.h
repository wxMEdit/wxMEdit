///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_utils.h
// Description: utility functions. collects from internet
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UTILS_H_
#define _WXM_UTILS_H_

#include <wx/defs.h>
#include <map>

struct HtmlColor
{
    const wxChar *name;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
extern HtmlColor HtmlColorTable[];
extern const int HtmlColorTableCount;

extern void SetHtmlColors();
extern wxString GetExecutablePath();
extern std::map<wxString, wxString> g_color_l10n_map;

#endif
