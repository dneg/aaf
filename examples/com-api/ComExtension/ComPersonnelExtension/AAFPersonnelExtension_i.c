/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */
//=--------------------------------------------------------------------------=
// (C) Copyright 1998-1999 Avid Technology.
//
// This file was created for the AAF SDK on 
//  Fri Nov 19 18:16:30 EDT 1999
//
// Permission to use, copy and modify this software and accompanying 
// documentation, and to distribute and sublicense application software 
// incorporating this software for any purpose is hereby granted, 
// provided that (i) the above copyright notice and this permission 
// notice appear in all copies of the software and related documentation, 
// and (ii) the name Avid Technology, Inc. may not be used in any 
// advertising or publicity relating to the software without the specific, 
// prior written permission of Avid Technology, Inc. 
//
// THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. 
// IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT, 
// SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR 
// OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF 
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND 
// ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES 
// RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT 
// ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF 
// LIABILITY. 
//=--------------------------------------------------------------------------=
// Definitions for all public IID's needed by an AAF SDK Plugin author
// and client.
//=--------------------------------------------------------------------------=
//

#ifdef __cplusplus
extern "C"{
#endif


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IAAFPersonnelResource = {0x7AFD6B0A,0x9ED7,0x11D3,{0xBF,0xF9,0x00,0x10,0x4B,0xC9,0x15,0x6D}};


const IID IID_IAAFAdminMob = {0x7AFD6B0B,0x9ED7,0x11D3,{0xBF,0xF9,0x00,0x10,0x4B,0xC9,0x15,0x6D}};


const CLSID CLSID_AAFPersonnelResource = {0x7AFD6B0C,0x9ED7,0x11D3,{0xBF,0xF9,0x00,0x10,0x4B,0xC9,0x15,0x6D}};


const CLSID CLSID_AAFAdminMob = {0x7AFD6B0D,0x9ED7,0x11D3,{0xBF,0xF9,0x00,0x10,0x4B,0xC9,0x15,0x6D}};


#ifdef __cplusplus
}
#endif

