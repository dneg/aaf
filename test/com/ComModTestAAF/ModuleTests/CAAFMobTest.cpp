// @doc INTERNAL
// @com This file implements the module test for CAAFMob
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };


static aafWChar *mobName = L"MOBTest";


static const aafTimeStamp_t creationTimeStamp = { 1941, 12, 7, 5, 31, 12, 1 };
static const aafTimeStamp_t modificationTimeStamp = { 1941, 12, 7, 5, 31, 12, 2 };

// {21F2083C-B260-11d3-BFFE-00104BC9156D}
static const aafMobID_t MOBTestID = 
{ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0x21f2083c, 0xb260, 0x11d3, { 0xbf, 0xfe, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
// {81831638-EDF4-11d3-A353-009027DFCA6A}
static const aafMobID_t MOBTestID2 = 
{ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0x81831638, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };


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
  IAAFMob						*pMob2 = NULL;
  IAAFTimelineMobSlot *newSlot = NULL;
  IAAFSegment		*seg = NULL;
  IAAFSourceClip	*sclp = NULL;
  IAAFComponent*		pComponent = NULL;
  aafProductIdentification_t	ProductInfo;
  HRESULT						hr = S_OK;
  aafNumSlots_t					numMobs;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFMob Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kAAFVersionUnknown;
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
 		
	  CAAFBuiltinDefs defs (pDictionary);

	  //Make the first mob
	  long	test;
	  aafRational_t	audioRate = { 44100, 1 };

	  // Create a Mob
	  checkResult(defs.cdMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(MOBTestID));
	  checkResult(pMob->SetName(mobName));
	  
	  checkResult(pMob->SetCreateTime(creationTimeStamp));
	  checkResult(pMob->SetModTime(modificationTimeStamp));

	  // Add some slots
	  for(test = 0; test < 5; test++)
		{
 		  checkResult(defs.cdSourceClip()->
					  CreateInstance(IID_IAAFSourceClip, 
									 (IUnknown **)&sclp));		
		 checkResult(sclp->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddPicture()));
		 pComponent->Release();
		 pComponent = NULL;

		  checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));

		  aafRational_t editRate = { 0, 1};
		  checkResult(pMob->AppendNewTimelineSlot (editRate,
												   seg,
												   test+1,
												   slotNames[test],
												   0,
												   &newSlot));

		  if(test == 4)
			  pMob->RemoveSlotAt(4);

		  newSlot->Release();
		  newSlot = NULL;

		  seg->Release();
		  seg = NULL;

		  sclp->Release();
		  sclp = NULL;
		}

	  // Add the mob to the file.
	  checkResult(pHeader->AddMob(pMob));

	  // Create another Mob, check mob count, then delete and recheck count
	  checkResult(defs.cdMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob2));

	  checkResult(pMob2->SetMobID(MOBTestID2));
	  checkResult(pMob2->SetName(mobName));
	  
	  checkResult(pMob2->SetCreateTime(creationTimeStamp));
	  checkResult(pMob2->SetModTime(modificationTimeStamp));
	  // Add the mob to the file.
	  checkResult(pHeader->AddMob(pMob2));
	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(numMobs == 2, AAFRESULT_TEST_FAILED);
	  checkResult(pHeader->RemoveMob(pMob2));
	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(numMobs == 1, AAFRESULT_TEST_FAILED);
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

  if (pComponent)
    pComponent->Release();

  if (sclp)
    sclp->Release();

  if (pMob2)

    pMob2->Release();



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

static void checkTimeStampsAreEqual(aafTimeStamp_constref ts1, aafTimeStamp_constref ts2)
{
  // Check the time stamp: should be 7 December, 1941 at 05:13:12.01
  checkExpression (ts1.date.year == ts2.date.year, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.date.month == ts2.date.month, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.date.day == ts2.date.day, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.hour == ts2.time.hour, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.minute == ts2.time.minute, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.second == ts2.time.second, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.fraction == ts2.time.fraction, AAFRESULT_TEST_FAILED);
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  IEnumAAFMobs *mobIter = NULL;
  IAAFMob			*aMob = NULL;
  IEnumAAFMobSlots	*slotIter = NULL;
  IAAFMobSlot		*slot = NULL;
  aafProductIdentification_t	ProductInfo;
  aafNumSlots_t	numMobs, n, s;
  HRESULT						hr = S_OK;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFMob Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kAAFVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.platform = NULL;

  try
	{
	  // Open the file
	  checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));


	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);


	  aafSearchCrit_t		criteria;
	  criteria.searchTag = kAAFNoSearch;
	  checkResult(pHeader->GetMobs (&criteria, &mobIter));

	  for(n = 0; n < numMobs; n++)
		{
		  aafWChar		name[500], slotName[500];
		  aafNumSlots_t	numSlots;
		  aafMobID_t		mobID;
		  aafSlotID_t		trackID;
		  aafUInt32 nameBufLen = 0;

		  checkResult(mobIter->NextOne (&aMob));
      
      // Check GetNameBufLen and GetName
      checkResult(aMob->GetNameBufLen(&nameBufLen));
      checkExpression(((wcslen(mobName) + 1) * sizeof(aafCharacter)) == nameBufLen, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetName (name, nameBufLen));
		  checkExpression (wcscmp(mobName, name) == 0, AAFRESULT_TEST_FAILED);

      // Check GetMobID
		  checkResult(aMob->GetMobID (&mobID));
		  checkExpression(memcmp(&MOBTestID, &mobID, sizeof(mobID)) == 0, AAFRESULT_TEST_FAILED);

		  // Check the time stamps
		  aafTimeStamp_t created = { 0 };
		  checkResult(aMob->GetCreateTime(&created));
		  checkTimeStampsAreEqual(creationTimeStamp, created);
		  aafTimeStamp_t modified = { 0 };
		  checkResult(aMob->GetModTime(&modified));
		  checkTimeStampsAreEqual(modificationTimeStamp, modified);

      // Check the GetMobInfo data.
      memset(&created, 0, sizeof(created));
      memset(&modified, 0, sizeof(modified));
      checkResult(aMob->GetMobInfo(&modified, &created, name, sizeof(name)));
		  checkTimeStampsAreEqual(creationTimeStamp, created);
		  checkTimeStampsAreEqual(modificationTimeStamp, modified);
			checkExpression (wcscmp(mobName, name) == 0, AAFRESULT_TEST_FAILED);

		  checkResult(aMob->CountSlots (&numSlots));
		  checkExpression(4 == numSlots, AAFRESULT_TEST_FAILED);

		  checkResult(aMob->GetSlots(&slotIter));

		  for(s = 0; s < numSlots; s++)
			{
			  checkResult(slotIter->NextOne (&slot));
			  checkResult(slot->GetName (slotName, sizeof(slotName)));
			  checkResult(slot->GetSlotID(&trackID));
			  checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);

			  slot->Release();
			  slot = NULL;
			}

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
 

extern "C" HRESULT CAAFMob_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFMobTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFMob_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  // Cleanup our object if it exists.

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFMob methods have not been implemented:" << endl; 
		cout << "     AppendSlot" << endl; 
		cout << "     EnumAAFAllMobSlots - needs unit test" << endl; 
		cout << "     AppendComment - needs unit test" << endl; 
		cout << "     GetNumComments" << endl; 
		cout << "     RemoveComment" << endl; 
		cout << "     EnumAAFAllMobComments - needs unit test" << endl; 
		cout << "     AppendNewTimelineSlot" << endl; 
		cout << "     OffsetToMobTimecode - needs unit test" << endl; 
		cout << "     ChangeRef - needs unit test" << endl; 
		cout << "     CloneExternal - needs unit test" << endl; 
		cout << "     Copy - needs unit test" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

  return hr;
}
