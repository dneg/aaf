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
#include "AAFResult.h"
#include "ModuleTestsCommon.h"
using namespace mtc;

#include "AAFFileKinds.h"
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
typedef IAAFSmartPointer<IAAFTypeDefStream3>        IAAFTypeDefStream3SP;
typedef IAAFSmartPointer<IAAFTypeDefStreamEx>       IAAFTypeDefStreamExSP;
typedef IAAFSmartPointer<IAAFSourceMob>             IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>        IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFEssenceDescriptor>     IAAFEssenceDescriptorSP;
typedef IAAFSmartPointer<IAAFEssenceData>           IAAFEssenceDataSP;
typedef IAAFSmartPointer<IEnumAAFEssenceData>       IEnumAAFEssenceDataSP;



#include <iostream>
#include <iomanip>
using namespace std;
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const aafMobID_t sMobID[] = {
  //{060c2b340205110101001000-13-00-00-00-{78de46cd-4622-11d4-8029-00104bc9156d}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
  0x13, 0x00, 0x00, 0x00,
  {0x78de46cd, 0x4622, 0x11d4, {0x80, 0x29, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}}},

  //{060c2b340205110101001000-13-00-00-00-{78de46ce-4622-11d4-8029-00104bc9156d}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
  0x13, 0x00, 0x00, 0x00, 
  {0x78de46ce, 0x4622, 0x11d4, {0x80, 0x29, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}}},

  //{060c2b340205110101001000-13-00-00-00-{521B0A41-77AC-4767-BFC8-34A13AC50671}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
  0x13, 0x00, 0x00, 0x00, 
  {0x521b0a41, 0x77ac, 0x4767, {0xbf, 0xc8, 0x34, 0xa1, 0x3a, 0xc5, 0x6, 0x71}}},

  //{060c2b340205110101001000-13-00-00-00-{F1EE3A2E-0A3A-4c2b-A422-7BECE6AE43FA}}
  {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
  0x13, 0x00, 0x00, 0x00, 
  {0xf1ee3a2e, 0x0a3a, 0x4c2b, {0xa4, 0x22, 0x7b, 0xec, 0xe6, 0xae, 0x43, 0xfa}}}
};

static aafCharacter_constptr sMobName[] = 
{
  L"TypeDefStreamTest File Mob - 1",
  L"TypeDefStreamTest File Mob - 2", 
  L"TypeDefStreamTest File Mob - 3",
  L"TypeDefStreamTest File Mob - 4"
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
  // Create the test file.
  void CAAFTypeDefStream_create (
    aafCharacter_constptr pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID);

  // Open the test file read only and validate the data.
  void CAAFTypeDefStream_read (aafCharacter_constptr pFileName); // throw HRESULT


extern "C" HRESULT CAAFTypeDefStream_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT result = AAFRESULT_SUCCESS;
  const size_t fileNameBufLen = 128;
  aafWChar wFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, wFileName );

  try
  {
    // Run through a basic set of tests. Create the file, 
    // and then read and validate the new file.
     if(mode == kAAFUnitTestReadWrite)
    	CAAFTypeDefStream_create (wFileName, fileKind, rawStorageType, productID);
    CAAFTypeDefStream_read (wFileName);
  }
  catch (HRESULT &rhr)
  {
    result = rhr;
  }

  return result;
}




// Create a SourceMob with the specified ID and name,
// then create an EssenceData object associating it
// with the new SourceMob.
static void Test_CreateEssenceData(
  CAAFBuiltinDefs& defs,
  IAAFHeader* pHeader,
  const aafMobID_t& mobID,
  aafCharacter_constptr mobName,
  IAAFEssenceData** pResult)
{
  IAAFSourceMobSP pSourceMob;
  IAAFMobSP pMob;
  IAAFEssenceDescriptorSP pEssenceDesciptor;
  IAAFEssenceDataSP pEssenceData;

  // Create a Mob
  CheckResult(defs.cdSourceMob()->CreateInstance(IID_IAAFSourceMob,
                                                 (IUnknown **)&pSourceMob));

  CheckResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
  CheckResult(pMob->SetMobID(mobID));
  CheckResult(pMob->SetName(mobName));

  // Create a concrete subclass of EssenceDescriptor
  CheckResult(defs.cdAIFCDescriptor()->CreateInstance(IID_IAAFEssenceDescriptor, 
                                                      (IUnknown **)&pEssenceDesciptor));

	IAAFAIFCDescriptor*			pAIFCDesc = NULL;
	CheckResult(pEssenceDesciptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
	CheckResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
	pAIFCDesc->Release();
	pAIFCDesc = NULL;

  CheckResult(pSourceMob->SetEssenceDescriptor (pEssenceDesciptor));
  CheckResult(pHeader->AddMob(pMob));


  // Attempt to create an AAFEssenceData.
  CheckResult(defs.cdEssenceData()->CreateInstance(IID_IAAFEssenceData,
                                             (IUnknown **)&pEssenceData));

  CheckResult(pEssenceData->SetFileMob(pSourceMob));
  CheckResult(pHeader->AddEssenceData(pEssenceData));

  *pResult = pEssenceData;
  (*pResult)->AddRef();
}

// Get the stream property values from the given essence data.
static void Test_LookupEssenceDataStreamPropertyDefinitions(
  IAAFDictionary *pDictionary,
  IAAFPropertyDef **ppDataPropertyDef,
  IAAFPropertyDef **ppSampleIndexPropertyDef)
{
 // Find the class definition for components class
  IAAFClassDefSP pEssenceDataClass;
  CheckResult (pDictionary->LookupClassDef (AUID_AAFEssenceData, 
                                            &pEssenceDataClass));

  // Find our property definition
  CheckResult (pEssenceDataClass->LookupPropertyDef (
                                      kAAFPropID_EssenceData_Data,
                                      ppDataPropertyDef));
  CheckResult (pEssenceDataClass->LookupPropertyDef (
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

  CheckResult(pEssenceData->QueryInterface(IID_IAAFObject, (void **)&pObject));
  Test_LookupEssenceDataStreamPropertyDefinitions(pDictionary,
                                             &pDataPropertyDef,
                                             &pSampleIndexPropertyDef);

  CheckResult(pObject->GetPropertyValue(pDataPropertyDef,
                                        ppDataPropertyValue));
  aafBoolean_t present = kAAFFalse;
  CheckResult(pObject->IsPropertyPresent(pSampleIndexPropertyDef, &present));
  if (kAAFTrue == present)
  {
    CheckResult(pObject->GetPropertyValue(pSampleIndexPropertyDef,
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

  CheckResult(pStreamPropertyValue->GetType(&pTypeDef));
  IAAFTypeDefStream3SP pTypeDefStreamRaw;
  CheckResult(pTypeDef->QueryInterface(IID_IAAFTypeDefStream3,
                                       (void **)&pTypeDefStreamRaw));
  CheckResult(pTypeDefStreamRaw->GetMXFEssenceStream(0, ppTypeDefStream));
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
  CheckResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  // The byte order has never...
  CheckExpression(kAAFFalse == hasByteOrder, AAFRESULT_TEST_FAILED);
  // Set the byte order of the stream to little endian...
  CheckResult(pTypeDefStream->SetStoredByteOrder(pStreamPropertyValue, kAAFByteOrderLittle));
  CheckResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  CheckExpression(kAAFTrue == hasByteOrder, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetStoredByteOrder(pStreamPropertyValue, &byteOrder));
  CheckExpression(kAAFByteOrderLittle == byteOrder, AAFRESULT_TEST_FAILED);
  // Change the byte order.
  CheckResult(pTypeDefStream->ClearStoredByteOrder(pStreamPropertyValue));
  CheckResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  CheckExpression(kAAFFalse == hasByteOrder, AAFRESULT_TEST_FAILED);
  // Set the byte order of the stream to big endian...
  CheckResult(pTypeDefStream->SetStoredByteOrder(pStreamPropertyValue, kAAFByteOrderBig));
  CheckResult(pTypeDefStream->HasStoredByteOrder(pStreamPropertyValue, &hasByteOrder));
  CheckExpression(kAAFTrue == hasByteOrder, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetStoredByteOrder(pStreamPropertyValue, &byteOrder));
  CheckExpression(kAAFByteOrderBig == byteOrder, AAFRESULT_TEST_FAILED);

  // Get the current position
  aafInt64 streamSize = 0, expectedSize = 0, streamPosition = 0, expectedPosition = 0;

  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(0 == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(0 == streamPosition, AAFRESULT_TEST_FAILED);

  // The postioning code has the following assumption:
  assert(sizeof(sSmiley) == sizeof(sFrowney));

  // The current position is where the stream data starts.
  aafInt64  dataStartPosition = 0;
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &dataStartPosition));
  expectedSize = dataStartPosition;
  expectedPosition = dataStartPosition;

  // Write an array of bytes.
  CheckResult(pTypeDefStream->Write(pStreamPropertyValue,
                                    sizeof(sSmiley),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sSmiley))));

  // Checking for non-zero size and position must be checked 
  // carefully since the persistent size for structures and 
  // characters may be smaller then the in memory size. This
  // is why this test only uses types that have a known
  // persistent size: aafUInt8, aafUInt16, aafCharacter.
  expectedSize += sizeof(sSmiley);
  expectedPosition += sizeof(sSmiley);
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);

  // Test append
  // Restore the position to the begining of the stream.
  CheckResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));

  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(0 == streamPosition, AAFRESULT_TEST_FAILED);


  // Append an array of bytes.
  CheckResult(pTypeDefStream->Append(pStreamPropertyValue,
                                     sizeof(sFrowney),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sFrowney))));

  expectedSize += sizeof(sFrowney);
  expectedPosition += sizeof(sFrowney);
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);

  // Test non-zero repositioning and overriting...
  expectedPosition -= sizeof(sFrowney);
  CheckResult(pTypeDefStream->SetPosition(pStreamPropertyValue, expectedPosition));
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);
  // Write an array of bytes.
  CheckResult(pTypeDefStream->Write(pStreamPropertyValue,
                                    sizeof(sSmiley),
    reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sSmiley))));
 
  expectedPosition += sizeof(sSmiley);
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);


  // Write an array of shorts
  CheckResult(pTypeDefStream->WriteElements(pStreamPropertyValue,
                                            defs.tdUInt16(),
                                            sizeof(sTestUInt16),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafUInt16 *>(sTestUInt16))));

  expectedSize += sizeof(sTestUInt16);
  expectedPosition += sizeof(sTestUInt16);
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);



  // Restore the position to the begining of the stream.
  CheckResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));

  
  // Append an array of characters
  CheckResult(pTypeDefStream->AppendElements(pStreamPropertyValue,
                                             defs.tdCharacter(),
                                            sizeof(sTestCharacter),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafCharacter *>(sTestCharacter))));
  

  // The expected size and position must be incremented according to the
  // *persisted* size of aafCharacter, which is always 2, whereas the 
  // in-memory size may be 2 or 4, depending on the platform.
  expectedSize += (sizeof(sTestCharacter)/sizeof(wchar_t))*2; // sizeof(sTestCharacter);
  expectedPosition += (sizeof(sTestCharacter)/sizeof(wchar_t))*2; // sizeof(sTestCharacter);
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);
  
  // Test truncating the stream.
  CheckResult(pTypeDefStream->AppendElements(pStreamPropertyValue,
                                             defs.tdCharacter(),
                                            sizeof(sTruncatedCharacters),
    reinterpret_cast<aafMemPtr_t>(const_cast<aafCharacter *>(sTruncatedCharacters))));
  CheckResult(pTypeDefStream->SetSize(pStreamPropertyValue, expectedSize));
  
  CheckResult(pTypeDefStream->GetSize(pStreamPropertyValue, &streamSize));
  CheckExpression(expectedSize == streamSize, AAFRESULT_TEST_FAILED);
  CheckResult(pTypeDefStream->GetPosition(pStreamPropertyValue, &streamPosition));
  CheckExpression(expectedPosition == streamPosition, AAFRESULT_TEST_FAILED);

  // Restore the position to the begining of the stream.
  CheckResult(pTypeDefStream->SetPosition(pStreamPropertyValue, 0));
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
  CheckResult(pTypeDefStream->Read(pStreamPropertyValue,
                                    sizeof(sSmiley),
                                    reinterpret_cast<aafMemPtr_t>(smileyTest),
                                    &bytesRead));
  CheckExpression(bytesRead == sizeof(sSmiley), AAFRESULT_TEST_FAILED);
  CheckExpression(0 == memcmp(smileyTest, sSmiley, bytesRead),
                  AAFRESULT_TEST_FAILED);

  bytesRead = 0;
  CheckResult(pTypeDefStream->Read(pStreamPropertyValue,
                                    sizeof(sSmiley),
                                    reinterpret_cast<aafMemPtr_t>(smileyTest),
                                    &bytesRead));
  CheckExpression(bytesRead == sizeof(sSmiley), AAFRESULT_TEST_FAILED);
  CheckExpression(0 == memcmp(smileyTest, sSmiley, bytesRead),
                  AAFRESULT_TEST_FAILED);
  

  // Read the "appended" array of bytes.
  //  char frowneyTest[sizeof(sFrowney)];
  //bytesRead = 0;
  //CheckResult(pTypeDefStream->Read(pStreamPropertyValue,
  //                                  sizeof(sFrowney),
  //                                  reinterpret_cast<aafMemPtr_t>(frowneyTest),
  //                                  &bytesRead));
  //CheckExpression(bytesRead == sizeof(sFrowney), AAFRESULT_TEST_FAILED);
  //CheckExpression(0 == memcmp(frowneyTest, sFrowney, bytesRead),
  //                AAFRESULT_TEST_FAILED);
   
  
  // Read an array of aafUInt16
  aafUInt16 uint16Test[sizeof(sTestUInt16)/sizeof(aafUInt16)];
  bytesRead = 0;
  CheckResult(pTypeDefStream->ReadElements(pStreamPropertyValue,
                                           defs.tdUInt16(),
                                           sizeof(sTestUInt16),
                  reinterpret_cast<aafMemPtr_t>(uint16Test),
                                    &bytesRead));
  CheckExpression(bytesRead == sizeof(sTestUInt16), AAFRESULT_TEST_FAILED);
  CheckExpression(0 == memcmp(uint16Test, sTestUInt16, bytesRead),
                  AAFRESULT_TEST_FAILED);

  // Read the "appended" array of aafCharacter(s)
  aafCharacter characterTest[sizeof(sTestCharacter)/sizeof(aafCharacter)];
  bytesRead = 0;
  CheckResult(pTypeDefStream->ReadElements(pStreamPropertyValue,
                                           defs.tdCharacter(),
                                           sizeof(sTestCharacter),
                  reinterpret_cast<aafMemPtr_t>(characterTest),
                                    &bytesRead));
  CheckExpression(bytesRead == sizeof(sTestCharacter), AAFRESULT_TEST_FAILED);
  CheckExpression(0 == memcmp(characterTest, sTestCharacter, bytesRead),
                  AAFRESULT_TEST_FAILED);
}

class TestStreamAccess : public IAAFStreamAccess 
    {
    public:
	// Defeat gcc warning about private ctor/dtor and no friends
	// Note that this dummy function cannot itself be called because
	// it requires a constructed TestStreamAccess object.
	friend void dummyFriend(TestStreamAccess);

        virtual HRESULT STDMETHODCALLTYPE WriteStream( 
            IAAFPropertyValue *propertyValue,
            aafMemPtr_t pUserData);
        virtual ULONG STDMETHODCALLTYPE AddRef( void);
        virtual ULONG STDMETHODCALLTYPE Release( void);
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(
           REFIID iid,
           void **ppIfc);

        static HRESULT Create(IAAFStreamAccess** ppStreamAccess);

    private:

       TestStreamAccess();

       virtual ~TestStreamAccess();

       aafUInt32 _referenceCount;

    };
 
HRESULT STDMETHODCALLTYPE
TestStreamAccess::WriteStream (IAAFPropertyValue *propertyValue, aafMemPtr_t pUserData)
{
	IAAFTypeDefSP			pTypeDef;
	IAAFTypeDefStream3SP	pTypeDefStreamRaw;
	IAAFTypeDefStreamSP		pTypeDefStream;
	IAAFMetaDefinitionSP	pMetaDef;
	aafCharacter			debugBuf[256];

	CheckResult(propertyValue->GetType(&pTypeDef));
	CheckResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void **)&pMetaDef));
	CheckResult(pMetaDef->GetName(debugBuf, 256));

	CheckResult(pTypeDef->QueryInterface(IID_IAAFTypeDefStream3,
                                       (void **)&pTypeDefStreamRaw));

	CheckResult(pTypeDefStreamRaw->GetMXFEssenceStream(0, &pTypeDefStream));

	// Set the byte order of the stream to big endian...
	CheckResult(pTypeDefStream->SetStoredByteOrder(propertyValue, kAAFByteOrderBig));

    // Write the bytes
	CheckResult(pTypeDefStream->Write(propertyValue, sizeof(sSmiley), pUserData));

    return AAFRESULT_SUCCESS;
}

ULONG STDMETHODCALLTYPE
    TestStreamAccess::AddRef ()
{
  return ++_referenceCount;
}

ULONG STDMETHODCALLTYPE
    TestStreamAccess::Release ()
{
  aafUInt32 r = --_referenceCount;
  if (r == 0) {
    delete this;
  }
  return r;
}

HRESULT STDMETHODCALLTYPE
    TestStreamAccess::QueryInterface (REFIID iid, void ** ppIfc)
{
  if (ppIfc == 0)
    return AAFRESULT_NULL_PARAM;

  if (memcmp(&iid, &IID_IUnknown, sizeof(IID)) == 0) {
    IUnknown* unk = (IUnknown*) this;
    *ppIfc = (void*) unk;
    AddRef ();
    return AAFRESULT_SUCCESS; 
  } else if (memcmp(&iid, &IID_IAAFStreamAccess, sizeof(IID)) == 0) {
    IAAFStreamAccess* cpa = this;
    *ppIfc = (void*) cpa;
    AddRef ();
    return AAFRESULT_SUCCESS;
  } else {
    return E_NOINTERFACE;
  }
}

HRESULT TestStreamAccess::Create(IAAFStreamAccess** ppStreamAccess)
{
  if (ppStreamAccess == 0)
    return AAFRESULT_NULL_PARAM;

  IAAFStreamAccess* result = new TestStreamAccess();
  if (result == 0)
    return AAFRESULT_NOMEMORY;

  result->AddRef();
  *ppStreamAccess = result;
  return AAFRESULT_SUCCESS;
}

TestStreamAccess::TestStreamAccess()
: _referenceCount(0)
{
}

TestStreamAccess::~TestStreamAccess()
{
  assert(_referenceCount == 0);
}

static void Test_EssenceStreamPullWrite(
	IAAFFile	*pFile,
	CAAFBuiltinDefs & defs)
{
  IAAFTypeDefStreamSP pTypeDefStream;
  IAAFTypeDefStreamExSP pTypeDefStreamEx;

    // Get the direct access interfaces.
	IAAFObjectSP		pObject;
	IAAFPropertyDefSP	pDataPropertyDef;
	IAAFPropertyDefSP	pSampleIndexPropertyDef;  
	IAAFPropertyValueSP pStreamPropertyValue;
	IAAFStreamAccess		*cb = 0;
	IAAFEssenceData		*pEssenceData;
	IAAFHeaderSP		pHeader;
	IAAFDictionarySP	pDictionary;
	IAAFSourceMobSP		pSourceMob;
	IAAFMobSP			pMob;
	IAAFEssenceDescriptorSP pEssenceDesciptor;

	CheckResult (pFile->GetHeader (&pHeader));
	CheckResult (pHeader->GetDictionary (&pDictionary));

	// Create an EssenceData object and associated Mob
	Test_CreateEssenceData(defs, pHeader, sMobID[2], sMobName[2], &pEssenceData);


	// Normally we'd add the essence data object here and, with it attached,
    // write essence to it. Instead establish a call back so that the data
    // may be written later during save.
	CheckResult(pEssenceData->QueryInterface(IID_IAAFObject, (void **)&pObject));
	Test_LookupEssenceDataStreamPropertyDefinitions(pDictionary,
                                             &pDataPropertyDef,
                                             &pSampleIndexPropertyDef);

	CheckResult(pObject->GetPropertyValue(pDataPropertyDef,
                                        &pStreamPropertyValue));
  
	Test_GetTypeDefStream(pStreamPropertyValue, &pTypeDefStream);

	CheckResult(TestStreamAccess::Create(&cb));
	CheckResult(pTypeDefStream->QueryInterface(IID_IAAFTypeDefStreamEx, (void **)&pTypeDefStreamEx));
	CheckResult(pTypeDefStreamEx->SetCallback(pStreamPropertyValue, cb,
		reinterpret_cast<aafMemPtr_t>(const_cast<char *>(sSmiley))));
	// !!! tjb not yet cb->Release();
}

// Create the test file.
void CAAFTypeDefStream_create (
    aafCharacter_constptr pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  IAAFFileSP pFile;

  try
  {
    IAAFHeaderSP pHeader;
    IAAFDictionarySP pDictionary;
    IAAFSourceMobSP pSourceMob;
    IAAFMobSP pMob;
    IAAFEssenceDescriptorSP pEssenceDesciptor;
    IAAFEssenceDataSP pEssenceData;
    IAAFPropertyValueSP pDataPropertyValue;
    IAAFPropertyValueSP pSampleIndexPropertyValue;

    // Remove the previous test file is one exists
    ::RemoveTestFile (pFileName);

    // Create the file.
    CheckResult (CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
    CheckResult (pFile->GetHeader (&pHeader));
    CheckResult (pHeader->GetDictionary (&pDictionary));

    CAAFBuiltinDefs defs (pDictionary);

    // Create an EssenceData object and associated Mob
    Test_CreateEssenceData(defs, pHeader, sMobID[0], sMobName[0], &pEssenceData);

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

    Test_EssenceStreamPullWrite(pFile, defs);

    // Test IAAFTypeDefStream3 methods
    IAAFEssenceDataSP pEssenceData2;
    IAAFPropertyValueSP pDataPropertyValue2;
    IAAFPropertyValueSP pSampleIndexPropertyValue2;

    Test_CreateEssenceData(defs, pHeader, sMobID[3], sMobName[3], &pEssenceData2);

    Test_EssenceStreamPropertyValues(pDictionary,
                                     pEssenceData2,
                                     &pDataPropertyValue2,
                                     &pSampleIndexPropertyValue2);

    CheckResult(pFile->Save());
    CheckResult(pFile->Close());
  }
  catch(AAFRESULT& hr)
  {
    // Ignore failure if file not open
    pFile->Close();
    CheckResult(hr);
  }
}

// Create the test file.
void CAAFTypeDefStream_read (aafCharacter_constptr pFileName) // throw HRESULT
{
  IAAFFileSP pFile;

  try
  {
    IAAFHeaderSP pHeader;
    IAAFDictionarySP pDictionary;
    IAAFEssenceDataSP pEssenceData;
    IAAFPropertyValueSP pDataPropertyValue;
    IAAFPropertyValueSP pSampleIndexPropertyValue;

    aafUID_t  fileKind;
    aafBool isAAFFile = kAAFFalse;
    CheckResult (AAFFileIsAAFFile(pFileName, &fileKind, &isAAFFile));
    CheckExpression(isAAFFile == kAAFTrue, AAFRESULT_TEST_FAILED);

    CheckResult (AAFFileOpenExistingRead(pFileName, 0, &pFile));
    CheckResult (pFile->GetHeader (&pHeader));
    CheckResult (pHeader->GetDictionary (&pDictionary));

    CAAFBuiltinDefs defs (pDictionary);

#if 1
    // Missing api method!
    CheckResult (pHeader->LookupEssenceData(sMobID[0], &pEssenceData));
#else
    // For this version of the test assume that there is only a single
    // essence data...
    aafUInt32 count;
    CheckResult(pHeader->CountEssenceData(&count));
    CheckExpression(1 == count, AAFRESULT_TEST_FAILED);
    IEnumAAFEssenceDataSP pEnumEssenceData;
    CheckResult(pHeader->EnumEssenceData(&pEnumEssenceData));
    CheckResult(pEnumEssenceData->NextOne(&pEssenceData));
    aafMobID_t testMobID = {0};
    CheckResult(pEssenceData->GetFileMobID(&testMobID));
    CheckExpression(0 == memcmp(&testMobID, &sMobID[0], sizeof(aafMobID_t)),
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


    // Test IAAFTypeDefStream3 methods
    IAAFEssenceDataSP pEssenceData2;

    // sMobID[3] may not be in the file, in the case
    // the file was created by an older test.
    if(pHeader->LookupEssenceData(sMobID[3], &pEssenceData2)==AAFRESULT_SUCCESS)
    {
      IAAFPropertyValueSP pDataPropertyValue2;
      IAAFPropertyValueSP pSampleIndexPropertyValue2;

      Test_EssenceStreamPropertyValues(pDictionary,
                                       pEssenceData2,
                                       &pDataPropertyValue2,
                                       &pSampleIndexPropertyValue2);

    }


    CheckResult(pFile->Close());
  }
  catch(AAFRESULT& hr)
  {
    // Ignore failure if file not open
    pFile->Close();
    CheckResult(hr);
  }
}
