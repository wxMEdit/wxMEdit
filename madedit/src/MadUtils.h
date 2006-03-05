///////////////////////////////////////////////////////////////////////////////
// Name:        MadUtils.h
// Description: utility functions. collects from internet
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADUTILS_H_
#define _MADUTILS_H_

extern void SetHtmlColors();
extern wxString GetExecutablePath();
extern wxString FormatThousands(const wxString& s);
extern bool StrToInt64(wxString str, wxInt64 &i64);

#endif
