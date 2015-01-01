///////////////////////////////////////////////////////////////////////////////
// vim:         sw=4 ts=4 expandtab
// Name:        wxmedit/mad_encdet.cpp
// Description: MadEdit Encoding Detector
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "mad_encdet.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

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
