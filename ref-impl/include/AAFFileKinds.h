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

// AAF files encoded as structured storage (binary).
//
const aafUID_t aafFileKindAafSSBinary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// MXF files encoded as structured storage (binary).
//
const aafUID_t aafFileKindMxfSSBinary = 
{0x4246584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// AAF files encoded as XML (text).
//
const aafUID_t aafFileKindAafXmlText = 
{0x58464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// MXF files encoded as XML (text).
//
const aafUID_t aafFileKindMxfXmlText = 
{0x5846584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// AAF files encoded as SMPTE KLV (binary).
//
const aafUID_t aafFileKindAafKlvBinary = 
{0x4b464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// MXF files encoded as SMPTE KLV (binary).
//
const aafUID_t aafFileKindMxfKlvBinary = 
{0x4b46584d, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

#endif // ! __AAFFileKinds_h__
