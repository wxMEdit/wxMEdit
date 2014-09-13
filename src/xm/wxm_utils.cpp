///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm_utils.cpp
// Description: Utilities
// Author:      wxmedit@gmail.com
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_utils.h"
#include "wxm_encoding/encoding.h"
#include "../mad_utils.h"
#include "../wxmedit/wxmedit.h"

#include <wx/string.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>
#include <wx/font.h>
#include <wx/window.h>
#include <wx/hashmap.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>

#include <unicode/uchar.h>
#include <unicode/locid.h>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <locale.h>

#ifdef __WXMAC__
# include <wx/mac/private.h>
# include <Processes.h>
#endif

#ifdef __WXMSW__
# include <wx/msw/registry.h>
#endif

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

extern bool g_DoNotSaveSettings;

namespace wxm
{

void SetL10nHtmlColors()
{
	wxColourDatabase *cdb=wxTheColourDatabase;
	HtmlColor *hc=HtmlColorTable;
	for(int i=0; i<HtmlColorTableCount; ++i, ++hc)
	{
		cdb->AddColour(hc->name, wxColor(hc->red, hc->green, hc->blue));
		g_color_l10n_map[wxString(hc->name).Upper()] = wxGetTranslation(hc->name);
	}
}

void FileList::Append(const wxString& file, const LineNumberList& bmklns)
{
	m_files += file + wxT("<");
	BOOST_FOREACH (size_t ln, bmklns)
		m_files << ln << wxT("<");
	m_files += wxT("|");

	m_filevec.push_back(FileDesc(file, bmklns));
}

void FileList::Init(const wxString& files)
{
	m_files = files;

	wxStringTokenizer tkzfiledescs(files,  wxT("|"));
	while (tkzfiledescs.HasMoreTokens())
	{
		wxStringTokenizer tkz(tkzfiledescs.GetNextToken(), wxT("<"));

		wxString file = tkz.GetNextToken();

		LineNumberList bmklns;
		while (tkz.HasMoreTokens())
		{
			unsigned long ln = 0;
			if (tkz.GetNextToken().ToULong(&ln))
				bmklns.push_back((size_t)ln);
		}

		m_filevec.push_back(FileDesc(file, bmklns));
	}
}

void OpenOriginalURL(const wxString& url)
{
#ifdef __WXGTK__
	const wxChar *browsers[]=
	{
		wxT("/usr/bin/firefox"),
		wxT("/usr/bin/mozilla"),
		wxT("/usr/bin/chromium"),
		wxT("/usr/bin/konqueror"),
# if defined(__APPLE__) && defined(__MACH__)
		wxT("/usr/bin/open"),
# endif
		wxT("/usr/kde/3.5/bin/konqueror"),
		wxT("/usr/kde/3.4/bin/konqueror"),
		wxT("/usr/kde/3.3/bin/konqueror"),
		wxT("/usr/kde/3.2/bin/konqueror"),
	};
	int idx=0;
	int count=sizeof(browsers)/sizeof(browsers[0]);
	do
	{
		if(wxFileExists(wxString(browsers[idx])))
			break;
	}while(++idx < count);

	if(idx < count)
	{
		wxExecute(wxString(browsers[idx]) +wxT(' ') +url);
	}
	else
	{
		wxLaunchDefaultBrowser(url);
	}
#else
	wxLaunchDefaultBrowser(url);
#endif
}

wxString WXMLanguageQuery()
{
	icu::Locale loc;
	std::string lang = loc.getLanguage();
	std::string ctry = loc.getCountry();

	return wxString((boost::format("?%s_%s") % lang % ctry).str().c_str(), wxConvUTF8);
}

void OpenURL(const wxString& url)
{
	if (! boost::algorithm::istarts_with(url, "http://wxmedit.github.io/"))
		return OpenOriginalURL(url);

	static wxString lang_q = WXMLanguageQuery();

	OpenOriginalURL(url + lang_q);
}

void SetDefaultMonoFont(wxWindow* win)
{
	const wxString fontname = wxm::WXMEncodingManager::Instance().GetSystemEncoding()->GetFontName();
	int fontsize = win->GetFont().GetPointSize();
	win->SetFont(wxFont(fontsize, wxDEFAULT, wxNORMAL, wxNORMAL, false, fontname));
}

wxString FilePathNormalCase(const wxString& name)
{
#ifndef __WXMSW__
	return name;
#else
	// wxString::Upper is buggy under Windows
	// and the filename insensitive of Windows is also buggy
	// but they are different
	wxString uppername;
	BOOST_FOREACH(wxChar ch, name)
		uppername.append(1, (wxChar)u_toupper((UChar32)(unsigned int)ch));

	return uppername;
#endif
}

bool FilePathEqual(const wxString& name1, const wxString& name2)
{
	return FilePathNormalCase(name1) == FilePathNormalCase(name2);
}

unsigned long FilePathHash(const wxString& name)
{
	return wxStringHash::wxCharStringHash(FilePathNormalCase(name));
}

struct HexAreaRawBytesCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst)
	{
		inst->CopyRawBytes();
	}
	virtual bool Hexadecimal()
	{
		return false;
	}
};

struct HexAreaRegularTextCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst)
	{
		inst->CopyRegularText();
	}
	virtual bool Hexadecimal()
	{
		return false;
	}
};

struct HexAreaHexStringCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst)
	{
		inst->CopyAsHexString(false);
	}
	virtual bool Hexadecimal()
	{
		return true;
	}
};

struct HexAreaHexStringWithSpaceCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst)
	{
		inst->CopyAsHexString(true);
	}
	virtual bool Hexadecimal()
	{
		return true;
	}
};

void HexAreaClipboardCopyProxy::DoInit()
{
	AddData(HACCI_RAWBYTES,           wxT("raw_bytes"),             _("Copy as Raw Bytes"),              SharedCopierPtr(new HexAreaRawBytesCopier));
	AddData(HACCI_REGULARTEXT,        wxT("regular_text"),          _("Copy as Regular Text"),           SharedCopierPtr(new HexAreaRegularTextCopier));
	AddData(HACCI_HEXSTRING,          wxT("hex_string"),            _("Copy as Hex String"),             SharedCopierPtr(new HexAreaHexStringCopier));
	AddData(HACCI_HEXSTRINGWITHSPACE, wxT("hex_string_with_space"), _("Copy as Hex String with Spaces"), SharedCopierPtr(new HexAreaHexStringWithSpaceCopier));

	SetDefault(HACCI_RAWBYTES, IndexToVal(HACCI_RAWBYTES));
}

bool GetRawBytesFromHexUnicodeText(std::vector<char>& cs, const std::vector<ucs4_t>& ucs)
{
	std::vector<int> tmp_hex;

	BOOST_FOREACH(ucs4_t u, ucs)
	{
		if (u_isUWhiteSpace(u) && tmp_hex.size()%2==0)
			continue;
		if (!isxdigit(u))
			return false;

		int hex = (u > ucs4_t('9'))? (u | 0x20) - 'a' + 10: u - '0';
		tmp_hex.push_back(hex);
	}

	if (tmp_hex.empty() || tmp_hex.size() % 2 != 0)
		return false;

	for (size_t i=0; i<tmp_hex.size(); i+=2)
		cs.push_back(char((tmp_hex[i]<<4) | tmp_hex[i+1]));

	return true;
}

struct HexAreaRawBytesHexPaster: public HexAreaClipboardPaster
{
	virtual bool GetRawBytesFromClipboardDirectly(MadEdit* inst, std::vector<char>& cs)
	{
		return inst->GetRawBytesFromClipboardDirectly(cs);
	}

	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) = 0;
};

struct HexAreaNerverHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs)
	{
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaIfPossibleHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs)
	{
		if (GetRawBytesFromHexUnicodeText(cs, ucs))
			return;
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaAccordingToCopierHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs)
	{
		HexAreaClipboardCopier& copier = HexAreaClipboardCopyProxy::Instance().GetSelectedCopier();
		if (copier.Hexadecimal() && GetRawBytesFromHexUnicodeText(cs, ucs))
			return;
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaAlwaysHexPaster: public HexAreaClipboardPaster
{
	virtual bool GetRawBytesFromClipboardDirectly(MadEdit* inst, std::vector<char>& cs)
	{
		return false;
	}

	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs)
	{
		GetRawBytesFromHexUnicodeText(cs, ucs);
	}
};

void HexAreaClipboardPasteProxy::DoInit()
{
	AddData(HAPAHCI_NEVER,             wxT("never"),               _("Never"),                         SharedPasterPtr(new HexAreaNerverHexPaster));
	AddData(HAPAHCI_ACCORDINGTOCOPIER, wxT("according_to_copier"), _("According To Copying Behavior"), SharedPasterPtr(new HexAreaAccordingToCopierHexPaster));
	AddData(HAPAHCI_IFPOSSIBLE,        wxT("if_possible"),         _("If Possible"),                   SharedPasterPtr(new HexAreaIfPossibleHexPaster));
	AddData(HAPAHCI_ALWAYS,            wxT("always"),              _("Always"),                        SharedPasterPtr(new HexAreaAlwaysHexPaster));

	SetDefault(HAPAHCI_NEVER, IndexToVal(HAPAHCI_NEVER));
}

void MouseCapturer::Capture()
{
	m_edit.CaptureMouse();
	m_captured = true;
}

void MouseCapturer::Release()
{
	if (!m_captured)
		return;

	m_edit.ReleaseMouse();
	m_captured = false;
}

AppPath* AppPath::s_inst = NULL;
wxString g_wxsRegKeyWxMEdit = wxT("HKEY_CURRENT_USER\\Software\\wxMEdit");
wxString g_wxsRegValConfigInHome = wxT("ConfigInUserHome");

// return application data directory in user home
//     ~/.wxmedit/          under *NIX
//     %APPDATA%\\wxmedit\\ under Windows
wxString GetDataDirInUserHome()
{
	wxString home_dir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH;
	if(!wxDirExists(home_dir))
	{
		wxLogNull nolog; // disable error message
		wxMkdir(home_dir);
	}
	return home_dir;
}

#ifdef __WXMSW__
bool FileWritable(const wxString& test_file)
{
	wxLogNull nolog;

	wxFile ft(test_file.c_str(), wxFile::write);
	if (!ft.IsOpened())
		return false;

	time_t t = time(NULL);
	if (ft.Write(&t, sizeof(t)) != sizeof(t))
		return false;

	return true;
}

bool ConfigInUserHomeFromRegistry()
{
	wxLogNull nolog;

	boost::scoped_ptr<wxRegKey> pRegKey( new wxRegKey(g_wxsRegKeyWxMEdit) );

	if(!pRegKey->Exists())
		return false;

	long cfg_in_usrhome = 0;
	pRegKey->QueryValue(g_wxsRegValConfigInHome, &cfg_in_usrhome);

	return cfg_in_usrhome != 0;
}
#endif

bool AppPath::ConfigWillBeInUserHome() const
{
#ifdef __WXMSW__
	return !app_dir_writable || ConfigInUserHomeFromRegistry();
#else
	return true;
#endif
}

void AppPath::Init(const wxString& appname)
{
	cfg_file = appname + wxT(".cfg");

	wxFileName filename(GetExecutablePath());
	filename.MakeAbsolute();
	app_dir = filename.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

	usr_dir = GetDataDirInUserHome();

#ifdef __WXMSW__
	app_dir_writable = FileWritable(app_dir + wxT("portable_test"));
	cfg_in_usrhome = ConfigWillBeInUserHome();
	if (!cfg_in_usrhome)
	{
		home_dir = app_dir;
		another_dir = usr_dir;
		return;
	}
#endif

	home_dir = usr_dir;
	another_dir = app_dir;
}

void AppPath::SaveConfig() const
{
#ifdef __WXMSW__
	if (g_DoNotSaveSettings)
		return;

	if (ConfigWillBeInUserHome() == cfg_in_usrhome)
		return;

	wxFileConfig *cfg=reinterpret_cast<wxFileConfig *>(wxFileConfig::Get(false));
	wxFileOutputStream another_cfg(another_dir + cfg_file);
	if (!another_cfg.IsOk())
		return;

	cfg->Save(another_cfg);
	g_DoNotSaveSettings = true;
#endif
}

} // namespace wxm
