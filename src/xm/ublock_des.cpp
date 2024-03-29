///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock_des.cpp
// Description: Unicode Block Descriptions
// Copyright:   2013-2023  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "ublock.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

void UnicodeBlockSet::InitBlockDescriptions()
{
#undef W_DELAY_TR_
#undef _
#define W_DELAY_TR_(str) L ## str
#define _(str) W_DELAY_TR_(str)
	m_des_map[UBLOCK_NO_BLOCK] = _("No_Block");
	m_des_map[UBLOCK_BASIC_LATIN] = _("Basic Latin");
	m_des_map[UBLOCK_LATIN_1_SUPPLEMENT] = _("Latin-1 Supplement");
	m_des_map[UBLOCK_LATIN_EXTENDED_A] = _("Latin Extended-A");
	m_des_map[UBLOCK_LATIN_EXTENDED_B] = _("Latin Extended-B");
	m_des_map[UBLOCK_IPA_EXTENSIONS] = _("IPA Extensions");
	m_des_map[UBLOCK_SPACING_MODIFIER_LETTERS] = _("Spacing Modifier Letters");
	m_des_map[UBLOCK_COMBINING_DIACRITICAL_MARKS] = _("Combining Diacritical Marks");
	m_des_map[UBLOCK_GREEK] = _("Greek and Coptic");
	m_des_map[UBLOCK_CYRILLIC] = _("Cyrillic");
	m_des_map[UBLOCK_ARMENIAN] = _("Armenian");
	m_des_map[UBLOCK_HEBREW] = _("Hebrew");
	m_des_map[UBLOCK_ARABIC] = _("Arabic");
	m_des_map[UBLOCK_SYRIAC] = _("Syriac");
	m_des_map[UBLOCK_THAANA] = _("Thaana");
	m_des_map[UBLOCK_DEVANAGARI] = _("Devanagari");
	m_des_map[UBLOCK_BENGALI] = _("Bengali");
	m_des_map[UBLOCK_GURMUKHI] = _("Gurmukhi");
	m_des_map[UBLOCK_GUJARATI] = _("Gujarati");
	m_des_map[UBLOCK_ORIYA] = _("Oriya");
	m_des_map[UBLOCK_TAMIL] = _("Tamil");
	m_des_map[UBLOCK_TELUGU] = _("Telugu");
	m_des_map[UBLOCK_KANNADA] = _("Kannada");
	m_des_map[UBLOCK_MALAYALAM] = _("Malayalam");
	m_des_map[UBLOCK_SINHALA] = _("Sinhala");
	m_des_map[UBLOCK_THAI] = _("Thai");
	m_des_map[UBLOCK_LAO] = _("Lao");
	m_des_map[UBLOCK_TIBETAN] = _("Tibetan");
	m_des_map[UBLOCK_MYANMAR] = _("Myanmar");
	m_des_map[UBLOCK_GEORGIAN] = _("Georgian");
	m_des_map[UBLOCK_HANGUL_JAMO] = _("Hangul Jamo");
	m_des_map[UBLOCK_ETHIOPIC] = _("Ethiopic");
	m_des_map[UBLOCK_CHEROKEE] = _("Cherokee");
	m_des_map[UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS] = _("Unified Canadian Aboriginal Syllabics");
	m_des_map[UBLOCK_OGHAM] = _("Ogham");
	m_des_map[UBLOCK_RUNIC] = _("Runic");
	m_des_map[UBLOCK_KHMER] = _("Khmer");
	m_des_map[UBLOCK_MONGOLIAN] = _("Mongolian");
	m_des_map[UBLOCK_LATIN_EXTENDED_ADDITIONAL] = _("Latin Extended Additional");
	m_des_map[UBLOCK_GREEK_EXTENDED] = _("Greek Extended");
	m_des_map[UBLOCK_GENERAL_PUNCTUATION] = _("General Punctuation");
	m_des_map[UBLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS] = _("Superscripts and Subscripts");
	m_des_map[UBLOCK_CURRENCY_SYMBOLS] = _("Currency Symbols");
	m_des_map[UBLOCK_COMBINING_MARKS_FOR_SYMBOLS] = _("Combining Diacritical Marks for Symbols");
	m_des_map[UBLOCK_LETTERLIKE_SYMBOLS] = _("Letterlike Symbols");
	m_des_map[UBLOCK_NUMBER_FORMS] = _("Number Forms");
	m_des_map[UBLOCK_ARROWS] = _("Arrows");
	m_des_map[UBLOCK_MATHEMATICAL_OPERATORS] = _("Mathematical Operators");
	m_des_map[UBLOCK_MISCELLANEOUS_TECHNICAL] = _("Miscellaneous Technical");
	m_des_map[UBLOCK_CONTROL_PICTURES] = _("Control Pictures");
	m_des_map[UBLOCK_OPTICAL_CHARACTER_RECOGNITION] = _("Optical Character Recognition");
	m_des_map[UBLOCK_ENCLOSED_ALPHANUMERICS] = _("Enclosed Alphanumerics");
	m_des_map[UBLOCK_BOX_DRAWING] = _("Box Drawing");
	m_des_map[UBLOCK_BLOCK_ELEMENTS] = _("Block Elements");
	m_des_map[UBLOCK_GEOMETRIC_SHAPES] = _("Geometric Shapes");
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS] = _("Miscellaneous Symbols");
	m_des_map[UBLOCK_DINGBATS] = _("Dingbats");
	m_des_map[UBLOCK_BRAILLE_PATTERNS] = _("Braille Patterns");
	m_des_map[UBLOCK_CJK_RADICALS_SUPPLEMENT] = _("CJK Radicals Supplement");
	m_des_map[UBLOCK_KANGXI_RADICALS] = _("Kangxi Radicals");
	m_des_map[UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS] = _("Ideographic Description Characters");
	m_des_map[UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION] = _("CJK Symbols and Punctuation");
	m_des_map[UBLOCK_HIRAGANA] = _("Hiragana");
	m_des_map[UBLOCK_KATAKANA] = _("Katakana");
	m_des_map[UBLOCK_BOPOMOFO] = _("Bopomofo");
	m_des_map[UBLOCK_HANGUL_COMPATIBILITY_JAMO] = _("Hangul Compatibility Jamo");
	m_des_map[UBLOCK_KANBUN] = _("Kanbun");
	m_des_map[UBLOCK_BOPOMOFO_EXTENDED] = _("Bopomofo Extended");
	m_des_map[UBLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS] = _("Enclosed CJK Letters and Months");
	m_des_map[UBLOCK_CJK_COMPATIBILITY] = _("CJK Compatibility");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A] = _("CJK Unified Ideographs Extension A");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS] = _("CJK Unified Ideographs");
	m_des_map[UBLOCK_YI_SYLLABLES] = _("Yi Syllables");
	m_des_map[UBLOCK_YI_RADICALS] = _("Yi Radicals");
	m_des_map[UBLOCK_HANGUL_SYLLABLES] = _("Hangul Syllables");
	m_des_map[UBLOCK_HIGH_SURROGATES] = _("High Surrogates");
	m_des_map[UBLOCK_HIGH_PRIVATE_USE_SURROGATES] = _("High Private Use Surrogates");
	m_des_map[UBLOCK_LOW_SURROGATES] = _("Low Surrogates");
	m_des_map[UBLOCK_PRIVATE_USE_AREA] = _("Private Use Area");
	m_des_map[UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS] = _("CJK Compatibility Ideographs");
	m_des_map[UBLOCK_ALPHABETIC_PRESENTATION_FORMS] = _("Alphabetic Presentation Forms");
	m_des_map[UBLOCK_ARABIC_PRESENTATION_FORMS_A] = _("Arabic Presentation Forms-A");
	m_des_map[UBLOCK_COMBINING_HALF_MARKS] = _("Combining Half Marks");
	m_des_map[UBLOCK_CJK_COMPATIBILITY_FORMS] = _("CJK Compatibility Forms");
	m_des_map[UBLOCK_SMALL_FORM_VARIANTS] = _("Small Form Variants");
	m_des_map[UBLOCK_ARABIC_PRESENTATION_FORMS_B] = _("Arabic Presentation Forms-B");
	m_des_map[UBLOCK_SPECIALS] = _("Specials");
	m_des_map[UBLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS] = _("Halfwidth and Fullwidth Forms");
	m_des_map[UBLOCK_OLD_ITALIC] = _("Old Italic");
	m_des_map[UBLOCK_GOTHIC] = _("Gothic");
	m_des_map[UBLOCK_DESERET] = _("Deseret");
	m_des_map[UBLOCK_BYZANTINE_MUSICAL_SYMBOLS] = _("Byzantine Musical Symbols");
	m_des_map[UBLOCK_MUSICAL_SYMBOLS] = _("Musical Symbols");
	m_des_map[UBLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS] = _("Mathematical Alphanumeric Symbols");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B] = _("CJK Unified Ideographs Extension B");
	m_des_map[UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT] = _("CJK Compatibility Ideographs Supplement");
	m_des_map[UBLOCK_TAGS] = _("Tags");
	m_des_map[UBLOCK_CYRILLIC_SUPPLEMENT] = _("Cyrillic Supplement");
	m_des_map[UBLOCK_TAGALOG] = _("Tagalog");
	m_des_map[UBLOCK_HANUNOO] = _("Hanunoo");
	m_des_map[UBLOCK_BUHID] = _("Buhid");
	m_des_map[UBLOCK_TAGBANWA] = _("Tagbanwa");
	m_des_map[UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A] = _("Miscellaneous Mathematical Symbols-A");
	m_des_map[UBLOCK_SUPPLEMENTAL_ARROWS_A] = _("Supplemental Arrows-A");
	m_des_map[UBLOCK_SUPPLEMENTAL_ARROWS_B] = _("Supplemental Arrows-B");
	m_des_map[UBLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B] = _("Miscellaneous Mathematical Symbols-B");
	m_des_map[UBLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS] = _("Supplemental Mathematical Operators");
	m_des_map[UBLOCK_KATAKANA_PHONETIC_EXTENSIONS] = _("Katakana Phonetic Extensions");
	m_des_map[UBLOCK_VARIATION_SELECTORS] = _("Variation Selectors");
	m_des_map[UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A] = _("Supplementary Private Use Area-A");
	m_des_map[UBLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B] = _("Supplementary Private Use Area-B");
	m_des_map[UBLOCK_LIMBU] = _("Limbu");
	m_des_map[UBLOCK_TAI_LE] = _("Tai Le");
	m_des_map[UBLOCK_KHMER_SYMBOLS] = _("Khmer Symbols");
	m_des_map[UBLOCK_PHONETIC_EXTENSIONS] = _("Phonetic Extensions");
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS] = _("Miscellaneous Symbols and Arrows");
	m_des_map[UBLOCK_YIJING_HEXAGRAM_SYMBOLS] = _("Yijing Hexagram Symbols");
	m_des_map[UBLOCK_LINEAR_B_SYLLABARY] = _("Linear B Syllabary");
	m_des_map[UBLOCK_LINEAR_B_IDEOGRAMS] = _("Linear B Ideograms");
	m_des_map[UBLOCK_AEGEAN_NUMBERS] = _("Aegean Numbers");
	m_des_map[UBLOCK_UGARITIC] = _("Ugaritic");
	m_des_map[UBLOCK_SHAVIAN] = _("Shavian");
	m_des_map[UBLOCK_OSMANYA] = _("Osmanya");
	m_des_map[UBLOCK_CYPRIOT_SYLLABARY] = _("Cypriot Syllabary");
	m_des_map[UBLOCK_TAI_XUAN_JING_SYMBOLS] = _("Tai Xuan Jing Symbols");
	m_des_map[UBLOCK_VARIATION_SELECTORS_SUPPLEMENT] = _("Variation Selectors Supplement");
	m_des_map[UBLOCK_ANCIENT_GREEK_MUSICAL_NOTATION] = _("Ancient Greek Musical Notation");
	m_des_map[UBLOCK_ANCIENT_GREEK_NUMBERS] = _("Ancient Greek Numbers");
	m_des_map[UBLOCK_ARABIC_SUPPLEMENT] = _("Arabic Supplement");
	m_des_map[UBLOCK_BUGINESE] = _("Buginese");
	m_des_map[UBLOCK_CJK_STROKES] = _("CJK Strokes");
	m_des_map[UBLOCK_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT] = _("Combining Diacritical Marks Supplement");
	m_des_map[UBLOCK_COPTIC] = _("Coptic");
	m_des_map[UBLOCK_ETHIOPIC_EXTENDED] = _("Ethiopic Extended");
	m_des_map[UBLOCK_ETHIOPIC_SUPPLEMENT] = _("Ethiopic Supplement");
	m_des_map[UBLOCK_GEORGIAN_SUPPLEMENT] = _("Georgian Supplement");
	m_des_map[UBLOCK_GLAGOLITIC] = _("Glagolitic");
	m_des_map[UBLOCK_KHAROSHTHI] = _("Kharoshthi");
	m_des_map[UBLOCK_MODIFIER_TONE_LETTERS] = _("Modifier Tone Letters");
	m_des_map[UBLOCK_NEW_TAI_LUE] = _("New Tai Lue");
	m_des_map[UBLOCK_OLD_PERSIAN] = _("Old Persian");
	m_des_map[UBLOCK_PHONETIC_EXTENSIONS_SUPPLEMENT] = _("Phonetic Extensions Supplement");
	m_des_map[UBLOCK_SUPPLEMENTAL_PUNCTUATION] = _("Supplemental Punctuation");
	m_des_map[UBLOCK_SYLOTI_NAGRI] = _("Syloti Nagri");
	m_des_map[UBLOCK_TIFINAGH] = _("Tifinagh");
	m_des_map[UBLOCK_VERTICAL_FORMS] = _("Vertical Forms");
	m_des_map[UBLOCK_NKO] = _("NKo");
	m_des_map[UBLOCK_BALINESE] = _("Balinese");
	m_des_map[UBLOCK_LATIN_EXTENDED_C] = _("Latin Extended-C");
	m_des_map[UBLOCK_LATIN_EXTENDED_D] = _("Latin Extended-D");
	m_des_map[UBLOCK_PHAGS_PA] = _("Phags-pa");
	m_des_map[UBLOCK_PHOENICIAN] = _("Phoenician");
	m_des_map[UBLOCK_CUNEIFORM] = _("Cuneiform");
	m_des_map[UBLOCK_CUNEIFORM_NUMBERS_AND_PUNCTUATION] = _("Cuneiform Numbers and Punctuation");
	m_des_map[UBLOCK_COUNTING_ROD_NUMERALS] = _("Counting Rod Numerals");
	m_des_map[UBLOCK_SUNDANESE] = _("Sundanese");
	m_des_map[UBLOCK_LEPCHA] = _("Lepcha");
	m_des_map[UBLOCK_OL_CHIKI] = _("Ol Chiki");
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_A] = _("Cyrillic Extended-A");
	m_des_map[UBLOCK_VAI] = _("Vai");
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_B] = _("Cyrillic Extended-B");
	m_des_map[UBLOCK_SAURASHTRA] = _("Saurashtra");
	m_des_map[UBLOCK_KAYAH_LI] = _("Kayah Li");
	m_des_map[UBLOCK_REJANG] = _("Rejang");
	m_des_map[UBLOCK_CHAM] = _("Cham");
	m_des_map[UBLOCK_ANCIENT_SYMBOLS] = _("Ancient Symbols");
	m_des_map[UBLOCK_PHAISTOS_DISC] = _("Phaistos Disc");
	m_des_map[UBLOCK_LYCIAN] = _("Lycian");
	m_des_map[UBLOCK_CARIAN] = _("Carian");
	m_des_map[UBLOCK_LYDIAN] = _("Lydian");
	m_des_map[UBLOCK_MAHJONG_TILES] = _("Mahjong Tiles");
	m_des_map[UBLOCK_DOMINO_TILES] = _("Domino Tiles");
	m_des_map[UBLOCK_SAMARITAN] = _("Samaritan");
	m_des_map[UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED] = _("Unified Canadian Aboriginal Syllabics Extended");
	m_des_map[UBLOCK_TAI_THAM] = _("Tai Tham");
	m_des_map[UBLOCK_VEDIC_EXTENSIONS] = _("Vedic Extensions");
	m_des_map[UBLOCK_LISU] = _("Lisu");
	m_des_map[UBLOCK_BAMUM] = _("Bamum");
	m_des_map[UBLOCK_COMMON_INDIC_NUMBER_FORMS] = _("Common Indic Number Forms");
	m_des_map[UBLOCK_DEVANAGARI_EXTENDED] = _("Devanagari Extended");
	m_des_map[UBLOCK_HANGUL_JAMO_EXTENDED_A] = _("Hangul Jamo Extended-A");
	m_des_map[UBLOCK_JAVANESE] = _("Javanese");
	m_des_map[UBLOCK_MYANMAR_EXTENDED_A] = _("Myanmar Extended-A");
	m_des_map[UBLOCK_TAI_VIET] = _("Tai Viet");
	m_des_map[UBLOCK_MEETEI_MAYEK] = _("Meetei Mayek");
	m_des_map[UBLOCK_HANGUL_JAMO_EXTENDED_B] = _("Hangul Jamo Extended-B");
	m_des_map[UBLOCK_IMPERIAL_ARAMAIC] = _("Imperial Aramaic");
	m_des_map[UBLOCK_OLD_SOUTH_ARABIAN] = _("Old South Arabian");
	m_des_map[UBLOCK_AVESTAN] = _("Avestan");
	m_des_map[UBLOCK_INSCRIPTIONAL_PARTHIAN] = _("Inscriptional Parthian");
	m_des_map[UBLOCK_INSCRIPTIONAL_PAHLAVI] = _("Inscriptional Pahlavi");
	m_des_map[UBLOCK_OLD_TURKIC] = _("Old Turkic");
	m_des_map[UBLOCK_RUMI_NUMERAL_SYMBOLS] = _("Rumi Numeral Symbols");
	m_des_map[UBLOCK_KAITHI] = _("Kaithi");
	m_des_map[UBLOCK_EGYPTIAN_HIEROGLYPHS] = _("Egyptian Hieroglyphs");
	m_des_map[UBLOCK_ENCLOSED_ALPHANUMERIC_SUPPLEMENT] = _("Enclosed Alphanumeric Supplement");
	m_des_map[UBLOCK_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT] = _("Enclosed Ideographic Supplement");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C] = _("CJK Unified Ideographs Extension C");
	m_des_map[UBLOCK_MANDAIC] = _("Mandaic");
	m_des_map[UBLOCK_BATAK] = _("Batak");
	m_des_map[UBLOCK_ETHIOPIC_EXTENDED_A] = _("Ethiopic Extended-A");
	m_des_map[UBLOCK_BRAHMI] = _("Brahmi");
	m_des_map[UBLOCK_BAMUM_SUPPLEMENT] = _("Bamum Supplement");
	m_des_map[UBLOCK_KANA_SUPPLEMENT] = _("Kana Supplement");
	m_des_map[UBLOCK_PLAYING_CARDS] = _("Playing Cards");
	m_des_map[UBLOCK_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS] = _("Miscellaneous Symbols And Pictographs");
	m_des_map[UBLOCK_EMOTICONS] = _("Emoticons");
	m_des_map[UBLOCK_TRANSPORT_AND_MAP_SYMBOLS] = _("Transport And Map Symbols");
	m_des_map[UBLOCK_ALCHEMICAL_SYMBOLS] = _("Alchemical Symbols");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D] = _("CJK Unified Ideographs Extension D");
	m_des_map[UBLOCK_ARABIC_EXTENDED_A] = _("Arabic Extended-A");
	m_des_map[UBLOCK_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS] = _("Arabic Mathematical Alphabetic Symbols");
	m_des_map[UBLOCK_CHAKMA] = _("Chakma");
	m_des_map[UBLOCK_MEETEI_MAYEK_EXTENSIONS] = _("Meetei Mayek Extensions");
	m_des_map[UBLOCK_MEROITIC_CURSIVE] = _("Meroitic Cursive");
	m_des_map[UBLOCK_MEROITIC_HIEROGLYPHS] = _("Meroitic Hieroglyphs");
	m_des_map[UBLOCK_MIAO] = _("Miao");
	m_des_map[UBLOCK_SHARADA] = _("Sharada");
	m_des_map[UBLOCK_SORA_SOMPENG] = _("Sora Sompeng");
	m_des_map[UBLOCK_SUNDANESE_SUPPLEMENT] = _("Sundanese Supplement");
	m_des_map[UBLOCK_TAKRI] = _("Takri");
#if (U_ICU_VERSION_MAJOR_NUM >= 54) // ICU 54, Unicode 7.0
	m_des_map[UBLOCK_BASSA_VAH] = _("Bassa Vah");
	m_des_map[UBLOCK_CAUCASIAN_ALBANIAN] = _("Caucasian Albanian");
	m_des_map[UBLOCK_COPTIC_EPACT_NUMBERS] = _("Coptic Epact Numbers");
	m_des_map[UBLOCK_COMBINING_DIACRITICAL_MARKS_EXTENDED] = _("Combining Diacritical Marks Extended");
	m_des_map[UBLOCK_DUPLOYAN] = _("Duployan");
	m_des_map[UBLOCK_ELBASAN] = _("Elbasan");
	m_des_map[UBLOCK_GEOMETRIC_SHAPES_EXTENDED] = _("Geometric Shapes Extended");
	m_des_map[UBLOCK_GRANTHA] = _("Grantha");
	m_des_map[UBLOCK_KHOJKI] = _("Khojki");
	m_des_map[UBLOCK_KHUDAWADI] = _("Khudawadi");
	m_des_map[UBLOCK_LATIN_EXTENDED_E] = _("Latin Extended-E");
	m_des_map[UBLOCK_LINEAR_A] = _("Linear A");
	m_des_map[UBLOCK_MAHAJANI] = _("Mahajani");
	m_des_map[UBLOCK_MANICHAEAN] = _("Manichaean");
	m_des_map[UBLOCK_MENDE_KIKAKUI] = _("Mende Kikakui");
	m_des_map[UBLOCK_MODI] = _("Modi");
	m_des_map[UBLOCK_MRO] = _("Mro");
	m_des_map[UBLOCK_MYANMAR_EXTENDED_B] = _("Myanmar Extended-B");
	m_des_map[UBLOCK_NABATAEAN] = _("Nabataean");
	m_des_map[UBLOCK_OLD_NORTH_ARABIAN] = _("Old North Arabian");
	m_des_map[UBLOCK_OLD_PERMIC] = _("Old Permic");
	m_des_map[UBLOCK_ORNAMENTAL_DINGBATS] = _("Ornamental Dingbats");
	m_des_map[UBLOCK_PAHAWH_HMONG] = _("Pahawh Hmong");
	m_des_map[UBLOCK_PALMYRENE] = _("Palmyrene");
	m_des_map[UBLOCK_PAU_CIN_HAU] = _("Pau Cin Hau");
	m_des_map[UBLOCK_PSALTER_PAHLAVI] = _("Psalter Pahlavi");
	m_des_map[UBLOCK_SHORTHAND_FORMAT_CONTROLS] = _("Shorthand Format Controls");
	m_des_map[UBLOCK_SIDDHAM] = _("Siddham");
	m_des_map[UBLOCK_SINHALA_ARCHAIC_NUMBERS] = _("Sinhala Archaic Numbers");
	m_des_map[UBLOCK_SUPPLEMENTAL_ARROWS_C] = _("Supplemental Arrows-C");
	m_des_map[UBLOCK_TIRHUTA] = _("Tirhuta");
	m_des_map[UBLOCK_WARANG_CITI] = _("Warang Citi");
# if (U_ICU_VERSION_MAJOR_NUM >= 56) // ICU 56, Unicode 8.0
	m_des_map[UBLOCK_AHOM] = _("Ahom");
	m_des_map[UBLOCK_ANATOLIAN_HIEROGLYPHS] = _("Anatolian Hieroglyphs");
	m_des_map[UBLOCK_CHEROKEE_SUPPLEMENT] = _("Cherokee Supplement");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E] = _("CJK Extension E");
	m_des_map[UBLOCK_EARLY_DYNASTIC_CUNEIFORM] = _("Early Dynastic Cuneiform");
	m_des_map[UBLOCK_HATRAN] = _("Hatran");
	m_des_map[UBLOCK_MULTANI] = _("Multani");
	m_des_map[UBLOCK_OLD_HUNGARIAN] = _("Old Hungarian");
	m_des_map[UBLOCK_SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS] = _("Supplemental Symbols and Pictographs");
	m_des_map[UBLOCK_SUTTON_SIGNWRITING] = _("Sutton SignWriting");
#  if (U_ICU_VERSION_MAJOR_NUM >= 58) // ICU 58, Unicode 9.0
	m_des_map[UBLOCK_ADLAM] = _("Adlam");
	m_des_map[UBLOCK_BHAIKSUKI] = _("Bhaiksuki");
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_C] = _("Cyrillic Extended-C");
	m_des_map[UBLOCK_GLAGOLITIC_SUPPLEMENT] = _("	Glagolitic Supplement");
	m_des_map[UBLOCK_IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION] = _("Ideographic Symbols and Punctuation");
	m_des_map[UBLOCK_MARCHEN] = _("Marchen");
	m_des_map[UBLOCK_MONGOLIAN_SUPPLEMENT] = _("Mongolian Supplement");
	m_des_map[UBLOCK_NEWA] = _("Newa");
	m_des_map[UBLOCK_OSAGE] = _("Osage");
	m_des_map[UBLOCK_TANGUT] = _("Tangut");
	m_des_map[UBLOCK_TANGUT_COMPONENTS] = _("Tangut Components");
#   if (U_ICU_VERSION_MAJOR_NUM >= 60) // ICU 60, Unicode 10.0
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F] = _("CJK Unified Ideographs Extension F");
	m_des_map[UBLOCK_KANA_EXTENDED_A] = _("Kana Extended-A");
	m_des_map[UBLOCK_MASARAM_GONDI] = _("Masaram Gondi");
	m_des_map[UBLOCK_NUSHU] = _("Nushu");
	m_des_map[UBLOCK_SOYOMBO] = _("Soyombo");
	m_des_map[UBLOCK_SYRIAC_SUPPLEMENT] = _("Syriac Supplement");
	m_des_map[UBLOCK_ZANABAZAR_SQUARE] = _("Zanabazar Square");
#    if (U_ICU_VERSION_MAJOR_NUM >= 62) // ICU 62, Unicode 11.0
	m_des_map[UBLOCK_CHESS_SYMBOLS] = _("Chess Symbols");
	m_des_map[UBLOCK_DOGRA] = _("Dogra");
	m_des_map[UBLOCK_GEORGIAN_EXTENDED] = _("Georgian Extended");
	m_des_map[UBLOCK_GUNJALA_GONDI] = _("Gunjala Gondi");
	m_des_map[UBLOCK_HANIFI_ROHINGYA] = _("Hanifi Rohingya");
	m_des_map[UBLOCK_INDIC_SIYAQ_NUMBERS] = _("Indic Siyaq Numbers");
	m_des_map[UBLOCK_MAKASAR] = _("Makasar");
	m_des_map[UBLOCK_MAYAN_NUMERALS] = _("Mayan Numerals");
	m_des_map[UBLOCK_MEDEFAIDRIN] = _("Medefaidrin");
	m_des_map[UBLOCK_OLD_SOGDIAN] = _("Old Sogdian");
	m_des_map[UBLOCK_SOGDIAN] = _("Sogdian");
#     if (U_ICU_VERSION_MAJOR_NUM >= 64) // ICU 64, Unicode 12.0
	m_des_map[UBLOCK_EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS] = _("Egyptian Hieroglyph Format Controls");
	m_des_map[UBLOCK_ELYMAIC] = _("Elymaic");
	m_des_map[UBLOCK_NANDINAGARI] = _("Nandinagari");
	m_des_map[UBLOCK_NYIAKENG_PUACHUE_HMONG] = _("Nyiakeng Puachue Hmong");
	m_des_map[UBLOCK_OTTOMAN_SIYAQ_NUMBERS] = _("Ottoman Siyaq Numbers");
	m_des_map[UBLOCK_SMALL_KANA_EXTENSION] = _("Small Kana Extension");
	m_des_map[UBLOCK_SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A] = _("Symbols and Pictographs Extended-A");
	m_des_map[UBLOCK_TAMIL_SUPPLEMENT] = _("Tamil Supplement");
	m_des_map[UBLOCK_WANCHO] = _("Wancho");
#      if (U_ICU_VERSION_MAJOR_NUM >= 66) // ICU 66, Unicode 13.0
	m_des_map[UBLOCK_CHORASMIAN] = _("Chorasmian");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G] = _("CJK Unified Ideographs Extension G");
	m_des_map[UBLOCK_DIVES_AKURU] = _("Dives Akuru");
	m_des_map[UBLOCK_KHITAN_SMALL_SCRIPT] = _("Khitan Small Script");
	m_des_map[UBLOCK_LISU_SUPPLEMENT] = _("Lisu Supplement");
	m_des_map[UBLOCK_SYMBOLS_FOR_LEGACY_COMPUTING] = _("Symbols for Legacy Computing");
	m_des_map[UBLOCK_TANGUT_SUPPLEMENT] = _("Tangut Supplement");
	m_des_map[UBLOCK_YEZIDI] = _("Yezidi");
#       if (U_ICU_VERSION_MAJOR_NUM >= 70) // ICU 70, Unicode 14.0
	m_des_map[UBLOCK_ARABIC_EXTENDED_B] = _("Arabic Extended-B");
	m_des_map[UBLOCK_CYPRO_MINOAN] = _("Cypro-Minoan");
	m_des_map[UBLOCK_ETHIOPIC_EXTENDED_B] = _("Ethiopic Extended-B");
	m_des_map[UBLOCK_KANA_EXTENDED_B] = _("Kana Extended-B");
	m_des_map[UBLOCK_LATIN_EXTENDED_F] = _("Latin Extended-F");
	m_des_map[UBLOCK_LATIN_EXTENDED_G] = _("Latin Extended-G");
	m_des_map[UBLOCK_OLD_UYGHUR] = _("Old Uyghur");
	m_des_map[UBLOCK_TANGSA] = _("Tangsa");
	m_des_map[UBLOCK_TOTO] = _("Toto");
	m_des_map[UBLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED_A] = _("Unified Canadian Aboriginal Syllabics Extended-A");
	m_des_map[UBLOCK_VITHKUQI] = _("Vithkuqi");
	m_des_map[UBLOCK_ZNAMENNY_MUSICAL_NOTATION] = _("Znamenny Musical Notation");
#        if (U_ICU_VERSION_MAJOR_NUM >= 72) // ICU 72, Unicode 15.0
	m_des_map[UBLOCK_ARABIC_EXTENDED_C] = _("Arabic Extended-C");
	m_des_map[UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H] = _("CJK Unified Ideographs Extension H");
	m_des_map[UBLOCK_CYRILLIC_EXTENDED_D] = _("Cyrillic Extended-D");
	m_des_map[UBLOCK_DEVANAGARI_EXTENDED_A] = _("Devanagari Extended-A");
	m_des_map[UBLOCK_KAKTOVIK_NUMERALS] = _("Kaktovik Numerals");
	m_des_map[UBLOCK_KAWI] = _("Kawi");
	m_des_map[UBLOCK_NAG_MUNDARI] = _("Nag Mundari");
#        endif // 72
#       endif // 70
#      endif // 66
#     endif // 64
#    endif // 62
#   endif // 60
#  endif // 58
# endif // 56
#endif // 54
	m_des_map[UBLOCK_INVALID_CODE] = _("Invalid Unicode Characters");
}

} // namespace xm
