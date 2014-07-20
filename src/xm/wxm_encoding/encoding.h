///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/wxm_encoding/encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_H_
#define _WXM_ENCODING_H_

#include "encoding_def.h"
#include "../wxm_line_enc_adapter.h"
#include "../../wxmedit/ucs4_t.h"

#ifdef WX_PRECOMP
#include <wx/wxprec.h>
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/string.h>
#endif

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace wxm
{

struct WXMEncoding;
struct WXMEncodingManager: private boost::noncopyable
{
	static WXMEncodingManager& Instance()
	{
		if (s_inst == NULL)
			s_inst = new WXMEncodingManager();
		return *s_inst;
	}
	static void DestroyInstance()
	{
		delete s_inst;
		s_inst = NULL;
	}

	static bool IsSimpleUnicodeEncoding(WXMEncodingID enc)
	{
		if( enc==ENC_UTF_8 || enc==ENC_UTF_16LE || enc==ENC_UTF_16BE ||
			enc==ENC_UTF_32LE || enc==ENC_UTF_32BE)
		{
			return true;
		}

		return false;
	}

	void InitEncodings()
	{
		if (!m_initialized)
			DoInit();
		m_initialized = true;
	}
	void FreeEncodings();

	WXMEncoding* GetWxmEncoding(ssize_t idx);
	WXMEncoding* GetWxmEncoding(WXMEncodingID enc);
	WXMEncoding* GetWxmEncoding(const wxString &name);

private:
	enum WXMEncodingType
	{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE, etCP20932, etGB18030, etCP437ART };

	static WXMEncodingManager* s_inst;

public:
	size_t GetEncodingsCount();
	wxString GetEncodingName(ssize_t idx);
	std::string GetEncodingInnerName(ssize_t idx);
	wxString GetEncodingDescription(ssize_t idx);
	wxString GetEncodingFontName(ssize_t idx);
	wxString EncodingToName(WXMEncodingID enc);
	WXMEncodingID NameToEncoding(const wxString &name);
	WXMEncodingID ExtNameToEncoding(const std::string &name);
	WXMEncoding* GetSystemEncoding();

	WXMEncodingType GetIdxEncType(ssize_t idx);

	std::vector<WXMEncodingGroupID> GetEncodingGroups(ssize_t idx);
	wxString EncodingGroupToName(WXMEncodingGroupID gid);
private:

	WXMEncodingID IdxToEncoding(ssize_t idx)
	{
		return NameToEncoding(GetEncodingName(idx));
	}

	void InitSystemEncoding();
	void InitEncodingGroups();
	WXMEncodingID GetSystemEncodingID();

	void DoInit();
	void AddEncoding(const std::string& encname, WXMEncodingID encid
		, const wxString& desc=wxString(), WXMEncodingType entype=etSingleByte
		, const std::string& innername0=std::string(), bool exact=true);

	WXMEncodingManager()
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
	typedef std::map<WXMEncodingID, std::string> EncInnerNameMap;
	typedef std::map<std::string, WXMEncodingID> ICUNameEncMap;
	typedef std::map<WXMEncodingGroupID, wxString> WXEncGrpNameMap;
	typedef std::map<WXMEncodingID, std::set<WXMEncodingGroupID> > WXEncGrpsMap;

	WXNameEncMap m_wxnameenc_map;
	WXEncNameMap m_wxencname_map;
	WXEncTypeMap m_wxenctype_map;
	WXEncFontMap m_wxencfont_map;
	WXEncDescMap m_wxencdesc_map;
	WXEncGrpNameMap m_wxencgrpname_map;
	WXEncGrpsMap m_wxencgrps_map;
	EncInnerNameMap m_encinnername_map;
	ICUNameEncMap m_icunameenc_map;

	typedef std::map<ssize_t, WXMEncoding*> EncInstMap;
	EncInstMap m_inst_map;
};

struct WXMBlockDumper;


struct WXMEncodingDecoder: private boost::noncopyable
{
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len) = 0;
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len) = 0;
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len) = 0;

	virtual ~WXMEncodingDecoder(){}
};

struct WXMEncodingDecoderISO646: virtual public WXMEncodingDecoder
{
	virtual bool IsUChar32_LineFeed(const wxByte* buf, size_t len);
	virtual bool IsUChar32_LineFeed(WXMBlockDumper& dumper, size_t len);
	virtual ucs4_t PeekUChar32_Newline(WXMBlockDumper& dumper, size_t len);
};

struct WXMEncoding: virtual public WXMEncodingDecoder
{
	enum SpecialValueType
	{
		svtInvaliad       = 0,
		svtNotCached      = 0xFF, 
		svtDByteNotCached = 0xFFFF,
		svtQByteNotCached = 0xFFFFFFFF,
		svtUCS4NotCached  = 0xFFFFFFFF,
	};

protected:
	wxString m_name;
	std::string m_innername;
	wxString m_desc;
	wxString m_fontname;
	WXMEncodingID m_enc;
	ssize_t m_idx;
	bool m_simp_unicode;

	virtual void Create(ssize_t idx);

protected:
	friend WXMEncoding* WXMEncodingManager::GetWxmEncoding(ssize_t idx);
	friend void WXMEncodingManager::FreeEncodings();
	WXMEncoding(): m_idx(-1), m_simp_unicode(false)
	{
	}
	virtual ~WXMEncoding()
	{
	}

public:
	// return the converted length of buf
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) = 0;
	virtual bool NextUChar32(MadUCQueue &ucqueue, UChar32BytesMapper& mapper) = 0;

	virtual ucs4_t MultiBytetoUCS4(const wxByte* buf)
	{
		return (ucs4_t)svtInvaliad;
	}

	virtual bool IsLeadByte(wxByte byte)
	{
		return false;
	}

	virtual bool IsUnicodeEncoding()
	{
		return m_simp_unicode;
	}

	virtual bool IsSingleByteEncoding()
	{
		return false;
	}

	virtual bool IsDoubleByteEncoding()
	{
		return false;
	}

	wxString GetName() { return m_name; }
	wxString GetDescription() { return m_desc; }
	wxString GetFontName() { return m_fontname; }
	WXMEncodingID GetEncoding() { return m_enc; }
};

};// namespace wxm

#endif // _WXM_ENCODING_H_
