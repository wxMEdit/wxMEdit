#include "test_detenc.h"
#include "../../src/wxmedit/wxm_encdet.h"
#include "../../src/xm/wxm_encoding/encoding.h"

#include <unicode/unistr.h>
#include <wx/string.h>
#include <boost/scoped_array.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>

bool javaesc_to_enc(std::string& dest, const std::string& src, const std::string& enc)
{
	UnicodeString us = UnicodeString(src.data(), src.size()).unescape();

	int32_t uslen = us.length();

	int32_t slen = us.extract(0, uslen, NULL, enc.c_str());
	if (slen <= 0)
		return false;
	boost::scoped_array<char> buf(new char[slen + 1]);
	us.extract(0, uslen, buf.get(), enc.c_str());

	dest = std::string(buf.get(), slen);

	return true;
}

void test_detenc(const std::string& text, const std::string& enc)
{
	wxm::WXMEncodingCreator& enccreator = wxm::WXMEncodingCreator::Instance();

	wxString wxdetenc;
	wxm::WXMEncodingID detencid = wxm::ENC_DEFAULT;
	const wxByte* wxtext = (const wxByte*)text.data();

	bool is_uenc = MatchSimpleUnicode(wxdetenc, wxtext, text.size());
	if (is_uenc)
	{
		detencid = enccreator.ExtNameToEncoding(wxdetenc.mb_str().data());
	}
	else
	{
		DetectEncoding(wxtext, text.size(), detencid);

		// use GB18030 instead of detected encoding MS936
		if (detencid == wxm::ENC_MS936)
			detencid = wxm::ENC_GB18030;
	}

	// use Windows-1252 instead of expected encoding ISO-8859-1
	// use UHC          instead of expected encoding EUC-KR
	// use CP20932      instead of expected encoding EUC-JP /* FIXME later */
	std::string eenc = enc;
	if (eenc == "ISO-8859-1")
		eenc = "Windows-1252";
	else if(eenc == "EUC-KR")
		eenc = "UHC";
	else if(eenc == "EUC-JP")
		eenc = "CP20932";

	// skip not supported encoding
	wxm::WXMEncodingID encid = enccreator.ExtNameToEncoding(eenc.c_str());
	if (encid == wxm::ENC_DEFAULT)
	{
		std::cout << "\t\t" << "<Not supported yet>" << std::endl;
		return;
	}

	BOOST_CHECK(detencid==encid);
}
