///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadUndo.h
// Description: Undo/Redo Buffer
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADUNDO_H_
#define _MADUNDO_H_

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

#include "MadLines.h"


enum MadUndoDataType { udtInsert, udtDelete, udtOverwrite};

struct MadUndoData
{
    MadUndoDataType  m_Type;    // undo data type
    wxFileOffset     m_Pos;
};

struct MadInsertUndoData:MadUndoData
{
    wxFileOffset m_Size;
    MadBlockVector m_Data;

    MadInsertUndoData()
    {
        m_Type = udtInsert;
    }
};

struct MadDeleteUndoData:MadUndoData
{
    wxFileOffset m_Size;
    MadBlockVector m_Data;

    MadDeleteUndoData()
    {
        m_Type = udtDelete;
    }
};

struct MadOverwriteUndoData:MadUndoData
{
    wxFileOffset m_DelSize, m_InsSize;
    MadBlockVector m_DelData, m_InsData;

    MadOverwriteUndoData()
    {
        m_Type = udtOverwrite;
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

    ~MadUndo()
    {
        if(m_Undos.size())
        {
            MadUndoDataIterator it = m_Undos.begin();
            do
            {
                switch((*it)->m_Type)
                {
                case udtInsert:
                    ((MadInsertUndoData*)(*it))->m_Data.clear();
                    break;
                case udtDelete:
                    ((MadDeleteUndoData*)(*it))->m_Data.clear();
                    break;
                case udtOverwrite:
                    ((MadOverwriteUndoData*)(*it))->m_DelData.clear();
                    ((MadOverwriteUndoData*)(*it))->m_InsData.clear();
                    break;
                }

                delete *it;
            }
            while(++it != m_Undos.end());

            m_Undos.clear();
        }
    }
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

