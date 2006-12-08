/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Universal charset detector code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *          Kohei TAKETA <k-tak@void.in>
 *          Jim Huang <jserv.tw@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#pragma GCC visibility push(hidden)

#include "chardetect.h"
#include "nscore.h"
#include "nsUniversalDetector.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#   include <windows.h>
#endif


#ifdef _MANAGED
#pragma managed(push, off)
#endif


class DllDetector : public nsUniversalDetector
{
protected:
    char *charset_;

public:
    DllDetector()
	: nsUniversalDetector()
    {
	charset_ = NULL;
    }

    virtual ~DllDetector()
    {
	if (charset_) free(charset_);  
    }

    virtual void Report(const char* charset)
    {
	charset_ = strdup(charset);
    }

    virtual void Reset()
    {
	nsUniversalDetector::Reset();
	charset_ = strdup("");
    }

    const char* GetCharset() const
    {
	return charset_;
    }
};


#ifdef _WIN32
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif


CHARDET_IMEXPORT
int chardet_create(chardet_t* pdet)
{
    if (!pdet) return CHARDET_RESULT_NOMEMORY;

    *pdet = reinterpret_cast<chardet_t>(new DllDetector);
    if (*pdet) {
	return CHARDET_RESULT_OK;
    } else {
	return CHARDET_RESULT_NOMEMORY;
    }
}


CHARDET_IMEXPORT
void chardet_destroy(chardet_t det)
{
    if (det) {
	delete reinterpret_cast<DllDetector*>(det);
    }
}


CHARDET_IMEXPORT
int chardet_handle_data(chardet_t det, const char* data, unsigned int len)
{
    if (det) {
	nsresult ret = reinterpret_cast<DllDetector*>(det)->HandleData(data, (PRUint32)len);
	if (ret == NS_OK) {
	    return CHARDET_RESULT_OK;
	} else {
	    return CHARDET_RESULT_NOMEMORY;
	}
    } else {
	return CHARDET_RESULT_INVALID_DETECTOR;
    }
}


CHARDET_IMEXPORT
int chardet_data_end(chardet_t det)
{
    if (det) {
	reinterpret_cast<DllDetector*>(det)->DataEnd();
	return CHARDET_RESULT_OK;
    } else {
	return CHARDET_RESULT_INVALID_DETECTOR;
    }
}


CHARDET_IMEXPORT
int chardet_reset(chardet_t det)
{
    if (det) {
	reinterpret_cast<DllDetector*>(det)->Reset();
	return CHARDET_RESULT_OK;
    } else {
	return CHARDET_RESULT_INVALID_DETECTOR;
    }
}


CHARDET_IMEXPORT
int chardet_get_charset(chardet_t det, char* namebuf, unsigned int buflen)
{
    if (det) {
	if (!namebuf) return CHARDET_RESULT_NOMEMORY;

	const char* name = reinterpret_cast<DllDetector*>(det)->GetCharset();
	if (name == NULL || *name == 0) {
	    // could not detect encoding
	    if (buflen > 0) {
		*namebuf = 0;
		return CHARDET_RESULT_OK;
	    } else {
		return CHARDET_RESULT_NOMEMORY;
	    }
	} else {
	    // encoding detected
	    if (buflen >= strlen(name)+1) {
		strcpy(namebuf, name);
		return CHARDET_RESULT_OK;
	    } else {
		return CHARDET_RESULT_NOMEMORY;
	    }
	}
    } else {
	return CHARDET_RESULT_INVALID_DETECTOR;
    }
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

#pragma GCC visibility pop

