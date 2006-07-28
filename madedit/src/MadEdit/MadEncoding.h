///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEncoding.h
// Description: define the Encodings which are supported by MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADENCODING_H_
#define _MADENCODING_H_

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

private:
    MadEncodingInfo *m_Info;
    wxCSConv        *m_CSConv;
    ucs2_t          *m_MBtoWC_Table;    // MultiByte To WideChar table
    wxWord          *m_WCtoMB_Table;    // WideChar To MultiByte table
    wxByte          *m_LeadByte_Table;  // DBCS Lead-Byte table, 0:unset, 1:IsLeadByte, 0xFF:NotLeadByte

    void Create(size_t idx);

public:
    MadEncoding(size_t idx);
    MadEncoding(wxFontEncoding enc);
    MadEncoding(const wxString &name);
    ~MadEncoding();

    // return the converted length of buf
    size_t UCS4toMB     (ucs4_t ucs4, wxByte *buf);
    size_t UCS4toUTF8   (ucs4_t ucs4, wxByte *buf);
    static size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte *buf); // special case
    size_t UCS4toUTF16LE(ucs4_t ucs4, wxByte *buf);
    size_t UCS4toUTF16BE(ucs4_t ucs4, wxByte *buf);
    size_t UCS4toUTF32LE(ucs4_t ucs4, wxByte *buf);
    size_t UCS4toUTF32BE(ucs4_t ucs4, wxByte *buf);

    ucs4_t SBtoUCS4(wxByte b1);     // Single-Byte to UCS4
    ucs4_t DBtoUCS4(wxByte *buf);   // Double-Byte to UCS4

    bool IsLeadByte(wxByte byte);   // for DBCS only

    typedef size_t (MadEncoding::*UCS4toMultiByteFuncPtr)(ucs4_t ucs4, wxByte *buf);
    UCS4toMultiByteFuncPtr UCS4toMultiByte;

    wxString GetName() { return m_Info->m_Name; }
    wxString GetDescription() { return m_Info->m_Description; }
    MadEncodingType GetType() { return m_Info->m_Type; }
    wxString GetFontName() { return m_Info->m_FontName; }
    wxFontEncoding GetEncoding() { return m_Info->m_Encoding; }
};


bool IsTextUTF32LE(wxByte *text, int size);
bool IsTextUTF32BE(wxByte *text, int size);
bool IsTextUTF16LE(wxByte *text, int size);
bool IsTextUTF16BE(wxByte *text, int size);
bool IsTextUTF8(wxByte *text, int size);
bool IsBinaryData(wxByte *data, int size);
void DetectChineseEncoding(const wxByte *text, int count, wxFontEncoding &enc);
void DetectJapaneseEncoding(const wxByte *text, int count, wxFontEncoding &enc);

#endif
