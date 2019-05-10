///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_undo.h
// Description: Undo/Redo Buffer
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UNDO_H_
#define _WXM_UNDO_H_

#include "../xm/cxx11.h"

#ifdef _MSC_VER
# pragma warning( push )
# pragma warning( disable : 4996 )
#endif
// disable 4996 {
#include <wx/wxprec.h>

#ifdef __BORLANDC__
# pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif
// disable 4996 }
#ifdef _MSC_VER
# pragma warning( pop )
#endif

#include <vector>
#include <list>
using std::vector;
using std::list;

#include "wxm_lines.h"

struct MadUndoData
{
    wxFileOffset     m_Pos;

    virtual wxFileOffset DelSize() = 0;
    virtual wxFileOffset InsSize() = 0;
    virtual xm::BlockVector* DelData() = 0;
    virtual xm::BlockVector* InsData() = 0;
    virtual void SetInsBlock(const MadBlock& blk) = 0;
    virtual void SetDelSize(wxFileOffset size) = 0;
    virtual ~MadUndoData(){}
};

struct MadInsertUndoData:MadUndoData
{
    wxFileOffset m_Size;
    xm::BlockVector m_Data;

    virtual wxFileOffset DelSize() override { return 0; }
    virtual wxFileOffset InsSize() override { return m_Size; }
    virtual xm::BlockVector* DelData() override { return nullptr; }
    virtual xm::BlockVector* InsData() override { return &m_Data; }
    virtual void SetDelSize(wxFileOffset size) override { wxASSERT(size == 0); }
    virtual void SetInsBlock(const MadBlock& blk) override
    {
        m_Size = blk.m_Size;
        m_Data.push_back(blk);
    }
};

struct MadDeleteUndoData:MadUndoData
{
    wxFileOffset m_Size;
    xm::BlockVector m_Data;

    virtual wxFileOffset DelSize() override { return m_Size; }
    virtual wxFileOffset InsSize() override { return 0; }
    virtual xm::BlockVector* DelData() override { return &m_Data; }
    virtual xm::BlockVector* InsData() override { return nullptr; }
    virtual void SetDelSize(wxFileOffset size) override { m_Size = size; }
    virtual void SetInsBlock(const MadBlock& blk) override { wxASSERT(blk.m_Size == 0); }
};

struct MadOverwriteUndoData:MadUndoData
{
    wxFileOffset m_DelSize, m_InsSize;
    xm::BlockVector m_DelData, m_InsData;

    virtual wxFileOffset DelSize() override { return m_DelSize; }
    virtual wxFileOffset InsSize() override { return m_InsSize; }
    virtual xm::BlockVector* DelData() override { return &m_DelData; }
    virtual xm::BlockVector* InsData() override { return &m_InsData; }
    virtual void SetDelSize(wxFileOffset size) override { m_DelSize = size; }
    virtual void SetInsBlock(const MadBlock& blk) override
    {
        m_InsSize = blk.m_Size;
        m_InsData.push_back(blk);
    }
};


typedef vector < MadUndoData * >::iterator MadUndoDataIterator;
typedef vector < MadUndoData * >::reverse_iterator MadUndoDataRvsIterator;


struct MadUndo
{
    vector < MadUndoData* > m_Undos;
    wxFileOffset m_CaretPosBefore, m_CaretPosAfter;

    MadUndo() {}
    MadUndo(wxFileOffset caretPosBefore, wxFileOffset caretPosAfter)
        :m_CaretPosBefore(caretPosBefore), m_CaretPosAfter(caretPosAfter)
    {}

    ~MadUndo();
};

typedef list < MadUndo > MadUndoList;
typedef list < MadUndo >::iterator MadUndoIterator;

class MadUndoBuffer
{
private:
    friend class MadLines;

    MadUndoList m_UndoList;
    MadUndoIterator m_CurrentUndo;   // iterator of list is constant, even if add/del new element

    void ClearTillEnd();       // clear current undo till end

public:
    MadUndoBuffer();
    ~MadUndoBuffer();

    void Clear();      // clear all

    MadUndo *Add();
    void Add(wxFileOffset caretPosBefore, wxFileOffset caretPosAfter);

    MadUndo *GetPrevUndo();      // just for savepoint

    MadUndo *Undo(bool noCaretMovement);
    MadUndo *Redo(bool noCaretMovement);

    bool CanUndo(bool noCaretMovement);
    bool CanRedo(bool noCaretMovement);

};

#endif

