// @doc INTERNAL
// @com This file implements the module test for CEnumAAFMobSlots
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

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
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFMob						*pMob = NULL;
	IAAFMobSlot		*newSlot = NULL;
	IAAFSegment		*seg = NULL;
	IAAFSourceClip	*sclp = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMob Test";
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

    // Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
 		
  //Make the first mob
	  long	test;
	  aafRational_t	audioRate = { 44100, 1 };

	  // Create a Mob
	  checkResult(pDictionary->CreateInstance(&AUID_AAFMob,
							  IID_IAAFMob, 
							  (IUnknown **)&pMob));

		checkResult(CoCreateGuid((GUID *)&newUID));
	  checkResult(pMob->SetMobID(&newUID));
	  checkResult(pMob->SetName(L"MOBTest"));
	  
	  // Add some slots
	  for(test = 0; test < 5; test++)
	  {
 		  checkResult(pDictionary->CreateInstance(&AUID_AAFSourceClip,
							     IID_IAAFSourceClip, 
							     (IUnknown **)&sclp));		

		  checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));

		  checkResult(pMob->AppendNewSlot (seg, test+1, slotNames[test], &newSlot));

		  newSlot->Release();
      newSlot = NULL;

		  seg->Release();
      seg = NULL;

      sclp->Release();
      sclp = NULL;
	  }

    // Add the mob to the file.
	  checkResult(pHeader->AppendMob(pMob));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (newSlot)
    newSlot->Release();

  if (seg)
    seg->Release();

  if (sclp)
    sclp->Release();

  if (pMob)
    pMob->Release();

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
	IAAFFile			*pFile = NULL;
	bool				bFileOpen = false;
	IAAFHeader			*pHeader = NULL;
	IEnumAAFMobs		*mobIter = NULL;
	IAAFMob				*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot			*slot = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t		numMobs, n, s;
	HRESULT				hr = S_OK;
	IAAFMobSlot*		pArray[2] = { NULL, NULL };
	IAAFMobSlot**		pArrayDef = pArray;
	aafUInt32			resultCount;
	IEnumAAFMobSlots	*slotClone = NULL;
	
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFMobSlots Test";
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
		
		
		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		aafSearchCrit_t		criteria;
		criteria.searchTag = kNoSearch;
		checkResult(pHeader->EnumAAFAllMobs (&criteria, &mobIter));
		
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafUID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->GetNumSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);
			
			checkResult(aMob->EnumAAFAllMobSlots(&slotIter));
			
			/* Read and check all elements using NextOne */
			for(s = 0; s < numSlots; s++)
			{
				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetName (slotName, sizeof(slotName)));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
				
				slot->Release();
				slot = NULL;
			}
			/* Read one past to make sure that it fails */
			checkExpression(slotIter->NextOne(&slot) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
			/* Reset, and check the first element again*/
			checkResult(slotIter->Reset());
			checkResult(slotIter->NextOne (&slot));
			checkResult(slot->GetName (slotName, sizeof(slotName)));
			checkResult(slot->GetSlotID(&trackID));
			checkExpression (wcscmp(slotName, slotNames[0]) == 0, AAFRESULT_TEST_FAILED);
			slot->Release();
			slot = NULL;
			/* Reset, Skip, and check the second element again*/
			checkResult(slotIter->Reset());
			checkResult(slotIter->Skip(1));
			checkResult(slotIter->NextOne (&slot));
			checkResult(slot->GetName (slotName, sizeof(slotName)));
			checkResult(slot->GetSlotID(&trackID));
			checkExpression (wcscmp(slotName, slotNames[1]) == 0, AAFRESULT_TEST_FAILED);
			slot->Release();
			slot = NULL;
			
			/* Reset, and read both elements */
			checkResult(slotIter->Reset());
			checkResult(slotIter->Next (2, (IAAFMobSlot **)&pArray, &resultCount));
			checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
			checkResult(pArrayDef[0]->GetName (slotName, sizeof(slotName)));
			checkResult(pArrayDef[0]->GetSlotID(&trackID));
			checkExpression (wcscmp(slotName, slotNames[0]) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pArrayDef[1]->GetName (slotName, sizeof(slotName)));
			checkResult(pArrayDef[1]->GetSlotID(&trackID));
			checkExpression (wcscmp(slotName, slotNames[1]) == 0, AAFRESULT_TEST_FAILED);
			pArrayDef[0]->Release();
			pArrayDef[0] = NULL;
			pArrayDef[1]->Release();
			pArrayDef[1] = NULL;

			/* Clone the enumerator, and read one element */
			checkResult(slotIter->Clone(&slotClone));
			checkResult(slotClone->Reset());
			checkResult(slotClone->NextOne (&slot));
			checkResult(slot->GetName (slotName, sizeof(slotName)));
			checkResult(slot->GetSlotID(&trackID));
			checkExpression (wcscmp(slotName, slotNames[0]) == 0, AAFRESULT_TEST_FAILED);
			slot->Release();
			slot = NULL;
			slotIter->Release();
			slotIter = NULL;
			slotClone->Release();
			slotClone = NULL;

			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (slot)
		slot->Release();
	
	if (slotIter)
		slotIter->Release();
	
	if (aMob)
		aMob->Release();
	
	if (mobIter)
		mobIter->Release();
	
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
 

extern "C" HRESULT CEnumAAFMobSlots_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"EnumAAFMobSlotsTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CEnumAAFMobSlots_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.

  	// When all of the functionality of this class is tested, we can return success
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFPluginDescriptor methods have not been tested:" << endl;       
//		cout << "     Next" << endl; 
//		cout << "     Clone" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
