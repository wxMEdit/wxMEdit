
// madedit: changed DoDraw() function (2005/08/17)

///////////////////////////////////////////////////////////////////////////////
// Name:        generic/caret.h
// Purpose:     generic wxCaret class
// Author:      Vadim Zeitlin (original code by Robert Roebling)
// Modified by:
// Created:     25.05.99
// RCS-ID:      $Id: caret.h,v 1.13 2005/09/23 12:49:03 MR Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXMSW__

#ifndef _WX_CARET_NEW_H_
#define _WX_CARET_NEW_H_

#include "../xm/cxx11.h"


#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/timer.h>
#include <wx/caret.h>
#include <wx/bitmap.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

class wxCaretNew;

class wxCaretTimerNew : public wxTimer
{
public:
    wxCaretTimerNew(wxCaretNew *caret);
    virtual void Notify() override;

private:
    wxCaretNew *m_caret;
};

class wxCaretNew : public wxCaret//Base
{
    friend class MadEdit;       // for DoDraw()
public:
    // ctors
    // -----
        // default - use Create()
    wxCaretNew() : m_timer(this) { InitGeneric(); }
        // creates a block caret associated with the given window
    wxCaretNew(wxWindowBase *window, int width, int height)
        : wxCaret(window, width, height), m_timer(this) { InitGeneric(); }
    wxCaretNew(wxWindowBase *window, const wxSize& size)
        : wxCaret(window, size), m_timer(this) { InitGeneric(); }

    virtual ~wxCaretNew();

    // implementation
    // --------------

    // called by wxWindow (not using the event tables)
    virtual void OnSetFocus() override;
    virtual void OnKillFocus() override;

    // called by wxCaretTimerNew
    void OnTimer();

protected:
    virtual void DoShow() override;
    virtual void DoHide() override;
    virtual void DoMove() override;
    virtual void DoSize() override;

    // blink the caret once
    void Blink();

    // refresh the caret
    void Refresh();

    // draw the caret on the given DC
    void DoDraw(wxDC *dc);

private:
    // GTK specific initialization
    void InitGeneric();

    // the bitmap holding the part of window hidden by the caret when it was
    // at (m_xOld, m_yOld)
    wxBitmap      m_bmpUnderCaret;
    int           m_xOld,
                  m_yOld;

    wxCaretTimerNew  m_timer;
    bool          m_blinkedOut,     // true => caret hidden right now
                  m_hasFocus;       // true => our window has focus
};

#endif // _WX_CARET_NEW_H_

#endif // !__WXMSW__
