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


static aafInt32 fadeInLen  = 1000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafRational_t fadeInEditUnit = { 25, 1};


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
	IAAFDictionary*	pDictionary = NULL;

	IAAFCompositionMob*			pCompMob = NULL;
	IAAFMob*					pMob = NULL;

	aafUID_t					newMobID;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;

  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);

	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFCompositionMobTest";
	  ProductInfo.productVersion.major = 1;
	  ProductInfo.productVersion.minor = 0;
	  ProductInfo.productVersion.tertiary = 0;
	  ProductInfo.productVersion.patchLevel = 0;
	  ProductInfo.productVersion.type = kVersionUnknown;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = UnitTestProductID;
	  ProductInfo.platform = NULL;

    
		// Create the new AAF file.
	  checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
    bFileOpen = true;

    // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

		// Create a CompositionMob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
								IID_IAAFCompositionMob, 
								(IUnknown **)&pCompMob));
		// Get a MOB Interface 
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
    // Assign the mob a new id.
    checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(&newMobID));
    
    // Initialize the composition mob.
		checkResult(pCompMob->Initialize( L"COMPMOB01" ));
		checkResult(pCompMob->SetDefaultFade(fadeInLen, fadeInType, fadeInEditUnit));

    // Add the mob to the file.
    checkResult(pHeader->AppendMob(pMob));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
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
	IAAFHeader *				pHeader = NULL;
 
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFCompositionMob*			pCompMob = NULL;

	aafSearchCrit_t				criteria;
	aafDefaultFade_t			defaultFade;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	HRESULT						hr = S_OK;


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFCompositionMob Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;

  
  try
  {
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs( kAllMob, &numMobs ));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
		{
			// Get A CompositionMob Interface 
			checkResult(pMob->QueryInterface(IID_IAAFCompositionMob,(void **)&pCompMob));
			checkResult(pCompMob->GetDefaultFade( &defaultFade ));
			
      checkExpression( (defaultFade.fadeLength == fadeInLen) &&
				  (defaultFade.fadeType == fadeInType) &&
				  (memcmp( &( defaultFade.fadeEditUnit), &fadeInEditUnit, sizeof( fadeInEditUnit ))== 0) &&
				  (defaultFade.valid == AAFTrue), 
         AAFRESULT_TEST_FAILED);

			pCompMob->Release();
			pCompMob = NULL;

			pMob->Release();
			pMob = NULL;
		}
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

	// Cleanup and return
	if (pMobIter)
		pMobIter->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pMob)
		pMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}



	return hr;
}

extern "C" HRESULT CAAFCompositionMob_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFCompositionMobTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFCompositionMob_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


  //@access Public Members

  //@rdesc Error code [see below].


  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	


  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade


