///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_uutils.cpp
// Description: Unicode Utilities
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "uutils.h"
#include <unicode/brkiter.h>
#include <unicode/locid.h>
#include <boost/scoped_ptr.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

void CountWords(const UnicodeString& ustr, size_t& cnt, size_t& ctrl_cnt, size_t& sp_cnt)
{
	UErrorCode status = U_ZERO_ERROR;
	boost::scoped_ptr<BreakIterator> bi (
			BreakIterator::createWordInstance(Locale::getDefault(), status)
			);
	bi->setText(ustr);
	int32_t i = bi->first();
	while (i < ustr.length())
	{
		++cnt;

		UChar32 ch = ustr.char32At(i);

		if (u_iscntrl(ch))
			++ctrl_cnt;
		else if(u_isspace(ch))
			++sp_cnt;

		i = bi->next();
	}
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

void AccumulativeWordCounter::operator+=(UChar32 ch)
{
	if (m_size >= m_capacity)
	{
		PiecewiseCount(ch);
		PiecewiseClear();
	}

	m_ustr += ch;
	++m_size;
}

void AccumulativeWordCounter::operator+=(const UnicodeString& ustr)
{
	for(int32_t i=0; i<ustr.length(); i=ustr.moveIndex32(i,1))
	{
		this->operator+=(ustr.char32At(i));
	}
}

size_t AccumulativeWordCounter::GetWordCount()
{
	size_t cnt = 0;
	size_t ctrl_cnt = 0;
	size_t sp_cnt = 0;
	CountWords(m_ustr, cnt, ctrl_cnt, sp_cnt);
	return m_cnt + cnt;
}

size_t AccumulativeWordCounter::GetWordCountNoCtrlNoSP()
{
	size_t cnt = 0;
	size_t ctrl_cnt = 0;
	size_t sp_cnt = 0;
	CountWords(m_ustr, cnt, ctrl_cnt, sp_cnt);
	return m_cnt + cnt - m_ctrl_cnt - ctrl_cnt - m_sp_cnt - sp_cnt;
}

void AccumulativeWordCounter::PiecewiseClear()
{
	m_size = 0;
	m_ustr.remove();
}

void AccumulativeWordCounter::PiecewiseCount(UChar32 ch)
{
	// count last part
	CountWords(m_ustr, m_cnt, m_ctrl_cnt, m_sp_cnt);

	// joint: last char of last part + first char of new part
	int32_t last_idx = m_ustr.moveIndex32(m_ustr.length(), -1);
	UnicodeString joint = m_ustr.char32At(last_idx);
	joint += ch;

	// count the joint
	size_t cnt = 0;
	size_t ctrl_cnt = 0;
	size_t sp_cnt = 0;
	CountWords(joint, cnt, ctrl_cnt, sp_cnt);

	// if joint is one word, should take off the last word from last part
	if (cnt == 1)
	{
		m_cnt -= cnt;
		m_ctrl_cnt -= ctrl_cnt;
		m_sp_cnt -= sp_cnt;
	}
}

}; // namespace xm
