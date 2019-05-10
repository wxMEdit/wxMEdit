///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxm/case_conv.h
// Description: Case Conversion for wxWidgets String
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_CASE_CONV_H_
#define _WXM_CASE_CONV_H_

#include "../xm/cxx11.h"
#include "wx_icu.h"
#include <unicode/uchar.h>

namespace wxm
{
	wxString WxStrToLower(const wxString& wxs);
	wxString WxStrToUpper(const wxString& wxs);
	inline wxString WxStrToNormalCase(const wxString& wxs)
	{
		return WxStrToLower(wxs);
	}

	int WxCaseCompare(const wxString& wxs1, const wxString& wxs2);

	inline void LowerWxStr(wxString& wxs)
	{
		wxs = WxStrToLower(wxs);
	}

	inline void UpperWxStr(wxString& wxs)
	{
		wxs = WxStrToUpper(wxs);
	}


	struct WxCaseConverter
	{
		void Convert();

		bool Modified()
		{
			return m_modified;
		}

		virtual ~WxCaseConverter()
		{ }
		WxCaseConverter(wxString& txt)
			:m_txt(txt), m_idx(0), m_in_surrogate(false), m_modified(false)
		{ }

	protected:
		virtual UChar32 ConvertCase(UChar32 ch) = 0;
	private:
		UChar32 PeekChar();
		void SaveChar(UChar32 ch);
		void NextChar();
		bool Finished()
		{
			return m_idx >= m_txt.Length();
		}

		wxString& m_txt;
		size_t m_idx;
		bool m_in_surrogate;
		bool m_modified;
	};

	struct WxLowerCaseConverter: public WxCaseConverter
	{
		WxLowerCaseConverter(wxString& txt):WxCaseConverter(txt)
		{ }

	protected:
		virtual UChar32 ConvertCase(UChar32 ch) override
		{
			return u_tolower(ch);
		}
	};

	struct WxUpperCaseConverter: public WxCaseConverter
	{
		WxUpperCaseConverter(wxString& txt):WxCaseConverter(txt)
		{ }

	protected:
		virtual UChar32 ConvertCase(UChar32 ch) override
		{
			return u_toupper(ch);
		}
	};

	struct WxCaseInverter: public WxCaseConverter
	{
		WxCaseInverter(wxString& txt):WxCaseConverter(txt)
		{ }

	protected:
		virtual UChar32 ConvertCase(UChar32 ch) override
		{
			if (u_islower(ch))
				return u_toupper(ch);

			return u_tolower(ch);
		}
	};

} //namespace wxm

#endif //_WXM_CASE_CONV_H_
