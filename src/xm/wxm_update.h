///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_update.h
// Description: Checking for update of wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UPDATE_H_
#define _WXM_UPDATE_H_

///////////////////////////////////////////////////////////////////////////////
// Version Numbers of Release and Prerelease:
//     Release   : A.B.C
//     Prerelease: A.B.C.D / A.B.C-D
///////////////////////////////////////////////////////////////////////////////

#include "wxm_choice_map.hpp"
#include <ctime>

class wxFileConfig;
class wxWindow;
class MadEditFrame;
extern MadEditFrame *g_MainFrame;
namespace wxm
{

extern std::string g_result_autocheckupdates;
extern bool g_check_prerelease;

std::string CheckUpdates(bool check_prerelease=g_check_prerelease);

void ConfirmUpdate(const std::string& newver, bool notify_all=true, 
                   wxWindow* parentwin=reinterpret_cast<wxWindow*>(g_MainFrame));

void AutoCheckUpdates(wxFileConfig* cfg);

struct UpdatePeriods: public ChoiceMap<UpdatePeriods, time_t>
{
	time_t ConfigToPeroid(const wxString& cfg) const;

private:
	friend class ChoiceMap<UpdatePeriods, time_t>;
	UpdatePeriods() {}

	virtual void DoInit();

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
