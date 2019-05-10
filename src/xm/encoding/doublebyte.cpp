///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/doublebyte.cpp
// Description: Define the Double-byte Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "doublebyte.h"
#include "../cxx11.h"
#include "../utils.hpp"

#include <cstring>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

void DoubleByteEncodingTableFixer::RemoveLeadByte(ubyte b)
{
	m_leadbyte_map[b] = DoubleByteEncoding::lbNotLeadByte;
}
void DoubleByteEncodingTableFixer::AddLeadByte(ubyte b)
{
	m_leadbyte_map[b] = DoubleByteEncoding::lbLeadByte;
}

void DoubleByteEncodingTableFixer::RemoveMB2U(uint16_t db)
{
	m_mb2u_map[db] = Encoding::svtInvaliad;
}
void DoubleByteEncodingTableFixer::ChangeMB2U(uint16_t db, ucs4_t u)
{
	m_mb2u_map[db] = u;
}

void DoubleByteEncodingTableFixer::RemoveU2MB(ucs4_t u)
{
	m_u2mb_map[u] = Encoding::svtInvaliad;
}
void DoubleByteEncodingTableFixer::ChangeU2MB(ucs4_t u, uint16_t db)
{
	m_u2mb_map[u] = db;
}
int DoubleByteEncodingTableFixer::LeadByteInfo(ubyte b)
{
	return wrap_map(m_leadbyte_map).get(b, DoubleByteEncoding::lbUnset);
}

ucs4_t DoubleByteEncodingTableFixer::MB2UInfo(uint16_t db)
{
	return wrap_map(m_mb2u_map).get(db, Encoding::svtUCS4NotCached);
}

uint16_t DoubleByteEncodingTableFixer::U2MBInfo(ucs4_t u)
{
	return wrap_map(m_u2mb_map).get(u, Encoding::svtDByteNotCached);
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

DoubleByteEncodingTableFixer* DoubleByteEncoding::CreateDoubleByteEncodingTableFixer()
{
	if (m_innername == "MS932")
		return new MS932TableFixer();
	return new DoubleByteEncodingTableFixer();
}

uint16_t DoubleByteEncoding::GetCachedMBofUCS4(ucs4_t u)
{
	uint16_t mb = m_dbfix->U2MBInfo(u);
	if (mb != (uint16_t)svtDByteNotCached)
		return mb;

	if (u < 0x10000)
		return m_bmp2mb_tab[u];

	// use DoubleByteEncodingTableFixer to support non-BMP
	return svtInvaliad;
}

void DoubleByteEncoding::CacheMBofUCS4(uint16_t& mb, ucs4_t u)
{
	// use DoubleByteEncodingTableFixer to support non-BMP
	if (u >= 0x10000)
	{
		assert(0); // should not be here
		return;
	}

	ubyte mbs[3];
	size_t len = m_mbcnv->WC2MB((char*)mbs, 3, u);

	// wxASSERT( len<=2 );
	mbs[len] = '\0';
	mb = (((uint16_t)mbs[0]) << 8) | mbs[1];

	if(len == 0)
		mb = (uint16_t)svtInvaliad;

	m_bmp2mb_tab[u] = mb;
}

void DoubleByteEncoding::MultiByteInit()
{
	InitMBConverter();

	m_dbfix = CreateDoubleByteEncodingTableFixer();
	m_dbfix->fix();

	memset(m_b2u_tab.c_array(), svtInvaliad, sizeof(ucs4_t)*256);

	memset(m_bmp2mb_tab.c_array(), svtNotCached, sizeof(uint16_t)*0x10000);

	memset(m_leadbyte_tab.c_array(), lbUnset, 256);
	m_leadbyte_tab[0]=lbNotLeadByte;
}

ucs4_t DoubleByteEncoding::MultiBytetoUCS4(const ubyte* buf)
{
	uint16_t dbtmp = (buf[0] << 8) | buf[1];
	ucs4_t uinfo = m_dbfix->MB2UInfo(dbtmp);
	if (uinfo != (ucs4_t)svtUCS4NotCached)
		return uinfo;

	if( IsLeadByte(buf[0]))
		return m_db2u_tab[buf[0]][buf[1]];

	return (buf[1] == '\0')? m_b2u_tab[buf[0]]: (ucs4_t)svtInvaliad;
}

bool DoubleByteEncoding::IsLeadByte(ubyte byte)
{
	int lbinfo = m_dbfix->LeadByteInfo(byte);
	if (lbinfo != lbUnset)
		return lbinfo==lbLeadByte;

	if(m_leadbyte_tab[byte]==lbUnset)
	{
		ubyte dbs[3]={byte,0,0};
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

size_t DoubleByteEncoding::UCS4toMultiByte(ucs4_t ucs4, ubyte* buf)
{
	uint16_t mb = GetCachedMBofUCS4(ucs4);
	if (mb == (uint16_t)svtDByteNotCached)
		CacheMBofUCS4(mb, ucs4);

	if(mb == (uint16_t)svtInvaliad)
		return 0;

	buf[0] = mb >> 8;
	if ((mb & 0xFF) == 0)
		return 1;

	buf[1] = mb & 0xFF;
	return 2;
}

bool DoubleByteEncoding::NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	int64_t rest;
	ubyte* buf = mapper.BufferLoadBytes(rest, 2);
	if (buf == nullptr)
		return false;

	ucs4_t uc;
	if(rest==1 || buf[1] == 0 || (uc=MultiBytetoUCS4(buf)) == (ucs4_t)svtInvaliad)
	{
		ubyte db[2] = {buf[0], 0}; // re-check by first byte
		if((uc=MultiBytetoUCS4(db)) == (ucs4_t)svtInvaliad)
			uc = buf[0];

		mapper.MoveUChar32Bytes(ucqueue, uc, 1);
		return true;
	}

	mapper.MoveUChar32Bytes(ucqueue, uc, 2);
	return true;
}

};// namespace xm
