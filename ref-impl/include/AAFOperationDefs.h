/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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




