// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "CAAFEssenceData.h"
#include "CAAFEssenceData.h"
#ifndef __CAAFEssenceData_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include <assert.h>
#include <string.h>


// Utility class to implement the test.
struct EssenceDataTest
{
  EssenceDataTest();
  ~EssenceDataTest();

  void createFile(wchar_t *pFileName);
  void openFile(wchar_t *pFileName);

  void createFileMob();
  void createEssenceData(IAAFSourceMob *pSourceMob);
  void openEssenceData();

  void writeEssenceData(IAAFEssenceData *pEssenceData,
		        const aafDataBuffer_t data,
			aafUInt32 dataSize);
  void readEssenceData(IAAFEssenceData *pEssenceData,
		        const aafDataBuffer_t data,
			aafUInt32 dataSize);

  void cleanupReferences();
  void setBufferSize(aafUInt32 bufferSize);
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
  aafDataBuffer_t _buffer;
  aafUInt32 _bufferSize;

  static const char _smiley[];
  static const char _frowney[];
};

HRESULT CAAFEssenceData::test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  wchar_t fileName[] = L"EssenceDataTest.aaf";
  EssenceDataTest edt;

  try
  {
    edt.createFile(fileName);
    edt.openFile(fileName);
  }
  catch (HRESULT& ehr)
  {
    // thrown by EssenceDataTest::check() method.
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


const char EssenceDataTest::_smiley[] =        /* 16x16 smiley face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "****************"
  "****************"
  " ** ******** ** "
  " *** ****** *** "
  "  ***  **  ***  "
  "   ****  ****   "
  "    ********    "
  "      ****     ";

const char EssenceDataTest::_frowney[] =        /* 16x16 frowney face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "*******  *******"
  "*****  **  *****"
  " *** ****** *** "
  " ** ******** ** "
  "  ************  "
  "   **********   "
  "    ********    "
  "      ****     ";



EssenceDataTest::EssenceDataTest():
  _hr(AAFRESULT_SUCCESS),
  _pFile(NULL),
  _bFileOpen(false),
  _pHeader(NULL),
  _pMob(NULL),
  _pSourceMob(NULL),
  _pEssenceDescriptor(NULL),
  _pFileDescriptor(NULL),
  _pEnumEssenceData(NULL),
  _pEssenceData(NULL),
  _buffer(NULL),
  _bufferSize(0)
{
  _productInfo.companyName = L"AAF Developers Desk";
  _productInfo.productName = L"EssenceData Module Test";
  _productInfo.productVersion.major = 1;
  _productInfo.productVersion.minor = 0;
  _productInfo.productVersion.tertiary = 0;
  _productInfo.productVersion.patchLevel = 0;
  _productInfo.productVersion.type = kVersionUnknown;
  _productInfo.productVersionString = NULL;
  _productInfo.productID = -1;
  _productInfo.platform = NULL;
}

EssenceDataTest::~EssenceDataTest()
{
  cleanupReferences();
}

void EssenceDataTest::cleanupReferences()
{

  if (NULL != _buffer)
  {
    CoTaskMemFree(_buffer);
    _buffer = NULL;
  }

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

void EssenceDataTest::setBufferSize(aafUInt32 bufferSize)
{
  // Allocate the buffer.
  if (NULL != _buffer && bufferSize > _bufferSize)
  {
    CoTaskMemFree(_buffer);
    _buffer = NULL;
  }

  // Allocate the buffer.
  if (NULL == _buffer)
  {
    _bufferSize = bufferSize;
    _buffer = static_cast<aafDataBuffer_t>(CoTaskMemAlloc(_bufferSize));
    if (NULL == _buffer)
      check(AAFRESULT_NOMEMORY);
  }
}

inline void EssenceDataTest::check(HRESULT hr)
{
  if (AAFRESULT_SUCCESS != (_hr = hr))
    throw hr;
}

void EssenceDataTest::createFile(wchar_t *pFileName)
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

  createFileMob();
  createFileMob();

  cleanupReferences();
}

void EssenceDataTest::openFile(wchar_t *pFileName)
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

void EssenceDataTest::createFileMob()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pSourceMob);
  assert(NULL == _pMob);
  assert(NULL == _pFileDescriptor);
  assert(NULL == _pEssenceDescriptor);
  assert(NULL == _pSourceMob);

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
  check(_pMob->SetName(L"EssenceDataTest File Mob"));
  
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

void EssenceDataTest::createEssenceData(IAAFSourceMob *pSourceMob)
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
  
  writeEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
  writeEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

  // Read back the data before saving and/or closing the file
  check(_pEssenceData->SetPosition(0));
  aafPosition_t dataPosition = 0;
  check(_pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != 0)
    check(AAFRESULT_TEST_FAILED);

  // Validate the current essence size.
  aafLength_t essenceSize = 0;
  aafLength_t expectedEssenceSize = sizeof(_smiley) + sizeof(_frowney);
  check(_pEssenceData->GetSize(&essenceSize));
  if (essenceSize != expectedEssenceSize)
    check(AAFRESULT_TEST_FAILED);
  

  readEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
  readEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

  _pEssenceData->Release();
  _pEssenceData = NULL;
}

void EssenceDataTest::openEssenceData()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumEssenceData);
  assert(NULL == _pEssenceData);
  assert(NULL == _pSourceMob);

  
  check(_pHeader->EnumEssenceData(&_pEnumEssenceData));
  while (AAFRESULT_SUCCESS == (_hr = _pEnumEssenceData->NextOne(&_pEssenceData)))
  {
    // Validate the essence.
    // Validate the current essence size.
    aafLength_t essenceSize = 0;
    aafLength_t expectedEssenceSize = sizeof(_smiley) + sizeof(_frowney);
    check(_pEssenceData->GetSize(&essenceSize));
    if (essenceSize != expectedEssenceSize)
      check(AAFRESULT_TEST_FAILED);

    // Validate that the correct data can be read back.
    readEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
    readEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

    // Make sure that the essence data still references
    // a valid mob.
    check(_pEssenceData->GetFileMob(&_pSourceMob));

    _pSourceMob->Release();
    _pSourceMob = NULL;

    _pEssenceData->Release();
    _pEssenceData = NULL;
  }
  // It is ok to run out of objects.
  if (AAFRESULT_NO_MORE_OBJECTS == _hr)
    _hr = AAFRESULT_SUCCESS;
  else
    check(_hr);  

  _pEnumEssenceData->Release();
  _pEnumEssenceData = NULL;
}

void EssenceDataTest::writeEssenceData(IAAFEssenceData *pEssenceData,
				       const aafDataBuffer_t data,
				       aafUInt32 dataSize)
{
  assert(NULL != pEssenceData);

  // NOTE: The following code assumes that the compiler supports 64 bit
  // numbers.

  aafPosition_t startingPosition = 0;
  aafPosition_t expectedPosition = 0;
  aafPosition_t dataPosition = 0;
  aafUInt32 bytesWritten = 0;


  // Save the starting position.
  check(pEssenceData->GetPosition(&startingPosition));

  bytesWritten = 0;
  check(pEssenceData->Write(dataSize, data, &bytesWritten));
  // Make sure that we wrote the expected number of bytes.
  if (bytesWritten != dataSize)
    check(AAFRESULT_TEST_FAILED);

  // Make sure the position is supported correctly.
  expectedPosition = startingPosition + dataSize;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);
}

void EssenceDataTest::readEssenceData(IAAFEssenceData *pEssenceData,
				      const aafDataBuffer_t data,
				      aafUInt32 dataSize)
{
  assert(NULL != pEssenceData);

  // NOTE: The following code assumes that the compiler supports 64 bit
  // numbers.

  aafPosition_t startingPosition = 0;
  aafPosition_t expectedPosition = 0;
  aafPosition_t dataPosition = 0;
  aafUInt32 bytesRead = 0;

  

  // Save the starting position.
  check(pEssenceData->GetPosition(&startingPosition));

  // Make sure the buffer is large enough to hold the data.
  setBufferSize(dataSize);

  // Read dataSize bytes from the essence data.
  bytesRead = 0;
  check(pEssenceData->Read(dataSize, _buffer, &bytesRead));
  // Make sure that we read the expected number of bytes.
  if (bytesRead != dataSize)
    check(AAFRESULT_TEST_FAILED);

  // Make sure the position is supported correctly.
  expectedPosition = startingPosition + dataSize;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);


  // Compare the bytes we read to the given input data.
  // (this is assumed to match the original data written
  // to the essence data)
  if (0 != memcmp(data, (char *)_buffer, dataSize))
    check(AAFRESULT_TEST_FAILED);
}
