// @doc INTERNAL
// @com This file implements the module test for CEnumAAFEssenceData
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#include "CEnumAAFEssenceData.h"
#include "CEnumAAFEssenceData.h"
#ifndef __CEnumAAFEssenceData_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


// Utility class to implement the test.
struct EnumEssenceDataTest
{
  EnumEssenceDataTest();
  ~EnumEssenceDataTest();

  void createFile(wchar_t *pFileName);
  void openFile(wchar_t *pFileName);

  void createFileMob(int itemNumber);
  void createEssenceData(IAAFSourceMob *pSourceMob);
  void openEssenceData();


  void cleanupReferences();
  void check(HRESULT hr);

  // Shared member data:
  HRESULT _hr;
  aafProductIdentification_t _productInfo;
  IAAFFile *_pFile;
  bool _bFileOpen;
  IAAFHeader *_pHeader;

  IAAFMob *_pMob;
  IAAFSourceMob *_pSourceMob;
  IAAFEssenceDescriptor *_pEssenceDescriptor;
  IAAFFileDescriptor *_pFileDescriptor;
  IEnumAAFEssenceData *_pEnumEssenceData;
  IAAFEssenceData *_pEssenceData;

  static const aafUInt32 _maxMobCount;
};

HRESULT CEnumAAFEssenceData::test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  wchar_t fileName[] = L"EnumEssenceDataTest.aaf";
  EnumEssenceDataTest edt;

  try
  {
    edt.createFile(fileName);
    edt.openFile(fileName);
  }
  catch (HRESULT& ehr)
  {
    // thrown by EnumEssenceDataTest::check() method.
    hr = ehr;
  }
  catch (...)
  {
    cerr << "CAAFEssenceData::test...Caught general C++"
    " exception!" << endl; 
  }

  // Cleanup our object if it exists.

  return hr;
}


const aafUInt32 EnumEssenceDataTest::_maxMobCount = 5;


EnumEssenceDataTest::EnumEssenceDataTest():
  _hr(AAFRESULT_SUCCESS),
  _pFile(NULL),
  _bFileOpen(false),
  _pHeader(NULL),
  _pMob(NULL),
  _pSourceMob(NULL),
  _pEssenceDescriptor(NULL),
  _pFileDescriptor(NULL),
  _pEnumEssenceData(NULL),
  _pEssenceData(NULL)
{
  _productInfo.companyName = L"AAF Developers Desk";
  _productInfo.productName = L"EnumEssenceData Module Test";
  _productInfo.productVersion.major = 1;
  _productInfo.productVersion.minor = 0;
  _productInfo.productVersion.tertiary = 0;
  _productInfo.productVersion.patchLevel = 0;
  _productInfo.productVersion.type = kVersionUnknown;
  _productInfo.productVersionString = NULL;
  _productInfo.productID = -1;
  _productInfo.platform = NULL;
}

EnumEssenceDataTest::~EnumEssenceDataTest()
{
  cleanupReferences();
}

void EnumEssenceDataTest::cleanupReferences()
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

inline void EnumEssenceDataTest::check(HRESULT hr)
{
  if (AAFRESULT_SUCCESS != (_hr = hr))
    throw hr;
}

void EnumEssenceDataTest::createFile(wchar_t *pFileName)
{
  check(CoCreateInstance(CLSID_AAFFile,
                         NULL, 
                         CLSCTX_INPROC_SERVER, 
                         IID_IAAFFile, 
                         (void **)&_pFile));
  check(_pFile->Initialize());
  check(_pFile->OpenNewModify(pFileName, 0, &_productInfo));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));

  for (aafUInt32 item = 0; item < _maxMobCount; ++item)
    createFileMob(item);
 
  cleanupReferences();
}

void EnumEssenceDataTest::openFile(wchar_t *pFileName)
{
  check(CoCreateInstance(CLSID_AAFFile,
                         NULL, 
                         CLSCTX_INPROC_SERVER, 
                         IID_IAAFFile, 
                         (void **)&_pFile));
  check(_pFile->Initialize());
  check(_pFile->OpenExistingRead(pFileName, 0));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));

  openEssenceData();

  cleanupReferences();
}

void EnumEssenceDataTest::createFileMob(int itemNumber)
{
  assert(_pFile && _pHeader);
  assert(NULL == _pSourceMob);
  assert(NULL == _pMob);
  assert(NULL == _pFileDescriptor);
  assert(NULL == _pEssenceDescriptor);
  assert(NULL == _pSourceMob);

  // Format the mob name.
  wchar_t wcBuffer[512];
  char cBuffer[256];
  sprintf(cBuffer, "EnumEssenceDataTest File Mob %d", itemNumber);
  size_t count = mbstowcs(wcBuffer, cBuffer, strlen(cBuffer) + 1);
  if (-1 == count)
    check(AAFRESULT_INTERNAL_ERROR);


  // Create a Mob
  check(CoCreateInstance(CLSID_AAFSourceMob,
              NULL, 
              CLSCTX_INPROC_SERVER, 
              IID_IAAFSourceMob, 
              (void **)&_pSourceMob));

  check(_pSourceMob->QueryInterface (IID_IAAFMob, (void **)&_pMob));
  
  aafUID_t newUID = {0};
  check(CoCreateGuid((GUID *)&newUID));
  check(_pMob->SetMobID(&newUID));
  check(_pMob->SetName(wcBuffer));
  
  check(CoCreateInstance(CLSID_AAFFileDescriptor,
              NULL, 
              CLSCTX_INPROC_SERVER, 
              IID_IAAFEssenceDescriptor, 
              (void **)&_pFileDescriptor));

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

void EnumEssenceDataTest::createEssenceData(IAAFSourceMob *pSourceMob)
{
  assert(_pFile && _pHeader);
  assert(pSourceMob);
  assert(NULL == _pEssenceData);


  // Attempt to create an AAFEssenceData.
  check(CoCreateInstance(CLSID_AAFEssenceData,
                         NULL, 
                         CLSCTX_INPROC_SERVER, 
                         IID_IAAFEssenceData,
                         (void **)&_pEssenceData));

  check(_pEssenceData->SetFileMob(pSourceMob));
  check(_pHeader->AppendEssenceData(_pEssenceData));
  
  _pEssenceData->Release();
  _pEssenceData = NULL;
}

void EnumEssenceDataTest::openEssenceData()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumEssenceData);
  assert(NULL == _pEssenceData);
  assert(NULL == _pSourceMob);

  aafUInt32 essenceDataCount = 0;
  check(_pHeader->GetNumEssenceData(&essenceDataCount));
  if (_maxMobCount != essenceDataCount)
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
