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

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFSmartPointer.h"
#include "AAFStoredObjectIDs.h"
#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFClassDef> IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFComponent> IAAFComponentSP;
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFFile> IAAFFileSP;
typedef IAAFSmartPointer<IAAFFiller> IAAFFillerSP;
typedef IAAFSmartPointer<IAAFHeader> IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFMetaDefinition> IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFMob> IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot> IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFObject> IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef> IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment> IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence> IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot> IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDef> IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef> IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef> IAAFTypeDefStrongObjRefSP;

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}

inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

// Function to compare COM interface pointers, taken from 
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
	checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
	checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
	if (spUnk1 == spUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;
}

// Function to test if two AUIDs are equal
static aafBoolean_t AreAUIDsEqual(aafUID_t& a1, aafUID_t& a2)
{
	// We cannot simply call memcmp() (or use the == operator), since there is 
	// padding in between the structure fields due to byte alignment, and this 
	// padding does not have to be equal in order for the AUIDs to be equal.
	if(memcmp(&a1.Data1,&a2.Data1,sizeof(a1.Data1)))
		return(kAAFFalse);
	if(memcmp(&a1.Data2,&a2.Data2,sizeof(a1.Data2)))
		return(kAAFFalse);
	if(memcmp(&a1.Data3,&a2.Data3,sizeof(a1.Data3)))
		return(kAAFFalse);
	if(memcmp(&a1.Data4,&a2.Data4,sizeof(a1.Data4)))
		return(kAAFFalse);
	return(kAAFTrue);
}

// Function to test if two class definitions are equivalent, i.e. if they have the
// same AUID.
static aafBoolean_t AreClassDefsEquivalent(IAAFClassDef *pClassDef1,
										   IAAFClassDef *pClassDef2)
{
	IAAFMetaDefinitionSP pMetaDef1,pMetaDef2;
	checkResult(pClassDef1->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pMetaDef1));
	checkResult(pClassDef2->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pMetaDef2));

	aafUID_t FirstAUID,SecondAUID;
	checkResult(pMetaDef1->GetAUID(&FirstAUID));
	checkResult(pMetaDef2->GetAUID(&SecondAUID));

	return(AreAUIDsEqual(FirstAUID,SecondAUID));
}

// These two functions fill in the product version and product info structures,
// respectively, for the AAF files we will create
static void FillInProductVersion(aafProductVersion_t& v)
{
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
}

static void FillInProductInfo(aafProductIdentification_t& ProductInfo,
	aafProductVersion_t& v)
{
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTypeDefObjectRef Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
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

// IDs of objects & types we will create

// AUID of our "StrongReferenceToFiller" type def
static const aafUID_t kTypeID_StrongRefToFiller = 
{ 0x3c317b00, 0x4d33, 0x11d4, { 0x92, 0x25, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// AUID of the optional property of type "StrongReferenceToFiller" we will add to
// AAFSequence
static const aafUID_t AUID_TestProperty = 
{ 0x4af53da0, 0x4e91, 0x11d4, { 0x92, 0x26, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// ID of Mob we will create
static const aafMobID_t	Test_MobID =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0xffd21460, 0x4e92, 0x11d4, 0x92, 0x26, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d} };	

static void CreateTypeDefObjectRefFile(aafWChar *pFilename)
{
	aafProductVersion_t v;
	aafProductIdentification_t	ProductInfo;
	FillInProductVersion(v);
	FillInProductInfo(ProductInfo,v);

	// Remove the previous test file, if any.
	RemoveTestFile(pFilename);

	// Create new AAF file.
	IAAFFileSP pFile;
	checkResult(AAFFileOpenNewModify(pFilename,0,&ProductInfo, &pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	CAAFBuiltinDefs defs(pDictionary);

	// Create a strong object reference
	IAAFTypeDefObjectRefSP pTypeDefObjectRef;
	checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefStrongObjRef,
		IID_IAAFTypeDefObjectRef,(IUnknown**)&pTypeDefObjectRef));

	// Initialize as strong reference to AAFFiller
	IAAFTypeDefStrongObjRefSP pTypeDefStrongObjRef;
	checkResult(pTypeDefObjectRef->QueryInterface(IID_IAAFTypeDefStrongObjRef,
		(void **)&pTypeDefStrongObjRef));
	checkResult(pTypeDefStrongObjRef->Initialize(kTypeID_StrongRefToFiller,
		defs.cdFiller(),L"StrongReferenceToFiller"));

	// Add this type definition to the dictionary
	IAAFTypeDefSP pTypeDef;
	checkResult(pTypeDefObjectRef->QueryInterface(IID_IAAFTypeDef,
		(void **)&pTypeDef));
	checkResult(pDictionary->RegisterTypeDef(pTypeDef));

	// Create a filler
	IAAFFillerSP pFiller;
	checkResult(defs.cdFiller()->CreateInstance(IID_IAAFFiller,
		(IUnknown **)&pFiller));
	checkResult(pFiller->Initialize(defs.ddPicture(),22));
	IAAFObjectSP pObject;
	checkResult(pFiller->QueryInterface(IID_IAAFObject,(void **)&pObject));

	// Now create a property value whose type is strong reference to AAFFiller
	IAAFPropertyValueSP pPropVal;
	pTypeDefObjectRef->CreateValue(pObject,&pPropVal);

	// Make sure GetObject() returns correct object
	IAAFObjectSP pReturnedObject;
	checkResult(pTypeDefObjectRef->GetObject(pPropVal,IID_IAAFObject,
		(IUnknown**)&pReturnedObject));
	checkExpression(AreUnksSame(pObject,pReturnedObject)==kAAFTrue);

	// Make sure GetObjectType() returns correct type
	IAAFClassDefSP pReturnedClassDef;
	checkResult(pTypeDefObjectRef->GetObjectType(&pReturnedClassDef));
	checkExpression(AreClassDefsEquivalent(pReturnedClassDef,defs.cdFiller())
		==kAAFTrue);

	// Create a new filler object
	IAAFFillerSP pNewFiller;
	checkResult(defs.cdFiller()->CreateInstance(IID_IAAFFiller,
		(IUnknown **)&pNewFiller));
	checkResult(pNewFiller->Initialize(defs.ddPicture(),36));
	IAAFObjectSP pNewObject;
	checkResult(pNewFiller->QueryInterface(IID_IAAFObject,(void **)&pNewObject));

	// Make sure SetObject() correctly sets new object
	checkResult(pTypeDefObjectRef->SetObject(pPropVal,pNewObject));

	checkResult(pTypeDefObjectRef->GetObject(pPropVal,IID_IAAFObject,
		(IUnknown**)&pReturnedObject));
	checkExpression(AreUnksSame(pNewObject,pReturnedObject)==kAAFTrue);

	// Make sure GetObjectType() still returns correct type
	checkResult(pTypeDefObjectRef->GetObjectType(&pReturnedClassDef));
	checkExpression(AreClassDefsEquivalent(pReturnedClassDef,defs.cdFiller())
		==kAAFTrue);

	// Now create an optional property that takes our new type, set it, and save
	// it in the file.

	// Add an optional property of type "StrongReferenceToFiller" to AAFSequence.
	IAAFClassDefSP pSequenceClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,
		&pSequenceClassDef));
	IAAFPropertyDefSP pTestProperty;
	checkResult(pSequenceClassDef->RegisterOptionalPropertyDef(AUID_TestProperty,
			L"Test property",
			pTypeDef,
			&pTestProperty));
	checkResult(pDictionary->RegisterClassDef(pSequenceClassDef));

	// Create an instance of IAAFSequence & initialize it
	IAAFSequenceSP pSequence;
	checkResult(pDictionary->CreateInstance(AUID_AAFSequence,IID_IAAFSequence,
		(IUnknown**)&pSequence));
	checkResult(pSequence->Initialize(defs.ddSound()));

	// Set value of our new property to property value created above
	checkResult(pSequence->QueryInterface(IID_IAAFObject,(void**)&pObject));
	checkResult(pObject->SetPropertyValue(pTestProperty,pPropVal));

	// Now add this sequence to a composition Mob, and add the composition Mob
	// to the file header.

	// Create a Composition Mob
	IAAFMobSP pMob;
	checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, 
		(IUnknown **)&pMob));
	checkResult(pMob->SetMobID(Test_MobID));
	checkResult(pMob->SetName(L"TypeDefObjectRefTest"));
  
	// Add timeline Mob slot
	IAAFSegmentSP pSegment;
	checkResult(pSequence->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
	aafRational_t editRate = { 0, 1};
	IAAFTimelineMobSlotSP pTimelineMobSlot;
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1,
		L"AAF Test Sequence",0,&pTimelineMobSlot));

	// Add composition Mob to file
	pHeader->AddMob(pMob);

	// Save & close file
	checkResult(pFile->Save());
	checkResult(pFile->Close());
}

static void ReadTypeDefObjectRefFile(aafWChar *pFilename)
{
	IAAFFileSP pFile;
	checkResult(AAFFileOpenExistingRead(pFilename,0,&pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	CAAFBuiltinDefs defs(pDictionary);

	// Look up "StrongReferenceToFiller" type definition
	IAAFTypeDefSP pTypeDef;
	checkResult(pDictionary->LookupTypeDef(kTypeID_StrongRefToFiller,&pTypeDef));
	IAAFTypeDefObjectRefSP pTypeDefObjectRef;
	checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefObjectRef,
		(void**)&pTypeDefObjectRef));

	// Retrieve the sequence we created

	// Look up Mob
	IAAFMobSP pMob;
	checkResult(pHeader->LookupMob(Test_MobID,&pMob));
	IAAFMobSlotSP pSlot;
	checkResult(pMob->GetSlotAt(0,&pSlot));

	IAAFSegmentSP pSegment;
	checkResult(pSlot->GetSegment(&pSegment));
	IAAFObjectSP pObject;
	checkResult(pSegment->QueryInterface(IID_IAAFObject,(void**)&pObject));

	// Look up our "StrongReferenceToFiller" property definition
	IAAFClassDefSP pClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,&pClassDef));
	IAAFPropertyDefSP pTestProperty;
	checkResult(pClassDef->LookupPropertyDef(AUID_TestProperty,&pTestProperty));

	// Get property value
	IAAFPropertyValueSP pTestPropertyValue;
	checkResult(pObject->GetPropertyValue(pTestProperty,&pTestPropertyValue));

	// Get object referenced by our strong reference property value
	IAAFObjectSP pReturnedObject;
	checkResult(pTypeDefObjectRef->GetObject(pTestPropertyValue,IID_IAAFObject,
		(IUnknown**)&pReturnedObject));

	// Make sure GetObjectType() returns correct class
	IAAFClassDefSP pReturnedClassDef;
	checkResult(pTypeDefObjectRef->GetObjectType(&pReturnedClassDef));
	checkExpression(AreClassDefsEquivalent(pReturnedClassDef,defs.cdFiller())
		==kAAFTrue);

	// Returned object should be an AAFFiller
	IAAFFillerSP pFiller;
	checkResult(pReturnedObject->QueryInterface(IID_IAAFFiller,(void**)&pFiller));

	// Verify filler object
	IAAFComponentSP pComponent;
	checkResult(pFiller->QueryInterface(IID_IAAFComponent,(void**)&pComponent));
	aafLength_t returnedLength;
	checkResult(pComponent->GetLength(&returnedLength));
	checkExpression(returnedLength==36);
	IAAFDataDefSP pDataDef;
	checkResult(pComponent->GetDataDef(&pDataDef));
	// Can't use a smart pointer here, since defs.ddPicture() does not 
	// AcquireReference()
	IAAFDataDef *pExpectedDataDef=defs.ddPicture();
	checkExpression(AreUnksSame(pDataDef,pExpectedDataDef)==kAAFTrue);
	// No need to Release() pExpectedDataDef, since defs.ddPicture() does not
	// AcquireReference()

	pFile->Close();
}

extern "C" HRESULT CAAFTypeDefObjectRef_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDefObjectRef_test(testMode_t mode)
{
	aafWChar *pTestFilename=L"TypeDefObjectRefTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			CreateTypeDefObjectRefFile(pTestFilename);
		ReadTypeDefObjectRefFile(pTestFilename);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}



















