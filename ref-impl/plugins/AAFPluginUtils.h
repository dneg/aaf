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
// The Original Code of this file is Copyright 2008-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef AAFPLUGINUTILS_H
#define AAFPLUGINUTILS_H

#include <math.h>
#include <string.h>

#include "AAFTypes.h"


typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola

AAFByteOrder GetNativeByteOrder(void);

//DUPLICATED in AAFUtils.h
aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2);


/************************
 *   Function:	aafIsEqualGUID
 *
 *	Determines whether specified GUIDs are equal.
 *
 *   ReturnValue:
 *	kAAFTrue - The GUIDs are equal.
 *	kAAFFalse - The GUIDs are not equal.
 */
aafBool aafIsEqualGUID( const GUID& guid1, const GUID& guid2 );
#define aafIsEqualIID(x, y) aafIsEqualGUID(x, y)
#define aafIsEqualCLSID(x, y) aafIsEqualGUID(x, y)



void AAFByteSwap16(aafInt16 * wp);
void AAFByteSwap32(aafInt32 *lp);
void AAFByteSwap64(aafInt64 *lp);



double FloatFromRational(aafRational_t	e);

//DUPLICATED in AAFUtils.h
aafRational_t RationalFromFloat(double	f);

//DUPLICATED in AAFUtils.h
void AAFGetDateTime(aafTimeStamp_t *time);

// aafUID_t comparison operators. DUPLICATED from AAFUtils.h

inline bool operator == (const aafUID_t& lhs,
                         const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) == 0;
}

inline bool operator != (const aafUID_t& lhs,
                         const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) != 0;
}




#endif

