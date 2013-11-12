///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/wxm_encdet.cpp
// Description: wxMEdit encoding detector
// Author:      wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encdet.h"
#include "../wxmedit/mad_encdet.h"
#include "wxm_encoding/encoding.h"

#include <unicode/ucsdet.h>
#include <unicode/uversion.h>
#include <boost/foreach.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/range/iterator_range.hpp>
#include <string>

struct EncodingChecker
{
	virtual bool MatchText(const wxByte *text, size_t len) const = 0;
	virtual std::string BOM() const = 0;
	virtual std::string EncodingName() const = 0;

	virtual ~EncodingChecker(){}
};

struct UTF32Checker: public EncodingChecker
{
	virtual bool MatchText(const wxByte *text, size_t len) const
	{
		if (len < 4)
			return false;

		ucs4_t *u32text = (ucs4_t *)text;

		size_t count = len / 4;
		for(size_t i=0; i<count; i++)
		{
			ucs4_t ucs4 = QByteToInt(u32text[i]);
			if(ucs4<=0 || ucs4>0x10FFFF)
				return false;
		}

		return true;
	}

	virtual ~UTF32Checker(){}

private:
	virtual ucs4_t QByteToInt(ucs4_t u) const = 0;
};

class UTF32LEChecker: public UTF32Checker
{
	virtual ucs4_t QByteToInt(ucs4_t u) const
	{
		return wxINT32_SWAP_ON_BE(u);
	}

	virtual std::string EncodingName() const
	{
		return "UTF-32LE";
	}

	virtual std::string BOM() const
	{
		static const char bom[] = {'\xFF', '\xFE', '\0', '\0'};
		return std::string(bom, 4);
	}
};

class UTF32BEChecker: public UTF32Checker
{
	virtual ucs4_t QByteToInt(ucs4_t u) const
	{
		return wxINT32_SWAP_ON_LE(u);
	}

	virtual std::string EncodingName() const
	{
		return "UTF-32BE";
	}

	virtual std::string BOM() const
	{
		static const char bom[] = {'\0', '\0', '\xFE', '\xFF'};
		return std::string(bom, 4);
	}
};


struct UTF16Checker: public EncodingChecker
{
	virtual bool MatchText(const wxByte *text, size_t len) const
	{
		bool ok = false;
		bool highsurrogate = false;

		wxUint16* u16text=(wxUint16*)text;
		size_t count = len / 2;
		for(size_t i=0; i<count; ++i)
		{
			wxUint16 u = DByteToUInt(u16text[i]);
			if(u == 0)
				return false;

			if(u >= 0xD800 && u <= 0xDB00)
			{
				if(highsurrogate)
					return false;

				highsurrogate = true;
				continue;
			}

			if(u >= 0xDC00 && u <= 0xDF00)
			{
				if(!highsurrogate)
					return false;

				ok = true;
			}
			else if((u & 0xFF00) == 0)
			{
				ok = true;
			}

			highsurrogate = false;
		}

		return ok;
	}

	virtual ~UTF16Checker(){}

private:
	virtual wxUint16 DByteToUInt(wxUint16 u) const = 0;
};

class UTF16LEChecker: public UTF16Checker
{
	virtual wxUint16 DByteToUInt(wxUint16 u) const
	{
		return wxINT16_SWAP_ON_BE(u);
	}

	virtual std::string EncodingName() const
	{
		return "UTF-16LE";
	}

	virtual std::string BOM() const
	{
		return "\xFF\xFE";
	}
};

class UTF16BEChecker: public UTF16Checker
{
	virtual wxUint16 DByteToUInt(wxUint16 u) const
	{
		return wxINT16_SWAP_ON_LE(u);
	}

	virtual std::string EncodingName() const
	{
		return "UTF-16BE";
	}

	virtual std::string BOM() const
	{
		return "\xFE\xFF";
	}
};


struct UTF8BytesChecker
{
	bool Check(const wxByte* str, size_t len, bool& nonascii) const
	{
		if (len < this->TakenBytes())
			return false;

		nonascii = nonascii || NonASCII();
		return this->BytesMatch(str);
	}

	virtual bool NonASCII() const { return true; }
	virtual bool BytesMatch(const wxByte* bytes) const= 0;
	virtual size_t TakenBytes() const = 0;

	virtual ~UTF8BytesChecker() {}
};

struct SingleByteChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const wxByte* bytes) const { return true; }
	virtual bool NonASCII() const { return false; }

	virtual size_t TakenBytes() const { return 1; }
};

struct TwoBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xC0)==0x80;
	}
	virtual size_t TakenBytes() const { return 2; }
};

struct ThreeBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xC0)==0x80 && (b[2] & 0xC0)==0x80;
	}

	virtual size_t TakenBytes() const { return 3; }
};

// first byte is 0xE0, second byte should >=0xA0
struct ThreeBytesCheckerLB: public ThreeBytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xE0)==0xA0 && (b[2] & 0xC0)==0x80;
	}

};

// first byte is 0xED, second byte should < 0xA0
// skip surrogates
struct ThreeBytesCheckerM: public ThreeBytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xA0)==0x80 && (b[2] & 0xC0)==0x80;
	}
};

struct FourBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xC0)==0x80 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

	virtual size_t TakenBytes() const { return 4; }
};

// first byte is 0xF0, second byte should >=0x90
struct FourBytesCheckerLB: public FourBytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xF0)>=0x90 && (b[1] & 0xF0)<=0xb0 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

};

// first byte is 0xF4, second byte should <=0x8F
struct FourBytesCheckerUB: public FourBytesChecker
{
	virtual bool BytesMatch(const wxByte* b) const
	{
		return (b[1] & 0xF0)==0x80 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

};

struct InvalidBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const wxByte* bytes) const { return false; }
	virtual size_t TakenBytes() const { return 1; }
};

struct UTF8Checker: public EncodingChecker
{
	UTF8Checker()
	{
		// 0x00-0x7F b1
		// 0x80-0xBF bx-2 bx-3 bx-4
		// 0xC0-0xC1 err
		// 0xC2-0xDF b2-1
		// 0xE0-0xEF b3-1
		// 0xF0-0xF7 b4-1
		m_decoders[0x7F] = &m_dec1;
		m_decoders[0xC1] = &m_dec_invalid;
		m_decoders[0xDF] = &m_dec2;
		m_decoders[0xE0] = &m_dec3lb;
		m_decoders[0xEC] = &m_dec3;
		m_decoders[0xED] = &m_dec3m;
		m_decoders[0xEF] = &m_dec3;
		m_decoders[0xF0] = &m_dec4lb;
		m_decoders[0xF3] = &m_dec4;
		m_decoders[0xF4] = &m_dec4ub;
		m_decoders[0xFF] = &m_dec_invalid;
	}

	virtual std::string BOM() const
	{
		return "\xEF\xBB\xBF";
	}

	virtual bool MatchText(const wxByte* str, size_t len) const
	{
		bool nonascii = false;
		while(len > 0)
		{
			CheckerMap::const_iterator it = m_decoders.lower_bound(str[0]);
			const UTF8BytesChecker* checker = it->second;
			if (!checker->Check(str, len, nonascii))
				return false;

			size_t bytes = checker->TakenBytes();
			len -= bytes;
			str += bytes;
		}

		return nonascii;
	}

	virtual std::string EncodingName() const
	{
		return "UTF-8";
	}

private:

	typedef std::map<wxByte, UTF8BytesChecker*> CheckerMap;
	CheckerMap m_decoders;

	SingleByteChecker   m_dec1;
	TwoBytesChecker     m_dec2;
	ThreeBytesCheckerLB m_dec3lb;
	ThreeBytesChecker   m_dec3;
	ThreeBytesCheckerM  m_dec3m;
	FourBytesCheckerLB  m_dec4lb;
	FourBytesChecker    m_dec4;
	FourBytesCheckerUB  m_dec4ub;
	InvalidBytesChecker m_dec_invalid;
};

bool IsUTF8(const wxByte *text, size_t len)
{
	static const UTF8Checker checker;
	return checker.MatchText(text, len);
}

struct GB18030Checker: public EncodingChecker
{
	virtual std::string BOM() const
	{
		return "\x84\x31\x95\x33";
	}

	virtual bool MatchText(const wxByte* text, size_t len) const
	{
		return false;
	}

	virtual std::string EncodingName() const
	{
		return "GB18030";
	}
};

struct ISO646Checker: public EncodingChecker
{
	virtual std::string BOM() const
	{
		return "";
	}

	virtual bool MatchText(const wxByte* text, size_t len) const
	{
		BOOST_FOREACH(wxByte b, boost::make_iterator_range(text, text+len))
		{
			if (b==0x00 || b >= 0x80)
				return false;
		}
		return true;
	}

	virtual std::string EncodingName() const
	{
		return "US-ASCII";
	}
};

struct BOMIterationPrior
{
	bool operator()(const std::string& s1, const std::string s2) const
	{
		return s1 > s2;
	}
};

struct WXMEncodingDetector
{
	bool MatchBOM(const std::string bom, const wxByte *text, size_t len) const
	{
		if (bom.empty() || len < bom.size())
			return false;

		return bom == std::string((const char*)text, bom.size());
	}

	std::string DetectEncoding(const wxByte *text, size_t len) const
	{
		BOOST_FOREACH(BOMEncMap::value_type bom_enc, m_bom_enc_map)
		{
			if (MatchBOM(bom_enc.first, text, len))
				return bom_enc.second;
		}

		BOOST_FOREACH(const EncodingChecker* checker, m_checkers)
		{
			if (checker->MatchText(text, len))
				return checker->EncodingName();
		}

		return std::string();
	}

	WXMEncodingDetector()
	{
		boost::assign::push_back(m_checkers)
			(new UTF16LEChecker)
			(new UTF16BEChecker)
			(new UTF8Checker)
			(new UTF32LEChecker)
			(new UTF32BEChecker)
			(new GB18030Checker)
			(new ISO646Checker)
			;

		BOOST_FOREACH(const EncodingChecker* checker, m_checkers)
		{
			m_bom_enc_map[checker->BOM()] = checker->EncodingName();
		}
	}

	~WXMEncodingDetector()
	{
		BOOST_FOREACH(EncodingChecker* checker, m_checkers)
			delete checker;
	}

private:
	std::vector<EncodingChecker*> m_checkers;

	typedef std::map<std::string, std::string, BOMIterationPrior> BOMEncMap;
	BOMEncMap m_bom_enc_map;
};

bool MatchWXMEncoding(wxString& enc, const wxByte *text, size_t len)
{
	static const WXMEncodingDetector det;

	std::string detenc = det.DetectEncoding(text, len);
	if (detenc.empty())
		return false;

	enc = wxString(detenc.c_str(), wxConvUTF8);
	return true;
}

bool MatchEUCJPMoreThanGB18030(const wxByte *text, size_t len)
{
	size_t i=0;
	size_t eucjp = 0;
	size_t other = 0;
	while(i < len - 1)
	{
		wxByte b0 = text[i];
		wxByte b1 = text[i+1];
		// hiragana & katakana (encoded the same in EUC-JP and GB2312/GBK/GB18030)
		if ((b0==0xA4 && b1>=0xA1 && b1<=0xF4) ||
		    (b0==0xA5 && b1>=0xA1 && b1<=0xF6))
		{
			++eucjp;
			++i;
		}
		else if(b0 >= 0x80)
		{
			++other;
			++i;
		}

		++i;
	}

	size_t nonascii = eucjp + other;
	return (nonascii > 0) && (100*eucjp/nonascii > 50);
}

#if U_ICU_VERSION_MAJOR_NUM *10 + U_ICU_VERSION_MINOR_NUM < 44
struct LocalUCharsetDetectorPointer
{
	LocalUCharsetDetectorPointer(UCharsetDetector* ptr)
		: m_ptr(ptr)
	{
	}
	~LocalUCharsetDetectorPointer()
	{
		if (m_ptr!=NULL)
			ucsdet_close(m_ptr);
		m_ptr = NULL;
	}
	UCharsetDetector* getAlias()
	{
		return m_ptr;
	}

private:
	UCharsetDetector* m_ptr;
};
#endif

void DetectEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc, bool skip_utf8)
{
	UErrorCode status = U_ZERO_ERROR;
	LocalUCharsetDetectorPointer csd(ucsdet_open(&status));
	ucsdet_setText(csd.getAlias(), (const char*)text, len, &status);
	int32_t match_count = 0;
	const UCharsetMatch **matches = ucsdet_detectAll(csd.getAlias(), &match_count, &status);
	std::string enc_name(ucsdet_getName(matches[0], &status));

	if (match_count>1 && enc_name=="GB18030")
	{
		// check if EUC-JP was detected as GB18030
		std::string enc2nd(ucsdet_getName(matches[1], &status));
		if (enc2nd=="EUC-JP" &&
		    ucsdet_getConfidence(matches[0], &status) == ucsdet_getConfidence(matches[1], &status) &&
		    MatchEUCJPMoreThanGB18030(text, len))
		{
			enc_name = "EUC-JP";
		}
	}
	else if (skip_utf8 && enc_name=="UTF-8")
	{
		if (match_count>1)
			enc_name = ucsdet_getName(matches[1], &status);
		else
			enc_name.clear();
	}

	if(enc_name == "EUC-KR")
		enc_name = "UHC";
	else if(enc_name == "EUC-JP")
		enc_name = "CP20932";

	wxm::WXMEncodingID init_enc = enc;
	enc = wxm::WXMEncodingCreator::Instance().ExtNameToEncoding(enc_name);

	if(enc == wxm::ENC_Windows_1252 && (init_enc==wxm::ENC_MS950 || init_enc==wxm::ENC_MS936))
	{
		wxm::WXMEncodingID det=wxm::ENC_DEFAULT;
		DetectChineseEncoding(text, len, det);
		if(det != wxm::ENC_DEFAULT)
			enc = det;
	}
}
