#ifndef __AAFPropertyDefs_h__
#define __AAFPropertyDefs_h__
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

//
// GUIDs for built-in property definitions
//

// Define the AAF_PROPERTY macro to create property GUID definitions.
// Include the meta dictionary.
//
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
const aafUID_t kAAFPropID_##container##_##name = id;

#include "AAFMetaDictionary.h"

//
// Special property ids used for specifying the starting strong reference in the
// targe list for weak references.
//

// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-01-01-00
const aafUID_t kAAFPropID_Root_MetaDictionary =
{0x0D010301, 0x0101, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};

// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-02-01-00
const aafUID_t kAAFPropID_Root_Header =
{0x0D010301, 0x0102, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};


#endif // ! __AAFPropertyDefs_h__
