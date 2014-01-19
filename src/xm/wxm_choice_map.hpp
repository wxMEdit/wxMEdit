///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_choice_map.hpp
// Description: Logical Data Structure for some Choice Items in Options Dialog
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_CHOICE_MAP_HPP_
#define _WXM_CHOICE_MAP_HPP_

#include <wx/string.h>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>

namespace wxm
{

template <typename _CLS, typename _VAL>
class ChoiceMap
{
	typedef std::map<int, _VAL>          IdxValMap;
	typedef std::map<int, wxString>      IdxTitleMap;
	typedef std::map<wxString, wxString> TitleCfgMap;
	typedef std::map<wxString, int>      CfgIdxMap;

	IdxValMap   m_idx_val_map;
	IdxTitleMap m_idx_title_map;
	TitleCfgMap m_title_cfg_map;
	CfgIdxMap   m_cfg_idx_map;

	int  m_default_idx;
	_VAL m_default_val;

	bool m_initialized;

protected:
	ChoiceMap(): m_initialized(false) {}

	void AddData(int idx, const wxString& cfg, const wxString &title, _VAL val)
	{
		m_idx_val_map[idx] = val;
		m_idx_title_map[idx] = title;
		m_title_cfg_map[title] = cfg;
		m_cfg_idx_map[cfg] = idx;
		
	}

	void SetDefault(int idx, _VAL val)
	{
		m_default_idx = idx;
		m_default_val = val;
	}

	virtual void DoInit() = 0;

public:
	virtual ~ChoiceMap() {}

	static _CLS& Instance()
	{
		static _CLS s_inst;
		if (!s_inst.m_initialized)
			s_inst.Initialize();
		return s_inst;
	}

	void Initialize()
	{
		if (m_initialized)
			return;
		m_initialized = true;
		DoInit();
	}

	wxString IndexToTitle(int idx) const
	{
		IdxTitleMap::const_iterator it = m_idx_title_map.find(idx);
		if (it != m_idx_title_map.end())
			return it->second;

		return wxString();	
	}

	_VAL IndexToVal(int idx) const
	{
		typename IdxValMap::const_iterator it = m_idx_val_map.find(idx);
		if (it != m_idx_val_map.end())
			return it->second;

		return m_default_val;
	}

	int ConfigToIndex(const wxString& cfg) const
	{
		CfgIdxMap::const_iterator it = m_cfg_idx_map.find(cfg);
		if (it != m_cfg_idx_map.end())
			return it->second;

		return m_default_idx;
	}

	wxString TitleToConfig(const wxString& title) const
	{
		TitleCfgMap::const_iterator it = m_title_cfg_map.find(title);
		if (it != m_title_cfg_map.end())
			return it->second;

		return wxString();
	}

	std::vector<wxString> GetTitles() const
	{
		std::vector<wxString> titles;
		BOOST_FOREACH(const IdxTitleMap::value_type& v, m_idx_title_map)
			titles.push_back(v.second);
		return titles;
	}

	wxString GetDefaultTitle() const
	{
		return IndexToTitle(m_default_idx);
	}

	wxString ConfigToTitle(const wxString& cfg) const
	{
		return IndexToTitle(ConfigToIndex(cfg));
	}
};

}; //namespace wxm

#endif //_WXM_CHOICE_MAP_HPP_
