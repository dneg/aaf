//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Following are the UIDs of some well-known operation definition objects in AAF.  Most of the definitions
// are for effects, as that is currently the most common operation performed.
//
// When an API call asks for a operation definition UID, store it into a local UID variable, and then pass
// in the address of the variable, as in:
//		aafUID_t	uid = DDEF_Picture;
//
// When you need an IAAFOperationDef *, then do the following (where pDictionary is a pointer to the
// dictionary object in the current file.)
//		aafUID_t	uid = kAAFEffectVideoDissolve;
//		IAAFOperationDef	*definition = NULL;
//		hr = pDictionary->LookupOperationDefintion(&uid, &definition);
//		... Use the definition
//		definition->Release()
//		definition = NULL;;
//
// {0C3BEA40-FC05-11d2-8A29-0050040EF7D2}
const aafUID_t kAAFEffectVideoDissolve = { 0xc3bea40, 0xfc05, 0x11d2, { 0x8a, 0x29, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } }; //tlk
// {0C3BEA41-FC05-11d2-8A29-0050040EF7D2}
const aafUID_t kAAFEffectMonoAudioDissolve = { 0xc3bea41, 0xfc05, 0x11d2, { 0x8a, 0x29, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } }; //tlk
// {0C3BEA42-FC05-11d2-8A29-0050040EF7D2}
const aafUID_t kAAFEffectStereoAudioDissolve = { 0xc3bea42, 0xfc05, 0x11d2, { 0x8a, 0x29, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {0C3BEA43-FC05-11d2-8A29-0050040EF7D2}
const aafUID_t kAAFEffectVideoFadeToBlack = { 0xc3bea43, 0xfc05, 0x11d2, { 0x8a, 0x29, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {0C3BEA44-FC05-11d2-8A29-0050040EF7D2}
const aafUID_t kAAFEffectSMPTEVideoWipe = { 0xc3bea44, 0xfc05, 0x11d2, { 0x8a, 0x29, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {0A3C75E0-FD82-11d2-8A2B-0050040EF7D2}
const aafUID_t kAAFEffectPictureWithMate = { 0xa3c75e0, 0xfd82, 0x11d2, { 0x8a, 0x2b, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA890-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectVideoSpeedControl = { 0x9d2ea890, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA891-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectVideoRepeat = { 0x9d2ea891, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA892-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectVideoFrameToMask = { 0x9d2ea892, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA893-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectMonoAudioPan = { 0x9d2ea893, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA894-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectMonoAudioGain = { 0x9d2ea894, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {9D2EA895-0968-11d3-8A38-0050040EF7D2}
const aafUID_t kAAFEffectStereoAudioGain = { 0x9d2ea895, 0x968, 0x11d3, { 0x8a, 0x38, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };
// {8D896AD0-2261-11d3-8A4C-0050040EF7D2}
const aafUID_t kAAFEffectMonoAudioMixdown ={ 0x8d896ad0, 0x2261, 0x11d3, {0x8a, 0x4c, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };

// {1575E350-FCA3-11d2-8A2A-0050040EF7D2}
const aafUID_t kAAFOperationUnknown = { 0x1575e350, 0xfca3, 0x11d2, { 0x8a, 0x2a, 0x0, 0x50, 0x4, 0xe, 0xf7, 0xd2 } };




