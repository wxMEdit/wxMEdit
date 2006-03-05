///////////////////////////////////////////////////////////////////////////////
// Name:        MadEditApp.h
// Description:
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADEDITAPP_H_
#define _MADEDITAPP_H_

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
