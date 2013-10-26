#include "data_from_icudet.h"
#include "../encdet_test.h"
#include "test_detenc.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <set>
#include <utility>
#include <iostream>

std::set<std::pair<std::string, std::string> > skiped_test_cases;

void skip_test_case(const std::string& tid, const std::string tenc)
{
	skiped_test_cases.insert(std::make_pair(tid, tenc));
}

void init_skiped_test_cases()
{
	skip_test_case("IUC10-ar", "ISO-8859-6");
	skip_test_case("IUC10-ar", "windows-1256");
	skip_test_case("IUC10-en", "ISO-8859-1");
	skip_test_case("IUC10-he", "ISO-8859-8-I");
	skip_test_case("IUC10-hu", "ISO-8859-2");
	skip_test_case("IUC10-hu-Q", "windows-1250");
	skip_test_case("IUC10-nl", "ISO-8859-1");
	skip_test_case("IUC10-ro", "ISO-8859-2");
	skip_test_case("WIU-cz", "ISO-8859-2");
	skip_test_case("WIU-el", "ISO-8859-7");
	skip_test_case("WIU-el-Q", "windows-1253");
	skip_test_case("WIU-pl", "ISO-8859-2");
	skip_test_case("WIU-tr", "ISO-8859-9");
	skip_test_case("WIU-tr-Q", "windows-1254");
}

bool is_skiped_test(const std::string& tid, const std::string tenc)
{
	if(skiped_test_cases.find(std::make_pair(tid, tenc)) != skiped_test_cases.end())
		return true;
		
	return false;
}

void test_icucase_encdet(const ICUEncDetTestCase& tcase)
{
	std::cout << tcase._tid << std::endl;

	std::vector<std::string> tenclangs;
	boost::algorithm::split(tenclangs, tcase._encs, boost::algorithm::is_any_of(" "));

	BOOST_FOREACH(const std::string& tenclang, tenclangs)
	{
		std::string tenc = tenclang.substr(0, tenclang.find('/'));
		std::string text;

		if (is_skiped_test(tcase._tid, tenc))
			continue;

		if (!javaesc_to_enc(text, tcase._text, tenc))
			continue;

		std::cout << "\t" << tenc << std::endl;

		test_detenc(text, tenc);
	}
}

void test_encdet_with_icucases()
{
	init_skiped_test_cases();

	BOOST_FOREACH(const ICUEncDetTestCase& tcase, data_icudet_tests)
	{
		test_icucase_encdet(tcase);
	}
}
