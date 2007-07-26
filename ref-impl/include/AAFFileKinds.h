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

//
// The following enumerations select the file encoding type without
// specifying the implementation (in cases where multiple
// implementations are available)
//
// New code should use one of these encodings.
//

// AAF file encoded using the default encoding for the particular SDK
// release.
const aafUID_t kAAFFileKind_DontCare =
{0,0,0,{0,0,0,0,0,0,0,0}};


// AAF files encoded as structured storage with a 512 bytes sector size
const aafUID_t kAAFFileKind_Aaf512Binary =
{ 0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff } };

// AAF files structured storage with a 4096 bytes sector size
const aafUID_t kAAFFileKind_Aaf4KBinary =
{0x92b02efb, 0xaf40, 0x4896, {0xa5, 0x8e, 0xd1, 0x57, 0x2f, 0x42, 0x2b, 0x58 } };


// AAF files encoded as XML (text)
const aafUID_t kAAFFileKind_AafXmlText =
{ 0xfe0d0101, 0x60e1, 0x4e78, { 0xb2, 0xcd, 0x2b, 0x03, 0xdb, 0xb0, 0xfa, 0x87 } };

// AAF files encoded as KLV (binary) (MXF)
//
const aafUID_t kAAFFileKind_AafKlvBinary =
{0x4b464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff } };

// the enum to select the default implementation with 4096 byte sectors


//
// The following encodings select a specific encoding implementation
// in cases where multiple implementations are available. It is not
// advisable to use these in new code. The exist primarily for test
// purposes.
//

// the enum to select the Microsoft implementation with 512 byte sectors
const aafUID_t kAAFFileKind_AafM512Binary =
{ 0xc95e8ee6, 0xa6ec, 0x4e53, { 0x92, 0x28, 0xbd, 0x9b, 0x57, 0x23, 0x57, 0xe5 } };

// the enum to select the SchemaSoft implementation with 512 byte sectors
const aafUID_t kAAFFileKind_AafS512Binary =
{ 0xbb153a22, 0xc2ed, 0x4b2e, { 0xbb, 0x69, 0x19, 0xbd, 0x58, 0x9d, 0xf6, 0xdc } };

// the enum to select the GSF implementation with 512 byte sectors
const aafUID_t kAAFFileKind_AafG512Binary =
{ 0xb965c7f1, 0xf89d, 0x4490, { 0xbd, 0x22, 0x77, 0x35, 0x69, 0xb4, 0xd3, 0x61 } };

// the enum to select the Microsoft implementation with 4096 byte sectors
const aafUID_t kAAFFileKind_AafM4KBinary =
{ 0x7653a218, 0x3e03, 0x4ecf, { 0x87, 0x98, 0xf4, 0x5f, 0xc1, 0x17, 0x11, 0x78 } };

// the enum to select the SchemaSoft implementation with 4096 byte sectors
const aafUID_t kAAFFileKind_AafS4KBinary =
{ 0xa8ab424a, 0xc5a0, 0x48d0, { 0x9e, 0xea, 0x96, 0x69, 0x69, 0x75, 0xc6, 0xd0 } };

// the enum to select the GSF implementation with 4096 byte sectors
const aafUID_t kAAFFileKind_AafG4KBinary =
{ 0xb44818b, 0xc3dd, 0x4f0a, { 0xad, 0x37, 0xe9, 0x71, 0x0, 0x7a, 0x88, 0xe8 } };


//
// The folloing enumerations exist for testing purposes only and will
// trigger anb error if used.
//

const aafUID_t kAAFFileKind_Pathological =
{0xff,0xff,0xff,{0,0,0,0,0,0,0,0}};


//
// Old deprecated symbols which may be removed in a future release.
// Don't use these in new code.
//

const aafUID_t aafFileKindDontCare     = kAAFFileKind_DontCare;
const aafUID_t aafFileKindPathalogical = kAAFFileKind_Pathological;
const aafUID_t aafFileKindAafKlvBinary = kAAFFileKind_AafKlvBinary;
const aafUID_t aafFileKindAafMSSBinary = kAAFFileKind_AafM512Binary;
const aafUID_t aafFileKindAafM4KBinary = kAAFFileKind_AafM4KBinary;
const aafUID_t aafFileKindAafSSSBinary = kAAFFileKind_AafS512Binary;
const aafUID_t aafFileKindAafS4KBinary = kAAFFileKind_AafS4KBinary;
const aafUID_t aafFileKindAafXmlText   = kAAFFileKind_AafXmlText;
const aafUID_t aafFileKindAafSSBinary  = kAAFFileKind_Aaf512Binary;
const aafUID_t aafFileKindAaf4KBinary  = kAAFFileKind_Aaf4KBinary;


#endif // ! __AAFFileKinds_h__
