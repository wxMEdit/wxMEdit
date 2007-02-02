///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadUndo.cpp
// Description: Undo/Redo Buffer
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadUndo.h"


#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif


MadUndo::~MadUndo()
{
    if(m_Undos.size())
    {
        MadUndoDataIterator it = m_Undos.begin();
        do
        {
            MadUndoData *pud = *it;
            switch(pud->m_Type)
            {
            case udtInsert:
                {
                    MadInsertUndoData *pd = (MadInsertUndoData*)pud;
                    //pd->m_Data.clear();
                    delete pd;
                }
                break;
            case udtDelete:
                {
                    MadDeleteUndoData *pd = (MadDeleteUndoData*)pud;
                    //pd->m_Data.clear();
                    delete pd;
                }
                break;
            case udtOverwrite:
                {
                    MadOverwriteUndoData *pd = (MadOverwriteUndoData*)pud;
                    //pd->m_DelData.clear();
                    //pd->m_InsData.clear();
                    delete pd;
                }
                break;
            }
        }
        while(++it != m_Undos.end());

        //m_Undos.clear();
    }
}


MadUndoBuffer::MadUndoBuffer()
{
    m_CurrentUndo = m_UndoList.begin();
}

MadUndoBuffer::~MadUndoBuffer()
{
    Clear();
}

void MadUndoBuffer::Clear()
{
    m_UndoList.clear();
    m_CurrentUndo = m_UndoList.begin();
}

void MadUndoBuffer::ClearTillEnd()
{
    if(m_CurrentUndo != m_UndoList.end())
    {
        m_CurrentUndo = m_UndoList.erase(m_CurrentUndo, m_UndoList.end());
    }
}


MadUndo *MadUndoBuffer::Add()
{
    ClearTillEnd();

    static MadUndo undo;
    m_UndoList.push_back(undo);
    return &m_UndoList.back();
}

void MadUndoBuffer::Add(wxFileOffset caretPosBefore, wxFileOffset caretPosAfter)
{
#if 0
    if(m_UndoList.size() && m_CurrentUndo!=m_UndoList.end())
    {
        //caretPosBefore=caretPosAfter;???
    }
#endif

    m_UndoList.insert(m_CurrentUndo, MadUndo(caretPosBefore, caretPosAfter) );
}

MadUndo *MadUndoBuffer::GetPrevUndo()
{
    if(m_CurrentUndo==m_UndoList.begin())
        return NULL;

    MadUndoIterator it = m_CurrentUndo;
    --it; // to previous

    while(it!=m_UndoList.begin() && it->m_Undos.size()==0)
        --it;

    if(it==m_UndoList.begin() && it->m_Undos.size()==0)
        return NULL;

    return &(*it);
}

MadUndo *MadUndoBuffer::Undo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.begin())
        return NULL;

    if(noCaretMovement)
    {
        --m_CurrentUndo;

        while(m_CurrentUndo != m_UndoList.begin() && m_CurrentUndo->m_Undos.size()==0)
            --m_CurrentUndo;

        if(m_CurrentUndo==m_UndoList.begin() && m_CurrentUndo->m_Undos.size()==0)
            return NULL;

        return &(*m_CurrentUndo);
    }

    return &(*--m_CurrentUndo);
}

MadUndo *MadUndoBuffer::Redo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.end())
        return NULL;

    if(noCaretMovement)
    {
        while(m_CurrentUndo != m_UndoList.end() && m_CurrentUndo->m_Undos.size()==0)
            ++m_CurrentUndo;

        if(m_CurrentUndo==m_UndoList.end())
            return NULL;
    }

    return &(*m_CurrentUndo++);
}

bool MadUndoBuffer::CanUndo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.begin())
        return false;

    if(noCaretMovement)
    {
        MadUndoIterator it=m_CurrentUndo;
        --it;

        while(it!=m_UndoList.begin() && it->m_Undos.size()==0)
            --it;

        if(it==m_UndoList.begin() && it->m_Undos.size()==0)
            return false;
    }

    return true;
}

bool MadUndoBuffer::CanRedo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.end())
        return false;

    if(noCaretMovement)
    {
        MadUndoIterator it=m_CurrentUndo;

        while(it != m_UndoList.end() && it->m_Undos.size()==0)
            ++it;

        if(it==m_UndoList.end())
            return false;
    }

    return true;
}
