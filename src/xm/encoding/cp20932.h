///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/cp20932.h
// Description: Define the Double-byte Encoding CP20932
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_CP20932_H_
#define _XM_ENCODING_CP20932_H_

#include "../cxx11.h"
#include "doublebyte.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif

namespace xm
{

struct CP20932Converter: public ICUConverter
{
	CP20932Converter(const std::string& encname)
		:ICUConverter(encname)
	{
	}

	virtual size_t MB2WC(UChar32& ch, const char* src, size_t src_len) override;
	virtual size_t WC2MB(char* dest, size_t dest_len, const UChar32& ch) override;
};

struct CP20932TableFixer: public DoubleByteEncodingTableFixer
{
	virtual void fix() override;
private:
	void icu42fix();
};

struct CP20932Encoding: public DoubleByteEncodingISO646Compatible
{
private:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	~CP20932Encoding(){}

	virtual void InitMBConverter() override
	{
		m_mbcnv = new CP20932Converter(m_innername);
	}

	virtual DoubleByteEncodingTableFixer* CreateDoubleByteEncodingTableFixer() override
	{
		return new CP20932TableFixer();
	}
};

};// namespace xm

#ifdef _MSC_VER
# pragma warning( pop )
#endif

#endif // _XM_ENCODING_CP20932_H_
