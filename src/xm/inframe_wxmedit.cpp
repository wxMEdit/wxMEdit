///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/inframe_wxmedit.cpp
// Description: Embedded wxMEdit in Main Frame
// Copyright:   2014-2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "inframe_wxmedit.h"
#include "../wxmedit_frame.h"
#include "../dialog/wxm_search_replace_dialog.h"

#include <wx/aui/auibook.h>
#include <wx/filename.h>

extern MadEdit* g_ActiveMadEdit;
extern wxMenu *g_Menu_Edit;

namespace wxm
{

InFrameWXMEdit::InFrameWXMEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: MadEdit(parent, id, pos, size, style)
{
	SetWindowStyleFlag(GetWindowStyleFlag() & ~wxTAB_TRAVERSAL);
	//SetDropTarget(new DnDFile());

	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MadEditFrame::MadEditFrameKeyDown));
}

void InFrameWXMEdit::DoSelectionChanged()
{
	g_MainFrame->m_Notebook->ConnectMouseClick();

	if (this != g_ActiveMadEdit)
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

	wxString filename = GetFileName(), title;
	if (!filename.IsEmpty())
	{
		wxFileName fn(filename);
		title = fn.GetFullName();
	}
	else
	{
		title = oldtitle;
	}

	if (IsModified())
	{
		if (title[title.Len() - 1] != wxT('*'))
			title += wxT('*');

		if (filename.IsEmpty())
			filename = title;
		else
			filename += wxT('*');
	}
	else
	{
		if (title[title.Len() - 1] == wxT('*'))
			title.Truncate(title.Len() - 1);
	}

	if (title != oldtitle)
	{
		g_MainFrame->m_Notebook->SetPageText(selid, title);
	}

	if (this != g_ActiveMadEdit)
		return;

	if (filename.IsEmpty())
		filename = title;
	g_MainFrame->SetTitle(wxString(wxT("wxMEdit - [")) + filename + wxString(wxT("] ")));

	wxString enc = GetEncodingName();
	if (HasBOM())
	{
		static wxString bom(wxT(".BOM"));
		enc += bom;
	}

	switch (GetNewLineType())
	{
	case nltDOS:
	{
		static wxString xdos(wxT(".DOS"));
		enc += xdos;
	}
	break;
	case nltUNIX:
	{
		static wxString xunix(wxT(".UNIX"));
		enc += xunix;
	}
	break;
	case nltMAC:
	{
		static wxString xmac(wxT(".MAC"));
		enc += xmac;
	}
	break;
	default:
		break;
	}
	wxm::GetFrameStatusBar().SetField(wxm::STBF_ENCFMT, enc);

	if (IsReadOnly())
	{
		static wxString rostr(_("ReadOnly"));
		wxm::GetFrameStatusBar().SetField(wxm::STBF_READONLY, rostr);
	}
	else
	{
		wxm::GetFrameStatusBar().SetField(wxm::STBF_READONLY, wxEmptyString);
	}

	if (GetInsertMode())
	{
		static wxString insstr(_("INS"));
		wxm::GetFrameStatusBar().SetField(wxm::STBF_INSOVR, insstr);
	}
	else
	{
		static wxString ovrstr(_("OVR"));
		wxm::GetFrameStatusBar().SetField(wxm::STBF_INSOVR, ovrstr);
	}

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

} //namespace wxm
