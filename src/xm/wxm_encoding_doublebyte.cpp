///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding_doublebyte.cpp
// Description: define the Double-byte Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding_doublebyte.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

void DoubleByteEncodingTableFixer::RemoveLeadByte(wxByte b)
{
	m_leadbyte_map[b] = WXMEncodingDoubleByte::lbNotLeadByte;
}
void DoubleByteEncodingTableFixer::AddLeadByte(wxByte b)
{
	m_leadbyte_map[b] = WXMEncodingDoubleByte::lbLeadByte;
}

void DoubleByteEncodingTableFixer::RemoveMB2U(wxWord db)
{
	m_mb2u_map[db] = WXMEncodingDoubleByte::svtInvaliad;
}
void DoubleByteEncodingTableFixer::ChangeMB2U(wxWord db, ucs4_t u)
{
	m_mb2u_map[db] = u;
}

void DoubleByteEncodingTableFixer::RemoveU2MB(ucs4_t u)
{
	m_u2mb_map[u] = WXMEncodingDoubleByte::svtInvaliad;
}
void DoubleByteEncodingTableFixer::ChangeU2MB(ucs4_t u, wxWord db)
{
	m_u2mb_map[u] = db;
}
int DoubleByteEncodingTableFixer::LeadByteInfo(wxByte b)
{
	std::map<wxByte, int>::const_iterator it = m_leadbyte_map.find(b);
	if (it == m_leadbyte_map.end())
		return WXMEncodingDoubleByte::lbUnset;

	return it->second;
}

ucs4_t DoubleByteEncodingTableFixer::MB2UInfo(wxWord db)
{
	std::map<wxWord, ucs4_t>::const_iterator it = m_mb2u_map.find(db);
	if (it == m_mb2u_map.end())
		return WXMEncodingDoubleByte::svtUCS4NotCached;

	return it->second;
}

wxWord DoubleByteEncodingTableFixer::U2MBInfo(ucs4_t u)
{
	std::map<ucs4_t, wxWord>::const_iterator it = m_u2mb_map.find(u);
	if (it == m_u2mb_map.end())
		return WXMEncodingDoubleByte::svtDByteNotCached;

	return it->second;
}

void MS932TableFixer::fix()
{
	ChangeMB2U(0x1A00, 0x00001A);
	ChangeMB2U(0x1C00, 0x00001C);
	ChangeMB2U(0x7F00, 0x00007F);
	ChangeMB2U(0x8000, 0x000080);

	RemoveMB2U(0x8500);
	RemoveMB2U(0x8600);
	RemoveMB2U(0xEB00);
	RemoveMB2U(0xEC00);
	RemoveMB2U(0xEF00);

	ChangeMB2U(0xA000, 0x00F8F0);
	ChangeMB2U(0xFD00, 0x00F8F1);
	ChangeMB2U(0xFE00, 0x00F8F2);
	ChangeMB2U(0xFF00, 0x00F8F3);


	ChangeU2MB(0x00001A, 0x1A00);
	ChangeU2MB(0x00001C, 0x1C00);
	ChangeU2MB(0x00007F, 0x7F00);
	ChangeU2MB(0x000080, 0x8000);

	ChangeU2MB(0x00F8F0, 0xA000);
	ChangeU2MB(0x00F8F1, 0xFD00);
	ChangeU2MB(0x00F8F2, 0xFE00);
	ChangeU2MB(0x00F8F3, 0xFF00);

	RemoveU2MB(0x00F86F);
}

DoubleByteEncodingTableFixer* WXMEncodingDoubleByte::CreateDoubleByteEncodingTableFixer()
{
	if (m_innername == "MS932")
		return new MS932TableFixer(*this);
	return new DoubleByteEncodingTableFixer(*this);
}

wxWord WXMEncodingDoubleByte::GetMBofUCS4(ucs4_t u)
{
	if (u < 0x10000)
		return m_bmp2mb_tab[u];

	std::map<ucs4_t, wxWord>::const_iterator it = m_nonbmp2mb_map.find(u);
	if (it == m_nonbmp2mb_map.end())
		return svtDByteNotCached;
	return it->second;
}

void WXMEncodingDoubleByte::SetMBofUCS4(ucs4_t u, wxWord mb)
{
	if (u < 0x10000)
	{
		m_bmp2mb_tab[u] = mb;
		return;
	}

	if (mb == (wxWord)svtInvaliad)
		return;

	m_nonbmp2mb_map[u] = mb;
}

void WXMEncodingDoubleByte::MultiByteInit()
{
	InitMBConverter();

	m_dbfix = CreateDoubleByteEncodingTableFixer();
	m_dbfix->fix();

	memset(m_b2u_tab.c_array(), svtInvaliad, sizeof(ucs4_t)*256);

	memset(m_bmp2mb_tab.c_array(), svtNotCached, sizeof(wxWord)*0x10000);

	memset(m_leadbyte_tab.c_array(), lbUnset, 256);
	m_leadbyte_tab[0]=lbNotLeadByte;
}

// return 0 if it is not a valid DB char
ucs4_t WXMEncodingDoubleByte::MultiBytetoUCS4(const wxByte* buf)
{
	wxWord dbtmp = (buf[0] << 8) | buf[1];
	ucs4_t uinfo = m_dbfix->MB2UInfo(dbtmp);
	if (uinfo != (ucs4_t)svtUCS4NotCached)
		return uinfo;

	if( IsLeadByte(buf[0]))
		return m_db2u_tab[buf[0]][buf[1]];

	return (buf[1] == '\0')? m_b2u_tab[buf[0]]: 0;
}

bool WXMEncodingDoubleByte::IsLeadByte(wxByte byte)
{
	int lbinfo = m_dbfix->LeadByteInfo(byte);
	if (lbinfo != lbUnset)
		return lbinfo==lbLeadByte;

	if(m_leadbyte_tab[byte]==lbUnset)
	{
		wxByte dbs[3]={byte,0,0};
		UChar32 ch;

		// check first byte
		if(m_mbcnv->MB2WC(ch, (char*)dbs, 1) == 1)
		{
			m_b2u_tab[byte] = ch;

			m_leadbyte_tab[byte]=lbNotLeadByte;
		}

		memset(m_db2u_tab[byte].c_array(), svtInvaliad, sizeof(ucs4_t)*256);
		for(int i=1; i<=0xFF; ++i)
		{
			dbs[1] = i;
			if(m_mbcnv->MB2WC(ch, (char*)dbs, 2) == 1)
			{
				m_db2u_tab[byte][i] = ch;

				m_leadbyte_tab[byte] = lbLeadByte;
			}
		}

		if (m_leadbyte_tab[byte] == lbLeadByte)
			m_b2u_tab[byte] = 0;
	}

	return m_leadbyte_tab[byte]==lbLeadByte;
}

size_t WXMEncodingDoubleByte::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	wxWord dbinfo = m_dbfix->U2MBInfo(ucs4);
	if (dbinfo != (ucs4_t)svtDByteNotCached)
	{
		if (dbinfo == svtInvaliad)
			return 0;

		buf[0] = dbinfo >> 8;
		wxByte b = dbinfo & 0xFF;

		if (b == svtInvaliad)
			return 1;

		buf[1] = b;
		return 2;
	}

	wxWord mb = GetMBofUCS4(ucs4);
	if(mb == 0) // invalid MB char
		return 0;

	if ((mb & 0xFF) == 0)
	{
		buf[0] = mb >> 8;
		return 1;
	}

	if(mb != (ucs4_t)svtDByteNotCached)
	{
		buf[0] = mb >> 8;
		buf[1] = mb & 0xFF;
		return 2;
	}

	// non-cached
	size_t len;
	wxByte mbs[3];
	len = m_mbcnv->WC2MB((char*)mbs, 3, ucs4);
	mb = (((wxWord)mbs[0])<<8) | mbs[1];
	if(len == 0 || mb == svtInvaliad)
	{
		SetMBofUCS4(ucs4, svtInvaliad);
		return 0;
	}

	wxASSERT( len<=2 );

	mbs[len] = '\0';
	SetMBofUCS4(ucs4, mb);

	buf[0] = mbs[0];
	if(len == 1)
		return 1;

	buf[1]=mbs[1];
	return 2;
}

bool WXMEncodingDoubleByte::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 2);
	if (buf == NULL)
		return false;

	ucs4_t uc;
	if(rest>1)
	{
		if(buf[1] == 0 || (uc=MultiBytetoUCS4(buf)) == (ucs4_t)svtInvaliad)
		{
			wxByte db[2] = {buf[0], 0}; // re-check by first byte
			if((uc=MultiBytetoUCS4(db)) == (ucs4_t)svtInvaliad)
				uc = buf[0];

			mapper.MoveUChar32Bytes(ucqueue, uc, 1);
			return true;
		}

		mapper.MoveUChar32Bytes(ucqueue, uc, 2);
		return true;
	}

	wxByte db[2] = {buf[0], 0}; // re-check by first byte
	if((uc=MultiBytetoUCS4(db)) == (ucs4_t)svtInvaliad)
		uc = buf[0];

	mapper.MoveUChar32Bytes(ucqueue, uc, 1);
	return true;
}

};// namespace wxm
