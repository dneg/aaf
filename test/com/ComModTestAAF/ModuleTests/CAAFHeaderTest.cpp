// @doc INTERNAL
// @com This file implements the module test for CAAFHeader.
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


const aafUInt32 gMaxMobCount = 5;


// Utility class to implement the test.
struct HeaderTest
{
  HeaderTest();
  ~HeaderTest();

  void createFile(wchar_t *pFileName);
  void openFile(wchar_t *pFileName);

  void createFileMob(int itemNumber);
  void createEssenceData(IAAFSourceMob *pSourceMob);
  void openMobs();
  void openEssenceData();

  int formatMobName(int itemNumber, wchar_t* mobName);
  void cleanupReferences();
  void check(HRESULT hr);
  void removeTestFile(const wchar_t* pFileName);

  // Shared member data:
  HRESULT _hr;
  aafProductIdentification_t _productInfo;
  IAAFFile *_pFile;
  bool _bFileOpen;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;

  IAAFMob *_pMob;
  IAAFSourceMob *_pSourceMob;
  IEnumAAFMobs *_pEnumMobs;
  IAAFEssenceDescriptor *_pEssenceDescriptor;
  IAAFFileDescriptor *_pFileDescriptor;
  IEnumAAFEssenceData *_pEnumEssenceData;
  IAAFEssenceData *_pEssenceData;

  aafMobID_t _mobID[gMaxMobCount];
};

extern "C" HRESULT CAAFHeader_test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  wchar_t fileName[] = L"AAFHeaderTest.aaf";
  HeaderTest ht;

  try
  {
    ht.createFile(fileName);
    ht.openFile(fileName);
  }
  catch (HRESULT& ehr)
  {
    // thrown by HeaderTest_check() method.
    hr = ehr;
  }
  catch (...)
  {
    cerr << "CAAFHeader_test...Caught general C++"
    " exception!" << endl; 
  }

  // Cleanup our object if it exists.

  // When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFHeader methods have not been implemented:" << endl; 
		cout << "     LookupMob" << endl; 
		cout << "     IsEssenceDataPresent - needs unit test" << endl; 
		cout << "     GetLastIdentification" << endl; 
		cout << "     GetIdentificationByGen - needs unit test" << endl; 
		cout << "     GetNumIdents - needs unit test" << endl; 
		cout << "     EnumAAFIdents - needs unit test" << endl; 
		cout << "     GetRefImplVersion - needs unit test" << endl; 
		cout << "     GetFileRevision - needs unit test" << endl; 
		cout << "     GetLastModified - needs unit test" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

  return hr;
}



HeaderTest::HeaderTest():
  _hr(AAFRESULT_SUCCESS),
  _pFile(NULL),
  _bFileOpen(false),
  _pHeader(NULL),
  _pDictionary(NULL),
  _pMob(NULL),
  _pSourceMob(NULL),
  _pEnumMobs(NULL),
  _pEssenceDescriptor(NULL),
  _pFileDescriptor(NULL),
  _pEnumEssenceData(NULL),
  _pEssenceData(NULL)
{
  _productInfo.companyName = L"AAF Developers Desk";
  _productInfo.productName = L"AAFHeader Test";
  _productInfo.productVersion.major = 1;
  _productInfo.productVersion.minor = 0;
  _productInfo.productVersion.tertiary = 0;
  _productInfo.productVersion.patchLevel = 0;
  _productInfo.productVersion.type = kVersionUnknown;
  _productInfo.productVersionString = NULL;
  _productInfo.productID = UnitTestProductID;
  _productInfo.platform = NULL;

  // Generate the ids for each created mob.
  for (int i = 0; i < gMaxMobCount; ++i)
	  CoCreateGuid((GUID *)&_mobID[i]);
}

HeaderTest::~HeaderTest()
{
  cleanupReferences();
}

void HeaderTest::cleanupReferences()
{
  if (NULL != _pEssenceData)
  {
    _pEssenceData->Release();
    _pEssenceData = NULL;
  }

  if (NULL != _pEnumEssenceData)
  {
    _pEnumEssenceData->Release();
    _pEnumEssenceData = NULL;
  }

  if (NULL != _pFileDescriptor)
  {
    _pFileDescriptor->Release();
    _pFileDescriptor = NULL;
  }

  if (NULL != _pEssenceDescriptor)
  {
    _pEssenceDescriptor->Release();
    _pEssenceDescriptor = NULL;
  }

  if (NULL != _pSourceMob)
  {
    _pSourceMob->Release();
    _pSourceMob = NULL;
  }

  if (NULL != _pMob)
  {
    _pMob->Release();
    _pMob = NULL;
  }

  if (NULL != _pEnumMobs)
  {
    _pEnumMobs->Release();
    _pEnumMobs = NULL;
  }

  if (NULL != _pDictionary)
  {
    _pDictionary->Release();
    _pDictionary = NULL;
  }

  if (NULL != _pHeader)
  {
    _pHeader->Release();
    _pHeader = NULL;
  }

  if (NULL != _pFile)
  {
    if (_bFileOpen)
      _pFile->Close();
    _pFile->Release();
    _pFile = NULL;
  }
}

inline void HeaderTest::check(HRESULT hr)
{
  if (AAFRESULT_SUCCESS != (_hr = hr))
    throw hr;
}

// Cross-platform utility to delete a file.
void HeaderTest::removeTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}


void HeaderTest::createFile(wchar_t *pFileName)
{
  // Remove the previous test file if any.
  removeTestFile(pFileName);
   
  check(AAFFileOpenNewModify(pFileName, 0, &_productInfo, &_pFile));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));
  check(_pHeader->GetDictionary(&_pDictionary));

  for (aafUInt32 item = 0; item < gMaxMobCount; ++item)
    createFileMob(item);
 
  check(_pFile->Save());
  cleanupReferences();
}

void HeaderTest::openFile(wchar_t *pFileName)
{
  check(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));

  openEssenceData();

  openMobs();

  cleanupReferences();
}


int HeaderTest::formatMobName(int itemNumber, wchar_t* mobName)
{
  assert(0 <= itemNumber && gMaxMobCount > itemNumber);
  assert(NULL != mobName);

  char cBuffer[64];

  sprintf(cBuffer, "HeaderTest File Mob %d", itemNumber);
  size_t count = mbstowcs(mobName, cBuffer, strlen(cBuffer) + 1);
  if (-1 == count)
    check(AAFRESULT_INTERNAL_ERROR);
  return count;
}


void HeaderTest::createFileMob(int itemNumber)
{
  assert(_pFile && _pHeader && _pDictionary);
  assert(NULL == _pSourceMob);
  assert(NULL == _pMob);
  assert(NULL == _pFileDescriptor);
  assert(NULL == _pEssenceDescriptor);
  assert(NULL == _pSourceMob);
  assert(0 <= itemNumber && gMaxMobCount > itemNumber);

  // Format the mob name.
  wchar_t wcBuffer[128];
  formatMobName(itemNumber, wcBuffer);

  CAAFBuiltinDefs defs (_pDictionary);

  // Create a Mob
  check(_pDictionary->CreateInstance(defs.cdSourceMob(),
             IID_IAAFSourceMob, 
             (IUnknown **)&_pSourceMob));

  check(_pSourceMob->QueryInterface (IID_IAAFMob, (void **)&_pMob));
  
  check(_pMob->SetMobID(_mobID[itemNumber]));

  check(_pMob->SetName(wcBuffer));
  
  check(_pDictionary->CreateInstance(defs.cdFileDescriptor(),
              IID_IAAFEssenceDescriptor, 
              (IUnknown **)&_pFileDescriptor));

  check(_pFileDescriptor->QueryInterface (IID_IAAFEssenceDescriptor,
                                          (void **)&_pEssenceDescriptor));
  check(_pSourceMob->SetEssenceDescriptor (_pEssenceDescriptor));

  check(_pHeader->AddMob(_pMob));

  createEssenceData(_pSourceMob);

  // Cleanup instance data for reuse...
  _pEssenceDescriptor->Release();
  _pEssenceDescriptor = NULL;

  _pFileDescriptor->Release();
  _pFileDescriptor = NULL;

  _pMob->Release();
  _pMob = NULL;

  _pSourceMob->Release();
  _pSourceMob = NULL;
}

void HeaderTest::createEssenceData(IAAFSourceMob *pSourceMob)
{
  assert(_pFile && _pHeader && _pDictionary);
  assert(pSourceMob);
  assert(NULL == _pEssenceData);

  CAAFBuiltinDefs defs (_pDictionary);

  // Attempt to create an AAFEssenceData.
  check(_pDictionary->CreateInstance(defs.cdEssenceData(),
                         IID_IAAFEssenceData,
                         (IUnknown **)&_pEssenceData));

  check(_pEssenceData->SetFileMob(pSourceMob));
  check(_pHeader->AddEssenceData(_pEssenceData));
  
  _pEssenceData->Release();
  _pEssenceData = NULL;
}

void HeaderTest::openMobs()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumMobs);
  assert(NULL == _pMob);
  assert(NULL == _pSourceMob);


  aafNumSlots_t mobCount = 0;
  check(_pHeader->CountMobs(kAllMob, &mobCount));
  if (gMaxMobCount != mobCount)
    check(AAFRESULT_TEST_FAILED);
 
  check(_pHeader->GetMobs(NULL, &_pEnumMobs));
  for (aafUInt32 item = 0; item < mobCount; ++item)
  {
    check(_pEnumMobs->NextOne(&_pMob));

    // Validate the mob data.
	aafMobID_t mobID;
    check(_pMob->GetMobID(&mobID));
    if (0 != memcmp(&mobID, &_mobID[item], sizeof(mobID)))
      check(AAFRESULT_TEST_FAILED);

	wchar_t mobName[128], expectedMobName[128];
    aafUInt32 expectedMobNameLen = (formatMobName(item, expectedMobName) + 1) * 2;
	aafUInt32 mobNameLen;
	check(_pMob->GetNameBufLen(&mobNameLen));
	if (mobNameLen != expectedMobNameLen)
      check(AAFRESULT_TEST_FAILED);
    check(_pMob->GetName(mobName, 128));
    if (0 != memcmp(mobName, expectedMobName, expectedMobNameLen))
      check(AAFRESULT_TEST_FAILED);
	
	// Make sure that we actually created a source mob.
	check(_pMob->QueryInterface(IID_IAAFSourceMob, (void **)&_pSourceMob));
	
    _pSourceMob->Release();
    _pSourceMob = NULL;

    _pMob->Release();
    _pMob = NULL;
  }


  _pEnumMobs->Release();
  _pEnumMobs = NULL;
}

void HeaderTest::openEssenceData()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumEssenceData);
  assert(NULL == _pEssenceData);
  assert(NULL == _pSourceMob);

  aafUInt32 essenceDataCount = 0;
  check(_pHeader->CountEssenceData(&essenceDataCount));
  if (gMaxMobCount != essenceDataCount)
    check(AAFRESULT_TEST_FAILED);
 
  check(_pHeader->EnumEssenceData(&_pEnumEssenceData));
  for (aafUInt32 item = 0; item < essenceDataCount; ++item)
  {
    check(_pEnumEssenceData->NextOne(&_pEssenceData));

    // Make sure that the essence data still references
    // a valid mob.
    check(_pEssenceData->GetFileMob(&_pSourceMob));

    _pSourceMob->Release();
    _pSourceMob = NULL;

    _pEssenceData->Release();
    _pEssenceData = NULL;
  }


  _pEnumEssenceData->Release();
  _pEnumEssenceData = NULL;
}

