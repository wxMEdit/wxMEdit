///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/doublebyte.h
// Description: Define the Double-byte Encodings Supported by wxMEdit
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_DOUBLEBYTE_H_
#define _WXM_ENCODING_DOUBLEBYTE_H_

#include "../../xm/cxx11.h"
#include "multibyte.h"

#include <boost/array.hpp>

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
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

	int LeadByteInfo(wxByte b);
	ucs4_t MB2UInfo(wxWord db);
	wxWord U2MBInfo(ucs4_t db);

protected:
	void RemoveLeadByte(wxByte b);
	void AddLeadByte(wxByte b);

	void RemoveMB2U(wxWord db);
	void ChangeMB2U(wxWord db, ucs4_t u);

	void RemoveU2MB(ucs4_t u);
	void ChangeU2MB(ucs4_t u, wxWord db);

private:
	std::map<wxByte, int> m_leadbyte_map;
	std::map<wxWord, ucs4_t> m_mb2u_map;
	std::map<ucs4_t, wxWord> m_u2mb_map;
};

struct MS932TableFixer: public DoubleByteEncodingTableFixer
{
	virtual void fix() override;
};

struct WXMEncodingDoubleByte: public WXMEncodingMultiByte
{
	enum LeadByteType{ lbUnset=0, lbLeadByte, lbNotLeadByte=0xFF};

	virtual void MultiByteInit() override;
	virtual bool IsLeadByte(wxByte byte) override;
	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf) override;
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) override;
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper) override;

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
	WXMEncodingDoubleByte(): m_mbcnv(nullptr), m_dbfix(nullptr)
	{
	}
	~WXMEncodingDoubleByte()
	{
		delete m_mbcnv; m_mbcnv = nullptr;
		delete m_dbfix; m_dbfix = nullptr;
	}

	MBConverter* m_mbcnv;
	DoubleByteEncodingTableFixer* m_dbfix;

private:
	boost::array<wxByte, 256> m_leadbyte_tab;

	boost::array<ucs4_t, 256> m_b2u_tab;
	std::map<wxByte, boost::array<ucs4_t, 256> > m_db2u_tab;

	boost::array<wxWord, 0x10000> m_bmp2mb_tab;

	wxWord GetCachedMBofUCS4(ucs4_t u);
	void CacheMBofUCS4(wxWord& mb, ucs4_t u);
};

struct WXMEncodingDoubleByteISO646Compatible: public WXMEncodingDoubleByte, public WXMEncodingDecoderISO646
{
protected:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	WXMEncodingDoubleByteISO646Compatible(): WXMEncodingDoubleByte(), WXMEncodingDecoderISO646() {}
	~WXMEncodingDoubleByteISO646Compatible() {}
};

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_DOUBLEBYTE_H_
