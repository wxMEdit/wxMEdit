///////////////////////////////////////////////////////////////////////////////
// Name:        MadUtils.cpp
// Description: utility functions. collects from internet
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <locale.h>

#if defined(__WXMAC__)
#include <wx/mac/private.h>
#include <Processes.h>
#endif

void SetHtmlColors()
{
    // add colors same with html color
    wxTheColourDatabase->AddColour(wxT("AliceBlue"),            wxColor(240,248,255));
    wxTheColourDatabase->AddColour(wxT("AntiqueWhite"),         wxColor(250,235,215));
    wxTheColourDatabase->AddColour(wxT("Aqua"),                 wxColor(0,255,255));
    wxTheColourDatabase->AddColour(wxT("Aquamarine"),           wxColor(127,255,212));
    wxTheColourDatabase->AddColour(wxT("Azure"),                wxColor(240,255,255));
    wxTheColourDatabase->AddColour(wxT("Beige"),                wxColor(245,245,220));
    wxTheColourDatabase->AddColour(wxT("Bisque"),               wxColor(255,228,196));
    wxTheColourDatabase->AddColour(wxT("Black"),                wxColor(0,0,0));
    wxTheColourDatabase->AddColour(wxT("BlanchedAlmond"),       wxColor(255,235,205));
    wxTheColourDatabase->AddColour(wxT("Blue"),                 wxColor(0,0,255));
    wxTheColourDatabase->AddColour(wxT("BlueViolet"),           wxColor(138,43,226));
    wxTheColourDatabase->AddColour(wxT("Brown"),                wxColor(165,42,42));
    wxTheColourDatabase->AddColour(wxT("BurlyWood"),            wxColor(222,184,135));
    wxTheColourDatabase->AddColour(wxT("CadetBlue"),            wxColor(95,158,160));
    wxTheColourDatabase->AddColour(wxT("Chartreuse"),           wxColor(127,255,0));
    wxTheColourDatabase->AddColour(wxT("Chocolate"),            wxColor(210,105,30));
    wxTheColourDatabase->AddColour(wxT("Coral"),                wxColor(255,127,80));
    wxTheColourDatabase->AddColour(wxT("CornflowerBlue"),       wxColor(100,149,237));
    wxTheColourDatabase->AddColour(wxT("Cornsilk"),             wxColor(255,248,220));
    wxTheColourDatabase->AddColour(wxT("Crimson"),              wxColor(237,164,61));
    wxTheColourDatabase->AddColour(wxT("Cyan"),                 wxColor(0,255,255));
    wxTheColourDatabase->AddColour(wxT("DarkBlue"),             wxColor(0,0,139));
    wxTheColourDatabase->AddColour(wxT("DarkCyan"),             wxColor(0,139,139));
    wxTheColourDatabase->AddColour(wxT("DarkGoldenRod"),        wxColor(184,134,11));
    wxTheColourDatabase->AddColour(wxT("DarkGray"),             wxColor(167,167,167));
    wxTheColourDatabase->AddColour(wxT("DarkGreen"),            wxColor(0,100,0));
    wxTheColourDatabase->AddColour(wxT("DarkKhaki"),            wxColor(189,183,107));
    wxTheColourDatabase->AddColour(wxT("DarkMagenta"),          wxColor(139,0,139));
    wxTheColourDatabase->AddColour(wxT("DarkOliveGreen"),       wxColor(85,107,47));
    wxTheColourDatabase->AddColour(wxT("DarkOrchid"),           wxColor(153,50,204));
    wxTheColourDatabase->AddColour(wxT("DarkRed"),              wxColor(139,0,0));
    wxTheColourDatabase->AddColour(wxT("DarkSalmon"),           wxColor(233,150,122));
    wxTheColourDatabase->AddColour(wxT("DarkSeaGreen"),         wxColor(143,188,143));
    wxTheColourDatabase->AddColour(wxT("DarkSlateBlue"),        wxColor(72,61,139));
    wxTheColourDatabase->AddColour(wxT("DarkSlateGray"),        wxColor(47,79,79));
    wxTheColourDatabase->AddColour(wxT("DarkTurquoise"),        wxColor(0,206,209));
    wxTheColourDatabase->AddColour(wxT("DarkViolet"),           wxColor(148,0,211));
    wxTheColourDatabase->AddColour(wxT("Darkorange"),           wxColor(255,140,0));
    wxTheColourDatabase->AddColour(wxT("DeepPink"),             wxColor(255,20,147));
    wxTheColourDatabase->AddColour(wxT("DeepSkyBlue"),          wxColor(0,191,255));
    wxTheColourDatabase->AddColour(wxT("DimGray"),              wxColor(105,105,105));
    wxTheColourDatabase->AddColour(wxT("DodgerBlue"),           wxColor(30,144,255));
    wxTheColourDatabase->AddColour(wxT("Feldspar"),             wxColor(209,146,117));
    wxTheColourDatabase->AddColour(wxT("FireBrick"),            wxColor(178,34,34));
    wxTheColourDatabase->AddColour(wxT("FloralWhite"),          wxColor(255,250,240));
    wxTheColourDatabase->AddColour(wxT("ForestGreen"),          wxColor(34,139,34));
    wxTheColourDatabase->AddColour(wxT("Fuchsia"),              wxColor(255,0,255));
    wxTheColourDatabase->AddColour(wxT("Gainsboro"),            wxColor(220,220,220));
    wxTheColourDatabase->AddColour(wxT("GhostWhite"),           wxColor(248,248,255));
    wxTheColourDatabase->AddColour(wxT("Gold"),                 wxColor(255,215,0));
    wxTheColourDatabase->AddColour(wxT("GoldenRod"),            wxColor(218,165,32));
    wxTheColourDatabase->AddColour(wxT("Gray"),                 wxColor(190,190,190));
    wxTheColourDatabase->AddColour(wxT("Green"),                wxColor(0,128,0));
    wxTheColourDatabase->AddColour(wxT("GreenYellow"),          wxColor(173,255,47));
    wxTheColourDatabase->AddColour(wxT("HoneyDew"),             wxColor(240,255,240));
    wxTheColourDatabase->AddColour(wxT("HotPink"),              wxColor(255,105,180));
    wxTheColourDatabase->AddColour(wxT("IndianRed"),            wxColor(205,92,92));
    wxTheColourDatabase->AddColour(wxT("Indigo"),               wxColor(75,0,130));
    wxTheColourDatabase->AddColour(wxT("Ivory"),                wxColor(255,255,240));
    wxTheColourDatabase->AddColour(wxT("Khaki"),                wxColor(240,230,140));
    wxTheColourDatabase->AddColour(wxT("Lavender"),             wxColor(230,230,250));
    wxTheColourDatabase->AddColour(wxT("LavenderBlush"),        wxColor(255,240,245));
    wxTheColourDatabase->AddColour(wxT("LawnGreen"),            wxColor(124,252,0));
    wxTheColourDatabase->AddColour(wxT("LemonChiffon"),         wxColor(255,250,205));
    wxTheColourDatabase->AddColour(wxT("LightBlue"),            wxColor(173,216,230));
    wxTheColourDatabase->AddColour(wxT("LightCoral"),           wxColor(240,128,128));
    wxTheColourDatabase->AddColour(wxT("LightCyan"),            wxColor(224,255,255));
    wxTheColourDatabase->AddColour(wxT("LightGoldenRodYellow"), wxColor(250,250,210));
    wxTheColourDatabase->AddColour(wxT("LightGreen"),           wxColor(144,238,144));
    wxTheColourDatabase->AddColour(wxT("LightGrey"),            wxColor(211,211,211));
    wxTheColourDatabase->AddColour(wxT("LightPink"),            wxColor(255,182,193));
    wxTheColourDatabase->AddColour(wxT("LightSalmon"),          wxColor(255,160,122));
    wxTheColourDatabase->AddColour(wxT("LightSeaGreen"),        wxColor(32,178,170));
    wxTheColourDatabase->AddColour(wxT("LightSkyBlue"),         wxColor(135,206,250));
    wxTheColourDatabase->AddColour(wxT("LightSlateBlue"),       wxColor(132,112,255));
    wxTheColourDatabase->AddColour(wxT("LightSlateGray"),       wxColor(119,136,153));
    wxTheColourDatabase->AddColour(wxT("LightSteelBlue"),       wxColor(176,196,222));
    wxTheColourDatabase->AddColour(wxT("LightYellow"),          wxColor(255,255,224));
    wxTheColourDatabase->AddColour(wxT("Lime"),                 wxColor(0,255,0));
    wxTheColourDatabase->AddColour(wxT("LimeGreen"),            wxColor(50,205,50));
    wxTheColourDatabase->AddColour(wxT("Linen"),                wxColor(250,240,230));
    wxTheColourDatabase->AddColour(wxT("Magenta"),              wxColor(255,0,255));
    wxTheColourDatabase->AddColour(wxT("Maroon"),               wxColor(128,0,0));
    wxTheColourDatabase->AddColour(wxT("MediumAquaMarine"),     wxColor(102,205,170));
    wxTheColourDatabase->AddColour(wxT("MediumBlue"),           wxColor(0,0,205));
    wxTheColourDatabase->AddColour(wxT("MediumOrchid"),         wxColor(186,85,211));
    wxTheColourDatabase->AddColour(wxT("MediumPurple"),         wxColor(147,112,219));
    wxTheColourDatabase->AddColour(wxT("MediumSeaGreen"),       wxColor(60,179,113));
    wxTheColourDatabase->AddColour(wxT("MediumSlateBlue"),      wxColor(123,104,238));
    wxTheColourDatabase->AddColour(wxT("MediumSpringGreen"),    wxColor(0,250,154));
    wxTheColourDatabase->AddColour(wxT("MediumTurquoise"),      wxColor(72,209,204));
    wxTheColourDatabase->AddColour(wxT("MediumVioletRed"),      wxColor(199,21,133));
    wxTheColourDatabase->AddColour(wxT("MidnightBlue"),         wxColor(25,25,112));
    wxTheColourDatabase->AddColour(wxT("MintCream"),            wxColor(245,255,250));
    wxTheColourDatabase->AddColour(wxT("MistyRose"),            wxColor(255,228,225));
    wxTheColourDatabase->AddColour(wxT("Moccasin"),             wxColor(255,228,181));
    wxTheColourDatabase->AddColour(wxT("NavajoWhite"),          wxColor(255,222,173));
    wxTheColourDatabase->AddColour(wxT("Navy"),                 wxColor(0,0,128));
    wxTheColourDatabase->AddColour(wxT("OldLace"),              wxColor(253,245,230));
    wxTheColourDatabase->AddColour(wxT("Olive"),                wxColor(128,128,0));
    wxTheColourDatabase->AddColour(wxT("OliveDrab"),            wxColor(107,142,35));
    wxTheColourDatabase->AddColour(wxT("Orange"),               wxColor(255,165,0));
    wxTheColourDatabase->AddColour(wxT("OrangeRed"),            wxColor(255,69,0));
    wxTheColourDatabase->AddColour(wxT("Orchid"),               wxColor(218,112,214));
    wxTheColourDatabase->AddColour(wxT("PaleGoldenRod"),        wxColor(238,232,170));
    wxTheColourDatabase->AddColour(wxT("PaleGreen"),            wxColor(152,251,152));
    wxTheColourDatabase->AddColour(wxT("PaleTurquoise"),        wxColor(175,238,238));
    wxTheColourDatabase->AddColour(wxT("PaleVioletRed"),        wxColor(219,112,147));
    wxTheColourDatabase->AddColour(wxT("PapayaWhip"),           wxColor(255,239,213));
    wxTheColourDatabase->AddColour(wxT("PeachPuff"),            wxColor(255,218,185));
    wxTheColourDatabase->AddColour(wxT("Peru"),                 wxColor(205,133,63));
    wxTheColourDatabase->AddColour(wxT("Pink"),                 wxColor(255,192,203));
    wxTheColourDatabase->AddColour(wxT("Plum"),                 wxColor(221,160,221));
    wxTheColourDatabase->AddColour(wxT("PowderBlue"),           wxColor(176,224,230));
    wxTheColourDatabase->AddColour(wxT("Purple"),               wxColor(128,0,128));
    wxTheColourDatabase->AddColour(wxT("Red"),                  wxColor(255,0,0));
    wxTheColourDatabase->AddColour(wxT("RosyBrown"),            wxColor(188,143,143));
    wxTheColourDatabase->AddColour(wxT("RoyalBlue"),            wxColor(65,105,225));
    wxTheColourDatabase->AddColour(wxT("SaddleBrown"),          wxColor(139,69,19));
    wxTheColourDatabase->AddColour(wxT("Salmon"),               wxColor(250,128,114));
    wxTheColourDatabase->AddColour(wxT("SandyBrown"),           wxColor(244,164,96));
    wxTheColourDatabase->AddColour(wxT("SeaGreen"),             wxColor(46,139,87));
    wxTheColourDatabase->AddColour(wxT("SeaShell"),             wxColor(255,245,238));
    wxTheColourDatabase->AddColour(wxT("Sienna"),               wxColor(160,82,45));
    wxTheColourDatabase->AddColour(wxT("Silver"),               wxColor(192,192,192));
    wxTheColourDatabase->AddColour(wxT("SkyBlue"),              wxColor(135,206,235));
    wxTheColourDatabase->AddColour(wxT("SlateBlue"),            wxColor(106,90,205));
    wxTheColourDatabase->AddColour(wxT("SlateGray"),            wxColor(112,128,144));
    wxTheColourDatabase->AddColour(wxT("Snow"),                 wxColor(255,250,250));
    wxTheColourDatabase->AddColour(wxT("SpringGreen"),          wxColor(0,255,127));
    wxTheColourDatabase->AddColour(wxT("SteelBlue"),            wxColor(70,130,180));
    wxTheColourDatabase->AddColour(wxT("Tan"),                  wxColor(210,180,140));
    wxTheColourDatabase->AddColour(wxT("Teal"),                 wxColor(0,128,128));
    wxTheColourDatabase->AddColour(wxT("Thistle"),              wxColor(216,191,216));
    wxTheColourDatabase->AddColour(wxT("Tomato"),               wxColor(255,99,71));
    wxTheColourDatabase->AddColour(wxT("Turquoise"),            wxColor(64,224,208));
    wxTheColourDatabase->AddColour(wxT("Violet"),               wxColor(238,130,238));
    wxTheColourDatabase->AddColour(wxT("VioletRed"),            wxColor(208,32,144));
    wxTheColourDatabase->AddColour(wxT("Wheat"),                wxColor(245,222,179));
    wxTheColourDatabase->AddColour(wxT("White"),                wxColor(255,255,255));
    wxTheColourDatabase->AddColour(wxT("WhiteSmoke"),           wxColor(245,245,245));
    wxTheColourDatabase->AddColour(wxT("Yellow"),               wxColor(255,255,0));
    wxTheColourDatabase->AddColour(wxT("YellowGreen"),          wxColor(154,205,50));

}

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


wxString FormatThousands(const wxString& s) 
{ 
    /* 
    // example: 
    int mynumber = 12345678; 
    wxString s = wxString::Format("%d", mynumber); // format the integer to string 
    s = FormatThousands(s); // add separators 
    // s now contains "12,345,678" or "12.345.678" according to locale. 
    */

    static wxString thousandssep = wxT(","); 
    static struct lconv *loc = 0; 
    if (!loc) { 
        loc = localeconv(); 
        if (loc && loc->thousands_sep && loc->thousands_sep[0]) 
        {
#if wxUSE_UNICODE
            thousandssep = wxString(loc->thousands_sep, wxConvLibc);
#else
            thousandssep = loc->thousands_sep;
#endif
        }
    } 

    wxString in = s, out; 
    while (in.Length() > 3) { 
            out.Prepend(thousandssep + in.Right(3)); 
            in.RemoveLast(3); 
    } 
    if (!in.IsEmpty()) 
            out.Prepend(in); 
    return out; 
} 

bool StrToInt64(wxString str, wxInt64 &i64)
{
    str.Trim(false);
    str.Trim(true);

    int base=10;
    str.MakeLower();
    if(str.Left(2)==wxT("0x")) 
    {
        base=16;
        str=str.Right(str.Len()-2);
    }

    bool ok=false;
    size_t i=0,len=str.Len();
    i64=0;
    while(i<len)
    {
        wxChar c=str[i];
        if(c<='9' && c>='0')
        {
            i64= (i64*base) + (c-'0');
        }
        else if(base==16 && c>='a' && c<='f')
        {
            i64= (i64*16) + (c-'a'+10);
        }
        else
        {
            return false;
        }

        ok=true;
        ++i;
    }

    return ok;
}

