// @doc INTERNAL
// @com This file implements the module test for CAAFEffect

/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/

#include "CAAFEffect.h"
#include "CAAFEffect.h"
#ifndef __CAAFEffect_h__
#error - improperly defined include guard
#endif

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEffect; // generated


#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "aafUtils.h"

static aafUID_t	zeroID = { 0 };
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
#define TEST_CATEGORY		L"Test Parameters"
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMasterMob Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	if(mode == kMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
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
	IAAFEffectDef*		pEffectDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	IAAFEffect			*pEffect = NULL;
	IAAFMob				*pMob = NULL;
	IAAFSegment			*pSeg = NULL;
	IAAFTimelineMobSlot	*pSlot = NULL;
	IAAFParameter		*pParm = NULL;
	IAAFSegment			*pFiller = NULL;
	IAAFComponent		*pComponent = NULL;
	IAAFSourceClip		 *pSourceClip = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			testDataDef = DDEF_Video;
	aafLength_t			effectLen = TEST_EFFECT_LEN;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
/*	long				test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
    
		checkResult(pDictionary->CreateInstance(&AUID_AAFEffectDef,
							  IID_IAAFEffectDef, 
							  (IUnknown **)&pEffectDef));
    
		checkResult(pDictionary->CreateInstance(&AUID_AAFParameterDef,
							  IID_IAAFParameterDef, 
							  (IUnknown **)&pParamDef));

		checkResult(pDictionary->RegisterEffectDefinition(pEffectDef));
		checkResult(pDictionary->RegisterParameterDefinition(pParamDef));

		checkResult(pEffectDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Init (&effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		pDefObject->Release();
		pDefObject = NULL;

//!!!Not testing the SetAUID on AAFDefObject
		checkResult(pEffectDef->SetDataDefinitionID (&testDataDef));
		checkResult(pEffectDef->SetIsTimeWarp (AAFFalse));
		checkResult(pEffectDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pEffectDef->SetCategory (TEST_CATEGORY));
		checkResult(pEffectDef->AddParameterDefs (pParamDef));
		checkResult(pEffectDef->SetBypass (TEST_BYPASS));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Init (&parmID, TEST_PARAM_NAME, TEST_PARAM_DESC));
		pDefObject->Release();
		pDefObject = NULL;


		//Make the first mob
		long	test;
		aafRational_t	videoRate = { 2997, 100 };

		// Create a Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
							  IID_IAAFMob, 
							  (IUnknown **)&pMob));

//		checkResult(CoCreateGuid((GUID *)&newUID));
//		checkResult(pMob->SetMobID(&newUID));
		checkResult(pMob->SetName(L"GroupTest"));
	  
		// Add some slots
		for(test = 0; test < 2; test++)
		{
 			checkResult(pDictionary->CreateInstance(&AUID_AAFEffect,
							     IID_IAAFEffect, 
							     (IUnknown **)&pEffect));
			
			checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
							     IID_IAAFSegment, 
							     (IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(&effectLen));
			checkResult(pComponent->SetDataDef(&testDataDef));
 			checkResult(pEffect->Initialize(&testDataDef, TEST_EFFECT_LEN, pEffectDef));

			checkResult(pDictionary->CreateInstance(&AUID_AAFParameter,
							  IID_IAAFParameter, 
							  (IUnknown **)&pParm));
			checkResult(pParm->SetParameterDefinition (pParamDef));
 // !!!  ImplAAFParameter::SetTypeDefinition (ImplAAFTypeDef*  pTypeDef)
			checkResult(pEffect->AddNewParameter (pParm));
			checkResult(pEffect->AppendNewInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;

			checkResult(pEffect->SetBypassOverride (1));
			checkResult(pDictionary->CreateInstance(&AUID_AAFSourceClip,
							  IID_IAAFSourceClip, 
							  (IUnknown **)&pSourceClip));
			aafSourceRef_t	sourceRef;
			sourceRef.sourceID = zeroID;
			sourceRef.sourceSlotID = 0;
			sourceRef.startTime = 0;
			checkResult(pSourceClip->Initialize (&testDataDef,&effectLen, sourceRef));
			checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
			checkResult(pEffect->SetRender (pSourceRef));
			checkResult(pEffect->QueryInterface (IID_IAAFSegment, (void **)&pSeg));

			checkResult(pMob->AppendNewTimelineSlot (videoRate, pSeg, test+1, slotNames[test], 0, &pSlot));

			pSlot->Release();
			pSlot = NULL;

			pSeg->Release();
			pSeg = NULL;

			pEffect->Release();
			pEffect = NULL;
			pParm->Release();
			pParm = NULL;
			pComponent->Release();
			pComponent = NULL;
			pSourceRef->Release();
			pSourceRef = NULL;
			pSourceClip->Release();
			pSourceClip = NULL;
		}

		// Add the mob to the file.
		checkResult(pHeader->AppendMob(pMob));
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
	if (pDefObject)
		pDefObject->Release();
	if (pEffect)
		pEffect->Release();
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
	if (pFiller)
		pFiller->Release();

	if (pEffectDef)
		pEffectDef->Release();

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
	IEnumAAFEffectDefs *pEffectEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IAAFEffectDef		*pEffectDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFParameter		*pParameter = NULL;
	IAAFDefObject*		pDefObject = NULL;
	IAAFSegment*		pSeg = NULL;
	IAAFEffect*			pEffect = NULL;
	IEnumAAFMobs		*mobIter = NULL;
	IAAFMob*			pMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFFiller*			pFill = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	bool				bFileOpen = false;
	aafUID_t			readSourceID;
	aafBool				readIsTimeWarp;
	aafInt32			catLen, checkNumInputs, testNumSources, testNumParam;
	aafUInt32			checkBypass, testLen;
	HRESULT				hr = S_OK;
	wchar_t				checkCat[256], checkName[256];
	aafNumSlots_t		s;
	aafNumSlots_t	numSlots;
	aafUInt32			readOverride;
	aafBool				readValidTransition;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;
		
		aafSearchCrit_t		criteria;
		criteria.searchTag = kNoSearch;
		checkResult(pHeader->EnumAAFAllMobs (&criteria, &mobIter));
				
		checkResult(mobIter->NextOne (&pMob));			
		checkResult(pMob->EnumAAFAllMobSlots(&slotIter));
		checkResult(pMob->GetNumSlots (&numSlots));
		
		for(s = 0; s < numSlots; s++)
		{
			checkResult(slotIter->NextOne (&pSlot));
			checkResult(pSlot->GetSegment (&pSeg));
			checkResult(pSeg->QueryInterface (IID_IAAFEffect, (void **)&pEffect));
			pSeg->Release();
			pSeg = NULL;

			checkResult(pEffect->GetNumSourceSegments(&testNumSources));
			checkExpression(testNumSources == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
			checkResult(pEffect->GetNumParameters(&testNumParam));
			checkExpression(testNumSources == 1, AAFRESULT_TEST_FAILED);

			checkResult(pEffect->IsATimeWarp (&readIsTimeWarp));
			checkExpression(readIsTimeWarp == AAFFalse, AAFRESULT_TEST_FAILED);

			checkResult(pEffect->GetBypassOverride (&readOverride));
			checkExpression(readOverride == 1, AAFRESULT_TEST_FAILED);

			checkResult(pEffect->IsValidTranEffect (&readValidTransition));
			checkExpression(readValidTransition == AAFFalse, AAFRESULT_TEST_FAILED);
			/**/
			checkResult(pEffect->GetIndexedInputSegment (0, &pSeg));
 			checkResult(pSeg->QueryInterface(IID_IAAFFiller, (void **) &pFill));
			/**/
			checkResult(pEffect->GetParameterByArgID (kTestParmID, &pParameter));
			checkResult(pParameter->GetParameterDefinition(&pParmDef));
			checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
			checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_PARAM_NAME) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_PARAM_DESC) == 0, AAFRESULT_TEST_FAILED);
			pDefObject->Release();
			pDefObject = NULL;
			pParmDef->Release();
			pParmDef = NULL;

			/**/
			checkResult(pEffect->GetEffectDefinition(&pEffectDef));
			checkResult(pEffectDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
			checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_EFFECT_NAME) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
			checkExpression(wcscmp(checkName, TEST_EFFECT_DESC) == 0, AAFRESULT_TEST_FAILED);
			pDefObject->Release();
			pDefObject = NULL;
			checkResult(pEffectDef->GetCategoryBufLen (&catLen));
			testLen = wcslen(TEST_CATEGORY);
			checkResult(pEffectDef->GetCategory (checkCat, sizeof(checkCat)));
			checkExpression(wcscmp(checkCat, TEST_CATEGORY) == 0, AAFRESULT_TEST_FAILED);
			checkExpression(testLen == wcslen(checkCat), AAFRESULT_TEST_FAILED);
			checkResult(pEffectDef->GetBypass (&checkBypass));
			checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
			checkResult(pEffectDef->GetNumberInputs (&checkNumInputs));
			checkExpression(checkNumInputs == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
			pEffectDef->Release();
			pEffectDef = NULL;
			/**/
			checkResult(pEffect->GetRender (&pSourceRef));
			checkResult(pSourceRef->GetSourceID (&readSourceID));
			checkExpression(EqualAUID(&readSourceID, &zeroID) == AAFTrue, AAFRESULT_TEST_FAILED);
			pEffect->Release();
			pEffect = NULL;
			pSlot->Release();
			pSlot = NULL;
			pSeg->Release();
			pSeg = NULL;
			pFill->Release();
			pFill = NULL;
			pParameter->Release();
			pParameter = NULL;
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
      
	if (pEffect)
		pEffect->Release();
      
	if (pDictionary)
		pDictionary->Release();
      
	if (pEffectEnum)
		pEffectEnum->Release();

	if (pParameter)
		pParameter->Release();

	if (pParmDefEnum)
		pParmDefEnum->Release();
      
	if (pEffectDef)
		pEffectDef->Release();
      
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
 

HRESULT CAAFEffect::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"GroupTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
//		if (SUCCEEDED(hr))
//			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFEffect::test...Caught general C++ exception!" << endl; 
	}

	return hr;
}
