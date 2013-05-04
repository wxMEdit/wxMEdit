//---------------------------------------------------------------------------
//
// Name:        image2xpm_app.cpp
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Created:     2005-8-3 21:37:20
//
//---------------------------------------------------------------------------

#include "image2xpm_app.h"
#include "image2xpm_frm.h"

IMPLEMENT_APP(image2xpmFrmApp)

bool image2xpmFrmApp::OnInit()
{
    image2xpmFrm *myFrame = new  image2xpmFrm(NULL);
    SetTopWindow(myFrame);
    myFrame->Show(TRUE);		
    return TRUE;
}
 
int image2xpmFrmApp::OnExit()
{
	return 0;
}
