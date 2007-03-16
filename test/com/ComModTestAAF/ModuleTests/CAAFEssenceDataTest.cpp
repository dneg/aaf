// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceData
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAF.h"

#include <iostream>
using namespace std;
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobIDs[2] =
{// begin mobid's ...
	//first id
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x79a699c8, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}},

	//second if
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xe7824a42, 0x040c, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}}

}; // ...end mobid's


// Utility class to implement the test.
struct EssenceDataTest
{
  EssenceDataTest(aafProductIdentification_constref productID);
  ~EssenceDataTest();

  void createFile(wchar_t *pFileName,
                  aafUID_constref fileKind,
                  testRawStorageType_t rawStorageType);
  void openFile(wchar_t *pFileName);

  void createFileMob(unsigned int mobid_Index);
  void createEssenceData(IAAFSourceMob *pSourceMob);
  void openEssenceData();

  void writeEssenceData(IAAFPlainEssenceData *pEssenceData,
		        const aafDataBuffer_t data,
			aafUInt32 dataSize);
  void readEssenceData(IAAFPlainEssenceData *pEssenceData,
		        const aafDataBuffer_t data,
			aafUInt32 dataSize);

  void cleanupReferences();
  void setBufferSize(aafUInt32 bufferSize);
  inline void check(HRESULT hr);
  inline void checkExpression(bool expression, HRESULT hr);
  const char * convert(const wchar_t* pwcName);

  // Shared member data:
  HRESULT _hr;
  aafProductIdentification_t _productInfo;
  IAAFFile *_pFile;
  bool _bFileOpen;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;

  IAAFMob *_pMob;
  IAAFSourceMob *_pSourceMob;
  IAAFEssenceDescriptor *_pEssenceDescriptor;
  IAAFFileDescriptor *_pFileDescriptor;
  IEnumAAFEssenceData *_pEnumEssenceData;
  IAAFPlainEssenceData *_pEssenceData;
  aafDataBuffer_t _buffer;
  aafUInt32 _bufferSize;

  static const char _smiley[];
  static const char _frowney[];

  static const aafUID_t _essenceElementKey;
};

extern "C" HRESULT CAAFEssenceData_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFEssenceData_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  const size_t fileNameBufLen = 128;
  aafWChar fileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, fileName );
  EssenceDataTest edt(productID);

  try
  {
	if(mode == kAAFUnitTestReadWrite)
    	edt.createFile(fileName, fileKind, rawStorageType);
    edt.openFile(fileName);
  }
  catch (HRESULT& ehr)
  {
    // thrown by EssenceDataTest_check() method.
    hr = ehr;
  }
  catch (...)
  {
    cerr << "CAAFEssenceData_test...Caught general C++"
		 << " exception!" << endl; 
	hr = AAFRESULT_TEST_FAILED;
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

/*static*/ const aafUID_t EssenceDataTest::_essenceElementKey =
{0x0d010301, 0xdddd, 0xdddd, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01}};



EssenceDataTest::EssenceDataTest(aafProductIdentification_constref productID):
  _hr(AAFRESULT_SUCCESS),
  _productInfo(productID),
  _pFile(NULL),
  _bFileOpen(false),
  _pHeader(NULL),
  _pDictionary(NULL),
  _pMob(NULL),
  _pSourceMob(NULL),
  _pEssenceDescriptor(NULL),
  _pFileDescriptor(NULL),
  _pEnumEssenceData(NULL),
  _pEssenceData(NULL),
  _buffer(NULL),
  _bufferSize(0)
{
}

EssenceDataTest::~EssenceDataTest()
{
  cleanupReferences();
}

void EssenceDataTest::cleanupReferences()
{

  if (NULL != _buffer)
  {
    delete [] _buffer;
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

void EssenceDataTest::setBufferSize(aafUInt32 bufferSize)
{
  // Allocate the buffer.
  if (NULL != _buffer && bufferSize > _bufferSize)
  {
    delete [] _buffer;
    _buffer = NULL;
  }

  // Allocate the buffer.
  if (NULL == _buffer)
  {
    _bufferSize = bufferSize;
    _buffer = (aafDataBuffer_t)new char[ _bufferSize ];
    checkExpression(NULL != _buffer, AAFRESULT_NOMEMORY);
  }
}

inline void EssenceDataTest::check(HRESULT hr)
{
  if (AAFRESULT_SUCCESS != (_hr = hr))
    throw hr;
}

inline void EssenceDataTest::checkExpression(bool expression, HRESULT hr)
{
  if (!expression)
    throw hr;
}


// local conversion utility.
const char * EssenceDataTest::convert(const wchar_t* pwcName)
{
  checkExpression(NULL != pwcName, E_INVALIDARG);
  const size_t kBufferSize = 256;
  static char ansiName[kBufferSize];

  size_t convertedBytes = wcstombs(ansiName, pwcName, kBufferSize);
  checkExpression(0 < convertedBytes, E_INVALIDARG);

  return ansiName;
}



void EssenceDataTest::createFile(
    wchar_t *pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType)
{
  // Delete the test file if it already exists.
  remove(convert(pFileName));

  check(CreateTestFile( pFileName, fileKind, rawStorageType, _productInfo, &_pFile ));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));
  check(_pHeader->GetDictionary(&_pDictionary));

  createFileMob(0); //use unique mobid's (without using cocreate guid()
  createFileMob(1); //use unique mobid's (without using cocreate guid()

  check(_pFile->Save());

  cleanupReferences();
}

void EssenceDataTest::openFile(wchar_t *pFileName)
{
  check(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));

  openEssenceData();

  cleanupReferences();
}

void EssenceDataTest::createFileMob(unsigned int mobid_Index)
{
  assert(_pFile && _pHeader && _pDictionary);
  assert(NULL == _pSourceMob);
  assert(NULL == _pMob);
  assert(NULL == _pFileDescriptor);
  assert(NULL == _pEssenceDescriptor);
  assert(NULL == _pSourceMob);

  CAAFBuiltinDefs defs (_pDictionary);

  // Create a Mob
  check(defs.cdSourceMob()->CreateInstance(IID_IAAFSourceMob, 
										   (IUnknown **)&_pSourceMob));

  check(_pSourceMob->QueryInterface (IID_IAAFMob, (void **)&_pMob));
  
  check(_pMob->SetMobID(TEST_MobIDs[mobid_Index]));
  check(_pMob->SetName(L"EssenceDataTest File Mob"));
  
  // instantiate a concrete subclass of FileDescriptor
  check(defs.cdAIFCDescriptor()->
		CreateInstance(IID_IAAFFileDescriptor,
					   (IUnknown **)&_pFileDescriptor));

	IAAFAIFCDescriptor*			pAIFCDesc = NULL;
	check(_pFileDescriptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
	check(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
	pAIFCDesc->Release();
	pAIFCDesc = NULL;

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

void EssenceDataTest::createEssenceData(IAAFSourceMob *pSourceMob)
{
  assert(_pFile && _pHeader && _pDictionary);
  assert(pSourceMob);
  assert(NULL == _pEssenceData);
  
  CAAFBuiltinDefs defs (_pDictionary);

  // Attempt to create an AAFEssenceData.
  IAAFEssenceData* pRawEssenceData = NULL;
  check(defs.cdEssenceData()->
		CreateInstance(IID_IAAFEssenceData,
					   (IUnknown **)&pRawEssenceData));

  check(pRawEssenceData->SetFileMob(pSourceMob));
  check(_pHeader->AddEssenceData(pRawEssenceData));
  
  IAAFEssenceData2* pEssenceData2 = NULL;
  check(pRawEssenceData->QueryInterface(IID_IAAFEssenceData2, (void**)&pEssenceData2));
  check(pEssenceData2->GetPlainEssenceData(0, &_pEssenceData));

  // Set essence element key
  IAAFKLVEssenceDataParameters* pParameters = NULL;
  check(_pEssenceData->QueryInterface(IID_IAAFKLVEssenceDataParameters, (void **)&pParameters));
  AAFRESULT hr = pParameters->SetEssenceElementKey(_essenceElementKey);
  checkExpression(AAFRESULT_SUCCEEDED(hr) || hr == AAFRESULT_OPERATION_NOT_PERMITTED,
                  AAFRESULT_TEST_FAILED);
  aafUID_t key;
  hr = pParameters->GetEssenceElementKey(&key);
  checkExpression(AAFRESULT_SUCCEEDED(hr) || hr == AAFRESULT_OPERATION_NOT_PERMITTED,
                  AAFRESULT_TEST_FAILED);
  checkExpression(AAFRESULT_FAILED(hr) || key == _essenceElementKey,
                  AAFRESULT_TEST_FAILED);


  writeEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
  writeEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

  // Read back the data before saving and/or closing the file
  check(_pEssenceData->SetPosition(0));
  aafPosition_t dataPosition = 0;
  check(_pEssenceData->GetPosition(&dataPosition));
  checkExpression(dataPosition == 0, AAFRESULT_TEST_FAILED);

  // Validate the current essence size.
  aafLength_t essenceSize = 0;
  aafLength_t expectedEssenceSize = sizeof(_smiley) + sizeof(_frowney);
  check(_pEssenceData->GetSize(&essenceSize));
  checkExpression(essenceSize == expectedEssenceSize, AAFRESULT_TEST_FAILED);
  

  readEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
  readEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

  pParameters->Release();
  pParameters = NULL;

  pRawEssenceData->Release();
  pRawEssenceData = NULL;

  pEssenceData2->Release();
  pEssenceData2 = NULL;

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
  IAAFEssenceData* pNextEssenceData = NULL;
  while (AAFRESULT_SUCCESS == (_hr = _pEnumEssenceData->NextOne(&pNextEssenceData)))
  {
    IAAFEssenceData2* pEssenceData2 = NULL;
    check(pNextEssenceData->QueryInterface(IID_IAAFEssenceData2, (void**)&pEssenceData2));
    check(pEssenceData2->GetPlainEssenceData(0, &_pEssenceData));

    // Validate essence element key
    IAAFKLVEssenceDataParameters* pParameters = NULL;
    check(_pEssenceData->QueryInterface(IID_IAAFKLVEssenceDataParameters, (void **)&pParameters));
    aafUID_t key;
    AAFRESULT hr = pParameters->GetEssenceElementKey(&key);
    checkExpression(AAFRESULT_SUCCEEDED(hr) || hr == AAFRESULT_OPERATION_NOT_PERMITTED,
                    AAFRESULT_TEST_FAILED);
    checkExpression(AAFRESULT_FAILED(hr) || key == _essenceElementKey,
                    AAFRESULT_TEST_FAILED);

    // Validate the essence.
    // Validate the current essence size.
    aafLength_t essenceSize = 0;
    aafLength_t expectedEssenceSize = sizeof(_smiley) + sizeof(_frowney);
    check(_pEssenceData->GetSize(&essenceSize));
    checkExpression(essenceSize == expectedEssenceSize, AAFRESULT_TEST_FAILED);

    // Validate that the correct data can be read back.
    readEssenceData(_pEssenceData, (aafDataBuffer_t)_smiley, sizeof(_smiley));
    readEssenceData(_pEssenceData, (aafDataBuffer_t)_frowney, sizeof(_frowney));

    // Make sure that the essence data still references
    // a valid mob.
    check(_pEssenceData->GetFileMob(&_pSourceMob));

    _pSourceMob->Release();
    _pSourceMob = NULL;

    pParameters->Release();
    pParameters = NULL;

    _pEssenceData->Release();
    _pEssenceData = NULL;

    pEssenceData2->Release();
    pEssenceData2 = NULL;

    pNextEssenceData->Release();
    pNextEssenceData = NULL;
  }

  // It is ok to run out of objects.
  checkExpression (AAFRESULT_NO_MORE_OBJECTS == _hr, _hr);
  _hr = AAFRESULT_SUCCESS;

  _pEnumEssenceData->Release();
  _pEnumEssenceData = NULL;
}

void EssenceDataTest::writeEssenceData(IAAFPlainEssenceData *pEssenceData,
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
  aafUInt32 offset = 0;



  // Save the starting position.
  check(pEssenceData->GetPosition(&startingPosition));

  for (offset = 0; offset < dataSize; ++offset)
  {   
    // Reset the position to test SetPosition.
    dataPosition = startingPosition + offset; 
    check(pEssenceData->SetPosition(dataPosition));

    bytesWritten = 0;
    check(pEssenceData->Write(dataSize - offset, data + offset, &bytesWritten));
    // Make sure that we wrote the expected number of bytes.
    checkExpression(bytesWritten == (dataSize - offset), AAFRESULT_TEST_FAILED);

    // Make sure the position is supported correctly.
    expectedPosition = startingPosition + dataSize;
    dataPosition = 0;
    check(pEssenceData->GetPosition(&dataPosition));
    checkExpression(dataPosition == expectedPosition, AAFRESULT_TEST_FAILED);
  }
}

void EssenceDataTest::readEssenceData(IAAFPlainEssenceData *pEssenceData,
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
  aafUInt32 offset = 0;

  

  // Save the starting position.
  check(pEssenceData->GetPosition(&startingPosition));

  // Make sure the buffer is large enough to hold the data.
  setBufferSize(dataSize);

  for (offset = 0; offset < dataSize; ++offset)
  {   
    // Reset the position to test SetPosition.
    dataPosition = startingPosition + offset; 
    check(pEssenceData->SetPosition(dataPosition));

    // Read dataSize bytes from the essence data.
    bytesRead = 0;
    check(pEssenceData->Read(dataSize - offset, _buffer, &bytesRead));
    // Make sure that we read the expected number of bytes.
    checkExpression(bytesRead == (dataSize - offset), AAFRESULT_TEST_FAILED);

    // Make sure the position is supported correctly.
    expectedPosition = startingPosition + dataSize;
    dataPosition = 0;
    check(pEssenceData->GetPosition(&dataPosition));
    checkExpression(dataPosition == expectedPosition, AAFRESULT_TEST_FAILED);


    // Compare the bytes we read to the given input data.
    // (this is assumed to match the original data written
    // to the essence data)
    checkExpression (0 == memcmp(data + offset, (char *)_buffer, dataSize - offset), AAFRESULT_TEST_FAILED);
  }
}
