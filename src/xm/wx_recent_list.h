///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wx_recent_list.h
// Description: Recent List
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RECENT_LIST_H_
#define _WX_RECENT_LIST_H_

#include <wx/docview.h>

class wxRecentList: public wxFileHistory
{
public:
	static inline bool OSCaseSensitive()
	{
#ifdef __WXMSW__
		return false;
#else
		return true;
#endif
	}

	wxRecentList(bool caseSensitive=OSCaseSensitive(), size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxFileHistory(maxFiles, idBase), m_caseSensitive(caseSensitive), m_idbase(idBase)
	{}

	void AddItemToHistory(const wxString& item)
	{
		return AddFileToHistory(item);
	}
private:
	virtual void AddFileToHistory(const wxString& file);

	bool ItemEQ(const wxString& item1, const wxString& item2);

	bool m_caseSensitive;

	wxWindowID m_idbase;

	wxWindowID GetBaseId()
	{
		return m_idbase;
	}
};

#endif //_WX_RECENT_LIST_H_
