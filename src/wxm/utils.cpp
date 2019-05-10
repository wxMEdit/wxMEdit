///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/utils.cpp
// Description: Utilities
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "utils.h"
#include "../xm/encoding/external.h"
#include "../xm/cxx11.h"
#include "../xm/encoding/encoding.h"
#include "../mad_utils.h"
#include "../wxmedit/wxmedit.h"
#include "../xm/uutils.h"
#include "../xm/line_enc_adapter.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
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
#include <wx/log.h>
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <unicode/uchar.h>
#include <unicode/locid.h>

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4819)
#endif
#include <boost/format.hpp>
#ifdef _MSC_VER
# pragma warning(pop)
#endif
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
	U_ICU_NAMESPACE::Locale loc;
	std::string lang = loc.getLanguage();
	std::string ctry = loc.getCountry();

	return wxString((boost::format("?hl=%s_%s") % lang % ctry).str().c_str(), wxConvUTF8);
}

void OpenURL(const wxString& url)
{
	if (! boost::algorithm::istarts_with(url.wc_str(), "http://wxmedit.github.io/"))
		return OpenOriginalURL(url);

	static wxString lang_q = WXMLanguageQuery();

	OpenOriginalURL(url + lang_q);
}

void SetDefaultMonoFont(wxWindow* win)
{
	const wxString fontname = xm::EncodingManager::Instance().GetSystemEncoding()->GetFontName().c_str();
	int fontsize = win->GetFont().GetPointSize();
	win->SetFont(wxFont(fontsize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, fontname));
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
	const wchar_t* s = FilePathNormalCase(name).wc_str();
#if wxMAJOR_VERSION == 2
	return wxStringHash::wxCharStringHash(s);
#else
	return wxStringHash::stringHash(s);
#endif
}

wxString& WxStrAppendUCS4(wxString& ws, ucs4_t ch)
{
#ifdef __WXMSW__
	if (ch < 0x10000)
	{
		ws << wxChar(ch);
	}
	else
	{
		wxChar wbuf[2];
		xm::NonBMPtoUTF16(ch, (UChar*)wbuf);
		ws << wbuf[0];
		ws << wbuf[1];
	}
#else
	ws << wxChar(ch);
#endif
	return ws;
}

struct HexAreaRawBytesCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst) override
	{
		inst->CopyRawBytes();
	}
	virtual bool Hexadecimal() override
	{
		return false;
	}
};

struct HexAreaRegularTextCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst) override
	{
		inst->CopyRegularText();
	}
	virtual bool Hexadecimal() override
	{
		return false;
	}
};

struct HexAreaHexStringCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst) override
	{
		inst->CopyAsHexString(false);
	}
	virtual bool Hexadecimal() override
	{
		return true;
	}
};

struct HexAreaHexStringWithSpaceCopier: public HexAreaClipboardCopier
{
	virtual void Copy(MadEdit* inst) override
	{
		inst->CopyAsHexString(true);
	}
	virtual bool Hexadecimal() override
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
	virtual bool GetRawBytesFromClipboardDirectly(MadEdit* inst, std::vector<char>& cs) override
	{
		return inst->GetRawBytesFromClipboardDirectly(cs);
	}

	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) override = 0;
};

struct HexAreaNerverHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) override
	{
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaIfPossibleHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) override
	{
		if (GetRawBytesFromHexUnicodeText(cs, ucs))
			return;
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaAccordingToCopierHexPaster: public HexAreaRawBytesHexPaster
{
	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) override
	{
		HexAreaClipboardCopier& copier = HexAreaClipboardCopyProxy::Instance().GetSelectedCopier();
		if (copier.Hexadecimal() && GetRawBytesFromHexUnicodeText(cs, ucs))
			return;
		inst->ConvertToRawBytesFromUnicodeText(cs, ucs);
	}
};

struct HexAreaAlwaysHexPaster: public HexAreaClipboardPaster
{
	virtual bool GetRawBytesFromClipboardDirectly(MadEdit* inst, std::vector<char>& cs) override
	{
		return false;
	}

	virtual void GetRawBytesFromUnicodeText(MadEdit* inst, std::vector<char>& cs, const std::vector<ucs4_t>& ucs) override
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

AppPath* AppPath::s_inst = nullptr;
wxString g_wxsRegKeyWxMEdit = wxT("HKEY_CURRENT_USER\\Software\\wxMEdit");
wxString g_wxsRegValConfigInHome = wxT("ConfigInUserHome");

#ifdef __WXMSW__
bool FileWritable(const wxString& test_file)
{
	wxLogNull nolog;

	wxFile ft(test_file.c_str(), wxFile::write);
	if (!ft.IsOpened())
		return false;

	time_t t = time(nullptr);
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

void AppPath::CreateConfigDirInUserHome() const
{
	if (wxDirExists(usr_dir))
		return;
	wxLogNull nolog; // disable error message
	wxMkdir(usr_dir);
}

void AppPath::Init(const wxString& appname)
{
	cfg_file = appname + wxT(".cfg");

	wxFileName filename(GetExecutablePath());
	filename.MakeAbsolute();
	app_dir = filename.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

	usr_dir = wxStandardPaths::Get().GetUserDataDir() + wxFILE_SEP_PATH;

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

	if (cfg_in_usrhome)
		CreateConfigDirInUserHome();

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

	if (ConfigWillBeInUserHome())
		CreateConfigDirInUserHome();

	wxFileConfig *cfg=reinterpret_cast<wxFileConfig *>(wxFileConfig::Get(false));
	wxFileOutputStream another_cfg(another_dir + cfg_file);
	if (!another_cfg.IsOk())
		return;

	cfg->Save(another_cfg);
	g_DoNotSaveSettings = true;
#endif
}

static int WXMNewID(int begin, size_t count)
{
	for (size_t i=1; i<count; ++i)
		wxRegisterId(int(begin + i));
	return int(begin + count - 1);
}

WXMControlIDReserver::WXMControlIDReserver()
	: fid1(-1), fid20(-1) , rid1(-1), rid20(-1)
{
	fid1 = wxNewId();
	fid20 = WXMNewID(fid1, 20);

	rid1 = wxNewId();
	rid20 = WXMNewID(rid1, 20);
}

bool UseForceEncoding(const wxConfigBase *cfg)
{
	bool force = false;
	cfg->Read(wxT("/wxMEdit/UseForceEncoding"), &force, false);
	return force;
}

wxString GetDefaultOrForceEncoding(const wxConfigBase *cfg)
{
	if (UseForceEncoding(cfg))
		return GetForceEncoding(cfg);

	return cfg->Read(wxT("/wxMEdit/DefaultEncoding"));
}

} // namespace wxm

// get fontname from registry mime database
static inline void MSW_GetFontName(const std::wstring codepage, std::wstring &fontname)
{
#ifdef __WXMSW__
	wxLogNull nolog;

	const wxString MIMEDB(wxm::s_wxsRegkeyClasses + wxT("MIME\\Database\\Codepage\\"));
	boost::scoped_ptr<wxRegKey> pRegKey(new wxRegKey(MIMEDB + codepage.c_str()));

	if (!pRegKey->Exists())
		return;

	long cp;
	if (pRegKey->QueryValue(wxT("Family"), &cp))
		pRegKey.reset(new wxRegKey(MIMEDB + wxString::Format(wxT("%d"), cp)));

	wxString wxsfontname = fontname.c_str();
	pRegKey->QueryValue(wxT("FixedWidthFont"), wxsfontname);
	fontname = wxsfontname.wc_str();
#endif
}

std::wstring GetMSCPFontName(const std::wstring & mscp)
{
	std::wstring fontname = wxm::MonoFontName;

	if (mscp.empty())
		return fontname;

	MSW_GetFontName(mscp, fontname);
	return fontname;
}

std::wstring GetASCIIArtFontName()
{
	return wxm::ASCIIArtFontName;
}

const wchar_t * LocalizeText(const wchar_t* txt)
{
#if wxMAJOR_VERSION == 2
	return wxGetTranslation(txt);
#else
	return wxGetTranslation(txt).wc_str();
#endif
}

namespace xm
{

void BackwardBlockDumper::Dump(ubyte* buf, size_t len)
{
	wxFileOffset bsize = m_bit->m_Size;
	size_t count = len;
	size_t n;
	while (true)
	{
		n = count;
		if (bsize < (wxFileOffset)n)
			n = size_t(bsize);
		m_bit->Get(bsize - n, buf + (count -= n), n);
		if (count == 0)
			break;

		bsize = (--m_bit)->m_Size;
	}
}

void ForwardBlockDumper::Dump(ubyte* buf, size_t len)
{
	wxFileOffset bsize = m_bit->m_Size;
	size_t idx = 0;
	size_t n;
	while (true)
	{
		n = len - idx;
		if (bsize < (wxFileOffset)n)
			n = size_t(bsize);
		m_bit->Get(0, buf + idx, n);
		if ((idx += n) == len)
			break;

		bsize = (++m_bit)->m_Size;
	}
}

} // namespace xm
