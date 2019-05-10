///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_remote.h
// Description: Remote File Access
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_REMOTE_H_
#define _WXM_REMOTE_H_

#include <string>

namespace xm
{
	bool RemoteAccessInit();
	void RemoteAccessCleanup();
	std::string GetRemoteText(const std::string& url);	
} //namespace wxm

#endif //_WXM_REMOTE_H_
