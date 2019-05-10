///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Test Utils
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
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
}

void modify_cases(const std::string& tid, std::vector<std::string>& enclangs)
{
	if (tid == "IUC10-jp")
		enclangs.push_back("EUC-JP/ja");

	if (tid == "IUC10-en")
	{
		enclangs.pop_back();
		enclangs.push_back("US-ASCII/en");
	}

	if (tid == "IUC10-nl")
	{
		enclangs.pop_back();
		enclangs.push_back("US-ASCII/nl");
	}
}

static bool is_skiped_test(const std::string& tid, const std::string tenc)
{
	return skiped_test_cases.find(std::make_pair(tid, tenc)) != skiped_test_cases.end();
}

void test_icucase_encdet(const ICUEncDetTestCase& tcase)
{
	std::cout << tcase._tid << std::endl;

	std::vector<std::string> tenclangs;
	boost::algorithm::split(tenclangs, tcase._encs, boost::algorithm::is_any_of(" "));
	modify_cases(tcase._tid, tenclangs);

	BOOST_FOREACH(const std::string& tenclang, tenclangs)
	{
		std::string tenc = tenclang.substr(0, tenclang.find('/'));

		test_detenc_javaescaped(tcase._text, tenc, is_skiped_test(tcase._tid, tenc));
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
