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
#include "utf8.h"


// convert a wchar_t to a utf8 (multibyte)char
// follows signature of int wctomb( char *m, wchar_t w )
// requires m to point to a buffer of length 4 or more
int wctou8( char *m, wchar_t w )
{
/* Unicode Table 3-5. UTF-8 Bit Distribution
Unicode                     1st Byte 2nd Byte 3rd Byte 4th Byte
00000000 0xxxxxxx           0xxxxxxx
00000yyy yyxxxxxx           110yyyyy 10xxxxxx
zzzzyyyy yyxxxxxx           1110zzzz 10yyyyyy 10xxxxxx
000uuuuu zzzzyyyy yyxxxxxx  11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
*/

	if( !(w&~0x7f) ) { m[0]=w&0x7f;
										 m[1]='\0';
										 return 1; }
	else if( !(w&~0x7ff) ) { m[0]=((w>>6)&0x1f)|0xc0;
													 m[1]=(w&0x3f)|0x80;
													 m[2]='\0';
													 return 2; }
	else if( !(w&~0xffff) ) { m[0]=((w>>12)&0x0f)|0xe0;
														m[1]=((w>>6)&0x3f)|0x80;
														m[2]=(w&0x3f)|0x80;
														m[3]='\0';
														return 3; }
	else if( !(w&~0x1fffff) ) { m[0]=((w>>18)&0x07)|0xf0;
															m[1]=((w>>12)&0x3f)|0x80;
															m[2]=((w>>6)&0x3f)|0x80;
															m[3]=(w&0x3f)|0x80;
															m[4]='\0';
															return 4; }
	else return -1;
}

// number of char required to represent a single wchar_t in utf8
// follows signature of mblen( const wchar_t )
int wcu8len( const wchar_t w )
{
	if( !(w&~0x7f) ) return 1;
	if( !(w&~0x7ff) ) return 2;
	if( !(w&~0xffff) ) return 3;
	if( !(w&~0x1fffff) ) return 4;
	return -1; /* error */
}

// number of char required to represent a wchar_t string in utf8
// follows signature of wcslen( wchar_t* ) or wcstombs( NULL, wchar_t*, )
int wcsu8slen( const wchar_t *pw )
{
	int len=0;
	wchar_t w;

	while( (w = *pw++) )
	{
		if( !(w&~0x7f) ) len+=1;
		else if( !(w&~0x7ff) ) len+=2;
		else if( !(w&~0xffff) ) len+=3;
		else if( !(w&~0x1fffff) ) len+=4;
		else /* error: add width of null character entity &#x00; */ len+=6;
	}
	return len;
}

// convert a wchar_t string to utf8 string
// follows signature of size_t wcstombs( char *u, const wchar_t *w, size_t count )
size_t wcstou8s( char *pu, const wchar_t *pw, size_t count )
{
	int len=wcsu8slen( pw );

	if( NULL==pu ) return (size_t)len;

	size_t clen=0;
	wchar_t w;
	while( (w = *pw++) )
	{
		int ulen=wcu8len(w);
		
		if( ulen>=0 )
		{
			if( (clen+wcu8len(w))<=count ) { clen+=wctou8( pu, w ); pu+=ulen; }
			else break;
		}
		else
		{
			// untranslatable character so insert null character entity &#x00;
			if( (clen+6)<=count )
			{
				*pu++='&'; *pu++='#'; *pu++='x';
				*pu++='0'; *pu++='0';
				*pu++=';';
			}
			else break;
		}
	}

	return (size_t)clen;
}

// length of a utf8 (multibyte)char
// follows signature of mblen( const char *, size_t count ) but always behaves as if count==1
// does not check validity of trailing bytes (u[1]&0xc0)==0x80
int u8len( const char *u, size_t count )
{
	if( 0==count ) return 0;

	if( NULL==u ) return 0;
	else if( 0==*u ) return 0;
	else if( !(*u&~0x7f) ) return 1;
	else if( (*u&0xe0)==0xc0 ) return 2;
	else if( (*u&0xf0)==0xe0 ) return 3;
	else if( (*u&0xf8)==0xf0 ) return 4;
	else /* error */ return -1;
}

// convert utf8 (multibyte)char to wchar_t
// follows signature of mbtowc( wchar_t *, const char *, size_t count ) but always behaves as if count==1
int u8towc( wchar_t *w, const char *u, size_t count )
{
	/* assert */ if( NULL==w ) return -1;

	int len=u8len( u,1 );

	if( len<1 ) return len;
	else if( 1==len ) { w[0]=u[0]&0x7f; return len; }
	else if( 2==len ) { if( (u[1]&0xc0)!=0x80 ) /* error */ return -1;
											w[0]=((u[0]&0x1f)<<6)|(u[1]&0x3f);
											return 2; }
	else if( 3==len ) { if( (u[1]&0xc0)!=0x80 ) /* error */ return -1;
	                    if( (u[2]&0xc0)!=0x80 ) /* error */ return -1;
											w[0]=((u[0]&0x0f)<<12)|((u[1]&0x3f)<<6)|(u[2]&0x3f);
	                    return 3; }
	else if( 4==len ) { if( (u[1]&0xc0)!=0x80 ) /* error */ return -1;
	                    if( (u[2]&0xc0)!=0x80 ) /* error */ return -1;
	                    if( (u[3]&0xc0)!=0x80 ) /* error */ return -1;
											w[0]=((u[0]&0x07)<<18)|((u[1]&0x3f)<<12)|((u[2]&0x3f)<<6)|(u[3]&0x3f);
	                    return 4; }
	else /* error */ return -1;
}

// number of wchar_t required to represent a utf8 string
// follows signature of mbstowcs( NULL, char*, )
int u8swcslen( const char* pu )
{
	int len=0;
	char c;

	while( (c = *pu) )
	{
		if( !(c&0x80) ) { len++; pu+=1; }
		else if( (c&0xe0)==0xc0 ) { len++; pu+=2; }
		else if( (c&0xf0)==0xe0 ) { len++; pu+=3; }
		else if( (c&0xf8)==0xf0 ) { len++; pu+=4; }
		else /* error: add width of single byte character entity &#xFF; */ { len+=6; pu+=1; }
	}
	return len;
}

// convert a utf8 string to a wchar_t string
// follows signature of size_t mbstowcs( wchar_t *, const char *, size_t count )
size_t u8stowcs( wchar_t *pw, const char *pu, size_t count )
{
	size_t clen=0;

	if( NULL==pw ) return u8swcslen( pu );

	while( *pu && clen<count )
	{
		int ulen=u8towc( &pw[clen], pu, 1 );
		if( ulen<0 ) return (size_t)-1;
		else { clen++; pu+=ulen; }
	}
	if( '\0'==*pu && clen<count) pw[clen++]=L'\0';
	return clen;
}
