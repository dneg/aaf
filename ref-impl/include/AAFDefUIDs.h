#ifndef __AAFDefUIDs_h__
#define __AAFDefUIDs_h__
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

// This file contains definitions used by the Unit Tests
// !!!None of these constants are required to use AAF, so this file should be moved into the unit test
// heirarchy.
const aafUID_t NilMOBID = { 0xF15D2C20, 0x6D22, 0x11d2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
//const aafUID_t kAAFFmtEnd = { 0x6AEFC4F1, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t UnitTestProductID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

#endif // ! __AAFDefUIDs_h__
