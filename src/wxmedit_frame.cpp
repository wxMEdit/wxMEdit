///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4 expandtab
// Name:        wxm_edit_frame.cpp
// Description: Main Frame of wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit_frame.h"
#include "xm/cxx11.h"
#include "wxmedit/wxmedit.h"
#include "wxmedit/wxmedit_command.h"
#include "wxmedit/wxm_syntax.h"
#include "wxmedit/trad_simp.h"
#include "dialog/wxm_search_replace_dialog.h"
#include "dialog/wxm_find_in_files_dialog.h"
#include "dialog/wxm_highlighting_dialog.h"
#ifdef __WXMSW__
#include "dialog/wxm_file_association_dialog.h"
#endif
#include "dialog/wxm_purge_histories_dialog.h"
#include "dialog/wxm_conv_enc_dialog.h"
#include "dialog/wxm_word_count_dialog.h"
#include "dialog/wxm_sort_dialog.h"
#include "dialog/wxmedit_about_dialog.h"
#include "dialog/wxmedit_options_dialog.h"
#include "wxm_plugin.h"
#include "wxm_printout.h"
#include "mad_utils.h"
#include "wxm_command.h"
#include "wxm/edit/inframe.h"
#include "wxm/update.h"
#include "wxm/recent_list.h"
#include "wxm/def.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/app.h>
//#include <wx/dynload.h>
#include <wx/tokenzr.h>
#include <wx/fontenum.h>
#include <wx/filename.h>
#include <wx/fontdlg.h>
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include <wx/dnd.h>
#include <wx/printdlg.h>
#include <wx/config.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/uchar.h>

#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/scoped_ptr.hpp>

#include <vector>
#include <utility>
#include <algorithm>


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

#define static static const

#if !defined(__WXMSW__) && !defined(__WXPM__)
#   include "../images/wxmedit.xpm"
#endif

#include "../images/null.xpm"
#define null_xpm_idx 0
#include "../images/new.xpm"
#define new_xpm_idx (null_xpm_idx+1)
#include "../images/fileopen.xpm"
#define fileopen_xpm_idx (new_xpm_idx+1)
#include "../images/filesave.xpm"
#define filesave_xpm_idx (fileopen_xpm_idx+1)
#include "../images/saveall.xpm"
#define saveall_xpm_idx (filesave_xpm_idx+1)
#include "../images/fileclose.xpm"
#define fileclose_xpm_idx (saveall_xpm_idx+1)
#include "../images/closeall.xpm"
#define closeall_xpm_idx (fileclose_xpm_idx+1)
#include "../images/preview.xpm"
#define preview_xpm_idx (closeall_xpm_idx+1)
#include "../images/print.xpm"
#define print_xpm_idx (preview_xpm_idx+1)
#include "../images/quit.xpm"
#define quit_xpm_idx (print_xpm_idx+1)

#include "../images/undo.xpm"
#define undo_xpm_idx (quit_xpm_idx+1)
#include "../images/redo.xpm"
#define redo_xpm_idx (undo_xpm_idx+1)
#include "../images/cut.xpm"
#define cut_xpm_idx (redo_xpm_idx+1)
#include "../images/copy.xpm"
#define copy_xpm_idx (cut_xpm_idx+1)
#include "../images/paste.xpm"
#define paste_xpm_idx (copy_xpm_idx+1)
#include "../images/delete.xpm"
#define delete_xpm_idx (paste_xpm_idx+1)
#include "../images/olist.xpm"
#define olist_xpm_idx (delete_xpm_idx+1)
#include "../images/indent.xpm"
#define indent_xpm_idx (olist_xpm_idx+1)
#include "../images/unindent.xpm"
#define unindent_xpm_idx (indent_xpm_idx+1)
#include "../images/comment.xpm"
#define comment_xpm_idx (unindent_xpm_idx+1)
#include "../images/uncomment.xpm"
#define uncomment_xpm_idx (comment_xpm_idx+1)

#include "../images/find.xpm"
#define find_xpm_idx (uncomment_xpm_idx+1)
#include "../images/findnext.xpm"
#define findnext_xpm_idx (find_xpm_idx+1)
#include "../images/findprev.xpm"
#define findprev_xpm_idx (findnext_xpm_idx+1)
#include "../images/replace.xpm"
#define replace_xpm_idx (findprev_xpm_idx+1)

#include "../images/bmktoggle.xpm"
#define bmktoggle_xpm_idx (replace_xpm_idx+1)
#include "../images/bmknext.xpm"
#define bmknext_xpm_idx (bmktoggle_xpm_idx+1)
#include "../images/bmkprev.xpm"
#define bmkprev_xpm_idx (bmknext_xpm_idx+1)
#include "../images/bmkclearall.xpm"
#define bmkclearall_xpm_idx (bmkprev_xpm_idx+1)

#include "../images/fontname.xpm"
#define fontname_xpm_idx (bmkclearall_xpm_idx+1)
#include "../images/fontsize.xpm"
#define fontsize_xpm_idx (fontname_xpm_idx+1)
#include "../images/font.xpm"
#define font_xpm_idx (fontsize_xpm_idx+1)
#include "../images/nowrap.xpm"
#define nowrap_xpm_idx (font_xpm_idx+1)
#include "../images/wrapbywin.xpm"
#define wrapbywin_xpm_idx (nowrap_xpm_idx+1)
#include "../images/wrapbycol.xpm"
#define wrapbycol_xpm_idx (wrapbywin_xpm_idx+1)
#include "../images/textmode.xpm"
#define textmode_xpm_idx (wrapbycol_xpm_idx+1)
#include "../images/columnmode.xpm"
#define columnmode_xpm_idx (textmode_xpm_idx+1)
#include "../images/hexmode.xpm"
#define hexmode_xpm_idx (columnmode_xpm_idx+1)

#include "../images/options.xpm"
#define options_xpm_idx (hexmode_xpm_idx+1)

#include "../images/wxmedit0_24x24.xpm"
#define wxmedit0_24x24_xpm_idx (options_xpm_idx+1)

#undef static

#if wxCHECK_VERSION(2,7,0)
    #define GetAccelFromString(x) wxAcceleratorEntry::Create(x)
#else
    #define GetAccelFromString(x) wxGetAccelFromString(x)
#endif

namespace wxm
{
const wxString g_wxMEdit_Homepage_URL(wxT("http://wxmedit.github.io/"));
const wxString g_wxMEdit_License_URL(wxT("http://www.gnu.org/licenses/gpl-3.0.html"));
wxString g_wxMEdit_About_URL = g_wxMEdit_Homepage_URL;

const static wxString s_wxMEdit_License(
wxT("Copyright (C) 2013-2019  JiaYanwei <wxmedit@gmail.com>\n")
wxT("Copyright (C) 2005-2010  Alston Chen <madedit@gmail.com>\n")
wxT("\n")
wxT("This program is free software: you can redistribute it and/or modify ")
wxT("it under the terms of the GNU General Public License as published by ")
wxT("the Free Software Foundation, either version 3 of the License, or ")
wxT("(at your option) any later version.\n")
wxT("\n")
wxT("This program is distributed in the hope that it will be useful, ")
wxT("but WITHOUT ANY WARRANTY; without even the implied warranty of ")
wxT("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ")
wxT("GNU General Public License for more details.\n")
);

#undef _
#define _(s)    wxT(s)
#define s_(s)   wxString(_T(s))
typedef std::vector<std::pair<wxString, wxString> > CreditsList;
const static CreditsList s_wxMEdit_Credits = boost::assign::pair_list_of
        (s_("JiaYanwei"),                   _("Current maintainer of wxMEdit"))
        (s_("j\u00E9r\u00F4me KASPER"),     _("Current maintainer of wxMEdit"))
        (s_("Alston Chen"),                 _("Creator of MadEdit"))
        (s_("Tilt"),                        _("Translator for Japanese"))
        (s_("Angelo Contardi"),             _("Translator for Italian"))
        (s_("Carlos S\u00E1nchez"),         _("Translator for Spanish"))
        (s_("Denir Li"),                    _("Translator for Traditional Chinese"))
        (s_("Vincent_TW"),                  _("Translator for Traditional Chinese"))
        (s_("Shamil Bikineyev"),            _("Translator for Russian"))
        (s_("\u0421\u0442\u0430\u043d\u0438\u0441\u043b\u0430\u0432 \u0411\u0443\u0434\u0438\u043d\u043e\u0432"),
                                            _("Translator for Russian"))
        (s_("Ronny Steiner"),               _("Translator for German"))
        (s_("Adam Massalski"),              _("Translator for Polish"))
        (s_("j\u00E9r\u00F4me KASPER"),     _("Translator for French"))
        (s_("hobbyscripter"),               _("Patch submitter for wxMEdit/MadEdit"))
        (s_("Huaren Zhong"),                _("Patch submitter for wxMEdit/MadEdit"))
        (s_("Gospodin Gyurov"),             _("Patch submitter for wxMEdit/MadEdit"))
        (s_("cfreeer"),                     _("Patch submitter for wxMEdit/MadEdit"))
        (s_("Nagy Gabor"),                  _("Patch submitter for wxMEdit/MadEdit"))
        (s_("LinXiaoHui"),                  _("Patch submitter for wxMEdit/MadEdit"))
        (s_("codestation"),                 _("Creator of wxmedit package in AUR"))
        (s_("Jingbei Li"),                  _("Maintainer of wxmedit package in AUR"))
        (s_("Micha\u0142 Zi\u0105bkowski"), _("Creator of Gentoo package wxmedit"))
    ;
#undef s_
#undef _
#define _(s)    wxGetTranslation(_T(s))

wxString g_lastpath_closingfiles;
} // namespace wxm


MadEditFrame* g_MainFrame = nullptr;
wxm::InFrameWXMEdit* g_active_wxmedit = nullptr;
int g_PrevPageID=-1;
bool g_CheckModTimeForReload = true;

wxMenu *g_PopMenu_Tab = nullptr;
wxMenu *g_Menu_File = nullptr;
wxMenu *g_Menu_Edit = nullptr;
wxMenu *g_Menu_Search = nullptr;
wxMenu *g_Menu_View = nullptr;
wxMenu *g_Menu_Tools = nullptr;
wxMenu *g_Menu_Window = nullptr;
wxMenu *g_Menu_Help = nullptr;
wxMenu *g_Menu_File_CloseMore = nullptr;
wxMenu *g_Menu_File_CopyPath = nullptr;
wxMenu *g_Menu_File_RecentFiles = nullptr;
wxMenu *g_Menu_Edit_Column = nullptr;
wxMenu *g_Menu_Edit_Sort = nullptr;
wxMenu *g_Menu_Edit_Advanced = nullptr;
wxMenu *g_Menu_View_Encoding = nullptr;
wxMenu *g_Menu_View_AllEncodings = nullptr;
wxMenu *g_Menu_View_Syntax = nullptr;
wxMenu *g_Menu_View_FontName = nullptr;
wxMenu *g_Menu_View_Font0 = nullptr;
wxMenu *g_Menu_View_Font1 = nullptr;
wxMenu *g_Menu_View_Font2 = nullptr;
wxMenu *g_Menu_View_Font3 = nullptr;
wxMenu *g_Menu_View_Font4 = nullptr;
wxMenu *g_Menu_View_Font5 = nullptr;
wxMenu *g_Menu_View_Font6 = nullptr;
wxMenu *g_Menu_View_FontSize = nullptr;
wxMenu *g_Menu_View_TabColumn = nullptr;
wxMenu *g_Menu_View_LineSpacing = nullptr;
wxMenu *g_Menu_Tools_BOM = nullptr;
wxMenu *g_Menu_Tools_NewLineChar = nullptr;
wxMenu *g_Menu_Tools_InsertNewLineChar = nullptr;
wxMenu *g_Menu_Tools_ConvertChineseChar = nullptr;

wxArrayString g_FontNames;

wxAcceleratorEntry g_AccelFindNext, g_AccelFindPrev;

//---------------------------------------------------------------------------

// for RestoreCaretPos
struct FileCaretPosManager
{
    struct PosData
    {
        wxFileOffset pos;
        wxString encoding;
        wxString fontname;
        int fontsize;
        wxString hex_fontname;
        int hex_fontsize;
        bool hexmode;

        PosData() : pos(0), fontsize(0), hex_fontsize(0), hexmode(false) {}
    };

private:
    int max_count;

    struct FilePosData
    {
        wxString name;
        unsigned long hash; // hash value of filename
        PosData data;

        FilePosData(const wxString& n, unsigned long h, const PosData& d)
            : name(n), hash(h), data(d)
        {}

        FilePosData(): hash(0) {}
    };
    std::list<FilePosData> files;

public:
    FileCaretPosManager() : max_count(40) {}

    void Add(const wxString &name, const PosData& data)
    {
        unsigned long hash = wxm::FilePathHash(name);
        if(files.size()==0)
        {
            files.push_back(FilePosData(name, hash, data));
        }
        else
        {
            std::list<FilePosData>::iterator it = files.begin();
            std::list<FilePosData>::iterator itend = files.end();
            do
            {
                if(it->hash == hash && wxm::FilePathEqual(it->name, name))
                {
                    break;
                }
            }
            while(++it != itend);

            if(it == itend)
            {
                files.push_front(FilePosData(name, hash, data));
            }
            else
            {
                it->data = data;

                files.push_front(*it);
                files.erase(it);
            }
        }
        if(int(files.size()) > max_count)
        {
            files.pop_back();
        }
    }

    void Add(wxm::InFrameWXMEdit* wxmedit)
    {
        if (wxmedit == nullptr)
            return;

        wxString name = wxmedit->GetFileName();
        if (name.IsEmpty())
            return;

        PosData data;
        wxmedit->GetTextFont(data.fontname, data.fontsize);
        wxmedit->GetHexFont(data.hex_fontname, data.hex_fontsize);
        data.pos = wxmedit->GetCaretPosition();
        data.hexmode = (wxmedit->GetEditMode() == emHexMode);
        Add(name, data);
    }

    void Save(wxConfigBase *cfg)
    {
        cfg->Write(wxT("MaxCount"), max_count);

        std::list<FilePosData>::iterator it = files.begin();
        wxString entry(wxT("file")), text;
        int idx=0, count=int(files.size());
        while(idx < count)
        {
            text = wxLongLong(it->data.pos).ToString();
            text += wxT("|");
            text += it->name;
            text += wxT("|");
            text += it->data.encoding;
            text += wxT("|");
            text += it->data.fontname;
            text += wxT("|");
            text += wxLongLong(it->data.fontsize).ToString();
            text += wxT("|");
            text += wxLongLong(int(it->data.hexmode)).ToString();
            text += wxT("|");
            text += it->data.hex_fontname;
            text += wxT("|");
            text += wxLongLong(it->data.hex_fontsize).ToString();
            cfg->Write(entry + (wxString()<<(idx+1)), text);
            ++idx;
            ++it;
        }
    }

    void Load(wxConfigBase *cfg)
    {
        cfg->Read(wxT("MaxCount"), &max_count);

        wxString entry(wxT("file"));

        for (int idx = 1; idx<=max_count; ++idx)
        {
            wxString text;
            if (!cfg->Read(entry + (wxString() << idx), &text))
                break;

            if (text.Find(wxT("|")) == wxNOT_FOUND)
                continue;

            FilePosData fpdata;

            wxStringTokenizer tkz(text, wxT("|"));

            wxInt64 i64 = 0;
            if (StrToInt64(tkz.GetNextToken(), i64))
                fpdata.data.pos = i64;

            if (tkz.HasMoreTokens())
                fpdata.name = tkz.GetNextToken();

            if (tkz.HasMoreTokens())
                fpdata.data.encoding = tkz.GetNextToken();

            if (tkz.HasMoreTokens())
                fpdata.data.fontname = tkz.GetNextToken();

            if (tkz.HasMoreTokens() && StrToInt64(tkz.GetNextToken(), i64))
                fpdata.data.fontsize = int(i64);

            if (tkz.HasMoreTokens() && StrToInt64(tkz.GetNextToken(), i64))
                fpdata.data.hexmode = (i64!=0);

            if (tkz.HasMoreTokens())
                fpdata.data.hex_fontname = tkz.GetNextToken();

            if (tkz.HasMoreTokens() && StrToInt64(tkz.GetNextToken(), i64))
                fpdata.data.hex_fontsize = int(i64);

            fpdata.hash = wxm::FilePathHash(fpdata.name);
            files.push_back(fpdata);
        }
    }

    void GetRestoreData(const wxString& name, PosData& data)
    {
        if (files.size() == 0)
            return;

        unsigned long hash = wxm::FilePathHash(name);

        std::list<FilePosData>::iterator it = files.begin();
        std::list<FilePosData>::iterator itend = files.end();
        do
        {
            if (it->hash != hash || !wxm::FilePathEqual(it->name, name))
                continue;

            data = it->data;
            break;
        }
        while(++it != itend);
    }
};
FileCaretPosManager g_FileCaretPosManager;


//---------------------------------------------------------------------------

// for FindInFilesResults
class CaretPosData: public wxTreeItemData
{
public:
    wxString filename;
    int pageid; // >=0 for 'NoName'
    wxFileOffset bpos, epos;
    CaretPosData(const wxString &fn, int pid, const wxFileOffset &b, wxFileOffset &e)
        : filename(fn), pageid(pid), bpos(b), epos(e) {}
};

//---------------------------------------------------------------------------

#ifndef __WXMSW__   // for Linux filename checking/converting
MadConvFileName MadConvFileNameObj;

wxString FixFileNameEncoding(const wxString &filename)
{
    size_t len=filename.Len();
    wxByte *bbuf=new wxByte[len+1];
    wxByte *ps=bbuf;
    const wxChar *pwcs=filename.c_str();
    for(size_t l=len;l>0;l--, ++ps, ++pwcs)
    {
        if(*pwcs >= 0x100)
        {
            delete []bbuf;
            return filename;
        }

        *ps = *pwcs;
    }
    *ps = 0;

    g_MB2WC_check_dir_filename=true;

    size_t wcslen=MadConvFileNameObj.MB2WC(nullptr, (const char*)bbuf, 0);
    if(wcslen==size_t(-1))
        return filename;

    wxChar *str=new wxChar[wcslen+1];
    MadConvFileNameObj.MB2WC(str, (const char*)bbuf, wcslen+1);

    g_MB2WC_check_dir_filename=false;

    wxString newfn(str);
    delete []bbuf;
    delete []str;

    return newfn;
}

#endif


class DnDFile : public wxFileDropTarget
{
public:
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override
    {
        size_t count=filenames.GetCount();
        for(size_t i=0;i<count;i++)
        {
#ifdef __WXMSW__
            g_MainFrame->OpenFile(filenames[i], true);
#else
            wxString fn=FixFileNameEncoding(filenames[i]);
            g_MainFrame->OpenFile(fn, true);
#endif
        }
        return true;
    }
};

//---------------------------------------------------------------------------

std::list<wxMadAuiNotebook::PageData> wxMadAuiNotebook::GetPagesList()
{
    wxAuiNotebookPageArray& pages = m_tabs.GetPages();
    size_t i, page_count = pages.GetCount();

    list<PageData> pages_list;
    list<PageData>::iterator it;

    for (i = 0; i < page_count; ++i)
    {
        wxAuiNotebookPage& page = pages.Item(i);
        wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)page.window;

        wxAuiTabCtrl *ctrl;
        int idx;
        if(FindTab(page.window, &ctrl, &idx))
        {
            wxPoint pt = ctrl->GetPosition();
            pt = ctrl->ClientToScreen(pt);
            it = pages_list.begin();
            size_t j=0;
            for(; j<pages_list.size(); ++j, ++it)
            {
                PageData &pd = *it;
                if(pt.y < pd.y)
                {
                    break;
                }
                else if(pt.y==pd.y)
                {
                    if(pt.x<pd.x)
                    {
                        break;
                    }
                    else if(pt.x==pd.x)
                    {
                        if(idx<pd.idx)
                        {
                            break;
                        }
                    }
                }
            }

            pages_list.insert(it, PageData(pt.x, pt.y, idx, wxmedit, (int)i));
        }
    }

    return pages_list;
}

size_t wxMadAuiNotebook::GetFilesListForReload(wxm::FileList& filelist)
{
    size_t count = 0;
    list<PageData> pages_list = GetPagesList();
    size_t page_count = pages_list.size();
    list<PageData>::iterator it = pages_list.begin();
    for (size_t i = 0; i < page_count; ++i, ++it)
    {
        wxString name(it->wxmedit->GetFileName());
        if(!name.IsEmpty())
        {
            LineNumberList bmklns = it->wxmedit->SaveBookmarkLineNumberList();
            filelist.Append(name, bmklns);
            ++count;
        }
    }

    wxString selname = g_active_wxmedit->GetFileName();
    if(count>1 && !selname.IsEmpty())
        filelist.Append(selname); // append selname to activate it

    return count;
}

void wxMadAuiNotebook::AdvanceSelection(bool bForward)
{
    int count = (int)GetPageCount();
    if(count <= 1) return;

    list<PageData> pages_list = GetPagesList();
    wxWindow* win = GetPage(GetSelection());

    FunctorA fa;
    fa.wxmedit = (wxm::InFrameWXMEdit*)win;
    list<PageData>::iterator it = std::find_if(pages_list.begin(), pages_list.end(), fa);

    wxASSERT(it != pages_list.end());

    if(bForward)
    {
        if(++it == pages_list.end()) it=pages_list.begin();
    }
    else
    {
        if(it==pages_list.begin()) it=pages_list.end();
        --it;
    }
    SetSelection(GetPageIndex(it->wxmedit));
}

void wxMadAuiNotebook::ConnectMouseClick()
{
    if(GetPageCount()!=0)
    {
        wxAuiTabCtrl *ctrl=GetActiveTabCtrl();
        ctrl->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxMadAuiNotebook::OnMouseClick));
        ctrl->Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(wxMadAuiNotebook::OnMouseClick));
    }
}

void wxMadAuiNotebook::OnMouseClick(wxMouseEvent &evt)
{
    //wxString s;
    //s.Printf(L"%d %d", evt.m_x, evt.m_y);
    //((wxTopLevelWindow*)g_MainFrame)->SetLabel(s);

    wxAuiTabCtrl *ctrl = (wxAuiTabCtrl*)evt.GetEventObject();
    wxWindow *win;
    if(ctrl->TabHitTest(evt.m_x, evt.m_y, &win))
    {
        g_MainFrame->CloseFile(g_MainFrame->m_Notebook->GetPageIndex(win));
    }
}

//---------------------------------------------------------------------------

void OnReceiveMessage(const wchar_t *msg, size_t size)
{
#ifdef __WXMSW__
    WINDOWPLACEMENT wp;
    wp.length=sizeof(WINDOWPLACEMENT);
    GetWindowPlacement((HWND)g_MainFrame->GetHWND(), &wp);

    if(wp.showCmd==SW_SHOWMINIMIZED)
    {
        if((wp.flags&WPF_RESTORETOMAXIMIZED)==0) // show normal
        {
            wp.showCmd=SW_NORMAL;
        }
        else                                     // show maximized
        {
            wp.showCmd=SW_SHOWMAXIMIZED;
        }
        SetWindowPlacement((HWND)g_MainFrame->GetHWND(), &wp);
    }
#else
    //g_MainFrame->Show(true);
    g_MainFrame->Raise();
# if wxMAJOR_VERSION == 2
    g_MainFrame->RequestUserAttention();//wxUSER_ATTENTION_ERROR);
# endif
    g_MainFrame->SetFocus();
#endif

    if (g_active_wxmedit != nullptr) g_active_wxmedit->SetFocus();

    wxm::FileList filelist(msg);

    BOOST_FOREACH (const wxm::FileList::FileDesc& fdesc, filelist.List())
        g_MainFrame->OpenFile(fdesc.file, false, fdesc.bmklinenums);
}

// return true for name; false for title
bool GetActiveMadEditPathNameOrTitle(wxString &name)
{
    name = g_active_wxmedit->GetFileName();
    if(name.IsEmpty())
    {
        name = g_MainFrame->m_Notebook->GetPageText( GetIdByEdit(g_active_wxmedit) );
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);
        return false;
    }
    return true;
}

void ApplySyntaxAttributes(MadSyntax *syn)
{
    int count=int(g_MainFrame->m_Notebook->GetPageCount());
    for(int id=0;id<count;id++)
    {
        wxm::InFrameWXMEdit *wxmedit = (wxm::InFrameWXMEdit*)g_MainFrame->m_Notebook->GetPage(id);
        wxmedit->ApplySyntaxAttributes(syn, true);
    }
}

void ResetStatusBar()
{
    g_MainFrame->m_Notebook->ConnectMouseClick();

    wxm::GetFrameStatusBar().SetField(wxm::STBF_ROWCOL,    wxEmptyString);
    wxm::GetFrameStatusBar().SetField(wxm::STBF_CHARPOS,   wxEmptyString);
    wxm::GetFrameStatusBar().SetField(wxm::STBF_SELECTION, wxEmptyString);
    wxm::GetFrameStatusBar().SetField(wxm::STBF_ENCFMT,    wxEmptyString);
    wxm::GetFrameStatusBar().SetField(wxm::STBF_READONLY,  wxEmptyString);
    wxm::GetFrameStatusBar().SetField(wxm::STBF_INSOVR,    wxEmptyString);
    wxm::GetFrameStatusBar().Update(); // repaint immediately
}

void UpdateMenus()
{
    g_PopMenu_Tab->UpdateUI();
    g_Menu_File->UpdateUI();
    g_Menu_Edit->UpdateUI();
    g_Menu_Search->UpdateUI();
    g_Menu_View->UpdateUI();
    g_Menu_Tools->UpdateUI();
    g_Menu_Window->UpdateUI();
    g_Menu_Help->UpdateUI();
}


//---------------------------------------------------------------------------

// enum & sort the facenames
class MadFontEnumerator : public wxFontEnumerator
{
    wxArrayString m_facenames_foldcase;
public:

    virtual bool OnFacename(const wxString& facename) override
    {
        size_t count = m_facenames_foldcase.Count();
        if(count == 0)
        {
            g_FontNames.Add(facename);
            m_facenames_foldcase.Add(wxm::FilePathNormalCase(facename));
        }
        else
        {
            wxString fn_foldcase = wxm::FilePathNormalCase(facename);
            size_t i = 0;
            for(; i<count; i++)
            {
                if(fn_foldcase < m_facenames_foldcase[i])
                    break;
            }

            g_FontNames.Insert(facename, i);
            m_facenames_foldcase.Insert(fn_foldcase, i);
        }
        return true;
    }

};

//----------------------------------------------------------------------------
// MadEditFrame
//----------------------------------------------------------------------------
DEFINE_LOCAL_EVENT_TYPE( wxmEVT_RESULT_AUTOCHECKUPDATES )
DEFINE_LOCAL_EVENT_TYPE( wxmEVT_RESULT_MANUALCHECKUPDATES )

   //Add Custom Events only in the appropriate Block.
BEGIN_EVENT_TABLE(MadEditFrame,wxFrame)
	EVT_SIZE(MadEditFrame::OnSize)
	EVT_AUINOTEBOOK_PAGE_CHANGING(ID_NOTEBOOK, MadEditFrame::OnNotebookPageChanging)
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MadEditFrame::OnNotebookPageChanged)
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NOTEBOOK, MadEditFrame::OnNotebookPageClosing)
	//EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NOTEBOOK, MadEditFrame::OnNotebookPageClosed)
	EVT_AUINOTEBOOK_TAB_RIGHT_UP(ID_NOTEBOOK, MadEditFrame::OnNotebookTabRightUp)
	//EVT_CHAR(MadEditFrame::OnChar)
	// file
	EVT_ACTIVATE(MadEditFrame::OnActivate)
	EVT_CUSTOM( wxmEVT_RESULT_AUTOCHECKUPDATES, wxID_ANY, MadEditFrame::OnResultAutoCheckUpdates)
	EVT_CUSTOM( wxmEVT_RESULT_MANUALCHECKUPDATES, wxID_ANY, MadEditFrame::OnResultManualCheckUpdates)
	EVT_UPDATE_UI(menuSave, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuSaveAs, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuSaveAll, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuReload, MadEditFrame::OnUpdateUI_MenuFile_CheckNamed)
	EVT_UPDATE_UI(menuClose, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseByPath, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseAll, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseMore, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseAllButThis, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseAllToTheLeft, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCloseAllToTheRight, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuCopyFilePath, MadEditFrame::OnUpdateUI_MenuFile_CheckNamed)
	EVT_UPDATE_UI(menuCopyFullPath, MadEditFrame::OnUpdateUI_MenuFile_CheckNamed)
	EVT_UPDATE_UI(menuCopyFilename, MadEditFrame::OnUpdateUI_MenuFile_CheckNamed)
	EVT_UPDATE_UI(menuCopyFileDir, MadEditFrame::OnUpdateUI_MenuFile_CheckNamed)
	EVT_UPDATE_UI(menuPrintPreview, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuPrint, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuRecentFiles, MadEditFrame::OnUpdateUI_MenuFileRecentFiles)
	// edit
	EVT_UPDATE_UI(menuUndo, MadEditFrame::OnUpdateUI_MenuEditUndo)
	EVT_UPDATE_UI(menuRedo, MadEditFrame::OnUpdateUI_MenuEditRedo)
	EVT_UPDATE_UI(menuCut, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelection)
	EVT_UPDATE_UI(menuCopy, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelection)
	EVT_UPDATE_UI(menuPaste, MadEditFrame::OnUpdateUI_MenuEditPaste)
	EVT_UPDATE_UI(menuPasteOvr, MadEditFrame::OnUpdateUI_MenuEditPasteOvr)
	EVT_UPDATE_UI(menuDelete, MadEditFrame::OnUpdateUI_Menu_CheckSize)
	EVT_UPDATE_UI(menuCutLine, MadEditFrame::OnUpdateUI_MenuEditDeleteLine)
	EVT_UPDATE_UI(menuDeleteLine, MadEditFrame::OnUpdateUI_MenuEditDeleteLine)
	EVT_UPDATE_UI(menuSelectAll, MadEditFrame::OnUpdateUI_Menu_CheckSize)
	EVT_UPDATE_UI(menuInsertTabChar, MadEditFrame::OnUpdateUI_MenuEditInsertTabChar)
	EVT_UPDATE_UI(menuInsertDateTime, MadEditFrame::OnUpdateUI_MenuEditInsertDateTime)
	EVT_UPDATE_UI(menuSortAscending, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuSortDescending, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuSortAscendingCase, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuSortDescendingCase, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuSortByOptions, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuCopyAsHexString, MadEditFrame::OnUpdateUI_MenuEditCopyAsHexString)
	EVT_UPDATE_UI(menuCopyAsHexStringWithSpace, MadEditFrame::OnUpdateUI_MenuEditCopyAsHexString)
	EVT_UPDATE_UI(menuIncreaseIndent, MadEditFrame::OnUpdateUI_MenuIndent)
	EVT_UPDATE_UI(menuDecreaseIndent, MadEditFrame::OnUpdateUI_MenuIndent)
	EVT_UPDATE_UI(menuComment, MadEditFrame::OnUpdateUI_MenuComment)
	EVT_UPDATE_UI(menuUncomment, MadEditFrame::OnUpdateUI_MenuComment)
	EVT_UPDATE_UI(menuWordWrapToNewLine, MadEditFrame::OnUpdateUI_Menu_CheckSize)
	EVT_UPDATE_UI(menuNewLineToWordWrap, MadEditFrame::OnUpdateUI_Menu_CheckSize)
	EVT_UPDATE_UI(menuToUpperCase, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuToLowerCase, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuInvertCase , MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuToHalfWidth, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuToHalfWidthByOptions, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuToFullWidth, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuToFullWidthByOptions, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuTabToSpace, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuSpaceToTab, MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize)
	EVT_UPDATE_UI(menuTrimTrailingSpaces, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuInsertEnumeration, MadEditFrame::OnUpdateUI_Menu_CheckTextFile)
	EVT_UPDATE_UI(menuColumn, MadEditFrame::OnUpdateUI_MenuEdit_Column)
	// search
	EVT_UPDATE_UI(menuFind, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuFindNext, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuFindPrevious, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuReplace, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	EVT_UPDATE_UI(menuGoToLine, MadEditFrame::OnUpdateUI_MenuSearchGoTo)
	EVT_UPDATE_UI(menuGoToPosition, MadEditFrame::OnUpdateUI_MenuSearchGoTo)
	EVT_UPDATE_UI(menuLeftBrace, MadEditFrame::OnUpdateUI_MenuSearchGoToBrace)
	EVT_UPDATE_UI(menuRightBrace, MadEditFrame::OnUpdateUI_MenuSearchGoToBrace)
	EVT_UPDATE_UI(menuToggleBookmark, MadEditFrame::OnUpdateUI_MenuEditToggleBookmark)
	EVT_UPDATE_UI(menuGotoNextBookmark, MadEditFrame::OnUpdateUI_MenuEditBookmarkExist)
	EVT_UPDATE_UI(menuGotoPreviousBookmark, MadEditFrame::OnUpdateUI_MenuEditBookmarkExist)
	EVT_UPDATE_UI(menuClearAllBookmarks, MadEditFrame::OnUpdateUI_MenuEditBookmarkExist)
	// view
	EVT_UPDATE_UI(menuEncoding, MadEditFrame::OnUpdateUI_MenuViewEncoding)
	EVT_UPDATE_UI(menuSyntax, MadEditFrame::OnUpdateUI_MenuViewSyntax)
	EVT_UPDATE_UI(menuFontName, MadEditFrame::OnUpdateUI_MenuViewFontName)
	EVT_UPDATE_UI(menuFontSize, MadEditFrame::OnUpdateUI_MenuViewFontSize)
	EVT_UPDATE_UI(menuSetFont, MadEditFrame::OnUpdateUI_MenuViewSetFont)
	EVT_UPDATE_UI(menuFixedWidthMode, MadEditFrame::OnUpdateUI_MenuViewFixedWidthMode)
	EVT_UPDATE_UI(menuTabColumn, MadEditFrame::OnUpdateUI_MenuViewTabColumn)
	EVT_UPDATE_UI(menuLineSpacing, MadEditFrame::OnUpdateUI_MenuViewLineSpacing)
	EVT_UPDATE_UI(menuNoWrap, MadEditFrame::OnUpdateUI_MenuViewNoWrap)
	EVT_UPDATE_UI(menuWrapByWindow, MadEditFrame::OnUpdateUI_MenuViewWrapByWindow)
	EVT_UPDATE_UI(menuWrapByColumn, MadEditFrame::OnUpdateUI_MenuViewWrapByColumn)
	EVT_UPDATE_UI(menuDisplayBookmark, MadEditFrame::OnUpdateUI_MenuViewDisplayBookmark)
	EVT_UPDATE_UI(menuDisplayLineNumber, MadEditFrame::OnUpdateUI_MenuViewDisplayLineNumber)
	EVT_UPDATE_UI(menuShowEndOfLine, MadEditFrame::OnUpdateUI_MenuViewShowEndOfLine)
	EVT_UPDATE_UI(menuShowTabChar, MadEditFrame::OnUpdateUI_MenuViewShowTabChar)
	EVT_UPDATE_UI(menuShowSpaceChar, MadEditFrame::OnUpdateUI_MenuViewShowSpaceChar)
	EVT_UPDATE_UI(menuMarkActiveLine, MadEditFrame::OnUpdateUI_MenuViewMarkActiveLine)
	EVT_UPDATE_UI(menuMarkBracePair, MadEditFrame::OnUpdateUI_MenuViewMarkBracePair)
	EVT_UPDATE_UI(menuTextMode, MadEditFrame::OnUpdateUI_MenuViewTextMode)
	EVT_UPDATE_UI(menuColumnMode, MadEditFrame::OnUpdateUI_MenuViewColumnMode)
	EVT_UPDATE_UI(menuHexMode, MadEditFrame::OnUpdateUI_MenuViewHexMode)
	// tools
	EVT_UPDATE_UI(menuByteOrderMark, MadEditFrame::OnUpdateUI_MenuToolsByteOrderMark)
	EVT_UPDATE_UI(menuNewLineChar, MadEditFrame::OnUpdateUI_MenuToolsNewLineChar)
	EVT_UPDATE_UI(menuInsertNewLineChar, MadEditFrame::OnUpdateUI_MenuToolsInsertNewLineChar)
	EVT_UPDATE_UI(menuConvertToDOS, MadEditFrame::OnUpdateUI_MenuToolsConvertNL)
	EVT_UPDATE_UI(menuConvertToMAC, MadEditFrame::OnUpdateUI_MenuToolsConvertNL)
	EVT_UPDATE_UI(menuConvertToUNIX, MadEditFrame::OnUpdateUI_MenuToolsConvertNL)
	EVT_UPDATE_UI(menuConvertEncoding, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuSimp2TradChinese, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuTrad2SimpChinese, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuKanji2TradChinese, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuKanji2SimpChinese, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuChinese2Kanji, MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding)
	EVT_UPDATE_UI(menuWordCount, MadEditFrame::OnUpdateUI_MenuFile_CheckCount)
	// window
	EVT_UPDATE_UI(menuToggleWindow, MadEditFrame::OnUpdateUI_MenuWindow_CheckCount)
	EVT_UPDATE_UI(menuNextWindow, MadEditFrame::OnUpdateUI_MenuWindow_CheckCount)
	EVT_UPDATE_UI(menuPreviousWindow, MadEditFrame::OnUpdateUI_MenuWindow_CheckCount)
	// help
	EVT_UPDATE_UI(menuCheckUpdates, MadEditFrame::OnUpdateUI_MenuHelp_CheckUpdates)
	// file
	EVT_MENU(menuNew, MadEditFrame::OnFileNew)
	EVT_MENU(menuOpen, MadEditFrame::OnFileOpen)
	EVT_MENU(menuSave, MadEditFrame::OnFileSave)
	EVT_MENU(menuSaveAs, MadEditFrame::OnFileSaveAs)
	EVT_MENU(menuSaveAll, MadEditFrame::OnFileSaveAll)
	EVT_MENU(menuReload, MadEditFrame::OnFileReload)
	EVT_MENU(menuClose, MadEditFrame::OnFileClose)
	EVT_MENU(menuCloseByPath, MadEditFrame::OnFileCloseByPath)
	EVT_MENU(menuCloseAll, MadEditFrame::OnFileCloseAll)
	EVT_MENU(menuCloseAllButThis, MadEditFrame::OnFileCloseAllButThis)
	EVT_MENU(menuCloseAllToTheLeft, MadEditFrame::OnFileCloseAllToTheLeft)
	EVT_MENU(menuCloseAllToTheRight, MadEditFrame::OnFileCloseAllToTheRight)
	EVT_MENU(menuCopyFullPath, MadEditFrame::OnFileCopyFullPath)
	EVT_MENU(menuCopyFilename, MadEditFrame::OnFileCopyFilename)
	EVT_MENU(menuCopyFileDir, MadEditFrame::OnFileCopyFileDir)
	EVT_MENU(menuPageSetup, MadEditFrame::OnFilePageSetup)
	EVT_MENU(menuPrintPreview, MadEditFrame::OnFilePrintPreview)
	EVT_MENU(menuPrint, MadEditFrame::OnFilePrint)
	EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, MadEditFrame::OnFileRecentFile)
	EVT_MENU(menuExit, MadEditFrame::OnFileExit)
	// edit
	EVT_MENU(menuUndo, MadEditFrame::OnEditUndo)
	EVT_MENU(menuRedo, MadEditFrame::OnEditRedo)
	EVT_MENU(menuCut, MadEditFrame::OnEditCut)
	EVT_MENU(menuCopy, MadEditFrame::OnEditCopy)
	EVT_MENU(menuPaste, MadEditFrame::OnEditPaste)
	EVT_MENU(menuPasteOvr, MadEditFrame::OnEditPasteOvr)
	EVT_MENU(menuDelete, MadEditFrame::OnEditDelete)
	EVT_MENU(menuCutLine, MadEditFrame::OnEditCutLine)
	EVT_MENU(menuDeleteLine, MadEditFrame::OnEditDeleteLine)
	EVT_MENU(menuSelectAll, MadEditFrame::OnEditSelectAll)
	EVT_MENU(menuInsertTabChar, MadEditFrame::OnEditInsertTabChar)
	EVT_MENU(menuInsertDateTime, MadEditFrame::OnEditInsertDateTime)
	EVT_MENU(menuSortAscending, MadEditFrame::OnEditSortAscending)
	EVT_MENU(menuSortDescending, MadEditFrame::OnEditSortDescending)
	EVT_MENU(menuSortAscendingCase, MadEditFrame::OnEditSortAscendingCase)
	EVT_MENU(menuSortDescendingCase, MadEditFrame::OnEditSortDescendingCase)
	EVT_MENU(menuSortByOptions, MadEditFrame::OnEditSortByOptions)
	EVT_MENU(menuSortOptions, MadEditFrame::OnEditSortOptions)
	EVT_MENU(menuCopyAsHexString, MadEditFrame::OnEditCopyAsHexString)
	EVT_MENU(menuCopyAsHexStringWithSpace, MadEditFrame::OnEditCopyAsHexStringWithSpace)
	EVT_MENU(menuIncreaseIndent, MadEditFrame::OnEditIncIndent)
	EVT_MENU(menuDecreaseIndent, MadEditFrame::OnEditDecIndent)
	EVT_MENU(menuComment, MadEditFrame::OnEditComment)
	EVT_MENU(menuUncomment, MadEditFrame::OnEditUncomment)
	EVT_MENU(menuWordWrapToNewLine, MadEditFrame::OnEditWordWrapToNewLine)
	EVT_MENU(menuNewLineToWordWrap, MadEditFrame::OnEditNewLineToWordWrap)
	EVT_MENU(menuToUpperCase, MadEditFrame::OnEditToUpperCase)
	EVT_MENU(menuToLowerCase, MadEditFrame::OnEditToLowerCase)
	EVT_MENU(menuInvertCase , MadEditFrame::OnEditInvertCase)
	EVT_MENU(menuToHalfWidth, MadEditFrame::OnEditToHalfWidth)
	EVT_MENU(menuToHalfWidthByOptions, MadEditFrame::OnEditToHalfWidthByOptions)
	EVT_MENU(menuToFullWidth, MadEditFrame::OnEditToFullWidth)
	EVT_MENU(menuToFullWidthByOptions, MadEditFrame::OnEditToFullWidthByOptions)
	EVT_MENU(menuTabToSpace, MadEditFrame::OnEditTabToSpace)
	EVT_MENU(menuSpaceToTab, MadEditFrame::OnEditSpaceToTab)
	EVT_MENU(menuTrimTrailingSpaces, MadEditFrame::OnEditTrimTrailingSpaces)
	EVT_MENU(menuInsertEnumeration, MadEditFrame::OnEditInsertEnumeration)
	EVT_MENU(menuColumnAlign, MadEditFrame::OnEditColumnAlign)
	EVT_MENU(menuColumnPaste, MadEditFrame::OnEditColumnPaste)
	// search
	EVT_MENU(menuFind, MadEditFrame::OnSearchFind)
	EVT_MENU(menuFindNext, MadEditFrame::OnSearchFindNext)
	EVT_MENU(menuFindPrevious, MadEditFrame::OnSearchFindPrevious)
	EVT_MENU(menuReplace, MadEditFrame::OnSearchReplace)
	EVT_MENU(menuFindInFiles, MadEditFrame::OnSearchFindInFiles)
	EVT_MENU(menuShowFindInFilesResults, MadEditFrame::OnSearchShowFindInFilesResults)
	EVT_MENU(menuGoToLine, MadEditFrame::OnSearchGoToLine)
	EVT_MENU(menuGoToPosition, MadEditFrame::OnSearchGoToPosition)
	EVT_MENU(menuLeftBrace, MadEditFrame::OnSearchGoToLeftBrace)
	EVT_MENU(menuRightBrace, MadEditFrame::OnSearchGoToRightBrace)
	EVT_MENU(menuToggleBookmark, MadEditFrame::OnSearchToggleBookmark)
	EVT_MENU(menuGotoNextBookmark, MadEditFrame::OnSearchGotoNextBookmark)
	EVT_MENU(menuGotoPreviousBookmark, MadEditFrame::OnSearchGotoPreviousBookmark)
	EVT_MENU(menuClearAllBookmarks, MadEditFrame::OnSearchClearAllBookmarks)
	// view
	EVT_MENU_RANGE(menuEncoding1, menuEncoding99, MadEditFrame::OnViewEncoding)
	EVT_MENU_RANGE(menuRecentEncoding1, menuRecentEncoding9, MadEditFrame::OnViewRecentEncoding)
	EVT_MENU_RANGE(menuSyntax1, menuSyntax199, MadEditFrame::OnViewSyntax)
	EVT_MENU_RANGE(menuFontName1, menuFontName999, MadEditFrame::OnViewFontName)
	EVT_MENU_RANGE(menuRecentFont1, menuRecentFont9, MadEditFrame::OnViewRecentFont)
	EVT_MENU_RANGE(menuFontSize1, menuFontSize99, MadEditFrame::OnViewFontSize)
	EVT_MENU(menuSetFont, MadEditFrame::OnViewSetFont)
	EVT_MENU(menuFixedWidthMode, MadEditFrame::OnViewFixedWidthMode)
	EVT_MENU_RANGE(menuTabColumn1, menuTabColumn16, MadEditFrame::OnViewTabColumn)
	EVT_MENU_RANGE(menuLineSpacing100, menuLineSpacing250, MadEditFrame::OnViewLineSpacing)
	EVT_MENU(menuNoWrap, MadEditFrame::OnViewNoWrap)
	EVT_MENU(menuWrapByWindow, MadEditFrame::OnViewWrapByWindow)
	EVT_MENU(menuWrapByColumn, MadEditFrame::OnViewWrapByColumn)
	EVT_MENU(menuDisplayBookmark, MadEditFrame::OnViewDisplayBookmark)
	EVT_MENU(menuDisplayLineNumber, MadEditFrame::OnViewDisplayLineNumber)
	EVT_MENU(menuShowEndOfLine, MadEditFrame::OnViewShowEndOfLine)
	EVT_MENU(menuShowTabChar, MadEditFrame::OnViewShowTabChar)
	EVT_MENU(menuShowSpaceChar, MadEditFrame::OnViewShowSpaceChar)
	EVT_MENU(menuMarkActiveLine, MadEditFrame::OnViewMarkActiveLine)
	EVT_MENU(menuMarkBracePair, MadEditFrame::OnViewMarkBracePair)
	EVT_MENU(menuTextMode, MadEditFrame::OnViewTextMode)
	EVT_MENU(menuColumnMode, MadEditFrame::OnViewColumnMode)
	EVT_MENU(menuHexMode, MadEditFrame::OnViewHexMode)
	// tools
	EVT_MENU(menuOptions, MadEditFrame::OnToolsOptions)
	EVT_MENU(menuHighlighting, MadEditFrame::OnToolsHighlighting)
	#ifdef __WXMSW__
	EVT_MENU(menuFileAssociation, MadEditFrame::OnToolsFileAssociation)
	#endif
	EVT_MENU(menuPurgeHistories, MadEditFrame::OnToolsPurgeHistories)
	EVT_MENU(menuToggleBOM, MadEditFrame::OnToolsToggleBOM)
	EVT_MENU(menuConvertToDOS, MadEditFrame::OnToolsConvertToDOS)
	EVT_MENU(menuConvertToMAC, MadEditFrame::OnToolsConvertToMAC)
	EVT_MENU(menuConvertToUNIX, MadEditFrame::OnToolsConvertToUNIX)
	EVT_MENU(menuInsertDOS, MadEditFrame::OnToolsInsertDOS)
	EVT_MENU(menuInsertMAC, MadEditFrame::OnToolsInsertMAC)
	EVT_MENU(menuInsertUNIX, MadEditFrame::OnToolsInsertUNIX)
	EVT_MENU(menuConvertEncoding, MadEditFrame::OnToolsConvertEncoding)
	EVT_MENU(menuSimp2TradChinese, MadEditFrame::OnToolsSimp2TradChinese)
	EVT_MENU(menuTrad2SimpChinese, MadEditFrame::OnToolsTrad2SimpChinese)
	EVT_MENU(menuKanji2TradChinese, MadEditFrame::OnToolsKanji2TradChinese)
	EVT_MENU(menuKanji2SimpChinese, MadEditFrame::OnToolsKanji2SimpChinese)
	EVT_MENU(menuChinese2Kanji, MadEditFrame::OnToolsChinese2Kanji)
	EVT_MENU(menuSimp2TradClipboard, MadEditFrame::OnToolsSimp2TradClipboard)
	EVT_MENU(menuTrad2SimpClipboard, MadEditFrame::OnToolsTrad2SimpClipboard)
	EVT_MENU(menuKanji2TradClipboard, MadEditFrame::OnToolsKanji2TradClipboard)
	EVT_MENU(menuKanji2SimpClipboard, MadEditFrame::OnToolsKanji2SimpClipboard)
	EVT_MENU(menuChinese2KanjiClipboard, MadEditFrame::OnToolsChinese2KanjiClipboard)
	EVT_MENU(menuWordCount, MadEditFrame::OnToolsWordCount)
	// window
	EVT_MENU(menuToggleWindow, MadEditFrame::OnWindowToggleWindow)
	EVT_MENU(menuNextWindow, MadEditFrame::OnWindowNextWindow)
	EVT_MENU(menuPreviousWindow, MadEditFrame::OnWindowPreviousWindow)
	// help
	EVT_MENU(menuCheckUpdates, MadEditFrame::OnHelpCheckUpdates)
	EVT_MENU(menuAbout, MadEditFrame::OnHelpAbout)
	
	EVT_CLOSE(MadEditFrame::MadEditFrameClose)
	EVT_KEY_DOWN(MadEditFrame::MadEditFrameKeyDown)
END_EVENT_TABLE()



//==========================================================
// do not use wxGetTranslation() now
#undef _
#define _(s)    wxT(s)

CommandData CommandTable[]=
{
    // file
    { 0, 0, 0, 0, _("&File"),       0, wxITEM_NORMAL, 0, &g_Menu_File, 0},
    { 0, 1, menuNew,                wxT("menuNew"),                _("&New File"),                  wxT("Ctrl-N"),       wxITEM_NORMAL,    new_xpm_idx,       0,                        _("Create a new file")},
    { 0, 1, menuOpen,               wxT("menuOpen"),               _("&Open File..."),              wxT("Ctrl-O"),       wxITEM_NORMAL,    fileopen_xpm_idx,  0,                        _("Open an existing file")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuSave,               wxT("menuSave"),               _("&Save File"),                 wxT("Ctrl-S"),       wxITEM_NORMAL,    filesave_xpm_idx,  0,                        _("Save the file")},
    { 0, 1, menuSaveAs,             wxT("menuSaveAs"),             _("Save &As..."),                wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Save the file with a new name")},
    { 0, 1, menuSaveAll,            wxT("menuSaveAll"),            _("Sa&ve All"),                  wxT("Ctrl-Shift-S"), wxITEM_NORMAL,    saveall_xpm_idx,   0,                        _("Save all files")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuReload,             wxT("menuReload"),             _("&Reload File"),               wxT("Ctrl-R"),       wxITEM_NORMAL,    -1,                0,                        _("Reload the file")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuClose,              wxT("menuClose"),              _("&Close File"),                wxT("Ctrl-F4"),      wxITEM_NORMAL,    fileclose_xpm_idx, 0,                        _("Close the file")},
    { 0, 1, menuCloseAll,           wxT("menuCloseAll"),           _("C&lose All"),                 wxT(""),             wxITEM_NORMAL,    closeall_xpm_idx,  0,                        _("Close all files")},
    { 0, 1, menuCloseByPath,        wxT("menuCloseByPath"),        _("Close All under &Folder..."), wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Close all files in given folder and in all the subfolders")},
    { 0, 1, menuCloseMore,          wxT("menuCloseMore"),          _("Close &More"),                0,                   wxITEM_NORMAL,    -1,                &g_Menu_File_CloseMore,    0 },
    { 0, 2, menuCloseAllButThis,    wxT("menuCloseAllButThis"),    _("Close All &BUT This"),        wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Close all files except the current one")},
    { 0, 2, menuCloseAllToTheLeft,  wxT("menuCloseAllToTheLeft"),  _("Close All To The &Left"),     wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Close all files at the left side of current one")},
    { 0, 2, menuCloseAllToTheRight, wxT("menuCloseAllToTheRight"), _("Close All To The &Right"),    wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Close all files at the right side of current one")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuCopyFilePath,       wxT("menuCopyFilePath"),       _("Copy File Pat&h"),             0,                   wxITEM_NORMAL,    -1,                &g_Menu_File_CopyPath,    0 },
    { 0, 2, menuCopyFullPath,       wxT("menuCopyFullPath"),       _("Copy &Full Path"),            wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Copy full file path to clipboard")},
    { 0, 2, menuCopyFilename,       wxT("menuCopyFilename"),       _("Copy File &Name"),            wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Copy file name to clipboard")},
    { 0, 2, menuCopyFileDir,        wxT("menuCopyFileDir"),        _("Copy Containing &Directory"), wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Copy containing directory to clipboard")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuPageSetup,          wxT("menuPageSetup"),          _("Page Set&up..."),             wxT(""),             wxITEM_NORMAL,    -1,                0,                        _("Setup the pages for printing")},
    { 0, 1, menuPrintPreview,       wxT("menuPrintPreview"),       _("Print Previe&w..."),          wxT(""),             wxITEM_NORMAL,    preview_xpm_idx,   0,                        _("Preview the result of printing")},
    { 0, 1, menuPrint,              wxT("menuPrint"),              _("&Print..."),                  wxT("Ctrl-P"),       wxITEM_NORMAL,    print_xpm_idx,     0,                        _("Print the file")},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuRecentFiles,        wxT("menuRecentFiles"),        _("Recent Files"),               wxT(""),             wxITEM_NORMAL,    -1,                &g_Menu_File_RecentFiles, 0},
    { 0, 1, 0,                      0,                             0,                               0,                   wxITEM_SEPARATOR, -1,                0,                        0},
    { 0, 1, menuExit,               wxT("menuExit"),               _("E&xit"),                      wxT(""),             wxITEM_NORMAL,    quit_xpm_idx,      0,                        _("Quit wxMEdit")},

    // Edit
    { 0, 0, 0, 0, _("&Edit"), 0, wxITEM_NORMAL, 0, &g_Menu_Edit, 0},
    { ecUndo,           1, menuUndo,                     wxT("menuUndo"),                     _("&Undo"),                                   wxT("Ctrl-Z"),       wxITEM_NORMAL,    undo_xpm_idx,      0,                     _("Undo the last action")},

    { ecRedo,           1, menuRedo,                     wxT("menuRedo"),                     _("&Redo"),
#ifdef __WXMSW__
                                                                                                                                            wxT("Ctrl-Y"),
#else
                                                                                                                                            wxT("Ctrl-Shift-Z"),
#endif
                                                                                                                                                                 wxITEM_NORMAL,    redo_xpm_idx,      0,                     _("Redo the previously undone action")},

    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecCut,            1, menuCut,                      wxT("menuCut"),                      _("Cu&t"),                                    wxT("Ctrl-X"),       wxITEM_NORMAL,    cut_xpm_idx,       0,                     _("Cut the selection and put it on the Clipboard")},
    { ecCopy,           1, menuCopy,                     wxT("menuCopy"),                     _("&Copy"),                                   wxT("Ctrl-C"),       wxITEM_NORMAL,    copy_xpm_idx,      0,                     _("Copy the selection and put it on the Clipboard")},
    { ecPaste,          1, menuPaste,                    wxT("menuPaste"),                    _("&Paste"),                                  wxT("Ctrl-V"),       wxITEM_NORMAL,    paste_xpm_idx,     0,                     _("Insert data from the Clipboard")},
    { ecPasteOvr,       1, menuPasteOvr,                 wxT("menuPasteOvr"),                 _("Paste with Over&writing"),                 wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Overwrite data from the Clipboard")},
    { ecDelete,         1, menuDelete,                   wxT("menuDelete"),                   _("&Delete"),                                 wxT("DEL"),          wxITEM_NORMAL,    delete_xpm_idx,   0,                     _("Delete data")},
    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecCutLine,        1, menuCutLine,                  wxT("menuCutLine"),                  _("Cut L&ine"),                               wxT("Ctrl-Shift-L"), wxITEM_NORMAL,    -1,                0,                     _("Cut the selected lines and put it on the Clipboard")},

    { ecDeleteLine,     1, menuDeleteLine,               wxT("menuDeleteLine"),               _("Delete &Line"),
#ifdef __WXMSW__
                                                                                                                                            wxT("Ctrl-L"),
#else
                                                                                                                                            wxT("Ctrl-Y"),
#endif
                                                                                                                                                                 wxITEM_NORMAL,    -1,                0,                     _("Delete the selected lines")},

    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecSelectAll,      1, menuSelectAll,                wxT("menuSelectAll"),                _("Select &All"),                             wxT("Ctrl-A"),       wxITEM_NORMAL,    -1,                0,                     _("Select all data")},
    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},

    { ecInsertTabChar,  1, menuInsertTabChar,            wxT("menuInsertTabChar"),            _("Insert Ta&b Char"),
#ifdef __WXMSW__
                                                                                                                                            wxT("Ctrl-~"),
#else
                                                                                                                                            wxT("Ctrl-`"),
#endif
                                                                                                                                                                 wxITEM_NORMAL,    -1,                0,                     _("Insert a Tab char at current position")},

    { ecInsertDateTime, 1, menuInsertDateTime,           wxT("menuInsertDateTime"),           _("Insert Dat&e and Time"),                   wxT("F7"),           wxITEM_NORMAL,    -1,                0,                     _("Insert date and time at current position")},
    { 0,                1, menuInsertEnumeration,        wxT("menuInsertEnumeration"),        _("Insert &Ordered Sequence..."),             wxT("Ctrl-Alt-N"),   wxITEM_NORMAL,    olist_xpm_idx,     0,                     _("Insert ordered sequence with certain format in specified numbering system")},

    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0 },
    { 0,                1, menuColumn,                   wxT("menuColumn"),                   _("Colum&n"),                                  0,                   wxITEM_NORMAL,    -1,                &g_Menu_Edit_Column,   0 },
    { 0,                2, menuColumnAlign,              wxT("menuColumnAlign"),              _("Column &Align"),                           wxT("Alt-DEL"),      wxITEM_NORMAL,    -1,                0,                     _("Delete spaces at the right of column selection") },
    { 0,                2, menuColumnPaste,              wxT("menuColumnPaste"),              _("Column &Paste"),                           wxT("Alt-INS"),      wxITEM_NORMAL,    -1,                0,                     _("Paste in each line of column selection") },

    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                1, menuAdvanced,                 wxT("menuAdvanced"),                 _("Ad&vanced"),                               0,                   wxITEM_NORMAL,    -1,                &g_Menu_Edit_Advanced, 0},
    { 0,                2, menuCopyAsHexString,          wxT("menuCopyAsHexString"),          _("Copy As &Hex String"),                     wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Copy the selection as hex-string")},
    { 0,                2, menuCopyAsHexStringWithSpace, wxT("menuCopyAsHexStringWithSpace"), _("Copy As He&x String with Space"),          wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Copy the selection as hex-string with space")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecIncreaseIndent, 2, menuIncreaseIndent,           wxT("menuIncreaseIndent"),           _("&Increase Indent"),                        wxT(""),             wxITEM_NORMAL,    indent_xpm_idx,    0,                     _("Increase the indent of seleted lines")},
    { ecDecreaseIndent, 2, menuDecreaseIndent,           wxT("menuDecreaseIndent"),           _("&Decrease Indent"),                        wxT("Shift-TAB"),    wxITEM_NORMAL,    unindent_xpm_idx,  0,                     _("Decrease the indent of seleted lines")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecComment,        2, menuComment,                  wxT("menuComment"),                  _("&Comment"),                                wxT("Ctrl-M"),       wxITEM_NORMAL,    comment_xpm_idx,   0,                     _("Comment out the selected lines")},
    { ecUncomment,      2, menuUncomment,                wxT("menuUncomment"),                _("&Uncomment"),                              wxT("Ctrl-Shift-M"), wxITEM_NORMAL,    uncomment_xpm_idx, 0,                     _("Uncomment the selected lines")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                2, menuWordWrapToNewLine,        wxT("menuWordWrapToNewLine"),        _("WordWraps To NewLine Chars"),              wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert WordWraps to NewLine chars in the file or selection")},
    { 0,                2, menuNewLineToWordWrap,        wxT("menuNewLineToWordWrap"),        _("NewLine Chars To WordWraps"),              wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert NewLine chars to WordWraps in the selection")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecToUpperCase,    2, menuToUpperCase,              wxT("menuToUpperCase"),              _("To U&pperCase"),                           wxT("Ctrl-U"),       wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to uppercase")},
    { ecToLowerCase,    2, menuToLowerCase,              wxT("menuToLowerCase"),              _("To L&owerCase"),                           wxT("Ctrl-Shift-U"), wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to lowercase")},
    { ecInvertCase ,    2, menuInvertCase ,              wxT("menuInvertCase") ,              _("Inver&t Case"),                            wxT("Ctrl-Alt-U"),   wxITEM_NORMAL,    -1,                0,                     _("Invert the case of the selection")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { ecToHalfWidth,    2, menuToHalfWidth,              wxT("menuToHalfWidth"),              _("To H&alfwidth"),                           wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to halfwidth")},
    { 0,                2, menuToHalfWidthByOptions,     wxT("menuToHalfWidthByOptions"),     _("To Halfwidth by Options..."),              wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to halfwidth by options")},
    { ecToFullWidth,    2, menuToFullWidth,              wxT("menuToFullWidth"),              _("To &Fullwidth"),                           wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to fullwidth")},
    { 0,                2, menuToFullWidthByOptions,     wxT("menuToFullWidthByOptions"),     _("To Fullwidth by Options..."),              wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert the selection to fullwidth by options")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                2, menuTabToSpace,               wxT("menuTabToSpace"),               _("Tab Chars To Space Chars"),                wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert Tab chars to Space chars in the selection")},
    { 0,                2, menuSpaceToTab,               wxT("menuSpaceToTab"),               _("Space Chars To Tab Chars"),                wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Convert Space chars to Tab chars in the selection")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                2, menuTrimTrailingSpaces,       wxT("menuTrimTrailingSpaces"),       _("Tri&m Trailing Spaces"),                   wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Trim trailing spaces at the end of lines")},
    { 0,                1, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                1, menuSort,                     wxT("menuSort"),                     _("&Sort"),                                   0,                   wxITEM_NORMAL,    -1,                &g_Menu_Edit_Sort,     0},
    { 0,                2, menuSortAscending,            wxT("menuSortAscending"),            _("Sort Lines (&Ascending)"),                 wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Sort the selected or all lines in ascending order")},
    { 0,                2, menuSortDescending,           wxT("menuSortDescending"),           _("Sort Lines (&Descending)"),                wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Sort the selected or all lines in descending order")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                2, menuSortAscendingCase,        wxT("menuSortAscendingCase"),        _("Sort Lines (A&scending, CaseSensitive)"),  wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Sort lines in ascending order and with case-sensitive")},
    { 0,                2, menuSortDescendingCase,       wxT("menuSortDescendingCase"),       _("Sort Lines (D&escending, CaseSensitive)"), wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Sort lines in descending order and with case-sensitive")},
    { 0,                2, 0,                            0,                                   0,                                            0,                   wxITEM_SEPARATOR, -1,                0,                     0},
    { 0,                2, menuSortByOptions,            wxT("menuSortByOptions"),            _("Sort Lines by &Current Options"),          wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Sort the selected or all lines by using current options")},
    { 0,                2, menuSortOptions,              wxT("menuSortOptions"),              _("Sort &Options..."),                        wxT(""),             wxITEM_NORMAL,    -1,                0,                     _("Set the sort options")},

    // Search
    { 0, 0, 0, 0, _("&Search"), 0, wxITEM_NORMAL, 0, &g_Menu_Search, 0},
    { 0,            1, menuFind,                   wxT("menuFind"),                   _("&Find..."),                                        wxT("Ctrl-F"),       wxITEM_NORMAL,    find_xpm_idx,        0, _("Find a string")},
    { 0,            1, menuFindNext,               wxT("menuFindNext"),               _("Find &Next"),                                      wxT("F3"),           wxITEM_NORMAL,    findnext_xpm_idx,    0, _("Find next occurrence")},
    { 0,            1, menuFindPrevious,           wxT("menuFindPrevious"),           _("Find &Previous"),                                  wxT("Ctrl-F3"),      wxITEM_NORMAL,    findprev_xpm_idx,    0, _("Find previous occurrence")},
    { 0,            1, 0,                          0,                                 0,                                                    0,                   wxITEM_SEPARATOR, -1,                  0, 0},
    { 0,            1, menuReplace,                wxT("menuReplace"),                _("&Replace..."),                                     wxT("Ctrl-H"),       wxITEM_NORMAL,    replace_xpm_idx,     0, _("Replace a string")},
    { 0,            1, 0,                          0,                                 0,                                                    0,                   wxITEM_SEPARATOR, -1,                  0, 0},
    { 0,            1, menuFindInFiles,            wxT("menuFindInFiles"),            _("Fin&d/Replace in Files..."),                       wxT("Ctrl-Shift-F"), wxITEM_NORMAL,    -1,                  0, _("Find or replace a string in files")},
    { 0,            1, menuShowFindInFilesResults, wxT("menuShowFindInFilesResults"), _("&Show/Hide the Results of Find/Replace in Files"), wxT("Ctrl-Shift-R"), wxITEM_NORMAL,    -1,                  0, _("Show or hide the results of find or replace a string in files")},
    { 0,            1, 0,                          0,                                 0,                                                    0,                   wxITEM_SEPARATOR, -1,                  0, 0},
    { 0,            1, menuGoToLine,               wxT("menuGoToLine"),               _("&Go To Line..."),                                  wxT("Ctrl-G"),       wxITEM_NORMAL,    -1,                  0, _("Go to the specified line")},
    { 0,            1, menuGoToPosition,           wxT("menuGoToPosition"),           _("G&o To Position..."),                              wxT("Ctrl-Shift-G"), wxITEM_NORMAL,    -1,                  0, _("Go to the specified position")},
    { 0,            1, 0,                          0,                                 0,                                                    0,                   wxITEM_SEPARATOR, -1,                  0, 0},
    { ecLeftBrace,  1, menuLeftBrace,              wxT("menuLeftBrace"),              _("Go To L&eft Brace"),                               wxT("Ctrl-["),       wxITEM_NORMAL,    -1,                  0, _("Go to left brace")},
    { ecRightBrace, 1, menuRightBrace,             wxT("menuRightBrace"),             _("Go To R&ight Brace"),                              wxT("Ctrl-]"),       wxITEM_NORMAL,    -1,                  0, _("Go to right brace")},

    { 0,            1, 0,                          0,                                 0,                                                    0,                   wxITEM_SEPARATOR, -1,                  0, 0},
    { 0,            1, menuToggleBookmark,         wxT("menuToggleBookmark"),         _("Toggle Bookmark"),                                 wxT("Ctrl-F2"),      wxITEM_NORMAL,    bmktoggle_xpm_idx,   0, _("Toggle bookmark at current line")},
    { 0,            1, menuGotoNextBookmark,       wxT("menuGotoNextBookmark"),       _("Go To Next Bookmark"),                             wxT("F2"),           wxITEM_NORMAL,    bmknext_xpm_idx,     0, _("Go to the next bookmark")},
    { 0,            1, menuGotoPreviousBookmark,   wxT("menuGotoPreviousBookmark"),   _("Go To Previous Bookmark"),                         wxT("Shift-F2"),     wxITEM_NORMAL,    bmkprev_xpm_idx,     0, _("Go to the previous bookmark")},
    { 0,            1, menuClearAllBookmarks,      wxT("menuClearAllBookmarks"),      _("Clear All Bookmarks"),                             0,                   wxITEM_NORMAL,    bmkclearall_xpm_idx, 0, _("Clear all bookmarks from current file")},

    // View
    { 0, 0, 0, 0, _("&View"), 0, wxITEM_NORMAL, 0, &g_Menu_View, 0},
    { 0,            1, menuEncoding,          wxT("menuEncoding"),          _("Encoding: "),           0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Encoding,     0},
    { 0,            2, menuAllEncodings,      wxT("menuAllEncodings"),      _("All "),                 0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_AllEncodings, 0},
    { 0,            1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { 0,            1, menuSyntax,            wxT("menuSyntax"),            _("Syntax Type: "),        0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Syntax,       0},
    { 0,            1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { 0,            1, menuFontName,          wxT("menuFontName"),          _("Font Name: "),          0,                   wxITEM_NORMAL,    fontname_xpm_idx,   &g_Menu_View_FontName,     0},
    { 0,            2, menuFont0,             wxT("menuFont0"),             _("[@]"),                  0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font0,        0},
    { 0,            2, menuFont1,             wxT("menuFont1"),             _("[ABCDE]"),              0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font1,        0},
    { 0,            2, menuFont2,             wxT("menuFont2"),             _("[FGHIJ]"),              0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font2,        0},
    { 0,            2, menuFont3,             wxT("menuFont3"),             _("[KLMNO]"),              0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font3,        0},
    { 0,            2, menuFont4,             wxT("menuFont4"),             _("[PQRST]"),              0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font4,        0},
    { 0,            2, menuFont5,             wxT("menuFont5"),             _("[UVWXYZ]"),             0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font5,        0},
    { 0,            2, menuFont6,             wxT("menuFont6"),             _("[other]"),              0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_Font6,        0},

    { 0,            1, menuFontSize,          wxT("menuFontSize"),          _("Font Size: "),          0,                   wxITEM_NORMAL,    fontsize_xpm_idx,   &g_Menu_View_FontSize,     0},
    { 0,            2, menuFontSize1,         wxT("menuFontSize1"),         wxT(" 1 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         _("Set font point-size")},
    { 0,            2, menuFontSize1+ 1,      wxT("menuFontSize2"),         wxT(" 2 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 2,      wxT("menuFontSize3"),         wxT(" 3 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 3,      wxT("menuFontSize4"),         wxT(" 4 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 4,      wxT("menuFontSize5"),         wxT(" 5 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 5,      wxT("menuFontSize6"),         wxT(" 6 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 6,      wxT("menuFontSize7"),         wxT(" 7 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 7,      wxT("menuFontSize8"),         wxT(" 8 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 8,      wxT("menuFontSize9"),         wxT(" 9 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+ 9,      wxT("menuFontSize10"),        wxT(" 10 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+10,      wxT("menuFontSize11"),        wxT(" 11 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+11,      wxT("menuFontSize12"),        wxT(" 12 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+12,      wxT("menuFontSize13"),        wxT(" 13 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+13,      wxT("menuFontSize14"),        wxT(" 14 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+14,      wxT("menuFontSize15"),        wxT(" 15 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+15,      wxT("menuFontSize16"),        wxT(" 16 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+16,      wxT("menuFontSize17"),        wxT(" 17 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+17,      wxT("menuFontSize18"),        wxT(" 18 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+18,      wxT("menuFontSize19"),        wxT(" 19 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+19,      wxT("menuFontSize20"),        wxT(" 20 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+20,      wxT("menuFontSize21"),        wxT(" 21 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+21,      wxT("menuFontSize22"),        wxT(" 22 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+22,      wxT("menuFontSize23"),        wxT(" 23 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+23,      wxT("menuFontSize24"),        wxT(" 24 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+24,      wxT("menuFontSize25"),        wxT(" 25 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+25,      wxT("menuFontSize26"),        wxT(" 26 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+26,      wxT("menuFontSize27"),        wxT(" 27 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+27,      wxT("menuFontSize28"),        wxT(" 28 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+28,      wxT("menuFontSize29"),        wxT(" 29 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+29,      wxT("menuFontSize30"),        wxT(" 30 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+30,      wxT("menuFontSize31"),        wxT(" 31 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+31,      wxT("menuFontSize32"),        wxT(" 32 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+32,      wxT("menuFontSize33"),        wxT(" 33 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+33,      wxT("menuFontSize34"),        wxT(" 34 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+34,      wxT("menuFontSize35"),        wxT(" 35 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+35,      wxT("menuFontSize36"),        wxT(" 36 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+36,      wxT("menuFontSize37"),        wxT(" 37 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+37,      wxT("menuFontSize38"),        wxT(" 38 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+38,      wxT("menuFontSize39"),        wxT(" 39 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+39,      wxT("menuFontSize40"),        wxT(" 40 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+40,      wxT("menuFontSize41"),        wxT(" 41 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+41,      wxT("menuFontSize42"),        wxT(" 42 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+42,      wxT("menuFontSize43"),        wxT(" 43 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+43,      wxT("menuFontSize44"),        wxT(" 44 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+44,      wxT("menuFontSize45"),        wxT(" 45 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+45,      wxT("menuFontSize46"),        wxT(" 46 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+46,      wxT("menuFontSize47"),        wxT(" 47 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+47,      wxT("menuFontSize48"),        wxT(" 48 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+48,      wxT("menuFontSize49"),        wxT(" 49 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+49,      wxT("menuFontSize50"),        wxT(" 50 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+50,      wxT("menuFontSize51"),        wxT(" 51 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+51,      wxT("menuFontSize52"),        wxT(" 52 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+52,      wxT("menuFontSize53"),        wxT(" 53 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+53,      wxT("menuFontSize54"),        wxT(" 54 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+54,      wxT("menuFontSize55"),        wxT(" 55 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+55,      wxT("menuFontSize56"),        wxT(" 56 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+56,      wxT("menuFontSize57"),        wxT(" 57 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+57,      wxT("menuFontSize58"),        wxT(" 58 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+58,      wxT("menuFontSize59"),        wxT(" 59 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+59,      wxT("menuFontSize60"),        wxT(" 60 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+60,      wxT("menuFontSize61"),        wxT(" 61 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+61,      wxT("menuFontSize62"),        wxT(" 62 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+62,      wxT("menuFontSize63"),        wxT(" 63 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+63,      wxT("menuFontSize64"),        wxT(" 64 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+64,      wxT("menuFontSize65"),        wxT(" 65 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+65,      wxT("menuFontSize66"),        wxT(" 66 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+66,      wxT("menuFontSize67"),        wxT(" 67 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+67,      wxT("menuFontSize68"),        wxT(" 68 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+68,      wxT("menuFontSize69"),        wxT(" 69 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+69,      wxT("menuFontSize70"),        wxT(" 70 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+70,      wxT("menuFontSize71"),        wxT(" 71 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},
    { 0,            2, menuFontSize1+71,      wxT("menuFontSize72"),        wxT(" 72 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set font point-size")},

    { 0,            1, menuSetFont,           wxT("menuSetFont"),           _("Set Font..."),          wxT(""),             wxITEM_NORMAL,    font_xpm_idx,       0,                         _("Change font settings")},
    { 0,            1, menuFixedWidthMode,    wxT("menuFixedWidthMode"),    _("&Fixed Width Mode"),    wxT("Ctrl-Alt-F"),   wxITEM_CHECK,     -1,                 0,                         _("Set/Unset the font with Fixed Width")},

    { 0,            1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { 0,            1, menuTabColumn,         wxT("menuTabColumn"),         _("Tab Column: "),         0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_TabColumn,    0},
    { 0,            2, menuTabColumn1,        wxT("menuTabColumn1"),        wxT(" 1 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         _("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 1,     wxT("menuTabColumn2"),        wxT(" 2 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 2,     wxT("menuTabColumn3"),        wxT(" 3 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 3,     wxT("menuTabColumn4"),        wxT(" 4 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 4,     wxT("menuTabColumn5"),        wxT(" 5 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 5,     wxT("menuTabColumn6"),        wxT(" 6 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 6,     wxT("menuTabColumn7"),        wxT(" 7 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 7,     wxT("menuTabColumn8"),        wxT(" 8 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 8,     wxT("menuTabColumn9"),        wxT(" 9 "),                0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+ 9,     wxT("menuTabColumn10"),       wxT(" 10 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+10,     wxT("menuTabColumn11"),       wxT(" 11 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+11,     wxT("menuTabColumn12"),       wxT(" 12 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+12,     wxT("menuTabColumn13"),       wxT(" 13 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+13,     wxT("menuTabColumn14"),       wxT(" 14 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+14,     wxT("menuTabColumn15"),       wxT(" 15 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},
    { 0,            2, menuTabColumn1+15,     wxT("menuTabColumn16"),       wxT(" 16 "),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set column of Tab char")},

    { 0,            1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { 0,            1, menuLineSpacing,       wxT("menuLineSpacing"),       _("Line Spacing: "),       0,                   wxITEM_NORMAL,    -1,                 &g_Menu_View_LineSpacing,  0},
    { 0,            2, menuLineSpacing100,    wxT("menuLineSpacing100"),    wxT("100%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         _("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 1, wxT("menuLineSpacing105"),    wxT("105%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 2, wxT("menuLineSpacing110"),    wxT("110%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 3, wxT("menuLineSpacing115"),    wxT("115%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 4, wxT("menuLineSpacing120"),    wxT("120%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 5, wxT("menuLineSpacing125"),    wxT("125%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 6, wxT("menuLineSpacing130"),    wxT("130%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 7, wxT("menuLineSpacing135"),    wxT("135%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 8, wxT("menuLineSpacing140"),    wxT("140%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+ 9, wxT("menuLineSpacing145"),    wxT("145%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+10, wxT("menuLineSpacing150"),    wxT("150%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+11, wxT("menuLineSpacing155"),    wxT("155%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+12, wxT("menuLineSpacing160"),    wxT("160%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+13, wxT("menuLineSpacing165"),    wxT("165%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+14, wxT("menuLineSpacing170"),    wxT("170%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+15, wxT("menuLineSpacing175"),    wxT("175%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+16, wxT("menuLineSpacing180"),    wxT("180%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+17, wxT("menuLineSpacing185"),    wxT("185%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+18, wxT("menuLineSpacing190"),    wxT("190%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+19, wxT("menuLineSpacing195"),    wxT("195%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+20, wxT("menuLineSpacing200"),    wxT("200%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+21, wxT("menuLineSpacing205"),    wxT("205%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+22, wxT("menuLineSpacing210"),    wxT("210%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+23, wxT("menuLineSpacing215"),    wxT("215%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+24, wxT("menuLineSpacing220"),    wxT("220%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+25, wxT("menuLineSpacing225"),    wxT("225%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+26, wxT("menuLineSpacing230"),    wxT("230%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+27, wxT("menuLineSpacing235"),    wxT("235%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+28, wxT("menuLineSpacing240"),    wxT("240%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+29, wxT("menuLineSpacing245"),    wxT("245%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},
    { 0,            2, menuLineSpacing100+30, wxT("menuLineSpacing250"),    wxT("250%"),               0,                   wxITEM_NORMAL,    -1,                 0,                         wxT("Set line spacing")},

    { 0,              1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { ecNoWrap,       1, menuNoWrap,            wxT("menuNoWrap"),            _("&No Wrap"),             wxT("Ctrl-Q"),       wxITEM_CHECK,     nowrap_xpm_idx,     0,                         _("Do not wrap lines")},
    { ecWrapByWindow, 1, menuWrapByWindow,      wxT("menuWrapByWindow"),      _("Wrap By &Window"),      wxT("Ctrl-W"),       wxITEM_CHECK,     wrapbywin_xpm_idx,  0,                         _("Wrap the lines by the window width")},
    { ecWrapByColumn, 1, menuWrapByColumn,      wxT("menuWrapByColumn"),      _("Wrap By Column"),       wxT("Ctrl-E"),       wxITEM_CHECK,     wrapbycol_xpm_idx,  0,                         _("Wrap the lines by the specified Max Columns")},
    { 0,              1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { 0,              1, menuDisplayBookmark,   wxT("menuDisplayBookmark"),   _("Display &Bookmark"),    wxT("Ctrl-Alt-B"),   wxITEM_CHECK,     -1,                 0,                         _("Display the Bookmarks")},
    { 0,              1, menuDisplayLineNumber, wxT("menuDisplayLineNumber"), _("&Display Line Number"), wxT("Ctrl-Alt-D"),   wxITEM_CHECK,     -1,                 0,                         _("Display the Line Numbers")},
    { 0,              1, menuShowEndOfLine,     wxT("menuShowEndOfLine"),     _("Show End Of Line"),     wxT("Ctrl-Alt-L"),   wxITEM_CHECK,     -1,                 0,                         _("Show the sign of EndOfLine")},
    { 0,              1, menuShowTabChar,       wxT("menuShowTabChar"),       _("Show Tab Char"),        wxT("Ctrl-Alt-T"),   wxITEM_CHECK,     -1,                 0,                         _("Show the sign of Tab char")},
    { 0,              1, menuShowSpaceChar,     wxT("menuShowSpaceChar"),     _("Show Space Char"),      wxT("Ctrl-Alt-S"),   wxITEM_CHECK,     -1,                 0,                         _("Show the sign of Space char")},
    { 0,              1, menuMarkActiveLine,    wxT("menuMarkActiveLine"),    _("Mark Active Line"),     wxT(""),             wxITEM_CHECK,     -1,                 0,                         _("Mark the current line")},
    { 0,              1, menuMarkBracePair,     wxT("menuMarkBracePair"),     _("Mark Brace Pair"),      wxT(""),             wxITEM_CHECK,     -1,                 0,                         _("Mark the BracePair under the caret")},
    { 0,              1, 0,                     0,                            0,                         0,                   wxITEM_SEPARATOR, -1,                 0,                         0},
    { ecTextMode,     1, menuTextMode,          wxT("menuTextMode"),          _("&Text Mode"),           wxT("Alt-1"),        wxITEM_CHECK,     textmode_xpm_idx,   0,                         _("Change the editing mode to Text-Mode")},
    { ecColumnMode,   1, menuColumnMode,        wxT("menuColumnMode"),        _("&Column Mode"),         wxT("Alt-2"),        wxITEM_CHECK,     columnmode_xpm_idx, 0,                         _("Change the editing mode to Column-Mode")},
    { ecHexMode,      1, menuHexMode,           wxT("menuHexMode"),           _("&Hex Mode"),            wxT("Alt-3"),        wxITEM_CHECK,     hexmode_xpm_idx,    0,                         _("Change the editing mode to Hex-Mode")},

    // Tools
    { 0, 0, 0, 0, _("&Tools"), 0, wxITEM_NORMAL, 0, &g_Menu_Tools, 0},
    { 0,               1, menuOptions,            wxT("menuOptions"),            _("&Options..."),                                   wxT(""),       wxITEM_NORMAL,    options_xpm_idx, 0,                   _("Change the configuration")},
    { 0,               1, menuHighlighting,       wxT("menuHighlighting"),       _("&Syntax Highlighting Settings..."),              wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Change syntax highlighting settings")},
#ifdef __WXMSW__
    { 0,               1, menuFileAssociation,    wxT("menuFileAssociation"),    _("&File Type Associations..."),                    wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Change file type associations")},
#endif
    { 0,               1, menuPurgeHistories,     wxT("menuPurgeHistories"),     _("&Purge Histories..."),                           wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Change file type associations")},
    { 0,               1, 0,                      0,                             0,                                                  0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               1, menuByteOrderMark,      wxT("menuByteOrderMark"),      _("Has Unicode BOM (Byte-Order Mark)"),             0,             wxITEM_NORMAL,    -1, &g_Menu_Tools_BOM,                0},
    { 0,               2, menuToggleBOM,          wxT("menuToggleBOM"),          _("Add/Remove BOM"),                                wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Add/Remove Unicode BOM")},
    { 0,               1, 0,                      0,                             0,                                                  0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               1, menuNewLineChar,        wxT("menuNewLineChar"),        _("NewLine Char (File Format): "),                  0,             wxITEM_NORMAL,    -1, &g_Menu_Tools_NewLineChar,        0},
    { 0,               2, menuConvertToDOS,       wxT("menuConvertToDOS"),       _("Convert To CRLF/0D0A (Windows/&DOS)"),           wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Convert the file format to Windows/DOS format")},
    { 0,               2, menuConvertToMAC,       wxT("menuConvertToMAC"),       _("Convert To CR/0D (Classic &Mac OS)"),            wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Convert the file format to classic Mac OS format")},
    { 0,               2, menuConvertToUNIX,      wxT("menuConvertToUNIX"),      _("Convert To LF/0A (&UNIX/macOS)"),                wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Convert the file format to UNIX/macOS format")},
    { 0,               1, menuInsertNewLineChar,  wxT("menuInsertNewLineChar"),  _("Press Enter to Insert NewLine Char: "),          0,             wxITEM_NORMAL,    -1, &g_Menu_Tools_InsertNewLineChar,  0},
    { 0,               2, menuInsertDOS,          wxT("menuInsertDOS"),          _("Insert CRLF/0D0A (Windows/&DOS)"),               wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Insert CR&LF chars when pressing Enter key")},
    { 0,               2, menuInsertMAC,          wxT("menuInsertMAC"),          _("Insert CR/0D (Classic &Mac OS)"),                wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Insert CR char when pressing Enter key")},
    { 0,               2, menuInsertUNIX,         wxT("menuInsertUNIX"),         _("Insert LF/0A (&UNIX/macOS)"),                    wxT(""),       wxITEM_NORMAL,    -1, 0,                                _("Insert LF char when pressing Enter key")},
    { 0,               1, 0,                      0,                             0,                                                  0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               1, menuConvertEncoding,    wxT("menuConvertEncoding"),    _("Convert File &Encoding..."),                     0,             wxITEM_NORMAL,    -1, 0,                                _("Convert to the specified encoding")},
    { 0,               1, 0,                      0,                             0,                                                  0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               1, menuConvertChineseChar,     wxT("menuConvertChineseChar"),     _("Convert &Chinese Char"),                                 0,             wxITEM_NORMAL,    -1, &g_Menu_Tools_ConvertChineseChar, 0},
    { 0,               2, menuSimp2TradChinese,       wxT("menuSimp2TradChinese"),       _("File: Simplified Chinese to &Traditional Chinese"),      0,             wxITEM_NORMAL,    -1, 0,                                _("Convert simplified Chinese chars to traditional Chinese chars in the file")},
    { 0,               2, menuTrad2SimpChinese,       wxT("menuTrad2SimpChinese"),       _("File: Traditional Chinese to &Simplified Chinese"),      0,             wxITEM_NORMAL,    -1, 0,                                _("Convert traditional Chinese chars to simplified Chinese chars in the file")},
    { 0,               2, menuKanji2TradChinese,      wxT("menuKanji2TradChinese"),      _("File: Japanese Kanji to Tr&aditional Chinese"),          0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Japanese Kanji to traditional Chinese chars in the file")},
    { 0,               2, menuKanji2SimpChinese,      wxT("menuKanji2SimpChinese"),      _("File: Japanese Kanji to Si&mplified Chinese"),           0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Japanese Kanji to simplified Chinese chars in the file")},
    { 0,               2, menuChinese2Kanji,          wxT("menuChinese2Kanji"),          _("File: Chinese to &Japanese Kanji"),                      0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Chinese chars to Japanese Kanji in the file")},
    { 0,               2, 0,                          0,                                 0,                                                          0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               2, menuSimp2TradClipboard,     wxT("menuSimp2TradClipboard"),     _("Clipboard: Simplified Chinese to T&raditional Chinese"), 0,             wxITEM_NORMAL,    -1, 0,                                _("Convert simplified Chinese chars to traditional Chinese chars in the clipboard")},
    { 0,               2, menuTrad2SimpClipboard,     wxT("menuTrad2SimpClipboard"),     _("Clipboard: Traditional Chinese to S&implified Chinese"), 0,             wxITEM_NORMAL,    -1, 0,                                _("Convert traditional Chinese chars to simplified Chinese chars in the clipboard")},
    { 0,               2, menuKanji2TradClipboard,    wxT("menuKanji2TradClipboard"),    _("Clipboard: Japanese Kanji to Tra&ditional Chinese"),     0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Japanese Kanji to traditional Chinese chars in the clipboard")},
    { 0,               2, menuKanji2SimpClipboard,    wxT("menuKanji2SimpClipboard"),    _("Clipboard: Japanese Kanji to Sim&plified Chinese"),      0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Japanese Kanji to simplified Chinese chars in the clipboard")},
    { 0,               2, menuChinese2KanjiClipboard, wxT("menuChinese2KanjiClipboard"), _("Clipboard: Chinese to Japanese &Kanji"),                 0,             wxITEM_NORMAL,    -1, 0,                                _("Convert Chinese chars to Japanese Kanji in the clipboard")},
    { 0,               1, 0,                      0,                             0,                                                  0,             wxITEM_SEPARATOR, -1, 0,                                0},
    { 0,               1, menuWordCount,          wxT("menuWordCount"),          _("&Word Count"),                                   0,             wxITEM_NORMAL,    -1, 0,                                _("Count the words and chars of the file or selection")},

    // Window
    { 0, 0, 0, 0, _("&Window"), 0, wxITEM_NORMAL, 0, &g_Menu_Window, 0},
    { ecToggleWindow, 1, menuToggleWindow,   wxT("menuToggleWindow"),   _("&Toggle Window"),   wxT("Ctrl-TAB"), wxITEM_NORMAL, -1, 0, _("Switch to the previous active window")},
    { 0,              1, menuPreviousWindow, wxT("menuPreviousWindow"), _("&Previous Window"), wxT("F5"),       wxITEM_NORMAL, -1, 0, _("Activate the previous window")},
    { 0,              1, menuNextWindow,     wxT("menuNextWindow"),     _("&Next Window"),     wxT("F6"),       wxITEM_NORMAL, -1, 0, _("Activate the next window")},

    // Help
    { 0, 0, 0, 0, _("&Help"), 0, wxITEM_NORMAL, 0, &g_Menu_Help, 0},
    { 0, 1, menuCheckUpdates, wxT("menuCheckUpdates"), _("&Check for updates..."), wxT(""),       wxITEM_NORMAL, -1,                    0, _("Check for new versions of wxMEdit")},
    { 0, 1, menuAbout,       wxT("menuAbout"),       _("&About wxMEdit..."),          wxT(""),       wxITEM_NORMAL, wxmedit0_24x24_xpm_idx, 0, _("Show about dialog")},
    // end menu

    // begin editor
    { ecLeft,      -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret left one char")},
    { ecRight,     -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret right one char")},
    { ecUp,        -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret up one line")},
    { ecDown,      -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret down one line")},
    { ecBeginLine, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to begin of line")},
    { ecEndLine,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to end of line")},
    { ecBeginDoc,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to begin of document")},
    { ecEndDoc,    -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to end of document")},
    { ecPrevPage,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to previous page")},
    { ecNextPage,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to next page")},
    { ecPrevWord,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to previous word")},
    { ecNextWord,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to next word")},

    { ecSelLeft,       -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret left one char and select")},
    { ecSelRight,      -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret right one char and select")},
    { ecSelUp,         -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret up one line and select")},
    { ecSelDown,       -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Move caret down one line and select")},
    { ecSelBeginLine,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to begin of line and select")},
    { ecSelEndLine,    -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to end of line and select")},
    { ecSelBeginDoc,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to begin of document and select")},
    { ecSelEndDoc,     -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to end of document and select")},
    { ecSelPrevPage,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to previous page and select")},
    { ecSelNextPage,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to next page and select")},
    { ecSelPrevWord,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to previous word and select")},
    { ecSelNextWord,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to next word and select")},
    { ecSelLeftBrace,  -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to left brace and select")},
    { ecSelRightBrace, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Go to right brace and select")},

    { ecScrollLineUp,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text up one line")},
    { ecScrollLineDown, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text down one line")},
    { ecScrollPageUp,   -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text up one page")},
    { ecScrollPageDown, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text down one page")},
    { ecScrollLeft,     -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text left")},
    { ecScrollRight,    -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Scroll text right")},

    { ecReturn,         -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Insert one NewLine char")},
    { ecReturnNoIndent, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Insert one NewLine char without indentation")},
    { ecTab,            -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Insert one Tab char or indent lines")},
    { ecBackSpace,      -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Delete one char to the left of the caret")},

    { ecDelPrevWord,    -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Delete a word left from the caret")},
    { ecDelNextWord,    -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Delete a word right from the caret")},

    { ecToggleInsertMode, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, _("Toggle Insert/Overwrite Mode")},

    // end editor
    { -1, -1, 0, 0, 0, 0, wxITEM_NORMAL , -1, 0, 0},
};

// restore the definition of _(s)
#undef _
#define _(s)    wxGetTranslation(_T(s))
//==========================================================

void LoadDefaultSettings(wxConfigBase *m_Config)
{
    m_Config->SetPath(wxT("/wxMEdit"));

    wxString behav;
    m_Config->Read(wxT("BehaviorCopyingInHexArea"), &behav);
    wxm::HexAreaClipboardCopyProxy::Instance().SelectCopierByConfig(behav);

    wxString cond;
    m_Config->Read(wxT("ConditionPastingAsHexInHexArea"), &cond);
    wxm::HexAreaClipboardPasteProxy::Instance().SelectConditionByConfig(cond);

    m_Config->Read(wxT("LastPathClosingFiles"), &wxm::g_lastpath_closingfiles);

    extern bool g_regex_dot_match_newline;
    m_Config->Read(wxT("RegexDotMatchNewline"), &g_regex_dot_match_newline, false);

    long templong, x,y;
    bool tempbool;
    wxString tempstr;

    long orien;
    m_Config->Read(wxT("PageOrientation"), &orien, wxPORTRAIT);
	g_PageSetupData->GetPrintData().SetOrientation((wxPrintOrientation)orien);

    wxSize psize=g_PageSetupData->GetPaperSize();
    m_Config->Read(wxT("PagePaperSizeW"), &psize.x);
    m_Config->Read(wxT("PagePaperSizeH"), &psize.y);
    if(orien!=wxPORTRAIT)
    {
        long tmp=psize.x;
        psize.x=psize.y;
        psize.y=tmp;
    }
    g_PageSetupData->SetPaperSize(psize);

    m_Config->Read(wxT("PageLeftMargin"), &x, 20);
    m_Config->Read(wxT("PageTopMargin"), &y, 30);
    g_PageSetupData->SetMarginTopLeft(wxPoint(x,y));
    m_Config->Read(wxT("PageRightMargin"), &x, 20);
    m_Config->Read(wxT("PageBottomMargin"), &y, 30);
    g_PageSetupData->SetMarginBottomRight(wxPoint(x,y));


    m_Config->Read(wxT("PrintSyntax"), &tempbool, false);
    m_Config->Read(wxT("PrintLineNumber"), &tempbool, true);
	m_Config->Read(wxT("PrintBookmark"), &tempbool, true);
	m_Config->Read(wxT("PrintEndOfLine"), &tempbool, false);
    m_Config->Read(wxT("PrintTabChar"), &tempbool, false);
    m_Config->Read(wxT("PrintSpaceChar"), &tempbool, false);

    m_Config->Read(wxT("PrintOffsetHeader"), &templong, 2);

    m_Config->Read(wxT("PrintPageHeader"), &tempbool, true);
    m_Config->Read(wxT("PageHeaderLeft"), &tempstr, wxT("%f"));
    m_Config->Read(wxT("PageHeaderCenter"), &tempstr, wxEmptyString);
    m_Config->Read(wxT("PageHeaderRight"), &tempstr, wxT("%n/%s"));

    m_Config->Read(wxT("PrintPageFooter"), &tempbool, true);
    m_Config->Read(wxT("PageFooterLeft"), &tempstr, wxEmptyString);
    m_Config->Read(wxT("PageFooterCenter"), &tempstr, wxEmptyString);
    m_Config->Read(wxT("PageFooterRight"), &tempstr, wxT("%d %t"));

    m_Config->SetPath(wxT("/FileCaretPos"));
    g_FileCaretPosManager.Load(m_Config);
}


MadEditFrame::MadEditFrame( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxFrame( parent, id, title, position, size, style)
{
#ifndef __WXMSW__
    wxConvFileName=&MadConvFileNameObj;
#endif

    this->SetWindowStyleFlag(this->GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);

    m_NewFileCount=0;
    m_Config=wxConfigBase::Get(false);

    xm::EncodingManager::Instance().InitEncodings();

    MadSyntax::SetAttributeFilePath(wxm::AppPath::Instance().HomeDir() + wxT("syntax/"));

#if defined(__WXMSW__)
    MadSyntax::AddSyntaxFilesPath(wxm::AppPath::Instance().AppDir() + wxT("syntax/"));
#elif defined(__WXGTK__) // linux
    MadSyntax::AddSyntaxFilesPath(wxm::AppPath::Instance().AppDir() + wxT("syntax/"));
    MadSyntax::AddSyntaxFilesPath(wxm::AppPath::Instance().HomeDir() + wxT("syntax/"));
    #if defined (DATA_DIR)
    MadSyntax::AddSyntaxFilesPath(wxT(DATA_DIR"/wxmedit/syntax/"));
    #else
    MadSyntax::AddSyntaxFilesPath(wxT("/usr/share/wxmedit/syntax/"));
    #endif
#else // other platform
    MadSyntax::AddSyntaxFilesPath(wxm::AppPath::Instance().AppDir() + wxT("syntax/"));
#endif

    CreateGUIControls();

    //g_PrintData = new wxPrintData;
    g_PageSetupData = new wxPageSetupDialogData;

    LoadDefaultSettings(m_Config);

    SetDropTarget(new DnDFile());

    m_PageClosing=false;
    g_MainFrame=this;
}

MadEditFrame::~MadEditFrame()
{}

void MadEditFrame::EncodingGroupMenuAppend(ssize_t idx, const wxString& text, size_t rsv_cnt)
{
    static int i = 0;

    int itemid = menuEncoding1 + int(idx);
    xm::EncodingManager& encmgr = xm::EncodingManager::Instance();
    std::vector<xm::EncodingGroupID> vec = encmgr.GetEncodingGroups(idx);
    BOOST_FOREACH(xm::EncodingGroupID gid, vec)
    {
        EncGrps::iterator it = m_encgrps.find(gid);
        if (it == m_encgrps.end())
        {
            wxMenu* menu = new wxMenu();
            it = m_encgrps.insert(std::make_pair(gid, menu)).first;
            size_t pos = g_Menu_View_Encoding->GetMenuItemCount() - rsv_cnt - 1;
            g_Menu_View_Encoding->Insert(pos, menuEncodingGroup1 + i + (int)rsv_cnt, encmgr.EncodingGroupToName(gid).c_str(), menu);
        }

        ++i;
        it->second->Append(itemid, text);
    }
    g_Menu_View_AllEncodings->Append(itemid, text);
}

size_t MadEditFrame::ReserveEncodingGrupMenus()
{
    xm::EncodingManager& encmgr = xm::EncodingManager::Instance();

    std::vector<xm::EncodingGroupID> reserve_grps =
        boost::assign::list_of(xm::ENCG_ISO8859)(xm::ENCG_WINDOWS)(xm::ENCG_OEM)(xm::ENCG_DEFAULT);

    size_t i = 0;
    BOOST_FOREACH(xm::EncodingGroupID gid, reserve_grps)
    {
        wxMenu* menu = new wxMenu();
        g_Menu_View_Encoding->Insert(i, menuEncodingGroup1 + (int)i, encmgr.EncodingGroupToName(gid).c_str(), menu);
        m_encgrps[gid] = menu;
        ++i;
    }

    return reserve_grps.size();
}

void MadEditFrame::InitEncodingMenus()
{
    xm::EncodingManager& encmgr = xm::EncodingManager::Instance();

    size_t rsv_cnt = ReserveEncodingGrupMenus();

    size_t cnt=xm::EncodingManager::Instance().GetEncodingsCount();
    for(size_t i=0; i<cnt; ++i)
    {
        wxString enc=wxString(wxT('['))+ encmgr.GetEncodingName(i).c_str() + wxT("] ");
        wxString des=wxGetTranslation(encmgr.GetEncodingDescription(i).c_str());

        EncodingGroupMenuAppend(i, enc+des, rsv_cnt);
    }
}

#ifdef __WXMSW__
 bool g_bHasMenuIcon = (wxGetOsVersion() != wxOS_WINDOWS_9X); // fixed win98 will crash if menuitem has icon
#endif

void CloneMenuItem(wxMenu* menu, const wxMenu* srcmenu, int itemid)
{
    if (itemid == 0)
    {
        menu->AppendSeparator();
        return;
    }

    wxMenuItem* srcitem = srcmenu->FindItem(itemid);
	wxString txt = srcitem->GetItemLabel();
#ifdef __WXGTK__
    txt.Replace(wxT("&"), wxT("&&"));
    txt.Replace(wxT("_"), wxT("&"));
#endif
    wxMenuItem* item = new wxMenuItem(menu, itemid, txt, srcitem->GetHelp());

#ifdef __WXMSW__
    if (g_bHasMenuIcon)
#endif
    {
        const wxBitmap& bit = srcitem->GetBitmap();
        if (!bit.IsNull())
            item->SetBitmap(bit);
    }
    menu->Append(item);
}

void MadEditFrame::CreateGUIControls()
{
    m_wxmstatusbar.Init(this, ID_WXSTATUSBAR1);

    WxToolBar1 = new wxToolBar(this, ID_WXTOOLBAR1, wxPoint(0, 0), wxSize(392, 38));
    WxToolBar1->SetToolBitmapSize(wxSize(24, 24));

    m_Notebook = new wxMadAuiNotebook(this, ID_NOTEBOOK, wxPoint(0, 38),wxSize(392, 320), wxWANTS_CHARS |wxAUI_NB_TOP|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_WINDOWLIST_BUTTON|wxAUI_NB_CLOSE_ON_ACTIVE_TAB);
    m_Notebook->wxControl::SetWindowStyleFlag(m_Notebook->wxControl::GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
    m_Notebook->SetDropTarget(new DnDFile());
    m_Notebook->SetArtProvider(new wxAuiSimpleTabArt);

    WxMenuBar1 = new wxMenuBar();
    this->SetMenuBar(WxMenuBar1);

    WxToolBar1->Realize();
    this->SetToolBar(WxToolBar1);
    m_wxmstatusbar.Attach();
    this->SetTitle(wxT("wxMEdit "));

#if !defined(__WXMSW__) //&& !defined(__WXPM__)
    SetIcon(wxIcon(wxmedit_xpm));
#else // __MSW__
    SetIcon(wxIcon(wxT("appicon")));
#endif

    // load MenuText
    {
        CommandData *cd = &CommandTable[0];
        do
        {
            if(cd->menuid_name!=0)
            {
                MadKeyBindings::AddMenuTextCommand(cd->menu_id, wxString(cd->menuid_name), cd->command);
            }
            ++cd;
        }while(cd->menu_level >= 0);
    }

    // load keybindings
    if(m_Config->Exists(wxT("/KeyBindings")))
    {
        m_Config->SetPath(wxT("/KeyBindings"));
        MadEdit::ms_KeyBindings.LoadFromConfig(m_Config);
        MadEdit::ms_KeyBindings.AddDefaultBindings(false);
    }

    // add keybinding of ecToggleWindow
    wxString str=GetMenuKey(wxT("menuToggleWindow"), wxEmptyString);
    if(!str.IsEmpty())
    {
        if(str[0]==wxT('\t'))
        {
            str=str.Right(str.Len()-1);
        }
        MadEdit::ms_KeyBindings.Add(StringToShortCut(str), ecToggleWindow, true);
    }


    //m_ImageList
    m_ImageList=new wxImageList(24, 24);
    m_ImageList->Add(wxBitmap(null_xpm));
    m_ImageList->Add(wxBitmap(new_xpm));
    m_ImageList->Add(wxBitmap(fileopen_xpm));
    m_ImageList->Add(wxBitmap(filesave_xpm));
    m_ImageList->Add(wxBitmap(saveall_xpm));
    m_ImageList->Add(wxBitmap(fileclose_xpm));
    m_ImageList->Add(wxBitmap(closeall_xpm));
    m_ImageList->Add(wxBitmap(preview_xpm));
    m_ImageList->Add(wxBitmap(print_xpm));
    m_ImageList->Add(wxBitmap(quit_xpm));
    m_ImageList->Add(wxBitmap(undo_xpm));
    m_ImageList->Add(wxBitmap(redo_xpm));
    m_ImageList->Add(wxBitmap(cut_xpm));
    m_ImageList->Add(wxBitmap(copy_xpm));
    m_ImageList->Add(wxBitmap(paste_xpm));
    m_ImageList->Add(wxBitmap(delete_xpm));
    m_ImageList->Add(wxBitmap(olist_xpm));
    m_ImageList->Add(wxBitmap(indent_xpm));
    m_ImageList->Add(wxBitmap(unindent_xpm));
    m_ImageList->Add(wxBitmap(comment_xpm));
    m_ImageList->Add(wxBitmap(uncomment_xpm));
    m_ImageList->Add(wxBitmap(find_xpm));
    m_ImageList->Add(wxBitmap(findnext_xpm));
    m_ImageList->Add(wxBitmap(findprev_xpm));
    m_ImageList->Add(wxBitmap(replace_xpm));
    m_ImageList->Add(wxBitmap(bmktoggle_xpm));
    m_ImageList->Add(wxBitmap(bmknext_xpm));
    m_ImageList->Add(wxBitmap(bmkprev_xpm));
    m_ImageList->Add(wxBitmap(bmkclearall_xpm));
    m_ImageList->Add(wxBitmap(fontname_xpm));
    m_ImageList->Add(wxBitmap(fontsize_xpm));
    m_ImageList->Add(wxBitmap(font_xpm));
    m_ImageList->Add(wxBitmap(nowrap_xpm));
    m_ImageList->Add(wxBitmap(wrapbywin_xpm));
    m_ImageList->Add(wxBitmap(wrapbycol_xpm));
    m_ImageList->Add(wxBitmap(textmode_xpm));
    m_ImageList->Add(wxBitmap(columnmode_xpm));
    m_ImageList->Add(wxBitmap(hexmode_xpm));
    m_ImageList->Add(wxBitmap(options_xpm));
    m_ImageList->Add(wxBitmap(wxmedit0_24x24_xpm));


    // add menuitems
    g_PopMenu_Tab = new wxMenu(0L);
    g_Menu_File = new wxMenu(0L);
    g_Menu_Edit = new wxMenu(0L);
    g_Menu_Search = new wxMenu(0L);
    g_Menu_View = new wxMenu(0L);
    g_Menu_Tools = new wxMenu(0L);
    g_Menu_Window = new wxMenu(0L);
    g_Menu_Help = new wxMenu(0L);
    g_Menu_File_CloseMore = new wxMenu(0L);
    g_Menu_File_CopyPath = new wxMenu(0L);
    g_Menu_File_RecentFiles = new wxMenu(0L);
    g_Menu_Edit_Column = new wxMenu(0L);
    g_Menu_Edit_Sort = new wxMenu(0L);
    g_Menu_Edit_Advanced = new wxMenu(0L);
    g_Menu_View_Encoding = new wxMenu(0L);
    g_Menu_View_AllEncodings = new wxMenu(0L);
    g_Menu_View_Syntax = new wxMenu(0L);
    g_Menu_View_FontName = new wxMenu(0L);
    g_Menu_View_FontSize = new wxMenu(0L);
    g_Menu_View_LineSpacing = new wxMenu(0L);
    g_Menu_View_TabColumn = new wxMenu(0L);
    g_Menu_Tools_BOM = new wxMenu(0L);
    g_Menu_Tools_NewLineChar = new wxMenu(0L);
    g_Menu_Tools_InsertNewLineChar = new wxMenu(0L);
    g_Menu_Tools_ConvertChineseChar = new wxMenu(0L);
    g_Menu_View_Font0 = new wxMenu(0L);
    g_Menu_View_Font1 = new wxMenu(0L);
    g_Menu_View_Font2 = new wxMenu(0L);
    g_Menu_View_Font3 = new wxMenu(0L);
    g_Menu_View_Font4 = new wxMenu(0L);
    g_Menu_View_Font5 = new wxMenu(0L);
    g_Menu_View_Font6 = new wxMenu(0L);


    list<wxMenu*> menu_stack;
    CommandData *cd = &CommandTable[0];
    do
    {
        if(cd->menu_level==0)
        {
            menu_stack.clear();
            menu_stack.push_back(*cd->menu_ptr);
            WxMenuBar1->Append(*cd->menu_ptr, wxGetTranslation(cd->text));
            ++cd;
            continue;
        }
        else if(cd->menu_level < int(menu_stack.size()))
        {
            do
            {
                menu_stack.pop_back();
            }
            while(cd->menu_level < int(menu_stack.size()));
        }

        if(cd->menu_ptr != 0)
        {
            menu_stack.back()->Append(cd->menu_id, wxGetTranslation(cd->text), *cd->menu_ptr);
            menu_stack.push_back(*cd->menu_ptr);
        }
        else if(cd->kind == wxITEM_SEPARATOR)
        {
            menu_stack.back()->AppendSeparator();
        }
        else
        {
            wxMenuItem *mit=new wxMenuItem(menu_stack.back(), cd->menu_id, wxString(wxGetTranslation(cd->text)) + GetMenuKey(cd->menuid_name,cd->key), wxGetTranslation(cd->hint), cd->kind);

#ifdef __WXMSW__
            if (g_bHasMenuIcon)
#endif
            if(cd->image_idx >= 0 && cd->kind==wxITEM_NORMAL)
            {
                mit->SetBitmap(m_ImageList->GetBitmap(cd->image_idx));
            }

            menu_stack.back()->Append(mit);
        }

        ++cd;
    }
    while(cd->menu_level>=0);

    // right click menu of tab
    int FileMenuInTabIDs[] = { menuSave, menuReload, 0, menuClose, menuCloseAll, menuCloseAllButThis,
                               menuCloseAllToTheLeft, menuCloseAllToTheRight, 0, menuCopyFullPath,
                               menuCopyFilename, menuCopyFileDir, 0, menuPrintPreview, menuPrint };

    BOOST_FOREACH(int itemid, FileMenuInTabIDs)
        CloneMenuItem(g_PopMenu_Tab, g_Menu_File, itemid);

    // set FindNext/FindPrev keys for search/replace dialog
    g_AccelFindNext.Set(0, 0, 0, 0);
    g_AccelFindPrev.Set(0, 0, 0, 0);

    wxAcceleratorEntry *ent=GetAccelFromString(g_Menu_Search->GetLabel(menuFindNext));
    if(ent!=nullptr)
    {
        g_AccelFindNext=*ent;
        delete ent;
    }
    ent=GetAccelFromString(g_Menu_Search->GetLabel(menuFindPrevious));
    if(ent!=nullptr)
    {
        g_AccelFindPrev=*ent;
        delete ent;
    }

    InitEncodingMenus();

    {
        size_t cnt=MadSyntax::GetSyntaxCount();
        for(size_t i=0;i<cnt;i++)
        {
            g_Menu_View_Syntax->Append(menuSyntax1+int(i), MadSyntax::GetSyntaxTitle(i));
        }
    }

    {
        MadFontEnumerator fontenumerator;
        fontenumerator.EnumerateFacenames(); // get all fontnames

        size_t count=g_FontNames.Count();
        for(size_t i=0;i<count; i++)
        {
            wxString &name=g_FontNames[i];
            wxChar firstchar=name[0];
            if(firstchar<='Z' && firstchar>='A')
            {
                firstchar |= 0x20; // to lower case
            }

            wxMenu *menu=g_Menu_View_Font6;
            if(firstchar==wxT('@'))
            {
                menu=g_Menu_View_Font0;
            }
            else if(firstchar<='z' && firstchar>='a')
            {
                if(firstchar<='e') menu=g_Menu_View_Font1;
                else if(firstchar<='j') menu=g_Menu_View_Font2;
                else if(firstchar<='o') menu=g_Menu_View_Font3;
                else if(firstchar<='t') menu=g_Menu_View_Font4;
                else if(firstchar<='z') menu=g_Menu_View_Font5;
            }

            menu->Append(menuFontName1 + int(i), name);
        }
    }

    if(!m_Config->Exists(wxT("/KeyBindings")))
    {
        MadEdit::ms_KeyBindings.AddDefaultBindings(true);
    }
    ResetAcceleratorTable();


    /***/
    m_RecentFiles=new wxFileHistory();
    m_RecentFiles->UseMenu(g_Menu_File_RecentFiles);
    m_Config->SetPath(wxT("/RecentFiles"));
    m_RecentFiles->Load(*m_Config);

    m_RecentEncodings = new wxm::wxCaseInsensitiveRecentList(9, menuRecentEncoding1);
    m_RecentEncodings->UseMenu(g_Menu_View_Encoding);
    m_Config->SetPath(wxT("/RecentEncodings"));
    m_RecentEncodings->Load(*m_Config);

    m_RecentFonts = new wxm::wxFilePathRecentList(9, menuRecentFont1);
    m_RecentFonts->UseMenu(g_Menu_View_FontName);
    m_Config->SetPath(wxT("/RecentFonts"));
    m_RecentFonts->Load(*m_Config);

    m_wxmstatusbar.Resize();

    /*
    // load plugins
    wxPluginLibrary *lib = wxPluginManager::LoadLibrary(wxT("./plugin"));
    if(lib)
    {
        wxLogWarning(wxString::Format(wxT("Loaded [ %s ]\n"), wxT("plugin")));
                          //wxString(path + filename).c_str()));

        if(lib->HasSymbol(wxT("PluginProc")))
        {
            //typedef const wchar_t* (*GetNameProc)();
            typedef int (*PluginProc_Proc)(int PluginID, int nMsg, void* pParam);
            PluginProc_Proc PluginProc=(PluginProc_Proc)lib->GetSymbol(wxT("PluginProc"));

            if(PluginProc)
            {
                wchar_t *name;

                PluginProc(0, PL_GET_NAME, (void*)&name);
                wxLogWarning(wxString::Format(wxT("GetName: [%s]\n"), name));

                char **xpm;
                PluginProc(0, PL_GET_XPM, (void*)&xpm);
                m_ImageList->Add(wxBitmap(xpm));

            }
        }
    }
    */


    //WxToolBar1
    //WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuNew,      _T("New"),      m_ImageList->GetBitmap(new_xpm_idx),       wxNullBitmap, wxITEM_NORMAL, _("New File") );
    WxToolBar1->AddTool(menuOpen,     _T("Open"),     m_ImageList->GetBitmap(fileopen_xpm_idx),  wxNullBitmap, wxITEM_NORMAL, _("Open File") );
    WxToolBar1->AddTool(menuSave,     _T("Save"),     m_ImageList->GetBitmap(filesave_xpm_idx),  wxNullBitmap, wxITEM_NORMAL, _("Save File") );
    WxToolBar1->AddTool(menuSaveAll,  _T("SaveAll"),  m_ImageList->GetBitmap(saveall_xpm_idx),   wxNullBitmap, wxITEM_NORMAL, _("Save All Files") );
    WxToolBar1->AddTool(menuClose,    _T("Close"),    m_ImageList->GetBitmap(fileclose_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Close File") );
    WxToolBar1->AddTool(menuCloseAll, _T("CloseAll"), m_ImageList->GetBitmap(closeall_xpm_idx),  wxNullBitmap, wxITEM_NORMAL, _("Close All Files") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuUndo, _T("Undo"), m_ImageList->GetBitmap(undo_xpm_idx),wxNullBitmap, wxITEM_NORMAL, _("Undo") );
    WxToolBar1->AddTool(menuRedo, _T("Redo"), m_ImageList->GetBitmap(redo_xpm_idx),wxNullBitmap, wxITEM_NORMAL, _("Redo") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuCut,   _T("Cut"),   m_ImageList->GetBitmap(cut_xpm_idx),   wxNullBitmap, wxITEM_NORMAL, _("Cut") );
    WxToolBar1->AddTool(menuCopy,  _T("Copy"),  m_ImageList->GetBitmap(copy_xpm_idx),  wxNullBitmap, wxITEM_NORMAL, _("Copy") );
    WxToolBar1->AddTool(menuPaste, _T("Paste"), m_ImageList->GetBitmap(paste_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Paste") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuInsertEnumeration, _T("InsertEnum"), m_ImageList->GetBitmap(olist_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Insert Ordered Sequence"));

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuIncreaseIndent, _T("IncIndent"), m_ImageList->GetBitmap(indent_xpm_idx),   wxNullBitmap, wxITEM_NORMAL, _("Increase Indent") );
    WxToolBar1->AddTool(menuDecreaseIndent, _T("DecIndent"), m_ImageList->GetBitmap(unindent_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Decrease Indent") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuComment,   _T("Comment"),   m_ImageList->GetBitmap(comment_xpm_idx),   wxNullBitmap, wxITEM_NORMAL, _("Comment") );
    WxToolBar1->AddTool(menuUncomment, _T("Uncomment"), m_ImageList->GetBitmap(uncomment_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Uncomment") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuFind,         _T("Find"),     m_ImageList->GetBitmap(find_xpm_idx),     wxNullBitmap, wxITEM_NORMAL, _("Find") );
    WxToolBar1->AddTool(menuFindNext,     _T("FindNext"), m_ImageList->GetBitmap(findnext_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Find Next") );
    WxToolBar1->AddTool(menuFindPrevious, _T("FindPrev"), m_ImageList->GetBitmap(findprev_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Find Previous") );
    WxToolBar1->AddTool(menuReplace,      _T("Replace"),  m_ImageList->GetBitmap(replace_xpm_idx),  wxNullBitmap, wxITEM_NORMAL, _("Replace") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuToggleBookmark,       _T("ToggleBookmark"),       m_ImageList->GetBitmap(bmktoggle_xpm_idx),   wxNullBitmap, wxITEM_NORMAL, _("Toggle Bookmark") );
    WxToolBar1->AddTool(menuGotoNextBookmark,     _T("GotoNextBookmark"),     m_ImageList->GetBitmap(bmknext_xpm_idx),     wxNullBitmap, wxITEM_NORMAL, _("Go To Next Bookmark") );
    WxToolBar1->AddTool(menuGotoPreviousBookmark, _T("GotoPreviousBookmark"), m_ImageList->GetBitmap(bmkprev_xpm_idx),     wxNullBitmap, wxITEM_NORMAL, _("Go To Previous Bookmark") );
    WxToolBar1->AddTool(menuClearAllBookmarks,    _T("ClearAllBookmarks"),    m_ImageList->GetBitmap(bmkclearall_xpm_idx), wxNullBitmap, wxITEM_NORMAL, _("Clear All Bookmarks") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuNoWrap,       _T("NoWrap"),       m_ImageList->GetBitmap(nowrap_xpm_idx),    wxNullBitmap, wxITEM_CHECK, _("No Line Wrap") );
    WxToolBar1->AddTool(menuWrapByWindow, _T("WrapByWindow"), m_ImageList->GetBitmap(wrapbywin_xpm_idx), wxNullBitmap, wxITEM_CHECK, _("Wrap Lines by Window") );
    WxToolBar1->AddTool(menuWrapByColumn, _T("WrapByColumn"), m_ImageList->GetBitmap(wrapbycol_xpm_idx), wxNullBitmap, wxITEM_CHECK, _("Wrap Lines by Max Columns") );

    WxToolBar1->AddSeparator();
    WxToolBar1->AddTool(menuTextMode,   _T("TextMode"),   m_ImageList->GetBitmap(textmode_xpm_idx),   wxNullBitmap, wxITEM_CHECK, _("Text Mode") );
    WxToolBar1->AddTool(menuColumnMode, _T("ColumnMode"), m_ImageList->GetBitmap(columnmode_xpm_idx), wxNullBitmap, wxITEM_CHECK, _("Column Mode") );
    WxToolBar1->AddTool(menuHexMode,    _T("HexMode"),    m_ImageList->GetBitmap(hexmode_xpm_idx),    wxNullBitmap, wxITEM_CHECK, _("Hex Mode") );

    WxToolBar1->Realize();

    //WxToolBar1->EnableTool(wxID_NEW, false);
    //WxToolBar1->ToggleTool(wxID_NEW, true);


    // information window
    int infoW = 300, infoH = 130;
    m_Config->Read(wxT("/wxMEdit/InfoWindowWidth"), &infoW);
    m_Config->Read(wxT("/wxMEdit/InfoWindowHeight"), &infoH);
    wxSize nbsize(infoW, infoH);
    m_InfoNotebook = new wxAuiNotebook(this, ID_OUTPUTNOTEBOOK, wxDefaultPosition, nbsize, wxAUI_NB_TOP|wxAUI_NB_SCROLL_BUTTONS);

    m_FindInFilesResults = new wxTreeCtrl(m_InfoNotebook, ID_FINDINFILESRESULTS, wxDefaultPosition, wxSize(infoW,4), wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT);
    m_FindInFilesResults->AddRoot(wxT("Root"));
    m_FindInFilesResults->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MadEditFrame::OnFindInFilesResultsDClick));

    m_InfoNotebook->AddPage(m_FindInFilesResults, _("Find/Replace in Files Results"));
    m_InfoNotebook->Connect(wxEVT_SIZE, wxSizeEventHandler(MadEditFrame::OnInfoNotebookSize));

    // wxAUI
    m_AuiManager.SetManagedWindow(this);
    m_AuiManager.SetFlags(m_AuiManager.GetFlags() | wxAUI_MGR_ALLOW_ACTIVE_PANE);
    m_AuiManager.SetDockSizeConstraint(0.75, 0.75);
    m_AuiManager.AddPane(m_Notebook, wxCENTER);
    m_AuiManager.AddPane(m_InfoNotebook, wxBOTTOM, _("Information Window"));
    m_AuiManager.GetPane(m_InfoNotebook).Show(false).FloatingSize(nbsize);
    m_AuiManager.Update();

    // fixed for using wxAUI
    m_wxmstatusbar.ConnectSizeEvent(wxSizeEventHandler(MadEditFrame::OnSizeStatusBar));
}

void MadEditFrame::MadEditFrameClose(wxCloseEvent& event)
{
    // --> Don't use Close with a Frame,
    // use Destroy instead.

    if(event.CanVeto())
    {
        if(QueryCloseAllFiles()==false)
        {
            event.Veto(true);
            return;
        }
    }


    // save ReloadFilesList
    wxm::FileList filelist;
    size_t count = m_Notebook->GetPageCount();
    bool bb=true;
    m_Config->Read(wxT("ReloadFiles"), &bb);
    if(bb && count>0)
        m_Notebook->GetFilesListForReload(filelist);

    m_Config->Write(wxT("/wxMEdit/ReloadFilesList"), filelist.String());

    m_Config->SetPath(wxT("/FileCaretPos"));
    g_FileCaretPosManager.Save(m_Config);

#if defined(__WXMSW__) || defined(__WXGTK__)
    m_Config->Write(wxT("/wxMEdit/WindowMaximize"), IsMaximized());

    wxRect rect = GetNormalRect();
    m_Config->Write(wxT("/wxMEdit/WindowLeft"),   rect.x );
    m_Config->Write(wxT("/wxMEdit/WindowTop"),    rect.y );
    m_Config->Write(wxT("/wxMEdit/WindowWidth"),  rect.width);
    m_Config->Write(wxT("/wxMEdit/WindowHeight"), rect.height);
#else
    int x,y;
    GetPosition(&x,&y);
    m_Config->Write(wxT("/wxMEdit/WindowLeft"), x );
    m_Config->Write(wxT("/wxMEdit/WindowTop"), y );

    GetSize(&x,&y);
    m_Config->Write(wxT("/wxMEdit/WindowWidth"), x );
    m_Config->Write(wxT("/wxMEdit/WindowHeight"), y );
#endif

    m_Config->SetPath(wxT("/RecentFiles"));
    m_RecentFiles->Save(*m_Config);

    m_Config->SetPath(wxT("/RecentEncodings"));
    m_RecentEncodings->Save(*m_Config);

    m_Config->SetPath(wxT("/RecentFonts"));
    m_RecentFonts->Save(*m_Config);

    if(g_SearchReplaceDialog!=nullptr)
    {
        g_SearchReplaceDialog->Show(false);
        m_Config->SetPath(wxT("/RecentFindText"));
        g_SearchReplaceDialog->m_RecentFindText->Save(*m_Config);
        m_Config->SetPath(wxT("/RecentReplaceText"));
        g_SearchReplaceDialog->m_RecentReplaceText->Save(*m_Config);
    }
    if(g_FindInFilesDialog!=nullptr)
    {
        g_FindInFilesDialog->Show(false);
        m_Config->SetPath(wxT("/RecentFindDir"));
        g_FindInFilesDialog->m_RecentFindDir->Save(*m_Config);
        m_Config->SetPath(wxT("/RecentFindFilter"));
        g_FindInFilesDialog->m_RecentFindFilter->Save(*m_Config);
        m_Config->SetPath(wxT("/RecentFindExcludeFilter"));
        g_FindInFilesDialog->m_RecentFindExclude->Save(*m_Config);
    }

    // reset SearchInSelection
    m_Config->Write(wxT("/wxMEdit/SearchInSelection"), false);
    m_Config->Write(wxT("/wxMEdit/SearchFrom"), wxEmptyString);
    m_Config->Write(wxT("/wxMEdit/SearchTo"), wxEmptyString);


    delete m_RecentFiles;
    m_RecentFiles=nullptr;
    delete m_RecentEncodings;
    m_RecentEncodings=nullptr;
    delete m_RecentFonts;
    m_RecentFonts=nullptr;

    delete m_ImageList;

    xm::EncodingManager::Instance().FreeEncodings();
    wxm::AppPath::Instance().DestroyInstance();

    FreeConvertChineseTable();

    //delete g_PrintData;
    delete g_PageSetupData;

    delete g_PopMenu_Tab;
    g_PopMenu_Tab = nullptr;

    extern void DeleteConfig();
    DeleteConfig();

    m_AuiManager.UnInit();

#ifndef __WXMSW__
    // it will crash randomly under linux.
    // so we must call exit() to quit the app.
    exit(0);
#else
    extern HANDLE g_Mutex;
    if(g_Mutex)
    {
        ReleaseMutex(g_Mutex);
    }
    Destroy(); // quit app normally.
#endif
}

/*
 * MadEditFrameKeyDown
 */
void MadEditFrame::MadEditFrameKeyDown(wxKeyEvent& event)
{
    int key=event.GetKeyCode();

    switch(key)
    {
    case WXK_ESCAPE:
        if(g_SearchReplaceDialog && g_SearchReplaceDialog->IsShown())
        {
            g_SearchReplaceDialog->Show(false);
        }
        if(g_FindInFilesDialog && g_FindInFilesDialog->IsShown())
        {
            g_FindInFilesDialog->Show(false);
        }

        break;
    }

    event.Skip();
}

void MadEditFrame::SetPageFocus(int pageId)
{
    int selid=m_Notebook->GetSelection();
    if (pageId == selid || pageId<0 || pageId>int(m_Notebook->GetPageCount()))
        return;

    m_Notebook->SetSelection(pageId);

    wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
    if (sel == g_active_wxmedit)
        return;

    wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
    event.SetSelection(pageId);
    event.SetOldSelection(selid);
    event.SetEventObject(this);
    OnNotebookPageChanged(event);
}

wxm::InFrameWXMEdit* MadEditFrame::GetEditByFileName(const wxString &filename, int &id)
{
    int count=int(m_Notebook->GetPageCount());
    wxString fn;
    for(id=0; id<count; id++)
    {
        wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(id);
        fn = wxmedit->GetFileName();
        if(wxm::FilePathEqual(fn, filename))
        {
            return wxmedit;
        }
    }
    id=-1;
    return nullptr;
}

void MadEditFrame::ResetAcceleratorTable()
{
    vector<wxAcceleratorEntry> entries;
    MadEdit::ms_KeyBindings.BuildAccelEntries(true, entries);
    if(entries.size()>0)
    {
        wxAcceleratorTable accel(int(entries.size()), &entries[0]);
        this->SetAcceleratorTable(accel);
    }
}

#ifdef __WXMSW__
WXLRESULT MadEditFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch(message)
    {
    case WM_COPYDATA:
        {
            COPYDATASTRUCT *pcds=*((COPYDATASTRUCT**)&lParam);
            OnReceiveMessage((wxChar*)pcds->lpData, pcds->cbData);
        }
        return TRUE;
    }

    return wxFrame::MSWWindowProc(message, wParam, lParam);
}
#endif


void MadEditFrame::OnNotebookPageChanging(wxAuiNotebookEvent& event)
{
    g_PrevPageID = event.GetOldSelection();
}

void MadEditFrame::OnNotebookPageChanged(wxAuiNotebookEvent& event)
{
    g_active_wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());

    int now=event.GetSelection();
    int old=event.GetOldSelection();
    if(event.GetEventObject()==this)
    {
        old=event.GetOldSelection();
    }

    //wxLogDebug(wxT("curr :%d,  prev: %d"), now, old);

    if(old!=now && old>=0)
    {
        g_PrevPageID=old;
    }

    //wxLogMessage( wxString()<<int(g_active_wxmedit));
    if (g_active_wxmedit != nullptr)
    {
        g_active_wxmedit->SetFocus();

        wxString title = g_active_wxmedit->GetFileName();
        if(title.IsEmpty())
        {
            title=m_Notebook->GetPageText(event.GetSelection());
        }

        if (g_active_wxmedit->IsModified() && title[title.Len()-1]!=wxT('*'))
            title += wxT('*');

        SetTitle(wxString(wxT("wxMEdit - ["))+ title +wxString(wxT("] ")));

        g_active_wxmedit->OnSelectionAndStatusChanged();

        if(g_CheckModTimeForReload)
        {
            g_active_wxmedit->ReloadByModificationTime();
        }
    }
    else
    {
        SetTitle(wxString(wxT("wxMEdit ")));
    }
}

void MadEditFrame::OnNotebookPageClosing(wxAuiNotebookEvent& event)
{
    if(m_PageClosing)
    {
        return;
    }

    m_PageClosing=true;
    if(QueryCloseFile(event.GetSelection())==false)
    {
        event.Veto();
    }
    else
    {
        if(m_Notebook->GetPageCount()==1) //close last file
        {
            OnNotebookPageClosed(true);
        }
    }
    m_PageClosing=false;
}

void MadEditFrame::OnNotebookPageClosed(bool bZeroPage)
{
    if(bZeroPage || m_Notebook->GetPageCount()==0)
    {
        g_active_wxmedit = nullptr;
        SetTitle(wxString(wxT("wxMEdit ")));
        ResetStatusBar();
    }
    else
    {
        wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
        if (g_active_wxmedit != wxmedit)
        {
            g_active_wxmedit = wxmedit;
            g_active_wxmedit->OnSelectionAndStatusChanged();

            wxString title = g_active_wxmedit->GetFileName();
            if(title.IsEmpty())
            {
                title=m_Notebook->GetPageText( m_Notebook->GetSelection() );
            }

            if(g_active_wxmedit->IsModified() && title[title.Len()-1]!=wxT('*'))
                title += wxT('*');

            SetTitle(wxString(wxT("wxMEdit - ["))+ title +wxString(wxT("] ")));
        }
        g_active_wxmedit->ReloadByModificationTime();
    }
}

void MadEditFrame::OnNotebookTabRightUp(wxAuiNotebookEvent& event)
{
    SetPageFocus(event.GetSelection());
    PopupMenu(g_PopMenu_Tab);
}

void MadEditFrame::OnSizeStatusBar(wxSizeEvent &evt)
{
    evt.Skip();

    g_MainFrame->m_wxmstatusbar.Resize();
}

//void MadEditFrame::OnChar(wxKeyEvent& evt)
//{
    //wxLogDebug(wxT("frame OnChar"));
    //evt.Skip();
//}

void MadEditFrame::OnActivate(wxActivateEvent &evt)
{
    if(evt.GetActive() && g_active_wxmedit)
    {
        g_active_wxmedit->SetFocus();
        g_active_wxmedit->ReloadByModificationTime();
    }
    evt.Skip();
}

void MadEditFrame::OnResultAutoCheckUpdates(wxEvent &evt)
{
    bool notify_all = false;

#ifdef _DEBUG
    notify_all = true;
#endif

    wxm::ConfirmUpdate(notify_all);
    evt.Skip();
}

void MadEditFrame::OnResultManualCheckUpdates(wxEvent &evt)
{
    wxm::ConfirmUpdate();
    evt.Skip();
}

//---------------------------------------------------------------------------

wxString MadEditFrame::GetMenuKey(const wxString &menu, const wxString &defaultkey)
{
    wxString key = MadEdit::ms_KeyBindings.GetKeyByMenuText(menu);
    if(!key.IsEmpty())
    {
        return wxString(wxT("\t"))+key;
    }

    // defaultkey is empty
    if(defaultkey.IsEmpty())
    {
        return wxEmptyString;
    }

    // check the defaultkey is applied to other menuitem or not
    if(MadEdit::ms_KeyBindings.KeyIsAssigned(defaultkey))
    {
        return wxEmptyString;
    }

    // the defaultkey doesn't apply yet, add it
    int menuid=MadKeyBindings::TextToMenuId(menu);
    MadEditShortCut sc=StringToShortCut(defaultkey);
    if(menuid!=0 && sc!=0)
    {
        MadEdit::ms_KeyBindings.Add(sc, true, menuid, true);
    }

    return wxString(wxT("\t"))+defaultkey;
}

void MadEditFrame::OnInfoNotebookSize(wxSizeEvent &evt)
{
    wxAuiPaneInfo &pinfo=g_MainFrame->m_AuiManager.GetPane(g_MainFrame->m_InfoNotebook);
    if(pinfo.IsOk())// && pinfo.IsShown())
    {
        wxSize size;
        if(pinfo.IsDocked())
        {
            size=g_MainFrame->m_InfoNotebook->GetSize();
#if defined(__WXMSW__) && wxMAJOR_VERSION==2
            size.IncBy(g_MainFrame->m_InfoNotebook->GetWindowBorderSize());
#endif
        }
        else
        {
            if(pinfo.frame != nullptr)
            {
                size=pinfo.frame->GetSize();
            }
            else
            {
                size.x=0;
            }
        }

        if(size.x>16 && size.y>16)
        {
            if(pinfo.IsDocked())
            {
                if(pinfo.dock_direction==wxAUI_DOCK_TOP || pinfo.dock_direction==wxAUI_DOCK_BOTTOM)
                {
                    pinfo.floating_size.y = size.y;
                    pinfo.best_size.y = size.y;
                    g_MainFrame->m_Config->Write(wxT("/wxMEdit/InfoWindowHeight"), size.y);
                }
                else
                {
                    pinfo.floating_size.x = size.x;
                    pinfo.best_size.x = size.x;
                    g_MainFrame->m_Config->Write(wxT("/wxMEdit/InfoWindowWidth"), size.x);
                }
            }
            else //IsFloating()
            {
                pinfo.BestSize(size);
                g_MainFrame->m_Config->Write(wxT("/wxMEdit/InfoWindowWidth"), size.x);
                g_MainFrame->m_Config->Write(wxT("/wxMEdit/InfoWindowHeight"), size.y);
            }
        }
    }
    evt.Skip();
}

void MadEditFrame::OnFindInFilesResultsDClick(wxMouseEvent& event)
{
    wxm::InFrameWXMEdit* wxmedit=nullptr;
    int flags;
    wxTreeItemId id = g_MainFrame->m_FindInFilesResults->HitTest(event.GetPosition(), flags);
    if(id.IsOk())
    {
        CaretPosData *cpdata = (CaretPosData*)g_MainFrame->m_FindInFilesResults->GetItemData(id);
        if(cpdata)
        {
            int count = int(g_MainFrame->m_Notebook->GetPageCount());
            if(cpdata->pageid>=0 && cpdata->pageid<count)
            {
                wxString title=g_MainFrame->m_Notebook->GetPageText(cpdata->pageid);
                if(title[title.Len()-1]==wxT('*'))
                    title.Truncate(title.Len()-1);
                if(title==cpdata->filename)
                {
                    g_MainFrame->SetPageFocus(cpdata->pageid);
                    wxmedit = g_active_wxmedit;
                }
            }

            if (wxmedit == nullptr && wxFileExists(cpdata->filename))
            {
                g_MainFrame->OpenFile(cpdata->filename, true);

                if (wxm::FilePathEqual(g_active_wxmedit->GetFileName(), cpdata->filename))
                {
                    wxmedit = g_active_wxmedit;
                }
            }

            if (wxmedit)
            {
                wxmedit->SetCaretPosition(cpdata->epos, cpdata->bpos, cpdata->epos);
                wxmedit->SetFocus();
            }
        }
    }

    if (wxmedit == nullptr) event.Skip();
}

void MadEditFrame::ResetFindInFilesResults()
{
    m_FindInFilesResults->DeleteAllItems();
    m_FindInFilesResults->AddRoot(wxT("Root"));
}

void MadEditFrame::AddItemToFindInFilesResults(const wxString &text, size_t index, const wxString &filename, int pageid, const wxFileOffset &begpos, wxFileOffset &endpos)
{
    static wxTreeItemId fileid;

    if(index==0)
    {
        if(m_FindInFilesResults->GetChildrenCount(m_FindInFilesResults->GetRootItem(), false)==0)
        {
            fileid = m_FindInFilesResults->AppendItem(m_FindInFilesResults->GetRootItem(), filename);
        }
        else
        {
            // sort the results
            wxTreeItemIdValue cookie;
            wxTreeItemId id=m_FindInFilesResults->GetFirstChild(m_FindInFilesResults->GetRootItem(), cookie);
            size_t before=0;
            while(id.IsOk())
            {
                wxString idname=m_FindInFilesResults->GetItemText(id);
                if(wxm::FilePathNormalCase(filename) < wxm::FilePathNormalCase(idname))
                {
                    break;
                }
                id=m_FindInFilesResults->GetNextChild(m_FindInFilesResults->GetRootItem(), cookie);
                ++before;
            }

            if(!id.IsOk()) // append item
            {
                fileid = m_FindInFilesResults->AppendItem(m_FindInFilesResults->GetRootItem(), filename);
            }
            else // insert item
            {
                fileid = m_FindInFilesResults->InsertItem(m_FindInFilesResults->GetRootItem(), before, filename);
            }
        }
    }

    m_FindInFilesResults->AppendItem(fileid, text, -1, -1, new CaretPosData(filename, pageid, begpos, endpos));
}

//---------------------------------------------------------------------------

int MadEditFrame::OpenedFileCount()
{
    return (int)m_Notebook->GetPageCount();
}

void MadEditFrame::OpenFile(const wxString &filename, bool mustExist, const LineNumberList& bmklns)
{
    wxString title;
    if(filename.IsEmpty())
    {
        title.Printf(_("NoName%d"), ++m_NewFileCount);
    }
    else
    {
        int selid=m_Notebook->GetSelection();

        // check this file is opened or not
        int count=int(m_Notebook->GetPageCount());
        for(int id=0;id<count;id++)
        {
            wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(id);
            if (!wxm::FilePathEqual(wxmedit->GetFileName(), filename))
                continue;

            // YES, it's opened. Activate it.
            g_CheckModTimeForReload=false;
            m_Notebook->SetSelection(id);

            wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
            if (sel != g_active_wxmedit)
            {
                wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
                event.SetSelection(m_Notebook->GetSelection());
                event.SetOldSelection(selid);
                event.SetEventObject(this);
                OnNotebookPageChanged(event);
            }

            g_CheckModTimeForReload=true;
            g_active_wxmedit->ReloadByModificationTime();
            m_RecentFiles->AddFileToHistory(filename);  // bring the filename to top of list
            g_active_wxmedit->SetFocus();
            return;
        }

        if(MadDirExists(filename)!=0)
        {
            wxLogError(wxString(_("This is a Directory:")) + wxT("\n\n") + filename);
            return;
        }

        int utf8test=MadFileNameIsUTF8(filename);
        bool exists= (wxFileExists(filename) || (utf8test!=0));

        if(mustExist && !exists)
        {
            wxLogError(wxString(_("This file does not exist:")) + wxT("\n\n") + filename);
            return;
        }

        wxFileName fn(filename);
        title=fn.GetFullName();

        // test parent dir
        if(MadDirExists(fn.GetPath(wxPATH_GET_VOLUME))==0)
        {
            wxLogError(wxString(_("The Parent Directory of this file does not exist:")) + wxT("\n\n") + filename);
            return;
        }

        // test access mode
        MadConvFileName_WC2MB_UseLibc uselibc(utf8test<0);
        if(exists && wxFile::Access(filename.c_str(), wxFile::read)==false)
        {
            wxLogError(wxString(_("Cannot access this file:")) + wxT("\n\n") + filename);
            return;
        }
    }

    wxm::InFrameWXMEdit* wxmedit = g_active_wxmedit;

    if (!filename.IsEmpty() && wxmedit != nullptr
        && !wxmedit->IsModified() && wxmedit->GetFileName().IsEmpty())
    {
        // load file in g_active_wxmedit
    }
    else
    {
        // create a new MadEdit
        wxmedit = new wxm::InFrameWXMEdit(m_Notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

        g_PrevPageID=m_Notebook->GetSelection();

        m_Notebook->AddPage(wxmedit, title, true);
        m_Notebook->SetSelection(m_Notebook->GetPageCount()-1);
    }

    if(!filename.IsEmpty())
    {
        FileCaretPosManager::PosData data;
        g_FileCaretPosManager.GetRestoreData(filename, data);

        if (!data.fontname.IsEmpty() && data.fontsize > 0)
            wxmedit->SetTextFont(data.fontname, data.fontsize, false);

        if (!data.hex_fontname.IsEmpty() && data.hex_fontsize > 0)
            wxmedit->SetHexFont(data.hex_fontname, data.hex_fontsize, false);

        wxString encoding = (wxm::UseForceEncoding(m_Config))? wxm::GetForceEncoding(m_Config): data.encoding;

        if (!wxmedit->LoadFromFile(filename, encoding.wc_str(), data.hexmode) && mustExist)
        {
            wxLogError(wxString(_("Cannot load this file:")) + wxT("\n\n") + filename);
        }
        else
        {
            // add filename, fontname, and encoding to recentlist
            m_RecentFiles->AddFileToHistory(filename);

            wxmedit->RestoreBookmarkByLineNumberList(bmklns);

            bool rcp;
            m_Config->Read(wxT("/wxMEdit/RestoreCaretPos"), &rcp, true);
            if (rcp)
                wxmedit->SetCaretPosition(data.pos);
        }
    }
    wxString str;
    int size;
    wxmedit->GetFont(str, size);
    m_RecentFonts->AddItemToHistory(str);

    str = wxString(wxT('[')) + wxmedit->GetEncodingName() + wxT("] ") + wxGetTranslation(wxmedit->GetEncodingDescription().c_str());
    m_RecentEncodings->AddItemToHistory(str);

    wxmedit->SetFocus();

    if (g_active_wxmedit != wxmedit)
    {
        g_active_wxmedit = wxmedit;
        g_active_wxmedit->OnSelectionAndStatusChanged();
    }

    title = g_active_wxmedit->GetFileName();
    if (title.IsEmpty())
        title = m_Notebook->GetPageText( GetIdByEdit(g_active_wxmedit) );

    if (g_active_wxmedit->IsModified() && title[title.Len()-1]!=wxT('*'))
        title += wxT('*');

    SetTitle(wxString(wxT("wxMEdit - ["))+ title +wxString(wxT("] ")));
}

void MadEditFrame::CloseFile(int pageId)
{
    if (!QueryCloseFile(pageId))
        return;

    m_PageClosing=true;
    g_CheckModTimeForReload=false;
    m_Notebook->DeletePage(pageId);
    if(m_Notebook->GetPageCount()==0) OnNotebookPageClosed();
    g_CheckModTimeForReload=true;
    m_PageClosing=false;
}

bool MadEditFrame::QueryCloseFile(int idx)
{
    wxm::InFrameWXMEdit* wxmedit=(wxm::InFrameWXMEdit*)m_Notebook->GetPage(idx);
    if (wxmedit == nullptr)
        return false;

    wxString name=m_Notebook->GetPageText(idx);
    if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

    if (wxmedit->Save(true, name, false) == wxID_CANCEL)
        return false;

    g_FileCaretPosManager.Add(wxmedit);

    return true;
}

bool MadEditFrame::QueryCloseAllFiles()
{
    if(m_Notebook->GetPageCount()==0) return true;

    int selid=m_Notebook->GetSelection();
    if(selid==-1) return true;

    wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(selid);
    wxString name;
    if (wxmedit->IsModified())
    {
        name=m_Notebook->GetPageText(selid);
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

        if (wxmedit->Save(true, name, false) == wxID_CANCEL)
            return false;
    }
    g_FileCaretPosManager.Add(wxmedit);

    int count=int(m_Notebook->GetPageCount());
    int id=0, sid=selid;
    do
    {
        if (id == selid)
            continue;

        wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(id);
        if (wxmedit->IsModified())
        {
            m_Notebook->SetSelection(id);

            wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
            if(sel != g_active_wxmedit)
            {
                wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
                event.SetSelection(id);
                event.SetOldSelection(sid);
                event.SetEventObject(this);
                OnNotebookPageChanged(event);
            }

            name=m_Notebook->GetPageText(id);
            if(name[name.Len()-1]==wxT('*'))
                name.Truncate(name.Len()-1);

            if (wxmedit->Save(true, name, false) == wxID_CANCEL)
                return false;

            sid=id;
        }
        g_FileCaretPosManager.Add(wxmedit);
    }
    while(++id<count);

    return true;
}

//---------------------------------------------------------------------------

void MadEditFrame::OnUpdateUI_MenuFile_CheckCount(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
}

void MadEditFrame::OnUpdateUI_MenuFile_CheckNamed(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && !g_active_wxmedit->GetFileName().empty());
}

void MadEditFrame::OnUpdateUI_MenuFileRecentFiles(wxUpdateUIEvent& event)
{
    event.Enable(m_RecentFiles->GetCount()!=0);
}

void MadEditFrame::OnUpdateUI_MenuEditUndo(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->CanUndo());
}
void MadEditFrame::OnUpdateUI_MenuEditRedo(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->CanRedo());
}

void MadEditFrame::OnUpdateUI_MenuEdit_CheckSelection(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->IsSelected());
}

void MadEditFrame::OnUpdateUI_MenuEdit_CheckSelSize(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetSelectionSize()>0);
}

void MadEditFrame::OnUpdateUI_MenuEditPaste(wxUpdateUIEvent& event)
{
#ifdef __WXMSW__
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->CanPaste());
#else
    event.Enable(g_active_wxmedit!=nullptr); // workaround for high CPU loading in Linux
#endif
}

void MadEditFrame::OnUpdateUI_MenuEditPasteOvr(wxUpdateUIEvent& event)
{
#ifdef __WXMSW__
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->EditingInHexAera() && g_active_wxmedit->CanPaste());
#else
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->EditingInHexAera());
#endif
}

void MadEditFrame::OnUpdateUI_Menu_CheckSize(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetFileSize());
}

void MadEditFrame::OnUpdateUI_MenuEditDeleteLine(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode);
}

void MadEditFrame::OnUpdateUI_MenuEditInsertTabChar(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode);
}
void MadEditFrame::OnUpdateUI_MenuEditInsertDateTime(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode);
}

void MadEditFrame::OnUpdateUI_MenuEditToggleBookmark(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit != nullptr);
}

void MadEditFrame::OnUpdateUI_MenuEditBookmarkExist(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit != nullptr && g_active_wxmedit->BookmarkExist());
}

void MadEditFrame::OnUpdateUI_Menu_CheckTextFile(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode);
}

void MadEditFrame::OnUpdateUI_MenuEditCopyAsHexString(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit && //g_active_wxmedit->GetEditMode()==emHexMode &&
        g_active_wxmedit->IsSelected());
}

void MadEditFrame::OnUpdateUI_MenuEdit_Column(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit != nullptr && g_active_wxmedit->GetEditMode() == emColumnMode);
}

void MadEditFrame::OnUpdateUI_MenuIndent(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit && !g_active_wxmedit->IsReadOnly() &&
        g_active_wxmedit->GetEditMode()!=emHexMode );
}

void MadEditFrame::OnUpdateUI_MenuComment(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit && !g_active_wxmedit->IsReadOnly() &&
        g_active_wxmedit->HasLineComment() && g_active_wxmedit->GetEditMode()!=emHexMode );
}

void MadEditFrame::OnUpdateUI_MenuSearchGoTo(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
}

void MadEditFrame::OnUpdateUI_MenuSearchGoToBrace(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && g_active_wxmedit->HasBracePair() &&
        g_active_wxmedit->GetEditMode()!=emHexMode);
}

void MadEditFrame::OnUpdateUI_MenuViewEncoding(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        wxString str=wxString(wxT('[')) + g_active_wxmedit->GetEncodingName() + wxT("] ") + wxGetTranslation(g_active_wxmedit->GetEncodingDescription().c_str());
        event.SetText(wxString(_("Encoding: ")) + str);

        //m_RecentEncodings->AddItemToHistory(str);
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Encoding: ")) + _("None") );
    }
}

void MadEditFrame::OnUpdateUI_MenuViewSyntax(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        event.SetText(wxString(_("Syntax Type: ")) + g_active_wxmedit->GetSyntaxTitle());
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Syntax Type: ")) + _("None") );
    }
}

void MadEditFrame::OnUpdateUI_MenuViewFontName(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        wxString name;
        int size;
        g_active_wxmedit->GetFont(name, size);
        event.SetText(wxString(_("Font Name: ")) + name);
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Font Name: ")) + _("None") );
    }
}
void MadEditFrame::OnUpdateUI_MenuViewFontSize(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        wxString name;
        int size;
        g_active_wxmedit->GetFont(name, size);
        event.SetText(wxString(_("Font Size: ")) << size);
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Font Size: ")) + _("None") );
    }
}
void MadEditFrame::OnUpdateUI_MenuViewSetFont(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
}

void MadEditFrame::OnUpdateUI_MenuViewFixedWidthMode(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetFixedWidthMode());
}

void MadEditFrame::OnUpdateUI_MenuViewTabColumn(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        event.SetText(wxString(_("Tab Column: ")) << g_active_wxmedit->GetTabColumns());
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Tab Column: ")) + _("None") );
    }
}

void MadEditFrame::OnUpdateUI_MenuViewLineSpacing(wxUpdateUIEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        event.Enable(true);
        event.SetText(wxString(_("Line Spacing: ")) << g_active_wxmedit->GetLineSpacing() << wxT('%'));
    }
    else
    {
        event.Enable(false);
        event.SetText(wxString(_("Line Spacing: ")) + _("None") );
    }
}

void MadEditFrame::OnUpdateUI_MenuViewNoWrap(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetWordWrapMode()==wwmNoWrap);
}
void MadEditFrame::OnUpdateUI_MenuViewWrapByWindow(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetWordWrapMode()==wwmWrapByWindow);
}
void MadEditFrame::OnUpdateUI_MenuViewWrapByColumn(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetWordWrapMode()==wwmWrapByColumn);
}

void MadEditFrame::OnUpdateUI_MenuViewDisplayLineNumber(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->LineNumberVisible());
}
void MadEditFrame::OnUpdateUI_MenuViewDisplayBookmark(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit && g_active_wxmedit->BookmarkVisible());
}
void MadEditFrame::OnUpdateUI_MenuViewShowEndOfLine(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetShowEndOfLine());
}
void MadEditFrame::OnUpdateUI_MenuViewShowTabChar(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetShowTabChar());
}
void MadEditFrame::OnUpdateUI_MenuViewShowSpaceChar(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetShowSpaceChar());
}
void MadEditFrame::OnUpdateUI_MenuViewMarkActiveLine(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetMarkActiveLine());
}
void MadEditFrame::OnUpdateUI_MenuViewMarkBracePair(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetMarkBracePair());
}

void MadEditFrame::OnUpdateUI_MenuViewTextMode(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()==emTextMode);
}
void MadEditFrame::OnUpdateUI_MenuViewColumnMode(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()==emColumnMode);
}
void MadEditFrame::OnUpdateUI_MenuViewHexMode(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr);
    event.Check(g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()==emHexMode);
}

void MadEditFrame::OnUpdateUI_MenuToolsByteOrderMark(wxUpdateUIEvent& event)
{
    if(g_active_wxmedit!=nullptr && g_active_wxmedit->IsTextFile() &&
       g_active_wxmedit->IsUnicodeFile())      // unicode format
    {
        event.Enable(true);
        if (g_active_wxmedit->HasBOM())
        {
            event.SetText(_("Has Unicode BOM (Byte-Order Mark)"));
        }
        else
        {
            event.SetText(_("Has No Unicode BOM (Byte-Order Mark)"));
        }
    }
    else
    {
        event.Enable(false);
        event.SetText(_("Has No Unicode BOM (Byte-Order Mark)"));
    }
}
void MadEditFrame::OnUpdateUI_MenuToolsNewLineChar(wxUpdateUIEvent& event)
{
    wxString text(_("NewLine Char (File Format): "));
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->IsTextFile())
    {
        event.Enable(true);

        text += g_active_wxmedit->GetNewLine().Description();
        event.SetText(text);
    }
    else
    {
        event.Enable(false);
        event.SetText(text + _("None"));
    }
}
void MadEditFrame::OnUpdateUI_MenuToolsInsertNewLineChar(wxUpdateUIEvent& event)
{
    wxString text(_("Press Enter to Insert NewLine Char: "));
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->IsTextFile())
    {
        event.Enable(true);

        text += g_active_wxmedit->GetNewLine4Insert().Description();
        event.SetText(text);
    }
    else
    {
        event.Enable(false);
        event.SetText(text + _("None"));
    }
}

void MadEditFrame::OnUpdateUI_MenuToolsConvertNL(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr && !g_active_wxmedit->IsReadOnly());
}

void MadEditFrame::OnUpdateUI_MenuToolsConvertEncoding(wxUpdateUIEvent& event)
{
    event.Enable(g_active_wxmedit!=nullptr &&
        !g_active_wxmedit->IsReadOnly() && g_active_wxmedit->IsTextFile());
}

void MadEditFrame::OnUpdateUI_MenuWindow_CheckCount(wxUpdateUIEvent& event)
{
    event.Enable(m_Notebook->GetPageCount()>=2);
}

void MadEditFrame::OnUpdateUI_MenuHelp_CheckUpdates(wxUpdateUIEvent& event)
{
    event.Enable(!wxm::g_update_checking);
}

//---------------------------------------------------------------------------

void MadEditFrame::OnFileNew(wxCommandEvent& event)
{
    OpenFile(wxEmptyString, false);
}

void MadEditFrame::OnFileOpen(wxCommandEvent& event)
{
    wxString dir;
    if(m_RecentFiles->GetCount())
    {
        wxFileName filename(m_RecentFiles->GetHistoryFile(0));
        dir=filename.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
    }

    wxFileDialog dlg(this, _("Open File"), dir, wxEmptyString, wxFileSelectorDefaultWildcardStr,
#if wxCHECK_VERSION(2,8,0)
    wxFD_OPEN|wxFD_MULTIPLE );
#else
    wxOPEN|wxMULTIPLE );
#endif

    if (dlg.ShowModal()==wxID_OK)
    {
        wxArrayString filenames;

        g_MB2WC_check_dir_filename=true;
        dlg.GetPaths(filenames);
        g_MB2WC_check_dir_filename=false;

        size_t count=filenames.GetCount();
        for(size_t i=0;i<count;i++)
        {
            OpenFile(filenames[i], false);
        }
    }
}

void MadEditFrame::OnFileSave(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr)
    {
        wxString name=m_Notebook->GetPageText(m_Notebook->GetSelection());
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

        if (g_active_wxmedit->Save(false, name, false)==wxID_YES)
        {
            m_RecentFiles->AddFileToHistory(g_active_wxmedit->GetFileName());
        }
    }
}

void MadEditFrame::OnFileSaveAs(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr)
    {
        wxString name=m_Notebook->GetPageText(m_Notebook->GetSelection());
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

        if (g_active_wxmedit->Save(false, name, true)==wxID_YES)
        {
            m_RecentFiles->AddFileToHistory(g_active_wxmedit->GetFileName());
        }
    }
}

void MadEditFrame::OnFileSaveAll(wxCommandEvent& event)
{
    int selid=m_Notebook->GetSelection();
    if(selid==-1) return; // no file was opened

    wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(selid);
    wxString name;
    if (wxmedit->IsModified())
    {
        name=m_Notebook->GetPageText(selid);
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

        if (wxmedit->Save(false, name, false) == wxID_CANCEL)
            return;
    }

    int count=int(m_Notebook->GetPageCount());
    int id=0, sid=selid;
    do
    {
        if (id == selid)
            continue;

        wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(id);
        if (!wxmedit->IsModified())
            continue;

        if (wxmedit->GetFileName().IsEmpty())
        {
            m_Notebook->SetSelection(id);

            wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
            if (sel != g_active_wxmedit)
            {
                wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
                event.SetSelection(id);
                event.SetOldSelection(sid);
                event.SetEventObject(this);
                OnNotebookPageChanged(event);
            }

        }
        name=m_Notebook->GetPageText(id);
        if(name[name.Len()-1]==wxT('*'))
            name.Truncate(name.Len()-1);

        if (wxmedit->Save(false, name, false) == wxID_CANCEL)
            return;

        sid=id;
    }
    while(++id<count);
}

void MadEditFrame::OnFileReload(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr)
    {
        g_active_wxmedit->Reload();
    }
}

void MadEditFrame::OnFileClose(wxCommandEvent& event)
{
    int idx=m_Notebook->GetSelection();
    CloseFile(idx);
}

void MadEditFrame::OnFileCloseByPath(wxCommandEvent& event)
{
    const wxString& dir = wxDirSelector(_("Close all files in this foler and in all the subfolers:"), wxm::g_lastpath_closingfiles,
                                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST, wxDefaultPosition, this);
    if (dir.empty())
        return;

    wxm::g_lastpath_closingfiles = dir;
    m_Config->Write(wxT("/wxMEdit/LastPathClosingFiles"), dir);

    wxString nmldir = wxm::FilePathNormalCase(dir);
    std::list<wxMadAuiNotebook::PageData> pglist = m_Notebook->GetPagesList();
    std::vector<wxMadAuiNotebook::PageData> pgvec(pglist.begin(), pglist.end());
    for (size_t i=0; i<pgvec.size(); ++i)
    {
        wxString file = wxm::FilePathNormalCase(pgvec[i].wxmedit->GetFileName());
        if (!file.StartsWith(nmldir.c_str()))
            continue;

        CloseFile(pgvec[i].pageid);
        --i;
        pglist = m_Notebook->GetPagesList();
        pgvec.assign(pglist.begin(), pglist.end());
    }
}

void MadEditFrame::OnFileCloseAll(wxCommandEvent& event)
{
    if(QueryCloseAllFiles())
    {
        m_PageClosing=true;
        //m_Notebook->DeleteAllPages();
        while(m_Notebook->GetPageCount()!=0)
        {
            m_Notebook->DeletePage(0);
        }
        m_PageClosing=false;

        g_active_wxmedit = nullptr;
        SetTitle(wxString(wxT("wxMEdit ")));
        ResetStatusBar();;
    }
}

void MadEditFrame::OnFileCloseAllButThis(wxCommandEvent& event)
{
    OnFileCloseAllToTheLeft(event);
    OnFileCloseAllToTheRight(event);
}

void MadEditFrame::OnFileCloseAllToTheLeft(wxCommandEvent& event)
{
    for (int pgid = m_Notebook->GetPagesList().begin()->pageid;
         pgid != m_Notebook->GetSelection();
         pgid = m_Notebook->GetPagesList().begin()->pageid)
    {
        CloseFile(pgid);
    }
}

void MadEditFrame::OnFileCloseAllToTheRight(wxCommandEvent& event)
{
    for (int pgid = m_Notebook->GetPagesList().rbegin()->pageid;
        pgid != m_Notebook->GetSelection();
        pgid = m_Notebook->GetPagesList().rbegin()->pageid)
    {
        CloseFile(pgid);
    }
}


void MadEditFrame::OnFileCopyFullPath(wxCommandEvent& event)
{
    MadEdit::PutTextToClipboard(g_active_wxmedit->GetFileName());
}

void MadEditFrame::OnFileCopyFilename(wxCommandEvent& event)
{
    MadEdit::PutTextToClipboard(wxFileName(g_active_wxmedit->GetFileName()).GetFullName());
}

void MadEditFrame::OnFileCopyFileDir(wxCommandEvent& event)
{
    MadEdit::PutTextToClipboard(wxFileName(g_active_wxmedit->GetFileName()).GetPath());
}


void MadEditFrame::OnFilePageSetup(wxCommandEvent& event)
{
    //(*g_PageSetupData) = *g_PrintData;

    wxPageSetupDialog pageSetupDialog(this, g_PageSetupData);
    if(pageSetupDialog.ShowModal()==wxID_OK)
    //pageSetupDialog.ShowModal();
    {
        //(*g_PrintData) = pageSetupDialog.GetPageSetupData().GetPrintData();
        (*g_PageSetupData) = pageSetupDialog.GetPageSetupData();
        //g_PageSetupData->SetPrintData( pageSetupDialog.GetPageSetupData().GetPrintData() );

        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/wxMEdit"));

        m_Config->Write(wxT("PageOrientation"), (long)g_PageSetupData->GetPrintData().GetOrientation());

        //((wxFrame*)wxTheApp->GetTopWindow())->SetTitle(wxString::Format(wxT("%d"), g_PageSetupData->GetPaperId()));
        wxSize size=g_PageSetupData->GetPaperSize();
        m_Config->Write(wxT("PagePaperSizeW"), size.x);
        m_Config->Write(wxT("PagePaperSizeH"), size.y);

        wxPoint p=g_PageSetupData->GetMarginTopLeft();
        m_Config->Write(wxT("PageLeftMargin"), p.x);
        m_Config->Write(wxT("PageTopMargin"), p.y);
        p=g_PageSetupData->GetMarginBottomRight();
        m_Config->Write(wxT("PageRightMargin"), p.x);
        m_Config->Write(wxT("PageBottomMargin"), p.y);

        m_Config->SetPath(oldpath);
    }
}

void MadEditFrame::OnFilePrintPreview(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    // Pass two printout objects: for preview, and possible printing.
    wxPrintDialogData printDialogData(g_PageSetupData->GetPrintData());//*g_PrintData);

    wxPrintPreview *preview = new wxPrintPreview(new MadPrintout, new MadPrintout, &printDialogData);

//#ifdef __WXMSW__
    //preview->SetZoom(30);
//#endif

    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox(_("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), _("Previewing"), wxOK);
    }
    else
    {
        wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _("wxMEdit Print Preview"),
            wxPoint(0, 0),
#ifdef __WXMSW__
            wxSize(600, 720)
#else
            wxSize(600, 620)
#endif
        );

        frame->Centre(wxBOTH);
        frame->Initialize();
        frame->Show();
    }
}


void PrintOut(wxWindow *parentWin)
{
    wxPrintDialogData printDialogData(g_PageSetupData->GetPrintData());//*g_PrintData);
    wxPrinter printer(&printDialogData);
    MadPrintout printout;

    if (!printer.Print(parentWin, &printout, true /*prompt*/))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
            wxMessageBox(_("There was a problem printing.\nPerhaps your current printer is not set correctly?"), _("Printing"), wxOK);
    }
    else
    {
        //(*g_PrintData) = printer.GetPrintDialogData().GetPrintData();
        g_PageSetupData->SetPrintData(printer.GetPrintDialogData().GetPrintData());
    }
}

#if defined(__WXMSW__)
/* temp modal-dialog for printing */
class TempPrintDialog : public wxDialog
{
private:
    DECLARE_EVENT_TABLE()
public:
    TempPrintDialog(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("TopPrintDialog"),
        const wxPoint& pos = wxPoint(10,10),//wxDefaultPosition,
        const wxSize& size = wxSize(1, 1),//wxDefaultSize,
        long style = wxDIALOG_NO_PARENT)
        : wxDialog(parent, id, title, pos, size, style), m_Printed(false) { }
    virtual ~TempPrintDialog() { }

    void OnPaint(wxPaintEvent &evt);

    bool m_Printed;
};

BEGIN_EVENT_TABLE(TempPrintDialog,wxDialog)
    EVT_PAINT(TempPrintDialog::OnPaint)
END_EVENT_TABLE()

void TempPrintDialog::OnPaint(wxPaintEvent &evt)
{
    if(!m_Printed)
    {
        m_Printed=true;

        PrintOut(this);

        Close(); //EndModal(wxID_OK);
    }
    evt.Skip();
}
#endif

void MadEditFrame::OnFilePrint(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

#if defined(__WXMSW__)
    // using a temp modal-dialog to avoid the user change the contents of Edit
    boost::scoped_ptr<TempPrintDialog>(new TempPrintDialog(this))->ShowModal();
#else
    PrintOut(this);
#endif
}


void MadEditFrame::OnFileRecentFile(wxCommandEvent& event)
{
    int idx=event.GetId()-wxID_FILE1;
    wxString file=m_RecentFiles->GetHistoryFile(idx);
    if(!file.IsEmpty())
    {
        OpenFile(file, true);
    }
}

void MadEditFrame::OnFileExit(wxCommandEvent& event)
{
    Close(false);
}

void MadEditFrame::OnEditUndo(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->Undo();
}

void MadEditFrame::OnEditRedo(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->Redo();
}

void MadEditFrame::OnEditCut(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CutToClipboard();
}

void MadEditFrame::OnEditCopy(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CopyToClipboard();
}

void MadEditFrame::OnEditPaste(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->PasteFromClipboard(false);
}

void MadEditFrame::OnEditPasteOvr(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->PasteFromClipboard(true);
}

void MadEditFrame::OnEditDelete(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->Delete();
}

void MadEditFrame::OnEditCutLine(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CutLine();
}

void MadEditFrame::OnEditDeleteLine(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->DeleteLine();
}

void MadEditFrame::OnEditSelectAll(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->SelectAll();
}

void MadEditFrame::OnEditInsertTabChar(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->InsertTabChar();
}

void MadEditFrame::OnEditInsertDateTime(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->InsertDateTime();
}

void MadEditFrame::OnEditSortAscending(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode)
    {
        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);
        g_active_wxmedit->SortLines(sfAscending, begin, end);
    }
}

void MadEditFrame::OnEditSortDescending(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode)
    {
        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);
        g_active_wxmedit->SortLines(sfDescending, begin, end);
    }
}

void MadEditFrame::OnEditSortAscendingCase(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode)
    {
        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);
        g_active_wxmedit->SortLines(sfAscending|sfCaseSensitive, begin, end);
    }
}

void MadEditFrame::OnEditSortDescendingCase(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode)
    {
        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);
        g_active_wxmedit->SortLines(sfDescending|sfCaseSensitive, begin, end);
    }
}

void MadEditFrame::OnEditSortByOptions(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr && g_active_wxmedit->GetEditMode()!=emHexMode)
    {
        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/wxMEdit"));

        int order;
        bool cs, num, rem;
        m_Config->Read(wxT("SortOrder"), &order, sfAscending);
        m_Config->Read(wxT("SortCaseSensitive"), &cs, false);
        m_Config->Read(wxT("SortNumeric"), &num, false);
        m_Config->Read(wxT("SortRemoveDup"), &rem, false);
        m_Config->SetPath(oldpath);

        MadSortFlags flags = order |
            (cs ? sfCaseSensitive : 0) |
            (num ? sfNumericSort : 0) |
            (rem ? sfRemoveDuplicate : 0) ;

        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);

        g_active_wxmedit->SortLines(flags, begin, end);
    }
}

void MadEditFrame::OnEditSortOptions(wxCommandEvent& event)
{
    if (g_active_wxmedit==nullptr || g_active_wxmedit->GetEditMode()==emHexMode)
        return;

    WXMSortDialog dialog(this);

    wxString oldpath=m_Config->GetPath();
    m_Config->SetPath(wxT("/wxMEdit"));

    int order;
    bool cs, num, rem;
    m_Config->Read(wxT("SortOrder"), &order, sfAscending);
    dialog.WxRadioBoxOrder->SetSelection(order);

    m_Config->Read(wxT("SortCaseSensitive"), &cs, false);
    dialog.WxCheckBoxCase->SetValue(cs);

    m_Config->Read(wxT("SortNumeric"), &num, false);
    dialog.WxCheckBoxNumeric->SetValue(num);

    m_Config->Read(wxT("SortRemoveDup"), &rem, false);
    dialog.WxCheckBoxRemoveDup->SetValue(rem);

    if(dialog.ShowModal()==wxID_OK)
    {
        order = dialog.WxRadioBoxOrder->GetSelection();
        cs = dialog.WxCheckBoxCase->GetValue();
        num = dialog.WxCheckBoxNumeric->GetValue();
        rem = dialog.WxCheckBoxRemoveDup->GetValue();

        m_Config->Write(wxT("SortOrder"), order);
        m_Config->Write(wxT("SortCaseSensitive"), cs);
        m_Config->Write(wxT("SortNumeric"), num);
        m_Config->Write(wxT("SortRemoveDup"), rem);

        int flags = order |
            (cs ? sfCaseSensitive : 0) |
            (num ? sfNumericSort : 0) |
            (rem ? sfRemoveDuplicate : 0) ;

        int begin, end;
        g_active_wxmedit->GetSelectionLineId(begin, end);

        g_active_wxmedit->SortLines(flags, begin, end);
    }

    m_Config->SetPath(oldpath);
}

void MadEditFrame::OnEditCopyAsHexString(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CopyAsHexString(false);
}
void MadEditFrame::OnEditCopyAsHexStringWithSpace(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CopyAsHexString(true);
}

void MadEditFrame::OnEditIncIndent(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->IncreaseDecreaseIndent(true);
}
void MadEditFrame::OnEditDecIndent(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->IncreaseDecreaseIndent(false);
}

void MadEditFrame::OnEditComment(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CommentUncomment(true);
}
void MadEditFrame::OnEditUncomment(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->CommentUncomment(false);
}

void MadEditFrame::OnEditWordWrapToNewLine(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ConvertWordWrapToNewLine();
}
void MadEditFrame::OnEditNewLineToWordWrap(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ConvertNewLineToWordWrap();
}

void MadEditFrame::OnEditToUpperCase(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ToUpperCase();
}

void MadEditFrame::OnEditToLowerCase(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ToLowerCase();
}

void MadEditFrame::OnEditInvertCase(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->InvertCase();
}

void MadEditFrame::OnEditToHalfWidth(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ToHalfWidth();
}

void MadEditFrame::OnEditToHalfWidthByOptions(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    static wxArrayInt selections;
    static bool initialized = false;
    if(!initialized)
    {
        selections.Add(0);
        selections.Add(1);
        selections.Add(2);
        selections.Add(3);
        initialized = true;
    }

    wxString choices[4] = { _("ASCII characters"), _("Japanese characters"),
                            _("Korean characters"), _("other characters") };
	size_t sels = wxGetSelectedChoices(selections,
        _("Choose the characters you want to convert:"), _("To Halfwidth by Options..."),
        4, choices, this );

    if(sels > 0)
    {
        bool ascii=false, japanese=false, korean=false, other=false;
        for(size_t i=0; i<sels; i++)
        {
            switch(selections[i])
            {
            case 0: ascii = true; break;
            case 1: japanese = true; break;
            case 2: korean = true; break;
            case 3: other = true; break;
            }
        }
        g_active_wxmedit->ToHalfWidth(ascii, japanese, korean, other);
    }
}

void MadEditFrame::OnEditToFullWidth(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ToFullWidth();
}

void MadEditFrame::OnEditToFullWidthByOptions(wxCommandEvent& event)
{
    if(g_active_wxmedit == nullptr) return;

    static wxArrayInt selections;
    static bool initialized = false;
    if(!initialized)
    {
        selections.Add(0);
        selections.Add(1);
        selections.Add(2);
        selections.Add(3);
        initialized = true;
    }

    wxString choices[4] = { _("ASCII characters"), _("Japanese characters"),
                            _("Korean characters"), _("other characters") };
	size_t sels = wxGetSelectedChoices(selections,
        _("Choose the characters you want to convert:"), _("To Fullwidth by Options..."),
        4, choices, this );

    if(sels > 0)
    {
        bool ascii=false, japanese=false, korean=false, other=false;
        for(size_t i=0; i<sels; i++)
        {
            switch(selections[i])
            {
            case 0: ascii = true; break;
            case 1: japanese = true; break;
            case 2: korean = true; break;
            case 3: other = true; break;
            }
        }
        g_active_wxmedit->ToFullWidth(ascii, japanese, korean, other);
    }
}

void MadEditFrame::OnEditTabToSpace(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ConvertTabToSpace();
}
void MadEditFrame::OnEditSpaceToTab(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ConvertSpaceToTab();
}

void MadEditFrame::OnEditTrimTrailingSpaces(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr) g_active_wxmedit->TrimTrailingSpaces();
}

void MadEditFrame::OnEditInsertEnumeration(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr) g_active_wxmedit->InsertEnumeration();
}

void MadEditFrame::OnEditColumnAlign(wxCommandEvent& event)
{
    if (g_active_wxmedit!=nullptr) g_active_wxmedit->ColumnAlign();
}

void MadEditFrame::OnEditColumnPaste(wxCommandEvent& event)
{
    if (g_active_wxmedit != nullptr) g_active_wxmedit->ColumnPaste();
}

namespace wxm
{

template<typename DLG>
struct SearchingTextAssigner
{
	bool AssignSelectedText()
	{
		if (!g_active_wxmedit->IsSelected() || g_active_wxmedit->GetSelectionSize() > 10240)
			return false;

		if (InHex())
		{
			wxString ws;
			g_active_wxmedit->GetSelHexString(ws, true);
			SetSearchingText(ws);
			return true;
		}

		if (RegexEnabled())
			return true;

		wxString ws;
		g_active_wxmedit->GetSelText(ws);
		SetSearchingText(ws);
		return true;
	}

	void AssignWordAtCaret()
	{
		if (RegexEnabled() || InHex())
			return;

		wxString ws;
		g_active_wxmedit->GetWordFromCaretPos(ws);
		if (!ws.IsEmpty() && !u_isspace(ws[0]))
			SetSearchingText(ws);
	}

	SearchingTextAssigner(DLG& dlg) : m_dlg(dlg) {}

private:
	bool InHex() { return m_dlg.WxCheckBoxFindHex->GetValue(); }
	bool RegexEnabled() { return m_dlg.WxCheckBoxRegex->GetValue(); }
	void SetSearchingText(const wxString& ws) { m_dlg.m_FindText->SetText(ws); }

	DLG& m_dlg;
};

}

void MadEditFrame::OnSearchFind(wxCommandEvent& event)
{
    if (g_active_wxmedit==nullptr)
        return;

    GetSearchReplaceDialog(this).ShowOnlyFindFunc();

    wxString fname;
    int fsize;

    GetSearchReplaceDialog(this).UpdateCheckBoxByCBHex();

    g_active_wxmedit->GetFont(fname, fsize);
    GetSearchReplaceDialog(this).m_FindText->SetFont(fname, 12);

    wxm::SearchingTextAssigner<WXMSearchReplaceDialog> assigner(GetSearchReplaceDialog(this));
    if (!assigner.AssignSelectedText())
        assigner.AssignWordAtCaret();

    GetSearchReplaceDialog(this).m_FindText->SelectAll();
    GetSearchReplaceDialog(this).m_FindText->SetFocus();
}

void MadEditFrame::OnSearchFindNext(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    GetSearchReplaceDialog(this).UpdateCheckBoxByCBHex();

    wxm::SearchingTextAssigner<WXMSearchReplaceDialog> assigner(GetSearchReplaceDialog(this));
    assigner.AssignSelectedText();

    wxCommandEvent e;
    GetSearchReplaceDialog(this).WxButtonFindNextClick(e);
}

void MadEditFrame::OnSearchFindPrevious(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    GetSearchReplaceDialog(this).UpdateCheckBoxByCBHex();

    wxm::SearchingTextAssigner<WXMSearchReplaceDialog> assigner(GetSearchReplaceDialog(this));
    assigner.AssignSelectedText();

    wxCommandEvent e;
    GetSearchReplaceDialog(this).WxButtonFindPrevClick(e);
}

void MadEditFrame::OnSearchReplace(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    GetSearchReplaceDialog(this).ShowWithReplaceFunc();

    wxString fname;
    int fsize;

    GetSearchReplaceDialog(this).UpdateCheckBoxByCBHex();

    g_active_wxmedit->GetFont(fname, fsize);
    GetSearchReplaceDialog(this).m_FindText->SetFont(fname, 12);
    GetSearchReplaceDialog(this).m_ReplaceText->SetFont(fname, 12);

    wxm::SearchingTextAssigner<WXMSearchReplaceDialog> assigner(GetSearchReplaceDialog(this));
    if (!assigner.AssignSelectedText())
        assigner.AssignWordAtCaret();

    GetSearchReplaceDialog(this).m_FindText->SelectAll();
    GetSearchReplaceDialog(this).m_FindText->SetFocus();
}

void MadEditFrame::OnSearchFindInFiles(wxCommandEvent& event)
{
    GetFindInFilesDialog(this).Show();
    GetFindInFilesDialog(this).SetFocus();
    GetFindInFilesDialog(this).Raise();

    wxString fname;
    int fsize;
    if (g_active_wxmedit != nullptr)
        g_active_wxmedit->GetFont(fname, fsize);
    else
        GetFindInFilesDialog(this).m_FindText->GetFont(fname, fsize);
    GetFindInFilesDialog(this).m_FindText->SetFont(fname, 12);
    GetFindInFilesDialog(this).m_ReplaceText->SetFont(fname, 12);

    wxm::SearchingTextAssigner<WXMFindInFilesDialog> assigner(GetFindInFilesDialog(this));
    if (g_active_wxmedit != nullptr)
        assigner.AssignSelectedText();

    GetFindInFilesDialog(this).m_FindText->SelectAll();
    GetFindInFilesDialog(this).m_FindText->SetFocus();
}

void MadEditFrame::OnSearchShowFindInFilesResults(wxCommandEvent& event)
{
    if(m_AuiManager.GetPane(m_InfoNotebook).IsShown())
    {
        m_AuiManager.GetPane(m_InfoNotebook).Hide();
    }
    else
    {
        m_AuiManager.GetPane(m_InfoNotebook).Show();
    }
    m_AuiManager.Update();
}

void MadEditFrame::OnSearchGoToLine(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    static wxString defstr;
    wxString str=wxGetTextFromUser(_("Line Number: (you can input HexNumber: 0xNNN)"), _("Go To Line"), defstr);

    if(!str.IsEmpty())
    {
        defstr=str;
    }

    str.Trim(false);
    str.Trim(true);
    long line;
    int base=10;
    if(str.Left(2).Lower()==wxT("0x")) base=16;

    if(!str.IsEmpty() && str.ToLong(&line, base))
    {
        g_active_wxmedit->GoToLine(line);
    }
}

void MadEditFrame::OnSearchGoToPosition(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    static wxString defstr;
    wxString str=wxGetTextFromUser(_("Position: (you can input HexNumber: 0xNNN)"), _("Go To Position"), defstr);
    if(!str.IsEmpty())
    {
        defstr=str;

        wxInt64 pos;
        if(StrToInt64(str, pos))
        {
            g_active_wxmedit->SetCaretPosition(pos);
        }
    }
}

void MadEditFrame::OnSearchGoToLeftBrace(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->GoToLeftBrace();
}
void MadEditFrame::OnSearchGoToRightBrace(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->GoToRightBrace();
}

void MadEditFrame::OnSearchToggleBookmark(wxCommandEvent& event)
{
    if ( g_active_wxmedit )
        g_active_wxmedit->ToggleBookmark();
}

void MadEditFrame::OnSearchGotoNextBookmark(wxCommandEvent& event)
{
    if ( g_active_wxmedit )
        g_active_wxmedit->GotoNextBookmark();
}

void MadEditFrame::OnSearchGotoPreviousBookmark(wxCommandEvent& event)
{
    if ( g_active_wxmedit )
        g_active_wxmedit->GotoPreviousBookmark();
}

void MadEditFrame::OnSearchClearAllBookmarks(wxCommandEvent& event)
{
    if ( g_active_wxmedit )
        g_active_wxmedit->ClearAllBookmarks();
}


void MadEditFrame::OnViewEncoding(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int idx=event.GetId()-menuEncoding1;
    std::wstring enc=xm::EncodingManager::Instance().GetEncodingName(idx);
    g_active_wxmedit->SetEncoding(enc);

    wxString str=wxString(wxT('['))+ enc.c_str() + wxT("] ")+ wxGetTranslation(xm::EncodingManager::Instance().GetEncodingDescription(idx).c_str());
    m_RecentEncodings->AddItemToHistory(str);

    int size;
    g_active_wxmedit->GetFont(str, size);
    m_RecentFonts->AddItemToHistory(str);
}

void MadEditFrame::OnViewRecentEncoding(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int idx=event.GetId()-menuRecentEncoding1;
    wxString str=m_RecentEncodings->GetHistoryFile(idx);
    if(!str.IsEmpty())
    {
        // get encname from str
        wxStringTokenizer tkz(str, wxT("[]"), wxTOKEN_STRTOK);
        if( tkz.HasMoreTokens() )
        {
            wxString enc = tkz.GetNextToken();
            g_active_wxmedit->SetEncoding(enc.wx_str());

            m_RecentEncodings->AddItemToHistory(str);

            wxString str;
            int size;
            g_active_wxmedit->GetFont(str, size);
            m_RecentFonts->AddItemToHistory(str);
        }

    }
}

void MadEditFrame::OnViewSyntax(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int idx=event.GetId()-menuSyntax1;
    wxString title=MadSyntax::GetSyntaxTitle(idx);
    g_active_wxmedit->SetSyntax(title, true);
}

void MadEditFrame::OnViewFontName(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int idx=event.GetId()-menuFontName1;
    wxString fn;
    int fs;
    g_active_wxmedit->GetFont(fn, fs);

    wxString &fontname=g_FontNames[idx];
    g_active_wxmedit->SetFont(fontname, fs);

    m_RecentFonts->AddItemToHistory(fontname);
}

void MadEditFrame::OnViewRecentFont(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int idx=event.GetId()-menuRecentFont1;
    wxString fontname=m_RecentFonts->GetHistoryFile(idx);
    if(!fontname.IsEmpty())
    {
        wxString fn;
        int fs;
        g_active_wxmedit->GetFont(fn, fs);
        g_active_wxmedit->SetFont(fontname, fs);

        m_RecentFonts->AddItemToHistory(fontname);
    }
}

void MadEditFrame::OnViewFontSize(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    wxString fn;
    int fs;
    g_active_wxmedit->GetFont(fn, fs);

    fs=event.GetId()-menuFontSize1 + 1;

    g_active_wxmedit->SetFont(fn, fs);
}

void MadEditFrame::OnViewSetFont(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    wxFontData data;
    wxFont font = g_active_wxmedit->GetFont();

    MadSyntax* syn = g_active_wxmedit->GetSyntax();
    MadAttributes* attr = syn->GetAttributes(aeText);
    data.SetColour(attr->color);

    if((attr->style&fsItalic) != 0) font.SetStyle(wxFONTSTYLE_ITALIC);
    if((attr->style&fsBold) != 0) font.SetWeight(wxFONTWEIGHT_BOLD);
    font.SetUnderlined((attr->style&fsUnderline) != 0);

    if(font.Ok())
    {
        data.SetInitialFont(font);
    }

    wxFontDialog dialog(this, data);
    if (dialog.ShowModal() == wxID_OK)
    {
        font = dialog.GetFontData().GetChosenFont();
        if(font.Ok())
        {
            wxString fn=FixUTF8ToWCS(font.GetFaceName());
            g_active_wxmedit->SetFont(fn, font.GetPointSize());
            m_RecentFonts->AddItemToHistory(fn);
        }

        attr->color = dialog.GetFontData().GetColour();

        MadFontStyles style = fsNone;
        if(font.GetWeight()==wxFONTWEIGHT_BOLD) style |= fsBold;
        if(font.GetStyle()==wxFONTSTYLE_ITALIC) style |= fsItalic;
        if(font.GetUnderlined()) style |= fsUnderline;
        attr->style = style;

        ApplySyntaxAttributes(syn);
        syn->SaveAttributes();
    }
}

void MadEditFrame::OnViewFixedWidthMode(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetFixedWidthMode(event.IsChecked());
}

void MadEditFrame::OnViewTabColumn(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int col=event.GetId()-menuTabColumn1 +1;
    g_active_wxmedit->SetTabColumns(col);
}

void MadEditFrame::OnViewLineSpacing(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    int ls=(event.GetId()-menuLineSpacing100)*5 + 100;
    g_active_wxmedit->SetLineSpacing(ls);
}

void MadEditFrame::OnViewNoWrap(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetWordWrapMode(wwmNoWrap);
}
void MadEditFrame::OnViewWrapByWindow(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetWordWrapMode(wwmWrapByWindow);
}
void MadEditFrame::OnViewWrapByColumn(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetWordWrapMode(wwmWrapByColumn);
}

void MadEditFrame::OnViewDisplayLineNumber(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetLineNumberVisible(event.IsChecked());
}
void MadEditFrame::OnViewDisplayBookmark(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetBookmarkVisible(event.IsChecked());
}
void MadEditFrame::OnViewShowEndOfLine(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetShowEndOfLine(event.IsChecked());
}
void MadEditFrame::OnViewShowTabChar(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetShowTabChar(event.IsChecked());
}
void MadEditFrame::OnViewShowSpaceChar(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetShowSpaceChar(event.IsChecked());
}
void MadEditFrame::OnViewMarkActiveLine(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetMarkActiveLine(event.IsChecked());
}
void MadEditFrame::OnViewMarkBracePair(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetMarkBracePair(event.IsChecked());
}

void MadEditFrame::OnViewTextMode(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetEditMode(emTextMode);
}
void MadEditFrame::OnViewColumnMode(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetEditMode(emColumnMode);
}
void MadEditFrame::OnViewHexMode(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetEditMode(emHexMode);
}


void MadEditFrame::OnToolsOptions(wxCommandEvent& event)
{
    if(g_OptionsDialog==nullptr)
        g_OptionsDialog=new WXMEditOptionsDialog(this);

    g_OptionsDialog->LoadOptions();
    if(g_OptionsDialog->ShowModal()==wxID_OK)
    {
        // save options
        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/wxMEdit"));

        bool rcm, isiot, ai, acp, msc, mscck, mmp;
        wxString mc, tc, ic;

        // General page
        m_Config->Write(wxT("UpdatesCheckingPeriod"), g_OptionsDialog->GetSelectedUpdatePeroid());

        wxm::g_check_prerelease =  g_OptionsDialog->WxCheckBoxCheckPrerelease->GetValue();
        m_Config->Write(wxT("CheckPrereleaseUpdates"), wxm::g_check_prerelease);

        m_Config->Write(wxT("Language"), g_OptionsDialog->GetSelectedLanguage());

        m_Config->Write(wxT("UseForceEncoding"), g_OptionsDialog->UseForceEncoding());
        m_Config->Write(wxT("ForceEncoding"), g_OptionsDialog->GetSelectedForceEncoding());
        m_Config->Write(wxT("DefaultEncoding"), g_OptionsDialog->GetSelectedEncoding());

        m_Config->Write(wxT("SingleInstance"), g_OptionsDialog->WxCheckBoxSingleInstance->GetValue());

        m_Config->Write(wxT("ReloadFiles"), g_OptionsDialog->WxCheckBoxReloadFiles->GetValue());

        rcm=g_OptionsDialog->WxCheckBoxRecordCaretMovements->GetValue();
        m_Config->Write(wxT("RecordCaretMovements"), rcm);

        m_Config->Write(wxT("RestoreCaretPos"), g_OptionsDialog->WxCheckBoxRestoreCaretPos->GetValue());

#ifdef __WXMSW__
        if(g_OptionsDialog->WxCheckBoxRightClickMenu->GetValue())
        {
            boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(wxm::s_wxsRegkeyClasses + wxT("*\\shell\\wxMEdit\\command")) );
            pRegKey->Create();
            wxString exepath=GetExecutablePath();
            pRegKey->SetValue(wxEmptyString, wxString(wxT('"'))+exepath+wxString(wxT("\" \"%1\"")));
        }
        else
        {
            boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(wxm::s_wxsRegkeyClasses + wxT("*\\shell\\wxMEdit")) );
            pRegKey->DeleteSelf();
        }

        bool cfg_in_usrhome = g_OptionsDialog->WxCheckBoxConfigInUserHome->GetValue();
        boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(wxm::g_wxsRegKeyWxMEdit) );
        pRegKey->Create();
        pRegKey->SetValue(wxm::g_wxsRegValConfigInHome, long(cfg_in_usrhome));
#endif

        // Edit page
        m_Config->Write(wxT("MaxLineLength"), g_OptionsDialog->WxEditMaxLineLength->GetValue());

        mc=g_OptionsDialog->WxEditMaxColumns->GetValue();
        m_Config->Write(wxT("MaxColumns"), mc);

        tc=g_OptionsDialog->WxEditTabColumns->GetValue();
        m_Config->Write(wxT("TabColumns"), tc);

        ic=g_OptionsDialog->WxEditIndentColumns->GetValue();
        m_Config->Write(wxT("IndentColumns"), ic);

        isiot=g_OptionsDialog->WxCheckBoxTabOrSpaces->GetValue();
        m_Config->Write(wxT("InsertSpacesInsteadOfTab"), isiot);

        ai=g_OptionsDialog->WxCheckBoxAutoIndent->GetValue();
        m_Config->Write(wxT("AutoIndent"), ai);

        acp=g_OptionsDialog->WxCheckBoxAutoCompletePair->GetValue();
        m_Config->Write(wxT("AutoCompletePair"), acp);

        msc=g_OptionsDialog->WxCheckBoxMouseSelectToCopy->GetValue();
        m_Config->Write(wxT("MouseSelectToCopy"), msc);

        mscck=g_OptionsDialog->WxCheckBoxWhenPressCtrlKey->GetValue();
        m_Config->Write(wxT("MouseSelectToCopyWithCtrlKey"), mscck);

        mmp=g_OptionsDialog->WxCheckBoxMiddleMouseToPaste->GetValue();
        m_Config->Write(wxT("MiddleMouseToPaste"), mmp);

        wxString behav = g_OptionsDialog->GetSelectedBehaviorCopyInHexArea();
        wxm::HexAreaClipboardCopyProxy::Instance().SelectCopierByConfig(behav);
        m_Config->Write(wxT("BehaviorCopyingInHexArea"), behav);

        wxString cond = g_OptionsDialog->GetSelectedConditionPasteAsHexInHexArea();
        wxm::HexAreaClipboardPasteProxy::Instance().SelectConditionByConfig(cond);
        m_Config->Write(wxT("ConditionPastingAsHexInHexArea"), cond);

        int count = int(m_Notebook->GetPageCount());
        for(int i=0; i<count; i++)
        {
            wxm::InFrameWXMEdit* wxmedit = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(i);

            wxmedit->SetRecordCaretMovements(rcm);
            wxmedit->SetInsertSpacesInsteadOfTab(isiot);
            wxmedit->SetAutoIndent(ai);
            wxmedit->SetAutoCompletePair(acp);
            wxmedit->SetMouseSelectToCopy(msc);
            wxmedit->SetMouseSelectToCopyWithCtrlKey(mscck);
            wxmedit->SetMiddleMouseToPaste(mmp);
            long lo;
            if (mc.ToLong(&lo)) wxmedit->SetMaxColumns(lo);
            if (tc.ToLong(&lo)) wxmedit->SetTabColumns(lo);
            if (ic.ToLong(&lo)) wxmedit->SetIndentColumns(lo);
        }

        // Print page
        bool bb;
        long ll;
        wxString ss;
        bb=g_OptionsDialog->WxCheckBoxPrintSyntax->GetValue();
        m_Config->Write(wxT("PrintSyntax"), bb);

        bb=g_OptionsDialog->WxCheckBoxPrintLineNumber->GetValue();
        m_Config->Write(wxT("PrintLineNumber"), bb);

		bb = g_OptionsDialog->WxCheckBoxPrintBookmark->GetValue();
		m_Config->Write(wxT("PrintBookmark"), bb);

        bb=g_OptionsDialog->WxCheckBoxPrintEndOfLine->GetValue();
        m_Config->Write(wxT("PrintEndOfLine"), bb);

        bb=g_OptionsDialog->WxCheckBoxPrintTabChar->GetValue();
        m_Config->Write(wxT("PrintTabChar"), bb);

        bb=g_OptionsDialog->WxCheckBoxPrintSpaceChar->GetValue();
        m_Config->Write(wxT("PrintSpaceChar"), bb);

        ll=g_OptionsDialog->WxRadioBoxPrintOffset->GetSelection();
        m_Config->Write(wxT("PrintOffsetHeader"), ll);

        bb=g_OptionsDialog->WxCheckBoxPrintPageHeader->GetValue();
        m_Config->Write(wxT("PrintPageHeader"), bb);

        ss=g_OptionsDialog->WxEditHeaderLeft->GetValue();
        m_Config->Write(wxT("PageHeaderLeft"), ss);
        ss=g_OptionsDialog->WxEditHeaderCenter->GetValue();
        m_Config->Write(wxT("PageHeaderCenter"), ss);
        ss=g_OptionsDialog->WxEditHeaderRight->GetValue();
        m_Config->Write(wxT("PageHeaderRight"), ss);

        bb=g_OptionsDialog->WxCheckBoxPrintPageFooter->GetValue();
        m_Config->Write(wxT("PrintPageFooter"), bb);

        ss=g_OptionsDialog->WxEditFooterLeft->GetValue();
        m_Config->Write(wxT("PageFooterLeft"), ss);
        ss=g_OptionsDialog->WxEditFooterCenter->GetValue();
        m_Config->Write(wxT("PageFooterCenter"), ss);
        ss=g_OptionsDialog->WxEditFooterRight->GetValue();
        m_Config->Write(wxT("PageFooterRight"), ss);


        // Keys page
        extern bool g_ResetAllKeys;
        g_ResetAllKeys=g_OptionsDialog->WxCheckBoxResetAllKeys->GetValue();

        // apply the changed keybindings
        if(g_OptionsDialog->ChangedTreeItemDataList.size()!=0)
        {
            list<TreeItemData*> ChangedMenuList;
            list<TreeItemData*>::iterator tidit=g_OptionsDialog->ChangedTreeItemDataList.begin();
            list<TreeItemData*>::iterator tiditend=g_OptionsDialog->ChangedTreeItemDataList.end();

            while(tidit!=tiditend)
            {
                TreeItemData *tid = *tidit;
                CommandData *cd = tid->cmddata;

                // remove key of changed menu
                if(cd->menu_id > 0)
                {
                    wxString menukey, newkey;

                    menukey=MadEdit::ms_KeyBindings.GetKeyByMenuText(cd->menuid_name);
                    if(!tid->keys.IsEmpty())
                    {
                        newkey=tid->keys[0];
                    }

                    if(menukey.Lower() != newkey.Lower())
                    {
                        // add it to ChangedMenuList for modifying menukey later
                        ChangedMenuList.push_back(tid);
                    }

                    MadEdit::ms_KeyBindings.RemoveByMenuId(cd->menu_id);
                }
                else if(cd->command > 0) // delete the keys of changed comnnad
                {
                    MadEdit::ms_KeyBindings.RemoveByCommand(cd->command);
                }

                ++tidit;
            }

            // add the keys of changed comnnad
            tidit=g_OptionsDialog->ChangedTreeItemDataList.begin();
            while(tidit!=tiditend)
            {
                TreeItemData *tid = *tidit;
                CommandData *cd = tid->cmddata;
                if(cd->menu_id > 0)
                {
                    size_t idx=0, count=tid->keys.GetCount();
                    for(;idx<count;idx++)
                    {
                        MadEdit::ms_KeyBindings.Add(StringToShortCut(tid->keys[idx]), idx==0, cd->menu_id, true);
                    }
                }
                else if(cd->command > 0)
                {
                    size_t idx=0, count=tid->keys.GetCount();
                    for(;idx<count;idx++)
                    {
                        MadEdit::ms_KeyBindings.Add(StringToShortCut(tid->keys[idx]), cd->command, true);
                    }
                }
                ++tidit;
            }

            // change the menukey
            tidit=ChangedMenuList.begin();
            tiditend=ChangedMenuList.end();
            while(tidit!=tiditend)
            {
                TreeItemData *tid = *tidit;
                CommandData *cd = tid->cmddata;

                wxString newkey;
                if(!tid->keys.IsEmpty())
                {
                    newkey=tid->keys[0];
                    newkey = wxString(wxT('\t'))+newkey;
                }

                // change the menu key
                wxMenuItem *mit=WxMenuBar1->FindItem(cd->menu_id);
				mit->SetItemLabel(mit->GetItemLabelText() + newkey);
#ifdef __WXMSW__
                if (g_bHasMenuIcon)
#endif
                if(cd->image_idx >= 0 && cd->kind==wxITEM_NORMAL)
                {
                    mit->SetBitmap(m_ImageList->GetBitmap(cd->image_idx));
                }

                ++tidit;
            }

            ResetAcceleratorTable();
        }

        // Misc page
        extern bool g_DoNotSaveSettings;
        g_DoNotSaveSettings=g_OptionsDialog->WxCheckBoxDoNotSaveSettings->GetValue();

        m_Config->Write(wxT("MaxSizeToLoad"), g_OptionsDialog->WxEditMaxSizeToLoad->GetValue());

        m_Config->Write(wxT("MaxTextFileSize"), g_OptionsDialog->WxEditMaxTextFileSize->GetValue());

        m_Config->Write(wxT("DateTimeFormat"), g_OptionsDialog->WxEditDateTime->GetValue());
        m_Config->Write(wxT("DateTimeInEnglish"), g_OptionsDialog->WxCheckBoxDateTimeInEnglish->GetValue());

        extern bool g_regex_dot_match_newline;
        g_regex_dot_match_newline = g_OptionsDialog->WxCheckBoxDotMatchNewline->GetValue();
        m_Config->Write(wxT("RegexDotMatchNewline"), g_regex_dot_match_newline);

        m_Config->SetPath(oldpath);
    }
}

void MadEditFrame::OnToolsHighlighting(wxCommandEvent& event)
{
    if(g_HighlightingDialog==nullptr) g_HighlightingDialog=new WXMHighlightingDialog(this);

    if (g_active_wxmedit != nullptr)
    {
        g_HighlightingDialog->m_InitSetting=g_active_wxmedit->GetSyntaxTitle();
    }
    else
    {
        g_HighlightingDialog->m_InitSetting=wxT("XXX");
    }
    int id=g_HighlightingDialog->ShowModal();
    g_HighlightingDialog->FreeSyntax(id!=wxID_OK); // press cancel to restore the syntax

    if (g_active_wxmedit != nullptr)
    {
        g_active_wxmedit->SetFocus();
    }
}

#ifdef __WXMSW__
void MadEditFrame::OnToolsFileAssociation(wxCommandEvent& event)
{
    WXMFileAssociationDialog fadialog(this);
    fadialog.ShowModal();
}
#endif

void MadEditFrame::OnToolsPurgeHistories(wxCommandEvent& event)
{
    WXMPurgeHistoriesDialog dlg(this);
    dlg.ShowModal();
}

void MadEditFrame::OnToolsToggleBOM(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->ToggleBOM();
}
void MadEditFrame::OnToolsConvertToDOS(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->ConvertNewLine(wxm::g_nl_dos);
}
void MadEditFrame::OnToolsConvertToMAC(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->ConvertNewLine(wxm::g_nl_mac);
}
void MadEditFrame::OnToolsConvertToUNIX(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->ConvertNewLine(wxm::g_nl_unix);
}

void MadEditFrame::OnToolsInsertDOS(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetInsertNewLine(wxm::g_nl_dos);
}
void MadEditFrame::OnToolsInsertMAC(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetInsertNewLine(wxm::g_nl_mac);
}
void MadEditFrame::OnToolsInsertUNIX(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    g_active_wxmedit->SetInsertNewLine(wxm::g_nl_unix);
}


void MadEditFrame::OnToolsConvertEncoding(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    if(g_ConvEncDialog==nullptr) g_ConvEncDialog=new WXMConvEncDialog(this);

    if(g_ConvEncDialog->ShowModal()==wxID_OK)
    {
        g_active_wxmedit->ConvertEncoding(g_ConvEncDialog->GetEncoding(),
                                          MadConvertEncodingFlag(g_ConvEncDialog->WxRadioBoxOption->GetSelection()));
        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/wxMEdit"));
        m_Config->Write(wxT("/wxMEdit/ConvertEncoding"), g_ConvEncDialog->GetEncoding().c_str());
        m_Config->SetPath(oldpath);

        wxString str = wxString(wxT('[')) + g_active_wxmedit->GetEncodingName() + wxT("] ")+
                      wxGetTranslation(g_active_wxmedit->GetEncodingDescription().c_str());
        m_RecentEncodings->AddItemToHistory(str);

        int size;
        g_active_wxmedit->GetFont(str, size);
        m_RecentFonts->AddItemToHistory(str);
    }
}

void MadEditFrame::OnToolsSimp2TradChinese(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->ConvertChinese(cefSC2TC);
}

void MadEditFrame::OnToolsTrad2SimpChinese(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->ConvertChinese(cefTC2SC);
}

void MadEditFrame::OnToolsKanji2TradChinese(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->ConvertChinese(cefJK2TC);
}

void MadEditFrame::OnToolsKanji2SimpChinese(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->ConvertChinese(cefJK2SC);
}

void MadEditFrame::OnToolsChinese2Kanji(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;
    g_active_wxmedit->ConvertChinese(cefC2JK);
}

void MadEditFrame::OnToolsSimp2TradClipboard(wxCommandEvent& event)
{
    ConvertChineseInClipboard(ccfSimp2Trad);
}

void MadEditFrame::OnToolsTrad2SimpClipboard(wxCommandEvent& event)
{
    ConvertChineseInClipboard(ccfTrad2Simp);
}

void MadEditFrame::OnToolsKanji2TradClipboard(wxCommandEvent& event)
{
    ConvertChineseInClipboard(ccfKanji2Trad);
}

void MadEditFrame::OnToolsKanji2SimpClipboard(wxCommandEvent& event)
{
    ConvertChineseInClipboard(ccfKanji2Simp);
}

void MadEditFrame::OnToolsChinese2KanjiClipboard(wxCommandEvent& event)
{
    ConvertChineseInClipboard(ccfChinese2Kanji);
}

void MadEditFrame::OnToolsWordCount(wxCommandEvent& event)
{
    if (g_active_wxmedit == nullptr)
        return;

    WXMWordCountDialog dialog(this);
    dialog.ShowModal();
}

void MadEditFrame::OnWindowToggleWindow(wxCommandEvent& event)
{
    int count=int(m_Notebook->GetPageCount());
    if(count<=1) return;

    int selid=m_Notebook->GetSelection();

    g_CheckModTimeForReload=false;

    if( g_PrevPageID >= 0 &&
        g_PrevPageID <  count &&
        g_PrevPageID != selid)
    {
        m_Notebook->SetSelection(g_PrevPageID);
        g_PrevPageID=selid;
    }
    else
    {
        g_PrevPageID=selid;
        m_Notebook->AdvanceSelection(true);
    }

    wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
    if (sel != g_active_wxmedit)
    {
        wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
        event.SetSelection(m_Notebook->GetSelection());
        event.SetOldSelection(g_PrevPageID);
        event.SetEventObject(this);
        OnNotebookPageChanged(event);
    }

    g_CheckModTimeForReload=true;
    g_active_wxmedit->ReloadByModificationTime();
}

void MadEditFrame::OnWindowPreviousWindow(wxCommandEvent& event)
{
    if(m_Notebook->GetPageCount()<=1) return;

    g_PrevPageID=m_Notebook->GetSelection();
    g_CheckModTimeForReload=false;
    m_Notebook->AdvanceSelection(false);

    wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
    if (sel != g_active_wxmedit)
    {
        wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
        event.SetSelection(m_Notebook->GetSelection());
        event.SetOldSelection(g_PrevPageID);
        event.SetEventObject(this);
        OnNotebookPageChanged(event);
    }

    g_CheckModTimeForReload=true;
    g_active_wxmedit->ReloadByModificationTime();
}
void MadEditFrame::OnWindowNextWindow(wxCommandEvent& event)
{
    if(m_Notebook->GetPageCount()<=1) return;

    g_PrevPageID=m_Notebook->GetSelection();
    g_CheckModTimeForReload=false;
    m_Notebook->AdvanceSelection(true);

    wxm::InFrameWXMEdit* sel = (wxm::InFrameWXMEdit*)m_Notebook->GetPage(m_Notebook->GetSelection());
    if (sel != g_active_wxmedit)
    {
        wxAuiNotebookEvent event(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, m_Notebook->GetId());
        event.SetSelection(m_Notebook->GetSelection());
        event.SetOldSelection(g_PrevPageID);
        event.SetEventObject(this);
        OnNotebookPageChanged(event);
    }

    g_CheckModTimeForReload=true;
    g_active_wxmedit->ReloadByModificationTime();
}

void MadEditFrame::OnHelpCheckUpdates(wxCommandEvent& event)
{
    wxm::ManualCheckUpdates();
}

const wxString& GetCredits()
{
    static wxString credits;
    if (credits.empty())
    {
        BOOST_FOREACH(const wxm::CreditsList::value_type& v, wxm::s_wxMEdit_Credits)
            credits += wxString::Format(wxT("  %-18s %s\n"), v.first.c_str(), wxGetTranslation(v.second.c_str()));
    }

    return credits;
}

void MadEditFrame::OnHelpAbout(wxCommandEvent& event)
{
    wxm::g_wxMEdit_About_URL = wxm::g_wxMEdit_Homepage_URL;
    WXMEditAboutDialog dlg(this);
    dlg.TxtAbout->AppendText(wxString(wxT("wxMEdit v")) + wxT(WXMEDIT_VERSION) + wxT("\n\n") +
                            wxm::g_wxMEdit_Homepage_URL + wxT("\n\n") +
                            _("Press OK to visit our HomePage."));

    dlg.TxtLicense->AppendText( wxString(_("wxMEdit, a cross-platform Text/Hex Editor")) + wxT("\n\n") +
                            wxm::s_wxMEdit_License + wxT("\n\n") +
                            wxm::g_wxMEdit_License_URL + wxT("\n\n") +
                            _("Press OK to view the license online version.\n"));
    dlg.TxtLicense->SetInsertionPoint(0L);

    dlg.TxtCredits->AppendText(GetCredits());
    dlg.TxtCredits->SetInsertionPoint(0L);

    if(dlg.ShowModal() == wxID_OK && !wxm::g_wxMEdit_About_URL.empty())
        wxm::OpenURL(wxm::g_wxMEdit_About_URL);
}

void MadEditFrame::PurgeRecentFiles()
{
    int n = (int) m_RecentFiles->GetCount();
    for(int i=n-1; i>=0; --i)
        m_RecentFiles->RemoveFileFromHistory((size_t)i);
}

void MadEditFrame::PurgeRecentFonts()
{
    int n = (int) m_RecentFonts->GetCount();
    for(int i=n-1; i>=1; --i)
        m_RecentFonts->RemoveFileFromHistory((size_t)i);
}

void MadEditFrame::PurgeRecentEncodings()
{
    int n = (int) m_RecentEncodings->GetCount();
    for(int i=n-1; i>=1; --i)
        m_RecentEncodings->RemoveFileFromHistory((size_t)i);
}

void MadEditFrame::OnSize(wxSizeEvent& event)
{
    if (!IsMaximized())
        m_nml_rect = GetRect();
    event.Skip(true);
}

wxRect MadEditFrame::GetNormalRect()
{
    if (IsMaximized())
        return m_nml_rect;
    return GetRect();
}
