///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/gb18030.h
// Description: define the Double-byte Encoding GB18030
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_GB18030_H_
#define _WXM_ENCODING_GB18030_H_

#include "multibyte.h"
#include <boost/array.hpp>
#include <map>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
{

struct WXMEncodingGB18030: public WXMEncodingMultiByte, WXMEncodingDecoderISO646
{
	virtual void MultiByteInit();
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf);
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper);

	virtual bool IsSingleByteEncoding()
	{
		return false;
	}
	virtual bool IsDoubleByteEncoding()
	{
		return false;
	}
	virtual bool IsUnicodeEncoding()
	{
		return true;
	}

private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingGB18030(): m_icucnv("GB18030")
	{
	}
	~WXMEncodingGB18030()
	{
	}

	static const size_t BMP_QBYTE_CNT = 39420;
	static const size_t BMP_QBYTE_IDX_FFFD = 39417;
	boost::array<ucs4_t, BMP_QBYTE_CNT> m_qb2u;

	static const size_t BMP_DBYTE_CNT = (0xFE - 0x81 + 1)*(0xFE - 0x40 + 1);
	boost::array<ucs4_t, BMP_DBYTE_CNT> m_db2u;

	boost::array<wxDword, 0x10000> m_bmp2mb;
	//std::map<ucs4_t, wxWord> m_nonbmp2db;

	ICUConverter m_icucnv;

	ucs4_t QByte2NONBMP(const wxByte* buf);
	ucs4_t QByte2BMP(const wxByte* buf);
	ucs4_t DByte2BMP(const wxByte* buf);
	size_t NonBMP2QByte(wxByte* buf, ucs4_t ucs4);

	void CacheMBofUCS4(wxDword& mb, ucs4_t u);
};

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_GB18030_H_
