// @doc INTERNAL
// @com This file implements the module test for CEnumAAFIdentifications
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


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

#define COMPANY_NAME		L"AAF Developers Desk"
#define PRODUCT_NAME		L"AAFDictionary Test"
#define TEST_VERSION		L"TEST VERSION"

static aafProductVersion_t			testVersion =  { 1, 0, 0, 0, kAAFVersionUnknown };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFIdentification			*pIdent = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafUInt32					readNumIdents;
	ProductInfo.companyName = COMPANY_NAME;
	ProductInfo.productName = PRODUCT_NAME;
	ProductInfo.productVersionString = TEST_VERSION;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.productVersion = &testVersion;

	try 
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		checkResult(pHeader->CountIdentifications(&readNumIdents));
		checkExpression(1 == readNumIdents, AAFRESULT_TEST_FAILED);
		checkResult(pHeader->GetLastIdentification (&pIdent));
		
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
	IAAFIdentification			*pIdent = NULL;
	HRESULT						hr = S_OK;
	aafUInt32					readNumIdents, readNameLen;
	aafWChar					readBuf[256];
	aafUID_t					readUID;
	aafProductVersion_t			readVersion;

	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		checkResult(pHeader->CountIdentifications(&readNumIdents));
		checkExpression(1 == readNumIdents, AAFRESULT_TEST_FAILED);

		
		checkResult(pHeader->GetIdentifications (&pEnum));
		checkResult(pEnum->NextOne (&pIdent));
		/***/
		checkResult(pIdent->GetCompanyNameBufLen (&readNameLen));
		checkExpression(readNameLen < 256, AAFRESULT_TEST_FAILED);
		checkResult(pIdent->GetCompanyName (readBuf, readNameLen));
		checkExpression(wcscmp(COMPANY_NAME, readBuf) == 0, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pIdent->GetProductNameBufLen (&readNameLen));
		checkExpression(readNameLen < 256, AAFRESULT_TEST_FAILED);
		checkResult(pIdent->GetProductName (readBuf, readNameLen));
		checkExpression(wcscmp(PRODUCT_NAME, readBuf) == 0, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pIdent->GetProductVersionStringBufLen (&readNameLen));
		checkExpression(readNameLen < 256, AAFRESULT_TEST_FAILED);
		checkResult(pIdent->GetProductVersionString (readBuf, readNameLen));
		checkExpression(wcscmp(TEST_VERSION, readBuf) == 0, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pIdent->GetProductID(&readUID));
		checkExpression(memcmp(&readUID, &UnitTestProductID, sizeof(UnitTestProductID)) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pIdent->GetProductVersion(&readVersion));
		/***/
		checkExpression(readVersion.major == testVersion.major, AAFRESULT_TEST_FAILED);
		checkExpression(readVersion.minor == testVersion.minor, AAFRESULT_TEST_FAILED);
		checkExpression(readVersion.tertiary == testVersion.tertiary, AAFRESULT_TEST_FAILED);
		checkExpression(readVersion.patchLevel == testVersion.patchLevel, AAFRESULT_TEST_FAILED);
		checkExpression(readVersion.type == testVersion.type, AAFRESULT_TEST_FAILED);		
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

extern "C" HRESULT CEnumAAFIdentifications_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFIdentificationsTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFMob_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following EnumAAFIdentifications tests have not been implemented:" << endl; 
		cout << "     Next" << endl; 
		cout << "     Skip" << endl; 
		cout << "     Reset" << endl; 
		cout << "     Clone" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}
	return hr;
}
