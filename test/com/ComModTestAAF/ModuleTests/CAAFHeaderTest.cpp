// @doc INTERNAL
// @com This file implements the module test for CAAFHeader.
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "AAF.h"

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"


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

  aafUID_t _mobID[gMaxMobCount];
};

extern "C" HRESULT CAAFHeader_test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  wchar_t fileName[] = L"HeaderTest.aaf";
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
  _productInfo.productName = L"Header Module Test";
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

  // Create a Mob
  check(_pDictionary->CreateInstance(&AUID_AAFSourceMob,
             IID_IAAFSourceMob, 
             (IUnknown **)&_pSourceMob));

  check(_pSourceMob->QueryInterface (IID_IAAFMob, (void **)&_pMob));
  
  check(_pMob->SetMobID(&_mobID[itemNumber]));

  check(_pMob->SetName(wcBuffer));
  
  check(_pDictionary->CreateInstance(&AUID_AAFFileDescriptor,
              IID_IAAFEssenceDescriptor, 
              (IUnknown **)&_pFileDescriptor));

  check(_pFileDescriptor->QueryInterface (IID_IAAFEssenceDescriptor,
                                          (void **)&_pEssenceDescriptor));
  check(_pSourceMob->SetEssenceDescriptor (_pEssenceDescriptor));

  check(_pHeader->AppendMob(_pMob));

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


  // Attempt to create an AAFEssenceData.
  check(_pDictionary->CreateInstance(&AUID_AAFEssenceData,
                         IID_IAAFEssenceData,
                         (IUnknown **)&_pEssenceData));

  check(_pEssenceData->SetFileMob(pSourceMob));
  check(_pHeader->AppendEssenceData(_pEssenceData));
  
  _pEssenceData->Release();
  _pEssenceData = NULL;
}

void HeaderTest::openMobs()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumMobs);
  assert(NULL == _pMob);
  assert(NULL == _pSourceMob);


  aafInt32 mobCount = 0;
  check(_pHeader->GetNumMobs(kAllMob, &mobCount));
  if (gMaxMobCount != mobCount)
    check(AAFRESULT_TEST_FAILED);
 
  check(_pHeader->EnumAAFAllMobs(NULL, &_pEnumMobs));
  for (aafInt32 item = 0; item < mobCount; ++item)
  {
    check(_pEnumMobs->NextOne(&_pMob));

    // Validate the mob data.
	aafUID_t mobID;
    check(_pMob->GetMobID(&mobID));
    if (0 != memcmp(&mobID, &_mobID[item], sizeof(mobID)))
      check(AAFRESULT_TEST_FAILED);

	wchar_t mobName[128], expectedMobName[128];
    int expectedMobNameLen = (formatMobName(item, expectedMobName) + 1) * 2;
	aafInt32 mobNameLen;
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
  check(_pHeader->GetNumEssenceData(&essenceDataCount));
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































































