///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/cxx11.h
// Description: C++ 11 adaptor
// Copyright:   2015-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_CXX11_H_
#define _XM_CXX11_H_

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_NULLPTR) || defined(BOOST_NO_NULLPTR)
# define XM_NO_NULLPTR_
#endif

#ifdef XM_NO_NULLPTR_
# if defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ == 5 || \
     defined(__clang__) && __clang_major__ < 3
#  define nullptr __null
# else
const
class {
public:
	template<class T>
	operator T*() const
	{
		return 0;
	}
	template<class C, class T>
	operator T C::*() const
	{
		return 0;
	}
private:
	void operator&() const;
} nullptr = {};
# endif
#endif

#ifdef _MSC_VER
# if _MSC_VER == 1600
#  define final sealed
# elif _MSC_VER < 1600
#  define XM_NO_FINAL_OVERRIDE_
# endif
#elif defined(__clang__)
# if __clang_major__ < 3
#  define XM_NO_FINAL_OVERRIDE_
# endif
#elif defined(__GNUC__)
# if __GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 7
#  define XM_NO_FINAL_OVERRIDE_
# endif
#else
# define XM_NO_FINAL_OVERRIDE_
#endif

#ifdef XM_NO_FINAL_OVERRIDE_
# define final
# define override
#endif

#endif //_XM_CXX11_H_
