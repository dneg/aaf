#ifndef __headerGenTypes_h__
#define __headerGenTypes_h__
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Tim Bingham (Tim_Bingham@avid.com) - after an initial
// implementation by Bob Tillman.

#include "AAFPlatform.h" // For integral types

// Local type definitions so that we don't depend on AAFTypes.h which
// might not exist yet, in fact, we might be generating it.

struct uid {
  aafUInt32 Data1;
  aafUInt16 Data2;
  aafUInt16 Data3;
  aafUInt8  Data4[8];
};

struct label {
  aafUInt8 octet0;
  aafUInt8 octet1;
  aafUInt8 octet2;
  aafUInt8 octet3;
  aafUInt8 octet4;
  aafUInt8 octet5;
  aafUInt8 octet6;
  aafUInt8 octet7;
  aafUInt8 octet8;
  aafUInt8 octet9;
  aafUInt8 octet10;
  aafUInt8 octet11;
  aafUInt8 octet12;
  aafUInt8 octet13;
  aafUInt8 octet14;
  aafUInt8 octet15;
};

#endif // ! __headerGenTypes_h__
