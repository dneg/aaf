// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefStream
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=






#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFPropertyDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IUnknown>                  IUnknownSP;
typedef IAAFSmartPointer<IAAFFile>                  IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>                IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFDictionary>            IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFObject>                IAAFObjectSP;
typedef IAAFSmartPointer<IAAFClassDef>              IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>           IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>         IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefStream>         IAAFTypeDefStreamSP;
typedef IAAFSmartPointer<IAAFSourceMob>             IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFEssenceDescriptor>     IAAFEssenceDescriptorSP;
typedef IAAFSmartPointer<IAAFEssenceData>           IAAFEssenceDataSP;
typedef IAAFSmartPointer<IEnumAAFEssenceData>       IEnumAAFEssenceDataSP;



#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const aafMobID_t sMobID[] = {
  //{060c2b340205110101001000-13-00-00-00-{78de46cd-4622-11d4-8029-00104bc9156d}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
  0x13, 0x00, 0x00, 0x00,
  {0x78de46cd, 0x4622, 0x11d4, 0x80, 0x29, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}},

  //{060c2b340205110101001000-13-00-00-00-{78de46ce-4622-11d4-8029-00104bc9156d}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
  0x13, 0x00, 0x00, 0x00, 
   {0x78de46ce, 0x4622, 0x11d4, 0x80, 0x29, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}}
};

static aafCharacter_constptr sMobName[] = 
{
  L"TypeDefStreamTest File Mob - 1",
  L"TypeDefStreamTest File Mob - 2"
};

static const char sSmiley[] =        /* 16x16 smiley face */
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

static const char sFrowney[] =        /* 16x16 frowney face */
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


static const aafUInt16 sTestUInt16[] =
{
  0x0123, 0x4567, 0x89AB, 0xCDEF
};


static const aafCharacter sTestCharacter[] = L"0123456789ABCDEF";
static const aafCharacter sTruncatedCharacters[] = L"Truncated Characters";

// Required function prototypes
extern "C"
{
  // Main test function.
  HRESULT CAAFTypeDefStream_test(testMode_t mode);

  // Create the test file.
  void CAAFTypeDefStream_create (aafCharacter_constptr pFileName); // throw HRESULT

  // Open the test file read only and validate the data.
  void CAAFTypeDefStream_read (aafCharacter_constptr pFileName); // throw HRESULT
}


HRESULT CAAFTypeDefStream_test(testMode_t mode)
{
  HRESULT result = AAFRESULT_SUCCESS;
  aafCharacter_constptr wFileName = L"AAFTypeDefStreamTest.aaf";
  //  const char *aFileName = "AAFTypeDefStreamTest.aaf";

  try
  {
    // Run through a basic set of tests. Create the file, 
    // and then read and validate the new file.
     if(mode == kAAFUnitTestReadWrite)
    	CAAFTypeDefStream_create (wFileName);
    CAAFTypeDefStream_read (wFileName);
  }
  catch (HRESULT &rhr)
  {
    result = rhr;
  }

  return result;
}

#ifndef _DEBUG
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

#else // #ifndef _DEBUG

// convenient error handlers.
#define checkResult(r)\
do {\
  if (FAILED(r))\
  {\
    cerr << "FILE:" << __FILE__ << " LINE:" << __LINE__ << " Error code = " << hex << r << dec << endl;\
    throw (HRESULT)r;\
  }\
} while (false)

#define checkExpression(expression, r)\
do {\
  if (!(expression))\
  {\
    cerr << "FILE:" << __FILE__ << " LINE:" << __LINE__ << " Expression failed = " << #expression << endl;\
    throw (HRESULT)r;\
  }\
} while (false)

#endif // #else // #ifndef _DEBUG



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

// Get the stream property values from the given essence data.
static void Test_LookupEssenceDataStreamPropertyDefinitions(
  IAAFDictionary *pDictionary,
  IAAFPropertyDef **ppDataPropertyDef,
  IAAFPropertyDef **ppSampleIndexPropertyDef)
{
 // Find the class definition for components class
  IAAFClassDefSP pEssenceDataClass;
  checkResult (pDictionary->LookupClassDef (AUID_AAFEssenceData, 
                                            &pEssenceDataClass));

  // Find our property definition
  checkResult (pEssenceDataClass->LookupPropertyDef (
                                      kAAFPropID_EssenceData_Data,
                                      ppDataPropertyDef));
  checkResult (pEssenceDataClass->LookupPropertyDef (
                                      kAAFPropID_EssenceData_SampleIndex,
                                      ppSampleIndexPropertyDef));
}


// Get the stream property values from the given essence data.
static void Test_EssenceStreamPropertyValues(
  IAAFDictionary *pDictionary,
  IAAFEssenceData *pEssenceData,
  IAAFPropertyValue **ppDataPropertyValue,
  IAAFPropertyValue **ppSampleIndexPropertyValue)
{
  // Get the direct access interfaces.
  IAAFObjectSP pObject;
  IAAFPropertyDefSP pDataPropertyDef;
  IAAFPropertyDefSP pSampleIndexPropertyDef;  

  checkResult(pEssenceData->QueryInterface(IID_IAAFObject, (void **)&pObject));
  Test_LookupEssenceDataStreamPropertyDefinitions(pDictionary,
                                             &pDataPropertyDef,
                                             &pSampleIndexPropertyDef);

  checkResult(pObject->GetPropertyValue(pDataPropertyDef,
                                        ppDataPropertyValue));
  aafBoolean_t present = kAAFFalse;
  checkResult(pObject->IsPropertyPresent(pSampleIndexPropertyDef, &present));
  if (kAAFTrue == present)
  {
    checkResult(pObject->GetPropertyValue(pSampleIndexPropertyDef,
                                          ppSampleIndexPropertyValue));
  }
  else
  {
    *ppSampleIndexPropertyValue = NULL;
  }
}

static void Test_GetTypeDefStream(
  IAAFPropertyValue *pStreamPropertyValue,
  IAAFTypeDefStream **ppTypeDefStream)
{
  IAAFTypeDefSP pTypeDef;

  checkResult(pStreamPropertyValue->GetType(&pTypeDef));
  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefStream,
                                       (void **)ppTypeDefStream));
}

static void Test_EssenceStreamWrite(
  CAAFBuiltinDefs & defs,
  IAAFPropertyValue *pStreamPropertyValue)
{
  IAAFTypeDefStreamSP pTypeDefStream;
  Test_GetTypeDefStream(pStreamPropertyValue, &pTypeDefStream);

  // Check the byte order of the stream.
  aafBoolean_t hasByteOrder;
  eAAFByteOrder_t byteOrder;
  checkResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  // The byte order has never...
  checkExpression(kAAFFalse == hasByteOrder, AAFRESULT_TEST_FAILED);
  // Set the byte order of the stream to little endian...
  checkResult(pTypeDefStream->SetStoredByteOrder(pStreamPropertyValue, kAAFByteOrderLittle));
  checkResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  checkExpression(kAAFTrue == hasByteOrder, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetStoredByteOrder(pStreamPropertyValue, &byteOrder));
  checkExpression(kAAFByteOrderLittle == byteOrder, AAFRESULT_TEST_FAILED);
  // Change the byte order.
  checkResult(pTypeDefStream->ClearStoredByteOrder(pStreamPropertyValue));
  checkResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  checkExpression(kAAFFalse == hasByteOrder, AAFRESULT_TEST_FAILED);
  // Set the byte order of the stream to big endian...
  checkResult(pTypeDefStream->SetStoredByteOrder(pStreamPropertyValue, kAAFByteOrderBig));
  checkResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  checkExpression(kAAFTrue == hasByteOrder, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetStoredByteOrder(pStreamPropertyValue, &byteOrder));
  checkExpression(kAAFByteOrderBig == byteOrder, AAFRESULT_TEST_FAILED);

  // Get the current position
  aafInt64 streamSize, expectedSize, streamPosition, expectedPosition;

  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(0 == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(0 == streamPosition, AAFRESULT_TEST_FAILED);

  // The postioning code has the following assumption:
  assert(sizeof(sSmiley) == sizeof(sFrowney));

  // Write an array of bytes.
  checkResult(pTypeDefStream->Write(pStreamPropertyValue,
                                    sizeof(sSmiley),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sSmiley))));

  // Checking for non-zero size and position must be checked 
  // carefully since the persistent size for structures and 
  // characters may be smaller then the in memory size. This
  // is why this test only uses types that have a known
  // persistent size: aafUInt8, aafUInt16, aafCharacter.
  expectedSize = sizeof(sSmiley);
  expectedPosition = sizeof(sSmiley);
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);

  // Test append
  // Restore the position to the begining of the stream.
  checkResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));

  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(0 == streamPosition, AAFRESULT_TEST_FAILED);


  // Append an array of bytes.
  checkResult(pTypeDefStream->Append(pStreamPropertyValue,
                                     sizeof(sFrowney),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sFrowney))));

  expectedSize += sizeof(sFrowney);
  expectedPosition += sizeof(sFrowney);
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);

  // Test non-zero repositioning and overriting...
  expectedPosition -= sizeof(sFrowney);
  checkResult(pTypeDefStream->SetPosition(pStreamPropertyValue, expectedPosition));
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);
  // Write an array of bytes.
  checkResult(pTypeDefStream->Write(pStreamPropertyValue,
                                    sizeof(sSmiley),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sSmiley))));
 
  expectedPosition += sizeof(sSmiley);
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);


  // Write an array of shorts
  checkResult(pTypeDefStream->WriteElements(pStreamPropertyValue,
                                            defs.tdUInt16(),
                                            sizeof(sTestUInt16),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafUInt16 *>(sTestUInt16))));

  expectedSize += sizeof(sTestUInt16);
  expectedPosition += sizeof(sTestUInt16);
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);



  // Restore the position to the begining of the stream.
  checkResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));

  
  // Append an array of characters
  checkResult(pTypeDefStream->AppendElements(pStreamPropertyValue,
                                             defs.tdCharacter(),
                                            sizeof(sTestCharacter),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafCharacter *>(sTestCharacter))));
  

  // The expected size and position must be incremented according to the
  // *persisted* size of aafCharacter, which is always 2, whereas the 
  // in-memory size may be 2 or 4, depending on the platform.
  expectedSize += (sizeof(sTestCharacter)/sizeof(wchar_t))*2; // sizeof(sTestCharacter);
  expectedPosition += (sizeof(sTestCharacter)/sizeof(wchar_t))*2; // sizeof(sTestCharacter);
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);
  
  // Test truncating the stream.
  checkResult(pTypeDefStream->AppendElements(pStreamPropertyValue,
                                             defs.tdCharacter(),
                                            sizeof(sTruncatedCharacters),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafCharacter *>(sTruncatedCharacters))));
  checkResult(pTypeDefStream->SetSize(pStreamPropertyValue, expectedSize));
  
  checkResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  checkExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  checkResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  checkExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);
  

  // Restore the position to the begining of the stream.
  checkResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));
}

static void Test_EssenceStreamRead(
  CAAFBuiltinDefs & defs,
  IAAFPropertyValue *pStreamPropertyValue)
{
  IAAFTypeDefStreamSP pTypeDefStream;
  aafUInt32 bytesRead;

  Test_GetTypeDefStream(pStreamPropertyValue, &pTypeDefStream);

  // Read an array of bytes.
  char smileyTest[sizeof(sSmiley)];
  bytesRead = 0;
  checkResult(pTypeDefStream->Read(pStreamPropertyValue,
                                    sizeof(sSmiley),
                                    reinterpret_cast<aafMemPtr_t>(smileyTest),
                                    &bytesRead));
  checkExpression(bytesRead == sizeof(sSmiley), AAFRESULT_TEST_FAILED);
  checkExpression(0 == memcmp(smileyTest, sSmiley, bytesRead),
                  AAFRESULT_TEST_FAILED);

  bytesRead = 0;
  checkResult(pTypeDefStream->Read(pStreamPropertyValue,
                                    sizeof(sSmiley),
                                    reinterpret_cast<aafMemPtr_t>(smileyTest),
                                    &bytesRead));
  checkExpression(bytesRead == sizeof(sSmiley), AAFRESULT_TEST_FAILED);
  checkExpression(0 == memcmp(smileyTest, sSmiley, bytesRead),
                  AAFRESULT_TEST_FAILED);
  

  // Read the "appended" array of bytes.
  //  char frowneyTest[sizeof(sFrowney)];
  //bytesRead = 0;
  //checkResult(pTypeDefStream->Read(pStreamPropertyValue,
  //                                  sizeof(sFrowney),
  //                                  reinterpret_cast<aafMemPtr_t>(frowneyTest),
  //                                  &bytesRead));
  //checkExpression(bytesRead == sizeof(sFrowney), AAFRESULT_TEST_FAILED);
  //checkExpression(0 == memcmp(frowneyTest, sFrowney, bytesRead),
  //                AAFRESULT_TEST_FAILED);
   
  
  // Read an array of aafUInt16
  aafUInt16 uint16Test[sizeof(sTestUInt16)/sizeof(aafUInt16)];
  bytesRead = 0;
  checkResult(pTypeDefStream->ReadElements(pStreamPropertyValue,
                                           defs.tdUInt16(),
                                           sizeof(sTestUInt16),
                  reinterpret_cast<aafMemPtr_t>(uint16Test),
                                    &bytesRead));
  checkExpression(bytesRead == sizeof(sTestUInt16), AAFRESULT_TEST_FAILED);
  checkExpression(0 == memcmp(uint16Test, sTestUInt16, bytesRead),
                  AAFRESULT_TEST_FAILED);

  // Read the "appended" array of aafCharacter(s)
  aafCharacter characterTest[sizeof(sTestCharacter)/sizeof(aafCharacter)];
  bytesRead = 0;
  checkResult(pTypeDefStream->ReadElements(pStreamPropertyValue,
                                           defs.tdCharacter(),
                                           sizeof(sTestCharacter),
                  reinterpret_cast<aafMemPtr_t>(characterTest),
                                    &bytesRead));
  checkExpression(bytesRead == sizeof(sTestCharacter), AAFRESULT_TEST_FAILED);
  checkExpression(0 == memcmp(characterTest, sTestCharacter, bytesRead),
                  AAFRESULT_TEST_FAILED);
}

// Create the test file.
void CAAFTypeDefStream_create (aafCharacter_constptr pFileName) // throw HRESULT
{
  aafProductIdentification_t	ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFSourceMobSP pSourceMob;
  IAAFMobSP pMob;
  IAAFEssenceDescriptorSP pEssenceDesciptor;
  IAAFEssenceDataSP pEssenceData;
  IAAFPropertyValueSP pDataPropertyValue;
  IAAFPropertyValueSP pSampleIndexPropertyValue;


  // Remove the previous test file is one exists
  RemoveTestFile (pFileName);

  // Create the file.
  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDefStream Test - create";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));
  
  CAAFBuiltinDefs defs (pDictionary);
 
  // Create a Mob
  checkResult(defs.cdSourceMob()->CreateInstance(IID_IAAFSourceMob,
                                                 (IUnknown **)&pSourceMob));

  checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
  checkResult(pMob->SetMobID(sMobID[0]));
  checkResult(pMob->SetName(sMobName[0]));

  // Create a concrete subclass of EssenceDescriptor
  checkResult(defs.cdAIFCDescriptor()->CreateInstance(IID_IAAFEssenceDescriptor, 
                                                      (IUnknown **)&pEssenceDesciptor));

	IAAFAIFCDescriptor*			pAIFCDesc = NULL;
	checkResult(pEssenceDesciptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
	checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
	pAIFCDesc->Release();
	pAIFCDesc = NULL;

  checkResult(pSourceMob->SetEssenceDescriptor (pEssenceDesciptor));
  checkResult(pHeader->AddMob(pMob));


  // Attempt to create an AAFEssenceData.
  checkResult(defs.cdEssenceData()->CreateInstance(IID_IAAFEssenceData,
                                             (IUnknown **)&pEssenceData));

  checkResult(pEssenceData->SetFileMob(pSourceMob));
  checkResult(pHeader->AddEssenceData(pEssenceData));

  Test_EssenceStreamPropertyValues(pDictionary,
                                   pEssenceData,
                                   &pDataPropertyValue,
                                   &pSampleIndexPropertyValue);
  
  // Test the required property
  Test_EssenceStreamWrite(defs, pDataPropertyValue);
  Test_EssenceStreamRead(defs, pDataPropertyValue);
  
  if (pSampleIndexPropertyValue)
  {
    // Test the optional property
    Test_EssenceStreamWrite(defs, pSampleIndexPropertyValue);
    Test_EssenceStreamRead(defs, pSampleIndexPropertyValue);
  }

  checkResult(pFile->Save());
  checkResult(pFile->Close());
}



// Create the test file.
void CAAFTypeDefStream_read (aafCharacter_constptr pFileName) // throw HRESULT
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFEssenceDataSP pEssenceData;
  IAAFPropertyValueSP pDataPropertyValue;
  IAAFPropertyValueSP pSampleIndexPropertyValue;

  checkResult (AAFFileOpenExistingRead(pFileName, 0, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));

  CAAFBuiltinDefs defs (pDictionary);
  
#if 0
  // Missing api method!
  checkResult (pHeader->LookupEssenceData(sMobID[0], &pEssenceData));
#else
  // For this version of the test assume that there is only a single
  // essence data...
  aafUInt32 count;
  checkResult(pHeader->CountEssenceData(&count));
  checkExpression(1 == count, AAFRESULT_TEST_FAILED);
  IEnumAAFEssenceDataSP pEnumEssenceData;
  checkResult(pHeader->EnumEssenceData(&pEnumEssenceData));
  checkResult(pEnumEssenceData->NextOne(&pEssenceData));
  aafMobID_t testMobID = {0};
  checkResult(pEssenceData->GetFileMobID(&testMobID));
  checkExpression(0 == memcmp(&testMobID, &sMobID[0], sizeof(aafMobID_t)),
                  AAFRESULT_TEST_FAILED);  
#endif


  Test_EssenceStreamPropertyValues(pDictionary,
                                   pEssenceData,
                                   &pDataPropertyValue,
                                   &pSampleIndexPropertyValue);
  
  // Test the required property
  Test_EssenceStreamRead(defs, pDataPropertyValue);
  
  if(pSampleIndexPropertyValue)
  {
    // Test the optional property
    Test_EssenceStreamRead(defs, pSampleIndexPropertyValue);
  }  

  checkResult(pFile->Close());
}























