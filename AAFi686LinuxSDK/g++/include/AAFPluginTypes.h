#ifndef __AAFPluginTypes_h__
#define __AAFPluginTypes_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAFCOMPlatformTypes_h__
#include "AAFCOMPlatformTypes.h"
#endif

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif



typedef aafInt32 aafDeinterleave_t;
typedef enum _aafDeinterleave_e
{
	kAAFdeinterleave,
	kAAFleaveInterleaved
} aafDeinterleave_e;

typedef aafInt32 aafCheckVerbose_t;
typedef enum _aafCheckVerbose_e
{
	kAAFCheckVerbose, 
	kAAFCheckQuiet
} aafCheckVerbose_e;

typedef aafInt32 aafCheckWarnings_t;
typedef enum _aafCheckWarnings_e
{
	kAAFCheckPrintWarnings, 
	kAAFCheckNoWarnings
} aafCheckWarnings_e;

typedef struct _aafSelectInfo_t
{
	aafBoolean_t         	hwAssisted;
	aafBoolean_t		isNative;
	aafBoolean_t   		willHandleMDES;
	aafInt16		relativeLoss;
	aafUInt32		avgBitsPerSec;
} aafSelectInfo_t;


#endif // #ifndef __AAFPluginTypes_h__

