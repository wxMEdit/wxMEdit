///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_uutils.cpp
// Description: Unicode Utilities
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "xm_uutils.h"
#include <unicode/brkiter.h>
#include <unicode/locid.h>
#include <boost/scoped_ptr.hpp>

namespace xm
{

void CountWords(const UnicodeString& ustr, size_t& cnt, size_t& ctrl_cnt, size_t& sp_cnt)
{
	UErrorCode status = U_ZERO_ERROR;
	boost::scoped_ptr<BreakIterator> bi (
			BreakIterator::createWordInstance(Locale::getDefault(), status)
			);
	bi->setText(ustr);
	int32_t p = bi->first();
	while (p != BreakIterator::DONE) 
	{
		++cnt;

		UChar32 ch = ustr.char32At(p);

		if (u_iscntrl(ch))
			++ctrl_cnt;
		else if(u_isspace(ch))
			++sp_cnt;

		p = bi->next();
	}
	--cnt;
}

size_t SimpleWordCounter::GetWordCount()
{
	size_t cnt = 0;
	size_t ctrl_cnt = 0;
	size_t sp_cnt = 0;
	CountWords(m_ustr, cnt, ctrl_cnt, sp_cnt);
	return cnt;
}

size_t SimpleWordCounter::GetWordCountNoCtrlNoSP()
{
	size_t cnt = 0;
	size_t ctrl_cnt = 0;
	size_t sp_cnt = 0;
	CountWords(m_ustr, cnt, ctrl_cnt, sp_cnt);
	return cnt - ctrl_cnt - sp_cnt;
}

}; // namespace xm
