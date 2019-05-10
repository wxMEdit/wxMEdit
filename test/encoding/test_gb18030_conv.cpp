///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Test Utils for GB18030
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "data_multibyte_conv.h"
#include "../encoding_test.h"
#include "../../src/xm/encoding/encoding.h"

#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>

#include <iostream>

void data_gb18030_conv_init()
{
	db2bmp_gb18030_init();
	qb2bmp_gb18030_init();
	bmp2db_gb18030_init();
	bmp2qb_gb18030_init();

}

void test_gb18030_conv()
{
	xm::EncodingManager::Instance().InitEncodings();
	data_gb18030_conv_init();

	const std::string encname("GB18030");
	std::cout << "wxMEdit-enc-" << encname << std::endl;
	std::cout << "\t" << encname << std::endl;

	std::wstring wencname(encname.begin(), encname.end());
	xm::Encoding* enc = xm::EncodingManager::Instance().GetEncoding(wencname);

	MB2UDataMap::const_iterator mb2uend = mb2u[encname].end();
	for (size_t i=0; i<256; ++i)
	{
		ubyte bs[3] = { ubyte(i), 0, 0 };
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

		for(size_t j=1; j<256; ++j)
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
	for (size_t i=0x81; i<0x90; ++i)
	{
		for (size_t j=0x30; j<=0x39; ++j)
		{
			for (size_t k=0x81; k<=0xFE; ++k)
			{
				for (size_t l=0x30; l<=0x39; ++l)
				{
					ubyte bs[4] = { ubyte(i), ubyte(j), ubyte(k), ubyte(l) };
					ucs4_t u = enc->MultiBytetoUCS4(bs);

					char mbs_arr[5] = { char(i), char(j), char(k), char(l), 0 };
					MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);
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
		}
	}
	for (size_t i=0x90; i<=0xFE; ++i)
	{
		for (size_t j=0x30; j<=0x39; ++j)
		{
			for (size_t k=0x81; k<=0xFE; ++k)
			{
				for (size_t l=0x30; l<=0x39; ++l)
				{
					ubyte bs[4] = { ubyte(i), ubyte(j), ubyte(k), ubyte(l) };
					ucs4_t u = enc->MultiBytetoUCS4(bs);
					ucs4_t t = (ucs4_t)((i-0x90)*12600 + (j-0x30)*1260 + (k-0x81)*10 + l-0x30 + 0x10000);
					if (u != 0)
						BOOST_CHECK(u == t);
					else
						BOOST_CHECK(t > 0x10FFFF);
				}
			}
		}
	}


	U2MBDataMap::const_iterator u2mbend = u2mb[encname].end();
	for (ucs4_t i=0; i<=0xFFFF; ++i)
	{
		ubyte buf[5] = {0, 0, 0, 0, 0};
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n<=4 && n!=3);

		U2MBDataMap::const_iterator it = u2mb[encname].find(i);
		if (n == 1 || n == 2 || n == 4)
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
	for (ucs4_t i=0x10000; i<=0x10FFFF; ++i)
	{
		ubyte buf[5] = {0, 0, 0, 0, 0};
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n==4);

		uint32_t t = i - 0x10000;
		BOOST_CHECK(buf[0] == 0x90 + t / 12600);

		t = t % 12600;
		BOOST_CHECK(buf[1] == 0x30 + t / 1260);

		t = t % 1260;
		BOOST_CHECK(buf[2] = 0x81 + t / 10);
		BOOST_CHECK(buf[3] = 0x30 + t % 10);
	}


	mb2u[encname].clear();
	u2mb[encname].clear();

	xm::EncodingManager::Instance().FreeEncodings();
}
