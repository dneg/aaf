//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;

static const aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
static const aafUID_t *	slotDDefs[5] = {&kAAFDataDef_Picture, &kAAFDataDef_Sound, &kAAFDataDef_Sound, &kAAFDataDef_Picture, &kAAFDataDef_Picture};
static aafLength_t	slotsLength[5] = { 297, 44100, 44100, 44100, 44100};

static aafSourceRef_t sourceRef; 
static const aafWChar* TagNames =  L"TAG01";
static const aafWChar* Comments =  L"Comment 1";	
static const aafWChar* AltComment = L"Alternate Comment";

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xebf452a8, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const 	aafMobID_t	TEST_referencedMobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x25ede122, 0x0405, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}


static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile *			pFile = NULL;
	bool				bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob				*pMob = NULL;
	IAAFMob*			pRMob = NULL;
	IAAFMasterMob*		pReferencedMob = NULL;
	IAAFCompositionMob* pCompMob = NULL;
	IAAFTimelineMobSlot	*newSlot = NULL;
	IAAFSegment			*seg = NULL;
	IAAFSourceClip		*sclp = NULL;
	HRESULT				hr = S_OK;

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the file.
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		//Make the first mob
		long	test;

		// Create a  Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));

		checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFTaggedValuesTest"));

		// append a comment to this mob !!
		checkResult(pMob->AppendComment(const_cast<aafWChar*>(TagNames), Comments));
	  
		checkResult(pMob->AppendComment(const_cast<aafWChar*>(TagNames), AltComment));

		// Create a master mob to be referenced
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, 
								   (IUnknown **)&pReferencedMob));

		checkResult(pReferencedMob->QueryInterface(IID_IAAFMob, (void **)&pRMob));
		checkResult(pRMob->SetMobID(TEST_referencedMobID));
		checkResult(pRMob->SetName(L"AAFTaggedValueTest::ReferencedMob"));

		// Add some slots
		for(test = 0; test < 5; test++)
		{
 			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&sclp));		
			// Set the properties for the SourceClip
			sourceRef.sourceID = TEST_referencedMobID;
			sourceRef.sourceSlotID = 0;
			sourceRef.startTime = 0;
			IAAFDataDefSP pDataDef;
			checkResult(pDictionary->LookupDataDef(*slotDDefs[test], &pDataDef));
			checkResult(sclp->Initialize(pDataDef, slotsLength[test], sourceRef));

			checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
			
			aafRational_t editRate = { 0, 1};
			checkResult(pMob->AppendNewTimelineSlot (editRate,
													 seg,
													 test+1,
													 slotNames[test],
													 0,
													 &newSlot));

			newSlot->Release();
			newSlot = NULL;

			seg->Release();
			seg = NULL;

			sclp->Release();
			sclp = NULL;
		}

		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pRMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (newSlot)
    newSlot->Release();

  if (seg)
    seg->Release();

  if (sclp)
    sclp->Release();

  if (pCompMob)
	  pCompMob->Release();

  if (pMob)
    pMob->Release();

  if (pReferencedMob)
	  pReferencedMob->Release();

  if (pRMob)
	  pRMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
	  {
		  pFile->Save();
		  pFile->Close();
	  }
    pFile->Release();
  }

  return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IEnumAAFTaggedValues* pCommentIterator = NULL;
	IAAFTaggedValue*		pComment = NULL;

	IAAFMobSlot		*slot = NULL;
	aafNumSlots_t	numMobs, n, slt;
	aafUInt32		numComments, bytesRead, com;
	HRESULT						hr = S_OK;
	aafWChar		tag[64];
	aafWChar		Value[64];
	aafSearchCrit_t	criteria;

	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
  		checkResult(pFile->GetHeader(&pHeader));

		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;


		checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);


		checkResult(pHeader->GetMobs (&criteria, &mobIter));

		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;

			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));

			// Check for comments
			checkResult(aMob->CountComments(&numComments));
			checkExpression(1 == numComments, AAFRESULT_TEST_FAILED);
			checkResult(aMob->GetComments(&pCommentIterator));
			for(com = 0; com < numComments; com++)
			{
				checkResult(pCommentIterator->NextOne(&pComment));
				checkResult(pComment->GetName(tag, sizeof(tag)));
				checkResult(pComment->GetValue( sizeof(Value), (unsigned char *)Value, &bytesRead));
				checkExpression(wcscmp(tag, TagNames)== 0, AAFRESULT_TEST_FAILED);
				checkExpression(wcscmp(Value, AltComment)== 0, AAFRESULT_TEST_FAILED);
				pComment->Release();
			}
			pCommentIterator->Release();
			
			checkResult(aMob->CountSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);

			checkResult(aMob->GetSlots(&slotIter));

			for(slt = 0; slt < numSlots; slt++)
			{
				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetName (slotName, sizeof(slotName)));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (wcscmp(slotName, slotNames[slt]) == 0, AAFRESULT_TEST_FAILED);

				slot->Release();
				slot = NULL;
			}

			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
  if (slot)
    slot->Release();

  if (slotIter)
    slotIter->Release();

  if (aMob)
    aMob->Release();

  if (mobIter)
    mobIter->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
      pFile->Close();
     pFile->Release();
  }

  return hr;
}
 

extern "C" HRESULT CAAFTaggedValue_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTaggedValue_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CTaggedValues_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}










  // 
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppTypeDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\, it will return the first one
  // encountered in the order given below:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  \(This is the only code indicating success.\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\(\) called on it.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef arg is NULL.)






/****/




