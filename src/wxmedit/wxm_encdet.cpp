///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_encdet.cpp
// Description: wxMEdit encoding detector
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "wxm_encdet.h"
#include "ucs4_t.h"
#include "chardetect.h"
#include <wx/config.h>

bool IsTextUTF32LE(wxByte *text, int size)
{
    // check BOM
    if(size>=4 && text[0]==0xFF && text[1]==0xFE && text[2]==0 && text[3]==0)
        return true;

    size>>=2;   // to count
    if(size<4) return false;

    ucs4_t ucs4, *p=(ucs4_t *)text;

    for(int i=0;i<size;i++, p++)
    {
        ucs4=wxINT32_SWAP_ON_BE(*p);

        if(ucs4<=0 || ucs4>0x10FFFF) return false;
    }

    return true;
}

bool IsTextUTF32BE(wxByte *text, int size)
{
    // check BOM
    if(size>=4 && text[0]==0 && text[1]==0 && text[2]==0xFE && text[3]==0xFF)
        return true;

    size>>=2;   // to count
    if(size<4) return false;

    ucs4_t ucs4, *p=(ucs4_t *)text;

    for(int i=0;i<size;i++, p++)
    {
        ucs4=wxINT32_SWAP_ON_LE(*p);

        if(ucs4<=0 || ucs4>0x10FFFF) return false;
    }

    return true;
}


bool IsTextUTF16LE(wxByte *text, int size)
{
    if(size >= 2)
    {
        if(text[0] == 0xFF && text[1] == 0xFE) // check BOM
        {
            if(size >= 4 && text[2] == 0 && text[3] == 0) // utf32le BOM
            {
                return false;
            }
            return true;
        }

        if(text[1] == 0xFF && text[0] == 0xFE)      // big-endian
            return false;

        bool ok = false;
        bool highsurrogate = false;

        size = size & 0x1FFFFFFE;   // to even
        while(size > 0)
        {
            if(text[1] == 0)
            {
                if(text[0] == 0)
                    return false;
                ok = true;
            }
            else if(text[1] >= 0xD8 && text[1] <= 0xDB)
            {
                if(highsurrogate)
                    return false;
                highsurrogate = true;
            }
            else if(text[1] >= 0xDC && text[1] <= 0xDF)
            {
                if(highsurrogate == false)
                    return false;
                highsurrogate = false;
            }

            size -= 2;
            text += 2;
        }

        return ok;
    }
    return false;
}

bool IsTextUTF16BE(wxByte *text, int size)
{
    if(size >= 2 && text[0] == 0xFE && text[1] == 0xFF)
        return true;

    bool ok = false;
    bool highsurrogate = false;
    size = size & 0x1FFFFFFE;     // to even
    while(size > 0)
    {
        if(text[0] == 0)
        {
            if(text[1] == 0)
                return false;
            ok = true;
        }
        else if(text[0] >= 0xD8 && text[0] <= 0xDB)
        {
            if(highsurrogate)
                return false;
            highsurrogate = true;
        }
        else if(text[0] >= 0xDC && text[0] <= 0xDF)
        {
            if(highsurrogate == false)
                return false;
            highsurrogate = false;
        }

        size -= 2;
        text += 2;
    }

    return ok;
}

inline bool IsUTF8Tail(wxByte b)
{
    return (b & 0xC0) == 0x80;
}
bool IsTextUTF8(wxByte *text, int size)
{
    //check BOM
    if(size >= 3 && text[0] == 0xEF && text[1] == 0xBB && text[2] == 0xBF)
        return true;

    int i = 0;
    int ok_count = 0;
    while(i < size)
    {
        if(text[i] < 0x80)          // 1 byte
        {
            if(text[i] == 0)
                return false;
            ++i;
        }
        else if(text[i] <= 0xDF)
        {
            if(++i < size && IsUTF8Tail(text[i]))     // 2 bytes
            {
                ++i;
                ++ok_count;
            }
            else if(size != i)
            {
                return false;
            }
        }
        else if(text[i] <= 0xEF)
        {
            if((++i < size && IsUTF8Tail(text[i]))    // 3 bytes
                && (++i < size && IsUTF8Tail(text[i])))
            {
                ++i;
                ++ok_count;
            }
            else if(size != i)
            {
                return false;
            }
        }
        else if(text[i] <= 0xF4)
        {
            if((++i < size && IsUTF8Tail(text[i]))    // 4 bytes
                && (++i < size && IsUTF8Tail(text[i])) //
                && (++i < size && IsUTF8Tail(text[i])))
            {
                ++i;
                ++ok_count;
            }
            else if(size != i)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return ok_count > 0;
}

bool IsBinaryData(wxByte *data, int size)
{
    int i = 0;
    wxByte b;
    while(i < size)
    {
        b = data[i];
        if(b == 0)
            return true;
        i++;
    }
    return false;
}

void DetectChineseEncoding(const wxByte *text, int count, wxm::WXMEncodingID &enc)
{
    // detect by punctuation marks
    int i=0;
    int b0=0, b1=0, c=0;
    int cp950=0;
    int cp936=0;

    while (i++ < count)
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
                int w=(b0<<8) + b1;
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

void DetectJapaneseEncoding(const wxByte *text, int count, wxm::WXMEncodingID &enc)
{
    wxByte c;
    int i=0;
    wxm::WXMEncodingID xenc= wxm::ENC_DEFAULT;

    while (xenc == 0 && i++ < count) {

        c = *text++;

        if (c >= 0x81 && c <= 0x9F) {
            if (c == 0x8E) /* SS2 */ {

                if(i++ >= count) break;

                c = *text++;
                if ((c >= 0x40 && c <= 0xA0) || (c >= 0xE0 && c <= 0xFC))
                    xenc = wxm::ENC_MS932;
            }
            else if (c == 0x8F) /* SS3 */ {

                if(i++ >= count) break;

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

            if(i++ >= count) break;

            c = *text++;
            if (c <= 0x9F)
                xenc = wxm::ENC_MS932;
            else if (c >= 0xFD)
                break;
        }
        else if (c >= 0xE0 && c <= 0xEF) {

            if(i++ >= count) break;

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

void DetectEncoding(const wxByte *text, int count, wxm::WXMEncodingID &enc)
{
    chardet_t det = NULL;
    char encoding_name[CHARDET_MAX_ENCODING_NAME];

    chardet_create(&det);
    chardet_handle_data(det, (const char*)text, count);
    chardet_data_end(det);
    chardet_get_charset(det, encoding_name, CHARDET_MAX_ENCODING_NAME);
    chardet_destroy(det);

    wxString name(encoding_name, wxConvLocal), rest;
    name.MakeUpper();
    long value;

    if(name.StartsWith(wxT("ISO-8859-"), &rest))
    {
        if(rest.ToLong(&value))
        {
            enc = wxm::WXMEncodingID( long(wxm::ENC_ISO_8859_1) - 1 + value );
        }
    }
    else if(name.StartsWith(wxT("WINDOWS-125"), &rest))
    {
        if(rest.ToLong(&value))
        {
            if(value==2)//1252 or ?
            {
                wxm::WXMEncodingID def=wxm::ENC_DEFAULT;
                if(enc==wxm::ENC_MS950 || wxm::ENC_MS936)
                {
                    DetectChineseEncoding(text, count, def);
                    if(def != wxm::ENC_DEFAULT)
                    {
                        value=-1;
                        enc = def;
                    }
                }
            }

            if(value>=0)
            {
                enc = wxm::WXMEncodingID( long(wxm::ENC_Windows_1250) + value );
            }
        }
    }
    else if(name.StartsWith(wxT("UTF-"), &rest))
    {
        if(rest[0] == wxT('8'))
        {
            enc = wxm::ENC_UTF_8;
        }
        else if(rest[0] == wxT('1')) // 16BE/LE
        {
            if(rest[2] == wxT('B')) enc = wxm::ENC_UTF_16BE;
            else                    enc = wxm::ENC_UTF_16LE;
        }
        else // 32BE/LE
        {
            if(rest[2] == wxT('B')) enc = wxm::ENC_UTF_32BE;
            else                    enc = wxm::ENC_UTF_32LE;
        }
    }
    else if(name.IsSameAs(wxT("BIG5")))
    {
        enc = wxm::ENC_MS950;
    }
    else if(name.IsSameAs(wxT("GB2312"))
         || name.IsSameAs(wxT("GB18030"))
         || name.IsSameAs(wxT("HZ-GB-2312")))
    {
        enc = wxm::ENC_MS936;
    }
    else if(name.IsSameAs(wxT("SHIFT_JIS")))
    {
        enc = wxm::ENC_MS932;
    }
    else if(name.IsSameAs(wxT("EUC-JP"))) // FIXME later
    {
        enc = wxm::ENC_CP20932;
    }
    else if(name.IsSameAs(wxT("EUC-KR")))
    {
        enc = wxm::ENC_MS949;
    }
    else if(name.IsSameAs(wxT("KOI8-R")))
    {
        enc = wxm::ENC_KOI8_R;
    }
}
