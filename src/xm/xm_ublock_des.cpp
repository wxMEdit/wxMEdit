///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock_des.cpp
// Description: Unicode Block Descriptions
// Copyright:   2013-2015  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "xm_ublock.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

void UnicodeBlockSet::InitBlockDescriptions()
{
	m_des_map[UBLOCK_NO_BLOCK] = L"No_Block";
	m_des_map[UBLOCK_BASIC_LATIN] = L"Basic Latin";
	m_des_map[UBLOCK_LATIN_1_SUPPLEMENT] = L"Latin-1 Supplement";
	m_des_map[UBLOCK_LATIN_EXTENDED_A] = L"Latin Extended-A";
	m_des_map[UBLOCK_LATIN_EXTENDED_B] = L"Latin Extended-B";
	m_des_map[UBLOCK_IPA_EXTENSIONS] = L"IPA Extensions";
	m_des_map[UBLOCK_SPACING_MODIFIER_LETTERS] = L"Spacing Modifier Letters";
	m_des_map[UBLOCK_COMBINING_DIACRITICAL_MARKS] = L"Combining Diacritical Marks";
	m_des_map[UBLOCK_GREEK] = L"Greek and Coptic";
	m_des_map[UBLOCK_CYRILLIC] = L"Cyrillic";
	m_des_map[UBLOCK_ARMENIAN] = L"Armenian";
	m_des_map[UBLOCK_HEBREW] = L"Hebrew";
	m_des_map[UBLOCK_ARABIC] = L"Arabic";
	m_des_map[UBLOCK_SYRIAC] = L"Syriac";
	m_des_map[UBLOCK_THAANA] = L"Thaana";
	m_des_map[UBLOCK_DEVANAGARI] = L"Devanagari";
	m_des_map[UBLOCK_BENGALI] = L"Bengali";
	m_des_map[UBLOCK_GURMUKHI] = L"Gurmukhi";
	m_des_map[UBLOCK_GUJARATI] = L"Gujarati";
	m_des_map[UBLOCK_ORIYA] = L"Oriya";
	m_des_map[UBLOCK_TAMIL] = L"Tamil";
	m_des_map[UBLOCK_TELUGU] = L"Telugu";
	m_des_map[UBLOCK_KANNADA] = L"Kannada";
	m_des_map[UBLOCK_MALAYALAM] = L"Malayalam";
	m_des_map[UBLOCK_SINHALA] = L"Sinhala";
	m_des_map[UBLOCK_THAI] = L"Thai";
	m_des_map[UBLOCK_LAO] = L"Lao";
	m_des_map[UBLOCK_TIBETAN] = L"Tibetan";
	m_des_map[UBLOCK_MYANMAR] = L"Myanmar";
	m_des_map[UBLOCK_GEORGIAN] = L"Georgian";
	m_des_map[UBLOCK_HANGUL_JAMO] = L"Hangul Jamo";
	m_des_map[UBLOCK_ETHIOPIC] = L"Ethiopic";
	m_des_map[UBLOCK_CHEROKEE] = L"Cherokee";
	m_des_map[UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS] = L"Unified Canadian Aboriginal Syllabics";
	m_des_map[UBLOCK_OGHAM] = L"Ogham";
	m_des_map[UBLOCK_RUNIC] = L"Runic";
	m_des_map[UBLOCK_KHMER] = L"Khmer";
	m_des_map[UBLOCK_MONGOLIAN] = L"Mongolian";
	m_des_map[UBLOCK_LATIN_EXTENDED_ADDITIONAL] = L"Latin Extended Additional";
	m_des_map[UBLOCK_GREEK_EXTENDED] = L"Greek Extended";
	m_des_map[UBLOCK_GENERAL_PUNCTUATION] = L"General Punctuation";
	m_des_map[UBLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS] = L"Superscripts and Subscripts";
	m_des_map[UBLOCK_CURRENCY_SYMBOLS] = L"Currency Symbols";
	m_des_map[UBLOCK_COMBINING_MARKS_FOR_SYMBOLS] = L"Combining Diacritical Marks for Symbols";
	m_des_map[UBLOCK_LETTERLIKE_SYMBOLS] = L"Letterlike Symbols";
	m_des_map[UBLOCK_NUMBER_FORMS] = L"Number Forms";
	m_des_map[UBLOCK_ARROWS] = L"Arrows";
	m_des_map[UBLOCK_MATHEMATICAL_OPERATORS] = L"Mathematical Operators";
	m_des_map[UBLOCK_MISCELLANEOUS_TECHNICAL] = L"Miscellaneous Technical";
	m_des_map[UBLOCK_CONTROL_PICTURES] = L"Control Pictures";
	m_des_map[UBLOCK_OPTICAL_CHARACTER_RECOGNITION] = L"Optical Character Recognition";
	m_des_map[UBLOCK_ENCLOSED_ALPHANUMERICS] = L"Enclosed Alphanumerics";
	m_des_map[UBLOCK_BOX_DRAWING] = L"Box Drawing";
	m_des_map[UBLOCK_BLOCK_ELEMENTS] = L"Block Elements";
	m_des_map[UBLOCK_GEOMETRIC_SHAPES] = L"Geometric Shapes";
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS] = L"Miscellaneous Symbols";
	m_des_map[UBLOCK_DINGBATS] = L"Dingbats";
	m_des_map[UBLOCK_BRAILLE_PATTERNS] = L"Braille Patterns";
	m_des_map[UBLOCK_CJK_RADICALS_SUPPLEMENT] = L"CJK Radicals Supplement";
	m_des_map[UBLOCK_KANGXI_RADICALS] = L"Kangxi Radicals";
	m_des_map[UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS] = L"Ideographic Description Characters";
	m_des_map[UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION] = L"CJK Symbols and Punctuation";
	m_des_map[UBLOCK_HIRAGANA] = L"Hiragana";
	m_des_map[UBLOCK_KATAKANA] = L"Katakana";
	m_des_map[UBLOCK_BOPOMOFO] = L"Bopomofo";
	m_des_map[UBLOCK_HANGUL_COMPATIBILITY_JAMO] = L"Hangul Compatibility Jamo";
	m_des_map[UBLOCK_KANBUN] = L"Kanbun";
	m_des_map[UBLOCK_BOPOMOFO_EXTENDED] = L"Bopomofo Extended";
	m_des_map[UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS] = L"Enclosed CJK Letters and Months";
	m_des_map[UBLOCK_CJK_COMPATIBILITY] = L"CJK Compatibility";
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A] = L"CJK Unified Ideographs Extension A";
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS] = L"CJK Unified Ideographs";
	m_des_map[UBLOCK_YI_SYLLABLES] = L"Yi Syllables";
	m_des_map[UBLOCK_YI_RADICALS] = L"Yi Radicals";
	m_des_map[UBLOCK_HANGUL_SYLLABLES] = L"Hangul Syllables";
	m_des_map[UBLOCK_HIGH_SURROGATES] = L"High Surrogates";
	m_des_map[UBLOCK_HIGH_PRIVATE_USE_SURROGATES] = L"High Private Use Surrogates";
	m_des_map[UBLOCK_LOW_SURROGATES] = L"Low Surrogates";
	m_des_map[UBLOCK_PRIVATE_USE_AREA] = L"Private Use Area";
	m_des_map[UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS] = L"CJK Compatibility Ideographs";
	m_des_map[UBLOCK_ALPHABETIC_PRESENTATION_FORMS] = L"Alphabetic Presentation Forms";
	m_des_map[UBLOCK_ARABIC_PRESENTATION_FORMS_A] = L"Arabic Presentation Forms-A";
	m_des_map[UBLOCK_COMBINING_HALF_MARKS] = L"Combining Half Marks";
	m_des_map[UBLOCK_CJK_COMPATIBILITY_FORMS] = L"CJK Compatibility Forms";
	m_des_map[UBLOCK_SMALL_FORM_VARIANTS] = L"Small Form Variants";
	m_des_map[UBLOCK_ARABIC_PRESENTATION_FORMS_B] = L"Arabic Presentation Forms-B";
	m_des_map[UBLOCK_SPECIALS] = L"Specials";
	m_des_map[UBLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS] = L"Halfwidth and Fullwidth Forms";
	m_des_map[UBLOCK_OLD_ITALIC] = L"Old Italic";
	m_des_map[UBLOCK_GOTHIC] = L"Gothic";
	m_des_map[UBLOCK_DESERET] = L"Deseret";
	m_des_map[UBLOCK_BYZANTINE_MUSICAL_SYMBOLS] = L"Byzantine Musical Symbols";
	m_des_map[UBLOCK_MUSICAL_SYMBOLS] = L"Musical Symbols";
	m_des_map[UBLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS] = L"Mathematical Alphanumeric Symbols";
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B] = L"CJK Unified Ideographs Extension B";
	m_des_map[UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT] = L"CJK Compatibility Ideographs Supplement";
	m_des_map[UBLOCK_TAGS] = L"Tags";
	m_des_map[UBLOCK_CYRILLIC_SUPPLEMENT] = L"Cyrillic Supplement";
	m_des_map[UBLOCK_TAGALOG] = L"Tagalog";
	m_des_map[UBLOCK_HANUNOO] = L"Hanunoo";
	m_des_map[UBLOCK_BUHID] = L"Buhid";
	m_des_map[UBLOCK_TAGBANWA] = L"Tagbanwa";
	m_des_map[UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A] = L"Miscellaneous Mathematical Symbols-A";
	m_des_map[UBLOCK_SUPPLEMENTAL_ARROWS_A] = L"Supplemental Arrows-A";
	m_des_map[UBLOCK_SUPPLEMENTAL_ARROWS_B] = L"Supplemental Arrows-B";
	m_des_map[UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B] = L"Miscellaneous Mathematical Symbols-B";
	m_des_map[UBLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS] = L"Supplemental Mathematical Operators";
	m_des_map[UBLOCK_KATAKANA_PHONETIC_EXTENSIONS] = L"Katakana Phonetic Extensions";
	m_des_map[UBLOCK_VARIATION_SELECTORS] = L"Variation Selectors";
	m_des_map[UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A] = L"Supplementary Private Use Area-A";
	m_des_map[UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B] = L"Supplementary Private Use Area-B";
	m_des_map[UBLOCK_LIMBU] = L"Limbu";
	m_des_map[UBLOCK_TAI_LE] = L"Tai Le";
	m_des_map[UBLOCK_KHMER_SYMBOLS] = L"Khmer Symbols";
	m_des_map[UBLOCK_PHONETIC_EXTENSIONS] = L"Phonetic Extensions";
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS] = L"Miscellaneous Symbols and Arrows";
	m_des_map[UBLOCK_YIJING_HEXAGRAM_SYMBOLS] = L"Yijing Hexagram Symbols";
	m_des_map[UBLOCK_LINEAR_B_SYLLABARY] = L"Linear B Syllabary";
	m_des_map[UBLOCK_LINEAR_B_IDEOGRAMS] = L"Linear B Ideograms";
	m_des_map[UBLOCK_AEGEAN_NUMBERS] = L"Aegean Numbers";
	m_des_map[UBLOCK_UGARITIC] = L"Ugaritic";
	m_des_map[UBLOCK_SHAVIAN] = L"Shavian";
	m_des_map[UBLOCK_OSMANYA] = L"Osmanya";
	m_des_map[UBLOCK_CYPRIOT_SYLLABARY] = L"Cypriot Syllabary";
	m_des_map[UBLOCK_TAI_XUAN_JING_SYMBOLS] = L"Tai Xuan Jing Symbols";
	m_des_map[UBLOCK_VARIATION_SELECTORS_SUPPLEMENT] = L"Variation Selectors Supplement";
	m_des_map[UBLOCK_ANCIENT_GREEK_MUSICAL_NOTATION] = L"Ancient Greek Musical Notation";
	m_des_map[UBLOCK_ANCIENT_GREEK_NUMBERS] = L"Ancient Greek Numbers";
	m_des_map[UBLOCK_ARABIC_SUPPLEMENT] = L"Arabic Supplement";
	m_des_map[UBLOCK_BUGINESE] = L"Buginese";
	m_des_map[UBLOCK_CJK_STROKES] = L"CJK Strokes";
	m_des_map[UBLOCK_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT] = L"Combining Diacritical Marks Supplement";
	m_des_map[UBLOCK_COPTIC] = L"Coptic";
	m_des_map[UBLOCK_ETHIOPIC_EXTENDED] = L"Ethiopic Extended";
	m_des_map[UBLOCK_ETHIOPIC_SUPPLEMENT] = L"Ethiopic Supplement";
	m_des_map[UBLOCK_GEORGIAN_SUPPLEMENT] = L"Georgian Supplement";
	m_des_map[UBLOCK_GLAGOLITIC] = L"Glagolitic";
	m_des_map[UBLOCK_KHAROSHTHI] = L"Kharoshthi";
	m_des_map[UBLOCK_MODIFIER_TONE_LETTERS] = L"Modifier Tone Letters";
	m_des_map[UBLOCK_NEW_TAI_LUE] = L"New Tai Lue";
	m_des_map[UBLOCK_OLD_PERSIAN] = L"Old Persian";
	m_des_map[UBLOCK_PHONETIC_EXTENSIONS_SUPPLEMENT] = L"Phonetic Extensions Supplement";
	m_des_map[UBLOCK_SUPPLEMENTAL_PUNCTUATION] = L"Supplemental Punctuation";
	m_des_map[UBLOCK_SYLOTI_NAGRI] = L"Syloti Nagri";
	m_des_map[UBLOCK_TIFINAGH] = L"Tifinagh";
	m_des_map[UBLOCK_VERTICAL_FORMS] = L"Vertical Forms";
	m_des_map[UBLOCK_NKO] = L"NKo";
	m_des_map[UBLOCK_BALINESE] = L"Balinese";
	m_des_map[UBLOCK_LATIN_EXTENDED_C] = L"Latin Extended-C";
	m_des_map[UBLOCK_LATIN_EXTENDED_D] = L"Latin Extended-D";
	m_des_map[UBLOCK_PHAGS_PA] = L"Phags-pa";
	m_des_map[UBLOCK_PHOENICIAN] = L"Phoenician";
	m_des_map[UBLOCK_CUNEIFORM] = L"Cuneiform";
	m_des_map[UBLOCK_CUNEIFORM_NUMBERS_AND_PUNCTUATION] = L"Cuneiform Numbers and Punctuation";
	m_des_map[UBLOCK_COUNTING_ROD_NUMERALS] = L"Counting Rod Numerals";
	m_des_map[UBLOCK_SUNDANESE] = L"Sundanese";
	m_des_map[UBLOCK_LEPCHA] = L"Lepcha";
	m_des_map[UBLOCK_OL_CHIKI] = L"Ol Chiki";
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_A] = L"Cyrillic Extended-A";
	m_des_map[UBLOCK_VAI] = L"Vai";
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_B] = L"Cyrillic Extended-B";
	m_des_map[UBLOCK_SAURASHTRA] = L"Saurashtra";
	m_des_map[UBLOCK_KAYAH_LI] = L"Kayah Li";
	m_des_map[UBLOCK_REJANG] = L"Rejang";
	m_des_map[UBLOCK_CHAM] = L"Cham";
	m_des_map[UBLOCK_ANCIENT_SYMBOLS] = L"Ancient Symbols";
	m_des_map[UBLOCK_PHAISTOS_DISC] = L"Phaistos Disc";
	m_des_map[UBLOCK_LYCIAN] = L"Lycian";
	m_des_map[UBLOCK_CARIAN] = L"Carian";
	m_des_map[UBLOCK_LYDIAN] = L"Lydian";
	m_des_map[UBLOCK_MAHJONG_TILES] = L"Mahjong Tiles";
	m_des_map[UBLOCK_DOMINO_TILES] = L"Domino Tiles";
#if ((U_ICU_VERSION_MAJOR_NUM << 8) + U_ICU_VERSION_MINOR_NUM >= 0x0404) // 4.4
	m_des_map[UBLOCK_SAMARITAN] = L"Samaritan";
	m_des_map[UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED] = L"Unified Canadian Aboriginal Syllabics Extended";
	m_des_map[UBLOCK_TAI_THAM] = L"Tai Tham";
	m_des_map[UBLOCK_VEDIC_EXTENSIONS] = L"Vedic Extensions";
	m_des_map[UBLOCK_LISU] = L"Lisu";
	m_des_map[UBLOCK_BAMUM] = L"Bamum";
	m_des_map[UBLOCK_COMMON_INDIC_NUMBER_FORMS] = L"Common Indic Number Forms";
	m_des_map[UBLOCK_DEVANAGARI_EXTENDED] = L"Devanagari Extended";
	m_des_map[UBLOCK_HANGUL_JAMO_EXTENDED_A] = L"Hangul Jamo Extended-A";
	m_des_map[UBLOCK_JAVANESE] = L"Javanese";
	m_des_map[UBLOCK_MYANMAR_EXTENDED_A] = L"Myanmar Extended-A";
	m_des_map[UBLOCK_TAI_VIET] = L"Tai Viet";
	m_des_map[UBLOCK_MEETEI_MAYEK] = L"Meetei Mayek";
	m_des_map[UBLOCK_HANGUL_JAMO_EXTENDED_B] = L"Hangul Jamo Extended-B";
	m_des_map[UBLOCK_IMPERIAL_ARAMAIC] = L"Imperial Aramaic";
	m_des_map[UBLOCK_OLD_SOUTH_ARABIAN] = L"Old South Arabian";
	m_des_map[UBLOCK_AVESTAN] = L"Avestan";
	m_des_map[UBLOCK_INSCRIPTIONAL_PARTHIAN] = L"Inscriptional Parthian";
	m_des_map[UBLOCK_INSCRIPTIONAL_PAHLAVI] = L"Inscriptional Pahlavi";
	m_des_map[UBLOCK_OLD_TURKIC] = L"Old Turkic";
	m_des_map[UBLOCK_RUMI_NUMERAL_SYMBOLS] = L"Rumi Numeral Symbols";
	m_des_map[UBLOCK_KAITHI] = L"Kaithi";
	m_des_map[UBLOCK_EGYPTIAN_HIEROGLYPHS] = L"Egyptian Hieroglyphs";
	m_des_map[UBLOCK_ENCLOSED_ALPHANUMERIC_SUPPLEMENT] = L"Enclosed Alphanumeric Supplement";
	m_des_map[UBLOCK_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT] = L"Enclosed Ideographic Supplement";
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C] = L"CJK Unified Ideographs Extension C";
# if ((U_ICU_VERSION_MAJOR_NUM << 8) + U_ICU_VERSION_MINOR_NUM >= 0x0406) // 4.6
	m_des_map[UBLOCK_MANDAIC] = L"Mandaic";
	m_des_map[UBLOCK_BATAK] = L"Batak";
	m_des_map[UBLOCK_ETHIOPIC_EXTENDED_A] = L"Ethiopic Extended-A";
	m_des_map[UBLOCK_BRAHMI] = L"Brahmi";
	m_des_map[UBLOCK_BAMUM_SUPPLEMENT] = L"Bamum Supplement";
	m_des_map[UBLOCK_KANA_SUPPLEMENT] = L"Kana Supplement";
	m_des_map[UBLOCK_PLAYING_CARDS] = L"Playing Cards";
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS] = L"Miscellaneous Symbols And Pictographs";
	m_des_map[UBLOCK_EMOTICONS] = L"Emoticons";
	m_des_map[UBLOCK_TRANSPORT_AND_MAP_SYMBOLS] = L"Transport And Map Symbols";
	m_des_map[UBLOCK_ALCHEMICAL_SYMBOLS] = L"Alchemical Symbols";
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D] = L"CJK Unified Ideographs Extension D";
#   if (U_ICU_VERSION_MAJOR_NUM >= 49) // 49
	m_des_map[UBLOCK_ARABIC_EXTENDED_A] = L"Arabic Extended-A";
	m_des_map[UBLOCK_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS] = L"Arabic Mathematical Alphabetic Symbols";
	m_des_map[UBLOCK_CHAKMA] = L"Chakma";
	m_des_map[UBLOCK_MEETEI_MAYEK_EXTENSIONS] = L"Meetei Mayek Extensions";
	m_des_map[UBLOCK_MEROITIC_CURSIVE] = L"Meroitic Cursive";
	m_des_map[UBLOCK_MEROITIC_HIEROGLYPHS] = L"Meroitic Hieroglyphs";
	m_des_map[UBLOCK_MIAO] = L"Miao";
	m_des_map[UBLOCK_SHARADA] = L"Sharada";
	m_des_map[UBLOCK_SORA_SOMPENG] = L"Sora Sompeng";
	m_des_map[UBLOCK_SUNDANESE_SUPPLEMENT] = L"Sundanese Supplement";
	m_des_map[UBLOCK_TAKRI] = L"Takri";
	m_des_map[UBLOCK_INVALID_CODE] = L"Invalid Unicode Characters";
#  endif // 49
# endif // 4.6
#endif // 4.4
}

}; // namespace xm
