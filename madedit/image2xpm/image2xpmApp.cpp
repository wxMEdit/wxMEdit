//---------------------------------------------------------------------------
//
// Name:        image2xpmApp.cpp
// Author:      madedit@gmail.com
// Created:     2005/8/3 ¤U¤È 09:37:20
//
//---------------------------------------------------------------------------

#include "image2xpmApp.h"
#include "image2xpmFrm.h"

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
