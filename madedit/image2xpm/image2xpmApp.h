//---------------------------------------------------------------------------
//
// Name:        image2xpmApp.h
// Author:      madedit@gmail.com
// Created:     2005/8/3 ¤U¤È 09:37:20
//
//---------------------------------------------------------------------------

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

class image2xpmFrmApp:public wxApp
{
public:
	bool OnInit();
	int OnExit();
};

 
