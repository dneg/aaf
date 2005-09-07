//=---------------------------------------------------------------------=
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
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ModuleTestsCommon_h__
#define __ModuleTestsCommon_h__

#include "AAF.h"
#include "AAFResult.h"
#include "AAFSmartPointer.h"

#include "ModuleTest.h"

// This is a collection of simple functions intended to ease the
// implemenation of module tests and to prevent the spread of
// cut/paste/modified code fragments.
//
// It resides in its own namespace "mtc" to avoid confusion with any
// existing routines that exist, and are repeated, throughout the
// module tests.  For example, mtc::RemoveTestFile() can be called
// rather than cutting and pasting the original RemoveTestFile routine
// that appears in most module test file.
//
// CAAFAuxiliaryDescriptorTest.cpp, CAAFRecordingDescriptorTest.cpp,
// and CAAFDescriptiveMarkerTest.cpp are the first module test
// implementaions that use the mtc functions.  They provide an example
// of how to use these functions.

// mtc  = Module Test Common

namespace mtc {

const	aafMobID_t	TEST_CompositionMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x4412fd64, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

const	aafMobID_t	TEST_ReferencedMasterMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x4ac5382a, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

const aafRational_t TEST_EditRate = {30, 1};

// Cross-platform utility to delete a file.
void RemoveTestFile(const wchar_t* pFileName);

// Create an AAF file with a CompositionMob that contains a single
// slot, a referenced master mob.  Both mobs have a single slot with
// with an ID if 1.
// The composition mob id is TEST_CompositionMobID
// The master mob id is Test_ReferencedMasterMobID

// Simple structure to return basic pointers a user of the file will
// always be interested in.
struct SimpleFilePointers {
  IAAFSmartPointer<IAAFFile> pFile;
  IAAFSmartPointer<IAAFHeader> pHeader;
  IAAFSmartPointer<IAAFDictionary> pDictionary;
  IAAFSmartPointer<IAAFMob> pCompositionMob;
  IAAFSmartPointer<IAAFMob> pReferencedMasterMob;
  IAAFSmartPointer<IAAFDataDef> pDataDef;
};

// Deprecated
void CreateSimpleAAFFile( aafCharacter* pFileName,
			  aafUID_constref fileKind,
			  const testRawStorageType_t rawStorageType,
			  aafProductIdentification_constref productIdentification,
			  SimpleFilePointers* pFilePointers );

void CreateSimpleAAFFile( aafCharacter* pFileName,
			  aafUID_constref fileKind,
			  const testRawStorageType_t rawStorageType,
			  aafProductIdentification_constref productIdentification,
			  SimpleFilePointers* pFilePointers );

void ReadSimpleAAFFile( aafCharacter* pFileName,
		        SimpleFilePointers* pFilePointers );


IAAFSmartPointer<IAAFMobSlot> GetSlotById( IAAFSmartPointer<IAAFMob>& pMob, aafSlotID_t id );

IAAFSmartPointer<IAAFSourceClip> GetSourceClipFromSlot( IAAFSmartPointer<IAAFMob>& pMob, aafSlotID_t id );

// Chains a source mob on to the source clip attached to slot 1 of the
// master mob in the SimpleFilePointer structure. The caller is
// responsible for adding the required essence descriptor.
IAAFSmartPointer<IAAFSourceMob> AddChainedSourceMob( SimpleFilePointers* pFilePointers );

// convenient error handlers.
void CheckResult(AAFRESULT r);

void CheckExpression(bool expression, AAFRESULT r = AAFRESULT_TEST_FAILED);

} // end of namespace mtc

#endif
