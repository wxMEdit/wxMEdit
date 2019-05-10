///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxmedit_basic.cpp
// Description: Basic Functions of wxMEdit
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2007-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxmedit.h"
#include "../xm/cxx11.h"
#include "../wxm/edit/simple.h"
#include "../xm/encoding/unicode.h"
#include "../wxm/utils.h"
#include "../mad_utils.h"
#include "../xm/uutils.h"

#ifdef __WXGTK__
#   include "clipbrd_gtk.h"
#else
#   include <wx/clipbrd.h>
#endif

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/filename.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace algo = boost::algorithm;

extern const ucs4_t HexHeader[];

//==============================================================================

const std::vector<wxPoint> CR_Points = boost::assign::list_of
    ( wxPoint( 3,  5) )
    ( wxPoint( 1,  9) )
    ( wxPoint( 9,  9) )
    ( wxPoint( 1,  9) )
    ( wxPoint( 3, 13) )
    ( wxPoint(10, 18) ); // max value

const std::vector<wxPoint> LF_Points = boost::assign::list_of
    ( wxPoint(1,  7) )
    ( wxPoint(3,  9) )
    ( wxPoint(3,  1) )
    ( wxPoint(3,  9) )
    ( wxPoint(5,  7) )
    ( wxPoint(7, 12) ); // max value

const std::vector<wxPoint> CRLF_Points = boost::assign::list_of
    ( wxPoint( 3,  5) )
    ( wxPoint( 1,  9) )
    ( wxPoint( 9,  9) )
    ( wxPoint( 9,  1) )
    ( wxPoint( 9,  9) )
    ( wxPoint( 1,  9) )
    ( wxPoint( 3, 13) )
    ( wxPoint(10, 18) ); // max value

void MadEdit::SetSyntax(const wxString &title, bool manual)
{
    if (m_Syntax->GetTitle() == title)
        return;

    delete m_Syntax;
    m_Syntax = MadSyntax::GetSyntaxByTitle(title);
    m_Syntax->SetEncoding(m_Encoding);
    m_Syntax->InitNextWord1(m_Lines, m_WordBuffer, m_WidthBuffer,
        m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), m_TextFont->GetFamily());

    m_Lines->m_Syntax=m_Syntax;
    m_Lines->SetManual(manual);

    if (m_LoadingFile)
        return;

    if (!IsTextFile())
        return;

    // data is text
    ReformatAll();

    DoStatusChanged();
    DoSelectionChanged();

    m_RepaintAll = true;
    Refresh(false);
}

void MadEdit::ApplySyntaxAttributes(MadSyntax *syn, bool matchTitle)
{
    if(!matchTitle || syn->GetTitle()==m_Syntax->GetTitle())
    {
        m_Syntax->AssignAttributes(syn);
        if(m_EditMode==emHexMode && m_HexDigitBitmap)
        {
            delete m_HexDigitBitmap;
            m_HexDigitBitmap=nullptr;
        }
        m_RepaintAll = true;
        Refresh(false);
    }
}

void MadEdit::SetEncoding(const std::wstring & encname)
{
    if (algo::iequals(encname, m_Encoding->GetName()))
        return;

    //delete m_Encoding;
    m_Encoding = xm::EncodingManager::Instance().GetEncoding(encname);
    m_Lines->SetEncoding(m_Encoding);
    m_Syntax->SetEncoding(m_Encoding);

    wxString fontname;
    m_Config->Read(wxString(wxT("/Fonts/"))+m_Encoding->GetName().c_str(), &fontname, m_Encoding->GetFontName().c_str());

    bool oldlf=m_LoadingFile;
    m_LoadingFile=true;
    SetTextFont(fontname, m_TextFont->GetPointSize(), false);
    m_LoadingFile=oldlf;

    if (m_LoadingFile)
        return;

    if(IsTextFile())    // data is text
    {
        ReformatAll();
    }
    else // hex data
    {
        AppearCaret();
        UpdateScrollBarPos();

        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    DoStatusChanged();
    DoSelectionChanged();

    m_RepaintAll = true;
    Refresh(false);
}


void MadEdit::SetRecordCaretMovements(bool value)
{
    if(value!=m_RecordCaretMovements)
    {
        m_RecordCaretMovements=value;

        m_cfg_writer->Record(wxT("/wxMEdit/RecordCaretMovements"), value);
    }
}


void MadEdit::CalcEOLMarkPoints(std::vector<wxPoint>& dest, const std::vector<wxPoint>& src, const wxSize& charsz)
{
    const int maxX = src.rbegin()->x;
    const int maxY = src.rbegin()->y;
    dest.clear();
    const size_t cnt = src.size() - 1;
    for (size_t i = 0; i< cnt; ++i)
    {
        int x = (int)round(1.0 * charsz.x * src[i].x / maxX);
        int y = (int)round(1.0 * charsz.y * src[i].y / maxY);
        dest.push_back(wxPoint(x, y));
    }
}

void MadEdit::CalcEOFMarkPoints(const wxSize& charsz)
{
    const int x0 = charsz.x / 16 + 1;
    const int x1 = charsz.x / 2;
    const int x2 = charsz.x * 15 / 16 - 1;
    const int y0 = charsz.y / 16 + 1;
    const int y1 = charsz.y / 2;
    const int y2 = charsz.y * 15 / 16 - 1;
    m_eof_points.clear();
    boost::assign::push_back(m_eof_points)
        ( wxPoint(x2, y0) )
        ( wxPoint(x0, y2) )
        ( wxPoint(x2, y2) )
        ( wxPoint(x2, y1) )
        ( wxPoint(x1, y2) );
}

void MadEdit::CalcSpaceMarkPoints(const wxSize& charsz)
{
    const int ty = charsz.y / 7 + 1;
    const int  y = charsz.y * 3 / 5 + 1;
    const int  x = charsz.x / 4;
    m_space_points.clear();
    boost::assign::push_back(m_space_points)
        ( wxPoint(x, y) )
        ( wxPoint(3 * x, y) )
        ( wxPoint(charsz.x / 2, y - ty) )
        ( wxPoint(x, y) );
}

void MadEdit::CalcTabMarkPoints(std::vector<wxPoint>& dest, const wxSize& charsz)
{
    const int ty = charsz.y / 7 + 1;
    const int  y = charsz.y * 3 / 5 + 1;

    const int tx = (ty > charsz.x / 2) ? (charsz.x / 2) : ty;
    const int  x = 2;
    const int dx = charsz.x - 3;

    dest.clear();
    boost::assign::push_back(dest)
        ( wxPoint(x, y - ty * 2 / 3) )
        ( wxPoint(x, y + ty * 2 / 3) )
        ( wxPoint(x, y) )
        ( wxPoint(x + dx, y) )
        ( wxPoint(x + dx - tx, y - ty) )
        ( wxPoint(x + dx, y) )
        ( wxPoint(x + dx - tx, y + ty) )
        ( wxPoint(x + dx, y) );
}

void MadEdit::SetTextFont(const wxString &name, int size, bool forceReset)
{
    if(size < 1) size = 1;

    if(forceReset || size!=m_TextFont->GetPointSize() || name!=m_TextFont->GetFaceName())
    {
        m_TextFont=wxTheFontList->FindOrCreateFont(size, //font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined(), name);
            wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, name);

        memset(m_TextFontWidths, 0, sizeof(m_TextFontWidths));
        m_TextFontWidths[0] = FontWidthManager::GetFontWidths(0, name, size, this);

        m_cfg_writer->Record(wxString(wxT("/Fonts/"))+m_Encoding->GetName().c_str(), name);
        m_cfg_writer->Record(wxT("/wxMEdit/TextFontSize"), size);


        if(m_EditMode==emHexMode)
        {
            m_DoRecountLineWidth=true;
        }
        else
        {
            MadEditSuperClass::SetFont(*m_TextFont);
            m_TextFontHeight = GetCharHeight();

            bool ofwm = m_FixedWidthMode;
            m_FixedWidthMode = false; // avoid effecting on GetUCharWidth();

            m_TextFontSpaceCharWidth=GetUCharWidth(0x20);

            // DO NOT use GetCharWidth(), for it takes '0' under Windows indeed, 'g' under other OSes
            m_TextFontAveCharWidth = std::max(GetUCharWidth('g'), m_TextFontSpaceCharWidth);

            m_TextFontMaxDigitWidth = GetUCharWidth('0');
            for (wxChar wc=wxT('1'); wc<=wxT('9'); ++wc)
                m_TextFontMaxDigitWidth = std::max(m_TextFontMaxDigitWidth, GetUCharWidth(wc));

            m_TextFontAveCharWidth = std::max(m_TextFontAveCharWidth, m_TextFontMaxDigitWidth);

            if (ofwm) //(m_FixedWidthMode)
            {
                if (!HexPrinting())
                {
                    m_TextFontAveCharWidth = std::max(m_TextFontAveCharWidth, GetUCharWidth('W'));
                }
                else
                {
                    for (wxChar wc=wxT('A'); wc<=wxT('F'); ++wc)
                        m_TextFontAveCharWidth = std::max(m_TextFontAveCharWidth, GetUCharWidth(wc));
                }

                m_TextFontMaxDigitWidth = m_TextFontAveCharWidth;
                m_TextFontSpaceCharWidth = m_TextFontAveCharWidth;
            }

            m_FixedWidthMode = ofwm;


            m_LeftMarginWidth = std::max(1, (m_TextFontAveCharWidth / 2) - 1);
            m_RightMarginWidth = m_TextFontAveCharWidth * 2;

            m_Syntax->InitNextWord1(m_Lines, m_WordBuffer, m_WidthBuffer,
                name, size, m_TextFont->GetFamily());


            const wxSize charsz(m_TextFontAveCharWidth, m_TextFontHeight);
            const wxSize spacesz(m_TextFontSpaceCharWidth, m_TextFontHeight);

            CalcSpaceMarkPoints(spacesz);
            CalcEOFMarkPoints(charsz);
            CalcEOLMarkPoints(m_cr_points, CR_Points, charsz);
            CalcEOLMarkPoints(m_lf_points, LF_Points, charsz);
            CalcEOLMarkPoints(m_crlf_points, CRLF_Points, charsz);

            UpdateAppearance();
            m_RepaintAll=true;

            SetCaretType(m_CaretType);

            if(!m_LoadingFile)
            {
                RecountLineWidth(true);

                UpdateScrollBarPos();
                Refresh(false);

                DoStatusChanged();
            }
            else
            {
                m_DoRecountLineWidth=true;
            }
        }
    }
}

void MadEdit::SetHexFont(const wxString &name, int size, bool forceReset)
{
    if(size < 1) size = 1;

    if(forceReset || size!=m_HexFont->GetPointSize() || name!=m_HexFont->GetFaceName())
    {
        m_HexFont=wxTheFontList->FindOrCreateFont(size, //font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined(), name);
            wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, name);

        memset(m_HexFontWidths, 0, sizeof(m_HexFontWidths));
        m_HexFontWidths[0]=FontWidthManager::GetFontWidths(0, name, size, this);

        m_cfg_writer->Record(wxString(wxT("/wxMEdit/HexFontName")), name);
        m_cfg_writer->Record(wxT("/wxMEdit/HexFontSize"), size);

        if(m_HexDigitBitmap)
        {
            delete m_HexDigitBitmap;
            m_HexDigitBitmap=nullptr;
        }

        if(m_EditMode==emHexMode)
        {
            MadEditSuperClass::SetFont(*m_HexFont);
            m_HexFontHeight=GetCharHeight();

            GetHexUCharWidth(0x20);

            int w;
            m_HexFontMaxDigitWidth=GetHexUCharWidth('0');
            for(wxChar wc=wxT('1');wc<=wxT('9');wc++)
            {
                w=GetHexUCharWidth(wc);
                if(w>m_HexFontMaxDigitWidth)
                    m_HexFontMaxDigitWidth=w;
            }
            for(wxChar wc=wxT('A');wc<=wxT('F');wc++)
            {
                w=GetHexUCharWidth(wc);
                if(w>m_HexFontMaxDigitWidth)
                    m_HexFontMaxDigitWidth=w;
            }

            UpdateAppearance();
            m_RepaintAll=true;

            SetCaretType(ctBlock);

            if(!m_LoadingFile)
            {
                UpdateScrollBarPos();

                if(!m_CaretAtHexArea)
                {
                    int crow = int(m_CaretPos.pos >> 4);
                    if(crow >= m_TopRow && crow < m_TopRow + m_HexRowCount)
                    {
                        UpdateTextAreaXPos();
                        m_LastTextAreaXPos = m_TextAreaXPos;
                    }
                }

                Refresh(false);
                DoStatusChanged();
            }
        }
    }
}

void MadEdit::SetFixedWidthMode(bool mode)
{
    if(mode!=m_FixedWidthMode)
    {
        m_FixedWidthMode=mode;
        m_cfg_writer->Record(wxT("/wxMEdit/FixedWidthMode"), mode);

        SetTextFont(m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), true);
    }
}

void MadEdit::SetLineSpacing(int percent)
{
    if(percent<100) percent=100;
    else if(percent>500) percent=500;

    if(percent != m_LineSpacing)
    {
        m_LineSpacing=percent;
        m_cfg_writer->Record(wxT("/wxMEdit/LineSpacing"), percent);

        if(m_EditMode!=emHexMode)
        {
            m_RepaintAll=true;
            UpdateAppearance();
            UpdateScrollBarPos();
            Refresh(false);
        }
    }
}

void MadEdit::HexModeToTextMode(MadEditMode mode)
{
    if (!m_LoadingFile)
    {
        if (!IsTextFile())
        {
            if (ManuallyCancelHexToText())
                return;

            m_DrawingXPos = 0;
            m_TopRow = m_TextTopRow;
            m_EditMode = mode;
            m_RepaintAll = true;

            SetTextFont(m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), true);
            UpdateAppearance();

            ReformatAll();
        }
        else
        {
            m_DrawingXPos = 0;
            m_TopRow = m_TextTopRow;
            m_EditMode = mode;
            m_RepaintAll = true;

            SetTextFont(m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), true);
            UpdateAppearance();

            if(m_DoRecountLineWidth)
            {
                RecountLineWidth(true);
            }
            UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

            AppearCaret();
            UpdateScrollBarPos();

            m_LastCaretXPos = m_CaretPos.xpos;

            if(m_Selection)
            {
                UpdateSelectionPos();
            }
        }

        m_DoRecountLineWidth = false;

        // reset m_ValidPos
        m_ValidPos_iter = m_CaretPos.iter;
        m_ValidPos_lineid = m_CaretPos.lineid;
        m_ValidPos_rowid = m_CaretPos.rowid - m_CaretPos.subrowid;
        m_ValidPos_pos = m_CaretPos.pos - m_CaretPos.linepos;

        DoStatusChanged();
        DoSelectionChanged();

        SetInsertMode(m_InsertMode);
    }
    else
    {
        m_DrawingXPos = 0;
        m_TopRow = m_TextTopRow;
        m_EditMode = mode;
        m_RepaintAll = true;

        UpdateAppearance();
        SetInsertMode(m_InsertMode);
    }
}

void MadEdit::SetEditMode(MadEditMode mode)
{
    if (m_EditMode == mode)
        return;

    bool selchange = false;

    switch(mode)
    {
    case emTextMode:
        if(m_EditMode == emColumnMode)
        {
            if(m_CaretPos.extraspaces)
            {
                m_CaretPos.xpos -= int(m_CaretPos.extraspaces * GetSpaceCharFontWidth());
                m_CaretPos.extraspaces = 0;

                m_LastCaretXPos = m_CaretPos.xpos;

                AppearCaret();
                UpdateScrollBarPos();
                selchange = true;
            }

            if(m_Selection)
            {
                if(m_SelectionPos1.extraspaces)
                {
                    m_SelectionPos1.xpos -= int(m_SelectionPos1.extraspaces * GetSpaceCharFontWidth());
                    m_SelectionPos1.extraspaces = 0;
                }
                if(m_SelectionPos2.extraspaces)
                {
                    m_SelectionPos2.xpos -= int(m_SelectionPos2.extraspaces * GetSpaceCharFontWidth());
                    m_SelectionPos2.extraspaces = 0;
                }

                m_RepaintSelection = true;
                selchange = true;
            }

            m_EditMode = emTextMode;

            if(selchange == true)
                DoSelectionChanged();
            DoStatusChanged();

            SetInsertMode(m_InsertMode);
        }
        else                      //HexMode
        {
            HexModeToTextMode(mode);
        }

        break;

    case emColumnMode:
        if(m_EditMode == emTextMode)
        {
            UpdateScrollBarPos();//update m_MaxColumnModeWidth;

            if(m_Selection)
            {
                if(m_SelectionPos1.xpos < m_SelectionPos2.xpos)
                {
                    m_SelLeftXPos = m_SelectionPos1.xpos;
                    m_SelRightXPos = m_SelectionPos2.xpos;
                }
                else
                {
                    m_SelLeftXPos = m_SelectionPos2.xpos;
                    m_SelRightXPos = m_SelectionPos1.xpos;
                }

                m_RepaintSelection = true;
                selchange = true;
            }

            m_EditMode = emColumnMode;

            if(selchange == true)
                DoSelectionChanged();
            DoStatusChanged();

            SetInsertMode(m_InsertMode);
        }
        else                      //HexMode
        {
            HexModeToTextMode(mode);
        }

        break;

    case emHexMode:
        GetSize(&m_OldWidth, &m_OldHeight);

        m_EditMode = emHexMode;
        m_TextTopRow = m_TopRow;
        m_DrawingXPos = 0;

        m_CaretAtHexArea = true;
        m_CaretAtHalfByte = false;

        SetHexFont(m_HexFont->GetFaceName(), m_HexFont->GetPointSize(), true);
        UpdateAppearance();

        m_RepaintAll = true;
        SetCaretType(ctBlock);

        if(m_LoadingFile == false)
        {
            m_TopRow = int(m_CaretPos.pos >> 4);
            if(m_TopRow >= (m_VisibleRowCount >> 1))
            {
                m_TopRow -= (m_VisibleRowCount >> 1);
                int rows = int(m_Lines->m_Size >> 4);

                if((((int)m_Lines->m_Size) & 0xF) != 0)
                    ++rows;

                if(rows - m_TopRow < m_CompleteRowCount)
                {
                    if(rows > m_CompleteRowCount)
                        m_TopRow = rows - m_CompleteRowCount;
                    else
                        m_TopRow = 0;
                }
            }
            else
                m_TopRow = 0;

            m_CaretPos.extraspaces = 0;
            if(m_Selection)
            {
                m_SelectionPos1.extraspaces = 0;
                m_SelectionPos2.extraspaces = 0;
            }

            AppearCaret();
            UpdateScrollBarPos();

            DoSelectionChanged();
            DoStatusChanged();
        }

        //if(Focused()) ShowCaret();
        break;

    }

    Refresh(false);
}

void MadEdit::HideScrollBars()
{
    m_VScrollBar->Show(false);
    m_HScrollBar->Show(false);
}

void MadEdit::SetTabColumns(long value)
{
    if(m_TabColumns != value && value >= 1 && value <= 80)
    {
        m_TabColumns = value;
        m_cfg_writer->Record(wxT("/wxMEdit/TabColumns"), value);

        if(m_HasTab)
        {
            UpdateAppearance();
            RecountLineWidth(true);
            UpdateScrollBarPos();
            m_RepaintAll = true;
            Refresh(false);
        }
    }
}

void MadEdit::SetInsertSpacesInsteadOfTab(bool value)
{
    if(value!=m_InsertSpacesInsteadOfTab)
    {
        m_InsertSpacesInsteadOfTab=value;
        m_cfg_writer->Record(wxT("/wxMEdit/InsertSpacesInsteadOfTab"), value);
    }
}

void MadEdit::SetShowEndOfLine(bool value)
{
    if(value!=m_ShowEndOfLine)
    {
        m_ShowEndOfLine=value;
        m_cfg_writer->Record(wxT("/wxMEdit/ShowEndOfLine"), value);

        m_RepaintAll=true;
        Refresh(false);
    }
}
void MadEdit::SetShowTabChar(bool value)
{
    if(value!=m_ShowTabChar)
    {
        m_ShowTabChar=value;
        m_cfg_writer->Record(wxT("/wxMEdit/ShowTabChar"), value);

        if(m_HasTab)
        {
            m_RepaintAll=true;
            Refresh(false);
        }
    }
}
void MadEdit::SetShowSpaceChar(bool value)
{
    if(value!=m_ShowSpaceChar)
    {
        m_ShowSpaceChar=value;
        m_cfg_writer->Record(wxT("/wxMEdit/ShowSpaceChar"), value);

        m_RepaintAll=true;
        Refresh(false);
    }
}
void MadEdit::SetMarkActiveLine(bool value)
{
    if(value!=m_MarkActiveLine)
    {
        m_MarkActiveLine=value;
        m_cfg_writer->Record(wxT("/wxMEdit/MarkActiveLine"), value);

        m_RepaintAll=true;
        Refresh(false);
    }
}

void MadEdit::SetMarkBracePair(bool value)
{
    if(value!=m_MarkBracePair)
    {
        m_MarkBracePair=value;
        m_cfg_writer->Record(wxT("/wxMEdit/MarkBracePair"), value);

        m_RepaintAll=true;
        Refresh(false);
    }
}

void MadEdit::SetMaxColumns(long cols)
{
    if(m_MaxColumns!=cols)
    {
        m_MaxColumns = cols;
        m_cfg_writer->Record(wxT("/wxMEdit/MaxColumns"), cols);

        if (GetWordWrapMode() == wwmWrapByColumn)
        {
            if(m_EditMode != emHexMode)
            {
                //UpdateAppearance();
                RecountLineWidth(true);
                UpdateScrollBarPos();
                m_RepaintAll = true;
                Refresh(false);
            }
            else
            {
                m_DoRecountLineWidth = true;
            }
        }

    }
}

void MadEdit::SetInsertMode(bool mode)
{
    m_InsertMode=mode;
    if(mode==true)
    {
        SetCaretType(ctVerticalLine);
    }
    else
    {
        SetCaretType(ctBlock);
    }
    DoStatusChanged();
}


void MadEdit::GetCaretPosition(int &line, int &subrow, wxFileOffset &column)
{
    line = m_CaretPos.lineid;
    subrow=m_CaretPos.subrowid;

    if(m_EditMode==emHexMode)
    {
        column=m_CaretPos.linepos;
    }
    else if(m_FixedWidthMode || GetUCharWidth('W')==GetUCharWidth('i'))
    {
        column=((m_CaretPos.xpos+m_TextFontAveCharWidth-1)/m_TextFontAveCharWidth);
    }
    else
    {
        column=m_CaretRowUCharPos+m_CaretPos.extraspaces;
    }
}

wxFileOffset MadEdit::GetSelectionSize()
{
    if(!m_Selection) return 0;

    if(m_EditMode==emColumnMode)
    {
        return GetColumnSelection(nullptr);
    }

    return m_SelectionEnd->pos - m_SelectionBegin->pos;
}

void MadEdit::GetSelectionLineId(int &beginline, int &endline)
{
    if(!m_Selection)
    {
        beginline = endline = -1;
    }
    else
    {
        beginline = m_SelectionBegin->lineid;
        endline = m_SelectionEnd->lineid;
    }
}


void MadEdit::GetSelText(wxString &ws)
{
    if(!m_Selection)
        return;

    if(m_EditMode == emColumnMode)
    {
        GetColumnSelection(&ws);
    }
    else
    {
        wxFileOffset pos = m_SelectionBegin->pos;
        xm::UCQueue ucqueue;

        MadLineIterator lit = m_SelectionBegin->iter;
        m_Lines->InitNextUChar(lit, m_SelectionBegin->linepos);
        do
        {
            if(!m_Lines->NextUChar(ucqueue))
            {
                ++lit;
                m_Lines->InitNextUChar(lit, 0);
                m_Lines->NextUChar(ucqueue);
            }

            wxm::WxStrAppendUCS4(ws, ucqueue.back().ucs4());

            pos += ucqueue.back().nbytes();
        }
        while(pos < m_SelectionEnd->pos);
    }
}

void MadEdit::GetText(wxString &ws, bool ignoreBOM)
{
    wxFileOffset pos = 0;
    xm::UCQueue ucqueue;

    MadLineIterator lit = m_Lines->m_LineList.begin();

    if(ignoreBOM)                 // if first line has BOM, we will ignore it
    {
        pos = lit->m_RowIndices.front().m_Start;
    }

    if(pos >= m_Lines->m_Size)
        return;

    m_Lines->InitNextUChar(lit, pos);
    do
    {
        if(!m_Lines->NextUChar(ucqueue))
        {
            ++lit;
            m_Lines->InitNextUChar(lit, 0);
            m_Lines->NextUChar(ucqueue);
        }

        wxm::WxStrAppendUCS4(ws, ucqueue.back().ucs4());

        pos += ucqueue.back().nbytes();
    }
    while(pos < m_Lines->m_Size);
}

void MadEdit::SetText(const wxString &ws)
{
    MadLineIterator lit;
    size_t size = ws.Len();

    if (!AdjustStringLength(ws, size))
        return;

    MadUndo *undo=nullptr;

    if(m_Lines->m_Size)
    {
        if(size == 0)               // delete
        {
            MadDeleteUndoData *dudata = new MadDeleteUndoData;

            dudata->m_Pos = 0;
            dudata->m_Size = m_Lines->m_Size;

            lit = DeleteInsertData(0, dudata->m_Size, &dudata->m_Data, 0, nullptr);

            undo = m_UndoBuffer->Add();
            undo->m_Undos.push_back(dudata);
        }
        else                        // overwrite
        {
            MadOverwriteUndoData *oudata = new MadOverwriteUndoData();

            oudata->m_Pos = 0;
            oudata->m_DelSize = m_Lines->m_Size;

            MadBlock blk(m_Lines->m_MemData, -1, 0);

            vector<ucs4_t> ucs;
            TranslateText(ws.c_str(), size, ucs, true);
            UCStoBlock(&ucs[0], ucs.size(), blk);

            oudata->m_InsSize = blk.m_Size;
            oudata->m_InsData.push_back(blk);

            lit = DeleteInsertData(0, oudata->m_DelSize, &oudata->m_DelData,
                                      oudata->m_InsSize, &oudata->m_InsData);

            undo = m_UndoBuffer->Add();
            undo->m_Undos.push_back(oudata);
        }
    }
    else                          //insert
    {
        if(size == 0)
            return;

        MadBlock blk(m_Lines->m_MemData, -1, 0);

        vector<ucs4_t> ucs;
        TranslateText(ws.c_str(), size, ucs, true);
        UCStoBlock(&ucs[0], ucs.size(), blk);

        MadInsertUndoData *insud = new MadInsertUndoData;
        insud->m_Pos = 0;
        insud->m_Size = blk.m_Size;
        insud->m_Data.push_back(blk);

        lit = DeleteInsertData(0, 0, nullptr, insud->m_Size, &insud->m_Data);

        undo = m_UndoBuffer->Add();
        undo->m_Undos.push_back(insud);
    }

    undo->m_CaretPosBefore=m_CaretPos.pos;

    m_Modified = true;
    m_Selection = false;
    m_RepaintAll = true;
    Refresh(false);

    if(IsTextFile())
    {
        m_Lines->Reformat(lit, lit);

        m_CaretPos.Reset(m_Lines->m_LineList.begin());
        undo->m_CaretPosAfter=m_CaretPos.pos;
        UpdateCaret(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

        AppearCaret();
        UpdateScrollBarPos();

        if(m_EditMode == emHexMode)
        {
            if(!m_CaretAtHexArea)
            {
                UpdateTextAreaXPos();
                m_LastTextAreaXPos = m_TextAreaXPos;
            }
        }
    }
    else
    {
        m_CaretPos.pos = 0;
        m_CaretPos.linepos = 0;
        undo->m_CaretPosAfter=0;

        AppearCaret();
        UpdateScrollBarPos();

        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    DoStatusChanged();
    DoSelectionChanged();
}

bool MadEdit::GetLine(wxString &ws, int line, size_t maxlen, bool ignoreBOM)
{
    wxFileOffset pos = 0;
    xm::UCQueue ucqueue;

    MadLineIterator lit = m_Lines->m_LineList.begin();

    if(line==0) // if first line has BOM, we will ignore it
    {
        if(ignoreBOM) pos = lit->m_RowIndices.front().m_Start;
    }
    else
    {
        if(line >= int(m_Lines->m_LineCount)) return true;

        for(int i=0; i<line; ++i)
        {
            ++lit;
        }
    }

    if(pos >= m_Lines->m_Size)
        return true;

    if(maxlen==0) maxlen=size_t(-1);

    m_Lines->InitNextUChar(lit, pos);
    do
    {
        if(!m_Lines->NextUChar(ucqueue))
        {
            return true;
        }

        ucs4_t uc=ucqueue.back().ucs4();
        if(uc==0x0D || uc==0x0A)
            return true;

        wxm::WxStrAppendUCS4(ws, uc);
    }
    while(ws.Len() < maxlen);

    if(m_Lines->NextUChar(ucqueue)) return false;
    return true;
}

int MadEdit::GetLineByPos(const wxFileOffset &pos)
{
    wxFileOffset p=pos;
    if(p<0) p=0;
    else if(p > m_Lines->m_Size) p=m_Lines->m_Size;

    MadLineIterator lit;
    int rid;
    m_UpdateValidPos=-1;
    int line=GetLineByPos(lit, p, rid);
    m_UpdateValidPos=0;
    return line;
}


void MadEdit::SelectAll()
{
    if(m_Lines->m_Size)
    {
        m_SelectionPos1.Reset(m_Lines->m_LineList.begin());
        m_SelectionBegin = &m_SelectionPos1;

        m_SelectionPos2.iter = m_Lines->m_LineList.end();
        --m_SelectionPos2.iter;
        m_SelectionPos2.pos = m_Lines->m_Size;
        m_SelectionPos2.linepos = m_SelectionPos2.iter->m_Size;
        m_SelectionPos2.lineid = int(m_Lines->m_LineCount - 1);
        m_SelectionPos2.rowid = int(m_Lines->m_RowCount - 1);
        m_SelectionPos2.subrowid = int(m_SelectionPos2.iter->RowCount()-1);
        m_SelectionPos2.xpos=0;

        if(m_SelectionPos2.linepos > 0)//update xpos
        {
            xm::UCQueue ucharQueue;
            vector<int> widthArray;
            int ucharPos;
            UpdateCaret(m_SelectionPos2, ucharQueue, widthArray ,ucharPos);
        }

        m_SelectionEnd = &m_SelectionPos2;

        if(m_SelectionPos1.pos != m_SelectionPos2.pos)
        {
            m_SelFirstRow = 0;
            m_SelLastRow = int(m_Lines->m_RowCount - 1);

            if(m_EditMode == emColumnMode)
            {
                if(m_SelectionPos1.xpos < m_SelectionPos2.xpos)
                {
                    m_SelLeftXPos = m_SelectionPos1.xpos;
                    m_SelRightXPos = m_SelectionPos2.xpos;
                }
                else
                {
                    m_SelLeftXPos = m_SelectionPos2.xpos;
                    m_SelRightXPos = m_SelectionPos1.xpos;
                }
            }

            m_RepaintSelection = true;
            m_Selection = true;
            Refresh(false);
        }

        DoSelectionChanged();
    }
}

void MadEdit::CutToClipboard()
{
    if(m_Selection)
    {
        CopyToClipboard();
        DeleteSelection(true, nullptr, true);
    }
}

void MadEdit::CopyColumnText()
{
    wxString ws;
    GetColumnSelection(&ws);
    if(ws.size())
        PutColumnDataToClipboard(ws, m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1);
}

void MadEdit::CopyRegularText()
{
    wxString ws;
    xm::UCQueue ucqueue;

    wxFileOffset pos = m_SelectionBegin->pos;
    MadLineIterator lit = m_SelectionBegin->iter;
    m_Lines->InitNextUChar(lit, m_SelectionBegin->linepos);
    do
    {
        if (ucqueue.empty() && !m_Lines->NextUChar(ucqueue))
        {
            m_Lines->InitNextUChar(++lit, 0);
            continue;
        }

        const ucs4_t &uc=ucqueue.front().ucs4();
        if(uc==0x0D || uc==0x0A)
        {
            ws << wxm::g_nl_default.wxValue();

            pos += ucqueue.front().nbytes();

            if(uc==0x0D && m_Lines->NextUChar(ucqueue) && ucqueue.back().ucs4()==0x0A)
                pos += ucqueue.back().nbytes();

            ucqueue.clear();
            continue;
        }

        wxm::WxStrAppendUCS4(ws, uc);
        pos += ucqueue.front().nbytes();
        ucqueue.clear();
    } while(pos < m_SelectionEnd->pos);

    PutTextToClipboard(ws);
}

void MadEdit::CopyRawBytes()
{
    wxFileOffset pos = m_SelectionBegin->pos;
    MadLineIterator lit = m_SelectionBegin->iter;
    wxFileOffset lpos = m_SelectionBegin->linepos;

    std::string data;
    do
    {
        if(lpos < lit->m_Size)
        {
            data += lit->Get(lpos++);
            ++pos;
        }
        else
        {
            ++lit;
            lpos = 0;
        }
    }
    while(pos < m_SelectionEnd->pos);

    PutRawBytesToClipboard(data);
}

void MadEdit::CopyToClipboard()
{
    if(!m_Selection) return;

    if(m_EditMode==emColumnMode)
        CopyColumnText();
    else if(EditingInHexAera())
        wxm::HexAreaClipboardCopyProxy::Instance().GetSelectedCopier().Copy(this);
    else //if(!EditingInHexAera())
        CopyRegularText();
}

void MadEdit::InsertColumnText(vector<ucs4_t>& ucs, int lines)
{
    if (ucs.empty())
        return;

    InsertColumnString(&ucs[0], ucs.size(), lines, false, false);
}

void MadEdit::PasteColumnText()
{
    vector<ucs4_t> ucs;
    int lines = GetColumnDataFromClipboard(ucs);

    InsertColumnText(ucs, lines);
}

void MadEdit::InsertRegularText(vector<ucs4_t>& ucs)
{
    if(ucs.empty())
        return;

    bool oldim = m_InsertMode;
    m_InsertMode = true;

    InsertString(&ucs[0], ucs.size(), false, true, false);

    m_InsertMode = oldim;
}

void MadEdit::PasteRegularText()
{
    vector<ucs4_t> ucs;
    GetTextFromClipboard(ucs);

    InsertRegularText(ucs);
}

void MadEdit::PasteRawBytes(bool overwirte)
{
    vector<char> cs;
    GetRawBytesFromClipboard(cs);

    if(!cs.empty())
        InsertRawBytes((wxByte*)&cs[0], cs.size(), overwirte);
}

void MadEdit::PasteFromClipboard(bool overwirte)
{
    if(IsReadOnly())
        return;

    if(m_EditMode == emColumnMode)
        PasteColumnText();
    else if(!EditingInHexAera())
        PasteRegularText();
    else //if(EditingInHexAera())
        PasteRawBytes(overwirte);
}


void MadEdit::Undo()
{
    MadUndo *undo = m_UndoBuffer->Undo(!m_RecordCaretMovements);
    if(undo==nullptr)
        return;

    if(undo->m_Undos.size()==0) // caret movement undo
    {
        bool oldrcm=m_RecordCaretMovements;
        m_RecordCaretMovements=false;
        SetCaretPosition(undo->m_CaretPosBefore);
        m_RecordCaretMovements=oldrcm;
        return;
    }

    size_t oldrows = m_Lines->m_RowCount;
    size_t oldlines = m_Lines->m_LineCount;

    MadUndoDataRvsIterator it = undo->m_Undos.rbegin();
    MadUndoDataRvsIterator itend = undo->m_Undos.rend();

    MadLineIterator lit, litfirst, litlast;
    int lineid, fid = INT_MAX, lid = -1;

    do
    {
        wxFileOffset &pos = (*it)->m_Pos;
        lit = DeleteInsertData(pos, (*it)->InsSize(), nullptr, (*it)->DelSize(), (*it)->DelData(), &lineid);

        if(lineid < fid)
        {
            fid = lineid;
            litfirst = lit;
        }

        if(lineid > lid)
        {
            lid = lineid;
            litlast = lit;
        }
    }
    while(++it != itend);

    if(IsTextFile())
    {
        size_t count = m_Lines->Reformat(litfirst, litlast);

        m_CaretPos.pos = undo->m_CaretPosBefore;
        UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

        if(m_EditMode != emTextMode || m_Selection || oldrows != m_Lines->m_RowCount
            || oldlines != m_Lines->m_LineCount || count > 1 || lid < m_CaretPos.lineid)
        {
            m_RepaintAll = true;
        }
        else
        {
            m_RepaintSelection = true;
            m_SelFirstRow = m_CaretPos.rowid - m_CaretPos.subrowid;
            m_SelLastRow = m_SelFirstRow + int(litfirst->RowCount() - 1);
        }

        AppearCaret();
        UpdateScrollBarPos();

        if(m_EditMode == emHexMode)
        {
            if(!m_CaretAtHexArea)
            {
                UpdateTextAreaXPos();
                m_LastTextAreaXPos = m_TextAreaXPos;
            }
        }

        m_LastCaretXPos = m_CaretPos.xpos;

    }
    else
    {
        m_CaretPos.pos = undo->m_CaretPosBefore;
        m_CaretPos.linepos = m_CaretPos.pos;

        m_RepaintAll = true;
        AppearCaret();
        UpdateScrollBarPos();

        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    m_Selection = false;

    bool oldmod=m_Modified;

    if(m_UndoBuffer->GetPrevUndo() == m_SavePoint)
        m_Modified = false;
    else
        m_Modified = true;

    DoSelectionChanged();
    if(oldmod!=m_Modified) DoStatusChanged();

    Refresh(false);
    Update();   // repaint immediately
}

void MadEdit::Redo()
{
    MadUndo *redo = m_UndoBuffer->Redo(!m_RecordCaretMovements);
    if(redo==nullptr)
        return;

    if(redo->m_Undos.size()==0) // caret movement redo
    {
        bool oldrcm=m_RecordCaretMovements;
        m_RecordCaretMovements=false;
        SetCaretPosition(redo->m_CaretPosAfter);
        m_RecordCaretMovements=oldrcm;
        return;
    }

    size_t oldrows = m_Lines->m_RowCount;
    size_t oldlines = m_Lines->m_LineCount;

    MadUndoDataIterator it = redo->m_Undos.begin();
    MadUndoDataIterator itend = redo->m_Undos.end();

    MadLineIterator lit, litfirst, litlast;
    int lineid, fid = INT_MAX, lid = -1;

    do
    {
        wxFileOffset & pos = (*it)->m_Pos;
        lit = DeleteInsertData(pos, (*it)->DelSize(), nullptr, (*it)->InsSize(), (*it)->InsData(), &lineid);

        if(lineid < fid)
        {
            fid = lineid;
            litfirst = lit;
        }

        if(lineid > lid)
        {
            lid = lineid;
            litlast = lit;
        }
    }
    while(++it != itend);

    if(IsTextFile())
    {
        size_t count = m_Lines->Reformat(litfirst, litlast);

        m_CaretPos.pos = redo->m_CaretPosAfter;
        UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

        if(m_EditMode != emTextMode || m_Selection || oldrows != m_Lines->m_RowCount
            || oldlines != m_Lines->m_LineCount || count > 1 || lid < m_CaretPos.lineid)
        {
            m_RepaintAll = true;
        }
        else
        {
            m_RepaintSelection = true;
            m_SelFirstRow = m_CaretPos.rowid - m_CaretPos.subrowid;
            m_SelLastRow = m_SelFirstRow + int(litfirst->RowCount() - 1);
        }

        AppearCaret();
        UpdateScrollBarPos();

        if(m_EditMode == emHexMode)
        {
            if(!m_CaretAtHexArea)
            {
                UpdateTextAreaXPos();
                m_LastTextAreaXPos = m_TextAreaXPos;
            }
        }

        m_LastCaretXPos = m_CaretPos.xpos;

    }
    else
    {
        m_CaretPos.pos = redo->m_CaretPosAfter;
        m_CaretPos.linepos = m_CaretPos.pos;

        m_RepaintAll = true;
        AppearCaret();
        UpdateScrollBarPos();

        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    m_Selection = false;

    bool oldmod=m_Modified;

    if(m_UndoBuffer->GetPrevUndo() == m_SavePoint)
        m_Modified = false;
    else
        m_Modified = true;

    DoSelectionChanged();
    if(oldmod!=m_Modified) DoStatusChanged();

    Refresh(false);
    Update();   // repaint immediately
}

void MadEdit::GoToLine(int line)
{
    wxFileOffset oldCaretPos=m_CaretPos.pos;

    --line;
    if(line<0) line=0;
    else if(line>= int(m_Lines->m_LineCount)) line=int(m_Lines->m_LineCount-1);

    m_UpdateValidPos = -1;

    m_CaretPos.rowid=GetLineByLine(m_CaretPos.iter, m_CaretPos.pos, line);
    m_CaretPos.lineid=line;
    m_CaretPos.subrowid=0;
    m_CaretPos.linepos=0;
    m_CaretPos.xpos=0;
    m_CaretPos.extraspaces=0;
    m_LastCaretXPos=0;

    m_UpdateValidPos=0;

    UpdateCaret(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

    if(m_EditMode==emHexMode)
    {
        AppearHexRow(m_CaretPos.pos);

        m_CaretAtHalfByte=false;

        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
        }
    }

    AppearCaret(true);
    UpdateScrollBarPos();

    m_LastTextAreaXPos=m_TextAreaXPos;

    m_RepaintAll=true;
    Refresh(false);

    if(m_RecordCaretMovements && oldCaretPos != m_CaretPos.pos)
    {
        m_UndoBuffer->Add(oldCaretPos, m_CaretPos.pos);
    }

    DoSelectionChanged();

}

void MadEdit::SetCaretPosition(wxFileOffset pos, wxFileOffset selbeg, wxFileOffset selend)
{
    wxFileOffset oldCaretPos=m_CaretPos.pos;

    if(pos<0) pos=0;
    else if(pos>m_Lines->m_Size) pos=m_Lines->m_Size;

    m_CaretPos.pos=pos;

    m_UpdateValidPos=-1;
    UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);
    m_UpdateValidPos=0;

    AppearCaret(true);
    UpdateScrollBarPos();

    m_LastCaretXPos=m_CaretPos.xpos;

    if(m_EditMode == emHexMode)
    {
        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    // update selection pos
    if(selbeg > m_Lines->m_Size) selbeg=m_Lines->m_Size;
    if(selend > m_Lines->m_Size) selend=m_Lines->m_Size;
    if(selbeg>=0 && selend>=0 && selbeg!=selend)
    {
        m_SelectionPos1.pos = selbeg;
        m_SelectionPos2.pos = selend;
        m_Selection=true;
        UpdateSelectionPos();
    }

    m_RepaintAll=true;
    Refresh(false);

    if(m_RecordCaretMovements && oldCaretPos != m_CaretPos.pos)
    {
        m_UndoBuffer->Add(oldCaretPos, m_CaretPos.pos);
    }

    DoSelectionChanged();
}


bool MadEdit::LoadFromFile(const wxString& filename, const std::wstring& encoding, bool hexmode)
{
    wxFileName fn(filename);
    if(MadDirExists(fn.GetPath(wxPATH_GET_VOLUME))==0)
    {
        wxLogError(wxString(_("The Parent Directory of this file does not exist:"))+ wxT("\n\n") + filename);
        return false;
    }

    if(m_Lines->LoadFromFile(filename, encoding, hexmode)==false)
        return false;

    m_UndoBuffer->Clear();
    m_SavePoint = nullptr;
    m_Modified = false;
    m_ModificationTime = wxFileModificationTime(filename);
    m_ReadOnly = false; // use IsReadOnly() to check ReadOnly or not

    m_newline_for_insert = m_newline;

    m_Selection = false;
    m_SelFirstRow=INT_MAX;
    m_SelLastRow=-1;

    m_TopRow = 0;
    m_TextTopRow = 0;
    m_HexTopRow = 0;
    m_DrawingXPos = 0;

    m_ValidPos_iter=m_Lines->m_LineList.begin();
    m_ValidPos_lineid=0;
    m_ValidPos_rowid=0;
    m_ValidPos_pos=0;

    m_CaretPos.Reset(m_ValidPos_iter);

    m_LastCaretXPos = 0;
    m_DoRecountLineWidth = false;

    if (m_EditMode != emHexMode)
        UpdateCaret(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);

    m_Selection = false;
    m_SelFirstRow=INT_MAX;
    m_SelLastRow=-1;

    m_SelectionPos1 = m_CaretPos;
    m_SelectionPos2 = m_CaretPos;
    m_SelectionBegin = &m_SelectionPos1;
    m_SelectionEnd = &m_SelectionPos2;

    AppearCaret();
    UpdateScrollBarPos();

    m_RepaintAll = true;
    Refresh(false);

    DoSelectionChanged();
    DoStatusChanged();

    return true;
}

bool MadEdit::SaveToFile(const wxString &filename)
{
    wxFileName fn(filename);
    if(MadDirExists(fn.GetPath(wxPATH_GET_VOLUME))==0)
    {
        wxLogError(wxString(_("The Parent Directory of this file does not exist:"))+ wxT("\n\n") + filename);
        return false;
    }

    wxFileOffset tempsize=m_Lines->GetMaxTempSize(filename);
    wxMemorySize memsize=wxGetFreeMemory();

    wxString tempdir;

    if(memsize>=0 && memsize < wxMemorySize(tempsize+ 20*1024*1024)) // use disk as tempdata
    {
        wxFileName fn(filename);
        tempdir=fn.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);

        // ToDo: test the disk size

    }

    if(m_Lines->SaveToFile(filename, tempdir)==false)
    {
        wxMessageDialog dlg(this, wxString(_("Cannot save this file:")) +wxT("\n\n") + filename,
                            wxT("wxMEdit"), wxOK|wxICON_ERROR );
        dlg.ShowModal();

        return false;
    }

    m_SavePoint = m_UndoBuffer->GetPrevUndo();
    m_Modified=false;
    wxLogNull nolog;
    m_ModificationTime = wxFileModificationTime(filename);
    m_ReadOnly=false;
    DoStatusChanged();

    return true;
}

int MadEdit::Save(bool ask, const wxString &title, bool saveas) // return YES, NO, CANCEL
{
    int ret=wxID_YES;
    wxString filename=m_Lines->m_Name;
    if(filename.IsEmpty())
    {
        filename=title;
    }

    if(m_Modified && ask) //ask the user to save this file
    {
        wxMessageDialog dlg(this, wxString(_("Do you want to save this file?")) +wxT("\n\n") + filename,
            wxT("wxMEdit"), wxYES_NO|wxCANCEL|wxICON_QUESTION );

        ret=dlg.ShowModal();
    }

    if(saveas || (ret==wxID_YES && m_Modified))
    {
        wxString dlgtitle;
        if(saveas)
        {
            dlgtitle=_("Save File As");
        }
        else
        {
            dlgtitle=_("Save File");
        }

        if(saveas || m_Lines->m_Name.IsEmpty())  // choose a file to save
        {
            wxFileDialog dlg(this, dlgtitle, wxEmptyString, filename, wxFileSelectorDefaultWildcardStr,
#if wxCHECK_VERSION(2,8,0)
            wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
#else
            wxSAVE|wxOVERWRITE_PROMPT );
#endif

            dlg.SetReturnCode(wxID_OK);

            ret=dlg.ShowModal();

            if(ret==wxID_OK)
            {
                g_MB2WC_check_dir_filename=true;
                filename=dlg.GetPath();
                g_MB2WC_check_dir_filename=false;
                ret=wxID_YES;
            }
        }

        if(ret==wxID_YES)
        {
            SaveToFile(filename);
        }
    }

    return ret;
}

MadEdit::WXMLocations MadEdit::SaveLocations()
{
    WXMLocations loc;

    loc.pos = m_CaretPos.pos;
    loc.toprow = m_TopRow;
    loc.bmklinenums = SaveBookmarkLineNumberList();

    return loc;
}

void MadEdit::RestoreLocations(const WXMLocations& loc)
{
    RestoreBookmarkByLineNumberList(loc.bmklinenums);

    m_CaretPos.pos=loc.pos;

    if( m_CaretPos.pos > m_Lines->m_Size)
         m_CaretPos.pos = m_Lines->m_Size;

    m_UpdateValidPos=-1;
    UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);
    m_UpdateValidPos=0;

    m_TopRow=loc.toprow;

    AppearCaret();
    UpdateScrollBarPos();

    m_LastCaretXPos=m_CaretPos.xpos;

    if(m_EditMode == emHexMode)
    {
        if(!m_CaretAtHexArea)
        {
            UpdateTextAreaXPos();
            m_LastTextAreaXPos = m_TextAreaXPos;
        }
    }

    m_RepaintAll=true;
    Refresh(false);

    if(m_RecordCaretMovements && 0 != m_CaretPos.pos)
    {
        m_UndoBuffer->Add(0, m_CaretPos.pos);
    }

    DoSelectionChanged();
}

bool StrToInt64(wxString str, wxInt64 &i64)
{
    str.Trim(false);
    str.Trim(true);

    int base=10;
    str.MakeLower();
    if(str.Left(2)==wxT("0x")) 
    {
        base=16;
        str=str.Right(str.Len()-2);
    }

    bool ok=false;
    size_t i=0,len=str.Len();
    i64=0;
    while(i<len)
    {
        wxChar c=str[i];
        if(c<='9' && c>='0')
        {
            i64= (i64*base) + (c-'0');
        }
        else if(base==16 && c>='a' && c<='f')
        {
            i64= (i64*16) + (c-'a'+10);
        }
        else
        {
            return false;
        }

        ok=true;
        ++i;
    }

    return ok;
}
