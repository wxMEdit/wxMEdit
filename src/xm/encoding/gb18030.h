///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/gb18030.h
// Description: Define the Multi-byte Encoding GB18030
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_GB18030_H_
#define _XM_ENCODING_GB18030_H_

#include "../cxx11.h"
#include "multibyte.h"
#include <boost/array.hpp>
#include <map>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace xm
{

struct EncodingGB18030: public MultiByteEncoding, EncodingDecoderISO646
{
	virtual void MultiByteInit() override;
	virtual ucs4_t MultiBytetoUCS4(const ubyte* buf) override;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;

	virtual bool IsSingleByteEncoding() override
	{
		return false;
	}
	virtual bool IsDoubleByteEncoding() override
	{
		return false;
	}
	virtual bool IsUnicodeEncoding() override
	{
		return true;
	}

private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	EncodingGB18030(): m_icucnv("GB18030")
	{
	}
	~EncodingGB18030()
	{
	}

	static const size_t BMP_QBYTE_CNT = 39420;
	static const size_t BMP_QBYTE_IDX_FFFD = 39417;
	boost::array<ucs4_t, BMP_QBYTE_CNT> m_qb2u;

	static const size_t BMP_DBYTE_CNT = (0xFE - 0x81 + 1)*(0xFE - 0x40 + 1);
	boost::array<ucs4_t, BMP_DBYTE_CNT> m_db2u;

	boost::array<uint32_t, 0x10000> m_bmp2mb;
	//std::map<ucs4_t, uint16_t> m_nonbmp2db;

	ICUConverter m_icucnv;

	ucs4_t QByte2NONBMP(const ubyte* buf);
	ucs4_t QByte2BMP(const ubyte* buf);
	ucs4_t DByte2BMP(const ubyte* buf);
	size_t NonBMP2QByte(ubyte* buf, ucs4_t ucs4);

	void CacheMBofUCS4(uint32_t& mb, ucs4_t u);
};

};// namespace xm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _XM_ENCODING_GB18030_H_
