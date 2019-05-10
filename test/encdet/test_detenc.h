///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Test Utils
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#ifndef WXMEDIT_TEST_DETENC_H
#define WXMEDIT_TEST_DETENC_H

#include <string>

bool javaesc_to_enc(std::string& dest, const std::string& src, const std::string& enc);
void test_detenc(const std::string& text, const std::string& enc);
void test_detenc_javaescaped(const std::string& jesc_text, const std::string& enc, bool skiped);
void test_predetenc(const std::string& text, const std::string& enc, bool matched);
void test_predetenc_javaescaped(const std::string& jesc_text, const std::string& enc, bool matched=true);
void test_predetenc_wrap(const std::string& raw_text, const std::string& enc, bool matched=true);

#endif //WXMEDIT_TEST_DETENC_H
