// @doc INTERNAL
// @com This file implements the module test for CAAFSourceReference
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
 *  prior written permission of Avid Technology, Inc.
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
	IAAFSourceReference	*pSourceReference = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					inSourceID, outSourceID;
	aafUInt32 inMobSlotID, outMobSlotID;
	HRESULT						hr = S_OK;


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFSourceReference Test";
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
 		

		// Create an Abstract SourceReference
		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceReference,
								  IID_IAAFSourceReference, 
								  (IUnknown **)&pSourceReference));

		// module-specific tests go here
		//		Set Values.	

		inSourceID = DDEF_Picture;   // Could have been any other value !
		checkResult(pSourceReference->SetSourceID( inSourceID));
		
		inMobSlotID = 100;   // Could have been any other value !
		checkResult(pSourceReference->SetSourceMobSlotID( inMobSlotID));

		//	Now Get Values
		checkResult(pSourceReference->GetSourceID( &outSourceID));

		// Compare value with the one we set
		checkExpression(memcmp(&inSourceID, &outSourceID, sizeof(inSourceID)) == 0, AAFRESULT_TEST_FAILED);

		
		checkResult(pSourceReference->GetSourceMobSlotID( &outMobSlotID));

		// Compare value with the one we set
		checkExpression(inMobSlotID == outMobSlotID, AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (pSourceReference)
    pSourceReference->Release();

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
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	HRESULT						hr = S_OK;


	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
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
 

extern "C" HRESULT CAAFSourceReference_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFSourceReferenceTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CSourceReferences_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
