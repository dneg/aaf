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



#ifndef __CAAFFile_h__
#include "CAAFFile.h"
#endif

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"

static aafUID_t		newUID;


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


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFMob			*pMob = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFile Test";
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


    // Create the file.
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
 		
 	  
	  // Create a Mob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFMob,
							  IID_IAAFMob, 
							  (IUnknown **)&pMob));
    
    // Initialize the Mob properties
		checkResult(CoCreateGuid((GUID *)&newUID));
		checkResult(pMob->SetMobID(&newUID));
	  checkResult(pMob->SetName(MOB_NAME_TEST));

		// Add the source mob into the tree
		checkResult(pHeader->AppendMob(pMob));

		// Attempt to save the file.
		checkResult(pFile->Save());

    // Attempt to close the file.
	  checkResult(pFile->Close());
	  bFileOpen = false;
  }
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}


	// Cleanup and return
	if (pMob)
		pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file
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
	IEnumAAFMobs *mobIter = NULL;
  IAAFMob			*pMob = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs, n;
	HRESULT						hr = S_OK;
	aafWChar					name[500];
	aafUID_t					mobID;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFile Test";
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

		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

    checkResult(pHeader->EnumAAFAllMobs (NULL, &mobIter));
    for(n = 0; n < numMobs; n++)
	  {
		  checkResult(mobIter->NextOne (&pMob));
		  checkResult(pMob->GetName (name, sizeof(name)));
		  checkResult(pMob->GetMobID (&mobID));
		  checkExpression(wcscmp( name, MOB_NAME_TEST) == 0, AAFRESULT_TEST_FAILED);
		  checkExpression(memcmp(&mobID, &newUID, sizeof(mobID)) == 0, AAFRESULT_TEST_FAILED);

		  pMob->Release();
		  pMob = NULL;
	  }

	  mobIter->Release();
	  mobIter = NULL;

	  checkResult(pFile->Close());
    bFileOpen = false;

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


	// Cleanup and return
  if (mobIter)
    mobIter->Release();

	if (pMob)
		pMob->Release();

  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	return hr;
}

HRESULT CAAFFile::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"FileTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFMob::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
  return hr;
}










