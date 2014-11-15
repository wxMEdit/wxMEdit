///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_utils.hpp
// Description: Template Utilities
// Author:      wxmedit@gmail.com
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UTILS_HPP_
#define _XM_UTILS_HPP_

namespace xm
{
	template<typename _MAP>
	struct map_wrapper
	{
		typedef typename _MAP::key_type _K;
		typedef typename _MAP::mapped_type _V;
		typedef typename _MAP::const_iterator _CIT;

		map_wrapper(const _MAP& m) :m_map(m) {}

		const _V& get(const _K& key, const _V& default_val) const
		{
			_CIT it = m_map.find(key);
			if (it == m_map.end())
				return default_val;

			return it->second;
		}
	private:
		const _MAP& m_map;
	};

	template<typename _MAP>
	map_wrapper<_MAP> wrap_map(const _MAP& m)
	{
		return map_wrapper<_MAP>(m);
	}
}; // namespace xm

#endif //_XM_UTILS_HPP_
