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

#ifndef _AAFPropertyIDs_H_
#define _AAFPropertyIDs_H_

// AAF PID (property id) definitions.
//
// A property is actually identified by an AUID (a pseudo-GUID derived
// from the SMPTE unique identifier for the property). To save space in
// an AAF file we don't persist the 16 byte AUID with each property,
// instead we persist a PID.
//

// Define the AAF_PROPERTY macros to create PID definitions.
// Include the meta dictionary.
//
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
const int PID_##container##_##name = tag;

#include "AAFMetaDictionary.h"

#endif
