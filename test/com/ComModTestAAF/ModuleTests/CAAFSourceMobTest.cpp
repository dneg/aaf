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

#include "CAAFSourceMob.h"
#include "CAAFSourceMob.h"
#ifndef __CAAFSourceMob_h__
#error - improperly defined include guard
#endif

#include <stdio.h>
#include <iostream.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

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



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
  IAAFEssenceDescriptor *edesc = NULL;

	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFSourceMob Test";
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

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
 		
    //Make the first mob
	  long			test;
	  aafUID_t		ddef = DDEF_Audio;

	  aafRational_t	audioRate = { 44100, 1 };

	  // Create a Mob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
							  IID_IAAFSourceMob, 
							  (IUnknown **)&pSourceMob));

	  checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

		checkResult(CoCreateGuid((GUID *)&newUID));
	  checkResult(pMob->SetMobID(&newUID));
	  checkResult(pMob->SetName(L"SourceMOBTest"));
	  
	  // Add some slots
	  for(test = 0; test < 2; test++)
	  {
		  checkResult(pSourceMob->AddNilReference (test+1, 0, &ddef, audioRate));
	  }

 	  checkResult(pDictionary->CreateInstance(&AUID_AAFEssenceDescriptor,
							  IID_IAAFEssenceDescriptor, 
							  (IUnknown **)&edesc));		
 	  checkResult(pSourceMob->SetEssenceDescriptor (edesc));

	  checkResult(pHeader->AppendMob(pMob));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	
  if (edesc)
	  edesc->Release();

  if (pMob)
    pMob->Release();

  if (pSourceMob)
    pSourceMob->Release();

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
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot		*slot = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n, s;
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


    checkResult(pHeader->EnumAAFAllMobs (NULL, &mobIter));
	  for(n = 0; n < numMobs; n++)
	  {
		  aafWChar		name[500];
		  aafNumSlots_t	numSlots;
		  aafUID_t		mobID;
		  aafSlotID_t		trackID;

		  checkResult(mobIter->NextOne (&aMob));
		  checkResult(aMob->GetName (name, sizeof(name)));
		  checkResult(aMob->GetMobID (&mobID));

		  checkResult(aMob->GetNumSlots (&numSlots));
		  if (2 != numSlots)
			  return AAFRESULT_TEST_FAILED;
		  if(numSlots != 0)
		  {
			  checkResult(aMob->EnumAAFAllMobSlots(&slotIter));

			  for(s = 0; s < numSlots; s++)
			  {
				  checkResult(slotIter->NextOne (&slot));
				  checkResult(slot->GetSlotID(&trackID));

				  slot->Release();
				  slot = NULL;
			  }
		  }
		  checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
		  checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));

		  pEdesc->Release();
		  pEdesc = NULL;

		  pSourceMob->Release();
		  pSourceMob = NULL;

		  aMob->Release();
		  aMob = NULL;
    }
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  
  // Cleanup and return
  if (slot)
    slot->Release();

  if (slotIter)
    slotIter->Release();

  if (pEdesc)
    pEdesc->Release();

  if (pSourceMob)
    pSourceMob->Release();

  if (aMob)
    aMob->Release();

	if(mobIter)
		mobIter->Release();

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
 
HRESULT CAAFSourceMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"SourceMOBTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFSourceMob::test...Caught general C++"
		" exception!" << endl; 
	}


	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	  
	return hr;
}
