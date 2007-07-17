///////////////////////////////////////////////////////////////////////////////
// Name:        MadUtils.h
// Description: utility functions. collects from internet
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADUTILS_H_
#define _MADUTILS_H_

#include <wx/defs.h>

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
extern bool StrToInt64(wxString str, wxInt64 &i64);

#endif
