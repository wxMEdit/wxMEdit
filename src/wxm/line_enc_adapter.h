///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/line_enc_adapter.h
// Description: Adapters for MadLines and WXMEncoding
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_LINE_ENC_ADAPTER_H_
#define _WXM_LINE_ENC_ADAPTER_H_

#include "../xm/cxx11.h"
#include "../wxmedit/ucs4_t.h"
#include "../wxmedit/wxm_deque.hpp"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/defs.h>
#include <wx/filefn.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <vector>
#include <utility>

struct MadBlock;
typedef std::vector < MadBlock > MadBlockVector;
typedef MadBlockVector::iterator MadBlockIterator;
typedef std::pair<ucs4_t, int>   MadUCPair;  // ucs4char, uc_len
typedef MadDeque<MadUCPair>      MadUCQueue;

namespace wxm
{

struct WXMBlockDumper
{
	WXMBlockDumper(MadBlockIterator& bit)
		:m_bit(bit)
	{
	}
	virtual void Dump(wxByte* buf, size_t len) = 0;

	virtual ~WXMBlockDumper() {}
protected:
	MadBlockIterator& m_bit;
};

struct WXMBackwardBlockDumper: public WXMBlockDumper
{
	WXMBackwardBlockDumper(MadBlockIterator& bit): WXMBlockDumper(bit) {}
	virtual void Dump(wxByte* buf, size_t len) override;
};

struct WXMForwardBlockDumper: public WXMBlockDumper
{
	WXMForwardBlockDumper(MadBlockIterator& bit): WXMBlockDumper(bit) {}
	virtual void Dump(wxByte* buf, size_t len) override;
};

struct UChar32BytesMapper
{
	virtual void MoveUChar32Bytes(MadUCQueue &ucqueue, ucs4_t uc, size_t len) = 0;
	virtual wxByte* BufferLoadBytes(wxFileOffset& rest, size_t buf_len) = 0;
	
	virtual ~UChar32BytesMapper() {}
};

}; // namespace wxm
#endif //_WXM_LINE_ENC_ADAPTER_H_
