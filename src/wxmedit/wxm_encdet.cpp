///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4 expandtab
// Name:        wxmedit/wxm_encdet.cpp
// Description: wxMEdit encoding detector
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encdet.h"
#include "../xm/wxm_encoding/encoding.h"

#include <unicode/ucsdet.h>
#include <unicode/uversion.h>
#include <string>

struct UTF32Checker
{
    bool IsUTF32(const wxByte *text, size_t len) const
    {
        if (len < 4)
            return false;

        ucs4_t *u32text = (ucs4_t *)text;

        // check UTF-32 BOM
        if(QByteToInt(u32text[0]) == 0x00FEFF)
            return true;

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
};

class UTF32BEChecker: public UTF32Checker
{
    virtual ucs4_t QByteToInt(ucs4_t u) const
    {
        return wxINT32_SWAP_ON_LE(u);
    }
};


bool IsUTF32LE(const wxByte *text, size_t len)
{
    static const UTF32LEChecker checker;

    return checker.IsUTF32(text, len);
}

bool IsUTF32BE(const wxByte *text, size_t len)
{
    static const UTF32BEChecker checker;

    return checker.IsUTF32(text, len);
}

struct UTF16Checker
{
    bool IsUTF16(const wxByte *text, size_t len) const
    {
        if(len < 2)
            return false;

        // UTF-32 BOM
        if(len >= 4)
        {
            ucs4_t* u32text = (ucs4_t *)text;
            if (wxINT32_SWAP_ON_BE(u32text[0]) == 0x00FEFF ||
                wxINT32_SWAP_ON_LE(u32text[0]) == 0x00FEFF)
            return false;
        }
        // reverse endian UTF-16 BOM
        wxUint16* u16text = (wxUint16 *)text;
        if(DByteToUInt(u16text[0]) == 0xFFFE)
            return false;

        // UTF-16 BOM
        if(DByteToUInt(u16text[0]) == 0xFEFF)
            return true;

        bool ok = false;
        bool highsurrogate = false;
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
};

class UTF16BEChecker: public UTF16Checker
{
    virtual wxUint16 DByteToUInt(wxUint16 u) const
    {
        return wxINT16_SWAP_ON_LE(u);
    }
};

bool IsUTF16LE(const wxByte *text, size_t len)
{
    static const UTF16LEChecker checker;
    return checker.IsUTF16(text, len);
}

bool IsUTF16BE(const wxByte *text, size_t len)
{
    static const UTF16BEChecker checker;
    return checker.IsUTF16(text, len);
}

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
private:
	bool m_worked;
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

struct UTF8Checker
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

	bool IsUTF8(const wxByte* str, size_t len) const
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
    //check BOM
    if(len >= 3 && text[0] == 0xEF && text[1] == 0xBB && text[2] == 0xBF)
        return true;

	static const UTF8Checker checker;

	return checker.IsUTF8(text, len);
}

bool MatchSimpleUnicode(wxString& enc, const wxByte *text, size_t len)
{
    if(IsUTF16LE(text, len))
    {
        enc = wxT("utf-16le");
        return true;
    }
    else if(IsUTF16BE(text, len))
    {
        enc = wxT("utf-16be");
        return true;
    }
    else if(IsUTF8(text, len))
    {
        enc = wxT("utf-8");
        return true;
    }
    else if(IsUTF32LE(text, len))
    {
        enc = wxT("utf-32le");
        return true;
    }
    else if(IsUTF32BE(text, len))
    {
        enc = wxT("utf-32be");
        return true;
    }

    return false;
}

bool IsBinaryData(const wxByte *data, size_t len)
{
    size_t i = 0;
    wxByte b;
    while(i < len)
    {
        b = data[i];
        if(b == 0)
            return true;
        i++;
    }
    return false;
}

void DetectChineseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc)
{
    // detect by punctuation marks
    size_t i=0;
    unsigned int b0=0, b1=0, c=0;
    size_t cp950=0;
    size_t cp936=0;

    while (i++ < len)
    {
        b0=b1;
        b1=c;
        c=*text++;

        if(c<0x40)
        {
            if(b0==0xa1)
            {
                switch(b1)
                {
                case 0x43:
                case 0x48:
                case 0x49:
                case 0x4b:
                case 0x58:
                case 0x5e:
                case 0x60:
                case 0x62:
                case 0x64:
                case 0x6a:
                case 0x6c:
                case 0x6e:
                case 0x70:
                case 0x72:
                case 0x76:
                case 0x7a:
                case 0xa2:
                case 0xa4:
                case 0xa6:
                case 0xa8:
                case 0xaa:
                case 0xac:
                    ++cp950;
                    break;
                case 0xa3:
                case 0xad:
                case 0xaf:
                case 0xb1:
                case 0xb5:
                case 0xb7:
                case 0xb9:
                case 0xbb:
                case 0xbf:
                case 0xe4:
                    ++cp936;
                    break;
                }
            }
            else if(b0==0xa3)
            {
                if(b1>=0x74 && b1<=0x7E)
                {
                    ++cp950;
                }
                else
                {
                    switch(b1)
                    {
                    case 0xa8:
                    case 0xaa:
                    case 0xab:
                    case 0xad:
                    case 0xaf:
                    case 0xbd:
                    case 0xbe:
                        ++cp950;
                        break;
                    case 0xa1:
                    case 0xa9:
                    case 0xac:
                    case 0xfd:
                        ++cp936;
                        break;
                    }
                }
            }
            else if(b1<=0xA0 && b1>=0x80)
            {
                if(b0>=0x81 && b0<=0xFE)
                {
                    enc = wxm::ENC_MS936; // [0x81~0xFE][0x80~0xA0] are invalid in big5
                    return;
                }
            }
            else
            {
                unsigned int w=(b0<<8) + b1;
                switch(w)
                {
                case 0xa6e1:
                case 0xa6e7:
                case 0xa6ef:
                case 0xa6f1:
                case 0xa895:
                case 0xa979:
                case 0xa97b:
                case 0xa9b4:
                case 0xa9bc:
                    ++cp936;
                    break;
                case 0xa27b:
                case 0xa27d:
                    ++cp950;
                    break;
                }
            }
        }
    }

    if(cp950>cp936) enc = wxm::ENC_MS950;
    else if(cp936>cp950) enc = wxm::ENC_MS936;
}

void DetectJapaneseEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc)
{
    wxByte c;
    size_t i=0;
    wxm::WXMEncodingID xenc= wxm::ENC_DEFAULT;

    while (xenc == 0 && i++ < len) {

        c = *text++;

        if (c >= 0x81 && c <= 0x9F) {
            if (c == 0x8E) /* SS2 */ {

                if(i++ >= len) break;

                c = *text++;
                if ((c >= 0x40 && c <= 0xA0) || (c >= 0xE0 && c <= 0xFC))
                    xenc = wxm::ENC_MS932;
            }
            else if (c == 0x8F) /* SS3 */ {

                if(i++ >= len) break;

                c = *text++;
                if (c >= 0x40 && c <= 0xA0)
                    xenc = wxm::ENC_MS932;
                else if (c >= 0xFD)
                    break;
            }
            else
                xenc = wxm::ENC_MS932;
        }
        else if (c >= 0xA1 && c <= 0xDF) {

            if(i++ >= len) break;

            c = *text++;
            if (c <= 0x9F)
                xenc = wxm::ENC_MS932;
            else if (c >= 0xFD)
                break;
        }
        else if (c >= 0xE0 && c <= 0xEF) {

            if(i++ >= len) break;

            c = *text++;
            if (c >= 0x40 && c <= 0xA0)
                xenc = wxm::ENC_MS932;
            else if (c >= 0xFD)
                break;
        }
        else if (c >= 0xF0)
            break;
    }

    if (xenc != 0)
        enc = xenc;
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
        if (b0==0xA4 && b1>=0xA1 && b1<=0xF4 ||
            b0==0xA5 && b1>=0xA1 && b1<=0xF6
            )
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

void DetectEncoding(const wxByte *text, size_t len, wxm::WXMEncodingID &enc)
{
    UErrorCode status = U_ZERO_ERROR;
    LocalUCharsetDetectorPointer csd(ucsdet_open(&status));
    ucsdet_setText(csd.getAlias(), (const char*)text, len, &status);
    int32_t match_count = 0;
    const UCharsetMatch **matches = ucsdet_detectAll(csd.getAlias(), &match_count, &status);
    std::string enc_name(ucsdet_getName(matches[0], &status));

    // check if EUC-JP was detected as GB18030
    if (match_count>1 && enc_name=="GB18030")
    {
        std::string enc2nd(ucsdet_getName(matches[1], &status));
        if (enc2nd=="EUC-JP" &&
            ucsdet_getConfidence(matches[0], &status) == ucsdet_getConfidence(matches[1], &status) &&
            MatchEUCJPMoreThanGB18030(text, len))
        {
            enc_name = "EUC-JP";
        }
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
