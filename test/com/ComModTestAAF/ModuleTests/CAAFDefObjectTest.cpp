// @doc INTERNAL
// @com This file implements the module test for CAAFDefObject
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

/*************************************************************************
 * 
 * @module AAFDefObject | AAFDefObject is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFDefObject specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/




#include "AAF.h"

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFContainerDefs.h"
#include "aafUtils.h"

#include "CAAFBuiltinDefs.h"

// Some handy smart pointers
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDefObject>             IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFPluginDescriptor>      IAAFPluginDescriptorSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>            IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IEnumAAFPluginDescriptors> IEnumAAFPluginDescriptorsSP;


//
// Some guids
//

// {005DFB20-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestTypeId = 
{ 0x5dfb20, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {005DFB21-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID1 = 
{ 0x5dfb21, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {005DFB22-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID2 = 
{ 0x5dfb22, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {005DFB23-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID3 = 
{ 0x5dfb23, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };



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

static wchar_t *sName = L"Test Descriptor Name";
static wchar_t *sDescription = L"Test Descriptor Description";


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFDefObject Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kAAFVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	if(mode == kAAFMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

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
	IAAFFile*			pFile = NULL;
	IAAFHeader *		pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFContainerDef*	pContainerDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
/*	long				test;
*/

  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


	// Create the AAF file
	checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
    bFileOpen = true;

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
	CAAFBuiltinDefs defs (pDictionary);

	checkResult(defs.cdContainerDef()->
				CreateInstance(IID_IAAFContainerDef, 
							   (IUnknown **)&pContainerDef));
    
	checkResult(pContainerDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

	checkResult(pContainerDef->Initialize (ContainerAAF, sName, sDescription));
	checkResult(pDictionary->RegisterContainerDef(pContainerDef));

	//
	// Test GetAUID() using a type def
	//
	IAAFTypeDefIntSP pTypeDef;
	checkResult (defs.cdTypeDefInt()->
				 CreateInstance (IID_IAAFTypeDefInt,
								 (IUnknown **)&pTypeDef));
	checkResult (pTypeDef->Initialize (kTestTypeId,
									   1,
									   kAAFFalse,
									   L"Test Unsigned Byte"));

	//
	// test Append, Prepend, and enum plugin descriptor using same type def
	//
	IAAFPluginDescriptorSP pd1;
	checkResult (defs.cdPluginDescriptor()->
				 CreateInstance (IID_IAAFPluginDescriptor,
								 (IUnknown **)&pd1));
	checkResult (pd1->Initialize (kTestPluginDescID1,
							L"PluginDesc1",
							L"Plugin Descriptor 1 description"));
	checkResult (pDictionary->RegisterPluginDef (pd1));

	IAAFPluginDescriptorSP pd2;
	checkResult (defs.cdPluginDescriptor()->
				 CreateInstance (IID_IAAFPluginDescriptor,
								 (IUnknown **)&pd2));
	checkResult (pd2->Initialize (kTestPluginDescID2,
							L"PluginDesc2",
							L"Plugin Descriptor 2 description"));
	checkResult (pDictionary->RegisterPluginDef (pd2));

	IAAFPluginDescriptorSP pd3;
	checkResult (defs.cdPluginDescriptor()->
				 CreateInstance (IID_IAAFPluginDescriptor,
								 (IUnknown **)&pd3));
	checkResult (pd3->Initialize (kTestPluginDescID3,
							L"PluginDesc3",
							L"Plugin Descriptor 3 description"));
	checkResult (pDictionary->RegisterPluginDef (pd3));

	IAAFDefObjectSP pDefObj;
	checkResult(pTypeDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDefObj));

	// pd2 will eventually be second one
	checkResult (pDefObj->AppendPluginDef (pd2));
	// pd3 will eventually be third one
	checkResult (pDefObj->AppendPluginDef (pd3));
	// pd1 will be first one
	checkResult (pDefObj->PrependPluginDef (pd1));

	IAAFTypeDefSP ptd;
	checkResult(pTypeDef->QueryInterface (IID_IAAFTypeDef,
                                          (void **)&ptd));
	checkResult (pDictionary->RegisterTypeDef (ptd));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pDef)
    pDef->Release();

  if (pContainerDef)
    pContainerDef->Release();

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

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFContainerDefs *pPlug = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFContainerDef*	pContainerDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	wchar_t				testString[256];

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetContainerDefs(&pPlug));
		checkResult(pPlug->NextOne(&pContainerDef));
		checkResult(pContainerDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName) == 0, AAFRESULT_TEST_FAILED);

		aafUInt32 nameLen;
		checkResult (pDef->GetNameBufLen (&nameLen));
		checkExpression (((wcslen (sName)+1) * sizeof (aafCharacter) == nameLen),
						 AAFRESULT_TEST_FAILED);

		checkResult(pDef->GetDescription (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sDescription) == 0, AAFRESULT_TEST_FAILED);

		checkResult (pDef->GetDescriptionBufLen (&nameLen));
		checkExpression (((wcslen (sDescription)+1) * sizeof (aafCharacter) == nameLen),
						 AAFRESULT_TEST_FAILED);

		IAAFTypeDefSP pTypeDef;
		checkResult (pDictionary->LookupTypeDef (kTestTypeId, &pTypeDef));

		IAAFDefObjectSP pDefObj;
		checkResult(pTypeDef->QueryInterface (IID_IAAFDefObject,
											  (void **)&pDefObj));

		aafUID_t id;
		checkResult (pDefObj->GetAUID (&id));
		checkExpression (0 != EqualAUID (&id, &kTestTypeId), AAFRESULT_TEST_FAILED);

		IEnumAAFPluginDescriptorsSP epd;
		checkResult (pDefObj->GetPluginDefs (&epd));

		IAAFPluginDescriptorSP pd;
		// Check that plugin descriptors are entered in order.
		checkResult (epd->NextOne (&pd));
		checkResult (pd->GetAUID (&id));
		checkExpression (0 != EqualAUID (&id, &kTestPluginDescID1), AAFRESULT_TEST_FAILED);

		checkResult (epd->NextOne (&pd));
		checkResult (pd->GetAUID (&id));
		checkExpression (0 != EqualAUID (&id, &kTestPluginDescID2), AAFRESULT_TEST_FAILED);

		checkResult (epd->NextOne (&pd));
		checkResult (pd->GetAUID (&id));
		checkExpression (0 != EqualAUID (&id, &kTestPluginDescID3), AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pHeader)
		pHeader->Release();
      
	if (pPlug)
		pPlug->Release();

	if (pDef)
		pDef->Release();
      
	if (pContainerDef)
		pContainerDef->Release();
            
	if (pDictionary)
		pDictionary->Release();
            
	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CAAFDefObject_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFDefObjectTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFDefObject_test...Caught general C++ exception!" << endl; 
	}

	return hr;
}



