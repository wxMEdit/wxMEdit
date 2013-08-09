///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_H_
#define _WXM_ENCODING_H_

#include "wxm_encoding_def.h"
#include "../wxmedit/ucs4_t.h"

#ifdef WX_PRECOMP
#include <wx/wxprec.h>
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/string.h>
#endif

#include <boost/utility.hpp>
#include <string>
#include <vector>
#include <map>

namespace wxm
{

enum WXMEncodingType
{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE, etEUCJPMS };

struct WXMEncoding;
struct WXMEncodingCreator: private boost::noncopyable
{
	static WXMEncodingCreator& Instance()
	{
		static WXMEncodingCreator creator;
		return creator;
	}

	void InitEncodings()
	{
		if (!m_initialized)
			DoInit();
		m_initialized = true;
	}
	void FreeEncodings();

	WXMEncoding* CreateWxmEncoding(ssize_t idx);
	WXMEncoding* CreateWxmEncoding(WXMEncodingID enc);
	WXMEncoding* CreateWxmEncoding(const wxString &name);

	size_t GetEncodingsCount();
	wxString GetEncodingName(ssize_t idx);
	std::string GetEncodingInnerName(ssize_t idx);
	wxString GetEncodingDescription(ssize_t idx);
	wxString GetEncodingFontName(ssize_t idx);
	wxString EncodingToName(WXMEncodingID enc);
	WXMEncodingID NameToEncoding(const wxString &name);
	WXMEncoding* GetSystemEncoding();

	WXMEncodingType GetIdxEncType(ssize_t idx);

private:
	WXMEncodingID IdxToEncoding(ssize_t idx)
	{
		return NameToEncoding(GetEncodingName(idx));
	}

	void InitSystemEncoding();
	WXMEncodingID GetSystemEncodingID();

	void DoInit();
	void AddEncoding(const std::string& encname, WXMEncodingID encid
		, const wxString& desc=wxString(), WXMEncodingType entype=etSingleByte
		, const std::string& innername0=std::string());

	WXMEncodingCreator()
	: m_initialized(false), m_sysenc_idx(-1), m_sysenc(NULL)
	{
		DoInit();
		m_initialized = true;
	}

	bool m_initialized;
	ssize_t m_sysenc_idx;
	WXMEncoding *m_sysenc;

	typedef std::map<std::string, WXMEncodingID> WXEncMap;
	WXEncMap m_wxenc_map;

	std::vector<wxString> m_wxenc_list;

	typedef std::map<wxString, WXMEncodingID> WXNameEncMap;
	typedef std::map<WXMEncodingID, wxString> WXEncNameMap;
	typedef std::map<WXMEncodingID, WXMEncodingType> WXEncTypeMap;
	typedef std::map<WXMEncodingID, wxString> WXEncFontMap;
	typedef std::map<WXMEncodingID, wxString> WXEncDescMap;
	typedef std::map<WXMEncodingID, std::string> WXEncInnerNameMap;
	WXNameEncMap m_wxnameenc_map;
	WXEncNameMap m_wxencname_map;
	WXEncTypeMap m_wxenctype_map;
	WXEncFontMap m_wxencfont_map;
	WXEncDescMap m_wxencdesc_map;
	WXEncInnerNameMap m_wxencinnername_map;

	typedef std::map<ssize_t, WXMEncoding*> WXEncInstMap;
	WXEncInstMap m_inst_map;
};

struct WXMEncoding: private boost::noncopyable
{
protected:
	wxString m_name;
	std::string m_innername;
	wxString m_desc;
	wxString m_fontname;
	WXMEncodingID m_enc;
	WXMEncodingType m_type;
	ssize_t m_idx;

	virtual void Create(ssize_t idx);

protected:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	friend void WXMEncodingCreator::FreeEncodings();
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
	WXMEncodingID GetEncoding() { return m_enc; }
};

};// namespace wxm

#endif // _WXM_ENCODING_H_
