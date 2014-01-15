///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxm_utils.cpp
// Description: Utility Functions
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_utils.h"
#include "xm/wxm_encoding/encoding.h"
#include "xm/wxm_def.h"

#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>
#include <wx/font.h>
#include <wx/window.h>
#include <wx/hashmap.h>

#include <unicode/uchar.h>

#include <boost/foreach.hpp>

#include <locale.h>

#if defined(__WXMAC__)
#include <wx/mac/private.h>
#include <Processes.h>
#endif

HtmlColor HtmlColorTable[]=
{
    { wxT("AliceBlue"),            240,248,255 },
    { wxT("AntiqueWhite"),         250,235,215 },
    { wxT("Aqua"),                 0,255,255 },
    { wxT("Aquamarine"),           127,255,212 },
    { wxT("Azure"),                240,255,255 },
    { wxT("Beige"),                245,245,220 },
    { wxT("Bisque"),               255,228,196 },
    { wxT("Black"),                0,0,0 },
    { wxT("BlanchedAlmond"),       255,235,205 },
    { wxT("Blue"),                 0,0,255 },
    { wxT("BlueViolet"),           138,43,226 },
    { wxT("Brown"),                165,42,42 },
    { wxT("BurlyWood"),            222,184,135 },
    { wxT("CadetBlue"),            95,158,160 },
    { wxT("Chartreuse"),           127,255,0 },
    { wxT("Chocolate"),            210,105,30 },
    { wxT("Coral"),                255,127,80 },
    { wxT("CornflowerBlue"),       100,149,237 },
    { wxT("Cornsilk"),             255,248,220 },
    { wxT("Crimson"),              237,164,61 },
    { wxT("Cyan"),                 0,255,255 },
    { wxT("DarkBlue"),             0,0,139 },
    { wxT("DarkCyan"),             0,139,139 },
    { wxT("DarkGoldenRod"),        184,134,11 },
    { wxT("DarkGray"),             167,167,167 },
    { wxT("DarkGreen"),            0,100,0 },
    { wxT("DarkKhaki"),            189,183,107 },
    { wxT("DarkMagenta"),          139,0,139 },
    { wxT("DarkOliveGreen"),       85,107,47 },
    { wxT("DarkOrchid"),           153,50,204 },
    { wxT("DarkRed"),              139,0,0 },
    { wxT("DarkSalmon"),           233,150,122 },
    { wxT("DarkSeaGreen"),         143,188,143 },
    { wxT("DarkSlateBlue"),        72,61,139 },
    { wxT("DarkSlateGray"),        47,79,79 },
    { wxT("DarkTurquoise"),        0,206,209 },
    { wxT("DarkViolet"),           148,0,211 },
    { wxT("Darkorange"),           255,140,0 },
    { wxT("DeepPink"),             255,20,147 },
    { wxT("DeepSkyBlue"),          0,191,255 },
    { wxT("DimGray"),              105,105,105 },
    { wxT("DodgerBlue"),           30,144,255 },
    { wxT("Feldspar"),             209,146,117 },
    { wxT("FireBrick"),            178,34,34 },
    { wxT("FloralWhite"),          255,250,240 },
    { wxT("ForestGreen"),          34,139,34 },
    { wxT("Fuchsia"),              255,0,255 },
    { wxT("Gainsboro"),            220,220,220 },
    { wxT("GhostWhite"),           248,248,255 },
    { wxT("Gold"),                 255,215,0 },
    { wxT("GoldenRod"),            218,165,32 },
    { wxT("Gray"),                 190,190,190 },
    { wxT("Green"),                0,128,0 },
    { wxT("GreenYellow"),          173,255,47 },
    { wxT("HoneyDew"),             240,255,240 },
    { wxT("HotPink"),              255,105,180 },
    { wxT("IndianRed"),            205,92,92 },
    { wxT("Indigo"),               75,0,130 },
    { wxT("Ivory"),                255,255,240 },
    { wxT("Khaki"),                240,230,140 },
    { wxT("Lavender"),             230,230,250 },
    { wxT("LavenderBlush"),        255,240,245 },
    { wxT("LawnGreen"),            124,252,0 },
    { wxT("LemonChiffon"),         255,250,205 },
    { wxT("LightBlue"),            173,216,230 },
    { wxT("LightCoral"),           240,128,128 },
    { wxT("LightCyan"),            224,255,255 },
    { wxT("LightGoldenRodYellow"), 250,250,210 },
    { wxT("LightGreen"),           144,238,144 },
    { wxT("LightGrey"),            211,211,211 },
    { wxT("LightPink"),            255,182,193 },
    { wxT("LightSalmon"),          255,160,122 },
    { wxT("LightSeaGreen"),        32,178,170 },
    { wxT("LightSkyBlue"),         135,206,250 },
    { wxT("LightSlateBlue"),       132,112,255 },
    { wxT("LightSlateGray"),       119,136,153 },
    { wxT("LightSteelBlue"),       176,196,222 },
    { wxT("LightYellow"),          255,255,224 },
    { wxT("Lime"),                 0,255,0 },
    { wxT("LimeGreen"),            50,205,50 },
    { wxT("Linen"),                250,240,230 },
    { wxT("Magenta"),              255,0,255 },
    { wxT("Maroon"),               128,0,0 },
    { wxT("MediumAquaMarine"),     102,205,170 },
    { wxT("MediumBlue"),           0,0,205 },
    { wxT("MediumOrchid"),         186,85,211 },
    { wxT("MediumPurple"),         147,112,219 },
    { wxT("MediumSeaGreen"),       60,179,113 },
    { wxT("MediumSlateBlue"),      123,104,238 },
    { wxT("MediumSpringGreen"),    0,250,154 },
    { wxT("MediumTurquoise"),      72,209,204 },
    { wxT("MediumVioletRed"),      199,21,133 },
    { wxT("MidnightBlue"),         25,25,112 },
    { wxT("MintCream"),            245,255,250 },
    { wxT("MistyRose"),            255,228,225 },
    { wxT("Moccasin"),             255,228,181 },
    { wxT("NavajoWhite"),          255,222,173 },
    { wxT("Navy"),                 0,0,128 },
    { wxT("OldLace"),              253,245,230 },
    { wxT("Olive"),                128,128,0 },
    { wxT("OliveDrab"),            107,142,35 },
    { wxT("Orange"),               255,165,0 },
    { wxT("OrangeRed"),            255,69,0 },
    { wxT("Orchid"),               218,112,214 },
    { wxT("PaleGoldenRod"),        238,232,170 },
    { wxT("PaleGreen"),            152,251,152 },
    { wxT("PaleTurquoise"),        175,238,238 },
    { wxT("PaleVioletRed"),        219,112,147 },
    { wxT("PapayaWhip"),           255,239,213 },
    { wxT("PeachPuff"),            255,218,185 },
    { wxT("Peru"),                 205,133,63 },
    { wxT("Pink"),                 255,192,203 },
    { wxT("Plum"),                 221,160,221 },
    { wxT("PowderBlue"),           176,224,230 },
    { wxT("Purple"),               128,0,128 },
    { wxT("Red"),                  255,0,0 },
    { wxT("RosyBrown"),            188,143,143 },
    { wxT("RoyalBlue"),            65,105,225 },
    { wxT("SaddleBrown"),          139,69,19 },
    { wxT("Salmon"),               250,128,114 },
    { wxT("SandyBrown"),           244,164,96 },
    { wxT("SeaGreen"),             46,139,87 },
    { wxT("SeaShell"),             255,245,238 },
    { wxT("Sienna"),               160,82,45 },
    { wxT("Silver"),               192,192,192 },
    { wxT("SkyBlue"),              135,206,235 },
    { wxT("SlateBlue"),            106,90,205 },
    { wxT("SlateGray"),            112,128,144 },
    { wxT("Snow"),                 255,250,250 },
    { wxT("SpringGreen"),          0,255,127 },
    { wxT("SteelBlue"),            70,130,180 },
    { wxT("Tan"),                  210,180,140 },
    { wxT("Teal"),                 0,128,128 },
    { wxT("Thistle"),              216,191,216 },
    { wxT("Tomato"),               255,99,71 },
    { wxT("Turquoise"),            64,224,208 },
    { wxT("Violet"),               238,130,238 },
    { wxT("VioletRed"),            208,32,144 },
    { wxT("Wheat"),                245,222,179 },
    { wxT("White"),                255,255,255 },
    { wxT("WhiteSmoke"),           245,245,245 },
    { wxT("Yellow"),               255,255,0 },
    { wxT("YellowGreen"),          154,205,50 }
};
const int HtmlColorTableCount= sizeof(HtmlColorTable) / sizeof(HtmlColor);

std::map<wxString, wxString> g_color_l10n_map;

wxString GetExecutablePath()
{
    static bool found = false;
    static wxString path;

    if (!found) {
#ifdef __WXMSW__

        wxChar buf[512];
        *buf = wxT('\0');
        GetModuleFileName(NULL, buf, 511);
        path = buf;

#elif defined(__WXMAC__)

        ProcessInfoRec processinfo;
        ProcessSerialNumber procno ;
        FSSpec fsSpec;

        procno.highLongOfPSN = NULL ;
        procno.lowLongOfPSN = kCurrentProcess ;
        processinfo.processInfoLength = sizeof(ProcessInfoRec);
        processinfo.processName = NULL;
        processinfo.processAppSpec = &fsSpec;

        GetProcessInformation( &procno , &processinfo ) ;
        path = wxMacFSSpec2MacFilename(&fsSpec);
#else
        wxString argv0 = wxTheApp->argv[0];

        if (wxIsAbsolutePath(argv0)) {
            path = argv0;
        }
        else {
            wxPathList pathlist;
            pathlist.AddEnvList(wxT("PATH"));
            path = pathlist.FindAbsoluteValidPath(argv0);
        }

        wxFileName filename(path);
        filename.Normalize();
        path = filename.GetFullPath();
#endif
        found = true;
    }

    return path;
}


namespace wxm
{

void SetL10nHtmlColors()
{
    wxColourDatabase *cdb=wxTheColourDatabase;
    HtmlColor *hc=HtmlColorTable;
    for(int i=0; i<HtmlColorTableCount; ++i, ++hc)
    {
        cdb->AddColour(hc->name, wxColor(hc->red, hc->green, hc->blue));
        g_color_l10n_map[wxString(hc->name).Upper()] = wxGetTranslation(hc->name);
    }
}

void FileList::Append(const wxString& file, const LineNumberList& bmklns)
{
    m_files += file + wxT("<");
    BOOST_FOREACH (size_t ln, bmklns)
        m_files << ln << wxT("<");
    m_files += wxT("|");

    m_filevec.push_back(FileDesc(file, bmklns));
}

void FileList::Init(const wxString& files)
{
    m_files = files;

    wxStringTokenizer tkzfiledescs(files,  wxT("|"));
    while (tkzfiledescs.HasMoreTokens())
    {
        wxStringTokenizer tkz(tkzfiledescs.GetNextToken(), wxT("<"));

        wxString file = tkz.GetNextToken();

        LineNumberList bmklns;
        while (tkz.HasMoreTokens())
        {
            unsigned long ln = 0;
            if (tkz.GetNextToken().ToULong(&ln))
                bmklns.push_back((size_t)ln);
        }

        m_filevec.push_back(FileDesc(file, bmklns));
    }
}

void OpenURL(const wxString& url)
{
#ifdef __WXGTK__
    const wxChar *browsers[]=
    {
        wxT("/usr/bin/firefox"),
        wxT("/usr/bin/mozilla"),
        wxT("/usr/bin/chromium"),
        wxT("/usr/bin/konqueror"),
# if defined(__APPLE__) && defined(__MACH__)
        wxT("/usr/bin/open"),
# endif
        wxT("/usr/kde/3.5/bin/konqueror"),
        wxT("/usr/kde/3.4/bin/konqueror"),
        wxT("/usr/kde/3.3/bin/konqueror"),
        wxT("/usr/kde/3.2/bin/konqueror"),
    };
    int idx=0;
    int count=sizeof(browsers)/sizeof(browsers[0]);
    do
    {
        if(wxFileExists(wxString(browsers[idx])))
            break;
    }while(++idx < count);

    if(idx < count)
    {
        wxExecute(wxString(browsers[idx]) +wxT(' ') +url);
    }
    else
    {
        wxLaunchDefaultBrowser(url);
    }
#else
    wxLaunchDefaultBrowser(url);
#endif
}

void SetDefaultMonoFont(wxWindow* win)
{
    const wxString fontname = wxm::WXMEncodingCreator::Instance().GetSystemEncoding()->GetFontName();
    int fontsize = win->GetFont().GetPointSize();
    win->SetFont(wxFont(fontsize, wxDEFAULT, wxNORMAL, wxNORMAL, false, fontname));
}

wxString FilePathNormalCase(const wxString& name)
{
#ifndef __WXMSW__
	return name;
#else
	// wxString::Upper is buggy under Windows
	// and the filename insensitive of Windows is also buggy
 	// but they are different
	wxString uppername;
	BOOST_FOREACH(wxChar ch, name)
		uppername.append(1, (wxChar)u_toupper((UChar32)(unsigned int)ch));

	return uppername;
#endif
}

bool FilePathEqual(const wxString& name1, const wxString& name2)
{
	return FilePathNormalCase(name1) == FilePathNormalCase(name2);
}

unsigned long FilePathHash(const wxString& name)
{
	return wxStringHash::wxCharStringHash(FilePathNormalCase(name));
}

} // namespace wxm
