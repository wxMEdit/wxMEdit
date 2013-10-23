#include "encoding_test.h"
#include "data_multibyte_conv.h"
#include "../src/xm/wxm_encoding/encoding.h"

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

MB2UMap mb2u;
U2MBMap u2mb;
std::vector<std::string> test_doublebyte_conv_enc_list;

void test_doublebyte_conv_init()
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
	wxString wxencname(encname.c_str(), wxConvUTF8);
	wxm::WXMEncoding* enc = wxm::WXMEncodingCreator::Instance().CreateWxmEncoding(wxencname);

	MB2UDataMap::const_iterator mb2uend = mb2u[encname].end();
	for (size_t i=0; i<256; ++i)
	{
		wxByte wxb[2] = {wxByte(i), 0};
		if (enc->IsLeadByte(wxb[0]))
		{
			for(size_t j=0; j<256; ++j)
			{
				char mbs_arr[3] = {i, j, 0};
				MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);

				wxb[1] = wxByte(j);
				ucs4_t u = enc->MultiBytetoUCS4(wxb);
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
			char mbs_arr[2] = {i, 0};
			MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);
			ucs4_t u = enc->MultiBytetoUCS4(wxb);
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
		wxByte buf[4];
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
	wxm::WXMEncodingCreator::Instance().InitEncodings();
	test_doublebyte_conv_init();

	BOOST_FOREACH(const std::string& encname, test_doublebyte_conv_enc_list)
	{
		std::cout << encname << std::endl;
		test_a_doublebyte_conv(encname);
	}

	wxm::WXMEncodingCreator::Instance().FreeEncodings();

//	return (int)boost::minimal_test::errors_counter();
}
