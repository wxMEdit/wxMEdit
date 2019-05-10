///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Test Utils
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#include "test_detenc.h"
#include "../../src/xm/cxx11.h"
#include "../../src/xm/encdet.h"
#include "../../src/xm/encoding/encoding.h"

#include <unicode/unistr.h>
using U_ICU_NAMESPACE::UnicodeString;

#define BOOST_TEST_INCLUDED
#include <boost/scoped_array.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>

bool javaesc_to_enc(std::string& dest, const std::string& src, const std::string& enc)
{
	UnicodeString us = UnicodeString(src.data(), (int32_t)src.size()).unescape();

	int32_t uslen = us.length();

	int32_t slen = us.extract(0, uslen, nullptr, enc.c_str());
	if (slen <= 0)
		return false;
	boost::scoped_array<char> buf(new char[slen + 1]);
	us.extract(0, uslen, buf.get(), enc.c_str());

	dest = std::string(buf.get(), slen);

	return true;
}

void test_detenc_javaescaped(const std::string& jesc_text, const std::string& enc, bool skiped)
{
	std::string text;
	std::cout << "\t" << enc << std::endl;

	if (!javaesc_to_enc(text, jesc_text, enc))
	{
		std::cout << "\t\t<Test data not supported>" << std::endl;
		return;
	}

	if (skiped)
	{
		std::cout << "\t\t<Skiped>" << std::endl;
		return;
	}

	test_detenc(text, enc);
}

void test_detenc(const std::string& text, const std::string& enc)
{
	xm::EncodingManager& enccreator = xm::EncodingManager::Instance();

	std::string detenc;
	xm::EncodingID detencid = xm::ENC_DEFAULT;
	const ubyte* btext = (const ubyte*)text.data();

	bool det_succ = xm::MatchEncoding(detenc, btext, text.size());
	if (det_succ)
	{
		detencid = enccreator.ExtNameToEncoding(detenc);
	}
	else
	{
		xm::DetectEncoding(btext, text.size(), detencid, true);

		// use GB18030      instead of detected encoding MS936
		if (detencid == xm::ENC_MS936)
			detencid = xm::ENC_GB18030;
	}

	// use UHC          instead of expected encoding EUC-KR
	// use CP20932      instead of expected encoding EUC-JP /* FIXME later */
	std::string eenc = enc;
	if(eenc == "EUC-KR")
		eenc = "UHC";
	else if(eenc == "EUC-JP")
		eenc = "CP20932";

	// skip not supported encoding
	xm::EncodingID encid = enccreator.ExtNameToEncoding(eenc.c_str());
	if (encid == xm::ENC_DEFAULT)
	{
		std::cout << "\t\t" << "<Not supported yet>" << std::endl;
		return;
	}

	BOOST_CHECK(detencid==encid);
}

void test_predetenc(const std::string& text, const std::string& enc, bool matched)
{
	xm::EncodingManager& enccreator = xm::EncodingManager::Instance();

	std::string detenc;
	xm::EncodingID detencid = xm::ENC_DEFAULT;
	const ubyte* btext = (const ubyte*)text.data();

	bool det_succ = xm::MatchEncoding(detenc, btext, text.size());

	BOOST_CHECK(det_succ || !matched);
	if (!det_succ)
		return;

	detencid = enccreator.ExtNameToEncoding(detenc);
	xm::EncodingID encid = enccreator.ExtNameToEncoding(enc.c_str());

	BOOST_CHECK((detencid==encid) == matched);
}

void test_predetenc_javaescaped(const std::string& jesc_text, const std::string& enc, bool matched)
{
	std::string text;
	std::string match = (matched)? "": "not ";
	std::cout << "\t" << match << enc << std::endl;

	if (!javaesc_to_enc(text, jesc_text, enc))
	{
		std::cout << "\t\t<Test data not supported>" << std::endl;
		return;
	}

	test_predetenc(text, enc, matched);
}

void test_predetenc_wrap(const std::string& raw_text, const std::string& enc, bool matched)
{
	std::string match = (matched)? "": "not ";
	std::cout << "\t" << match << enc << std::endl;

	test_predetenc(raw_text, enc, matched);
}
