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

/*
#ifdef __WATCOMC__

#include <string>

namespace std {
  template< >
  struct char_traits< ucs4_t > {
    typedef ucs4_t   char_type;
    typedef wint_t    int_type;
    typedef mbstate_t state_type;

    static void assign( char_type &c1, const char_type &c2 )
      { c1 = c2; }

    static bool eq( const char_type &c1, const char_type &c2 )
      { return( c1 == c2 ); }

    static bool lt( const char_type &c1, const char_type &c2 )
      { return( c1 < c2 ); }

    static int compare( const char_type *s1, const char_type *s2, size_t n )
    {
      for( size_t i = 0; i < n; ++i ) {
        if( *s1 < *s2 ) return( -1 );
        if( *s1 > *s2 ) return(  1 );
        ++s1; ++s2;
      }
      return( 0 );
    }

    static size_t length( const char_type *s )
    {
        size_t len=0;
        while(*s != 0)
        {
            ++len;
            ++s;
        }
        return len;
    }

    static const char_type *find( const char_type *s, size_t n, const char_type &a )
    {
      const char_type *result = 0;
      for( size_t i = 0; i < n; ++i ) {
        if( *s == a ) {
          result = s;
          break;
        }
        ++s;
      }
      return( result );
    }

    static char_type *move( char_type *s1, const char_type *s2, size_t n )
    {
      return( static_cast< char_type * >
              ( memmove(s1, s2, n * sizeof( char_type ) ) ) );
    }

    static char_type *copy( char_type *s1, const char_type *s2, size_t n )
    {
      return( static_cast< char_type * >
              ( memcpy(s1, s2, n * sizeof( char_type ) ) ) );
    }

    static char_type *assign( char_type *s, size_t n, char_type a )
    {
      char_type *p = s;
      for( size_t i = 0; i < n; ++i ) {
        *p = a;
        ++p;
      }
      return( s );
    }

    static int_type not_eof( const int_type &c )
      { return( (c != WEOF) ? c : static_cast< int_type >( 0 ) ); }

    static char_type to_char_type( const int_type &c )
      { return( static_cast< char_type >(c) ); }

    static int_type to_int_type( const char_type &c )
      { return( static_cast< int_type >(c) ); }

    static bool eq_int_type( const int_type &c1, const int_type &c2 )
      { return( c1 == c2 ); }

    static int_type eof( )
      { return( WEOF ); }
  }; // struct char_traits< ucs4_t >

} // namespace std
#endif
*/

#endif
