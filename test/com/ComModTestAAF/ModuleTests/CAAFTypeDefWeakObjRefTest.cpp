// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefWeakObjRef
/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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


#include "AAF.h"
#include "AAFResult.h"

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
typedef IAAFSmartPointer<IAAFTypeDefWeakObjRef>     IAAFTypeDefWeakObjRefSP;


#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


// Weak references may not be in v1.0...
#ifndef ENABLE_WEAK_REFERENCES
#define ENABLE_WEAK_REFERENCES 0
#endif


// Required function prototypes
extern "C"
{
  // Main test function.
  HRESULT CAAFTypeDefWeakObjRef_test(void);

  // Create the test file.
  void CAAFTypeDefWeakObjRef_create (aafCharacter_constptr pFileName); // throw HRESULT

  // Open the test file read only and validate the data.
  void CAAFTypeDefWeakObjRef_read (aafCharacter_constptr pFileName); // throw HRESULT
}


HRESULT CAAFTypeDefWeakObjRef_test()
{
#if ENABLE_WEAK_REFERENCES

  HRESULT result = AAFRESULT_SUCCESS;
  aafCharacter_constptr wFileName = L"AAFTypeDefWeakObjRefTest.aaf";
  //  const char *aFileName = "AAFTypeDefWeakObjRefTest.aaf";

  try
  {
    // Run through a basic set of tests. Create the file, 
    // and then read and validate the new file.
    CAAFTypeDefWeakObjRef_create (wFileName);
    CAAFTypeDefWeakObjRef_read (wFileName);
  }
  catch (HRESULT &rhr)
  {
    result = rhr;
  }

  if (SUCCEEDED (result))
  {
    cout << "The following IAAFTypeWeakObjRef methods have not been tested yet:" << endl;
    cout << "     Initialize" << endl;
    cout << "     GetObjectType" << endl;
    cout << "     GetObject" << endl;
    cout << "     SetObject" << endl;
    cout << "     CreateValue" << endl;
    result = AAFRESULT_TEST_PARTIAL_SUCCESS;
  }
  return result;

#else // #if ENABLE_WEAK_REFERENCES
    
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif // #else // #if ENABLE_WEAK_REFERENCES
}



// Constant for "new" weak reference to a type definition.
extern "C" const aafUID_t kAAFTypeID_TestWeakReferenceToType = 
{ 0x012345678, 0x0123, 0x0123, {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}};

const aafCharacter kMyWeakReferenceToTypeDefinitionName[] = L"My Weak Reference to Type Definitions";

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




// Create the test file.
void CAAFTypeDefWeakObjRef_create (aafCharacter_constptr pFileName) // throw HRESULT
{
  aafProductIdentification_t	ProductInfo;
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFTypeDefWeakObjRefSP pWeakObjRef;
  IAAFClassDefSP pClassDef;
  IAAFTypeDefSP pTypeDef;
  eAAFTypeCategory_t category;
  IAAFPropertyValueSP pPropertyValue;

	aafUID_t targetSet[1];


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
  ProductInfo.productName = L"AAFTypeDefWeakObjRef Test - create";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;


  checkResult (AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));
  
  CAAFBuiltinDefs defs (pDictionary);
 
  // Create a Mob
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                              IID_IAAFTypeDefWeakObjRef,
                                              (IUnknown **)&pWeakObjRef));
  
  // Find the class definition for all type definitions.
  checkResult(pDictionary->LookupClassDef(AUID_AAFTypeDef, &pClassDef));
  

  targetSet[0] = kAAFPropID_MetaDictionary_TypeDefinitions;
#if 0
  checkResult(pWeakObjRef->Initialize(kAAFTypeID_TestWeakReferenceToType,
                                      pClassDef,
                                      kMyWeakReferenceToTypeDefinitionName,
                                      1,
                                      targetSet);
#else
  HRESULT hr = pWeakObjRef->Initialize(kAAFTypeID_TestWeakReferenceToType,
                                      pClassDef,
                                      kMyWeakReferenceToTypeDefinitionName,
                                      1,
                                      targetSet);
 checkResult(hr);
#endif

  // Validate that we make the correct "type" by inspecting the type category.
  checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef));
  checkResult(pTypeDef->GetTypeCategory(&category));
  checkExpression(kAAFTypeCatWeakObjRef == category, AAFRESULT_TEST_FAILED);


	// Add the new type to the dictionary.
	checkResult(pDictionary->RegisterTypeDef(pTypeDef));

  checkResult(pFile->Save());
  checkResult(pFile->Close());
}



// Create the test file.
void CAAFTypeDefWeakObjRef_read (aafCharacter_constptr pFileName) // throw HRESULT
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  IAAFTypeDefWeakObjRefSP pWeakReference;
  IAAFPropertyValueSP pPropertyValue;

  checkResult (AAFFileOpenExistingRead(pFileName, 0, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDictionary));

  CAAFBuiltinDefs defs (pDictionary);
  

  checkResult(pFile->Close());
}





