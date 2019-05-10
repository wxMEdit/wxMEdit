///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Test Utils for DoubleByte Encodings
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "data_multibyte_conv.h"
#include "../encoding_test.h"
#include "../../src/xm/encoding/encoding.h"

#define BOOST_TEST_INCLUDED
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

MB2UMap mb2u;
U2MBMap u2mb;
std::vector<std::string> test_doublebyte_conv_enc_list;

void data_doublebyte_conv_init()
{
	mb2u_cp932_init();
	u2mb_cp932_init();

	mb2u_cp936_init();
	u2mb_cp936_init();

	mb2u_cp949_init();
	u2mb_cp949_init();

	mb2u_cp950_init();
	u2mb_cp950_init();

	mb2u_cp20932_init();
	u2mb_cp20932_init();

}

void test_a_doublebyte_conv(const std::string& encname)
{
	std::wstring wencname(encname.begin(), encname.end());
	xm::Encoding* enc = xm::EncodingManager::Instance().GetEncoding(wencname);

	MB2UDataMap::const_iterator mb2uend = mb2u[encname].end();
	for (size_t i=0; i<256; ++i)
	{
		ubyte bs[2] = { ubyte(i), 0 };
		if (enc->IsLeadByte(bs[0]))
		{
			for(size_t j=0; j<256; ++j)
			{
				char mbs_arr[3] = { char(i), char(j), 0 };
				MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);

				bs[1] = ubyte(j);
				ucs4_t u = enc->MultiBytetoUCS4(bs);
				if (u != 0)
				{
					BOOST_CHECK(it != mb2uend);
					if (it != mb2uend)
					{
						ucs4_t t = it->second;
						BOOST_CHECK(u == t);
					}
				}
				else
				{
					BOOST_CHECK(it == mb2uend);
				}
			}
		}
		else
		{
			char mbs_arr[2] = { char(i), 0 };
			MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);
			ucs4_t u = enc->MultiBytetoUCS4(bs);
			if (u != 0)
			{
				BOOST_CHECK(it != mb2uend);
				if (it != mb2uend)
				{
					ucs4_t t = it->second;
					BOOST_CHECK(u == t);
				}
			}
			else
			{
				BOOST_CHECK(it == mb2uend);
			}
		}
	}

	U2MBDataMap::const_iterator u2mbend = u2mb[encname].end();
	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		ubyte buf[4];
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n <= 2);

		U2MBDataMap::const_iterator it = u2mb[encname].find(i);
		if (n == 1 || n == 2)
		{
			BOOST_CHECK(it != u2mbend);
			if (it != u2mbend)
			{
				buf[n] = 0;
				std::string mbs((const char*)buf);
				BOOST_CHECK(it->second == mbs);
			}
		}
		else if(n == 0)
		{
			BOOST_CHECK(it == u2mbend);
		}
	}

	mb2u[encname].clear();
	u2mb[encname].clear();
}

void test_doublebyte_conv()
{
	xm::EncodingManager::Instance().InitEncodings();
	data_doublebyte_conv_init();

	std::cout << "wxMEdit-enc-doublebyte" << std::endl;
	BOOST_FOREACH(const std::string& encname, test_doublebyte_conv_enc_list)
	{
		std::cout << "\t" << encname << std::endl;
		test_a_doublebyte_conv(encname);
	}

	xm::EncodingManager::Instance().FreeEncodings();
}
