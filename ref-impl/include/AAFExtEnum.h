#ifndef __AAFExtEnum_h__
#define __AAFExtEnum_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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


#include "AAFTypes.h"

// AAF extensible enumeration member UIDs.
//

// Members of OperationCategoryType
//
//{0d010102-0101-0100-060e-2b3404010101}
const aafUID_t kAAFOperationCategory_Effect =
{0x0d010102, 0x0101, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of TransferCharacteristicType
//
//{04010101-0101-0000-060e-2b3404010101}
const aafUID_t kAAFTransferCharacteristic_ITU470_PAL =
{0x04010101, 0x0101, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0102-0000-060e-2b3404010101}
const aafUID_t kAAFTransferCharacteristic_ITU709 =
{0x04010101, 0x0102, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0103-0000-060e-2b3404010101}
const aafUID_t kAAFTransferCharacteristic_SMPTE240M =
{0x04010101, 0x0103, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0105-0000-060e-2b3404010101}
const aafUID_t kAAFTransferCharacteristic_ITU1361 =
{0x04010101, 0x0105, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0106-0000-060e-2b3404010101}
const aafUID_t kAAFTransferCharacteristic_linear =
{0x04010101, 0x0106, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of PluginCategoryType
//
//{0d010102-0101-0200-060e-2b3404010101}
const aafUID_t kAAFPluginCategory_Effect =
{0x0d010102, 0x0101, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0300-060e-2b3404010101}
const aafUID_t kAAFPluginCategory_Codec =
{0x0d010102, 0x0101, 0x0300, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0400-060e-2b3404010101}
const aafUID_t kAAFPluginCategory_Interpolation =
{0x0d010102, 0x0101, 0x0400, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of UsageType
//
//{0d010102-0101-0500-060e-2b3404010101}
const aafUID_t kAAFUsage_SubClip =
{0x0d010102, 0x0101, 0x0500, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0600-060e-2b3404010101}
const aafUID_t kAAFUsage_AdjustedClip =
{0x0d010102, 0x0101, 0x0600, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0700-060e-2b3404010101}
const aafUID_t kAAFUsage_TopLevel =
{0x0d010102, 0x0101, 0x0700, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0800-060e-2b3404010101}
const aafUID_t kAAFUsage_LowerLevel =
{0x0d010102, 0x0101, 0x0800, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{0d010102-0101-0900-060e-2b3404010101}
const aafUID_t kAAFUsage_Template =
{0x0d010102, 0x0101, 0x0900, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

// Members of ColorPrimariesType
//
//{04010101-0301-0000-060e-2b3404010106}
const aafUID_t kAAFColorPrimaries_SMPTE170M =
{0x04010101, 0x0301, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

//{04010101-0302-0000-060e-2b3404010106}
const aafUID_t kAAFColorPrimaries_ITU470_PAL =
{0x04010101, 0x0302, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

//{04010101-0303-0000-060e-2b3404010106}
const aafUID_t kAAFColorPrimaries_ITU709 =
{0x04010101, 0x0303, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};

// Members of CodingEquationsType
//
//{04010101-0201-0000-060e-2b3404010101}
const aafUID_t kAAFCodingEquations_ITU601 =
{0x04010101, 0x0201, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0202-0000-060e-2b3404010101}
const aafUID_t kAAFCodingEquations_ITU709 =
{0x04010101, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

//{04010101-0203-0000-060e-2b3404010101}
const aafUID_t kAAFCodingEquations_SMPTE240M =
{0x04010101, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


#endif // ! __AAFExtEnum_h__
