// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceData
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
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobIDs[2] =
{// begin mobid's ...
	//first id
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x79a699c8, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}},

	//second if
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xe7824a42, 0x040c, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}

}; // ...end mobid's


// Utility class to implement the test.
struct EssenceDataTest
{
  EssenceDataTest();
  ~EssenceDataTest();

  void createFile(wchar_t *pFileName);
  void openFile(wchar_t *pFileName);

  void createFileMob(unsigned int mobid_Index);
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
  void checkExpression(bool expression, HRESULT hr);
  const char * convert(const wchar_t* pwcName);

  // Shared member data:
  HRESULT _hr;
  aafProductVersion_t _prodecutVersion;
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
  IAAFEssenceData *_pEssenceData;
  aafDataBuffer_t _buffer;
  aafUInt32 _bufferSize;

  static const char _smiley[];
  static const char _frowney[];
};

extern "C" HRESULT CAAFEssenceData_test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  wchar_t fileName[] = L"AAFEssenceDataTest.aaf";
  EssenceDataTest edt;

  try
  {
    edt.createFile(fileName);
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
  _prodecutVersion.major = 1;
  _prodecutVersion.minor = 0;
  _prodecutVersion.tertiary = 0;
  _prodecutVersion.patchLevel = 0;
  _prodecutVersion.type = kAAFVersionUnknown;
  _productInfo.companyName = L"AAF Developers Desk";
  _productInfo.productName = L"AAFEssenceData Test";
  _productInfo.productVersion = &_prodecutVersion;
  _productInfo.productVersionString = NULL;
  _productInfo.productID = UnitTestProductID;
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
    CoTaskMemFree(_buffer);
    _buffer = NULL;
  }

  // Allocate the buffer.
  if (NULL == _buffer)
  {
    _bufferSize = bufferSize;
    _buffer = static_cast<aafDataBuffer_t>(CoTaskMemAlloc(_bufferSize));
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



void EssenceDataTest::createFile(wchar_t *pFileName)
{
  // Delete the test file if it already exists.
  remove(convert(pFileName));

  check(AAFFileOpenNewModify(pFileName, 0, &_productInfo, &_pFile));
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
  
  check(defs.cdFileDescriptor()->
		CreateInstance(IID_IAAFEssenceDescriptor, 
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

void EssenceDataTest::createEssenceData(IAAFSourceMob *pSourceMob)
{
  assert(_pFile && _pHeader && _pDictionary);
  assert(pSourceMob);
  assert(NULL == _pEssenceData);
  
  CAAFBuiltinDefs defs (_pDictionary);

  // Attempt to create an AAFEssenceData.
  check(defs.cdEssenceData()->
		CreateInstance(IID_IAAFEssenceData,
					   (IUnknown **)&_pEssenceData));

  check(_pEssenceData->SetFileMob(pSourceMob));
  check(_pHeader->AddEssenceData(_pEssenceData));
  
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
    checkExpression(essenceSize == expectedEssenceSize, AAFRESULT_TEST_FAILED);

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
  checkExpression (AAFRESULT_NO_MORE_OBJECTS == _hr, _hr);
  _hr = AAFRESULT_SUCCESS;

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
