///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/inframe_wxmedit.cpp
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "inframe_wxmedit.h"
#include "simple_wxmedit.h"
#include "wxm_encoding/unicode.h"
#include "../wxmedit_frame.h"
#include "../dialog/wxm_search_replace_dialog.h"
#include "xm_utils.hpp"

#include <wx/aui/auibook.h>
#include <wx/filename.h>

#include <boost/assign/list_of.hpp>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern wxm::InFrameWXMEdit* g_active_wxmedit;
extern wxMenu *g_Menu_Edit;
extern const ucs4_t HexHeader[];

namespace wxm
{

struct GlobalConfigWriter: public ConfigWriter
{
	virtual void Record(const wxString& key, long val);
	virtual void Record(const wxString& key, double val);
	virtual void Record(const wxString& key, const wxString& val);

	virtual void SetConfig(wxConfigBase* cfg) { m_cfg = cfg; }

	GlobalConfigWriter(): m_cfg(NULL) {}
private:
	wxConfigBase* m_cfg;
};

void GlobalConfigWriter::Record(const wxString& key, long val)
{
	wxString oldpath = m_cfg->GetPath();
	m_cfg->Write(key, val);
	m_cfg->SetPath(oldpath);
}

void GlobalConfigWriter::Record(const wxString& key, double val)
{
	wxString oldpath = m_cfg->GetPath();
	m_cfg->Write(key, val);
	m_cfg->SetPath(oldpath);
}

void GlobalConfigWriter::Record(const wxString& key, const wxString& val)
{
	wxString oldpath = m_cfg->GetPath();
	m_cfg->Write(key, val);
	m_cfg->SetPath(oldpath);
}

InFrameWXMEdit::InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: MadEdit(new GlobalConfigWriter(), parent, id, pos, size, style)
{
	m_PrintPageCount = 0;

	long mode;
	m_Config->Read(wxT("WordWrapMode"), &mode, (long)wwmNoWrap);
	m_WordWrapMode = (MadWordWrapMode)mode;

	bool has_linenum = true;
	m_Config->Read(wxT("DisplayLineNumber"), &has_linenum, true);
	SetLineNumberVisible(has_linenum);

	bool bookmark_visible = true;
	m_Config->Read(wxT("DisplayBookmark"), &bookmark_visible, true);
	SetBookmarkVisible(bookmark_visible);

	InitTextFont();
	InitHexFont();

	m_LineNumberAreaWidth = GetLineNumberAreaWidth(0);

	SetWindowStyleFlag(GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
	//SetDropTarget(new DnDFile());

	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadEditFrame::MadEditFrameKeyDown));
}

wxString InserModeText(bool insertmode)
{
	static wxString insstr(_("INS"));
	static wxString ovrstr(_("OVR"));
	return insertmode? insstr: ovrstr;
}

wxString ReadOnlyText(bool readonly)
{
	static wxString rostr(_("ReadOnly"));
	return readonly ? rostr : wxString();
}

wxString BOMText(bool hasbom)
{
	static wxString bom(wxT(".BOM"));
	return hasbom ? bom : wxString();
}

wxString NewLineTypeText(MadNewLineType nlty)
{
	static std::map<int, wxString> nltytxts = boost::assign::map_list_of
			(nltDOS,  wxString(wxT(".DOS")))
			(nltUNIX, wxString(wxT(".UNIX")))
			(nltMAC,  wxString(wxT(".MAC")))
		;
	return xm::wrap_map(nltytxts).get((int)nlty, wxEmptyString);
}

void InFrameWXMEdit::DoSelectionChanged()
{
	g_MainFrame->m_Notebook->ConnectMouseClick();

	if (this != g_active_wxmedit)
		return;

	int line, subrow;
	wxFileOffset col;
	GetCaretPosition(line, subrow, col);
	if (GetEditMode() != emHexMode)
	{
		++line;
		++col;
	}

	wxString s1 = FormatThousands(wxString::Format(wxT("%d"), line));
	wxString s2 = FormatThousands(wxString::Format(wxT("%u"), GetLineCount()));
	wxString s4 = FormatThousands(wxLongLong(col).ToString());

	static wxString lnstr(_("Ln:"));
	static wxString sepstr(wxT(" /"));
	static wxString sepstr1(wxT(" ("));
	static wxString substr(_("Sub:"));
	static wxString sepstr2(wxT(')'));
	static wxString sepstr3(wxT(' '));
	static wxString colstr(_("Col:"));
	static wxString fpstr(_("CharPos:"));
	static wxString ssstr(_("SelSize:"));

	wxString text = lnstr + s1 + sepstr + s2;
	if (subrow>0)
	{
		wxString s3 = FormatThousands(wxString::Format(wxT("%d"), subrow + 1));
		text += (sepstr1 + substr + s3 + sepstr2);
	}
	text += (sepstr3 + colstr + s4);
	wxm::GetFrameStatusBar().SetField(wxm::STBF_ROWCOL, text);

	s1 = FormatThousands(wxLongLong(GetCaretPosition()).ToString());
	s2 = FormatThousands(wxLongLong(GetFileSize()).ToString());
	wxm::GetFrameStatusBar().SetField(wxm::STBF_CHARPOS, fpstr + s1 + sepstr + s2);

	s1 = FormatThousands(wxLongLong(GetSelectionSize()).ToString());
	wxm::GetFrameStatusBar().SetField(wxm::STBF_SELECTION, ssstr + s1);

	wxm::GetFrameStatusBar().Update(); // repaint immediately
}

void InFrameWXMEdit::DoStatusChanged()
{
	g_MainFrame->m_Notebook->ConnectMouseClick();

	// check the title is changed or not
	int selid = GetIdByEdit(this);

	wxString oldtitle = g_MainFrame->m_Notebook->GetPageText(selid);

	wxString filename = GetFileName();
	wxString title = (filename.IsEmpty()) ? oldtitle : wxFileName(filename).GetFullName();

	if (IsModified())
	{
		if (*title.rbegin() != wxT('*'))
			title += wxT('*');

		if (filename.IsEmpty())
			filename = title;
		else
			filename += wxT('*');
	}
	else
	{
		if (!title.IsEmpty() && *title.rbegin() == wxT('*'))
			title.Truncate(title.Len() - 1);
	}

	if (title != oldtitle)
	{
		g_MainFrame->m_Notebook->SetPageText(selid, title);
	}

	if (this != g_active_wxmedit)
		return;

	if (filename.IsEmpty())
		filename = title;
	g_MainFrame->SetTitle(wxString(wxT("wxMEdit - [")) + filename + wxString(wxT("] ")));

	wxString encfmt = GetEncodingName();

	encfmt += BOMText(HasBOM());

	encfmt += NewLineTypeText(GetNewLineType());

	wxm::GetFrameStatusBar().SetField(wxm::STBF_ENCFMT, encfmt);

	wxm::GetFrameStatusBar().SetField(wxm::STBF_READONLY, ReadOnlyText(IsReadOnly()));

	wxm::GetFrameStatusBar().SetField(wxm::STBF_INSOVR, InserModeText(IsInsertMode()));

	wxm::GetFrameStatusBar().Update(); // repaint immediately

	if (g_SearchReplaceDialog != NULL)
	{
		g_SearchReplaceDialog->UpdateCheckBoxByCBHex(g_SearchReplaceDialog->WxCheckBoxFindHex->GetValue());
	}
}

void InFrameWXMEdit::DoToggleWindow()
{
	wxCommandEvent e(0, 0);
	g_MainFrame->OnWindowToggleWindow(e);
}

void InFrameWXMEdit::DoMouseRightUp()
{
	g_MainFrame->PopupMenu(g_Menu_Edit);
}

void InFrameWXMEdit::SetWordWrapMode(MadWordWrapMode mode)
{
	if (m_WordWrapMode == mode)
		return;

	m_WordWrapMode = mode;
	m_cfg_writer->Record(wxT("/wxMEdit/WordWrapMode"), (long)mode);

	if (GetEditMode() != emHexMode)
	{
		if (mode == wwmWrapByWindow)
			m_DrawingXPos = 0;

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

int InFrameWXMEdit::CalcLineNumberAreaWidth(MadLineIterator lit, int lineid, int rowid, int toprow, int rowcount)
{
	int LineNumberAreaWidth = GetLineNumberAreaWidth(lineid);
	if (LineNumberAreaWidth == GetLineNumberAreaWidth(lineid + rowcount))
		return LineNumberAreaWidth;

	// find last lineid
	int count = rowcount + (toprow - rowid);
	++lineid;
	while ((count -= int(lit->RowCount())) > 0)
	{
		++lit;
		++lineid;
	}

	return GetLineNumberAreaWidth(lineid);
}

int InFrameWXMEdit::GetLineNumberAreaWidth(int number)
{
	if (!LineNumberAreaVisible())
		return 0;

	if (!m_linenum_visible)
		return m_TextFontMaxDigitWidth + m_TextFontMaxDigitWidth/4;

	int n = 2;
	while (number >= 100)
	{
		number /= 10;
		++n;
	}

	return (n * m_TextFontMaxDigitWidth) + (m_TextFontMaxDigitWidth / 4);
}

void InFrameWXMEdit::SetLineNumberVisible(bool visible)
{
	if (visible == m_linenum_visible)
		return;

	m_linenum_visible = visible;
	m_cfg_writer->Record(wxT("/wxMEdit/DisplayLineNumber"), visible);

	m_RepaintAll = true;
	Refresh(false);
}

void InFrameWXMEdit::SetBookmarkVisible(bool visible)
{
	if (visible == m_bookmark_visible)
		return;

	m_bookmark_visible = visible;
	m_cfg_writer->Record(wxT("/wxMEdit/DisplayBookmark"), visible);

	m_RepaintAll = true;
	Refresh(false);
}

void InFrameWXMEdit::BeginPrint(const wxRect &printRect)
{
	if (InPrinting()) return;

	//SetReadOnly(true);
	// store settings
	m_old_ClientWidth = m_ClientWidth;
	m_old_ClientHeight = m_ClientHeight;
	m_old_WordWrapMode = m_WordWrapMode;
	m_old_Selection = m_Selection;
	m_old_linenum_visible = m_linenum_visible;
	m_old_ShowEndOfLine = m_ShowEndOfLine;
	m_old_ShowSpaceChar = m_ShowSpaceChar;
	m_old_ShowTabChar = m_ShowTabChar;
	m_old_LeftMarginWidth = m_LeftMarginWidth;
	m_old_DrawingXPos = m_DrawingXPos;

	// apply printing settings
	m_PrintRect = printRect;

	// calc page count
	if (GetEditMode() == emHexMode)
		BeginHexPrinting();
	else
		BeginTextPrinting();
}

void InFrameWXMEdit::BeginTextPrinting()
{
	SetTextPrinting();
	m_ClientWidth = m_PrintRect.width + m_RightMarginWidth;
	m_ClientHeight = m_PrintRect.height;
	m_WordWrapMode = wwmWrapByWindow;
	m_DrawingXPos = 0;

	m_PrintSyntax = false;

	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath(wxT("/wxMEdit"));
	m_Config->Read(wxT("PrintSyntax"), &m_PrintSyntax);
	m_Config->Read(wxT("PrintLineNumber"), &m_linenum_visible);
	m_Config->Read(wxT("PrintBookmark"), &m_bookmark_visible);
	m_Config->Read(wxT("PrintEndOfLine"), &m_ShowEndOfLine);
	m_Config->Read(wxT("PrintTabChar"), &m_ShowSpaceChar);
	m_Config->Read(wxT("PrintSpaceChar"), &m_ShowTabChar);
	m_Config->SetPath(oldpath);

	BeginSyntaxPrint(m_PrintSyntax);
	if (!m_linenum_visible) m_LeftMarginWidth = 0;

	ReformatAll();

	m_Selection = false;

	m_RowCountPerPage = m_ClientHeight / m_RowHeight;
	m_PrintPageCount = int(m_Lines->m_RowCount) / m_RowCountPerPage;
	if (m_RowCountPerPage == 0)
		++m_RowCountPerPage;

	if (int(m_Lines->m_RowCount) > (m_PrintPageCount * m_RowCountPerPage))
		++m_PrintPageCount;
}

void InFrameWXMEdit::BeginHexPrinting()
{
	SetHexPrinting();
	m_old_CaretAtHexArea = m_CaretAtHexArea;
	m_CaretAtHexArea = true;

	wxString oldpath = m_Config->GetPath();
	m_Config->SetPath(wxT("/wxMEdit"));
	m_Config->Read(wxT("PrintOffsetHeader"), &m_PrintOffsetHeader);
	m_Config->SetPath(oldpath);

	// use a temporary MadEdit to print Hex-Data
	m_HexPrintWXMEdit = new wxm::HexPrintingWXMEdit(this, m_HexFont);

	// In Hex-Printing mode, every char is only one maxdigit width.
	// calc max columns of one line
	int maxcols = m_PrintRect.width / m_HexPrintWXMEdit->m_TextFontMaxDigitWidth;
	if (maxcols == 0) maxcols = 1;

	const int MaxColumns = 76;
	if (MaxColumns <= maxcols)
	{
		m_RowCountPerHexLine = 1;
	}
	else
	{
		m_RowCountPerHexLine = MaxColumns / maxcols;
		if (m_RowCountPerHexLine*maxcols < MaxColumns)
			++m_RowCountPerHexLine;
	}

	m_HexPrintWXMEdit->SetMaxColumns(maxcols);

	// calc m_PrintPageCount
	m_RowCountPerPage = m_PrintRect.height / m_RowHeight;
	if (m_RowCountPerPage == 0)
		++m_RowCountPerPage;

	m_HexLineCountPerPage = m_RowCountPerPage / m_RowCountPerHexLine;
	if (m_HexLineCountPerPage == 0)
		++m_HexLineCountPerPage;

	if (m_Lines->m_Size == 0) // no data
	{
		m_PrintTotalHexLineCount = 0;
		m_PrintPageCount = 1;
		return;
	}

	m_PrintTotalHexLineCount = m_Lines->m_Size >> 4; // /16
	if ((size_t(m_Lines->m_Size) & 0x0F) != 0)
		++m_PrintTotalHexLineCount;

	switch (m_PrintOffsetHeader)
	{
	case 0: // do not print OffsetHeader
		m_PrintPageCount = m_PrintTotalHexLineCount / m_HexLineCountPerPage;
		if (m_PrintPageCount*m_HexLineCountPerPage < m_PrintTotalHexLineCount)
			++m_PrintPageCount;
		break;
	case 1: // print OffsetHeader on first page only
		m_PrintPageCount = (m_PrintTotalHexLineCount + 1) / m_HexLineCountPerPage;
		if (m_PrintPageCount*m_HexLineCountPerPage < (m_PrintTotalHexLineCount + 1))
			++m_PrintPageCount;
		break;
	case 2: // print OffsetHeader on every page
		if (m_HexLineCountPerPage == 1)
		{
			m_PrintPageCount = m_PrintTotalHexLineCount;
		}
		else
		{
			m_PrintPageCount = m_PrintTotalHexLineCount / (m_HexLineCountPerPage - 1);
			if (m_PrintPageCount*(m_HexLineCountPerPage - 1) < m_PrintTotalHexLineCount)
				++m_PrintPageCount;
		}
		break;
	default:
		break;
	}
}

void InFrameWXMEdit::EndPrint()
{
	if (!InPrinting()) return;

	// restore settings
	m_ClientWidth = m_old_ClientWidth;
	m_ClientHeight = m_old_ClientHeight;
	m_WordWrapMode = m_old_WordWrapMode;
	m_Selection = m_old_Selection;

	if (TextPrinting())
		EndTextPrinting();
	else
		EndHexPrinting();

	SetNoPrinting();
	m_PrintPageCount = 0;

	m_RepaintAll = true;
	Refresh(false);
}

void InFrameWXMEdit::EndTextPrinting()
{
	m_linenum_visible = m_old_linenum_visible;
	m_ShowEndOfLine = m_old_ShowEndOfLine;
	m_ShowSpaceChar = m_old_ShowSpaceChar;
	m_ShowTabChar = m_old_ShowTabChar;
	m_LeftMarginWidth = m_old_LeftMarginWidth;
	m_DrawingXPos = m_old_DrawingXPos;

	EndSyntaxPrint();

	ReformatAll();
}

void InFrameWXMEdit::EndHexPrinting()
{
	m_CaretAtHexArea = m_old_CaretAtHexArea;
	delete m_HexPrintWXMEdit;
	m_HexPrintWXMEdit = NULL;
}

bool InFrameWXMEdit::PrintPage(wxDC *dc, int pageNum)
{
	if (!InPrinting() || !(pageNum >= 1 && pageNum <= m_PrintPageCount))
		return false;

	if (TextPrinting())
		PrintTextPage(dc, pageNum);
	else
		PrintHexPage(dc, pageNum);

	return true;
}

void InFrameWXMEdit::PrintTextPage(wxDC *dc, int pageNum)
{
	int rowcount = m_RowCountPerPage;
	if (pageNum*rowcount > int(m_Lines->m_RowCount))
	{
		rowcount -= (pageNum*rowcount - int(m_Lines->m_RowCount));
	}

	int toprow = (pageNum - 1)*m_RowCountPerPage;

	MadLineIterator lit;
	wxFileOffset tmppos;
	int rowid = toprow;
	int lineid = GetLineByRow(lit, tmppos, rowid);

	// update LineNumberAreaWidth cache
	CacheLineNumberAreaWidth(CalcLineNumberAreaWidth(lit, lineid, rowid, toprow, rowcount));

	PaintTextLines(dc, m_PrintRect, toprow, rowcount, *wxWHITE);

	if (!LineNumberAreaVisible() || m_PrintSyntax)
		return;

	// draw a line between LineNumberArea and Text
	dc->SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxSOLID));
	int x1 = m_PrintRect.x + CachedLineNumberAreaWidth();
	dc->DrawLine(x1, m_PrintRect.y, x1, m_PrintRect.y + (rowcount*m_RowHeight));
}

void InFrameWXMEdit::PrintHexPage(wxDC *dc, int pageNum)
{
	int toprow = 0/*?*/, rowcount = 1/*?*/;

	switch (m_PrintOffsetHeader)
	{
	case 0: // do not print OffsetHeader
		toprow = (pageNum - 1) * m_HexLineCountPerPage;
		rowcount = m_HexLineCountPerPage;
		if (pageNum == m_PrintPageCount && m_HexLineCountPerPage>1)// last page
		{
			rowcount = m_PrintTotalHexLineCount - toprow;
		}
		break;
	case 1: // print OffsetHeader on first page only
		if (m_HexLineCountPerPage == 1)
		{
			toprow = (pageNum - 1);
			rowcount = 1;
		}
		else
		{
			if (pageNum == 1)
			{
				toprow = 0;
				rowcount = m_HexLineCountPerPage - 1;
			}
			else
			{
				toprow = (pageNum - 1) * m_HexLineCountPerPage - 1;
				rowcount = m_HexLineCountPerPage;
			}
			if (pageNum == m_PrintPageCount)
			{
				rowcount = m_PrintTotalHexLineCount - toprow;
			}
		}
		break;
	case 2: // print OffsetHeader on every page
		if (m_HexLineCountPerPage == 1)
		{
			toprow = (pageNum - 1);
			rowcount = 1;
		}
		else
		{
			rowcount = m_HexLineCountPerPage - 1;
			toprow = (pageNum - 1) * rowcount;
			if (pageNum == m_PrintPageCount)// last page
			{
				rowcount = m_PrintTotalHexLineCount - toprow;
			}
		}
		break;
	}

	PrepareHexRowIndex(toprow, rowcount);

	wxString lines;
	if (m_PrintOffsetHeader == 2 || (m_PrintOffsetHeader == 1 && pageNum == 1))
	{
		for (int i = 0; i<76; i++)
			lines << wxChar(HexHeader[i]);

		lines << wxT('\n');
	}

	MadLineIterator lit, lineend = m_Lines->m_LineList.end();
	int rn;
	wxFileOffset pos;
	MadUCQueue ucqueue;

	wxFileOffset hexrowpos = wxFileOffset(toprow) << 4;
	wxString offset(wxT("12345678"));
	for (int rowidx = 0; rowidx<rowcount; rowidx++, hexrowpos += 16)// for every hex-line
	{
		// paint offset
		size_t hex = size_t(hexrowpos);
		for (int i = 7; i >= 0; --i)
		{
			offset[i] = wxChar(ToHex(int(hex & 0x0F)));
			hex >>= 4;
		}
		lines << offset;
		lines << wxT(": ");

		// paint Binary Data
		pos = hexrowpos;
		GetLineByPos(lit, pos, rn);
		pos = hexrowpos - pos;
		int idx = 0;
		for (int i = 0; i < 16; i++)
		{
			if (pos == lit->m_Size)       // to next line
			{
				if ((++lit) == lineend || lit->m_Size == 0)
				{
					break;
				}
				pos = 0;
			}

			hex = lit->Get(pos++);

			lines << wxChar(ToHex(int(hex >> 4)));
			lines << wxChar(ToHex(int(hex & 0x0F)));
			lines << wxChar(0x20);
			idx += 3;
		}

		if (idx < 16 * 3)
			do
			{
				lines << wxChar(0x20);
			} while (++idx < 16 * 3);

			lines << wxT("| ");

			// paint Text Data
			ucqueue.clear();
			wxFileOffset rowpos = m_HexRowIndex[rowidx];
			if (rowpos>hexrowpos)
			{
				idx = rowpos - hexrowpos;
				if (idx>0)
				{
					do
					{
						lines << wxChar(0x20);  // append space
					} while (--idx>0);
				}
			}
			pos = rowpos;
			GetLineByPos(lit, pos, rn);
			pos = rowpos - pos;
			const wxFileOffset hexrowpos16 = hexrowpos + 16;
			m_Lines->InitNextUChar(lit, pos);
			do
			{
				if (!m_Lines->NextUChar(ucqueue))
				{
					if (++lit == lineend || lit->m_Size == 0)
					{
						break;
					}
					m_Lines->InitNextUChar(lit, 0);
					m_Lines->NextUChar(ucqueue);
				}

				MadUCPair &ucp = ucqueue.back();
				rowpos += ucp.second;
				if (ucp.first <= 0x20)
				{
					lines << wxT('.');
				}
				else
				{
#ifdef __WXMSW__
					if (ucp.first<0x10000)
					{
						lines << wxChar(ucp.first);
					}
					else
					{
						wchar_t wbuf[2];
						wxm::UCS4toUTF16LE_U10000(ucp.first, (wxByte*)wbuf);
						lines << wbuf[0];
						lines << wbuf[1];
					}
#else
					lines << wxChar(ucp.first);
#endif
				}

				idx = ucp.second - 1;
				if (idx>0 && rowpos<hexrowpos16)
				{
					do
					{
						lines << wxChar(0x20);  // append space
					} while (--idx>0);
				}
			} while (rowpos<hexrowpos16);

			lines << wxT('\n');
	}

	m_HexPrintWXMEdit->SetText(lines);

	m_HexPrintWXMEdit->m_DrawingXPos = 0;

	m_HexPrintWXMEdit->PaintTextLines(dc, m_PrintRect,
		0, int(m_HexPrintWXMEdit->m_Lines->m_RowCount), *wxWHITE);
}

void InFrameWXMEdit::OnPaintInPrinting(wxPaintDC& dc, wxMemoryDC& memdc)
{
	dc.Blit(0, 0, m_old_ClientWidth, m_old_ClientHeight, &memdc, 0, 0);
}

LineNumberList InFrameWXMEdit::SaveBookmarkLineNumberList()
{
	return m_Lines->m_LineList.SaveBookmarkLineNumberList();
}

void InFrameWXMEdit::RestoreBookmarkByLineNumberList(const LineNumberList& linenums)
{
	m_Lines->m_LineList.RestoreBookmarkByLineNumberList(linenums);
}

void InFrameWXMEdit::ToggleBookmark()
{
	m_Lines->m_LineList.ToggleBookmark(m_CaretPos.iter);
	m_RepaintAll = true;
	Refresh(false);
}

void InFrameWXMEdit::GotoNextBookmark()
{
	int lineNum = m_Lines->m_LineList.GetNextBookmark(m_CaretPos.iter);
	if (lineNum > 0)
		GoToLine(lineNum);
}

void InFrameWXMEdit::GotoPreviousBookmark()
{
	int lineNum = m_Lines->m_LineList.GetPreviousBookmark(m_CaretPos.iter);
	if (lineNum > 0)
		GoToLine(int(m_Lines->m_LineCount + 1) - lineNum);
}

void InFrameWXMEdit::ClearAllBookmarks()
{
	m_Lines->m_LineList.ClearAllBookmarks();
	m_RepaintAll = true;
	Refresh(false);
}

void InFrameWXMEdit::PaintLineNumberArea(const wxColor & bgcolor, wxDC * dc, const wxRect& rect, bool is_trailing_subrow, MadLineIterator lineiter, int lineid, int text_top)
{
	if (!LineNumberAreaVisible())
		return;

	GetSyntax()->SetAttributes(aeLineNumber);

	// paint background
	if (GetSyntax()->nw_BgColor != bgcolor)
	{
		dc->SetPen(*wxThePenList->FindOrCreatePen(GetSyntax()->nw_BgColor, 1, wxSOLID));
		dc->SetBrush(*wxTheBrushList->FindOrCreateBrush(GetSyntax()->nw_BgColor));
		dc->DrawRectangle(rect);
	}

	if (is_trailing_subrow)
		return;

	if (m_bookmark_visible && m_Lines->m_LineList.Bookmarked(lineiter))
	{
		int b = m_RowHeight < 24 ? 1 : m_RowHeight / 24;

		MadAttributes* attr = GetSyntax()->GetAttributes(aeBookmark);
		dc->SetPen(*wxThePenList->FindOrCreatePen(attr->color, b, wxSOLID));
		dc->SetBrush(*wxTheBrushList->FindOrCreateBrush(attr->bgcolor));

		wxRect rdrect(rect.x + b/2, rect.y + b, rect.width - b, rect.height - b*3/2);
		double r = m_RowHeight < 18 ? 3.0 : m_RowHeight / 6.0;
		dc->DrawRoundedRectangle(rdrect, r);
	}

	if (!m_linenum_visible)
		return;

	const int ncount = CachedLineNumberAreaWidth() / m_TextFontMaxDigitWidth;

	wxString fmt(wxString::Format(wxT("%%%dd"), ncount));
	wxString lnstr = wxString::Format(fmt, lineid);

	dc->SetTextForeground(GetSyntax()->nw_Color);
	dc->SetFont(*(GetSyntax()->nw_Font));

	int l = rect.GetLeft();
	for (int i = 0; i < ncount; i++, l += m_TextFontMaxDigitWidth)
	{
		if (lnstr[i] != 0x20)
		{
			dc->DrawText(lnstr[i], l, text_top);
		}
	}
}

} //namespace wxm
