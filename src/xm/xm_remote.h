///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_remote.h
// Description: Remote File Access
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_REMOTE_H_
#define _WXM_REMOTE_H_

#include <string>

namespace xm
{
	std::string get_remote_txt(const std::string& url);	
} //namespace wxm

#endif //_WXM_REMOTE_H_
