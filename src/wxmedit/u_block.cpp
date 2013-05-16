///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/u_block.cpp
// Description: Unicode Blocks Information
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "u_block.h"

//==============================================================================
// Unicode-4.1 Blocks (from: http://www.unicode.org/Public/UNIDATA/Blocks.txt)

struct UnicodeBlock
{
	ucs4_t       begin;
	ucs4_t       end;
	bool         is_alphabet;
	bool         is_fullwidth;
	const wxChar *description;
};

// do not use wxGetTranslation() now
#undef _
#define _(s)    wxT(s)
const UnicodeBlock UnicodeBlocks[]=
{
	{ 0x0000, 0x001F, false, false, _("Control Characters") },
	{ 0x0020, 0x007F, true, false, _("Basic Latin") },
	{ 0x0080, 0x00FF, true, false, _("Latin-1 Supplement") },
	{ 0x0100, 0x017F, true, false, _("Latin Extended-A") },
	{ 0x0180, 0x024F, true, false, _("Latin Extended-B") },
	{ 0x0250, 0x02AF, true, false, _("IPA Extensions") },
	{ 0x02B0, 0x02FF, true, false, _("Spacing Modifier Letters") },
	{ 0x0300, 0x036F, true, false, _("Combining Diacritical Marks") },
	{ 0x0370, 0x03FF, true, false, _("Greek and Coptic") },
	{ 0x0400, 0x04FF, true, false, _("Cyrillic") },
	{ 0x0500, 0x052F, true, false, _("Cyrillic Supplement") },
	{ 0x0530, 0x058F, true, false, _("Armenian") },
	{ 0x0590, 0x05FF, true, false, _("Hebrew") },
	{ 0x0600, 0x06FF, true, false, _("Arabic") },
	{ 0x0700, 0x074F, true, false, _("Syriac") },
	{ 0x0750, 0x077F, true, false, _("Arabic Supplement") },
	{ 0x0780, 0x07BF, true, false, _("Thaana") },
	{ 0x0900, 0x097F, true, false, _("Devanagari") },
	{ 0x0980, 0x09FF, true, false, _("Bengali") },
	{ 0x0A00, 0x0A7F, true, false, _("Gurmukhi") },
	{ 0x0A80, 0x0AFF, true, false, _("Gujarati") },
	{ 0x0B00, 0x0B7F, true, false, _("Oriya") },
	{ 0x0B80, 0x0BFF, true, false, _("Tamil") },
	{ 0x0C00, 0x0C7F, true, false, _("Telugu") },
	{ 0x0C80, 0x0CFF, true, false, _("Kannada") },
	{ 0x0D00, 0x0D7F, true, false, _("Malayalam") },
	{ 0x0D80, 0x0DFF, true, false, _("Sinhala") },
	{ 0x0E00, 0x0E7F, true, false, _("Thai") },
	{ 0x0E80, 0x0EFF, true, false, _("Lao") },
	{ 0x0F00, 0x0FFF, true, false, _("Tibetan") },
	{ 0x1000, 0x109F, true, false, _("Myanmar") },
	{ 0x10A0, 0x10FF, true, false, _("Georgian") },
	{ 0x1100, 0x11FF, false, true, _("Hangul Jamo") },
	{ 0x1200, 0x137F, true, false, _("Ethiopic") },
	{ 0x1380, 0x139F, true, false, _("Ethiopic Supplement") },
	{ 0x13A0, 0x13FF, true, false, _("Cherokee") },
	{ 0x1400, 0x167F, true, false, _("Unified Canadian Aboriginal Syllabics") },
	{ 0x1680, 0x169F, true, false, _("Ogham") },
	{ 0x16A0, 0x16FF, true, false, _("Runic") },
	{ 0x1700, 0x171F, true, false, _("Tagalog") },
	{ 0x1720, 0x173F, true, false, _("Hanunoo") },
	{ 0x1740, 0x175F, true, false, _("Buhid") },
	{ 0x1760, 0x177F, true, false, _("Tagbanwa") },
	{ 0x1780, 0x17FF, true, false, _("Khmer") },
	{ 0x1800, 0x18AF, true, false, _("Mongolian") },
	{ 0x1900, 0x194F, true, false, _("Limbu") },
	{ 0x1950, 0x197F, true, false, _("Tai Le") },
	{ 0x1980, 0x19DF, true, false, _("New Tai Lue") },
	{ 0x19E0, 0x19FF, true, false, _("Khmer Symbols") },
	{ 0x1A00, 0x1A1F, true, false, _("Buginese") },
	{ 0x1D00, 0x1D7F, true, false, _("Phonetic Extensions") },
	{ 0x1D80, 0x1DBF, true, false, _("Phonetic Extensions Supplement") },
	{ 0x1DC0, 0x1DFF, true, false, _("Combining Diacritical Marks Supplement") },
	{ 0x1E00, 0x1EFF, true, false, _("Latin Extended Additional") },
	{ 0x1F00, 0x1FFF, true, false, _("Greek Extended") },
	{ 0x2000, 0x206F, true, false, _("General Punctuation") },
	{ 0x2070, 0x209F, true, false, _("Superscripts and Subscripts") },
	{ 0x20A0, 0x20CF, true, false, _("Currency Symbols") },
	{ 0x20D0, 0x20FF, true, false, _("Combining Diacritical Marks for Symbols") },
	{ 0x2100, 0x214F, true, false, _("Letterlike Symbols") },
	{ 0x2150, 0x218F, true, false, _("Number Forms") },
	{ 0x2190, 0x21FF, true, false, _("Arrows") },
	{ 0x2200, 0x22FF, true, false, _("Mathematical Operators") },
	{ 0x2300, 0x23FF, true, false, _("Miscellaneous Technical") },
	{ 0x2400, 0x243F, true, false, _("Control Pictures") },
	{ 0x2440, 0x245F, true, false, _("Optical Character Recognition") },
	{ 0x2460, 0x24FF, true, false, _("Enclosed Alphanumerics") },
	{ 0x2500, 0x257F, true, false, _("Box Drawing") },
	{ 0x2580, 0x259F, true, false, _("Block Elements") },
	{ 0x25A0, 0x25FF, true, false, _("Geometric Shapes") },
	{ 0x2600, 0x26FF, true, false, _("Miscellaneous Symbols") },
	{ 0x2700, 0x27BF, true, false, _("Dingbats") },
	{ 0x27C0, 0x27EF, true, false, _("Miscellaneous Mathematical Symbols-A") },
	{ 0x27F0, 0x27FF, true, false, _("Supplemental Arrows-A") },
	{ 0x2800, 0x28FF, true, false, _("Braille Patterns") },
	{ 0x2900, 0x297F, true, false, _("Supplemental Arrows-B") },
	{ 0x2980, 0x29FF, true, false, _("Miscellaneous Mathematical Symbols-B") },
	{ 0x2A00, 0x2AFF, true, false, _("Supplemental Mathematical Operators") },
	{ 0x2B00, 0x2BFF, true, false, _("Miscellaneous Symbols and Arrows") },
	{ 0x2C00, 0x2C5F, true, false, _("Glagolitic") },
	{ 0x2C80, 0x2CFF, true, false, _("Coptic") },
	{ 0x2D00, 0x2D2F, true, false, _("Georgian Supplement") },
	{ 0x2D30, 0x2D7F, true, false, _("Tifinagh") },
	{ 0x2D80, 0x2DDF, true, false, _("Ethiopic Extended") },
	{ 0x2E00, 0x2E7F, true, false, _("Supplemental Punctuation") },
	{ 0x2E80, 0x2EFF, false, true, _("CJK Radicals Supplement") },
	{ 0x2F00, 0x2FDF, false, true, _("Kangxi Radicals") },
	{ 0x2FF0, 0x2FFF, false, true, _("Ideographic Description Characters") },
	{ 0x3000, 0x303F, false, true, _("CJK Symbols and Punctuation") },
	{ 0x3040, 0x309F, false, true, _("Hiragana") },
	{ 0x30A0, 0x30FF, false, true, _("Katakana") },
	{ 0x3100, 0x312F, false, true, _("Bopomofo") },
	{ 0x3130, 0x318F, false, true, _("Hangul Compatibility Jamo") },
	{ 0x3190, 0x319F, false, true, _("Kanbun") },
	{ 0x31A0, 0x31BF, false, true, _("Bopomofo Extended") },
	{ 0x31C0, 0x31EF, false, true, _("CJK Strokes") },
	{ 0x31F0, 0x31FF, false, true, _("Katakana Phonetic Extensions") },
	{ 0x3200, 0x32FF, false, true, _("Enclosed CJK Letters and Months") },
	{ 0x3300, 0x33FF, false, true, _("CJK Compatibility") },
	{ 0x3400, 0x4DBF, false, true, _("CJK Unified Ideographs Extension A") },
	{ 0x4DC0, 0x4DFF, false, true, _("Yijing Hexagram Symbols") },
	{ 0x4E00, 0x9FFF, false, true, _("CJK Unified Ideographs") },
	{ 0xA000, 0xA48F, false, true, _("Yi Syllables") },
	{ 0xA490, 0xA4CF, false, true, _("Yi Radicals") },
	{ 0xA700, 0xA71F, true, false, _("Modifier Tone Letters") },
	{ 0xA800, 0xA82F, true, false, _("Syloti Nagri") },
	{ 0xAC00, 0xD7AF, false, true, _("Hangul Syllables") },
	{ 0xD800, 0xDB7F, true, false, _("High Surrogates") },
	{ 0xDB80, 0xDBFF, true, false, _("High Private Use Surrogates") },
	{ 0xDC00, 0xDFFF, true, false, _("Low Surrogates") },
	{ 0xE000, 0xF8FF, false, true, _("Private Use Area") },
	{ 0xF900, 0xFAFF, false, true, _("CJK Compatibility Ideographs") },
	{ 0xFB00, 0xFB4F, true, false, _("Alphabetic Presentation Forms") },
	{ 0xFB50, 0xFDFF, true, false, _("Arabic Presentation Forms-A") },
	{ 0xFE00, 0xFE0F, true, false, _("Variation Selectors") },
	{ 0xFE10, 0xFE1F, true, false, _("Vertical Forms") },
	{ 0xFE20, 0xFE2F, true, false, _("Combining Half Marks") },
	{ 0xFE30, 0xFE4F, false, true, _("CJK Compatibility Forms") },
	{ 0xFE50, 0xFE6F, true, false, _("Small Form Variants") },
	{ 0xFE70, 0xFEFF, true, false, _("Arabic Presentation Forms-B") },
	{ 0xFF00, 0xFFEF, true, false, _("Halfwidth and Fullwidth Forms") },
	{ 0xFFF0, 0xFFFF, true, false, _("Specials") },
	{ 0x10000, 0x1007F, true, false, _("Linear B Syllabary") },
	{ 0x10080, 0x100FF, true, false, _("Linear B Ideograms") },
	{ 0x10100, 0x1013F, true, false, _("Aegean Numbers") },
	{ 0x10140, 0x1018F, true, false, _("Ancient Greek Numbers") },
	{ 0x10300, 0x1032F, true, false, _("Old Italic") },
	{ 0x10330, 0x1034F, true, false, _("Gothic") },
	{ 0x10380, 0x1039F, true, false, _("Ugaritic") },
	{ 0x103A0, 0x103DF, true, false, _("Old Persian") },
	{ 0x10400, 0x1044F, true, false, _("Deseret") },
	{ 0x10450, 0x1047F, true, false, _("Shavian") },
	{ 0x10480, 0x104AF, true, false, _("Osmanya") },
	{ 0x10800, 0x1083F, true, false, _("Cypriot Syllabary") },
	{ 0x10A00, 0x10A5F, true, false, _("Kharoshthi") },
	{ 0x1D000, 0x1D0FF, true, false, _("Byzantine Musical Symbols") },
	{ 0x1D100, 0x1D1FF, true, false, _("Musical Symbols") },
	{ 0x1D200, 0x1D24F, true, false, _("Ancient Greek Musical Notation") },
	{ 0x1D300, 0x1D35F, true, false, _("Tai Xuan Jing Symbols") },
	{ 0x1D400, 0x1D7FF, true, false, _("Mathematical Alphanumeric Symbols") },
	{ 0x20000, 0x2A6DF, false, true, _("CJK Unified Ideographs Extension B") },
	{ 0x2F800, 0x2FA1F, false, true, _("CJK Compatibility Ideographs Supplement") },
	{ 0xE0000, 0xE007F, true, false, _("Tags") },
	{ 0xE0100, 0xE01EF, true, false, _("Variation Selectors Supplement") },
	{ 0xF0000, 0xFFFFF, false, true, _("Supplementary Private Use Area-A") },
	{ 0x100000, 0x10FFFF, false, true, _("Supplementary Private Use Area-B") }
};
// restore the definition of _(s)
#undef _
#define _(s)    wxGetTranslation(_T(s))

const int UnicodeBlocksCount = sizeof(UnicodeBlocks) / sizeof(UnicodeBlock) ;

int FindBlockIndex(ucs4_t uc)
{
	if(uc<0 || uc>0x10FFFF) return UnicodeBlocksCount; // not in the ranges
	if(uc<=0x1F) return 0;
	if(uc<=0x7F) return 1;

	static const UnicodeBlock *pblock=UnicodeBlocks;
	static int index=0;

	if(uc >= pblock->begin)
	{
		if(uc <= pblock->end) return index;

		do
		{
			++index;
			++pblock;
			wxASSERT(index < UnicodeBlocksCount);
		}while(uc > pblock->end);

		if(uc >= pblock->begin) return index;
	}
	else //if(uc < pblock->begin)
	{
		do
		{
			--index;
			--pblock;
			wxASSERT(index >= 0);
		}while(uc < pblock->begin);

		if(uc <= pblock->end) return index;
	}

	return UnicodeBlocksCount; // not in the ranges
}

int UnicodeBlockSet::FindBlockIndex(ucs4_t uc)
{
	return ::FindBlockIndex(uc);
}

int UnicodeBlockSet::GetUnicodeBlocksCount()
{
	return UnicodeBlocksCount;
}


ucs4_t UnicodeBlockSet::Begin(int index)
{
	return UnicodeBlocks[index].begin;
}

ucs4_t UnicodeBlockSet::End(int index)
{
	return UnicodeBlocks[index].end;
}

bool UnicodeBlockSet::IsAlphabet(int index)
{
	return UnicodeBlocks[index].is_alphabet;
}

bool UnicodeBlockSet::IsFullWidth(int index)
{
	return UnicodeBlocks[index].is_fullwidth;
}

const wxChar* UnicodeBlockSet::Description(int index)
{
	return UnicodeBlocks[index].description;
}


bool UnicodeBlockSet::Valid(int index)
{
	return index < GetUnicodeBlocksCount();
}



UnicodeBlockCharCounter::UnicodeBlockCharCounter():
	m_index(0)
{
	m_counts.resize(m_ublock_set.GetUnicodeBlocksCount()+1, 0); // all counts=0
}


void UnicodeBlockCharCounter::Count(int index)
{
	m_counts[index]++;
}

int UnicodeBlockCharCounter::BlockIndexBegin()
{
	for(m_index=0; m_index<m_ublock_set.GetUnicodeBlocksCount(); ++m_index)
	{
		if(m_counts[m_index]>0)
			return m_index;
	}
	return m_index;
}

int UnicodeBlockCharCounter::NextBlock()
{
	for(++m_index; m_index<m_ublock_set.GetUnicodeBlocksCount(); ++m_index)
	{
		if(m_counts[m_index]>0)
			return m_index;
	}
	return m_index;
}

bool UnicodeBlockCharCounter::IsValidBlock(int index)
{
	return index < m_ublock_set.GetUnicodeBlocksCount();
}

int UnicodeBlockCharCounter::GetBlockCharCount(int index)
{
	return m_counts[index];
}

int UnicodeBlockCharCounter::GetInvalidBlockCharCount()
{
	return m_counts[m_ublock_set.GetUnicodeBlocksCount()];
}
