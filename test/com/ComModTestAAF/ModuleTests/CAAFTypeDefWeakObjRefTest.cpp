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
typedef IAAFSmartPointer<IAAFTypeDefWeakObjRef>     IAAFTypeDefWeakObjRefSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFSequence>              IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFFiller>                IAAFFillerSP;
typedef IAAFSmartPointer<IAAFComponent>             IAAFComponentSP;
typedef IAAFSmartPointer<IAAFSegment>               IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>       IAAFTimelineMobSlotSP;


#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


// Weak references may not be in v1.0...
#ifndef ENABLE_WEAK_REFERENCES
#define ENABLE_WEAK_REFERENCES 1
#endif


// Required function prototypes
extern "C"
{
  // Main test function.
  HRESULT CAAFTypeDefWeakObjRef_test(testMode_t mode);

  // Create the test file.
  void CAAFTypeDefWeakObjRef_create (aafCharacter_constptr pFileName); // throw HRESULT

  // Open the test file read only and validate the data.
  void CAAFTypeDefWeakObjRef_read (aafCharacter_constptr pFileName); // throw HRESULT
}


extern "C" HRESULT CAAFTypeDefWeakObjRef_test(testMode_t);
extern "C" HRESULT CAAFTypeDefWeakObjRef_test(testMode_t mode)
{
#if ENABLE_WEAK_REFERENCES

  HRESULT result = AAFRESULT_SUCCESS;
  aafCharacter_constptr wFileName = L"AAFTypeDefWeakObjRefTest.aaf";
  //  const char *aFileName = "AAFTypeDefWeakObjRefTest.aaf";

  try
  {
    // Run through a basic set of tests. Create the file, 
    // and then read and validate the new file.
     if(mode == kAAFUnitTestReadWrite)
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
//    cout << "     Initialize" << endl;
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

// Constant for "new" weak reference to a type definition.
extern "C" const aafUID_t kAAFPropID_TestWeakReferenceToType = 
{ 0x012345679, 0x0123, 0x0123, {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}};

const aafCharacter kMyWeakReferenceToTypeDefinitionName[] = L"My Weak Reference to Type Definitions";
const aafCharacter kMyWeakReferenceToTypeDefinitionPropertyName[] = L"My Weak Reference to Type Definition Property";


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc68dee89, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


#if 1 //#ifndef _DEBUG
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
#define checkResult(x)\
do {\
  HRESULT r = (x);\
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
  // Remove the previous test file is one exists
  RemoveTestFile (pFileName);

  // Create the file.
  aafProductIdentification_t	ProductInfo;
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


  IAAFFileSP pFile;
  checkResult (AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
  
  try
  {
    IAAFHeaderSP pHeader;
    checkResult (pFile->GetHeader (&pHeader));
    
    IAAFDictionarySP pDictionary;
    checkResult (pHeader->GetDictionary (&pDictionary));
    
    CAAFBuiltinDefs defs (pDictionary);
   
    // Create a Mob
    IAAFTypeDefWeakObjRefSP pWeakObjRef;
    checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                                IID_IAAFTypeDefWeakObjRef,
                                                (IUnknown **)&pWeakObjRef));
    
    // Find the class definition for all type definitions.
    IAAFClassDefSP pClassDef;
    checkResult(pDictionary->LookupClassDef(AUID_AAFTypeDef, &pClassDef));
    

	  aafUID_t targetSet[2];
    targetSet[0] = kAAFPropID_Root_MetaDictionary;
    targetSet[1] = kAAFPropID_MetaDictionary_TypeDefinitions;
    checkResult(pWeakObjRef->Initialize(kAAFTypeID_TestWeakReferenceToType,
                                        pClassDef,
                                        kMyWeakReferenceToTypeDefinitionName,
                                        sizeof(targetSet)/sizeof(aafUID_t),
                                        targetSet));

    // Validate that we make the correct "type" by inspecting the type category.
    IAAFTypeDefSP pTypeDef;
    checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef));

    eAAFTypeCategory_t category;
    checkResult(pTypeDef->GetTypeCategory(&category));
    checkExpression(kAAFTypeCatWeakObjRef == category, AAFRESULT_TEST_FAILED);


  	// Add the new type to the dictionary.
  	checkResult(pDictionary->RegisterTypeDef(pTypeDef));
  	
  	
  	// Now add a new optional weak reference property to an existing class.
  	IAAFClassDefSP pFillerClass;
  	IAAFPropertyDefSP pTypePropertyDef;  	
    checkResult(pDictionary->LookupClassDef(AUID_AAFComponent, &pFillerClass));   
    checkResult(pFillerClass->RegisterOptionalPropertyDef(
      kAAFPropID_TestWeakReferenceToType,
      kMyWeakReferenceToTypeDefinitionPropertyName,
      pTypeDef,
      &pTypePropertyDef));
  


  
    //
	  // Create a Composition Mob
	  //
	  IAAFMobSP pMob;
	  checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"TestCompMob"));
	  
	  //
	  // Create a sequence to hold our test fillers components.
	  //
	  IAAFSequenceSP pSequence;
	  checkResult (defs.cdSequence()->CreateInstance(IID_IAAFSequence, (IUnknown **)&pSequence));
	  checkResult (pSequence->Initialize (defs.ddPicture()));

	  
	  IAAFFillerSP pFiller1;
	  checkResult (defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown **)&pFiller1));
	  checkResult (pFiller1->Initialize (defs.ddPicture(), 16));
	
		IAAFFillerSP pFiller2;
	  checkResult (defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown **)&pFiller2));
	  checkResult (pFiller2->Initialize (defs.ddPicture(), 32));


    //
    // Add the initialized fillers to the sequence.
    //
    IAAFComponentSP pComp1;
  	checkResult(pFiller1->QueryInterface(IID_IAAFComponent, (void **)&pComp1));
  	checkResult(pSequence->AppendComponent(pComp1));

    IAAFComponentSP pComp2;
  	checkResult(pFiller2->QueryInterface(IID_IAAFComponent, (void **)&pComp2));	
  	checkResult(pSequence->AppendComponent(pComp2));
  	
  	//
  	// Append the sequence as the segment in a new timeline mob slot.
  	//
  	IAAFSegmentSP pSegment;
  	checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
   	aafRational_t  editRate = {30000, 1001};
   	aafCharacter_constptr pSlotName = L"Slot 1";
   	aafPosition_t  origin = 0;
   	IAAFTimelineMobSlotSP pNewSlot;
   	checkResult(pMob->AppendNewTimelineSlot(editRate, pSegment, 1, pSlotName, origin, &pNewSlot));

    //
    // Add to the set of mobs in the file.
    //
    checkResult(pHeader->AddMob(pMob));

    checkResult(pFile->Save());
    checkResult(pFile->Close());
  }
  catch (HRESULT& rhr)
  {
     pFile->Save();  // This may not be safe???
     pFile->Close();
     throw rhr;
  }
  catch (...)
  {
     pFile->Close();  
     throw;
  }
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





