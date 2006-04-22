// Fixes primary selection problem in wxClipboard under Gtk
// patched from wx-gtk-clipbrd.patch:
// http://sourceforge.net/tracker/index.php?func=detail&aid=1280049&group_id=9863&atid=309863

/////////////////////////////////////////////////////////////////////////////
// Name:        clipboard.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: clipbrd.h,v 1.22 2005/09/23 12:49:14 MR Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

#ifndef _CLIPBRD_GTK_H_
#define _CLIPBRD_GTK_H_

#if wxUSE_CLIPBOARD

#include <wx/object.h>
#include <wx/list.h>
#include <wx/dataobj.h>
#include <wx/control.h>
#include <wx/module.h>
#include <wx/clipbrd.h>

// ----------------------------------------------------------------------------
// wxClipboardGtk
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxClipboardGtk : public wxClipboardBase
{
public:
    wxClipboardGtk();
    ~wxClipboardGtk();

    // open the clipboard before SetData() and GetData()
    virtual bool Open();

    // close the clipboard after SetData() and GetData()
    virtual void Close();

    // query whether the clipboard is opened
    virtual bool IsOpened() const;

    // set the clipboard data. all other formats will be deleted.
    virtual bool SetData( wxDataObject *data );

    // add to the clipboard data.
    virtual bool AddData( wxDataObject *data );

    // ask if data in correct format is available
    virtual bool IsSupported( const wxDataFormat& format );

    // fill data with data on the clipboard (if available)
    virtual bool GetData( wxDataObject& data );

    // clears wxTheClipboard and the system's clipboard if possible
    virtual void Clear();

    // If primary == TRUE, use primary selection in all further ops,
    // primary == FALSE resets it.
    virtual void UsePrimarySelection(bool primary = TRUE)
        { m_usePrimary = primary; }
    
    // implementation from now on
    bool              m_open;
    bool              m_ownsClipboard;
    bool              m_ownsPrimarySelection;
    //[Mad]wxDataObject     *m_data;
    wxDataObject     *m_clipboardData;//[Mad]
    wxDataObject     *m_primarySelectionData;//[Mad]

    GtkWidget        *m_clipboardWidget;  /* for getting and offering data */
    GtkWidget        *m_targetsWidget;    /* for getting list of supported formats */
    bool              m_waiting;          /* querying data or formats is asynchronous */

    bool              m_formatSupported;
    GdkAtom           m_targetRequested;
    bool              m_usePrimary;
    wxDataObject     *m_receivedData;

private:
    DECLARE_DYNAMIC_CLASS(wxClipboardGtk)
};


extern wxClipboardGtk *GetClipboardGtk();
#ifdef wxTheClipboard
#undef wxTheClipboard
#endif
#define wxTheClipboard GetClipboardGtk()


#endif
   // wxUSE_CLIPBOARD

#endif
    // _CLIPBRD_GTK_H_

#endif
    // __WXGTK__
