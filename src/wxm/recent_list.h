///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/recent_list.h
// Description: Recent List
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_RECENT_LIST_H_
#define _WXM_RECENT_LIST_H_

#include "../xm/cxx11.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/docview.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

namespace wxm
{

struct wxRecentList : public wxFileHistory
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
	virtual void AddFileToHistory(const wxString& file) override;

	wxWindowID m_idbase;

	wxWindowID GetBaseId()
	{
		return m_idbase;
	}
};

struct wxSimpleRecentList : public wxRecentList
{
	wxSimpleRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2) override
	{
		return item1 == item2;
	}
};

struct wxFilePathRecentList : public wxRecentList
{
	wxFilePathRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2) override;
};

struct wxCaseInsensitiveRecentList : public wxRecentList
{
	wxCaseInsensitiveRecentList(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
		: wxRecentList(maxFiles, idBase)
	{}
private:
	virtual bool ItemEqual(const wxString& item1, const wxString& item2) override;
};

} // namespace wxm

#endif //_WXM_RECENT_LIST_H_
