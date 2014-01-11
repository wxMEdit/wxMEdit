///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxm_utils.h
// Description: utility functions. collects from internet
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UTILS_H_
#define _WXM_UTILS_H_

#include "xm/wxm_def.h"

#include <wx/defs.h>
#include <wx/string.h>
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

extern void SetHtmlColors();
extern std::map<wxString, wxString> g_color_l10n_map;

extern wxString GetExecutablePath();

extern void OpenURL(const wxString& url);

struct FileList
{
    FileList(const wxString& files = wxString())
    {
        Init(files);
    }
    FileList(const wchar_t* files)
    {
        Init(wxString(files));
    }

    void Append(const wxString& file, const LineNumberList& bmklns=LineNumberList());

    const wxString& String()
    {
        return m_files;
    }

    struct FileDesc
    {
        wxString file;
        LineNumberList bmklinenums;

        FileDesc() {}
        FileDesc(const wxString& f, const LineNumberList& lns) : file(f), bmklinenums(lns) {}
    };

    typedef std::vector<FileDesc> FDescList;

    const FDescList& List()
    {
        return m_filevec;
    }

private:
    void Init(const wxString& files);

    wxString m_files;
    FDescList m_filevec;
};

#endif
