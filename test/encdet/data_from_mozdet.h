///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Description: Encoding Detection Cases from Mozilla
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////
#ifndef WXMEDIT_TEST_DATA_FROM_MOZDET_H
#define WXMEDIT_TEST_DATA_FROM_MOZDET_H

#include <string>
#include <vector>
#include <utility>

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
typedef std::vector<std::pair<size_t, MozCaseVec> > MozMultiCasesTickets;
extern MozMultiCasesTickets moz_muticases_tickets;

typedef std::vector<std::pair<size_t, EncAndText> > MozOtherTickets;
extern MozOtherTickets moz_other_tickes;
extern MozCaseVec moz811363cases;

#endif //WXMEDIT_TEST_DATA_FROM_MOZDET_H
