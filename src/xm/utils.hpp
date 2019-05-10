///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_utils.hpp
// Description: Template Utilities
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UTILS_HPP_
#define _XM_UTILS_HPP_

namespace xm
{
	template<typename MAP>
	struct map_wrapper
	{
		typedef typename MAP::key_type K;
		typedef typename MAP::mapped_type V;
		typedef typename MAP::const_iterator CIT;

		map_wrapper(const MAP& m) :m_map(m) {}

		const V& get(const K& key, const V& default_val) const
		{
			CIT it = m_map.find(key);
			if (it == m_map.end())
				return default_val;

			return it->second;
		}
	private:
		const MAP& m_map;
	};

	template<typename MAP>
	map_wrapper<MAP> wrap_map(const MAP& m)
	{
		return map_wrapper<MAP>(m);
	}
}; // namespace xm

#endif //_XM_UTILS_HPP_
