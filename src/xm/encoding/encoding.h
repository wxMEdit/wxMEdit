///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/encoding/encoding.h
// Description: Define the Encodings Supported by wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_ENCODING_H_
#define _XM_ENCODING_H_

#include "../cxx11.h"
#include "encoding_def.h"
#include "../line_enc_adapter.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace xm
{

struct Encoding;
struct EncodingManager: private boost::noncopyable
{
	static void PreInit();
	static EncodingManager& Instance()
	{
		PreInit();
		if (s_inst == nullptr)
			s_inst = new EncodingManager();
		return *s_inst;
	}
	static void DestroyInstance()
	{
		delete s_inst;
		s_inst = nullptr;
	}

	static bool IsSimpleUnicodeEncoding(EncodingID enc)
	{
		if( enc==ENC_UTF_8 || enc==ENC_UTF_16LE || enc==ENC_UTF_16BE ||
			enc==ENC_UTF_32LE || enc==ENC_UTF_32BE)
		{
			return true;
		}

		return false;
	}

	static std::wstring ExtractEncodingName(const std::wstring& name_with_aliases)
	{
		return name_with_aliases.substr(0, name_with_aliases.find(L' '));
	}

	void InitEncodings()
	{
		if (!m_initialized)
			DoInit();
		m_initialized = true;
	}
	void FreeEncodings();

	Encoding* GetEncoding(ssize_t idx);
	Encoding* GetEncoding(EncodingID enc);
	Encoding* GetEncoding(const std::wstring& name);

private:
	enum EncodingType
	{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE, etCP20932, etGB18030, etCP437ART };

	static EncodingManager* s_inst;
	static std::string s_sysenc_icuname;

public:
	size_t GetEncodingsCount();
	std::wstring GetEncodingName(ssize_t idx);
	std::string GetEncodingInnerName(ssize_t idx);
	std::wstring GetEncodingDescription(ssize_t idx);
	std::wstring GetEncodingNameWithAliases(ssize_t idx) { return GetEncodingName(idx) + GetEncodingAliases(idx); }
	std::wstring GetEncodingFontName(ssize_t idx);
	std::wstring EncodingToName(EncodingID enc);
	EncodingID NameToEncoding(const std::wstring& name);
	EncodingID ExtNameToEncoding(const std::string& name);
	Encoding* GetSystemEncoding();
	std::wstring ExpandEncodingAliases(const std::wstring& name);

	EncodingType GetIdxEncType(ssize_t idx);

	std::vector<EncodingGroupID> GetEncodingGroups(ssize_t idx);
	std::wstring EncodingGroupToName(EncodingGroupID gid);
private:
	std::wstring GetEncodingAliases(ssize_t idx);

	EncodingID IdxToEncoding(ssize_t idx)
	{
		return NameToEncoding(GetEncodingName(idx));
	}

	void InitSystemEncoding();
	void InitEncodingGroups();
	EncodingID GetSystemEncodingID();

	void DoInit();
	void AddEncoding(const std::string& encname, EncodingID encid,
	                 const std::wstring& desc= std::wstring(), const std::wstring& aliases= std::wstring(),
	                 EncodingType entype=etSingleByte, const std::string& innername0=std::string(),
	                 bool exact=true);

	EncodingManager()
	: m_initialized(false), m_sysenc_idx(-1), m_sysenc(nullptr)
	{
		DoInit();
		m_initialized = true;
	}

	bool m_initialized;
	ssize_t m_sysenc_idx;
	Encoding *m_sysenc;

	typedef std::map<std::string, EncodingID> EncMap;
	EncMap m_enc_map;

	std::vector<std::wstring> m_enc_list;

	typedef std::map<std::wstring, EncodingID> NameEncMap;
	typedef std::map<EncodingID, std::wstring> EncNameMap;
	typedef std::map<EncodingID, EncodingType> EncTypeMap;
	typedef std::map<EncodingID, std::wstring> EncFontMap;
	typedef std::map<EncodingID, std::wstring> EncDescMap;
	typedef std::map<EncodingID, std::wstring> EncAliasesMap;
	typedef std::map<EncodingID, std::string> EncInnerNameMap;
	typedef std::map<std::string, EncodingID> ICUNameEncMap;
	typedef std::map<EncodingGroupID, std::wstring> EncGrpNameMap;
	typedef std::map<EncodingID, std::set<EncodingGroupID> > EncGrpsMap;

	NameEncMap m_nameenc_map;
	EncNameMap m_encname_map;
	EncTypeMap m_enctype_map;
	EncFontMap m_wxencfont_map;
	EncDescMap m_encdesc_map;
	EncAliasesMap m_encaliases_map;
	EncGrpNameMap m_encgrpname_map;
	EncGrpsMap m_encgrps_map;
	EncInnerNameMap m_encinnername_map;
	ICUNameEncMap m_icunameenc_map;

	typedef std::map<ssize_t, Encoding*> EncInstMap;
	EncInstMap m_inst_map;
};

struct BlockDumper;


struct EncodingDecoder: private boost::noncopyable
{
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) = 0;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) = 0;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) = 0;

	virtual ~EncodingDecoder(){}
};

struct EncodingDecoderISO646: virtual public EncodingDecoder
{
	virtual bool IsUChar32_LineFeed(const ubyte* buf, size_t len) override;
	virtual bool IsUChar32_LineFeed(BlockDumper& dumper, size_t len) override;
	virtual ucs4_t PeekUChar32_Newline(BlockDumper& dumper, size_t len) override;
};

struct Encoding: virtual public EncodingDecoder
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
	std::wstring m_name;
	std::string m_innername;
	std::wstring m_desc;
	std::wstring m_name_with_aliases;
	std::wstring m_fontname;
	EncodingID m_enc;
	ssize_t m_idx;
	bool m_simp_unicode;

	virtual void Create(ssize_t idx);

protected:
	friend Encoding* EncodingManager::GetEncoding(ssize_t idx);
	friend void EncodingManager::FreeEncodings();
	Encoding(): m_idx(-1), m_simp_unicode(false)
	{
	}
	virtual ~Encoding()
	{
	}

public:
	// return the converted length of buf
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, ubyte* buf) = 0;
	virtual bool NextUChar32(UCQueue &ucqueue, UChar32BytesMapper& mapper) = 0;

	virtual ucs4_t MultiBytetoUCS4(const ubyte* buf)
	{
		return (ucs4_t)svtInvaliad;
	}

	virtual bool IsLeadByte(ubyte byte)
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

	std::wstring GetName() { return m_name; }
	std::wstring GetDescription() { return m_desc; }
	std::wstring GetNameWithAliases() { return m_name_with_aliases; }
	std::wstring GetFontName() { return m_fontname; }
	EncodingID GetEncoding() { return m_enc; }
};

};// namespace xm

#endif // _XM_ENCODING_H_
