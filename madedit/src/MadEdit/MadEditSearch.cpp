///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditSearch.cpp
// Description: searching and replacing functions
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEdit.h"
#include "MadEncoding.h"
#include <iostream>
#include <string>

//#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/xpressive/traits/null_regex_traits.hpp>
#include <boost/xpressive/traits/cpp_regex_traits.hpp>

using namespace std;
using namespace boost::xpressive;

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

template<typename char_type>
inline char_type xtolower(char_type ch)
{
    if(ch<0 || ch>0xFFFF) return ch;
    return towlower(wchar_t(ch));
}

template<>
inline wchar_t xtolower(wchar_t ch)
{
    return towlower(ch);
}

template<>
inline wxByte xtolower(wxByte ch)
{
    return ch;
}

template <typename char_type, typename Iter>
bool Search(Iter &begin, Iter &end, const char_type *wanted, size_t count, bool bCaseSensitive)
{
    wxASSERT(count != 0);

    size_t idx=0;
    Iter beginpos;
    char_type c1, c2;

    while(begin != end)
    {
        c1 = *begin;
        c2 = wanted[idx];

        if(bCaseSensitive==false)
        {
            c1=xtolower(c1);
            c2=xtolower(c2);
        }

        if(c1 == c2)
        {
            if(idx==0)
            {
                beginpos = begin;
            }
            ++idx;

            if(idx==count) // found whole wanted data
            {
                end = begin;
                ++end;
                begin = beginpos;
                return true;
            }
        }
        else // c1 != c2
        {
            if(idx!=0)
            {
                idx = 0;
                begin = beginpos;
            }
        }

        ++begin;
    }

    return false;
}


#ifdef __WXMSW__
namespace boost { namespace xpressive { namespace detail
{

template<char Ch, wchar_t Wch>
struct char_literal<ucs4_t, Ch, Wch>
{
    BOOST_STATIC_CONSTANT(ucs4_t, value = Wch);
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
template<char Ch, wchar_t Wch>
ucs4_t const char_literal<ucs4_t, Ch, Wch>::value;
#endif

template<>
struct string_literal<ucs4_t>
{
    static ucs4string inter_str;

    static ucs4_t const *pick(char const *, wchar_t const *cstr)
    {
        inter_str.clear();
        while(*cstr != 0)
        {
            inter_str.push_back(ucs4_t(*cstr));
            ++cstr;
        }

        return inter_str.c_str();
    }

    static ucs4_t pick(char, wchar_t ch)
    {
        return ch;
    }
};

ucs4string string_literal<ucs4_t>::inter_str;

}}}


struct ucs4_regex_traits: public null_regex_traits<ucs4_t>
{
    typedef ucs4_t char_type;
    typedef detail::umaskex_t char_class_type;

    template<typename char_type2>
    char_type2 tolower(char_type2 ch) const
    {
        if(ch<0 || ch>0xFFFF) return ch;
        return towlower(wchar_t(ch));
    }
    wchar_t tolower(wchar_t ch) const
    {
        return towlower(ch);
    }

    template<typename char_type2>
    char_type2 toupper(char_type2 ch) const
    {
        if(ch<0 || ch>0xFFFF) return ch;
        return towupper(wchar_t(ch));
    }
    wchar_t toupper(wchar_t ch) const
    {
        return towupper(ch);
    }

    char_type widen(char ch) const
    {
        return char_type(ch);
    }

    char_type translate_nocase(char_type ch) const
    {
        return this->tolower(ch);
    }

    bool in_range_nocase(char_type first, char_type last, char_type ch) const
    {
        return this->in_range(first, last, ch)
            || this->in_range(first, last, this->toupper(ch))
            || this->in_range(first, last, this->tolower(ch));
    }

    template<typename FwdIter>
    static char_class_type lookup_classname(FwdIter begin, FwdIter end, bool icase)
    {
        cpp_regex_traits<char> cpptraits;
        char_class_type c=cpptraits.lookup_classname(begin, end, icase);
        return c;
    }

    static bool isctype(char_type ch, char_class_type mask)
    {
        cpp_regex_traits<char> cpptraits;
        int i=ch;
        if(i<0 || i>0xFF) ch=0x0;//?
        return cpptraits.isctype(char(ch), mask);
    }

    static int value(char_type ch, int radix)
    {
        switch(radix)
        {
        case 8:
            if(ch>='0' && ch<='7') return ch-'0';
            break;
        case 10:
            if(ch>='0' && ch<='9') return ch-'0';
            break;
        case 16:
            if(ch>='0' && ch<='9') return ch-'0';
            if(ch>='A' && ch<='F') return ch-'A'+10;
            if(ch>='a' && ch<='f') return ch-'a'+10;
            break;
        }
        return -1;
    }
};

#else // __WXGTK__

typedef cpp_regex_traits<ucs4_t> ucs4_regex_traits;

#endif


struct UCQueueSet
{
    MadUCQueue   ucq;
    int          lock;
};

struct UCIterator   // ucs4_t widechar iterator
{
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef ucs4_t value_type;
    typedef wxFileOffset difference_type;
    typedef const value_type *pointer;
    typedef const value_type &reference;

    static MadLines         *s_lines;
    static MadLines::NextUCharFuncPtr s_NextUChar;
    static wxFileOffset     s_endpos;
    static list<UCQueueSet> s_ucqueues;

    static void Init(MadLines *lines, const wxFileOffset &endpos)
    {
        wxASSERT(endpos>=0 && endpos<=lines->GetSize());

        s_lines=lines;
        s_NextUChar=lines->NextUChar;
        s_endpos=endpos;
        s_ucqueues.clear();
    }

    typedef list<UCQueueSet>::iterator UCQIterator;
    #define UCQ_MAXSIZE (10 * 1024)
    #define BUF_MAXSIZE (5 * 1024)

    wxFileOffset    pos;
    MadLineIterator lit;
    wxFileOffset    linepos;
    UCQIterator     ucqit;
    int             ucqidx;

    UCIterator():ucqidx(-1) {}

    ~UCIterator()
    {
        if(ucqidx>=0 && --ucqit->lock == 0)
        {
            s_ucqueues.erase(ucqit);
        }
    }

    //UCIterator(wxFileOffset pos0):ucqidx(-1), pos(pos0) {}

    UCIterator(const UCIterator &ucit):ucqidx(-1)
    {
        this->operator =( ucit );
    }

    UCIterator(wxFileOffset pos0, const MadLineIterator &lit0, wxFileOffset linepos0)
        :pos(pos0), lit(lit0), linepos(linepos0)
    {
        if(linepos == lit->m_Size && pos<s_lines->GetSize())
        {
            ++lit;
            linepos = 0;
        }

        s_ucqueues.push_back(UCQueueSet());
        ucqit=s_ucqueues.end();
        --ucqit;

        ucqit->lock=1;            // lock this ucqueue

        ucqidx=0;

        if(pos<s_lines->GetSize())
        {
            MadUCQueue &ucqueue=ucqit->ucq;
            s_lines->InitNextUChar(lit, linepos);
            int i = BUF_MAXSIZE;

            if(pos>=s_endpos) i=10;

            while(--i>0 && (s_lines->*s_NextUChar)(ucqueue))
            {
                //ucqit->size += ucqueue.back().second;
            }
        }
    }

    UCIterator & operator=(const UCIterator & it)
    {
        if(ucqidx>=0 && --ucqit->lock == 0)
        {
            wxASSERT(ucqit != it.ucqit);
            s_ucqueues.erase(ucqit);
        }

        pos=it.pos;
        lit=it.lit;
        linepos=it.linepos;
        ucqit=it.ucqit;
        ucqidx=it.ucqidx;

        if(ucqidx>=0) ucqit->lock++;

        return *this;
    }

    const value_type operator*() const
    {
        wxASSERT(ucqidx>=0 && ucqidx < int(ucqit->ucq.size()));

        return ucqit->ucq[ucqidx].first;
    }

    /***
    ucs4_t *operator->() const
    {
        return _ws_ + pos;
    }
    ***/

    // pre-increment operator
    UCIterator & operator++()
    {
        wxASSERT(ucqidx>=0 && ucqidx < int(ucqit->ucq.size()));

        MadUCQueue *ucqueue = &(ucqit->ucq);

        int len = (*ucqueue)[ucqidx].second;
        pos += len;
        linepos += len;

        if(linepos == lit->m_Size)
        {
            if(pos==s_endpos) return *this; // end

            ++lit;
            linepos = 0;
        }

        ++ucqidx;

        if(ucqidx == (int)(*ucqueue).size())
        {
            wxASSERT(pos <= s_endpos);

            if(ucqidx>=UCQ_MAXSIZE)
            {
                if(--ucqit->lock == 0)
                {
                    s_ucqueues.erase(ucqit);
                }

                s_ucqueues.push_back(UCQueueSet());
                ucqit=s_ucqueues.end();
                --ucqit;

                ucqit->lock=1;            // lock this ucqueue

                ucqidx=0;

                ucqueue = &(ucqit->ucq);
            }

            s_lines->InitNextUChar(lit, linepos);
            int i = BUF_MAXSIZE;
            while(--i>0 && (s_lines->*s_NextUChar)(*ucqueue))
            {
                //ucqit->size += ucqueue->back().second;
            }

        }

        return *this;
    }

    /***
    // post-increment operator
    UCIterator operator++(int)
    {
        UCIterator tmp = *this;
        ++*this;
        return tmp;
    }
    ***/

    //***
    // pre-decrement operator
    UCIterator & operator--()
    {
        wxASSERT(pos>0 && ucqidx>=0 && ucqidx <= int(ucqit->ucq.size()));

        if(ucqidx==0)                   //rarely happen
        {
            if(--ucqit->lock == 0)
            {
                s_ucqueues.erase(ucqit);
            }

            s_ucqueues.push_back(UCQueueSet());
            ucqit=s_ucqueues.end();
            --ucqit;

            ucqit->lock=1;              // lock this ucqueue

            ucqidx=0;

            MadUCPair ucp=s_lines->PreviousUChar(lit, linepos);

            wxASSERT(ucp.second!=0);

            pos-=ucp.second;
            ucqit->ucq.push_back(ucp);

            return *this;
        }

        --ucqidx;
        int len = ucqit->ucq[ucqidx].second;
        pos -= len;

        if(linepos == 0)
        {
            --lit;
            linepos = lit->m_Size;
        }
        linepos -= len;

        return *this;
    }
    //***/

    /***
    // post-decrement operator
    UCIterator operator--(int)
    {
        UCIterator tmp = *this;
        --*this;
        return tmp;
    }
    ***/

    bool operator==(const UCIterator & it) const
    {
        if(pos == it.pos) return true;
        return (pos>=s_endpos && it.pos>=s_endpos);
    }

    bool operator!=(const UCIterator & it) const
    {
        return ! (this->operator==(it)) ;
    }

};

MadLines *UCIterator::s_lines=NULL;
MadLines::NextUCharFuncPtr UCIterator::s_NextUChar=NULL;
wxFileOffset UCIterator::s_endpos=0;
list<UCQueueSet> UCIterator::s_ucqueues;


typedef basic_regex<UCIterator> ucs4regex;

MadSearchResult MadEdit::Search(/*IN_OUT*/MadCaretPos &beginpos, /*IN_OUT*/MadCaretPos &endpos,
    const wxString &text, bool bRegex, bool bCaseSensitive, bool bWholeWord)
{
    if(beginpos.pos>=endpos.pos || text.IsEmpty())
        return SR_NO;

    regex_constants::syntax_option_type opt = regex_constants::ECMAScript;
    if(bCaseSensitive == false)
    {
        opt = opt | regex_constants::icase;
    }

#ifdef __WXMSW__
    vector<ucs4_t> ucs;
    TranslateText(text.c_str(), text.Len(), &ucs, true);
    ucs4_t *puc=&(*ucs.begin());
    size_t len=ucs.size();
    ucs4string exprstr(puc, puc+len);
#else
    const ucs4_t *puc=text.c_str();
    size_t len=text.Len();
    ucs4string exprstr(puc, puc+len);
#endif

    regex_compiler<UCIterator, ucs4_regex_traits > ucs4comp;
    ucs4regex expression;
    if(bRegex)
    {
        try
        {
            expression=ucs4comp.compile(exprstr, opt);
        }
        catch(regex_error)
        {
            wxMessageDialog dlg(this, wxString::Format(_("'%s' is not a valid regular expression."), text.c_str()),
                                wxT("MadEdit"), wxOK|wxICON_ERROR );
            dlg.ShowModal();
            return SR_EXPR_ERROR;
        }
    }


    UCIterator::Init(m_Lines, endpos.pos);

    UCIterator start(beginpos.pos, beginpos.iter, beginpos.linepos);
    UCIterator end(endpos.pos, endpos.iter, endpos.linepos);
    UCIterator fbegin, fend;
    match_results < UCIterator > what;
    bool found;

    try
    {
        for(;;)
        {
            if(bRegex)
            {
                found=regex_search(start, end, what, expression);
            }
            else
            {
                fbegin=start;
                fend=end;
                found=::Search(fbegin, fend, puc, len, bCaseSensitive);
            }

            if(!found) break;

            if(bWholeWord)              // check if is WholeWord
            {
                UCIterator cpos1, cpos2;
                if(bRegex)
                {
                    cpos1=what[0].first;
                    cpos2=what[0].second;
                }
                else
                {
                    cpos1=fbegin;
                    cpos2=fend;
                }

                // check cpos2
                if(cpos2.linepos > cpos2.lit->m_RowIndices[0].m_Start      // not at begin/end of line
                    && cpos2.linepos < (cpos2.lit->m_Size - cpos2.lit->m_NewLineSize))
                {
                    ucs4_t uc = *cpos2;
                    if(uc > 0x20 && !m_Syntax->IsDelimiter(uc) && uc != 0x3000)
                    {
                        // check prev-uchar of cpos2
                        --cpos2;
                        uc = *cpos2;
                        if(uc>0x20 &&  !m_Syntax->IsDelimiter(uc) && uc != 0x3000)
                        {
                            found = false;
                        }
                    }
                }

                // check cpos1
                if(found)
                {
                    wxFileOffset lpos = cpos1.lit->m_RowIndices[0].m_Start;
                    if(cpos1.linepos > lpos)        // not at begin of line
                    {
                        ucs4_t uc = *cpos1;
                        if(uc > 0x20 && !m_Syntax->IsDelimiter(uc) && uc != 0x3000)
                        {
                            // check prev-uchar of cpos1
                            --cpos1;
                            uc = *cpos1;
                            if(uc > 0x20 && !m_Syntax->IsDelimiter(uc) && uc != 0x3000)
                            {
                                found = false;
                            }
                        }
                    }
                }
            }

            if(found)
            {
                if(bRegex)
                {
                    beginpos.pos = what[0].first.pos;
                    beginpos.iter= what[0].first.lit;
                    beginpos.linepos=what[0].first.linepos;
                    endpos.pos = what[0].second.pos;
                    endpos.iter= what[0].second.lit;
                    endpos.linepos=what[0].second.linepos;
                }
                else
                {
                    beginpos.pos = fbegin.pos;
                    beginpos.iter= fbegin.lit;
                    beginpos.linepos=fbegin.linepos;
                    endpos.pos = fend.pos;
                    endpos.iter= fend.lit;
                    endpos.linepos=fend.linepos;
                }

                return SR_YES;
            }

            // not found, repeat...
            if(bRegex)
            {
                start = what[0].second;
            }
            else
            {
                start = fend;
            }
        }
    }
    catch(regex_error)
    {
        wxMessageDialog dlg(this, _("Catched a exception of 'regex_error'.\nMaybe the regular expression is invalid."),
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
        dlg.ShowModal();
        return SR_EXPR_ERROR;
    }

    return SR_NO;
}


// convert escape char to literal char
ucs4string ConvertEscape(const ucs4string &str)
{
    ucs4string out;
    detail::escape_value<ucs4_t, ucs4_regex_traits::char_class_type> esc;

    ucs4string::const_iterator begin=str.begin();
    ucs4string::const_iterator end=str.end();
    compiler_traits<ucs4_regex_traits> ucs4traits;

    while(begin!=end)
    {
        if(*begin=='\\')
        {
            if(++begin == end)
            {
                //out.push_back('\\'); // last char is '\'
                throw regex_error(regex_constants::error_escape);
            }
            else
            {
                esc = detail::parse_escape(begin, end, ucs4traits);
                out.push_back(esc.ch_);
            }
        }
        else
        {
            out.push_back(*begin);
            ++begin;
        }
    }

    return out;
}

MadSearchResult MadEdit::Replace(ucs4string &out, const MadCaretPos &beginpos, const MadCaretPos &endpos,
    const wxString &expr, const wxString &fmt,
    bool bRegex, bool bCaseSensitive, bool bWholeWord)
{
    if(expr.IsEmpty()) return SR_NO;

    if(bRegex == false)
    {
        // fmt is the wanted string
        vector<ucs4_t> ucs;
        TranslateText(fmt.c_str(), fmt.Len(), &ucs, true);

        for(size_t i=0, size=ucs.size(); i<size; i++)
        {
            out.push_back( ucs[i] );
        }

        return SR_YES;
    }

    regex_constants::syntax_option_type opt = regex_constants::ECMAScript;
    if(bCaseSensitive == false)
    {
        opt = opt | regex_constants::icase;
    }

#ifdef __WXMSW__
    vector<ucs4_t> ucs;
    TranslateText(expr.c_str(), expr.Len(), &ucs, true);
    ucs4_t *puc=&(*ucs.begin());
    ucs4string exprstr(puc, puc+ucs.size());
#else
    const ucs4_t *puc=expr.c_str();
    ucs4string exprstr(puc, puc+expr.Len());
#endif

    typedef ucs4string::const_iterator iterator;
    regex_compiler<iterator, ucs4_regex_traits > ucs4comp;

    basic_regex<iterator> expression;
    try
    {
        expression = ucs4comp.compile(exprstr, opt);
    }
    catch(regex_error)
    {
        wxMessageDialog dlg(this, wxString::Format(_("'%s' is not a valid regular expression."), expr.c_str()),
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
        dlg.ShowModal();
        return SR_EXPR_ERROR;
    }

#ifdef __WXMSW__
    ucs.clear();
    TranslateText(fmt.c_str(), fmt.Len(), &ucs, true);
    puc=&(*ucs.begin());
    ucs4string fmtstr(puc, puc+ucs.size());
#else
    puc=fmt.c_str();
    ucs4string fmtstr(puc, puc+fmt.Len());
#endif

    UCIterator begin(beginpos.pos, beginpos.iter, beginpos.linepos);
    UCIterator end(endpos.pos, endpos.iter, endpos.linepos);

    ucs4string str;

    //back_insert_iterator<ucs4string> oi(str);
    //std::copy(first, last, oi);
    while(begin!=end)
    {
        str.push_back(*begin);
        ++begin;
    }

    try
    {
        out=regex_replace(str, expression, fmtstr);
        out=ConvertEscape(out);
    }
    catch(regex_error)
    {
        wxMessageDialog dlg(this, wxString::Format(_("The format of '%s' is invalid."), fmt.c_str()),
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
        dlg.ShowModal();
        return SR_EXPR_ERROR;
    }

    return SR_YES;

    /***
    back_insert_iterator<ucs4string> oi(out);
    regex_replace(oi, first, last, expression, puc);
    ***/
}


// Hex Search
struct ByteIterator
{
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef wxByte value_type;
    typedef wxFileOffset difference_type;
    typedef const value_type *pointer;
    typedef const value_type &reference;

    static MadLines         *s_lines;
    static wxFileOffset     s_endpos;

    static void Init(MadLines *lines, const wxFileOffset &endpos)
    {
        wxASSERT(endpos>=0 && endpos<=lines->GetSize());

        s_lines=lines;
        s_endpos=endpos;
    }

    wxFileOffset    pos;
    MadLineIterator lit;
    wxFileOffset    linepos;

    ByteIterator() {}

    ByteIterator(const ByteIterator &it)
    {
        this->operator =( it );
    }

    ByteIterator(wxFileOffset pos0, const MadLineIterator &lit0, wxFileOffset linepos0)
        :pos(pos0), lit(lit0), linepos(linepos0)
    {
        if(linepos == lit->m_Size && pos<s_lines->GetSize())
        {
            ++lit;
            linepos = 0;
        }
    }

    ByteIterator & operator=(const ByteIterator & it)
    {
        pos=it.pos;
        lit=it.lit;
        linepos=it.linepos;
        return *this;
    }

    const value_type operator*()
    {
        wxASSERT(linepos < lit->m_Size);
        return lit->Get(linepos);
    }

    /***
    ucs4_t *operator->() const
    {
        return _ws_ + pos;
    }
    ***/

    // pre-increment operator
    ByteIterator & operator++()
    {
        ++pos;
        ++linepos;

        if(linepos == lit->m_Size)
        {
            if(pos==s_endpos) return *this; // end

            ++lit;
            linepos = 0;
        }

        return *this;
    }

    /***
    // post-increment operator
    ByteIterator operator++(int)
    {
        ByteIterator tmp = *this;
        ++*this;
        return tmp;
    }
    ***/

    //***
    // pre-decrement operator
    ByteIterator & operator--()
    {
        wxASSERT(pos>0);

        --pos;

        if(linepos == 0)
        {
            --lit;
            linepos = lit->m_Size;
        }
        --linepos;

        return *this;
    }
    //***/

    /***
    // post-decrement operator
    ByteIterator operator--(int)
    {
        ByteIterator tmp = *this;
        --*this;
        return tmp;
    }
    ***/

    bool operator==(const ByteIterator & it) const
    {
        if(pos == it.pos) return true;
        return (pos>=s_endpos && it.pos>=s_endpos);
    }

    bool operator!=(const ByteIterator & it) const
    {
        return ! (this->operator==(it)) ;
    }

};

MadLines *ByteIterator::s_lines=NULL;
wxFileOffset ByteIterator::s_endpos=0;

MadSearchResult MadEdit::SearchHex(/*IN_OUT*/MadCaretPos &beginpos, /*IN_OUT*/MadCaretPos &endpos,
                        const wxByte *hex, size_t count)
{
    if(beginpos.pos>=endpos.pos || count==0)
        return SR_NO;

    ByteIterator::Init(m_Lines, endpos.pos);
    ByteIterator start(beginpos.pos, beginpos.iter, beginpos.linepos);
    ByteIterator end(endpos.pos, endpos.iter, endpos.linepos);

    if(::Search(start, end, hex, count, true))
    {
        beginpos.pos = start.pos;
        beginpos.iter= start.lit;
        beginpos.linepos=start.linepos;

        endpos.pos = end.pos;
        endpos.iter= end.lit;
        endpos.linepos=end.linepos;

        return SR_YES;
    }

    return SR_NO;
}

