// @doc INTERNAL
// @com This file implements the module test for CAAFClassDef
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
#include "AAFResult.h"
#include "AAFSmartPointer.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;


// {CADEF283-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t ourPid1 = 
{ 0xcadef283, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {CADEF284-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t ourPid2 = 
{ 0xcadef284, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {CADEF285-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t ourPid3 = 
{ 0xcadef285, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


static wchar_t testFileName[] = L"AAFClassDef.aaf";


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


static HRESULT ClassDefTest ()
{
  HRESULT hr = E_FAIL;
  aafProductIdentification_t ProductInfo;
  IAAFFileSP       pFile;
  IAAFHeaderSP     pHeader;
  IAAFDictionarySP pDict;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFClassDef Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  RemoveTestFile (testFileName);
  hr = AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile);
  if (FAILED (hr)) return hr;
  assert (pFile);

  hr = pFile->GetHeader (&pHeader);
  if (FAILED (hr)) return hr;
  assert (pHeader);

  hr = pHeader->GetDictionary (&pDict);
  if (FAILED (hr)) return hr;
  assert (pDict);
  IAAFTypeDefSP ptd;
  hr = pDict->LookupType (kAAFTypeID_UInt8, &ptd);
  if (FAILED (hr))	return hr;

  // Try to extend a typeDefInt.  Should fail.
  IAAFClassDefSP badClass1;
  hr = pDict->LookupClass (AUID_AAFTypeDefInt, &badClass1);
  if (FAILED (hr))	return hr;

  IAAFPropertyDefSP propDef1;
  hr = badClass1->AppendOptionalPropertyDef (ourPid1,
											L"First prop",
											ptd,
											&propDef1);
  if (AAFRESULT_NOT_EXTENDABLE != hr)
	return E_FAIL;

  // Try to extend an AAFObject.  Should fail, for now at least.
  IAAFClassDefSP badClass2;
  hr = pDict->LookupClass (AUID_AAFObject, &badClass2);
  if (FAILED (hr))	return hr;

  IAAFPropertyDefSP propDef2;
  hr = badClass2->AppendOptionalPropertyDef (ourPid2,
											 L"Second prop",
											 ptd,
											 &propDef2);
  if (AAFRESULT_NOT_EXTENDABLE != hr)
	return E_FAIL;

  // Try to extend an AAFSequence.  Should succeed.
  IAAFClassDefSP goodClass;
  hr = pDict->LookupClass (AUID_AAFSequence, &goodClass);
  if (FAILED (hr))	return hr;

  IAAFPropertyDefSP propDef3;
  hr = goodClass->AppendOptionalPropertyDef (ourPid3,
											 L"Third prop",
											 ptd,
											 &propDef3);
  if (FAILED (hr))
	return hr;

  hr = pFile->Save();
  if (FAILED (hr))
	return hr;

  hr = pFile->Close();
  if (FAILED (hr))
	return hr;

  return AAFRESULT_SUCCESS;
}


extern "C" HRESULT CAAFClassDef_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  hr = ClassDefTest ();
	}
  catch (...)
	{
	  cerr << "CAAFClassDef_test"
		   << "...Caught general C++ exception!" << endl; 
	}

  // When all of the functionality of this class is tested, we can
  // return success.  When a method and its unit test have been
  // implemented, remove it from the list. 
  if (SUCCEEDED(hr))
	{
	  cout << "The following AAFClassDef tests have not"
		   << "been implemented:" << endl;  
	  cout << "     Initialize" << endl; 
	  cout << "     GetPropertyDefs" << endl; 
	  cout << "     CountPropertyDefs" << endl; 
	  cout << "     AppendNewPropertyDef" << endl; 
	  cout << "     AppendOptionalPropertyDef" << endl; 
	  cout << "     LookupPropertyDef" << endl; 
	  cout << "     GetParent" << endl; 
	  hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}
  return hr;
}
