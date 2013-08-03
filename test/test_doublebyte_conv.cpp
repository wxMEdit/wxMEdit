#include "test.h"
#include "../src/wxmedit/ucs4_t.h"
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
MB2UMap mb2u;
typedef std::map<ucs4_t, std::string> U2MBDataMap;
typedef std::map<std::string, U2MBDataMap> U2MBMap;
U2MBMap u2mb;
std::vector<std::string> test_doublebyte_conv_enc_list;

#ifdef __WXMSW__
void mb2u_cp932_init();
void u2mb_cp932_init();
#endif

void mb2u_cp936_init();
void u2mb_cp936_init();

void mb2u_cp949_init();
void u2mb_cp949_init();

void mb2u_cp950_init();
void u2mb_cp950_init();

#ifdef __WXMSW__
void mb2u_euc_jp_init();
void u2mb_euc_jp_init();
#endif

void test_doublebyte_conv_init()
{
#ifdef __WXMSW__
	mb2u_cp932_init();
	u2mb_cp932_init();
#endif

	mb2u_cp936_init();
	u2mb_cp936_init();

	mb2u_cp949_init();
	u2mb_cp949_init();

	mb2u_cp950_init();
	u2mb_cp950_init();

#ifdef __WXMSW__
	mb2u_euc_jp_init();
	u2mb_euc_jp_init();
#endif

}
