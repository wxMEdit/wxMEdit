///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_app.h
// Description:
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// Licence:     GPLv3
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
