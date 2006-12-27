///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/MadLines.cpp
// Description: the Text/Data Buffer of MadEdit
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#include "MadLines.h"
#include "MadEncoding.h"
#include "MadSyntax.h"
#include "MadEdit.h"
#include <wx/filename.h>

#ifdef _DEBUG
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#endif


const int BUFFER_SIZE = 1024 * 256;     // 256KB
const int BUFFER_BITS = 18;
const int BUFFER_MASK = BUFFER_SIZE - 1;
const wxFileOffset BUFFER_BASE_MASK = wxFileOffset(-1) << BUFFER_BITS;
                                    //wxFileOffset(-1) -  BUFFER_MASK;

#define NEXTUCHAR_BUFFER_SIZE (1024 * 10)

//===========================================================================
// MadFileNameIsUTF8, MadDirExists, MadConvFileName_WC2MB_UseLibc
// for Testing/Converting of FileName Encoding under Linux
//===========================================================================

bool g_WC2MB_2_utf8=true;
bool g_MB2WC_is_utf8=false;
bool g_MB2WC_check_dir_filename=false;//check dir, filename separately

// return 0: filename not found, 1:yes, -1:no
int MadFileNameIsUTF8(const wxString& filename)
{
    g_WC2MB_2_utf8=false;
    wxLogNull nolog;
    if(wxFileModificationTime(filename)!=0)
    {
        return -1;//libc or under win32
    }

#ifndef __WXMSW__
    // retry by using utf8 filename
    g_WC2MB_2_utf8=true;
    if(wxFileModificationTime(filename)!=0)
    {
        return 1;//utf8
    }
#endif

    return 0;//not found
}

// return 0: name is not a dir or not found, 1:yes(utf8), -1:yes(libc)
int MadDirExists(const wxString& name)
{
#ifdef __WXMSW__
    if(wxDirExists(name))
        return -1;
#else
    int test=MadFileNameIsUTF8(name);

    g_WC2MB_2_utf8 = (test>0);

    if(test==0)
    {
        if(wxDirExists(name))
            return -1;

        return 0;
    }

    if(wxDirExists(name))
        return test;
#endif

    return 0;
}

#ifndef __WXMSW__
size_t MadConvFileName::MB2WC(wchar_t *outputBuf, const char *psz, size_t outputSize) const
{
    size_t len=strlen(psz);

    if(g_MB2WC_check_dir_filename==false)
    {
        g_MB2WC_is_utf8=IsTextUTF8((wxByte *)psz, int(len));

        if(g_MB2WC_is_utf8)
        {
            return wxConvUTF8.MB2WC(outputBuf, psz, outputSize);
        }

        return wxConvLibc.MB2WC(outputBuf, psz, outputSize);
    }

    //check dir, filename separately
    size_t dirlen=0, fnlen=0;
    if(len>0)
    {
        for(int i=int(len-1);i>=0;i--, fnlen++)
        {
            if(psz[i]=='/')
                break;
        }
        dirlen=len-fnlen;
    }

    size_t dirret=0;
    if(dirlen!=0)
    {
        vector<char> cbuf;
        cbuf.resize(dirlen+1);
        char *pbuf = (char *) &(*cbuf.begin());
        memcpy(pbuf, psz, dirlen);
        cbuf[dirlen]=0;

        bool is_utf8=IsTextUTF8((wxByte *)pbuf, int(dirlen));
        if(is_utf8)
        {
            dirret=wxConvUTF8.MB2WC(outputBuf, pbuf, outputSize);
        }
        else
        {
            dirret=wxConvLibc.MB2WC(outputBuf, pbuf, outputSize);
        }
    }

    if(dirret==size_t(-1))
        return size_t(-1);

    size_t fnret=0;
    if(fnlen!=0)
    {
        bool is_utf8=IsTextUTF8((wxByte *)psz+dirlen, int(fnlen));
        wchar_t *obuf=outputBuf;
        if(outputBuf!=NULL)
            obuf+=dirret;

        if(is_utf8)
        {
            fnret=wxConvUTF8.MB2WC(obuf, psz+dirlen, outputSize-dirret);
        }
        else
        {
            fnret=wxConvLibc.MB2WC(obuf, psz+dirlen, outputSize-dirret);
        }
    }

    if(fnret==size_t(-1))
        return size_t(-1);

    return dirret+fnret;
}

size_t MadConvFileName::WC2MB(char *outputBuf, const wchar_t *psz, size_t outputSize) const
{
    if(g_WC2MB_2_utf8)
        return wxConvUTF8.WC2MB(outputBuf, psz, outputSize);

    return wxConvLibc.WC2MB(outputBuf, psz, outputSize);
}
#endif

//===========================================================================
// MadMemData
//===========================================================================

MadMemData::MadMemData()
{
    m_Size = 0;
}

MadMemData::~MadMemData()
{
    size_t i, count;
    for(i = 0, count = m_Buffers.size(); i < count; ++i)
    {
        delete []m_Buffers[i];
    }
    m_Buffers.clear();
}

wxByte MadMemData::Get(const wxFileOffset &pos)
{
    wxASSERT((pos >= 0) && (pos < m_Size));

    int buf = pos >> BUFFER_BITS;    //pos / BUFFER_SIZE;
    int idx = pos & BUFFER_MASK;    //pos % BUFFER_SIZE;

    return m_Buffers[buf][idx];
}

void MadMemData::Get(const wxFileOffset &pos, wxByte *buffer, size_t size)
{
    wxASSERT((pos >= 0) && (size > 0) && ((pos + size) <= m_Size));

    int buf = pos >> BUFFER_BITS;    //pos / BUFFER_SIZE;
    int idx = pos & BUFFER_MASK;    //pos % BUFFER_SIZE;

    size_t buf_size = BUFFER_SIZE - idx;

    while(true)
    {
        if(buf_size > size) buf_size = size;

        /***
        if(buf_size <= sizeof(int)*40) buggy!!!
        {
            int *src=(int*)buffer;
            int *des=(int*)(m_Buffers[buf] + idx);
            size_t s=buf_size;

            if(s >= sizeof(int)*20)     //max:40
            {
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                s-=sizeof(int)*20;
            }
            if(s >= sizeof(int)*10)     //max:20
            {
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                s-=sizeof(int)*10;
            }
            if(s >= sizeof(int)*10)     //max:10
            {
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                *src++ = *des++; *src++ = *des++;
                s-=sizeof(int)*10;
            }
            if(s >= sizeof(int)*5)      //max:9
            {
                *src++ = *des++;
                *src++ = *des++;
                *src++ = *des++;
                *src++ = *des++;
                *src++ = *des++;
                s-=sizeof(int)*5;
            }
            if(s >= sizeof(int))        //max:4
            {
                *src++ = *des++;
                s-=sizeof(int);
                if(s >= sizeof(int))        //max:3
                {
                    *src++ = *des++;
                    s-=sizeof(int);
                    if(s >= sizeof(int))        //max:2
                    {
                        *src++ = *des++;
                        s-=sizeof(int);
                        if(s >= sizeof(int))        //max:1
                        {
                            *src++ = *des++;
                            s-=sizeof(int);
                        }
                    }
                }
            }
            if(s)       //max:3byte
            {
                buffer=(wxByte*)src;
                wxByte *bdes=(wxByte*)des;

                *buffer++=*bdes++;
                s--;
                if(s)   //max:2b
                {
                    *buffer++=*bdes++;
                    s--;
                    if(s)   //max:1byte
                    {
                        *buffer=*bdes;
                        s--;            //over
                    }
                }
            }

            wxASSERT(s==0);
        }
        else
            ***/
        {
            memcpy(buffer, m_Buffers[buf] + idx, buf_size);
        }

        if((size -= buf_size) ==0) break;

        ++buf;
        idx = 0;
        buffer += buf_size;
        buf_size = BUFFER_SIZE;
    }
}

wxFileOffset MadMemData::Put(wxByte * buffer, size_t size)
{
    wxASSERT(size > 0);

    size_t buf = m_Size >> BUFFER_BITS;        //m_Size / BUFFER_SIZE;
    int idx = m_Size & BUFFER_MASK;            //m_Size % BUFFER_SIZE;
    size_t buf_size = BUFFER_SIZE - idx;
    wxByte *tmp = idx!=0 ? m_Buffers[buf] : 0;

    wxFileOffset pos = m_Size;
    m_Size += size;
    do
    {
        if(idx == 0)
        {
            if(buf < m_Buffers.size())
            {
                tmp = m_Buffers[buf];
            }
            else
            {
                tmp = new wxByte[BUFFER_SIZE];
                m_Buffers.push_back(tmp);                // add a new buffer
            }
            buf_size = BUFFER_SIZE;
        }

        if(size < buf_size) buf_size = size;

        memcpy(tmp + idx, buffer, buf_size);
        buffer += buf_size;
        size -= buf_size;
        idx = 0;
        ++buf;
    }
    while(size > 0);

    return pos;
}


//===========================================================================
// MadFileData
//===========================================================================

MadFileData::MadFileData(const wxString &name)
{
    m_Name = name;
    m_Size = 0;
    m_OpenSuccess = true;
    m_ReadOnly = false;
    m_Buffer1 = NULL;
    m_Buffer2 = NULL;

    int utf8test=MadFileNameIsUTF8(name);

    if(utf8test==0)// this file/dir is not exist
    {
        return;
    }

    MadConvFileName_WC2MB_UseLibc uselibc(utf8test<0);

    if(wxDirExists(name))// test this name is a dir or not
    {
        m_OpenSuccess = false;
        return;
    }

    if(OpenFile())
    {
        m_Size = m_File.SeekEnd();    // get size
        m_File.Seek(0);                // move to begin

        m_Buffer1 = new wxByte[BUFFER_SIZE];
        m_Buf1Pos = 0;
        m_Buffer2 = new wxByte[BUFFER_SIZE];
        m_Buf2Pos = -1;

        // read first data-block to buffer1
        size_t size = BUFFER_SIZE;
        if(BUFFER_SIZE > m_Size) size = m_Size;
        m_File.Read(m_Buffer1, size);
    }
}

MadFileData::~MadFileData()
{
    if(m_File.IsOpened())   m_File.Close();
    if(m_Buffer1)       delete []m_Buffer1;
    if(m_Buffer2)       delete []m_Buffer2;
}

wxByte MadFileData::Get(const wxFileOffset &pos)
{
    wxASSERT((pos >= 0) && (pos < m_Size));

    wxFileOffset idx;
    if(m_Buf1Pos>=0)
    {
        idx = pos - m_Buf1Pos;
        if(idx >= 0 && idx < BUFFER_SIZE)       // wanted byte in buffer1
        {
            return m_Buffer1[(size_t)idx];
        }
    }

    if(m_Buf2Pos>=0)
    {
        idx = pos - m_Buf2Pos;
        if(idx >= 0 && idx < BUFFER_SIZE)       // wanted byte in buffer2
        {
            return m_Buffer2[(size_t)idx];
        }
    }

    if(m_Buf1Pos>=0)
    {
        // swap buffer1 buffer2
        wxByte *tmp=m_Buffer2;
        m_Buffer2=m_Buffer1;
        m_Buffer1=tmp;

        m_Buf2Pos=m_Buf1Pos;
    }

    // read data-block to buffer1
    m_Buf1Pos = pos & BUFFER_BASE_MASK; //(pos >> BUFFER_BITS) << BUFFER_BITS;    //(pos / BUFFER_SIZE) * BUFFER_SIZE;
    m_File.Seek(m_Buf1Pos);

    wxFileOffset size = m_Size - m_Buf1Pos;
    if(size > BUFFER_SIZE)    size = BUFFER_SIZE;

    m_File.Read(m_Buffer1, size);

    idx = pos - m_Buf1Pos;
    return m_Buffer1[(size_t)idx];
}

void MadFileData::Get(const wxFileOffset &pos, wxByte * buffer, size_t size)
{
    wxASSERT((pos >= 0) && (size > 0) && ((pos + size) <= m_Size));

    wxFileOffset idx;
    if(m_Buf1Pos>=0)
    {
        idx = pos - m_Buf1Pos;
        if(idx >= 0 && (idx+size)<=BUFFER_SIZE )        // wanted bytes in buffer1
        {
            memcpy(buffer, m_Buffer1+idx, size);
            return;
        }
    }

    if(m_Buf2Pos>=0)
    {
        idx = pos - m_Buf2Pos;
        if(idx >= 0 && (idx+size)<=BUFFER_SIZE )    // wanted bytes in buffer2
        {
            memcpy(buffer, m_Buffer2+idx, size);
            return;
        }
    }

    idx = pos & BUFFER_BASE_MASK; //(pos >> BUFFER_BITS) << BUFFER_BITS;        //(pos / BUFFER_SIZE) * BUFFER_SIZE;
    if(pos+size <= idx+BUFFER_SIZE)                    // wanted bytes in one block
    {
        if(m_Buf1Pos>=0)
        {
            // swap buffer1 buffer2
            wxByte *tmp=m_Buffer2;
            m_Buffer2=m_Buffer1;
            m_Buffer1=tmp;

            m_Buf2Pos=m_Buf1Pos;
        }

        // read data-block to buffer1
        m_Buf1Pos = idx;
        m_File.Seek(m_Buf1Pos);

        wxFileOffset bufsize = m_Size - m_Buf1Pos;
        if(bufsize > BUFFER_SIZE)    bufsize = BUFFER_SIZE;

        m_File.Read(m_Buffer1, bufsize);

        idx = pos - m_Buf1Pos;
        memcpy(buffer, m_Buffer1+idx, size);
        return;
    }

    // wanted bytes cross more than one block
    m_File.Seek(pos);
    m_File.Read(buffer, size);
}

wxFileOffset MadFileData::Put(wxByte *buffer, size_t size)
{
    wxFileOffset pos=m_SavePos;

    m_File.Seek(m_SavePos);
    m_File.Write(buffer, size);
    m_SavePos+=size;

    if(m_Size < m_SavePos)
    {
        m_Size = m_SavePos;
    }

    m_Buf1Pos=m_Buf2Pos=-1;

    return pos;
}

bool MadFileData::OpenFile()
{
    wxLogNull nolog;// disable error message of file.Open()

    m_ReadOnly = false;
    m_OpenSuccess = m_File.Open(m_Name.c_str(), wxFile::read_write);    // read&write
    if(m_OpenSuccess == false)
    {
        m_OpenSuccess = m_File.Open(m_Name.c_str(), wxFile::read);    // read only
        if(m_OpenSuccess == false)
        {
            return false;
        }

        m_ReadOnly = true;
    }

    return true;
}

bool MadFileData::Rename(const wxString &name)
{
    //wxASSERT(handle_ != INVALID_HANDLE_VALUE);

    m_File.Close();

    MadConvFileName_WC2MB_UseLibc uselibc(MadFileNameIsUTF8(m_Name)<0);

    if(wxRenameFile(m_Name, name))
    {
        m_Name = name;
        OpenFile();
        return true;
    }

    // can't rename the file, reopen the file
    OpenFile();
    return false;
}


//===========================================================================
// MadLine
//===========================================================================

void MadLine::Reset(void)
{
    m_Blocks.clear();
    m_RowIndices.clear();
    m_BracePairIndices.clear();
}

void MadLine::Empty(void)
{
    m_Size = 0;
    m_NewLineSize = 0;
    m_Blocks.resize(1);
    m_Blocks[0].Reset();
    m_RowIndices.resize(2);
    m_RowIndices[0].Reset();
    m_RowIndices[1].Reset();
    m_BracePairIndices.clear();
}

ucs4_t MadLine::LastUCharIsNewLine(MadEncoding *encoding)
{
    if(m_Size == 0)
        return 0;

    MadBlockIterator bit = m_Blocks.end();
    wxFileOffset bsize = (--bit)->m_Size;

    ucs4_t uc;
    wxByte buf[4];
    int count,cnt;

    switch (encoding->GetType())
    {
    case etUTF16LE:
        if((m_Size & 1) != 0)                // odd m_Size is invalid
            return 0;

        if(bsize >= 2)
        {
            //uc = (((ucs4_t) bit->Get(bsize - 1)) << 8) | bit->Get(bsize - 2);
            bit->Get(bsize-2, buf, 2);
            uc= (((ucs4_t)buf[1])<<8) | buf[0];
        }
        else
        {
            uc = ((ucs4_t)bit->Get(0)) << 8;
            bsize = (--bit)->m_Size;
            uc = uc | bit->Get(bsize - 1);
        }
        break;
    case etUTF16BE:
        if((m_Size & 1) != 0)                // odd m_Size is invalid
            return 0;

        if(bsize >= 2)
        {
            //uc = (((ucs4_t) bit->Get(bsize - 2)) << 8) | bit->Get(bsize - 1);
            bit->Get(bsize-2, buf, 2);
            uc= (((ucs4_t)buf[0])<<8) | buf[1];
        }
        else
        {
            uc = bit->Get(0);
            bsize = (--bit)->m_Size;
            uc = (((ucs4_t) bit->Get(bsize - 1)) << 8) | uc;
        }
        break;
    case etUTF32LE:
        if((m_Size & 0x03) != 0)                // m_Size must be 4X
            return 0;

        count=4;
        while(true)
        {
            cnt=count;
            if(bsize<cnt) cnt=bsize;
            bit->Get(bsize-cnt, buf+(count-=cnt), cnt);
            if(count==0) break;

            bsize=(--bit)->m_Size;
        }
        uc= (ucs4_t(buf[3])<<24)|(ucs4_t(buf[2])<<16)|(ucs4_t(buf[1])<<8)|buf[0];

        break;
    case etUTF32BE:
        if((m_Size & 0x03) != 0)                // m_Size must be 4X
            return 0;

        count=4;
        while(true)
        {
            cnt=count;
            if(bsize<cnt) cnt=bsize;
            bit->Get(bsize-cnt, buf+(count-=cnt), cnt);
            if(count==0) break;

            bsize=(--bit)->m_Size;
        }
        uc= (ucs4_t(buf[0])<<24)|(ucs4_t(buf[1])<<16)|(ucs4_t(buf[2])<<8)|buf[3];

        break;
    case etSingleByte:
        uc = encoding->SBtoUCS4(bit->Get(bsize - 1));            // for EBCDIC
        break;
    default:
        uc = bit->Get(bsize - 1);
    }

    if((uc == 0x0A) || (uc == 0x0D)) return uc;
    return 0;
}

bool MadLine::FirstUCharIs0x0A(MadEncoding *encoding)
{
    if(m_Size == 0) return false;

    MadBlockIterator bit = m_Blocks.begin();
    wxFileOffset bsize = bit->m_Size;

    ucs4_t uc;
    wxByte buf[4];
    int idx,cnt;

    switch (encoding->GetType())
    {
    case etUTF16LE:
        if((m_Size & 1) != 0)                // odd m_Size is invalid
            return false;

        if(bsize >= 2)
        {
            //uc = (((ucs4_t) bit->Get(1)) << 8) | bit->Get(0);
            bit->Get(0, buf, 2);
            uc = (ucs4_t(buf[1]) << 8) | buf[0];
        }
        else
        {
            uc = bit->Get(0);
            uc = (ucs4_t((++bit)->Get(0)) << 8) | uc;
        }
        break;
    case etUTF16BE:
        if((m_Size & 1) != 0)                // odd m_Size is invalid
            return false;

        if(bsize > 1)
        {
            //uc = (((ucs4_t) bit->Get(0)) << 8) | bit->Get(1);
            bit->Get(0, buf, 2);
            uc = (ucs4_t(buf[0]) << 8) | buf[1];
        }
        else
        {
            uc = bit->Get(0);
            uc = (uc << 8) | (++bit)->Get(0);
        }
        break;
    case etUTF32LE:
        if((m_Size & 0x03) != 0)                // m_Size must be 4X
            return false;

        idx=0;
        while(true)
        {
            cnt=4-idx;
            if(bsize<cnt) cnt=bsize;
            bit->Get(0, buf+idx, cnt);
            if((idx+=cnt)==4) break;

            bsize=(++bit)->m_Size;
        }
        uc= (ucs4_t(buf[3])<<24)|(ucs4_t(buf[2])<<16)|(ucs4_t(buf[1])<<8)|buf[0];

        break;
    case etUTF32BE:
        if((m_Size & 0x03) != 0)                // m_Size must be 4X
            return false;

        idx=0;
        while(true)
        {
            cnt=4-idx;
            if(bsize<cnt) cnt=bsize;
            bit->Get(0, buf+idx, cnt);
            if((idx+=cnt)==4) break;

            bsize=(++bit)->m_Size;
        }
        uc= (ucs4_t(buf[0])<<24)|(ucs4_t(buf[1])<<16)|(ucs4_t(buf[2])<<8)|buf[3];

        break;
    case etSingleByte:
        uc = encoding->SBtoUCS4(bit->Get(0));            // for EBCDIC
        break;
    default:
        uc = bit->Get(0);
    }

    return uc == 0x0A;
}


//===========================================================================
// MadLines
//===========================================================================

MadLines::MadLines(MadEdit *madedit)
{
    m_MadEdit = madedit;
    m_Syntax = madedit->m_Syntax;
    SetEncoding(madedit->m_Encoding);

    // reserve one empty line
    Empty(false);

    //m_Name="";
    m_ReadOnly = false;
    m_MaxLineWidth = 0;

    m_FileData = NULL;
    m_TmpFileData = NULL;

    m_MemData = new MadMemData();

    m_WriteBuffer=NULL;

    m_NextUChar_Buffer = new wxByte[NEXTUCHAR_BUFFER_SIZE+10];
    m_NextUChar_BufferLoadNew=true;
    m_NextUChar_BufferStart=0;
    m_NextUChar_BufferSize=0;
}

MadLines::~MadLines(void)
{
    // clear all lines
    Empty(true);

    if(m_FileData)
        delete m_FileData;

    if(m_TmpFileData)
    {
        wxString tmpfile = m_TmpFileData->m_Name;
        delete m_TmpFileData;
        wxRemoveFile(tmpfile);
    }

    if(m_MemData)
        delete m_MemData;

    delete []m_NextUChar_Buffer;
}


void MadLines::Empty(bool freeAll)
{
    m_Size = 0;
    m_LineCount = 0;
    m_RowCount = 0;

    MadLineIterator iter, end;
    iter = m_LineList.begin();
    end = m_LineList.end();

    if(!freeAll)
    {
        if(m_LineList.empty())
        {
            iter = m_LineList.insert(end, MadLine());
        }

        // reserve one empty line
        iter->Empty();
        // to second line
        ++iter;

        ++m_LineCount;
        ++m_RowCount;
    }

    if(iter != end)
        do
        {
            iter->m_Blocks.clear();
            iter->m_RowIndices.clear();
            iter = m_LineList.erase(iter);
        }
        while(iter != end);

}

void MadLines::Clear(bool freeAll)
{
    Empty(freeAll);

    m_Name = wxT("");
    m_ReadOnly = false;
    m_MaxLineWidth = 0;

    if(m_FileData)
    {
        delete m_FileData;
        m_FileData = NULL;
    }

    if(m_TmpFileData)
    {
        wxString tmpfile = m_TmpFileData->m_Name;
        delete m_TmpFileData;
        wxRemoveFile(tmpfile);
        m_TmpFileData = NULL;
    }

    m_MemData->Reset();
}

void MadLines::SetEncoding(MadEncoding *encoding)
{
    m_Encoding=encoding;

    switch(encoding->GetType())
    {
    case etSingleByte:
        NextUChar=&MadLines::NextUChar_SBCS;
        break;
    case etDoubleByte:
        NextUChar=&MadLines::NextUChar_DBCS;
        break;
    case etUTF8:
        NextUChar=&MadLines::NextUChar_UTF8;
        break;
    case etUTF16LE:
        NextUChar=&MadLines::NextUChar_UTF16LE;
        break;
    case etUTF16BE:
        NextUChar=&MadLines::NextUChar_UTF16BE;
        break;
    case etUTF32LE:
        NextUChar=&MadLines::NextUChar_UTF32LE;
        break;
    case etUTF32BE:
        NextUChar=&MadLines::NextUChar_UTF32BE;
        break;
    }
}

void MadLines::LoadNewBuffer()
{
    if(m_NextUChar_BufferSize!=0) // move rest data to begin of buffer
    {
        if(m_NextUChar_BufferSize<=sizeof(int))
        {
            int tmp=*((int*)(m_NextUChar_Buffer+m_NextUChar_BufferStart));
            *((int*)m_NextUChar_Buffer)=tmp;
        }
        else
        {
            memcpy(m_NextUChar_Buffer, m_NextUChar_Buffer+m_NextUChar_BufferStart, m_NextUChar_BufferSize);
        }
    }

    m_NextUChar_BufferStart=0;
    wxFileOffset size=m_NextUChar_LineSize-m_NextUChar_BufferNextPos;

    if(size>NEXTUCHAR_BUFFER_SIZE)
        size=NEXTUCHAR_BUFFER_SIZE;

    wxASSERT(size>0);

    m_NextUChar_LineIter->Get(m_NextUChar_BufferNextPos,
                              m_NextUChar_Buffer + m_NextUChar_BufferSize,
                              size);

    m_NextUChar_BufferSize+=size;
    m_NextUChar_BufferNextPos+=size;
}

void MadLines::InitNextUChar(const MadLineIterator &iter, const wxFileOffset pos)
{
    m_NextUChar_LineIter = iter;
    m_NextUChar_Pos = pos;
    m_NextUChar_LineSize = iter->m_Size;


    // LoadNewBuffer()
    if(m_NextUChar_BufferLoadNew || m_NextUChar_BufferSize==0)
    {
        m_NextUChar_BufferStart=0;
        wxFileOffset size=m_NextUChar_LineSize-pos;

        if(size>NEXTUCHAR_BUFFER_SIZE)
            size=NEXTUCHAR_BUFFER_SIZE;

        if(size>0)
        {
            m_NextUChar_LineIter->Get(pos, m_NextUChar_Buffer, size);
        }
        m_NextUChar_BufferSize=size;
    }

    m_NextUChar_BufferNextPos=pos+m_NextUChar_BufferSize;

}

bool MadLines::NextUChar_SBCS(MadUCQueue &ucqueue)
{
    if(m_NextUChar_Pos >= m_NextUChar_LineSize)
        return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    ucqueue.push_back(MadUCPair(m_Encoding->SBtoUCS4(m_NextUChar_Buffer[m_NextUChar_BufferStart]), 1));
    m_NextUChar_Pos++;
    m_NextUChar_BufferStart++;
    m_NextUChar_BufferSize--;
    return true;
}

bool MadLines::NextUChar_DBCS(MadUCQueue &ucqueue)
{
    wxFileOffset rest = m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<2)
    {
        LoadNewBuffer();
    }

    if(rest>1)
    {
        ucs4_t uc;
        if((uc=m_Encoding->DBtoUCS4(m_NextUChar_Buffer+m_NextUChar_BufferStart))==0)// not a valid db-char
        {
            ucqueue.push_back(MadUCPair(m_NextUChar_Buffer[m_NextUChar_BufferStart], 1));
            m_NextUChar_Pos++;
            m_NextUChar_BufferStart++;
            m_NextUChar_BufferSize--;
            return true;
        }

        ucqueue.push_back(MadUCPair(uc, 2));
        m_NextUChar_Pos+=2;
        m_NextUChar_BufferStart+=2;
        m_NextUChar_BufferSize-=2;
        return true;
    }

    ucqueue.push_back(MadUCPair(m_NextUChar_Buffer[m_NextUChar_BufferStart], 1));
    m_NextUChar_Pos++;
    m_NextUChar_BufferStart++;
    m_NextUChar_BufferSize--;
    return true;
}

bool MadLines::NextUChar_UTF8(MadUCQueue &ucqueue)
{
    /***    from rfc3629
    Char. number range    |                UTF-8 octet sequence
    (hexadecimal)        |                            (binary)
    --------------------+---------------------------------------------
    0000 0000-0000 007F | 0xxxxxxx
    0000 0080-0000 07FF | 110xxxxx 10xxxxxx
    0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
    0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

    UTF8-octets = *( UTF8-char )
    UTF8-char     = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
    UTF8-1            = %x00-7F
    UTF8-2            = %xC2-DF UTF8-tail
    UTF8-3            = %xE0 %xA0-BF UTF8-tail / %xE1-EC 2( UTF8-tail ) /
                    %xED %x80-9F UTF8-tail / %xEE-EF 2( UTF8-tail )
    UTF8-4            = %xF0 %x90-BF 2( UTF8-tail ) / %xF1-F3 3( UTF8-tail ) /
                    %xF4 %x80-8F 2( UTF8-tail )
    UTF8-tail     = %x80-BF
    ***/

    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    int cnt=4;
    if(rest<4) cnt=rest;

    wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;

    if(buf[0]<=0x7F)
    {
        ucqueue.push_back(MadUCPair(buf[0], 1));
        m_NextUChar_Pos++;
        m_NextUChar_BufferStart++;
        m_NextUChar_BufferSize--;
        return true;
    }

    ucs4_t uc;
    if(buf[0]<=0xDF)
    {
        if(cnt>=2 && (buf[1] & 0xC0) == 0x80)     // valid 2 bytes
        {
            if((uc= ((ucs4_t(buf[0] & 0x1F)<<6) | (buf[1] & 0x3F) )) >= 0x80)
            {
                ucqueue.push_back(MadUCPair(uc, 2));
                m_NextUChar_Pos+=2;
                m_NextUChar_BufferStart+=2;
                m_NextUChar_BufferSize-=2;
                return true;
            }
        }
        ucqueue.push_back(MadUCPair(buf[0], 1));
        m_NextUChar_Pos++;
        m_NextUChar_BufferStart++;
        m_NextUChar_BufferSize--;
        return true;
    }

    if(buf[0]<=0xEF)
    {
        if(cnt>=3 && (buf[1] & 0xC0)==0x80 && (buf[2] & 0xC0)==0x80)     // valid 3 bytes
        {
            uc= (ucs4_t(buf[0] & 0x0F)<<12) | (ucs4_t(buf[1] & 0x3F) << 6) | (buf[2] & 0x3F);
            if(uc>=0x800 )//&& !(uc>=0xD800 && uc<=0xDFFF))
            {
                ucqueue.push_back(MadUCPair(uc, 3));
                m_NextUChar_Pos+=3;
                m_NextUChar_BufferStart+=3;
                m_NextUChar_BufferSize-=3;
                return true;
            }
        }
        ucqueue.push_back(MadUCPair(buf[0], 1));
        m_NextUChar_Pos++;
        m_NextUChar_BufferStart++;
        m_NextUChar_BufferSize--;
        return true;
    }

    if((buf[0]&0xF0) == 0xF0)
    {
        if(cnt>=4 && (buf[1] & 0xC0)==0x80 && (buf[2] & 0xC0)==0x80 && (buf[3] & 0xC0)==0x80)     // valid 4 bytes
        {
            uc= (ucs4_t(buf[0] & 0x07)<<18) | (ucs4_t(buf[1] & 0x3F) << 12) | (ucs4_t(buf[2] & 0x3F) << 6) | (buf[3] & 0x3F);
            if(uc>=0x10000 && uc<=0x10FFFF)
            {
                ucqueue.push_back(MadUCPair(uc, 4));
                m_NextUChar_Pos+=4;
                m_NextUChar_BufferStart+=4;
                m_NextUChar_BufferSize-=4;
                return true;
            }
        }
    }

    ucqueue.push_back(MadUCPair(buf[0], 1));
    m_NextUChar_Pos++;
    m_NextUChar_BufferStart++;
    m_NextUChar_BufferSize--;
    return true;
}

bool MadLines::NextUChar_UTF16LE(MadUCQueue &ucqueue)
{
    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;

    if(rest >= 2)
    {
        ucs4_t uc;
        uc=(ucs4_t(buf[1])<<8) | buf[0];

        //utf16 surrogates
        if(uc>=0xD800 && uc<=0xDBFF && rest >= 4)
        {
            buf+=2;
            ucs4_t uc1=(ucs4_t(buf[1])<<8) | buf[0];

            if(uc1>=0xDC00 && uc1<=0xDFFF)
            {
                //ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
                ucqueue.push_back(MadUCPair( ((uc-0xD800)<<10) + (uc1-0xDC00) + 0x10000, 4));
                m_NextUChar_Pos+=4;
                m_NextUChar_BufferStart+=4;
                m_NextUChar_BufferSize-=4;
                return true;
            }
        }

        ucqueue.push_back(MadUCPair(uc, 2));
        m_NextUChar_Pos+=2;
        m_NextUChar_BufferStart+=2;
        m_NextUChar_BufferSize-=2;
        return true;
    }

    /***
    ucs4_t uc;
    if(m_MadEdit->editMode_!=emHexMode)
    {
        uc=wxT('?');
    }
    else
    {
        uc=m_NextUChar_LineIter->Get(m_NextUChar_Pos);
    }
    ***/

    ucqueue.push_back(MadUCPair('?', 1));
    m_NextUChar_Pos++;
    m_NextUChar_BufferStart++;
    m_NextUChar_BufferSize--;
    return true;
}

bool MadLines::NextUChar_UTF16BE(MadUCQueue &ucqueue)
{
    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;

    if(rest >= 2)
    {
        ucs4_t uc;
        uc=(ucs4_t(buf[0])<<8) | buf[1];

        //utf16 surrogates
        if(uc>=0xD800 && uc<=0xDBFF && rest>=4)
        {
            buf+=2;
            ucs4_t uc1=(ucs4_t(buf[0])<<8) | buf[1];

            if(uc1>=0xDC00 && uc1<=0xDFFF)
            {
                //ucs4=(highChar -0xD800) * 0x400 + (lowChar -0xDC00) + 0x10000
                ucqueue.push_back(MadUCPair( ((uc-0xD800)<<10) + (uc1-0xDC00) + 0x10000, 4));
                m_NextUChar_Pos+=4;
                m_NextUChar_BufferStart+=4;
                m_NextUChar_BufferSize-=4;
                return true;
            }
        }

        ucqueue.push_back(MadUCPair(uc, 2));
        m_NextUChar_Pos+=2;
        m_NextUChar_BufferStart+=2;
        m_NextUChar_BufferSize-=2;
        return true;
    }

    /***
    ucs4_t uc;
    if(m_MadEdit->editMode_!=emHexMode)
    {
        uc=wxT('?');
    }
    else
    {
        uc=m_NextUChar_LineIter->Get(m_NextUChar_Pos);
    }
    ***/

    ucqueue.push_back(MadUCPair('?', 1));
    m_NextUChar_Pos++;
    m_NextUChar_BufferStart++;
    m_NextUChar_BufferSize--;
    return true;
}

bool MadLines::NextUChar_UTF32LE(MadUCQueue &ucqueue)
{
    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    if(rest>=4)
    {
        wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;
        ucs4_t ucs4=wxUINT32_SWAP_ON_BE(*((ucs4_t*)buf));
        if(ucs4>0x10FFFF || ucs4<0)
        {
            ucs4='?'; // not a valid ucs4 char
        }

        ucqueue.push_back(MadUCPair(ucs4, 4));
        m_NextUChar_Pos+=4;
        m_NextUChar_BufferStart+=4;
        m_NextUChar_BufferSize-=4;
        return true;
    }

    /***
    ucs4_t uc;
    if(m_MadEdit->editMode_!=emHexMode)
    {
        uc=wxT('?');
    }
    else
    {
        uc=m_NextUChar_LineIter->Get(m_NextUChar_Pos);
    }
    ***/

    ucqueue.push_back(MadUCPair('?', MadUCPair::second_type(rest)));
    m_NextUChar_Pos+=rest;
    m_NextUChar_BufferStart+=size_t(rest);
    m_NextUChar_BufferSize-=size_t(rest);
    return true;
}

bool MadLines::NextUChar_UTF32BE(MadUCQueue &ucqueue)
{
    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    if(rest>=4)
    {
        wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;
        ucs4_t ucs4=wxUINT32_SWAP_ON_LE(*((ucs4_t*)buf));
        if(ucs4>0x10FFFF || ucs4<0)
        {
            ucs4='?'; // not a valid ucs4 char
        }

        ucqueue.push_back(MadUCPair( ucs4, 4));
        m_NextUChar_Pos+=4;
        m_NextUChar_BufferStart+=4;
        m_NextUChar_BufferSize-=4;
        return true;
    }

    /***
    ucs4_t uc;
    if(m_MadEdit->editMode_!=emHexMode)
    {
        uc=wxT('?');
    }
    else
    {
        uc=m_NextUChar_LineIter->Get(m_NextUChar_Pos);
    }
    ***/

    ucqueue.push_back(MadUCPair('?', MadUCPair::second_type(rest)));
    m_NextUChar_Pos+=rest;
    m_NextUChar_BufferStart+=size_t(rest);
    m_NextUChar_BufferSize-=size_t(rest);
    return true;
}

bool MadLines::NextUCharIs0x0A(void)
{
    wxFileOffset rest=m_NextUChar_LineSize - m_NextUChar_Pos;
    if(rest<=0) return false;

    //check buffersize
    if(m_NextUChar_BufferNextPos!=m_NextUChar_LineSize && m_NextUChar_BufferSize<4)
    {
        LoadNewBuffer();
    }

    wxByte *buf=m_NextUChar_Buffer+m_NextUChar_BufferStart;

    switch(m_Encoding->GetType())
    {
    case etUTF16LE:
        if(rest>=2)
        {
            return buf[0]==0x0A && buf[1]==0;
        }
        break;
    case etUTF16BE:
        if(rest>=2)
        {
            return buf[1]==0x0A && buf[0]==0;
        }
        break;
    case etUTF32LE:
        if(rest>=4)
        {
            return buf[0]==0x0A && buf[1]==0 && buf[2]==0 && buf[3]==0;
        }
        break;
    case etUTF32BE:
        if(rest>=4)
        {
            return buf[3]==0x0A && buf[2]==0 && buf[1]==0 && buf[0]==0;
        }
        break;
    case etSingleByte:
        return m_Encoding->SBtoUCS4(*buf) == 0x0A;
        break;
    default:
        return *buf == 0x0A;
        break;
    }

    return false;
}

MadUCPair MadLines::PreviousUChar(/*IN_OUT*/MadLineIterator &lit, /*IN_OUT*/wxFileOffset &linepos)
{
    wxASSERT(linepos>=0);

    MadUCQueue ucq;

    if(linepos==0)
    {
        if(lit==m_LineList.begin())
            return MadUCPair(0, 0);

        --lit;
        linepos=lit->m_Size-lit->m_NewLineSize;

        InitNextUChar(lit, linepos);

        (this->*NextUChar)(ucq);     // OD or OA

        if((this->*NextUChar)(ucq))  // 0D -> 0A
        {
            linepos+=ucq.front().second;
        }

        return ucq.back();
    }

    wxFileOffset lpos=linepos-4;    // max size of ucs4-char == 4 (utf-8,16,32)
    if(lpos<0) lpos=0;

    do
    {
        InitNextUChar(lit, lpos);
        (this->*NextUChar)(ucq);

        if(lpos+ucq.back().second == linepos)
        {
            break; //found
        }
    }
    while(++lpos<linepos);

    if(lpos==linepos)
    {
        return MadUCPair(0, 0);
    }

    linepos=lpos;

    return ucq.back();
}


int MadLines::FindStringCase(MadUCQueue &ucqueue, MadStringIterator begin,
                            const MadStringIterator &end, size_t &len)
// 0: none, 1: first, 2:...
{
    wxASSERT(!ucqueue.empty());

    int idx = 1;
    bool bNextUC, bNotNewLine;
    ucs4_t uc, firstuc = ucqueue.front().first;
    size_t ucsize = ucqueue.size();
    do
    {
        const wchar_t *cstr = begin->c_str();
        len = begin->Length();

        if(firstuc == (ucs4_t)cstr[0])
        {
            if(len == 1) return idx;

            uc = ucqueue[ucsize-1].first;

            bNextUC = true;
            bNotNewLine = (uc != 0x0D && uc != 0x0A);

            if(ucsize < len && bNotNewLine)
            {
                do
                {
                    bNextUC = (this->*NextUChar)(ucqueue);
                    uc = ucqueue.back().first;
                    bNotNewLine = (uc != 0x0D && uc != 0x0A);
                }
                while(bNextUC && bNotNewLine && (++ucsize) < len && uc == (ucs4_t)cstr[ucsize - 1]);
            }

            if(bNextUC && ucsize >= len)
            {
                if(bNotNewLine || ucsize > len)
                {
                    deque < MadUCPair >::iterator it = ucqueue.begin();
                    ++it;
                    while(*(++cstr) != 0)
                    {
                        if((ucs4_t)*cstr != it->first) break;
                        ++it;
                    }
                    if(*cstr == 0) return idx;
                }
            }
        }

        ++idx;
    }
    while(++begin != end);

    return 0;
}

int MadLines::FindStringNoCase(MadUCQueue &ucqueue, MadStringIterator begin,
                            const MadStringIterator &end, size_t &len)
// 0: none, 1: first, 2:...
{
    wxASSERT(!ucqueue.empty());

    int idx = 1;
    bool bNextUC, bNotNewLine;
    ucs4_t uc, firstuc = ucqueue.front().first;

    if(firstuc>='A' && firstuc<='Z')
    {
        firstuc |= 0x20;    // to lower case
    }

    size_t ucsize = ucqueue.size();
    do
    {
        const wchar_t *cstr = begin->c_str();
        len = begin->Length();

        if(firstuc == (ucs4_t)cstr[0])
        {
            if(len == 1) return idx;

            uc = ucqueue[ucsize-1].first;
            if(uc>='A' && uc<='Z')
            {
                uc |= 0x20; // to lower case
            }

            bNextUC = true;
            bNotNewLine = (uc != 0x0D && uc != 0x0A);

            if(ucsize < len && bNotNewLine)
            {
                do
                {
                    bNextUC = (this->*NextUChar)(ucqueue);
                    uc = ucqueue.back().first;
                    if(uc>='A' && uc<='Z')
                    {
                        uc |= 0x20; // to lower case
                    }
                    bNotNewLine = (uc != 0x0D && uc != 0x0A);
                }
                while(bNextUC && bNotNewLine && (++ucsize) < len && uc == (ucs4_t)cstr[ucsize - 1]);
            }

            if(bNextUC && ucsize >= len)
            {
                if(bNotNewLine || ucsize > len)
                {
                    deque < MadUCPair >::iterator it = ucqueue.begin();
                    ++it;
                    while(*(++cstr) != 0)
                    {
                        uc=it->first;
                        if(uc>='A' && uc<='Z')
                        {
                            uc |= 0x20; // to lower case
                        }

                        if((ucs4_t)*cstr != uc) break;
                        ++it;
                    }
                    if(*cstr == 0) return idx;
                }
            }
        }

        ++idx;
    }
    while(++begin != end);

    return 0;
}


MadLineState MadLines::Reformat(MadLineIterator iter)
{
    ReformatCount = 1;

    iter->m_BracePairIndices.clear();

    if(iter->m_Size == 0)                    // is a empty line
        return iter->m_State;

    m_NextUChar_BufferLoadNew=false;
    m_NextUChar_BufferStart=0;
    m_NextUChar_BufferSize=0;

    MadLineIterator nextline = iter;
    ++nextline;

    iter->m_State.CommentOff = false;
    MadLineState state = iter->m_State;

    InitNextUChar(iter, 0);
    iter->m_NewLineSize = 0;

    // every line must be one row at first
    m_RowCount -= (iter->RowCount() - 1);
    iter->m_RowIndices.resize(2);

    MadRowIndex rowidx;
    int rowidx_idx = 0;

    MadUCQueue ucqueue;
    int ucwidth;
    size_t rowlen;                                     // byte-length
    size_t wordlength;
    int wordwidth;
    int wordisdelimiter=0;//1:is delimiter, -1:not delimiter, 0:other

    const size_t maxlinelength = m_MadEdit->m_MaxLineLength;
    int maxwidth = m_MadEdit->GetMaxWordWrapWidth();
    size_t bomlen=0;

    (this->*NextUChar)(ucqueue);

    switch (m_Encoding->GetType())
    {
    case etUTF8:
    case etUTF16LE:
    case etUTF16BE:
    case etUTF32LE:
    case etUTF32BE:
        // Byte-Order Mark
        if(ucqueue.front().first == 0xFEFF && iter == m_LineList.begin())
        {
            // ignore BOM in first line
            rowidx.m_Start = ucqueue.front().second;
            bomlen = rowidx.m_Start;
            ucqueue.pop_front();
            (this->*NextUChar)(ucqueue);
        }
    default: break;
    }

    ucs4_t firstuc, lastuc, prevuc;
    int index, notSpaceCount;
    size_t firstuclen, length, eatUCharCount, bracepos;

    int *bracexpos;
    int bracexpos_count;

    MadStringIterator sit, sitend;
    vector < wxString > strvec;
    MadSyntaxRange *srange = NULL;
    if(state.RangeId)
        srange = m_Syntax->GetSyntaxRange(state.RangeId);

    int CheckState = m_Syntax->m_CheckState;

    if(m_Syntax->m_CaseSensitive)
        FindString=&MadLines::FindStringCase;
    else
        FindString=&MadLines::FindStringNoCase;

    if(!ucqueue.empty())
    {
        while(true)
        {
            firstuc = 0;
            wordlength = 0;
            wordwidth = 0;
            rowlen = 0;
            eatUCharCount = 0;

            notSpaceCount=0;
            bracepos=bomlen;    // first line only
            bracexpos=NULL;
            bracexpos_count=0;
            bomlen=0;

            bool BeginOfLine=true;

            do
            {
                prevuc = firstuc;

                MadUCPair &ucp = ucqueue.front();
                firstuc = ucp.first;
                firstuclen = ucp.second;
                if(firstuc == 0x0D)
                {
                    if(prevuc != m_Syntax->nw_EscapeChar)
                    {
                        state.LineComment = 0;
                        state.Directive = 0;
                    }

                    if(NextUCharIs0x0A()) // DOS newline chars
                    {
                        (this->*NextUChar)(ucqueue);
                        m_MadEdit->m_NewLineType = nltDOS;
                        iter->m_NewLineSize = ucqueue.back().second;
                    }
                    else
                    {
                        if(m_MadEdit->m_NewLineType == nltDefault)
                        {
                            m_MadEdit->m_NewLineType = nltMAC;
                        }
                    }

                    iter->m_NewLineSize += (wxByte)firstuclen;

                    ucqueue.clear();

                    break;
                }
                if(firstuc == 0x0A)
                {
                    wxASSERT(ucqueue.size() == 1);

                    if(prevuc != m_Syntax->nw_EscapeChar)
                    {
                        state.LineComment = 0;
                        state.Directive = 0;
                    }

                    if(m_MadEdit->m_NewLineType == nltDefault)
                    {
                        m_MadEdit->m_NewLineType = nltUNIX;
                    }

                    iter->m_NewLineSize = (wxByte)firstuclen;

                    ucqueue.clear();

                    break;
                }

                if(eatUCharCount == 0)
                {
                    ++eatUCharCount; // = 1;

                    if(CheckState)
                    {
                        if(firstuc < 0x100 && firstuc != 0x20 && firstuc != 0x09)
                        {
                            notSpaceCount++;

                            // check left/right brace
                            if(//state.CommentId==0 && state.StringId==0 &&
                                !m_Syntax->m_LeftBrace.empty())
                            {
                                bool ok=false;
                                index=(this->*FindString)(ucqueue, m_Syntax->m_LeftBrace.begin(),
                                                        m_Syntax->m_LeftBrace.end(), length);
                                if(index != 0)
                                {
                                    ok=true;
                                    ucs4_t uc=ucqueue[length-1].first;
                                    if(!m_Syntax->IsDelimiter(uc) && uc>0x20)// check last char for wholeword
                                    {
                                        if(ucqueue.size()>length || (this->*NextUChar)(ucqueue))
                                        {
                                            uc = ucqueue[length].first;
                                            if(!m_Syntax->IsDelimiter(uc) && uc>0x20)
                                            {
                                                ok=false;
                                            }
                                        }
                                    }
                                    if(ok) // check first char for wholeword
                                    {
                                        if(!m_Syntax->IsDelimiter(firstuc) && firstuc>0x20)
                                        {
                                            if(!m_Syntax->IsDelimiter(prevuc) && prevuc>0x20)
                                            {
                                                ok=false;
                                            }
                                        }
                                    }
                                }

                                if(ok)
                                {
                                    //eatUCharCount = length;
                                    wxUint16 len=wxUint16(firstuclen);
                                    wxUint16 width=m_MadEdit->GetUCharWidth(firstuc);
                                    if(length>1)
                                    {
                                        size_t idx=1;
                                        do
                                        {
                                            len+=ucqueue[idx].second;
                                            width+=m_MadEdit->GetUCharWidth(ucqueue[idx].first);
                                        }
                                        while(++idx < length);
                                    }
                                    iter->m_BracePairIndices.push_back(BracePairIndex(0, width, bracepos, len, 1, index-1));
                                    bracexpos=&iter->m_BracePairIndices.back().XPos;
                                    bracexpos_count++;
                                }
                                else // check right brace
                                    if((index=(this->*FindString)(ucqueue, m_Syntax->m_RightBrace.begin(),
                                                                m_Syntax->m_RightBrace.end(), length)) != 0 )
                                {
                                    ok=true;
                                    ucs4_t uc=ucqueue[length-1].first;
                                    if(!m_Syntax->IsDelimiter(uc) && uc>0x20)// check last char for wholeword
                                    {
                                        if(ucqueue.size()>length || (this->*NextUChar)(ucqueue))
                                        {
                                            uc = ucqueue[length].first;
                                            if(!m_Syntax->IsDelimiter(uc) && uc>0x20)
                                            {
                                                ok=false;
                                            }
                                        }
                                    }
                                    if(ok) // check first char for wholeword
                                    {
                                        if(!m_Syntax->IsDelimiter(firstuc) && firstuc>0x20)
                                        {
                                            if(!m_Syntax->IsDelimiter(prevuc) && prevuc>0x20)
                                            {
                                                ok=false;
                                            }
                                        }
                                    }

                                    if(ok)
                                    {
                                        //eatUCharCount = length;
                                        wxUint16 len=wxUint16(firstuclen);
                                        wxUint16 width=m_MadEdit->GetUCharWidth(firstuc);
                                        if(length>1)
                                        {
                                            size_t idx=1;
                                            do
                                            {
                                                len+=ucqueue[idx].second;
                                                width+=m_MadEdit->GetUCharWidth(ucqueue[idx].first);
                                            }
                                            while(++idx < length);
                                        }
                                        iter->m_BracePairIndices.push_back(BracePairIndex(0, width, bracepos, len, 0, index-1));
                                        bracexpos=&iter->m_BracePairIndices.back().XPos;
                                        bracexpos_count++;
                                    }
                                }
                            }

                            if(state.LineComment==0)
                            {
                                // check EscapeChar
                                if(firstuc == m_Syntax->nw_EscapeChar)
                                {
                                    if(ucqueue.size() == 1)
                                        (this->*NextUChar)(ucqueue);

                                    if(ucqueue.size() != 1)
                                    {
                                        lastuc = ucqueue[1].first;
                                        if(lastuc!=0x0D && lastuc!=0x0A)
                                        {
                                            ++eatUCharCount;//=2
                                        }
                                    }
                                    goto _NOCHECK_;
                                }

                                // check block comment off
                                if(state.CommentId != 0)
                                {
                                    sit = m_Syntax->m_BlockCommentOff.begin();
                                    std::advance(sit, state.CommentId - 1);
                                    sitend = sit;
                                    ++sitend;
                                    index = (this->*FindString)(ucqueue, sit, sitend, length);
                                    if(index != 0)
                                    {
                                        state.CommentId = 0;
                                        eatUCharCount = length;

                                        iter->m_State.CommentOff = true;
                                    }
                                    goto _NOCHECK_;
                                }

                                // check string off
                                if(state.StringId != 0)
                                {
                                    //if(firstuc == m_Syntax->nw_EscapeChar)
                                    //{
                                        //if(ucqueue.size() == 1)
                                            //(this->*NextUChar)(ucqueue);

                                        //if(ucqueue.size() != 1)
                                        //{
                                            //lastuc = ucqueue[1].first;

                                            //if(lastuc == m_Syntax->nw_EscapeChar
                                                //|| m_Syntax->m_StringChar.Find(lastuc)+1 == state.StringId)
                                            //{
                                                //++eatUCharCount;//=2
                                            //}
                                        //}
                                        //goto _NOCHECK_;
                                    //}

                                    if(m_Syntax->m_StringChar.Find(firstuc)+1 == state.StringId)
                                    {
                                        state.StringId = 0;
                                        //eatUCharCount=1;
                                    }
                                    goto _NOCHECK_;

                                }

                                // check string on
                                if(!m_Syntax->m_StringChar.IsEmpty() && m_Syntax->IsInRange(state.RangeId, m_Syntax->m_StringInRange))
                                {
                                    if((index = m_Syntax->m_StringChar.Find(firstuc)+1) != 0)
                                    {
                                        state.StringId = index;
                                        //eatUCharCount=1;

                                        goto _NOCHECK_;
                                    }
                                }

                                // check directive on
                                if(!m_Syntax->m_DirectiveLeadingAtBOL || BeginOfLine)
                                {
                                    if(notSpaceCount==1 && !m_Syntax->m_DirectiveLeading.IsEmpty())
                                    {
                                        if((index = m_Syntax->m_DirectiveLeading.Find(firstuc)+1) != 0)
                                        {
                                            state.Directive = index;
                                            //eatUCharCount=1;

                                            goto _NOCHECK_;
                                        }
                                    }
                                }

                                // check block comment on
                                if(!m_Syntax->m_BlockCommentOn.empty())
                                {
                                    index =(this->*FindString)(ucqueue, m_Syntax->m_BlockCommentOn.begin(),
                                            m_Syntax->m_BlockCommentOn.end(), length);
                                    if(index != 0)    // got
                                    {
                                        // check InRange
                                        if(m_Syntax->IsInRange(state.RangeId,m_Syntax->m_BlockCommentInRange[index - 1]))
                                        {
                                            state.CommentId = index;
                                            //state.CommentOff = false;
                                            eatUCharCount = length;
                                            goto _NOCHECK_;
                                        }
                                    }
                                }

                                // check line comment
                                if(!m_Syntax->m_LineCommentAtBOL || BeginOfLine)
                                {
                                    if(!m_Syntax->m_LineComment.empty())
                                    {
                                        index =(this->*FindString)(ucqueue, m_Syntax->m_LineComment.begin(),
                                                m_Syntax->m_LineComment.end(), length);
                                        if(index != 0)    // got
                                        {
                                            // check InRange
                                            if(m_Syntax->IsInRange(state.RangeId, m_Syntax->m_LineCommentInRange))
                                            {
                                                if(state.RangeId==0)
                                                {
                                                    eatUCharCount = INT_MAX; // eat all line
                                                }
                                                else
                                                {
                                                    // range-off mark may be beyond linecomment (ex: javascript)
                                                    eatUCharCount = length;
                                                }
                                                state.LineComment = index;
                                                state.Directive = 0;
                                                goto _NOCHECK_;
                                            }
                                        }
                                    }
                                }
                            }

                            // check range off
                            if(state.RangeId != 0)
                            {
                                if(srange)
                                {
                                    strvec.push_back(srange->end);
                                    if((this->*FindString)(ucqueue, strvec.begin(), strvec.end(), length) != 0)
                                    {
                                        eatUCharCount = length;
                                        state.RangeId = 0;
                                        strvec.clear();
                                        goto _NOCHECK_;
                                    }
                                    strvec.clear();
                                }
                            }

                            // check range on
                            if(state.LineComment==0 && state.RangeId == 0 && !m_Syntax->m_RangeBeginString.empty())
                            {
                                index =(this->*FindString)(ucqueue, m_Syntax->m_RangeBeginString.begin(),
                                        m_Syntax->m_RangeBeginString.end(), length);
                                if(index != 0)
                                {
                                    eatUCharCount = length;
                                    state.RangeId = m_Syntax->m_CustomRange[index - 1].id;
                                    srange = m_Syntax->GetSyntaxRange(state.RangeId);
                                    //goto _NOCHECK_;
                                }
                            }

                        }
                    }
                }

            _NOCHECK_:
                // eat one front uchar
                --eatUCharCount;

                ucqueue.pop_front();

                bracepos+=firstuclen;


                if(rowlen + int (firstuclen) > maxlinelength)     // wordwrap by line length
                {
                    bool move_word_to_next_line = false;
                    if(wordlength != maxlinelength)
                    {
                        if(wordisdelimiter>0)
                        {
                            if(m_Syntax->IsDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                        else if(wordisdelimiter<0)
                        {
                            if(m_Syntax->IsNotDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                    }

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Width -= wordwidth;

                        if(bracexpos_count>0) // correct bracexpos after wordwrap
                        {
                            vector <BracePairIndex>::iterator it=iter->m_BracePairIndices.end();
                            do
                            {
                                --it;
                                if(it->XPos>=rowidx.m_Width)
                                {
                                    it->XPos-=rowidx.m_Width;
                                }
                            }
                            while(--bracexpos_count > 0);
                        }
                    }
                    bracexpos_count=0;

                    iter->m_RowIndices[rowidx_idx++] = rowidx;
                    iter->m_RowIndices.push_back(MadRowIndex());
                    m_RowCount++;

                    if(rowidx.m_Width > m_MaxLineWidth)
                        m_MaxLineWidth = rowidx.m_Width;

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Start += rowlen - wordlength;
                        rowidx.m_Width = wordwidth;
                        rowlen = wordlength;
                    }
                    else
                    {
                        rowidx.m_Start += rowlen;
                        rowidx.m_Width = 0;
                        rowlen = 0;

                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                ucwidth = m_MadEdit->GetUCharWidth(firstuc);
                if(firstuc == 0x09)         // Tab char
                {
                    m_MadEdit->m_HasTab = true;

                    int tabwidth = m_MadEdit->m_TabColumns * m_MadEdit->GetUCharWidth(0x20);

                    ucwidth = maxwidth - rowidx.m_Width;
                    if(ucwidth == 0)            // Tab at line-end
                    {
                        ucwidth = tabwidth;
                    }
                    else                                    // ucwidth >0
                    {
                        tabwidth = tabwidth - (rowidx.m_Width % tabwidth);
                        if(tabwidth < ucwidth)
                            ucwidth = tabwidth;
                    }
                }

                if(rowidx.m_Width + ucwidth > maxwidth)    // wordwrap by width
                {
                    bool move_word_to_next_line = false;
                    if(wordwidth != rowidx.m_Width && !m_MadEdit->HexPrinting())
                    {
                        if(wordisdelimiter>0)
                        {
                            if(m_Syntax->IsDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                        else if(wordisdelimiter<0)
                        {
                            if(m_Syntax->IsNotDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                    }

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Width -= wordwidth;

                        if(bracexpos_count>0) // correct bracexpos after wordwrap
                        {
                            vector <BracePairIndex>::iterator it=iter->m_BracePairIndices.end();
                            do
                            {
                                --it;
                                if(it->XPos>=rowidx.m_Width)
                                {
                                    it->XPos-=rowidx.m_Width;
                                }
                            }
                            while(--bracexpos_count > 0);
                        }
                    }
                    bracexpos_count=0;

                    iter->m_RowIndices[rowidx_idx++] = rowidx;
                    iter->m_RowIndices.push_back(MadRowIndex());
                    m_RowCount++;

                    if(rowidx.m_Width > m_MaxLineWidth)
                        m_MaxLineWidth = rowidx.m_Width;

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Start += rowlen - wordlength;
                        rowidx.m_Width = wordwidth;
                        rowlen = wordlength;
                    }
                    else
                    {
                        rowidx.m_Start += rowlen;
                        rowidx.m_Width = 0;
                        rowlen = 0;

                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                if(wordlength == 0)
                {
                    if(m_Syntax->IsDelimiter(firstuc))
                    {
                        wordisdelimiter = 1;
                        wordlength = firstuclen;
                        wordwidth = ucwidth;
                    }
                    else if(m_Syntax->IsNotDelimiter(firstuc))
                    {
                        wordisdelimiter = -1;
                        wordlength = firstuclen;
                        wordwidth = ucwidth;
                    }
                }
                else
                {
                    if(wordisdelimiter<0)
                    {
                        if(m_Syntax->IsNotDelimiter(firstuc))
                        {
                            wordlength += firstuclen;
                            wordwidth += ucwidth;
                        }
                        else if(m_Syntax->IsDelimiter(firstuc))
                        {
                            wordisdelimiter = 1;
                            wordlength = firstuclen;
                            wordwidth = ucwidth;
                        }
                        else
                        {
                            wordisdelimiter=0;
                            wordlength = 0;
                            wordwidth = 0;
                        }
                    }
                    else if(wordisdelimiter>0)
                    {
                        if(m_Syntax->IsDelimiter(firstuc))
                        {
                            wordlength += firstuclen;
                            wordwidth += ucwidth;
                        }
                        else if(m_Syntax->IsNotDelimiter(firstuc))
                        {
                            wordisdelimiter = -1;
                            wordlength = firstuclen;
                            wordwidth = ucwidth;
                        }
                        else
                        {
                            wordisdelimiter=0;
                            wordlength = 0;
                            wordwidth = 0;
                        }
                    }
                    else
                    {
                        //wordisdelimiter=0;
                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                if(bracexpos!=NULL)
                {
                    *bracexpos = rowidx.m_Width;
                    bracexpos=NULL;
                }

                rowidx.m_Width += ucwidth;
                rowlen += firstuclen;

                BeginOfLine=false;
            }
            while(!ucqueue.empty() || (this->*NextUChar)(ucqueue));

            wxASSERT(ucqueue.empty());

            if(rowidx.m_Width > m_MaxLineWidth)
                m_MaxLineWidth = rowidx.m_Width;

            // set rowindex
            iter->m_RowIndices[rowidx_idx] = rowidx;
            rowidx.m_Start += rowlen;
            rowidx.m_Width = 0;
            iter->m_RowIndices[++rowidx_idx] = rowidx;

            if(m_NextUChar_Pos >= m_NextUChar_LineSize)
            {
                break;
            }

            iter->m_Size = m_NextUChar_Pos;

            // add a new line
            MadLineIterator nline = m_LineList.insert(nextline, MadLine());
            ++ReformatCount;

            //*** get corresponding block ***//
            MadBlockIterator blockiter = iter->m_Blocks.begin();
            wxFileOffset blockpos = m_NextUChar_Pos;
            if(blockpos >= blockiter->m_Size)
                do
                {
                    blockpos -= blockiter->m_Size;
                    ++blockiter;
                }
                while(blockpos >= blockiter->m_Size);

            if(blockpos != 0)                 //seperate block
            {
                // prev block
                blockiter = iter->m_Blocks.insert(blockiter, *blockiter);
                blockiter->m_Size = blockpos;

                // this block
                ++blockiter;
                blockiter->m_Pos += blockpos;
                blockiter->m_Size -= blockpos;
            }
            //********************************

            //size_t nn = iter->m_Blocks.size();

            // insert blocks to new line
            nline->m_Blocks.insert(nline->m_Blocks.begin(), blockiter,
                                                        iter->m_Blocks.end());

            // erase blocks from old line
            iter->m_Blocks.erase(blockiter, iter->m_Blocks.end());

            nline->m_RowIndices.resize(2);
            nline->m_Size = m_NextUChar_LineSize - m_NextUChar_Pos;
            //state.CommentOff = false;
            nline->m_State = state;
            m_LineCount++;
            m_RowCount++;

            rowidx_idx = 0;
            rowidx.m_Start = 0;

            iter = nline;
            InitNextUChar(iter, 0);
            (this->*NextUChar)(ucqueue);
        }
    }
    else
    {
        iter->m_RowIndices[0] = rowidx;
        iter->m_RowIndices[1] = rowidx;
    }

    m_NextUChar_BufferLoadNew=true;

    return state;
}

// reformat lines [first,last].
// if State isn't OK , may format all rest of lines [first, end)
// return reformated line count
size_t MadLines::Reformat(MadLineIterator first, MadLineIterator last)
{
    MadLineState state;
    MadLineIterator next = first, end = m_LineList.end();
    bool bContinue = true, bIsNotEnd = true, bStateIsNotOkay;
    size_t count = 0;
    do
    {
        if(bContinue && first == last)
            bContinue = false;

        // concatenate the lines that without newline-char or (lastwc=0x0D && firstuc==0x0A)
        if(++next != end)
        {
            ucs4_t uc = first->LastUCharIsNewLine(m_Encoding);
            if(uc == 0 || (uc == 0x0D && next->FirstUCharIs0x0A(m_Encoding)))
                do
                {
                    if(bContinue && next == last)
                    {
                        bContinue = false;
                    }

                    Append(first, next);

                    m_LineCount--;
                    m_RowCount -= next->RowCount();
                    next->Reset();

                    if((next = m_LineList.erase(next)) == end)
                    {
                        bIsNotEnd = false;
                        break;
                    }
                }
                while((uc = first->LastUCharIsNewLine(m_Encoding)) == 0
                            || (uc == 0x0D && next->FirstUCharIs0x0A(m_Encoding)));
        }
        else
            bIsNotEnd = false;

        // reformat this line
        state = Reformat(first);
        count += ReformatCount;

        if(bIsNotEnd)
        {
            // to next line
            first = next;

            if(first->m_State == state)
                bStateIsNotOkay = false;
            else
            {
                first->m_State = state;
                bStateIsNotOkay = true;
            }
        }
        else
        {
            first = next;
            --first;
            if(first->LastUCharIsNewLine(m_Encoding) != 0)
            {
                // add a empty line to end
                first = m_LineList.insert(next, MadLine());
                first->Empty();
                ++m_LineCount;
                ++m_RowCount;
                first->m_State = state;
            }

            m_EndState = state;
            break;
        }
    }
    while(bContinue | bStateIsNotOkay);

    return count;
}

void MadLines::RecountLineWidth(void)
{
    MadLineIterator iter = m_LineList.begin();
    MadLineIterator iterend = m_LineList.end();

    m_RowCount = 0;
    m_MaxLineWidth = 0;

    size_t rowidx_idx;
    MadRowIndex rowidx;
    MadUCQueue ucqueue;
    int ucwidth;
    size_t rowlen;                                     // byte-length
    size_t wordlength;
    int wordwidth;
    int wordisdelimiter=0;//1:is delimiter, -1:not delimiter, 0:other

    ucs4_t firstuc;
    size_t firstuclen;

    const size_t maxlinelength = m_MadEdit->m_MaxLineLength;
    const int maxwidth = m_MadEdit->GetMaxWordWrapWidth();

    BracePairIndex *bpi;
    vector <BracePairIndex>::iterator bpit, bpitend;
    size_t bracepos, bracelen=0, bracemaxlen=0;
    wxUint16 *bracewidth=NULL;
    vector<int*> bracexpos_thisrow;

    do
    {
        rowidx_idx = 0;

        rowidx = iter->m_RowIndices.front();
        rowidx.m_Width = 0;
        InitNextUChar(iter, rowidx.m_Start);

        if(iter->m_RowIndices.size() != 2)
        {
            iter->m_RowIndices.resize(2);
        }

        // correct BracePairIndex
        if(iter->m_BracePairIndices.size() != 0)
        {
            bpit = iter->m_BracePairIndices.begin();
            bpitend = iter->m_BracePairIndices.end();
            bpi = &(*bpit);
        }
        else
        {
            bpi=NULL;
        }
        bracexpos_thisrow.clear();

        ++m_RowCount;

        (this->*NextUChar)(ucqueue);
        if(!ucqueue.empty())
        {
            wordlength = 0;
            wordwidth = 0;
            rowlen = 0;
            bracepos=0;

            do
            {
                MadUCPair &ucp = ucqueue.front();
                firstuc = ucp.first;
                firstuclen = ucp.second;
                if(firstuc == 0x0D)
                {
                    wxASSERT(ucqueue.size() == 1);

                    if(NextUCharIs0x0A()) // DOS newline chars
                    {
                        (this->*NextUChar)(ucqueue);
                    }
                    ucqueue.clear();

                    break;
                }
                if(firstuc == 0x0A)
                {
                    wxASSERT(ucqueue.size() == 1);

                    ucqueue.clear();
                    break;
                }

                ucqueue.pop_front();

                if(rowlen + int (firstuclen) > maxlinelength)     // wordwrap by line length
                {
                    bool move_word_to_next_line = false;
                    if(wordlength != maxlinelength)
                    {
                        if(wordisdelimiter>0)
                        {
                            if(m_Syntax->IsDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                        else if(wordisdelimiter<0)
                        {
                            if(m_Syntax->IsNotDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                    }

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Width -= wordwidth;

                        if(!bracexpos_thisrow.empty())
                        {
                            vector<int*>::iterator it=bracexpos_thisrow.begin();
                            do
                            {
                                if(*(*it) >= rowidx.m_Width)
                                    *(*it) -= rowidx.m_Width;
                            }
                            while(++it != bracexpos_thisrow.end());
                        }
                    }
                    bracexpos_thisrow.clear();

                    iter->m_RowIndices[rowidx_idx++] = rowidx;
                    iter->m_RowIndices.push_back(MadRowIndex());
                    m_RowCount++;

                    if(rowidx.m_Width > m_MaxLineWidth)
                        m_MaxLineWidth = rowidx.m_Width;

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Start += rowlen - wordlength;
                        rowidx.m_Width = wordwidth;
                        rowlen = wordlength;
                    }
                    else
                    {
                        rowidx.m_Start += rowlen;
                        rowidx.m_Width = 0;
                        rowlen = 0;

                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                ucwidth = m_MadEdit->GetUCharWidth(firstuc);
                if(firstuc == 0x09)         // Tab char
                {
                    //m_MadEdit->FHasTab = true;

                    int tabwidth = m_MadEdit->m_TabColumns * m_MadEdit->GetUCharWidth(0x20);

                    ucwidth = maxwidth - rowidx.m_Width;
                    if(ucwidth == 0)            // Tab at line-end
                    {
                        ucwidth = tabwidth;
                    }
                    else                                    // ucwidth >0
                    {
                        tabwidth = tabwidth - (rowidx.m_Width % tabwidth);
                        if(tabwidth < ucwidth)
                            ucwidth = tabwidth;
                    }
                }

                if(rowidx.m_Width + ucwidth > maxwidth)    // wordwrap by width
                {
                    bool move_word_to_next_line = false;
                    if(wordwidth != rowidx.m_Width)
                    {
                        if(wordisdelimiter>0)
                        {
                            if(m_Syntax->IsDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                        else if(wordisdelimiter<0)
                        {
                            if(m_Syntax->IsNotDelimiter(firstuc))
                                move_word_to_next_line = true;
                        }
                    }

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Width -= wordwidth;

                        if(!bracexpos_thisrow.empty())
                        {
                            vector<int*>::iterator it=bracexpos_thisrow.begin();
                            do
                            {
                                if(*(*it) >= rowidx.m_Width)
                                    *(*it) -= rowidx.m_Width;
                            }
                            while(++it != bracexpos_thisrow.end());
                        }
                    }
                    bracexpos_thisrow.clear();

                    iter->m_RowIndices[rowidx_idx++] = rowidx;
                    iter->m_RowIndices.push_back(MadRowIndex());
                    m_RowCount++;

                    if(rowidx.m_Width > m_MaxLineWidth)
                        m_MaxLineWidth = rowidx.m_Width;

                    if(move_word_to_next_line)
                    {
                        rowidx.m_Start += rowlen - wordlength;
                        rowidx.m_Width = wordwidth;
                        rowlen = wordlength;
                    }
                    else
                    {
                        rowidx.m_Start += rowlen;
                        rowidx.m_Width = 0;
                        rowlen = 0;

                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                if(wordlength == 0)
                {
                    if(m_Syntax->IsDelimiter(firstuc))
                    {
                        wordisdelimiter = 1;
                        wordlength = firstuclen;
                        wordwidth = ucwidth;
                    }
                    else if(m_Syntax->IsNotDelimiter(firstuc))
                    {
                        wordisdelimiter = -1;
                        wordlength = firstuclen;
                        wordwidth = ucwidth;
                    }
                }
                else
                {
                    if(wordisdelimiter<0)
                    {
                        if(m_Syntax->IsNotDelimiter(firstuc))
                        {
                            wordlength += firstuclen;
                            wordwidth += ucwidth;
                        }
                        else if(m_Syntax->IsDelimiter(firstuc))
                        {
                            wordisdelimiter = 1;
                            wordlength = firstuclen;
                            wordwidth = ucwidth;
                        }
                        else
                        {
                            wordisdelimiter = 0;
                            wordlength = 0;
                            wordwidth = 0;
                        }
                    }
                    else if(wordisdelimiter>0)
                    {
                        if(m_Syntax->IsDelimiter(firstuc))
                        {
                            wordlength += firstuclen;
                            wordwidth += ucwidth;
                        }
                        else if(m_Syntax->IsNotDelimiter(firstuc))
                        {
                            wordisdelimiter = -1;
                            wordlength = firstuclen;
                            wordwidth = ucwidth;
                        }
                        else
                        {
                            wordisdelimiter=0;
                            wordlength = 0;
                            wordwidth = 0;
                        }
                    }
                    else
                    {
                        //wordisdelimiter=0;
                        wordlength = 0;
                        wordwidth = 0;
                    }
                }

                if(bpi!=NULL)
                {
                    wxASSERT(bracepos<=bpi->LinePos);

                    if(bpi->LinePos==bracepos)
                    {
                        bpi->XPos = rowidx.m_Width;

                        bracexpos_thisrow.push_back(&bpi->XPos);

                        bracemaxlen=bpi->Length;
                        bracelen=0;
                        bracewidth=&bpi->Width;
                        *bracewidth=0;

                        if((++bpit)==bpitend)
                        {
                            bpi=NULL;
                        }
                        else
                        {
                            bpi=&(*bpit);
                        }
                    }
                }
                bracepos+=firstuclen;

                if(bracewidth!=NULL)
                {
                    *bracewidth+=ucwidth;
                    if((bracelen+=firstuclen)>=bracemaxlen)
                    {
                        bracewidth=NULL;
                    }
                }

                rowidx.m_Width += ucwidth;
                rowlen += firstuclen;
            }
            while(!ucqueue.empty() || (this->*NextUChar)(ucqueue));

            if(rowidx.m_Width > m_MaxLineWidth)
                m_MaxLineWidth = rowidx.m_Width;

            // set MadRowIndex
            iter->m_RowIndices[rowidx_idx] = rowidx;
            rowidx.m_Start += rowlen;
            rowidx.m_Width = 0;
            iter->m_RowIndices[++rowidx_idx] = rowidx;
        }
        else
        {
            iter->m_RowIndices.back() = rowidx;
        }

        wxASSERT(bpi==NULL);
    }
    while(++iter != iterend);

}

void MadLines::Append(const MadLineIterator &lit1, const MadLineIterator &lit2)
{
    if(lit2->m_Size == 0)                    // is a empty line
        return;

    lit1->m_Size += lit2->m_Size;

    MadBlockVector &blks1 = lit1->m_Blocks;
    MadBlockVector &blks2 = lit2->m_Blocks;

    MadBlock &blk1 = blks1.back();
    MadBlockIterator bit2 = blks2.begin();

    if(blk1.m_Data == bit2->m_Data && (blk1.m_Pos + blk1.m_Size) == bit2->m_Pos)
    {
        blk1.m_Size += bit2->m_Size;
        ++bit2;
    }

    blks1.insert(blks1.end(), bit2, blks2.end());
}

//===========================================================================

bool MadLines::LoadFromFile(const wxString &filename, const wxString &encoding)
{
    MadFileData *fd = new MadFileData(filename);

    if(!fd->OpenSuccess())
    {
        delete fd;
        return false;
    }

    if(m_FileData)
        delete m_FileData;
    m_FileData = fd;

    if(m_TmpFileData)
    {
        wxString tmpfile = m_TmpFileData->m_Name;
        delete m_TmpFileData;
        wxRemoveFile(tmpfile);
        m_TmpFileData = NULL;
    }

    m_MemData->Reset();

    // reserve one empty line
    Empty(false);

    m_Name = filename;
    m_ReadOnly = fd->IsReadOnly();
    m_MaxLineWidth = 0;

    m_MadEdit->m_HasTab = false;
    m_MadEdit->m_NewLineType = nltDefault;

    m_MadEdit->m_LoadingFile = true;

    if(m_MadEdit->m_EditMode == emHexMode)
        m_MadEdit->SetEditMode(emTextMode);

    const int max_detecting_size = 8192;
    int s;
    if(m_FileData->m_Size > max_detecting_size)
        s = max_detecting_size;
    else
        s = m_FileData->m_Size;

    wxString defaultenc;
    if(encoding.IsEmpty())
    {
        m_MadEdit->m_Config->Read(wxT("/MadEdit/DefaultEncoding"), &defaultenc);
    }
    else
    {
        defaultenc = encoding;
    }

    if(s == 0)
    {
        delete m_FileData;
        m_FileData = NULL;

        // reload syntax
        delete m_Syntax;

        wxFileName fn(filename);
        m_Syntax = MadSyntax::GetSyntaxByExt(fn.GetExt());
        if(m_Syntax==NULL)
        {
            m_Syntax = MadSyntax::GetSyntaxByFileName(fn.GetName());
            if(m_Syntax==NULL)
            {
                m_Syntax = MadSyntax::GetSyntaxByTitle(MadPlainTextTitle);
            }
        }

        wxFont *font=m_MadEdit->m_TextFont;
        m_Syntax->InitNextWord1(m_MadEdit->m_Lines, m_MadEdit->m_WordBuffer, m_MadEdit->m_WidthBuffer, font->GetFaceName(), font->GetPointSize(), font->GetFamily());
        m_MadEdit->m_Syntax = m_Syntax;

        m_MadEdit->m_LoadingFile = false;

        if(m_Syntax->m_Encoding.IsEmpty())
            m_MadEdit->SetEncoding(defaultenc);
        else
            m_MadEdit->SetEncoding(m_Syntax->m_Encoding);

        return true;
    }

    // set size
    m_Size = m_FileData->m_Size;
    MadLineIterator iter = m_LineList.begin();
    iter->m_Size = m_Size;

    // set line's blocks
    iter->m_Blocks[0].m_Size = m_Size;

    // set line's row indices
    iter->m_RowIndices[1].m_Start = m_Size;

    long MaxSizeToLoad;
    m_MadEdit->m_Config->Read(wxT("/MadEdit/MaxSizeToLoad"), &MaxSizeToLoad, 20*1000*1000);

    wxMemorySize memsize=wxGetFreeMemory();

    wxByte *buf;
    if(m_Size <= MaxSizeToLoad && memsize>0 && (m_Size*2 + 15*1024*1024)<memsize)    // load filedata to MemData
    {
        buf = m_FileData->m_Buffer1;
        int ss = 0;
        int bs = BUFFER_SIZE;
        if(BUFFER_SIZE > m_Size)
        {
            bs = m_Size;
        }
        m_MemData->Put(buf, bs);
        ss += bs;

        while(ss < m_Size)
        {
        // now bs == BUFFER_SIZE;
        if(BUFFER_SIZE > (m_Size - ss))
        {
            bs = (m_Size - ss);
        }

        m_FileData->Get(ss, buf, bs);
        m_MemData->Put(buf, bs);

        ss += bs;
        }

        delete m_FileData;
        m_FileData = NULL;

        iter->m_Blocks[0].m_Data = m_MemData;
        buf = m_MemData->m_Buffers.front();
    }
    else
    {
        iter->m_Blocks[0].m_Data = m_FileData;
        buf = m_FileData->m_Buffer1;
    }


    // reload syntax
    delete m_Syntax;

    if(m_MadEdit->m_UseDefaultSyntax)
    {
        m_Syntax = new MadSyntax(false);
        if(m_MadEdit->m_SearchWholeWord==false)
        {
            m_Syntax->m_Delimiter.Empty();
        }
        m_Syntax->m_CaseSensitive = true;
    }
    else
    {
        wxFileName fn(filename);
        m_Syntax = MadSyntax::GetSyntaxByExt(fn.GetExt());
        if(m_Syntax==NULL)
        {
            m_Syntax = MadSyntax::GetSyntaxByFirstLine(buf, s);
            if(m_Syntax==NULL)
            {
                m_Syntax = MadSyntax::GetSyntaxByFileName(fn.GetName());
                if(m_Syntax==NULL)
                {
                    m_Syntax = MadSyntax::GetSyntaxByTitle(MadPlainTextTitle);
                }
            }
        }
    }

    wxFont *font=m_MadEdit->m_TextFont;
    m_Syntax->InitNextWord1(m_MadEdit->m_Lines, m_MadEdit->m_WordBuffer, m_MadEdit->m_WidthBuffer, font->GetFaceName(), font->GetPointSize(), font->GetFamily());
    m_MadEdit->m_Syntax = m_Syntax;


    long maxtextfilesize;
    wxString oldpath=m_MadEdit->m_Config->GetPath();
    m_MadEdit->m_Config->Read(wxT("/MadEdit/MaxTextFileSize"), &maxtextfilesize, 10*1000*1000);
    m_MadEdit->m_Config->SetPath(oldpath);

    bool ok=false;
    bool isbinary=false;

    if(m_Size>=maxtextfilesize)
    {
        isbinary=true;
    }
    else
    {
        if(IsTextUTF16LE(buf, s))
        {
            m_MadEdit->SetEncoding(wxT("utf-16le"));
            Reformat(iter, iter);
            ok=true;
        }
        else if(IsTextUTF16BE(buf, s))
        {
            m_MadEdit->SetEncoding(wxT("utf-16be"));
            Reformat(iter, iter);
            ok=true;
        }
        else if(IsTextUTF8(buf, s))
        {
            m_MadEdit->SetEncoding(wxT("utf-8"));
            Reformat(iter, iter);
            ok=true;
        }
        else if(IsTextUTF32LE(buf, s))
        {
            m_MadEdit->SetEncoding(wxT("utf-32le"));
            Reformat(iter, iter);
            ok=true;
        }
        else if(IsTextUTF32BE(buf, s))
        {
            m_MadEdit->SetEncoding(wxT("utf-32be"));
            Reformat(iter, iter);
            ok=true;
        }
    }

    if(!ok)
    {
        if(!m_Syntax->m_Encoding.IsEmpty())
        {
            m_MadEdit->SetEncoding(m_Syntax->m_Encoding);
        }
        else
        {
            wxFontEncoding enc=m_MadEdit->m_Encoding->GetEncoding();
            if( enc==wxFONTENCODING_UTF8 ||
                enc==wxFONTENCODING_UTF16LE ||
                enc==wxFONTENCODING_UTF16BE ||
                enc==wxFONTENCODING_UTF32LE ||
                enc==wxFONTENCODING_UTF32BE
                )
            {
                // use default encoding
                //enc=wxLocale::GetSystemEncoding();
                enc=MadEncoding::NameToEncoding(defaultenc);
            }

            /* old method to detect encoding
            bool detectchinese=false;
            bool detectjapanese=false;

            extern wxLocale g_Locale;
            wxString cname=g_Locale.GetCanonicalName();

            if(cname.Len()>=2)
            {
                if(cname[0]==wxT('z') && cname[1]==wxT('h'))
                {
                    detectchinese=true;
                }
                else if(cname[0]==wxT('j') && cname[1]==wxT('a'))
                {
                    detectjapanese=true;
                }
            }

            if(detectchinese || enc==wxFONTENCODING_CP936 || enc==wxFONTENCODING_CP950)
            {
                DetectChineseEncoding(buf, s, enc);
            }
            else if(detectjapanese || enc==wxFONTENCODING_CP932 || enc==wxFONTENCODING_EUC_JP)
            {
                DetectJapaneseEncoding(buf, s, enc);
            }
            */

            // use charset-detector
            DetectEncoding(buf, s, enc);

            m_MadEdit->SetEncoding(MadEncoding::EncodingToName(enc));
        }

        if(isbinary || IsBinaryData(buf, s))
        {
            m_MaxLineWidth = -1;       // indicate the data is not text data
            m_MadEdit->SetEditMode(emHexMode);
        }
        else
        {
            Reformat(iter, iter);
        }
    }

    m_MadEdit->m_LoadingFile = false;

    return true;
}

//===========================================================================

bool TruncateFile(const wxString &filename, wxFileOffset size)
{
#ifdef __WXMSW__

    HANDLE handle = CreateFile(filename.c_str(),        // file to open
                               GENERIC_WRITE,   // open for writing
                               0,       // do not share
                               NULL,    // default security
                               OPEN_ALWAYS,   // open the file, if not exist then create it
                               FILE_ATTRIBUTE_NORMAL,   // normal file
                               NULL);

    if(handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    LARGE_INTEGER oldsize;
    //if(GetFileSizeEx(handle, &oldsize)==FALSE)

    oldsize.LowPart = GetFileSize(handle, (DWORD *) & oldsize.HighPart);
    if(oldsize.LowPart==INVALID_FILE_SIZE && GetLastError()!=NO_ERROR)
    {
        CloseHandle(handle);
        return false;
    }

    if(size==oldsize.QuadPart) // same size, do nothing
    {
        CloseHandle(handle);
        return true;
    }

    if(size>oldsize.QuadPart)
    {
        /**
        if(SetFileValidData(handle, size)==FALSE)
        {
            CloseHandle(handle);
            return false;
        }
        **/

        wxFileOffset s=size-oldsize.QuadPart;

        DWORD ret=SetFilePointer(handle, 0, NULL, FILE_END);
        if(ret==INVALID_SET_FILE_POINTER && GetLastError()!=NO_ERROR)
        {
            CloseHandle(handle);
            return false;
        }

        static char *buf=NULL;
        static vector<char> buffervector;
        if(buf==NULL)
        {
            buffervector.resize(256*1024);
            buf=& (*buffervector.begin());
            ZeroMemory(buf, 256*1024);
        }

        DWORD len=256*1024, wlen;

        do
        {
            if(len>s) len=s;
            WriteFile(handle, buf, len, &wlen, NULL);
        }
        while((s-=len)>0);

    }
    else    // less
    {
        oldsize.QuadPart=size;
        DWORD ret=SetFilePointer(handle, oldsize.LowPart, &oldsize.HighPart, FILE_BEGIN);
        if(ret==INVALID_SET_FILE_POINTER && GetLastError()!=NO_ERROR)
        {
            CloseHandle(handle);
            return false;
        }
    }

    bool ok= (SetEndOfFile(handle)!=FALSE);
    CloseHandle(handle);
    return ok;

#else

    int utf8test=MadFileNameIsUTF8(filename);
    MadConvFileName_WC2MB_UseLibc uselibc(utf8test<0);
    if(utf8test==0)
    {
        // create a new file
        wxFile f(filename, wxFile::write);
    }

    return 0==truncate( wxConvFile.cWX2MB(filename.c_str()), size );

#endif
}

void MadLines::WriteBlockToData(MadOutData *data, const MadBlockIterator &bit)
{
    wxASSERT(data!=NULL);

    wxDword bs = BUFFER_SIZE;
    wxFileOffset size = bit->m_Size;
    wxFileOffset pos = bit->m_Pos;

    do
    {
        if(BUFFER_SIZE > size)
            bs = size;

        bit->m_Data->Get(pos, m_WriteBuffer, bs);
        pos += bs;

        data->Put(m_WriteBuffer, bs);
    }
    while((size -= bs) > 0);
}

void MadLines::WriteToFile(wxFile &file, MadFileData *oldfd, MadFileData *newfd)
{
    if(m_Size > 0)
    {
        if(m_WriteBuffer == NULL)
        {
            m_WriteBufferVector.resize(BUFFER_SIZE);
            m_WriteBuffer = & (*m_WriteBufferVector.begin());
        }

        MadLineIterator lit = m_LineList.begin();
        wxFileOffset filepos=0;

        do
        {
            if(lit->m_Size != 0)
            {
                MadBlockIterator bit=lit->m_Blocks.begin();
                size_t count=lit->m_Blocks.size();
                do
                {
                    wxDword bs = BUFFER_SIZE;
                    wxFileOffset size = bit->m_Size;
                    wxFileOffset pos = bit->m_Pos;

                    do
                    {
                        if(BUFFER_SIZE > size)
                            bs = size;

                        bit->m_Data->Get(pos, m_WriteBuffer, bs);
                        pos += bs;

                        file.Write(m_WriteBuffer, bs);
                    }
                    while((size -= bs) > 0);

                    if(bit->m_Data == oldfd)
                    {
                        bit->m_Data = newfd;
                        bit->m_Pos = filepos;
                    }
                    filepos+=bit->m_Size;

                    ++bit;
                }
                while(--count != 0);
            }
        }
        while(++lit != m_LineList.end());
    }
}

wxFileOffset MadLines::GetMaxTempSize(const wxString &filename)
{
    if(m_FileData==NULL || filename!=m_Name || m_Size==0)
        return 0;

    wxFileOffset maxsize=0, temppos=0, tempsize=0, filepos=0;
    wxFileOffset writable_size=0, writepos=0;

    MadLineIterator write_lit;
    MadBlockIterator write_bit, write_bitend;

    MadLineIterator file_lit;
    MadBlockIterator file_bit, file_bitend;

    bool file_lit_not_set=true, ignore_overlap=false;

    write_lit=m_LineList.begin();
    write_bit=write_lit->m_Blocks.begin();
    write_bitend=write_lit->m_Blocks.end();

    do  // per block
    {
        // find the block that must write back to file
        while(writable_size==0 && write_bit->m_Data == m_FileData &&
              write_bit->m_Pos  == writepos )
        {
            // this block is not necessary to write back
            writepos+=write_bit->m_Size;
            file_lit_not_set=true;

            if(++write_bit == write_bitend)
            {
                if(writepos==m_Size) // end of file
                    break;

                wxASSERT(writepos<m_Size);

                ++write_lit;
                write_bit=write_lit->m_Blocks.begin();
                write_bitend=write_lit->m_Blocks.end();
            }
        }

        if(writepos==m_Size) // end of file
            break;

        // make sure that writable_size >= write_bit->m_Size
        if(writable_size < write_bit->m_Size)
        {
            if(file_lit_not_set)
            {
                file_lit=write_lit;
                file_bit=write_bit;
                file_bitend=write_bitend;
                file_lit_not_set=false;
                ignore_overlap=false;
            }

            do
            {
                if(file_bit->m_Data==m_FileData)
                {
                    filepos=file_bit->m_Pos;

                    if(ignore_overlap==false)
                    {
                        ignore_overlap=true;

                        wxFileOffset overlap_size= file_bit->m_Pos - (writepos+writable_size);
                        if( (writable_size+=overlap_size) >= write_bit->m_Size )
                        {
                            break;
                        }
                    }

                    // move the block to tempdata

                    file_bit->m_Data=NULL; // temp data

                    tempsize+=file_bit->m_Size;
                    writable_size+=file_bit->m_Size;

                    if(tempsize>maxsize)
                    {
                        maxsize=tempsize;
                    }

                }

                ignore_overlap=false;

                // to next block
                if(++file_bit==file_bitend)
                {
                    if(++file_lit != m_LineList.end() && file_lit->m_Size!=0)
                    {
                        file_bit=file_lit->m_Blocks.begin();
                        file_bitend=file_lit->m_Blocks.end();
                    }
                    else // at end of file
                    {
                        writable_size+=m_Size-(writepos+writable_size);
                        wxASSERT(writable_size >= write_bit->m_Size);
                    }

                }

            }
            while(writable_size < write_bit->m_Size);
        }

        // write the block to file

        if(write_bit->m_Data==m_FileData)
        {
            if(file_lit_not_set==false && write_bit->m_Pos >= filepos)
            {
                file_lit_not_set=true;
            }
        }
        else if(write_bit->m_Data==NULL)  // this block is in the tempdata
        {
            write_bit->m_Data=m_FileData;

            temppos+=write_bit->m_Size;
            tempsize-=write_bit->m_Size;

            wxASSERT(tempsize>=0);

            if(tempsize==0)
            {
                temppos=0;
            }
        }

        writepos+=write_bit->m_Size;
        writable_size-=write_bit->m_Size;

        if(file_lit_not_set==false && writable_size==0 && tempsize==0)
        {
            file_lit_not_set=true;
        }

        // to next block
        if(++write_bit == write_bitend)
        {
            if(writepos==m_Size) // end of file
                break;

            wxASSERT(writepos<m_Size);

            ++write_lit;
            write_bit=write_lit->m_Blocks.begin();
            write_bitend=write_lit->m_Blocks.end();
        }

    }
    while(writepos<m_Size);

    wxASSERT(tempsize==0 && writable_size>=0);

    return maxsize;
}

bool MadLines::SaveToFile(const wxString &filename, const wxString &tempdir)
{
    if(m_FileData == NULL)
    {
        int utf8test=MadFileNameIsUTF8(filename);
#ifndef __WXMSW__
        if(utf8test==0) // not found
        {
            // test parent dir
            wxFileName fn(filename);
            utf8test=MadDirExists(fn.GetPath(wxPATH_GET_VOLUME));
        }
#endif
        MadConvFileName_WC2MB_UseLibc uselibc(utf8test<0);

        wxFile file(filename, wxFile::write);
        if(!file.IsOpened())
        {
            return false;   // open error;
        }

        wxFileOffset filesize=file.SeekEnd(0);

        file.Seek(0);
        WriteToFile(file, NULL, NULL);
        file.Close();

        if(filesize>m_Size)
        {
            TruncateFile(filename, m_Size);
        }

        if(filename != m_Name)
        {
            m_Name = filename;
            m_ReadOnly = false;
        }

        return true;
    }

    if(filename != m_Name) // save to another file
    {
        int utf8test=MadFileNameIsUTF8(filename);
        bool createnew=false;
        if(utf8test==0) // not found
        {
#ifndef __WXMSW__
            // test parent dir
            wxFileName fn(filename);
            utf8test=MadDirExists(fn.GetPath(wxPATH_GET_VOLUME));
#endif
            createnew=true;
        }
        MadConvFileName_WC2MB_UseLibc uselibc(utf8test<0);

        if(createnew)
        {
            // create it
            wxFile file(filename, wxFile::write);
        }

        MadFileData *fd = new MadFileData(filename);

        if(!fd->OpenSuccess() || fd->IsReadOnly())
        {
            delete fd;
            return false;
        }

        wxFileOffset filesize=fd->m_File.SeekEnd(0);

        fd->m_File.Seek(0);
        WriteToFile(fd->m_File, m_FileData, fd);    // replace old fd by new fd

        if(filesize>m_Size)
        {
            fd->m_File.Close();
            TruncateFile(filename, m_Size); // truncate the file
            fd->OpenFile();                 // reopen the file
        }

        fd->m_Size=m_Size;
        fd->m_Buf1Pos = -1;
        fd->m_Buf2Pos = -1;

        delete m_FileData;
        m_FileData=fd;

        m_Name=filename;
        m_ReadOnly=false;

        return true;
    }


    // write back to the same file

    if(m_ReadOnly) return false;

    if(m_Size==0)
    {
        if(m_FileData->m_File.SeekEnd(0)!=0)
        {
            m_FileData->m_File.Close();
            TruncateFile(filename, m_Size); // truncate the file
            m_FileData->OpenFile();         // reopen the file
            m_FileData->m_Buf1Pos = -1;
            m_FileData->m_Buf2Pos = -1;
        }
        return true;
    }

    if(m_WriteBuffer == NULL)
    {
        m_WriteBufferVector.resize(BUFFER_SIZE);
        m_WriteBuffer = & (*m_WriteBufferVector.begin());
    }

    MadConvFileName_WC2MB_UseLibc uselibc(MadFileNameIsUTF8(filename)<0);

    wxString tempfilename;

    if(!tempdir.IsEmpty())
    {
        wxFileName fn(filename);
        tempfilename=tempdir+fn.GetFullName()+wxT(".MadEdit.tmp");

        int id=0;
        while(wxFileExists(tempfilename + wxString::Format(wxT("%d"), id))) ++id;
        tempfilename<<id;   // append "id"
    }

    MadFileData *tempfiledata=NULL;
    MadMemData *tempmemdata=NULL;
    MadInData *tempindata=NULL;
    MadOutData *tempoutdata=NULL;

#ifdef _DEBUG
    wxFileOffset maxsize=0;
#endif

    wxFileOffset temppos=0, tempsize=0, filepos=0;
    wxFileOffset writable_size=0, writepos=0;

    MadLineIterator write_lit;
    MadBlockIterator write_bit, write_bitend;

    MadLineIterator file_lit;
    MadBlockIterator file_bit, file_bitend;

    bool file_lit_not_set=true, ignore_overlap=false;

    write_lit=m_LineList.begin();
    write_bit=write_lit->m_Blocks.begin();
    write_bitend=write_lit->m_Blocks.end();

    do  // per block
    {
        // find the block that must write back to file
        while(writable_size==0 && write_bit->m_Data == m_FileData &&
              write_bit->m_Pos  == writepos )
        {
            // this block is not necessary for writing back
            writepos+=write_bit->m_Size;
            file_lit_not_set=true;

            if(++write_bit == write_bitend)
            {
                if(writepos==m_Size) // end of file
                    break;

                wxASSERT(writepos<m_Size);

                ++write_lit;
                write_bit=write_lit->m_Blocks.begin();
                write_bitend=write_lit->m_Blocks.end();
            }
        }

        if(writepos==m_Size) // end of file
            break;

        // make sure that writable_size >= write_bit->m_Size
        if(writable_size < write_bit->m_Size)
        {
            if(file_lit_not_set)
            {
                file_lit=write_lit;
                file_bit=write_bit;
                file_bitend=write_bitend;
                file_lit_not_set=false;
                ignore_overlap=false;
            }

            do
            {
                if(file_bit->m_Data==m_FileData)
                {
                    filepos=file_bit->m_Pos;

                    if(ignore_overlap==false)
                    {
                        ignore_overlap=true;

                        wxFileOffset overlap_size= file_bit->m_Pos - (writepos+writable_size);
                        if( (writable_size+=overlap_size) >= write_bit->m_Size )
                        {
                            break;
                        }
                    }

                    // move the block to tempdata
                    if(tempoutdata==NULL)
                    {
                        if(tempfilename.IsEmpty())
                        {
                            tempmemdata=new MadMemData();

                            tempoutdata=tempmemdata;
                            tempindata=tempmemdata;
                        }
                        else
                        {
                            wxFile tf(tempfilename, wxFile::write);
                            tf.Close();
                            tempfiledata=new MadFileData(tempfilename);

                            tempoutdata=tempfiledata;
                            tempindata=tempfiledata;
                        }
                    }

                    wxFileOffset pos0=temppos+tempsize;

                    if(tempfiledata!=NULL)
                    {
                        tempfiledata->m_SavePos=pos0;
                    }
                    else
                    {
                        wxASSERT(pos0 <= tempmemdata->m_Buffers.size()*BUFFER_SIZE);
                        tempmemdata->m_Size=pos0;
                    }
                    WriteBlockToData(tempoutdata, file_bit);

                    file_bit->m_Data=tempindata;
                    file_bit->m_Pos=pos0;


                    tempsize+=file_bit->m_Size;
                    writable_size+=file_bit->m_Size;

#ifdef _DEBUG
                    if(tempsize>maxsize)
                    {
                        maxsize=tempsize;
                    }
#endif
                }

                ignore_overlap=false;

                // to next block
                if(++file_bit==file_bitend)
                {
                    if(++file_lit != m_LineList.end() && file_lit->m_Size!=0)
                    {
                        file_bit=file_lit->m_Blocks.begin();
                        file_bitend=file_lit->m_Blocks.end();
                    }
                    else // at end of file
                    {
                        writable_size+=m_Size-(writepos+writable_size);
                        wxASSERT(writable_size >= write_bit->m_Size);
                    }

                }

            }
            while(writable_size < write_bit->m_Size);
        }

        // write the block to file
        m_FileData->m_SavePos=writepos;
        WriteBlockToData(m_FileData, write_bit);

        if(write_bit->m_Data==m_FileData)
        {
            if(file_lit_not_set==false && write_bit->m_Pos >= filepos)
            {
                file_lit_not_set=true;
            }

            write_bit->m_Pos=writepos;
        }
        else if(write_bit->m_Data==tempindata) // this block is in the tempdata
        {
            write_bit->m_Data=m_FileData;
            write_bit->m_Pos=writepos;


            temppos+=write_bit->m_Size;
            tempsize-=write_bit->m_Size;

            wxASSERT(tempsize>=0);

            if(tempsize==0)
            {
                temppos=0;

                if(tempmemdata!=NULL)
                {
                    tempmemdata->Reset();
                }
            }
        }

        writepos+=write_bit->m_Size;
        writable_size-=write_bit->m_Size;

        if(file_lit_not_set==false && writable_size==0 && tempsize==0)
        {
            file_lit_not_set=true;
        }

        // to next block
        if(++write_bit == write_bitend)
        {
            if(writepos==m_Size) // end of file
                break;

            wxASSERT(writepos<m_Size);

            ++write_lit;
            write_bit=write_lit->m_Blocks.begin();
            write_bitend=write_lit->m_Blocks.end();
        }

    }
    while(writepos<m_Size);

    wxASSERT(tempsize==0 && writable_size>=0);

//#ifdef _DEBUG
//    maxsize += 0;
//#endif

    if(m_FileData->m_Size > m_Size)
    {
        m_FileData->m_File.Close();
        TruncateFile(filename, m_Size);     // truncate the file
        m_FileData->OpenFile();             // reopen the file
    }

    m_FileData->m_Size=m_Size;
    m_FileData->m_Buf1Pos=-1;
    m_FileData->m_Buf2Pos=-1;

    if(tempmemdata!=NULL)
    {
        delete tempmemdata;
    }
    else if(tempfiledata!=NULL)
    {
        delete tempfiledata;
        wxRemoveFile(tempfilename);
    }

    return true;
}

