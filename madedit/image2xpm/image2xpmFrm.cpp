//---------------------------------------------------------------------------
//
// Name:        image2xpmFrm.cpp
// Author:      madedit@gmail.com
// Created:     2005/8/3 ¤U¤È 09:37:20
//
//---------------------------------------------------------------------------

#include <wx/image.h>
#include <wx/filename.h>
#include <wx/textfile.h>

#include "image2xpmFrm.h"
//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End


wxImage g_Image;
wxFileName g_filename;

class MyPanel: public wxPanel
{
    DECLARE_EVENT_TABLE()

public:    
    MyPanel(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxT("panel"))
        :wxPanel(parent, id, pos, size, style, name)
    {}
    
    void OnPaint(wxPaintEvent& event)
    {
        if(!g_Image.Ok()) 
        {
            event.Skip();
            return;
        }
            
            
        wxPaintDC dc(this);
        wxBitmap bmp(g_Image);
        
        dc.DrawBitmap(bmp, 10, 65, true);
        
        //event.Skip();       
    }

};
BEGIN_EVENT_TABLE(MyPanel,wxPanel)
    EVT_PAINT(MyPanel::OnPaint)
END_EVENT_TABLE()

//----------------------------------------------------------------------------
// image2xpmFrm
//----------------------------------------------------------------------------
   //Add Custom Events only in the appropriate Block.
   // Code added in  other places will be removed by wx-dvcpp 
  ////Event Table Start
BEGIN_EVENT_TABLE(image2xpmFrm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(image2xpmFrm::image2xpmFrmClose)
	EVT_BUTTON(ID_WXBUTTON2,image2xpmFrm::WxButton2Click)
	EVT_BUTTON(ID_WXBUTTON1,image2xpmFrm::WxButton1Click)
END_EVENT_TABLE()
  ////Event Table End



image2xpmFrm::image2xpmFrm( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxFrame( parent, id, title, position, size, style)
{
    
    
#if wxUSE_LIBPNG
  wxImage::AddHandler( new wxPNGHandler );
#endif

#if wxUSE_LIBJPEG
  wxImage::AddHandler( new wxJPEGHandler );
#endif

#if wxUSE_LIBTIFF
  wxImage::AddHandler( new wxTIFFHandler );
#endif

#if wxUSE_GIF
  wxImage::AddHandler( new wxGIFHandler );
#endif

#if wxUSE_PCX
  wxImage::AddHandler( new wxPCXHandler );
#endif

#if wxUSE_PNM
  wxImage::AddHandler( new wxPNMHandler );
#endif

#if wxUSE_XPM
  wxImage::AddHandler( new wxXPMHandler );
#endif

#if wxUSE_ICO_CUR
  wxImage::AddHandler( new wxICOHandler );
  wxImage::AddHandler( new wxCURHandler );
  wxImage::AddHandler( new wxANIHandler );
#endif    
    
    

    
    
    CreateGUIControls();
}

image2xpmFrm::~image2xpmFrm() {} 

void image2xpmFrm::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	wxBoxSizer* WxBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(TRUE);

	WxPanel1 = new MyPanel(this, ID_WXPANEL1, wxPoint(5,5), wxSize(240,240));
	WxBoxSizer1->Add(WxPanel1,1,wxGROW | wxALL,5);

	WxButton1 = new wxButton(WxPanel1, ID_WXBUTTON1, wxT("Load Image..."), wxPoint(7,8), wxSize(100,30), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(WxPanel1, ID_WXBUTTON2, wxT("Save as xpm..."), wxPoint(117,8), wxSize(100,30), 0, wxDefaultValidator, wxT("WxButton2"));

	WxStaticText1 = new wxStaticText(WxPanel1, ID_WXSTATICTEXT1, wxT("Image Info:"), wxPoint(10,43), wxSize(225,17), 0, wxT("WxStaticText1"));

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	this->SetTitle(wxT("image2xpm"));
	this->Center();
	this->SetIcon(wxNullIcon);
	
    ////GUI Items Creation End
    
    this->SetSize(300,300);
}

void image2xpmFrm::image2xpmFrmClose(wxCloseEvent& event)
{
    // --> Don't use Close with a Frame,
    // use Destroy instead.
    Destroy();
}
 

/*
 * WxButton1Click
 */
void image2xpmFrm::WxButton1Click(wxCommandEvent& event)
{
	wxString filename = wxFileSelector(_T("Select image file"));
    if ( !filename )
        return;

    
    if ( !g_Image.LoadFile(filename) )
    {
        wxLogError(_T("Couldn't load image from '%s'."), filename.c_str());

        return;
    }
    
    WxPanel1->Refresh(true);
    
    g_filename=filename;
    WxStaticText1->SetLabel(wxString::Format(wxT("%s: %dx%d"), 
        g_filename.GetFullName().c_str(), g_Image.GetWidth(), g_Image.GetHeight() ));
    
}

/*
 * WxButton2Click
 */
void image2xpmFrm::WxButton2Click(wxCommandEvent& event)
{
    if(!g_Image.Ok()) return;


    wxString savefilename = wxFileSelector( wxT("Save Image"),
                                            g_filename.GetPath(wxPATH_GET_VOLUME),
                                            g_filename.GetName(),
                                            (const wxChar *)NULL,
                                            wxT("XPM files (*.xpm)|*.xpm|")
                                            wxT("BMP files (*.bmp)|*.bmp|")
                                            wxT("PNG files (*.png)|*.png|")
                                            wxT("JPEG files (*.jpg)|*.jpg|")
                                            wxT("GIF files (*.gif)|*.gif|")
                                            wxT("TIFF files (*.tif)|*.tif|")
                                            wxT("PCX files (*.pcx)|*.pcx|")
                                            wxT("ICO files (*.ico)|*.ico|")
                                            wxT("CUR files (*.cur)|*.cur"),
                                            wxSAVE,
                                            this);

    if ( savefilename.empty() )
        return;

    wxString name, extension;
    wxFileName::SplitPath(savefilename, NULL, &name, &extension);

    bool saved = false;
    if ( extension == _T("bpp") )
    {
        static const int bppvalues[] =
        {
            wxBMP_1BPP,
            wxBMP_1BPP_BW,
            wxBMP_4BPP,
            wxBMP_8BPP,
            wxBMP_8BPP_GREY,
            wxBMP_8BPP_RED,
            wxBMP_8BPP_PALETTE,
            wxBMP_24BPP
        };

        const wxString bppchoices[] =
        {
            _T("1 bpp color"),
            _T("1 bpp B&W"),
            _T("4 bpp color"),
            _T("8 bpp color"),
            _T("8 bpp greyscale"),
            _T("8 bpp red"),
            _T("8 bpp own palette"),
            _T("24 bpp")
        };

        int bppselection = wxGetSingleChoiceIndex(_T("Set BMP BPP"),
                                                  _T("Image sample: save file"),
                                                  WXSIZEOF(bppchoices),
                                                  bppchoices,
                                                  this);
        if ( bppselection != -1 )
        {
            int format = bppvalues[bppselection];
            g_Image.SetOption(wxIMAGE_OPTION_BMP_FORMAT, format);

            if ( format == wxBMP_8BPP_PALETTE )
            {
                unsigned char *cmap = new unsigned char [256];
                for ( int i = 0; i < 256; i++ )
                    cmap[i] = (unsigned char)i;
                g_Image.SetPalette(wxPalette(256, cmap, cmap, cmap));

                delete[] cmap;
            }
        }
    }
    else if ( extension == _T("png") )
    {
        static const int pngvalues[] =
        {
            wxPNG_TYPE_COLOUR,
            wxPNG_TYPE_COLOUR,
            wxPNG_TYPE_GREY,
            wxPNG_TYPE_GREY,
            wxPNG_TYPE_GREY_RED,
            wxPNG_TYPE_GREY_RED,
        };

        const wxString pngchoices[] =
        {
            _T("Colour 8bpp"),
            _T("Colour 16bpp"),
            _T("Grey 8bpp"),
            _T("Grey 16bpp"),
            _T("Grey red 8bpp"),
            _T("Grey red 16bpp"),
        };

        int sel = wxGetSingleChoiceIndex(_T("Set PNG format"),
                                         _T("Image sample: save file"),
                                         WXSIZEOF(pngchoices),
                                         pngchoices,
                                         this);
        if ( sel != -1 )
        {
            g_Image.SetOption(wxIMAGE_OPTION_PNG_FORMAT, pngvalues[sel]);
            g_Image.SetOption(wxIMAGE_OPTION_PNG_BITDEPTH, sel % 2 ? 16 : 8);
        }
    }
    else if ( extension == _T("cur") )
    {
        g_Image.Rescale(32,32);
        g_Image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 0);
        g_Image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 0);
        // This shows how you can save an image with explicitly
        // specified image format:
        saved = g_Image.SaveFile(savefilename, wxBITMAP_TYPE_CUR);
    }

    if ( !saved )
    {
        // This one guesses image format from filename extension
        // (it may fail if the extension is not recognized):
        g_Image.SaveFile(savefilename);
    }	
	
    // save Alpha Channel data as text file like xpm format for C/C++.
    if(g_Image.HasAlpha())
    {
        wxString alphaname = savefilename + _T(".alpha");
        wxTextFile alpha;
        if( alpha.Create(alphaname) == false )
        {
            alpha.Open(alphaname);
            alpha.Clear(); // clear all data
        }

        wxString s1, s2;
        s1.Printf( _T("// %s.%s %d x %d"), name.c_str(), extension.c_str(), g_Image.GetWidth(), g_Image.GetHeight());
        alpha.AddLine(s1);
        s1.Printf( _T("unsigned char %s_alpha[] = {"), name.c_str());
        alpha.AddLine(s1);

        unsigned char *adata = g_Image.GetAlpha();

        for(int y=0; y<g_Image.GetHeight(); ++y)
        {
            s1.Empty();
            for(int x=0; x<g_Image.GetWidth(); ++x, ++adata)
            {
                s2.Printf( _T("%3d, "), *adata);
                s1 += s2;
            }
            alpha.AddLine(s1);
        }

        alpha.AddLine(wxString(_T("};")));
        alpha.Write();
    }

}
