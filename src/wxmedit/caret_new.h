///////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/caret.h
// Purpose:     generic wxCaret class
// Author:      Vadim Zeitlin (original code by Robert Roebling)
// Modified by: madedit 2005-08-17, changed DoDraw() function
//              JiaYanwei 2018-03-03, synchronized with wxWidgets 3.0
// Created:     25.05.99
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXMEDIT_CARET_NEW_H_
#define _WXMEDIT_CARET_NEW_H_

#include "../xm/cxx11.h"

#include <wx/timer.h>
#include <wx/caret.h>

class wxCaretNew;

class wxCaretTimerNew : public wxTimer
{
public:
    wxCaretTimerNew(wxCaretNew *caret);
    virtual void Notify() override;

private:
    wxCaretNew *m_caret;
};

class wxCaretNew : public wxCaret
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

#endif // _WXMEDIT_CARET_NEW_H_
