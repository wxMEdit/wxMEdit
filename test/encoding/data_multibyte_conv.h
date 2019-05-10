///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Test Utils for MultiByte Encodings
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#ifndef WXMEDIT_TEST_MULTIBYTE_CONV_DATA_H
#define WXMEDIT_TEST_MULTIBYTE_CONV_DATA_H

#include "../../src/xm/encoding/encoding_def.h"

#include <string>
#include <map>
#include <vector>

struct MB2UData
{
	std::string	mbs;
	ucs4_t u;
};
struct U2MBData
{
	ucs4_t u;
	std::string	mbs;
};
typedef std::map<std::string, ucs4_t> MB2UDataMap;
typedef std::map<std::string, MB2UDataMap> MB2UMap;
extern MB2UMap mb2u;
typedef std::map<ucs4_t, std::string> U2MBDataMap;
typedef std::map<std::string, U2MBDataMap> U2MBMap;
extern U2MBMap u2mb;
extern std::vector<std::string> test_doublebyte_conv_enc_list;

void mb2u_cp932_init();
void u2mb_cp932_init();

void mb2u_cp936_init();
void u2mb_cp936_init();

void mb2u_cp949_init();
void u2mb_cp949_init();

void mb2u_cp950_init();
void u2mb_cp950_init();

void mb2u_cp20932_init();
void u2mb_cp20932_init();

void db2bmp_gb18030_init();
void qb2bmp_gb18030_init();
void bmp2db_gb18030_init();
void bmp2qb_gb18030_init();

#endif //WXMEDIT_TEST_MULTIBYTE_CONV_DATA_H
