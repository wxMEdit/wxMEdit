///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxm_utils.h
// Description: Utility Functions
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAD_UTILS_H_
#define _MAD_UTILS_H_

#include "xm/encoding/encoding_def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/defs.h>
#include <wx/string.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <map>
#include <vector>

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

inline ucs4_t ToHex(int d)// 0 <= d <= 15
{
    if (d < 10)
        return '0' + d;
    return 'A' + d - 10;
}

inline int FromHex(wxChar c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool StringToHex(wxString ws, std::vector<wxByte> &hex);

#endif
