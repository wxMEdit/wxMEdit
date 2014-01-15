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

struct wxRecentList: public wxFileHistory
{
	wxRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxFileHistory(maxFiles, idBase), m_idbase(idBase)
	{}

	void AddItemToHistory(const wxString& item)
	{
		return AddFileToHistory(item);
	}

protected:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2) = 0;
private:
	virtual void AddFileToHistory(const wxString& file);

	wxWindowID m_idbase;

	wxWindowID GetBaseId()
	{
		return m_idbase;
	}
};

struct wxSimpleRecentList: public wxRecentList
{
	wxSimpleRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2)
	{
		return item1 == item2;
	}
};

struct wxFilePathRecentList: public wxRecentList
{
	wxFilePathRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2);
};

struct wxCaseInsensitiveRecentList: public wxRecentList
{
	wxCaseInsensitiveRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2);
};

#endif //_WX_RECENT_LIST_H_
