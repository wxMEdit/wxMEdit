///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/u_block.h
// Description: Unicode Blocks Information
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _U_BLOCK_H_
#define _U_BLOCK_H_

#include "ucs4_t.h"
#include <vector>

struct UnicodeBlockSet
{
	int FindBlockIndex(ucs4_t uc);

	int GetUnicodeBlocksCount();

	ucs4_t Begin(int index);
	ucs4_t End(int index);
	bool IsAlphabet(int index);
	bool IsFullWidth(int index);
	const wxChar* Description(int index);

	bool Valid(int index);
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
	std::vector<int> m_counts;
	int m_index;
	UnicodeBlockSet m_ublock_set;
};

#endif //_U_BLOCK_H_
