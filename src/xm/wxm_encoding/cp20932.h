///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/cp20932.h
// Description: define the Double-byte Encoding CP20932
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_CP20932_H_
#define _WXM_ENCODING_CP20932_H_

#include "doublebyte.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace wxm
{

struct CP20932Converter: public ICUConverter
{
	CP20932Converter(const std::string& encname)
		:ICUConverter(encname)
	{
	}

	virtual size_t MB2WC(UChar32& ch, const char* src, size_t src_len);
	virtual size_t WC2MB(char* dest, size_t dest_len, const UChar32& ch);
};

struct CP20932TableFixer: public DoubleByteEncodingTableFixer
{
	virtual void fix();
private:
	void icu42fix();
};

struct WXMEncodingCP20932: public WXMEncodingDoubleByteISO646Compatible
{
private:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	~WXMEncodingCP20932(){}

	virtual void InitMBConverter()
	{
		m_mbcnv = new CP20932Converter(m_innername);
	}

	virtual DoubleByteEncodingTableFixer* CreateDoubleByteEncodingTableFixer()
	{
		return new CP20932TableFixer();
	}
};

};// namespace wxm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _WXM_ENCODING_CP20932_H_
