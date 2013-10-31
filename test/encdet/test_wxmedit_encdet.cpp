#include "../encdet_test.h"
#include "test_detenc.h"

#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> utf32_cases = boost::assign::list_of
	("a") // 1 character == 4 bytes
	("\\u6f22\\u5b57") // 2 characters == 8 bytes
	("\\ud85a\\ude77\\ud85a\\ude87\\u4e2d") // 3 characters == 12 bytes
	;

void test_encdet_wxmedit_utf32()
{
	std::cout << "wxMEdit-UTF32" << std::endl;
	BOOST_FOREACH(const std::string& txt, utf32_cases)
	{
		test_predetenc_javaescaped(txt, "UTF-32LE");
		test_predetenc_javaescaped(txt, "UTF-32BE");
	}
}
