///////////////////////////////////////////////////////////////////////////////
// Name:		wxFlatNotebook.cpp 
// Purpose:     generic implementation of flat style notebook class.
// Author:      Eran Ifrah <eranif@bezeqint.net>
// Modified by: Priyank Bolia <soft@priyank.in>
// Created:     30/12/2005
// Modified:    01/01/2006
// Copyright:   Eran Ifrah (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
///////////////////////////////////////////////////////////////////////////////

#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/wxFlatNotebook/renderer.h>
#include <wx/wxFlatNotebook/popup_dlg.h>
#include <algorithm>
#include <wx/tooltip.h>
#include <wx/tipwin.h>
#include <wx/arrimpl.cpp>


#if wxCHECK_VERSION(2,8,0)
    #define Contains_ Contains
#else
    #define Contains_ Inside
#endif


#ifdef DEVELOPMENT
# define FNB_LOG_MSG( msg ) { wxString logmsg; logmsg << msg; wxLogMessage( logmsg ); }
#else
# define FNB_LOG_MSG( msg ) { wxString logmsg; logmsg << msg; }
#endif

#ifdef DEVELOPMENT
#include <map>
wxString WhereToString( int where )
{
	static std::map<int, wxString> whereMap;
	static bool first = true;

	if( first )
	{
		whereMap[wxFNB_TAB] = wxT("wxFNB_TAB");
		whereMap[wxFNB_X] = wxT("wxFNB_X");
		whereMap[wxFNB_TAB_X] = wxT("wxFNB_TAB_X");
		whereMap[wxFNB_LEFT_ARROW] = wxT("wxFNB_LEFT_ARROW");
		whereMap[wxFNB_RIGHT_ARROW] = wxT("wxFNB_RIGHT_ARROW");
		whereMap[wxFNB_DROP_DOWN_ARROW] = wxT("wxFNB_DROP_DOWN_ARROW");
		whereMap[wxFNB_NOWHERE] = wxT("wxFNB_NOWHERE");
		first = false;
	}
	return whereMap[where];
}
#endif 

//-------------------------------------------------------------------
// Provide user with a nice feedback when tab is being dragged
//-------------------------------------------------------------------
bool wxFNBDropSource::GiveFeedback(wxDragResult effect)
{
	wxUnusedVar(effect);
	static_cast<wxPageContainer*>( m_win )->DrawDragHint();
	return false;
}

IMPLEMENT_DYNAMIC_CLASS(wxFlatNotebookEvent, wxNotifyEvent)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CHANGING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_FLATNOTEBOOK_CONTEXT_MENU)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSED)

IMPLEMENT_DYNAMIC_CLASS(wxFlatNotebook, wxPanel)

WX_DEFINE_OBJARRAY(wxFlatNotebookImageList);
WX_DEFINE_OBJARRAY(wxPageInfoArray)
WX_DEFINE_OBJARRAY(wxWindowPtrArray)

BEGIN_EVENT_TABLE(wxFlatNotebook, wxPanel)
EVT_NAVIGATION_KEY(wxFlatNotebook::OnNavigationKey)
END_EVENT_TABLE()

wxFlatNotebook::wxFlatNotebook(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
	Init();
	Create(parent, id, pos, size, style, name);
}

void wxFlatNotebook::CleanUp ()
{
    wxFNBRendererMgrST::Free();
}

wxFlatNotebook::~wxFlatNotebook(void)
{
}

void wxFlatNotebook::Init()
{
    m_popupWin = NULL;
    m_sendPageChangeEvent = true;
	m_bForceSelection = false;
	m_nPadding = 6;
	m_nFrom = 0;
	m_pages = NULL;

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_mainSizer);
}

bool wxFlatNotebook::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                             long style, const wxString& name)
{
	style |= wxTAB_TRAVERSAL;
	wxPanel::Create(parent, id, pos, size, style, name);

	m_pages = new wxPageContainer(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	m_pages->m_colorBorder = wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

	m_mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_mainSizer);

	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));

	// Set default page height
	wxMemoryDC memDc;

	wxBitmap bmp(10, 10);
	memDc.SelectObject(bmp);

	int width, height;


#ifdef __WXGTK__
	// For GTK it seems that we must do this steps in order
	// for the tabs will get the proper height on initialization
	// on MSW, preforming these steps yields wierd results
	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
	memDc.SetFont( boldFont );
#endif

	wxString stam = wxT("Tp");	// Temp data to get the text height;
	memDc.GetTextExtent(stam, &width, &height);

	int tabHeight = height + wxFNB_HEIGHT_SPACER; // We use 8 pixels as padding
#ifdef __WXGTK__
	// On GTK the tabs are should be larger
	tabHeight += 6;
#endif
	m_pages->SetSizeHints(wxSize(-1, tabHeight));
	
	// Add the tab container to the sizer
	m_mainSizer->Insert(0, m_pages, 0, wxEXPAND);
	m_mainSizer->Layout();

	m_pages->m_nFrom = m_nFrom;
	m_pDropTarget = new wxFNBDropTarget<wxFlatNotebook>(this, &wxFlatNotebook::OnDropTarget);
	SetDropTarget(m_pDropTarget);
	return true;
}

void wxFlatNotebook::SetActiveTabTextColour(const wxColour& textColour)
{
	m_pages->m_activeTextColor = textColour;
}

wxDragResult wxFlatNotebook::OnDropTarget(wxCoord x, wxCoord y, int nTabPage, wxWindow * wnd_oldContainer)
{
	return m_pages->OnDropTarget(x, y, nTabPage, wnd_oldContainer);
}

int wxFlatNotebook::GetPreviousSelection() const
{
	return m_pages->m_iPreviousActivePage;
}

bool wxFlatNotebook::AddPage(wxWindow* window, const wxString& caption, const bool selected, const int imgindex)
{
	return InsertPage(m_windows.GetCount(), window, caption, selected, imgindex);
}

void wxFlatNotebook::SetImageList(wxFlatNotebookImageList * imglist)
{
	m_pages->SetImageList(imglist);
}

wxFlatNotebookImageList * wxFlatNotebook::GetImageList()
{
	return m_pages->GetImageList();
}

bool wxFlatNotebook::InsertPage(size_t index, wxWindow* page, const wxString& text, bool select, const int imgindex)
{
	// sanity check
	if (!page)
		return false;

	// reparent the window to us
	page->Reparent(this);

	if( !m_pages->IsShown() )
		m_pages->Show();

	index = FNB_MIN((unsigned int)index, (unsigned int)m_windows.GetCount());
	// Insert tab
	bool bSelected = select || m_windows.empty();
	int curSel = m_pages->GetSelection();

	if(index <= m_windows.GetCount())
	{
		m_windows.Insert(page, index);
		wxLogTrace(wxTraceMask(), wxT("New page inserted. Index = %i"), index);
	}
	else
	{
		m_windows.Add(page);
		wxLogTrace(wxTraceMask(), wxT("New page appended. Index = %i"), index);
	}

	if( !m_pages->InsertPage(index, page, text, bSelected, imgindex) )
		return false;

	if((int)index <= curSel) curSel++;

	Freeze();

	// Check if a new selection was made
	if(bSelected)
	{
		if(curSel >= 0)
		{
			// Remove the window from the main sizer
			m_mainSizer->Detach(m_windows[curSel]);
			m_windows[curSel]->Hide();
		}
		m_pages->SetSelection(index);
	}
	else
	{
		// Hide the page
		page->Hide();
	}
	m_mainSizer->Layout();
	Thaw();
	Refresh();

	return true;
}

void wxFlatNotebook::SetSelection(size_t page)
{
	if(page >= m_windows.GetCount())
		return;

	// Support for disabed tabs
	if(!m_pages->GetEnabled(page) && m_windows.GetCount() > 1 && !m_bForceSelection)
		return;

	if( m_sendPageChangeEvent )
	{
		// Allow the user to veto the selection
		int oldSelection = GetSelection();

		wxFlatNotebookEvent event(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CHANGING, GetId());
		event.SetSelection( (int)page );
		event.SetOldSelection( oldSelection );
		event.SetEventObject( this );
		GetEventHandler()->ProcessEvent(event);

		if( !event.IsAllowed() )
		{
			return;
		}
	}

	int curSel = m_pages->GetSelection();

	// program allows the page change
	Freeze();
	if(curSel >= 0)
	{
		// Remove the window from the main sizer
		m_mainSizer->Detach(m_windows[curSel]);
		m_windows[curSel]->Hide();
	}

	if(m_windowStyle & wxFNB_BOTTOM)
	{
		m_mainSizer->Insert(0, m_windows[page], 1, wxEXPAND);
	}
	else
	{
		// We leave a space of 1 pixel around the window
		m_mainSizer->Add(m_windows[page], 1, wxEXPAND);
	}

	m_windows[page]->Show();
	Thaw();
	m_mainSizer->Layout();

	if( page != (size_t)m_pages->m_iActivePage )
		//there is a real poge changing
		m_pages->m_iPreviousActivePage = m_pages->m_iActivePage;

	m_pages->m_iActivePage = (int)page;
	m_pages->DoSetSelection(page);

	if( m_sendPageChangeEvent )
	{
		// Fire event 'Page Changed'
		wxFlatNotebookEvent event(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CHANGED, GetId());
		event.SetSelection( (int)page );
		event.SetEventObject( this );
		GetEventHandler()->ProcessEvent(event);
	}
}

void wxFlatNotebook::DeletePage(size_t page, bool notify)
{
	if(page >= m_windows.GetCount())
		return;

	// Fire a closing event
	if( notify )
	{
		wxFlatNotebookEvent event(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSING, GetId());
		event.SetSelection((int)page);
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);


		// The event handler allows it?
		if (!event.IsAllowed())
			return;
	}

	Freeze();

	// Delete the requested page
	wxWindow *pageRemoved = m_windows[page];

	// If the page is the current window, remove it from the sizer
	// as well
	if((int)page == m_pages->GetSelection())
	{
		m_mainSizer->Detach(pageRemoved);
	}

	// Remove it from the array as well
	m_windows.RemoveAt(page);

	// Now we can destroy it; in wxWidgets use Destroy instead of delete
	pageRemoved->Destroy();

	Thaw();

	m_pages->DoDeletePage(page);
	Refresh();

	// Fire a closed event
	if( notify )
	{
		wxFlatNotebookEvent closedEvent(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSED, GetId());
		closedEvent.SetSelection((int)page);
		closedEvent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(closedEvent);
	}
}

bool wxFlatNotebook::DeleteAllPages()
{
	if(m_windows.empty())
		return false;

	Freeze();
	int i = 0;
	for(; i<(int)m_windows.GetCount(); i++)
	{
		delete m_windows[i];
	}

	m_windows.Clear();
	Thaw();

	// Clear the container of the tabs as well
	m_pages->DeleteAllPages();
	return true;
}

wxWindow* wxFlatNotebook::GetCurrentPage() const
{
	int sel = m_pages->GetSelection();
	if(sel < 0)
		return NULL;

	return m_windows[sel];
}

wxWindow* wxFlatNotebook::GetPage(size_t page) const
{
	if(page >= m_windows.GetCount())
		return NULL;

	return m_windows[page];
}

int wxFlatNotebook::GetPageIndex(wxWindow* win) const
{
	for (size_t i = 0; i < m_windows.GetCount(); ++i)
	{
		if (m_windows[i] == win)
			return (int)i;
	}
	return -1;
}

int wxFlatNotebook::GetSelection() const
{
	return m_pages->GetSelection();
}

void wxFlatNotebook::AdvanceSelection(bool bForward)
{
	m_pages->AdvanceSelection(bForward);
}

int wxFlatNotebook::GetPageCount() const
{
	return (int)m_pages->GetPageCount();
}

void wxFlatNotebook::OnNavigationKey(wxNavigationKeyEvent& event)
{
	if ( event.IsWindowChange() )
	{
		if( HasFlag(wxFNB_SMART_TABS) )
		{
			if( !m_popupWin )
			{
				m_popupWin = new wxTabNavigatorWindow( this );
				m_popupWin->ShowModal();
				m_popupWin->Destroy(); 
				m_popupWin = NULL;
			}
			else
			{
				// a dialog is already opened
				m_popupWin->OnNavigationKey( event );
				return;
			}
		}
		else
		{
			// change pages
			AdvanceSelection(event.GetDirection());
		}
	}
	else
	{
		// pass to the parent
		if ( GetParent() )
		{
			event.SetCurrentFocus(this);
			GetParent()->ProcessEvent(event);
		}
	}
}

bool wxFlatNotebook::GetPageShapeAngle(int page_index, unsigned int * result)
{
	if(page_index < 0 || page_index >= (int)m_pages->m_pagesInfoVec.GetCount()) return false;
	*result = m_pages->m_pagesInfoVec[page_index].GetTabAngle();
	return true;
}

void wxFlatNotebook::SetPageShapeAngle(int page_index, unsigned int angle)
{
	if(page_index < 0 || page_index >= (int)m_pages->m_pagesInfoVec.GetCount()) return;
	if(angle > 15) return;

	m_pages->m_pagesInfoVec[page_index].SetTabAngle(angle);
}

void wxFlatNotebook::SetAllPagesShapeAngle(unsigned int angle)
{
	if(angle > 15) return;
	for(unsigned int i = 0; i < m_pages->m_pagesInfoVec.GetCount(); i++)
	{
		m_pages->m_pagesInfoVec[i].SetTabAngle(angle);
	}
	Refresh();
}

wxSize wxFlatNotebook::GetPageBestSize()
{
	return m_pages->GetClientSize();
}

bool wxFlatNotebook::SetPageText(size_t page, const wxString& text)
{
	bool bVal = m_pages->SetPageText(page, text);
	m_pages->Refresh();
	return bVal;
}

void wxFlatNotebook::SetPadding(const wxSize& padding)
{
	m_nPadding = padding.GetWidth();
}

void wxFlatNotebook::SetWindowStyleFlag(long style)
{
	wxPanel::SetWindowStyleFlag(style);

	if(m_pages)
	{
		// For changing the tab position (i.e. placing them top/bottom)
		// refreshing the tab container is not enough
		m_sendPageChangeEvent = false;
		SetSelection(m_pages->m_iActivePage);
		m_sendPageChangeEvent = true;
	}
}

bool wxFlatNotebook::RemovePage(size_t page, bool notify)
{
	if(page >= m_windows.GetCount())
		return false;

	// Fire a closing event
	if( notify )
	{
		wxFlatNotebookEvent event(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSING, GetId());
		event.SetSelection((int)page);
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);

		// The event handler allows it?
		if (!event.IsAllowed())
			return false;
	}

	Freeze();

	// Remove the requested page
	wxWindow *pageRemoved = m_windows[page];

	// If the page is the current window, remove it from the sizer
	// as well
	if((int)page == m_pages->GetSelection())
	{
		m_mainSizer->Detach(pageRemoved);
	}

	// Remove it from the array as well
	m_windows.RemoveAt(page);
	Thaw();

	m_pages->DoDeletePage(page);

	// Fire a closed event
	if( notify )
	{
		wxFlatNotebookEvent closedEvent(wxEVT_COMMAND_FLATNOTEBOOK_PAGE_CLOSED, GetId());
		closedEvent.SetSelection((int)page);
		closedEvent.SetEventObject(this);
		GetEventHandler()->ProcessEvent(closedEvent);
	}
	return true;
}

void wxFlatNotebook::SetRightClickMenu(wxMenu* menu)
{
	m_pages->m_pRightClickMenu = menu;
}

wxString wxFlatNotebook::GetPageText(size_t page)
{
	return m_pages->GetPageText(page);
}

void wxFlatNotebook::SetGradientColors(const wxColour& from, const wxColour& to, const wxColour& border)
{
	m_pages->m_colorFrom = from;
	m_pages->m_colorTo   = to;
	m_pages->m_colorBorder = border;
}

void wxFlatNotebook::SetGradientColorFrom(const wxColour& from)
{
	m_pages->m_colorFrom = from;
}

void wxFlatNotebook::SetGradientColorTo(const wxColour& to)
{
	m_pages->m_colorTo   = to;
}

void wxFlatNotebook::SetGradientColorBorder(const wxColour& border)
{
	m_pages->m_colorBorder = border;
}

/// Gets first gradient colour
const wxColour& wxFlatNotebook::GetGradientColorFrom()
{
	return m_pages->m_colorFrom;
}

/// Gets second gradient colour
const wxColour& wxFlatNotebook::GetGradientColorTo()
{
	return m_pages->m_colorTo;
}

/// Gets the tab border colour
const wxColour& wxFlatNotebook::SetGradientColorBorder()
{
	return m_pages->m_colorBorder;
}

/// Get the active tab text
const wxColour& wxFlatNotebook::GetActiveTabTextColour()
{
	return m_pages->m_activeTextColor;
}

void wxFlatNotebook::SetPageImageIndex(size_t page, int imgindex)
{
	m_pages->SetPageImageIndex(page, imgindex);
}

int wxFlatNotebook::GetPageImageIndex(size_t page)
{
	return m_pages->GetPageImageIndex(page);
}

bool wxFlatNotebook::GetEnabled(size_t page)
{
	return m_pages->GetEnabled(page);
}

void wxFlatNotebook::Enable(size_t page, bool enabled)
{
	if(page >= m_windows.GetCount())
		return;

	m_windows[page]->Enable(enabled);
	m_pages->Enable(page, enabled);
}

const wxColour& wxFlatNotebook::GetNonActiveTabTextColour()
{
	return m_pages->m_nonActiveTextColor;
}

void wxFlatNotebook::SetNonActiveTabTextColour(const wxColour& color)
{
	m_pages->m_nonActiveTextColor = color;
}

void wxFlatNotebook::SetTabAreaColour(const wxColour& color)
{
	m_pages->m_tabAreaColor = color;
}

const wxColour& wxFlatNotebook::GetTabAreaColour()
{
	return m_pages->m_tabAreaColor;
}

void wxFlatNotebook::SetActiveTabColour(const wxColour& color)
{
	m_pages->m_activeTabColor = color;
}

const wxColour& wxFlatNotebook::GetActiveTabColour()
{
	return m_pages->m_activeTabColor;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//	wxPageContainer
//
///////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxPageContainer, wxPanel)
EVT_PAINT(wxPageContainer::OnPaint)
EVT_SIZE(wxPageContainer::OnSize)
EVT_LEFT_DOWN(wxPageContainer::OnLeftDown)
EVT_LEFT_UP(wxPageContainer::OnLeftUp)
EVT_RIGHT_DOWN(wxPageContainer::OnRightDown)
EVT_MIDDLE_DOWN(wxPageContainer::OnMiddleDown)
EVT_MOTION(wxPageContainer::OnMouseMove)
EVT_ERASE_BACKGROUND(wxPageContainer::OnEraseBackground)
EVT_LEAVE_WINDOW(wxPageContainer::OnMouseLeave)
EVT_ENTER_WINDOW(wxPageContainer::OnMouseEnterWindow)
EVT_LEFT_DCLICK(wxPageContainer::OnLeftDClick)
END_EVENT_TABLE()

wxPageContainer::wxPageContainer(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: m_ImageList(NULL)
, m_iActivePage(-1)
, m_pDropTarget(NULL)
, m_nLeftClickZone(wxFNB_NOWHERE)
, m_iPreviousActivePage(-1)
{
	m_pRightClickMenu = NULL;
	m_nXButtonStatus = wxFNB_BTN_NONE;
	m_nArrowDownButtonStatus = wxFNB_BTN_NONE;
	m_pParent = parent;
	m_nRightButtonStatus = wxFNB_BTN_NONE;
	m_nLeftButtonStatus = wxFNB_BTN_NONE;
	m_nTabXButtonStatus = wxFNB_BTN_NONE;

	m_colorTo = wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	m_colorFrom   = wxColor(*wxWHITE);
	m_activeTabColor = wxColor(*wxWHITE);
	m_activeTextColor = wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	m_nonActiveTextColor = wxColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
	m_tabAreaColor = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

	// Set default page height, this is done according to the system font
	wxMemoryDC memDc;
	wxBitmap bmp(10, 10);
	memDc.SelectObject(bmp);

	int width, height;

#ifdef __WXGTK__
	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxBOLD);
	memDc.SetFont( boldFont );
#endif

	memDc.GetTextExtent(wxT("Tp"), &width, &height);
	int tabHeight = height + wxFNB_HEIGHT_SPACER; // We use 10 pixels as padding

	wxWindow::Create(parent, id, pos, wxSize(size.x, tabHeight), style | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);

	m_pDropTarget = new wxFNBDropTarget<wxPageContainer>(this, &wxPageContainer::OnDropTarget);
	SetDropTarget(m_pDropTarget);
}

wxPageContainer::~wxPageContainer(void)
{
	if(m_pRightClickMenu)
	{
		delete m_pRightClickMenu;
		m_pRightClickMenu = NULL;
	}
}

void wxPageContainer::OnPaint(wxPaintEvent & event)
{
	wxBufferedPaintDC dc(this);
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer( GetParent()->GetWindowStyleFlag() );

	render->DrawTabs(this, dc, event);
}

bool wxPageContainer::AddPage(const wxString& caption, const bool selected, const int imgindex)
{
	if(selected)
	{
		m_iPreviousActivePage = m_iActivePage;
		m_iActivePage = (int)m_pagesInfoVec.GetCount();
	}

	/// Create page info and add it to the vector
	wxPageInfo pageInfo(caption, imgindex);
	m_pagesInfoVec.Add(pageInfo);
	Refresh();
	return true;
}

bool wxPageContainer::InsertPage(size_t index, wxWindow* /*page*/, const wxString& text, bool select, const int imgindex)
{
	if(select)
	{
		m_iPreviousActivePage = m_iActivePage;
		m_iActivePage = (int)m_pagesInfoVec.GetCount();
	}
	wxPageInfo pgInfo(text, imgindex);
//	pgInfo.SetPosition(wxPoint(1, 1));
	m_pagesInfoVec.Insert(pgInfo, index);
	Refresh();
	return true;
}

void wxPageContainer::OnSize(wxSizeEvent& WXUNUSED(event))
{
	// When resizing the control, try to fit to screen as many tabs as we we can
	long style = GetParent()->GetWindowStyleFlag();
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer(style);
	std::vector<wxRect> vTabInfo;

	int from = 0;
	int page = GetSelection();
	for(; from<m_nFrom; from++) 
	{
		vTabInfo.clear();
		render->NumberTabsCanFit( this, vTabInfo, from );
		if(page - from >= static_cast<int>( vTabInfo.size() ))
			continue;
		break;
	}
	m_nFrom = from;
	Refresh(); // Call on paint
}

void wxPageContainer::OnMiddleDown(wxMouseEvent& event)
{
	// Test if this style is enabled
	long style = GetParent()->GetWindowStyleFlag();
	if(!(style & wxFNB_MOUSE_MIDDLE_CLOSES_TABS))
		return;

	wxPageInfo pgInfo;
	int tabIdx;
	int where = HitTest(event.GetPosition(), pgInfo, tabIdx);
	switch(where)
	{
	case wxFNB_TAB:
		{
			DeletePage((size_t)tabIdx);
			break;
		}
	default:
		break;
	}
	event.Skip();
}

void wxPageContainer::OnRightDown(wxMouseEvent& event)
{
	wxPageInfo pgInfo;
	int tabIdx;
	int where = HitTest(event.GetPosition(), pgInfo, tabIdx);
	switch(where)
	{
	case wxFNB_TAB:
	case wxFNB_TAB_X:
		{
			if(!m_pagesInfoVec[tabIdx].GetEnabled())
				break;

			// Set the current tab to be active
			SetSelection((size_t)tabIdx);

			// If the owner has defined a context menu for the tabs,
			// popup the right click menu
			if (m_pRightClickMenu)
				PopupMenu(m_pRightClickMenu);
			else
			{
				// send a message to popup a custom menu
				wxFlatNotebookEvent event(wxEVT_COMMAND_FLATNOTEBOOK_CONTEXT_MENU, GetParent()->GetId());
				event.SetSelection((int)tabIdx);
				event.SetOldSelection((int)m_iActivePage);
				event.SetEventObject(GetParent());
				GetParent()->GetEventHandler()->ProcessEvent(event);
			}
		}
		break;
	default:
		break;
	}
	event.Skip();
}

void wxPageContainer::OnLeftDown(wxMouseEvent& event)
{
	wxPageInfo pgInfo;
	int tabIdx;

	// Reset buttons status
	m_nXButtonStatus     = wxFNB_BTN_NONE;
	m_nLeftButtonStatus  = wxFNB_BTN_NONE;
	m_nRightButtonStatus = wxFNB_BTN_NONE;
	m_nTabXButtonStatus  = wxFNB_BTN_NONE;
	m_nArrowDownButtonStatus = wxFNB_BTN_NONE;

	m_nLeftClickZone = HitTest(event.GetPosition(), pgInfo, tabIdx);
	switch(m_nLeftClickZone)
	{
	case wxFNB_DROP_DOWN_ARROW:
		m_nArrowDownButtonStatus = wxFNB_BTN_PRESSED;
		Refresh();
		break;
	case wxFNB_LEFT_ARROW:
		m_nLeftButtonStatus = wxFNB_BTN_PRESSED;
		Refresh();
		break;
	case wxFNB_RIGHT_ARROW:
		m_nRightButtonStatus = wxFNB_BTN_PRESSED;
		Refresh();
		break;
	case wxFNB_X:
		m_nXButtonStatus = wxFNB_BTN_PRESSED;
		Refresh();
		break;
	case wxFNB_TAB_X:
		m_nTabXButtonStatus = wxFNB_BTN_PRESSED;
		Refresh();
		break;
	case wxFNB_TAB:
		{
			if(m_iActivePage != tabIdx)
			{
				// Incase the tab is disabled, we dont allow to choose it
				if(!m_pagesInfoVec[tabIdx].GetEnabled())
					break;

				SetSelection(tabIdx);
			}
			break;
		}
	}
}

void wxPageContainer::OnLeftUp(wxMouseEvent& event)
{
	wxPageInfo pgInfo;
	int tabIdx;

	// forget the zone that was initially clicked
	m_nLeftClickZone = wxFNB_NOWHERE;

	int where = HitTest(event.GetPosition(), pgInfo, tabIdx);
	switch(where)
	{
	case wxFNB_LEFT_ARROW:
		{
			if(m_nFrom == 0)
				break;

			// Make sure that the button was pressed before
			if(m_nLeftButtonStatus != wxFNB_BTN_PRESSED)
				break;

			m_nLeftButtonStatus = wxFNB_BTN_HOVER;

			// We scroll left with bulks of 5
			int scrollLeft = GetNumTabsCanScrollLeft();

			m_nFrom -= scrollLeft;
			if(m_nFrom < 0)
				m_nFrom = 0;

			Refresh();
			break;
		}
	case wxFNB_RIGHT_ARROW:
		{
			if(m_nFrom >= (int)m_pagesInfoVec.GetCount() - 1)
				break;

			// Make sure that the button was pressed before
			if(m_nRightButtonStatus != wxFNB_BTN_PRESSED)
				break;

			m_nRightButtonStatus = wxFNB_BTN_HOVER;

			// Check if the right most tab is visible, if it is
			// don't rotate right anymore
			if(m_pagesInfoVec[m_pagesInfoVec.GetCount()-1].GetPosition() != wxPoint(-1, -1))
				break;

			int lastVisibleTab = GetLastVisibleTab();
			if(lastVisibleTab < 0)
			{
				// Probably the screen is too small for displaying even a single
				// tab, in this case we do nothing
				break;
			}

			m_nFrom += GetNumOfVisibleTabs();
			Refresh();
			break;
		}
	case wxFNB_X:
		{
			// Make sure that the button was pressed before
			if(m_nXButtonStatus != wxFNB_BTN_PRESSED)
				break;

			m_nXButtonStatus = wxFNB_BTN_HOVER;

			DeletePage((size_t)m_iActivePage);
			break;
		}
	case wxFNB_TAB_X:
		{
			// Make sure that the button was pressed before
			if(m_nTabXButtonStatus != wxFNB_BTN_PRESSED)
				break;

			m_nTabXButtonStatus = wxFNB_BTN_HOVER;

			DeletePage((size_t)m_iActivePage);
			break;
		}
	case wxFNB_DROP_DOWN_ARROW:
		{
			// Make sure that the button was pressed before
			if(m_nArrowDownButtonStatus != wxFNB_BTN_PRESSED)
				break;

			m_nArrowDownButtonStatus = wxFNB_BTN_NONE;

			// Refresh the button status
			wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer( GetParent()->GetWindowStyleFlag() );
			wxClientDC dc(this);
			render->DrawDropDownArrow(this, dc);

			PopupTabsMenu();
			break;
		}
	}
}

int wxPageContainer::HitTest(const wxPoint& pt, wxPageInfo& pageInfo, int &tabIdx)
{
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer( GetParent()->GetWindowStyleFlag() );
	
	wxRect rect = GetClientRect();
	int btnLeftPos = render->GetLeftButtonPos(this);
	int btnRightPos = render->GetRightButtonPos(this);
	int btnXPos =render->GetXPos(this);
	long style = GetParent()->GetWindowStyleFlag();

	tabIdx = -1;
	if(m_pagesInfoVec.IsEmpty())
	{
		return wxFNB_NOWHERE;
	}

	rect = wxRect(btnXPos, 8, 16, 16);
	if(rect.Contains_(pt))
	{
		return (style & wxFNB_NO_X_BUTTON) ? wxFNB_NOWHERE : wxFNB_X;
	}

	rect = wxRect(btnRightPos, 8, 16, 16);
	if( style & wxFNB_DROPDOWN_TABS_LIST )
	{
		rect = wxRect(render->GetDropArrowButtonPos( this ), 8, 16, 16);
		if( rect.Contains_(pt) )
			return wxFNB_DROP_DOWN_ARROW;
	}

	if(rect.Contains_(pt))
	{
		return (style & wxFNB_NO_NAV_BUTTONS) ? wxFNB_NOWHERE : wxFNB_RIGHT_ARROW;
	}


	rect = wxRect(btnLeftPos, 8, 16, 16);
	if(rect.Contains_(pt))
	{
		return (style & wxFNB_NO_NAV_BUTTONS) ? wxFNB_NOWHERE : wxFNB_LEFT_ARROW;
	}

	// Test whether a left click was made on a tab
	bool bFoundMatch = false;
	for(size_t cur=m_nFrom; cur<m_pagesInfoVec.GetCount(); cur++)
	{
		wxPageInfo pgInfo = m_pagesInfoVec[cur];
		if(pgInfo.GetPosition() == wxPoint(-1, -1))
			continue;

		// check for mouse over tab's x button
		if(style & wxFNB_X_ON_TAB && (int)cur == GetSelection())
		{
			// 'x' button exists on a tab
			if(m_pagesInfoVec[cur].GetXRect().Contains_(pt))
			{
				pageInfo = pgInfo;
				tabIdx = (int)cur;
				return wxFNB_TAB_X;
			}
		}

		if(style & wxFNB_VC8)
		{
			if(m_pagesInfoVec[cur].GetRegion().Contains(pt) == wxInRegion)
			{
				if(bFoundMatch || (int)cur == GetSelection())
				{
					pageInfo = pgInfo;
					tabIdx = (int)cur;
					return wxFNB_TAB;
				}
				pageInfo = pgInfo;
				tabIdx = (int)cur;
				bFoundMatch = true;
			}
		}
		else
		{

			wxRect tabRect = wxRect(pgInfo.GetPosition().x, pgInfo.GetPosition().y, 
				pgInfo.GetSize().x, pgInfo.GetSize().y);
			if(tabRect.Contains_(pt))
			{
				// We have a match
				pageInfo = pgInfo;
				tabIdx = (int)cur;
				return wxFNB_TAB;
			}
		}
	}

	if(bFoundMatch)
		return wxFNB_TAB;

	// Default
	return wxFNB_NOWHERE;
}

void wxPageContainer::SetSelection(size_t page)
{
	wxFlatNotebook* book = (wxFlatNotebook*)GetParent();
	book->SetSelection(page);
	DoSetSelection(page);
}

void wxPageContainer::DoSetSelection(size_t page)
{
	// Make sure that the selection is visible
	if(page < m_pagesInfoVec.GetCount())
	{
		//! fix for tabfocus
		wxWindow* da_page = ((wxFlatNotebook *)m_pParent)->GetPage(page);
		if ( da_page!=NULL )
			da_page->SetFocus();
	}

	if( !IsTabVisible(page) )
	{
		FNB_LOG_MSG( wxT("Tab ") << (int)page << wxT(" is not visible"));
		FNB_LOG_MSG( wxT("m_nFrom=") << m_nFrom << wxT(", Selection=") << (int)page );

		// Try to remove one tab from start and try again
		if( !CanFitToScreen(page) )
		{
			if( m_nFrom > (int)page )
				m_nFrom = (int)page;
			else
			{
				while( m_nFrom < (int)page )
				{
					m_nFrom++;
					if( CanFitToScreen(page) )
						break;
				}
			}
			FNB_LOG_MSG( wxT("Adjusting m_nFrom to=") << m_nFrom);
		}
	}
	else
	{
		FNB_LOG_MSG( wxT("Tab ") << (int)page << wxT(" is visible"));
	}
	Refresh();
}

void wxPageContainer::DeletePage(size_t page)
{
	wxFlatNotebook* book = (wxFlatNotebook*)GetParent();
	book->DeletePage(page);
	book->Refresh();
}

bool wxPageContainer::IsTabVisible(size_t page)
{
	int iPage = (int)page;
	int iLastVisiblePage = GetLastVisibleTab();

	return iPage <= iLastVisiblePage && iPage >= m_nFrom;
}

void wxPageContainer::DoDeletePage(size_t page)
{
	// Remove the page from the vector
	wxFlatNotebook* book = (wxFlatNotebook*)GetParent();
	m_pagesInfoVec.RemoveAt(page);

	// Armel Asselin's patch
	int newActivePageIndex = m_iActivePage, newPreviousPageIndex = m_iPreviousActivePage;
	// if the "previous page" is after the deleted page, decrement the index
	if (m_iPreviousActivePage > (int)page)
	{
		newPreviousPageIndex = m_iPreviousActivePage-1;
	}
	else if (m_iPreviousActivePage == (int)page)
	{
		newPreviousPageIndex = -1;
	}
	
	// same thing with the active page
	if (m_iActivePage > (int)page || (int)page >= m_pagesInfoVec.Count())
	{
		newActivePageIndex = m_iActivePage-1;
	}
	else if (m_iActivePage == (int)page)
	{
		newActivePageIndex = newPreviousPageIndex;
 	}

	m_iActivePage = newActivePageIndex;
	m_iPreviousActivePage = newPreviousPageIndex;
	if (m_iActivePage == m_iPreviousActivePage)
		m_iPreviousActivePage = -1;

	// The delete page was the last first on the array,
	// but the book still has more pages, so we set the
	// active page to be the first one (0)
	if(m_iActivePage < 0 && !m_pagesInfoVec.empty())
	{
		m_iPreviousActivePage = -1;
		m_iActivePage = 0;
	}

	// Refresh the tabs
	if(m_iActivePage >= 0)
	{
		book->SetForceSelection(true);
		book->SetSelection(m_iActivePage);
		book->SetForceSelection(false);
	}

	if(m_pagesInfoVec.empty())
	{
		// Erase the page container drawings
		wxClientDC dc(this);
		dc.Clear();
	}
}

void wxPageContainer::DeleteAllPages()
{
	m_iActivePage = -1;
	m_iPreviousActivePage = -1;
	m_nFrom = 0;
	m_pagesInfoVec.Clear();

	// Erase the page container drawings
	wxClientDC dc(this);
	dc.Clear();
}

void wxPageContainer::OnMouseMove(wxMouseEvent& event)
{
	if (!m_pagesInfoVec.empty() && IsShown())
	{
		const int xButtonStatus = m_nXButtonStatus;
		const int xTabButtonStatus = m_nTabXButtonStatus;
		const int rightButtonStatus = m_nRightButtonStatus;
		const int leftButtonStatus = m_nLeftButtonStatus;
		const int dropDownButtonStatus = m_nArrowDownButtonStatus;

		long style = GetParent()->GetWindowStyleFlag();
 
		m_nXButtonStatus = wxFNB_BTN_NONE;
		m_nRightButtonStatus = wxFNB_BTN_NONE;
		m_nLeftButtonStatus = wxFNB_BTN_NONE;
		m_nTabXButtonStatus = wxFNB_BTN_NONE;
		m_nArrowDownButtonStatus = wxFNB_BTN_NONE;

		wxPageInfo pgInfo;
		int tabIdx;

		int where = HitTest(event.GetPosition(), pgInfo, tabIdx);
		switch ( where )
		{
		case wxFNB_X:
			if (event.LeftIsDown())
			{
				m_nXButtonStatus = (m_nLeftClickZone==wxFNB_X) ? wxFNB_BTN_PRESSED : wxFNB_BTN_NONE;
			}
			else
			{
				m_nXButtonStatus = wxFNB_BTN_HOVER;
			}
			break;
		case wxFNB_DROP_DOWN_ARROW:
			if (event.LeftIsDown())
			{
				m_nArrowDownButtonStatus = (m_nLeftClickZone == wxFNB_DROP_DOWN_ARROW) ? wxFNB_BTN_PRESSED : wxFNB_BTN_NONE;
			}
			else
			{
				m_nArrowDownButtonStatus = wxFNB_BTN_HOVER;
			}
			break;
		case wxFNB_TAB_X:
			if (event.LeftIsDown())
			{
				m_nTabXButtonStatus = (m_nLeftClickZone==wxFNB_TAB_X) ? wxFNB_BTN_PRESSED : wxFNB_BTN_NONE;
			}
			else
			{
				m_nTabXButtonStatus = wxFNB_BTN_HOVER;
			}
			break;
		case wxFNB_RIGHT_ARROW:
			if (event.LeftIsDown())
			{
				m_nRightButtonStatus = (m_nLeftClickZone==wxFNB_RIGHT_ARROW) ? wxFNB_BTN_PRESSED : wxFNB_BTN_NONE;
			}
			else
			{
				m_nRightButtonStatus = wxFNB_BTN_HOVER;
			}
			break;

		case wxFNB_LEFT_ARROW:
			if (event.LeftIsDown())
			{
				m_nLeftButtonStatus = (m_nLeftClickZone==wxFNB_LEFT_ARROW) ? wxFNB_BTN_PRESSED : wxFNB_BTN_NONE;
			}
			else
			{
				m_nLeftButtonStatus = wxFNB_BTN_HOVER;
			}
			break;

		case wxFNB_TAB:
			// Call virtual method for showing tooltip
			ShowTabTooltip(tabIdx);
			if(!GetEnabled((size_t)tabIdx))
			{
				// Set the cursor to be 'No-entry'
				::wxSetCursor(wxCURSOR_NO_ENTRY);
			}

			// Support for drag and drop
			if(event.Dragging() && !(style & wxFNB_NODRAG))
			{
				wxFNBDragInfo draginfo(this, tabIdx);
				wxFNBDragInfoDataObject dataobject(wxDataFormat(wxT("wxFNB")));
				dataobject.SetData(sizeof(wxFNBDragInfo), &draginfo);
				wxFNBDropSource dragSource(this);
				dragSource.SetData(dataobject);
				dragSource.DoDragDrop(wxDrag_DefaultMove);
			}
			break;
		default:
			m_nTabXButtonStatus = wxFNB_BTN_NONE;
			break;
		}

		const bool bRedrawX = m_nXButtonStatus != xButtonStatus;
		const bool bRedrawDropArrow = m_nArrowDownButtonStatus != dropDownButtonStatus;
		const bool bRedrawRight = m_nRightButtonStatus != rightButtonStatus;
		const bool bRedrawLeft = m_nLeftButtonStatus != leftButtonStatus;
		const bool bRedrawTabX = m_nTabXButtonStatus != xTabButtonStatus;
		
		wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer( GetParent()->GetWindowStyleFlag() );

		if (bRedrawX || bRedrawRight || bRedrawLeft || bRedrawTabX || bRedrawDropArrow)
		{
			wxClientDC dc(this);
			if (bRedrawX)
			{
				render->DrawX(this, dc);
			}
			if (bRedrawLeft)
			{
				render->DrawLeftArrow(this, dc);
			}
			if (bRedrawRight)
			{
				render->DrawRightArrow(this, dc);
			}
			if (bRedrawTabX)
			{
				FNB_LOG_MSG( wxT("Refreshing Tab 'X' button with status=") << m_nTabXButtonStatus << wxT(" and tabIdx=") << tabIdx );
				render->DrawTabX(this, dc, pgInfo.GetXRect(), tabIdx, m_nTabXButtonStatus);
			}
			if (bRedrawDropArrow)
			{
				render->DrawDropDownArrow(this, dc);
			}
		}
	}
	event.Skip();
}

int wxPageContainer::GetLastVisibleTab()
{
	int i;
	if( m_nFrom < 0)
		return -1;

	for(i=m_nFrom; i<(int)m_pagesInfoVec.GetCount(); i++)
	{
		if(m_pagesInfoVec[i].GetPosition() == wxPoint(-1, -1))
			break;
	}
	return (i-1);
}

int wxPageContainer::GetNumTabsCanScrollLeft()
{
	int i;

	// Reserved area for the buttons (<>x)
	wxRect rect = GetClientRect();
	int clientWidth = rect.width;
	int posx = ((wxFlatNotebook *)m_pParent)->GetPadding(), numTabs = 0, tabHeight, tabWidth;

	wxClientDC dc(this);

	// Incase we have error prevent crash
	if(m_nFrom < 0)
		return 0;

	long style = GetParent()->GetWindowStyleFlag();
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer(style);

	tabHeight = render->CalcTabHeight(this);
	for(i=m_nFrom; i>=0; i--)
	{
		tabWidth = render->CalcTabWidth(this, i, tabHeight);
		if(posx + tabWidth + render->GetButtonsAreaLength(this) >= clientWidth)
			break;

		numTabs++;
		posx += tabWidth;
	}
	return numTabs;
}

bool wxPageContainer::IsDefaultTabs()
{
	long style = GetParent()->GetWindowStyleFlag();
	bool res = (style & wxFNB_VC71) || (style & wxFNB_FANCY_TABS) || (style & wxFNB_VC8);
	return !res;
}

void wxPageContainer::AdvanceSelection(bool bForward)
{
	int nSel = GetSelection();

	if(nSel < 0)
		return;

	int nMax = (int)GetPageCount() - 1;
	if ( bForward )
		SetSelection(nSel == nMax ? 0 : nSel + 1);
	else
		SetSelection(nSel == 0 ? nMax : nSel - 1);
}


void wxPageContainer::OnMouseLeave(wxMouseEvent& event)
{
	m_nLeftButtonStatus = wxFNB_BTN_NONE;
	m_nXButtonStatus = wxFNB_BTN_NONE;
	m_nRightButtonStatus = wxFNB_BTN_NONE;
	m_nTabXButtonStatus = wxFNB_BTN_NONE;
	m_nArrowDownButtonStatus = wxFNB_BTN_NONE;

	long style = GetParent()->GetWindowStyleFlag();
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer(style);

	wxClientDC dc(this);
	render->DrawX(this, dc);
	render->DrawLeftArrow(this, dc);
	render->DrawRightArrow(this, dc);
	if(GetSelection() != -1 && IsTabVisible((size_t)GetSelection()))
	{
		render->DrawTabX(this, dc, m_pagesInfoVec[GetSelection()].GetXRect(), GetSelection(), m_nTabXButtonStatus);
	}

	event.Skip();
}

void wxPageContainer::OnMouseEnterWindow(wxMouseEvent& event)
{
	m_nLeftButtonStatus = wxFNB_BTN_NONE;
	m_nXButtonStatus = wxFNB_BTN_NONE;
	m_nRightButtonStatus = wxFNB_BTN_NONE;
	m_nLeftClickZone = wxFNB_BTN_NONE;
	m_nArrowDownButtonStatus = wxFNB_BTN_NONE;

	event.Skip();
}

void wxPageContainer::ShowTabTooltip(int tabIdx)
{
	wxWindow *pWindow = ((wxFlatNotebook *)m_pParent)->GetPage(tabIdx);
	if( pWindow )
	{
		wxToolTip *pToolTip = pWindow->GetToolTip();
		if(pToolTip && pToolTip->GetWindow() == pWindow)
			SetToolTip(pToolTip->GetTip());
	}
}

void wxPageContainer::SetPageImageIndex(size_t page, int imgindex)
{
	if(page < m_pagesInfoVec.GetCount())
	{
		m_pagesInfoVec[page].SetImageIndex(imgindex);
		Refresh();
	}
}

int wxPageContainer::GetPageImageIndex(size_t page)
{
	if(page < m_pagesInfoVec.GetCount())
	{
		return m_pagesInfoVec[page].GetImageIndex();
	}
	return -1;
}

wxDragResult wxPageContainer::OnDropTarget(wxCoord x, wxCoord y, int nTabPage, wxWindow * wnd_oldContainer)
{
	// Disable drag'n'drop for disabled tab
	if(!((wxPageContainer *)wnd_oldContainer)->m_pagesInfoVec[nTabPage].GetEnabled())
		return wxDragCancel;

	wxLogTrace(wxTraceMask(), _("Old Page Index = %i"), nTabPage);
	wxPageContainer * oldContainer = (wxPageContainer *)wnd_oldContainer;
	int nIndex = -1;
	wxPageInfo pgInfo;
	int where = HitTest(wxPoint(x, y), pgInfo, nIndex);
	wxLogTrace(wxTraceMask(), _("OnDropTarget: index by HitTest = %i"), nIndex);
	wxFlatNotebook * oldNotebook = (wxFlatNotebook *)oldContainer->GetParent();
	wxFlatNotebook * newNotebook = (wxFlatNotebook *)GetParent();

	if(oldNotebook == newNotebook)
	{
		if(nTabPage >= 0)
		{
			switch(where)
			{
			case wxFNB_TAB:
				MoveTabPage(nTabPage, nIndex);
				break;
			case wxFNB_NOWHERE:
				{
				}
				break;
			default:
				break;
			}
		}
	}
	else if ( GetParent()->GetWindowStyleFlag() & wxFNB_ALLOW_FOREIGN_DND )
	{
#if defined(__WXMSW__) || defined(__WXGTK__)
		if(nTabPage >= 0)
		{
			wxWindow * window = oldNotebook->GetPage(nTabPage);
			if(window)
			{
				wxString caption = oldContainer->GetPageText(nTabPage);

				// Pass the image to the new container
				// incase that the new container (this) does not have image list we dont pass the image
				// to the new notebook
				int newIndx( wxNOT_FOUND );

				if( m_ImageList ) 
				{
					int imageindex = oldContainer->GetPageImageIndex(nTabPage);
					if( imageindex >= 0 )
					{
						wxBitmap bmp( (*oldContainer->GetImageList())[imageindex] );
						m_ImageList->Add( bmp );
						newIndx = static_cast<int>(m_ImageList->GetCount() - 1);
					}
				}
				
				oldNotebook->RemovePage( nTabPage );
				window->Reparent( newNotebook );
				newNotebook->InsertPage(nIndex, window, caption, true, newIndx);
			}
		}
#endif
	}
	return wxDragMove;
}

void wxPageContainer::MoveTabPage(int nMove, int nMoveTo)
{
	if(nMove == nMoveTo)
		return;

	else if(nMoveTo < (int)((wxFlatNotebook *)m_pParent)->GetWindows().GetCount())
		nMoveTo++;

	m_pParent->Freeze();
	// Remove the window from the main sizer
	int nCurSel = ((wxFlatNotebook *)m_pParent)->GetPages()->GetSelection();
	((wxFlatNotebook *)m_pParent)->GetMainSizer()->Detach(((wxFlatNotebook *)m_pParent)->GetWindows().Item(nCurSel));
	((wxFlatNotebook *)m_pParent)->GetWindows().Item(nCurSel)->Hide();

	wxWindow *pWindow = ((wxFlatNotebook *)m_pParent)->GetWindows().Item(nMove);
	((wxFlatNotebook *)m_pParent)->GetWindows().RemoveAt(nMove);
	((wxFlatNotebook *)m_pParent)->GetWindows().Insert(pWindow, nMoveTo-1);

	wxPageInfo pgInfo = m_pagesInfoVec[nMove];

	m_pagesInfoVec.RemoveAt( nMove );
	m_pagesInfoVec.Insert(pgInfo, nMoveTo - 1);

	// Add the page according to the style
	wxBoxSizer* pSizer = ((wxFlatNotebook *)m_pParent)->GetMainSizer();
	long style = GetParent()->GetWindowStyleFlag();


	if(style & wxFNB_BOTTOM)
	{
		pSizer->Insert(0, pWindow, 1, wxEXPAND);
	}
	else
	{
		// We leave a space of 1 pixel around the window
		pSizer->Add(pWindow, 1, wxEXPAND);
	}
	pWindow->Show();

	pSizer->Layout();
	m_iActivePage = nMoveTo-1;
	m_iPreviousActivePage = -1;
	DoSetSelection(m_iActivePage);
//	Refresh();
	m_pParent->Thaw();
}

bool wxPageContainer::CanFitToScreen(size_t page)
{
	// Incase the from is greater than page,
	// we need to reset the m_nFrom, so in order
	// to force the caller to do so, we return false
	if(m_nFrom > (int)page)
		return false;

	long style = GetParent()->GetWindowStyleFlag();
	wxFNBRendererPtr render = wxFNBRendererMgrST::Get()->GetRenderer(style);
	std::vector<wxRect> vTabInfo;
	render->NumberTabsCanFit( this, vTabInfo );

	if(static_cast<int>(page) - m_nFrom >= static_cast<int>( vTabInfo.size() ))
		return false;
	return true;
}

int wxPageContainer::GetNumOfVisibleTabs()
{
	int i=m_nFrom;
	int counter = 0;
	for(; i<(int)m_pagesInfoVec.GetCount(); i++, ++counter)
	{
		if(m_pagesInfoVec[i].GetPosition() == wxPoint(-1, -1))
			break;
	}
	return counter;
}

bool wxPageContainer::GetEnabled(size_t page)
{
	if(page >= m_pagesInfoVec.GetCount())
		return true;	// Seems strange, but this is the default
	return m_pagesInfoVec[page].GetEnabled();
}

void wxPageContainer::Enable(size_t page, bool enabled)
{
	if(page >= m_pagesInfoVec.GetCount())
		return ;
    m_pagesInfoVec[page].Enable(enabled);
    return;
}

wxColor wxPageContainer::GetSingleLineBorderColor()
{
	if(HasFlag(wxFNB_FANCY_TABS))
		return m_colorFrom;
	return *wxWHITE;
}

bool wxPageContainer::HasFlag(int flag)
{
	long style = GetParent()->GetWindowStyleFlag();
	bool res = style & flag ? true : false;
	return res;
}

void wxPageContainer::ClearFlag(int flag)
{
	long style = GetParent()->GetWindowStyleFlag();
	style &= ~( flag );
	wxWindowBase::SetWindowStyleFlag(style);
}

bool wxPageContainer::TabHasImage(int tabIdx)
{
	if(m_ImageList)
		return m_pagesInfoVec[tabIdx].GetImageIndex() != -1;
	return false;
}

void wxPageContainer::OnLeftDClick(wxMouseEvent& event)
{
	wxPageInfo pgInfo;
	int tabIdx;
	int where = HitTest(event.GetPosition(), pgInfo, tabIdx);
	switch(where)
	{
	case wxFNB_TAB:
		if(HasFlag(wxFNB_DCLICK_CLOSES_TABS))
		{
			DeletePage((size_t)tabIdx);
		}
		break;
	case wxFNB_X:
		{
			OnLeftDown(event);
		}
		break;
	default:
		event.Skip();
		break;
	}
}

void wxPageContainer::PopupTabsMenu()
{
	wxMenu popupMenu;

	for(size_t i=0; i<m_pagesInfoVec.GetCount(); i++)
	{
		wxPageInfo pi = m_pagesInfoVec[i];
		wxMenuItem *item = new wxMenuItem(&popupMenu, static_cast<int>(i), pi.GetCaption(), pi.GetCaption(), wxITEM_NORMAL);

		// This code is commented, since there is an alignment problem with wx2.6.3 & Menus
//		if( TabHasImage(static_cast<int>(i)) )
//			item->SetBitmaps( (*m_ImageList)[pi.GetImageIndex()] );

		popupMenu.Append( item );
	}

	// connect an event handler to our menu
	popupMenu.Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxPageContainer::OnTabMenuSelection), NULL, this);
	PopupMenu( &popupMenu );
}

void wxPageContainer::OnTabMenuSelection(wxCommandEvent &event)
{
	int selection = event.GetId();
	static_cast<wxFlatNotebook*>(m_pParent)->SetSelection( (size_t)selection );
}

// Draw small arrow at the place that the tab will be placed
void wxPageContainer::DrawDragHint()
{
	// get the index of tab that will be replaced with the dragged tab
	wxPageInfo info;
	int tabIdx;
	wxPoint pt = ::wxGetMousePosition();
    wxPoint client_pt = ScreenToClient(pt);
	HitTest(client_pt, info, tabIdx);
	wxFNBRendererMgrST::Get()->GetRenderer( GetParent()->GetWindowStyleFlag() )->DrawDragHint(this, tabIdx);
}
