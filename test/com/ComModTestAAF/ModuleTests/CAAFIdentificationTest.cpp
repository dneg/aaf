// @doc INTERNAL
// @com This file implements the module test for CAAFIdentification
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
#include <iomanip.h>
#include <stdio.h>

#if defined(macintosh) || defined(_MAC)
#include <wstring.h>
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "CAAFBuiltinDefs.h"
#include "AAFUtils.h"

static aafUID_t		newUID;
HRESULT				localhr = AAFRESULT_SUCCESS;
HRESULT				hr = S_OK;

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
inline void TestMethod(HRESULT expression, HRESULT r)
{
  if (expression != r)
    localhr = AAFRESULT_TEST_FAILED;
}
inline void PrintTestResult(char *testName)
{
  if (localhr == AAFRESULT_SUCCESS)
	cout<< "    "<< setw(40)<< setiosflags(ios::left) << testName<< "Passed"<< endl;
  else
	{
 	  cout<< "    " << setw(40)<< setiosflags(ios::left) << testName<< "FAILED"<< endl;
	  hr = AAFRESULT_TEST_FAILED;
	}
}

#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

#define COMPANY_NAME		L"AAF Developers Desk"
#define PRODUCT_NAME		L"AAFDictionary Test"
#define TEST_VERSION		L"TEST VERSION"

aafProductVersion_t			testVersion =  { 1, 0, 0, 0, kAAFVersionUnknown };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool 						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary				*pDictionary = NULL;
	IAAFIdentification			*pIdent = NULL;
	IAAFIdentification			*pTestIdent = NULL;
	aafUInt32					readNumIdents;
	char 						testName[35];
	aafCharacter 				*myBuffer;
	aafUInt32 					bufSize = 0;
	aafUInt32 					bufSize2 = 0;

	aafProductIdentification_t	ProductInfo;
  memset(&ProductInfo, 0, sizeof(ProductInfo));
	ProductInfo.companyName = COMPANY_NAME;
	ProductInfo.productName = PRODUCT_NAME;
	ProductInfo.productVersionString = TEST_VERSION;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.productVersion = &testVersion;
	hr = S_OK;

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
		checkResult(pHeader->GetLastIdentification (&pIdent));

		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	

	/* Initialize */
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "Initialize()");
		TestMethod(pTestIdent->Initialize(NULL,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   NULL,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   NULL,
									   UnitTestProductID), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_ALREADY_INITIALIZED);									   
		pTestIdent->Release();
		PrintTestResult(testName);
									   

	/* GetCompanyNameBufLen *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetCompanyNameBufLen()");
		bufSize = sizeof(COMPANY_NAME);
		bufSize2 = 0;
		TestMethod(pIdent->GetCompanyNameBufLen(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetCompanyNameBufLen(&bufSize2), AAFRESULT_SUCCESS);
		if (bufSize != bufSize2)
			localhr = AAFRESULT_TEST_FAILED;
		
		PrintTestResult(testName);

	/* GetCompanyName *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetCompanyName()");
		myBuffer = new aafCharacter [bufSize];
		TestMethod(pIdent->GetCompanyName(NULL, bufSize), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetCompanyName(myBuffer, bufSize-1), AAFRESULT_SMALLBUF);
		TestMethod(pIdent->GetCompanyName(myBuffer, bufSize), AAFRESULT_SUCCESS);
		if (wcscmp(myBuffer, COMPANY_NAME))
			localhr = AAFRESULT_TEST_FAILED;
								
		delete [] myBuffer;

		PrintTestResult(testName);		

	/* GetProductNameBufLen *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductNameBufLen()");
		bufSize = sizeof(PRODUCT_NAME);
		bufSize2 = 0;
		TestMethod(pIdent->GetProductNameBufLen(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetProductNameBufLen(&bufSize2), AAFRESULT_SUCCESS);
		if (bufSize != bufSize2)
			localhr = AAFRESULT_TEST_FAILED;
		
		PrintTestResult(testName);

	/* GetProductName *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductName()");
		myBuffer = new aafCharacter [bufSize];
		TestMethod(pIdent->GetProductName(NULL, bufSize), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetProductName(myBuffer, bufSize-1), AAFRESULT_SMALLBUF);
		TestMethod(pIdent->GetProductName(myBuffer, bufSize), AAFRESULT_SUCCESS);
		if (wcscmp(myBuffer, PRODUCT_NAME))
			localhr = AAFRESULT_TEST_FAILED;
							
		delete [] myBuffer;

		PrintTestResult(testName);
		
	/* GetProductVersionStringBufLen *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductVersionStringBufLen()");
		bufSize = sizeof(TEST_VERSION);
		bufSize2 = 0;
		TestMethod(pIdent->GetProductVersionStringBufLen(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetProductVersionStringBufLen(&bufSize2), AAFRESULT_SUCCESS);
		if (bufSize != bufSize2)
			localhr = AAFRESULT_TEST_FAILED;
		
		PrintTestResult(testName);

	/* GetProductVersionString *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductVersionString()");
		myBuffer = new aafCharacter [bufSize];
		TestMethod(pIdent->GetProductVersionString(NULL, bufSize), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetProductVersionString(myBuffer, bufSize-1), AAFRESULT_SMALLBUF);
		TestMethod(pIdent->GetProductVersionString(myBuffer, bufSize), AAFRESULT_SUCCESS);	
		if (wcscmp(myBuffer, TEST_VERSION))
			localhr = AAFRESULT_TEST_FAILED;
					
		delete [] myBuffer;

		PrintTestResult(testName);

	/* SetProductVersion *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "SetProductVersion()");

		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	

		TestMethod(pTestIdent->SetProductVersion(testVersion), AAFRESULT_NOT_INITIALIZED);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);
		TestMethod(pTestIdent->SetProductVersion(testVersion), AAFRESULT_SUCCESS);
		TestMethod(pIdent->SetProductVersion(testVersion), AAFRESULT_SUCCESS);
			
		pTestIdent->Release();
		PrintTestResult(testName);

	/* GetProductVersion *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductVersion()");
		aafProductVersion_t productVersion;
		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	

		TestMethod(pTestIdent->GetProductVersion(&productVersion), AAFRESULT_NOT_INITIALIZED);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);
									   
		TestMethod(pTestIdent->GetProductVersion(&productVersion), AAFRESULT_PROP_NOT_PRESENT);
		TestMethod(pTestIdent->SetProductVersion(testVersion), AAFRESULT_SUCCESS);

		TestMethod(pTestIdent->GetProductVersion(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->GetProductVersion(&productVersion), AAFRESULT_SUCCESS);
		if (productVersion.major != testVersion.major ||
			productVersion.minor != testVersion.minor ||
			productVersion.tertiary != testVersion.tertiary ||
			productVersion.patchLevel != testVersion.patchLevel ||
			productVersion.type != testVersion.type)
			localhr = AAFRESULT_TEST_FAILED;	


		pTestIdent->Release();
		PrintTestResult(testName);
		
	/* GetPlatformBufLen *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetPlatformBufLen()");
		TestMethod(pIdent->GetPlatformBufLen(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetPlatformBufLen(&bufSize), AAFRESULT_SUCCESS);
		
#if defined(macintosh) || defined(_MAC)
		if (bufSize != sizeof(L"MacOS"))
			localhr = AAFRESULT_TEST_FAILED;
#elif defined(_WIN32) || defined(WIN32)
		if (bufSize != sizeof(L"Win32"))
			localhr = AAFRESULT_TEST_FAILED;
#else
		if (bufSize != sizeof(L"Unknown"))
			localhr = AAFRESULT_TEST_FAILED;
#endif

		PrintTestResult(testName);

	/* GetPlatform *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetPlatform()");
		myBuffer = new aafCharacter [bufSize];
		TestMethod(pIdent->GetPlatform(NULL, bufSize), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetPlatform(myBuffer, bufSize-1), AAFRESULT_SMALLBUF);
		TestMethod(pIdent->GetPlatform(myBuffer, bufSize), AAFRESULT_SUCCESS);

#if defined(macintosh) || defined(_MAC)
		if (wcscmp(myBuffer, L"MacOS"))
			localhr = AAFRESULT_TEST_FAILED;
#elif defined(_WIN32) || defined(WIN32)
		if (wcscmp(myBuffer, L"Win32"))
			localhr = AAFRESULT_TEST_FAILED;
#else
		if (wcscmp(myBuffer, L"Unknown"))
			localhr = AAFRESULT_TEST_FAILED;
#endif
							
		delete [] myBuffer;

		PrintTestResult(testName);


	/* GetProductID *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetProductID()");
		aafUID_t thisProductID;
		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	
		TestMethod(pTestIdent->GetProductID(&thisProductID), AAFRESULT_NOT_INITIALIZED);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);
									   
		TestMethod(pTestIdent->GetProductID(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->GetProductID(&thisProductID), AAFRESULT_SUCCESS);
		
		if (memcmp(&UnitTestProductID, &thisProductID, sizeof(UnitTestProductID)) != 0)
			localhr = AAFRESULT_TEST_FAILED;
			
		pTestIdent->Release();
		PrintTestResult(testName);

	/* GetRefImplVersion *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetRefImplVersion()");
		aafProductVersion_t refImplVersion;
		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	
		TestMethod(pTestIdent->GetRefImplVersion(&refImplVersion), AAFRESULT_NOT_INITIALIZED);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);

		TestMethod(pTestIdent->GetRefImplVersion(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->GetRefImplVersion(&refImplVersion), AAFRESULT_SUCCESS);

		if (memcmp(&refImplVersion, &AAFReferenceImplementationVersion, sizeof(refImplVersion)) != 0)
			localhr = AAFRESULT_TEST_FAILED;
		
		pTestIdent->Release();
		PrintTestResult(testName);


	/* GetDate *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetDate()");
		aafTimeStamp_t timeStamp;
		aafTimeStamp_t startTimeStamp;
		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	
		TestMethod(pTestIdent->GetDate(&timeStamp), AAFRESULT_NOT_INITIALIZED);

		AAFGetDateTime (&startTimeStamp);
		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);

		TestMethod(pTestIdent->GetDate(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pTestIdent->GetDate(&timeStamp), AAFRESULT_SUCCESS);

		if (timeStamp.date.year != startTimeStamp.date.year ||
			timeStamp.date.month != startTimeStamp.date.month ||
			timeStamp.date.day != startTimeStamp.date.day)
			localhr = AAFRESULT_TEST_FAILED;

		if (timeStamp.time.hour != startTimeStamp.time.hour ||
			timeStamp.time.minute != startTimeStamp.time.minute ||
			timeStamp.time.second != startTimeStamp.time.second)
			localhr = AAFRESULT_TEST_FAILED;

		pTestIdent->Release();
		PrintTestResult(testName);


	/* GetGeneration *****/
		localhr = AAFRESULT_SUCCESS;
		strcpy(testName, "GetGeneration()");
		aafUID_t generation;
		checkResult(defs.cdIdentification()->
					CreateInstance(IID_IAAFIdentification, 
								   (IUnknown **)&pTestIdent));	
		TestMethod(pTestIdent->GetGenerationID(&generation), AAFRESULT_NOT_INITIALIZED);

		TestMethod(pTestIdent->Initialize(COMPANY_NAME,
									   PRODUCT_NAME,
									   TEST_VERSION,
									   UnitTestProductID), AAFRESULT_SUCCESS);

		TestMethod(pIdent->GetGenerationID(NULL), AAFRESULT_NULL_PARAM);
		TestMethod(pIdent->GetGenerationID(&generation), AAFRESULT_SUCCESS);
		
		pTestIdent->Release();
		PrintTestResult(testName);

		
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
	
	if (pDictionary)
		pDictionary->Release();

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
		checkResult(pHeader->GetLastIdentification (&pIdent));
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
	
	if (pFile)
	{	// Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

extern "C" HRESULT CAAFIdentification_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFIdentificationTest.aaf";
	
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
	
	return hr;
}
