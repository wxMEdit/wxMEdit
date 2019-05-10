///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/encoding.cpp
// Description: Define the Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "../cxx11.h"
#include "../utils.hpp"
#include "unicode.h"
#include "singlebyte.h"
#include "doublebyte.h"
#include "cp20932.h"
#include "gb18030.h"
#include "external.h"

#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string/predicate.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#undef _
#define U_(txt) L ## txt
#define _(txt) LocalizeText(U_(txt))

namespace algo = boost::algorithm;

namespace xm
{
EncodingManager* EncodingManager::s_inst = nullptr;

std::string GetEncodingICUName(const char* innername)
{
	UErrorCode err=U_ZERO_ERROR;
	UConverter* ucnv = ucnv_open(innername, &err);
	if (ucnv == nullptr || U_FAILURE(err))
		return std::string();

	const char* icuname = ucnv_getName(ucnv, &err);
	if (icuname == nullptr || U_FAILURE(err))
		return std::string();

	return std::string(icuname);
}

std::string EncodingManager::s_sysenc_icuname;
void EncodingManager::PreInit()
{
	if (s_sysenc_icuname.empty())
		s_sysenc_icuname = GetEncodingICUName(nullptr);
}
void EncodingManager::AddEncoding(const std::string& encname, EncodingID encid,
                                     const std::wstring& desc, const std::wstring& aliases, EncodingType entype,
                                     const std::string& innername0, bool exact)
{
	m_enc_map[encname] = encid;
	m_enctype_map[encid] = entype;

	std::wstring wencname(encname.begin(), encname.end());

	m_enc_list.push_back(wencname);
	m_nameenc_map[wencname] = encid;
	m_encname_map[encid] = wencname;

	std::string innername = innername0.empty()? encname: innername0;
	m_encinnername_map[encid] = innername;

	std::wstring aliases_tmp = aliases.empty()? aliases: L"(" + aliases + L")";
	m_encdesc_map[encid] = desc + aliases_tmp;
	m_encaliases_map[encid] = L" " + aliases_tmp;

	std::string icuname = GetEncodingICUName(innername.c_str());
	if (exact && !icuname.empty())
		m_icunameenc_map[icuname] = encid;
}

void EncodingManager::DoInit()
{
	AddEncoding("ISO-8859-1", ENC_ISO_8859_1, _("Western European "), L"latin1");
	AddEncoding("ISO-8859-2", ENC_ISO_8859_2, _("Central European "), L"latin2");
	AddEncoding("ISO-8859-3", ENC_ISO_8859_3, _("South European "), L"latin3");
	AddEncoding("ISO-8859-4", ENC_ISO_8859_4, _("North European "), L"latin4");
	AddEncoding("ISO-8859-5", ENC_ISO_8859_5, _("Cyrillic "), L"cyrillic");
	AddEncoding("ISO-8859-6", ENC_ISO_8859_6, _("Arabic "), L"arabic");
	AddEncoding("ISO-8859-7", ENC_ISO_8859_7, _("Greek "), L"greek", etSingleByte, "IBM-9005");
	AddEncoding("ISO-8859-8", ENC_ISO_8859_8, _("Hebrew "), L"hebrew");
	AddEncoding("ISO-8859-9", ENC_ISO_8859_9, _("Turkish "), L"latin5");
	AddEncoding("ISO-8859-10", ENC_ISO_8859_10, _("Nordic "), L"latin6");
	AddEncoding("ISO-8859-11", ENC_ISO_8859_11, _("Thai "), L"TIS-620*");
	AddEncoding("ISO-8859-13", ENC_ISO_8859_13, _("Baltic "), L"latin7");
	AddEncoding("ISO-8859-14", ENC_ISO_8859_14, _("Celtic "), L"latin8");
	AddEncoding("ISO-8859-15", ENC_ISO_8859_15, _("Western European with Euro "), L"latin9");
	AddEncoding("ISO-8859-16", ENC_ISO_8859_16, _("South-Eastern European "), L"laint10", etSingleByte, "ISO-8859-15", false);
	AddEncoding("Windows-874", ENC_Windows_874, _("Windows Thai "), L"TIS-620*");
	AddEncoding("Windows-1250", ENC_Windows_1250, _("Windows Central European "), L"", etSingleByte, "CP1250");
	AddEncoding("Windows-1251", ENC_Windows_1251, _("Windows Cyrillic "), L"", etSingleByte, "CP1251");
	AddEncoding("Windows-1252", ENC_Windows_1252, _("Windows Western European "), L"", etSingleByte, "CP1252");
	AddEncoding("Windows-1253", ENC_Windows_1253, _("Windows Greek "), L"", etSingleByte, "CP1253");
	AddEncoding("Windows-1254", ENC_Windows_1254, _("Windows Turkish "), L"", etSingleByte, "CP1254");
	AddEncoding("Windows-1255", ENC_Windows_1255, _("Windows Hebrew "), L"", etSingleByte, "CP1255");
	AddEncoding("Windows-1256", ENC_Windows_1256, _("Windows Arabic "), L"", etSingleByte, "CP1256");
	AddEncoding("Windows-1257", ENC_Windows_1257, _("Windows Baltic "), L"", etSingleByte, "CP1257");
	AddEncoding("Windows-1258", ENC_Windows_1258, _("Windows Vietnamese "), L"", etSingleByte, "CP1258");
	AddEncoding("CP437-Art", ENC_CP437_ART, _("CP437 Variant for ASCII Art "), L"", etCP437ART, "CP437");
	AddEncoding("CP437", ENC_CP437, _("Windows/DOS OEM "));
	AddEncoding("CP850", ENC_CP850, _("Windows/DOS OEM - Latin 1 "));
	AddEncoding("CP852", ENC_CP852, _("Windows/DOS OEM - Latin 2 "));
	AddEncoding("CP855", ENC_CP855, _("Windows/DOS OEM - Cyrillic "));
	AddEncoding("CP866", ENC_CP866, _("Windows/DOS OEM - Cyrillic "));
	AddEncoding("KOI8-R", ENC_KOI8_R, _("Cyrillic "));
	AddEncoding("KOI8-U", ENC_KOI8_U, _("Cyrillic "));

	AddEncoding("Windows-31J", ENC_MS932, _("Windows Japanese "), L"Shift-JIS*", etDoubleByte, "MS932");
	AddEncoding("MS936"      , ENC_MS936, _("Windows Chinese Simplified "), L"GBK*",       etDoubleByte, "MS936");
	AddEncoding("UHC"        , ENC_MS949, _("Windows Korean "), L"EUC-KR*",    etDoubleByte, "MS949");
	AddEncoding("MS950"      , ENC_MS950, _("Windows Chinese Traditional "), L"Big5*",      etDoubleByte, "Windows-950-2000");
	AddEncoding("CP20932", ENC_CP20932, _("EUC-JP Double-Byte Edition of Windows "), L"", etCP20932, "EUC-JP", false);

	AddEncoding("GB18030", ENC_GB18030, _("Unicode/Chinese Simplified "), L"", etGB18030);

	AddEncoding("UTF-8",    ENC_UTF_8, _("Unicode 8 bit "), L"",    etUTF8);
	AddEncoding("UTF-16LE", ENC_UTF_16LE, _("Unicode 16 bit Little Endian "), L"", etUTF16LE);
	AddEncoding("UTF-16BE", ENC_UTF_16BE, _("Unicode 16 bit Big Endian "), L"", etUTF16BE);
	AddEncoding("UTF-32LE", ENC_UTF_32LE, _("Unicode 32 bit Little Endian "), L"", etUTF32LE);
	AddEncoding("UTF-32BE", ENC_UTF_32BE, _("Unicode 32 bit Big Endian "), L"", etUTF32BE);

	// only for encoding detection not for editing
	m_icunameenc_map[GetEncodingICUName("US-ASCII")] = ENC_ISO_646;

	m_wxencfont_map[ENC_CP437_ART]   = GetASCIIArtFontName();
	m_wxencfont_map[ENC_Windows_874] = GetMSCPFontName(L"874");
	m_wxencfont_map[ENC_MS932]       = GetMSCPFontName(L"932");
	m_wxencfont_map[ENC_MS936]       = GetMSCPFontName(L"936");
	m_wxencfont_map[ENC_MS949]       = GetMSCPFontName(L"949");
	m_wxencfont_map[ENC_MS950]       = GetMSCPFontName(L"950");
	m_wxencfont_map[ENC_CP20932]     = GetMSCPFontName(L"51932");
	m_wxencfont_map[ENC_GB18030]     = GetMSCPFontName(L"936");

	InitEncodingGroups();
	InitSystemEncoding();
}

EncodingID EncodingManager::GetSystemEncodingID()
{
	static EncodingID sysencid = ENC_DEFAULT;
	if (sysencid != ENC_DEFAULT)
		return sysencid;

	sysencid = wrap_map(m_icunameenc_map).get(s_sysenc_icuname, ENC_ISO_8859_1);

	if (sysencid == ENC_ISO_646)
		sysencid = ENC_ISO_8859_1;

	return sysencid;
}

void EncodingManager::InitSystemEncoding()
{
	EncodingID id = GetSystemEncodingID();

	std::wstring wxenc = EncodingToName(id);
	for(size_t i=0; i<m_enc_list.size(); ++i)
	{
		if (m_enc_list[i] == wxenc)
		{
			m_sysenc_idx = i;
			break;
		}
	}
}

template <typename L, typename C>
C cnv(const L& l, const C& c)
{
	return l.to_container(c);
}

void EncodingManager::InitEncodingGroups()
{
	boost::assign::insert(m_encgrpname_map)
		(ENCG_DEFAULT,       _("Other "))
		(ENCG_WESTERNEUROPE, _("Western European "))
		(ENCG_CENTRALEUROPE, _("Central European "))
		(ENCG_SOUTHEUROPE,   _("South European "))
		(ENCG_NORTHEUROPE,   _("North European "))
		(ENCG_CYRILLIC,      _("Cyrillic "))
		(ENCG_ARABIC,        _("Arabic "))
		(ENCG_GREEK,         _("Greek "))
		(ENCG_HEBREW,        _("Hebrew "))
		(ENCG_TURKISH,       _("Turkish "))
		(ENCG_BALTIC,        _("Baltic "))
		(ENCG_EASTASIA,      _("East Asian "))
		(ENCG_SOUTHEASTASIA, _("Southeast Asian "))
		(ENCG_UNICODE,       _("Unicode "))
		(ENCG_ISO8859,       _("ISO-8859 "))
		(ENCG_WINDOWS,       _("Windows "))
		(ENCG_OEM,           _("OEM "))
	;

	std::set<EncodingGroupID> t;
	using boost::assign::list_of;
	boost::assign::insert(m_encgrps_map)
		// ISO-8859
		( ENC_ISO_8859_1,  cnv(list_of(ENCG_ISO8859)(ENCG_WESTERNEUROPE), t) )
		( ENC_ISO_8859_2,  cnv(list_of(ENCG_ISO8859)(ENCG_CENTRALEUROPE), t) )
		( ENC_ISO_8859_3,  cnv(list_of(ENCG_ISO8859)(ENCG_SOUTHEUROPE), t) )
		( ENC_ISO_8859_4,  cnv(list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE), t) )
		( ENC_ISO_8859_5,  cnv(list_of(ENCG_ISO8859)(ENCG_CYRILLIC), t) )
		( ENC_ISO_8859_6,  cnv(list_of(ENCG_ISO8859)(ENCG_ARABIC), t) )
		( ENC_ISO_8859_7,  cnv(list_of(ENCG_ISO8859)(ENCG_GREEK), t) )
		( ENC_ISO_8859_8,  cnv(list_of(ENCG_ISO8859)(ENCG_HEBREW), t) )
		( ENC_ISO_8859_9,  cnv(list_of(ENCG_ISO8859)(ENCG_TURKISH), t) )
		( ENC_ISO_8859_10, cnv(list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE), t) )
		( ENC_ISO_8859_11, cnv(list_of(ENCG_ISO8859)(ENCG_SOUTHEASTASIA), t) )
		( ENC_ISO_8859_13, cnv(list_of(ENCG_ISO8859)(ENCG_BALTIC), t) )
		( ENC_ISO_8859_14, cnv(list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE), t) )
		( ENC_ISO_8859_15, cnv(list_of(ENCG_ISO8859)(ENCG_WESTERNEUROPE), t) )
		( ENC_ISO_8859_16, cnv(list_of(ENCG_ISO8859)(ENCG_SOUTHEUROPE), t) )
		// Windows
		( ENC_Windows_874,  cnv(list_of(ENCG_WINDOWS)(ENCG_SOUTHEASTASIA), t) )
		( ENC_Windows_1250, cnv(list_of(ENCG_WINDOWS)(ENCG_CENTRALEUROPE), t) )
		( ENC_Windows_1251, cnv(list_of(ENCG_WINDOWS)(ENCG_CYRILLIC), t) )
		( ENC_Windows_1252, cnv(list_of(ENCG_WINDOWS)(ENCG_WESTERNEUROPE), t) )
		( ENC_Windows_1253, cnv(list_of(ENCG_WINDOWS)(ENCG_GREEK), t) )
		( ENC_Windows_1254, cnv(list_of(ENCG_WINDOWS)(ENCG_TURKISH), t) )
		( ENC_Windows_1255, cnv(list_of(ENCG_WINDOWS)(ENCG_HEBREW), t) )
		( ENC_Windows_1256, cnv(list_of(ENCG_WINDOWS)(ENCG_ARABIC), t) )
		( ENC_Windows_1257, cnv(list_of(ENCG_WINDOWS)(ENCG_BALTIC), t) )
		( ENC_Windows_1258, cnv(list_of(ENCG_WINDOWS)(ENCG_SOUTHEASTASIA), t) )
		// OEM
		( ENC_CP437, cnv(list_of(ENCG_OEM), t) )
		( ENC_CP850, cnv(list_of(ENCG_OEM), t) )
		( ENC_CP852, cnv(list_of(ENCG_OEM), t) )
		( ENC_CP855, cnv(list_of(ENCG_OEM)(ENCG_CYRILLIC), t) )
		( ENC_CP866, cnv(list_of(ENCG_OEM)(ENCG_CYRILLIC), t) )
		// Cyrillic
		( ENC_KOI8_R, cnv(list_of(ENCG_CYRILLIC), t) )
		( ENC_KOI8_U, cnv(list_of(ENCG_CYRILLIC), t) )
		// Windows & East Asia
		( ENC_MS932,   cnv(list_of(ENCG_WINDOWS)(ENCG_EASTASIA), t) )
		( ENC_MS936,   cnv(list_of(ENCG_WINDOWS)(ENCG_EASTASIA), t) )
		( ENC_MS949,   cnv(list_of(ENCG_WINDOWS)(ENCG_EASTASIA), t) )
		( ENC_MS950,   cnv(list_of(ENCG_WINDOWS)(ENCG_EASTASIA), t) )
		( ENC_CP20932, cnv(list_of(ENCG_WINDOWS)(ENCG_EASTASIA), t) )
		// Unicode
		( ENC_GB18030,  cnv(list_of(ENCG_UNICODE)(ENCG_EASTASIA), t) )
		( ENC_UTF_8,    cnv(list_of(ENCG_UNICODE), t) )
		( ENC_UTF_16LE, cnv(list_of(ENCG_UNICODE), t) )
		( ENC_UTF_16BE, cnv(list_of(ENCG_UNICODE), t) )
		( ENC_UTF_32LE, cnv(list_of(ENCG_UNICODE), t) )
		( ENC_UTF_32BE, cnv(list_of(ENCG_UNICODE), t) )
	;
}

std::vector<EncodingGroupID> EncodingManager::GetEncodingGroups(ssize_t idx)
{
	EncGrpsMap::const_iterator it = m_encgrps_map.find(IdxToEncoding(idx));
	if (it == m_encgrps_map.end())
	{
		std::vector<EncodingGroupID> v;
		v.push_back(ENCG_DEFAULT);
		return v;
	}

	std::vector<EncodingGroupID> v(it->second.begin(), it->second.end());
	return v;
}

std::wstring EncodingManager::EncodingGroupToName(EncodingGroupID gid)
{
	return wrap_map(m_encgrpname_map).get(gid, std::wstring());
}

void EncodingManager::FreeEncodings()
{
	BOOST_FOREACH(EncInstMap::value_type val, m_inst_map)
	{
		if (m_sysenc == val.second)
			m_sysenc = nullptr;

		delete val.second;
	}
	m_inst_map.clear();

	if (m_sysenc != nullptr)
		delete m_sysenc;
	m_sysenc = nullptr;

	m_initialized = false;
	DestroyInstance();
}

size_t EncodingManager::GetEncodingsCount()
{
	return m_enc_map.size();
}

std::wstring EncodingManager::GetEncodingName(ssize_t idx)
{
	if (idx<0 || (size_t)idx>=m_enc_list.size())
		return EncodingToName(GetSystemEncodingID());

	return m_enc_list[idx];
}

std::string EncodingManager::GetEncodingInnerName(ssize_t idx)
{
	return wrap_map(m_encinnername_map).get(IdxToEncoding(idx), "unknown");
}

std::wstring EncodingManager::GetEncodingDescription(ssize_t idx)
{
	return wrap_map(m_encdesc_map).get(IdxToEncoding(idx), GetEncodingName(idx));
}

std::wstring EncodingManager::GetEncodingAliases(ssize_t idx)
{
	return wrap_map(m_encaliases_map).get(IdxToEncoding(idx), std::wstring());
}

std::wstring EncodingManager::GetEncodingFontName(ssize_t idx)
{
	return wrap_map(m_wxencfont_map).get(IdxToEncoding(idx), GetMSCPFontName(std::wstring()));
}

std::wstring EncodingManager::EncodingToName(EncodingID enc)
{
	return wrap_map(m_encname_map).get(enc, L"Unknown Encoding");
}

EncodingID EncodingManager::NameToEncoding(const std::wstring& name)
{
	return wrap_map(m_nameenc_map).get(name, GetSystemEncodingID());
}

EncodingID EncodingManager::ExtNameToEncoding(const std::string& name)
{
	NameEncMap::const_iterator itwx = m_nameenc_map.find(std::wstring(name.begin(), name.end()));
	if (itwx != m_nameenc_map.end())
		return itwx->second;

	return wrap_map(m_icunameenc_map).get(GetEncodingICUName(name.c_str()), ENC_DEFAULT);
}

std::wstring EncodingManager::ExpandEncodingAliases(const std::wstring& name)
{
	std::wstring aliases = wrap_map(m_encaliases_map).get(NameToEncoding(name), std::wstring());
	return name + aliases;
}

Encoding* EncodingManager::GetSystemEncoding()
{
	if (m_sysenc == nullptr)
		m_sysenc = GetEncoding(m_sysenc_idx);

	return m_sysenc;
}

EncodingManager::EncodingType EncodingManager::GetIdxEncType(ssize_t idx)
{
	return wrap_map(m_enctype_map).get(IdxToEncoding(idx), etSingleByte/*FIXME later*/);
}

Encoding* EncodingManager::GetEncoding(ssize_t idx)
{
	// wxASSERT(idx<(ssize_t)m_wxenc_list.size() && idx>=0);

	EncInstMap::iterator it = m_inst_map.find(idx);
	if (it!=m_inst_map.end() && it->second!=nullptr)
		return it->second;

	Encoding* enc = nullptr;

	EncodingType t = GetIdxEncType(idx);

	switch(t)
	{
		case etUTF8:
			enc = new UTF8Encoding();
			break;
		case etUTF16LE:
			enc = new UTF16LE_Encoding();
			break;
		case etUTF16BE:
			enc = new UTF16BE_Encoding();
			break;
		case etUTF32LE:
			enc = new UTF32LE_Encoding();
			break;
		case etUTF32BE:
			enc = new UTF32BE_Encoding();
			break;
		case etSingleByte:
			enc = new SingleByteEncodingISO646Compatible();
			break;
		case etDoubleByte:
			enc = new DoubleByteEncodingISO646Compatible();
			break;
		case etCP20932:
			enc = new CP20932Encoding();
			break;
		case etGB18030:
			enc = new EncodingGB18030();
			break;
		case etCP437ART:
			enc = new CP437ArtEncoding();
			break;
	}

	enc->Create(idx);
	m_inst_map[idx] = enc;

	return enc;
}

Encoding* EncodingManager::GetEncoding(EncodingID enc)
{
	size_t idx;
	for (idx=0; idx<m_enc_list.size(); idx++)
	{
		if (m_nameenc_map[m_enc_list[idx]]==enc)
			return GetEncoding(idx);
	}

	return GetEncoding(m_sysenc_idx);
}

Encoding* EncodingManager::GetEncoding(const std::wstring& name)
{
	size_t idx;
	for (idx=0; idx<m_enc_list.size(); idx++)
	{
		if (algo::iequals(m_enc_list[idx], name))
			return GetEncoding(idx);
	}

	return GetEncoding(m_sysenc_idx);
}

void Encoding::Create(ssize_t idx)
{
	m_idx = idx;
	m_name = EncodingManager::Instance().GetEncodingName(idx);
	m_enc = EncodingManager::Instance().NameToEncoding(m_name);
	m_innername = EncodingManager::Instance().GetEncodingInnerName(idx);
	m_desc = EncodingManager::Instance().GetEncodingDescription(idx);
	m_name_with_aliases = EncodingManager::Instance().GetEncodingNameWithAliases(idx);
	m_fontname = EncodingManager::Instance().GetEncodingFontName(m_idx);
	m_simp_unicode = EncodingManager::IsSimpleUnicodeEncoding(m_enc);
}


bool EncodingDecoderISO646::IsUChar32_LineFeed(const ubyte* buf, size_t len)
{
	return *buf == 0x0A;
}
bool EncodingDecoderISO646::IsUChar32_LineFeed(BlockDumper& dumper, size_t len)
{
	ubyte b;
	dumper.Dump(&b, 1);
	return b == 0x0A;
}
ucs4_t EncodingDecoderISO646::PeekUChar32_Newline(BlockDumper& dumper, size_t len)
{
	ubyte b;
	dumper.Dump(&b, 1);
	if (b == 0x0A || b == 0x0D)
		return (ucs4_t)b;
	return 0;
}

};// namespace xm
