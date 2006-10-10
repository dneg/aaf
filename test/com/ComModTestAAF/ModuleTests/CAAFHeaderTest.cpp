// @doc INTERNAL
// @com This file implements the module test for CAAFHeader.
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


const aafUInt32 gMaxMobCount = 5;

//--cf  //5 mobid's need to be genererated 
static const aafMobID_t  _mobID[gMaxMobCount] = 
{//start mobid block
	//first id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
		0x13, 0x00, 0x00, 0x00,
	{0xca9af110, 0x0400, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}},
	
	//second id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0xe00691a8, 0x0400, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}},
	
	//third id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0xea00082e, 0x0400, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}},
	
	//fourth id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0xf6d0abee, 0x0400, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}},
	
	//fifth id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x002f299a, 0x0401, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}}
	
}; //end mobid block

const aafUID_t descSchID_1 = { 0x070D, 0x4400, 0xA00B, { 0x06, 0x07, 0x07, 0x08, 0x00, 0x08, 0x09, 0x09 } };
const aafUID_t descSchID_2 = { 0xAF45, 0x3221, 0xA05C, { 0x05, 0x05, 0x06, 0x07, 0x08, 0x08, 0x09, 0x10 } };
const aafUID_t setOpPatternID = {0x4e8404ff, 0x0f29, 0x11f4, {0xf3, 0x59, 0x0f, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


// {B8CF441F-F2F5-11d4-8040-00104BC9156D}
static const aafUID_t Header_OptionalPropID =
{ 0xb8cf441f, 0xf2f5, 0x11d4, { 0x80, 0x40, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

static aafCharacter_constptr Header_OptionalPropName = L"Header optional property";

static const aafUInt32 Header_OptionalPropertyValue = 42;

inline void checkhr(HRESULT expression, HRESULT r)
{
  if (expression != r)
    throw r;
}

// Utility class to implement the test.
struct HeaderTest
{
  HeaderTest(aafProductIdentification_constref productID);
  ~HeaderTest();

  void createFile(wchar_t *pFileName,
                  aafUID_constref fileKind,
                  testRawStorageType_t rawStorageType);
  void openFile(wchar_t *pFileName);

  void writeOptionalProperties();
  void readOptionalProperties();

  void checkByteOrder();
  void checkOperationalPattern();
  void checkEssenceContainer();
  void checkDescriptiveSchemes();
  void createFileMob(aafUInt32 itemNumber);
  void createEssenceData(IAAFSourceMob *pSourceMob);
  void openMobs();
  void openEssenceData();

  int formatMobName(aafUInt32 itemNumber, wchar_t* mobName);
  void cleanupReferences();
  void check(HRESULT hr);
  void removeTestFile(const wchar_t* pFileName);

  // Shared member data:
  HRESULT _hr;
  aafProductVersion_t _productVersion;
  aafProductIdentification_t _productInfo;
  IAAFFile *_pFile;
  bool _bFileOpen;
  IAAFHeader *_pHeader;
  IAAFHeader2 *_pHeader2;
  IAAFDictionary *_pDictionary;
  IAAFDictionary2 *_pDictionary2;

  IAAFMob *_pMob;
  IAAFSourceMob *_pSourceMob;
  IEnumAAFMobs *_pEnumMobs;
  IAAFEssenceDescriptor *_pEssenceDescriptor;
  IAAFFileDescriptor *_pFileDescriptor;
  IEnumAAFEssenceData *_pEnumEssenceData;
  IAAFEssenceData *_pEssenceData;
  IAAFIdentification *_pIdent;
  IEnumAAFIdentifications *_pEnumIdent;
  IAAFObject *_pHeaderObject;
  IAAFClassDef *_pClassDef;
  IAAFTypeDef *_pOptionalPropTypeDef;
  IAAFTypeDefInt *_pTypeDefInt;
  IAAFPropertyDef *_pOptionalPropDef;
  IAAFPropertyValue *_pOptionalPropValue;
  IAAFEndian *_pEndian;
};

extern "C" HRESULT CAAFHeader_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFHeader_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  const size_t fileNameBufLen = 128;
  aafWChar fileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, fileName );
  HeaderTest ht(productID);

  try
  {
	if(mode == kAAFUnitTestReadWrite)
	   ht.createFile(fileName, fileKind, rawStorageType);
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
		 << " exception!" << endl; 
	hr = AAFRESULT_TEST_FAILED;
  }

  // Cleanup our object if it exists.

  return hr;
}

HeaderTest::HeaderTest(aafProductIdentification_constref productID):
  _hr(AAFRESULT_SUCCESS),
  _productInfo(productID),
  _pFile(NULL),
  _bFileOpen(false),
  _pHeader(NULL),
  _pHeader2(NULL),
  _pDictionary(NULL),
  _pDictionary2(NULL),
  _pMob(NULL),
  _pSourceMob(NULL),
  _pEnumMobs(NULL),
  _pEssenceDescriptor(NULL),
  _pFileDescriptor(NULL),
  _pEnumEssenceData(NULL),
  _pEssenceData(NULL),
  _pHeaderObject(NULL),
  _pClassDef(NULL),
  _pOptionalPropTypeDef(NULL),
  _pTypeDefInt(NULL),
  _pOptionalPropDef(NULL),
  _pOptionalPropValue(NULL),
  _pEndian(NULL)
{
}

HeaderTest::~HeaderTest()
{
  cleanupReferences();
}

void HeaderTest::cleanupReferences()
{
  if (NULL != _pOptionalPropValue)
  {
    _pOptionalPropValue->Release();
    _pOptionalPropValue = NULL;
  }

  if (NULL != _pOptionalPropDef)
  {
    _pOptionalPropDef->Release();
    _pOptionalPropDef = NULL;
  }

  if (NULL != _pTypeDefInt)
  {
    _pTypeDefInt->Release();
    _pTypeDefInt = NULL;
  }

  if (NULL != _pOptionalPropTypeDef)
  {
    _pOptionalPropTypeDef->Release();
    _pOptionalPropTypeDef = NULL;
  }

  if (NULL != _pClassDef)
  {
    _pClassDef->Release();
    _pClassDef = NULL;
  }

  if (NULL != _pHeaderObject)
  {
    _pHeaderObject->Release();
    _pHeaderObject = NULL;
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

  if(NULL != _pEndian)
  {
    _pEndian->Release();
    _pEndian = NULL;
  }

  if(NULL != _pHeader2)
  {
    _pHeader2->Release();
    _pHeader2 = NULL;
  }

  if(NULL != _pDictionary2)
  {
    _pDictionary2->Release();
    _pDictionary2 = NULL;
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


void HeaderTest::createFile(
    wchar_t *pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType)
{
  // Remove the previous test file if any.
  removeTestFile(pFileName);

  check(CreateTestFile( pFileName, fileKind, rawStorageType, _productInfo, &_pFile ));

  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));

  // GetDictionary
  checkhr(_pHeader->GetDictionary(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetDictionary(&_pDictionary));
  
  CAAFBuiltinDefs defs (_pDictionary);

  // GetRefImplVersion  
  aafProductVersion_t	productVerison;
  checkhr(_pHeader->GetRefImplVersion(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetRefImplVersion(&productVerison));

  // GetFileRevision  
  aafVersionType_t	fileVersion;
  checkhr(_pHeader->GetFileRevision(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetFileRevision(&fileVersion));

  // GetLastModified  
  aafTimeStamp_t	timeStamp;
  checkhr(_pHeader->GetLastModified(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetLastModified(&timeStamp));

  // CountIdentifications
  aafUInt32	numIdents = 0;
  checkhr(_pHeader->CountIdentifications(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->CountIdentifications(&numIdents));
  checkhr(1 == numIdents, true);
  
  // GetLastIdentification
  checkhr(_pHeader->GetLastIdentification(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetLastIdentification(&_pIdent));
  _pIdent->Release();
  _pIdent = NULL;
  
  // AppendIdentification
  check(defs.cdIdentification()->
		CreateInstance(IID_IAAFIdentification, 
					   (IUnknown **)&_pIdent));

  // {319EFBBF-1189-11d4-804A-080036210804}
  static const  aafUID_t productId = 
  { 0x319efbbf, 0x1189, 0x11d4, { 0x80, 0x4a, 0x8, 0x0, 0x36, 0x21, 0x8, 0x4 } };

  check(_pIdent->Initialize(L"Company Name", L"Product Name", L"Product Version String", productId));

  checkhr(_pHeader->AppendIdentification(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->AppendIdentification(_pIdent));
  check(_pHeader->CountIdentifications(&numIdents));
  checkhr(2 == numIdents, true);  
  _pIdent->Release();
  _pIdent = NULL;

  // GetIdentificationAt
  checkhr(_pHeader->GetIdentificationAt(0, NULL), AAFRESULT_NULL_PARAM);
  checkhr(_pHeader->GetIdentificationAt(numIdents, &_pIdent), AAFRESULT_BADINDEX);
  check(_pHeader->GetIdentificationAt(0, &_pIdent));
  _pIdent->Release();
  _pIdent = NULL;
  check(_pHeader->GetIdentificationAt(1, &_pIdent));
  _pIdent->Release();
  _pIdent = NULL;

  // GetIdentifications
  checkhr(_pHeader->GetIdentifications(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetIdentifications(&_pEnumIdent));
  _pEnumIdent->Release();
  _pEnumIdent = NULL;

  aafUID_t generation;
  IAAFObject *pObject = NULL;
  
  // {2865256E-0BEC-11d4-A3FA-0004AC96A937} - guaranteed to *not* match any existing generations
  static const aafUID_t fakeGeneration = 
  	{ 0x2865256e, 0xbec, 0x11d4, { 0xa3, 0xfa, 0x0, 0x4, 0xac, 0x96, 0xa9, 0x37 } };

  // LookupIdentification
  checkhr(_pHeader->LookupIdentification(generation, NULL), AAFRESULT_NULL_PARAM);
  checkhr(_pHeader->LookupIdentification(fakeGeneration, &_pIdent), AAFRESULT_OBJECT_NOT_FOUND);

  check(_pHeader->QueryInterface (IID_IAAFObject, (void **)&pObject));
  check(pObject->EnableGenerationTracking());
  _pFile->Save();  // tjb Hack - cause a generation to be assigned.
  check(pObject->GetGenerationAUID(&generation));
  check(_pHeader->LookupIdentification(generation, &_pIdent));
  _pIdent->Release();
  _pIdent = NULL;
  pObject->Release();
  pObject = NULL;

  // IsEssenceDataPresent
  aafBool 	pResult = true;
  checkhr(_pHeader->IsEssenceDataPresent(_mobID[0], kAAFEssence, NULL), AAFRESULT_NULL_PARAM);  
  checkhr(_pHeader->IsEssenceDataPresent(_mobID[0], 2, &pResult), AAFRESULT_INVALID_ENUM_VALUE);  
  check(_pHeader->IsEssenceDataPresent(_mobID[0], kAAFForeignMedia, &pResult));
  if (pResult) check(AAFRESULT_TEST_FAILED);  
  check(_pHeader->IsEssenceDataPresent(_mobID[0], kAAFEssence, &pResult));  
  if (pResult) check(AAFRESULT_TEST_FAILED);  

  for (aafUInt32 item = 0; item < gMaxMobCount; ++item)
    createFileMob(item);
 
  writeOptionalProperties();
  readOptionalProperties();
 
  check(_pFile->Save());
  cleanupReferences();
}

void HeaderTest::openFile(wchar_t *pFileName)
{
  check(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bFileOpen = true;
  check(_pFile->GetHeader(&_pHeader));
  check(_pHeader->GetDictionary(&_pDictionary));

  openEssenceData();
  openMobs();
  readOptionalProperties();
  checkByteOrder();
  checkOperationalPattern();
  checkEssenceContainer();
  checkDescriptiveSchemes();
  cleanupReferences();
}


void HeaderTest::writeOptionalProperties()
{
  assert(_pHeader && _pDictionary);

  aafProductVersion_t toolkitVersion;
  check(GetAAFVersions(_pHeader, &toolkitVersion, NULL));

  if (ExtendingAAFObjectSupported(toolkitVersion))
  {
    // First register the new optional property with the class
    // definition for the Header.
    check(_pDictionary->LookupTypeDef(kAAFTypeID_UInt32, &_pOptionalPropTypeDef));
    check(_pOptionalPropTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void **)&_pTypeDefInt));
    check(_pDictionary->LookupClassDef(AUID_AAFHeader, &_pClassDef));
    check(_pClassDef->RegisterOptionalPropertyDef(Header_OptionalPropID, 
                                                  Header_OptionalPropName,
                                                  _pOptionalPropTypeDef,
                                                  &_pOptionalPropDef));
    
    // Now write a value into the new optional property.
    check(_pHeader->QueryInterface(IID_IAAFObject, (void **)&_pHeaderObject));
    check(_pTypeDefInt->CreateValue((aafMemPtr_t)&Header_OptionalPropertyValue,
                                    sizeof(Header_OptionalPropertyValue),
                                    &_pOptionalPropValue));
    check(_pHeaderObject->SetPropertyValue(_pOptionalPropDef, _pOptionalPropValue));

    //add some descriptive schemes
    check(_pHeader->QueryInterface(IID_IAAFHeader2, reinterpret_cast<void**>(&_pHeader2) ) );
    assert(_pHeader2);  

    if(_pHeader2->AddDescriptiveScheme(descSchID_1) != AAFRESULT_SUCCESS)
      check(AAFRESULT_TEST_FAILED);

    if(_pHeader2->AddDescriptiveScheme(descSchID_2) != AAFRESULT_SUCCESS)
      check(AAFRESULT_TEST_FAILED);

    //now remove one descriptive scheme
    if(_pHeader2->RemoveDescriptiveScheme(descSchID_2) != AAFRESULT_SUCCESS)
      check(AAFRESULT_TEST_FAILED);

    //test setOperationalPattern method
    if(_pHeader2->SetOperationalPattern(setOpPatternID) != AAFRESULT_SUCCESS)
      check(AAFRESULT_TEST_FAILED);
    
    // Release references
    _pOptionalPropValue->Release();
    _pOptionalPropValue = NULL;
    _pHeaderObject->Release();
    _pHeaderObject = NULL;
    _pOptionalPropDef->Release();
    _pOptionalPropDef = NULL;
    _pClassDef->Release();
    _pClassDef = NULL;
    _pTypeDefInt->Release();
    _pTypeDefInt = NULL;
    _pOptionalPropTypeDef->Release();
    _pOptionalPropTypeDef = NULL;
  }
}


void HeaderTest::readOptionalProperties()
{
  assert(_pHeader && _pDictionary);

  aafProductVersion_t toolkitVersion, fileToolkitVersion;
  check(GetAAFVersions(_pHeader, &toolkitVersion, &fileToolkitVersion));

  if (ExtendingAAFObjectSupported(toolkitVersion) && 
      ExtendingAAFObjectSupported(fileToolkitVersion))
  {
    // First find the new optional property definition in the class
    // definition for the Header.
    check(_pDictionary->LookupTypeDef(kAAFTypeID_UInt32, &_pOptionalPropTypeDef));
    check(_pOptionalPropTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void **)&_pTypeDefInt));
    check(_pDictionary->LookupClassDef(AUID_AAFHeader, &_pClassDef));
    check(_pClassDef->LookupPropertyDef(Header_OptionalPropID, &_pOptionalPropDef));
    
    // Now read a value into the new optional property.
    check(_pHeader->QueryInterface(IID_IAAFObject, (void **)&_pHeaderObject));
    check(_pHeaderObject->GetPropertyValue(_pOptionalPropDef, &_pOptionalPropValue));
    aafUInt32 value = 0;
    check(_pTypeDefInt->GetInteger(_pOptionalPropValue, (aafMemPtr_t)&value, sizeof(value)));
    if (Header_OptionalPropertyValue != value) check(AAFRESULT_TEST_FAILED);

   
    // Release references
    _pOptionalPropValue->Release();
    _pOptionalPropValue = NULL;
    _pHeaderObject->Release();
    _pHeaderObject = NULL;
    _pOptionalPropDef->Release();
    _pOptionalPropDef = NULL;
    _pClassDef->Release();
    _pClassDef = NULL;
    _pTypeDefInt->Release();
    _pTypeDefInt = NULL;
    _pOptionalPropTypeDef->Release();
    _pOptionalPropTypeDef = NULL;
  }
}


int HeaderTest::formatMobName(aafUInt32 itemNumber, wchar_t* mobName)
{
  assert(0 <= itemNumber && gMaxMobCount > itemNumber);
  assert(NULL != mobName);

  char cBuffer[64];

  sprintf(cBuffer, "HeaderTest File Mob %d", itemNumber);
  size_t count = mbstowcs(mobName, cBuffer, strlen(cBuffer) + 1);
  if (static_cast<size_t>(-1) == count)
    check(AAFRESULT_INTERNAL_ERROR);
  return count;
}


void HeaderTest::createFileMob(aafUInt32 itemNumber)
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
  check(defs.cdSourceMob()->
		CreateInstance(IID_IAAFSourceMob, 
					   (IUnknown **)&_pSourceMob));

  check(_pSourceMob->QueryInterface (IID_IAAFMob, (void **)&_pMob));
  
  check(_pMob->SetMobID(_mobID[itemNumber]));

  check(_pMob->SetName(wcBuffer));
  
  // create concrete subclass of FileDescriptor
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
  

	//create a container for the AIFCDescriptor, assigning it a container definition
	//4 containers will be created
  	check(_pDictionary->QueryInterface(IID_IAAFDictionary2, reinterpret_cast<void**>(&_pDictionary2) ) );
  	assert(_pDictionary2);

	aafUInt32 num;
	check(_pDictionary2->CountContainerDefs(&num));

	IEnumAAFContainerDefs *pEnum;
	IAAFContainerDef *pDef;

	check(_pDictionary2->GetContainerDefs(&pEnum));

	for(unsigned int i = 0; i <= itemNumber && i < num; i++)
	{
		check(pEnum->NextOne(&pDef));
	}

	check(pDef->SetEssenceIsIdentified((aafBoolean_t)true));
	check(_pFileDescriptor->SetContainerFormat(pDef));


  // AddMob
  checkhr(_pHeader->AddMob(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->AddMob(_pMob));
  checkhr(_pHeader->AddMob(_pMob), AAFRESULT_DUPLICATE_MOBID);
  
  // RemoveMob
  checkhr(_pHeader->RemoveMob(NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->RemoveMob(_pMob));
  checkhr(_pHeader->RemoveMob(_pMob), AAFRESULT_MOB_NOT_FOUND);
  
  // Add it again, for real this time
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
  check(defs.cdEssenceData()->
		CreateInstance(IID_IAAFEssenceData,
					   (IUnknown **)&_pEssenceData));

  check(_pEssenceData->SetFileMob(pSourceMob));
  // AddEssenceData
  checkhr(_pHeader->AddEssenceData(NULL), AAFRESULT_NULL_PARAM);  
  check(_pHeader->AddEssenceData(_pEssenceData));
  checkhr(_pHeader->AddEssenceData(_pEssenceData), AAFRESULT_DUPLICATE_MOBID);  

  // RemoveEssenceData
  checkhr(_pHeader->RemoveEssenceData(NULL), AAFRESULT_NULL_PARAM);  
  check(_pHeader->RemoveEssenceData(_pEssenceData));
  checkhr(_pHeader->RemoveEssenceData(_pEssenceData), AAFRESULT_ESSENCE_NOT_FOUND);  

  // Add it for real this time
  check(_pHeader->AddEssenceData(_pEssenceData));

  _pEssenceData->Release();
  _pEssenceData = NULL;
}

void HeaderTest::openMobs()
{
  aafUInt32	srchItem;
  bool		found;

  assert(_pFile && _pHeader);
  assert(NULL == _pEnumMobs);
  assert(NULL == _pMob);
  assert(NULL == _pSourceMob);


  // CountMobs
  aafNumSlots_t mobCount = 0;
  checkhr(_pHeader->CountMobs(kAAFAllMob, NULL), AAFRESULT_NULL_PARAM);
  checkhr(_pHeader->CountMobs(-1, NULL), AAFRESULT_INVALID_ENUM_VALUE);
  check(_pHeader->CountMobs(kAAFAllMob, &mobCount));
  if (gMaxMobCount != mobCount)
    check(AAFRESULT_TEST_FAILED);
 
  // GetMobs
  checkhr(_pHeader->GetMobs(NULL, NULL), AAFRESULT_NULL_PARAM);
  check(_pHeader->GetMobs(NULL, &_pEnumMobs));
  for (aafUInt32 item = 0; item < mobCount; ++item)
  {
    check(_pEnumMobs->NextOne(&_pMob));

    // Validate the mob data.
	aafMobID_t mobID;
    check(_pMob->GetMobID(&mobID));
	for(srchItem = 0, found = false; !found && srchItem < mobCount; ++srchItem)
	{
		 if (0 == memcmp(&mobID, &_mobID[srchItem], sizeof(mobID)))
		 {
			 found = true;
			 break;
		 }
	}
	if(!found)
	{
		check(AAFRESULT_TEST_FAILED);
	}

	wchar_t mobName[128], expectedMobName[128];
    aafUInt32 expectedMobNameLen = (formatMobName(srchItem, expectedMobName) + 1) * sizeof(wchar_t);
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

  // LookupMob
  static const aafMobID_t  _badMobID = 	{{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
											0xFF, 0xFF, 0xFF, 0xFF,
										{0xFFFFFFFF, 0xFFFF, 0xFFFF, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}};
										

  checkhr(_pHeader->LookupMob(_mobID[0], NULL), AAFRESULT_NULL_PARAM);
  checkhr(_pHeader->LookupMob(_badMobID, &_pMob), AAFRESULT_MOB_NOT_FOUND);
  check(_pHeader->LookupMob(_mobID[0], &_pMob));


  _pEnumMobs->Release();
  _pEnumMobs = NULL;
}

void HeaderTest::openEssenceData()
{
  assert(_pFile && _pHeader);
  assert(NULL == _pEnumEssenceData);
  assert(NULL == _pEssenceData);
  assert(NULL == _pSourceMob);

  // IsEssenceDataPresent
  aafBool 	pResult = false;
  check(_pHeader->IsEssenceDataPresent(_mobID[0], kAAFForeignMedia, &pResult));
  if (pResult == false) check(AAFRESULT_TEST_FAILED);  
  check(_pHeader->IsEssenceDataPresent(_mobID[0], kAAFEssence, &pResult));  
  if (pResult == false) check(AAFRESULT_TEST_FAILED);  


  // CountEssenceData
  checkhr(_pHeader->CountEssenceData(NULL), AAFRESULT_NULL_PARAM);  
  aafUInt32 essenceDataCount = 0;
  check(_pHeader->CountEssenceData(&essenceDataCount));
  if (gMaxMobCount != essenceDataCount)
    check(AAFRESULT_TEST_FAILED);
 
  // LookupEssenceData
  check(_pHeader->LookupEssenceData(_mobID[0], &_pEssenceData));  
  // Make sure that the essence data still references
  // a valid mob.
  check(_pEssenceData->GetFileMob(&_pSourceMob));
  _pSourceMob->Release();
  _pSourceMob = NULL;
  _pEssenceData->Release();
  _pEssenceData = NULL;

  // EnumEssenceData
  checkhr(_pHeader->EnumEssenceData(NULL), AAFRESULT_NULL_PARAM);   
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

void HeaderTest::checkByteOrder()
{
  //little endian tests blanked out because they will fail on mac/sparc systems since
  //those systems use big endian. intel byte order

  if(_pHeader2 == NULL)
  {
    check(_pHeader->QueryInterface(IID_IAAFEndian, reinterpret_cast<void**>(&_pEndian) ) );
    assert(_pEndian);
  }
  eAAFByteOrder_t result;

  //test GetStoredByteOrder method
  if(_pEndian->GetStoredByteOrder(&result) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  //ensure that the returned val is little endian
  if(result != kAAFByteOrderLittle && result != kAAFByteOrderBig)
    check(AAFRESULT_TEST_FAILED);

  //test GetNativeByteOrder method
  if(_pEndian->GetNativeByteOrder(&result) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  //ensure that the returned val is little endian
  if(result != kAAFByteOrderLittle && result != kAAFByteOrderBig)
    check(AAFRESULT_TEST_FAILED);

}

void HeaderTest::checkOperationalPattern()
{
  if(_pHeader2 == NULL)
  {
    check(_pHeader->QueryInterface(IID_IAAFHeader2, reinterpret_cast<void**>(&_pHeader2) ) );
    assert(_pHeader2);  
  }
  aafUID_t getOpPatternID;

  //test getOperationalPattern method
  if(_pHeader2->GetOperationalPattern(&getOpPatternID) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  //ensure the retrieved operational pattern is the same as the set pattern
  if(setOpPatternID != getOpPatternID)   
    check(AAFRESULT_TEST_FAILED);

}

void HeaderTest::checkEssenceContainer()
{
  //Essence containers created in createFileMob() so that below methods could be tested
  if(_pDictionary2 == NULL)
  {
    check(_pDictionary->QueryInterface(IID_IAAFDictionary2, reinterpret_cast<void**>(&_pDictionary2) ) );
    assert(_pDictionary2);
  }

  //NOTE:_pHeader2 init'd in (this*)checkOperationalPattern()
  if(_pHeader2->UpdateEssenceContainers() != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED); 
  //
  aafUInt32 count;
  if(_pHeader2->CountEssenceContainers(&count) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  //ensure the # of container defs equals # of essence containers
  aafUInt32 num;
  check(_pDictionary2->CountContainerDefs(&num));
  if(count != num)
    check(AAFRESULT_TEST_FAILED);    

  aafUID_t* pContIDs = new aafUID_t [count]; 
  if(_pHeader2->GetEssenceContainers(count, pContIDs) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  aafBoolean_t isPresent = false;
  if(_pHeader2->IsEssenceContainerPresent(pContIDs[0], &isPresent) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);  

  //ensure the container was found
  if(isPresent == false)
    check(AAFRESULT_TEST_FAILED); 

  delete [] pContIDs;

}

void HeaderTest::checkDescriptiveSchemes()
{
  if(_pHeader2 == NULL)
  {
    check(_pHeader->QueryInterface(IID_IAAFHeader2, reinterpret_cast<void**>(&_pHeader2) ) );
    assert(_pHeader2);  
  }

  aafUInt32 count;

  //ensure there is only one desc scheme present
  if(_pHeader2->CountDescriptiveSchemes(&count) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  if(count != 1)
    check(AAFRESULT_TEST_FAILED);

  aafUID_t* pDescIDs = new aafUID_t [count];
  if(_pHeader2->GetDescriptiveSchemes(count, pDescIDs) != AAFRESULT_SUCCESS) 
    check(AAFRESULT_TEST_FAILED);

  aafBoolean_t isPresent = false;
  if(_pHeader2->IsDescriptiveSchemePresent( pDescIDs[0], &isPresent) != AAFRESULT_SUCCESS)
    check(AAFRESULT_TEST_FAILED);

  delete [] pDescIDs;

}
