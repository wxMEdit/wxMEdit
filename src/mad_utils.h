///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxm_utils.h
// Description: Utility Functions
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAD_UTILS_H_
#define _MAD_UTILS_H_

#include <wx/defs.h>
#include <wx/string.h>
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

extern std::map<wxString, wxString> g_color_l10n_map;

extern wxString GetExecutablePath();

#endif
