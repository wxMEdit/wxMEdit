///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock.cpp
// Description: Unicode Blocks Information
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "ublock.h"
#include "cxx11.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif

namespace xm
{

UnicodeBlockSet::UnicodeBlockSet()
{
	int last_block = UBLOCK_INVALID_CODE;
	for(UChar32 u=0; u<=0x110000; ++u)
	{
		int block = ublock_getCode(u);
		if (block != last_block)
		{
			// last block end
			m_block_map[last_block].second = u-1;

			// current block begin
			m_block_map[block].first = u;
		}
		last_block = block;
	}

	m_block_map.erase(UBLOCK_INVALID_CODE);
	m_block_map.erase(UBLOCK_NO_BLOCK);

	InitBlockDescriptions();
}

UnicodeBlockSet& UnicodeBlockSet::GetInstance()
{
	static UnicodeBlockSet obj;
	create_object.do_nothing();
	return obj;
}
UnicodeBlockSet::object_creator UnicodeBlockSet::create_object;

int UnicodeBlockSet::FindBlockIndex(UChar32 ch)
{
	int block = ublock_getCode(ch);
	if (block == UBLOCK_INVALID_CODE)
		block = UBLOCK_NO_BLOCK;

	return block;
}

UChar32 UnicodeBlockSet::Begin(int index)
{
	if (m_block_map.find(index) == m_block_map.end())
		return 0x10FFFF;

	return m_block_map[index].first;
}

UChar32 UnicodeBlockSet::End(int index)
{
	if (m_block_map.find(index) == m_block_map.end())
		return 0x10FFFF;

	return m_block_map[index].second;
}

const wchar_t* UnicodeBlockSet::Description(int index)
{
	std::map<int, std::wstring>::const_iterator it = m_des_map.find(index);
	if (it == m_des_map.end())
		return nullptr;

	return it->second.c_str();
}

bool UnicodeBlockSet::Valid(int index)
{
	return index < UBLOCK_COUNT && index > UBLOCK_NO_BLOCK;
}



UnicodeBlockCharCounter::UnicodeBlockCharCounter():
	m_ublock_set(UnicodeBlockSet::GetInstance())
{
}


void UnicodeBlockCharCounter::Count(int index)
{
	++m_counts_map[index];
}

void UnicodeBlockCharCounter::FillBlockIndexSet()
{
	m_blockidx_set.clear();
	typedef std::map<int, int>::const_iterator CntMapCIter;
	for(CntMapCIter it=m_counts_map.begin(); it!=m_counts_map.end(); ++it)
	{
		BlockIndex bi;
		bi.index = it->first;
		bi.ubegin = m_ublock_set.Begin(bi.index);
		m_blockidx_set.insert(bi);
	}
}

int UnicodeBlockCharCounter::BlockIndexBegin()
{
	FillBlockIndexSet();

	m_it = m_blockidx_set.begin();
	if (m_it == m_blockidx_set.end())
		return UBLOCK_NO_BLOCK;

	return m_it->index;
}

int UnicodeBlockCharCounter::NextBlock()
{
	++m_it;
	if (m_it == m_blockidx_set.end())
		return UBLOCK_NO_BLOCK;

	return m_it->index;
}

bool UnicodeBlockCharCounter::IsValidBlock(int index)
{
	if (m_counts_map.find(index)==m_counts_map.end())
		return false;

	return m_ublock_set.Valid(index);
}

int UnicodeBlockCharCounter::GetBlockCharCount(int index)
{
	if (m_counts_map.find(index)==m_counts_map.end())
		return 0;

	return m_counts_map[index];
}

int UnicodeBlockCharCounter::GetInvalidBlockCharCount()
{
	return m_counts_map[UBLOCK_NO_BLOCK];
}

}; // namespace xm
