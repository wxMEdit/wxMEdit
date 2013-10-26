#ifndef WXMEDIT_TEST_DATA_FROM_ICUDET_H
#define WXMEDIT_TEST_DATA_FROM_ICUDET_H

#include <string>
#include <vector>

struct ICUEncDetTestCase
{
	std::string _tid;
	std::string _encs;
	std::string _text;

	ICUEncDetTestCase(const std::string& tid, 
			const std::string& encs, 
			const std::string& text)
		:_tid(tid), _encs(encs), _text(text)
	{
	}
};

extern std::vector<ICUEncDetTestCase> data_icudet_tests;

#endif //WXMEDIT_TEST_DATA_FROM_ICUDET_H
