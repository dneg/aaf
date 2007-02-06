#ifndef __OperationDefinition_h__
#define __OperationDefinition_h__

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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF well-known OperationDefinition instances
//

//{0c3bea40-fc05-11d2-8a29-0050040ef7d2}
const aafUID_t kAAFOperationDef_VideoDissolve =
{0x0c3bea40, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea44-fc05-11d2-8a29-0050040ef7d2}
const aafUID_t kAAFOperationDef_SMPTEVideoWipe =
{0x0c3bea44, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea890-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_VideoSpeedControl =
{0x9d2ea890, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea891-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_VideoRepeat =
{0x9d2ea891, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{f1db0f32-8d64-11d3-80df-006008143e6f}
const aafUID_t kAAFOperationDef_Flip =
{0xf1db0f32, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{f1db0f34-8d64-11d3-80df-006008143e6f}
const aafUID_t kAAFOperationDef_Flop =
{0xf1db0f34, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{f1db0f33-8d64-11d3-80df-006008143e6f}
const aafUID_t kAAFOperationDef_FlipFlop =
{0xf1db0f33, 0x8d64, 0x11d3, {0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{86f5711e-ee72-450c-a118-17cf3b175dff}
const aafUID_t kAAFOperationDef_VideoPosition =
{0x86f5711e, 0xee72, 0x450c, {0xa1, 0x18, 0x17, 0xcf, 0x3b, 0x17, 0x5d, 0xff}};


//{f5826680-26c5-4149-8554-43d3c7a3bc09}
const aafUID_t kAAFOperationDef_VideoCrop =
{0xf5826680, 0x26c5, 0x4149, {0x85, 0x54, 0x43, 0xd3, 0xc7, 0xa3, 0xbc, 0x09}};


//{2e0a119d-e6f7-4bee-b5dc-6dd42988687e}
const aafUID_t kAAFOperationDef_VideoScale =
{0x2e0a119d, 0xe6f7, 0x4bee, {0xb5, 0xdc, 0x6d, 0xd4, 0x29, 0x88, 0x68, 0x7e}};


//{f2ca330d-8d45-4db4-b1b5-136ab055586f}
const aafUID_t kAAFOperationDef_VideoRotate =
{0xf2ca330d, 0x8d45, 0x4db4, {0xb1, 0xb5, 0x13, 0x6a, 0xb0, 0x55, 0x58, 0x6f}};


//{21d5c51a-8acb-46d5-9392-5cae640c8836}
const aafUID_t kAAFOperationDef_VideoCornerPinning =
{0x21d5c51a, 0x8acb, 0x46d5, {0x93, 0x92, 0x5c, 0xae, 0x64, 0x0c, 0x88, 0x36}};


//{14db900e-d537-49f6-889b-012568fcc234}
const aafUID_t kAAFOperationDef_VideoAlphaWithinVideoKey =
{0x14db900e, 0xd537, 0x49f6, {0x88, 0x9b, 0x01, 0x25, 0x68, 0xfc, 0xc2, 0x34}};


//{e599cb0f-ba5f-4192-9356-51eb19c08589}
const aafUID_t kAAFOperationDef_VideoSeparateAlphaKey =
{0xe599cb0f, 0xba5f, 0x4192, {0x93, 0x56, 0x51, 0xeb, 0x19, 0xc0, 0x85, 0x89}};


//{38ff7903-69e5-476b-be5a-eafc2000f011}
const aafUID_t kAAFOperationDef_VideoLuminanceKey =
{0x38ff7903, 0x69e5, 0x476b, {0xbe, 0x5a, 0xea, 0xfc, 0x20, 0x00, 0xf0, 0x11}};


//{30a315c2-71e5-4e82-a4ef-0513ee056b65}
const aafUID_t kAAFOperationDef_VideoChromaKey =
{0x30a315c2, 0x71e5, 0x4e82, {0xa4, 0xef, 0x05, 0x13, 0xee, 0x05, 0x6b, 0x65}};


//{9d2ea894-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_MonoAudioGain =
{0x9d2ea894, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea893-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_MonoAudioPan =
{0x9d2ea893, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea41-fc05-11d2-8a29-0050040ef7d2}
const aafUID_t kAAFOperationDef_MonoAudioDissolve =
{0x0c3bea41, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{2311bd90-b5da-4285-aa3a-8552848779b3}
const aafUID_t kAAFOperationDef_TwoParameterMonoAudioDissolve =
{0x2311bd90, 0xb5da, 0x4285, {0xaa, 0x3a, 0x85, 0x52, 0x84, 0x87, 0x79, 0xb3}};


//{1575e350-fca3-11d2-8a2a-0050040ef7d2}
const aafUID_t kAAFOperationDef_Unknown =
{0x1575e350, 0xfca3, 0x11d2, {0x8a, 0x2a, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea43-fc05-11d2-8a29-0050040ef7d2}
const aafUID_t kAAFOperationDef_VideoFadeToBlack =
{0x0c3bea43, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0a3c75e0-fd82-11d2-8a2b-0050040ef7d2}
const aafUID_t kAAFOperationDef_PictureWithMate =
{0x0a3c75e0, 0xfd82, 0x11d2, {0x8a, 0x2b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea892-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_VideoFrameToMask =
{0x9d2ea892, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{0c3bea42-fc05-11d2-8a29-0050040ef7d2}
const aafUID_t kAAFOperationDef_StereoAudioDissolve =
{0x0c3bea42, 0xfc05, 0x11d2, {0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{9d2ea895-0968-11d3-8a38-0050040ef7d2}
const aafUID_t kAAFOperationDef_StereoAudioGain =
{0x9d2ea895, 0x0968, 0x11d3, {0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


//{8d896ad0-2261-11d3-8a4c-0050040ef7d2}
const aafUID_t kAAFOperationDef_MonoAudioMixdown =
{0x8d896ad0, 0x2261, 0x11d3, {0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2}};


// AAF OperationDefinition legacy aliases
//

const aafUID_t kAAFEffectVideoDissolve = kAAFOperationDef_VideoDissolve;
const aafUID_t kAAFEffectSMPTEVideoWipe = kAAFOperationDef_SMPTEVideoWipe;
const aafUID_t kAAFEffectVideoSpeedControl = kAAFOperationDef_VideoSpeedControl;
const aafUID_t kAAFEffectVideoRepeat = kAAFOperationDef_VideoRepeat;
const aafUID_t kAAFEffectMonoAudioGain = kAAFOperationDef_MonoAudioGain;
const aafUID_t kAAFEffectMonoAudioPan = kAAFOperationDef_MonoAudioPan;
const aafUID_t kAAFEffectMonoAudioDissolve = kAAFOperationDef_MonoAudioDissolve;
const aafUID_t kAAFOperationUnknown = kAAFOperationDef_Unknown;
const aafUID_t kAAFEffectVideoFadeToBlack = kAAFOperationDef_VideoFadeToBlack;
const aafUID_t kAAFEffectPictureWithMate = kAAFOperationDef_PictureWithMate;
const aafUID_t kAAFEffectVideoFrameToMask = kAAFOperationDef_VideoFrameToMask;
const aafUID_t kAAFEffectStereoAudioDissolve = kAAFOperationDef_StereoAudioDissolve;
const aafUID_t kAAFEffectStereoAudioGain = kAAFOperationDef_StereoAudioGain;
const aafUID_t kAAFEffectMonoAudioMixdown = kAAFOperationDef_MonoAudioMixdown;

#endif // ! __OperationDefinition_h__
