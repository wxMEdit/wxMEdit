///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_app.h
// Description:
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_APP_H_
#define _WXMEDIT_APP_H_

#include <wx/wxprec.h>
#ifdef __BORLANDC__
        #pragma hdrstop
#endif
#ifndef WX_PRECOMP
        #include <wx/wx.h>
#endif

class MadEditApp:public wxApp
{
public:
	bool OnInit();
	int OnExit();
};

 
#endif
