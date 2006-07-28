///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEncoding.cpp
// Description: define the Encodings which are supported by MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEncoding.h"
#include <wx/config.h>
#include <vector>
using std::vector;

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif


static vector<MadEncodingInfo> EncodingsTable;

wxChar TestEncoding(const wxChar *name, wxFontEncoding enc, wxByte *mb)
{
#if defined(__WXGTK__)
    wxCSConv conv(name);
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
    wxCSConv conv(enc);
#endif

    wxChar wcs[4]={0};
    conv.MB2WC(wcs, (char*)mb, 4);
    return wcs[0];
}

#ifdef __WXMSW__
#define MSW_GET_FONT_NAME(cp,fn) MSW_GetFontName(cp,fn)

// get fontname from registry mime database
void MSW_GetFontName(wxChar *codepage, wxString &fontname)
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

size_t MadEncoding::ms_SystemEncodingIndex=0;
MadEncoding *MadEncoding::ms_SystemEncoding=NULL;

void MadEncoding::InitEncodings()
{
    wxLogNull nolog;

    wxFontEncoding sysenc=wxLocale::GetSystemEncoding();

    size_t count=wxFontMapper::GetSupportedEncodingsCount();
    for(size_t idx=0;idx<count;idx++)
    {
        wxFontEncoding enc=wxFontMapper::GetEncoding(idx);
        wxString name=wxFontMapper::GetEncodingName(enc);
        wxString desc=wxFontMapper::GetEncodingDescription(enc);
        MadEncodingType type=etSingleByte;

#ifdef __WXMSW__
        wxString fontname(wxT("Courier New"));
#else
        wxString fontname(wxT("Monospace"));
#endif

        bool ignore=false;
        bool dotest=true;      // test the encoding if is supported in this machine
        wxChar testwc=0;
        wxByte testmb[8]={0};
        switch(enc)
        {
        case wxFONTENCODING_CP932:          // Japanese (shift-JIS)
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("SHIFT-JIS");
                    type=etDoubleByte;
                    testwc=0x3000;
                    testmb[0]=0x81;
                    testmb[1]=0x40;
                    MSW_GET_FONT_NAME(wxT("932"), fontname);
                    added=true;
                }
                else
                {
                    dotest=false;
                    ignore=true;
                }
            }
            break;
        case wxFONTENCODING_CP936:          // Chinese simplified (GB)
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("GBK");
                    type=etDoubleByte;
                    testwc=0x3000;
                    testmb[0]=0xa1;
                    testmb[1]=0xa1;
                    MSW_GET_FONT_NAME(wxT("936"), fontname);
                    added=true;
                }
                else
                {
                    dotest=false;
                    ignore=true;
                }
            }
            break;
        case wxFONTENCODING_CP949:          // Korean (Hangul charset)
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("EUC-KR");
                    type=etDoubleByte;
                    testwc=0x3000;
                    testmb[0]=0xa1;
                    testmb[1]=0xa1;
                    MSW_GET_FONT_NAME(wxT("949"), fontname);
                    added=true;
                }
                else
                {
                    dotest=false;
                    ignore=true;
                }
            }
            break;
        case wxFONTENCODING_CP950:          // Chinese (traditional - Big5)
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("BIG5");
                    type=etDoubleByte;
                    testwc=0x3000;
                    testmb[0]=0xa1;
                    testmb[1]=0x40;
                    MSW_GET_FONT_NAME(wxT("950"), fontname);
                    added=true;
                }
                else
                {
                    dotest=false;
                    ignore=true;
                }
            }
            break;
        case wxFONTENCODING_EUC_JP:         // Extended Unix Codepage for Japanese
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("EUC-JP");
                    type=etDoubleByte;
                    testwc=0x3000;
                    testmb[0]=0xa1;
                    testmb[1]=0xa1;
                    MSW_GET_FONT_NAME(wxT("51932"), fontname);
                    added=true;
                }
                else
                {
                    dotest=false;
                    ignore=true;
                }
            }
            break;
        case wxFONTENCODING_CP437:
            name=wxT("CP437");
            testwc=0xa0;
            testmb[0]=0xff;
            break;
        case wxFONTENCODING_UTF7:    // not support
            ignore=true;
            dotest=false;
            break;
        case wxFONTENCODING_UTF8:
            type=etUTF8;
            dotest=false;
            break;
        case wxFONTENCODING_UTF16BE:
            type=etUTF16BE;
            dotest=false;
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("utf-16be");
                    desc=wxT("Unicode 16 bit Big Endian (UTF-16BE)");
                    added=true;
                }
                else ignore=true;
            }
            break;
        case wxFONTENCODING_UTF16LE:
            type=etUTF16LE;
            dotest=false;
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("utf-16le");
                    desc=wxT("Unicode 16 bit Little Endian (UTF-16LE)");
                    added=true;
                }
                else ignore=true;
            }
            break;
        case wxFONTENCODING_UTF32BE:
            type=etUTF32BE;
            dotest=false;
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("utf-32be");
                    desc=wxT("Unicode 32 bit Big Endian (UTF-32BE)");
                    added=true;
                }
                else ignore=true;
            }
            break;
        case wxFONTENCODING_UTF32LE:
            type=etUTF32LE;
            dotest=false;
            {
                static bool added=false;
                if(!added)
                {
                    name=wxT("utf-32le");
                    desc=wxT("Unicode 32 bit Little Endian (UTF-32LE)");
                    added=true;
                }
                else ignore=true;
            }
            break;
        default:
            if(enc>=wxFONTENCODING_ISO8859_2 && enc<=wxFONTENCODING_ISO8859_15)
            {
                static wxChar wctable[]={0x0102, 0x0108, 0x0100, 0x00a7, 0x060c, 0x037a, 0x00d7, 0x011e, 0x0100, 0x0e01, 0, 0x00c6, 0x010a, 0x0152};
                static wxByte mbtable[]={0xc3,   0xc6,   0xc0,   0xfd,   0xac,   0xaa,   0xaa,   0xd0,   0xc0,   0xa1,   0, 0xaf,   0xa4,   0xbc};
                testwc    = wctable[enc-wxFONTENCODING_ISO8859_2];
                testmb[0] = mbtable[enc-wxFONTENCODING_ISO8859_2];
            }
            else if(enc>=wxFONTENCODING_CP1250 && enc<=wxFONTENCODING_CP1257)
            {
                static wxChar wctable[]={0x0102, 0x0401, 0x0152, 0x0192, 0x011e, 0x00d7, 0x0152, 0x00a8};
                static wxByte mbtable[]={0xc3,   0xa8,   0x8c,   0x83,   0xd0,   0xaa,   0x8c,   0x8d};
                testwc    = wctable[enc-wxFONTENCODING_CP1250];
                testmb[0] = mbtable[enc-wxFONTENCODING_CP1250];
            }
            else if(name==wxT("windows-874"))
            {
                testwc=0x0e50;
                testmb[0]=0xf0;
                MSW_GET_FONT_NAME(wxT("874"), fontname);
            }
            else if(name==wxT("default") || name==wxGetTranslation(wxT("default")))   // unnecessary
            {
                ignore=true;
                dotest=false;
            }
        }

        // test the encoding is supported in the system
        if(dotest)
        {
            if(testwc!=0)
            {
                ignore= (testwc!=TestEncoding(name.c_str(), enc, testmb));
            }
#ifdef __WXMSW__
            else
            {
                ignore= ! wxFontMapper::Get()->IsEncodingAvailable(enc);
            }
#endif
        }

        if(!ignore)
        {
            if(enc == sysenc)
            {
                ms_SystemEncodingIndex=EncodingsTable.size();
            }

            EncodingsTable.push_back( MadEncodingInfo(enc, name.Upper(), desc, type, fontname) );
        }
    }
}

void MadEncoding::FreeEncodings()
{
    if(ms_SystemEncoding!=NULL)
    {
        delete ms_SystemEncoding;
    }

    vector<MadEncodingInfo>::iterator it=EncodingsTable.begin();
    vector<MadEncodingInfo>::iterator itend=EncodingsTable.end();
    while(it != itend)
    {
        if(it->m_CSConv) delete it->m_CSConv;
        if(it->m_MBtoWC_Table) delete []it->m_MBtoWC_Table;
        if(it->m_WCtoMB_Table) delete []it->m_WCtoMB_Table;
        if(it->m_LeadByte_Table) delete []it->m_LeadByte_Table;
        ++it;
    }
}


size_t MadEncoding::GetEncodingsCount()
{
    return EncodingsTable.size();
}

wxString MadEncoding::GetEncodingName(size_t idx)
{
    wxASSERT(idx<EncodingsTable.size());
    //return wxFontMapper::GetEncodingName( EncodingsTable[idx].m_Encoding );
    return EncodingsTable[idx].m_Name;
}

wxString MadEncoding::GetEncodingDescription(size_t idx)
{
    wxASSERT(idx<EncodingsTable.size());
    //return wxFontMapper::GetEncodingDescription( EncodingsTable[idx].m_Encoding );
    return EncodingsTable[idx].m_Description;
}

wxString MadEncoding::GetEncodingFontName(size_t idx)
{
    wxASSERT(idx<EncodingsTable.size());
    return EncodingsTable[idx].m_FontName;
}

wxString MadEncoding::EncodingToName(wxFontEncoding enc)
{
    size_t idx;
    for(idx=0;idx<EncodingsTable.size();idx++)
    {
        if(EncodingsTable[idx].m_Encoding==enc)
        {
            return EncodingsTable[idx].m_Name;
        }
    }
    return EncodingsTable[ms_SystemEncodingIndex].m_Name;
}

wxFontEncoding MadEncoding::NameToEncoding(const wxString &name)
{
    size_t idx;
    for(idx=0;idx<EncodingsTable.size();idx++)
    {
        if(EncodingsTable[idx].m_Name==name.Upper())
        {
            return EncodingsTable[idx].m_Encoding;
        }
    }
    return EncodingsTable[ms_SystemEncodingIndex].m_Encoding;
}

MadEncoding *MadEncoding::GetSystemEncoding()
{
    if(ms_SystemEncoding==NULL)
    {
        ms_SystemEncoding=new MadEncoding(ms_SystemEncodingIndex);
    }
    return ms_SystemEncoding;
}

MadEncoding::MadEncoding(size_t idx)
{
    Create(idx);
}

MadEncoding::MadEncoding(wxFontEncoding enc)
{
    size_t idx;
    for(idx=0;idx<EncodingsTable.size();idx++)
    {
        if(EncodingsTable[idx].m_Encoding==enc)
        {
            Create(idx);
            break;
        }
    }

    if(idx==EncodingsTable.size())
    {
        Create(ms_SystemEncodingIndex);
    }
}

MadEncoding::MadEncoding(const wxString &name)
{
    size_t idx;
    for(idx=0;idx<EncodingsTable.size();idx++)
    {
        if(EncodingsTable[idx].m_Name==name.Upper())
        {
            Create(idx);
            break;
        }
    }

    if(idx==EncodingsTable.size())
    {
        Create(ms_SystemEncodingIndex);
    }
}

void MadEncoding::Create(size_t idx)
{
    wxASSERT(idx<EncodingsTable.size());

    m_Info=&EncodingsTable[idx];
    m_CSConv=m_Info->m_CSConv;
    if(m_CSConv!=NULL)
    {
        m_MBtoWC_Table=m_Info->m_MBtoWC_Table;
        m_WCtoMB_Table=m_Info->m_WCtoMB_Table;
        m_LeadByte_Table=m_Info->m_LeadByte_Table;
    }
    else
    {
#if defined(__WXGTK__)
        m_CSConv=new wxCSConv(m_Info->m_Name.c_str());
#else //#elif defined(__WXMSW__) || defined(__WXMAC__)
        m_CSConv=new wxCSConv(m_Info->m_Encoding);
#endif
        m_Info->m_CSConv=m_CSConv;

        switch(m_Info->m_Type)
        {
        case etSingleByte:
            m_MBtoWC_Table=new ucs2_t[256];
            memset(m_MBtoWC_Table, 0, sizeof(ucs2_t)*256);

            m_WCtoMB_Table=new wxWord[65536];
            memset(m_WCtoMB_Table, 0, sizeof(wxWord)*65536);

            {   // cache the results of Single-Byte <==> Wide-Char
                wxByte singlebyte[2]={0,0};
                wchar_t wc[2];
                for(wxWord i=0;i<256;i++)
                {
                    singlebyte[0]=i;
                    if(m_CSConv->MB2WC(wc,(char*)singlebyte,2)==1)
                    {
                        m_MBtoWC_Table[i]=wc[0];
                    }
                    else
                    {
                        m_MBtoWC_Table[i]=i;
                        wc[0]=i;
                    }
                    m_WCtoMB_Table[ wc[0] ]= (i<<8) ;
                }
            }

            m_Info->m_MBtoWC_Table=m_MBtoWC_Table;
            m_Info->m_WCtoMB_Table=m_WCtoMB_Table;

            break;
        case etDoubleByte:
            m_MBtoWC_Table=new ucs2_t[65536];
            // value: 0x0000, indicate the column isn't a valid Double-Byte char
            memset(m_MBtoWC_Table, 0, sizeof(ucs2_t)*256);
            // value: 0xFFFF, indicate the column is non-cached
            memset(&m_MBtoWC_Table[256], 0xFF, sizeof(ucs2_t)*(65536-256));
            m_WCtoMB_Table=new wxWord[65536];
            memset(m_WCtoMB_Table, 0xFF, sizeof(wxWord)*65536);
            m_LeadByte_Table=new wxByte[256];
            memset(m_LeadByte_Table, 0, 256);
            m_LeadByte_Table[0]=0xFF;

            m_Info->m_MBtoWC_Table=m_MBtoWC_Table;
            m_Info->m_WCtoMB_Table=m_WCtoMB_Table;
            m_Info->m_LeadByte_Table=m_LeadByte_Table;

            break;
        default:
            break;
        }
    }

    // set member function pointer: UCS4toMultiByte
    switch(m_Info->m_Type)
    {
    case etUTF8:
        UCS4toMultiByte=&MadEncoding::UCS4toUTF8;
        break;
    case etUTF16LE:
        UCS4toMultiByte=&MadEncoding::UCS4toUTF16LE;
        break;
    case etUTF16BE:
        UCS4toMultiByte=&MadEncoding::UCS4toUTF16BE;
        break;
    case etUTF32LE:
        UCS4toMultiByte=&MadEncoding::UCS4toUTF32LE;
        break;
    case etUTF32BE:
        UCS4toMultiByte=&MadEncoding::UCS4toUTF32BE;
        break;
    default:
    //case etSingleByte:
    //case etDoubleByte:
        UCS4toMultiByte=&MadEncoding::UCS4toMB;
        break;
    }
}

MadEncoding::~MadEncoding()
{
}


size_t MadEncoding::UCS4toMB(ucs4_t ucs4, wxByte *buf)
{
    if(ucs4>0xFFFF)
        return 0;

    wxWord mb=m_WCtoMB_Table[ucs4];
    if(mb==0)        // invalid MB char
    {
        return 0;
    }

    size_t len;
    if(mb==0xFFFF)    // non-cached
    {
        wxByte mbs[4];
        wchar_t wc[2]={ucs4,0};
        len=m_CSConv->WC2MB((char*)mbs,wc,4);
        if(len==0 || len==(size_t)-1)
        {
            m_WCtoMB_Table[ucs4]=0;
            return 0;
        }

        wxASSERT( len<=2 );

        //mbs[len]=0;
        mb= (((wxWord)mbs[0])<<8) | mbs[1];
        m_WCtoMB_Table[ucs4]=mb;

        buf[0]=mbs[0];
        if(len==2) buf[1]=mbs[1];

        return len;
    }

    buf[0]=mb>>8;
    len=1;

    if((mb&0xFF)!=0)
    {
        buf[1]=mb&0xFF;
        ++len;//len=2
    }

    return len;
}

size_t MadEncoding::UCS4toUTF8(ucs4_t ucs4, wxByte *buf)
{
    /***  from rfc3629
    Char. number range  |        UTF-8 octet sequence
    (hexadecimal)    |              (binary)
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

size_t MadEncoding::UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte *buf)
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

size_t MadEncoding::UCS4toUTF16LE(ucs4_t ucs4, wxByte *buf)
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
size_t MadEncoding::UCS4toUTF16BE(ucs4_t ucs4, wxByte *buf)
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

size_t MadEncoding::UCS4toUTF32LE(ucs4_t ucs4, wxByte *buf)
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
size_t MadEncoding::UCS4toUTF32BE(ucs4_t ucs4, wxByte *buf)
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

ucs4_t MadEncoding::SBtoUCS4(wxByte b1)    // Single-Byte to UCS4
{
    return m_MBtoWC_Table[ b1 ];
}

// return 0 if it is not a valid DB char
ucs4_t MadEncoding::DBtoUCS4(wxByte *buf)    // Double-Byte to UCS4
{
    if(m_LeadByte_Table[ buf[0] ]==0)
        IsLeadByte(buf[0]);

    wxWord w=(((wxWord)buf[0])<<8) | buf[1];
    return m_MBtoWC_Table[w];
}

bool MadEncoding::IsLeadByte(wxByte byte)
{
    if(m_LeadByte_Table[byte]==0)
    {
        wxWord db= ((wxWord)byte)<<8 ;
        m_MBtoWC_Table[ db++ ]=0;
        wxByte dbs[4]={byte,1,0,0};
        wchar_t wc[4];
        for(wxWord i=1; i<=0xFF; i++, dbs[1]++, db++)
        {
            if(m_CSConv->MB2WC(wc,(char*)dbs,4)==1)
            {
                m_MBtoWC_Table[ db ]=       wc[0];
                m_WCtoMB_Table[ wc[0] ]=    db;
                m_LeadByte_Table[byte]=1;       // is lead-byte
            }
            else
            {
                m_MBtoWC_Table[ db ]=0;
            }
        }

        if(m_LeadByte_Table[byte]==0)
            m_LeadByte_Table[byte]=0xFF;
    }

    return m_LeadByte_Table[byte]==1;
}


//==================================================

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

        if(ucs4<=7 || (ucs4<=0x10&&ucs4>=0x0e) || ucs4>0x10FFFF) return false;
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

        if(ucs4<=7 || (ucs4<=0x10&&ucs4>=0x0e) || ucs4>0x10FFFF) return false;
    }

    return true;
}


bool IsTextUTF16LE(wxByte *text, int size)
{
    if(size >= 2)
    {
        if(text[0] == 0xFF && text[1] == 0xFE)
        {
            if(size >= 4 && text[2] == 0 && text[3] == 0) // utf32le
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
            if(text[1] == 0)          //wc<=0x0008
            {
                if(text[0] <= 0x10 && (text[0] <= 7 || text[0] >= 0x0E))
                    return false;
                if((text[0] >= 9 && text[0] <= 0x0D)
                    || (text[0] >= 0x20 && text[0] <= 0x7E))
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
        if(text[0] == 0)            //wc<=0x0008
        {
            wxByte b = text[1];
            if(b <= 0x10 && (b <= 7 || b >= 0x0E))
                return false;
            if((b >= 9 && b <= 0x0D) || (b >= 0x20 && b <= 0x7E))
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
            if(text[i] <= 0x10 && (text[i] <= 7 || text[i] >= 0x0E))
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
        if(b <= 0x10 && (b <= 7 || b >= 0x0E))
            return true;
        i++;
    }
    return false;
}

void DetectChineseEncoding(const wxByte *text, int count, wxFontEncoding &enc)
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
                    enc = wxFONTENCODING_CP936; // [0x81~0xFE][0x80~0xA0] are invalid in big5
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

    if(cp950>cp936) enc = wxFONTENCODING_CP950;
    else if(cp936>cp950) enc = wxFONTENCODING_CP936;
}

void DetectJapaneseEncoding(const wxByte *text, int count, wxFontEncoding &enc)
{
    wxByte c;
    int i=0;
    wxFontEncoding xenc= wxFontEncoding(0);

    while (xenc == 0 && i++ < count) {

        c = *text++;

        if (c >= 0x81 && c <= 0x9F) {
            if (c == 0x8E) /* SS2 */ {

                if(i++ >= count) break;

                c = *text++;
                if ((c >= 0x40 && c <= 0xA0) || (c >= 0xE0 && c <= 0xFC))
                    xenc = wxFONTENCODING_CP932;
            }
            else if (c == 0x8F) /* SS3 */ {

                if(i++ >= count) break;

                c = *text++;
                if (c >= 0x40 && c <= 0xA0)
                    xenc = wxFONTENCODING_CP932;
                else if (c >= 0xFD)
                    break;
            }
            else
                xenc = wxFONTENCODING_CP932;
        }
        else if (c >= 0xA1 && c <= 0xDF) {

            if(i++ >= count) break;

            c = *text++;
            if (c <= 0x9F)
                xenc = wxFONTENCODING_CP932;
            else if (c >= 0xFD)
                break;
        }
        else if (c >= 0xE0 && c <= 0xEF) {

            if(i++ >= count) break;

            c = *text++;
            if (c >= 0x40 && c <= 0xA0)
                xenc = wxFONTENCODING_CP932;
            else if (c >= 0xFD)
                break;
        }
        else if (c >= 0xF0)
            break;
    }

    if (xenc != 0)
        enc = xenc;
}
