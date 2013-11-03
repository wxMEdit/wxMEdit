#include "data_singlebyte_conv.h"
#include "../encoding_test.h"
#include "../../src/xm/wxm_encoding/encoding.h"

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

void test_a_singlebyte_conv(const std::string& encname)
{
	wxString wxencname(encname.c_str(), wxConvUTF8);
	wxm::WXMEncoding* enc = wxm::WXMEncodingCreator::Instance().CreateWxmEncoding(wxencname);

	for (size_t i=0; i<256; ++i)
	{
		wxByte wxb = wxByte(i);
		ucs4_t u = enc->MultiBytetoUCS4(&wxb);
		ucs4_t t = (*b2u[encname])[i];
		BOOST_CHECK(u == t);
	}

	U2BDataMap::const_iterator u2bend = u2b[encname]->end();
	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		wxByte buf[4];
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
	wxm::WXMEncodingCreator::Instance().InitEncodings();
	data_singlebyte_conv_init();

	std::cout << "wxMEdit-enc-singlebyte" << std::endl;
	BOOST_FOREACH(const std::string& encname, test_singlebyte_conv_enc_list)
	{
		std::cout << "\t" << encname << std::endl;
		test_a_singlebyte_conv(encname);
	}

	wxm::WXMEncodingCreator::Instance().FreeEncodings();
}
