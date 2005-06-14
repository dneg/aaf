#ifndef __DataDefinition_h__
#define __DataDefinition_h__

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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF well-known DataDefinition instances
//

//{01030202-0100-0000-060e-2b3404010101}
const aafUID_t kAAFDataDef_Picture =
{0x01030202, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{6f3c8ce1-6cef-11d2-807d-006008143e6f}
const aafUID_t kAAFDataDef_LegacyPicture =
{0x6f3c8ce1, 0x6cef, 0x11d2, {0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{05cba731-1daa-11d3-80ad-006008143e6f}
const aafUID_t kAAFDataDef_Matte =
{0x05cba731, 0x1daa, 0x11d3, {0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{05cba732-1daa-11d3-80ad-006008143e6f}
const aafUID_t kAAFDataDef_PictureWithMatte =
{0x05cba732, 0x1daa, 0x11d3, {0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{01030202-0200-0000-060e-2b3404010101}
const aafUID_t kAAFDataDef_Sound =
{0x01030202, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{78e1ebe1-6cef-11d2-807d-006008143e6f}
const aafUID_t kAAFDataDef_LegacySound =
{0x78e1ebe1, 0x6cef, 0x11d2, {0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{01030201-0100-0000-060e-2b3404010101}
const aafUID_t kAAFDataDef_Timecode =
{0x01030201, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{7f275e81-77e5-11d2-807f-006008143e6f}
const aafUID_t kAAFDataDef_LegacyTimecode =
{0x7f275e81, 0x77e5, 0x11d2, {0x80, 0x7f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{d2bb2af0-d234-11d2-89ee-006097116212}
const aafUID_t kAAFDataDef_Edgecode =
{0xd2bb2af0, 0xd234, 0x11d2, {0x89, 0xee, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12}};


//{01030201-1000-0000-060e-2b3404010101}
const aafUID_t kAAFDataDef_DescriptiveMetadata =
{0x01030201, 0x1000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};


//{01030203-0100-0000-060e-2b3404010105}
const aafUID_t kAAFDataDef_Auxiliary =
{0x01030203, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05}};


//{851419d0-2e4f-11d3-8a5b-0050040ef7d2}
const aafUID_t kAAFDataDef_Unknown =
{0x851419d0, 0x2e4f, 0x11d3, {0x8a, 0x5b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


// AAF DataDefinition legacy aliases
//

const aafUID_t DDEF_Picture = kAAFDataDef_LegacyPicture;
const aafUID_t DDEF_Matte = kAAFDataDef_Matte;
const aafUID_t DDEF_PictureWithMatte = kAAFDataDef_PictureWithMatte;
const aafUID_t DDEF_Sound = kAAFDataDef_LegacySound;
const aafUID_t DDEF_Timecode = kAAFDataDef_LegacyTimecode;
const aafUID_t DDEF_Edgecode = kAAFDataDef_Edgecode;
const aafUID_t DDEF_Unknown = kAAFDataDef_Unknown;

#endif // ! __DataDefinition_h__
