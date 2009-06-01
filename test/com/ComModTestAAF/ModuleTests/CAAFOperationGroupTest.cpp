
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafMobID_t	zeroMobID = { {0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,
					{0,0,0,{0,0,0,0,0,0,0,0}} };
static const aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

// convenient error handlers.
inline void checkResult(HRESULT r)
{
	if (FAILED(r))
		throw r;
}
inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
	if (!expression)
		throw r;
}

static const aafUID_t TEST_CATEGORY = 
{ 0x0d010102, 0x0101, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafRational_t kTestLevel = { 1, 2 };
const aafRational_t kTestLevel2 = { 22, 7 };

const aafWChar * const  TEST_PARAM_NAME [2]	=	{L"A TestEffect parameter", L"aprox PI"};
const aafWChar * const  TEST_PARAM_DESC [2]	=	{L"A longer description of the TestEffect parameter",
												L"An aproximation of PI"};
const aafWChar * const  TEST_PARAM_UNITS[2]	=   {L"Furlongs per Fortnight", L"PI fractional members"};

static const 	aafUID_t	kTestParm2ID =
{ 0x47240c2b, 0x19d, 0x11d4, { 0x8e, 0x3d, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x7c } };


typedef IAAFSmartPointer<IAAFParameterDef>					IAAFParameterDefSP;
typedef IAAFSmartPointer<IEnumAAFParameters>				IEnumAAFParametersSP;
typedef IAAFSmartPointer<IAAFParameter>						IAAFParameterSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;

static HRESULT verifyParams(IAAFOperationGroup * const pOperationGroup )
{
	//Get parameters
	IEnumAAFParametersSP  spEnumParams;
	checkResult(pOperationGroup->GetParameters(&spEnumParams));

	aafUInt32 bufferSize = 256;
	aafCharacter checkBuffer[256] = {0};

	IAAFParameterSP spParameter;
	IAAFParameterDefSP spParamDef;
	IAAFDefObjectSP spDefObject;


  // Attempt to load the parameters into an array.
	aafUInt32 num_fetched = 0;
  aafUInt32 num_released = 0;
	//make sure 2 parameters can be fetched
  IAAFParameter * parameterArray[2] = {0};
	checkResult(spEnumParams->Next(2, parameterArray, &num_fetched));
  if (parameterArray[0])
  {
    parameterArray[0]->Release();
    parameterArray[0] = NULL;
    ++num_released;
  }
  if (parameterArray[1])
  {
    parameterArray[1]->Release();
    parameterArray[1] = NULL;
    ++num_released;
  }

	checkExpression(num_fetched == 2, AAFRESULT_TEST_FAILED);
	checkExpression(num_released == 2, AAFRESULT_TEST_FAILED);

	//so far, so good.  Reset
	spEnumParams->Reset();


	HRESULT hr = AAFRESULT_TEST_FAILED;
	
	while(SUCCEEDED(hr = spEnumParams->NextOne(&spParameter)))
	{
		//Get the Param def and the Def object
		checkResult(spParameter->GetParameterDefinition(&spParamDef));
		checkResult(spParamDef->QueryInterface(IID_IAAFDefObject, (void **) &spDefObject));
		aafUID_t  auid = {0};
		checkResult(spDefObject->GetAUID(&auid));

		//Since the enumerated set is unordered, we need to set the index of our test cases appropriately
		unsigned int i = (memcmp(&auid, &kTestParmID, sizeof(auid))==0) ? 0:1;

		//Check all the values ....
		checkResult(spParamDef->GetDisplayUnits (checkBuffer, bufferSize));
		checkExpression(wcscmp(checkBuffer, TEST_PARAM_UNITS[i]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(spDefObject->GetName (checkBuffer, bufferSize));
		checkExpression(wcscmp(checkBuffer, TEST_PARAM_NAME[i]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(spDefObject->GetDescription (checkBuffer, bufferSize));
		checkExpression(wcscmp(checkBuffer, TEST_PARAM_DESC[i]) == 0, AAFRESULT_TEST_FAILED);

	}//while we've got a param

	checkExpression(hr == AAFRESULT_NO_MORE_OBJECTS, AAFRESULT_TEST_FAILED);

	return S_OK;
	
}//verifyParams()

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFOperationDef*		pOperationDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	IAAFOperationGroup			*pOperationGroup = NULL;
	IAAFMob				*pMob = NULL;
	IAAFSegment			*pSeg = NULL;
	IAAFTimelineMobSlot	*pSlot = NULL;
	IAAFConstantValue *pConstValue = NULL;
	IAAFParameter		*pParm = NULL;
	IAAFSegment			*pFiller = NULL;
	IAAFComponent		*pComponent = NULL;
	IAAFSourceClip		 *pSourceClip = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
	aafUInt32			numSegments;
	aafRational_t		testLevel = kTestLevel;
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		CAAFBuiltinDefs defs (pDictionary);
		
		checkResult(defs.cdOperationDef()->
			CreateInstance(IID_IAAFOperationDef, 
			(IUnknown **)&pOperationDef));
		
		//add 1st param
		checkResult(defs.cdParameterDef()->
			CreateInstance(IID_IAAFParameterDef, (IUnknown **)&pParamDef));	
		checkResult(pParamDef->Initialize (parmID, TEST_PARAM_NAME[0], TEST_PARAM_DESC[0], defs.tdRational()));
		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS[0]));		
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		
		//add 2nd param
		IAAFParameterDefSP spParam2Def;
		checkResult(defs.cdParameterDef()->
			CreateInstance(IID_IAAFParameterDef, (IUnknown **)&spParam2Def));
		checkResult(spParam2Def->Initialize (kTestParm2ID, TEST_PARAM_NAME[1], TEST_PARAM_DESC[1], defs.tdRational()));
		checkResult(spParam2Def->SetDisplayUnits(TEST_PARAM_UNITS[1]));
		checkResult(pDictionary->RegisterParameterDef(spParam2Def));
		
		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pOperationDef->SetDataDef (defs.ddkAAFPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (3));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));
		
		//Register params
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->AddParameterDef (spParam2Def));

		
		//Make the first mob
		long	test;
		aafRational_t	videoRate = { 2997, 100 };
		
		// Create a Mob
		checkResult(defs.cdCompositionMob()->
			CreateInstance(IID_IAAFMob, 
			(IUnknown **)&pMob));
		
		checkResult(pMob->SetName(L"AAFOperationGroupTest"));
		
		// Add some slots
		for(test = 0; test < 2; test++)
		{
			checkResult(defs.cdOperationGroup()->
				CreateInstance(IID_IAAFOperationGroup, 
				(IUnknown **)&pOperationGroup));
			
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFSegment, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(TEST_EFFECT_LEN+2));
			checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
			checkResult(pOperationGroup->Initialize(defs.ddkAAFPicture(),
				TEST_EFFECT_LEN,
				pOperationDef));

      pComponent->Release();
      pComponent = NULL;
			
			checkResult(defs.cdConstantValue()->
				CreateInstance(IID_IAAFConstantValue, 
				(IUnknown **)&pConstValue));
			checkResult(pConstValue->Initialize (pParamDef, sizeof(testLevel), (aafDataBuffer_t)&testLevel));
			checkResult(pConstValue->SetValue(sizeof(testLevel), (aafDataBuffer_t)&testLevel));
			checkResult(pConstValue->QueryInterface (IID_IAAFParameter, (void **)&pParm));

			checkResult(pOperationGroup->AddParameter (pParm));		
			
			pConstValue->Release ();
			pConstValue = NULL;
			pParm->Release();
			pParm = NULL;

			//add the 2nd value
			checkResult(defs.cdConstantValue()->
				CreateInstance(IID_IAAFConstantValue, 
				(IUnknown **)&pConstValue));
			testLevel = kTestLevel2;  //assigning level2
			checkResult(pConstValue->Initialize (spParam2Def, sizeof(testLevel), (aafDataBuffer_t)&testLevel));
			checkResult(pConstValue->SetValue(sizeof(testLevel), (aafDataBuffer_t)&testLevel));
			checkResult(pConstValue->QueryInterface (IID_IAAFParameter, (void **)&pParm));
	
			checkResult(pOperationGroup->AddParameter (pParm));			
			pConstValue->Release ();
			pConstValue = NULL;
			pParm->Release();
			pParm = NULL;


			// filler ....
			checkResult(pOperationGroup->AppendInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;
			
			// Create another input segment, count segments, then delete & recount
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFSegment, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(TEST_EFFECT_LEN+3));
			checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
			checkResult(pOperationGroup->AppendInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;
      pComponent->Release();
      pComponent = NULL;

			checkResult(pOperationGroup->CountSourceSegments (&numSegments));
			checkExpression(2 == numSegments, AAFRESULT_TEST_FAILED);
			checkResult(pOperationGroup->RemoveInputSegmentAt (1));
			checkResult(pOperationGroup->CountSourceSegments (&numSegments));
			checkExpression(1 == numSegments, AAFRESULT_TEST_FAILED);

			// Now try prepending an input segment
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFSegment, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(TEST_EFFECT_LEN));
			checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
			checkResult(pOperationGroup->PrependInputSegment (pFiller));
			pComponent->Release();
			pComponent = NULL;
			pFiller->Release();
			pFiller = NULL;

			// Now insert one in the middle
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFSegment, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(TEST_EFFECT_LEN+1));
			checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
			checkResult(pOperationGroup->InsertInputSegmentAt(1,pFiller));
			pComponent->Release();
			pComponent = NULL;
			pFiller->Release();
			pFiller = NULL;

			checkResult(pOperationGroup->SetBypassOverride (1));
			checkResult(defs.cdSourceClip()->
				CreateInstance(IID_IAAFSourceClip, 
				(IUnknown **)&pSourceClip));
			aafSourceRef_t	sourceRef;
			sourceRef.sourceID = zeroMobID;
			sourceRef.sourceSlotID = 0;
			sourceRef.startTime = 0;
			checkResult(pSourceClip->Initialize (defs.ddkAAFPicture(),
				TEST_EFFECT_LEN,
				sourceRef));
			checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
			checkResult(pOperationGroup->SetRender (pSourceRef));
			checkResult(pOperationGroup->QueryInterface (IID_IAAFSegment, (void **)&pSeg));
			
			checkResult(pMob->AppendNewTimelineSlot (videoRate, pSeg, test+1, slotNames[test], 0, &pSlot));
			
			pSlot->Release();
			pSlot = NULL;
			
			pSeg->Release();
			pSeg = NULL;
			
			pOperationGroup->Release();
			pOperationGroup = NULL;
			pSourceRef->Release();
			pSourceRef = NULL;
			pSourceClip->Release();
			pSourceClip = NULL;
		}
		
		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if(pSourceRef)
		pSourceRef->Release();
	if(pSourceClip)
		pSourceClip->Release();
	if (pOperationGroup)
		pOperationGroup->Release();
	if (pMob)
		pMob->Release();
	if (pSeg)
		pSeg->Release();
	if (pSlot)
		pSlot->Release();
	if (pComponent)
		pComponent->Release();
	if (pConstValue)   
		pConstValue->Release ();
	
	if (pParm)
		pParm->Release();
	if (pFiller)
		pFiller->Release();
	
	if (pOperationDef)
		pOperationDef->Release();
	
	if (pParamDef)
		pParamDef->Release();
	
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

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFOperationDefs *pOperationGroupEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFParameter		*pParameter = NULL;
	IAAFDefObject*		pDefObject = NULL;
	IAAFSegment*		pSeg = NULL;
	IAAFOperationGroup*			pOperationGroup = NULL;
	IEnumAAFMobs		*mobIter = NULL;
	IAAFMob*			pMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFFiller*			pFill = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	bool				bFileOpen = false;
	aafMobID_t			readSourceID;
	aafBool				readIsTimeWarp;
	aafUInt32			checkBypass, testNumSources, testNumParam;
	aafInt32			checkNumInputs;
	HRESULT				hr = S_OK;
	wchar_t				checkName[256];
	aafUID_t			checkCat;
	aafNumSlots_t		s;
	aafNumSlots_t	numSlots;
	aafUInt32			readOverride;
	aafBool				readValidTransition;
	
	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		aafSearchCrit_t		criteria;
		criteria.searchTag = kAAFNoSearch;
		checkResult(pHeader->GetMobs (&criteria, &mobIter));
		
		checkResult(mobIter->NextOne (&pMob));			
		checkResult(pMob->GetSlots(&slotIter));
		checkResult(pMob->CountSlots (&numSlots));
		
		for(s = 0; s < numSlots; s++)
		{
			checkResult(slotIter->NextOne (&pSlot));
			checkResult(pSlot->GetSegment (&pSeg));
			checkResult(pSeg->QueryInterface (IID_IAAFOperationGroup, (void **)&pOperationGroup));
			pSeg->Release();
			pSeg = NULL;
			
			checkResult(pOperationGroup->CountSourceSegments(&testNumSources));
			checkExpression(testNumSources == 3, AAFRESULT_TEST_FAILED);

			// Verify each source segment

			IAAFComponent *pComponent;

			for(aafUInt32 n=0;n<3;n++)
			{
				checkResult(pOperationGroup->GetInputSegmentAt (n, &pSeg));
				checkResult(pSeg->QueryInterface(IID_IAAFFiller,(void**)&pFill));
				checkResult(pSeg->QueryInterface(IID_IAAFComponent,
					(void**)&pComponent));
				aafLength_t Length;
				checkResult(pComponent->GetLength(&Length));
				checkExpression(Length==TEST_EFFECT_LEN+n);
				pFill->Release();
				pFill=NULL;
				pComponent->Release();
				pComponent=NULL;
				pSeg->Release();
				pSeg = NULL;
			}

			checkResult(pOperationGroup->CountParameters(&testNumParam));
			checkExpression(testNumParam == 2, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsATimeWarp (&readIsTimeWarp));
			checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->GetBypassOverride (&readOverride));
			checkExpression(readOverride == 1, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsValidTranOperation (&readValidTransition));
			checkExpression(readValidTransition == kAAFFalse, AAFRESULT_TEST_FAILED);

			//--cf  Get and Check  Parameters
			verifyParams(pOperationGroup);
			
			/**/
			checkResult(pOperationGroup->GetOperationDefinition(&pOperationDef));
			checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
			checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_EFFECT_NAME) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_EFFECT_DESC) == 0, AAFRESULT_TEST_FAILED);
			pDefObject->Release();
			pDefObject = NULL;
			checkResult(pOperationDef->GetCategory (&checkCat));
			checkExpression(memcmp(&checkCat, &TEST_CATEGORY, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pOperationDef->GetBypass (&checkBypass));
			checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
			checkResult(pOperationDef->GetNumberInputs (&checkNumInputs));
			checkExpression(checkNumInputs == 3, AAFRESULT_TEST_FAILED);
			pOperationDef->Release();
			pOperationDef = NULL;
			/**/
			checkResult(pOperationGroup->GetRender (&pSourceRef));
			checkResult(pSourceRef->GetSourceID (&readSourceID));
			checkExpression(memcmp(&readSourceID, &zeroMobID, sizeof(zeroMobID)) == 0, AAFRESULT_TEST_FAILED);
			pOperationGroup->Release();
			pOperationGroup = NULL;
			pSlot->Release();
			pSlot = NULL;
			pSourceRef->Release();
			pSourceRef = NULL;
		}
		
		slotIter->Release();
		slotIter = NULL;
		pMob->Release();
		pMob = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup and return
	if (pFill)
		pFill->Release();
	
	if (pSourceRef)
		pSourceRef->Release();
	
	if (mobIter)
		mobIter->Release();
	
	if (pSlot)
		pSlot->Release();
	
	if (slotIter)
		slotIter->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pSeg)
		pSeg->Release();
	
	if (pOperationGroup)
		pOperationGroup->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pOperationGroupEnum)
		pOperationGroupEnum->Release();
	
	if (pParameter)
		pParameter->Release();
	
	if (pParmDefEnum)
		pParmDefEnum->Release();
	
	if (pOperationDef)
		pOperationDef->Release();
	
	if (pDefObject)
		pDefObject->Release();
	
	if (pParmDef)
		pParmDef->Release();
	
	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
}


extern "C" HRESULT CAAFOperationGroup_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFOperationGroup_test(
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
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFOperationGroup_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
