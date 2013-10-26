#ifndef WXMEDIT_TEST_DATA_FROM_MOZDET_H
#define WXMEDIT_TEST_DATA_FROM_MOZDET_H

#include <string>
#include <vector>
#include <map>

struct EncAndText
{
	std::string _enc;
	std::string _text;
	EncAndText(const std::string& enc, const std::string& text)
		:_enc(enc), _text(text)
	{
	}
};

typedef std::vector<EncAndText> MozCaseVec;
typedef std::map<size_t, MozCaseVec> MozMultiCasesTicketsMap;
extern MozMultiCasesTicketsMap moz_muticases_map;

typedef std::map<size_t, EncAndText> MozOtherTicketsMap;
extern  MozOtherTicketsMap moz_others_map;

#endif //WXMEDIT_TEST_DATA_FROM_MOZDET_H
