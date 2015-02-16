///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/cxx11.h
// Description: C++ 11 adaptor
// Copyright:   2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_CXX11_H_
#define _XM_CXX11_H_

#include <boost/version.hpp>
#include <boost/config.hpp>

#if (__cplusplus <= 199711L) && (BOOST_VERSION <= 104000)
# define XM_NO_NULLPTR_
#else
# if defined(BOOST_NO_CXX11_NULLPTR) || defined(BOOST_NO_NULLPTR)
#  define XM_NO_NULLPTR_
# endif
#endif

#if defined(XM_NO_NULLPTR_)
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
#endif

#endif //_XM_CXX11_H_
