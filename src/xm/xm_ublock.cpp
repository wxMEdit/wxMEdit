///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/xm_ublock.cpp
// Description: Unicode Blocks Information
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "xm_ublock.h"

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

bool UnicodeBlockSet::IsFullWidth(int index)
{
	return false;	// FIXME
}

const wchar_t* UnicodeBlockSet::Description(int index)
{
	return NULL;	// FIXME
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

int UnicodeBlockCharCounter::BlockIndexBegin()
{
	m_it = m_counts_map.begin();
	if (m_it == m_counts_map.end())
		return UBLOCK_NO_BLOCK;

	return m_it->first;
}

int UnicodeBlockCharCounter::NextBlock()
{
	++m_it;
	if (m_it == m_counts_map.end())
		return UBLOCK_NO_BLOCK;

	return m_it->first;
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
