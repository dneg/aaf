#ifndef __CodecDefinition_h__
#define __CodecDefinition_h__

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

// AAF well-known CodecDefinition instances
//

//{568fb761-9458-11d2-8089-006008143e6f}
const aafUID_t kAAFCodecDef_None =
{0x568fb761, 0x9458, 0x11d2, {0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{90ac17c8-e3e2-4596-9e9e-a6dd1c70c892}
const aafUID_t kAAFCodecDef_PCM =
{0x90ac17c8, 0xe3e2, 0x4596, {0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92}};


//{820f09b1-eb9b-11d2-809f-006008143e6f}
const aafUID_t kAAFCodecDef_WAVE =
{0x820f09b1, 0xeb9b, 0x11d2, {0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4b1c1a45-03f2-11d4-80fb-006008143e6f}
const aafUID_t kAAFCodecDef_AIFC =
{0x4b1c1a45, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{18634f8c-3bab-11d3-bfd6-00104bc9156d}
const aafUID_t kAAFCodecDef_JPEG =
{0x18634f8c, 0x3bab, 0x11d3, {0xbf, 0xd6, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}};


//{4e84045e-0f29-11d4-a359-009027dfca6a}
const aafUID_t kAAFCodecDef_CDCI =
{0x4e84045e, 0x0f29, 0x11d4, {0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


//{4e84045f-0f29-11d4-a359-009027dfca6a}
const aafUID_t kAAFCodecDef_RGBA =
{0x4e84045f, 0x0f29, 0x11d4, {0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


//{1b31f3b1-9450-11d2-8089-006008143e6f}
const aafUID_t kAAFCodecFlavour_None =
{0x1b31f3b1, 0x9450, 0x11d2, {0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{af4de587-23d7-4c7c-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_LegacyDV_625_50 =
{0xaf4de587, 0x23d7, 0x4c7c, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7d-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_LegacyDV_525_60 =
{0xaf4de587, 0x23d7, 0x4c7d, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7e-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_IEC_DV_625_50 =
{0xaf4de587, 0x23d7, 0x4c7e, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7f-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_IEC_DV_525_60 =
{0xaf4de587, 0x23d7, 0x4c7f, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


// AAF CodecDefinition legacy aliases
//

const aafUID_t kAAFNoCodec = kAAFCodecDef_None;
const aafUID_t kAAFCodecPCM = kAAFCodecDef_PCM;
const aafUID_t kAAFCodecWAVE = kAAFCodecDef_WAVE;
const aafUID_t kAAFCODEC_AIFC = kAAFCodecDef_AIFC;
const aafUID_t kAAFCodecJPEG = kAAFCodecDef_JPEG;
const aafUID_t kAAFCodecCDCI = kAAFCodecDef_CDCI;
const aafUID_t kAAFCodecRGBA = kAAFCodecDef_RGBA;
const aafUID_t kAAFNilCodecFlavour = kAAFCodecFlavour_None;

#endif // ! __CodecDefinition_h__
