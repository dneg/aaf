// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefIndirect
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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
#include "AAFResult.h"

#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
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
typedef IAAFSmartPointer<IAAFTypeDefIndirect>       IAAFTypeDefIndirectSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>       IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFMobSlot>               IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFSequence>              IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFSegment>               IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFComponent>             IAAFComponentSP;


#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

// Required function prototypes
extern "C"
{
  // Main test function.
  HRESULT CAAFTypeDefIndirect_test(void);

  // Create the test file.
  void CAAFTypeDefIndirect_create (aafCharacter_constptr pFileName); // throw HRESULT

  // Open the test file read only and validate the data.
  void CAAFTypeDefIndirect_read (aafCharacter_constptr pFileName); // throw HRESULT

  // Open the test file and modify the data.
  void CAAFTypeDefIndirect_modify (aafCharacter_constptr pFileName); // throw HRESULT
}


HRESULT CAAFTypeDefIndirect_test()
{
  HRESULT result = AAFRESULT_SUCCESS;
  aafCharacter_constptr wFileName = L"AAFTypeDefIndirectTest.aaf";
  const char *aFileName = "AAFTypeDefIndirectTest.aaf";

  try
  {
    // Run through a basic set of tests. Create the file, modify the file,
    // and then read and validate the modified file.

//    cout << "     Creating " << aFileName << "..." << endl;
    CAAFTypeDefIndirect_create (wFileName);
//    cout << "Modifying " << aFileName << "..." << endl;
//    CAAFTypeDefIndirect_modify (wFileName);
//    cout << "     Reading " << aFileName << "..." << endl;
    CAAFTypeDefIndirect_read (wFileName);
  }
  catch (HRESULT &rhr)
  {
    result = rhr;
  }


  if (SUCCEEDED (result))
  {
    cout << "The following IAAFTypeIndirect methods have not been tested yet:" << endl; 
    cout << "     CreateValueFromActualValue" << endl; 
    cout << "     CreateValueFromActualData  - incomplete" << endl; 
    cout << "     GetActualValue" << endl; 
//    cout << "     GetActualSize" << endl; 
//    cout << "     GetActualType" << endl; 
    cout << "     GetActualData              - incomplete" << endl; 

    // The types of tests that still need to be done are getting and setting
    // data of type definitions other then string including renamed types. 
    // Other tests and the converter have already tested Boolean, UInt32, and 
    // Rational. (transdel 2000-MAR-10)
    result = AAFRESULT_TEST_PARTIAL_SUCCESS;
  }

  return (result);
}


//
// Constants for new class and property definitions
//

// {3FEEC650-F579-11d3-94ED-006097FE2BA5}
static const aafUID_t kPropAUID_Component_Annotation1 = 
{ 0x3feec650, 0xf579, 0x11d3, { 0x94, 0xed, 0x0, 0x60, 0x97, 0xfe, 0x2b, 0xa5 } };
static const aafUID_t kPropAUID_Component_Annotation2 = 
{ 0x3feec651, 0xf579, 0x11d3, { 0x94, 0xed, 0x0, 0x60, 0x97, 0xfe, 0x2b, 0xa5 } };

aafCharacter_constptr kSequenceAnnotation1 = 
  L"Sequence - Component Annotation 1 - as a unicode string";
const char kSequenceAnnotation2[] = 
   "Sequence - Component Annotation 2 - as array of UInt8";


// The test mob id that we added...
static aafMobID_t sTestMob = {0};


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


static void Test_ObjectEquality (IUnknown *obj1, IUnknown *obj2)
{
  // The only way to test object equality in COM is to compare IUnknown
  // interface pointers.
  checkExpression (obj1 && obj2, AAFRESULT_TEST_FAILED);

  IUnknownSP pUnknown1, pUnknown2;
  checkResult (obj1->QueryInterface (IID_IUnknown, (void **)&pUnknown1));
  checkResult (obj2->QueryInterface (IID_IUnknown, (void **)&pUnknown2));
  checkExpression (pUnknown1 == pUnknown2, AAFRESULT_TEST_FAILED);  
}

//
static void Test_RegisterIndirectComponentAnnotations (
  IAAFDictionary * pDictionary)
{
  //
  // Create an and register the new indirect properties.
  //

  // Get type definition for indirect type and verify that the 
  // type is an indirect type.
  IAAFTypeDefSP pIndirectType;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_Indirect, &pIndirectType));
  checkResult (pIndirectType->QueryInterface (IID_IAAFTypeDefIndirect, (void**)&pTypeDefIndirect));

  // Look up the class definition for components class
  IAAFClassDefSP pComponentClass;
  checkResult (pDictionary->LookupClassDef (AUID_AAFComponent, &pComponentClass));

  // Initialize new property
  IAAFPropertyDefSP pComponentAnnotationPropertyDef1;
  checkResult (pComponentClass->RegisterOptionalPropertyDef (
                                  kPropAUID_Component_Annotation1,
                                  L"Indirect Component Annotation 1",
                                  pIndirectType,
                                  &pComponentAnnotationPropertyDef1));

  IAAFPropertyDefSP pComponentAnnotationPropertyDef2;
  checkResult (pComponentClass->RegisterOptionalPropertyDef (
                                  kPropAUID_Component_Annotation2,
                                  L"Indirect Component Annotation 2",
                                  pIndirectType,
                                  &pComponentAnnotationPropertyDef2));
}

static void Test_LookupIndirectComponentAnnotations (
  IAAFDictionary * pDictionary,
  IAAFPropertyDef **ppComponentAnnotationPropertyDef1,
  IAAFPropertyDef **ppComponentAnnotationPropertyDef2)
{
  // Find the class definition for components class
  IAAFClassDefSP pComponentClass;
  checkResult (pDictionary->LookupClassDef (AUID_AAFComponent, &pComponentClass));

  // Find our property definition
  checkResult (pComponentClass->LookupPropertyDef (
                                      kPropAUID_Component_Annotation1,
                                      ppComponentAnnotationPropertyDef1));
  checkResult (pComponentClass->LookupPropertyDef (
                                      kPropAUID_Component_Annotation2,
                                      ppComponentAnnotationPropertyDef2));
}


//
// The type of an indirect property MUST be an indirect type
//
static void Test_GetIndirectTypeFromPropertyDef (
  IAAFPropertyDef * pIndirectPropertyDef,
  IAAFTypeDefIndirect **ppTypeDefIndirect)
{
  IAAFTypeDefSP pIndirectPropertyDefType;
  checkResult (pIndirectPropertyDef->GetTypeDef (&pIndirectPropertyDefType));  
  checkResult (pIndirectPropertyDefType->QueryInterface (IID_IAAFTypeDefIndirect,
                                                         (void**)ppTypeDefIndirect));
}


//
// Test to create and initialize an indirect property with the
// IAAFTypeDefIndirect::CreateValueFromActualData method.
//
static void Test_CreateValueFromActualData (
  IUnknown *pUknownObject,
  IAAFPropertyDef * pIndirectPropertyDef,
  IAAFTypeDef *pActualType,
  aafUInt32 actualDataSize,
  aafDataBuffer_t actualData)
{
  IAAFObjectSP pObject;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  IAAFPropertyValueSP pIndirectValue;

  
  // Use the direct access interface to set the value.
  checkResult (pUknownObject->QueryInterface (IID_IAAFObject, (void**)&pObject));

  Test_GetIndirectTypeFromPropertyDef (pIndirectPropertyDef, &pTypeDefIndirect);

  // Test the method...
  checkResult (pTypeDefIndirect->CreateValueFromActualData (
                                pActualType,
                                actualData,
                                actualDataSize,
                                &pIndirectValue));

  // Set the value.
  checkResult (pObject->SetPropertyValue (pIndirectPropertyDef, pIndirectValue));
}


//
// Test to create and initialize an indirect property with the
// IAAFTypeDefIndirect::GetActualData method.
//
static aafUInt32 Test_GetActualSize (
  IUnknown *pUknownObject,
  IAAFPropertyDef * pIndirectPropertyDef,
  aafUInt32 expectedDataSize)
{
  IAAFObjectSP pObject;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  IAAFPropertyValueSP pIndirectValue;
  aafUInt32 actualDataSize = 0;

  
  // Use the direct access interface to set the value.
  checkResult (pUknownObject->QueryInterface (IID_IAAFObject, (void**)&pObject));

  Test_GetIndirectTypeFromPropertyDef (pIndirectPropertyDef, &pTypeDefIndirect);

  // Set the value.
  checkResult (pObject->GetPropertyValue (pIndirectPropertyDef, &pIndirectValue));

  // Test the method...
  checkResult (pTypeDefIndirect->GetActualSize (
                                pIndirectValue,
                                &actualDataSize));
  checkExpression (actualDataSize == actualDataSize, AAFRESULT_TEST_FAILED);
  
  return (actualDataSize);
}


//
// Test to create and initialize an indirect property with the
// IAAFTypeDefIndirect::GetActualData method.
//
static void Test_GetActualType (
  IUnknown *pUknownObject,
  IAAFPropertyDef * pIndirectPropertyDef,
  IAAFTypeDef ** ppActualType)
{
  IAAFObjectSP pObject;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  IAAFPropertyValueSP pIndirectValue;
  aafUInt32 actualDataSize = 0;

  
  // Use the direct access interface to set the value.
  checkResult (pUknownObject->QueryInterface (IID_IAAFObject, (void**)&pObject));

  Test_GetIndirectTypeFromPropertyDef (pIndirectPropertyDef, &pTypeDefIndirect);

  // Set the value.
  checkResult (pObject->GetPropertyValue (pIndirectPropertyDef, &pIndirectValue));

  // Test the method...
  checkResult (pTypeDefIndirect->GetActualType (
                                pIndirectValue,
                                ppActualType));
}


//
// Test to create and initialize an indirect property with the
// IAAFTypeDefIndirect::GetActualData method.
//
static void Test_GetActualData (
  IUnknown *pUknownObject,
  IAAFPropertyDef * pIndirectPropertyDef,
  IAAFTypeDef *pActualType,
  aafUInt32 actualDataSize,
  aafDataBuffer_t actualData)
{
  IAAFObjectSP pObject;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  IAAFPropertyValueSP pIndirectValue;

  
  // Use the direct access interface to set the value.
  checkResult (pUknownObject->QueryInterface (IID_IAAFObject, (void**)&pObject));

  Test_GetIndirectTypeFromPropertyDef (pIndirectPropertyDef, &pTypeDefIndirect);

  // Set the value.
  checkResult (pObject->GetPropertyValue (pIndirectPropertyDef, &pIndirectValue));

  // Test the method...
  checkResult (pTypeDefIndirect->GetActualData (
                                pIndirectValue,
                                actualData,
                                actualDataSize));

}




// Create the test file.
void CAAFTypeDefIndirect_create (aafCharacter_constptr pFileName)
{
  aafProductIdentification_t	ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFPropertyDefSP pComponentAnnotationPropertyDef1;
  IAAFPropertyDefSP pComponentAnnotationPropertyDef2;
  IAAFMobSP pMob;
  IAAFTimelineMobSlotSP pTimelineMobSlot;
  IAAFMobSlotSP pMobSlot;
  IAAFSequenceSP pSequence;
  IAAFSegmentSP pSequenceSegment;
  IAAFComponentSP pSequenceComponent;


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
  ProductInfo.productName = L"AAFTypeDefIndirect Test - create";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));
  
  Test_RegisterIndirectComponentAnnotations (pDictionary);

  Test_LookupIndirectComponentAnnotations (pDictionary,
             &pComponentAnnotationPropertyDef1,
             &pComponentAnnotationPropertyDef2);


  // Create and composition mob and a sequence with several components.
  // each component will have a different indirect annotion type.
  CAAFBuiltinDefs defs (pDictionary);

  checkResult (defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
  checkResult (pMob->SetName(L"AAFIndirectTypeTest-Mob"));
  // Save the new mob id so that we can just look it up...
  checkResult (pMob->GetMobID (&sTestMob));
  

  checkResult (defs.cdTimelineMobSlot()->CreateInstance(IID_IAAFTimelineMobSlot, (IUnknown**)&pTimelineMobSlot));
  checkResult (pTimelineMobSlot->Initialize ());
  aafRational_t editRate = { 30000, 1001 };
  checkResult (pTimelineMobSlot->SetEditRate (editRate));
  checkResult (pTimelineMobSlot->SetOrigin (0));
  checkResult (pTimelineMobSlot->QueryInterface (IID_IAAFMobSlot, (void**)&pMobSlot));
  checkResult (pMobSlot->SetSlotID (0));
  checkResult (pMobSlot->SetName (L"AAFIndirectTypeTest-MobSlot"));

  checkResult (defs.cdSequence()->CreateInstance(IID_IAAFSequence, (IUnknown **)&pSequence));
  checkResult (pSequence->Initialize(defs.ddPicture()));


  // Test kAAFTypeID_String
  // 
  // Set a string value into the component annotation property of the new sequence.
  //
  aafUInt32 sequenceAnnotation1Size = 2 * (wcslen (kSequenceAnnotation1) + 1);
  Test_CreateValueFromActualData (
          pSequence,
          pComponentAnnotationPropertyDef1,
          defs.tdString (),
          sequenceAnnotation1Size,
          (aafDataBuffer_t)kSequenceAnnotation1);

  // 
  // Get the length of the actual data and check that it matches the expected size.
  //
  Test_GetActualSize (
          pSequence,
          pComponentAnnotationPropertyDef1,
          sequenceAnnotation1Size);

  // 
  // Get the type of the actual data and check that it matches the expected type.
  //
  IAAFTypeDefSP pActualType1;
  Test_GetActualType (
          pSequence,
          pComponentAnnotationPropertyDef1,
          &pActualType1);
  Test_ObjectEquality (pActualType1, defs.tdString ());

  //
  // Get the string that we just wrote to the property (before file is saved) and
  // validate the string.
  //
  assert (sequenceAnnotation1Size <= 128);
  aafCharacter testAnnotation1String[128];
  Test_GetActualData (
          pSequence,
          pComponentAnnotationPropertyDef1,
          pActualType1,
          sequenceAnnotation1Size,
          (aafDataBuffer_t)testAnnotation1String);
  checkExpression (0 == wcscmp (testAnnotation1String, kSequenceAnnotation1),
          AAFRESULT_TEST_FAILED);

  


  // Test kAAFTypeID_UInt8Array
  // 
  // Set an ascii string as an array of unsigned bytes into the 2nd component annotation 
  // property of the new sequence.
  //
  IAAFTypeDefSP annotation2Type;
  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &annotation2Type));
  aafUInt32 sequenceAnnotation2Size = strlen (kSequenceAnnotation2) + 1;
  Test_CreateValueFromActualData (
          pSequence,
          pComponentAnnotationPropertyDef2,
          annotation2Type,
          sequenceAnnotation2Size,
          (aafDataBuffer_t)kSequenceAnnotation2);

  // 
  // Get the length of the actual data and check that it matches the expected size.
  //
  Test_GetActualSize (
          pSequence,
          pComponentAnnotationPropertyDef2,
          sequenceAnnotation2Size);

  // 
  // Get the type of the actual data and check that it matches the expected type.
  //
  IAAFTypeDefSP pActualType2;
  Test_GetActualType (
          pSequence,
          pComponentAnnotationPropertyDef2,
          &pActualType2);
  Test_ObjectEquality (pActualType2, annotation2Type);

  //
  // Get the ascii string that we just wrote as an array of unsigned bytes to the 2nd 
  // property (before file is saved) and validate the string.
  //
  assert (sequenceAnnotation2Size <= 128);
  char testAnnotation2String[128];
  Test_GetActualData (
          pSequence,
          pComponentAnnotationPropertyDef2,
          annotation2Type,
          sequenceAnnotation2Size,
          (aafDataBuffer_t)testAnnotation2String);
  checkExpression (0 == strcmp (testAnnotation2String, kSequenceAnnotation2),
          AAFRESULT_TEST_FAILED);




  checkResult (pSequence->QueryInterface (IID_IAAFSegment, (void**)&pSequenceSegment));
  checkResult (pMobSlot->SetSegment (pSequenceSegment));
  checkResult (pMob->AppendSlot (pMobSlot));
  checkResult (pHeader->AddMob (pMob));


  checkResult (pFile->Save ());
  checkResult (pFile->Close ());
}

// Open the test file read only and validate the data.
void CAAFTypeDefIndirect_read (aafCharacter_constptr pFileName)
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFClassDefSP pComponentClass;
  IAAFPropertyDefSP pComponentAnnotationPropertyDef1;
  IAAFPropertyDefSP pComponentAnnotationPropertyDef2;
  IAAFMobSP pMob;
  IAAFTimelineMobSlotSP pTimelineMobSlot;
  IAAFMobSlotSP pMobSlot;
  IAAFSequenceSP pSequence;
  IAAFSegmentSP pSequenceSegment;
  IAAFComponentSP pSequenceComponent;


  checkResult (AAFFileOpenExistingRead (pFileName, 0, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));


  Test_LookupIndirectComponentAnnotations (pDictionary,
             &pComponentAnnotationPropertyDef1,
             &pComponentAnnotationPropertyDef2);
  
  CAAFBuiltinDefs defs (pDictionary);

  // Find the mob that we added in the create.
  checkResult (pHeader->LookupMob (sTestMob, &pMob));
  checkResult (pMob->GetSlotAt (0, &pMobSlot));
  checkResult (pMobSlot->GetSegment (&pSequenceSegment));
  checkResult (pSequenceSegment->QueryInterface (IID_IAAFSegment, (void**)&pSequence));


  // Test kAAFTypeID_String
  //
  aafUInt32 sequenceAnnotation1Size = 2 * (wcslen (kSequenceAnnotation1) + 1);

  // 
  // Get the length of the actual data and check that it matches the expected size.
  //
  Test_GetActualSize (
          pSequence,
          pComponentAnnotationPropertyDef1,
          sequenceAnnotation1Size);

  // 
  // Get the type of the actual data and check that it matches the expected type.
  //
  IAAFTypeDefSP pActualType1;
  Test_GetActualType (
          pSequence,
          pComponentAnnotationPropertyDef1,
          &pActualType1);
  Test_ObjectEquality (pActualType1, defs.tdString ());

  // Get the string that we just wrote to the property (before file is saved) and
  // validate the string.
  //
  assert (sequenceAnnotation1Size <= 128);
  aafCharacter testAnnotationString[128];
  Test_GetActualData (
          pSequence,
          pComponentAnnotationPropertyDef1,
          pActualType1,
          sequenceAnnotation1Size,
          (aafDataBuffer_t)testAnnotationString);
  checkExpression (0 == wcscmp (testAnnotationString, kSequenceAnnotation1),
          AAFRESULT_TEST_FAILED);


  // Test kAAFTypeID_UInt8Array
  //
  IAAFTypeDefSP annotation2Type;
  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &annotation2Type));

  aafUInt32 sequenceAnnotation2Size = strlen (kSequenceAnnotation2) + 1;
  Test_GetActualSize (
          pSequence,
          pComponentAnnotationPropertyDef2,
          sequenceAnnotation2Size);

  // 
  // Get the type of the actual data and check that it matches the expected type.
  //
  IAAFTypeDefSP pActualType2;
  Test_GetActualType (
          pSequence,
          pComponentAnnotationPropertyDef2,
          &pActualType2);
  Test_ObjectEquality (pActualType2, annotation2Type);

  //
  // Get the ascii string that we just wrote as an array of unsigned bytes to the 2nd 
  // property (before file is saved) and validate the string.
  //
  assert (sequenceAnnotation2Size <= 128);
  char testAnnotation2String[128];
  Test_GetActualData (
          pSequence,
          pComponentAnnotationPropertyDef2,
          annotation2Type,
          sequenceAnnotation2Size,
          (aafDataBuffer_t)testAnnotation2String);
  checkExpression (0 == strcmp (testAnnotation2String, kSequenceAnnotation2),
          AAFRESULT_TEST_FAILED);

  checkResult (pFile->Close ());
}

// Open the test file and modify the data.
void  CAAFTypeDefIndirect_modify (aafCharacter_constptr pFileName)
{
  aafProductIdentification_t	ProductInfo;
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
  ProductInfo.productName = L"AAFTypeDefIndirect Test - modify";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenExistingModify (pFileName, 0, &ProductInfo, &pFile));
//  checkResult (pFile->GetHeader (&pHeader));
//  checkResult (pHeader->GetDictionary (&pDictionary));

 
  checkResult (pFile->Save ());
  checkResult (pFile->Close ());
}
