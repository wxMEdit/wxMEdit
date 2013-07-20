///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_H_
#define _WXM_ENCODING_H_

#include "../wxmedit/ucs4_t.h"

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <boost/utility.hpp>
#include <string>
#include <vector>
#include <map>

namespace wxm
{

enum WXMEncodingType
{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE };


class WXMEncoding: private boost::noncopyable
{
private:
	static ssize_t ms_sysenc_idx;
	static WXMEncoding *ms_sysenc;

	typedef std::map<std::string, wxFontEncoding> WXEncMap;
	static WXEncMap ms_wxenc_map;

	static std::vector<wxString> ms_wxenc_list;
	static std::vector<std::string> ms_enc_list;

	typedef std::map<wxString, wxFontEncoding> WXNameEncMap;
	typedef std::map<wxFontEncoding, wxString> WXEncNameMap;
	typedef std::map<wxFontEncoding, WXMEncodingType> WXEncTypeMap;
	typedef std::map<wxFontEncoding, wxString> WXEncFontMap;
	static WXNameEncMap ms_wxnameenc_map;
	static WXEncNameMap ms_wxencname_map;
	static WXEncTypeMap ms_wxenctype_map;
	static WXEncFontMap ms_wxencfont_map;

	typedef std::map<ssize_t, WXMEncoding*> WXEncInstMap;
	static WXEncInstMap ms_inst_map;

public:
	static void     InitEncodings(); // must call this before use WXMEncoding
	static void     FreeEncodings();

	static size_t   GetEncodingsCount();
	static wxString GetEncodingName(ssize_t idx);
	static wxString GetEncodingDescription(ssize_t idx);
	static wxString GetEncodingFontName(ssize_t idx);
	static wxString EncodingToName(wxFontEncoding enc);
	static wxFontEncoding NameToEncoding(const wxString &name);
	static WXMEncoding* GetSystemEncoding();
protected:
	static WXMEncodingType GetIdxEncType(ssize_t idx);
	static ssize_t AdjustIndex(ssize_t idx);

protected:
	wxString m_name;
	wxString m_desc;
	wxString m_fontname;
	wxFontEncoding m_enc;
	WXMEncodingType m_type;
	ssize_t m_idx;

	virtual void Create(ssize_t idx);

public:
	static WXMEncoding* CreateWxmEncoding(ssize_t idx);
	static WXMEncoding* CreateWxmEncoding(wxFontEncoding enc);
	static WXMEncoding* CreateWxmEncoding(const wxString &name);

protected:
	WXMEncoding(): m_idx(-1)
	{
	}
	virtual ~WXMEncoding()
	{
	}

public:
	// return the converted length of buf
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) = 0;

	virtual ucs4_t MultiBytetoUCS4(wxByte* buf)
	{
		return 0xFFFD;
	}

	virtual bool IsLeadByte(wxByte byte)
	{
		return false;
	}

	wxString GetName() { return m_name; }
	wxString GetDescription() { return m_desc; }
	WXMEncodingType GetType() { return m_type; }
	wxString GetFontName() { return m_fontname; }
	wxFontEncoding GetEncoding() { return m_enc; }
};

struct WXMEncodingMultiByte: public WXMEncoding
{
    virtual void MultiByteInit() = 0;
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf) = 0;
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual void Create(ssize_t idx);

protected:
    wxCSConv        *m_CSConv;
    ucs2_t          *m_MBtoWC_Table;    // MultiByte To WideChar table
    wxWord          *m_WCtoMB_Table;    // WideChar To MultiByte table
    wxByte          *m_LeadByte_Table;  // DBCS Lead-Byte table, 0:unset, 1:IsLeadByte, 0xFF:NotLeadByte

	WXMEncodingMultiByte(): m_CSConv(NULL), m_MBtoWC_Table(NULL), m_WCtoMB_Table(NULL), m_LeadByte_Table(NULL)
	{
	}
	~WXMEncodingMultiByte()
	{
		delete m_CSConv; m_CSConv = NULL;
		delete m_MBtoWC_Table; m_MBtoWC_Table = NULL;
		delete m_WCtoMB_Table; m_WCtoMB_Table = NULL;
		delete m_LeadByte_Table; m_LeadByte_Table = NULL;
	}
};

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingSingleByte(){}
	~WXMEncodingSingleByte(){}
};

struct WXMEncodingDoubleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual bool IsLeadByte(wxByte byte);
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingDoubleByte(){}
	~WXMEncodingDoubleByte(){}
};

struct WXMEncodingUTF8: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF8(){}
	~WXMEncodingUTF8(){}
};

struct WXMEncodingUTF16LE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16LE(){}
	~WXMEncodingUTF16LE(){}
};

struct WXMEncodingUTF16BE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16BE(){}
	~WXMEncodingUTF16BE(){}
};

struct WXMEncodingUTF32LE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32LE(){}
	~WXMEncodingUTF32LE(){}
};

struct WXMEncodingUTF32BE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncoding::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32BE(){}
	~WXMEncodingUTF32BE(){}
};

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf);

};// namespace wxm

#endif
