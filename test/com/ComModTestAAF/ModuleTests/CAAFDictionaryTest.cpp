// @doc INTERNAL
// @com This file implements the module test for CAAFDictionary
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#define kNumComponents	5



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



static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFDictionary Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*		pMob = NULL;
	IAAFMobSlot*	pMobSlot = NULL;
	IAAFSequence*	pSequence = NULL;
	IAAFSegment*	pSegment = NULL;
	IAAFComponent*	pComponent = NULL;
	aafUID_t		NewMobID;
	int				i;
	HRESULT			hr = S_OK;
	
	
	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// Create a Composition Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
			IID_IAAFMob, 
			(IUnknown **)&pMob));
		
		checkResult(CoCreateGuid((GUID *)&NewMobID));
		checkResult(pMob->SetMobID(&NewMobID));
		checkResult(pMob->SetName(L"AAFDictionaryTest"));
		
		// Add mob slot w/ Sequence
		checkResult(pDictionary->CreateInstance(&AUID_AAFSequence,
			IID_IAAFSequence, 
			(IUnknown **)&pSequence));		
		checkResult(pSequence->Initialize((aafUID_t*)&DDEF_Picture));
		
		//
		//	Add some segments.  Need to test failure conditions
		//	(i.e. starting/ending w/ transition, two trans back
		//	to bacl).
		//
		for(i = 0; i < kNumComponents; i++)
		{
			aafLength_t		len = 10;
			
			checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
				IID_IAAFComponent, 
				(IUnknown **)&pComponent));
			
			if(i == 0)
			{
				checkResult(pComponent->SetDataDef((aafUID_t*)&DDEF_PictureWithMatte));
			}
			else
			{
				checkResult(pComponent->SetDataDef((aafUID_t*)&DDEF_Picture));
			}

			checkResult(pComponent->SetLength(&len));
			checkResult(pSequence->AppendComponent(pComponent));
			
			pComponent->Release();
			pComponent = NULL;
		}
		
		checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		
		checkResult(pMob->AppendNewSlot(pSegment, 1, L"AAF Test Sequence", &pMobSlot));
		
		pMobSlot->Release();
		pMobSlot = NULL;
		
		pSegment->Release();
		pSegment = NULL;
		
		// Add the master mob to the file and cleanup
		pHeader->AppendMob(pMob);
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pComponent)
		pComponent->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFComponent*		pComp = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFDataDef*		pDataDef = NULL;
	IAAFSequence*		pSequence = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	aafNumSlots_t		numMobs;
	aafSearchCrit_t		criteria;
	HRESULT				hr = S_OK;
	
	
	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		
		// Validate that there is only one composition mob.
		checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// Check a data definition from a composition MOB in order to test weak references
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
					
		// Enumerate the first MOB slot for this MOB
		checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
		checkResult(pSlotIter->NextOne(&pSlot));
				
		checkResult(pSlot->GetSegment(&pSegment));
		checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
		checkResult(pSequence->EnumComponents(&pCompIter));
		checkResult(pCompIter->NextOne(&pComp));
		
		aafUID_t	dataDef, pwmID = DDEF_PictureWithMatte;
		aafBool		testBool;
				
		checkResult(pComp->GetDataDef(&dataDef));
		checkResult(pDictionary->LookupDataDefintion(&dataDef, &pDataDef));
		checkResult(pDataDef->IsSoundKind(&testBool));
		checkExpression(testBool == AAFFalse, AAFRESULT_TEST_FAILED);
		
		checkResult(pDataDef->IsDataDefOf(&pwmID, &testBool));
		checkExpression(testBool == AAFTrue, AAFRESULT_TEST_FAILED);
		
		pComp->Release();
		pComp = NULL;
		pDataDef->Release();
		pDataDef = NULL;
		
		pCompIter->Release();
		pCompIter = NULL;
		
		pSequence->Release();
		pSequence = NULL;
		
		pSegment->Release();
		pSegment = NULL;
		
		pSlot->Release();
		pSlot = NULL;
		
		
		pSlotIter->Release();
		pSlotIter = NULL;
		
		pMob->Release();
		pMob = NULL;
		
		
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (pComp)
		pComp->Release();
	
	if (pCompIter)
		pCompIter->Release();
	
	if (pDataDef)
		pDataDef->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pSlot)
		pSlot->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pSlotIter)
		pSlotIter->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pMobIter)
		pMobIter->Release();
	
	if (pHeader) pHeader->Release();
	
	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}
	
	
	return 	hr;
}

extern "C" HRESULT CAAFDictionary_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFDictionaryTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFDictionary_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFDictionary tests have not been implemented:" << endl; 
		cout << "     RegisterClass" << endl; 
		cout << "     LookupClass" << endl; 
		cout << "     GetClassDefinitions" << endl; 
		cout << "     RegisterType" << endl; 
		cout << "     LookupType" << endl; 
		cout << "     GetTypeDefinitions" << endl; 
		cout << "     RegisterDataDefintion" << endl; 
		cout << "     GetDataDefinitions" << endl; 
		cout << "     RegisterOperationDefinition" << endl; 
		cout << "     LookupOperationDefinition" << endl; 
		cout << "     GetOperationDefinitions" << endl; 
		cout << "     RegisterParameterDefinition" << endl; 
		cout << "     LookupParameterDefinition" << endl; 
		cout << "     GetParameterDefinitions" << endl; 
		cout << "     RegisterCodecDefinition" << endl; 
		cout << "     LookupCodecDefinition" << endl; 
		cout << "     GetCodecDefinitions" << endl; 
		cout << "     RegisterContainerDefinition" << endl; 
		cout << "     LookupContainerDefinition" << endl; 
		cout << "     GetContainerDefinitions" << endl; 
		cout << "     RegisterInterpolationDefinition" << endl; 
		cout << "     LookupInterpolationDefinition" << endl; 
		cout << "     GetInterpolationDefinitions" << endl; 
		cout << "     RegisterPluginDescriptor" << endl; 
		cout << "     LookupPluginDescriptor" << endl; 
		cout << "     GetPluginDescriptors" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
