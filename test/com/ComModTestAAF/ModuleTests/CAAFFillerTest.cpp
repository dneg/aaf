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



#ifndef __CAAFFiller_h__
#include "CAAFFiller.h"
#endif

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"


static aafUID_t    fillerUID = DDEF_Timecode;
static aafLength_t  fillerLength = 3200;


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
  IAAFFile *          pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
  IAAFCompositionMob*      pCompMob = NULL;
  IAAFMob*          pMob = NULL;
  IAAFFiller*          pFiller = NULL;
  IAAFMobSlot*        pSlot = NULL;
  IAAFSegment*        pSegment = NULL;
  aafProductIdentification_t  ProductInfo;
  aafUID_t          newMobID;
  HRESULT            hr = AAFRESULT_SUCCESS;


  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"Make AVR Example";
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
    checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
    bFileOpen = true;
  
    // We can't really do anthing in AAF without the header.
    checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
     
     
    // Create a Composition mob - it should work !!
    checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
                IID_IAAFCompositionMob, 
                (IUnknown **)&pCompMob));
    // get a IAAFMob interface
    checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
    // Initialize the CompMob
    checkResult(CoCreateGuid((GUID *)&newMobID));
    checkResult(pMob->SetMobID(&newMobID));
    checkResult(pMob->SetName(L"AAFFillerTest"));

    // Create a AAFFiller - since it is the first time we will check the error code
    checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
                IID_IAAFFiller, 
                (IUnknown **)&pFiller));
    // Get a IAAFSegment interface for it
    checkResult(pFiller->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
    // Set filler properties
    checkResult(pFiller->Initialize( &fillerUID, fillerLength));
    // append the filler to the MOB tree
    checkResult(pMob->AppendNewSlot(pSegment, 1, L"FillerSlot", &pSlot)); 

    // Add the Mob to the file
    checkResult(pHeader->AppendMob(pMob));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pSlot)
    pSlot->Release();

  if (pSegment)
    pSegment->Release();

  if (pFiller)
    pFiller->Release();

  if (pMob)
    pMob->Release();

  if (pCompMob)
    pCompMob->Release();

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

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  // IAAFSession *        pSession = NULL;
  IAAFFile *          pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *        pHeader = NULL;

//  IAAFSourceMob*        pSourceMob = NULL;
  IAAFMob*          pMob = NULL;
  IAAFFiller*          pFiller = NULL;
  IAAFMobSlot*        pSlot = NULL;
  IAAFSegment*        pSegment = NULL;
  IAAFComponent*        pComponent = NULL;
  IEnumAAFMobs*        pMobIter = NULL;
  IEnumAAFMobSlots*      pSlotIter = NULL;
  
  aafProductIdentification_t  ProductInfo;
  aafNumSlots_t        numMobs, numSlots;
  aafSearchCrit_t        criteria;
  aafUID_t          readFillerUID;
  aafLength_t          readFillerLength;
  HRESULT            hr = AAFRESULT_SUCCESS;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"Make AVR Example";
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
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

    // Enumerate over all Composition Mobs
    criteria.searchTag = kByMobKind;
    criteria.tags.mobKind = kCompMob;
    checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
    while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
    {
      checkResult(pMob->GetNumSlots(&numSlots));
      checkExpression (1 == numSlots, AAFRESULT_TEST_FAILED);

      checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
      while (pSlotIter && (pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS))
      {
        checkResult(pSlot->GetSegment(&pSegment));
        checkResult(pSegment->QueryInterface(IID_IAAFFiller, (void **) &pFiller));
        // Make sure the segment is a AAFFiller !!

        // Get The component interface
        checkResult(pSegment->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
        // retrieve properties
        checkResult(pComponent->GetLength( &readFillerLength));
        checkResult(pComponent->GetDataDef( &readFillerUID));
        
        // Compare results
        checkExpression((readFillerLength == fillerLength) &&
                       (memcmp(&readFillerUID, &fillerUID, sizeof(fillerUID)) == 0),
                       AAFRESULT_TEST_FAILED);

        pComponent->Release();
        pComponent = NULL;

        pFiller->Release();
        pFiller = NULL;

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

  // Cleanup and return

  if (pComponent)
    pComponent->Release();

  if (pSegment)
    pSegment->Release();

  if (pSlot)
    pSlot->Release();

  if (pFiller)
    pFiller->Release();

  if (pSlotIter)
    pSlotIter->Release();

  if (pMob)
    pMob->Release();

  if (pMobIter)
    pMobIter->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
      pFile->Close();
     pFile->Release();
  }

  return hr;
}

HRESULT CAAFFiller::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"FillerTest.aaf";

  try
  {
    hr = CreateAAFFile( pFileName );
    if (hr == AAFRESULT_SUCCESS)
      hr = ReadAAFFile( pFileName );
  }
  catch (...)
  {
    cerr << "CAAFFiller::test...Caught general C++"
    " exception!" << endl; 
  }

  // Cleanup our object if it exists.
  return hr;
}





