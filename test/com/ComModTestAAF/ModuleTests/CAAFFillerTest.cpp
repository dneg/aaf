// @doc INTERNAL
// @com This file implements the module test for CAAFFiller
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
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafUID_t    fillerUID = DDEF_Timecode;
static aafLength_t  fillerLength = 3200;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x1d760b6a, 0x03ff, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

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
  IAAFTimelineMobSlot*        pSlot = NULL;
  IAAFSegment*        pSegment = NULL;
  aafProductIdentification_t  ProductInfo;
  HRESULT            hr = AAFRESULT_SUCCESS;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFFiller Test";
  ProductInfo.productVersion = &v;
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
     
	  // Create a Composition mob - it should work !!
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFCompositionMob, 
								 (IUnknown **)&pCompMob));
	  // get a IAAFMob interface
	  checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	  // Initialize the CompMob
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"AAFFillerTest"));

	  // Create a AAFFiller - since it is the first time we will check
	  // the error code
	  checkResult(defs.cdFiller()->
				  CreateInstance(IID_IAAFFiller, 
								 (IUnknown **)&pFiller));
	  // Get a IAAFSegment interface for it
	  checkResult(pFiller->QueryInterface (IID_IAAFSegment,
										   (void **)&pSegment));
	  // Set filler properties
	  checkResult(pFiller->Initialize(defs.ddTimecode(), fillerLength));
	  // append the filler to the MOB tree
	  aafRational_t editRate = { 0, 1};
	  checkResult(pMob->AppendNewTimelineSlot(editRate,
											  pSegment,
											  1,
											  L"FillerSlot",
											  0,
											  &pSlot)); 

	  // Add the Mob to the file
	  checkResult(pHeader->AddMob(pMob));
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
  IAAFDataDef * pDataDef = 0;
  IAAFDefObject * pDefObj = 0;
  
  aafProductIdentification_t  ProductInfo;
  aafNumSlots_t        numMobs, numSlots;
  aafSearchCrit_t        criteria;
  aafUID_t          readFillerUID;
  aafLength_t          readFillerLength;
  HRESULT            hr = AAFRESULT_SUCCESS;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFFiller Test";
  ProductInfo.productVersion = &v;
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
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over all Composition Mobs
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;
	  checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
		{
		  checkResult(pMob->CountSlots(&numSlots));
		  checkExpression (1 == numSlots, AAFRESULT_TEST_FAILED);

		  checkResult(pMob->GetSlots(&pSlotIter));
		  while (pSlotIter && (pSlotIter->NextOne(&pSlot) ==
							   AAFRESULT_SUCCESS))
			{
			  checkResult(pSlot->GetSegment(&pSegment));
			  checkResult(pSegment->QueryInterface(IID_IAAFFiller,
												   (void **) &pFiller));
			  // Make sure the segment is a AAFFiller !!

			  // Get The component interface
			  checkResult(pSegment->QueryInterface(IID_IAAFComponent,
												   (void **) &pComponent));
			  // retrieve properties
			  checkResult(pComponent->GetLength( &readFillerLength));
			  checkResult(pComponent->GetDataDef( &pDataDef));
			  checkResult(pDataDef->QueryInterface(IID_IAAFDefObject,
												   (void **) &pDefObj));
			  pDataDef->Release ();
			  pDataDef = 0;
			  checkResult(pDefObj->GetAUID( &readFillerUID));
			  pDefObj->Release ();
			  pDefObj = 0;
        
			  // Compare results
			  checkExpression((readFillerLength == fillerLength) &&
							  (memcmp(&readFillerUID,
									  &fillerUID,
									  sizeof(fillerUID)) == 0),
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

  if (pDataDef)
	{
	  pDataDef->Release ();
	  pDataDef = 0;
	}

  if (pDefObj)
	{
	  pDefObj->Release ();
	  pDefObj = 0;
	}

  if (pComponent)
	{
	  pComponent->Release();
	  pComponent = 0;
	}

  if (pSegment)
	{
	  pSegment->Release();
	  pSegment = 0;
	}

  if (pSlot)
	{
	  pSlot->Release();
	  pSlot = 0;
	}

  if (pFiller)
	{
	  pFiller->Release();
	  pFiller = 0;
	}

  if (pSlotIter)
	{
	  pSlotIter->Release();
	  pSlotIter = 0;
	}

  if (pMob)
	{
	  pMob->Release();
	  pMob = 0;
	}

  if (pMobIter)
	{
	  pMobIter->Release();
	  pMobIter = 0;
	}

  if (pHeader)
	{
	  pHeader->Release();
	  pHeader = 0;
	}
      
  if (pFile)
	{
	  if (bFileOpen)
		pFile->Close();
	  pFile->Release();
	  pFile = 0;
	}

  return hr;
}

extern "C" HRESULT CAAFFiller_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"AAFFillerTest.aaf";

  try
  {
    hr = CreateAAFFile( pFileName );
    if (hr == AAFRESULT_SUCCESS)
      hr = ReadAAFFile( pFileName );
  }
  catch (...)
  {
    cerr << "CAAFFiller_test...Caught general C++"
		 << " exception!" << endl; 
	hr = AAFRESULT_TEST_FAILED;
  }

  // Cleanup our object if it exists.
  return hr;
}
