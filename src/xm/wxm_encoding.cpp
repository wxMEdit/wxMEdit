///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.cpp
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding_unicode.h"
#include "wxm_encoding_singlebyte.h"
#include "wxm_encoding_doublebyte.h"
#include "wxm_encoding_eucjpms.h"
#include <wx/config.h>
#include <wx/log.h>
#include <wx/fontenc.h>
#include <wx/intl.h>
#include <unicode/uvernum.h>
#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

BOOST_STATIC_ASSERT(U_ICU_VERSION_MAJOR_NUM >= 51);

#ifdef __WXMSW__
#define MSW_GET_FONT_NAME(cp,fn) MSW_GetFontName(cp,fn)

// get fontname from registry mime database
static void MSW_GetFontName(const wxString codepage, wxString &fontname)
{
	const wxString MIMEDB(wxT("HKEY_CLASSES_ROOT\\MIME\\Database\\Codepage\\"));
	wxRegKey *pRegKey = new wxRegKey(MIMEDB + codepage);

	if(!pRegKey->Exists())
	{
		delete pRegKey;
		return;
	}

	long cp;
	if(pRegKey->QueryValue(wxT("Family"), &cp))
	{
		delete pRegKey;
		pRegKey = new wxRegKey(MIMEDB + wxString::Format(wxT("%d"), cp));
	}

	pRegKey->QueryValue(wxT("FixedWidthFont"), fontname);

	delete pRegKey;
}

#else
#define MSW_GET_FONT_NAME(cp,fn)
#endif

static wxString GetMSCPFontName(const wxString mscp)
{
	wxString fontname;
#ifdef __WXMSW__
	fontname=wxT("Courier New");
#else
	fontname=wxT("Monospace");
#endif

	if (mscp.IsEmpty())
		return fontname;

	MSW_GET_FONT_NAME(mscp, fontname);
	return fontname;
}

namespace wxm
{

void WXMEncodingCreator::AddEncoding(const std::string& encname, WXMEncodingID encid
		, const wxString& desc, WXMEncodingType entype, const std::string& innername0)
{
	m_wxenc_map[encname] = encid;
	m_wxenctype_map[encid] = entype;

	wxString wxencname(encname.c_str(), wxConvUTF8);

	m_wxenc_list.push_back(wxencname);
	m_wxnameenc_map[wxencname] = encid;
	m_wxencname_map[encid] = wxencname;

	m_wxencinnername_map[encid] = innername0.empty()? encname: innername0;

	m_wxencdesc_map[encid] = desc.IsEmpty()? wxencname: desc;
}

void WXMEncodingCreator::DoInit()
{
	wxLogNull nolog;

	AddEncoding("ISO-8859-1", ENC_ISO_8859_1, wxT("Western European (ISO-8859-1)"));
	AddEncoding("ISO-8859-2", ENC_ISO_8859_2, wxT("Central European (ISO-8859-2)"));
	AddEncoding("ISO-8859-3", ENC_ISO_8859_3, wxT("Esperanto (ISO-8859-3)"));
	AddEncoding("ISO-8859-4", ENC_ISO_8859_4, wxT("Baltic (old) (ISO-8859-4)"));
	AddEncoding("ISO-8859-5", ENC_ISO_8859_5, wxT("Cyrillic (ISO-8859-5)"));
	AddEncoding("ISO-8859-6", ENC_ISO_8859_6, wxT("Arabic (ISO-8859-6)"));
	AddEncoding("ISO-8859-7", ENC_ISO_8859_7, wxT("Greek (ISO-8859-7)"), etSingleByte, "IBM-9005");
	AddEncoding("ISO-8859-8", ENC_ISO_8859_8, wxT("Hebrew (ISO-8859-8)"));
	AddEncoding("ISO-8859-9", ENC_ISO_8859_9, wxT("Turkish (ISO-8859-9)"));
	AddEncoding("ISO-8859-10", ENC_ISO_8859_10, wxT("Nordic (ISO-8859-10)"));
	AddEncoding("ISO-8859-11", ENC_ISO_8859_11, wxT("Thai (ISO-8859-11)"));
	AddEncoding("ISO-8859-13", ENC_ISO_8859_13, wxT("Baltic (ISO-8859-13)"));
	AddEncoding("ISO-8859-14", ENC_ISO_8859_14, wxT("Celtic (ISO-8859-14)"));
	AddEncoding("ISO-8859-15", ENC_ISO_8859_15, wxT("Western European with Euro (ISO-8859-15)"));
	AddEncoding("ISO-8859-16", ENC_ISO_8859_16, wxT("South-Eastern European (ISO-8859-16)"), etSingleByte, "ISO-8859-15");
	AddEncoding("Windows-874", ENC_Windows_874, wxT("Windows Thai (CP 874)"));
	AddEncoding("Windows-1250", ENC_Windows_1250, wxT("Windows Central European (CP 1250)"), etSingleByte, "CP1250");
	AddEncoding("Windows-1251", ENC_Windows_1251, wxT("Windows Cyrillic (CP 1251)"), etSingleByte, "CP1251");
	AddEncoding("Windows-1252", ENC_Windows_1252, wxT("Windows Western European (CP 1252)"), etSingleByte, "CP1252");
	AddEncoding("Windows-1253", ENC_Windows_1253, wxT("Windows Greek (CP 1253)"), etSingleByte, "CP1253");
	AddEncoding("Windows-1254", ENC_Windows_1254, wxT("Windows Turkish (CP 1254)"), etSingleByte, "CP1254");
	AddEncoding("Windows-1255", ENC_Windows_1255, wxT("Windows Hebrew (CP 1255)"), etSingleByte, "CP1255");
	AddEncoding("Windows-1256", ENC_Windows_1256, wxT("Windows Arabic (CP 1256)"), etSingleByte, "CP1256");
	AddEncoding("Windows-1257", ENC_Windows_1257, wxT("Windows Baltic (CP 1257)"), etSingleByte, "CP1257");
	AddEncoding("Windows-1258", ENC_Windows_1258, wxT("Windows Vietnamese (CP 1258)"), etSingleByte, "CP1258");
	AddEncoding("CP437", ENC_CP437, wxT("Windows/DOS OEM (CP 437)"));
	AddEncoding("CP850", ENC_CP850, wxT("Windows/DOS OEM - Latin 1 (CP 850)"));
	AddEncoding("CP852", ENC_CP852, wxT("Windows/DOS OEM - Latin 2 (CP 852)"));
	AddEncoding("CP855", ENC_CP855, wxT("Windows/DOS OEM - Cyrillic (CP 855)"));
	AddEncoding("CP866", ENC_CP866, wxT("Windows/DOS OEM - Cyrillic (CP 866)"));
	AddEncoding("KOI8-R", ENC_KOI8_R, wxT(""));
	AddEncoding("KOI8-U", ENC_KOI8_U, wxT(""));

	AddEncoding("Windows-31J", ENC_MS932, wxT("Windows Japanese (CP 932)"), etDoubleByte, "MS932");
	AddEncoding("MS936"      , ENC_MS936, wxT("Windows Chinese Simplified (CP 936)"), etDoubleByte, "MS936");
	AddEncoding("UHC"        , ENC_MS949, wxT("Windows Korean (CP 949)"), etDoubleByte, "MS949");
	AddEncoding("MS950"      , ENC_MS950, wxT("Windows Chinese Traditional (CP 950)"), etDoubleByte, "MS950");
	AddEncoding("EUC-JP-MS", ENC_EUCJP_MS, wxT("Windows EUC-JP Double-Byte Edition (CP 20932)"), etEUCJPMS, "EUC-JP");

	AddEncoding("UTF-8", ENC_UTF_8, wxT("Unicode 8 bit (UTF-8)"), etUTF8);
	AddEncoding("UTF-16LE", ENC_UTF_16LE, wxT("Unicode 16 bit Little Endian (UTF-16LE)"), etUTF16LE);
	AddEncoding("UTF-16BE", ENC_UTF_16BE, wxT("Unicode 16 bit Big Endian (UTF-16BE)"), etUTF16BE);
	AddEncoding("UTF-32LE", ENC_UTF_32LE, wxT("Unicode 32 bit Little Endian (UTF-32LE)"), etUTF32LE);
	AddEncoding("UTF-32BE", ENC_UTF_32BE, wxT("Unicode 32 bit Big Endian (UTF-32BE)"), etUTF32BE);
	
	m_wxencfont_map[ENC_Windows_874] = GetMSCPFontName(wxT("874"));
	m_wxencfont_map[ENC_MS932] = GetMSCPFontName(wxT("932"));
	m_wxencfont_map[ENC_MS936] = GetMSCPFontName(wxT("936"));
	m_wxencfont_map[ENC_MS949] = GetMSCPFontName(wxT("949"));
	m_wxencfont_map[ENC_MS950] = GetMSCPFontName(wxT("950"));
	m_wxencfont_map[ENC_EUCJP_MS] = GetMSCPFontName(wxT("51932"));

	InitSystemEncoding();
	
}

WXMEncodingID WXMEncodingCreator::GetSystemEncodingID()
{
	static WXMEncodingID sysencid = ENC_DEFAULT;
	if (sysencid != ENC_DEFAULT)
		return sysencid;

	wxFontEncoding wxenc = wxLocale::GetSystemEncoding();

	if (wxenc>=wxFONTENCODING_ISO8859_1 && wxenc<=wxFONTENCODING_ISO8859_15)
		sysencid = WXMEncodingID(ENC_ISO_8859_1 + (wxenc - wxFONTENCODING_ISO8859_1));
	else if(wxenc>=wxFONTENCODING_CP1250 && wxenc<=wxFONTENCODING_CP1257)
		sysencid = WXMEncodingID(ENC_Windows_1250 + (wxenc - wxFONTENCODING_CP1250));
	else if(wxenc==wxFONTENCODING_CP437)
		sysencid = ENC_CP437;
	else if(wxenc==wxFONTENCODING_CP850)
		sysencid = ENC_CP850;
	else if(wxenc==wxFONTENCODING_CP852)
		sysencid = ENC_CP852;
	else if(wxenc==wxFONTENCODING_CP855)
		sysencid = ENC_CP855;
	else if(wxenc==wxFONTENCODING_CP866)
		sysencid = ENC_CP866;
	else if(wxenc==wxFONTENCODING_KOI8)
		sysencid = ENC_KOI8_R;
	else if(wxenc==wxFONTENCODING_KOI8_U)
		sysencid = ENC_KOI8_U;
	else if(wxenc==wxFONTENCODING_CP932)
		sysencid = ENC_MS932;
	else if(wxenc==wxFONTENCODING_CP936)
		sysencid = ENC_MS936;
	else if(wxenc==wxFONTENCODING_CP949)
		sysencid = ENC_MS949;
	else if(wxenc==wxFONTENCODING_CP950)
		sysencid = ENC_MS950;
	// EUC-JP-MS cannot mapping directly
	else if(wxenc==wxFONTENCODING_UTF8)
		sysencid = ENC_UTF_8;
	else if(wxenc==wxFONTENCODING_UTF16LE)
		sysencid = ENC_UTF_16LE;
	else if(wxenc==wxFONTENCODING_UTF16BE)
		sysencid = ENC_UTF_16BE;
	else if(wxenc==wxFONTENCODING_UTF32LE)
		sysencid = ENC_UTF_32LE;
	else if(wxenc==wxFONTENCODING_UTF32BE)
		sysencid = ENC_UTF_32BE;
	else
		sysencid = ENC_ISO_8859_1;

	return sysencid;
}

void WXMEncodingCreator::InitSystemEncoding()
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

void WXMEncodingCreator::FreeEncodings()
{
	BOOST_FOREACH(WXEncInstMap::value_type val, m_inst_map)
		delete val.second;
	m_inst_map.clear();

	if (m_sysenc)
		delete m_sysenc;
	m_sysenc = NULL;

	m_initialized = false;
}

size_t WXMEncodingCreator::GetEncodingsCount()
{
	return m_wxenc_map.size();
}

wxString WXMEncodingCreator::GetEncodingName(ssize_t idx)
{
	if (idx<0 || (size_t)idx>=m_wxenc_list.size())
		return EncodingToName(GetSystemEncodingID());

	return m_wxenc_list[idx];
}

std::string WXMEncodingCreator::GetEncodingInnerName(ssize_t idx)
{
	WXEncInnerNameMap::const_iterator it = m_wxencinnername_map.find(IdxToEncoding(idx));
	if (it == m_wxencinnername_map.end())
		return "unknown";

	return it->second;
}

wxString WXMEncodingCreator::GetEncodingDescription(ssize_t idx)
{
	WXEncDescMap::const_iterator it = m_wxencdesc_map.find(IdxToEncoding(idx));
	if (it == m_wxencdesc_map.end())
		return GetEncodingName(idx);

	return it->second;
}

wxString WXMEncodingCreator::GetEncodingFontName(ssize_t idx)
{
	WXEncFontMap::const_iterator it = m_wxencfont_map.find(IdxToEncoding(idx));
	if (it == m_wxencfont_map.end())
		return GetMSCPFontName(wxString());

	return it->second;
}

wxString WXMEncodingCreator::EncodingToName(WXMEncodingID enc)
{
	WXEncNameMap::const_iterator it = m_wxencname_map.find(enc);
	if (it == m_wxencname_map.end())
		return wxT("Unknown Encoding");

	return it->second;
}

WXMEncodingID WXMEncodingCreator::NameToEncoding(const wxString &name)
{
	WXNameEncMap::const_iterator it = m_wxnameenc_map.find(name);
	if (it == m_wxnameenc_map.end())
		return GetSystemEncodingID();

	return it->second;
}

WXMEncoding* WXMEncodingCreator::GetSystemEncoding()
{
	if (m_sysenc == NULL)
		m_sysenc = CreateWxmEncoding(m_sysenc_idx);

	return m_sysenc;
}

WXMEncodingType WXMEncodingCreator::GetIdxEncType(ssize_t idx)
{
	WXEncTypeMap::const_iterator it = m_wxenctype_map.find(IdxToEncoding(idx));
	if (it == m_wxenctype_map.end())
		return etSingleByte; //FIXME later

	return it->second;
}

WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx)
{
	wxASSERT(idx<(ssize_t)m_wxenc_list.size() && idx>=0);
	WXEncInstMap::iterator it = m_inst_map.find(idx);
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
		case etEUCJPMS:
			enc = new WXMEncodingEUCJPMS();
			break;
	}

	enc->Create(idx);
	m_inst_map[idx] = enc;

	return enc;
}

WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(WXMEncodingID enc)
{
	size_t idx;
	for(idx=0;idx<m_wxenc_list.size();idx++)
	{
		if(m_wxnameenc_map[m_wxenc_list[idx]]==enc)
		{
			return CreateWxmEncoding(idx);
		}
	}

	return CreateWxmEncoding(m_sysenc_idx);
}

WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(const wxString &name)
{
	size_t idx;
	for(idx=0;idx<m_wxenc_list.size();idx++)
	{
		if(m_wxenc_list[idx].Lower()==name.Lower())
		{
			return CreateWxmEncoding(idx);
		}
	}

	return CreateWxmEncoding(m_sysenc_idx);
}

void WXMEncoding::Create(ssize_t idx)
{
	m_idx = idx;
	m_name = WXMEncodingCreator::Instance().GetEncodingName(idx);
	m_enc = WXMEncodingCreator::Instance().NameToEncoding(m_name);
	m_innername = WXMEncodingCreator::Instance().GetEncodingInnerName(idx);
	m_type = WXMEncodingCreator::Instance().GetIdxEncType(idx);
	m_desc = WXMEncodingCreator::Instance().GetEncodingDescription(idx);
	m_fontname = WXMEncodingCreator::Instance().GetEncodingFontName(m_idx);
	m_simp_unicode = WXMEncodingCreator::IsSimpleUnicodeEncoding(m_enc);
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
