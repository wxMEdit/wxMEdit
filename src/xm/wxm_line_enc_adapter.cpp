///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_line_enc_adapter.cpp
// Description: Adapters for MadLines and WXMEncoding
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_line_enc_adapter.h"
#include "../wxmedit/wxm_lines.h"

namespace wxm
{

void WXMBackwardBlockDumper::Dump(wxByte* buf, size_t len)
{
	wxFileOffset bsize = m_bit->m_Size;
	size_t count=len;
	size_t n;
	while (true)
	{
		n = count;
		if (bsize < n)
			n = bsize;
		m_bit->Get(bsize-n, buf+(count-=n), n);
		if (count == 0)
			break;

		bsize = (--m_bit)->m_Size;
	}
}

void WXMForwardBlockDumper::Dump(wxByte* buf, size_t len)
{
	wxFileOffset bsize = m_bit->m_Size;
	size_t idx = 0;
	size_t n;
	while (true)
	{
		n = len - idx;
		if (bsize < n)
			n = bsize;
		m_bit->Get(0, buf+idx, n);
		if ((idx+=n) == len)
			break;

		bsize = (++m_bit)->m_Size;
	}
}

}; // namespace wxm
