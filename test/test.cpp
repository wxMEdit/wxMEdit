#include "test.h"
#include "../src/xm/wxm_encoding.h"
#include <boost/foreach.hpp>
#include <boost/test/minimal.hpp>

int test_main(int, char** const)
{
	return test_singlebyte_conv();
}
typedef ucs4_t ByteArr[256];
typedef std::map<std::string, const ByteArr*> B2UMap;
extern B2UMap b2u;
typedef unsigned char uchar_t_;
typedef std::map<ucs4_t, uchar_t_> U2BDataMap;
typedef std::map<std::string, const U2BDataMap*> U2BMap;
extern U2BMap u2b;
extern std::vector<std::string> test_singlebyte_conv_enc_list;

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
	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		wxByte buf[4];
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n < 2);

		U2BDataMap::const_iterator it = u2b[encname]->find(i);
		U2BDataMap::const_iterator u2bend = u2b[encname]->end();
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
}

int test_singlebyte_conv()
{
	wxm::WXMEncodingCreator::Instance().InitEncodings();
	test_singlebyte_conv_init();

	BOOST_FOREACH(const std::string& encname, test_singlebyte_conv_enc_list)
	{
		std::cout << encname << std::endl;
		test_a_singlebyte_conv(encname);
	}

	wxm::WXMEncodingCreator::Instance().FreeEncodings();

	return (int)boost::minimal_test::errors_counter();
}
