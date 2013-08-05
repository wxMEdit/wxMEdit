///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.h
// Description: define the Double-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_DOUBLEBYTE_H_
#define _WXM_ENCODING_DOUBLEBYTE_H_

#include "wxm_encoding_multibyte.h"

#include <boost/array.hpp>

namespace wxm
{

struct WXMEncodingDoubleByte;
struct DoubleByteEncodingTableFixer
{
	DoubleByteEncodingTableFixer(WXMEncodingDoubleByte& dbenc)
		:m_dbenc(dbenc)
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
	WXMEncodingDoubleByte& m_dbenc;

	std::map<wxByte, int> m_leadbyte_map;
	std::map<wxWord, ucs4_t> m_mb2u_map;
	std::map<ucs4_t, wxWord> m_u2mb_map;
};

struct MS932TableFixer: public DoubleByteEncodingTableFixer
{
	MS932TableFixer(WXMEncodingDoubleByte& dbenc)
		:DoubleByteEncodingTableFixer(dbenc)
	{
	}
	virtual void fix();
};

struct WXMEncodingDoubleByte: public WXMEncodingMultiByte
{
	enum LeadByteType{ lbUnset=0, lbLeadByte, lbNotLeadByte=0xFF};
	enum SpecialValueType
	{
		svtInvaliad       = 0,
		svtNotCached      = 0xFF, 
		svtDByteNotCached = 0xFFFF,
		svtUCS4NotCached  = 0xFFFFFFFF,
	};

	virtual void MultiByteInit();
	virtual bool IsLeadByte(wxByte byte);
	virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);

protected:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	virtual void InitMBConverter()
	{
		m_mbcnv = new ICUConverter(m_innername);
	}
	WXMEncodingDoubleByte(): m_mbcnv(NULL), m_dbfix(NULL)
	{
	}
	~WXMEncodingDoubleByte()
	{
		delete m_mbcnv; m_mbcnv = NULL;
		delete m_dbfix; m_dbfix = NULL;
	}

	MBConverter* m_mbcnv;
	DoubleByteEncodingTableFixer* m_dbfix;

private:
	boost::array<wxByte, 256> m_leadbyte_tab;

	boost::array<ucs4_t, 256> m_b2u_tab;
	std::map<wxByte, boost::array<ucs4_t, 256> > m_db2u_tab;

	boost::array<wxWord, 0x10000> m_bmp2mb_tab;
	std::map<ucs4_t, wxWord> m_nonbmp2mb_map;

	DoubleByteEncodingTableFixer* CreateDoubleByteEncodingTableFixer();
	wxWord GetMBofUCS4(ucs4_t u);
	void SetMBofUCS4(ucs4_t u, wxWord mb);
};

struct WXMEncodingWXDoubleByte: public WXMEncodingDoubleByte
{
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	virtual void Create(ssize_t idx)
	{
		WXMEncodingDoubleByte::Create(idx);

		m_type = etDoubleByte;
	}
	~WXMEncodingWXDoubleByte(){}

	virtual void InitMBConverter()
	{
		m_mbcnv = new WXConverter(m_innername, m_enc);
	}
};

};// namespace wxm

#endif // _WXM_ENCODING_DOUBLEBYTE_H_
