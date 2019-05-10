///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Test Utils
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "data_from_mozdet.h"
#include "../encdet_test.h"
#include "test_detenc.h"

#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <set>

std::set<size_t> skiped_test_ids = boost::assign::list_of (488426)(638318);

static bool is_skiped_test(size_t tid)
{
	return skiped_test_ids.find(tid) != skiped_test_ids.end();
}

void test_encdet_moz_muticases()
{
	BOOST_FOREACH(const MozMultiCasesTickets::value_type& tickets, moz_muticases_tickets)
	{
		std::cout << "Mozilla-" << tickets.first << std::endl;
		BOOST_FOREACH(const EncAndText& enc_txt, tickets.second)
			test_detenc_javaescaped(enc_txt._text, enc_txt._enc, false);
	}
}

void test_encdet_moz_others()
{
	BOOST_FOREACH(const MozOtherTickets::value_type& tcase, moz_other_tickes)
	{
		std::cout << "Mozilla-" << tcase.first << std::endl;
		const EncAndText& enc_txt = tcase.second;

		test_detenc_javaescaped(enc_txt._text, enc_txt._enc, is_skiped_test(tcase.first));
	}
}
