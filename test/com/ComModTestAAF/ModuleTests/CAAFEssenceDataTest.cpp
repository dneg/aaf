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

  void writeEssenceData(IAAFEssenceData *pEssenceData);
  void readEssenceData(IAAFEssenceData *pEssenceData);

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
  aafDataBuffer_t _buffer;

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

#ifndef TIM_TEST
		// todo:
		// om support for reading immediately after writing (access violation)
		// om support for position and size for unopened/created streams (assertion)

		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
#endif
  }
  catch (HRESULT& ehr)
  {
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
  "      ****      ";

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
  "      ****      ";



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
  _buffer(NULL)
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
  
  writeEssenceData(_pEssenceData);

  // Read back the data before saving and/or closing the file
#ifdef TIM_TEST
  check(_pEssenceData->SetPosition(0));
  aafPosition_t dataPosition = 0;
  check(_pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != 0)
    check(AAFRESULT_TEST_FAILED);
  readEssenceData(_pEssenceData);
#endif
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
		readEssenceData(_pEssenceData);

		// Make sure that the essence data still references
		// a valid mob.
		check(_pEssenceData->GetFileMob(&_pSourceMob));

		_pSourceMob->Release();
		_pSourceMob = NULL;

		_pEssenceData->Release();
		_pEssenceData = NULL;
	}

	if (AAFRESULT_NO_MORE_OBJECTS == _hr)
		_hr = AAFRESULT_SUCCESS;
	check(_hr);	

	_pEnumEssenceData->Release();
	_pEnumEssenceData = NULL;
}

void EssenceDataTest::writeEssenceData(IAAFEssenceData *pEssenceData)
{
  assert(NULL != pEssenceData);

  // NOTE: The following code assumes that the compiler supports 64 bit
  // numbers.

  aafPosition_t startingPosition = 0;
  aafPosition_t expectedPosition = 0;
  aafPosition_t dataPosition = 0;
  aafUInt32 bytesWritten = 0;
  aafUInt32 bytes = strlen(_smiley) + 1; 

  // Save the starting position.
#ifdef TIM_TEST
  check(pEssenceData->GetPosition(&startingPosition));
#endif
  // Write the "smiley face" into the mediaData propert
  bytesWritten = 0;
  check(pEssenceData->Write(bytes, (aafDataBuffer_t)_smiley, &bytesWritten));
  // Make sure that we wrote the expected number of bytes.
  if (bytesWritten != bytes)
    check(AAFRESULT_TEST_FAILED);


  // Make sure the position is supported correctly.
#ifdef TIM_TEST
  expectedPosition = startingPosition + bytes;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);
#endif
  
  // Test sequencial writes...
  //
  // Write the "frowney face" into the mediaData propert
  bytes = strlen(_frowney)+ 1;
  bytesWritten = 0;
  check(pEssenceData->Write(bytes, (aafDataBuffer_t)_frowney, &bytesWritten));
  // Make sure that we wrote the expected number of bytes.
  if (bytesWritten != bytes)
    check(AAFRESULT_TEST_FAILED);

  // Make sure the position is supported correctly.
#ifdef TIM_TEST
  expectedPosition += bytes;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);
#endif
}

void EssenceDataTest::readEssenceData(IAAFEssenceData *pEssenceData)
{
  assert(NULL != pEssenceData);

  // NOTE: The following code assumes that the compiler supports 64 bit
  // numbers.

  aafPosition_t startingPosition = 0;
  aafPosition_t expectedPosition = 0;
  aafPosition_t dataPosition = 0;
  aafUInt32 bytesRead = 0;
  aafUInt32 bytes = strlen(_smiley) + 1; 

  // Save the starting position.
#ifdef TIM_TEST
  check(pEssenceData->GetPosition(&startingPosition));
#endif
  // Allocate the buffer.
  if (NULL == _buffer)
  {
    _buffer = static_cast<aafDataBuffer_t>(CoTaskMemAlloc(bytes));
    if (NULL == _buffer)
      check(AAFRESULT_NOMEMORY);
  }

  // Validate that "smiley face" was written.
  bytesRead = 0;
  check(pEssenceData->Read(bytes, _buffer, &bytesRead));
  // Make sure that we read the expected number of bytes.
  if (bytesRead != bytes)
    check(AAFRESULT_TEST_FAILED);

  // Make sure the position is supported correctly.
#ifdef TIM_TEST
  expectedPosition = startingPosition + bytes;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);
#endif
  // Compare the bytes
  if (0 != strcmp(_smiley, (char *)_buffer))
    check(AAFRESULT_TEST_FAILED);


  // Validate that "frowney face" was written.
  bytesRead = 0;
  check(pEssenceData->Read(bytes, _buffer, &bytesRead));
  // Make sure that we read the expected number of bytes.
  if (bytesRead != bytes)
    check(AAFRESULT_TEST_FAILED);

  // Make sure the position is supported correctly.
#ifdef TIM_TEST
  expectedPosition += bytes;
  dataPosition = 0;
  check(pEssenceData->GetPosition(&dataPosition));
  if (dataPosition != expectedPosition)
    check(AAFRESULT_TEST_FAILED);
#endif
  // Compare the bytes
  if (0 != strcmp(_frowney, (char *)_buffer))
    check(AAFRESULT_TEST_FAILED);
}
