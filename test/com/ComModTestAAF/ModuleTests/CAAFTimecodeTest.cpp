// @doc INTERNAL
// @com This file implements the module test for CAAFTimecode
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
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x72e4ca72, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob						*pMob = NULL;
	IAAFTimelineMobSlot			*pNewSlot = NULL;
	IAAFTimecode				*pTimecode = NULL;
	IAAFSegment					*pSeg = NULL;

	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafLength_t					zero;
	aafTimecode_t				startTC;

	CvtInt32toLength(0, zero);
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTimecode Test";
	ProductInfo.productVersion = &v;
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
	  CAAFBuiltinDefs defs (pDictionary);
 		
	  // Create a CompositionMob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFCompositionMob, 
								 (IUnknown **)&pCompMob));

	  // Get a MOB interface
	  checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	  checkResult(pMob->SetMobID(TEST_MobID));

	  checkResult(pCompMob->Initialize(L"COMPMOB01"));
		
	  checkResult(defs.cdTimecode()->
				  CreateInstance(IID_IAAFTimecode, 
								 (IUnknown **)&pTimecode));		

	  // try Get and Set before its been initialized
	  checkExpression(pTimecode->GetTimecode(&startTC) == AAFRESULT_NOT_INITIALIZED,
	  																AAFRESULT_TEST_FAILED);
	  checkExpression(pTimecode->SetTimecode(&startTC) == AAFRESULT_NOT_INITIALIZED,
	  																AAFRESULT_TEST_FAILED);
	  
	  startTC.startFrame = 0;	// midnight
	  startTC.drop = kAAFTcNonDrop;
	  startTC.fps = 24;

	  // Initialize()
	  // Test Initialize with a NULL timecode
	  checkExpression(pTimecode->Initialize (zero, NULL) == AAFRESULT_NULL_PARAM,
	  																AAFRESULT_TEST_FAILED);
	  // test initialize with bogus drop value
	  startTC.drop = 2; 
	  checkExpression(pTimecode->Initialize (zero, &startTC) == AAFRESULT_INVALID_TIMECODE,
	  																AAFRESULT_TEST_FAILED);
	  // Set drop to valid value
	  startTC.drop = kAAFTcDrop;
	  checkResult(pTimecode->Initialize (zero, &startTC));
	  
	  // Make sure it can't be intialized again
	  checkExpression(pTimecode->Initialize (zero, &startTC) == AAFRESULT_ALREADY_INITIALIZED,
	  																AAFRESULT_TEST_FAILED);

       checkResult(pTimecode->GetTimecode (&startTC));

       // Check results !!
       checkExpression(startTC.startFrame == 0, AAFRESULT_TEST_FAILED);
       checkExpression(startTC.drop == kAAFTcDrop, AAFRESULT_TEST_FAILED);
       checkExpression(startTC.fps == 24, AAFRESULT_TEST_FAILED);

	  // SetTimecode()
	  startTC.startFrame = 108000;	// One hour
	  startTC.drop = kAAFTcNonDrop;
	  startTC.fps = 30;

	  // Test SetTimecode with a NULL timecode
	  checkExpression(pTimecode->SetTimecode(NULL) == AAFRESULT_NULL_PARAM,
	  												AAFRESULT_TEST_FAILED);
	  // test SetTimecode with bogus drop value
	  startTC.drop = 2; 
	  checkExpression(pTimecode->SetTimecode (&startTC) == AAFRESULT_INVALID_TIMECODE,
	  															AAFRESULT_TEST_FAILED);
	  startTC.drop = kAAFTcNonDrop;
	  checkResult(pTimecode->SetTimecode(&startTC));

	  checkResult(pTimecode->QueryInterface (IID_IAAFSegment, (void **)&pSeg));

	  aafRational_t editRate = { 0, 1};
	  checkResult(pMob->AppendNewTimelineSlot (editRate,
											   pSeg,
											   0,
											   L"timecode",
											   0,
											   &pNewSlot));
		
	  checkResult(pHeader->AddMob(pMob));
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

  if (pTimecode)
		pTimecode->Release();

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
  // IAAFSession *				pSession = NULL;
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  IEnumAAFMobs*				pMobIter = NULL;
  IEnumAAFMobSlots*			pEnum = NULL;
  IAAFMob*					pMob = NULL;
  IAAFMobSlot*				pMobSlot = NULL;
  IAAFSegment*				pSeg = NULL;
  IAAFTimecode*				pTimecode = NULL;
  aafTimecode_t				startTC;

  aafProductIdentification_t	ProductInfo;
  aafNumSlots_t				numMobs;
  HRESULT						hr = S_OK;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk. NOT!";
  ProductInfo.productName = L"AAFTimecode test. NOT!";
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

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

    checkResult(pHeader->GetMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
      checkResult(pMob->GetSlots (&pEnum));

      while (AAFRESULT_SUCCESS == pEnum->NextOne (&pMobSlot))
      {
        checkResult(pMobSlot->GetSegment (&pSeg));
        // Get a Timecode interface 
        checkResult(pSeg->QueryInterface (IID_IAAFTimecode, (void **)&pTimecode));
        checkResult(pTimecode->GetTimecode (&startTC));
        checkExpression(pTimecode->GetTimecode (NULL) == AAFRESULT_NULL_PARAM,
        													 AAFRESULT_TEST_FAILED);
        // Check results !!
        checkExpression(startTC.startFrame == 108000, AAFRESULT_TEST_FAILED);
        checkExpression(startTC.drop == kAAFTcNonDrop, AAFRESULT_TEST_FAILED);
        checkExpression(startTC.fps == 30, AAFRESULT_TEST_FAILED);

        pTimecode->Release();
        pTimecode = NULL;

        pSeg->Release();
        pSeg = NULL;
      }

      pEnum->Release();
      pEnum = NULL;

      pMob->Release();
      pMob = NULL;
	  }
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

	// Cleanup and return
  if (pTimecode)
		pTimecode->Release();

	if (pSeg)
		pSeg->Release();

  if (pMobSlot)
		pMobSlot->Release();

  if (pEnum)
    pEnum->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

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

extern "C" HRESULT CAAFTimecode_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFTimecodeTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTimecodeMob_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


