///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock.h
// Description: Unicode Blocks Information
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UBLOCK_H_
#define _XM_UBLOCK_H_

#include <unicode/uchar.h>
#include <boost/utility.hpp>
#include <map>

namespace xm
{

struct UnicodeBlockSet: private boost::noncopyable
{
	int FindBlockIndex(UChar32 ch);

	UChar32 Begin(int index);
	UChar32 End(int index);
	bool IsAlphabet(int index);
	bool IsFullWidth(int index);
	const wchar_t* Description(int index);

	bool Valid(int index);

	static UnicodeBlockSet& GetInstance();
private:
	UnicodeBlockSet();

	typedef std::pair<UChar32, UChar32> UBlockRange;
	typedef std::map<int, UBlockRange>  UBlockMap;

	struct object_creator
	{
        object_creator(){ UnicodeBlockSet::GetInstance(); }
        inline void do_nothing()const {}
	};

	static object_creator create_object;

	UBlockMap m_block_map;
};

struct UnicodeBlockCharCounter
{
	void Count(int index);

	int BlockIndexBegin();
	int NextBlock();
	bool IsValidBlock(int index);
	int GetBlockCharCount(int index);

	int GetInvalidBlockCharCount();

	UnicodeBlockCharCounter();
private:
	std::map<int, int> m_counts_map;
	std::map<int, int>::const_iterator m_it;
	UnicodeBlockSet& m_ublock_set;
};

}; // namespace xm
#endif //_XM_UBLOCK_H_
