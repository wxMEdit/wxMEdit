///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_remote.h
// Description: Remote File Access
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "remote.h"
#include "cxx11.h"

#include <curl/curl.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

static size_t string_append(const void *buffer, size_t size, size_t nmemb, void *pstr)
{
	reinterpret_cast<std::string *>(pstr)->append((const char *)buffer, size * nmemb);
	return size * nmemb;
}

namespace xm
{

static bool initialized = false;

bool RemoteAccessInit()
{
	if (!initialized)
		initialized = (curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK);

	return initialized;
}

std::string GetRemoteText(const std::string& url)
{
	if (!RemoteAccessInit())
		return std::string();

	CURL* h = curl_easy_init();
	if (h == nullptr)
	{
		curl_global_cleanup();
		return std::string();
	}
	
	curl_easy_setopt(h, CURLOPT_URL, url.c_str());

	std::string txt;

	curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, &string_append);
	curl_easy_setopt(h, CURLOPT_WRITEDATA, &txt);

	curl_easy_setopt(h, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(h, CURLOPT_MAXREDIRS, 10);

	curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 0L);
	//curl_easy_setopt(h, CURLOPT_CAINFO, "./cacert.pem");

	curl_easy_setopt(h, CURLOPT_TIMEOUT, 3*60L);

	curl_easy_perform(h);

	curl_easy_cleanup(h);

	curl_global_cleanup();

	return txt;
}

void RemoteAccessCleanup()
{
	if (initialized)
		curl_global_cleanup();
}

} //namespace wxm

