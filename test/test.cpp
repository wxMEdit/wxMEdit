#include "encoding_test.h"

#include <boost/version.hpp>

#if BOOST_VERSION >= 103400
# include <boost/test/included/unit_test.hpp>
#else
# include <boost/test/included/unit_test_framework.hpp>
#endif

#include <iostream>

boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
	boost::unit_test::test_suite* test = BOOST_TEST_SUITE("encoding_test");

	test->add(BOOST_TEST_CASE(&test_gb18030_conv));
	test->add(BOOST_TEST_CASE(&test_doublebyte_conv));
	test->add(BOOST_TEST_CASE(&test_singlebyte_conv));

	return test;
}
