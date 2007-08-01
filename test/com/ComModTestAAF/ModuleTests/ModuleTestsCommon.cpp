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

#include "ModuleTestsCommon.h"
#include "ModuleTest.h"

#include "AAFSmartPointer.h"
#include "AAFDefUIDs.h"
#include "AAFResult.h"
#include "CAAFBuiltinDefs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

namespace mtc {

//======================================================================

const aafWChar *slotName = L"SLOT1";

//======================================================================

void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

//======================================================================

void CreateSimpleAAFFile(aafWChar * pFileName,
			 aafUID_constref fileKind,
			 const testRawStorageType_t rawStorageType,
			 aafProductIdentification_constref productIdentification,
			 SimpleFilePointers* pFilePointers )
{
  // Remove the previous test file if any.
  RemoveTestFile(pFileName);

  // Create the file.
  IAAFSmartPointer<IAAFFile> pFile;
  CheckResult(CreateTestFile(pFileName,
                             fileKind,
                             rawStorageType,
                             productIdentification,
                             &pFile));
    
  IAAFSmartPointer<IAAFHeader> pHeader;
  CheckResult(pFile->GetHeader(&pHeader));

  IAAFSmartPointer<IAAFDictionary> pDictionary;
  CheckResult(pHeader->GetDictionary(&pDictionary));
  CAAFBuiltinDefs defs(pDictionary);
    
  // Make the MOB to be referenced.
  IAAFSmartPointer<IAAFMob> pMob;
  IAAFSmartPointer<IAAFMob> pReferencedMob;

  CheckResult(defs.cdMasterMob()->
	      CreateInstance(IID_IAAFMob, 
			     (IUnknown **)&pReferencedMob));
  CheckResult(pReferencedMob->SetMobID(TEST_ReferencedMasterMobID));
  CheckResult(pReferencedMob->SetName(L"ReferencedMob"));
  
  // Create a composition mob and a slot containging a source
  // reference to the master mob.

  CheckResult(defs.cdCompositionMob()->
	      CreateInstance(IID_IAAFMob, 
			     (IUnknown **)&pMob));
  CheckResult(pMob->SetMobID(TEST_CompositionMobID));
  CheckResult(pMob->SetName( productIdentification.productName ));
  
  IAAFSmartPointer<IAAFSourceClip> pSourceClip;
  CheckResult(defs.cdSourceClip()->
	      CreateInstance(IID_IAAFSourceClip, 
			     (IUnknown **)&pSourceClip));
  aafSourceRef_t srcRef;
  srcRef.sourceID = TEST_ReferencedMasterMobID;  // Create a SourceClip
  srcRef.sourceSlotID = 1;
  srcRef.startTime = 0;
  CheckResult(defs.cdSourceClip()->
	      CreateInstance(IID_IAAFSourceClip, 
			     (IUnknown **)&pSourceClip));
  CheckResult(pSourceClip->Initialize( defs.ddkAAFPicture(), 1, srcRef));

  IAAFSmartPointer<IAAFSegment>	pSeg;
  CheckResult(pSourceClip->QueryInterface(IID_IAAFSegment, (void **)&pSeg));

  IAAFSmartPointer<IAAFTimelineMobSlot>	pNewSlot;
  CheckResult(pMob->AppendNewTimelineSlot(TEST_EditRate,
					  pSeg,
					  1,
					  slotName,
					  0,
					  &pNewSlot ) );

  // Now properly terminate the mob chain at the master mob.

  IAAFSmartPointer<IAAFSourceClip> pEndSourceClip;
  CheckResult(defs.cdSourceClip()->
	      CreateInstance(IID_IAAFSourceClip, 
			     (IUnknown **)&pEndSourceClip));
  memset( &srcRef, 0, sizeof(srcRef) );
  CheckResult(pEndSourceClip->Initialize( defs.ddkAAFPicture(), 1, srcRef));
  
  IAAFSmartPointer<IAAFSegment>	pEndSeg;
  CheckResult(pEndSourceClip->QueryInterface(IID_IAAFSegment, (void **)&pEndSeg));

  CheckResult(pReferencedMob->AppendNewTimelineSlot(TEST_EditRate,
						    pEndSeg,
						    1,
						    slotName,
						    0,
						    &pNewSlot ));

  CheckResult(pHeader->AddMob(pMob));
  CheckResult(pHeader->AddMob(pReferencedMob));

  assert(pFilePointers);
  pFilePointers->pFile = pFile;
  pFilePointers->pHeader = pHeader;
  pFilePointers->pDictionary = pDictionary;
  pFilePointers->pCompositionMob = pMob;
  pFilePointers->pReferencedMasterMob = pReferencedMob;

  IAAFSmartPointer<IAAFDataDef> pDataDef( defs.ddkAAFPicture() );
  pFilePointers->pDataDef = pDataDef;
}

//======================================================================

void ReadSimpleAAFFile( aafCharacter* pFileName,
		        SimpleFilePointers* pFilePointers )
{
  IAAFSmartPointer<IAAFFile> pFile;
  CheckResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

  IAAFSmartPointer<IAAFHeader> pHeader;
  CheckResult(pFile->GetHeader(&pHeader));

  IAAFSmartPointer<IAAFDictionary> pDictionary;
  CheckResult(pHeader->GetDictionary(&pDictionary));

  IAAFSmartPointer<IAAFMob> pCompositionMob;
  CheckResult(pHeader->LookupMob( TEST_CompositionMobID, &pCompositionMob ));

  IAAFSmartPointer<IAAFMob> pReferencedMasterMob;
  CheckResult(pHeader->LookupMob( TEST_ReferencedMasterMobID, &pReferencedMasterMob ));

  assert(pFilePointers);
  pFilePointers->pFile = pFile;
  pFilePointers->pHeader = pHeader;
  pFilePointers->pDictionary = pDictionary;
  pFilePointers->pCompositionMob = pCompositionMob;
  pFilePointers->pReferencedMasterMob = pReferencedMasterMob;
}

//======================================================================

IAAFSmartPointer<IAAFMobSlot> GetSlotById( IAAFSmartPointer<IAAFMob>& pMob, aafSlotID_t id )
{
  IAAFSmartPointer<IEnumAAFMobSlots> pSlotIter;
  CheckResult( pMob->GetSlots(&pSlotIter) );

  IAAFSmartPointer<IAAFMobSlot> pSlot;
  HRESULT hr;
  for( hr = pSlotIter->NextOne( &pSlot );
       hr == AAFRESULT_SUCCESS;
       hr = pSlotIter->NextOne( &pSlot ) ) {
    aafSlotID_t slotId;
    CheckResult( pSlot->GetSlotID( &slotId ) );
    if ( id == slotId ) {
      return pSlot;
    }
  }

  CheckResult( AAFRESULT_TEST_FAILED );

  // Return something to keep MSVC6 happy.
  IAAFSmartPointer<IAAFMobSlot> null;
  return null;
}

//======================================================================

IAAFSmartPointer<IAAFSourceClip> GetSourceClipFromSlot( IAAFSmartPointer<IAAFMob>& pMob, aafSlotID_t id )
{
  IAAFSmartPointer<IAAFMobSlot> pSlot = GetSlotById( pMob, id );

  IAAFSmartPointer<IAAFSegment> pSeg;
  CheckResult( pSlot->GetSegment( &pSeg ) );

  IAAFSmartPointer<IAAFSourceClip> pSourceClip;
  CheckResult( pSeg->QueryInterface( IID_IAAFSourceClip, (void**)&pSourceClip ) );

  return pSourceClip;
}

//======================================================================

IAAFSmartPointer<IAAFSourceMob> AddChainedSourceMob( SimpleFilePointers* pFilePointers )
{
  // Create a new source mob and chain it to the master mob in
  // pFilePointers.

  CAAFBuiltinDefs defs(pFilePointers->pDictionary);

  // Create a source mob with one slot and a source clip.

  IAAFSmartPointer<IAAFSourceMob> pSrcMob;
  CheckResult( pFilePointers->pDictionary->CreateInstance( AUID_AAFSourceMob,
							   IID_IAAFSourceMob,
							   (IUnknown**)&pSrcMob ) );

  CheckResult( pSrcMob->Initialize() );

  IAAFSmartPointer<IAAFSourceClip> pSourceClip;
  CheckResult(defs.cdSourceClip()->
	      CreateInstance(IID_IAAFSourceClip, 
			     (IUnknown **)&pSourceClip));
  aafSourceRef_t srcRef;
  memset( &srcRef, 0, sizeof(srcRef) );
  CheckResult(pSourceClip->Initialize( defs.ddkAAFPicture(), 1, srcRef));

  IAAFSmartPointer<IAAFSegment>	pSeg;
  CheckResult(pSourceClip->QueryInterface(IID_IAAFSegment, (void **)&pSeg));

  IAAFSmartPointer<IAAFMob> pMob;
  CheckResult( pSrcMob->QueryInterface( IID_IAAFMob, (void**)&pMob ));

  IAAFSmartPointer<IAAFTimelineMobSlot>	pNewSlot;
  CheckResult(pMob->AppendNewTimelineSlot(TEST_EditRate,
					  pSeg,
					  1,
					  slotName,
					  0,
					  &pNewSlot));


  // Initialize a source reference to the new mob.
  
  CheckResult( pMob->GetMobID( &srcRef.sourceID ));
  srcRef.sourceSlotID = 1;
  srcRef.startTime = 0;

  // Now, link the new source mob to the master mob in pFilePointers.

  IAAFSmartPointer<IAAFMobSlot> pMasterSlot = GetSlotById( pFilePointers->pReferencedMasterMob, 1 );

  CheckResult( pMasterSlot->GetSegment( &pSeg ) );
  CheckResult( pSeg->QueryInterface( IID_IAAFSourceClip, (void**)&pSourceClip ) );
  CheckResult( pSourceClip->SetSourceReference( srcRef ) );


  // Add the new mob to the file.

  CheckResult( pFilePointers->pHeader->AddMob( pMob ) );

  return pSrcMob;
}

// convenient error handlers.
void CheckResult(AAFRESULT r)
{
  if (FAILED(r))
    throw r;
}

void CheckExpression(bool expression, AAFRESULT r)
{
  if (!expression)
    throw r;
}

} // end of namespace mtc
