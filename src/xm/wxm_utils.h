///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm_utils.h
// Description: Utilities
// Author:      wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UTILS_H_
#define _WXM_UTILS_H_

#include "wxm_choice_map.hpp"
#include "wxm_def.h"
#include "../wxmedit/ucs4_t.h"

#include <wx/defs.h>
#include <wx/string.h>
#include <boost/shared_ptr.hpp>
#include <vector>

class wxWindow;
class MadEdit;

namespace wxm
{

static wxString s_wxsRegkeyClasses = wxT("HKEY_CURRENT_USER\\Software\\Classes\\");

void SetL10nHtmlColors();

void OpenURL(const wxString& url);

void SetDefaultMonoFont(wxWindow* win);

wxString FilePathNormalCase(const wxString& path);
bool FilePathEqual(const wxString& path1, const wxString& path2);
unsigned long FilePathHash(const wxString& path);

struct FileList
{
	FileList(const wxString& files = wxString())
	{
		Init(files);
	}
	FileList(const wchar_t* files)
	{
		Init(wxString(files));
	}

	void Append(const wxString& file, const LineNumberList& bmklns=LineNumberList());

	const wxString& String()
	{
		return m_files;
	}

	struct FileDesc
	{
		wxString file;
		LineNumberList bmklinenums;

		FileDesc() {}
		FileDesc(const wxString& f, const LineNumberList& lns) : file(f), bmklinenums(lns) {}
	};

	typedef std::vector<FileDesc> FDescList;

	const FDescList& List()
	{
		return m_filevec;
	}

private:
	void Init(const wxString& files);

	wxString m_files;
	FDescList m_filevec;
};

struct HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst) = 0;
	virtual bool Hexadecimal() = 0;
	virtual ~HexAreaClipboardCopier() {}
};

typedef boost::shared_ptr<HexAreaClipboardCopier> SharedCopierPtr;
struct HexAreaClipboardCopyProxy: public ChoiceMap<HexAreaClipboardCopyProxy, SharedCopierPtr>
{
	void SelectCopierByConfig(const wxString& cfg)
	{
		m_selected_idx = ConfigToIndex(cfg);
	}

	wxString GetSelectedCopierTitle()
	{
		return IndexToTitle(m_selected_idx);
	}

	HexAreaClipboardCopier& GetSelectedCopier()
	{
		return *IndexToVal(m_selected_idx);
	}

private:
	friend class ChoiceMap<HexAreaClipboardCopyProxy, SharedCopierPtr>;
	HexAreaClipboardCopyProxy(): m_selected_idx(HACCI_RAWBYTES) {}

	virtual void DoInit();

	enum HexAreaClipboardCopierIndex
	{
		HACCI_RAWBYTES,
		HACCI_REGULARTEXT,
		HACCI_HEXSTRING,
		HACCI_HEXSTRINGWITHSPACE,
	};

	int m_selected_idx;
};

struct MouseCapturer
{
	void Capture();
	void Release();

	void Reset()
	{
		m_captured = false;
	}

	MouseCapturer(MadEdit& edit): m_edit(edit), m_captured(false) {}
private:
	MadEdit& m_edit;
	bool m_captured;
};

struct HexAreaClipboardPaster
{
	virtual bool GetRawBytesFromClipboardDirectly(MadEdit* inst, std::vector<char>& cs) = 0;
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) = 0;
	virtual ~HexAreaClipboardPaster() {}
};

typedef boost::shared_ptr<HexAreaClipboardPaster> SharedPasterPtr;
struct HexAreaClipboardPasteProxy: public ChoiceMap<HexAreaClipboardPasteProxy, SharedPasterPtr>
{
	void SelectConditionByConfig(const wxString& cfg)
	{
		m_selected_idx = ConfigToIndex(cfg);
	}

	wxString GetSelectedConditionTitle()
	{
		return IndexToTitle(m_selected_idx);
	}

	HexAreaClipboardPaster& GetSelectedPaster()
	{
		return *IndexToVal(m_selected_idx);
	}

private:
	friend class ChoiceMap<HexAreaClipboardPasteProxy, SharedPasterPtr>;
	HexAreaClipboardPasteProxy(): m_selected_idx(HAPAHCI_NEVER) {}

	virtual void DoInit();

	enum HexAreaPasteAsHexConditionIndex
	{
		HAPAHCI_NEVER,
		HAPAHCI_ACCORDINGTOCOPIER,
		HAPAHCI_IFPOSSIBLE,
		HAPAHCI_ALWAYS,
	};

	int m_selected_idx;
};

} //namespace wxm

#endif //_WXM_UTILS_H_
