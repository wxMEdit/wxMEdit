///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        wxm/update.cpp
// Description: Checking for Update of wxMEdit
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "update.h"
#include "../xm/cxx11.h"
#include "def.h"
#include "utils.h"
#include "../xm/remote.h"
#include "../wxmedit_frame.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/msgdlg.h>
#include <wx/intl.h>
#include <wx/thread.h>
#include <wx/fileconf.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <functional>
#include <limits>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xp =  boost::xpressive;
namespace algo = boost::algorithm;
namespace wxm
{

std::string g_result_checkupdates;
bool g_check_prerelease = false;
bool g_update_checking = false;
const std::string NO_NEW_VERSION("no-new-ver");

static std::string GetVersionFromRemoteChangeLog();
static std::vector<unsigned int> ParseVersion(const std::string& ver);
static bool IsFirstNewer(const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2, bool check_prerelease);

inline bool IsFirstNewer(const std::string& v1, const std::string& v2, bool check_prerelease)
{
	return IsFirstNewer(ParseVersion(v1), ParseVersion(v2), check_prerelease);
}

bool IsPrerelease(const std::string& ver)
{
	return !algo::find_nth(ver, ".", 1).empty();
}

std::string AdjustVersion(const std::string& ver, bool with_prerelease)
{
	if (with_prerelease)
		return ver;

	std::string::const_iterator dot2nd = algo::find_nth(ver, ".", 1).begin();

	return std::string(ver.begin(), dot2nd);
}

std::string CheckUpdates(bool check_prerelease)
{
	std::string remote_ver = GetVersionFromRemoteChangeLog();
	if (remote_ver.empty())
		return std::string();

	if (IsFirstNewer(remote_ver, WXMEDIT_VERSION, check_prerelease))
	{
		return AdjustVersion(remote_ver, check_prerelease);
	}

	return NO_NEW_VERSION;
}

void ConfirmUpdate(bool notify_all)
{
	g_update_checking = false;

	if (g_result_checkupdates.empty())
	{
		if (notify_all)
			wxMessageBox(_("Cannot check for new version!"), wxT("wxMEdit"), wxICON_ERROR|wxOK);
		return;
	}

	if (g_result_checkupdates == NO_NEW_VERSION)
	{
		if (notify_all)
			wxMessageBox(_("wxMEdit is already the newest version."), wxT("wxMEdit"), wxICON_INFORMATION|wxOK);
		return;
	}

	wxString download_page = wxT("http://wxmedit.github.io/downloads.html");
	if (IsPrerelease(g_result_checkupdates))
		download_page = wxT("http://wxmedit.github.io/prerelease.html");

	wxString title(_("wxMEdit - New version available"));
	wxString msg( wxString::Format( _("wxMEdit %s is available. \nClick OK to open the download page."), 
	                                wxString(g_result_checkupdates.c_str(), wxConvUTF8).c_str() ) );

	if (wxOK == wxMessageBox(msg, title, wxICON_INFORMATION|wxOK|wxCANCEL))
		OpenURL(download_page);
}

std::string GetVersionFromRemoteChangeLog()
{
	std::string str(xm::GetRemoteText("https://raw.github.com/wxMEdit/wxMEdit/master/ChangeLog"));

	size_t poslf1 = str.find_first_of('\n', 0);
	size_t poslf2 = str.find_first_of('\n', poslf1+1);
	size_t poslf3 = str.find_first_of('\n', poslf2+1);

	std::string ver_line = str.substr(poslf2+1, poslf3-poslf2-1);

	static const xp::sregex ver_regex = xp::sregex::compile("[0-9]+\\.[0-9]+(?:\\.[0-9]+[\\.-][0-9]+)?");
	xp::smatch what;
	if (xp::regex_search(ver_line, what, ver_regex))
		return algo::replace_all_copy(what[0].str(), "-", ".");

	return std::string();
}

std::vector<unsigned int> ParseVersion(const std::string& ver)
{
	std::vector<std::string> svec;
	algo::split(svec, ver, std::bind1st(std::equal_to<char>(),'.') );

	std::vector<unsigned int> uvec = boost::assign::list_of(0u)(0u)(0u)(0u);

	size_t len = std::min(uvec.size(), svec.size());
	try
	{
		for(size_t i=0; i<len; ++i)
			uvec[i] = boost::lexical_cast<unsigned int>(svec[i]);
	}
	catch (boost::bad_lexical_cast&) {}

	return uvec;
}

bool IsFirstNewer(const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2, bool check_prerelease)
{
	for(size_t i=0; i<3; ++i)
	{
		if (v1[i] > v2[i])
			return true;

		if (v1[i] < v2[i])
			return false;
	}

	if (check_prerelease && (v1[3] > v2[3]))
		return true;

	return false;
}

void UpdatePeriods::DoInit()
{
	time_t timemax = std::numeric_limits<time_t>::max();

	AddData(UP_DAILY,   wxT("daily"),   _("every day"),   time_t(3600*24));
	AddData(UP_WEEKLY,  wxT("weekly"),  _("every week"),  time_t(3600*24*7));
	AddData(UP_MONTHLY, wxT("monthly"), _("every month"), time_t(3600*24*30));
	AddData(UP_NEVER,   wxT("never"),   _("never"),       timemax);

	SetDefault(default_idx, default_peroid);
}

time_t UpdatePeriods::ConfigToPeroid(const wxString& cfg) const
{
	return IndexToVal(ConfigToIndex(cfg));
}

struct UpdatesCheckingThread : public wxThread
{
	UpdatesCheckingThread(wxEventType evt_type)
		: m_env_type(evt_type)
	{}

	virtual ExitCode Entry() override
	{
		g_result_checkupdates = wxm::CheckUpdates();

		wxCommandEvent evt(m_env_type);
		wxPostEvent(g_MainFrame, evt);

		return (ExitCode)0;
	}

private:
	wxEventType m_env_type;
};

void AutoCheckUpdates(wxFileConfig* cfg)
{
	bool default_check_prerelease = false;
#ifdef __WXMSW__
	default_check_prerelease = true;
#endif
	cfg->Read(wxT("/wxMEdit/CheckPrereleaseUpdates"), &g_check_prerelease, default_check_prerelease);

	long lasttime = 0;
	wxString periodcfg;
	cfg->Read(wxT("/wxMEdit/LastTimeAutoCheckUpdates"), &lasttime, 0);
	cfg->Read(wxT("/wxMEdit/UpdatesCheckingPeriod"), &periodcfg);

	time_t now = time(nullptr);
	if (now-lasttime < UpdatePeriods::Instance().ConfigToPeroid(periodcfg))
		return;

	cfg->Write(wxT("/wxMEdit/LastTimeAutoCheckUpdates"), long(now));

	g_update_checking = true;

	UpdatesCheckingThread* t = new UpdatesCheckingThread(wxmEVT_RESULT_AUTOCHECKUPDATES);
	if (t->Create()==wxTHREAD_NO_ERROR && t->Run()==wxTHREAD_NO_ERROR)
		return;

	g_update_checking = false;

#ifdef _DEBUG
	wxMessageBox(wxT("Automatically Update Thread Error."), wxT("wxMEdit - Error"), wxICON_ERROR|wxOK);
#endif
}

void ManualCheckUpdates()
{
	g_update_checking = true;

	wxm::UpdatesCheckingThread* t = new wxm::UpdatesCheckingThread(wxmEVT_RESULT_MANUALCHECKUPDATES);
	if (t->Create()==wxTHREAD_NO_ERROR && t->Run()==wxTHREAD_NO_ERROR)
		return;

	wxm::g_result_checkupdates.clear();

	wxCommandEvent evt(wxmEVT_RESULT_MANUALCHECKUPDATES);
	
	wxPostEvent(g_MainFrame, evt);
}

} //namespace wxm
