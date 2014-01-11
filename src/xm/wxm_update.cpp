///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_update.cpp
// Description: Checking for update of wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_update.h"
#include "xm_remote.h"
#include "wxm_def.h"
#include "../wxm_utils.h"

#include <wx/msgdlg.h>
#include <wx/intl.h>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include <algorithm>

namespace xp =  boost::xpressive;
namespace algo = boost::algorithm;
namespace wxm
{

std::string GetVersionFromRemoteChangeLog();
std::vector<unsigned int> ParseVersion(const std::string& ver);
bool IsFirstNewer(const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2);

inline bool IsFirstNewer(const std::string& v1, const std::string& v2)
{
	return IsFirstNewer(ParseVersion(v1), ParseVersion(v2));
}

void CheckUpdates()
{
	std::string remote_ver = GetVersionFromRemoteChangeLog();
	if (IsFirstNewer(remote_ver, WXMEDIT_VERSION))
	{
		wxString title(_("wxMEdit - New version available"));
		wxString msg( wxString::Format( _("wxMEdit %s is available. \nClick OK to open the download page."), 
		                                wxString(remote_ver.c_str(), wxConvUTF8).c_str() ) );
		if (wxOK == wxMessageBox(msg, title, wxICON_INFORMATION|wxOK|wxCANCEL))
			OpenURL(wxT("http://code.google.com/p/wxmedit/wiki/download?tm=2"));

		return;
	}

	wxMessageBox(_("wxMEdit is already the newest version."), wxT("wxMEdit"), wxICON_INFORMATION|wxOK);
}

std::string GetVersionFromRemoteChangeLog()
{
	std::string str(xm::get_remote_txt("https://raw.github.com/hltj/wxMEdit/master/ChangeLog"));

	size_t poslf1 = str.find_first_of('\n', 0);
	size_t poslf2 = str.find_first_of('\n', poslf1+1);
	size_t poslf3 = str.find_first_of('\n', poslf2+1);

	std::string ver_line = str.substr(poslf2+1, poslf3-poslf2-1);

	xp::sregex ver_regex = xp::sregex::compile("[0-9]+\\.[0-9]+\\.[0-9]+([\\.-][0-9]+)?");
	xp::smatch what;
	if (xp::regex_search(ver_line, what, ver_regex))
	{
		return what[0].str();
	}

	return std::string();
}

std::vector<unsigned int> ParseVersion(const std::string& ver)
{
	std::vector<std::string> svec;
	algo::split(svec, ver, algo::is_any_of(".-"));


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

bool IsFirstNewer(const std::vector<unsigned int>& v1, const std::vector<unsigned int>& v2)
{
	for(size_t i=0; i<4; ++i)
	{
		if (v1[i] > v2[i])
			return true;
	}

	return false;
}

} //namespace wxm

