///////////////////////////////////////////////////////////////////////////////
// Name:        MadEdit/ucs4.h
// Description: declare UCS4/UCS2 wide-char type
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _UCS4_T_H_
#define _UCS4_T_H_

#include <wx/defs.h>

// ucs4_t is Little-Endian under x86 Linux & MSWindows

#if defined(__WXMSW__)
    typedef wxInt32     ucs4_t;
    typedef wchar_t     ucs2_t;
#else    // __WXGTK__ __WXMAC__
    typedef wchar_t     ucs4_t;
    typedef wxUint16    ucs2_t;
#endif

wxCOMPILE_TIME_ASSERT( sizeof(ucs4_t)==4 , SizeofUCS4IsNot4Bytes );
wxCOMPILE_TIME_ASSERT( sizeof(ucs2_t)==2 , SizeofUCS2IsNot2Bytes );

#endif
