// Fixes primary selection problem in wxClipboard under Gtk
// patched from wx-gtk-clipbrd.patch:
// http://sourceforge.net/tracker/index.php?func=detail&aid=1280049&group_id=9863&atid=309863

/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/clipbrd.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: clipbrd.cpp,v 1.72 2006/05/02 10:38:37 ABX Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __WXGTK__

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#if wxUSE_CLIPBOARD

#include "clipbrd_gtk.h"

#ifndef WX_PRECOMP
    #include <wx/log.h>
    #include <wx/utils.h>
#endif

#include <wx/dataobj.h>

//[mad]#include "wx/gtk/private.h"
#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
class _wxGtkString_
{
public:
    explicit _wxGtkString_(gchar *s) : m_str(s) { }
    ~_wxGtkString_() { g_free(m_str); }

    const gchar *c_str() const { return m_str; }

    operator gchar *() const { return m_str; }

private:
    gchar *m_str;

    DECLARE_NO_COPY_CLASS(_wxGtkString_)
};


//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

GdkAtom  g_clipboardAtom   = 0;
GdkAtom  g_targetsAtom     = 0;
GdkAtom  g_timestampAtom   = 0;

#if wxUSE_UNICODE
extern GdkAtom g_altTextAtom;
#endif

// the trace mask we use with wxLogTrace() - call
// wxLog::AddTraceMask(TRACE_CLIPBOARD) to enable the trace messages from here
// (there will be a *lot* of them!)
static const wxChar *TRACE_CLIPBOARD = _T("clipboard");

//-----------------------------------------------------------------------------
// reminder
//-----------------------------------------------------------------------------

/* The contents of a selection are returned in a GtkSelectionData
   structure. selection/target identify the request.
   type specifies the type of the return; if length < 0, and
   the data should be ignored. This structure has object semantics -
   no fields should be modified directly, they should not be created
   directly, and pointers to them should not be stored beyond the duration of
   a callback. (If the last is changed, we'll need to add reference
   counting)

struct _GtkSelectionData
{
  GdkAtom selection;
  GdkAtom target;
  GdkAtom type;
  gint    format;
  guchar *data;
  gint    length;
};

*/

//-----------------------------------------------------------------------------
// "selection_received" for targets
//-----------------------------------------------------------------------------

extern "C" {
static void
targets_selection_received( GtkWidget *WXUNUSED(widget),
                            GtkSelectionData *selection_data,
                            guint32 WXUNUSED(time),
                            wxClipboardGtk *clipboard )
{
    if ( wxTheClipboard && selection_data->length > 0 )
    {
        // make sure we got the data in the correct form
        GdkAtom type = selection_data->type;
        if ( type != GDK_SELECTION_TYPE_ATOM )
        {
            if ( strcmp(_wxGtkString_(gdk_atom_name(type)), "TARGETS") )
            {
                wxLogTrace( TRACE_CLIPBOARD,
                            _T("got unsupported clipboard target") );

                clipboard->m_waiting = false;
                return;
            }
        }

#ifdef __WXDEBUG__
        wxDataFormat clip( selection_data->selection );
        wxLogTrace( TRACE_CLIPBOARD,
                    wxT("selection received for targets, clipboard %s"),
                    clip.GetId().c_str() );
#endif // __WXDEBUG__

        // the atoms we received, holding a list of targets (= formats)
        GdkAtom *atoms = (GdkAtom *)selection_data->data;

        for (unsigned int i=0; i<selection_data->length/sizeof(GdkAtom); i++)
        {
            wxDataFormat format( atoms[i] );

            wxLogTrace( TRACE_CLIPBOARD,
                        wxT("selection received for targets, format %s"),
                        format.GetId().c_str() );

//            printf( "format %s requested %s\n",
//                    gdk_atom_name( atoms[i] ),
//                    gdk_atom_name( clipboard->m_targetRequested ) );

            if (format == clipboard->m_targetRequested)
            {
                clipboard->m_waiting = false;
                clipboard->m_formatSupported = true;
                return;
            }
        }
    }

    clipboard->m_waiting = false;
}
}

//-----------------------------------------------------------------------------
// "selection_received" for the actual data
//-----------------------------------------------------------------------------

extern "C" {
static void
selection_received( GtkWidget *WXUNUSED(widget),
                    GtkSelectionData *selection_data,
                    guint32 WXUNUSED(time),
                    wxClipboardGtk *clipboard )
{
    if (!wxTheClipboard)
    {
        clipboard->m_waiting = false;
        return;
    }

    wxDataObject *data_object = clipboard->m_receivedData;

    if (!data_object)
    {
        clipboard->m_waiting = false;
        return;
    }

    if (selection_data->length <= 0)
    {
        clipboard->m_waiting = false;
        return;
    }

    wxDataFormat format( selection_data->target );

    // make sure we got the data in the correct format
    if (!data_object->IsSupportedFormat( format ) )
    {
        clipboard->m_waiting = false;
        return;
    }

#if 0
    This seems to cause problems somehow
    // make sure we got the data in the correct form (selection type).
    // if so, copy data to target object
    if (selection_data->type != GDK_SELECTION_TYPE_STRING)
    {
        clipboard->m_waiting = false;
        return;
    }
#endif

    data_object->SetData( format, (size_t) selection_data->length, (const char*) selection_data->data );

    wxTheClipboard->m_formatSupported = true;
    clipboard->m_waiting = false;
}
}

//-----------------------------------------------------------------------------
// "selection_clear"
//-----------------------------------------------------------------------------

extern "C" {
static gint
selection_clear_clip( GtkWidget *WXUNUSED(widget), GdkEventSelection *event )
{
    if (!wxTheClipboard) return TRUE;

    if (event->selection == GDK_SELECTION_PRIMARY)
    {
        wxTheClipboard->m_ownsPrimarySelection = false;

        if (wxTheClipboard->m_primarySelectionData)                                 //[mad]
        {                                                                           //[mad]
            wxLogTrace(TRACE_CLIPBOARD, wxT("Primary selection will get cleared" ));//[mad]
                                                                                    //[mad]
            delete wxTheClipboard->m_primarySelectionData;                          //[mad]
            wxTheClipboard->m_primarySelectionData = (wxDataObject*) NULL;          //[mad]
        }                                                                           //[mad]
    }
    else
    if (event->selection == g_clipboardAtom)
    {
        wxTheClipboard->m_ownsClipboard = false;

        if (wxTheClipboard->m_clipboardData)                                //[mad]
        {                                                                   //[mad]
            wxLogTrace(TRACE_CLIPBOARD, wxT("Clipboard will get cleared" ));//[mad]
                                                                            //[mad]
            delete wxTheClipboard->m_clipboardData;                         //[mad]
            wxTheClipboard->m_clipboardData = (wxDataObject*)NULL;          //[mad]
        }                                                                   //[mad]
    }
    else
    {
        wxTheClipboard->m_waiting = false;
        return FALSE;
    }

    if ((!wxTheClipboard->m_ownsPrimarySelection) &&
        (!wxTheClipboard->m_ownsClipboard))
    {
        /* the clipboard is no longer in our hands. we can the delete clipboard data. */
        //[mad]if (wxTheClipboard->m_data)
        //[mad]{
        //[mad]    wxLogTrace(TRACE_CLIPBOARD, wxT("wxClipboard will get cleared" ));
        //[mad]
        //[mad]    delete wxTheClipboard->m_data;
        //[mad]    wxTheClipboard->m_data = (wxDataObject*) NULL;
        //[mad]}
        if (wxTheClipboard->m_clipboardData)                                //[mad]
        {                                                                   //[mad]
            wxLogTrace(TRACE_CLIPBOARD, wxT("Clipboard will get cleared" ));//[mad]
                                                                            //[mad]
            delete wxTheClipboard->m_clipboardData;                         //[mad]
            wxTheClipboard->m_clipboardData = (wxDataObject*)NULL;          //[mad]
        }                                                                   //[mad]
        if (wxTheClipboard->m_primarySelectionData)                                 //[mad]
        {                                                                           //[mad]
            wxLogTrace(TRACE_CLIPBOARD, wxT("Primary selection will get cleared" ));//[mad]
                                                                                    //[mad]
            delete wxTheClipboard->m_primarySelectionData;                          //[mad]
            wxTheClipboard->m_primarySelectionData = (wxDataObject*) NULL;          //[mad]
        }                                                                           //[mad]
    }

    wxTheClipboard->m_waiting = false;
    return TRUE;
}
}

//-----------------------------------------------------------------------------
// selection handler for supplying data
//-----------------------------------------------------------------------------

extern "C" {
static void
selection_handler( GtkWidget *WXUNUSED(widget),
                   GtkSelectionData *selection_data,
                   guint WXUNUSED(info),
                   guint WXUNUSED(time),
                   gpointer signal_data )
{
    if (!wxTheClipboard) return;

    //[mad]if (!wxTheClipboard->m_data) return;
    wxDataObject *data;//[mad]

    //[mad]wxDataObject *data = wxTheClipboard->m_data;
    if (selection_data->selection == GDK_SELECTION_PRIMARY)//[mad]
        data = wxTheClipboard->m_primarySelectionData;     //[mad]
    else                                                   //[mad]
        data = wxTheClipboard->m_clipboardData;            //[mad]
                                                           //[mad]
    if (!data) return;                                     //[mad]

    // ICCCM says that TIMESTAMP is a required atom.
    // In particular, it satisfies Klipper, which polls
    // TIMESTAMP to see if the clipboards content has changed.
    // It shall return the time which was used to set the data.
    if (selection_data->target == g_timestampAtom)
    {
        guint timestamp = GPOINTER_TO_UINT (signal_data);
        gtk_selection_data_set(selection_data,
                               GDK_SELECTION_TYPE_INTEGER,
                               32,
                               (guchar*)&(timestamp),
                               sizeof(timestamp));
        wxLogTrace(TRACE_CLIPBOARD,
                   _T("Clipboard TIMESTAMP requested, returning timestamp=%u"),
                   timestamp);
        return;
    }

    wxDataFormat format( selection_data->target );

#ifdef __WXDEBUG__
    wxLogTrace(TRACE_CLIPBOARD,
               _T("clipboard data in format %s, GtkSelectionData is target=%s type=%s selection=%s timestamp=%u"),
               format.GetId().c_str(),
               wxString::FromAscii(_wxGtkString_(gdk_atom_name(selection_data->target))).c_str(),
               wxString::FromAscii(_wxGtkString_(gdk_atom_name(selection_data->type))).c_str(),
               wxString::FromAscii(_wxGtkString_(gdk_atom_name(selection_data->selection))).c_str(),
               GPOINTER_TO_UINT( signal_data )
               );
#endif

    if (!data->IsSupportedFormat( format )) return;

    int size = data->GetDataSize( format );

    if (size == 0) return;

    void *d = malloc(size);

    // Text data will be in UTF8 in Unicode mode.
    data->GetDataHere( selection_data->target, d );

    // NB: GTK+ requires special treatment of UTF8_STRING data, the text
    //     would show as UTF-8 data interpreted as latin1 (?) in other
    //     GTK+ apps if we used gtk_selection_data_set()
    if (format == wxDataFormat(wxDF_UNICODETEXT))
    {
        gtk_selection_data_set_text(
            selection_data,
            (const gchar*)d,
            size );
    }
    else
    {
        gtk_selection_data_set(
            selection_data,
            GDK_SELECTION_TYPE_STRING,
            8*sizeof(gchar),
            (unsigned char*) d,
            size );
    }

    free(d);
}
}

//-----------------------------------------------------------------------------
// wxClipboardGtk
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClipboardGtk,wxObject)

wxClipboardGtk::wxClipboardGtk()
{
    m_open = false;
    m_waiting = false;

    m_ownsClipboard = false;
    m_ownsPrimarySelection = false;

    //[mad]m_data = (wxDataObject*) NULL;
    m_primarySelectionData = (wxDataObject*) NULL;//[mad]
    m_clipboardData = (wxDataObject*) NULL;       //[mad]
    m_receivedData = (wxDataObject*) NULL;

    /* we use m_targetsWidget to query what formats are available */

    m_targetsWidget = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_widget_realize( m_targetsWidget );

    g_signal_connect (m_targetsWidget, "selection_received",
                      G_CALLBACK (targets_selection_received), this);

    /* we use m_clipboardWidget to get and to offer data */

    m_clipboardWidget = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_widget_realize( m_clipboardWidget );

    g_signal_connect (m_clipboardWidget, "selection_received",
                      G_CALLBACK (selection_received), this);

    g_signal_connect (m_clipboardWidget, "selection_clear_event",
                      G_CALLBACK (selection_clear_clip), NULL);

    if (!g_clipboardAtom) g_clipboardAtom = gdk_atom_intern( "CLIPBOARD", FALSE );
    if (!g_targetsAtom) g_targetsAtom = gdk_atom_intern ("TARGETS", FALSE);
    if (!g_timestampAtom) g_timestampAtom = gdk_atom_intern ("TIMESTAMP", FALSE);

    m_formatSupported = false;
    m_targetRequested = 0;

    m_usePrimary = false;
}

wxClipboardGtk::~wxClipboardGtk()
{
    m_usePrimary = false;//[mad]
    Clear();             //[mad]
    m_usePrimary = true; //[mad]
    Clear();

    if (m_clipboardWidget) gtk_widget_destroy( m_clipboardWidget );
    if (m_targetsWidget) gtk_widget_destroy( m_targetsWidget );
}

void wxClipboardGtk::Clear()
{
    //[mad]if (m_data)
    if (((m_usePrimary) && (m_primarySelectionData)) ||//[mad]
            ((!m_usePrimary) && (m_clipboardData)))    //[mad]
    {
#if wxUSE_THREADS
        /* disable GUI threads */
#endif

        //  As we have data we also own the clipboard. Once we no longer own
        //  it, clear_selection is called which will set m_data to zero
        
        if ((!m_usePrimary) && (m_clipboardData))//[mad]
        {                                        //[mad]
        if (gdk_selection_owner_get( g_clipboardAtom ) == m_clipboardWidget->window)
        {
            m_waiting = true;

            gtk_selection_owner_set( (GtkWidget*) NULL, g_clipboardAtom,
                                     (guint32) GDK_CURRENT_TIME );

            while (m_waiting) gtk_main_iteration();
        }
        delete m_clipboardData;                //[mad]
        m_clipboardData = (wxDataObject*) NULL;//[mad]
        }                                      //[mad]

        if ((m_usePrimary) && (m_primarySelectionData))//[mad]
        {                                              //[mad]
        if (gdk_selection_owner_get( GDK_SELECTION_PRIMARY ) == m_clipboardWidget->window)
        {
            m_waiting = true;

            gtk_selection_owner_set( (GtkWidget*) NULL, GDK_SELECTION_PRIMARY,
                                     (guint32) GDK_CURRENT_TIME );

            while (m_waiting) gtk_main_iteration();
        }
        delete m_primarySelectionData;                //[mad]
        m_primarySelectionData = (wxDataObject*) NULL;//[mad]
        }                                             //[mad]

        //[mad]if (m_data)
        //[mad]{
        //[mad]    delete m_data;
        //[mad]    m_data = (wxDataObject*) NULL;
        //[mad]}

#if wxUSE_THREADS
        /* re-enable GUI threads */
#endif
    }

    m_targetRequested = 0;
    m_formatSupported = false;
}

bool wxClipboardGtk::Open()
{
    wxCHECK_MSG( !m_open, false, wxT("clipboard already open") );

    m_open = true;

    return true;
}

bool wxClipboardGtk::SetData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    wxCHECK_MSG( data, false, wxT("data is invalid") );

    Clear();

    return AddData( data );
}

bool wxClipboardGtk::AddData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    wxCHECK_MSG( data, false, wxT("data is invalid") );

    // we can only store one wxDataObject
    Clear();

    //[mad]m_data = data;

    // get formats from wxDataObjects
    //[mad]wxDataFormat *array = new wxDataFormat[ m_data->GetFormatCount() ];
    //[mad]m_data->GetAllFormats( array );
    wxDataFormat *array = new wxDataFormat[ data->GetFormatCount() ];
    data->GetAllFormats( array );

    // primary selection or clipboard
    GdkAtom clipboard = m_usePrimary ? (GdkAtom)GDK_SELECTION_PRIMARY
                                     : g_clipboardAtom;

    // by default provide TIMESTAMP as a target
    gtk_selection_add_target( GTK_WIDGET(m_clipboardWidget),
                              clipboard,
                              g_timestampAtom,
                              0 );

    //[mad]for (size_t i = 0; i < m_data->GetFormatCount(); i++)
    for (size_t i = 0; i < data->GetFormatCount(); i++)//[mad]
    {
        wxLogTrace( TRACE_CLIPBOARD,
                    wxT("wxClipboardGtk now supports atom %s"), //[mad]
                    array[i].GetId().c_str() );

//        printf( "added %s\n",
//                    gdk_atom_name( array[i].GetFormatId() ) );

        gtk_selection_add_target( GTK_WIDGET(m_clipboardWidget),
                                  clipboard,
                                  array[i],
                                  0 );  /* what is info ? */
    }

    delete[] array;

    g_signal_connect (m_clipboardWidget, "selection_get",
                      G_CALLBACK (selection_handler),
                      GUINT_TO_POINTER (gtk_get_current_event_time()) );

#if wxUSE_THREADS
    /* disable GUI threads */
#endif

    /* Tell the world we offer clipboard data */
    bool res = (gtk_selection_owner_set( m_clipboardWidget,
                                         clipboard,
                                         (guint32) GDK_CURRENT_TIME ));

    if (m_usePrimary)
    {                                    //[mad]
        m_primarySelectionData = data;   //[mad]
        m_ownsPrimarySelection = res;    //[mad]
    }                                    //[mad]
    else
    {                                    //[mad]
        m_clipboardData = data;          //[mad]
        m_ownsClipboard = res;           //[mad]
    }                                    //[mad]

#if wxUSE_THREADS
    /* re-enable GUI threads */
#endif

    return res;
}

void wxClipboardGtk::Close()
{
    wxCHECK_RET( m_open, wxT("clipboard not open") );

    m_open = false;
}

bool wxClipboardGtk::IsOpened() const
{
    return m_open;
}

bool wxClipboardGtk::IsSupported( const wxDataFormat& format )
{
    /* reentrance problems */
    if (m_waiting) return false;

    /* store requested format to be asked for by callbacks */
    m_targetRequested = format;

    wxLogTrace( TRACE_CLIPBOARD,
                wxT("wxClipboardGtk:IsSupported: requested format: %s"),
                format.GetId().c_str() );

    wxCHECK_MSG( m_targetRequested, false, wxT("invalid clipboard format") );

    m_formatSupported = false;

    /* perform query. this will set m_formatSupported to
       true if m_targetRequested is supported.
       also, we have to wait for the "answer" from the
       clipboard owner which is an asynchronous process.
       therefore we set m_waiting = true here and wait
       until the callback "targets_selection_received"
       sets it to false */

    m_waiting = true;

    gtk_selection_convert( m_targetsWidget,
                           m_usePrimary ? (GdkAtom)GDK_SELECTION_PRIMARY
                                        : g_clipboardAtom,
                           g_targetsAtom,
                           (guint32) GDK_CURRENT_TIME );

    while (m_waiting) gtk_main_iteration();

#if wxUSE_UNICODE
    if (!m_formatSupported && format == wxDataFormat(wxDF_UNICODETEXT))
    {
        // Another try with plain STRING format
        extern GdkAtom g_altTextAtom;
        return IsSupported(g_altTextAtom);
    }
#endif

    return m_formatSupported;
}

bool wxClipboardGtk::GetData( wxDataObject& data )
{
    wxCHECK_MSG( m_open, false, wxT("clipboard not open") );

    /* get formats from wxDataObjects */
    wxDataFormat *array = new wxDataFormat[ data.GetFormatCount() ];
    data.GetAllFormats( array );

    for (size_t i = 0; i < data.GetFormatCount(); i++)
    {
        wxDataFormat format( array[i] );

        wxLogTrace( TRACE_CLIPBOARD,
                    wxT("wxClipboardGtk::GetData: requested format: %s"),
                    format.GetId().c_str() );

        /* is data supported by clipboard ? */

        /* store requested format to be asked for by callbacks */
        m_targetRequested = format;

        wxCHECK_MSG( m_targetRequested, false, wxT("invalid clipboard format") );

        m_formatSupported = false;

       /* perform query. this will set m_formatSupported to
          true if m_targetRequested is supported.
          also, we have to wait for the "answer" from the
          clipboard owner which is an asynchronous process.
          therefore we set m_waiting = true here and wait
          until the callback "targets_selection_received"
          sets it to false */

        m_waiting = true;

        gtk_selection_convert( m_targetsWidget,
                           m_usePrimary ? (GdkAtom)GDK_SELECTION_PRIMARY
                                        : g_clipboardAtom,
                           g_targetsAtom,
                           (guint32) GDK_CURRENT_TIME );

        while (m_waiting) gtk_main_iteration();

        if (!m_formatSupported) continue;

        /* store pointer to data object to be filled up by callbacks */
        m_receivedData = &data;

        /* store requested format to be asked for by callbacks */
        m_targetRequested = format;

        wxCHECK_MSG( m_targetRequested, false, wxT("invalid clipboard format") );

        /* start query */
        m_formatSupported = false;

        /* ask for clipboard contents.  this will set
           m_formatSupported to true if m_targetRequested
           is supported.
           also, we have to wait for the "answer" from the
           clipboard owner which is an asynchronous process.
           therefore we set m_waiting = true here and wait
           until the callback "targets_selection_received"
           sets it to false */

        m_waiting = true;

        wxLogTrace( TRACE_CLIPBOARD,
                    wxT("wxClipboardGtk::GetData: format found, start convert") );

        gtk_selection_convert( m_clipboardWidget,
                               m_usePrimary ? (GdkAtom)GDK_SELECTION_PRIMARY
                                            : g_clipboardAtom,
                               m_targetRequested,
                               (guint32) GDK_CURRENT_TIME );

        while (m_waiting) gtk_main_iteration();

        /* this is a true error as we checked for the presence of such data before */
        wxCHECK_MSG( m_formatSupported, false, wxT("error retrieving data from clipboard") );

        /* return success */
        delete[] array;
        return true;
    }

    wxLogTrace( TRACE_CLIPBOARD,
                wxT("wxClipboardGtk::GetData: format not found") );

    /* return failure */
    delete[] array;
    return false;
}


wxClipboardGtk *g_ClipboardGtk=NULL;
wxClipboardGtk *GetClipboardGtk()
{
    if(g_ClipboardGtk==NULL)
    {
        g_ClipboardGtk=new wxClipboardGtk;
    }
    return g_ClipboardGtk;
}


#endif
  // wxUSE_CLIPBOARD

#endif
  // __WXGTK__
