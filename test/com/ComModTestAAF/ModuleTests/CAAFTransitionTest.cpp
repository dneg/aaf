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

#ifndef __CAAFTransition_h__
#include "CAAFTransition.h"
#endif


#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

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


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pNewSlot = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFGroup*					pGroup = NULL;
	IAAFSegment*				pSeg = NULL;
	IAAFComponent*				pComponent = NULL;
	
	aafUID_t					newMobID;
	aafUID_t					datadef = DDEF_Video;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint = 0;

	CvtInt32toLength(10, transitionLength);
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTransition Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


    // Create the file
		checkResult(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		checkResult(pFile->Initialize());
		checkResult(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
    // Get the number of mobs to force creation of the content storage.
	// This is temporary as the content storage should be created by
	// the call to OpenNewModify above.
    aafNumSlots_t n;
    checkResult(pHeader->GetNumMobs(kAllMob, &n));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
 		
		// Create a CompositionMob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
							IID_IAAFCompositionMob, 
							(IUnknown **)&pCompMob));

    // Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(&newMobID));

		checkResult(pCompMob->Initialize(L"COMPMOB01"));
		
	    checkResult(pDictionary->CreateInstance(&AUID_AAFTransition,
								IID_IAAFTransition, 
								(IUnknown **)&pTransition));
		// Create an empty Effect object !!
		checkResult(pDictionary->CreateInstance(&AUID_AAFGroup,
												IID_IAAFGroup,
												(IUnknown **)&pGroup));

		checkResult(pTransition->Create (&datadef, transitionLength, cutPoint, pGroup));
		checkResult(pTransition->QueryInterface (IID_IAAFComponent, (void **)&pComponent));

		checkResult(pMob->AppendNewSlot (pSeg, 0, L"Transition", &pNewSlot));
		
		checkResult(pHeader->AppendMob(pMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

	// Cleanup and return
	if (pNewSlot)
		pNewSlot->Release();

	if (pSeg)
		pSeg->Release();

	if (pTransition)
		pTransition->Release();

	if (pGroup)
		pGroup->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

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
	IAAFSegment*				pSeg = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFComponent*				pComponent = NULL;
//	aafUID_t					datadef ;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint;

	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


  try
  {
    // Open the file
		checkResult(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		checkResult(pFile->Initialize());
		checkResult(pFile->OpenExistingRead(pFileName, 0));
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
        checkResult(pMobSlot->GetSegment (&pSeg));
        // Get an Edgeecode interface 
        checkResult(pSeg->QueryInterface (IID_IAAFEdgecode, (void **)&pTransition));
        checkResult(pTransition->GetCutPoint (&cutPoint));
		checkResult(pTransition->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->GetLength(&transitionLength));
        // Check results !!
        checkExpression(cutPoint == 0, AAFRESULT_TEST_FAILED);
        checkExpression(transitionLength == 10, AAFRESULT_TEST_FAILED);
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

	if (pComponent)
		pComponent->Release();

	if (pSeg)
		pSeg->Release();

	if (pMobSlot)
		pMobSlot->Release();

	if (pEnum)
		pEnum->Release();

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

HRESULT CAAFTransition::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TransitionTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTransition::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

  return hr;
}



	 


	//@comm Replaces part of omfsTransitionGetInfo


	//@comm Replaces part of omfsTransitionGetInfo







