#ifndef WXMEDIT_TEST_DATA_SINGLEBYTE_CONV_H
#define WXMEDIT_TEST_DATA_SINGLEBYTE_CONV_H

#include "../../src/xm/encoding/encoding_def.h"

#include <string>
#include <map>
#include <vector>

typedef ucs4_t ByteArr[256];
typedef std::map<std::string, const ByteArr*> B2UMap;
extern B2UMap b2u;
typedef unsigned char uchar_t_;
typedef std::map<ucs4_t, uchar_t_> U2BDataMap;
typedef std::map<std::string, U2BDataMap*> U2BMap;
extern U2BMap u2b;
extern std::vector<std::string> test_singlebyte_conv_enc_list;

#endif //WXMEDIT_TEST_DATA_SINGLEBYTE_CONV_H
