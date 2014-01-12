///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_update.h
// Description: Checking for update of wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UPDATE_H_
#define _WXM_UPDATE_H_

#include <wx/string.h>
#include <vector>
#include <map>
#include <ctime>

class wxFileConfig;

namespace wxm
{

std::string CheckUpdates();

void ConfirmUpdate(const std::string& newver, bool notify_newest=true);

void AutoCheckUpdates(wxFileConfig* cfg);

struct UpdatePeriods
{
	std::vector<wxString> GetTitles() const;
	wxString GetDefaultTitle() const;
	wxString TitleToConfig(const wxString& title) const;
	wxString ConfigToTitle(const wxString& cfg) const;
	time_t ConfigToPeroid(const wxString& cfg) const;

	static UpdatePeriods& Instance();

	void Initialize();
private:
	UpdatePeriods(): m_initialized(false) {}

	int ConfigToIndex(const wxString& cfg) const;
	wxString IndexToTitle(int idx) const;
	time_t IndexToPeriod(int idx) const;

	void AddData(int idx, time_t peroid, const wxString& cfg, const wxString &l10ntitle);

	enum Index
	{
		UP_DAILY,
		UP_WEEKLY,
		UP_MONTHLY,
		UP_NEVER,
	};

	typedef std::map<int, time_t>        IdxPeriodMap;
	typedef std::map<int, wxString>      IdxL10nTitleMap;
	typedef std::map<wxString, wxString> L10nTitleCfgMap;
	typedef std::map<wxString, int>      CfgIdxMap;

	IdxPeriodMap    m_idx_peroid_map;
	IdxL10nTitleMap m_idx_l10ntitle_map;
	L10nTitleCfgMap m_l10ntitle_cfg_map;
	CfgIdxMap       m_cfg_idx_map;

	bool m_initialized;
	static const int default_idx = UP_WEEKLY;
	static const int default_peroid = 3600*24*7;
};

} //namespace wxm

#endif //_WXM_UPDATE_H_
