//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */
//=--------------------------------------------------------------------------=
// Definitions for all public IID's needed by an AAF SDK Plugin author
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

const IID IID_IAAFPlugin = {0x3631F7A4,0x9121,0x11d2,{0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f}};


const IID IID_IAAFClassExtension = {0x65C07344,0xDF36,0x11d3,{0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d}};


const IID IID_IAAFEssenceStream = {0x83402902,0x9146,0x11d2,{0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f}};


const IID IID_IAAFEssenceDataStream = {0xCDDB6AB1,0x98DC,0x11d2,{0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f}};


const IID IID_IAAFEssenceCodec = {0x3631F7A2,0x9121,0x11d2,{0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f}};


const IID IID_IAAFMultiEssenceCodec = {0x7F93992D,0xDDA3,0x11d3,{0x80,0x08,0x00,0x10,0x4b,0xc9,0x15,0x6d}};


const IID IID_IAAFEssenceContainer = {0xa7337031,0xc103,0x11d2,{0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f}};


const IID IID_IAAFInterpolator = {0x75C6CDF2,0x0D67,0x11d3,{0x80,0xA9,0x00,0x60,0x08,0x14,0x3e,0x6f}};


#ifdef __cplusplus
}
#endif

