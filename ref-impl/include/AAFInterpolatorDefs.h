#ifndef __InterpolationDefinition_h__
#define __InterpolationDefinition_h__

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
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF well-known InterpolationDefinition instances
//

//{5b6c85a3-0ede-11d3-80a9-006008143e6f}
const aafUID_t kAAFInterpolationDef_None =
{0x5b6c85a3, 0x0ede, 0x11d3, {0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{5b6c85a4-0ede-11d3-80a9-006008143e6f}
const aafUID_t kAAFInterpolationDef_Linear =
{0x5b6c85a4, 0x0ede, 0x11d3, {0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{5b6c85a5-0ede-11d3-80a9-006008143e6f}
const aafUID_t kAAFInterpolationDef_Constant =
{0x5b6c85a5, 0x0ede, 0x11d3, {0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{5b6c85a6-0ede-11d3-80a9-006008143e6f}
const aafUID_t kAAFInterpolationDef_BSpline =
{0x5b6c85a6, 0x0ede, 0x11d3, {0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{15829ec3-1f24-458a-960d-c65bb23c2aa1}
const aafUID_t kAAFInterpolationDef_Log =
{0x15829ec3, 0x1f24, 0x458a, {0x96, 0x0d, 0xc6, 0x5b, 0xb2, 0x3c, 0x2a, 0xa1}};


//{c09153f7-bd18-4e5a-ad09-cbdd654fa001}
const aafUID_t kAAFInterpolationDef_Power =
{0xc09153f7, 0xbd18, 0x4e5a, {0xad, 0x09, 0xcb, 0xdd, 0x65, 0x4f, 0xa0, 0x01}};


// AAF InterpolationDefinition legacy aliases
//

const aafUID_t NoInterpolator = kAAFInterpolationDef_None;
const aafUID_t LinearInterpolator = kAAFInterpolationDef_Linear;
const aafUID_t ConstantInterpolator = kAAFInterpolationDef_Constant;
const aafUID_t BSplineInterpolator = kAAFInterpolationDef_BSpline;
const aafUID_t LogInterpolator = kAAFInterpolationDef_Log;
const aafUID_t PowerInterpolator = kAAFInterpolationDef_Power;

#endif // ! __InterpolationDefinition_h__
