///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/encoding/gb18030.cpp
// Description: Define the Multi-byte Encoding GB18030
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "gb18030.h"
#include <unicode/uversion.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace wxm
{

void WXMEncodingGB18030::MultiByteInit()
{
	memset(m_db2u.c_array(), svtNotCached, sizeof(ucs4_t)*BMP_DBYTE_CNT);
	memset(m_qb2u.c_array(), svtNotCached, sizeof(ucs4_t)*BMP_QBYTE_CNT);

	memset(m_bmp2mb.c_array(), svtNotCached, sizeof(wxDword)*0xD800);
	memset(m_bmp2mb.c_array()+0xD800, svtInvaliad, sizeof(wxDword)*0x800);
	memset(m_bmp2mb.c_array()+0xE000, svtNotCached, sizeof(wxDword)*0x2000);

	m_qb2u[BMP_QBYTE_IDX_FFFD] = 0x00FFFD;
}

inline bool IsQByte2NONBMP(const wxByte* buf)
{
	return buf[0]>=0x90 && buf[0]<=0xFE && buf[1]>=0x30 && buf[1]<=0x39 && 
			buf[2]>=0x81 && buf[2]<=0xFE && buf[3]>=0x30 && buf[3]<=0x39;
}
inline bool IsQByte2BMP(const wxByte* buf)
{
	return buf[0]>=0x81 && buf[0]<0x90 && buf[1]>=0x30 && buf[1]<=0x39 && 
			buf[2]>=0x81 && buf[2]<=0xFE && buf[3]>=0x30 && buf[3]<=0x39;
}
inline bool IsDByte(const wxByte* buf)
{
	return buf[2]=='\0' && buf[0]>=0x81 && buf[0]<=0xFE && buf[1]>=0x40 && buf[1]<=0xFE;
}
inline bool IsSByte(const wxByte* buf)
{
	return buf[1]=='\0' && (buf[0] & 0x80)==0;
}

ucs4_t WXMEncodingGB18030::MultiBytetoUCS4(const wxByte* buf)
{
	if (IsQByte2NONBMP(buf))
		return QByte2NONBMP(buf);

	if (IsQByte2BMP(buf))
		return QByte2BMP(buf);

	if (IsDByte(buf))
		return DByte2BMP(buf);

	if (IsSByte(buf))
		return (ucs4_t)buf[0];

	return (ucs4_t)svtInvaliad;
}

ucs4_t WXMEncodingGB18030::QByte2NONBMP(const wxByte* buf)
{
	ucs4_t u = (ucs4_t)((buf[0]-0x90)*12600 + (buf[1]-0x30)*1260 + (buf[2]-0x81)*10 + buf[3]-0x30 + 0x10000);
	if (u > 0x10FFFF)
		return (ucs4_t)svtInvaliad;

	return u;
}

ucs4_t WXMEncodingGB18030::QByte2BMP(const wxByte* buf)
{
	size_t idx = (buf[0]-0x81)*12600 + (buf[1]-0x30)*1260 + (buf[2]-0x81)*10 + buf[3]-0x30;
	if (idx >= BMP_QBYTE_CNT)
		return (ucs4_t)svtInvaliad;

	if (m_qb2u[idx] != (ucs4_t)svtUCS4NotCached)
		return m_qb2u[idx];

#if (U_ICU_VERSION_MAJOR_NUM*10+U_ICU_VERSION_MINOR_NUM < 48)
	if (idx == (0x81-0x81)*12600 + (0x35-0x30)*1260 + (0xF4-0x81)*10 + 0x37-0x30)
	{
		m_db2u[idx] = (ucs4_t)0x00E7C7;
		return (ucs4_t)0x00E7C7;
	}
#endif

	UChar32 ch = 0;
	size_t len = m_icucnv.MB2WC(ch, (const char*)buf, 4);
	if (len == 0)
		return (ucs4_t)svtInvaliad;

	m_qb2u[idx] = (ucs4_t)ch;
	return (ucs4_t)ch;
}

ucs4_t WXMEncodingGB18030::DByte2BMP(const wxByte* buf)
{
	size_t idx = (buf[0] - 0x81)*(0xFE - 0x40 + 1) + (buf[1] - 0x40);
	if (m_db2u[idx] != (ucs4_t)svtUCS4NotCached)
		return m_db2u[idx];

#if (U_ICU_VERSION_MAJOR_NUM*10+U_ICU_VERSION_MINOR_NUM < 48)
	if (idx == (0xA8 - 0x81)*(0xFE - 0x40 + 1) + (0xBC - 0x40))
	{
		m_db2u[idx] = (ucs4_t)0x001E3F;
		return (ucs4_t)0x001E3F;
	}
#endif

	UChar32 ch = 0;
	size_t len = m_icucnv.MB2WC(ch, (const char*)buf, 2);
	if (len==0)
		return (ucs4_t)svtInvaliad;

	m_db2u[idx] = (ucs4_t)ch;
	return (ucs4_t)ch;
}

size_t WXMEncodingGB18030::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if (ucs4 == 0x000000)
		return 0;

	if (ucs4 < 0x0080)
	{
		buf[0] = (wxByte)ucs4;
		return 1;
	}

	if (ucs4 >= 0x10000)
	{
		return NonBMP2QByte(buf, ucs4);
	}

	wxDword mb = m_bmp2mb[ucs4];
	if (mb == (wxDword)svtQByteNotCached)
		CacheMBofUCS4(mb, ucs4);

	if (mb == (wxDword)svtInvaliad)
		return 0;

	buf[0] = mb >> 24;
	buf[1] = (mb >> 16) & 0xFF;
	if ((mb & 0xFFFF)==0)
		return 2;

	buf[2] = (mb >> 8) & 0xFF;
	buf[3] = mb & 0xFF;
	return 4;
}

size_t WXMEncodingGB18030::NonBMP2QByte(wxByte* buf, ucs4_t ucs4)
{
	wxDword t = ucs4 - 0x10000;
	buf[0] = 0x90 + t / 12600;

	t = t % 12600;
	buf[1] = 0x30 + t / 1260;

	t = t % 1260;
	buf[2] = 0x81 + t / 10;
	buf[3] = 0x30 + t % 10;

	return 4;
}

void WXMEncodingGB18030::CacheMBofUCS4(wxDword& mb, ucs4_t u)
{
#if (U_ICU_VERSION_MAJOR_NUM*10+U_ICU_VERSION_MINOR_NUM < 48)
	if (u == 0x00E7C7)
	{
		mb = 0x8135F437;
		m_bmp2mb[u] = mb;
		return;
	}
	if(u == 0x001E3F)
	{
		mb = 0xA8BC0000;
		m_bmp2mb[u] = mb;
		return;
	}
#endif

	wxByte mbs[4] = {'\0', '\0', '\0', '\0'};
	size_t len = m_icucnv.WC2MB((char*)mbs, 4, u);
	if (len!=4 && len!=2)
	{
		m_bmp2mb[u] = (wxDword)svtInvaliad;
		assert(0);
		return;
	}

	mb = (((wxDword)mbs[0]) << 24) | (((wxDword)mbs[1]) << 16) | (((wxDword)mbs[2]) << 8) | mbs[3];
	m_bmp2mb[u] = mb;
}


bool WXMEncodingGB18030::NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper)
{
	wxFileOffset rest;
	wxByte *buf=mapper.BufferLoadBytes(rest, 4);
	if (buf == NULL)
		return false;

	ucs4_t uc = (ucs4_t)svtInvaliad;
	if (rest >= 4)
	{
		if (IsQByte2NONBMP(buf))
			uc = QByte2NONBMP(buf);
		else if (IsQByte2BMP(buf))
			uc = QByte2BMP(buf);

		if (uc != (ucs4_t)svtInvaliad)
		{
			mapper.MoveUChar32Bytes(ucqueue, uc, 4);
			return true;
		}
	}

	if (rest >= 2)
	{
		wxByte buf2[3] = {buf[0], buf[1], '\0'};
		if (IsDByte(buf2))
			uc = DByte2BMP(buf2);

		if (uc != (ucs4_t)svtInvaliad)
		{
			mapper.MoveUChar32Bytes(ucqueue, uc, 2);
			return true;
		}
	}

	uc = buf[0];
	mapper.MoveUChar32Bytes(ucqueue, uc, 1);
	return true;
}

};// namespace wxm
