// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/









#include "CAAFSequence.h"
#include "CAAFSequence.h"
#ifndef __CAAFSequence_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
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
						   // IAAFSession**		ppSession,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFSequence Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)ppSession);
	*/
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)ppFile);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
    hr = (*ppFile)->Initialize();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = (*ppSession)->SetDefaultIdentification(&ProductInfo);
	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		// hr = (*ppSession)->OpenReadFile(pFileName, ppFile);
		hr = (*ppFile)->OpenExistingRead(pFileName, 0);
		break;

	case kMediaOpenAppend:
		// hr = (*ppSession)->CreateFile(pFileName, kAAFRev1, ppFile);
		hr = (*ppFile)->OpenNewModify(pFileName, 0, &ProductInfo);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		// (*ppSession)->Release();
		// *ppSession = NULL;
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		// (*ppSession)->Release();
		// *ppSession = NULL;
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
	  checkResult(pMob->SetName(L"AAFSequenceTest"));
	  
	  // Add mob slot w/ sequence
 	  checkResult(pDictionary->CreateInstance(&AUID_AAFSequence,
						     IID_IAAFSequence, 
						     (IUnknown **)&pSequence));		
	  checkResult(pSequence->Initialize((aafUID_t*)&DDEF_Audio));

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

		  checkResult(pComponent->SetDataDef((aafUID_t*)&DDEF_Audio));
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
	IAAFSequence*		pSequence = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr;


  try
  {
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));

    // Validate that there is only one composition mob.
	  checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over Composition MOBs
	  criteria.searchTag = kByMobKind;
	  criteria.tags.mobKind = kCompMob;
    checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
	  {
		  aafNumSlots_t		numSlots = 0;

		  checkResult(pMob->GetNumSlots(&numSlots));
		  checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

      // Enumerate over all MOB slots for this MOB
			checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafInt32			numCpnts;

				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));

				checkResult(pSequence->GetNumComponents(&numCpnts));
				checkExpression(numCpnts == kNumComponents, AAFRESULT_TEST_FAILED);

        checkResult(pSequence->EnumComponents(&pCompIter));
			  numCpnts = 0;
			  while (pCompIter && pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
			  {
					aafLength_t	len;
					aafUID_t	dataDef;

					numCpnts++;

					checkResult(pComp->GetDataDef(&dataDef));
					checkExpression(memcmp(&DDEF_Audio, &dataDef, sizeof(aafUID_t)) == 0,
					                AAFRESULT_TEST_FAILED);

					checkResult(pComp->GetLength(&len));
					checkExpression(len == 10, AAFRESULT_TEST_FAILED);

					pComp->Release();
          pComp = NULL;
				}


        checkExpression(numCpnts == kNumComponents, AAFRESULT_TEST_FAILED);

				pCompIter->Release();
        pCompIter = NULL;

        pSequence->Release();
        pSequence = NULL;

				pSegment->Release();
        pSegment = NULL;

        pSlot->Release();
        pSlot = NULL;
			}

			pSlotIter->Release();
      pSlotIter = NULL;

		  pMob->Release();
      pMob = NULL;
		}


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

  if (pSequence)
    pSequence->Release();

  if (pSegment)
    pSegment->Release();

  if (pSlot)
    pSlot->Release();

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

HRESULT CAAFSequence::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFSequenceTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFSequence::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFSequence methods have not been implemented:" << endl; 
		cout << "     RemoveComponent" << endl; 
		cout << "     SegmentOffsetToTC - needs unit test" << endl; 
		cout << "     SegmentTCToOffset - needs unit test" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}



















