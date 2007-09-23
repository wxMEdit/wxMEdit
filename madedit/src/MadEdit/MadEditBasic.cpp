///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadEditBasic.cpp
// Description: basic functions of MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadEdit.h"

#ifdef __WXGTK__
#   include "clipbrd_gtk.h"
#else
#   include <wx/clipbrd.h>
#endif

#include <wx/filename.h>


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern const ucs4_t HexHeader[];

//==============================================================================

const int CR_Points_Count = 5;
const int CR_Points[CR_Points_Count+1][2]=
{
    {   0,   0 },
    { 400,   0 },
    { 400, 999 },
    { 101, 700 },
    { 400, 700 },
    { 600, 999 }     // max value
};

const int LF_Points_Count = 5;
const int LF_Points[LF_Points_Count+1][2]=
{
    { 578,   0 },
    { 178,   0 },
    { 178, 999 },
    { 477, 700 },
    { 178, 700 },
    { 600, 999 } // max value
};

const int CRLF_Points_Count = 8;
const int CRLF_Points[CRLF_Points_Count+1][2]=
{
    {   0,   0 },
    { 560,   0 },
    { 300,   0 },
    { 300, 700 },
    {  76, 700 },
    { 300, 999 },
    { 524, 700 },
    { 300, 700 },
    { 600, 999 } // max value
};

/*
const int CR_Points2_Count = 14;
const int CR_Points2[CR_Points2_Count+1][2]=
{
    {   0,   0 },
    {   0, 601 },
    { 255, 348 },
    { 255, 850 },
    {  79, 644 },
    {  56, 667 },
    { 278, 999 },
    { 502, 664 },
    { 479, 641 },
    { 304, 850 },
    { 304, 270 },
    { 281, 247 },
    {  70, 457 },
    {  70,   0 },
    { 600, 999 }     // max value
};

const int LF_Points2_Count = 14;
const int LF_Points2[LF_Points2_Count+1][2]=
{
    { 255, 273 },
    { 255, 850 },
    {  79, 644 },
    {  56, 667 },
    { 278, 999 },
    { 502, 664 },
    { 479, 641 },
    { 304, 850 },
    { 304, 345 },
    { 558, 559 },
    { 558,   0 },
    { 488,   0 },
    { 488, 454 },
    { 281, 247 },
    { 600, 999 } // max value
};

const int CRLF_Points2_Count = 18;
const int CRLF_Points2[CRLF_Points2_Count+1][2]=
{
    {   0,   0 },
    {   0, 601 },
    { 255, 348 },
    { 255, 850 },
    {  79, 644 },
    {  56, 667 },
    { 278, 999 },
    { 502, 664 },
    { 479, 641 },
    { 304, 850 },
    { 304, 345 },
    { 558, 559 },
    { 558,   0 },
    { 488,   0 },
    { 488, 454 },
    { 281, 247 },
    {  70, 457 },
    {  70,   0 },
    { 600, 999 } // max value
};
*/

void MadEdit::SetSyntax(const wxString &title)
{
    if(m_Syntax->m_Title != title)
    {
        delete m_Syntax;
        m_Syntax = MadSyntax::GetSyntaxByTitle(title);
        m_Syntax->SetEncoding(m_Encoding);
        m_Syntax->InitNextWord1(m_Lines, m_WordBuffer, m_WidthBuffer,
            m_TextFont->GetFaceName(), m_TextFont->GetPointSize(), m_TextFont->GetFamily());

        m_Lines->m_Syntax=m_Syntax;

        if(!m_LoadingFile)
        {
            if(IsTextFile())    // data is text
            {
                ReformatAll();

                DoStatusChanged();
                DoSelectionChanged();

                m_RepaintAll = true;
                Refresh(false);
            }
        }
    }
}

void MadEdit::ApplySyntaxAttributes(MadSyntax *syn, bool matchTitle)
{
    if(!matchTitle || syn->m_Title==m_Syntax->m_Title)
    {
        m_Syntax->AssignAttributes(syn);
        if(m_EditMode==emHexMode && m_HexDigitBitmap)
        {
            delete m_HexDigitBitmap;
            m_HexDigitBitmap=NULL;
        }
        m_RepaintAll = true;
        Refresh(false);
    }
}

void MadEdit::SetEncoding(const wxString &encname)
{
    if(encname.Lower() != m_Encoding->GetName().Lower())
    {
        delete m_Encoding;
        m_Encoding=new MadEncoding(encname);
        m_Lines->SetEncoding(m_Encoding);
        m_Syntax->SetEncoding(m_Encoding);

        wxString fontname;
        m_Config->Read(wxString(wxT("/Fonts/"))+m_Encoding->GetName(), &fontname, m_Encoding->GetFontName());

        bool oldlf=m_LoadingFile;
        m_LoadingFile=true;
        SetTextFont(fontname, m_TextFont->GetPointSize(), false);
        m_LoadingFile=oldlf;

        if(!m_LoadingFile)
        {
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
    }
}


void MadEdit::SetRecordCaretMovements(bool value)
{
    if(value!=m_RecordCaretMovements)
    {
        m_RecordCaretMovements=value;

        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/RecordCaretMovements"), value);
            m_Config->SetPath(oldpath);
        }
    }
}


void MadEdit::SetTextFont(const wxString &name, int size, bool forceReset)
{
    if(forceReset || size!=m_TextFont->GetPointSize() || name!=m_TextFont->GetFaceName())
    {
        m_TextFont=wxTheFontList->FindOrCreateFont(size, //font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined(), name);
            wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, name);

        memset(m_TextFontWidths, 0, sizeof(m_TextFontWidths));
        m_TextFontWidths[0] = FontWidthManager::GetFontWidths(0, name, size, this);

        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxString(wxT("/Fonts/"))+m_Encoding->GetName(), name);
            m_Config->Write(wxT("/MadEdit/TextFontSize"), size);
            m_Config->SetPath(oldpath);
        }


        if(m_EditMode==emHexMode)
        {
            m_DoRecountLineWidth=true;
        }
        else
        {
            MadEditSuperClass::SetFont(*m_TextFont);
            m_TextFontHeight=GetCharHeight();

            bool ofwm=m_FixedWidthMode;
            m_FixedWidthMode=false; // avoid effecting on GetUCharWidth();

            m_TextFontAveCharWidth=GetUCharWidth(0x20);//GetCharWidth();

            int w;
            m_TextFontMaxDigitWidth=GetUCharWidth('0');
            for(wxChar wc=wxT('1');wc<=wxT('9');wc++)
            {
                w=GetUCharWidth(wc);
                if(w>m_TextFontMaxDigitWidth)
                {
                    m_TextFontMaxDigitWidth=w;
                    m_TextFontAveCharWidth=w;
                }
            }

            if(ofwm)//(m_FixedWidthMode)
            {
                m_TextFontAveCharWidth=m_TextFontMaxDigitWidth;

                if(!HexPrinting())
                {
                    w=GetUCharWidth('W');
                    if(w>m_TextFontMaxDigitWidth)
                    {
                        m_TextFontMaxDigitWidth=w;
                        m_TextFontAveCharWidth=w;
                    }
                }
                else
                {
                    for(wxChar wc=wxT('A');wc<=wxT('F');wc++)
                    {
                        w=GetUCharWidth(wc);
                        if(w>m_TextFontMaxDigitWidth)
                        {
                            m_TextFontMaxDigitWidth=w;
                            m_TextFontAveCharWidth=w;
                        }
                    }
                }
            }

            m_FixedWidthMode=ofwm;


            m_LeftMarginWidth = (m_TextFontAveCharWidth >> 1)-1;
            m_RightMarginWidth = m_TextFontAveCharWidth << 1;

            m_Syntax->InitNextWord1(m_Lines, m_WordBuffer, m_WidthBuffer,
                name, size, m_TextFont->GetFamily());


            // prepare m_Space_Points, m_EOF_Points
            const int cw = GetUCharWidth(0x20);   //FFontAveCharWidth;
            {
                const int t1 = m_TextFontHeight / 5;
                const int y = 1 + m_TextFontHeight - t1;
                const int x = cw / 4;
                m_Space_Points[0].x = x;
                m_Space_Points[0].y = y;
                m_Space_Points[1].x = 3 * x;
                m_Space_Points[1].y = y;
                m_Space_Points[2].x = cw >> 1;
                m_Space_Points[2].y = y - m_TextFontHeight/6;
                m_Space_Points[3].x = x;
                m_Space_Points[3].y = y;
            }
            {
                const int t1 = m_TextFontHeight / 5;
                const int x = cw - 1;
                int y = t1 + 1;
                m_EOF_Points[0].x = x;
                m_EOF_Points[0].y = y;
                m_EOF_Points[1].x = x;
                m_EOF_Points[1].y = y += (m_TextFontHeight - (t1 << 1));
                m_EOF_Points[2].x = x - (cw - 3);
                m_EOF_Points[2].y = y;
                m_EOF_Points[3].x = cw / 2;
                m_EOF_Points[3].y = y - (m_TextFontHeight  / 6 );
                m_EOF_Points[4].x = cw / 2;
                m_EOF_Points[4].y = y;
            }
            //m_CR_Points[], m_LF_Points[], m_CRLF_Points[];
            {
                const int t1 = m_TextFontHeight / 5;
                const int x = cw/5;

                m_CR_Points_Count = CR_Points_Count;
                m_LF_Points_Count = LF_Points_Count;
                m_CRLF_Points_Count = CRLF_Points_Count;
                {
                    const int maxX = CR_Points[CR_Points_Count][0] * 7;
                    const int maxY = CR_Points[CR_Points_Count][1] * 7;
                    for(int i=0; i< CR_Points_Count; ++i)
                    {
                        int tx = CR_Points[i][0]*cw*5 / maxX;
                        int ty = CR_Points[i][1]*m_TextFontHeight*5 / maxY;
                        m_CR_Points[i].x = tx + x;
                        m_CR_Points[i].y = ty + t1;
                    }
                }
                {
                    const int maxX = LF_Points[LF_Points_Count][0] * 7;
                    const int maxY = LF_Points[LF_Points_Count][1] * 7;
                    for(int i=0; i< LF_Points_Count; ++i)
                    {
                        int tx = LF_Points[i][0]*cw*5 / maxX;
                        int ty = LF_Points[i][1]*m_TextFontHeight*5 / maxY;
                        m_LF_Points[i].x = tx + x ;
                        m_LF_Points[i].y = ty + t1;
                    }
                }
                {
                    const int maxX = CRLF_Points[CRLF_Points_Count][0] * 7;
                    const int maxY = CRLF_Points[CRLF_Points_Count][1] * 7;
                    for(int i=0; i< CRLF_Points_Count; ++i)
                    {
                        int tx = CRLF_Points[i][0]*cw*5 / maxX;
                        int ty = CRLF_Points[i][1]*m_TextFontHeight*5 / maxY;
                        m_CRLF_Points[i].x = tx + x ;
                        m_CRLF_Points[i].y = ty + t1;
                    }
                }

                /*
                m_CR_Points_Count = CR_Points2_Count;
                m_LF_Points_Count = LF_Points2_Count;
                m_CRLF_Points_Count = CRLF_Points2_Count;
                {
                    const int maxX = CR_Points2[CR_Points2_Count][0] * 7;
                    const int maxY = CR_Points2[CR_Points2_Count][1] * 7;
                    for(int i=0; i< CR_Points2_Count; ++i)
                    {
                        int tx = CR_Points2[i][0]*cw*5 / maxX;
                        int ty = CR_Points2[i][1]*m_TextFontHeight*5 / maxY;
                        m_CR_Points[i].x = tx + x;
                        m_CR_Points[i].y = ty + t1;
                    }
                }
                {
                    const int maxX = LF_Points2[LF_Points2_Count][0] * 7;
                    const int maxY = LF_Points2[LF_Points2_Count][1] * 7;
                    for(int i=0; i< LF_Points2_Count; ++i)
                    {
                        int tx = LF_Points2[i][0]*cw*5 / maxX;
                        int ty = LF_Points2[i][1]*m_TextFontHeight*5 / maxY;
                        m_LF_Points[i].x = tx + x;
                        m_LF_Points[i].y = ty + t1;
                    }
                }
                {
                    const int maxX = CRLF_Points2[CRLF_Points2_Count][0] * 7;
                    const int maxY = CRLF_Points2[CRLF_Points2_Count][1] * 7;
                    for(int i=0; i< CRLF_Points2_Count; ++i)
                    {
                        int tx = CRLF_Points2[i][0]*cw*5 / maxX;
                        int ty = CRLF_Points2[i][1]*m_TextFontHeight*5 / maxY;
                        m_CRLF_Points[i].x = tx + x;
                        m_CRLF_Points[i].y = ty + t1;
                    }
                }
                */
            }

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
    if(forceReset || size!=m_HexFont->GetPointSize() || name!=m_HexFont->GetFaceName())
    {
        m_HexFont=wxTheFontList->FindOrCreateFont(size, //font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined(), name);
            wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, name);

        memset(m_HexFontWidths, 0, sizeof(m_HexFontWidths));
        m_HexFontWidths[0]=FontWidthManager::GetFontWidths(0, name, size, this);

        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxString(wxT("/MadEdit/HexFontName")), name);
            m_Config->Write(wxT("/MadEdit/HexFontSize"), size);
            m_Config->SetPath(oldpath);
        }

        if(m_HexDigitBitmap)
        {
            delete m_HexDigitBitmap;
            m_HexDigitBitmap=NULL;
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
                    int crow = m_CaretPos.pos >> 4;
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
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/FixedWidthMode"), mode);
            m_Config->SetPath(oldpath);
        }

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
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/LineSpacing"), percent);
            m_Config->SetPath(oldpath);
        }

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
    if(m_LoadingFile == false)
    {
        long maxtextfilesize;
        wxString oldpath=m_Config->GetPath();
        m_Config->Read(wxT("/MadEdit/MaxTextFileSize"), &maxtextfilesize, 1024*1024*10);//10MB
        m_Config->SetPath(oldpath);

        if(!IsTextFile())
        {
            if(m_Lines->m_Size>=maxtextfilesize)
            {
                wxString size=FormatThousands(wxLongLong(m_Lines->m_Size).ToString());
                if(wxNO==wxMessageBox(wxString::Format(_("Do you want to continue?\nThe file size is %s bytes.\nIt may take long time and large memories to convert to Text/Column Mode."), size.c_str()), _("Hex Mode to Text/Column Mode"), wxYES_NO))
                {
                    return;
                }
            }

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
    if(m_EditMode != mode && !m_SingleLineMode)
    {
        bool selchange = false;

        switch(mode)
        {
        case emTextMode:
            if(m_EditMode == emColumnMode)
            {
                if(m_CaretPos.extraspaces)
                {
                    m_CaretPos.xpos -= int(m_CaretPos.extraspaces * GetUCharWidth(0x20));
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
                        m_SelectionPos1.xpos -= int(m_SelectionPos1.extraspaces * GetUCharWidth(0x20));
                        m_SelectionPos1.extraspaces = 0;
                    }
                    if(m_SelectionPos2.extraspaces)
                    {
                        m_SelectionPos2.xpos -= int(m_SelectionPos2.extraspaces * GetUCharWidth(0x20));
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
                m_TopRow = (m_CaretPos.pos >> 4);
                if(m_TopRow >= (m_VisibleRowCount >> 1))
                {
                    m_TopRow -= (m_VisibleRowCount >> 1);
                    int rows = (m_Lines->m_Size >> 4);

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
}

void MadEdit::SetSingleLineMode(bool mode)
{
    if(m_SingleLineMode != mode)
    {
        if(mode)
        {
            m_StorePropertiesToGlobalConfig = false;
            SetDisplayLineNumber(false);
            SetWordWrapMode(wwmNoWrap);
            SetEditMode(emTextMode);
            SetMarkActiveLine(false);

            m_VScrollBar->Show(false);
            m_HScrollBar->Show(false);
        }

        m_SingleLineMode = mode;

        wxSizeEvent evt;
        OnSize(evt);
    }
}

void MadEdit::SetTabColumns(long value)
{
    if(m_TabColumns != value && value >= 1 && value <= 80)
    {
        m_TabColumns = value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/TabColumns"), value);
            m_Config->SetPath(oldpath);
        }

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
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/InsertSpacesInsteadOfTab"), value);
            m_Config->SetPath(oldpath);
        }
    }
}

void MadEdit::SetWordWrapMode(MadWordWrapMode mode)
{
    if(m_WordWrapMode != mode && !m_SingleLineMode)
    {
        m_WordWrapMode = mode;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/WordWrapMode"), (long)mode);
            m_Config->SetPath(oldpath);
        }

        if(m_EditMode != emHexMode)
        {
            if(mode==wwmWrapByWindow)
                m_DrawingXPos=0;

            UpdateAppearance();
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

void MadEdit::SetDisplayLineNumber(bool value)
{
    if(value!=m_DisplayLineNumber)
    {
        m_DisplayLineNumber=value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/DisplayLineNumber"), value);
            m_Config->SetPath(oldpath);
        }

        m_RepaintAll=true;
        Refresh(false);
    }
}
void MadEdit::SetShowEndOfLine(bool value)
{
    if(value!=m_ShowEndOfLine)
    {
        m_ShowEndOfLine=value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/ShowEndOfLine"), value);
            m_Config->SetPath(oldpath);
        }

        m_RepaintAll=true;
        Refresh(false);
    }
}
void MadEdit::SetShowTabChar(bool value)
{
    if(value!=m_ShowTabChar)
    {
        m_ShowTabChar=value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/ShowTabChar"), value);
            m_Config->SetPath(oldpath);
        }

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
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/ShowSpaceChar"), value);
            m_Config->SetPath(oldpath);
        }

        m_RepaintAll=true;
        Refresh(false);
    }
}
void MadEdit::SetMarkActiveLine(bool value)
{
    if(value!=m_MarkActiveLine)
    {
        m_MarkActiveLine=value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/MarkActiveLine"), value);
            m_Config->SetPath(oldpath);
        }

        m_RepaintAll=true;
        Refresh(false);
    }
}

void MadEdit::SetMarkBracePair(bool value)
{
    if(value!=m_MarkBracePair)
    {
        m_MarkBracePair=value;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/MarkBracePair"), value);
            m_Config->SetPath(oldpath);
        }

        m_RepaintAll=true;
        Refresh(false);
    }
}

void MadEdit::SetMaxColumns(long cols)
{
    if(m_MaxColumns!=cols)
    {
        m_MaxColumns = cols;
        if(m_StorePropertiesToGlobalConfig)
        {
            wxString oldpath=m_Config->GetPath();
            m_Config->Write(wxT("/MadEdit/MaxColumns"), cols);
            m_Config->SetPath(oldpath);
        }

        if(m_WordWrapMode==wwmWrapByColumn)
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
    else if(m_FixedWidthMode || GetUCharWidth('W')==GetUCharWidth('i'))//m_TextFontAveCharWidth)
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
        return GetColumnSelection(NULL);
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
        MadUCQueue ucqueue;

        MadLineIterator lit = m_SelectionBegin->iter;
        m_Lines->InitNextUChar(lit, m_SelectionBegin->linepos);
        MadLines::NextUCharFuncPtr NextUChar=m_Lines->NextUChar;
        do
        {
            if(!(m_Lines->*NextUChar)(ucqueue))
            {
                ++lit;
                m_Lines->InitNextUChar(lit, 0);
                (m_Lines->*NextUChar)(ucqueue);
            }

#ifdef __WXMSW__
            ucs4_t uc=ucqueue.back().first;
            if(uc>=0x10000)
            {
                wchar_t wbuf[2];
                m_Encoding->UCS4toUTF16LE_U10000(uc, (wxByte*)wbuf);
                ws<<wbuf[0];
                ws<<wbuf[1];
            }
            else
            {
                ws<<wxChar(uc);
            }
#else
            ws<<wxChar(ucqueue.back().first);
#endif

            pos += ucqueue.back().second;
        }
        while(pos < m_SelectionEnd->pos);
    }
}

void MadEdit::GetText(wxString &ws, bool ignoreBOM)
{
    wxFileOffset pos = 0;
    MadUCQueue ucqueue;

    MadLineIterator lit = m_Lines->m_LineList.begin();

    if(ignoreBOM)                 // if first line has BOM, we will ignore it
    {
        pos = lit->m_RowIndices.front().m_Start;
    }

    if(pos >= m_Lines->m_Size)
        return;

    MadLines::NextUCharFuncPtr NextUChar=m_Lines->NextUChar;
    m_Lines->InitNextUChar(lit, pos);
    do
    {
        if(!(m_Lines->*NextUChar)(ucqueue))
        {
            ++lit;
            m_Lines->InitNextUChar(lit, 0);
            (m_Lines->*NextUChar)(ucqueue);
        }

#ifdef __WXMSW__
        ucs4_t uc=ucqueue.back().first;
        if(uc>=0x10000)
        {
            wchar_t wbuf[2];
            m_Encoding->UCS4toUTF16LE_U10000(uc, (wxByte*)wbuf);
            ws<<wbuf[0];
            ws<<wbuf[1];
        }
        else
        {
            ws<<wxChar(uc);
        }
#else
        ws<<wxChar(ucqueue.back().first);
#endif

        pos += ucqueue.back().second;
    }
    while(pos < m_Lines->m_Size);
}

void MadEdit::SetText(const wxString &ws)
{
    MadLineIterator lit;
    size_t size = ws.Len();

    if(m_SingleLineMode)
    {
        const wxChar *wcs = ws.c_str();
        size_t sss = 0;

        while(sss < size && *wcs != 0x0D && *wcs != 0x0A)
        {
            ++sss;
            ++wcs;
        }

        long maxlen = m_MaxLineLength - 100;
        if(m_Lines->m_Size + sss > maxlen)
            sss = maxlen - long (m_Lines->m_Size);

        size = sss;
    }

    MadUndo *undo=NULL;

    if(m_Lines->m_Size)
    {
        if(size == 0)               // delete
        {
            MadDeleteUndoData *dudata = new MadDeleteUndoData;

            dudata->m_Pos = 0;
            dudata->m_Size = m_Lines->m_Size;

            lit = DeleteInsertData(0, dudata->m_Size, &dudata->m_Data, 0, NULL);

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
            TranslateText(ws.c_str(), size, &ucs, true);
            UCStoBlock(&(*ucs.begin()), ucs.size(), blk);

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
        TranslateText(ws.c_str(), size, &ucs, true);
        UCStoBlock(&(*ucs.begin()), ucs.size(), blk);

        MadInsertUndoData *insud = new MadInsertUndoData;
        insud->m_Pos = 0;
        insud->m_Size = blk.m_Size;
        insud->m_Data.push_back(blk);

        lit = DeleteInsertData(0, 0, NULL, insud->m_Size, &insud->m_Data);

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
    MadUCQueue ucqueue;

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

    MadLines::NextUCharFuncPtr NextUChar=m_Lines->NextUChar;
    m_Lines->InitNextUChar(lit, pos);
    do
    {
        if(!(m_Lines->*NextUChar)(ucqueue))
        {
            return true;
        }

        ucs4_t uc=ucqueue.back().first;
        if(uc==0x0D || uc==0x0A)
            return true;

#ifdef __WXMSW__
        if(uc>=0x10000)
        {
            wchar_t wbuf[2];
            m_Encoding->UCS4toUTF16LE_U10000(uc, (wxByte*)wbuf);
            ws<<wbuf[0];
            ws<<wbuf[1];
        }
        else
        {
            ws<<wxChar(uc);
        }
#else
        ws<<wxChar(uc);
#endif
    }
    while(ws.Len() < maxlen);

    if((m_Lines->*NextUChar)(ucqueue)) return false;
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
            MadUCQueue ucharQueue;
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
        DeleteSelection(true, NULL, true);
    }
}

void MadEdit::CopyToClipboard()
{
    if(!m_Selection) return;

    if(m_EditMode==emColumnMode)
    {
        wxString ws;
        GetColumnSelection(&ws);
        if(ws.size())
        {
            PutColumnDataToClipboard(ws, m_SelectionEnd->rowid - m_SelectionBegin->rowid + 1);
        }
    }
    else if(m_EditMode==emTextMode || !m_CaretAtHexArea)
    {
        wxString ws;
        MadUCQueue ucqueue;
        MadLines::NextUCharFuncPtr NextUChar=m_Lines->NextUChar;

        wxFileOffset pos = m_SelectionBegin->pos;
        MadLineIterator lit = m_SelectionBegin->iter;
        m_Lines->InitNextUChar(lit, m_SelectionBegin->linepos);
        do
        {
            if(ucqueue.size() || (m_Lines->*NextUChar)(ucqueue))
            {
                ucs4_t &uc=ucqueue.front().first;
                if(uc==0x0D || uc==0x0A)
                {
#ifdef __WXMSW__
                    ws<<wxChar(0x0D);
#endif
                    ws<<wxChar(0x0A);

                    pos += ucqueue.front().second;

                    if(uc==0x0D && (m_Lines->*NextUChar)(ucqueue) &&
                        ucqueue.back().first==0x0A)
                    {
                        pos += ucqueue.back().second;
                    }

                    ucqueue.clear();

                }
#ifdef __WXMSW__
                else if(uc>=0x10000)
                {
                    wchar_t wcbuf[2];
                    m_Encoding->UCS4toUTF16LE_U10000(uc, (wxByte*)wcbuf);
                    ws<<wxChar(wcbuf[0]);
                    ws<<wxChar(wcbuf[1]);
                    pos += ucqueue.front().second;
                    ucqueue.clear();
                }
#endif
                else
                {
                    ws<<wxChar(uc);
                    pos += ucqueue.front().second;
                    ucqueue.clear();
                }

            }
            else
            {
                m_Lines->InitNextUChar(++lit, 0);
            }
        }
        while(pos < m_SelectionEnd->pos);

        PutTextToClipboard(ws);
    }
    else //m_EditMode==emHexMode && m_CaretAtHexArea
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

        PutHexDataToClipboard(data.c_str(), data.size());
    }
}

void MadEdit::PasteFromClipboard()
{
    if(IsReadOnly())
        return;

    if(m_EditMode == emColumnMode)
    {
        vector < ucs4_t > ucs;
        int lines=GetColumnDataFromClipboard(&ucs);

        if(ucs.size())
            InsertColumnString(&(*ucs.begin()), ucs.size(), lines, false, false);
    }
    else if(m_EditMode == emHexMode && m_CaretAtHexArea)
    {
        vector < char >cs;
        GetHexDataFromClipboard(&cs);

        if(cs.size())
        {
            InsertHexData((wxByte*)&(*cs.begin()), cs.size());
        }
    }
    else //if(m_EditMode == emTextMode || !m_CaretAtHexArea)
    {
        vector < ucs4_t > ucs;
        GetTextFromClipboard(&ucs);

        size_t size = ucs.size();
        if(size)
        {
            bool oldim = m_InsertMode;
            m_InsertMode = true;

            InsertString(&(*ucs.begin()), size, false, true, false);

            m_InsertMode = oldim;
        }
    }
}


void MadEdit::Undo()
{
    MadUndo *undo = m_UndoBuffer->Undo(!m_RecordCaretMovements);
    if(undo==NULL)
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
        switch ((*it)->m_Type)
        {
        case udtInsert:
            {
                MadInsertUndoData *iudata = (MadInsertUndoData *) (*it);
                lit = DeleteInsertData(pos, iudata->m_Size, NULL, 0, NULL, &lineid);
            }
            break;
        case udtDelete:
            {
                MadDeleteUndoData *dudata = (MadDeleteUndoData *) (*it);
                lit = DeleteInsertData(pos, 0, NULL, dudata->m_Size, &dudata->m_Data, &lineid);
            }
            break;
        case udtOverwrite:
            {
                MadOverwriteUndoData *oudata = (MadOverwriteUndoData *) (*it);
                lit = DeleteInsertData(pos, oudata->m_InsSize, NULL,
                                            oudata->m_DelSize, &oudata->m_DelData,
                                            &lineid);
            }
            break;
        }

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
    if(redo==NULL)
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
        switch ((*it)->m_Type)
        {
        case udtInsert:
            {
                MadInsertUndoData *iudata = (MadInsertUndoData *) (*it);
                lit = DeleteInsertData(pos, 0, NULL, iudata->m_Size, &iudata->m_Data, &lineid);
            }
            break;
        case udtDelete:
            {
                MadDeleteUndoData *dudata = (MadDeleteUndoData *) (*it);
                lit = DeleteInsertData(pos, dudata->m_Size, NULL, 0, NULL, &lineid);
            }
            break;
        case udtOverwrite:
            {
                MadOverwriteUndoData *oudata = (MadOverwriteUndoData *) (*it);
                lit = DeleteInsertData(pos, oudata->m_DelSize, NULL,
                                            oudata->m_InsSize, &oudata->m_InsData,
                                            &lineid);
            }
            break;
        }

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


bool MadEdit::LoadFromFile(const wxString &filename, const wxString &encoding)
{
    wxFileName fn(filename);
    if(MadDirExists(fn.GetPath(wxPATH_GET_VOLUME))==0)
    {
        wxLogError(wxString(_("The Parent Directory of this file does not exist:"))+ wxT("\n\n") + filename);
        return false;
    }

    if(m_Lines->LoadFromFile(filename, encoding)==false)
        return false;

    m_UndoBuffer->Clear();
    m_SavePoint = NULL;
    m_Modified = false;
    m_ModificationTime = wxFileModificationTime(filename);
    m_ReadOnly = false; // use IsReadOnly() to check ReadOnly or not

    m_InsertNewLineType=m_NewLineType;

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

    if(m_EditMode == emHexMode)
    {

    }
    else
    {
        UpdateCaret(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);
    }

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

    if(memsize>=0 && memsize < (tempsize+ 20*1024*1024)) // use disk as tempdata
    {
        wxFileName fn(filename);
        tempdir=fn.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);

        // ToDo: test the disk size

    }

    if(m_Lines->SaveToFile(filename, tempdir)==false)
    {
        wxMessageDialog dlg(this, wxString(_("Cannot save this file:")) +wxT("\n\n") + filename,
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
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
            wxT("MadEdit"), wxYES_NO|wxCANCEL|wxICON_QUESTION );

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

bool MadEdit::Reload()
{
    if(m_Lines->m_Name.IsEmpty()) return false;

    if(m_Modified)
    {
        wxMessageDialog dlg(this, _("Do you want to discard changes?"), wxT("MadEdit"), wxYES_NO|wxICON_QUESTION );
        if(dlg.ShowModal()!=wxID_YES)
        {
            return false;
        }
    }

    int toprow=m_TopRow;
    wxFileOffset oldCaretPos=m_CaretPos.pos;
    MadEditMode editmode=m_EditMode;

    LoadFromFile(m_Lines->m_Name);
    SetEditMode(editmode);
    RestorePosition(oldCaretPos, toprow);
    return true;
}

bool MadEdit::ReloadByModificationTime()
{
    if(m_Lines->m_Name.IsEmpty()) return false;

    wxLogNull nolog;
    time_t modtime=wxFileModificationTime(m_Lines->m_Name);

    if(modtime==0) // the file has been deleted
    {
        m_ModificationTime=0;
        return false;
    }

    if(modtime == m_ModificationTime) return false; // the file doesn't change.

    m_ModificationTime = modtime;

    wxMessageDialog dlg(this,
        wxString(_("This file has been changed by another application."))+ wxT("\n")+
        wxString(_("Do you want to reload it?"))+ wxT("\n\n")+ m_Lines->m_Name,
        wxT("MadEdit"), wxYES_NO|wxICON_QUESTION );
    if(dlg.ShowModal()!=wxID_YES)
    {
        return false;
    }

    // YES, reload it.
    return Reload();
}

void MadEdit::RestorePosition(wxFileOffset pos, int toprow)
{
    if(pos > m_Lines->m_Size) pos=m_Lines->m_Size;

    m_CaretPos.pos=pos;

    m_UpdateValidPos=-1;
    UpdateCaretByPos(m_CaretPos, m_ActiveRowUChars, m_ActiveRowWidths, m_CaretRowUCharPos);
    m_UpdateValidPos=0;

    m_TopRow=toprow;

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


/************** Find/Replace functions ****************/

MadSearchResult MadEdit::FindTextNext(const wxString &text,
                bool bRegex, bool bCaseSensitive, bool bWholeWord,
                wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
    MadCaretPos bpos, epos;
    MadUCQueue ucharQueue;
    vector<int> widthArray;
    int tmp;

    if(rangeFrom == -1)
    {
        bpos=m_CaretPos;
    }
    else
    {
        bpos.pos = rangeFrom;
        UpdateCaretByPos(bpos, ucharQueue, widthArray, tmp);
    }

    if(rangeTo == -1)
    {
        epos.iter=m_Lines->m_LineList.end();
        --epos.iter;
        epos.linepos=epos.iter->m_Size;
        epos.pos=m_Lines->m_Size;
    }
    else
    {
        epos.pos = rangeTo;
        UpdateCaretByPos(epos, ucharQueue, widthArray, tmp);
    }

    MadSearchResult state=Search(bpos, epos, text, bRegex, bCaseSensitive, bWholeWord);
    if(state==SR_YES)
    {
        SetSelection(bpos.pos, epos.pos);
    }

    return state;
}

MadSearchResult MadEdit::FindTextPrevious(const wxString &text,
                bool bRegex, bool bCaseSensitive, bool bWholeWord,
                wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
    MadCaretPos bpos, epos;

    if(rangeFrom == -1)
    {
        epos=m_CaretPos;
    }
    else
    {
        MadUCQueue ucharQueue;
        vector<int> widthArray;
        int tmp;
        epos.pos = rangeFrom;
        UpdateCaretByPos(epos, ucharQueue, widthArray, tmp);
    }

    if(rangeTo < 0) rangeTo = 0;

    if(epos.pos <= rangeTo) return SR_NO;

    size_t searchsize= 50 * 1024;
    if(searchsize< text.Len()*8) searchsize=text.Len()*8;

    wxFileOffset startpos=epos.pos;

    for(;;)
    {
        startpos -= searchsize;
        if(startpos<rangeTo)
        {
            startpos=rangeTo;
        }
        else
        {
            size_t s=size_t(startpos);
            s=s%4;
            startpos-=s;
        }

        wxFileOffset len=epos.pos-startpos;
        if(epos.linepos >= len)
        {
            bpos.pos=epos.pos-len;
            bpos.iter=epos.iter;
            bpos.linepos=epos.linepos-len;
        }
        else
        {
            bpos.pos=startpos;
            GetLineByPos(bpos.iter, bpos.pos, bpos.rowid);
            bpos.linepos=0;
            startpos=bpos.pos;
        }

        MadCaretPos bpos1=bpos, epos1=epos;
        int state=Search(bpos1, epos1, text, bRegex, bCaseSensitive, bWholeWord);
        if(state==SR_EXPR_ERROR)
        {
            return SR_EXPR_ERROR;
        }

        if(state==SR_YES) // found
        {
            MadLines::NextUCharFuncPtr NextUChar=m_Lines->NextUChar;
            MadUCQueue ucq;

            MadCaretPos bp, ep;
            do
            {
                bp=bpos1;
                ep=epos1;

                // advance one uchar
                m_Lines->InitNextUChar(bpos1.iter, bpos1.linepos);
                if(!(m_Lines->*NextUChar)(ucq))
                {
                    ++bpos1.iter;
                    bpos1.linepos=0;
                    m_Lines->InitNextUChar(bpos1.iter, 0);
                    (m_Lines->*NextUChar)(ucq);
                }
                bpos1.pos+=ucq.back().second;
                bpos1.linepos+=ucq.back().second;

                epos1=epos;
            }
            while(Search(bpos1, epos1, text, bRegex, bCaseSensitive, bWholeWord));

            SetSelection(bp.pos, ep.pos, true);
            return SR_YES;
        }

        // not found
        if(startpos<=rangeTo) break;

        epos=bpos;
        size_t s=text.Len()*4;

        if(epos.pos+ s > epos1.pos)
        {
            s=text.Len();
            if(epos.pos+ s > epos1.pos)
            {
                s=0;
            }
        }

        if(s!=0)
        {
            epos.pos+=s;
            do
            {
                len=epos.iter->m_Size-epos.linepos;
                if(len>s) len=s;

                if((epos.linepos+=len)==epos.iter->m_Size)
                {
                    epos.linepos=0;
                    epos.iter++;
                }
            }
            while((s-=len) > 0);
        }
    }

    return SR_NO;
}


bool MadEdit::StringToHex(wxString ws, vector<wxByte> &hex)
{
    ws.Trim(false);

    size_t len=ws.Len();
    if(len==0)
        return true;    // it's OK!!!

    wxString errmsg(_("The input string is not a valid Hex-String:"));

    const wxChar *pc=ws.c_str();

    do
    {
        if(len<2)
        {
            wxMessageDialog dlg(NULL, errmsg+wxT("\n\n")+ws,
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
            dlg.ShowModal();
            return false;
        }

        int b0=FromHex(*pc++);
        int b1=FromHex(*pc++);
        len-=2;

        if(b0<0 || b1<0)
        {
            wxMessageDialog dlg(NULL, errmsg+wxT("\n\n")+ws,
                            wxT("MadEdit"), wxOK|wxICON_ERROR );
            dlg.ShowModal();
            return false;
        }

        hex.push_back( (b0<<4) | b1 );

        while(len>0 && ((*pc)==0x20 || (*pc==0x09))) // ignore spaces
        {
            --len;
            ++pc;
        }
    }
    while(len>0);

    return true;
}

MadSearchResult MadEdit::FindHexNext(const wxString &hexstr,
                                     wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
    vector<wxByte> hex;
    if(!StringToHex(hexstr, hex))
        return SR_EXPR_ERROR;

    MadCaretPos bpos, epos;
    MadUCQueue ucharQueue;
    vector<int> widthArray;
    int tmp;

    if(rangeFrom == -1)
    {
        bpos=m_CaretPos;
    }
    else
    {
        bpos.pos = rangeFrom;
        UpdateCaretByPos(bpos, ucharQueue, widthArray, tmp);
    }

    if(rangeTo == -1)
    {
        epos.iter=m_Lines->m_LineList.end();
        --epos.iter;
        epos.linepos=epos.iter->m_Size;
        epos.pos=m_Lines->m_Size;
    }
    else
    {
        epos.pos = rangeTo;
        UpdateCaretByPos(epos, ucharQueue, widthArray, tmp);
    }

    if(SR_YES==SearchHex(bpos, epos, &(*hex.begin()), hex.size()))
    {
        SetSelection(bpos.pos, epos.pos);
        return SR_YES;
    }

    return SR_NO;
}

MadSearchResult MadEdit::FindHexPrevious(const wxString &hexstr,
                                         wxFileOffset rangeFrom, wxFileOffset rangeTo)
{
    vector<wxByte> hex;
    if(!StringToHex(hexstr, hex))
        return SR_EXPR_ERROR;

    MadCaretPos bpos, epos;

    if(rangeFrom == -1)
    {
        epos=m_CaretPos;
    }
    else
    {
        MadUCQueue ucharQueue;
        vector<int> widthArray;
        int tmp;
        epos.pos = rangeFrom;
        UpdateCaretByPos(epos, ucharQueue, widthArray, tmp);
    }

    if(rangeTo < 0) rangeTo = 0;

    if(rangeFrom <= rangeTo) return SR_NO;

    size_t searchsize= 50 * 1024;
    if(searchsize< hexstr.Len()*8) searchsize=hexstr.Len()*8;

    wxFileOffset startpos=epos.pos;

    for(;;)
    {
        startpos -= searchsize;
        if(startpos<rangeTo)
        {
            startpos=rangeTo;
        }
        else
        {
            size_t s=size_t(startpos);
            s=s%4;
            startpos-=s;
        }

        wxFileOffset len=epos.pos-startpos;
        if(epos.linepos >= len)
        {
            bpos.pos=epos.pos-len;
            bpos.iter=epos.iter;
            bpos.linepos=epos.linepos-len;
        }
        else
        {
            bpos.pos=startpos;
            GetLineByPos(bpos.iter, bpos.pos, bpos.rowid);
            bpos.linepos=0;
            startpos=bpos.pos;
        }

        MadCaretPos bpos1=bpos, epos1=epos;
        if(SR_YES==SearchHex(bpos1, epos1, &(*hex.begin()), hex.size())) // found
        {
            MadCaretPos bp, ep;
            do
            {
                bp=bpos1;
                ep=epos1;

                // advance one byte
                if(bpos1.linepos==bpos1.iter->m_Size)
                {
                    ++bpos1.iter;
                    bpos1.linepos=0;
                }
                bpos1.pos++;
                bpos1.linepos++;

                epos1=epos;
            }
            while(SearchHex(bpos1, epos1, &(*hex.begin()), hex.size()));

            SetSelection(bp.pos, ep.pos, true);
            return SR_YES;
        }

        // not found
        if(startpos<=rangeTo) break;

        epos=bpos;
        size_t s=hexstr.Len()*4;

        if(epos.pos+ s > epos1.pos)
        {
            s=hexstr.Len();
            if(epos.pos+ s > epos1.pos)
            {
                s=0;
            }
        }

        if(s!=0)
        {
            epos.pos+=s;
            do
            {
                len=epos.iter->m_Size-epos.linepos;
                if(len>s) len=s;

                if((epos.linepos+=len)==epos.iter->m_Size)
                {
                    epos.linepos=0;
                    epos.iter++;
                }
            }
            while((s-=len) > 0);
        }

    }

    return SR_NO;
}

bool MadEdit::ReplaceText(const wxString &expr, const wxString &fmt,
                          bool bRegex, bool bCaseSensitive, bool bWholeWord)
{
    if(expr.Len()==0)
        return true;

    bool selok=false;

    MadCaretPos bpos=*m_SelectionBegin;
    MadCaretPos epos=*m_SelectionEnd;

    if(m_Selection) // test the selection is wanted text
    {
        int state=Search(bpos, epos, expr, bRegex, bCaseSensitive, bWholeWord);

        if(state==SR_EXPR_ERROR)
        {
            return false;
        }

        if(state==SR_YES)
        {
            if(bpos.pos==m_SelectionBegin->pos && epos.pos==m_SelectionEnd->pos)
            {
                selok=true;
            }
        }
    }

    if(!selok)  // just find next
    {
        return SR_EXPR_ERROR!=FindTextNext(expr, bRegex, bCaseSensitive, bWholeWord, false);
    }

    ucs4string out;

    // replace the selected text
    int state=Replace(out, bpos, epos, expr, fmt, bRegex, bCaseSensitive, bWholeWord);

    if(state==SR_EXPR_ERROR)
    {
        return false;
    }

    if(out.length()==0)
    {
        DeleteSelection(true, NULL, false);
    }
    else
    {
        InsertString(out.c_str(), out.length(), false, true, false);
    }

    FindTextNext(expr, bRegex, bCaseSensitive, bWholeWord, false);

    return true;
}

bool MadEdit::ReplaceHex(const wxString &expr, const wxString &fmt)
{
    if(expr.Len()==0)
        return true;

    bool selok=false;

    vector<wxByte> hex;
    if(!StringToHex(expr, hex)) return false;

    vector<wxByte> fmthex;
    if(!StringToHex(fmt, fmthex)) return false;


    MadCaretPos bpos=*m_SelectionBegin;
    MadCaretPos epos=*m_SelectionEnd;

    if(m_Selection) // test the selection is wanted text
    {
        int state=SearchHex(bpos, epos, &(*hex.begin()), hex.size());

        if(state==SR_EXPR_ERROR)
        {
            return false;
        }

        if(state==SR_YES)
        {
            if(bpos.pos==m_SelectionBegin->pos && epos.pos==m_SelectionEnd->pos)
            {
                selok=true;
            }
        }
    }

    if(!selok)  // just find next
    {
        FindHexNext(expr, false);
        return true;
    }

    if(fmthex.size()==0)
    {
        DeleteSelection(true, NULL, false);
    }
    else
    {
        InsertHexData(&(*fmthex.begin()), fmthex.size());
    }

    FindHexNext(expr, false);

    return true;
}


int MadEdit::ReplaceTextAll(const wxString &expr, const wxString &fmt,
    bool bRegex, bool bCaseSensitive, bool bWholeWord,
    vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos)
{
    if(expr.Len()==0)
        return 0;

    vector<wxFileOffset> del_bpos;
    vector<wxFileOffset> del_epos;
    vector<const ucs4_t*> ins_ucs;
    vector<wxFileOffset> ins_len;

    list<ucs4string> outs;

    MadCaretPos bpos, epos, endpos;

    bpos.iter=m_Lines->m_LineList.begin();
    bpos.pos=bpos.iter->m_RowIndices[0].m_Start;
    if(m_CaretPos.pos < bpos.pos || m_EditMode==emHexMode)
    {
        bpos.pos=0;
    }
    bpos.linepos=bpos.pos;

    epos.pos=m_Lines->m_Size;
    epos.iter=m_Lines->m_LineList.end();
    --epos.iter;
    epos.linepos=epos.iter->m_Size;

    endpos=epos;
    int multi=0;
    int state;

    while((state=Search(bpos, epos, expr, bRegex, bCaseSensitive, bWholeWord))==SR_YES)
    {
        ucs4string out;
        int state=Replace(out, bpos, epos, expr, fmt, bRegex, bCaseSensitive, bWholeWord);

        if(state==SR_EXPR_ERROR)
        {
            return SR_EXPR_ERROR;
        }

        del_bpos.push_back(bpos.pos);
        del_epos.push_back(epos.pos);

        outs.push_back(out);
        ucs4string &str=outs.back();
        ins_ucs.push_back( str.c_str() );
        ins_len.push_back( str.length() );

        if(bpos.iter!=epos.iter)
            ++multi;

        bpos=epos;
        epos=endpos;
    }

    if(state==SR_EXPR_ERROR) return SR_EXPR_ERROR;

    int count=int(del_bpos.size());

    if(count>0)
    {
        wxFileOffset size=del_epos.back() - del_bpos.front();
        if((size <= 2*1024*1024) || (multi>=40 && size<= 10*1024*1024))
        {
            OverwriteDataSingle(del_bpos, del_epos, &ins_ucs, NULL, ins_len);
        }
        else
        {
            OverwriteDataMultiple(del_bpos, del_epos, &ins_ucs, NULL, ins_len);
        }

        if(pbegpos!=0 && pendpos!=0)
        {
            pbegpos->resize(count);
            pendpos->resize(count);

            wxFileOffset diff=0, b, e, l;
            for(int i=0; i<count; i++)
            {
                b = del_bpos[i];
                e = del_epos[i];
                l = ins_len[i];
                size = b + diff;
                (*pbegpos)[i] = size;
                (*pendpos)[i] = size + l;
                diff += (l - (e - b));
            }
        }
    }

    return count;
}

int MadEdit::ReplaceHexAll(const wxString &expr, const wxString &fmt,
    vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos)
{
    if(expr.Len()==0)
        return 0;

    vector<wxByte> hex;
    if(!StringToHex(expr, hex)) return SR_EXPR_ERROR;

    vector<wxByte> fmthex;
    if(!StringToHex(fmt, fmthex)) return SR_EXPR_ERROR;

    vector<wxFileOffset> del_bpos;
    vector<wxFileOffset> del_epos;
    vector<wxByte*> ins_data;
    vector<wxFileOffset> ins_len;

    MadCaretPos bpos, epos, endpos;
    bpos.pos=0;
    bpos.iter=m_Lines->m_LineList.begin();
    bpos.linepos=0;

    epos.pos=m_Lines->m_Size;
    epos.iter=m_Lines->m_LineList.end();
    --epos.iter;
    epos.linepos=epos.iter->m_Size;

    endpos=epos;
    int multi=0;

    while(SearchHex(bpos, epos, &(*hex.begin()), hex.size())==SR_YES)
    {
        del_bpos.push_back(bpos.pos);
        del_epos.push_back(epos.pos);
        ins_data.push_back(&(*fmthex.begin()));
        ins_len.push_back(fmthex.size());

        if(bpos.iter!=epos.iter)
            ++multi;

        bpos=epos;
        epos=endpos;
    }

    int count=int(del_bpos.size());

    if(count>0)
    {
        wxFileOffset size=del_epos.back() - del_bpos.front();
        if(IsTextFile() && ((size <= 2*1024*1024) || (multi>=40 && size<= 10*1024*1024)))
        {
            OverwriteDataSingle(del_bpos, del_epos, NULL, &ins_data, ins_len);
        }
        else
        {
            OverwriteDataMultiple(del_bpos, del_epos, NULL, &ins_data, ins_len);
        }

        if(pbegpos!=0 && pendpos!=0)
        {
            pbegpos->resize(count);
            pendpos->resize(count);

            wxFileOffset diff=0, b, e, l;
            for(int i=0; i<count; i++)
            {
                b = del_bpos[i];
                e = del_epos[i];
                l = ins_len[i];
                size = b + diff;
                (*pbegpos)[i] = size;
                (*pendpos)[i] = size + l;
                diff += (l - (e - b));
            }
        }
    }

    return count;
}

int MadEdit::FindTextAll(const wxString &expr,
                         bool bRegex, bool bCaseSensitive, bool bWholeWord, bool bFirstOnly,
                         vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos)
{
    if(expr.Len()==0)
        return 0;

    MadCaretPos bpos, epos, endpos;

    bpos.iter=m_Lines->m_LineList.begin();
    bpos.pos=bpos.iter->m_RowIndices[0].m_Start;
    if(m_CaretPos.pos < bpos.pos || m_EditMode==emHexMode)
    {
        bpos.pos=0;
    }
    bpos.linepos=bpos.pos;

    epos.pos=m_Lines->m_Size;
    epos.iter=m_Lines->m_LineList.end();
    --epos.iter;
    epos.linepos=epos.iter->m_Size;

    endpos=epos;
    int count=0;
    int state;

    while((state=Search(bpos, epos, expr, bRegex, bCaseSensitive, bWholeWord))==SR_YES)
    {
        if(pbegpos) pbegpos->push_back(bpos.pos);
        if(pendpos) pendpos->push_back(epos.pos);
        ++count;
        if(bFirstOnly) break;

        bpos=epos;
        epos=endpos;
    }

    if(state==SR_EXPR_ERROR) return SR_EXPR_ERROR;

    return count;
}

int MadEdit::FindHexAll(const wxString &expr, bool bFirstOnly,
                        vector<wxFileOffset> *pbegpos, vector<wxFileOffset> *pendpos)
{
    if(expr.Len()==0)
        return 0;

    vector<wxByte> hex;
    if(!StringToHex(expr, hex)) return SR_EXPR_ERROR;

    MadCaretPos bpos, epos, endpos;
    bpos.pos=0;
    bpos.iter=m_Lines->m_LineList.begin();
    bpos.linepos=0;

    epos.pos=m_Lines->m_Size;
    epos.iter=m_Lines->m_LineList.end();
    --epos.iter;
    epos.linepos=epos.iter->m_Size;

    endpos=epos;
    int count=0;

    while(SearchHex(bpos, epos, &(*hex.begin()), hex.size())==SR_YES)
    {
        if(pbegpos) pbegpos->push_back(bpos.pos);
        if(pendpos) pendpos->push_back(epos.pos);
        ++count;
        if(bFirstOnly) break;

        bpos=epos;
        epos=endpos;
    }

    return count;
}








/******** Printing Functions ********/
void MadEdit::BeginPrint(const wxRect &printRect)
{
    if(InPrinting()) return;

    //SetReadOnly(true);
    // store settings
    m_old_ClientWidth       = m_ClientWidth;
    m_old_ClientHeight      = m_ClientHeight;
    m_old_WordWrapMode      = m_WordWrapMode;
    m_old_Selection         = m_Selection;
    m_old_DisplayLineNumber = m_DisplayLineNumber;
    m_old_ShowEndOfLine     = m_ShowEndOfLine;
    m_old_ShowSpaceChar     = m_ShowSpaceChar;
    m_old_ShowTabChar       = m_ShowTabChar;
    m_old_LeftMarginWidth   = m_LeftMarginWidth;
    m_old_DrawingXPos       = m_DrawingXPos;

    // apply printing settings
    m_PrintRect=printRect;

    // calc page count
    if(m_EditMode!=emHexMode) // Print Text
    {
        m_Printing=-1;
        m_ClientWidth=m_PrintRect.width + m_RightMarginWidth;
        m_ClientHeight=m_PrintRect.height;
        m_WordWrapMode=wwmWrapByWindow;
        m_DrawingXPos=0;

        m_PrintSyntax=false;

        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/MadEdit"));
        m_Config->Read(wxT("PrintSyntax"), &m_PrintSyntax);
        m_Config->Read(wxT("PrintLineNumber"), &m_DisplayLineNumber);
        m_Config->Read(wxT("PrintEndOfLine"), &m_ShowEndOfLine);
        m_Config->Read(wxT("PrintTabChar"), &m_ShowSpaceChar);
        m_Config->Read(wxT("PrintSpaceChar"), &m_ShowTabChar);
        m_Config->SetPath(oldpath);

        m_Syntax->BeginPrint(m_PrintSyntax);
        if(!m_DisplayLineNumber) m_LeftMarginWidth=0;

        ReformatAll();

        m_Selection=false;

        m_RowCountPerPage = m_ClientHeight / m_RowHeight;
        m_PrintPageCount = int(m_Lines->m_RowCount) / m_RowCountPerPage;
        if(m_RowCountPerPage==0)
            ++m_RowCountPerPage;

        if(int(m_Lines->m_RowCount) > (m_PrintPageCount * m_RowCountPerPage))
            ++m_PrintPageCount;
    }
    else // Print Hex
    {
        m_Printing=1;
        m_old_CaretAtHexArea=m_CaretAtHexArea;
        m_CaretAtHexArea=true;

        wxString oldpath=m_Config->GetPath();
        m_Config->SetPath(wxT("/MadEdit"));
        m_Config->Read(wxT("PrintOffsetHeader"), &m_PrintOffsetHeader);
        m_Config->SetPath(oldpath);

        // use a temporary MadEdit to print Hex-Data
        m_PrintHexEdit=new MadEdit(this, -1, wxPoint(-999,-999));
        m_PrintHexEdit->m_StorePropertiesToGlobalConfig = false;

        // set properties of m_PrintHexEdit
        m_PrintHexEdit->m_Printing=1;
        m_PrintHexEdit->SetEncoding(wxT("UTF-32LE"));
        m_PrintHexEdit->SetLineSpacing(100);
        m_PrintHexEdit->SetDisplayLineNumber(false);
        m_PrintHexEdit->SetShowEndOfLine(false);
        m_PrintHexEdit->SetShowSpaceChar(false);
        m_PrintHexEdit->SetShowTabChar(false);

        m_PrintHexEdit->SetFixedWidthMode(true);
        m_PrintHexEdit->SetTextFont(m_HexFont->GetFaceName(), m_HexFont->GetPointSize(), true);
        m_PrintHexEdit->SetWordWrapMode(wwmWrapByColumn);

        m_PrintHexEdit->m_Syntax->BeginPrint(false);

        m_PrintHexEdit->m_LineNumberAreaWidth = 0;
        m_PrintHexEdit->m_LeftMarginWidth = 0;
        m_PrintHexEdit->m_RightMarginWidth = 0;

        // In Hex-Printing mode, every char is only one maxdigit width.
        // calc max columns of one line
        int maxcols=printRect.width / m_PrintHexEdit->m_TextFontMaxDigitWidth;
        if(maxcols==0) maxcols=1;

        const int MaxColumns=76;
        if(MaxColumns <= maxcols)
        {
            m_RowCountPerHexLine=1;
        }
        else
        {
            m_RowCountPerHexLine = MaxColumns / maxcols;
            if(m_RowCountPerHexLine*maxcols < MaxColumns)
                ++m_RowCountPerHexLine;
        }

        m_PrintHexEdit->SetMaxColumns(maxcols);

        // calc m_PrintPageCount
        m_RowCountPerPage = printRect.height / m_RowHeight;
        if(m_RowCountPerPage==0)
            ++m_RowCountPerPage;

        m_HexLineCountPerPage=m_RowCountPerPage/m_RowCountPerHexLine;
        if(m_HexLineCountPerPage==0)
            ++m_HexLineCountPerPage;

        if(m_Lines->m_Size==0) // no data
        {
            m_PrintTotalHexLineCount=0;
            m_PrintPageCount=1;
        }
        else
        {
            m_PrintTotalHexLineCount=m_Lines->m_Size >> 4; // /16
            if((size_t(m_Lines->m_Size) & 0x0F)!=0)
                ++m_PrintTotalHexLineCount;

            switch(m_PrintOffsetHeader)
            {
            case 0: // do not print OffsetHeader
                m_PrintPageCount= m_PrintTotalHexLineCount/m_HexLineCountPerPage;
                if(m_PrintPageCount*m_HexLineCountPerPage < m_PrintTotalHexLineCount)
                    ++m_PrintPageCount;
                break;
            case 1: // print OffsetHeader on first page only
                m_PrintPageCount= (m_PrintTotalHexLineCount+1)/m_HexLineCountPerPage;
                if(m_PrintPageCount*m_HexLineCountPerPage < (m_PrintTotalHexLineCount+1))
                    ++m_PrintPageCount;
                break;
            case 2: // print OffsetHeader on every page
                if(m_RowCountPerPage==1)
                {
                    m_PrintPageCount=m_PrintTotalHexLineCount;
                }
                else
                {
                    m_PrintPageCount= m_PrintTotalHexLineCount/(m_HexLineCountPerPage-1);
                    if(m_PrintPageCount*(m_HexLineCountPerPage-1) < m_PrintTotalHexLineCount)
                        ++m_PrintPageCount;
                }
                break;
            }
        }
    }
}

void MadEdit::EndPrint()
{
    if(!InPrinting()) return;

    // restore settings
    m_ClientWidth= m_old_ClientWidth;
    m_ClientHeight=m_old_ClientHeight;
    m_WordWrapMode=m_old_WordWrapMode;
    m_Selection=m_old_Selection;

    if(TextPrinting())
    {
        m_DisplayLineNumber = m_old_DisplayLineNumber;
        m_ShowEndOfLine     = m_old_ShowEndOfLine;
        m_ShowSpaceChar     = m_old_ShowSpaceChar;
        m_ShowTabChar       = m_old_ShowTabChar;
        m_LeftMarginWidth   = m_old_LeftMarginWidth;
        m_DrawingXPos       = m_old_DrawingXPos;

        m_Syntax->EndPrint();

        ReformatAll();
    }
    else //HexPrinting()
    {
        m_CaretAtHexArea=m_old_CaretAtHexArea;
        delete m_PrintHexEdit;
        m_PrintHexEdit=NULL;
    }

    m_Printing=0;
    m_PrintPageCount=0;

    m_RepaintAll=true;
    Refresh(false);

    //SetReadOnly(false);
}

bool MadEdit::PrintPage(wxDC *dc, int pageNum)
{
    if(!InPrinting() || !(pageNum>=1 && pageNum<=m_PrintPageCount))
        return false;

    if(TextPrinting())
    {
        int rowcount=m_RowCountPerPage;
        if(pageNum*rowcount > int(m_Lines->m_RowCount))
        {
            rowcount -= (pageNum*rowcount - int(m_Lines->m_RowCount));
        }

        int toprow=(pageNum-1)*m_RowCountPerPage;

        MadLineIterator lit;
        wxFileOffset tmppos;
        int rowid=toprow;
        int lineid=GetLineByRow(lit, tmppos, rowid);

        // update m_LineNumberAreaWidth
        if(m_DisplayLineNumber)
        {
            m_LineNumberAreaWidth=CalcLineNumberAreaWidth(lit, lineid, rowid, toprow, rowcount);
        }
        else
        {
            m_LineNumberAreaWidth = 0;
        }

        PaintTextLines(dc, m_PrintRect, toprow, rowcount, *wxWHITE);

        if(m_DisplayLineNumber && !m_PrintSyntax)
        {
            // draw a line between LineNumberArea and Text
            dc->SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxSOLID));
            int x1=m_PrintRect.x+m_LineNumberAreaWidth;
            dc->DrawLine(x1, m_PrintRect.y, x1, m_PrintRect.y+(rowcount*m_RowHeight));
        }
    }
    else //HexPrinting()
    {
        int toprow, rowcount;

        switch(m_PrintOffsetHeader)
        {
        case 0: // do not print OffsetHeader
            toprow = (pageNum-1) * m_HexLineCountPerPage;
            rowcount=m_HexLineCountPerPage;
            if(pageNum==m_PrintPageCount && m_HexLineCountPerPage>1)// last page
            {
                rowcount=m_PrintTotalHexLineCount-toprow;
            }
            break;
        case 1: // print OffsetHeader on first page only
            if(m_HexLineCountPerPage==1)
            {
                toprow=(pageNum-1);
                rowcount=1;
            }
            else
            {
                if(pageNum==1)
                {
                    toprow=0;
                    rowcount=m_HexLineCountPerPage-1;
                }
                else
                {
                    toprow = (pageNum-1) * m_HexLineCountPerPage -1;
                    rowcount=m_HexLineCountPerPage;
                }
                if(pageNum==m_PrintPageCount)
                {
                    rowcount=m_PrintTotalHexLineCount-toprow;
                }
            }
            break;
        case 2: // print OffsetHeader on every page
            if(m_HexLineCountPerPage==1)
            {
                toprow=(pageNum-1);
                rowcount=1;
            }
            else
            {
                rowcount = m_HexLineCountPerPage-1;
                toprow = (pageNum-1) * rowcount;
                if(pageNum==m_PrintPageCount)// last page
                {
                    rowcount=m_PrintTotalHexLineCount-toprow;
                }
            }
            break;
        }

        PrepareHexRowIndex(toprow, rowcount);

        wxString lines;
        if(m_PrintOffsetHeader==2 || (m_PrintOffsetHeader==1&&pageNum==1))
        {
            for(int i=0;i<76;i++)
                lines << wxChar(HexHeader[i]);

            lines << wxT('\n');
        }

        MadLineIterator lit, lineend = m_Lines->m_LineList.end();
        int rn;
        wxFileOffset pos;
        MadUCQueue ucqueue;
        MadLines::NextUCharFuncPtr NextUChar= m_Lines->NextUChar;

        wxFileOffset hexrowpos = size_t(toprow)<<4;
        wxString offset(wxT("12345678"));
        for(int rowidx=0;rowidx<rowcount;rowidx++, hexrowpos+=16)// for every hex-line
        {
            // paint offset
            size_t hex = size_t(hexrowpos);
            for(int i = 7; i >= 0; --i)
            {
                offset[i]=wxChar(ToHex(int(hex & 0x0F)));
                hex >>= 4;
            }
            lines << offset;
            lines << wxT(": ");

            // paint Binary Data
            pos=hexrowpos;
            GetLineByPos(lit, pos, rn);
            pos=hexrowpos-pos;
            int idx = 0;
            for(int i = 0; i < 16; i++)
            {
                if(pos == lit->m_Size)       // to next line
                {
                    if((++lit) == lineend || lit->m_Size == 0)
                    {
                        break;
                    }
                    pos = 0;
                }

                hex = lit->Get(pos++);

                lines << wxChar(ToHex(int(hex >> 4)));
                lines << wxChar(ToHex(int(hex & 0x0F)));
                lines << wxChar(0x20);
                idx+=3;
            }

            if(idx < 16 * 3)
                do
                {
                    lines << wxChar(0x20);
                }
                while(++idx < 16 * 3);

            lines << wxT("| ");

            // paint Text Data
            ucqueue.clear();
            wxFileOffset rowpos = m_HexRowIndex[rowidx];
            if(rowpos>hexrowpos)
            {
                idx=rowpos-hexrowpos;
                if(idx>0)
                {
                    do
                    {
                        lines << wxChar(0x20);  // append space
                    }while(--idx>0);
                }
            }
            pos = rowpos;
            GetLineByPos(lit, pos, rn);
            pos = rowpos - pos;
            const wxFileOffset hexrowpos16=hexrowpos+16;
            m_Lines->InitNextUChar(lit, pos);
            do
            {
                if(!(m_Lines->*NextUChar)(ucqueue))
                {
                    if(++lit == lineend || lit->m_Size == 0)
                    {
                        break;
                    }
                    m_Lines->InitNextUChar(lit, 0);
                    (m_Lines->*NextUChar)(ucqueue);
                }

                MadUCPair &ucp=ucqueue.back();
                rowpos+=ucp.second;
                if(ucp.first<=0x20)
                {
                    lines << wxT('.');
                }
                else
                {
#ifdef __WXMSW__
                    if(ucp.first<0x10000)
                    {
                        lines << wxChar(ucp.first);
                    }
                    else
                    {
                        wchar_t wbuf[2];
                        m_Encoding->UCS4toUTF16LE_U10000(ucp.first, (wxByte*)wbuf);
                        lines << wbuf[0];
                        lines << wbuf[1];
                    }
#else
                    lines << wxChar(ucp.first);
#endif
                }

                idx=ucp.second-1;
                if(idx>0 && rowpos<hexrowpos16)
                {
                    do
                    {
                        lines << wxChar(0x20);  // append space
                    }while(--idx>0);
                }
            }
            while(rowpos<hexrowpos16);

            lines << wxT('\n');
        }

        m_PrintHexEdit->SetText(lines);

        m_PrintHexEdit->m_DrawingXPos=0;

        m_PrintHexEdit->PaintTextLines(dc, m_PrintRect,
            0, int(m_PrintHexEdit->m_Lines->m_RowCount), *wxWHITE);
    }

    return true;
}

wxString FormatThousands(const wxString& s) 
{ 
    /* 
    // example: 
    int mynumber = 12345678; 
    wxString s = wxString::Format("%d", mynumber); // format the integer to string 
    s = FormatThousands(s); // add separators 
    // s now contains "12,345,678" or "12.345.678" according to locale. 
    */

    static wxString thousandssep = wxT(","); 
    static struct lconv *loc = 0; 
    if (!loc) { 
        loc = localeconv(); 
        if (loc && loc->thousands_sep && loc->thousands_sep[0]) 
        {
#if wxUSE_UNICODE
            thousandssep = wxString(loc->thousands_sep, wxConvLibc);
#else
            thousandssep = loc->thousands_sep;
#endif
        }
    } 

    wxString in = s, out; 
    while (in.Length() > 3) { 
            out.Prepend(thousandssep + in.Right(3)); 
            in.RemoveLast(3); 
    } 
    if (!in.IsEmpty()) 
            out.Prepend(in); 
    return out; 
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
