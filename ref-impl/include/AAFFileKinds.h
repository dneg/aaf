#ifndef __AAFFileKinds_h__
#define __AAFFileKinds_h__
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

// the enum to select DON'T CARE (whatever works)
#define kAAFFileKind_DontCare_Value \
{0,0,0,{0,0,0,0,0,0,0,0}};
const aafUID_t kAAFFileKind_DontCare = kAAFFileKind_DontCare_Value;

// to cause error if used
#define kAAFFileKind_Pathological_Value \
{0xff,0xff,0xff,{0,0,0,0,0,0,0,0}};
const aafUID_t kAAFFileKind_Pathological = kAAFFileKind_Pathological_Value;

// the enum to select the Microsoft implementation with 512 byte sectors
#define kAAFFileKind_AafM512Binary_Value \
{ 0xc95e8ee6, 0xa6ec, 0x4e53, { 0x92, 0x28, 0xbd, 0x9b, 0x57, 0x23, 0x57, 0xe5 } }
const aafUID_t kAAFFileKind_AafM512Binary = kAAFFileKind_AafM512Binary_Value;

// the enum to select the SchemaSoft implementation with 512 byte sectors
#define kAAFFileKind_AafS512Binary_Value \
{ 0xbb153a22, 0xc2ed, 0x4b2e, { 0xbb, 0x69, 0x19, 0xbd, 0x58, 0x9d, 0xf6, 0xdc } }
const aafUID_t kAAFFileKind_AafS512Binary = kAAFFileKind_AafS512Binary_Value;

// the enum to select the GSF implementation with 512 byte sectors
#define kAAFFileKind_AafG512Binary_Value \
{ 0xb965c7f1, 0xf89d, 0x4490, { 0xbd, 0x22, 0x77, 0x35, 0x69, 0xb4, 0xd3, 0x61 } }
const aafUID_t kAAFFileKind_AafG512Binary = kAAFFileKind_AafG512Binary_Value;

// the enum to select the Microsoft implementation with 4096 byte sectors
#define kAAFFileKind_AafM4KBinary_Value \
{ 0x7653a218, 0x3e03, 0x4ecf, { 0x87, 0x98, 0xf4, 0x5f, 0xc1, 0x17, 0x11, 0x78 } }
const aafUID_t kAAFFileKind_AafM4KBinary = kAAFFileKind_AafM4KBinary_Value;

// the enum to select the SchemaSoft implementation with 4096 byte sectors
#define kAAFFileKind_AafS4KBinary_Value \
{ 0xa8ab424a, 0xc5a0, 0x48d0, { 0x9e, 0xea, 0x96, 0x69, 0x69, 0x75, 0xc6, 0xd0 } }
const aafUID_t kAAFFileKind_AafS4KBinary = kAAFFileKind_AafS4KBinary_Value;

// the enum to select the GSF implementation with 4096 byte sectors
#define kAAFFileKind_AafG4KBinary_Value \
{ 0xb44818b, 0xc3dd, 0x4f0a, { 0xad, 0x37, 0xe9, 0x71, 0x0, 0x7a, 0x88, 0xe8 } }
const aafUID_t kAAFFileKind_AafG4KBinary = kAAFFileKind_AafG4KBinary_Value;

// AAF files encoded as XML (text).
//
// used in prototype only (TEST/ComFileKindTest)
#define kAAFFileKind_AafXmlText_Value \
{ 0xc59ef56f, 0x5cb4, 0x4cb8, { 0xbd, 0xe3, 0x8d, 0x7c, 0xd9, 0x25, 0x8e, 0x18 } }
const aafUID_t kAAFFileKind_AafXmlText = kAAFFileKind_AafXmlText_Value;


// MXF files encoded as SMPTE KLV (binary).
//
// not yet in use. this value is incorrect, see SMPTE 377M
#define kAAFFileKind_MxfKlvBinary_Value \
{ 0xfc88c800, 0x879d, 0x48a3, { 0x90, 0x53, 0xee, 0x33, 0x5e, 0x87, 0xe0, 0xc5 } }
const aafUID_t kAAFFileKind_MxfKlvBinary = kAAFFileKind_MxfKlvBinary_Value;


// the enum to select the installation default
// this MUST match what is selected in ImplAAFFile.cpp
#if defined( OS_WINDOWS )
// DEFAULT is Schemasoft (via libSSRW2C.a).
const aafUID_t kAAFFileKind_Aaf512Binary = kAAFFileKind_AafS512Binary_Value;
const aafUID_t kAAFFileKind_Aaf4KBinary = kAAFFileKind_AafS4KBinary_Value;

#elif defined( OS_UNIX )

#ifdef USE_LIBGSF
// When LIBGSF is requested make it the default for 512 and 4k
const aafUID_t kAAFFileKind_Aaf512Binary = kAAFFileKind_AafG512Binary_Value;
const aafUID_t kAAFFileKind_Aaf4KBinary = kAAFFileKind_AafG4KBinary_Value;
#else
// Otherwise default is Schemasoft (via libSSRW2C.a)
const aafUID_t kAAFFileKind_Aaf512Binary = kAAFFileKind_AafS512Binary_Value;
const aafUID_t kAAFFileKind_Aaf4KBinary = kAAFFileKind_AafS4KBinary_Value;
#endif

#else
#error Unknown platform
#endif




// the following signatures could be factored into a separate file

// AAF files encoded as structured storage (binary).
//
// the signature actually stored in all AAF SS (512) files
// note this is not a properly-formed SMPTE label, but this is legacy
#define kAAFSignature_Aaf512Binary_Value \
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};
const aafUID_t kAAFSignature_Aaf512Binary = kAAFSignature_Aaf512Binary_Value; 


// the signature actually stored in all AAF SS (4096) files
// [060e2b34.0302.0101.0d010201.02000000]
#define kAAFSignature_Aaf4KBinary_Value \
{0x0d010201, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x03, 0x02, 0x01, 0x01}};
const aafUID_t kAAFSignature_Aaf4KBinary = kAAFSignature_Aaf4KBinary_Value;


// Deprecated symbols, which may be removed in a future release
const aafUID_t aafFileKindDontCare = kAAFFileKind_DontCare;
const aafUID_t aafFileKindPathalogical = kAAFFileKind_Pathological;
const aafUID_t aafFileKindAafMSSBinary = kAAFFileKind_AafM512Binary;
const aafUID_t aafFileKindAafM4KBinary = kAAFFileKind_AafM4KBinary;
const aafUID_t aafFileKindAafSSSBinary = kAAFFileKind_AafS512Binary;
const aafUID_t aafFileKindAafS4KBinary = kAAFFileKind_AafS4KBinary;
const aafUID_t aafFileKindAafXmlText = kAAFFileKind_AafXmlText;
const aafUID_t aafFileKindMxfKlvBinary = kAAFFileKind_MxfKlvBinary;
const aafUID_t aafFileKindAafSSBinary = kAAFFileKind_Aaf512Binary;
const aafUID_t aafFileKindAaf4KBinary = kAAFFileKind_Aaf4KBinary;
const aafUID_t aafSignature_Aaf_SSBinary = kAAFSignature_Aaf512Binary;
const aafUID_t aafSignature_Aaf_SSBin_4K = kAAFSignature_Aaf4KBinary;


#endif // ! __AAFFileKinds_h__
