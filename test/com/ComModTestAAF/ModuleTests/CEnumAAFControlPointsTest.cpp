// @doc INTERNAL
// @com This file implements the module test for CEnumAAFControlPoints
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



#include "AAF.h"


#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFInterpolatorDefs.h"
#include "AAFTypeDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafMobID_t	zeroMobID = { 0 };
static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

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

#define TEST_NUM_INPUTS		1
static const aafUID_t TEST_CATEGORY = 
{ 0x9f0e730c, 0xbf8, 0x11d4, { 0xa3, 0x58, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafRational_t kTestLevel1 = { 1, 1 };
const aafRational_t kTestLevel2 = { 0, 1 };

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFControlPoints Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	if(mode == kAAFMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

	if (FAILED(hr))
	{
	  if ( *ppFile ) {
	    (*ppFile)->Release();
	    *ppFile = NULL;
	  }
	  return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFOperationDef*		pOperationDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	IAAFOperationGroup			*pOperationGroup = NULL;
	IAAFMob				*pMob = NULL;
	IAAFSegment			*pSeg = NULL;
	IAAFTimelineMobSlot	*pSlot = NULL;
	IAAFParameter		*pParm = NULL;
	IAAFVaryingValue		*pVaryingValue = NULL;
	IAAFSegment			*pFiller = NULL;
	IAAFComponent		*pComponent = NULL;
	IAAFSourceClip		 *pSourceClip = NULL;
	IAAFControlPoint	 *pControlPoint = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	IAAFInterpolationDef *pInterpDef = NULL;
	IAAFPluginManager	*pMgr = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafLength_t			effectLen = TEST_EFFECT_LEN;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
	aafRational_t		testLevel1 = kTestLevel1;
	aafRational_t		testLevel2 = kTestLevel2;
/*	long				test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
    
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));
		checkResult(pDictionary->LookupTypeDef (kAAFTypeID_Rational, &pTypeDef));
		checkResult(AAFGetPluginManager(&pMgr));
		checkResult(pMgr->CreatePluginDefinition(LinearInterpolator, pDictionary, &pDefObject));
		checkResult(pDefObject->QueryInterface(IID_IAAFInterpolationDef, (void **) &pInterpDef));
		pDefObject->Release();
		pDefObject = NULL;

		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pParamDef->Initialize (parmID, TEST_PARAM_NAME, TEST_PARAM_DESC, pTypeDef));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		checkResult(pDictionary->RegisterInterpolationDef(pInterpDef));


		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));

		//Make the first mob
		long	test;
		aafRational_t	videoRate = { 2997, 100 };
		aafRational_t	startTime = { 0, 1 } , endTime = { 1, 1};
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
			checkResult(pComponent->SetLength(effectLen));
			checkResult(pComponent->SetDataDef(defs.ddPicture()));
 			checkResult(pOperationGroup->Initialize(defs.ddPicture(),
													TEST_EFFECT_LEN,
													pOperationDef));

			checkResult(defs.cdVaryingValue()->
						CreateInstance(IID_IAAFVaryingValue, 
									   (IUnknown **)&pVaryingValue));
			checkResult(pVaryingValue->Initialize (pParamDef, pInterpDef));

			checkResult(defs.cdControlPoint()->
						CreateInstance(IID_IAAFControlPoint, 
									   (IUnknown **)&pControlPoint));
			checkResult(pControlPoint->Initialize (pVaryingValue, startTime, sizeof(testLevel1), (aafDataBuffer_t)&testLevel1));
			checkResult(pVaryingValue->AddControlPoint(pControlPoint));
			pControlPoint->Release();
			pControlPoint = NULL;

			checkResult(defs.cdControlPoint()->
						CreateInstance(IID_IAAFControlPoint, 
									   (IUnknown **)&pControlPoint));
			checkResult(pControlPoint->Initialize (pVaryingValue, endTime, sizeof(testLevel2), (aafDataBuffer_t)&testLevel2));
			checkResult(pVaryingValue->AddControlPoint(pControlPoint));
			pControlPoint->Release();
			pControlPoint = NULL;

			checkResult(pVaryingValue->QueryInterface (IID_IAAFParameter, (void **)&pParm));
			checkResult(pOperationGroup->AddParameter (pParm));

			checkResult(pOperationGroup->AppendInputSegment (pFiller));
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
			checkResult(pSourceClip->Initialize (defs.ddPicture(),
												 effectLen,
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
			pParm->Release();
			pParm = NULL;
			pVaryingValue->Release();
			pVaryingValue = NULL;
			pComponent->Release();
			pComponent = NULL;
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
	if(pControlPoint)
		pControlPoint->Release();
	if(pSourceClip)
		pSourceClip->Release();
	if (pDefObject)
		pDefObject->Release();
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

	if (pParm)
		pParm->Release();
	if (pInterpDef)
		pInterpDef->Release();
	if (pVaryingValue)
		pVaryingValue->Release();
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
	if(pMgr)
		pMgr->Release();
	if(pTypeDef)
		pTypeDef->Release();
      
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
	IEnumAAFControlPoints *pEnumCP = NULL;
	IEnumAAFControlPoints *pCloneEnum = NULL;
	IAAFControlPoint	*pControlPoint = NULL;
	IAAFVaryingValue	*pVaryingValue = NULL;
	IAAFInterpolationDef	*pInterpDef = NULL;
	bool				bFileOpen = false;
	aafBool				readIsTimeWarp;
	aafUInt32			testNumSources, testNumParam;
	HRESULT				hr = S_OK;
	aafNumSlots_t		s;
	aafNumSlots_t	numSlots;
	aafUInt32			readOverride;
	aafBool				readValidTransition;
	IAAFControlPoint*	pArray[2] = { NULL, NULL };
	IAAFControlPoint**	pArrayPoint = pArray;
	aafUInt32			resultCount;
	
	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;
		
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
			checkExpression(testNumSources == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
			checkResult(pOperationGroup->CountParameters(&testNumParam));
			checkExpression(testNumSources == 1, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsATimeWarp (&readIsTimeWarp));
			checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->GetBypassOverride (&readOverride));
			checkExpression(readOverride == 1, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsValidTranOperation (&readValidTransition));
			checkExpression(readValidTransition == kAAFFalse, AAFRESULT_TEST_FAILED);
			/**/
			checkResult(pOperationGroup->GetInputSegmentAt (0, &pSeg));
			checkResult(pSeg->QueryInterface(IID_IAAFFiller, (void **) &pFill));
			pFill->Release();
			pFill = NULL;
			/**/
			checkResult(pOperationGroup->LookupParameter (kTestParmID, &pParameter));
			checkResult(pParameter->QueryInterface(IID_IAAFVaryingValue, (void **) &pVaryingValue));
			
			aafUInt32		testLen, bytesRead;
			aafRational_t	sampleValue1 = kTestLevel1, sampleValue2 = kTestLevel2, testValue;
			
			/* Read and check the first element */
			checkResult(pVaryingValue->GetControlPoints(&pEnumCP));
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue1), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue1.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue1.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;

			/**/
			/* Read and check the second element */
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue2), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue2.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue2.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;
			/*****/
			
			/* Reset, and check the first element again*/
			checkResult(pEnumCP->Reset());
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue1), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue1.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue1.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;
			
			/* Reset, Skip, and check the second element again*/
			checkResult(pEnumCP->Reset());
			checkResult(pEnumCP->Skip(1));
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue2), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue2.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue2.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;

			/* Reset, and read both elements */
			checkResult(pEnumCP->Reset());
			checkResult(pEnumCP->Next (2, (IAAFControlPoint **)&pArray, &resultCount));
			checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
			checkResult(pArrayPoint[0]->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pArrayPoint[0]->GetValue (sizeof(sampleValue1), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue1.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue1.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			pArrayPoint[0]->Release();
			pArrayPoint[0] = NULL;
			
			checkResult(pArrayPoint[1]->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			checkResult(pArrayPoint[1]->GetValue (sizeof(sampleValue2), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue2.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue2.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			pArrayPoint[1]->Release();
			pArrayPoint[1] = NULL;
			
			/* Read one past to make sure that it fails */
			checkExpression(pEnumCP->NextOne(&pControlPoint) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
			/* Clone the enumerator, and read one element */
			checkResult(pEnumCP->Clone(&pCloneEnum));
			checkResult(pCloneEnum->Reset());
			checkResult(pCloneEnum->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
			checkExpression(testLen == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue1), (aafDataBuffer_t)&testValue, &bytesRead));
			checkExpression(testValue.numerator == sampleValue1.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testValue.denominator == sampleValue1.denominator, AAFRESULT_TEST_FAILED);
			checkExpression(bytesRead == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;
			pCloneEnum->Release();
			pCloneEnum = NULL;

			pEnumCP->Release();
			pEnumCP = NULL;
			pSeg->Release();
			pSeg = NULL;
			pOperationGroup->Release();
			pOperationGroup = NULL;
			pSlot->Release();
			pSlot = NULL;
			pVaryingValue->Release();
			pVaryingValue = NULL;
			pParameter->Release();
			pParameter = NULL;
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
	
	if(pInterpDef)
		pInterpDef->Release();
	if(pVaryingValue)
		pVaryingValue->Release();
	if (pSourceRef)
		pSourceRef->Release();
	if (pCloneEnum)
		pCloneEnum->Release();
	
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
	
	if (pControlPoint)
		pControlPoint->Release();
	
	if(pEnumCP)
		pEnumCP->Release();
	
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


extern "C" HRESULT CEnumAAFControlPoints_test(testMode_t mode);
extern "C" HRESULT CEnumAAFControlPoints_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFControlPointsTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFControlPoints_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}












