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
#ifndef ___CHARDET_H___
#define ___CHARDET_H___

//#ifdef _WIN32
//#   ifdef DLL_EXPORTS
//#	define CHARDET_IMEXPORT extern _declspec(dllexport)
//#   else
//#	define CHARDET_IMEXPORT extern _declspec(dllimport)
//#   endif
//#else
//#   define CHARDET_IMEXPORT extern __attribute__ ((visibility("default")))
//#endif
#define CHARDET_IMEXPORT

#include <stddef.h>

#define CHARDET_RESULT_OK		    0
#define CHARDET_RESULT_NOMEMORY		    (-1)
#define CHARDET_RESULT_INVALID_DETECTOR	    (-2)

#define CHARDET_MAX_ENCODING_NAME	    64


typedef void* chardet_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create an encoding detector.
 * @param pdet [out] pointer to a chardet_t variable that receives
 *             the encoding detector handle.
 * @return CHARDET_RESULT_OK if succeeded. CHARDET_RESULT_NOMEMORY otherwise.
 */
CHARDET_IMEXPORT int chardet_create(chardet_t* pdet);

/**
 * Destroy an encoding detector.
 * @param det [in] the encoding detector handle to be destroyed.
 */
CHARDET_IMEXPORT void chardet_destroy(chardet_t det);

/**
 * Feed data to an encoding detector.
 * @param det [in] the encoding detector handle
 * @param data [in] data
 * @param len [in] length of data in bytes.
 * @return CHARDET_RESULT_OK if succeeded.
 *         CHARSET_RESULT_NOMEMORY if running out of memory.
 *         CHARDET_RESULT_INVALID_DETECTOR if det was invalid.
 */
CHARDET_IMEXPORT int chardet_handle_data(chardet_t det, const char* data, unsigned int len);

/**
 * Notify an end of data to an encoding detctor.
 * @param det [in] the encoding detector handle
 * @return CHARDET_RESULT_OK if succeeded.
 *         CHARDET_RESULT_INVALID_DETECTOR if det was invalid.
 */
CHARDET_IMEXPORT int chardet_data_end(chardet_t det);

/**
 * Reset an encoding detector.
 * @param det [in] the encoding detector handle
 * @return CHARDET_RESULT_OK if succeeded.
 *         CHARDET_RESULT_INVALID_DETECTOR if det was invalid.
 */
CHARDET_IMEXPORT int chardet_reset(chardet_t det);

/**
 * Get the name of encoding that was detected.
 * @param det [in] the encoding detector handle
 * @param namebuf [in/out] pointer to a buffer that receives the name of
 *                detected encoding. A valid encoding name or an empty string
 *                will be written to namebuf. If an empty strng was written,
 *                the detector could not detect any encoding.
 *                Written strings will always be NULL-terminated.
 * @param buflen [in] length of namebuf
 * @return CHARDET_RESULT_OK if succeeded.
 *         CHARDET_RESULT_NOMEMORY if namebuf was too small to store
 *         the entire encoding name.
 *         CHARDET_RESULT_INVALID_DETECTOR if det was invalid.
 */
CHARDET_IMEXPORT int chardet_get_charset(chardet_t det, char* namebuf, unsigned int buflen);


#ifdef __cplusplus
};
#endif

#endif
