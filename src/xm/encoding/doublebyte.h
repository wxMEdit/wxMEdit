///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/doublebyte.h
// Description: Define the Double-byte Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_DOUBLEBYTE_H_
#define _XM_ENCODING_DOUBLEBYTE_H_

#include "../cxx11.h"
#include "multibyte.h"

#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace xm
{

struct DoubleByteEncodingTableFixer
{
	DoubleByteEncodingTableFixer()
	{
	}
	virtual ~DoubleByteEncodingTableFixer()
	{
	}
	virtual void fix()
	{
	}

	int LeadByteInfo(ubyte b);
	ucs4_t MB2UInfo(uint16_t db);
	uint16_t U2MBInfo(ucs4_t db);

protected:
	void RemoveLeadByte(ubyte b);
	void AddLeadByte(ubyte b);

	void RemoveMB2U(uint16_t db);
	void ChangeMB2U(uint16_t db, ucs4_t u);

	void RemoveU2MB(ucs4_t u);
	void ChangeU2MB(ucs4_t u, uint16_t db);

private:
	std::map<ubyte, int> m_leadbyte_map;
	std::map<uint16_t, ucs4_t> m_mb2u_map;
	std::map<ucs4_t, uint16_t> m_u2mb_map;
};

struct MS932TableFixer: public DoubleByteEncodingTableFixer
{
	virtual void fix() override;
};

struct DoubleByteEncoding: public MultiByteEncoding
{
	enum LeadByteType{ lbUnset=0, lbLeadByte, lbNotLeadByte=0xFF};

	virtual void MultiByteInit() override;
	virtual bool IsLeadByte(ubyte byte) override;
	virtual ucs4_t MultiBytetoUCS4(const ubyte* buf) override;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) override;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) override;

	virtual bool IsSingleByteEncoding() override
	{
		return false;
	}
	virtual bool IsDoubleByteEncoding() override
	{
		return true;
	}

protected:
	virtual DoubleByteEncodingTableFixer* CreateDoubleByteEncodingTableFixer();
	virtual void InitMBConverter()
	{
		m_mbcnv = new ICUConverter(m_innername);
	}
	DoubleByteEncoding(): m_mbcnv(nullptr), m_dbfix(nullptr)
	{
	}
	~DoubleByteEncoding()
	{
		delete m_mbcnv; m_mbcnv = nullptr;
		delete m_dbfix; m_dbfix = nullptr;
	}

	MBConverter* m_mbcnv;
	DoubleByteEncodingTableFixer* m_dbfix;

private:
	boost::array<ubyte, 256> m_leadbyte_tab;

	boost::array<ucs4_t, 256> m_b2u_tab;
	std::map<ubyte, boost::array<ucs4_t, 256> > m_db2u_tab;

	boost::array<uint16_t, 0x10000> m_bmp2mb_tab;

	uint16_t GetCachedMBofUCS4(ucs4_t u);
	void CacheMBofUCS4(uint16_t& mb, ucs4_t u);
};

struct DoubleByteEncodingISO646Compatible: public DoubleByteEncoding, public EncodingDecoderISO646
{
protected:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	DoubleByteEncodingISO646Compatible(): DoubleByteEncoding(), EncodingDecoderISO646() {}
	~DoubleByteEncodingISO646Compatible() {}
};

};// namespace xm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _XM_ENCODING_DOUBLEBYTE_H_
