// @doc INTERNAL
// @com This file implements the module test for CEnumAAFMobs
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
	// IAAFSession *				pSession = NULL;
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
	ProductInfo.productName = L"EnumAAFMobs Test";
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
 		
    //Make the first mob

	  // Create a FileMob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
							IID_IAAFSourceMob, 
							(IUnknown **)&pSourceMob));

	  checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newUID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(&newUID));
	  checkResult(pMob->SetName(L"File Mob"));
	
 	  checkResult(pDictionary->CreateInstance(&AUID_AAFFileDescriptor,
							IID_IAAFFileDescriptor, 
							(IUnknown **)&edesc));		

    checkResult(pSourceMob->SetEssenceDescriptor (edesc));

	  checkResult(pHeader->AppendMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a MasterMob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFMasterMob,
							IID_IAAFMob, 
							(IUnknown **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newUID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(&newUID));
	  checkResult(pMob->SetName(L"Master Mob"));

	  checkResult(pHeader->AppendMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a CompositionMob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
							  IID_IAAFMob, 
							  (IUnknown **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newUID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(&newUID));
  	checkResult(pMob->SetName(L"Composition Mob"));

	  checkResult(pHeader->AppendMob(pMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

  // Cleanup and return
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
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
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
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

	  // Make sure that we have one master, one file, and one composition (three total)
	  checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
	  checkExpression (3 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetNumMobs(kMasterMob, &numMobs));
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetNumMobs(kFileMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

	// Cleanup and return

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
 
extern "C" HRESULT CEnumAAFMobs_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"EnumMOBTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(AAFRESULT_SUCCESS == hr)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFSourceMob_test...Caught general C++"
		" exception!" << endl; 
	}


  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

	return hr;
}














