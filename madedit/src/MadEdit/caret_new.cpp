
// madedit: changed DoDraw() function (2005/08/17)

///////////////////////////////////////////////////////////////////////////////
// Name:        generic/caret.cpp
// Purpose:     generic wxCaret class implementation
// Author:      Vadim Zeitlin (original code by Robert Roebling)
// Modified by:
// Created:     25.05.99
// RCS-ID:      $Id: caret.cpp,v 1.15 2005/09/23 12:53:25 MR Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CARET

#ifndef WX_PRECOMP
    #include <wx/window.h>
    #include <wx/dcclient.h>
    #include <wx/dcmemory.h>
#endif //WX_PRECOMP

#include <wx/caret.h>

#include "caret_new.h"

// ----------------------------------------------------------------------------
// global variables for this module
// ----------------------------------------------------------------------------

// the blink time (common to all carets for MSW compatibility)
static int gs_blinkTime = 500;  // in milliseconds

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// timer stuff
// ----------------------------------------------------------------------------

wxCaretTimerNew::wxCaretTimerNew(wxCaretNew *caret)
{
    m_caret = caret;
}

void wxCaretTimerNew::Notify()
{
    m_caret->OnTimer();
}

void wxCaretNew::OnTimer()
{
    // don't blink the caret when we don't have the focus
    if ( m_hasFocus )
        Blink();
}

// ----------------------------------------------------------------------------
// wxCaretNew static functions and data
// ----------------------------------------------------------------------------
/***
int wxCaretBase::GetBlinkTime()
{
    return gs_blinkTime;
}

void wxCaretBase::SetBlinkTime(int milliseconds)
{
    gs_blinkTime = milliseconds;
}
***/
// ----------------------------------------------------------------------------
// initialization and destruction
// ----------------------------------------------------------------------------

void wxCaretNew::InitGeneric()
{
    m_hasFocus = true;
    m_blinkedOut = true;

    m_xOld =
    m_yOld = -1;
    m_bmpUnderCaret.Create(m_width, m_height);
}

wxCaretNew::~wxCaretNew()
{
    if ( IsVisible() )
    {
        // stop blinking
        if ( m_timer.IsRunning() )
            m_timer.Stop();
    }
}

// ----------------------------------------------------------------------------
// showing/hiding/moving the caret (base class interface)
// ----------------------------------------------------------------------------

void wxCaretNew::DoShow()
{
    int blinkTime = GetBlinkTime();
    if ( blinkTime )
        m_timer.Start(blinkTime);

    if ( m_blinkedOut )
        Blink();
}

void wxCaretNew::DoHide()
{
    m_timer.Stop();

    if ( !m_blinkedOut )
    {
        Blink();
    }
}

void wxCaretNew::DoMove()
{
    if ( IsVisible() )
    {
        if ( !m_blinkedOut )
        {
            // hide it right now and it will be shown the next time it blinks
            Blink();

            // but if the caret is not blinking, we should blink it back into
            // visibility manually
            if ( !m_timer.IsRunning() )
                Blink();
        }
    }
    //else: will be shown at the correct location when it is shown
}

void wxCaretNew::DoSize()
{
    int countVisible = m_countVisible;
    if (countVisible > 0)
    {
        m_countVisible = 0;
        DoHide();
    }
    // Change bitmap size
    m_bmpUnderCaret = wxBitmap(m_width, m_height);
    if (countVisible > 0)
    {
        m_countVisible = countVisible;
        DoShow();
    }
}

// ----------------------------------------------------------------------------
// handling the focus
// ----------------------------------------------------------------------------

void wxCaretNew::OnSetFocus()
{
    m_hasFocus = true;

    if ( IsVisible() )
        Refresh();
}

void wxCaretNew::OnKillFocus()
{
    m_hasFocus = false;

    if ( IsVisible() )
    {
        // the caret must be shown - otherwise, if it is hidden now, it will
        // stay so until the focus doesn't return because it won't blink any
        // more

        // hide it first if it isn't hidden ...
        if ( !m_blinkedOut )
            Blink();

        // .. and show it in the new style
        Blink();
    }
}

// ----------------------------------------------------------------------------
// drawing the caret
// ----------------------------------------------------------------------------

void wxCaretNew::Blink()
{
    m_blinkedOut = !m_blinkedOut;

    Refresh();
}

void wxCaretNew::Refresh()
{
    wxClientDC dcWin(GetWindow());
    wxMemoryDC dcMem;
    dcMem.SelectObject(m_bmpUnderCaret);
    if ( m_blinkedOut )
    {
        // restore the old image
        dcWin.Blit(m_xOld, m_yOld, m_width, m_height,
                   &dcMem, 0, 0);
        m_xOld =
        m_yOld = -1;
    }
    else
    {
        if ( m_xOld == -1 && m_yOld == -1 )
        {
            // save the part we're going to overdraw

            int x = m_x,
                y = m_y;
#if defined(__WXGTK__) && !defined(__WX_DC_BLIT_FIXED__)
            wxPoint pt = dcWin.GetDeviceOrigin();
            x += pt.x;
            y += pt.y;
#endif // broken wxGTK wxDC::Blit
            dcMem.Blit(0, 0, m_width, m_height,
                       &dcWin, x, y);

            m_xOld = m_x;
            m_yOld = m_y;
        }
        //else: we already saved the image below the caret, don't do it any
        //      more

        // and draw the caret there
        DoDraw(&dcWin);
    }
}


#include "MadEdit.h"

#ifndef FIXINVERT
//#   ifdef __WXMSW__
#       define FIXINVERT 0
//#   else 
//#       define FIXINVERT 1
//#   endif
#endif

void wxCaretNew::DoDraw(wxDC *dc)
{

    if(m_hasFocus)
    {

#if   FIXINVERT == 0
        dc->Blit(m_x, m_y, m_width, m_height, dc, m_x, m_y, wxINVERT);
#elif FIXINVERT == 1
        MadEdit *me=(MadEdit*)GetWindow();
        (me->*(me->InvertRect))(dc, m_x, m_y, m_width, m_height);
#elif FIXINVERT == 2
        ((MadEdit*)GetWindow())->InvertRectManual(dc, m_x, m_y, m_width, m_height);
#else
#     error FIXINVERT not defined
#endif

    }
}

#endif // wxUSE_CARET
