// @doc INTERNAL
// @com This file implements the module test for CAAFSequence
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

#define kNumComponents	5

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x75529074, 0x0404, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFSequence Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
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
	IAAFTimelineMobSlot*	pMobSlot = NULL;
	IAAFSequence*	pSequence = NULL;
	IAAFSegment*	pSegment = NULL;
	IAAFComponent*	pComponent = NULL;
	aafUInt32		numComponents;
	int				i;
	HRESULT			hr = S_OK;


  try
  {  
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);


	  // Create the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);
 		
	  // Create a Composition Mob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"AAFSequenceTest"));
	  
	  // Add mob slot w/ sequence
 	  checkResult(defs.cdSequence()->
				  CreateInstance(IID_IAAFSequence, 
								 (IUnknown **)&pSequence));		
	  checkResult(pSequence->Initialize(defs.ddSound()));

	  //
	  //	Add some segments.  Need to test failure conditions
	  //	(i.e. starting/ending w/ transition, two trans back
	  //	to bacl).
	  //
	  for(i = 0; i < kNumComponents + 1; i++)	// Add one more for delete below
	  {
		  aafLength_t		len = 10;
		  
		  checkResult(defs.cdFiller()->
			  CreateInstance(IID_IAAFComponent, 
			  (IUnknown **)&pComponent));
		  
		  checkResult(pComponent->SetDataDef(defs.ddSound()));
		  checkResult(pComponent->SetLength(len));
		  checkResult(pSequence->AppendComponent(pComponent));
		  
		  pComponent->Release();
		  pComponent = NULL;
	  }
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(kNumComponents + 1 == numComponents, AAFRESULT_TEST_FAILED);
		checkResult(pSequence->RemoveComponentAt(kNumComponents));
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(kNumComponents == numComponents, AAFRESULT_TEST_FAILED);

		checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));

		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"AAF Test Sequence",
												0,
												&pMobSlot));
		
    pMobSlot->Release();
    pMobSlot = NULL;

		pSegment->Release();
    pSegment = NULL;

		// Add the master mob to the file and cleanup
		pHeader->AddMob(pMob);

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
		pFile->Save();
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
	IAAFDataDef*        pDataDef = 0;
	IAAFDefObject*      pDefObj = 0;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr = S_OK;


  try
  {
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));

    // Validate that there is only one composition mob.
	  checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over Composition MOBs
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;
    checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
	  {
		  aafNumSlots_t		numSlots = 0;

		  checkResult(pMob->CountSlots(&numSlots));
		  checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

      // Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafUInt32			numCpnts;

				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));

				checkResult(pSequence->CountComponents(&numCpnts));
				checkExpression(numCpnts == kNumComponents, AAFRESULT_TEST_FAILED);

			    checkResult(pSequence->GetComponents(&pCompIter));
				numCpnts = 0;
				while (pCompIter && pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
				{
					aafLength_t	len;
					aafUID_t	dataDef;

					numCpnts++;

					checkResult(pComp->GetDataDef(&pDataDef));
					checkResult(pDataDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObj));
					pDataDef->Release();
					pDataDef = 0;
					checkResult(pDefObj->GetAUID(&dataDef));
					pDefObj->Release();
					pDefObj = 0;
					checkExpression(memcmp(&DDEF_Sound, &dataDef, sizeof(aafUID_t)) == 0,
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
	  {
		pComp->Release();
		pComp = 0;
	  }

	if (pCompIter)
	  {
		pCompIter->Release();
		pCompIter = 0;
	  }

	if (pSequence)
	  {
		pSequence->Release();
		pSequence = 0;
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

	if (pDataDef)
	  {
		pDataDef->Release();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release();
		pDefObj = 0;
	  }

	if (pFile)
	  {
		pFile->Close();
		pFile->Release();
		pFile = 0;
	  }

	return 	hr;
}

extern "C" HRESULT CAAFSequence_test()
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
		cerr << "CAAFSequence_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFSequence methods have not been implemented:" << endl; 
//		cout << "     RemoveComponent" << endl; 
		cout << "     SegmentOffsetToTC - needs unit test" << endl; 
		cout << "     SegmentTCToOffset - needs unit test" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}



















