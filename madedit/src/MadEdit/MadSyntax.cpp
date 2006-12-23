///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadSyntax.cpp
// Description: syntax parsing and syntax file management
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadSyntax.h"
#include "MadEdit.h"
#include "MadEncoding.h"

#include <wx/fileconf.h>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/filename.h>


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern wxString g_MadEditHomeDir;


wxChar *SystemAttributesName[] = {
    wxT("Text"), wxT("Delimiter"), wxT("Space"), wxT("Number"), wxT("String"), wxT("Comment"),
    wxT("Directive"), wxT("SpecialWord"), wxT("LineNumber"), wxT("ActiveLine"), wxT("Bookmark"),
};

wxChar *SystemAttributesColor[]= {
    wxT("Black"), wxT("SaddleBrown"), wxT("Aqua"), wxT("Blue"), wxT("Red"), wxT("Teal"), wxT("Green"), wxT("Maroon"), wxT("White"), wxT("Fuchsia"), wxT("#\xC0\xFF\xFF")
};
wxChar *SystemAttributesBgColor[]= {
    wxT("White"), wxT(""), wxT(""), wxT(""), wxT(""), wxT(""), wxT(""), wxT(""), wxT("#\xA0\xA0\xA0"), wxT(""), wxT("")
};

const wxString MadPlainTextTitle(wxT("Plain Text"));

vector<wxString> g_SynPaths;

typedef deque< pair<wxString, wxString> > StringPairTable;
typedef deque< pair<wxString, wxString> >::iterator StringPairIterator;

// synfile of g_TitleSynfileTable is full path
StringPairTable g_TitleSynfileTable;

// syntax scheme files table (filename[*], fullpath)
StringPairTable g_NameSchfileTable;

WX_DECLARE_HASH_MAP( wxString, wxString, wxStringHash, wxStringEqual, StringMap );

// synfile of below is filename only
StringMap g_ExtSynfileMap;
StringMap g_FirstlineSynfileMap;
StringMap g_FilenameSynfileMap;


void LoadListFile(const wxString &listfile, StringMap &map, bool noCase)
{
    wxTextFile list;

    if(wxFileExists(listfile) && list.Open(listfile) && !list.Eof())
    {
        wxString str = list.GetFirstLine(), tok1, tok2;
        for(;;)
        {
            str.Trim(false);
            if(!str.IsEmpty() && str[0]!='#') // not a comment line
            {
                wxStringTokenizer tkz(str);
                tok1=tkz.GetNextToken();
                tok2=tkz.GetNextToken();

                if(!tok1.IsEmpty() && !tok2.IsEmpty())
                {
                    if(noCase)
                    {
                        tok1.MakeUpper();
                    }

                    StringMap::iterator it=map.find(tok1);
                    if(it==map.end())
                    {
                        map.insert(StringMap::value_type(tok1, tok2));
                    }
                }
            }

            if(list.Eof())
                break;

            str = list.GetNextLine();
        }

    }
}

wxString MadSyntax::s_AttributeFilePath;
bool MadSyntax::s_Loaded=false;

void MadSyntax::LoadSyntaxFiles()
{
    for(size_t i=0;i<g_SynPaths.size();i++)
    {
        wxDir dir(g_SynPaths[i]);
        wxString filename;

        bool cont=dir.GetFirst(&filename, wxT("syntax_*.syn"), wxDIR_FILES);
        while(cont)
        {
            //wxLogMessage(filename);
            filename=g_SynPaths[i]+filename;
            wxFileConfig syn(wxEmptyString,wxEmptyString,filename,wxEmptyString,wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);

            wxString entry, title;
            long idx=0;
            bool econt=syn.GetNextEntry(entry, idx);
            while(econt)
            {
                syn.Read(entry, &title);
                entry.MakeLower();
                title.Trim(true);
                title.Trim(false);

                if(entry == wxT("title"))
                {
                    if(title==MadPlainTextTitle)
                    {
                        // "Plain Text" synfile must be first item of g_TitleSynfileTable
                        if(g_TitleSynfileTable.size()==0 || g_TitleSynfileTable[0].first!=title)
                        {
                            g_TitleSynfileTable.insert(g_TitleSynfileTable.begin(), pair<wxString, wxString>(title, filename));
                        }
                    }
                    else
                    {
                        StringPairIterator it=g_TitleSynfileTable.begin();
                        StringPairIterator itend=g_TitleSynfileTable.end();

                        bool insert=true;

                        if(it!=itend)
                        {
                            if(g_TitleSynfileTable[0].first==MadPlainTextTitle)
                            {
                                ++it;
                            }

                            while(it!=itend)
                            {
                                int cmp=it->first.CmpNoCase(title);
                                if(cmp==0)
                                {
                                    insert=false;
                                    break;
                                }
                                if(cmp>0)
                                {
                                    break;
                                }
                                ++it;
                            }
                        }

                        if(insert)
                        {
                            g_TitleSynfileTable.insert(it, pair<wxString, wxString>(title, filename));
                        }

                    }

                    break;
                }

                econt=syn.GetNextEntry(entry, idx);
            }

            cont = dir.GetNext(&filename);
        }

        // load syntax scheme files
        cont=dir.GetFirst(&filename, wxT("*.sch"), wxDIR_FILES);
        static int front=0, back=0;
        while(cont)
        {
            filename=g_SynPaths[i]+filename;
            wxFileName fn(filename);
            wxString name=fn.GetName();
            StringPairIterator it=g_NameSchfileTable.begin()+front;
#ifdef __WXMSW__
            if(filename.Upper().Find(s_AttributeFilePath.Upper().c_str())<0)
#else
            if(filename.Find(s_AttributeFilePath.c_str())<0)
#endif
            {
                name+=wxT('*');
                ++front;
            }
            else
            {
                it+=back;
                ++back;
            }
            g_NameSchfileTable.insert(it, pair<wxString, wxString>(name, filename));

            cont = dir.GetNext(&filename);
        }


        LoadListFile(g_SynPaths[i]+wxT("extension.list"), g_ExtSynfileMap, true);
        LoadListFile(g_SynPaths[i]+wxT("firstline.list"), g_FirstlineSynfileMap, false);
        LoadListFile(g_SynPaths[i]+wxT("filename.list"),  g_FilenameSynfileMap, true);
    }

    // make sure "Plain Text" was added, otherwise add it
    if(g_TitleSynfileTable.size()==0 || g_TitleSynfileTable[0].first!=MadPlainTextTitle)
    {
        g_TitleSynfileTable.insert(g_TitleSynfileTable.begin(), pair<wxString, wxString>(MadPlainTextTitle, wxEmptyString));
    }

    // add "Default*" to the g_NameSchfileTable
    g_NameSchfileTable.push_front(pair<wxString, wxString>(wxT("Default*"), wxEmptyString));

    s_Loaded=true;
}

void MadSyntax::AddSyntaxFilesPath(const wxString &path)
{
    if(wxDirExists(path)) g_SynPaths.push_back(path);
}

size_t MadSyntax::GetSyntaxCount()
{
    if(!s_Loaded) LoadSyntaxFiles();

    return g_TitleSynfileTable.size();
}

wxString MadSyntax::GetSyntaxTitle(size_t index)
{
    if(!s_Loaded) LoadSyntaxFiles();

    if(index<g_TitleSynfileTable.size())
    {
        return g_TitleSynfileTable[index].first;
    }
    return wxEmptyString;
}

wxString MadSyntax::GetSyntaxFile(size_t index)
{
    if(!s_Loaded) LoadSyntaxFiles();

    if(index<g_TitleSynfileTable.size())
    {
        return g_TitleSynfileTable[index].second;
    }
    return wxEmptyString;
}

wxString MadSyntax::GetSyntaxFileByTitle(const wxString &title)
{
    if(!s_Loaded) LoadSyntaxFiles();

    for(size_t i=0;i<g_TitleSynfileTable.size();i++)
    {
        if(g_TitleSynfileTable[i].first==title)
        {
            return g_TitleSynfileTable[i].second;
        }
    }
    return wxEmptyString;
}

wxString MadSyntax::GetAttributeFileByTitle(const wxString &title)
{
    wxString file=GetSyntaxFileByTitle(title);
    if(file.IsEmpty())
    {
        file=s_AttributeFilePath +wxT("temp.att");
    }
    else
    {
        wxFileName fn(file);
        file=s_AttributeFilePath +fn.GetName() +wxT(".att");
    }
    return file;
}

MadSyntax* MadSyntax::GetSyntaxByTitle(const wxString &title)
{
    if(!s_Loaded) LoadSyntaxFiles();

    wxString synfile=GetSyntaxFileByTitle(title);
    if(wxFileExists(synfile))
    {
        return new MadSyntax(synfile);
    }

    return new MadSyntax(); // return a default syntax
}

MadSyntax* MadSyntax::GetSyntaxByExt(const wxString &ext)
{
    if(!s_Loaded) LoadSyntaxFiles();

    wxString synfile;

    StringMap::iterator it=g_ExtSynfileMap.find(ext.Upper());
    if(it!=g_ExtSynfileMap.end())
    {
        synfile=it->second;
    }

    if(!synfile.IsEmpty())
    {
        for(size_t idx=0;idx<g_SynPaths.size();idx++)
        {
            if(wxFileExists(g_SynPaths[idx]+synfile))
            {
                return new MadSyntax(g_SynPaths[idx]+synfile);
            }
        }
    }

    return NULL;
}

MadSyntax* MadSyntax::GetSyntaxByFirstLine(wxByte *data, int size)
{
    wxString line;
    while(size>0 && *data!=0x0D && *data!=0x0A && *data!=0)
    {
        line<<wxChar(*data);
        ++data;
        --size;
    }

    wxString synfile;

    if(!line.IsEmpty() && g_FirstlineSynfileMap.size()!=0)
    {
        StringMap::iterator it=g_FirstlineSynfileMap.begin();
        do
        {
            if(line.Find(it->first.c_str())>=0)
            {
                synfile=it->second;
                break;
            }
        }
        while(++it!=g_FirstlineSynfileMap.end());
    }

    if(!synfile.IsEmpty())
    {
        for(size_t idx=0;idx<g_SynPaths.size();idx++)
        {
            if(wxFileExists(g_SynPaths[idx]+synfile))
            {
                return new MadSyntax(g_SynPaths[idx]+synfile);
            }
        }
    }

    return NULL;
}

MadSyntax* MadSyntax::GetSyntaxByFileName(const wxString &filename)
{
    if(!s_Loaded) LoadSyntaxFiles();

    wxString synfile;

    StringMap::iterator it=g_FilenameSynfileMap.find(filename.Upper());
    if(it!=g_FilenameSynfileMap.end())
    {
        synfile=it->second;
    }

    if(!synfile.IsEmpty())
    {
        for(size_t idx=0;idx<g_SynPaths.size();idx++)
        {
            if(wxFileExists(g_SynPaths[idx]+synfile))
            {
                return new MadSyntax(g_SynPaths[idx]+synfile);
            }
        }
    }

    return NULL;
}

size_t MadSyntax::GetSchemeCount()
{
    if(!s_Loaded) LoadSyntaxFiles();

    return g_NameSchfileTable.size();
}
wxString MadSyntax::GetSchemeName(size_t index)
{
    if(!s_Loaded) LoadSyntaxFiles();

    if(index<g_NameSchfileTable.size())
    {
        return g_NameSchfileTable[index].first;
    }
    return wxEmptyString;
}

wxString MadSyntax::GetSchemeFileByName(const wxString &schname, MadSyntax *default_syn, bool &star)
{
    if(!s_Loaded) LoadSyntaxFiles();

    wxString name=schname;
    if(name.Right(1)==wxT('*')) name=schname.Left(schname.Len()-1);

    if(name==wxT("Default"))
    {
        star=true;
        if(default_syn) return GetSyntaxFileByTitle(default_syn->m_Title);
        return wxEmptyString;
    }

    for(size_t i=0; i<g_NameSchfileTable.size(); i++)
    {
        wxFileName fn(g_NameSchfileTable[i].second);
        wxString n=fn.GetName();
        if(n==name) 
        {
            star= (g_NameSchfileTable[i].first.Last()==wxT('*'));
            return g_NameSchfileTable[i].second;
        }
    }
    star=false;
    return wxEmptyString;
}

bool MadSyntax::LoadScheme(const wxString &schname, MadSyntax *syn)
{
    bool star;
    wxString schfile=GetSchemeFileByName(schname, syn, star);

    if(schfile.IsEmpty() && syn->m_Title==MadPlainTextTitle)
    {
        if(schname==wxT("Default*") || schname==wxT("Default"))
        {
            MadSyntax *sch=new MadSyntax(false);
            syn->AssignAttributes(sch);
            delete sch;
            return true;
        }
    }

    if(schfile.IsEmpty() || !wxFileExists(schfile)) return false;

    MadSyntax *sch=new MadSyntax(schfile, false);
    syn->AssignAttributes(sch);
    delete sch;
    return true;
}

bool MadSyntax::SaveScheme(const wxString &schname, MadSyntax *syn)
{
    wxASSERT(syn!=NULL);

    wxString name=schname;
    if(name.Right(1)==wxT('*')) name=schname.Left(schname.Len()-1);

    bool star;
    wxString schfile=GetSchemeFileByName(name, syn, star);

    if(star || name.IsEmpty()) return false;

    if(schfile.IsEmpty() || !wxFileExists(schfile))
    {
        MadSyntax *sch=new MadSyntax(false);
        size_t i;
        
        MadSyntaxRange ra;
        ra.bgcolor=*wxWHITE;
        for(i=sch->m_CustomRange.size(); i<5; i++)
        {
            sch->m_CustomRange.push_back(ra);
        }
        
        MadSyntaxKeyword ke;
        ke.m_Attr.color=*wxBLACK;
        ke.m_Attr.bgcolor=wxNullColour;
        ke.m_Attr.style=fsNone;
        for(i=sch->m_CustomKeyword.size(); i<10; i++)
        {
            sch->m_CustomKeyword.push_back(ke);
        }
        
        sch->AssignAttributes(syn, true);
        if(schfile.IsEmpty())
        {
            schfile = s_AttributeFilePath +name +wxT(".sch");
            g_NameSchfileTable.push_back(pair<wxString, wxString>(name, schfile));
        }
        sch->SaveAttributes(schfile);
        delete sch;
    }
    else
    {
        MadSyntax *sch=new MadSyntax(schfile, false);
        sch->AssignAttributes(syn, true);
        sch->SaveAttributes(schfile);
        delete sch;
    }

    if(!wxFileExists(schfile)) return false;
    return true;
}

bool MadSyntax::DeleteScheme(const wxString &schname)
{
    bool star;
    wxString schfile=GetSchemeFileByName(schname, NULL, star);

    if(star || schfile.IsEmpty()) return false;

    StringPairIterator it=g_NameSchfileTable.begin();
    do
    {
        if(it->second == schfile)
        {
            g_NameSchfileTable.erase(it);
            break;
        }
    }while(++it != g_NameSchfileTable.end());

    wxRemoveFile(schfile);
    return true;
}

//========================================================

MadSyntax::MadSyntax(const wxString &filename, bool loadAttr)
{
    LoadFromFile(filename);
    wxFileName fn(filename);
    if(loadAttr && fn.GetExt().CmpNoCase(wxT("syn"))==0)
    {
        LoadAttributes();
    }
}

MadSyntax::MadSyntax(bool loadAttr)
{
    Reset();    // use default attributes
    if(loadAttr) LoadAttributes();
}

MadSyntax::~MadSyntax()
{
    Reset();    // free memories
}

void MadSyntax::LoadFromFile(const wxString &filename)
{
    Reset();    // reset attributes
    ParseSyntax(filename);
}

void MadSyntax::ParseSyntax(const wxString &filename)
{
    wxFileConfig syn(wxEmptyString,wxEmptyString,filename,wxEmptyString,wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);

    wxString entry, value;

    m_Title=wxT("No Title");

    long idx=0;
    bool cont=syn.GetNextEntry(entry, idx);
    while(cont)
    {
        //wxLogMessage(entry);
        syn.Read(entry, &value);
        entry.MakeLower();
        value.Trim(true);
        value.Trim(false);

        if(entry == wxT("title"))
        {
            m_Title = value;
        }
        else if(entry == wxT("encoding"))
        {
            m_Encoding = value;
        }
        else if(entry == wxT("casesensitive"))
        {
            if(value.Lower() == wxT("yes"))
                m_CaseSensitive = true;
        }
        else if(entry == wxT("delimiter"))
        {
            m_Delimiter = value;
        }
        else if(entry == wxT("escapechar"))
        {
            m_EscapeChar = value;
            if(!m_EscapeChar.IsEmpty())
                nw_EscapeChar = m_EscapeChar[0];
        }
        else if(entry == wxT("stringchar"))
        {
            wxStringTokenizer tkz(value);
            wxString s=tkz.GetNextToken();
            while(!s.IsEmpty())
            {
                m_StringChar += s;
                s=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("directiveleading"))
        {
            m_DirectiveLeading = value;
        }
        else if(entry == wxT("directiveleadingatbol"))
        {
            if(value.Lower() == wxT("yes"))
                m_DirectiveLeadingAtBOL=true;
        }
        else if(entry == wxT("keywordprefix"))
        {
            m_KeywordPrefix = value;
        }
        else if(entry == wxT("specialwordprefix"))
        {
            m_SpecialWordPrefix = value;
        }
        else if(entry == wxT("indentchar"))
        {
            wxStringTokenizer tkz(value);
            wxString s=tkz.GetNextToken();
            while(!s.IsEmpty())
            {
                m_IndentChar += s;
                s=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("unindentchar"))
        {
            wxStringTokenizer tkz(value);
            wxString s=tkz.GetNextToken();
            while(!s.IsEmpty())
            {
                m_UnindentChar += s;
                s=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("bracepair"))
        {
            wxStringTokenizer tkz(value);
            wxString pl=tkz.GetNextToken();
            wxString pr=tkz.GetNextToken();
            while(!pl.IsEmpty() && !pr.IsEmpty())
            {
                m_LeftBrace.push_back(pl);
                m_RightBrace.push_back(pr);

                pl=tkz.GetNextToken();
                pr=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("autocompletepair"))
        {
            wxStringTokenizer tkz(value);
            wxString p=tkz.GetNextToken();
            while(!p.IsEmpty())
            {
                if(p.Len()==2)
                {
                    m_AutoCompleteLeftChar += p[0];
                    m_AutoCompleteRightChar += p[1];
                }

                p=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("customrange"))
        {
            wxStringTokenizer tkz(value);
            MadSyntaxRange ra;

            while(true)
            {
                wxString s=tkz.GetNextToken();
                if(!s.ToULong(&ra.id)) break;

                if((ra.begin=tkz.GetNextToken()).IsEmpty()) break;
                m_RangeBeginString.push_back(ra.begin);

                if((ra.end=tkz.GetNextToken()).IsEmpty()) break;

                if((s=tkz.GetNextToken()).IsEmpty()) break;
                SetColor(s, ra.bgcolor);

                m_CustomRange.push_back(ra);
            }
        }
        else if(entry == wxT("stringinrange"))
        {
            SetInRange(value, m_StringInRange);
        }
        else if(entry == wxT("linecomment"))
        {
            wxStringTokenizer tkz(value);
            wxString s=tkz.GetNextToken();
            while(!s.IsEmpty())
            {
                m_LineComment.push_back(s);
                s=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("linecommentinrange"))
        {
            SetInRange(value, m_LineCommentInRange);
        }
        else if(entry == wxT("linecommentatbol"))
        {
            if(value.Lower() == wxT("yes"))
                m_LineCommentAtBOL=true;
        }
        else if(entry == wxT("blockcomment"))
        {
            wxStringTokenizer tkz(value);
            wxString on=tkz.GetNextToken();
            wxString off=tkz.GetNextToken();
            while(!on.IsEmpty() && !off.IsEmpty())
            {
                m_BlockCommentOn.push_back(on);
                m_BlockCommentOff.push_back(off);

                on=tkz.GetNextToken();
                off=tkz.GetNextToken();
            }
        }
        else if(entry == wxT("blockcommentinrange"))
        {
            wxStringTokenizer tkz(value, wxT(","), wxTOKEN_RET_EMPTY_ALL);
            size_t idx=0;
            while(tkz.HasMoreTokens())
            {
                wxString s=tkz.GetNextToken();
                m_BlockCommentInRange.push_back(vector<int>());
                SetInRange(s, m_BlockCommentInRange[idx++]);
            }
        }
        else
        {
            wxString s;
            int attr=0, alen=5;
            if(entry.Right(7)==wxT("bgcolor"))
            {
                attr=1;
                alen=7;
            }
            else if((s=entry.Right(5))==wxT("color"))
            {
                attr=2;
            }
            else if(s==wxT("style"))
            {
                attr=3;
            }

            int elen=(int)entry.size();
            if(attr!=0 && (elen-=alen)>0)
            {
                MadAttributes *at = GetAttributes(entry.Left(elen));
                if(at)
                {
                    switch(attr)
                    {
                    case 1:
                        SetColor(value, at->bgcolor);
                        break;
                    case 2:
                        SetColor(value, at->color);
                        break;
                    case 3:
                        SetStyle(value, at->style);
                        break;
                    }
                }
            }

        }

        cont=syn.GetNextEntry(entry, idx);
    }

    size_t kwlen;
    nw_MaxKeywordLen=0;

    // load custom keywords
    wxString group;
    long gidx=0;
    bool gcont=syn.GetNextGroup(group, gidx);
    while(gcont)
    {
        //wxLogMessage(entry);
        MadSyntaxKeyword *ck = GetCustomKeyword(group);
        ck->m_CaseSensitive = m_CaseSensitive;

        syn.SetPath(wxString(wxT('/'))+group);
        idx=0;
        cont=syn.GetNextEntry(entry, idx);
        while(cont)
        {
            syn.Read(entry, &value);
            entry.MakeLower();
            value.Trim(true);
            value.Trim(false);

            if(entry == wxT("color"))
            {
                SetColor(value, ck->m_Attr.color);
            }
            else if(entry == wxT("style"))
            {
                SetStyle(value, ck->m_Attr.style);
            }
            else if(entry == wxT("inrange"))
            {
                SetInRange(value, ck->m_InRange);
            }
            else if(entry == wxT("casesensitive"))
            {
                value.MakeLower();
                if(value==wxT("yes")) ck->m_CaseSensitive=true;
                else if(value==wxT("no")) ck->m_CaseSensitive=false;
            }
            else if(entry == wxT("keyword"))
            {
                wxStringTokenizer tkz(value);
                wxString kw=tkz.GetNextToken();
                while(!kw.IsEmpty())
                {
                    if((kwlen=kw.size())>nw_MaxKeywordLen)
                    {
                        nw_MaxKeywordLen=kwlen;
                    }

                    if(!ck->m_CaseSensitive)
                        kw.MakeLower();

                    ck->m_Keywords.insert(kw);

                    kw=tkz.GetNextToken();
                }

            }

            cont=syn.GetNextEntry(entry, idx);
        }

        syn.SetPath(wxString(wxT('/')));
        gcont=syn.GetNextGroup(group, gidx);
    }

    // syn loaded, begin post-processing

    if(m_SystemAttributes[aeText].color==wxNullColour ||
        m_SystemAttributes[aeText].bgcolor==wxNullColour)
    {
        m_SystemAttributes[aeText].color=*wxBLACK;
        m_SystemAttributes[aeText].bgcolor=*wxWHITE;
    }

    vector < MadSyntaxKeyword >::iterator kit = m_CustomKeyword.begin();
    vector < MadSyntaxKeyword >::iterator kend = m_CustomKeyword.end();
    while(kit != kend)
    {
        if(kit->m_InRange.empty())
            kit->m_InRange.push_back(0); // Keyword have effect in global range

        ++kit;
    }

    if(m_StringInRange.empty())
        m_StringInRange.push_back(0); // String have effect in global range

    if(!m_LineComment.empty() && m_LineCommentInRange.empty())
        m_LineCommentInRange.push_back(0);        // m_LineComment have effect in global range

    size_t si = m_BlockCommentOn.size();
    if(si > m_BlockCommentInRange.size())
    {
        m_BlockCommentInRange.resize(si);
        for(size_t sz = 0; sz < si; sz++)
        {
            if(m_BlockCommentInRange[sz].empty())
                m_BlockCommentInRange[sz].push_back(0);     // BlockComment[i] have effect in global range
        }
    }


    vector < wxString >::iterator sit, send;
    if(m_CaseSensitive == false)
    {
        // change string data to lowercase
        sit = m_LineComment.begin();
        send = m_LineComment.end();
        while(sit != send)
        {
            sit->MakeLower();
            ++sit;
        }

        sit = m_BlockCommentOn.begin();
        send = m_BlockCommentOn.end();
        while(sit != send)
        {
            sit->MakeLower();
            ++sit;
        }

        sit = m_BlockCommentOff.begin();
        send = m_BlockCommentOff.end();
        while(sit != send)
        {
            sit->MakeLower();
            ++sit;
        }

        sit = m_LeftBrace.begin();
        send = m_LeftBrace.end();
        while(sit != send)
        {
            sit->MakeLower();
            ++sit;
        }

        sit = m_RightBrace.begin();
        send = m_RightBrace.end();
        while(sit != send)
        {
            sit->MakeLower();
            ++sit;
        }
    }

    if(!m_LineComment.empty() || !m_BlockCommentOn.empty() || !m_StringChar.IsEmpty()
        || !m_DirectiveLeading.IsEmpty() || !m_CustomRange.empty() || !m_LeftBrace.empty())
    {
        m_CheckState = true;
    }

}

void MadSyntax::Reset()
{
    m_Title = MadPlainTextTitle;
    m_CaseSensitive = false;
    m_Delimiter = wxT("~`!@#$%^&*()-+=|\\{}[]:;\"\',.<>/?");

    m_LineComment.clear();
    m_BlockCommentOn.clear();
    m_BlockCommentOff.clear();
    m_EscapeChar.clear();
    m_StringChar.clear();
    m_DirectiveLeading.clear();
    m_KeywordPrefix.clear();
    m_SpecialWordPrefix.clear();
    m_IndentChar.clear();
    m_UnindentChar.clear();
    m_LeftBrace.clear();
    m_RightBrace.clear();
    m_AutoCompleteLeftChar.clear();
    m_AutoCompleteRightChar.clear();

    m_Encoding.clear();

    nw_EscapeChar=0xFFFFFFFF;

    m_StringInRange.clear();
    m_LineCommentInRange.clear();

    size_t i;
    for(i = 0; i < m_BlockCommentInRange.size(); i++)
    {
        m_BlockCommentInRange[i].clear();
    }
    m_BlockCommentInRange.clear();

    m_LineCommentAtBOL=false;
    m_DirectiveLeadingAtBOL=false;

    m_CustomRange.clear();
    m_RangeBeginString.clear();

    for(i = 0; i < m_CustomKeyword.size(); i++)
    {
        m_CustomKeyword[i].m_InRange.clear();
        m_CustomKeyword[i].m_Keywords.clear();
    }
    m_CustomKeyword.clear();

    m_CheckState = false;

    nw_MaxKeywordLen=0;

    // default Text color bgcolor
    MadAttributes *pat = m_SystemAttributes;

    for(i = 0; i < aeNone; i++)
    {
        if(SystemAttributesColor[i][0]==0)
            pat->color = wxNullColour;
        else if(SystemAttributesColor[i][0]==wxT('#'))
            pat->color.Set(SystemAttributesColor[i][1], SystemAttributesColor[i][2], SystemAttributesColor[i][3]);
        else
            pat->color = wxColour(SystemAttributesColor[i]);

        if(SystemAttributesBgColor[i][0]==0)
            pat->bgcolor = wxNullColour;
        else if(SystemAttributesBgColor[i][0]==wxT('#'))
            pat->bgcolor.Set(SystemAttributesBgColor[i][1], SystemAttributesBgColor[i][2], SystemAttributesBgColor[i][3]);
        else
            pat->bgcolor = wxColour(SystemAttributesBgColor[i]);

        pat->style=fsNone;
        pat++;
    }
}

MadAttributes *MadSyntax::GetAttributes(const wxString &name)
{
    for(int i = 0; i < aeNone; i++)
    {
        if(name.CmpNoCase(SystemAttributesName[i])==0)
            return &m_SystemAttributes[i];
    }
    return NULL;
}

MadSyntaxKeyword *MadSyntax::GetCustomKeyword(const wxString &name)
{
    vector < MadSyntaxKeyword >::iterator it = m_CustomKeyword.begin();
    while(it != m_CustomKeyword.end())
    {
        if(it->m_Name == name)
        {
            return &(*it);
        }
        ++it;
    }

    it = m_CustomKeyword.insert(m_CustomKeyword.end(), MadSyntaxKeyword());
    it->m_Name = name;
    return &(*it);
}

void MadSyntax::SetColor(const wxString &value, wxColour &c)
{
    c=wxTheColourDatabase->Find(value);
    if(!c.Ok() && !value.IsEmpty())
    {
        unsigned long ul;
        if(value.ToULong(&ul,16))
        {
            c.Set((ul>>16)&0xFF, (ul>>8)&0xFF, ul&0xFF);
        }
    }
}

void MadSyntax::SetStyle(const wxString &value, MadFontStyles &fs)
{
    wxStringTokenizer tkz(value);
    wxString s=tkz.GetNextToken();

    while(!s.IsEmpty())
    {
        s.MakeLower();

        if(s == wxT("bold"))
        {
            fs |= fsBold;
        }
        else if(s == wxT("italic"))
        {
            fs |= fsItalic;
        }
        else if(s == wxT("underline"))
        {
            fs |= fsUnderline;
        }
        else if(s == wxT("strikeout"))
        {
            fs |= fsStrikeOut;
        }

        s=tkz.GetNextToken();
    }
}

void MadSyntax::SetInRange(const wxString &value, vector < int > &ir)
{
    wxStringTokenizer tkz(value);
    wxString s=tkz.GetNextToken();
    unsigned long ul;

    while(!s.IsEmpty())
    {
        if(s.ToULong(&ul))
        {
            ir.push_back(ul);
        }
        s=tkz.GetNextToken();
    }
}

bool MadSyntax::IsInRange(int range, vector < int >&InRangeVector)
{
    if(!InRangeVector.empty())
    {
        vector < int >::iterator it = InRangeVector.begin();
        do
        {
            if(range == *it) return true;
            ++it;
        }
        while(it != InRangeVector.end());
    }
    return false;
}

MadSyntaxRange *MadSyntax::GetSyntaxRange(int rangeid)
{
    if(!m_CustomRange.empty())
    {
        vector < MadSyntaxRange >::iterator it = m_CustomRange.begin();
        do
        {
            if((int)it->id == rangeid) return &(*it);
            ++it;
        }
        while(it != m_CustomRange.end());
    }
    return NULL;
}

wxString MadSyntax::GetAttributeName(MadAttributeElement ae)
{
    return wxString(SystemAttributesName[ae]);
}

void MadSyntax::SetAttributes(MadAttributes *attr)
{
    if(attr == m_SystemAttributes + aeText)
    {
        nw_BgColor = nw_CurrentBgColor;
    }
    else 
    {
        nw_BgColor = attr->bgcolor;
    }
    if(nw_BgColor==wxNullColour)
    {
        nw_BgColor = nw_CurrentBgColor;
    }

    if((nw_Color = attr->color) == wxNullColour)
    {
        nw_Color = m_SystemAttributes[aeText].color;
    }

    //if((attr->style&fsStrikeOut)!=0)
    
    if( attr != &m_SystemAttributes[aeSpace] &&
        attr != &m_SystemAttributes[aeActiveLine] &&
        attr != &m_SystemAttributes[aeBookmark] )
    {
        nw_Font=wxTheFontList->FindOrCreateFont(nw_FontSize, nw_FontFamily,
            (attr->style&fsItalic) == 0 ? wxFONTSTYLE_NORMAL : wxFONTSTYLE_ITALIC, 
            (attr->style&fsBold) == 0 ? wxFONTWEIGHT_NORMAL : wxFONTWEIGHT_BOLD,
            (attr->style&fsUnderline) != 0,
            nw_FontName);
    }
}


void MadSyntax::InitNextWord1(MadLines *madlines, ucs4_t *word, int *widths, const wxString &fontname, int fontsize, int fontfamily)
{
    nw_MadLines=madlines;
    nw_MadEdit=madlines->m_MadEdit;
    nw_Encoding=nw_MadEdit->m_Encoding;
    nw_Word=word;
    nw_Widths=widths;
    nw_FontName=fontname;
    nw_FontSize=fontsize;
    nw_FontFamily=fontfamily;
    nw_Font=wxTheFontList->FindOrCreateFont(nw_FontSize, nw_FontFamily,
        wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, nw_FontName);

    if(m_CaseSensitive)
        FindString=&MadSyntax::FindStringCase;
    else
        FindString=&MadSyntax::FindStringNoCase;
}

void MadSyntax::InitNextWord2(MadLineIterator &lit, size_t row)
{
    nw_LineIter = lit;

    nw_State.Reset();

    nw_CommentUntilEOL = false;
    nw_BeginOfLine = (row==0);
    nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor; // set bgcolor for empty lines

    nw_RowIndexIter = lit->m_RowIndices.begin();
    std::advance(nw_RowIndexIter, row);
    nw_MadLines->InitNextUChar(lit, nw_RowIndexIter->m_Start);

    nw_NotSpaceCount = 0;
    nw_LineWidth = 0;
    nw_FirstIndex = 0;
    nw_RestCount = 0;
    nw_MaxLength = lit->m_RowIndices[row+1].m_Start - nw_RowIndexIter->m_Start;

    if(m_CheckState)
    {
        MadLineState &state = lit->m_State;

        nw_SynRange = 0;
        if((nw_State.rangeid = state.RangeId) != 0)
        {
            nw_SynRange = GetSyntaxRange(nw_State.rangeid);
            nw_CurrentBgColor = nw_SynRange->bgcolor; // set bgcolor for empty lines
        }

        if((nw_State.blkcmtid = state.CommentId) != 0)
        {
            if(m_SystemAttributes[aeComment].bgcolor != wxNullColour)
            {
                nw_CurrentBgColor = m_SystemAttributes[aeComment].bgcolor; // set bgcolor for empty lines
            }
        }
        nw_ContainCommentOff = (state.CommentOff!=0);

        if((nw_State.stringid = state.StringId) != 0)
        {
            nw_StringChar = m_StringChar[nw_State.stringid-1];
            if(m_SystemAttributes[aeString].bgcolor != wxNullColour)
            {
                nw_CurrentBgColor = m_SystemAttributes[aeString].bgcolor; // set bgcolor for empty lines
            }
        }

        if((nw_State.linecmt = state.LineComment) !=0)
        {
            nw_CommentUntilEOL = true;
        }

        nw_State.directive = state.Directive;
    }
    nw_NextState = nw_State;

    nw_EndOfLine = false;
    if(nw_RowIndexIter->m_Width == 0)
    {
        nw_EndOfLine = true;
    }

    if(!nw_ucqueue.empty())
    {
        nw_ucqueue.clear();
    }
}

void MadSyntax::SetEncoding(MadEncoding *encoding)
{
    nw_Encoding=encoding;
}

// 0: none, 1: first, 2:...
int MadSyntax::FindStringCase(MadUCQueue & ucqueue, size_t first,
                        MadStringIterator begin, const MadStringIterator & end,
                        size_t & len)
{
    int idx = 1;
    bool bNextUC, bNotNewLine;
    ucs4_t uc, firstuc = ucqueue[first].first;
    size_t ucsize = ucqueue.size() - first;

    wxASSERT(ucsize > 0);

    do
    {
        const wchar_t *cstr = begin->c_str();
        len = begin->size();

        if(firstuc == (ucs4_t)cstr[0])
        {
            if(len == 1) return idx;

            uc = ucqueue.back().first;

            bNextUC = true;
            bNotNewLine = (uc != 0x0D && uc != 0x0A);

            if(ucsize < len && bNotNewLine)
                do
                {
                    bNextUC = (nw_MadLines->*(nw_MadLines->NextUChar))(ucqueue);
                    uc = ucqueue.back().first;
                    bNotNewLine = (uc != 0x0D && uc != 0x0A);
                }
                while(bNextUC && bNotNewLine && (++ucsize) < len
                            && uc == (ucs4_t)cstr[ucsize - 1]);

            if(bNextUC && ucsize >= len)
                if(bNotNewLine || ucsize > len)
                {
                    deque < MadUCPair >::iterator it = ucqueue.begin();
                    std::advance(it, first + 1);
                    while(*(++cstr) != 0)
                    {
                        if((ucs4_t)*cstr != it->first)
                            break;
                        ++it;
                    }
                    if(*cstr == 0)
                        return idx;
                }
        }

        ++idx;
    }
    while(++begin != end);

    return 0;
}

int MadSyntax::FindStringNoCase(MadUCQueue & ucqueue, size_t first,
                   MadStringIterator begin, const MadStringIterator & end,
                   size_t & len)
{
    int idx = 1;
    bool bNextUC, bNotNewLine;
    ucs4_t uc, firstuc = ucqueue[first].first;

    if(firstuc>='A' && firstuc<='Z')
    {
        firstuc |= 0x20;    // to lower case
    }

    size_t ucsize = ucqueue.size() - first;
    wxASSERT(ucsize > 0);

    do
    {
        const wchar_t *cstr = begin->c_str();
        len = begin->size();

        if(firstuc == (ucs4_t)cstr[0])
        {
            if(len == 1) return idx;

            uc = ucqueue.back().first;
            if(uc>='A' && uc<='Z')
            {
                uc |= 0x20; // to lower case
            }

            bNextUC = true;
            bNotNewLine = (uc != 0x0D && uc != 0x0A);

            if(ucsize < len && bNotNewLine)
                do
                {
                    bNextUC = (nw_MadLines->*(nw_MadLines->NextUChar))(ucqueue);
                    uc = ucqueue.back().first;
                    if(uc>='A' && uc<='Z')
                    {
                        uc |= 0x20; // to lower case
                    }

                    bNotNewLine = (uc != 0x0D && uc != 0x0A);
                }
                while(bNextUC && bNotNewLine && (++ucsize) < len
                            && uc == (ucs4_t)cstr[ucsize - 1]);

            if(bNextUC && ucsize >= len)
                if(bNotNewLine || ucsize > len)
                {
                    deque < MadUCPair >::iterator it = ucqueue.begin();
                    std::advance(it, first + 1);
                    while(*(++cstr) != 0)
                    {
                        uc = it->first;
                        if(uc>='A' && uc<='Z')
                        {
                            uc |= 0x20; // to lower case
                        }

                        if((ucs4_t)*cstr != uc)
                            break;
                        ++it;
                    }
                    if(*cstr == 0)
                        return idx;
                }
        }

        ++idx;
    }
    while(++begin != end);

    return 0;
}

// return wordlength
int MadSyntax::NextWord(int &wordwidth)
{
    wordwidth = 0;

    if(nw_RestCount == 0)
    {
        if(nw_EndOfLine && nw_ucqueue.empty())
            return 0;

        MadStringIterator sit, sitend;
        size_t strlen;
        int idx;

        nw_RestCount += nw_FirstIndex;

        do
        {
            if(nw_EndOfLine == false)
            {
                (nw_MadLines->*(nw_MadLines->NextUChar))(nw_ucqueue);
            }

            size_t ucsize = nw_ucqueue.size();
            if(ucsize)
            {
                ucs4_t lastuc = nw_ucqueue.back().first;
                if(lastuc == 0x0D || lastuc == 0x0A)
                {
                    nw_ucqueue.pop_back();
                    --ucsize;
                }
            }

            if(nw_FirstIndex >= ucsize)
            {
                //nw_EndOfLine = true;

                if(nw_RestCount == 0)
                    return 0;

                break;
            }

            if(!nw_CommentUntilEOL)
            {
                ucs4_t firstuc = nw_ucqueue[nw_FirstIndex].first;

                if( /*firstuc < 0x100 && */ !IsSpace(firstuc))
                {
                    ++nw_NotSpaceCount;

                    if(m_CheckState && firstuc < 0x100)
                    {
                        // check EscapeChar
                        if(firstuc == nw_EscapeChar)
                        {
                            (nw_MadLines->*(nw_MadLines->NextUChar))(nw_ucqueue);
                            idx = (int)nw_FirstIndex + 1;
                            if(nw_ucqueue.size() > (size_t) idx)
                            {
                                ucs4_t c = nw_ucqueue[idx].first;
                                if(c!=0x0D && c!=0x0A)
                                {
                                    ++nw_FirstIndex;
                                    ++nw_RestCount;
                                }
                            }
                            goto _NEXTUCHAR_;
                        }

                        // check String Off
                        if(nw_State.stringid != 0)
                        {
                            if(firstuc == nw_StringChar)
                            {
                                nw_NextState.stringid = 0;
                                ++nw_FirstIndex;
                                ++nw_RestCount;
                                break;
                            }
                            goto _NEXTUCHAR_;
                        }

                        // check Block Comment Off
                        if(nw_State.blkcmtid != 0)
                        {
                            if(!nw_ContainCommentOff)
                            {
                                nw_CommentUntilEOL = true;
                                goto _NEXTUCHAR_;
                            }

                            sit = m_BlockCommentOff.begin();
                            std::advance(sit, nw_State.blkcmtid - 1);
                            sitend = sit;
                            ++sitend;
                            if((this->*FindString)(nw_ucqueue, nw_FirstIndex, sit, sitend, strlen) != 0)
                            {
                                nw_NextState.blkcmtid = 0;
                                nw_FirstIndex += strlen;
                                nw_RestCount = nw_FirstIndex;
                                break;
                            }
                            goto _NEXTUCHAR_;
                        }

                        // check Range Off
                        if(nw_State.rangeid != 0)
                        {
                            if(nw_SynRange)
                            {
                                vector < wxString > strvec;
                                strvec.push_back(nw_SynRange->end);
                                if((this->*FindString)(nw_ucqueue, nw_FirstIndex,
                                    strvec.begin(), strvec.end(), strlen) != 0)
                                {
                                    nw_NextState.rangeid = 0;
                                    nw_FirstIndex += strlen;
                                    nw_RestCount = nw_FirstIndex;
                                    break;
                                }
                            }
                        }

                        // check Directive
                        if(!m_DirectiveLeadingAtBOL||nw_BeginOfLine)
                        {
                            if(nw_NotSpaceCount == 1 && !m_DirectiveLeading.IsEmpty())
                            {
                                if(m_DirectiveLeading.Find(firstuc) >= 0)
                                {
                                    nw_NextState.directive=1;
                                    nw_FirstIndex += 1;

                                    if(nw_RestCount == 0)
                                    {
                                        nw_State = nw_NextState;
                                        nw_RestCount = nw_FirstIndex;
                                        continue;
                                    }

                                    break;
                                }
                            }
                        }

                        // check Block Comment On
                        if(!m_BlockCommentOn.empty())
                        {
                            idx =(this->*FindString)(nw_ucqueue, nw_FirstIndex,
                                m_BlockCommentOn.begin(), m_BlockCommentOn.end(), strlen);
                            if(idx != 0)
                            {
                                // check InRange
                                if(IsInRange(nw_State.rangeid, m_BlockCommentInRange[idx - 1]))
                                {
                                    nw_NextState.blkcmtid = idx;
                                    nw_FirstIndex += strlen;

                                    if(nw_RestCount == 0)
                                    {
                                        nw_State = nw_NextState;
                                        nw_RestCount = nw_FirstIndex;
                                        continue;
                                    }

                                    break;
                                }
                            }
                        }

                        // check Line Comment On
                        if(!m_LineCommentAtBOL||nw_BeginOfLine)
                        {
                            if(!m_LineComment.empty())
                            {
                                idx =(this->*FindString)(nw_ucqueue, nw_FirstIndex,
                                    m_LineComment.begin(), m_LineComment.end(), strlen);
                                if(idx != 0)
                                {
                                    if(IsInRange(nw_State.rangeid, m_LineCommentInRange))
                                    {
                                        // range-off mark may be beyond linecomment (ex: javascript)
                                        if(nw_State.rangeid==0)
                                        {
                                            nw_CommentUntilEOL = true;
                                        }
                                        nw_NextState.linecmt = idx;
                                        nw_FirstIndex += strlen;

                                        if(nw_RestCount == 0)
                                        {
                                            nw_State = nw_NextState;
                                            nw_RestCount = nw_FirstIndex;
                                            continue;
                                        }

                                        break;
                                    }
                                }
                            }
                        }

                        // check String On
                        if(!m_StringChar.IsEmpty()
                             && IsInRange(nw_State.rangeid, m_StringInRange))
                        {
                            if((idx = m_StringChar.Find(firstuc)+1) > 0)
                            {
                                nw_NextState.stringid = idx;
                                nw_StringChar = firstuc;
                                ++nw_FirstIndex;

                                if(nw_RestCount == 0)
                                {
                                    nw_State = nw_NextState;
                                    nw_RestCount = nw_FirstIndex;
                                    continue;
                                }

                                break;
                            }
                        }

                        // check Range On
                        if(nw_State.rangeid == 0 && !m_RangeBeginString.empty())
                        {
                            idx = (this->*FindString)(nw_ucqueue, nw_FirstIndex,
                                m_RangeBeginString.begin(), m_RangeBeginString.end(), strlen);
                            if(idx != 0)
                            {
                                nw_NextState.rangeid = m_CustomRange[idx - 1].id;
                                nw_SynRange = GetSyntaxRange(nw_NextState.rangeid);

                                nw_FirstIndex += strlen;

                                if(nw_RestCount == 0)
                                {
                                    nw_State = nw_NextState;
                                    nw_RestCount = nw_FirstIndex;
                                    continue;
                                }

                                break;
                            }
                        }
                    }
                }
            }
        _NEXTUCHAR_:
            ++nw_FirstIndex;
            ++nw_RestCount;
            nw_BeginOfLine=false;
        }
        while(nw_RestCount < nw_MaxLength);
    }

    wxASSERT(nw_RestCount != 0);
    wxASSERT(nw_RowIndexIter->m_Width != 0);

    nw_BeginOfLine=false;

    // set current bgcolor by state
    bool setbgcolor=false;
    if(nw_State.blkcmtid!=0 || nw_State.linecmt!=0)
    {
        nw_CurrentBgColor = m_SystemAttributes[aeComment].bgcolor;
        if(nw_CurrentBgColor!=wxNullColour) setbgcolor=true;
    }
    if(setbgcolor==false)
    {
        if(nw_State.directive)
        {
            nw_CurrentBgColor = m_SystemAttributes[aeDirective].bgcolor;
            if(nw_CurrentBgColor!=wxNullColour) setbgcolor=true;
        }
        else if(nw_State.stringid!=0)
        {
            nw_CurrentBgColor = m_SystemAttributes[aeString].bgcolor;
            if(nw_CurrentBgColor!=wxNullColour) setbgcolor=true;
        }
    }
    if(setbgcolor==false)
    {
        if(nw_State.rangeid!=0)
        {
            MadSyntaxRange * r=GetSyntaxRange(nw_State.rangeid);
            nw_CurrentBgColor = r->bgcolor;
            //if(nw_CurrentBgColor==wxNullColour)
            //{
                //nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
            //}
        }
        else
        {
            nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
        }
    }


    // now the nw_ucqueue is the wanted Word
    // calc the width/length/type and return the word

    int idx = 0;
    int old_line_width = nw_LineWidth;

    int width;
    ucs4_t uc = nw_ucqueue.front().first;
    if(IsSpace(uc))
    {
        SetAttributes(aeSpace);

        do
        {
            nw_Word[idx] = uc;
            nw_ucqueue.pop_front();

            width = nw_MadEdit->GetUCharWidth(uc);
            if(uc == 0x09)
            {
                int tabwidth = nw_MadEdit->m_TabColumns * nw_MadEdit->GetUCharWidth(0x20);
                width = nw_RowIndexIter->m_Width - nw_LineWidth;
                tabwidth -= (nw_LineWidth % tabwidth);
                if(tabwidth < width)
                    width = tabwidth;
            }
            nw_Widths[idx] = width;

            nw_LineWidth += width;
            ++idx;
            --nw_FirstIndex;
        }
        while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                    && IsSpace(uc = nw_ucqueue.front().first));

        nw_Word[idx] = 0;

    }
    else if(nw_State.blkcmtid != 0 || nw_State.linecmt != 0)
    {
        SetAttributes(aeComment);

        do
        {
            nw_Word[idx] = uc;
            nw_ucqueue.pop_front();

            width = nw_MadEdit->GetUCharWidth(uc);
            nw_Widths[idx] = width;
            nw_LineWidth += width;
            ++idx;
            --nw_FirstIndex;
        }
        while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                    && !IsSpace(uc = nw_ucqueue.front().first));

        nw_Word[idx] = 0;

    }
    else if(nw_State.directive)
    {
        SetAttributes(aeDirective);

        do
        {
            nw_Word[idx] = uc;
            nw_ucqueue.pop_front();

            width = nw_MadEdit->GetUCharWidth(uc);
            nw_Widths[idx] = width;
            nw_LineWidth += width;
            ++idx;
            --nw_FirstIndex;
        }
        while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                    && !IsSpace(uc = nw_ucqueue.front().first));

        nw_Word[idx] = 0;

    }
    else if(nw_State.stringid != 0)
    {
        SetAttributes(aeString);

        do
        {
            nw_Word[idx] = uc;
            nw_ucqueue.pop_front();

            width = nw_MadEdit->GetUCharWidth(uc);
            nw_Widths[idx] = width;
            nw_LineWidth += width;
            ++idx;
            --nw_FirstIndex;
        }
        while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                    && !IsSpace(uc = nw_ucqueue.front().first));

        nw_Word[idx] = 0;

    }
    else if(uc < 0x100)
    {

        if(uc >= '0' && uc <= '9')
        {
            SetAttributes(aeNumber);

            do
            {
                nw_Word[idx] = uc;
                nw_ucqueue.pop_front();

                width = nw_MadEdit->GetUCharWidth(uc);
                nw_Widths[idx] = width;
                nw_LineWidth += width;
                ++idx;
                --nw_FirstIndex;
            }
            while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                        && IsNotDelimiter(uc = nw_ucqueue.front().first));

            nw_Word[idx] = 0;

        }
        else if(m_SpecialWordPrefix.Find(uc)>=0)
        {
            SetAttributes(aeSpecialWord);

            do
            {
                nw_Word[idx] = uc;
                nw_ucqueue.pop_front();

                width = nw_MadEdit->GetUCharWidth(uc);
                nw_Widths[idx] = width;
                nw_LineWidth += width;
                ++idx;
                --nw_FirstIndex;
            }
            while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                        && IsNotDelimiter(uc = nw_ucqueue.front().first));

            nw_Word[idx] = 0;

        }
        else if(m_KeywordPrefix.Find(uc)>=0)
        {
            size_t old_firstindex = nw_FirstIndex;
            size_t old_rest_count = nw_RestCount;
            int old_line_width = nw_LineWidth;
            // get full word
            do
            {
                nw_Word[idx] = uc;
                //nw_ucqueue.pop_front();

                width = nw_MadEdit->GetUCharWidth(uc);
                nw_Widths[idx] = width;
                nw_LineWidth += width;
                ++idx;
                --nw_FirstIndex;
            }
            while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                        && (uc = nw_ucqueue.front().first) < 0x100 && !IsSpace(uc));

            nw_Word[idx] = 0;

            // check if is Keyword
            vector < MadSyntaxKeyword >::iterator kit = m_CustomKeyword.begin();
            vector < MadSyntaxKeyword >::iterator kend = m_CustomKeyword.end();
            bool bIsKeyword = false;

            if(nw_MaxKeywordLen != 0 && idx <= (int)nw_MaxKeywordLen && kit != kend)
            {
                wxString strorg(wxT(' '), idx), strlower(wxT(' '), idx);
                ucs4_t *puc=nw_Word;
                for(int i=0; i<idx; ++i)
                {
                    ucs4_t uc = *puc++;
#ifdef __WXMSW__
                    if(uc < 0x10000)
#endif
                    {
                        strorg[i] = wxChar(uc);

                        if(uc<=(ucs4_t)wxT('Z') && uc>=(ucs4_t)wxT('A'))
                            strlower[i] = wxChar(uc|0x20);
                        else
                            strlower[i] = wxChar(uc);
                    }
#ifdef __WXMSW__
                    else
                    {
                        //to surrogates????
                        strorg[i] = wxChar(0xFFFF);
                        strlower[i] = wxChar(0xFFFF);
                    }
#endif
                }

                MadKeywordSet::iterator it;
                do
                {
                    if(IsInRange(nw_State.rangeid, kit->m_InRange))
                    {
                        if(kit->m_CaseSensitive) it = kit->m_Keywords.find(strorg);
                        else                     it = kit->m_Keywords.find(strlower);
                        if(it != kit->m_Keywords.end())
                        {
                            bIsKeyword = true;
                            break;
                        }
                    }
                }
                while(++kit != kend);
            }

            if(bIsKeyword)
            {
                SetAttributes(&(kit->m_Attr));
                int i = 0;
                while(i < idx)
                {
                    nw_ucqueue.pop_front();
                    ++i;
                }
            }
            else
            {
                SetAttributes(aeDelimiter);
                idx = 1;
                nw_Word[1] = 0;
                nw_ucqueue.pop_front();
                nw_FirstIndex = old_firstindex - 1;
                nw_RestCount = old_rest_count - 1;
                nw_LineWidth = old_line_width + nw_MadEdit->GetUCharWidth(nw_Word[0]);
            }

        }
        else if(IsDelimiter(uc))
        {
            SetAttributes(aeDelimiter);

            do
            {
                nw_Word[idx] = uc;
                nw_ucqueue.pop_front();

                width = nw_MadEdit->GetUCharWidth(uc);
                nw_Widths[idx] = width;
                nw_LineWidth += width;
                ++idx;
                --nw_FirstIndex;
            }
            while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                        && IsDelimiter(uc = nw_ucqueue.front().first)
                        && m_SpecialWordPrefix.Find(uc) < 0);

            nw_Word[idx] = 0;

        }
        else // for Keywords
        {
            // get full word
            do
            {
                nw_Word[idx] = uc;
                nw_ucqueue.pop_front();

                width = nw_MadEdit->GetUCharWidth(uc);
                nw_Widths[idx] = width;
                nw_LineWidth += width;
                ++idx;
                --nw_FirstIndex;
            }
            while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                        && IsNotDelimiter(uc = nw_ucqueue.front().first));

            nw_Word[idx] = 0;

            // check if is Keyword
            vector < MadSyntaxKeyword >::iterator kit = m_CustomKeyword.begin();
            vector < MadSyntaxKeyword >::iterator kend = m_CustomKeyword.end();
            bool bIsKeyword = false;

            if(nw_MaxKeywordLen != 0 && idx <= (int)nw_MaxKeywordLen && kit != kend)
            {
                wxString strorg(wxT(' '), idx), strlower(wxT(' '), idx);
                ucs4_t *puc=nw_Word;
                for(int i=0; i<idx; ++i)
                {
                    ucs4_t uc = *puc++;
#ifdef __WXMSW__
                    if(uc < 0x10000)
#endif
                    {
                        strorg[i] = wxChar(uc);

                        if(uc<=(ucs4_t)wxT('Z') && uc>=(ucs4_t)wxT('A'))
                            strlower[i] = wxChar(uc|0x20);
                        else
                            strlower[i] = wxChar(uc);
                    }
#ifdef __WXMSW__
                    else
                    {
                        //to surrogates????
                        strorg[i] = wxChar(0xFFFF);
                        strlower[i] = wxChar(0xFFFF);
                    }
#endif
                }

                MadKeywordSet::iterator it;
                do
                {
                    if(IsInRange(nw_State.rangeid, kit->m_InRange))
                    {
                        if(kit->m_CaseSensitive) it = kit->m_Keywords.find(strorg);
                        else                     it = kit->m_Keywords.find(strlower);
                        if(it != kit->m_Keywords.end())
                        {
                            bIsKeyword = true;
                            break;
                        }
                    }
                }
                while(++kit != kend);
            }

            if(bIsKeyword)
            {
                SetAttributes(&(kit->m_Attr));
            }
            else
            {
                SetAttributes(aeText);
            }
        }

    }
    else                                                    // Text
    {
        SetAttributes(aeText);

        do
        {
            nw_Word[idx] = uc;
            nw_ucqueue.pop_front();

            width = nw_MadEdit->GetUCharWidth(uc);
            nw_Widths[idx] = width;
            nw_LineWidth += width;
            ++idx;
            --nw_FirstIndex;
        }
        while(--nw_RestCount && nw_LineWidth < nw_RowIndexIter->m_Width
                    && (uc = nw_ucqueue.front().first) >= 0x100);

        nw_Word[idx] = 0;

    }

    //wxASSERT(nw_RowIndexIter->m_Width >= nw_LineWidth);
    //if(nw_RowIndexIter->m_Width < nw_LineWidth)
    //    nw_LineWidth += 0;

    wordwidth = nw_LineWidth - old_line_width;

    if(nw_RestCount == 0)
    {
        // reset bgcolor
        if(nw_NextState.rangeid != nw_State.rangeid)
        {
            nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
        }
        else if(nw_NextState.blkcmtid != nw_State.blkcmtid ||
                nw_NextState.stringid != nw_State.stringid )
        {
            if(nw_NextState.directive!=0)
            {
                nw_CurrentBgColor = m_SystemAttributes[aeDirective].bgcolor;
                if(nw_CurrentBgColor==wxNullColour)
                {
                    nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
                }
            }
            else
            {
                if(nw_NextState.rangeid!=0)
                {
                    MadSyntaxRange * r=GetSyntaxRange(nw_NextState.rangeid);
                    nw_CurrentBgColor = r->bgcolor;
                    //if(nw_CurrentBgColor==wxNullColour)
                    //{
                        //nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
                    //}
                }
                else
                {
                    nw_CurrentBgColor = m_SystemAttributes[aeText].bgcolor;
                }
            }
        }

        nw_State = nw_NextState;
    }

    if(nw_LineWidth == nw_RowIndexIter->m_Width)
    {
        nw_LineWidth = 0;

        if((++nw_RowIndexIter)->m_Width == 0)
        {
            nw_EndOfLine = true;
        }
        else
        {
            MadRowIndexIterator nextit=nw_RowIndexIter;
            ++nextit;
            nw_MaxLength = nextit->m_Start - nw_RowIndexIter->m_Start;
        }
    }

    return idx;
}

// for Printing
void MadSyntax::BeginPrint(bool printSyntax)
{
    m_PrintSyntax=printSyntax;

    if(m_PrintSyntax) return;

    // store colors
    MadAttributes *pat = m_SystemAttributes;
    MadAttributes *old = m_old_SystemAttributes;
    for(int i = 0; i < aeNone; i++)
    {
        old->color=pat->color;
        old->bgcolor=pat->bgcolor;

        pat->color=*wxBLACK;
        pat->bgcolor=*wxWHITE;

        ++pat;
        ++old;
    }

    size_t count=m_CustomRange.size();
    m_CustomRangeBgColor.resize(count);
    for(size_t i=0;i<count;i++)
    {
        m_CustomRangeBgColor[i]=m_CustomRange[i].bgcolor;
        m_CustomRange[i].bgcolor=*wxWHITE;
    }

    count = m_CustomKeyword.size();
    m_CustomKeywordColor.resize(count);
    for(size_t i=0;i<count;i++)
    {
        m_CustomKeywordColor[i].color=m_CustomKeyword[i].m_Attr.color;
        m_CustomKeywordColor[i].bgcolor=m_CustomKeyword[i].m_Attr.bgcolor;

        m_CustomKeyword[i].m_Attr.color=*wxBLACK;
        m_CustomKeyword[i].m_Attr.bgcolor=*wxWHITE;
    }
}

void MadSyntax::EndPrint()
{
    if(m_PrintSyntax) return;

    // store colors
    MadAttributes *pat = m_SystemAttributes;
    MadAttributes *old = m_old_SystemAttributes;
    for(int i = 0; i < aeNone; i++)
    {
        pat->color=old->color;
        pat->bgcolor=old->bgcolor;

        ++pat;
        ++old;
    }

    size_t count=m_CustomRange.size();
    for(size_t i=0;i<count;i++)
    {
        m_CustomRange[i].bgcolor=m_CustomRangeBgColor[i];
    }

    count = m_CustomKeyword.size();
    for(size_t i=0;i<count;i++)
    {
        m_CustomKeyword[i].m_Attr.color=m_CustomKeywordColor[i].color;
        m_CustomKeyword[i].m_Attr.bgcolor=m_CustomKeywordColor[i].bgcolor;
    }
}

void MadSyntax::LoadAttributes(const wxString &file)
{
    wxString attfile=file;
    if(attfile.IsEmpty())
    {
        attfile=GetAttributeFileByTitle(m_Title);
    }
    if(wxFileExists(attfile))
    {
        MadSyntax *att=new MadSyntax(attfile);
        AssignAttributes(att);
        delete att;
    }
}

wxString GetColorName(wxColour &c)
{
    if(!c.Ok() || c==wxNullColour) return wxEmptyString;
    
    wxString name=wxTheColourDatabase->FindName(c);
    if(name.IsEmpty())
    {
        name.Printf(wxT("%02X%02X%02X"), c.Red(), c.Green(), c.Blue());
    }
    return name;
}
wxString GetStyleString(MadFontStyles s)
{
    wxString str;
    if((s&fsBold)!=0) str+=wxT(" Bold");
    if((s&fsItalic)!=0) str+=wxT(" Italic");
    if((s&fsUnderline)!=0) str+=wxT(" Underline");
    return str;
}

void MadSyntax::SaveAttributes(const wxString &file)
{
    wxString attfile=file;
    if(attfile.IsEmpty())
    {
        attfile=GetAttributeFileByTitle(m_Title);
    }
    wxFileName fn(attfile);
    wxString dir=fn.GetPath(wxPATH_GET_VOLUME );
    if(!wxDirExists(dir))
    {
        wxMkdir(dir);
    }

    wxFileConfig syn(wxEmptyString,wxEmptyString,attfile,wxEmptyString,wxCONFIG_USE_RELATIVE_PATH|wxCONFIG_USE_NO_ESCAPE_CHARACTERS);

    // write custom ranges
    wxString str, value;
    size_t i;
    for(i=0; i<m_CustomRange.size(); i++)
    {
        wxString s, cname;
        cname=GetColorName(m_CustomRange[i].bgcolor);
        s.Printf(wxT(" %d b%d e%d %s"), i+1, i+1, i+1, cname.c_str());
        str+=s;
    }
    syn.Write(wxT("/CustomRange"), str);

    // write system attributes
    for(i=aeText; i<aeActiveLine; ++i)
    {
        str.Printf(wxT("/%sColor"), SystemAttributesName[i]);
        value.Printf(wxT("%s"), GetColorName(m_SystemAttributes[i].color).c_str());
        syn.Write(str, value);
        str.Printf(wxT("/%sBgColor"), SystemAttributesName[i]);
        value.Printf(wxT("%s"), GetColorName(m_SystemAttributes[i].bgcolor).c_str());
        syn.Write(str, value);
        str.Printf(wxT("/%sStyle"), SystemAttributesName[i]);
        value.Printf(wxT("%s"), GetStyleString(m_SystemAttributes[i].style).c_str());
        syn.Write(str, value);
    }
    str.Printf(wxT("/%sColor"), SystemAttributesName[aeActiveLine]);
    value.Printf(wxT("%s"), GetColorName(m_SystemAttributes[aeActiveLine].color).c_str());
    syn.Write(str, value);
    str.Printf(wxT("/%sColor"), SystemAttributesName[aeBookmark]);
    value.Printf(wxT("%s"), GetColorName(m_SystemAttributes[aeBookmark].color).c_str());
    syn.Write(str, value);

    // write custom keywords
    for(i=0; i<m_CustomKeyword.size(); i++)
    {
        str.Printf(wxT("/%x.Keyword/Color"), i+1);
        value.Printf(wxT("%s"), GetColorName(m_CustomKeyword[i].m_Attr.color).c_str());
        syn.Write(str, value);
        str.Printf(wxT("/%x.Keyword/BgColor"), i+1);
        value.Printf(wxT("%s"), GetColorName(m_CustomKeyword[i].m_Attr.bgcolor).c_str());
        syn.Write(str, value);
        str.Printf(wxT("/%x.Keyword/Style"), i+1);
        value.Printf(wxT("%s"), GetStyleString(m_CustomKeyword[i].m_Attr.style).c_str());
        syn.Write(str, value);
    }
}

void MadSyntax::AssignAttributes(MadSyntax *syn, bool add)
{
    size_t i;
    for(i=0; i<m_CustomRange.size() && i<syn->m_CustomRange.size(); i++)
    {
        m_CustomRange[i].bgcolor = syn->m_CustomRange[i].bgcolor;
    }
    if(add)
    {
        for(; i<syn->m_CustomRange.size(); i++)
        {
            MadSyntaxRange ra;
            ra.bgcolor = syn->m_CustomRange[i].bgcolor;
            m_CustomRange.push_back(ra);
        }
    }

    for(i=aeText; i<aeNone; ++i)
    {
        m_SystemAttributes[i].color = syn->m_SystemAttributes[i].color;
        m_SystemAttributes[i].bgcolor = syn->m_SystemAttributes[i].bgcolor;
        m_SystemAttributes[i].style = syn->m_SystemAttributes[i].style;
    }

    for(i=0; i<m_CustomKeyword.size() && i<syn->m_CustomKeyword.size(); i++)
    {
        m_CustomKeyword[i].m_Attr.color = syn->m_CustomKeyword[i].m_Attr.color;
        m_CustomKeyword[i].m_Attr.bgcolor = syn->m_CustomKeyword[i].m_Attr.bgcolor;
        m_CustomKeyword[i].m_Attr.style = syn->m_CustomKeyword[i].m_Attr.style;
    }
    if(add)
    {
        for(; i<syn->m_CustomKeyword.size(); i++)
        {
            MadSyntaxKeyword ke;
            ke.m_Attr.color  = syn->m_CustomKeyword[i].m_Attr.color;
            ke.m_Attr.bgcolor = syn->m_CustomKeyword[i].m_Attr.bgcolor;
            ke.m_Attr.style = syn->m_CustomKeyword[i].m_Attr.style;
            m_CustomKeyword.push_back(ke);
        }
    }
}
