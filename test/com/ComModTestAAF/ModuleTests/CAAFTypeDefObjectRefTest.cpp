// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefObjectRef
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
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFClassDef>         IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFComponent>        IAAFComponentSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>   IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFDictionary>       IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>             IAAFFileSP;
typedef IAAFSmartPointer<IAAFFiller>           IAAFFillerSP;
typedef IAAFSmartPointer<IAAFHeader>           IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFMob>              IAAFMobSP;
typedef IAAFSmartPointer<IAAFObject>           IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef>      IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>    IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>          IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef> IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef>
    IAAFTypeDefStrongObjRefSP;
typedef IAAFSmartPointer<IEnumAAFMobs>         IEnumAAFMobsSP;

#include "CAAFBuiltinDefs.h"

//
// TypeID for our new component obj ref typedef
//

// {0248F0CD-7CB6-11d3-8450-00600832ACB8}
static const aafUID_t kTestTypeID_ObjRef = 
{ 0x248f0cd, 0x7cb6, 0x11d3, { 0x84, 0x50, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//
// Property IDs for new comp obj ref props on CompMob
//

// {0248F0CE-7CB6-11d3-8450-00600832ACB8}
static const aafUID_t kTestPropID_CompMob_NewCompProp1 = 
{ 0x248f0ce, 0x7cb6, 0x11d3, { 0x84, 0x50, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {0248F0CF-7CB6-11d3-8450-00600832ACB8}
static const aafUID_t kTestPropID_CompMob_NewCompProp2 = 
{ 0x248f0cf, 0x7cb6, 0x11d3, { 0x84, 0x50, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc68dee88, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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


inline bool EqualGUID (aafUID_t * a,
					   aafUID_t * b)
{
  assert (a);
  assert (b);
  return memcmp (a, b, sizeof (aafUID_t)) ? false : true;
}


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
  aafProductIdentification_t	ProductInfo;
  HRESULT						hr = AAFRESULT_SUCCESS;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFDictionary Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  *ppFile = NULL;

  switch (mode)
	{
	case kAAFMediaOpenReadOnly:
	  hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
	  break;

	case kAAFMediaOpenAppend:
	  hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	  break;

	default:
	  hr = AAFRESULT_TEST_FAILED;
	  break;
	}

  if (FAILED(hr))
	{
	  if (*ppFile)
		{
		  (*ppFile)->Release();
		  *ppFile = NULL;
		}
	  return hr;
	}
  
  hr = (*ppFile)->GetHeader(ppHeader);
  if (FAILED(hr))
	{
	  (*ppFile)->Release();
	  *ppFile = NULL;
	  return hr;
	}
 	
  return hr;
}


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
  IAAFFileSP       pFile;
  HRESULT          hr = S_OK;
	
	
  try
	{  
	  // Remove the previous test file if any.
	  RemoveTestFile (pFileName);
		
	  // Create the AAF file
	  IAAFHeaderSP pHeader;
	  checkResult (OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));
		
	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  IAAFDictionarySP pDictionary;
	  checkResult (pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);

	  //
	  // Create a type def describing a strong object ref to
	  // AAFComponent types.  Register it.
	  //
	  // We could have picked any top of object, but just for grins we
	  // decided to go with components.
	  //

	  // create
	  IAAFTypeDefObjectRefSP tdor;
	  checkResult (pDictionary->CreateMetaInstance(AUID_AAFTypeDefStrongObjRef,
                                                       IID_IAAFTypeDefObjectRef,
                                                       (IUnknown**)&tdor));

	  // get class def for the referenced type.  In this case,
	  // AAFComponent.
	  IAAFClassDefSP cdComp;
	  checkResult (pDictionary->LookupClassDef (AUID_AAFComponent, &cdComp));

	  // init our new type def strong obj ref
	  IAAFTypeDefStrongObjRefSP tdsor;
	  checkResult (tdor->QueryInterface (IID_IAAFTypeDefStrongObjRef,
										  (void **)&tdsor));
	  checkResult (tdsor->Initialize (kTestTypeID_ObjRef,
									  cdComp,
									  L"StrongRefToComponent"));

	  // Blast it into the dictionary.  To do so, we'll need to use
	  // the TypeDef interface.
	  IAAFTypeDefSP td;
	  checkResult (tdor->QueryInterface (IID_IAAFTypeDef,
										 (void **)&td));
	  checkResult (pDictionary->RegisterTypeDef (td));

	  //
	  // It's now ready for use.  Let's try appending two properties
	  // of this type to CompositionMob.
	  //

	  // get the existing class def describing comp mob
	  IAAFClassDefSP cdCompMob;
	  checkResult (pDictionary->
				   LookupClassDef (AUID_AAFCompositionMob, &cdCompMob));

	  // append the new prop defs to it
	  IAAFPropertyDefSP pd1; // remember this for later use
	  checkResult (cdCompMob->RegisterOptionalPropertyDef
				   (kTestPropID_CompMob_NewCompProp1,
					L"NewCompProp1",
					td,
					&pd1));
	  IAAFPropertyDefSP pd2; // remember this for later use
	  checkResult (cdCompMob->RegisterOptionalPropertyDef
				   (kTestPropID_CompMob_NewCompProp2,
					L"NewCompProp2",
					td,
					&pd2));

	  //
	  // Class def describing comp mob is now ready to go with new
	  // properties.  Let's create one and try to add props.
	  //

	  // Create a Composition Mob
	  IAAFMobSP pMob;
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"TestCompMob"));

	  //
	  // Create component objects (in this case, AAFFiller) to put
	  // into our augmented comp mob.  Tell them apart by the
	  // different lengths (13 for #1, 26 for #2).
	  //
	  IAAFFillerSP fill1;
	  checkResult (defs.cdFiller()->
				   CreateInstance(IID_IAAFFiller,
								  (IUnknown **)&fill1));
	  checkResult (fill1->Initialize (defs.ddPictureWithMatte(), 13));
	  IAAFFillerSP fill2;
	  checkResult (defs.cdFiller()->
				   CreateInstance(IID_IAAFFiller,
								  (IUnknown **)&fill2));
	  checkResult (fill2->Initialize (defs.ddPictureWithMatte(), 26));

	  // get the AAFObject interfaces
	  IAAFObjectSP fillObj1;
	  IAAFObjectSP fillObj2;
	  checkResult (fill1->QueryInterface (IID_IAAFObject,
										  (void **)&fillObj1));
	  checkResult (fill2->QueryInterface (IID_IAAFObject,
										  (void **)&fillObj2));

	  // create new property value whose val is the second component
	  IAAFPropertyValueSP pv1;
	  checkResult (tdor->CreateValue (fillObj2, &pv1));

	  // Since we want to test SetObject(), let's now set the value of
	  // pv1 to the first component.
	  checkResult (tdor->SetObject (pv1, fillObj1));

	  // Now, create another new property value whose val is the second component
	  IAAFPropertyValueSP pv2;
	  checkResult (tdor->CreateValue (fillObj2, &pv2));

	  // Set the prop values
	  // First, get the AAFObject ifc for the comp mob
	  IAAFObjectSP compMobObject;
	  checkResult (pMob->QueryInterface (IID_IAAFObject,
										 (void **)&compMobObject));

	  // now set the values
	  checkResult (compMobObject->SetPropertyValue (pd1, pv1));
	  checkResult (compMobObject->SetPropertyValue (pd2, pv2));

	  // CompMob is ready to go.  Pop it into the file.
	  checkResult (pHeader->AddMob(pMob));
	}		
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // Cleanup and return
  if (pFile)
	{
	  pFile->Save();
	  pFile->Close();
	}
  return hr;
}


static HRESULT ReadAAFFile(aafWChar* pFileName)
{
  HRESULT              hr = S_OK;
  IAAFFileSP   file;
	
  try
	{
	  // Open the AAF file
	  IAAFHeaderSP header;
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &file, &header));

	  // Get the dictionary.
	  IAAFDictionarySP dict;
	  checkResult (header->GetDictionary (&dict));

	  // Validate that there is only one composition mob.
	  aafNumSlots_t numMobs = 0;
	  checkResult(header->CountMobs(kAAFCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Get an enumerator for the composition mob.
	  aafSearchCrit_t      criteria;
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;

	  IEnumAAFMobsSP mobEnum;
	  checkResult(header->GetMobs(&criteria, &mobEnum));

	  // Now get the mob.
	  IAAFMobSP mob;
	  checkResult (mobEnum->NextOne(&mob));

	  // Get the class def for the mob.  Have to do it through the
	  // IAAFObject interface.
	  IAAFObjectSP mobObj;
	  checkResult (mob->QueryInterface (IID_IAAFObject,
										(void **)&mobObj));
	  IAAFClassDefSP cdMob;
	  checkResult (mobObj->GetDefinition (&cdMob));

	  // Get the prop defs for our two new props
	  IAAFPropertyDefSP pd1;
	  IAAFPropertyDefSP pd2;
	  checkResult (cdMob->LookupPropertyDef (kTestPropID_CompMob_NewCompProp1,
											 &pd1));
	  checkResult (cdMob->LookupPropertyDef (kTestPropID_CompMob_NewCompProp2,
											 &pd2));

	  // Find the object type for one of them.
	  IAAFTypeDefSP propType;
	  checkResult (pd1->GetTypeDef (&propType));
	  // Verify that it's an obj ref, and that the ref'd object is a
	  // Component.
	  IAAFTypeDefObjectRefSP tdor;
	  checkResult (propType->QueryInterface (IID_IAAFTypeDefObjectRef,
											 (void **)&tdor));
	  IAAFClassDefSP refdObjClass;
	  checkResult (tdor->GetObjectType (&refdObjClass));

	  // Compare this class def with one from the dict, using GUIDs
	  // First, get the referenced object class def
	  IAAFMetaDefinitionSP refdDef;
	  checkResult (refdObjClass->QueryInterface (IID_IAAFMetaDefinition,
												 (void **)&refdDef));
	  aafUID_t refdObjID;
	  checkResult (refdDef->GetAUID (&refdObjID));
	  
	  // now get the expected class from the dictionary 
	  IAAFClassDefSP dictClass;
	  checkResult (dict->LookupClassDef (AUID_AAFComponent,
									  &dictClass));
	  IAAFMetaDefinitionSP dictDef;
	  checkResult (dictClass->QueryInterface (IID_IAAFMetaDefinition,
											  (void **)&dictDef));
	  aafUID_t dictObjID;
	  checkResult (dictDef->GetAUID (&dictObjID));

	  // compare ref'd with dict class ID guids
	  checkExpression (EqualGUID (&refdObjID, &dictObjID), AAFRESULT_TEST_FAILED);
	  
	  //
	  // Now get the objects.  We expect them to be Filler objects,
	  // and to have lengths of 13 and 26.
	  //

	  // first object (len==13)
	  IAAFPropertyValueSP pv1;
	  checkResult (mobObj->GetPropertyValue (pd1, &pv1));
	  IAAFObjectSP fillObj1;
	  checkResult (tdor->GetObject (pv1, IID_IAAFObject, (IUnknown **)&fillObj1));
	  IAAFComponentSP comp1;
	  checkResult (fillObj1->QueryInterface (IID_IAAFComponent,
											 (void **)&comp1));
	  aafLength_t len1;
	  checkResult (comp1->GetLength (&len1));
	  checkExpression (13 == len1, AAFRESULT_TEST_FAILED);

	  // second object (len==26)
	  IAAFPropertyValueSP pv2;
	  checkResult (mobObj->GetPropertyValue (pd2, &pv2));
	  IAAFObjectSP fillObj2;
	  checkResult (tdor->GetObject (pv2, IID_IAAFObject, (IUnknown **)&fillObj2));
	  IAAFComponentSP comp2;
	  checkResult (fillObj2->QueryInterface (IID_IAAFComponent,
											 (void **)&comp2));
	  aafLength_t len2;
	  checkResult (comp2->GetLength (&len2));
	  checkExpression (26 == len2, AAFRESULT_TEST_FAILED);
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // Cleanup and return.
  if (file)
	{
	  file->Close();
	}
  return 	hr;
}

extern "C" HRESULT CAAFTypeDefObjectRef_test();

HRESULT CAAFTypeDefObjectRef_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  aafWChar * pFileName = L"AAFTypeDefObjectRefTest.aaf";

  try
	{
	  hr = CreateAAFFile(pFileName);
	  if (SUCCEEDED(hr))
		hr = ReadAAFFile(pFileName);
	}
  catch (...)
	{
	  cerr << "CAAFTypeDefObjectRef_test..."
		   << "Caught general C++ exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
