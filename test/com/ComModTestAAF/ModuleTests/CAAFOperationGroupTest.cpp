// @doc INTERNAL
// @com This file implements the module test for CAAFOperationGroup

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
* prior written permission of Avid Technology, Inc.
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

#include "AAF.h"


#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

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
#define TEST_CATEGORY		L"Test Parameters"
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafRational_t kTestLevel = { 1, 2 };
const aafRational_t kTestLevel2 = { 22, 7 };

const aafString_t  TEST_PARAM_NAME [2]	=	{L"A TestEffect parameter", L"aprox PI"};
const aafString_t  TEST_PARAM_DESC [2]	=	{L"A longer description of the TestEffect parameter",
												L"An aproximation of PI"};
const aafString_t  TEST_PARAM_UNITS[2]	=   {L"Furlongs per Fortnight", L"PI fractional members"};

aafUID_t kTestParm2ID = {0x14b66cc5, 0x1a1, 0x11d4, { 0x80, 0x46, 0x8, 0x0, 0x36, 0x21, 0x8, 0x4 } };

typedef IAAFSmartPointer<IAAFParameterDef>					IAAFParameterDefSP;
typedef IAAFSmartPointer<IEnumAAFParameters>				IEnumAAFParametersSP;
typedef IAAFSmartPointer<IAAFParameter>						IAAFParameterSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;

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
	ProductInfo.productName = L"AAFOperationGroup Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	*ppFile = NULL;
	
	if(mode == kAAFMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
	
	if (FAILED(hr))
	{
		if (*ppFile)
		{
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


	aafUInt32 num_fetched = 0;
	//make sure 2 parameters can be fetched
	checkResult(spEnumParams->Next(2, &spParameter, &num_fetched));
	checkExpression(num_fetched == 2, AAFRESULT_TEST_FAILED);
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

static HRESULT CreateAAFFile(aafWChar * pFileName)
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
	aafLength_t			effectLen = TEST_EFFECT_LEN;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
	aafInt32			numSegments;
	aafRational_t		testLevel = kTestLevel;
	
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
		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
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
			checkResult(pComponent->SetLength(effectLen));
			checkResult(pComponent->SetDataDef(defs.ddPicture()));
			checkResult(pOperationGroup->Initialize(defs.ddPicture(),
				TEST_EFFECT_LEN,
				pOperationDef));
			
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


			// filler ....
			checkResult(pOperationGroup->AppendInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;
			
			// Create another input segment, count segments, then delete & recount
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFSegment, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(effectLen));
			checkResult(pComponent->SetDataDef(defs.ddPicture()));
			checkResult(pOperationGroup->AppendInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;
			checkResult(pOperationGroup->CountSourceSegments (&numSegments));
			checkExpression(2 == numSegments, AAFRESULT_TEST_FAILED);
			checkResult(pOperationGroup->RemoveInputSegmentAt (1));
			checkResult(pOperationGroup->CountSourceSegments (&numSegments));
			checkExpression(1 == numSegments, AAFRESULT_TEST_FAILED);
			
			
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
	aafUInt32			catLen;
	aafInt32			checkNumInputs, testNumSources, testNumParam;
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
			checkExpression(testNumParam == 2, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsATimeWarp (&readIsTimeWarp));
			checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->GetBypassOverride (&readOverride));
			checkExpression(readOverride == 1, AAFRESULT_TEST_FAILED);
			
			checkResult(pOperationGroup->IsValidTranOperation (&readValidTransition));
			checkExpression(readValidTransition == kAAFFalse, AAFRESULT_TEST_FAILED);
			/**/
			checkResult(pOperationGroup->GetInputSegmentAt (0, &pSeg));
			checkResult(pSeg->QueryInterface(IID_IAAFFiller, (void **) &pFill));
			/**/

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
			checkResult(pOperationDef->GetCategoryBufLen (&catLen));
			testLen = wcslen(TEST_CATEGORY);
			checkResult(pOperationDef->GetCategory (checkCat, sizeof(checkCat)));
			checkExpression(wcscmp(checkCat, TEST_CATEGORY) == 0, AAFRESULT_TEST_FAILED);
			checkExpression(testLen == wcslen(checkCat), AAFRESULT_TEST_FAILED);
			checkResult(pOperationDef->GetBypass (&checkBypass));
			checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
			checkResult(pOperationDef->GetNumberInputs (&checkNumInputs));
			checkExpression(checkNumInputs == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
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
			pSeg->Release();
			pSeg = NULL;
			pFill->Release();
			pFill = NULL;
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


extern "C" HRESULT CAAFOperationGroup_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFOperationGroupTest.aaf";
	
	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFOperationGroup_test...Caught general C++ exception!" << endl; 
	}
	
	return hr;
}
