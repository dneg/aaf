// @doc INTERNAL
// @com This file implements the module test for CAAFTransition
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "AAF.h"


#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

// This values are used for testing purposes
static aafUID_t    fillerUID = DDEF_Picture;
static aafLength_t  fillerLength = 3200;
static aafUID_t	zeroID = { 0 };

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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pNewSlot = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFSourceReference*		pSourceRef = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFOperationGroup*			pOperationGroup = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSegment*				pEffectFiller = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFOperationDef*				pOperationDef = NULL;
	IAAFParameter				*pParm = NULL;
	IAAFParameterDef*			pParamDef = NULL;
	IAAFDefObject*				pDefObject = NULL;
	
	aafUID_t					newMobID;
	aafUID_t					datadef = DDEF_Picture;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint = 0;
	aafUID_t					testDataDef = DDEF_Picture;
	aafLength_t					effectLen = TEST_EFFECT_LEN;
	aafUID_t					effectID = kTestEffectID;
	aafUID_t					parmID = kTestParmID;

	CvtInt32toLength(100, transitionLength);
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTransition Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;


	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
 		
		// Create the effect and parameter definitions
		checkResult(pDictionary->CreateInstance(&AUID_AAFOperationDef,
							  IID_IAAFOperationDef, 
							  (IUnknown **)&pOperationDef));
    
		checkResult(pDictionary->CreateInstance(&AUID_AAFParameterDef,
							  IID_IAAFParameterDef, 
							  (IUnknown **)&pParamDef));

		checkResult(pDictionary->RegisterOperationDefinition(pOperationDef));
		checkResult(pDictionary->RegisterParameterDefinition(pParamDef));

		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Init (&effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		pDefObject->Release();
		pDefObject = NULL;

		checkResult(pOperationDef->SetDataDefinitionID (&testDataDef));
		checkResult(pOperationDef->SetIsTimeWarp (AAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDefs (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));
		// !!!Added circular definitions because we don't have optional properties
		checkResult(pOperationDef->AppendDegradeToOperations (pOperationDef));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Init (&parmID, TEST_PARAM_NAME, TEST_PARAM_DESC));
		pDefObject->Release();
		pDefObject = NULL;

		// ------------------------------------------------------------
		//	To test a Transition we need to create a Sequence which will 
		//	a Filler, a transition and another Filler. I know this is not 
		//  very interesting, but it will let us test the Transition 
		//  interface with the least amount of other stuff.
		// ------------------------------------------------------------
		//
		// Create a CompositionMob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
							IID_IAAFCompositionMob, 
							(IUnknown **)&pCompMob));

		checkResult(pCompMob->Initialize(L"Transition Test"));
		// Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(&newMobID));

		// Create a Sequence
		checkResult(pDictionary->CreateInstance(&AUID_AAFSequence,
												IID_IAAFSequence,
												(IUnknown **) &pSequence));

		// Get a Segment interface
		checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		// Get a component interface and 
		checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		// set the Data definition for it !
		checkResult(pComponent->SetDataDef(&fillerUID));
		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create a new Mob Slot that will contain the sequence
		checkResult(pMob->AppendNewSlot(pSegment, 1, L"Transition", &pNewSlot));


		// Create a Filler
		checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller,
												(IUnknown **) &pFiller));

		// Get a component interface
		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize( &fillerUID, fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		
	    checkResult(pDictionary->CreateInstance(&AUID_AAFTransition,
												IID_IAAFTransition, 
												(IUnknown **)&pTransition));

		// Create an empty EffectGroup object !!
		checkResult(pDictionary->CreateInstance(&AUID_AAFOperationGroup,
												IID_IAAFOperationGroup,
												(IUnknown **)&pOperationGroup));

		checkResult(pDictionary->CreateInstance(&AUID_AAFParameter,
												IID_IAAFParameter, 
												(IUnknown **)&pParm));
		checkResult(pParm->SetParameterDefinition (pParamDef));
 // !!!  ImplAAFParameter::SetTypeDefinition (ImplAAFTypeDef*  pTypeDef)
		checkResult(pOperationGroup->Initialize(&datadef, transitionLength, pOperationDef));
		checkResult(pOperationGroup->AddNewParameter (pParm));
		checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller,
												(IUnknown **) &pEffectFiller));
		checkResult(pOperationGroup->AppendNewInputSegment (pEffectFiller));
		// release the filler
		pEffectFiller->Release();
		pFiller->Release();
		pFiller = NULL;

		checkResult(pOperationGroup->SetBypassOverride (1));
		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceClip,
						  IID_IAAFSourceClip, 
						  (IUnknown **)&pSourceClip));
		aafSourceRef_t	sourceRef;
		sourceRef.sourceID = zeroID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pSourceClip->Initialize (&testDataDef,&effectLen, sourceRef));
		checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
		checkResult(pOperationGroup->SetRender (pSourceRef));

		checkResult(pTransition->Create (&datadef, transitionLength, cutPoint, pOperationGroup));
		checkResult(pTransition->QueryInterface (IID_IAAFComponent, (void **)&pComponent));

		// now append the transition
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create the second filler 
		checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller,
												(IUnknown **) &pFiller));

		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize( &fillerUID, fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));

		// Now, we append the composition mob to the file	
		checkResult(pHeader->AppendMob(pMob));
		// and we are done !
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	

	// Cleanup and return
	if (pNewSlot)
		pNewSlot->Release();

	if (pSegment)
		pSegment->Release();

	if (pSequence)
		pSequence->Release();

	if (pFiller)
		pFiller->Release();
	if (pOperationDef)
		pOperationDef->Release();


	if (pOperationGroup)
		pOperationGroup->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();


	if (pTransition)
		pTransition->Release();

	if (pFile) 
	{
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
    // IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pEnum = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFOperationGroup*					pOperationGroup = NULL;
	IEnumAAFComponents*			pCompIter = NULL;
//	aafUID_t					datadef ;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint;

	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	aafInt32					numComponents = 0;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"AAFTransition Test. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;


	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->EnumAAFAllMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->EnumAAFAllMobSlots (&pEnum));

			while (AAFRESULT_SUCCESS == pEnum->NextOne (&pMobSlot))
			{
				checkResult(pMobSlot->GetSegment (&pSegment));
				// Check to see if Segment is a Sequence
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
				// It is, so get a Component Iterator
				checkResult(pSequence->GetNumComponents(&numComponents));
				// Verify that all 3 components(Filler, Transition, Filler) are present
				checkExpression(numComponents == 3,  AAFRESULT_TEST_FAILED);
				checkResult(pSequence->EnumComponents(&pCompIter));
				// Now visit each and every one of the components.
				while(AAFRESULT_SUCCESS == pCompIter->NextOne(&pComponent))
				{
					// Find out what kind of segment we have
					if ((pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition)) == AAFRESULT_SUCCESS)
					{
						// This is the transition 
						checkResult(pTransition->GetCutPoint (&cutPoint));
						checkResult(pComponent->GetLength(&transitionLength));
						checkResult(pTransition->GetOperationGroup(&pOperationGroup));
						// Check results !!
						checkExpression(cutPoint == 0, AAFRESULT_TEST_FAILED);
						checkExpression(transitionLength == 100, AAFRESULT_TEST_FAILED);
					}
					else
					{
						// validate that the other segments are Fillers
						checkResult(pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
					}
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
	if (pTransition)
		pTransition->Release();

	if (pOperationGroup)
		pOperationGroup->Release();

	if (pComponent)
		pComponent->Release();

	if (pSegment)
		pSegment->Release();

	if (pMobSlot)
		pMobSlot->Release();

	if (pSequence)
		pSequence->Release();

	if (pCompIter)
		pCompIter->Release();

	if (pEnum)
		pEnum->Release();

	if (pFiller)
		pFiller->Release();

	if (pMob)
		pMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CAAFTransition_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFTransitionTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTransition_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}



	 


	//@comm Replaces part of omfsTransitionGetInfo


	//@comm Replaces part of omfsTransitionGetInfo







