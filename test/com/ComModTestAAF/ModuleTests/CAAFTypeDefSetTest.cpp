// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefSet
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
 *  prior written permission of Avid Technology, Inc.
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

// Conditional symbol for testing the creation of a new optional 
// strong reference set property on the dictionary. This is experimental
// because the AAFDictionary is still considered "axiomatic" by
// ImplAAFBuiltinClasses and itself. The problem is that the
// class definitions for such axiomatic objects are unpersisted
// but NOT used by the DM, the "builtin" class definition is used instead!
// 2000-SEPT-14 transdel.
#ifndef SUPPORT_EXPERIMENTAL_OPTIONAL_SETS
#define SUPPORT_EXPERIMENTAL_OPTIONAL_SETS 0
#endif



#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"

#include <iostream.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IUnknown>                 IUnknownSP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFDefObject>            IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>       IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFMob>                  IAAFMobSP;
typedef IAAFSmartPointer<IAAFObject>               IAAFObjectSP;
typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>        IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>     IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef>  IAAFTypeDefStrongObjRefSP;
typedef IAAFSmartPointer<IAAFTypeDefSet>           IAAFTypeDefSetSP;
typedef IAAFSmartPointer<IAAFDataDef>              IAAFDataDefSP;
typedef IAAFSmartPointer<IEnumAAFDataDefs>         IEnumAAFDataDefsSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues>   IEnumAAFPropertyValuesSP;



struct MyDefRecord
{
  MyDefRecord(aafUID_constref xid,
                aafCharacter_constptr xname,
                aafCharacter_constptr xdescription,
                bool xremove) :
    id(xid),
    name(xname),
    description(xdescription),
    remove(xremove)
  {}

  aafUID_constref id;
  aafCharacter_constptr name;
  aafCharacter_constptr description;
  bool remove;
};


//
// Test data for modidication of existing set of DataDefinitions.
//

// {ed74d481-88b2-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID1 = 
{ 0xed74d481, 0x88b2, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {6d02a381-88b3-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID2 = 
{ 0x6d02a381, 0x88b3, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {8a377201-88b3-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID3 = 
{ 0x8a377201, 0x88b3, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {08007b01-88be-11d4-a812-8a896f177d33}
static const aafUID_t kDataDefID4 = 
{ 0x08007b01, 0x88be, 0x11d4, { 0xa8, 0x12, 0x8a, 0x89, 0x6f, 0x17, 0x7d, 0x33 } };

// {587ad701-88c4-11d4-a812-8c4af597f4a3}
static const aafUID_t kDataDefID5 = 
{ 0x587ad701, 0x88c4, 0x11d4, { 0xa8, 0x12, 0x8c, 0x4a, 0xf5, 0x97, 0xf4, 0xa3 } };

static const MyDefRecord kDDTestData[] = 
{
  
  MyDefRecord( kDataDefID1,
    L"Data Def 1",
    L"Fake DataDefinition 1 created for testing sets",
    true
  ),
  
  MyDefRecord( kDataDefID2,
    L"Data Def 2",
    L"Fake DataDefinition 2 created for testing sets",
    false
  ),
  
  MyDefRecord( kDataDefID3,
    L"Data Def 3",
    L"Fake DataDefinition 3 created for testing sets",
    true
  ),
  
  MyDefRecord( kDataDefID4,
    L"Data Def 4",
    L"Fake DataDefinition 4 created for testing sets",
    false
  ),
  
  MyDefRecord( kDataDefID5,
    L"Data Def 5",
    L"Fake DataDefinition 5 created for testing sets",
    true
  )  
};

static const aafUInt32 kDataDefTestDataCount = sizeof(kDDTestData) / sizeof(MyDefRecord);



//
// Test data for creating an new set of definitions in the dictionary.
//

// {afcd8582-895e-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefClassID = 
{ 0xafcd8582, 0x895e, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyClassDefData =
  MyDefRecord( kMyDefClassID,
      L"My Def Class",
      L"My Def Class Defintion created for testing sets",
      false
    );

// {1904c702-8966-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefStrongRefTypeID = 
{ 0x1904c702, 0x8966, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefTypeData =
  MyDefRecord( kMyDefStrongRefTypeID,
      L"StrongRefernce<My Def>",
      L"Strong reference to MyDefs created for testing sets",
      false
    );


// {3cc80d02-8966-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefStrongRefSetTypeID = 
{ 0x3cc80d02, 0x8966, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefSetTypeData =
  MyDefRecord( kMyDefStrongRefSetTypeID,
      L"StrongRefernceSet<My Def>",
      L"Strong reference set to MyDefs created for testing sets",
      false
    );


// {48afd102-895e-11d4-a812-8b94a917bb03}
static const aafUID_t kMyPropID_Dictionary_MyDefinitions = 
{ 0x48afd102, 0x895e, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefSetPropData =
  MyDefRecord( kMyPropID_Dictionary_MyDefinitions,
      L"MyDefinitions",
      L"StrongReferenceSetProperty<MyDef>",
      false
    );


// {54e95d82-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID1 = 
{ 0x54e95d82, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {6468a682-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID2 = 
{ 0x6468a682, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {7ea28482-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID3 = 
{ 0x7ea28482, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {cd501e82-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID4 = 
{ 0xcd501e82, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {df31c182-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID5 = 
{ 0xdf31c182, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {ece74702-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID6 = 
{ 0xece74702, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {1607d782-8962-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID7 = 
{ 0x1607d782, 0x8962, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };


static const MyDefRecord kMyDefTestData[] = 
{
  
  MyDefRecord( kMyDefID1,
    L"My Def 1",
    L"My Definition 1 created for testing sets",
    true
  ),
  
  MyDefRecord( kMyDefID2,
    L"My Def 2",
    L"My Definition 2 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID3,
    L"My Def 3",
    L"My Definition 3 created for testing sets",
    true
  ),
  
  MyDefRecord( kMyDefID4,
    L"My Def 4",
    L"My Definition 4 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID5,
    L"My Def 5",
    L"My Definition 5 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID6,
    L"My Def 6",
    L"My Definition 6 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID7,
    L"My Def 7",
    L"My Definition 7 created for testing sets",
    true
  )    
};

static const aafUInt32 kMyDefTestDataCount = sizeof(kMyDefTestData) / sizeof(MyDefRecord);


// forward declarations and prototypes
extern "C"
{
  HRESULT CAAFTypeDefSet_test(testMode_t);
  void CAAFTypeDefSet_Create(aafCharacter_constptr fileName);
  void CAAFTypeDefSet_Open(aafCharacter_constptr fileName);
  
  void CAAFTypeDefSet_Register(IAAFHeader * pHeader, IAAFDictionary* pDictionary);
  void CAAFTypeDefSet_Write(IAAFHeader* pHeader, IAAFDictionary* pDictionary);
  void CAAFTypeDefSet_Read(IAAFHeader* pHeader, IAAFDictionary* pDictionary);
} 

extern "C" HRESULT CAAFTypeDefSet_test(testMode_t mode)
{
  HRESULT hr = S_OK;
  aafCharacter_constptr pFileName = L"AAFTypeDefSetTest.aaf";
  
  try
  {
    if(mode == kAAFUnitTestReadWrite)
		  CAAFTypeDefSet_Create(pFileName);
    CAAFTypeDefSet_Open(pFileName);
  }
  catch (HRESULT& rhr)
  {
    hr = rhr;
  }

  if (SUCCEEDED(hr))
  {
    hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
  }

  return hr;
}




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

// convenient error handlers.
/*inline*/ static void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
/*inline*/ static void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

static bool EqualObject(IUnknown* pObject1, IUnknown* pObject2)
{
  IUnknownSP pUnknown1, pUnknown2;
  checkResult(pObject1->QueryInterface(IID_IUnknown, (void**)&pUnknown1));
  checkResult(pObject2->QueryInterface(IID_IUnknown, (void**)&pUnknown2));
  if (pUnknown1 == pUnknown2)
    return true;
  else
    return false;
}


void CAAFTypeDefSet_Create(aafCharacter_constptr fileName)
{
  HRESULT            hr = AAFRESULT_SUCCESS;
  aafProductIdentification_t  ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDefSet Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;
  
  // Cleanup the old test file...
  RemoveTestFile(fileName);
  
  checkResult(AAFFileOpenNewModify(fileName, 0, &ProductInfo, &pFile));
  try
  {
    checkResult(pFile->GetHeader(&pHeader));
    checkResult(pHeader->GetDictionary(&pDictionary));
    
    CAAFTypeDefSet_Register(pHeader, pDictionary);
    CAAFTypeDefSet_Write(pHeader, pDictionary);
    CAAFTypeDefSet_Read(pHeader, pDictionary);
    
    checkResult(pFile->Save());
    checkResult(pFile->Close());
  }
  catch (...)
  {
    pFile->Save(); // ignore result
    pFile->Close(); // ignore result
    throw;
  }
}


void CAAFTypeDefSet_Open(aafCharacter_constptr fileName)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;

  checkResult(AAFFileOpenExistingRead(fileName, 0, &pFile));
  try
  {
    checkResult(pFile->GetHeader(&pHeader));
    checkResult(pHeader->GetDictionary(&pDictionary));
    
    CAAFTypeDefSet_Read(pHeader, pDictionary);
    
    checkResult(pFile->Close());
  }
  catch (...)
  {
    pFile->Close(); // ignore result
    throw;
  }
}




void CAAFTypeDefSet_Register(IAAFHeader * pHeader, IAAFDictionary* pDictionary)
{
  aafProductVersion_t toolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, NULL));

  //
  // Create and register a new subclass of AAFDefObject.
  //
  IAAFClassDefSP pDefObjectClass, pMyDefClass;
  checkResult(pDictionary->LookupClassDef(AUID_AAFDefObject, &pDefObjectClass));
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFClassDef, IID_IAAFClassDef, (IUnknown**)&pMyDefClass));
  checkResult(pMyDefClass->Initialize(kMyClassDefData.id, pDefObjectClass, kMyClassDefData.name, kAAFTrue /*concrete*/));
  checkResult(pDictionary->RegisterClassDef(pMyDefClass));

  //
  // Now create and register types for a set of my definitions.
  //
  IAAFTypeDefSP pTempType;
  IAAFTypeDefStrongObjRefSP pMyStrongRefType;
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefStrongObjRef, IID_IAAFTypeDefStrongObjRef, (IUnknown**)&pMyStrongRefType));
  checkResult(pMyStrongRefType->Initialize(kMyStrongRefTypeData.id, pDefObjectClass, kMyStrongRefTypeData.name));
  checkResult(pMyStrongRefType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType));
  checkResult(pDictionary->RegisterTypeDef(pTempType));
  
  IAAFTypeDefSetSP pMyStrongRefSetType;
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefSet, IID_IAAFTypeDefSet, (IUnknown**)&pMyStrongRefSetType));
  checkResult(pMyStrongRefSetType->Initialize(kMyStrongRefSetTypeData.id, pTempType, kMyStrongRefSetTypeData.name));
  checkResult(pMyStrongRefSetType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
  checkResult(pDictionary->RegisterTypeDef(pTempType));
  

#if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS
  
  //
  // Attempt to register a new set type as an optional property on the dictionary.
  //
  IAAFClassDefSP pDictionaryClass;
  checkResult(pDictionary->LookupClassDef(AUID_AAFDictionary, &pDictionaryClass));
  IAAFPropertyDefSP pMyDefsSetPropertyDef;
  checkResult(pDictionaryClass->RegisterOptionalPropertyDef(kMyStrongRefSetPropData.id, kMyStrongRefSetPropData.name, pTempType, &pMyDefsSetPropertyDef));
  //kMyStrongRefSetPropData

#endif // #if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS
  
} 


static void CAAFTypeDefSet_GetDefinitionsSet(
  IAAFDictionary* pDictionary,
  aafUID_constref propertyID,
  bool createOptional,
  IAAFTypeDefSet** ppDefinitionsSet,
  IAAFTypeDefObjectRef** ppElementType, 
  IAAFPropertyValue** ppDefinitionsValue)
{    
  // Read selected sets in the dictionary.
  IAAFObjectSP pDictionaryObject;
  checkResult(pDictionary->QueryInterface(IID_IAAFObject, (void**)&pDictionaryObject));
  IAAFClassDefSP pDictionaryClass;
  checkResult(pDictionaryObject->GetDefinition(&pDictionaryClass));

  //
  // Read the data definitions
  //
  IAAFPropertyDefSP pDefinitionsPropertyDef;
  checkResult(pDictionaryClass->LookupPropertyDef(propertyID, &pDefinitionsPropertyDef));
  // Make sure that the type is a set
  IAAFTypeDefSP pDefinitionsType;
  checkResult(pDefinitionsPropertyDef->GetTypeDef(&pDefinitionsType));
  IAAFTypeDefSetSP pDataDefinitionsSet;
  checkResult(pDefinitionsType->QueryInterface(IID_IAAFTypeDefSet, (void**)&pDataDefinitionsSet));

  // If this an optional property?
  aafBoolean_t optional = kAAFFalse;
  checkResult(pDefinitionsPropertyDef->GetIsOptional(&optional));
  
  // Get the property value that represents the set.
  if (kAAFTrue == optional)
  {
    aafBoolean_t present = kAAFFalse;
    checkResult(pDictionaryObject->IsPropertyPresent(pDefinitionsPropertyDef, &present));
    if (kAAFTrue == present)
    {
      checkResult(pDictionaryObject->GetPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
    }
    else
    {
      if (createOptional)
      {
        checkResult(pDictionaryObject->CreateOptionalPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
      }
      else
      {
        checkResult(AAFRESULT_PROP_NOT_PRESENT);
      }
    }
  }
  else
  {
    checkResult(pDictionaryObject->GetPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
  }
  
  // If this value really a set type?
  checkResult((*ppDefinitionsValue)->GetType(&pDefinitionsType));
  checkResult(pDefinitionsType->QueryInterface(IID_IAAFTypeDefSet, (void**)ppDefinitionsSet));
  
  // Validate the types...
  checkExpression(EqualObject(*ppDefinitionsSet, pDataDefinitionsSet), AAFRESULT_TEST_FAILED);
  
  // Get the element type.
  IAAFTypeDefSP pType;
  checkResult(pDataDefinitionsSet->GetElementType(&pType));
  checkResult(pType->QueryInterface(IID_IAAFTypeDefObjectRef, (void**)ppElementType));
}


void CAAFTypeDefSet_Write(IAAFHeader* pHeader, IAAFDictionary* pDictionary)
{
  if (!pHeader || !pDictionary)
    throw AAFRESULT_NULL_PARAM;

  aafProductVersion_t toolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, NULL));
  
  // Get the property value that represents the set of data definitions.
  IAAFTypeDefSetSP pDataDefinitionsSet;
  IAAFTypeDefObjectRefSP pElementType;
  IAAFPropertyValueSP pDataDefinitionsValue;
  CAAFTypeDefSet_GetDefinitionsSet(pDictionary,
                                   kAAFPropID_Dictionary_DataDefinitions,
                                   false, /*createOptional*/
                                   &pDataDefinitionsSet,
                                   &pElementType,
                                   &pDataDefinitionsValue);

  // Save the current count...
  aafUInt32 expectedDefinitionCount = 0;
  aafUInt32 definitionCount = 0;
  checkResult(pDictionary->CountDataDefs(&expectedDefinitionCount));
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);


  //
  // Add new data definitions to the set.
  //
  aafUInt32 index;
  IAAFDataDefSP pDataDef;
  IAAFPropertyValueSP pDataDefValue;
  for (index = 0; index < kDataDefTestDataCount; ++index)
  {
    checkResult(pDictionary->CreateInstance(AUID_AAFDataDef, IID_IAAFDataDef, (IUnknown**)&pDataDef));
    checkResult(pDataDef->Initialize(kDDTestData[index].id, kDDTestData[index].name, kDDTestData[index].description));
    
    // Create the new element and add it to the set.
    checkResult(pElementType->CreateValue(pDataDef, &pDataDefValue));
    checkResult(pDataDefinitionsSet->AddElement(pDataDefinitionsValue, pDataDefValue));
  }

  // Check the count...
  expectedDefinitionCount += kDataDefTestDataCount;
  checkResult(pDictionary->CountDataDefs(&expectedDefinitionCount));
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);

  
  // Now test removal of selected elements.
  aafUInt32 removeCount = 0;
  for (index = 0; index < kDataDefTestDataCount; ++index)
  {
    IAAFPropertyValueSP pKeyValue;
    checkResult(pDataDefinitionsSet->CreateKey((aafDataBuffer_t)&kDDTestData[index].id, sizeof(aafUID_t), &pKeyValue));
  
    // Get the value with the same key from the set.
    checkResult(pDataDefinitionsSet->LookupElement(pDataDefinitionsValue, pKeyValue, &pDataDefValue));
    
    if (kDDTestData[index].remove)
    {
      ++removeCount;    
      checkResult(pDataDefinitionsSet->RemoveElement(pDataDefinitionsValue, pDataDefValue));
    }    
  }  

  // Check the count...
  expectedDefinitionCount -= removeCount;
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);


  
#if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS

  // Get the property value that represents the set of the new definitions.
  IAAFTypeDefSetSP pMyDefinitionsSet;
  IAAFPropertyValueSP pMyDefinitionsValue;
  IAAFTypeDefObjectRefSP pMyElementType;
  CAAFTypeDefSet_GetDefinitionsSet(pDictionary,
                                   kMyStrongRefSetPropData.id,
                                   true, /*createOptional*/
                                   &pMyDefinitionsSet,
                                   &pMyElementType,
                                   &pMyDefinitionsValue);

  IAAFDefObjectSP pMyDefObject;
  IAAFPropertyValueSP pMyDefValue;
  for (index = 0; index < kMyDefTestDataCount; ++index)
  {
    checkResult(pDictionary->CreateInstance(kMyDefClassID, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
    checkResult(pMyDefObject->Initialize(kMyDefTestData[index].id, kMyDefTestData[index].name));
    checkResult(pMyDefObject->SetDescription(kMyDefTestData[index].description));
    
    // Create the new element and add it to the set.
    checkResult(pMyElementType->CreateValue(pMyDefObject, &pMyDefValue));
    checkResult(pMyDefinitionsSet->AddElement(pMyDefinitionsValue, pMyDefValue));
  }

#endif // #if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS

  
} // CAAFTypeDefSet_Write


void CAAFTypeDefSet_Read(IAAFHeader* pHeader, IAAFDictionary* pDictionary)
{
  if (!pHeader || !pDictionary)
    throw AAFRESULT_NULL_PARAM;    
  
  aafProductVersion_t toolkitVersion, fileToolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, &fileToolkitVersion));

  // Get the property value that represents the set of data definitions.
  IAAFTypeDefSetSP pDataDefinitionsSet;
  IAAFPropertyValueSP pDataDefinitionsValue;
  IAAFTypeDefObjectRefSP pElementType;
  CAAFTypeDefSet_GetDefinitionsSet(pDictionary,
                                   kAAFPropID_Dictionary_DataDefinitions,
                                   false, /*createOptional*/
                                   &pDataDefinitionsSet,
                                   &pElementType,
                                   &pDataDefinitionsValue);

  // Validate the property value set with the public data defintiion access methods
  //

  // Check the count...
  aafUInt32 actualDataDefinitionCount = 0;
  aafUInt32 definitionCount = 0;
  checkResult(pDictionary->CountDataDefs(&actualDataDefinitionCount));
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == actualDataDefinitionCount, AAFRESULT_TEST_FAILED);
  
  // Make sure all of the objects can be found in the set value.
  IEnumAAFDataDefsSP pEnumDataDefs;
  checkResult(pDictionary->GetDataDefs(&pEnumDataDefs));
  IAAFDataDefSP pDataDef, pSetDataDef;
  IAAFDefObjectSP pDefObject;
  IAAFPropertyValueSP pKeyValue;
  IAAFPropertyValueSP pDataDefValue;
  IAAFTypeDefSP pDataDefValueType;
  IAAFTypeDefObjectRefSP pDataDefValueObjectRef;
  aafUID_t id;
  aafBoolean_t containsKey = kAAFFalse;
  while (SUCCEEDED(pEnumDataDefs->NextOne(&pDataDef)))
  {
    checkResult(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObject));
    checkResult(pDefObject->GetAUID(&id));
    checkResult(pDataDefinitionsSet->CreateKey((aafDataBuffer_t)&id, sizeof(id), &pKeyValue));
    
    // Is the key in the set?
    checkResult(pDataDefinitionsSet->ContainsKey(pDataDefinitionsValue, pKeyValue, &containsKey));
    checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
    
    // Get the value with the same key from the set.
    checkResult(pDataDefinitionsSet->LookupElement(pDataDefinitionsValue, pKeyValue, &pDataDefValue));
    
    // The property value's type  must be the same as the elment type of the set!
    checkResult(pDataDefValue->GetType(&pDataDefValueType));
    checkExpression(EqualObject(pDataDefValueType, pElementType), AAFRESULT_TEST_FAILED);

    // Make sure that we actually found the "same" data definition.
    checkResult(pElementType->GetObject(pDataDefValue, IID_IAAFDataDef, (IUnknown**)&pSetDataDef));
    checkExpression(EqualObject(pSetDataDef, pDataDef), AAFRESULT_TEST_FAILED);
  }

  
  // Turn the test around...
  IEnumAAFPropertyValuesSP pEnumDataDefValues;
  checkResult(pDataDefinitionsSet->GetElements(pDataDefinitionsValue, &pEnumDataDefValues));
  while (SUCCEEDED(pEnumDataDefValues->NextOne(&pDataDefValue)))
  {  
    // The property value's type  must be the same as the elment type of the set!
    checkResult(pDataDefValue->GetType(&pDataDefValueType));
    checkExpression(EqualObject(pDataDefValueType, pElementType), AAFRESULT_TEST_FAILED);
    
    // Make sure that we actually found the "same" data definition.
    checkResult(pElementType->GetObject(pDataDefValue, IID_IAAFDataDef, (IUnknown**)&pSetDataDef));
    checkResult(pSetDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObject));
    checkResult(pDefObject->GetAUID(&id));
    
    checkResult(pDictionary->LookupDataDef(id, &pDataDef));
    checkExpression(EqualObject(pSetDataDef, pDataDef), AAFRESULT_TEST_FAILED);
  }  

#if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS

  // Get the property value that represents the set of data definitions.
  IAAFTypeDefSetSP pMyDefinitionsSet;
  IAAFPropertyValueSP pMyDefinitionsValue;
  IAAFTypeDefObjectRefSP pMyElementType;
  CAAFTypeDefSet_GetDefinitionsSet(pDictionary,
                                   kMyStrongRefSetPropData.id,
                                   false, /*createOptional*/
                                   &pMyDefinitionsSet,
                                   &pMyElementType,
                                   &pMyDefinitionsValue);

#endif // #if SUPPORT_EXPERIMENTAL_OPTIONAL_SETS
  
} // CAAFTypeDefSet_Read
