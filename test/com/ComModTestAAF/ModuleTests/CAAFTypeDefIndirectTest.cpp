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

    cout << "     Creating " << aFileName << "..." << endl;
    CAAFTypeDefIndirect_create (wFileName);
//    cout << "Modifying " << aFileName << "..." << endl;
//    CAAFTypeDefIndirect_modify (wFileName);
    cout << "     Reading " << aFileName << "..." << endl;
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
    cout << "     CreateValueFromActualData" << endl; 
    cout << "     GetActualValue" << endl; 
    cout << "     GetActualData" << endl; 
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

aafCharacter_constptr kSequenceAnnotation = L"Sequence - Component Annotation 1";


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


//
static void RegisterIndirectComponentAnnotation (
  IAAFDictionary * pDictionary,
  IAAFTypeDefIndirect ** ppTypeDefIndirect,
  IAAFClassDef ** ppComponentClass,
  IAAFPropertyDef **ppComponentAnnotationPropertyDef)
{
  //
  // Create an and register the new indirect properties.
  //

  // Get type definition for indirect type.
  IAAFTypeDefSP pIndirectType;
  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_Indirect, &pIndirectType));
  checkResult (pIndirectType->QueryInterface (IID_IAAFTypeDefIndirect, (void**)ppTypeDefIndirect));

  // Look up the class definition for components class
  checkResult (pDictionary->LookupClassDef (AUID_AAFComponent, ppComponentClass));

  // Initialize new property
  checkResult ((*ppComponentClass)->RegisterOptionalPropertyDef (
                                        kPropAUID_Component_Annotation1,
                                        L"Indirect Component Annotation 1",
                                        pIndirectType,
                                        ppComponentAnnotationPropertyDef));

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

  // Test 1
  // Create the indirect component annotation property value.
  checkResult (pTypeDefIndirect->CreateValueFromActualData (
                                pActualType,
                                actualData,
                                actualDataSize,
                                &pIndirectValue));

  // Set the value.
  checkResult (pObject->SetPropertyValue (pIndirectPropertyDef, pIndirectValue));
}

// Create the test file.
void CAAFTypeDefIndirect_create (aafCharacter_constptr pFileName)
{
  aafProductIdentification_t	ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFTypeDefIndirectSP pTypeDefIndirect;
  IAAFClassDefSP pComponentClass;
  IAAFPropertyDefSP pComponentAnnotationPropertyDef;
  IAAFMobSP pMob;
  IAAFTimelineMobSlotSP pTimelineMobSlot;
  IAAFMobSlotSP pMobSlot;
  IAAFSequenceSP pSequence;
  IAAFSegmentSP pSequenceSegment;
  IAAFComponentSP pSequenceComponent;


  // Remove the previous test file is one exists
  RemoveTestFile (pFileName);

  // Create the file.
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDefIndirect Test - create";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kAAFVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));
  
  RegisterIndirectComponentAnnotation (pDictionary,
                                       &pTypeDefIndirect, 
                                       &pComponentClass, 
                                       &pComponentAnnotationPropertyDef);

  // Create and composition mob and a sequence with several components.
  // each component will have a different indirect annotion type.
  CAAFBuiltinDefs defs (pDictionary);

  checkResult (defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
  checkResult (pMob->SetName(L"AAFIndirectTypeTest-Mob"));

  checkResult (defs.cdTimelineMobSlot()->CreateInstance(IID_IAAFTimelineMobSlot, (IUnknown**)&pTimelineMobSlot));
  checkResult (pTimelineMobSlot->Initialize ());
  checkResult (pTimelineMobSlot->QueryInterface (IID_IAAFMobSlot, (void**)&pMobSlot));
  checkResult (pMobSlot->SetName (L"AAFIndirectTypeTest-MobSlot"));

  checkResult (defs.cdSequence()->CreateInstance(IID_IAAFSequence, (IUnknown **)&pSequence));
  checkResult (pSequence->Initialize(defs.ddPicture()));

  // 
  // Set a string value into the component annotation property of the new sequence.
  //
  aafUInt32 sequenceAnnotationSize = 2 * (wcslen (kSequenceAnnotation) + 1);
  Test_CreateValueFromActualData (pSequence,
                                  pComponentAnnotationPropertyDef,
                                  defs.tdString (),
                                  sequenceAnnotationSize,
                                  (aafDataBuffer_t)kSequenceAnnotation);

  

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

  checkResult (AAFFileOpenExistingRead (pFileName, 0, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));


  checkResult (pFile->Close ());
}

// Open the test file and modify the data.
void  CAAFTypeDefIndirect_modify (aafCharacter_constptr pFileName)
{
  aafProductIdentification_t	ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDefIndirect Test - modify";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kAAFVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenExistingModify (pFileName, 0, &ProductInfo, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));

 
  checkResult (pFile->Save ());
  checkResult (pFile->Close ());
}
