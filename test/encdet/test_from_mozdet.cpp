#include "data_from_mozdet.h"
#include "../encdet_test.h"
#include "test_detenc.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <utility>

void check_enc_and_text(const EncAndText& enc_and_text)
{
	std::string text;
	const std::string tenc = enc_and_text._enc;

	if (!javaesc_to_enc(text, enc_and_text._text, tenc))
		return;

	std::cout << "\t" << tenc << std::endl;
	test_detenc(text, tenc);
}

void test_encdet_moz_muticases()
{
	BOOST_FOREACH(const MozMultiCasesTicketsMap::value_type& tickets, moz_muticases_map)
	{
		std::cout << "Mozilla-" << tickets.first << std::endl;
		BOOST_FOREACH(const EncAndText& enc_and_text, tickets.second)
			check_enc_and_text(enc_and_text);
	}
}

void test_encdet_moz_others()
{
	BOOST_FOREACH(const MozOtherTicketsMap::value_type& tcase, moz_others_map)
	{
		std::cout << "Mozilla-" << tcase.first << std::endl;
		check_enc_and_text(tcase.second);
	}
}