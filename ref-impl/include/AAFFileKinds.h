#ifndef __AAFFileKinds_h__
#define __AAFFileKinds_h__
//=---------------------------------------------------------------------=
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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

// the enum to select DON'T CARE (whatever works)
const aafUID_t aafFileKindDontCare = {0,0,0,{0,0,0,0,0,0,0,0}};

// the enum to select the Microsoft implementation
const aafUID_t aafFileKindAafMSSBinary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the enum to select the SchemaSoft implementation with 512 byte sectors
const aafUID_t aafFileKindAafSSSBinary = 
{0x42464141, 0x1205, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the enum to select the SchemaSoft implementation with 4096 byte sectors
const aafUID_t aafFileKindAafS4KBinary = 
{0x42464141, 0x9640, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the enum to select the Microsoft implementation with 4096 byte sectors
const aafUID_t aafFileKindAafM4KBinary = 
{0x42464141, 0xa838, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// AAF files encoded as XML (text).
//
// used in prototype only (TEST/ComFileKindTest)
const aafUID_t aafFileKindAafXmlText = 
{0x58464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// MXF files encoded as SMPTE KLV (binary).
//
// not yet in use. this value is incorrect, see SMPTE 377M
const aafUID_t aafFileKindMxfKlvBinary = 
{0x4b46584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


// the enum to select the installation default
// this MUST match what is selected in ImplAAFFile.cpp
#if defined( OS_WINDOWS )
// DEFAULT for this build is SchemaSoft 512.
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary;
const aafUID_t aafFileKindAafSSAlternate = aafFileKindAafSSSBinary;

//NOTE: Add default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafM4KBinary;
const aafUID_t aafFileKindAaf4KAlternate = aafFileKindAafS4KBinary;

#elif defined( OS_MACOS )
// DEFAULT is Microsoft 512 (via MacOLE). SchemaSoft not yet ported
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary;

#elif defined( OS_MACOS10 )
// DEFAULT is Microsoft 512 (via Carbon lib(?). SchemaSoft not yet ported
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary;

#elif defined( OS_IRIX )
// DEFAULT is Microsoft 512 (via librefstg). SchemaSoft not yet ported
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary;

#elif defined( OS_LINUX )
// DEFAULT is Microsoft 512 (via librefstg). SchemaSoft not yet ported
//NOTE: change to Schemasoft for testing
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary;
//NOTE: Add default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary;

#elif defined( OS_FREEBSD )
// DEFAULT is Microsoft 512 (via librefstg). SchemaSoft not yet ported
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafMSSBinary;

#elif defined( OS_SOLARIS )
// DEFAULT is SSS 512. MSS not available
const aafUID_t aafFileKindAafSSBinary = aafFileKindAafSSSBinary;
// default 4k binary
const aafUID_t aafFileKindAaf4KBinary = aafFileKindAafS4KBinary;
#else
#error Unknown platform
#endif




// the following signatures could be factored into a separate file

// AAF files encoded as structured storage (binary).
//
// the signature actually stored in all AAF SS (512) files
// note this is not a properly-formed SMPTE label, but this is legacy
const aafUID_t aafSignature_Aaf_SSBinary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the signature actually stored in all AAF SS (4096) files
// TO BE CONFIRMED
// [060e2b34.0401.0105.0d010101.03020000]
const aafUID_t aafSignature_Aaf_SSBin_4K = 
{0x0101010d, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05}};


#endif // ! __AAFFileKinds_h__

