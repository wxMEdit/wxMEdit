///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/update.h
// Description: Checking for Update of wxMEdit
// Copyright:   2014-2023  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UPDATE_H_
#define _WXM_UPDATE_H_

///////////////////////////////////////////////////////////////////////////////
// Version Numbers of Release and Prerelease:
//     Release   : A.B.C
//     Prerelease: A.B.C.D / A.B.C-D
///////////////////////////////////////////////////////////////////////////////

#include "../xm/cxx11.h"
#include "choice_map.hpp"
#include <ctime>

class wxFileConfig;
class wxWindow;
namespace wxm
{

extern std::string g_result_checkupdates;
extern bool g_check_prerelease;
extern bool g_update_checking;

std::vector<unsigned int> ParseVersion(const std::string& ver);
bool IsFirstNewer(const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2, bool check_prerelease);

inline bool IsFirstNewer(const std::string& v1, const std::string& v2, bool check_prerelease)
{
	return IsFirstNewer(ParseVersion(v1), ParseVersion(v2), check_prerelease);
}

std::string CheckUpdates(bool check_prerelease=g_check_prerelease);

void ConfirmUpdate(bool notify_all=true);

void AutoCheckUpdates(wxFileConfig* cfg);
void ManualCheckUpdates();

struct UpdatePeriods: public ChoiceMap<UpdatePeriods, time_t>
{
	time_t ConfigToPeroid(const wxString& cfg) const;

private:
	friend class ChoiceMap<UpdatePeriods, time_t>;
	UpdatePeriods() {}

	virtual void DoInit() override;

	enum Index
	{
		UP_DAILY,
		UP_WEEKLY,
		UP_MONTHLY,
		UP_NEVER,
	};

	static const int default_idx = UP_WEEKLY;
	static const time_t default_peroid = time_t(3600*24*7);
};

} //namespace wxm

#endif //_WXM_UPDATE_H_
