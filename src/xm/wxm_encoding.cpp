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
#include <wx/fontmap.h>
#include <boost/foreach.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#ifdef __WXMSW__
#define MSW_GET_FONT_NAME(cp,fn) MSW_GetFontName(cp,fn)

// get fontname from registry mime database
void MSW_GetFontName(const wxString codepage, wxString &fontname)
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

wxString GetMSCPFontName(const wxString mscp)
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

void WXMEncodingCreator::AddEncoding(const std::string& encname, wxFontEncoding wxenc
	, WXMEncodingType entype, const std::string& innername0)
{
	m_wxenc_map[encname] = wxenc;
	m_wxenctype_map[wxenc] = entype;

	wxString wxencname(encname.c_str(), wxConvUTF8);

	m_wxenc_list.push_back(wxencname);
	m_wxnameenc_map[wxencname] = wxenc;
	m_wxencname_map[wxenc] = wxencname;

	std::string innername = innername0.empty()? encname: innername0;

	m_wxencinnername_map[wxenc] = innername;
}

void WXMEncodingCreator::DoInit()
{
	wxLogNull nolog;

	AddEncoding("ISO-8859-1", wxFONTENCODING_ISO8859_1);
	AddEncoding("ISO-8859-2", wxFONTENCODING_ISO8859_2);
	AddEncoding("ISO-8859-3", wxFONTENCODING_ISO8859_3);
	AddEncoding("ISO-8859-4", wxFONTENCODING_ISO8859_4);
	AddEncoding("ISO-8859-5", wxFONTENCODING_ISO8859_5);
	AddEncoding("ISO-8859-6", wxFONTENCODING_ISO8859_6);
	AddEncoding("ISO-8859-7", wxFONTENCODING_ISO8859_7, etSingleByte, "IBM-9005");
	AddEncoding("ISO-8859-8", wxFONTENCODING_ISO8859_8);
	AddEncoding("ISO-8859-9", wxFONTENCODING_ISO8859_9);
	AddEncoding("ISO-8859-10", wxFONTENCODING_ISO8859_10);
	AddEncoding("ISO-8859-11", wxFONTENCODING_ISO8859_11);
	AddEncoding("ISO-8859-13", wxFONTENCODING_ISO8859_13);
	AddEncoding("ISO-8859-14", wxFONTENCODING_ISO8859_14);
	AddEncoding("ISO-8859-15", wxFONTENCODING_ISO8859_15);
	AddEncoding("Windows-874", wxFONTENCODING_CP874);
	AddEncoding("Windows-1250", wxFONTENCODING_CP1250, etSingleByte, "CP1250");
	AddEncoding("Windows-1251", wxFONTENCODING_CP1251, etSingleByte, "CP1251");
	AddEncoding("Windows-1252", wxFONTENCODING_CP1252, etSingleByte, "CP1252");
	AddEncoding("Windows-1253", wxFONTENCODING_CP1253, etSingleByte, "CP1253");
	AddEncoding("Windows-1254", wxFONTENCODING_CP1254, etSingleByte, "CP1254");
	AddEncoding("Windows-1255", wxFONTENCODING_CP1255, etSingleByte, "CP1255");
	AddEncoding("Windows-1256", wxFONTENCODING_CP1256, etSingleByte, "CP1256");
	AddEncoding("Windows-1257", wxFONTENCODING_CP1257, etSingleByte, "CP1257");
	AddEncoding("CP437", wxFONTENCODING_CP437);
	AddEncoding("CP850", wxFONTENCODING_CP850);
	AddEncoding("CP852", wxFONTENCODING_CP852);
	AddEncoding("CP855", wxFONTENCODING_CP855);
	AddEncoding("CP866", wxFONTENCODING_CP866);
	AddEncoding("KOI8-R", wxFONTENCODING_KOI8);
	AddEncoding("KOI8-U", wxFONTENCODING_KOI8_U);
	AddEncoding("Windows-31J", wxFONTENCODING_CP932, etDoubleByte, "MS932");
	AddEncoding("MS936"      , wxFONTENCODING_CP936, etDoubleByte, "MS936");
	AddEncoding("UHC"        , wxFONTENCODING_CP949, etDoubleByte, "MS949");
	AddEncoding("MS950"      , wxFONTENCODING_CP950, etDoubleByte, "MS950");
	AddEncoding("EUC-JP-MS", wxFONTENCODING_EUC_JP, etEUCJPMS, "EUC-JP");
	AddEncoding("UTF-8", wxFONTENCODING_UTF8, etUTF8);
	AddEncoding("UTF-16LE", wxFONTENCODING_UTF16LE, etUTF16LE);
	AddEncoding("UTF-16BE", wxFONTENCODING_UTF16BE, etUTF16BE);
	AddEncoding("UTF-32LE", wxFONTENCODING_UTF32LE, etUTF32LE);
	AddEncoding("UTF-32BE", wxFONTENCODING_UTF32BE, etUTF32BE);
	
	m_wxencfont_map[wxFONTENCODING_CP874] = GetMSCPFontName(wxT("874"));
	m_wxencfont_map[wxFONTENCODING_CP932] = GetMSCPFontName(wxT("932"));
	m_wxencfont_map[wxFONTENCODING_CP936] = GetMSCPFontName(wxT("936"));
	m_wxencfont_map[wxFONTENCODING_CP949] = GetMSCPFontName(wxT("949"));
	m_wxencfont_map[wxFONTENCODING_CP950] = GetMSCPFontName(wxT("950"));
	m_wxencfont_map[wxFONTENCODING_EUC_JP] = GetMSCPFontName(wxT("51932"));

	m_wxencdesc_map[wxFONTENCODING_CP850] = wxT("Windows/DOS OEM - Latin 1 (CP 850)");
	m_wxencdesc_map[wxFONTENCODING_CP852] = wxT("Windows/DOS OEM - Latin 2 (CP 852)");
	m_wxencdesc_map[wxFONTENCODING_CP855] = wxT("Windows/DOS OEM - Cyrillic (CP 855)");
	m_wxencdesc_map[wxFONTENCODING_CP866] = wxT("Windows/DOS OEM - Cyrillic (CP 866)");

	m_sysenc_idx = AdjustIndex(m_sysenc_idx);
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
	if (idx < 0)
		return wxFontMapper::GetEncodingName(wxLocale::GetSystemEncoding());

	return m_wxenc_list[idx];
}

std::string WXMEncodingCreator::GetEncodingInnerName(ssize_t idx)
{
	WXEncInnerNameMap::const_iterator it = m_wxencinnername_map.find(IdxToEncoding(idx));
	if (it == m_wxencinnername_map.end())
		return wxFontMapper::GetEncodingName(wxLocale::GetSystemEncoding()).mb_str(wxConvUTF8).data();

	return it->second;
}

wxString WXMEncodingCreator::GetEncodingDescription(ssize_t idx)
{
	WXEncDescMap::const_iterator it = m_wxencdesc_map.find(IdxToEncoding(idx));
	if (it == m_wxencdesc_map.end())
		return wxFontMapper::GetEncodingDescription(IdxToEncoding(idx));

	return it->second;
}

wxString WXMEncodingCreator::GetEncodingFontName(ssize_t idx)
{
	WXEncFontMap::const_iterator it = m_wxencfont_map.find(IdxToEncoding(idx));
	if (it == m_wxencfont_map.end())
		return GetMSCPFontName(wxString());

	return it->second;
}

wxString WXMEncodingCreator::EncodingToName(wxFontEncoding enc)
{
	WXEncNameMap::const_iterator it = m_wxencname_map.find(enc);
	if (it == m_wxencname_map.end())
		return wxFontMapper::GetEncodingName(wxLocale::GetSystemEncoding());

	return it->second;
}

wxFontEncoding WXMEncodingCreator::NameToEncoding(const wxString &name)
{
	WXNameEncMap::const_iterator it = m_wxnameenc_map.find(name);
	if (it == m_wxnameenc_map.end())
		return wxLocale::GetSystemEncoding();

	return it->second;
}

WXMEncoding* WXMEncodingCreator::GetSystemEncoding()
{
	if (m_sysenc == NULL)
		m_sysenc = CreateWxmEncoding(m_sysenc_idx);

	return m_sysenc;
}

ssize_t WXMEncodingCreator::AdjustIndex(ssize_t idx)
{
	wxFontEncoding enc = IdxToEncoding(idx);

	for(size_t i=0; i<m_wxenc_list.size(); i++)
	{
		if(m_wxnameenc_map[m_wxenc_list[i]]==enc)
		{
			return i;
		}
	}

	return idx;
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
			enc = new WXMEncodingSingleByte();
			break;
		case etDoubleByte:
			enc = new WXMEncodingDoubleByte();
			break;
		case etEUCJPMS:
			enc = new WXMEncodingEUCJPMS();
			break;
	}

	enc->Create(idx);
	m_inst_map[idx] = enc;

	return enc;
}

WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(wxFontEncoding enc)
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
}

};// namespace wxm
