#include "test.h"
#include "../src/xm/wxm_encoding/encoding.h"
#include <boost/foreach.hpp>
#include <boost/test/minimal.hpp>

int test_main(int, char** const)
{
	int n = test_gb18030_conv();
	n += test_doublebyte_conv();
	n += test_singlebyte_conv();

	return n;
}

typedef ucs4_t ByteArr[256];
typedef std::map<std::string, const ByteArr*> B2UMap;
extern B2UMap b2u;
typedef unsigned char uchar_t_;
typedef std::map<ucs4_t, uchar_t_> U2BDataMap;
typedef std::map<std::string, U2BDataMap*> U2BMap;
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

typedef std::map<std::string, ucs4_t> MB2UDataMap;
typedef std::map<std::string, MB2UDataMap> MB2UMap;
extern MB2UMap mb2u;
typedef std::map<ucs4_t, std::string> U2MBDataMap;
typedef std::map<std::string, U2MBDataMap> U2MBMap;
extern U2MBMap u2mb;
extern std::vector<std::string> test_doublebyte_conv_enc_list;

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

int test_doublebyte_conv()
{
	wxm::WXMEncodingCreator::Instance().InitEncodings();
	test_doublebyte_conv_init();

	BOOST_FOREACH(const std::string& encname, test_doublebyte_conv_enc_list)
	{
		std::cout << encname << std::endl;
		test_a_doublebyte_conv(encname);
	}

	wxm::WXMEncodingCreator::Instance().FreeEncodings();

	return (int)boost::minimal_test::errors_counter();
}


int test_gb18030_conv()
{
	wxm::WXMEncodingCreator::Instance().InitEncodings();
	test_gb18030_conv_init();

	const std::string encname("GB18030");
	std::cout << encname << std::endl;

	wxString wxencname(encname.c_str(), wxConvUTF8);
	wxm::WXMEncoding* enc = wxm::WXMEncodingCreator::Instance().CreateWxmEncoding(wxencname);

	MB2UDataMap::const_iterator mb2uend = mb2u[encname].end();
	for (size_t i=0; i<256; ++i)
	{
		wxByte wxb[3] = {wxByte(i), 0, 0};
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

		for(size_t j=1; j<256; ++j)
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
	for (size_t i=0x81; i<0x90; ++i)
	{
		for (size_t j=0x30; j<=0x39; ++j)
		{
			for (size_t k=0x81; k<=0xFE; ++k)
			{
				for (size_t l=0x30; l<=0x39; ++l)
				{
					wxByte wxb[4] = {wxByte(i), wxByte(j), wxByte(k), wxByte(l)};
					ucs4_t u = enc->MultiBytetoUCS4(wxb);
					if (u != 0)
					{
						char mbs_arr[5] = {i, j, k, l, 0};
						MB2UDataMap::const_iterator it = mb2u[encname].find(mbs_arr);
						BOOST_CHECK(it != mb2uend);
						if (it != mb2uend)
						{
							ucs4_t t = it->second;
							BOOST_CHECK(u == t);
						}
						else
						{
							BOOST_CHECK(it == mb2uend);
						}
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
					wxByte wxb[4] = {wxByte(i), wxByte(j), wxByte(k), wxByte(l)};
					ucs4_t u = enc->MultiBytetoUCS4(wxb);
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
		wxByte buf[5] = {0, 0, 0, 0, 0};
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
		wxByte buf[5] = {0, 0, 0, 0, 0};
		size_t n = enc->UCS4toMultiByte(i, buf);
		BOOST_CHECK(n==4);

		wxDword t = i - 0x10000;
		BOOST_CHECK(buf[0] == 0x90 + t / 12600);

		t = t % 12600;
		BOOST_CHECK(buf[1] == 0x30 + t / 1260);

		t = t % 1260;
		BOOST_CHECK(buf[2] = 0x81 + t / 10);
		BOOST_CHECK(buf[3] = 0x30 + t % 10);
	}


	mb2u[encname].clear();
	u2mb[encname].clear();

	wxm::WXMEncodingCreator::Instance().FreeEncodings();

	return (int)boost::minimal_test::errors_counter();
}

