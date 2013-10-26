#ifndef WXMEDIT_TEST_DETENC_H
#define WXMEDIT_TEST_DETENC_H

#include <string>

bool javaesc_to_enc(std::string& dest, const std::string& src, const std::string& enc);
void test_detenc(const std::string& text, const std::string& enc);

#endif //WXMEDIT_TEST_DETENC_H
