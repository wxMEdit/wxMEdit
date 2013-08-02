///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.cpp
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encoding.h"
#include "wxm_wx_icu.h"
#include <wx/config.h>
#include <wx/fontmap.h>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

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
	AddEncoding("Windows-31J", wxFONTENCODING_CP932, etDoubleByte, "CP932");
	AddEncoding("GBK/CP936", wxFONTENCODING_CP936, etDoubleByte, "CP936");
	AddEncoding("UHC", wxFONTENCODING_CP949, etDoubleByte, "CP949");
	AddEncoding("Big-5/CP950", wxFONTENCODING_CP950, etDoubleByte, "CP950");
	AddEncoding("EUC-JP", wxFONTENCODING_EUC_JP, etDoubleByte);
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

extern "C"
{
	void error_callback(const void *context, UConverterFromUnicodeArgs *args, const UChar *codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode *pErrorCode)
	{
		*pErrorCode = U_INVALID_CHAR_FOUND;
	}
}

ICUConverter::ICUConverter(const std::string& encname): m_ucnv(NULL)
{
	UErrorCode err = U_ZERO_ERROR;

	m_ucnv = ucnv_open(encname.c_str(), &err);

	ucnv_setFallback(m_ucnv, FALSE);
	ucnv_setFromUCallBack(m_ucnv, error_callback, NULL, NULL, NULL, &err);
}

ICUConverter::~ICUConverter()
{
	ucnv_close(m_ucnv);
	m_ucnv = NULL;
}

size_t ICUConverter::MB2WC(UChar32& ch, const char* src, size_t src_len)
{
	UChar dest[2];
	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_toUChars(m_ucnv, dest, 2, src, src_len, &err);

	if (n == 1)
	{
		if (dest[0]==0xFFFD)
			return 0;

		ch = dest[0];
		return 1;
	}

	if (n<1 || n>=2 && (dest[0]<0xD800 || dest[0]>0xDBFF || dest[1]<0xDC00 || dest[1]>0xDFFF))
		return 0;

	ch = 0x10000 + (((dest[0] & 0x03FF) << 10) | (dest[1] & 0x03FF));

	return 1;
}

size_t ICUConverter::WC2MB(char* dest, size_t dest_len, const UChar32& ch)
{
	UChar src[2] = {0, 0};
	size_t src_len = 1;
	if (ch <= 0xFFFF)
	{
		src[0] = ch;
	}
	else
	{
		UChar32 tmp = ch - 0x10000;
		src[0] = (ch >> 10) | 0xD800;
		src[1] = (ch & 0x3FF) | 0xDC00;
	}

	UErrorCode err = U_ZERO_ERROR;
	int32_t n = ucnv_fromUChars(m_ucnv, dest, dest_len, src, src_len, &err);

	if (n!=1)
		return 0;

	return 1;
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

void WXMEncodingMultiByte::Create(ssize_t idx)
{
	WXMEncoding::Create(idx);

	MultiByteInit();
}

void OEMTableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	toutab[0x1A] = 0x00001A;
	toutab[0x1C] = 0x00001C;
	toutab[0x7f] = 0x00007f;

	fromutab[0x00001A] = 0x1A;
	fromutab[0x00001C] = 0x1C;
	fromutab[0x00007F] = 0x7F;
}

void CP437TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	OEMTableFixer::fix(toutab, fromutab);

	toutab[0xE6] = 0x0000B5;

	fromutab.erase(0x0003BC);
	fromutab[0x0000B5] = 0xE6;
}

void CP852TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	OEMTableFixer::fix(toutab, fromutab);

	toutab[0xAA] = 0x0000AC;

	fromutab.erase(0x0000AA);
	fromutab[0x0000AC] = 0xAA;
}

void Windows874TableFixer::fix(ByteUnicodeArr& toutab, UnicodeByteMap& fromutab)
{
	toutab[0xDB] = 0x0000DB;
	toutab[0xDC] = 0x0000DC;
	toutab[0xDD] = 0x0000DD;
	toutab[0xDE] = 0x0000DE;
	toutab[0xFC] = 0x0000FC;
	toutab[0xFD] = 0x0000FD;
	toutab[0xFE] = 0x0000FE;
	toutab[0xFF] = 0x0000FF;

	fromutab[0x0000DB] = 0xDB;
	fromutab[0x0000DC] = 0xDC;
	fromutab[0x0000DD] = 0xDD;
	fromutab[0x0000DE] = 0xDE;
	fromutab[0x0000FC] = 0xFC;
	fromutab[0x0000FD] = 0xFD;
	fromutab[0x0000FE] = 0xFE;
	fromutab[0x0000FF] = 0xFF;
}

EncodingTableFixer* WXMEncodingSingleByte::CreateEncodingTableFixer()
{
	if (m_name == wxT("CP437"))
		return new CP437TableFixer();
	if (m_name == wxT("CP850") || m_name == wxT("CP855") || m_name == wxT("CP866"))
		return new OEMTableFixer();
	if (m_name == wxT("CP852"))
		return new CP852TableFixer();
	if (m_name == wxT("Windows-874"))
		return new Windows874TableFixer();
	return new EncodingTableFixer();
}

void WXMEncodingSingleByte::MultiByteInit()
{
	m_icucnv = new ICUConverter(m_innername);

	boost::scoped_ptr<EncodingTableFixer>enc_fix(CreateEncodingTableFixer());

	char singlebyte[1];
	UChar32 ch;
	for (size_t i=0; i<256; ++i)
	{
		singlebyte[0] = i;
		if (m_icucnv->MB2WC(ch, singlebyte, 1) == 1)
		{
			m_tounicode[i] = ch;
		}
		else
		{
			m_tounicode[i] = i;
			m_fromunicode[i] = i;
		}
	}

	for (ucs4_t i=0; i<=0x10FFFF; ++i)
	{
		if (i>=0xD800 && i<=0xF8FF) // skip PUA and surrogates
			continue;

		ch = i;
		if (m_icucnv->WC2MB(singlebyte, 1, ch) == 1)
		{
			m_fromunicode[i] = singlebyte[0];
		}
	}

	enc_fix->fix(m_tounicode, m_fromunicode);
}

ucs4_t WXMEncodingSingleByte::MultiBytetoUCS4(wxByte* buf)
{
	return m_tounicode[*buf];
}

size_t WXMEncodingSingleByte::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	UnicodeByteMap::const_iterator it = m_fromunicode.find(ucs4);
	if (it == m_fromunicode.end() || it->second == 0)
		return 0;

	buf[0] = it->second;
	return 1;
}

void WXMEncodingDoubleByte::MultiByteInit()
{
#if defined(__WXGTK__)
	wxString wxinnnername = wxString(m_innername.c_str(), wxConvUTF8);
	m_CSConv=new wxCSConv(wxinnnername.c_str());
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
	m_CSConv=new wxCSConv(m_enc);
#endif

	memset(m_b2u_tab.data(), svtInvaliad, sizeof(ucs4_t)*256);

	memset(m_bmp2mb_tab.data(), svtNotCached, sizeof(wxWord)*0x10000);

	memset(m_leadbyte_tab.data(), lbUnset, 256);
	m_leadbyte_tab[0]=lbNotLeadByte;
}

// return 0 if it is not a valid DB char
ucs4_t WXMEncodingDoubleByte::MultiBytetoUCS4(wxByte* buf)
{
	if( IsLeadByte(buf[0]))
		return m_db2u_tab[buf[0]][buf[1]];

	return m_b2u_tab[buf[0]];
}

bool WXMEncodingDoubleByte::IsLeadByte(wxByte byte)
{
	if(m_leadbyte_tab[byte]==lbUnset)
	{
		wxByte dbs[3]={byte,0,0};
		wchar_t wc[4];

		// check first byte
		if(m_CSConv->MB2WC(wc,(char*)dbs,4)==1) // FIXME: no non-BMP support on Windows
		{
			m_b2u_tab[byte] = wc[0];

			m_leadbyte_tab[byte]=lbNotLeadByte;
			return false;
		}

		m_b2u_tab[byte] = 0;

		memset(m_db2u_tab[byte].data(), 0, sizeof(ucs4_t)*256);
		for(int i=1; i<=0xFF; ++i)
		{
			dbs[1] = i;
			if(m_CSConv->MB2WC(wc,(char*)dbs,4)==1)
			{
				m_db2u_tab[byte][i] = wc[0]; // FIXME: no non-BMP support on Windows

				m_leadbyte_tab[byte]  = lbLeadByte;
			}
		}
	}

	return m_leadbyte_tab[byte]==lbLeadByte;
}

size_t WXMEncodingDoubleByte::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if(ucs4>0xFFFF) // FIXME: no non-BMP support
		return 0;

	wxWord mb=m_bmp2mb_tab[ucs4];
	if(mb == 0) // invalid MB char
		return 0;

	if ((mb & 0xFF) == 0)
	{
		buf[0] = mb >> 8;
		return 1;
	}

	if(mb!=0xFFFF) // cached
	{
		buf[0] = mb >> 8;
		buf[1] = mb & 0xFF;
		return 2;
	}

	// non-cached
	size_t len;
	wxByte mbs[3];
	wchar_t wc[2]={ucs4, 0}; // FIXME: no non-BMP support on Windows
	len=m_CSConv->WC2MB((char*)mbs,wc,3);
	if(len==0 || len==(size_t)-1)
	{
		m_bmp2mb_tab[ucs4]=0;
		return 0;
	}

	wxASSERT( len<=2 );

	//mbs[len]=0;
	mb= (((wxWord)mbs[0])<<8) | mbs[1];
	m_bmp2mb_tab[ucs4]=mb;

	buf[0]=mbs[0];
	if(len==2)
		buf[1]=mbs[1];

	return len;
}

size_t WXMEncodingUTF8::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	/***  from rfc3629
	  Char. number range  |        UTF-8 octet sequence
	  (hexadecimal)       |              (binary)
	  --------------------+---------------------------------------------
	  0000 0000-0000 007F | 0xxxxxxx
	  0000 0080-0000 07FF | 110xxxxx 10xxxxxx
	  0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
	  0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

	  UTF8-octets = *( UTF8-char )
	  UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
	  UTF8-1      = %x00-7F
	  UTF8-2      = %xC2-DF UTF8-tail
	  UTF8-3      = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
	  %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
	  UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
	  %xF4 %x80-8F 2( UTF8-tail )
	  UTF8-tail   = %x80-BF
	 ***/

	if(ucs4<0)
		return 0;

	if(ucs4<=0x7F)
	{
		buf[0]=ucs4;
		return 1;
	}
	if(ucs4<=0x7FF)
	{
		buf[0]= 0xC0 | (ucs4>>6);
		buf[1]= 0x80 | (ucs4&0x3F);
		return 2;
	}
	if(ucs4<=0xFFFF)
	{
		buf[0]= 0xE0 | (ucs4>>12);
		buf[1]= 0x80 | ((ucs4>>6)&0x3F);
		buf[2]= 0x80 | (ucs4&0x3F);
		return 3;
	}
	if(ucs4<=0x10FFFF)
	{
		buf[0]= 0xF0 | (ucs4>>18);
		buf[1]= 0x80 | ((ucs4>>12)&0x3F);
		buf[2]= 0x80 | ((ucs4>>6)&0x3F);
		buf[3]= 0x80 | (ucs4&0x3F);
		return 4;
	}

	return 0;
}

size_t WXMEncodingUTF16LE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if(ucs4>=0x10000)// to unicode surrogates
	{
		if(ucs4>0x10FFFF) return 0;
		return UCS4toUTF16LE_U10000(ucs4, buf);
	}

	buf[0]=ucs4;
	buf[1]=ucs4>>8;

	return 2;
}
size_t WXMEncodingUTF16BE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	if(ucs4>=0x10000)// to unicode surrogates
	{
		if(ucs4>0x10FFFF) return 0;

		ucs4-=0x10000;
		ucs2_t high=(ucs4>>10)+0xD800;    // high surrogate
		ucs2_t low=(ucs4&0x3FF)+0xDC00;    // low surrogate

		buf[0]=high>>8;
		buf[1]=high;
		buf[2]=low>>8;
		buf[3]=low;

		return 4;
	}

	buf[0]=ucs4>>8;
	buf[1]=ucs4;

	return 2;
}

size_t WXMEncodingUTF32LE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	wxASSERT(ucs4>=0 && ucs4<=0x10FFFF);

#if wxBYTE_ORDER == wxBIG_ENDIAN
	wxByte *p = (wxByte*)&ucs4;
	buf[0]=p[3];
	buf[1]=p[2];
	buf[2]=p[1];
	buf[3]=p[0];
#else
	*((ucs4_t*)buf) = ucs4;
#endif
	return 4;
}
size_t WXMEncodingUTF32BE::UCS4toMultiByte(ucs4_t ucs4, wxByte* buf)
{
	wxASSERT(ucs4>=0 && ucs4<=0x10FFFF);

#if wxBYTE_ORDER == wxBIG_ENDIAN
	*((ucs4_t*)buf) = ucs4;
#else
	wxByte *p = (wxByte*)&ucs4;
	buf[0]=p[3];
	buf[1]=p[2];
	buf[2]=p[1];
	buf[3]=p[0];
#endif
	return 4;
}

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf)
{
	//ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
	//if(ucs4>0x10FFFF) return 0;

	//wxASSERT(ucs4>=0x10000 && ucs4<=0x10FFFF);

	ucs4-=0x10000;
	ucs2_t high=(ucs4>>10)+0xD800;    // high surrogate
	ucs2_t low=(ucs4&0x3FF)+0xDC00;    // low surrogate

	buf[0]=high;
	buf[1]=high>>8;
	buf[2]=low;
	buf[3]=low>>8;

	return 4;
}

};// namespace wxm
