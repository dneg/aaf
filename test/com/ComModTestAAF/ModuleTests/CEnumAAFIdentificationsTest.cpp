// @doc INTERNAL
// @com This file implements the module test for CEnumAAFIdentifications
//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "AAF.h"

#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "CAAFBuiltinDefs.h"


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
inline void TestMethod(bool expression, HRESULT *pLocalhr)
{
  if (!expression)
    *pLocalhr = AAFRESULT_TEST_FAILED;
}
inline void PrintTestResult(char *testName, HRESULT localhr, HRESULT *phr)
{
  if (localhr == AAFRESULT_SUCCESS)
	cout<< "    "<< setw(40)<< setiosflags(ios::left) << testName<< "Passed"<< endl;
  else
	{
 	  cout<< "    " << setw(40)<< setiosflags(ios::left) << testName<< "FAILED"<< endl;
	  *phr = AAFRESULT_TEST_FAILED;
	}
}


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

#define COMPANY_NAME		L"AAF Developers Desk"
#define PRODUCT_NAME		L"AAFIdentification Test"
#define PRODUCT_NAME1		L"AAFIdentification Test1"
#define PRODUCT_NAME2		L"AAFIdentification Test2"
#define PRODUCT_NAME3		L"AAFIdentification Test3"
#define PRODUCT_NAME4		L"AAFIdentification Test4"
#define TEST_VERSION		L"TEST VERSION"

static aafProductVersion_t			testVersion =  { 1, 0, 0, 0, kAAFVersionUnknown };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFDictionary				*pDictionary = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFIdentification			*pIdent = NULL;
	IAAFIdentification			*pNewIdent = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafUInt32					readNumIdents;
	ProductInfo.companyName = COMPANY_NAME;
	ProductInfo.productName = PRODUCT_NAME;
	ProductInfo.productVersionString = TEST_VERSION;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.productVersion = &testVersion;
	aafWChar					*pProductName[5] = { PRODUCT_NAME,
													PRODUCT_NAME1,
													PRODUCT_NAME2,
													PRODUCT_NAME3,
													PRODUCT_NAME4};

	try 
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

 	    checkResult(pHeader->GetDictionary(&pDictionary));
	    CAAFBuiltinDefs defs (pDictionary);
		
		checkResult(pHeader->CountIdentifications(&readNumIdents));
		checkExpression(1 == readNumIdents, AAFRESULT_TEST_FAILED);		

		for (int i=1; i < 5; ++i)
		{
			checkResult(defs.cdIdentification()->
				CreateInstance(IID_IAAFIdentification, 
							   (IUnknown **)&pNewIdent));
		
			checkResult(pNewIdent->Initialize(COMPANY_NAME,
										   pProductName[i],
										   TEST_VERSION,
										   UnitTestProductID));
										   
			checkResult(pNewIdent->SetProductVersion(testVersion));
		
			checkResult(pHeader->AppendIdentification(pNewIdent));
			pNewIdent->Release();
		}
		
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
	if (pIdent)
		pIdent->Release();
	
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
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFIdentifications *	pEnum = NULL;
	IEnumAAFIdentifications *	pEnumClone = NULL;
	IAAFIdentification			*pIdent = NULL;
  	IAAFIdentification*			pIdentArray[5] = {0};
	HRESULT						hr = S_OK;
	aafUInt32					readNumIdents, readNameLen, numFetched;
	aafWChar					readBuf[256];
	char 						testName[35];
	HRESULT						localhr = AAFRESULT_SUCCESS;
	aafUInt32					i;
	aafWChar					*pProductName[5] = { PRODUCT_NAME,
													PRODUCT_NAME1,
													PRODUCT_NAME2,
													PRODUCT_NAME3,
													PRODUCT_NAME4};

	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		checkResult(pHeader->CountIdentifications(&readNumIdents));
		checkExpression(5 == readNumIdents, AAFRESULT_TEST_FAILED);

		
		checkResult(pHeader->GetIdentifications (&pEnum));
		
	/* Test the Reset method *******************************/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "Reset()");
		TestMethod (pEnum->Reset() == AAFRESULT_SUCCESS, &localhr);
		PrintTestResult(testName, localhr, &hr);

	/* Test the NextOne method ******************************/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "NextOne()");
		pIdent = NULL;

		// Call NextOne once for each mob for a total of readNumIdents times	
		for (i=0; i<readNumIdents; i++)	{
			if (pEnum->NextOne(&pIdent) == AAFRESULT_SUCCESS)	{
				/***/
				TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdent->Release();
				pIdent = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}			

		// Make sure we are at the end
		TestMethod(pEnum->NextOne(&pIdent) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod(pIdent == NULL, &localhr);		
		
	
		pEnum->Reset();
		// this should return AAFRESULT_NULL_PARAM
		TestMethod(pEnum->NextOne(NULL) == AAFRESULT_NULL_PARAM, &localhr);
		TestMethod(pIdent == NULL, &localhr);		

		PrintTestResult(testName, localhr, &hr);

	/* Test the Skip method ******************************/
		localhr = S_OK;
		strcpy(testName, "Skip()");
		pEnum->Reset();

		// skip over objects one at a time.
		for (i=1; i<readNumIdents; i=i+2)
		{
			TestMethod(pEnum->Skip(1) == AAFRESULT_SUCCESS, &localhr);
			/***/
			TestMethod(pEnum->NextOne(&pIdent) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
			/***/
			pIdent->Release();
			pIdent = NULL;
		}
		TestMethod(pEnum->Skip(1) == AAFRESULT_SUCCESS, &localhr);

		// Make sure we are at the end.
		TestMethod(pEnum->Skip(1) == AAFRESULT_NO_MORE_OBJECTS, &localhr);

		pEnum->Reset();
		// Skip over multiple Components at a time.		
		for (i=2; i<=readNumIdents; i++)	{
			TestMethod(pEnum->Skip(i) == AAFRESULT_SUCCESS, &localhr);
			/***/
			if (i<readNumIdents)
			{
				TestMethod(pEnum->NextOne(&pIdent) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdent->Release();
				pIdent = NULL;
			}
			pEnum->Reset();
		}

		// Make sure we can't skip past the end.
		TestMethod(pEnum->Skip(i+1) == AAFRESULT_NO_MORE_OBJECTS, &localhr);

		PrintTestResult(testName, localhr, &hr);

	/* Next()  ******************************************/
		localhr = S_OK;
		strcpy(testName, "Next()");
		pEnum->Reset();
		pIdent = NULL;
		numFetched = 0;

		// Iterate thru the objects using Next doing 1 at a time
		for ( i=0; i<readNumIdents ;i++) {
			if (pEnum->Next(1, &pIdent, &numFetched) == AAFRESULT_SUCCESS)	{
				TestMethod(NULL != pIdent, &localhr);	
				TestMethod(1 == numFetched, &localhr);
				TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdent->Release();
		 	    pIdent = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}

		// Make sure we are at the end
		TestMethod(pEnum->Next(1, &pIdent, &numFetched) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod(0 == numFetched, &localhr);
		TestMethod(NULL == pIdent, &localhr);
					
		// Test the Next method filling out an array
		numFetched = 0;
		pEnum->Reset();
		for ( i=2; i<=readNumIdents ;i++)	{
			if (pEnum->Next(i, pIdentArray, &numFetched) == AAFRESULT_SUCCESS)	{
				TestMethod(i == numFetched, &localhr);

				for (i = 0; i < numFetched; i++) {
					if (pIdentArray[i] != NULL)	// should have been set
					{
						TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
						TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
						TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
						/***/
						pIdentArray[i]->Release();
						pIdentArray[i] = NULL;
					}
					else {
						localhr = AAFRESULT_TEST_FAILED;
					}
				}
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
			
			pEnum->Reset();
		}

		// Make sure we can't get more than actually exist
		numFetched = 0;
		TestMethod(pEnum->Next(readNumIdents+1, pIdentArray, &numFetched) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod(numFetched == readNumIdents, &localhr);		
		for (i = 0; i < numFetched; i++) {
			if (pIdentArray[i] != NULL)	
			{
				TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdentArray[i]->Release();
				pIdentArray[i] = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}
		
	
		pEnum->Reset();
		pEnum->Skip(2);
		// Start at offset and make sure we can't go past the end to fill the array
		TestMethod(pEnum->Next(readNumIdents, pIdentArray, &numFetched) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod((readNumIdents-2) == numFetched, &localhr);
		
		for (i = 0; i < readNumIdents-2; i++) {
			if (pIdentArray[i] != NULL)
			{	
				TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i+2], readBuf) == 0, &localhr);
				/***/
				pIdentArray[i]->Release();
				pIdentArray[i] = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}

		pEnum->Reset();
		// Make sure it returns AAFRESULT_NULL_PARAM
		TestMethod(pEnum->Next(1, NULL, &numFetched) == AAFRESULT_NULL_PARAM, &localhr);
		TestMethod(pEnum->Next(1, pIdentArray, NULL) == AAFRESULT_NULL_PARAM, &localhr);

		PrintTestResult(testName, localhr, &hr);

	/* Clone() ************************************/

		// Test the Clone method with with enumerator at begining
		localhr = S_OK;
		pEnum->Reset();
		strcpy(testName, "Clone()");
		pIdent = NULL;

		if (pEnum->Clone(&pEnumClone) == AAFRESULT_SUCCESS)	{
			for (i=0; i < readNumIdents; i++) {
				if (pEnumClone->NextOne(&pIdent) == AAFRESULT_SUCCESS)	{
					TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
					TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
					TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
					/***/
					pIdent->Release();
	    			pIdent = NULL;
				}
				else {
					localhr = AAFRESULT_TEST_FAILED;
				}
			}
	
		TestMethod(pEnumClone->NextOne(&pIdent) == AAFRESULT_NO_MORE_OBJECTS, &localhr);

		pEnumClone->Reset();
		numFetched = 0;
		TestMethod(pEnumClone->Next(readNumIdents, pIdentArray, &numFetched) == AAFRESULT_SUCCESS, &localhr);
		TestMethod(readNumIdents == numFetched, &localhr);
		
		for (i = 0; i < readNumIdents; i++)	{
			if (pIdentArray[i] != NULL)	{
				TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdentArray[i]->Release();
				pIdentArray[i] = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}

		pEnumClone->Reset();
		numFetched = 0;
		TestMethod(pEnumClone->Next(readNumIdents+1, pIdentArray, &numFetched) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod(readNumIdents == numFetched, &localhr);
		
		for (i = 0; i < readNumIdents; i++) {
			if (pIdentArray[i] != NULL)	{
				TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i], readBuf) == 0, &localhr);
				/***/
				pIdentArray[i]->Release();
				pIdentArray[i] = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}

		pEnumClone->Reset();
		numFetched = 0;
		pEnumClone->Skip(1);

		TestMethod(pEnumClone->Next(readNumIdents, pIdentArray, &numFetched) == AAFRESULT_NO_MORE_OBJECTS, &localhr);
		TestMethod((readNumIdents-1) == numFetched, &localhr);
		
		for (i = 0; i < readNumIdents-1; i++) {
			if (pIdentArray[i] != NULL)	{
				TestMethod(pIdentArray[i]->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(pIdentArray[i]->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
				TestMethod(wcscmp(pProductName[i+1], readBuf) == 0, &localhr);
				/***/
				pIdentArray[i]->Release();
				pIdentArray[i] = NULL;
			}
			else {
				localhr = AAFRESULT_TEST_FAILED;
			}
		}
	
		pEnumClone->Release();
	 	pEnumClone = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;
	
	// Test the Clone method with with enumerator at end.
	// Indirectly tests the Skip and Reset methods.
	pEnum->Reset();
	numFetched = 0;
	pEnum->Skip(readNumIdents-1);
	if (pEnum->Clone(&pEnumClone) == AAFRESULT_SUCCESS) {
		if (pEnumClone->NextOne(&pIdent) == AAFRESULT_SUCCESS)	{
			TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(wcscmp(pProductName[readNumIdents-1], readBuf) == 0, &localhr);
			/***/
			pIdent->Release();
		    pIdent = NULL;
		}
		TestMethod(pEnumClone->NextOne(&pIdent) == AAFRESULT_NO_MORE_OBJECTS, &localhr);

		pEnumClone->Release();
  		pEnumClone = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	// Test the Clone method with with enumerator in the middle.
	// Indirectly tests the Skip and Reset methods.
	pEnum->Reset();
	pEnum->Skip(readNumIdents-2);
	if (pEnum->Clone(&pEnumClone) == AAFRESULT_SUCCESS)	{
		pEnumClone->Skip(1);
		if (pEnumClone->NextOne(&pIdent) == AAFRESULT_SUCCESS)	{
			TestMethod(pIdent->GetProductNameBufLen (&readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(pIdent->GetProductName (readBuf, readNameLen) == AAFRESULT_SUCCESS, &localhr);
			TestMethod(wcscmp(pProductName[readNumIdents-2+1], readBuf) == 0, &localhr);
			/***/
			pIdent->Release();
		    pIdent = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
		
		TestMethod(pEnumClone->NextOne(&pIdent) == AAFRESULT_NO_MORE_OBJECTS, &localhr);

		pEnumClone->Release();
  		pEnumClone = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;


	pEnum->Reset();
	if (pEnum->Clone(&pEnumClone) == AAFRESULT_SUCCESS)	{
		TestMethod(pEnumClone->Next(1, NULL, &numFetched) == AAFRESULT_NULL_PARAM, &localhr);	
		TestMethod(pEnumClone->Next(1, pIdentArray, NULL) == AAFRESULT_NULL_PARAM, &localhr);

		pEnumClone->Release();
 		pEnumClone = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	
		PrintTestResult(testName, localhr, &hr);

		checkResult(pFile->Close());
		bFileOpen = false;
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
		
	if (pHeader)
		pHeader->Release();
	
	if (pIdent)
		pIdent->Release();
	
	if (pEnum)
		pEnum->Release();
	
	if (pFile)
	{	// Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

extern "C" HRESULT CEnumAAFIdentifications_test(testMode_t mode);
extern "C" HRESULT CEnumAAFIdentifications_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFIdentificationsTest.aaf";
	
	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFMob_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	return hr;
}
