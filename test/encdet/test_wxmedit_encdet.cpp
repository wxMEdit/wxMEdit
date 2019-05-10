///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Test Utils
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "../encdet_test.h"
#include "test_detenc.h"
#include "data_from_mozdet.h"

#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> utf32_cases = boost::assign::list_of
	("a") // 1 character == 4 bytes
	("\\u6F22\\u5B57") // 2 characters == 8 bytes
	("\\uD85A\\uDE77\\uD85A\\uDE87\\u4E2D") // 3 characters == 12 bytes
	;

std::vector<std::string> utf32_invalid_cases = boost::assign::list_of
	("a")
	("ab")
	("abc")
	;

std::vector<std::string> utf16_cases = boost::assign::list_of
	("a")
	("Unicode")
	("\\u6F22\\u5B57 - Chinese")
	("\\uD85A\\uDE77\\uD85A\\uDE87\\u4E2D")
	;

std::vector<std::string> utf16_notsure_cases = boost::assign::list_of
	("\\u6F22\\u5B57")
	("\\uFF21\\uFF41")
	("\\u03A9\\u03C9")
	("\\u0411\\u0431")
	("\\u1FFB\\u1F7D")
	("\\u10A5\\u2D05")
	("\\u2C88\\u2C89")
	("\\uA758\\uA759")
	("\\uA758\\uA759")
	;

std::vector<std::string> utf8_invalid_cases = boost::assign::list_of
	("\xC0\xAF")
	("\xE0\x9F\xBF")
	("\xED\xA0\x80")
	("\xED\xBF\xBF")
	("\xF0\x8F\xBF\xBF")
	("\xF4\x90\x80\x80")
	;

const char u32le_bom[] = {'\xFF', '\xFE', '\0', '\0'};
const char u32be_bom[] = {'\0', '\0', '\xFE', '\xFF'};
std::vector<EncAndText> bom_cases = boost::assign::list_of
	(EncAndText("UTF-8", "\xEF\xBB\xBF"))
	(EncAndText("UTF-16LE", "\xFF\xFE"))
	(EncAndText("UTF-16BE", "\xFE\xFF"))
	(EncAndText("UTF-32LE", std::string(u32le_bom, 4)))
	(EncAndText("UTF-32BE", std::string(u32be_bom, 4)))
	(EncAndText("GB18030", "\x84\x31\x95\x33"))
	;

std::vector<std::string> iso646_cases = boost::assign::list_of
	("an US-ASCII text")
	("$ is RMB yuan in ISO-646-CN")
	("# is pound in ISO-646-GB")
	("\\ is yen in ISO-646-JA")
	(":-)")
	;

void test_encdet_wxmedit_utf32()
{
	std::cout << "wxMEdit-encdet-UTF32" << std::endl;
	BOOST_FOREACH(const std::string& txt, utf32_cases)
	{
		test_predetenc_javaescaped(txt, "UTF-32LE");
		test_predetenc_javaescaped(txt, "UTF-32BE");
	}

	BOOST_FOREACH(const std::string& txt, utf32_invalid_cases)
	{
		test_predetenc_wrap(txt, "UTF-32LE", false);
		test_predetenc_wrap(txt, "UTF-32BE", false);
	}
}

void test_encdet_wxmedit_utf16()
{
	std::cout << "wxMEdit-encdet-UTF16" << std::endl;
	BOOST_FOREACH(const std::string& txt, utf16_cases)
	{
		test_predetenc_javaescaped(txt, "UTF-16LE");
		test_predetenc_javaescaped(txt, "UTF-16BE");
	}

	BOOST_FOREACH(const std::string& txt, utf16_notsure_cases)
	{
		test_predetenc_javaescaped(txt, "UTF-16LE", false);
		test_predetenc_javaescaped(txt, "UTF-16BE", false);
	}
}

void test_encdet_wxmedit_utf8()
{
	std::cout << "wxMEdit-encdet-UTF8" << std::endl;
	BOOST_FOREACH(const EncAndText& enc_txt, moz811363cases)
	{
		if (enc_txt._enc == "UTF-8")
			test_predetenc_javaescaped(enc_txt._text, "UTF-8");
	}

	BOOST_FOREACH(const std::string& txt, utf8_invalid_cases)
	{
		test_predetenc_wrap(txt, "UTF-8", false);
	}
}

void test_encdet_wxmedit_bom()
{
	std::cout << "wxMEdit-encdet-BOM" << std::endl;
	BOOST_FOREACH(const EncAndText& enc_txt, bom_cases)
	{
		test_predetenc_wrap(enc_txt._text, enc_txt._enc);
	}
}

void test_encdet_wxmedit_iso646()
{
	std::cout << "wxMEdit-encdet-ISO-646" << std::endl;
	BOOST_FOREACH(const std::string& txt, iso646_cases)
	{
		test_predetenc_wrap(txt, "US-ASCII");
	}
}
