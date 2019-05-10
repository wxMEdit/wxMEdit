///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/edit/inframe.cpp
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "inframe.h"
#include "../../xm/cxx11.h"
#include "../wx_icu.h"
#include "../../wxmedit_frame.h"
#include "../../dialog/wxm_search_replace_dialog.h"
#include "../../mad_utils.h"
#include "../../dialog/wxm_enumeration_dialog.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
# pragma warning( disable : 4819 )
#endif
// disable 4996,4819 {
#include <wx/aui/auibook.h>
#include <wx/filename.h>
# if wxMAJOR_VERSION == 3
#include <wx/debug.h>
# endif
#include <boost/format.hpp>
// disable 4996,4819 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

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
	virtual void Record(const wxString& key, long val) override;
	virtual void Record(const wxString& key, double val) override;
	virtual void Record(const wxString& key, const wxString& val) override;

	virtual void SetConfig(wxConfigBase* cfg) override { m_cfg = cfg; }

	GlobalConfigWriter(): m_cfg(nullptr) {}
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

#ifdef _DEBUG
void test_HumanReadableFilesize();
#endif

InFrameWXMEdit::InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: MadEdit(new GlobalConfigWriter(), parent, id, pos, size, style), m_auto_searcher(this)
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

	UErrorCode uerr = U_ZERO_ERROR;
	m_numfmt.reset(U_ICU_NAMESPACE::NumberFormat::createInstance(U_ICU_NAMESPACE::Locale::getDefault(), uerr));

#ifdef _DEBUG
	test_HumanReadableFilesize();
#endif
}

wxString InsertModeText(bool insertmode)
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

std::wstring HumanReadableFilesize(uint64_t size);

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

	wxString lineInfo = wxString::Format(_("Ln: %s/%s"), FormattedNumber(line).c_str(), FormattedNumber(GetLineCount()).c_str());
	wxString subrowInfo = (subrow <= 0)? wxString(): wxString::Format(_(" (Sub: %s)"), FormattedNumber(subrow + 1).c_str());
	wxString colInfo = wxString::Format(_(" Col: %s"), FormattedNumber(col).c_str());
	wxm::GetFrameStatusBar().SetField(wxm::STBF_ROWCOL, lineInfo + subrowInfo + colInfo);

	wxFileOffset filesize = GetFileSize();
	wxString charpos = wxString::Format(_("CharPos: %s/%s"), FormattedNumber(GetCaretPosition()).c_str(), FormattedNumber(filesize).c_str());
	wxString readableSize = (filesize < 1024)? wxString(): wxString::Format(_(" (%s)"), HumanReadableFilesize(filesize).c_str());
	wxm::GetFrameStatusBar().SetField(wxm::STBF_CHARPOS, charpos + readableSize);

	wxString selsize = wxString::Format(_("SelSize: %s"), FormattedNumber(GetSelectionSize()).c_str());
	wxm::GetFrameStatusBar().SetField(wxm::STBF_SELECTION, selsize);

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
		g_MainFrame->m_Notebook->SetPageText(selid, title);

	if (this != g_active_wxmedit)
		return;

	if (filename.IsEmpty())
		filename = title;
	g_MainFrame->SetTitle(wxString(wxT("wxMEdit - [")) + filename + wxString(wxT("] ")));

	wxString encfmt = GetEncodingName();

	encfmt += BOMText(HasBOM());

	encfmt += wxString(wxT(".")) + GetNewLine().Name();

	wxm::GetFrameStatusBar().SetField(wxm::STBF_ENCFMT, encfmt);

	wxm::GetFrameStatusBar().SetField(wxm::STBF_READONLY, ReadOnlyText(IsReadOnly()));

	wxm::GetFrameStatusBar().SetField(wxm::STBF_INSOVR, InsertModeText(IsInsertMode()));

	wxm::GetFrameStatusBar().Update(); // repaint immediately

	if (g_SearchReplaceDialog != nullptr)
		g_SearchReplaceDialog->UpdateCheckBoxByCBHex();
}

void InFrameWXMEdit::DoToggleWindow()
{
	wxCommandEvent e(0, 0);
	g_MainFrame->OnWindowToggleWindow(e);
}

void InFrameWXMEdit::DoMouseRightUp()
{
#if wxMAJOR_VERSION == 3
	wxAssertHandler_t oldHandler = wxSetAssertHandler(nullptr);
#endif
	g_MainFrame->PopupMenu(g_Menu_Edit);
#if wxMAJOR_VERSION == 3
	wxSetAssertHandler(oldHandler);
#endif
}

void InFrameWXMEdit::SetWordWrapMode(MadWordWrapMode mode)
{
	if (m_WordWrapMode == mode)
		return;

	m_WordWrapMode = mode;
	m_cfg_writer->Record(wxT("/wxMEdit/WordWrapMode"), (long)mode);

	if (GetEditMode() == emHexMode)
	{
		m_DoRecountLineWidth = true;
		return;
	}

	if (mode == wwmWrapByWindow)
		m_DrawingXPos = 0;

	UpdateAppearance();
	RecountLineWidth(true);
	UpdateScrollBarPos();

	m_RepaintAll = true;
	Refresh(false);
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

	m_PrintTotalHexLineCount = int(m_Lines->m_Size / 16);
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
	m_HexPrintWXMEdit = nullptr;
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
		rowcount -= (pageNum*rowcount - int(m_Lines->m_RowCount));

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
	dc->SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxPENSTYLE_SOLID));
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
	xm::UCQueue ucqueue;

	wxFileOffset hexrowpos = wxFileOffset(toprow) * 16;
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
			idx = int(rowpos - hexrowpos);
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

			xm::CharUnit& cu = ucqueue.back();
			rowpos += cu.nbytes();
			if (cu.ucs4() <= 0x20)
				lines << wxT('.');
			else
				WxStrAppendUCS4(lines, cu.ucs4());

			idx = cu.nbytes() - 1;
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
		dc->SetPen(*wxThePenList->FindOrCreatePen(GetSyntax()->nw_BgColor, 1, wxPENSTYLE_SOLID));
		dc->SetBrush(*wxTheBrushList->FindOrCreateBrush(GetSyntax()->nw_BgColor));
		dc->DrawRectangle(rect);
	}

	if (is_trailing_subrow)
		return;

	if (m_bookmark_visible && m_Lines->m_LineList.Bookmarked(lineiter))
	{
		int b = m_RowHeight < 24 ? 1 : m_RowHeight / 24;

		MadAttributes* attr = GetSyntax()->GetAttributes(aeBookmark);
		dc->SetPen(*wxThePenList->FindOrCreatePen(attr->color, b, wxPENSTYLE_SOLID));
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
			dc->DrawText(lnstr[i], l, text_top);
	}
}

void InFrameWXMEdit::TrimTrailingSpaces()
{
	if (IsReadOnly() || GetEditMode() == emHexMode)
		return;

	// use Regular Expressions to trim all trailing spaces
	Searcher(false, true)->SetOption(true, false);
	Searcher(false, true)->ReplaceAll(wxT("[ \t]+(\r|\n|$)"), wxT("$1"));
}

void InFrameWXMEdit::InsertEnumeration()
{
	int selbeg, selend;
	GetSelectionLineId(selbeg, selend);

	std::vector<ucs4_t> seq;
	size_t rows;
	WXMEnumerationDialog dlg(seq, rows, g_MainFrame);

	if (selbeg != -1)
		dlg.SetSelectedRows(size_t(selend - selbeg + 1));
	int rc = dlg.ShowModal();

	if (rc != wxID_OK || rows==0)
		return;

	if (GetEditMode() == emColumnMode)
		InsertColumnText(seq, int(rows));
	else
		InsertRegularText(seq);
}

void InFrameWXMEdit::ColumnAlign()
{
	if (IsReadOnly() || GetEditMode() != emColumnMode)
		return;

	MadUndo *undo = nullptr;

	MadLineIterator firstlit = m_SelectionBegin->iter;
	MadLineIterator lastlit = m_SelectionEnd->iter;
	wxFileOffset pos = m_SelectionEnd->pos - m_SelectionEnd->linepos;
	int rxpos = m_SelRightXPos;
	if (!m_Selection || m_SelectionBegin->pos == m_SelectionEnd->pos)
	{
		firstlit = lastlit = m_CaretPos.iter;
		pos = m_CaretPos.pos - m_SelectionEnd->linepos;
		rxpos = m_CaretPos.xpos;
	}

	xm::UCQueue ucqueue;
	for (MadLineIterator lit = lastlit; ; --lit, pos -= lit->m_Size)
	{
		int rowwidth = lit->m_RowIndices[0].m_Width;
		wxFileOffset rowpos = lit->m_RowIndices[0].m_Start;
		wxFileOffset rowendpos = lit->m_RowIndices[1].m_Start;

		if (rxpos > rowwidth)
			continue;

		m_Lines->InitNextUChar(lit, rowpos);

		int nowxpos = 0;
		do
		{
			ucs4_t uc = 0x0D;
			if (m_Lines->NextUChar(ucqueue))
				uc = ucqueue.back().ucs4();

			if (uc == 0x0D || uc == 0x0A)  // EOL
				break;

			int ucwidth = GetUCharTextFontWidth(uc, rowwidth, nowxpos);
			nowxpos += ucwidth;

			if (nowxpos < rxpos + ucwidth / 2)
				rowpos += ucqueue.back().nbytes();

		} while (rowpos < rowendpos && nowxpos < rxpos);

		size_t dellen = 0;
		ucqueue.clear();
		for (wxFileOffset tmprowpos = rowpos; tmprowpos < rowendpos; )
		{
			ucs4_t uc = 0x0D;
			if (m_Lines->NextUChar(ucqueue))
				uc = ucqueue.back().ucs4();

			if (uc != 0x20 && uc != ucs4_t('\t'))
				break;
			++dellen;
			tmprowpos += ucqueue.back().nbytes();
		}
		if (dellen != 0)
		{
			MadDeleteUndoData *dudata = new MadDeleteUndoData;

			dudata->m_Pos = pos + rowpos;
			dudata->m_Size = dellen;

#ifdef _DEBUG
			wxASSERT(DeleteInsertData(dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr) == lit);
#else
			DeleteInsertData(dudata->m_Pos, dudata->m_Size, &dudata->m_Data, 0, nullptr);
#endif

			if (undo == nullptr)
				undo = m_UndoBuffer->Add();
			undo->m_Undos.push_back(dudata);
		}

		if (lit == firstlit)
			break;
	}

	if (undo)
		m_Modified = true;
	m_Selection = false;
	m_RepaintAll = true;
	Refresh(false);

	if (undo)
		m_Lines->Reformat(firstlit, lastlit);

	AppearCaret();
	UpdateScrollBarPos();

	DoSelectionChanged();
	if (undo)
		DoStatusChanged();
}

void InFrameWXMEdit::ColumnPaste()
{
	if (!CanPaste())
		return;

	std::vector<ucs4_t> ucs;
	GetTextFromClipboard(ucs);
	InsertString(&ucs[0], ucs.size(), true, true, false);
}

bool InFrameWXMEdit::Reload()
{
	if (m_Lines->m_Name.IsEmpty())
		return false;

	if (m_Modified)
	{
		wxMessageDialog dlg(this, _("Do you want to discard changes?"), wxT("wxMEdit"), wxYES_NO | wxICON_QUESTION);
		if (dlg.ShowModal() != wxID_YES)
			return false;
	}

	WXMLocations loc = SaveLocations();
	MadEditMode editmode = m_EditMode;

	LoadFromFile(m_Lines->m_Name, m_Lines->m_Encoding->GetName(), m_EditMode == emHexMode);
	SetEditMode(editmode);
	RestoreLocations(loc);
	return true;
}

bool InFrameWXMEdit::ReloadByModificationTime()
{
	if (m_Lines->m_Name.IsEmpty())
		return false;

	wxLogNull nolog;
	time_t modtime = wxFileModificationTime(m_Lines->m_Name);

	if (modtime == 0) // the file has been deleted
	{
		m_ModificationTime = 0;
		return false;
	}

	if (modtime == m_ModificationTime)
		return false; // the file doesn't change.

	m_ModificationTime = modtime;

	wxMessageDialog dlg(this,
		wxString(_("This file has been changed by another application.")) + wxT("\n") +
		wxString(_("Do you want to reload it?")) + wxT("\n\n") + m_Lines->m_Name,
		wxT("wxMEdit"), wxYES_NO | wxICON_QUESTION);
	if (dlg.ShowModal() != wxID_YES)
	{
		return false;
	}

	// YES, reload it.
	return Reload();
}

bool InFrameWXMEdit::ManuallyCancelHexToText()
{
	long maxtextfilesize;
	wxString oldpath = m_Config->GetPath();
	m_Config->Read(wxT("/wxMEdit/MaxTextFileSize"), &maxtextfilesize, 1024 * 1024 * 10 /* 10MiB */);
	m_Config->SetPath(oldpath);

	if (m_Lines->m_Size < maxtextfilesize)
		return false;

	wxString size = FormattedNumber(m_Lines->m_Size);
	if (wxNO == wxMessageBox(wxString::Format(_("Do you want to continue?\nThe file size is %s bytes.\nIt may take long time and large memories to convert to Text/Column Mode."), size.c_str()), _("Hex Mode to Text/Column Mode"), wxYES_NO))
	{
		return true;
	}

	return false;
}

wxString InFrameWXMEdit::FormattedNumber(int64_t num)
{
	UnicodeString us;
	m_numfmt->format(num, us);
	return ICUStrToWx(us);
}

std::wstring WindowsStyleFilesize(double size, const std::wstring& unit)
{
	if (size >= 100)
		return (boost::wformat(L"%.0f%s") % floor(size) % unit).str();

	if (size >= 10)
		return (boost::wformat(L"%.1f%s") % (floor(size*10)/10) % unit).str();

	return (boost::wformat(L"%.2f%s") % (floor(size*100)/100) % unit).str();
}

std::wstring HumanReadableFilesize(uint64_t size)
{
	const size_t KiB = 1024;
	const size_t MiB = 1024 * KiB;
	const size_t GiB = 1024 * MiB;

	if (size >= 1000 * MiB)
		return WindowsStyleFilesize(double(size / GiB) + double(size % GiB) / GiB, L"GiB");

	if (size >= 1000 * KiB)
		return WindowsStyleFilesize(double(size) / MiB, L"MiB");

	if (size >= KiB)
		return WindowsStyleFilesize(double(size) / KiB, L"KiB");

	return (boost::wformat(L"%u") % size).str();
}

#ifdef _DEBUG
void test_HumanReadableFilesize()
{
	wxASSERT(HumanReadableFilesize(1) == L"1");
	wxASSERT(HumanReadableFilesize(1023) == L"1023");
	wxASSERT(HumanReadableFilesize(1024) == L"1.00KiB");
	wxASSERT(HumanReadableFilesize(1025) == L"1.00KiB");
	wxASSERT(HumanReadableFilesize(1034) == L"1.00KiB");
	wxASSERT(HumanReadableFilesize(1035) == L"1.01KiB");
	wxASSERT(HumanReadableFilesize(1044) == L"1.01KiB");
	wxASSERT(HumanReadableFilesize(1045) == L"1.02KiB");
	wxASSERT(HumanReadableFilesize(1126) == L"1.09KiB");
	wxASSERT(HumanReadableFilesize(1127) == L"1.10KiB");
	wxASSERT(HumanReadableFilesize(2047) == L"1.99KiB");
	wxASSERT(HumanReadableFilesize(2048) == L"2.00KiB");
	wxASSERT(HumanReadableFilesize(10239) == L"9.99KiB");
	wxASSERT(HumanReadableFilesize(10240) == L"10.0KiB");
	wxASSERT(HumanReadableFilesize(10342) == L"10.0KiB");
	wxASSERT(HumanReadableFilesize(10343) == L"10.1KiB");
	wxASSERT(HumanReadableFilesize(102399) == L"99.9KiB");
	wxASSERT(HumanReadableFilesize(102400) == L"100KiB");
	wxASSERT(HumanReadableFilesize(1023999) == L"999KiB");
	wxASSERT(HumanReadableFilesize(1024000) == L"0.97MiB");
	wxASSERT(HumanReadableFilesize(1048575) == L"0.99MiB");
	wxASSERT(HumanReadableFilesize(1048576) == L"1.00MiB");
	wxASSERT(HumanReadableFilesize(1059061) == L"1.00MiB");
	wxASSERT(HumanReadableFilesize(1059062) == L"1.01MiB");
	wxASSERT(HumanReadableFilesize(1153433) == L"1.09MiB");
	wxASSERT(HumanReadableFilesize(1153434) == L"1.10MiB");
	wxASSERT(HumanReadableFilesize(10485759) == L"9.99MiB");
	wxASSERT(HumanReadableFilesize(10485760) == L"10.0MiB");
	wxASSERT(HumanReadableFilesize(104857599) == L"99.9MiB");
	wxASSERT(HumanReadableFilesize(104857600) == L"100MiB");
	wxASSERT(HumanReadableFilesize(1048575999) == L"999MiB");
	wxASSERT(HumanReadableFilesize(1048576000) == L"0.97GiB");
	wxASSERT(HumanReadableFilesize(1073741823) == L"0.99GiB");
	wxASSERT(HumanReadableFilesize(1073741824) == L"1.00GiB");
	wxASSERT(HumanReadableFilesize(1084479242) == L"1.00GiB");
	wxASSERT(HumanReadableFilesize(1084479243) == L"1.01GiB");
	wxASSERT(HumanReadableFilesize(1181116006) == L"1.09GiB");
	wxASSERT(HumanReadableFilesize(1181116007) == L"1.10GiB");
	wxASSERT(HumanReadableFilesize(10737418239) == L"9.99GiB");
	wxASSERT(HumanReadableFilesize(10737418240) == L"10.0GiB");
	wxASSERT(HumanReadableFilesize(107374182399) == L"99.9GiB");
	wxASSERT(HumanReadableFilesize(107374182400) == L"100GiB");
}
#endif

} //namespace wxm
