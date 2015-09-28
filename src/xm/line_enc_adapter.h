///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/line_enc_adapter.h
// Description: Adapters for MadLines and xm::Encoding
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_LINE_ENC_ADAPTER_H_
#define _XM_LINE_ENC_ADAPTER_H_

#include "../xm/cxx11.h"
#include "../xm/encoding/encoding_def.h"
#include "../wxmedit/wxm_deque.hpp"

#include <vector>
#include <utility>

struct MadBlock;
typedef std::vector < MadBlock > MadBlockVector;
typedef MadBlockVector::iterator MadBlockIterator;
typedef std::pair<ucs4_t, int>   MadUCPair;  // ucs4char, uc_len
typedef MadDeque<MadUCPair>      MadUCQueue;

namespace xm
{

struct BlockDumper
{
	BlockDumper(MadBlockIterator& bit)
		:m_bit(bit)
	{
	}
	virtual void Dump(ubyte* buf, size_t len) = 0;

	virtual ~BlockDumper() {}
protected:
	MadBlockIterator& m_bit;
};

struct BackwardBlockDumper: public BlockDumper
{
	BackwardBlockDumper(MadBlockIterator& bit): BlockDumper(bit) {}
	virtual void Dump(ubyte* buf, size_t len) override;
};

struct ForwardBlockDumper: public BlockDumper
{
	ForwardBlockDumper(MadBlockIterator& bit): BlockDumper(bit) {}
	virtual void Dump(ubyte* buf, size_t len) override;
};

struct UChar32BytesMapper
{
	virtual void MoveUChar32Bytes(MadUCQueue &ucqueue, ucs4_t uc, size_t len) = 0;
	virtual ubyte* BufferLoadBytes(int64_t& rest, size_t buf_len) = 0;
	
	virtual ~UChar32BytesMapper() {}
};

}; // namespace xm
#endif //_XM_LINE_ENC_ADAPTER_H_
