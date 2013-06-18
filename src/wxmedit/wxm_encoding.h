///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxmedit/wxm_encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_H_
#define _WXM_ENCODING_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <wx/fontmap.h>
#include <wx/string.h>
#include "ucs4_t.h"

enum MadEncodingType
{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE };

struct MadEncodingInfo
{
    wxFontEncoding  m_Encoding;
    wxString        m_Name;
    wxString        m_Description;
    wxString        m_FontName;
    MadEncodingType m_Type;
    wxCSConv       *m_CSConv;
    ucs2_t         *m_MBtoWC_Table;      // MultiByte To WideChar table
    wxWord         *m_WCtoMB_Table;      // WideChar To MultiByte table
    wxByte         *m_LeadByte_Table;    // DBCS Lead-Byte table

    MadEncodingInfo(wxFontEncoding e, const wxString &n, const wxString &de, MadEncodingType t, const wxString &fn)
        :m_Encoding(e), m_Name(n), m_Description(de), m_FontName(fn), m_Type(t),
        m_CSConv(NULL), m_MBtoWC_Table(NULL), m_WCtoMB_Table(NULL), m_LeadByte_Table(NULL)
    {
    }
};


class MadEncoding
{
private:
    static size_t ms_SystemEncodingIndex;
    static MadEncoding *ms_SystemEncoding;

public:
    static void     InitEncodings(); // must call this before use MadEncoding
    static void     FreeEncodings(); //

    static size_t   GetEncodingsCount();
    static wxString GetEncodingName(size_t idx);
    static wxString GetEncodingDescription(size_t idx);
    static wxString GetEncodingFontName(size_t idx);
    static wxString EncodingToName(wxFontEncoding enc);
    static wxFontEncoding NameToEncoding(const wxString &name);
    static MadEncoding *GetSystemEncoding();

protected:
    MadEncodingInfo *m_Info;

    virtual void Create(size_t idx);

public:
    static MadEncoding* CreateWxmEncoding(size_t idx);
    static MadEncoding* CreateWxmEncoding(wxFontEncoding enc);
    static MadEncoding* CreateWxmEncoding(const wxString &name);

	MadEncoding(): m_Info(NULL)
	{
	}
    virtual ~MadEncoding();

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

    wxString GetName() { return m_Info->m_Name; }
    wxString GetDescription() { return m_Info->m_Description; }
    MadEncodingType GetType() { return m_Info->m_Type; }
    wxString GetFontName() { return m_Info->m_FontName; }
    wxFontEncoding GetEncoding() { return m_Info->m_Encoding; }
};

struct WXMEncodingMultiByte: public MadEncoding
{
	WXMEncodingMultiByte(): m_CSConv(NULL), m_MBtoWC_Table(NULL), m_WCtoMB_Table(NULL), m_LeadByte_Table(NULL)
	{
	}

    virtual void MultiByteInit() = 0;
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf) = 0;
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
	virtual void Create(size_t idx);

protected:
    wxCSConv        *m_CSConv;
    ucs2_t          *m_MBtoWC_Table;    // MultiByte To WideChar table
    wxWord          *m_WCtoMB_Table;    // WideChar To MultiByte table
    wxByte          *m_LeadByte_Table;  // DBCS Lead-Byte table, 0:unset, 1:IsLeadByte, 0xFF:NotLeadByte
};

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
};

struct WXMEncodingDoubleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual bool IsLeadByte(wxByte byte);
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
};

struct WXMEncodingUTF8: public MadEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
};

struct WXMEncodingUTF16LE: public MadEncoding
{
    static size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf); // special case
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
};

struct WXMEncodingUTF16BE: public MadEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
};

struct WXMEncodingUTF32LE: public MadEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
};

struct WXMEncodingUTF32BE: public MadEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
};

#endif
