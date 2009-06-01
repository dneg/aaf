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
// Schema Software Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=


/************************************************************************
* StructuredStorage.h
* Public Header file for Structured Storage library
*
* (c) Schema Software Inc., 2001-2004
* Authors: Bob Sabiston, Yuri Khramov, Mark Ambachtsheer
************************************************************************

$Revision$
$Date$
*/
#ifndef OMSSSSTORAGERWINPUTSOURCE_H
#define OMSSSSTORAGERWINPUTSOURCE_H

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMRawStorage.h"
#include "StructuredStorage.h"
extern "C" int openStructuredStorageInOMRawStorage (
    const OMRawStorage* in_pRaw,
    StgMode in_accessMode,
    RootStorage** out_ppRoot);

extern "C" int createStructuredStorageInOMRawStorageEx (
    const OMRawStorage* in_pRaw,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_sectorSize);

#endif // !OM_NO_STRUCTURED_STORAGE

#endif //OMSSSSTORAGERWINPUTSOURCE_H
