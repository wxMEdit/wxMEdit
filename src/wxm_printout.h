///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_printout.h
// Description: Printing Functions
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_PRINTOUT_H_
#define _WXM_PRINTOUT_H_

#include "xm/cxx11.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/print.h>
#include <wx/intl.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class MadPrintout: public wxPrintout
{
    static int s_PrintoutCount;
public:
    MadPrintout(const wxString& title = _("wxMEdit Printout"));
    virtual ~MadPrintout();

    virtual void GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo) override;
    virtual bool HasPage(int page) override;
    virtual bool OnPrintPage(int page) override;
    
    virtual void OnPreparePrinting() override;
    
    void CalcPrintInfo(wxPageSetupData *pPageSetupData, double &xScale, double &yScale, wxRect &printRect);
    
    bool m_PrintHeader, m_PrintFooter;
    wxString m_HeaderLeft, m_HeaderCenter, m_HeaderRight;
    wxString m_FooterLeft, m_FooterCenter, m_FooterRight;
    int m_CharHeight, m_HeaderHeight, m_FooterHeight;
};

//extern wxPrintData *g_PrintData;
extern wxPageSetupData* g_PageSetupData;

#endif
