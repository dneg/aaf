#ifndef __ContainerDefinition_h__
#define __ContainerDefinition_h__

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

// AAF well-known ContainerDefinition instances
//

//{4313b572-d8ba-11d2-809b-006008143e6f}
const aafUID_t kAAFContainerDef_External =
{0x4313b572, 0xd8ba, 0x11d2, {0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4b1c1a46-03f2-11d4-80fb-006008143e6f}
const aafUID_t kAAFContainerDef_OMF =
{0x4b1c1a46, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4313b571-d8ba-11d2-809b-006008143e6f}
const aafUID_t kAAFContainerDef_AAF =
{0x4313b571, 0xd8ba, 0x11d2, {0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{42464141-000d-4d4f-060e-2b34010101ff}
const aafUID_t kAAFContainerDef_AAFMSS =
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{4b464141-000d-4d4f-060e-2b34010101ff}
const aafUID_t kAAFContainerDef_AAFKLV =
{0x4b464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{58464141-000d-4d4f-060e-2b34010101ff}
const aafUID_t kAAFContainerDef_AAFXML =
{0x58464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


//{0d011301-0101-0100-060e-2b3404010106}
const aafUID_t kAAFContainerDef_RIFFWAVE =
{0x0d011301, 0x0101, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};


//{0d011301-0102-0200-060e-2b3404010107}
const aafUID_t kAAFContainerDef_JFIF =
{0x0d011301, 0x0102, 0x0200, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07}};


//{0d011301-0104-0100-060e-2b3404010106}
const aafUID_t kAAFContainerDef_AIFFAIFC =
{0x0d011301, 0x0104, 0x0100, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06}};


// AAF ContainerDefinition legacy aliases
//

const aafUID_t ContainerFile = kAAFContainerDef_External;
const aafUID_t ContainerOMF = kAAFContainerDef_OMF;
const aafUID_t ContainerAAF = kAAFContainerDef_AAF;
const aafUID_t ContainerAAFMSS = kAAFContainerDef_AAFMSS;
const aafUID_t ContainerAAFKLV = kAAFContainerDef_AAFKLV;
const aafUID_t ContainerAAFXML = kAAFContainerDef_AAFXML;

#endif // ! __ContainerDefinition_h__
