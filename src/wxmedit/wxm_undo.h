///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_undo.h
// Description: Undo/Redo Buffer
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_UNDO_H_
#define _WXM_UNDO_H_

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
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
    virtual MadBlockVector* DelData() = 0;
    virtual MadBlockVector* InsData() = 0;
    virtual ~MadUndoData(){}
};

struct MadInsertUndoData:MadUndoData
{
    wxFileOffset m_Size;
    MadBlockVector m_Data;

    virtual wxFileOffset DelSize() {return 0;}
    virtual wxFileOffset InsSize() {return m_Size;}
    virtual MadBlockVector* DelData() {return NULL;}
    virtual MadBlockVector* InsData() {return &m_Data;}
};

struct MadDeleteUndoData:MadUndoData
{
    wxFileOffset m_Size;
    MadBlockVector m_Data;

    virtual wxFileOffset DelSize() {return m_Size;}
    virtual wxFileOffset InsSize() {return 0;}
    virtual MadBlockVector* DelData() {return &m_Data;}
    virtual MadBlockVector* InsData() {return NULL;}
};

struct MadOverwriteUndoData:MadUndoData
{
    wxFileOffset m_DelSize, m_InsSize;
    MadBlockVector m_DelData, m_InsData;

    virtual wxFileOffset DelSize() {return m_DelSize;}
    virtual wxFileOffset InsSize() {return m_InsSize;}
    virtual MadBlockVector* DelData() {return &m_DelData;}
    virtual MadBlockVector* InsData() {return &m_InsData;}
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

