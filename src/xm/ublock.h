///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock.h
// Description: Unicode Blocks Information
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _XM_UBLOCK_H_
#define _XM_UBLOCK_H_

#include <unicode/uchar.h>
#include <boost/noncopyable.hpp>
#include <map>
#include <set>
#include <string>

namespace xm
{

struct UnicodeBlockSet: private boost::noncopyable
{
	int FindBlockIndex(UChar32 ch);

	UChar32 Begin(int index);
	UChar32 End(int index);
	const wchar_t* Description(int index);

	bool Valid(int index);

	static UnicodeBlockSet& GetInstance();
private:
	UnicodeBlockSet();
	void InitBlockDescriptions();

	typedef std::pair<UChar32, UChar32> UBlockRange;
	typedef std::map<int, UBlockRange>  UBlockMap;

	struct object_creator
	{
        object_creator(){ UnicodeBlockSet::GetInstance(); }
        inline void do_nothing()const {}
	};

	static object_creator create_object;

	UBlockMap m_block_map;
	std::map<int, std::wstring> m_des_map;
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
	void FillBlockIndexSet();

	std::map<int, int> m_counts_map;
	UnicodeBlockSet& m_ublock_set;

	struct BlockIndex
	{
		int index;
		UChar32 ubegin;

		BlockIndex(): index(0), ubegin(0) {}
		bool operator<(const BlockIndex& bi2) const
		{
			return (ubegin<bi2.ubegin || (ubegin==bi2.ubegin && index<bi2.index));
		}
	};
	typedef std::set<BlockIndex> BlockIndexSet;

	BlockIndexSet m_blockidx_set;
	BlockIndexSet::const_iterator m_it;
};

}; // namespace xm
#endif //_XM_UBLOCK_H_
