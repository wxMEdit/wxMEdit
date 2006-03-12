///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditAdvanced.cpp
// Description: advanced functions of MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEdit.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

//==============================================================================
void MadEdit::ToUpperCase()
{
    if(IsReadOnly() || !m_Selection)
        return;

    wxString text;
    GetSelText(text);
    bool modified=false;
    
    size_t i=0, count=text.Len();
    while(i<count)
    {
        int c=text[i];
        int nc=towupper(c);
        if(nc != c)
        {
            text.SetChar(i, nc);
            modified=true;
        }
        ++i;
    }
    
    if(modified)
    {
        vector<ucs4_t> ucs;
        TranslateText(text.c_str(), text.Len(), &ucs, true);
        
        if(m_EditMode==emColumnMode)
        {
            int colcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;
            InsertColumnString(&(*ucs.begin()), ucs.size(), colcount, false, true);
        }
        else
        {
            InsertString(&(*ucs.begin()), ucs.size(), false, false, true);
        }
    }
}

void MadEdit::ToLowerCase()
{
    if(IsReadOnly() || !m_Selection)
        return;

    wxString text;
    GetSelText(text);
    bool modified=false;
    
    size_t i=0, count=text.Len();
    while(i<count)
    {
        int c=text[i];
        int nc=towlower(c);
        if(nc != c)
        {
            text.SetChar(i, nc);
            modified=true;
        }
        ++i;
    }
    
    if(modified)
    {
        vector<ucs4_t> ucs;
        TranslateText(text.c_str(), text.Len(), &ucs, true);
        
        if(m_EditMode==emColumnMode)
        {
            int colcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;
            InsertColumnString(&(*ucs.begin()), ucs.size(), colcount, false, true);
        }
        else
        {
            InsertString(&(*ucs.begin()), ucs.size(), false, false, true);
        }
    }
}

void MadEdit::InvertCase()
{
    if(IsReadOnly() || !m_Selection)
        return;

    wxString text;
    GetSelText(text);
    bool modified=false;
    
    size_t i=0, count=text.Len();
    while(i<count)
    {
        int c=text[i];
        int nc=c;
        if(iswlower(c))
        {
            nc=towupper(c);
        }
        else if(iswupper(c))
        {
            nc=towlower(c);
        }
        
        if(nc != c)
        {
            text.SetChar(i, nc);
            modified=true;
        }
        ++i;
    }
    
    if(modified)
    {
        vector<ucs4_t> ucs;
        TranslateText(text.c_str(), text.Len(), &ucs, true);
        
        if(m_EditMode==emColumnMode)
        {
            int colcount = m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1;
            InsertColumnString(&(*ucs.begin()), ucs.size(), colcount, false, true);
        }
        else
        {
            InsertString(&(*ucs.begin()), ucs.size(), false, false, true);
        }
    }
}

void MadEdit::ToHalfWidth()
{
}

void MadEdit::ToFullWidth()
{
}

//==============================================================================
// Unicode-4.1 Blocks (from: http://www.unicode.org/Public/UNIDATA/Blocks.txt)

struct UnicodeBlock
{
    ucs4_t       begin;
    ucs4_t       end;
    bool         alphabet;
    ucs4_t       *delimiters;
    const wxChar *description;
};

ucs4_t U0000_Delimiters[]={0};


// do not use wxGetTranslation() now
#undef _
#define _(s)    wxT(s)

UnicodeBlock UnicodeBlocks[]=
{
    {0x0000, 0x007F, true, U0000_Delimiters, _("Basic Latin") },

};

// restore the definition of _(s)
#undef _
#define _(s)    wxGetTranslation(_T(s))


void MadEdit::WordCount(bool selection, int &wordCount, int &charCount, int &spaceCount, int &halfWidthCount, int &fullWidthCount, wxArrayString *details)
{
    
}
