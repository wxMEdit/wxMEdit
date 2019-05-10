///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/line_enc_adapter.h
// Description: Adapters for MadLines and xm::Encoding
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_LINE_ENC_ADAPTER_H_
#define _XM_LINE_ENC_ADAPTER_H_

#include "../xm/cxx11.h"
#include "../xm/encoding/encoding_def.h"

#include <deque>

#ifdef _MSC_VER
# include <boost/container/deque.hpp>
	namespace xm { using boost::container::deque; }
#else
# include <deque>
	namespace xm { using std::deque; }
#endif

#include <vector>
#include <utility>

struct MadBlock;

namespace xm
{

typedef std::vector < MadBlock > BlockVector;
typedef BlockVector::iterator BlockIterator;

struct CharUnit {
	ucs4_t ucs4() const { return u.first; }
	int nbytes() const { return u.second; }

	CharUnit(ucs4_t ucs4, int nbytes) : u(ucs4, nbytes) {}
private:
	std::pair<ucs4_t, int> u;
};

typedef deque<CharUnit> UCQueue;

struct BlockDumper
{
	BlockDumper(BlockIterator& bit)
		:m_bit(bit)
	{
	}
	virtual void Dump(ubyte* buf, size_t len) = 0;

	virtual ~BlockDumper() {}
protected:
	BlockIterator& m_bit;
};

struct BackwardBlockDumper: public BlockDumper
{
	BackwardBlockDumper(BlockIterator& bit): BlockDumper(bit) {}
	virtual void Dump(ubyte* buf, size_t len) override;
};

struct ForwardBlockDumper: public BlockDumper
{
	ForwardBlockDumper(BlockIterator& bit): BlockDumper(bit) {}
	virtual void Dump(ubyte* buf, size_t len) override;
};

struct UChar32BytesMapper
{
	virtual void MoveUChar32Bytes(UCQueue &ucqueue, ucs4_t uc, size_t len) = 0;
	virtual ubyte* BufferLoadBytes(int64_t& rest, size_t buf_len) = 0;
	
	virtual ~UChar32BytesMapper() {}
};

}; // namespace xm
#endif //_XM_LINE_ENC_ADAPTER_H_
