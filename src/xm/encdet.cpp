///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4
// Name:        xm/encdet.cpp
// Description: wxMEdit Encoding Detector
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "encdet.h"
#include "../xm/cxx11.h"
#include "../xm/uutils.h"
#include "encoding/encoding.h"
#include "mad_encdet.h"

#include <unicode/ucsdet.h>
#include <unicode/uversion.h>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <string>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

struct EncodingChecker
{
	virtual bool MatchText(const ubyte* text, size_t len) const = 0;
	virtual std::string BOM() const = 0;
	virtual std::string EncodingName() const = 0;

	virtual ~EncodingChecker(){}
};

struct UTF32Checker: public EncodingChecker
{
	virtual bool MatchText(const ubyte* text, size_t len) const override
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
	virtual uint32_t QByteToInt(uint32_t u) const = 0;
};

class UTF32LEChecker: public UTF32Checker
{
	virtual uint32_t QByteToInt(uint32_t u) const override
	{
		return ToLE(u);
	}

	virtual std::string EncodingName() const override
	{
		return "UTF-32LE";
	}

	virtual std::string BOM() const override
	{
		static const char bom[] = {'\xFF', '\xFE', '\0', '\0'};
		return std::string(bom, 4);
	}
};

class UTF32BEChecker: public UTF32Checker
{
	virtual uint32_t QByteToInt(uint32_t u) const override
	{
		return ToBE(u);
	}

	virtual std::string EncodingName() const override
	{
		return "UTF-32BE";
	}

	virtual std::string BOM() const override
	{
		static const char bom[] = {'\0', '\0', '\xFE', '\xFF'};
		return std::string(bom, 4);
	}
};


struct UTF16Checker: public EncodingChecker
{
	virtual bool MatchText(const ubyte* text, size_t len) const override
	{
		bool ok = false;
		bool highsurrogate = false;

		uint16_t* u16text=(uint16_t*)text;
		size_t count = len / 2;
		for(size_t i=0; i<count; ++i)
		{
			uint16_t u = DByteToUInt(u16text[i]);
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
	virtual uint16_t DByteToUInt(uint16_t u) const = 0;
};

class UTF16LEChecker: public UTF16Checker
{
	virtual uint16_t DByteToUInt(uint16_t u) const override
	{
		return ToLE(u);
	}

	virtual std::string EncodingName() const override
	{
		return "UTF-16LE";
	}

	virtual std::string BOM() const override
	{
		return "\xFF\xFE";
	}
};

class UTF16BEChecker: public UTF16Checker
{
	virtual uint16_t DByteToUInt(uint16_t u) const override
	{
		return ToBE(u);
	}

	virtual std::string EncodingName() const override
	{
		return "UTF-16BE";
	}

	virtual std::string BOM() const override
	{
		return "\xFE\xFF";
	}
};


struct UTF8BytesChecker
{
	bool Check(const ubyte* str, size_t len, bool& nonascii) const
	{
		if (len < this->TakenBytes())
			return false;

		nonascii = nonascii || NonASCII();
		return this->BytesMatch(str);
	}

	virtual bool NonASCII() const { return true; }
	virtual bool BytesMatch(const ubyte* bytes) const= 0;
	virtual size_t TakenBytes() const = 0;

	virtual ~UTF8BytesChecker() {}
};

struct UTF8SingleByteChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const ubyte* bytes) const override { return true; }
	virtual bool NonASCII() const override { return false; }

	virtual size_t TakenBytes() const override { return 1; }
};

struct UTF8TwoBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xC0)==0x80;
	}
	virtual size_t TakenBytes() const override { return 2; }
};

struct UTF8ThreeBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xC0)==0x80 && (b[2] & 0xC0)==0x80;
	}

	virtual size_t TakenBytes() const override { return 3; }
};

// first byte is 0xE0, second byte should >=0xA0
struct UTF8ThreeBytesCheckerLB: public UTF8ThreeBytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xE0)==0xA0 && (b[2] & 0xC0)==0x80;
	}

};

// first byte is 0xED, second byte should < 0xA0
// skip surrogates
struct UTF8ThreeBytesCheckerM: public UTF8ThreeBytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xA0)==0x80 && (b[2] & 0xC0)==0x80;
	}
};

struct UTF8FourBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xC0)==0x80 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

	virtual size_t TakenBytes() const override { return 4; }
};

// first byte is 0xF0, second byte should >=0x90
struct UTF8FourBytesCheckerLB: public UTF8FourBytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xF0)>=0x90 && (b[1] & 0xF0)<=0xb0 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

};

// first byte is 0xF4, second byte should <=0x8F
struct UTF8FourBytesCheckerUB: public UTF8FourBytesChecker
{
	virtual bool BytesMatch(const ubyte* b) const override
	{
		return (b[1] & 0xF0)==0x80 && (b[2] & 0xC0)==0x80 && (b[3] & 0xC0)==0x80;
	}

};

struct UTF8InvalidBytesChecker: public UTF8BytesChecker
{
	virtual bool BytesMatch(const ubyte* bytes) const override { return false; }
	virtual size_t TakenBytes() const override { return 1; }
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

	virtual std::string BOM() const override
	{
		return "\xEF\xBB\xBF";
	}

	virtual bool MatchText(const ubyte* str, size_t len) const override
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

	virtual std::string EncodingName() const override
	{
		return "UTF-8";
	}

private:

	typedef std::map<ubyte, UTF8BytesChecker*> CheckerMap;
	CheckerMap m_decoders;

	UTF8SingleByteChecker   m_dec1;
	UTF8TwoBytesChecker     m_dec2;
	UTF8ThreeBytesCheckerLB m_dec3lb;
	UTF8ThreeBytesChecker   m_dec3;
	UTF8ThreeBytesCheckerM  m_dec3m;
	UTF8FourBytesCheckerLB  m_dec4lb;
	UTF8FourBytesChecker    m_dec4;
	UTF8FourBytesCheckerUB  m_dec4ub;
	UTF8InvalidBytesChecker m_dec_invalid;
};

bool IsUTF8(const ubyte* text, size_t len)
{
	static const UTF8Checker checker;
	return checker.MatchText(text, len);
}

struct GB18030Checker: public EncodingChecker
{
	virtual std::string BOM() const override
	{
		return "\x84\x31\x95\x33";
	}

	virtual bool MatchText(const ubyte* text, size_t len) const override
	{
		return false;
	}

	virtual std::string EncodingName() const override
	{
		return "GB18030";
	}
};

struct ISO646Checker: public EncodingChecker
{
	virtual std::string BOM() const override
	{
		return "";
	}

	virtual bool MatchText(const ubyte* text, size_t len) const override
	{
		BOOST_FOREACH(ubyte b, boost::make_iterator_range(text, text+len))
		{
			if (b==0x00 || b >= 0x80)
				return false;
		}
		return true;
	}

	virtual std::string EncodingName() const override
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

struct EncodingDetector
{
	bool MatchBOM(const std::string bom, const ubyte * text, size_t len) const
	{
		if (bom.empty() || len < bom.size())
			return false;

		return bom == std::string((const char*)text, bom.size());
	}

	std::string DetectEncoding(const ubyte* text, size_t len) const
	{
		BOOST_FOREACH(BOMEncMap::value_type bom_enc, m_bom_enc_map)
		{
			if (MatchBOM(bom_enc.first, text, len))
				return bom_enc.second;
		}

		BOOST_FOREACH(const boost::shared_ptr<EncodingChecker> checker, m_checkers)
		{
			if (checker->MatchText(text, len))
				return checker->EncodingName();
		}

		return std::string();
	}

	EncodingDetector()
	{
		boost::assign::push_back(m_checkers)
			(boost::shared_ptr<EncodingChecker>(new UTF16LEChecker))
			(boost::shared_ptr<EncodingChecker>(new UTF16BEChecker))
			(boost::shared_ptr<EncodingChecker>(new UTF8Checker))
			(boost::shared_ptr<EncodingChecker>(new UTF32LEChecker))
			(boost::shared_ptr<EncodingChecker>(new UTF32BEChecker))
			(boost::shared_ptr<EncodingChecker>(new GB18030Checker))
			(boost::shared_ptr<EncodingChecker>(new ISO646Checker))
			;

		BOOST_FOREACH(const boost::shared_ptr<EncodingChecker> checker, m_checkers)
		{
			m_bom_enc_map[checker->BOM()] = checker->EncodingName();
		}
	}

private:
	std::vector<boost::shared_ptr<EncodingChecker> > m_checkers;

	typedef std::map<std::string, std::string, BOMIterationPrior> BOMEncMap;
	BOMEncMap m_bom_enc_map;
};

bool MatchEncoding(std::string& enc, const ubyte* text, size_t len)
{
	static const EncodingDetector det;

	std::string detenc = det.DetectEncoding(text, len);
	if (detenc.empty())
		return false;

	enc = detenc;
	return true;
}

bool MatchEUCJPMoreThanGB18030(const ubyte* text, size_t len)
{
	size_t i=0;
	size_t eucjp = 0;
	size_t other = 0;
	while(i < len - 1)
	{
		ubyte b0 = text[i];
		ubyte b1 = text[i+1];
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

bool MatchMBMoreThanUTF16(const ubyte * text, size_t len)
{
	if (len < 2)
		return true;

	// for counting C0 bytes ( < 0x20)
	size_t byte_cnt[0x20] = { 0 };
	for (size_t i = 0; i < len; ++i)
	{
		if (text[i] >= 0x20)
			continue;

		++byte_cnt[text[i]];
	}

	// counting effetive C0 bytes
	const static std::vector<ubyte> exceptC0bytes( boost::assign::list_of('\t')('\r')('\n')('\v')('\f')('\x1B')
		.convert_to_container<std::vector<ubyte> >() );

	BOOST_FOREACH(ubyte b, exceptC0bytes)
		byte_cnt[size_t(b)] = 0;

	size_t eff_c0_cnt = 0;
	for (size_t i = 0; i<0x20; ++i)
		eff_c0_cnt += byte_cnt[i];

	// whether effetive C0 bytes accounted less than 1/100
	return eff_c0_cnt * 100 < len;
}

void DetectEncoding(const ubyte* text, size_t len, EncodingID& enc, bool skip_utf8)
{
	UErrorCode status = U_ZERO_ERROR;
	LocalUCharsetDetectorPointer csd(ucsdet_open(&status));
	ucsdet_setText(csd.getAlias(), (const char*)text, (int32_t)len, &status);
	int32_t match_count = 0;
	const UCharsetMatch **matches = ucsdet_detectAll(csd.getAlias(), &match_count, &status);
	std::string enc_name(ucsdet_getName(matches[0], &status));

#if U_ICU_VERSION_MAJOR_NUM >= 53
	if (match_count > 2 && boost::algorithm::starts_with(enc_name, "UTF-16") &&
		ucsdet_getConfidence(matches[0], &status) == ucsdet_getConfidence(matches[2], &status) &&
		MatchMBMoreThanUTF16(text, len))
	{
		enc_name = ucsdet_getName(matches[2], &status);
		matches += 2;
	}
#endif

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

	if (enc_name == "EUC-KR")
		enc_name = "UHC";
	else if (enc_name == "EUC-JP")
		enc_name = "CP20932";

	EncodingID init_enc = enc;
	enc = EncodingManager::Instance().ExtNameToEncoding(enc_name);

	if (enc == ENC_Windows_1252 && (init_enc==ENC_MS950 || init_enc==ENC_MS936))
	{
		EncodingID det=ENC_DEFAULT;
		DetectChineseEncoding(text, len, det);
		if(det != ENC_DEFAULT)
			enc = det;
	}
}

} // namespace xm
