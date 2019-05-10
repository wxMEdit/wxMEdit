///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Test Utils for SingleByte Encodings
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "data_singlebyte_conv.h"
#include "../encoding_test.h"
#include "../../src/xm/encoding/encoding.h"

#define BOOST_TEST_INCLUDED
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

void test_a_singlebyte_conv(const std::string& encname)
{
	std::wstring wencname(encname.begin(), encname.end());
	xm::Encoding* enc = xm::EncodingManager::Instance().GetEncoding(wencname);

	for (size_t i=0; i<256; ++i)
	{
		ubyte bs = ubyte(i);
		ucs4_t u = enc->MultiBytetoUCS4(&bs);
		ucs4_t t = (*b2u[encname])[i];
		BOOST_CHECK(u == t);
	}

	U2BDataMap::const_iterator u2bend = u2b[encname]->end();
	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		ubyte buf[4];
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n < 2);

		U2BDataMap::const_iterator it = u2b[encname]->find(i);
		if (n == 1)
		{
			BOOST_CHECK(it != u2bend);
			if (it != u2bend)
				BOOST_CHECK(buf[0] == it->second);
		}
		else if(n == 0)
		{
			BOOST_CHECK(it == u2bend);
		}
	}

	u2b[encname]->clear();
}

void test_singlebyte_conv()
{
	xm::EncodingManager::Instance().InitEncodings();
	data_singlebyte_conv_init();

	std::cout << "wxMEdit-enc-singlebyte" << std::endl;
	BOOST_FOREACH(const std::string& encname, test_singlebyte_conv_enc_list)
	{
		std::cout << "\t" << encname << std::endl;
		test_a_singlebyte_conv(encname);
	}

	xm::EncodingManager::Instance().FreeEncodings();
}
