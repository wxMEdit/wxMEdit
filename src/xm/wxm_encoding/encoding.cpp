///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "../wxm_def.h"
#include "unicode.h"
#include "singlebyte.h"
#include "doublebyte.h"
#include "cp20932.h"
#include "gb18030.h"
#include "../wxm_utils.h"

#include <wx/config.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/scoped_ptr.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#ifdef __WXMSW__
#define MSW_GET_FONT_NAME(cp,fn) MSW_GetFontName(cp,fn)

// get fontname from registry mime database
static void MSW_GetFontName(const wxString codepage, wxString &fontname)
{
	const wxString MIMEDB(wxm::s_wxsRegkeyClasses + wxT("MIME\\Database\\Codepage\\"));
	boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(MIMEDB + codepage) );

	if(!pRegKey->Exists())
		return;

	long cp;
	if(pRegKey->QueryValue(wxT("Family"), &cp))
		pRegKey.reset( new wxRegKey(MIMEDB + wxString::Format(wxT("%d"), cp)) );

	pRegKey->QueryValue(wxT("FixedWidthFont"), fontname);
}

#else
#define MSW_GET_FONT_NAME(cp,fn)
#endif

static wxString GetMSCPFontName(const wxString mscp)
{
	wxString fontname = wxm::MonoFontName;

	if (mscp.IsEmpty())
		return fontname;

	MSW_GET_FONT_NAME(mscp, fontname);
	return fontname;
}

namespace wxm
{
WXMEncodingManager* WXMEncodingManager::s_inst = NULL;

std::string GetEncodingICUName(const char* innername)
{
	UErrorCode err=U_ZERO_ERROR;
	UConverter* ucnv = ucnv_open(innername, &err);
	if (ucnv == NULL || U_FAILURE(err))
		return std::string();

	const char* icuname = ucnv_getName(ucnv, &err);
	if (icuname == NULL || U_FAILURE(err))
		return std::string();

	return std::string(icuname);
}

void WXMEncodingManager::AddEncoding(const std::string& encname, WXMEncodingID encid, 
		const wxString& desc, const wxString& aliases, WXMEncodingType entype, 
		const std::string& innername0, bool exact)
{
	m_wxenc_map[encname] = encid;
	m_wxenctype_map[encid] = entype;

	wxString wxencname(encname.c_str(), wxConvUTF8);

	m_wxenc_list.push_back(wxencname);
	m_wxnameenc_map[wxencname] = encid;
	m_wxencname_map[encid] = wxencname;

	std::string innername = innername0.empty()? encname: innername0;
	m_encinnername_map[encid] = innername;

	wxString aliases_tmp = aliases.empty()? aliases: wxT("(") + aliases + wxT(")");
	m_wxencdesc_map[encid] = desc + aliases_tmp;
	m_wxencalias_map[encid] = wxT(" ") + aliases_tmp;

	std::string icuname = GetEncodingICUName(innername.c_str());
	if (exact && !icuname.empty())
		m_icunameenc_map[icuname] = encid;
}

void WXMEncodingManager::DoInit()
{
	wxLogNull nolog;

	AddEncoding("ISO-8859-1", ENC_ISO_8859_1, _("Western European "),             wxT("latin1"));
	AddEncoding("ISO-8859-2", ENC_ISO_8859_2, _("Central European "),             wxT("latin2"));
	AddEncoding("ISO-8859-3", ENC_ISO_8859_3, _("South European "),               wxT("latin3"));
	AddEncoding("ISO-8859-4", ENC_ISO_8859_4, _("North European "),               wxT("latin4"));
	AddEncoding("ISO-8859-5", ENC_ISO_8859_5, _("Cyrillic "),                     wxT("cyrillic"));
	AddEncoding("ISO-8859-6", ENC_ISO_8859_6, _("Arabic "),                       wxT("arabic"));
	AddEncoding("ISO-8859-7", ENC_ISO_8859_7, _("Greek "),                        wxT("greek"), etSingleByte, "IBM-9005");
	AddEncoding("ISO-8859-8", ENC_ISO_8859_8, _("Hebrew "),                       wxT("hebrew"));
	AddEncoding("ISO-8859-9", ENC_ISO_8859_9, _("Turkish "),                      wxT("latin5"));
	AddEncoding("ISO-8859-10", ENC_ISO_8859_10, _("Nordic "),                     wxT("latin6"));
	AddEncoding("ISO-8859-11", ENC_ISO_8859_11, _("Thai "));
	AddEncoding("ISO-8859-13", ENC_ISO_8859_13, _("Baltic "),                     wxT("latin7"));
	AddEncoding("ISO-8859-14", ENC_ISO_8859_14, _("Celtic "),                     wxT("latin8"));
	AddEncoding("ISO-8859-15", ENC_ISO_8859_15, _("Western European with Euro "), wxT("latin9"));
	AddEncoding("ISO-8859-16", ENC_ISO_8859_16, _("South-Eastern European "),     wxT("laint10"), etSingleByte, "ISO-8859-15", false);
	AddEncoding("Windows-874", ENC_Windows_874, _("Windows Thai "),               wxT("TIS-620*"));
	AddEncoding("Windows-1250", ENC_Windows_1250, _("Windows Central European "), wxT(""), etSingleByte, "CP1250");
	AddEncoding("Windows-1251", ENC_Windows_1251, _("Windows Cyrillic "),         wxT(""), etSingleByte, "CP1251");
	AddEncoding("Windows-1252", ENC_Windows_1252, _("Windows Western European "), wxT(""), etSingleByte, "CP1252");
	AddEncoding("Windows-1253", ENC_Windows_1253, _("Windows Greek "),            wxT(""), etSingleByte, "CP1253");
	AddEncoding("Windows-1254", ENC_Windows_1254, _("Windows Turkish "),          wxT(""), etSingleByte, "CP1254");
	AddEncoding("Windows-1255", ENC_Windows_1255, _("Windows Hebrew "),           wxT(""), etSingleByte, "CP1255");
	AddEncoding("Windows-1256", ENC_Windows_1256, _("Windows Arabic "),           wxT(""), etSingleByte, "CP1256");
	AddEncoding("Windows-1257", ENC_Windows_1257, _("Windows Baltic "),           wxT(""), etSingleByte, "CP1257");
	AddEncoding("Windows-1258", ENC_Windows_1258, _("Windows Vietnamese "),       wxT(""), etSingleByte, "CP1258");
	AddEncoding("CP437-Art", ENC_CP437_ART, _("CP437 Variant for ASCII Art "), wxT(""), etCP437ART, "CP437");
	AddEncoding("CP437", ENC_CP437, _("Windows/DOS OEM "));
	AddEncoding("CP850", ENC_CP850, _("Windows/DOS OEM - Latin 1 "));
	AddEncoding("CP852", ENC_CP852, _("Windows/DOS OEM - Latin 2 "));
	AddEncoding("CP855", ENC_CP855, _("Windows/DOS OEM - Cyrillic "));
	AddEncoding("CP866", ENC_CP866, _("Windows/DOS OEM - Cyrillic "));
	AddEncoding("KOI8-R", ENC_KOI8_R, _("Cyrillic "));
	AddEncoding("KOI8-U", ENC_KOI8_U, _("Cyrillic "));

	AddEncoding("Windows-31J", ENC_MS932, _("Windows Japanese "),            wxT("Shift-JIS*"), etDoubleByte, "MS932");
	AddEncoding("MS936"      , ENC_MS936, _("Windows Chinese Simplified "),  wxT("GBK*"),       etDoubleByte, "MS936");
	AddEncoding("UHC"        , ENC_MS949, _("Windows Korean "),              wxT("EUC-KR*"),    etDoubleByte, "MS949");
	AddEncoding("MS950"      , ENC_MS950, _("Windows Chinese Traditional "), wxT("Big5*"),      etDoubleByte, "Windows-950-2000");
	AddEncoding("CP20932", ENC_CP20932, _("EUC-JP Double-Byte Edition of Windows "), wxT(""), etCP20932, "EUC-JP", false);

	AddEncoding("GB18030", ENC_GB18030, _("Unicode/Chinese Simplified "), wxT(""), etGB18030);

	AddEncoding("UTF-8",    ENC_UTF_8,    _("Unicode 8 bit "),                wxT(""),    etUTF8);
	AddEncoding("UTF-16LE", ENC_UTF_16LE, _("Unicode 16 bit Little Endian "), wxT(""), etUTF16LE);
	AddEncoding("UTF-16BE", ENC_UTF_16BE, _("Unicode 16 bit Big Endian "),    wxT(""), etUTF16BE);
	AddEncoding("UTF-32LE", ENC_UTF_32LE, _("Unicode 32 bit Little Endian "), wxT(""), etUTF32LE);
	AddEncoding("UTF-32BE", ENC_UTF_32BE, _("Unicode 32 bit Big Endian "),    wxT(""), etUTF32BE);

	// only for encoding detection not for editing
	m_icunameenc_map[GetEncodingICUName("US-ASCII")] = ENC_ISO_646;

	m_wxencfont_map[ENC_CP437_ART]   = ASCIIArtFontName;
	m_wxencfont_map[ENC_Windows_874] = GetMSCPFontName(wxT("874"));
	m_wxencfont_map[ENC_MS932]       = GetMSCPFontName(wxT("932"));
	m_wxencfont_map[ENC_MS936]       = GetMSCPFontName(wxT("936"));
	m_wxencfont_map[ENC_MS949]       = GetMSCPFontName(wxT("949"));
	m_wxencfont_map[ENC_MS950]       = GetMSCPFontName(wxT("950"));
	m_wxencfont_map[ENC_CP20932]     = GetMSCPFontName(wxT("51932"));
	m_wxencfont_map[ENC_GB18030]     = GetMSCPFontName(wxT("936"));

	InitEncodingGroups();
	InitSystemEncoding();
}

WXMEncodingID WXMEncodingManager::GetSystemEncodingID()
{
	static WXMEncodingID sysencid = ENC_DEFAULT;
	if (sysencid != ENC_DEFAULT)
		return sysencid;

	sysencid = xm::wrap_map(m_icunameenc_map).get(GetEncodingICUName(NULL), ENC_ISO_8859_1);

	if (sysencid == ENC_ISO_646)
		sysencid = ENC_ISO_8859_1;

	return sysencid;
}

void WXMEncodingManager::InitSystemEncoding()
{
	WXMEncodingID id = GetSystemEncodingID();

	wxString wxenc = EncodingToName(id);
	for(size_t i=0; i<m_wxenc_list.size(); ++i)
	{
		if (m_wxenc_list[i] == wxenc)
		{
			m_sysenc_idx = i;
			break;
		}
	}
}

void WXMEncodingManager::InitEncodingGroups()
{
	boost::assign::insert(m_wxencgrpname_map)
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


	boost::assign::insert(m_wxencgrps_map)
		// ISO-8859
		(ENC_ISO_8859_1,  boost::assign::list_of(ENCG_ISO8859)(ENCG_WESTERNEUROPE))
		(ENC_ISO_8859_2,  boost::assign::list_of(ENCG_ISO8859)(ENCG_CENTRALEUROPE))
		(ENC_ISO_8859_3,  boost::assign::list_of(ENCG_ISO8859)(ENCG_SOUTHEUROPE))
		(ENC_ISO_8859_4,  boost::assign::list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE))
		(ENC_ISO_8859_5,  boost::assign::list_of(ENCG_ISO8859)(ENCG_CYRILLIC))
		(ENC_ISO_8859_6,  boost::assign::list_of(ENCG_ISO8859)(ENCG_ARABIC))
		(ENC_ISO_8859_7,  boost::assign::list_of(ENCG_ISO8859)(ENCG_GREEK))
		(ENC_ISO_8859_8,  boost::assign::list_of(ENCG_ISO8859)(ENCG_HEBREW))
		(ENC_ISO_8859_9,  boost::assign::list_of(ENCG_ISO8859)(ENCG_TURKISH))
		(ENC_ISO_8859_10, boost::assign::list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE))
		(ENC_ISO_8859_11, boost::assign::list_of(ENCG_ISO8859)(ENCG_SOUTHEASTASIA))
		(ENC_ISO_8859_13, boost::assign::list_of(ENCG_ISO8859)(ENCG_BALTIC))
		(ENC_ISO_8859_14, boost::assign::list_of(ENCG_ISO8859)(ENCG_NORTHEUROPE))
		(ENC_ISO_8859_15, boost::assign::list_of(ENCG_ISO8859)(ENCG_WESTERNEUROPE))
		(ENC_ISO_8859_16, boost::assign::list_of(ENCG_ISO8859)(ENCG_SOUTHEUROPE))
		// Windows
		(ENC_Windows_874,  boost::assign::list_of(ENCG_WINDOWS)(ENCG_SOUTHEASTASIA))
		(ENC_Windows_1250, boost::assign::list_of(ENCG_WINDOWS)(ENCG_CENTRALEUROPE))
		(ENC_Windows_1251, boost::assign::list_of(ENCG_WINDOWS)(ENCG_CYRILLIC))
		(ENC_Windows_1252, boost::assign::list_of(ENCG_WINDOWS)(ENCG_WESTERNEUROPE))
		(ENC_Windows_1253, boost::assign::list_of(ENCG_WINDOWS)(ENCG_GREEK))
		(ENC_Windows_1254, boost::assign::list_of(ENCG_WINDOWS)(ENCG_TURKISH))
		(ENC_Windows_1255, boost::assign::list_of(ENCG_WINDOWS)(ENCG_HEBREW))
		(ENC_Windows_1256, boost::assign::list_of(ENCG_WINDOWS)(ENCG_ARABIC))
		(ENC_Windows_1257, boost::assign::list_of(ENCG_WINDOWS)(ENCG_BALTIC))
		(ENC_Windows_1258, boost::assign::list_of(ENCG_WINDOWS)(ENCG_SOUTHEASTASIA))
		// OEM
		(ENC_CP437, boost::assign::list_of(ENCG_OEM))
		(ENC_CP850, boost::assign::list_of(ENCG_OEM))
		(ENC_CP852, boost::assign::list_of(ENCG_OEM))
		(ENC_CP855, boost::assign::list_of(ENCG_OEM)(ENCG_CYRILLIC))
		(ENC_CP866, boost::assign::list_of(ENCG_OEM)(ENCG_CYRILLIC))
		// Cyrillic
		(ENC_KOI8_R, boost::assign::list_of(ENCG_CYRILLIC))
		(ENC_KOI8_U, boost::assign::list_of(ENCG_CYRILLIC))
		// Windows & East Asia
		(ENC_MS932,   boost::assign::list_of(ENCG_WINDOWS)(ENCG_EASTASIA))
		(ENC_MS936,   boost::assign::list_of(ENCG_WINDOWS)(ENCG_EASTASIA))
		(ENC_MS949,   boost::assign::list_of(ENCG_WINDOWS)(ENCG_EASTASIA))
		(ENC_MS950,   boost::assign::list_of(ENCG_WINDOWS)(ENCG_EASTASIA))
		(ENC_CP20932, boost::assign::list_of(ENCG_WINDOWS)(ENCG_EASTASIA))
		// Unicode
		(ENC_GB18030,  boost::assign::list_of(ENCG_UNICODE)(ENCG_EASTASIA))
		(ENC_UTF_8,    boost::assign::list_of(ENCG_UNICODE))
		(ENC_UTF_16LE, boost::assign::list_of(ENCG_UNICODE))
		(ENC_UTF_16BE, boost::assign::list_of(ENCG_UNICODE))
		(ENC_UTF_32LE, boost::assign::list_of(ENCG_UNICODE))
		(ENC_UTF_32BE, boost::assign::list_of(ENCG_UNICODE))
	;
}

std::vector<WXMEncodingGroupID> WXMEncodingManager::GetEncodingGroups(ssize_t idx)
{
	WXEncGrpsMap::const_iterator it = m_wxencgrps_map.find(IdxToEncoding(idx));
	if (it == m_wxencgrps_map.end())
	{
		std::vector<WXMEncodingGroupID> v;
		v.push_back(ENCG_DEFAULT);
		return v;
	}

	std::vector<WXMEncodingGroupID> v(it->second.begin(), it->second.end());
	return v;
}

wxString WXMEncodingManager::EncodingGroupToName(WXMEncodingGroupID gid)
{
	return xm::wrap_map(m_wxencgrpname_map).get(gid, wxString());
}

void WXMEncodingManager::FreeEncodings()
{
	BOOST_FOREACH(EncInstMap::value_type val, m_inst_map)
	{
		if (m_sysenc == val.second)
			m_sysenc = NULL;

		delete val.second;
	}
	m_inst_map.clear();

	if (m_sysenc != NULL)
		delete m_sysenc;
	m_sysenc = NULL;

	m_initialized = false;
	DestroyInstance();
}

size_t WXMEncodingManager::GetEncodingsCount()
{
	return m_wxenc_map.size();
}

wxString WXMEncodingManager::GetEncodingName(ssize_t idx)
{
	if (idx<0 || (size_t)idx>=m_wxenc_list.size())
		return EncodingToName(GetSystemEncodingID());

	return m_wxenc_list[idx];
}

std::string WXMEncodingManager::GetEncodingInnerName(ssize_t idx)
{
	return xm::wrap_map(m_encinnername_map).get(IdxToEncoding(idx), "unknown");
}

wxString WXMEncodingManager::GetEncodingDescription(ssize_t idx)
{
	return xm::wrap_map(m_wxencdesc_map).get(IdxToEncoding(idx), GetEncodingName(idx));
}

wxString WXMEncodingManager::GetEncodingAliases(ssize_t idx)
{
	return xm::wrap_map(m_wxencalias_map).get(IdxToEncoding(idx), wxString());
}

wxString WXMEncodingManager::GetEncodingFontName(ssize_t idx)
{
	return xm::wrap_map(m_wxencfont_map).get(IdxToEncoding(idx), GetMSCPFontName(wxString()));
}

wxString WXMEncodingManager::EncodingToName(WXMEncodingID enc)
{
	return xm::wrap_map(m_wxencname_map).get(enc, wxT("Unknown Encoding"));
}

WXMEncodingID WXMEncodingManager::NameToEncoding(const wxString &name)
{
	return xm::wrap_map(m_wxnameenc_map).get(name, GetSystemEncodingID());
}

WXMEncodingID WXMEncodingManager::ExtNameToEncoding(const std::string &name)
{
	WXNameEncMap::const_iterator itwx = m_wxnameenc_map.find(wxString(name.c_str(), wxConvUTF8));
	if (itwx != m_wxnameenc_map.end())
		return itwx->second;

	return xm::wrap_map(m_icunameenc_map).get(GetEncodingICUName(name.c_str()), ENC_DEFAULT);
}

WXMEncoding* WXMEncodingManager::GetSystemEncoding()
{
	if (m_sysenc == NULL)
		m_sysenc = GetWxmEncoding(m_sysenc_idx);

	return m_sysenc;
}

WXMEncodingManager::WXMEncodingType WXMEncodingManager::GetIdxEncType(ssize_t idx)
{
	return xm::wrap_map(m_wxenctype_map).get(IdxToEncoding(idx), etSingleByte/*FIXME later*/);
}

WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx)
{
	wxASSERT(idx<(ssize_t)m_wxenc_list.size() && idx>=0);

	EncInstMap::iterator it = m_inst_map.find(idx);
	if (it!=m_inst_map.end() && it->second!=NULL)
		return it->second;

	WXMEncoding* enc = NULL;

	WXMEncodingType t = GetIdxEncType(idx);

	switch(t)
	{
		case etUTF8:
			enc = new WXMEncodingUTF8();
			break;
		case etUTF16LE:
			enc = new WXMEncodingUTF16LE();
			break;
		case etUTF16BE:
			enc = new WXMEncodingUTF16BE();
			break;
		case etUTF32LE:
			enc = new WXMEncodingUTF32LE();
			break;
		case etUTF32BE:
			enc = new WXMEncodingUTF32BE();
			break;
		case etSingleByte:
			enc = new WXMEncodingSingleByteISO646Compatible();
			break;
		case etDoubleByte:
			enc = new WXMEncodingDoubleByteISO646Compatible();
			break;
		case etCP20932:
			enc = new WXMEncodingCP20932();
			break;
		case etGB18030:
			enc = new WXMEncodingGB18030();
			break;
		case etCP437ART:
			enc = new WXMEncodingCP437Art();
			break;
	}

	enc->Create(idx);
	m_inst_map[idx] = enc;

	return enc;
}

WXMEncoding* WXMEncodingManager::GetWxmEncoding(WXMEncodingID enc)
{
	size_t idx;
	for(idx=0;idx<m_wxenc_list.size();idx++)
	{
		if(m_wxnameenc_map[m_wxenc_list[idx]]==enc)
		{
			return GetWxmEncoding(idx);
		}
	}

	return GetWxmEncoding(m_sysenc_idx);
}

WXMEncoding* WXMEncodingManager::GetWxmEncoding(const wxString &name)
{
	size_t idx;
	for(idx=0;idx<m_wxenc_list.size();idx++)
	{
		if(m_wxenc_list[idx].Lower()==name.Lower())
		{
			return GetWxmEncoding(idx);
		}
	}

	return GetWxmEncoding(m_sysenc_idx);
}

void WXMEncoding::Create(ssize_t idx)
{
	m_idx = idx;
	m_name = WXMEncodingManager::Instance().GetEncodingName(idx);
	m_enc = WXMEncodingManager::Instance().NameToEncoding(m_name);
	m_innername = WXMEncodingManager::Instance().GetEncodingInnerName(idx);
	m_desc = WXMEncodingManager::Instance().GetEncodingDescription(idx);
	m_aliases = WXMEncodingManager::Instance().GetEncodingAliases(idx);
	m_fontname = WXMEncodingManager::Instance().GetEncodingFontName(m_idx);
	m_simp_unicode = WXMEncodingManager::IsSimpleUnicodeEncoding(m_enc);
}


bool WXMEncodingDecoderISO646::IsUChar32_LineFeed(const wxByte* buf, size_t len)
{
	return *buf == 0x0A;
}
bool WXMEncodingDecoderISO646::IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len)
{
	wxByte b;
	dumper.Dump(&b, 1);
	return b == 0x0A;
}
ucs4_t WXMEncodingDecoderISO646::PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len)
{
	wxByte b;
	dumper.Dump(&b, 1);
	if (b == 0x0A || b == 0x0D)
		return (ucs4_t)b;
	return 0;
}

};// namespace wxm
