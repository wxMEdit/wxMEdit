///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit_app.h
// Description:
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_APP_H_
#define _WXMEDIT_APP_H_

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/wxprec.h>

#ifdef __BORLANDC__
# pragma hdrstop
#endif

#ifndef WX_PRECOMP
# include <wx/wx.h>
#endif
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class MadEditApp:public wxApp
{
public:
	bool OnInit();
	int OnExit();
};

 
#endif
