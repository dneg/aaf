//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=--
#ifndef __UTF8_h__
#define __UTF8_h__

#include <wchar.h>

// UTF8 versions of mbs/wcs libc functions
// (because MSFT and GNU take different approaches from each other)

#ifndef MG_UTF8
#define MG_UTF8 1
#endif

#if defined(MG_UTF8)

// convert a wchar_t to a utf8 (multibyte)char
// follows signature of int wctomb( char *m, wchar_t w )
// requires m to point to a buffer of length 4 or more
int wctou8( char *m, wchar_t w );

// number of char required to represent a single wchar_t in utf8
// follows signature of mblen( const wchar_t, 1 )
int wcu8len( const wchar_t w );

// number of char required to represent a wchar_t string in utf8
// follows signature of wcslen( wchar_t* ) or wcstombs( NULL, wchar_t*, )
int wcsu8slen( const wchar_t *pw );

// convert a wchar_t string to utf8 string
// follows signature of size_t wcstombs( char *u, const wchar_t *w, size_t count )
size_t wcstou8s( char *pu, const wchar_t *pw, size_t count );

// length of a utf8 (multibyte)char
// follows signature of mblen( const char *, size_t count ) but always behaves as if count==1
// does not check validity of trailing bytes (u[1]&0xc0)==0x80
int u8len( const char *u, size_t count );

// convert utf8 (multibyte)char to wchar_t
// follows signature of mbtowc( wchar_t *, const char *, size_t count ) but always behaves as if count==1
int u8towc( wchar_t *w, const char *u, size_t count );

// number of wchar_t required to represent a utf8 string
// follows signature of mbstowcs( NULL, char*, )
int u8swcslen( const char* pu );

// convert a utf8 string to a wchar_t string
// follows signature of size_t mbstowcs( wchar_t *, const char *, size_t count )
size_t u8stowcs( wchar_t *pw, const char *pu, size_t count );

#else

#define wctou8( A, B ) wctomb( A, B )
#define wcu8len( A ) mblen( A, 1 )
#define wcsu8slen( A ) wcstombs( NULL, A, 1 )
#define wcstou8s( A, B, C ) wcstombs( A, B, C )
#define u8len( A, B ) mblen( A, B )
#define u8towc( A, B, C ) mbtowc( A, B, C )
#define u8swcslen( A ) mbstowcs( NULL, A, 1 )
#define u8stowcs( A, B, C ) mbstowcs( A, B, C )

#endif





#endif // __UTF8_h__


