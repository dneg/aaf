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

#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include <assert.h>

static const aafWChar *slotName = L"SLOT1";

static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xb9768af0, 0x03fd, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const	aafMobID_t	TEST_referencedMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc2fff2f0, 0x03fd, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


namespace {

// The remove test removes the first comment and attribute. The test
// uses this index to compute an index into these arrays taking into
// account that the initial entry was removed.

const int nameValuePairOffset = 1;

const aafCharacter* AttributeNames[]  = { L"Star Count", L"Mob Count" };
const aafCharacter* AttributeValues[] = { L"Billions and Billions", L"Thousands and Thousands" };

const aafCharacter* CommentNames[]   = { L"Component Comment A Name", L"Component Comment B Name" };
const aafCharacter* CommentValues[] = { L"Component Comment A Value", L"Component Comment B Value" };

};

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

#define TEST_DDEF	kAAFDataDef_Sound
#define TEST_LENGTH	42L

void testCommentAttributeMethods(IAAFComponent2 *pComp2)
{
	//test remove methods for comments and attributes
	IEnumAAFTaggedValues* pEnum = NULL;
  	IAAFTaggedValue* pTagVal = NULL;

	//test RemoveComment - remove the very first comment
	pComp2->GetComments(&pEnum);
  	pEnum->NextOne(&pTagVal); 

	if(pComp2->RemoveComment(pTagVal) != AAFRESULT_SUCCESS)
		checkResult(AAFRESULT_TEST_FAILED);

	pTagVal->Release();
	pTagVal = NULL;
	pEnum->Release();
	pEnum = NULL;

	//test RemoveAttribute - remove the very first attribute
	pComp2->GetAttributes(&pEnum);
  	pEnum->NextOne(&pTagVal);
		
	if(pComp2->RemoveAttribute(pTagVal) != AAFRESULT_SUCCESS)
		checkResult(AAFRESULT_TEST_FAILED);

	//test count methods for comments and attributes
	aafUInt32 numComments;
	aafUInt32 numAttributes;
		
	if(pComp2->CountComments(&numComments) != AAFRESULT_SUCCESS)
		checkResult(AAFRESULT_TEST_FAILED);

	if(pComp2->CountAttributes(&numAttributes) != AAFRESULT_SUCCESS)
		checkResult(AAFRESULT_TEST_FAILED);

	//initially there were 2 comments and 2 attributes, but we deleted one of each
	if(numComments != 1 || numAttributes != 1)
		checkResult(AAFRESULT_TEST_FAILED);

	pTagVal->Release();
	pTagVal = NULL;
	pEnum->Release();
	pEnum = NULL;
}

void CheckNameValuePairs( IEnumAAFTaggedValues* pEnum, 
			  const aafCharacter* nameArray[], 
			  const aafCharacter* valueArray[],
			  int nameValArraySize )
{			  
  // "name" is 500 chars long... sized more than
  // large enough for a simple test.
  aafCharacter strbuf[500];
  int count = 0;
  HRESULT atthr = AAFRESULT_SUCCESS;

  IAAFTaggedValue* pTagVal = 0;
  for( atthr = pEnum->NextOne( &pTagVal );
       atthr == AAFRESULT_SUCCESS;
       atthr = pEnum->NextOne( &pTagVal ) ) {
 
    /// check the name

    aafUInt32 bufLen = 0;
    checkResult( pTagVal->GetNameBufLen(&bufLen) );
   
    checkExpression( count < nameValArraySize, AAFRESULT_TEST_FAILED );
    checkResult( pTagVal->GetName( strbuf, bufLen ) );
    //comment @ fist index was deleted so check @ second index
    checkExpression( wcscmp(strbuf, nameArray[nameValuePairOffset+count] ) == 0, AAFRESULT_TEST_FAILED ); 

    // check the value,    
    IAAFTypeDef* pTypeDef = NULL;
    checkResult( pTagVal->GetTypeDefinition( &pTypeDef ) );
    // It should be a string.
    IAAFTypeDefString* pTDString = NULL;
    checkResult( pTypeDef->QueryInterface( IID_IAAFTypeDefString, reinterpret_cast<void**>(&pTDString) ) );
    checkResult( pTagVal->GetValue( sizeof(strbuf),
				    reinterpret_cast<aafDataBuffer_t>(strbuf), &bufLen ) );
    //value @ fist index was deleted so check @ second index
    checkExpression( wcscmp( valueArray[nameValuePairOffset+count], strbuf ) == 0, AAFRESULT_TEST_FAILED );

    pTypeDef->Release();
    pTypeDef = NULL;
    
    pTDString->Release();
    pTDString = NULL;
    
    pTagVal->Release();
    pTagVal = NULL;
    
    count++;
  }

  //since we removed one attribute & comment count is count--
  checkExpression( count == nameValArraySize - nameValuePairOffset, AAFRESULT_TEST_FAILED );
}

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	// IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFTimelineMobSlot*		newSlot = NULL;
	IAAFComponent*				comp = NULL;
	IAAFSegment*				seg = NULL;
	IAAFComponent2*             pComp2 = NULL;
	IAAFDataDef *               pDataDef = 0;
	aafLength_t					testLength = TEST_LENGTH;
	bool bFileOpen = false;
	aafUID_t					dataDef = TEST_DDEF;
	HRESULT						hr = AAFRESULT_SUCCESS;

	try
	{
	    // Remove the previous test file if any.
	    RemoveTestFile(pFileName);

		// Create the file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
 		
		CAAFBuiltinDefs defs (pDictionary);

		//Make the MOB to be referenced
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pReferencedMob));
		checkResult(pReferencedMob->SetMobID(TEST_referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));

		// Create a Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob,
								   (IUnknown **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFSourceClipTest"));

		// Create a SourceClip
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSegment, 
								   (IUnknown **)&seg));
		checkResult(seg->QueryInterface (IID_IAAFComponent,
                                          (void **)&comp));
		checkResult(pDictionary->LookupDataDef(dataDef, &pDataDef));
		checkResult(comp->SetDataDef (pDataDef));
		pDataDef->Release ();
		pDataDef = 0;
		checkResult(comp->SetLength (testLength));
		comp->Release();
		comp = NULL;
								 		
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot (editRate, seg, 1, slotName, 0, &newSlot));

		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pReferencedMob));

		// Get the IAAFComponent2 interface from the
		// SourceClip object and test the attribute and user
		// comment methods.
		checkResult( seg->QueryInterface( IID_IAAFComponent2, reinterpret_cast<void**>(&pComp2) ) );
		checkResult( pComp2->AppendAttribute( AttributeNames[0], AttributeValues[0] ) );
		checkResult( pComp2->AppendAttribute( AttributeNames[1], AttributeValues[1] ) );
		checkResult( pComp2->AppendComment( CommentNames[0], CommentValues[0] ) );
		checkResult( pComp2->AppendComment( CommentNames[1], CommentValues[1] ) );

		testCommentAttributeMethods(pComp2);
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  // Cleanup and return
	if (newSlot)
	  {
		newSlot->Release();
		newSlot = 0;
	  }

	if (pComp2)
	  {
		pComp2->Release();
		pComp2 = 0;
	  }
	if (seg)
	  {
		seg->Release();
		seg = 0;
	  }

	if (comp)
	  {
		comp->Release();
		comp = 0;
	  }

	if (pMob)
	  {
		pMob->Release();
		pMob = 0;
	  }

	if (pReferencedMob)
	  {
		pReferencedMob->Release();
		pReferencedMob = 0;
	  }

	if (pDictionary)
	  {
		pDictionary->Release();
		pDictionary = 0;
	  }

	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release ();
		pDataDef = 0;
	  }

	if (pFile) 
	{
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
		pFile->Release();
		pFile = 0;
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFComponent*				comp = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFComponent2*                         pComp2 = NULL;
	IAAFDataDef *               pDataDef = 0;
	IAAFDefObject *             pDefObj = 0;
	aafLength_t					testLength;
	aafUID_t					testUID, checkUID = TEST_DDEF;
	bool bFileOpen = false;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	HRESULT						hr = AAFRESULT_SUCCESS;

	try
	{ 
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(2 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			aafMobID_t	debugMobID;

			checkResult(pMob->GetMobID(&debugMobID));
			checkResult(pMob->CountSlots(&numSlots));
			if(0 != numSlots)	// numSlots == 0 on referenced mob
			{
				checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
				
				checkResult(pMob->GetSlots(&pSlotIter));
				while (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
				{
					// The segment should be a source clip, and should
				        // support IAAFComponent2.
					checkResult(pSlot->GetSegment(&pSegment));
					checkResult(pSegment->QueryInterface (IID_IAAFComponent,
						(void **)&comp));
					checkResult(pSegment->QueryInterface (IID_IAAFComponent2,
						(void **)&pComp2));
					checkResult(pSegment->QueryInterface (IID_IAAFSourceClip,
						(void **)&pSourceClip));
					checkResult(pComp2->GetDataDef (&pDataDef));
					checkResult(pDataDef->QueryInterface (IID_IAAFDefObject,
						(void **)&pDefObj));
					checkResult(pDefObj->GetAUID (&testUID));
					pDataDef->Release ();
					pDataDef = 0;
					pDefObj->Release ();
					pDefObj = 0;
					checkExpression(memcmp(&testUID, &checkUID, sizeof(testUID)) == 0, 						AAFRESULT_TEST_FAILED);
					checkResult(pComp2->GetLength (&testLength));
					checkExpression(TEST_LENGTH == testLength, AAFRESULT_TEST_FAILED);


					// Verify the attributes and comments.
					IEnumAAFTaggedValues* pEnumComments = NULL;
					checkResult( pComp2->GetComments( &pEnumComments ) );
					CheckNameValuePairs( pEnumComments, CommentNames, CommentValues, 
							     sizeof(CommentNames)/sizeof(CommentNames[0]) );
					pEnumComments->Release();
					pEnumComments = NULL;


					IEnumAAFTaggedValues* pEnumAttributes = NULL;
					checkResult( pComp2->GetAttributes( &pEnumAttributes ) );
					CheckNameValuePairs( pEnumAttributes, AttributeNames, AttributeValues, 
							     sizeof(AttributeNames)/sizeof(AttributeNames[0]) );
					pEnumAttributes->Release();
					pEnumAttributes = NULL;

					comp->Release();
					comp = NULL;
					pSegment->Release();
					pSegment = NULL;
					pSourceClip->Release();
					pSourceClip = NULL;
					pComp2->Release();
					pComp2 = NULL;
					
					pSlot->Release();
					pSlot = NULL;
				}
			}

			pMob->Release();
			pMob = NULL;
		}
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

	// Cleanup and return
	if (pReferencedMob)
	  {
		pReferencedMob->Release();
		pReferencedMob = 0;
	  }

	if (pSourceClip)
	  {
		pSourceClip->Release();
		pSourceClip = 0;
	  }

	if (comp)
	  {
		comp->Release();
		comp = 0;
	  }

	if (pSegment)
	  {
		pSegment->Release();
		pSegment = 0;
	  }

	if (pSlot)
	  {
		pSlot->Release();
		pSlot = 0;
	  }

	if (pSlotIter)
	  {
		pSlotIter->Release();
		pSlotIter = 0;
	  }

	if (pMob)
	  {
		pMob->Release();
		pMob = 0;
	  }

	if (pMobIter)
	  {
		pMobIter->Release();
		pMobIter = 0;
	  }

	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release ();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release ();
		pDefObj = 0;
	  }

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
		pFile = 0;
	}

	return hr;
}
 


extern "C" HRESULT CAAFComponent_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFComponent_test(
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
	  cerr << "CAAFComponent_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


	return hr;
}






















