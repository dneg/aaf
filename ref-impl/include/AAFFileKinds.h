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
#define aafFileKindDontCare_Value \
{0,0,0,{0,0,0,0,0,0,0,0}};
const aafUID_t aafFileKindDontCare = aafFileKindDontCare_Value;

// to cause error if used
#define aafFileKindPathalogical_Value \
{0xff,0xff,0xff,{0,0,0,0,0,0,0,0}};
const aafUID_t aafFileKindPathalogical = aafFileKindPathalogical_Value;

// the enum to select the Microsoft implementation with 512 byte sectors
#define aafFileKindAafMSSBinary_Value \
{ 0xc95e8ee6, 0xa6ec, 0x4e53, { 0x92, 0x28, 0xbd, 0x9b, 0x57, 0x23, 0x57, 0xe5 } }
const aafUID_t aafFileKindAafMSSBinary = aafFileKindAafMSSBinary_Value;

// the enum to select the Microsoft implementation with 4096 byte sectors
#define aafFileKindAafM4KBinary_Value \
{ 0x7653a218, 0x3e03, 0x4ecf, { 0x87, 0x98, 0xf4, 0x5f, 0xc1, 0x17, 0x11, 0x78 } }
const aafUID_t aafFileKindAafM4KBinary = aafFileKindAafM4KBinary_Value;

// the enum to select the SchemaSoft implementation with 512 byte sectors
#define aafFileKindAafSSSBinary_Value \
{ 0xbb153a22, 0xc2ed, 0x4b2e, { 0xbb, 0x69, 0x19, 0xbd, 0x58, 0x9d, 0xf6, 0xdc } }
const aafUID_t aafFileKindAafSSSBinary = aafFileKindAafSSSBinary_Value;

// the enum to select the SchemaSoft implementation with 4096 byte sectors
#define aafFileKindAafS4KBinary_Value \
{ 0xa8ab424a, 0xc5a0, 0x48d0, { 0x9e, 0xea, 0x96, 0x69, 0x69, 0x75, 0xc6, 0xd0 } }
const aafUID_t aafFileKindAafS4KBinary = aafFileKindAafS4KBinary_Value;


// AAF files encoded as XML (text).
//
// used in prototype only (TEST/ComFileKindTest)
#define aafFileKindAafXmlText_Value \
{ 0xc59ef56f, 0x5cb4, 0x4cb8, { 0xbd, 0xe3, 0x8d, 0x7c, 0xd9, 0x25, 0x8e, 0x18 } }
const aafUID_t aafFileKindAafXmlText = aafFileKindAafXmlText_Value;


// MXF files encoded as SMPTE KLV (binary).
//
// not yet in use. this value is incorrect, see SMPTE 377M
#define aafFileKindMxfKlvBinary_Value \
{ 0xfc88c800, 0x879d, 0x48a3, { 0x90, 0x53, 0xee, 0x33, 0x5e, 0x87, 0xe0, 0xc5 } }
const aafUID_t aafFileKindMxfKlvBinary = aafFileKindMxfKlvBinary_Value;


// the enum to select the installation default
// this MUST match what is selected in ImplAAFFile.cpp
#if defined( OS_WINDOWS )
// DEFAULT for this build is Microsoft 512.
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary_Value;
//NOTE: Add default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafM4KBinary_Value;

#elif defined( OS_MACOS )
// No SS implementation available since Microsoft 512 (via MacOLE) has been
// deleted
const aafUID_t aafFileKindAafSSBinary = aafFileKindPathalogical_Value;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindPathalogical_Value;


#elif defined( OS_DARWIN )
// DEFAULT is Schemasoft 512 (via libSSRW2C.a).
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary_Value;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary_Value;


#elif defined( OS_IRIX )

// DEFAULT is Schemasoft 512 (via libSSRW2C.a). 
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary_Value;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary_Value;

#elif defined( OS_LINUX )
// DEFAULT is Schemasoft 512 (via libSSRW2C.a). 
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary_Value;
//NOTE: Add default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary_Value;

#elif defined( OS_FREEBSD )
// No SS implementations ported as yet
const aafUID_t aafFileKindAafSSBinary = aafFileKindPathalogical_Value;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindPathalogical_Value;


#elif defined( OS_SOLARIS )
// DEFAULT is SSS 512 (via libSSRW2C.a)
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary_Value;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary_Value;

#else
#error Unknown platform
#endif




// the following signatures could be factored into a separate file

// AAF files encoded as structured storage (binary).
//
// the signature actually stored in all AAF SS (512) files
// note this is not a properly-formed SMPTE label, but this is legacy
#define aafSignature_Aaf_SSBinary_Value \
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};
const aafUID_t aafSignature_Aaf_SSBinary = aafSignature_Aaf_SSBinary_Value; 


// the signature actually stored in all AAF SS (4096) files
// TO BE CONFIRMED
// [060e2b34.0401.0105.0d010101.03020000]
#define aafSignature_Aaf_SSBin_4K_Value \
{0x0101010d, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05}};
const aafUID_t aafSignature_Aaf_SSBin_4K = aafSignature_Aaf_SSBin_4K_Value;

#endif // ! __AAFFileKinds_h__

