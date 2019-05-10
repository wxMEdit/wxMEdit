///////////////////////////////////////////////////////////////////////////////
// Name:        wxmedit/wxm_undo.cpp
// Description: Undo/Redo Buffer
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2005-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#include "wxm_undo.h"
#include  "../xm/cxx11.h"

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
            MadUndoData* pud = *it;
            delete pud;
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
        return nullptr;

    MadUndoIterator it = m_CurrentUndo;
    --it; // to previous

    while(it!=m_UndoList.begin() && it->m_Undos.size()==0)
        --it;

    if(it==m_UndoList.begin() && it->m_Undos.size()==0)
        return nullptr;

    return &(*it);
}

MadUndo *MadUndoBuffer::Undo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.begin())
        return nullptr;

    if(noCaretMovement)
    {
        --m_CurrentUndo;

        while(m_CurrentUndo != m_UndoList.begin() && m_CurrentUndo->m_Undos.size()==0)
            --m_CurrentUndo;

        if(m_CurrentUndo==m_UndoList.begin() && m_CurrentUndo->m_Undos.size()==0)
            return nullptr;

        return &(*m_CurrentUndo);
    }

    return &(*--m_CurrentUndo);
}

MadUndo *MadUndoBuffer::Redo(bool noCaretMovement)
{
    if(m_CurrentUndo == m_UndoList.end())
        return nullptr;

    if(noCaretMovement)
    {
        while(m_CurrentUndo != m_UndoList.end() && m_CurrentUndo->m_Undos.size()==0)
            ++m_CurrentUndo;

        if(m_CurrentUndo==m_UndoList.end())
            return nullptr;
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
