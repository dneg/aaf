// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefVariableArray
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
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFClassDef>				IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>			IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>			IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>			IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>					IUnknownSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFTypeDefVariableArray>		IAAFTypeDefVariableArraySP;
typedef IAAFSmartPointer<IAAFTypeDefVariableArrayEx>		IAAFTypeDefVariableArrayExSP;


typedef IAAFSmartPointer<IAAFObject>				IAAFObjectSP;
typedef IAAFSmartPointer<IAAFMob>					IAAFMobSP;
//typedef IAAFSmartPointer<IAAFCompositionMob>		IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFSourceMob>				IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFEssenceDescriptor> IAAFEssenceDescriptorSP;
typedef IAAFSmartPointer<IAAFFileDescriptor>    IAAFFileDescriptorSP;
typedef IAAFSmartPointer<IAAFSourceMob>         IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFFiller>            IAAFFillerSP;
typedef IAAFSmartPointer<IAAFSegment>           IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>   IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IEnumAAFMobSlots>      IEnumAAFMobSlotsSP;
typedef IAAFSmartPointer<IAAFMobSlot>           IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>  IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef>   IAAFTypeDefStrongObjRefSP;
typedef IAAFSmartPointer<IAAFTaggedValue>       IAAFTaggedValueSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>    IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues> IEnumAAFPropertyValuesSP;

//// Our TEST values ...
	
//Variable Array
#define									TEST_VA_NAME	L"VA type Name"
static  const aafUID_t						TEST_VA_TYPE_ID =
{ 0x47240c2e, 0x19d, 0x11d4, { 0x8e, 0x3d, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x7c } };

static const aafUInt32					TEST_VA_COUNT =  5;
#define		 TEST_ELEM_t				aafInt16
static const aafUID_t					TEST_ELEM_TYPE_ID =	kAAFTypeID_Int16;
static const TEST_ELEM_t				TEST_VA_VALUES [TEST_VA_COUNT] = {-27, 3000, -50, 94, -8};

aafUID_t  some_int_id = { 0x14b66cc6, 0x1a1, 0x11d4, { 0x80, 0x46, 0x8, 0x0, 0x36, 0x21, 0x8, 0x4 } };

// Variable Array Property
#define									TEST_PROP_NAME	L"VA Property Name"
static  const aafUID_t						TEST_PROP_ID =
{ 0x47240c2f, 0x19d, 0x11d4, { 0x8e, 0x3d, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x7c } };

// Mob id
static  aafMobID_t						TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xda5ab5f4, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

// Slot id
static const aafSlotID_t				TEST_SLOT_ID = 7;
#define									TEST_SLOT_NAME	L"Slot containing our VA Segment"



static aafCharacter_constptr kTaggedValueStrongReferenceName = L"TaggedValueStrongReference type";
// {da60bb00-ba41-11d4-a812-8e541b972ea3}
static const aafUID_t kTaggedValueStrongReferenceTypeID =
{ 0xda60bb00, 0xba41, 0x11d4, { 0xa8, 0x12, 0x8e, 0x54, 0x1b, 0x97, 0x2e, 0xa3 } };

static aafCharacter_constptr kTaggedValueVariableArrayName = L"TaggedValueVariableArray type";
// {c9fb1100-ba3e-11d4-a812-8e541b972ea3}
static const aafUID_t kTaggedValueVariableArrayTypeID =
{ 0xc9fb1100, 0xba3e, 0x11d4, { 0xa8, 0x12, 0x8e, 0x54, 0x1b, 0x97, 0x2e, 0xa3 } };

static aafCharacter_constptr kComponentAttributePropertyName1 = L"Component Attributes property1";
// {198e0c80-ba40-11d4-a812-8e541b972ea3}
static const aafUID_t kComponentAttributesProperty1ID =
{ 0x198e0c80, 0xba40, 0x11d4, { 0xa8, 0x12, 0x8e, 0x54, 0x1b, 0x97, 0x2e, 0xa3 } };

static aafCharacter_constptr kComponentAttributePropertyName2 = L"Component Attributes property2";
// {198e0c81-ba40-11d4-a812-8e541b972ea3}
static const aafUID_t kComponentAttributesProperty2ID =
{ 0x198e0c81, 0xba40, 0x11d4, { 0xa8, 0x12, 0x8e, 0x54, 0x1b, 0x97, 0x2e, 0xa3 } };

static const aafUInt32 kMaxComponentAttributes = 5;

static aafCharacter_constptr kTaggedValueTag[kMaxComponentAttributes] =
{
  L"TAG 1",
  L"TAG 2",
  L"TAG 3",
  L"TAG 4",
  L"TAG 5"  
};

static aafCharacter_constptr kTaggedValueValue[kMaxComponentAttributes] =
{
  L"VALUE 1",
  L"VALUE 2",
  L"VALUE 3",
  L"VALUE 4",
  L"VALUE 5"  
};
 

// i find this convenient to compare the IUNK's of two interfaces :
template <class T1, class T2>
static aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IUnknownSP    spUnk1, spUnk2;
	
	checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
	checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
	if (spUnk1 == spUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;
}

// i find this macro convenient for "returning" in case of failed expressions
#define stopGap(expr)\
{\
	HRESULT  hr = (expr);\
	if (FAILED(hr))\
	return hr;\
}

// convenient error handlers.
inline void checkResult(const HRESULT r)
{
	if (FAILED(r))
		throw r;
}
inline void checkExpression(const aafBoolean_t expression, const HRESULT r)
{
	if (!expression)
		throw r;
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


static HRESULT  createVAType (IAAFDictionary* const pDict)
{
	//Look up the Type def based on 16-bit signed integer
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//Create a Variable Array
	IAAFTypeDefVariableArraySP spVA;
	checkResult(pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, IID_IAAFTypeDefVariableArray, (IUnknown **) &spVA));
	
	//IAAFTypeDefVariableArray::Initialize
	checkResult(spVA->Initialize(TEST_VA_TYPE_ID, spTD_elem,  TEST_VA_NAME));
	
	//  Register our new VA type def :
	IAAFTypeDefSP spTD_fa;
	checkResult(spVA->QueryInterface(IID_IAAFTypeDef, (void**)&spTD_fa));
	checkResult(pDict->RegisterTypeDef(spTD_fa));
	
	return S_OK;
}//createVAType()


static HRESULT addVATypeToComponent(IAAFDictionary* const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	///////////////////////////////////////////////////////////////////////
	//Look up a Class based on a component? or some meaningful class
	IAAFClassDefSP spSomeClass;
	checkResult(pDict->LookupClassDef (AUID_AAFComponent, &spSomeClass));	
	//OK, lets register (and get) a property def, give it a name + typedef, and
	
	//Lookup the TEST_VA_TYPE_ID we just created!
	IAAFTypeDefSP spTD;
	checkResult(pDict->LookupTypeDef (TEST_VA_TYPE_ID, &spTD));
	
		
	//	Register the Property
	IAAFPropertyDefSP spPropDef;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (TEST_PROP_ID,
		TEST_PROP_NAME,
		spTD,
		&spPropDef));
	
	return S_OK;
}//addVATypeToComponent()


static HRESULT  createTaggedValueStrongReferenceType (IAAFDictionary* const pDict)
{
	IAAFClassDefSP spTargetClass;
	checkResult(pDict->LookupClassDef (AUID_AAFTaggedValue, &spTargetClass));
	
	IAAFTypeDefStrongObjRefSP spTaggedValueStrongReference;
	checkResult(pDict->CreateMetaInstance (AUID_AAFTypeDefStrongObjRef, IID_IAAFTypeDefStrongObjRef, (IUnknown **) &spTaggedValueStrongReference));
	checkResult(spTaggedValueStrongReference->Initialize(kTaggedValueStrongReferenceTypeID, spTargetClass,  kTaggedValueStrongReferenceName));
	
	IAAFTypeDefSP spTD_fa;
	checkResult(spTaggedValueStrongReference->QueryInterface(IID_IAAFTypeDef, (void**)&spTD_fa));
	checkResult(pDict->RegisterTypeDef(spTD_fa));
	
	return S_OK;
}//createVAType()


static HRESULT  createTaggedValueVAType (IAAFDictionary* const pDict)
{
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (kTaggedValueStrongReferenceTypeID, &spTD_elem));
	
	//Create a Variable Array
	IAAFTypeDefVariableArraySP spVA;
	checkResult(pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, IID_IAAFTypeDefVariableArray, (IUnknown **) &spVA));
	
	//IAAFTypeDefVariableArray::Initialize
	checkResult(spVA->Initialize(kTaggedValueVariableArrayTypeID, spTD_elem,  kTaggedValueVariableArrayName));
	
	//  Register our new VA type def :
	IAAFTypeDefSP spTD_fa;
	checkResult(spVA->QueryInterface(IID_IAAFTypeDef, (void**)&spTD_fa));
	checkResult(pDict->RegisterTypeDef(spTD_fa));
	
	return S_OK;
}//createVAType()


static HRESULT addTaggedValueVATypeToComponent(IAAFDictionary* const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	///////////////////////////////////////////////////////////////////////
	//Look up a Class based on a component? or some meaningful class
	IAAFClassDefSP spSomeClass;
	checkResult(pDict->LookupClassDef (AUID_AAFComponent, &spSomeClass));	
	//OK, lets register (and get) a property def, give it a name + typedef, and
	
	//Lookup the TEST_VA_TYPE_ID we just created!
	IAAFTypeDefSP spTD;
	checkResult(pDict->LookupTypeDef (kTaggedValueVariableArrayTypeID, &spTD));
	
		
	//	Register the Properties
	IAAFPropertyDefSP spPropDef1;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (kComponentAttributesProperty1ID,
		kComponentAttributePropertyName1,
		spTD,
		&spPropDef1));

	IAAFPropertyDefSP spPropDef2;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (kComponentAttributesProperty2ID,
		kComponentAttributePropertyName2,
		spTD,
		&spPropDef2));
	
	return S_OK;
}//addTaggedValueVATypeToComponent()


static HRESULT createStringTaggedValue(
  IAAFDictionary* const pDict,
  aafCharacter_constptr tag, 
  aafCharacter_constptr value, 
  IAAFPropertyValue **ppPropertyValue)
{
  assert (pDict && tag && value && ppPropertyValue);
	CAAFBuiltinDefs defs(pDict);
	
	/// first, create  tagged value 
	IAAFTaggedValueSP  spTaggedValue;
	checkResult(defs.cdTaggedValue()->CreateInstance(IID_IAAFTaggedValue, (IUnknown**)&spTaggedValue));
	checkResult(spTaggedValue->Initialize(tag, defs.tdString(), (wcslen(value) + 1) * sizeof(aafCharacter), (aafDataBuffer_t)value));
	
	// Now create the strong reference to tagged value
	IAAFTypeDefSP spType;
	checkResult(pDict->LookupTypeDef(kTaggedValueStrongReferenceTypeID, &spType));
	
	IAAFTypeDefObjectRefSP spObjectReferenceType;
	checkResult(spType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&spObjectReferenceType));
	
	checkResult(spObjectReferenceType->CreateValue(spTaggedValue, ppPropertyValue));
	
	return S_OK;
}

static HRESULT createTaggedValueVAFiller(IAAFDictionary* const pDict, aafUID_constref propertyID, IAAFFillerSP&  spFill)
{
	
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::VA
	IAAFClassDefSP spCD_comp;
	checkResult(pDict->LookupClassDef(AUID_AAFComponent, &spCD_comp));
	
	//From Class Def, get the Property Def
	IAAFPropertyDefSP spPD_comp;
	checkResult(spCD_comp->LookupPropertyDef(propertyID, &spPD_comp));
	
	aafBoolean_t  bIsPresent = kAAFTrue;
	
	//Verify that optional property is NOT yet present in object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFFalse, AAFRESULT_TEST_FAILED);
	
	
	//Now, create a property value .......
	
	//first, get the type def
	//Lookup the VA type
	IAAFTypeDefSP spTypeDef;
	checkResult(pDict->LookupTypeDef(kTaggedValueVariableArrayTypeID, &spTypeDef));
	
	//Get the VA typedef
	IAAFTypeDefVariableArraySP  spVA;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArray, (void**)&spVA));
	
	
	//Set the array up: Create an array of strong references to tagged values...
	IAAFPropertyValueSP spElementPropertyValueArray[kMaxComponentAttributes];
	IAAFPropertyValue * pElementPropertyValueArray[kMaxComponentAttributes]; // copies of pointers "owned by" the smartptr array.
  aafUInt32 i;
  
	for (i = 0; i < kMaxComponentAttributes; i++)
	{
	  IAAFPropertyValueSP spElementPropertyValue;
	  checkResult(createStringTaggedValue(pDict, kTaggedValueTag[i], kTaggedValueValue[i], &spElementPropertyValue));
	  
	  spElementPropertyValueArray[i] = spElementPropertyValue;
	  pElementPropertyValueArray[i] = spElementPropertyValueArray[i];
	}

  
	IAAFPropertyValueSP spArrayPropertyValue;
	
	if (0 == memcmp(&propertyID, &kComponentAttributesProperty1ID, sizeof(aafUID_t)))
	{
	  // Create an empty array and then fill it by appending elements...
	  //
  	checkResult( spVA->CreateEmptyValue (&spArrayPropertyValue) );
  	for (i = 0; i < kMaxComponentAttributes; i++)
  	{	  
  	  checkResult(spVA->AppendElement(spArrayPropertyValue, pElementPropertyValueArray[i]));
  	}
	}
	else if (0 == memcmp(&propertyID, &kComponentAttributesProperty2ID, sizeof(aafUID_t)))
	{
	  // Create a new array from and array of values.
	  //
	  checkResult(spVA->CreateValueFromValues(pElementPropertyValueArray, kMaxComponentAttributes, &spArrayPropertyValue));
	}
	else
	{
	  checkResult(AAFRESULT_INVALID_PARAM);
	}
	
	//Set the value VA to the Object *****************************************
	checkResult(spObj->SetPropertyValue(spPD_comp, spArrayPropertyValue));
	
	//Verify that the optional property is now present in the object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue,  AAFRESULT_TEST_FAILED);
	
	return S_OK;
}//createTaggedValueVAFiller()

static HRESULT verityTaggedValueAttributes(
  IAAFDictionary* const pDict, 
  IAAFObject * pObject, 
  aafUID_constref propertyID, 
  testMode_t testMode)
{
  if (kAAFUnitTestReadWrite == testMode)
  { 
    // Write this test so that it will still work for DR3 & DR4 files.  
    IAAFClassDefSP spComponentClass;
    checkResult(pDict->LookupClassDef(AUID_AAFComponent, &spComponentClass));

    IAAFPropertyDefSP spPropertyDef;
  	checkResult(spComponentClass->LookupPropertyDef(propertyID, &spPropertyDef));

  	// Verify that optional property is present in object
  	aafBoolean_t  bIsPresent = kAAFFalse;
  	checkResult(pObject->IsPropertyPresent(spPropertyDef, &bIsPresent));
  	checkExpression(bIsPresent == kAAFTrue, AAFRESULT_TEST_FAILED);
  	
  	// Get the value...
  	IAAFPropertyValueSP spArrayPropertyValue;
  	checkResult(pObject->GetPropertyValue(spPropertyDef, &spArrayPropertyValue));
  	IAAFTypeDefSP spPropertyValueType;
  	checkResult(spArrayPropertyValue->GetType(&spPropertyValueType));
  	
  	// Make sure that the type is a variable array.
  	IAAFTypeDefVariableArraySP spVA;
  	checkResult(spPropertyValueType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&spVA));
  	
  	IAAFTypeDefSP spElementType;
  	checkResult(spVA->GetType(&spElementType));
  	
  	IAAFTypeDefStrongObjRefSP spStrongObjectReference;
  	checkResult(spElementType->QueryInterface(IID_IAAFTypeDefStrongObjRef, (void **)&spStrongObjectReference));
  	
  	IAAFTypeDefObjectRefSP spObjectReference;
  	checkResult(spStrongObjectReference->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&spObjectReference));
  	
  	IAAFClassDefSP spObjectClass;
  	checkResult(spObjectReference->GetObjectType(&spObjectClass));
  	
  	IAAFMetaDefinitionSP spMetaDefinition;
  	checkResult(spObjectClass->QueryInterface(IID_IAAFMetaDefinition, (void **)&spMetaDefinition));
  	
  	aafUID_t objectClassID;
  	checkResult(spMetaDefinition->GetAUID(&objectClassID));
    checkExpression(0 == memcmp(&objectClassID, &AUID_AAFTaggedValue, sizeof(aafUID_t)), AAFRESULT_TEST_FAILED);
    
    // Check the count.
    aafUInt32 elementCount;
    checkResult(spVA->GetCount(spArrayPropertyValue, &elementCount));
    checkExpression(kMaxComponentAttributes == elementCount, AAFRESULT_TEST_FAILED);
    
    // Check the contents
    IEnumAAFPropertyValuesSP spPropertyValues;
    checkResult(spVA->GetElements(spArrayPropertyValue, &spPropertyValues));
    
    aafCharacter buffer[32]; // must be big enought for sample tags and values.
    aafUInt32 index, bufferSize;;
    for (index = 0; index < elementCount; index++)
    {
      IAAFPropertyValueSP spElementValue;
      checkResult(spPropertyValues->NextOne(&spElementValue));
      
      IAAFTaggedValueSP spTaggedValue;
      checkResult(spObjectReference->GetObject(spElementValue, IID_IAAFTaggedValue, (IUnknown **)&spTaggedValue));
      
      // Validate the tag...
      checkResult(spTaggedValue->GetNameBufLen(&bufferSize));
      checkExpression(sizeof(buffer) >= bufferSize, AAFRESULT_TEST_FAILED);  
      checkResult(spTaggedValue->GetName(buffer, bufferSize));
      checkExpression(0 == wcscmp(buffer, kTaggedValueTag[index]), AAFRESULT_TEST_FAILED);
      
      // Validate the value...
      aafUInt32 bytesRead = 0;
      checkResult(spTaggedValue->GetValueBufLen(&bufferSize));
      checkExpression(sizeof(buffer) >= bufferSize, AAFRESULT_TEST_FAILED);  
      checkResult(spTaggedValue->GetValue(bufferSize, (aafDataBuffer_t)buffer, &bytesRead));
      checkExpression(0 == wcscmp(buffer, kTaggedValueValue[index]), AAFRESULT_TEST_FAILED);
    }
  }
  	
	return S_OK;
}

static HRESULT createVAFiller(IAAFDictionary* const pDict, IAAFFillerSP&  spFill)
{
	
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::VA
	IAAFClassDefSP spCD_comp;
	checkResult(pDict->LookupClassDef(AUID_AAFComponent, &spCD_comp));
	
	//From Class Def, get the Property Def
	IAAFPropertyDefSP spPD_comp;
	checkResult(spCD_comp->LookupPropertyDef(TEST_PROP_ID, &spPD_comp));
	
	aafBoolean_t  bIsPresent = kAAFTrue;
	
	//Verify that optional property is NOT yet present in object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFFalse, AAFRESULT_TEST_FAILED);
	
	
	//Now, create a property value .......
	
	//first, get the type def
	//Lookup the VA type
	IAAFTypeDefSP spTypeDef;
	checkResult(pDict->LookupTypeDef(TEST_VA_TYPE_ID, &spTypeDef));
	
	//Get the VA typedef
	IAAFTypeDefVariableArraySP  spVA;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArray, (void**)&spVA));
	
	
	//Set the array up
	IAAFPropertyValueSP spPropVal;
	checkResult( spVA->CreateValueFromCArray (
		(aafMemPtr_t)  TEST_VA_VALUES,
		sizeof(TEST_VA_VALUES),
		&spPropVal)	);
	
	
	//Set the value VA to the Object *****************************************
	checkResult(spObj->SetPropertyValue(spPD_comp, spPropVal));
	
	//Verify that the optional property is now present in the object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue,  AAFRESULT_TEST_FAILED);
	
	return S_OK;
}//createVAFiller()



static HRESULT verifyContents (testMode_t testMode, IAAFHeader* const pHeader, IAAFDictionary* const pDict,
							   const aafBoolean_t bMinimalTesting)
{

	HRESULT hr;
	
	/////////////////////////////////////////
	//  Check the MOb stuff 
	IAAFMobSP spMob;
	checkResult(pHeader->LookupMob (TEST_MobID, &spMob));
	
	aafNumSlots_t numSlots = 0;
	checkResult(spMob->CountSlots (&numSlots));
	// we only put one in
	assert (1 == numSlots);
	
	IEnumAAFMobSlotsSP spSlotEnum;
	checkResult(spMob->GetSlots (&spSlotEnum));
	
	// Since we only put one in, just bother with the first one.
	IAAFMobSlotSP spMobSlot;
	checkResult(spSlotEnum->NextOne (&spMobSlot));
	
	aafCharacter buf[128] = {0};
	checkResult(spMobSlot->GetName(buf, 128));
	checkExpression( wcscmp(buf, TEST_SLOT_NAME) == 0,   AAFRESULT_TEST_FAILED );	
	
	aafSlotID_t slotid = {0};
	checkResult(spMobSlot->GetSlotID(&slotid));
	checkExpression( slotid == TEST_SLOT_ID,    AAFRESULT_TEST_FAILED );	
	
	
	// Get the segment; it's got to be our filler.
	IAAFSegmentSP spSegment;
	checkResult(spMobSlot->GetSegment (&spSegment));
	// Get filler interface
	IAAFFillerSP spFill;
	checkResult(spSegment->QueryInterface(IID_IAAFFiller,
		(void**)&spFill));
	
	//Make sure Property is preset!  Can't do anything without it
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::VA
	IAAFClassDefSP spCD_comp;
	checkResult(pDict->LookupClassDef(AUID_AAFComponent, &spCD_comp));
	
	//From Class Def, get the Property Def
	IAAFPropertyDefSP spPD_comp;
	checkResult(spCD_comp->LookupPropertyDef(TEST_PROP_ID, &spPD_comp));
	
	//Verify that optional property is present in object
	aafBoolean_t  bIsPresent = kAAFFalse;
	
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue, AAFRESULT_TEST_FAILED);
	
	//////////////////////////////////////
	//get the value
	IAAFPropertyValueSP spPropVal;
	checkResult(spObj->GetPropertyValue(spPD_comp, &spPropVal));
	
	//Get the Variable Array typedef
	//first, get the Type Def from the  Property def
	IAAFTypeDefSP spTypeDef;
	checkResult(spPD_comp->GetTypeDef(&spTypeDef));
	//now get the VA intf
	IAAFTypeDefVariableArraySP spVA;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArray, (void**)&spVA));
	
	//get the array out of it ...
	TEST_ELEM_t	check_va [TEST_VA_COUNT] = {0};  //init a checking variable

	//IAAFTypeDefVariableArray::GetCount()
	aafUInt32 check_count = 0;
	checkResult(spVA->GetCount(spPropVal, &check_count));
	checkExpression( check_count == TEST_VA_COUNT, AAFRESULT_TEST_FAILED );	

	//IAAFTypeDefVariableArray::GetType()
	IAAFTypeDefSP spTestType;
	checkResult(spVA->GetType(&spTestType));
	//Look up our elem Type def 
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
//!!!	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );

	//IAAFTypeDefVariableArray::GetCArray()
	aafUInt32 i=0;
	checkResult(spVA->GetCArray(spPropVal, (aafMemPtr_t) check_va, sizeof(check_va)));
	//VERIFY values:
	for (i=0; i<TEST_VA_COUNT; i++)
		checkExpression( check_va[i] == TEST_VA_VALUES[i], AAFRESULT_TEST_FAILED );
	

  aafProductVersion_t			testRev;
  checkResult(pHeader->GetRefImplVersion(&testRev));
  if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
  { 	
  	//Test see if the variable arrays of tagged values are present.
  	checkResult(verityTaggedValueAttributes(pDict, spObj, kComponentAttributesProperty1ID, testMode));
  	checkResult(verityTaggedValueAttributes(pDict, spObj, kComponentAttributesProperty2ID, testMode));
	}

	//At this point, the test is succesful for both the CREATE and READ (unscrambling of .aaf file) routines
	if (bMinimalTesting)
		// so,  bail if we're called from CREATE 
		return S_OK;

	/////  READ routine .... continue with more tests ....................


	//IAAFTypeDefVariableArray::GetElementValue()
	//Get 3rd index out of array
	aafUInt32  test_index = 2;
	IAAFPropertyValueSP spSomeVal;
	checkResult(spVA->GetElementValue(spPropVal, test_index, &spSomeVal));
	//Make sure both have same types ...
	checkResult(spSomeVal->GetType(&spTestType));
//!!!	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
	//now, test spSomeVal for integer
	IAAFTypeDefIntSP spSomeInt;
	checkResult(spTestType->QueryInterface(IID_IAAFTypeDefInt, (void**)&spSomeInt));
	TEST_ELEM_t some_int = -1;
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	checkExpression( some_int == TEST_VA_VALUES[test_index], AAFRESULT_TEST_FAILED );

	//IAAFTypeDefVariableArray::SetCArray()
	const TEST_ELEM_t  newArray[TEST_VA_COUNT] = {99, -99, 22, -22, 120};
	checkResult(spVA->SetCArray (spPropVal, (aafMemPtr_t) newArray,  sizeof(newArray)));
	//Verify that the new values are set .... 
	// .... call GetCArray
	checkResult(spVA->GetCArray(spPropVal, (aafMemPtr_t) check_va, sizeof(check_va)));
	for (i=0; i<TEST_VA_COUNT; i++)
		checkExpression( check_va[i] == newArray[i], AAFRESULT_TEST_FAILED );	


	//IAAFTypeDefVariableArray::CreateEmptyValue()
	checkResult(spVA->CreateEmptyValue (&spPropVal));
	// Verify that our array is indeed empty
	check_count=999; //init
	checkResult(spVA->GetCount(spPropVal, &check_count));
	//count should be 0 - since array is empty
	checkExpression( check_count == 0, AAFRESULT_TEST_FAILED );


	//Try a NEW Size  array, say 7 elements ....

	const TEST_ELEM_t  newSize_array[7] = {10, -20, 30, -44, 55, 360, 99};
	aafUInt32 newCount = 7;
	checkResult(spVA->SetCArray (spPropVal, (aafMemPtr_t) newSize_array,  sizeof(newSize_array)));
	//the NEW size  should have been set properly at this stage. Lets read it ...
	check_count=0; //init
	checkResult(spVA->GetCount(spPropVal, &check_count));
	checkExpression( check_count == newCount, AAFRESULT_TEST_FAILED );
	//Get the array
	TEST_ELEM_t  check_va2[7] = {0}; //init
	checkResult(spVA->GetCArray(spPropVal, (aafMemPtr_t) check_va2, sizeof(check_va2)));
	//check it
	for (i=0; i<newCount; i++)
		checkExpression( check_va2[i] == newSize_array[i], AAFRESULT_TEST_FAILED );	


    //IAAFTypeDefArray::CreateValueFromValues ()  ...
#define		TEST1_C 9

	const TEST_ELEM_t  newsize_array[TEST1_C] = {1, -2, 3, -4, 5, -6, -7, 8, -9};

	aafUInt32 good_count = TEST1_C;
	IAAFPropertyValue * pSourceValArr[TEST1_C];
	//setup
	for (i=0; i<good_count; i++)
		checkResult(spSomeInt->CreateValue((aafMemPtr_t)&newsize_array[i], 
						sizeof (some_int), &pSourceValArr[i]));
	IAAFPropertyValueSP spTargetValArr;
	checkResult(spVA->CreateValueFromValues(pSourceValArr, good_count, &spTargetValArr ));
	//done with creating Target ValArr;  release Source ValArr elements
	for (i=0; i<good_count; i++)
		pSourceValArr[i]->Release();
	//verify spPropValArr values
	TEST_ELEM_t	check2_fa [TEST1_C] = {0};  
	aafUInt32 small_count = sizeof(TEST_ELEM_t) * (TEST1_C - 2); //say, 2 elements less
	hr = spVA->GetCArray(spTargetValArr, (aafMemPtr_t) check2_fa, small_count);
	//should have failed with wrong size
	checkExpression(AAFRESULT_BAD_SIZE == hr, AAFRESULT_TEST_FAILED);
	//ok, try again, with good-count size	...
	checkResult(spVA->GetCArray(spTargetValArr, (aafMemPtr_t) check2_fa, sizeof(check2_fa)));
	for (i=0; i<good_count; i++)
		checkExpression( check2_fa[i] == newsize_array[i], AAFRESULT_TEST_FAILED );	


	//Test IAAFTypeDefFixedArray::IAAFTypeDefArray::SetElementValue() ....

	some_int = -13;
	checkResult(spSomeInt->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spSomeVal));
	//try to set the value out-of-bounds
	hr = spVA->SetElementValue(spPropVal, 7, spSomeVal); //7 is one elem out of bounds
	checkExpression( hr == AAFRESULT_BADINDEX, AAFRESULT_TEST_FAILED );
	//Set the element to last index ...
	checkResult(spVA->SetElementValue(spPropVal, 6, spSomeVal));  //6 is last index position
	//Now get back the element ...
	checkResult(spVA->GetElementValue(spPropVal, 6, &spSomeVal));
	some_int = 0; //reset value
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	//verify retrieved integer
	checkExpression( -13 == some_int  , AAFRESULT_TEST_FAILED );

	//Append an element, and retrieve/check the value from the array, etc ...

	CAAFBuiltinDefs defs (pDict);

	//first, create a new member, and add it to the array
	IAAFTypeDefIntSP spNewInt;
	checkResult(defs.tdInt16()->QueryInterface(IID_IAAFTypeDefInt, (void **)&spNewInt));
	TEST_ELEM_t  new_int = -115;
	IAAFPropertyValueSP spNewInt_PV;
	checkResult(spNewInt->CreateValue ((aafMemPtr_t)&new_int, sizeof(new_int), &spNewInt_PV));
    //add the new value to the array
	//IAAFTypeDefVariableArray::AppendElement()  test
	checkResult(spVA->AppendElement (spPropVal, spNewInt_PV));
	//  NOW, get back the array.
	newCount++; //increment the gold count with one more element that we just added
	check_count=0; //init
	checkResult(spVA->GetCount(spPropVal, &check_count));
	checkExpression( check_count == newCount, AAFRESULT_TEST_FAILED );
	//  Check the latest element's  value
	checkResult(spVA->GetElementValue(spPropVal, newCount-1, &spSomeVal)); //0 based index
	//Make sure both have same types ...
	checkResult(spSomeVal->GetType(&spTestType));
//!!!	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
	//now, test spSomeVal for integer 
	checkResult(spTestType->QueryInterface(IID_IAAFTypeDefInt, (void**)&spSomeInt));
	some_int = -1;
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	//the returned some_int should be the same as new_int
	checkExpression( some_int == new_int, AAFRESULT_TEST_FAILED );

	//////////////
	//////////////
	////////////////////   PREPEND

	check_count =0;
	spVA->GetCount(spPropVal, &check_count); //get current count

	//Create some value, and Prepend it
	some_int = 123;
	checkResult(spSomeInt->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spSomeVal));
	//now get the VA intf
	IAAFTypeDefVariableArrayExSP spVA_Ex;
	hr = spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArrayEx, (void**)&spVA_Ex);
	if(hr == AAFRESULT_SUCCESS)	//!!!Need a regression switch to enable this
	{
		checkResult(spVA_Ex->PrependElement(spPropVal, spSomeVal)) ;
		//Reset, and then get new count
		good_count = 0;
		checkResult(spVA->GetCount(spPropVal, &good_count)); 
		//Should be one more than previous count.
		checkExpression( good_count == (check_count+1), AAFRESULT_TEST_FAILED );
		
		//////////////
		////////////////////   INSERT
		
		check_count =0;
		spVA->GetCount(spPropVal, &check_count); //get current count
		
		//Create some value, and Insert it somewhere
		some_int = 456;
		checkResult(spSomeInt->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spSomeVal));
		//now get the VA intf
		//checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArrayEx, (void**)&spVA_Ex));
		checkResult(spVA_Ex->InsertElement(spPropVal, 4, spSomeVal)) ;
		//Reset, and then get new count
		good_count = 0;
		checkResult(spVA->GetCount(spPropVal, &good_count)); 
		//Should be one more than previous count.
		checkExpression( good_count == (check_count+1), AAFRESULT_TEST_FAILED );
		
		//////////////
		////////////////////   Remove
		
		check_count =0;
		spVA->GetCount(spPropVal, &check_count); //get current count
		
		//now get the VA intf
		//checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefVariableArrayEx, (void**)&spVA_Ex));
		checkResult(spVA_Ex->RemoveElement(spPropVal, 2)) ;
		//Reset, and then get new count
		good_count = 0;
		checkResult(spVA->GetCount(spPropVal, &good_count)); 
		//Should be one LESS than previous count.
		checkExpression( good_count == (check_count-1), AAFRESULT_TEST_FAILED );
		
	}

	// how about some negative testing before we're done ?! ....
	
	hr = spVA->GetElementValue(spPropVal, good_count+1, &spSomeVal); 
	//out-of-range by 1 element
	checkExpression( hr == AAFRESULT_BADINDEX,    AAFRESULT_TEST_FAILED );

	return S_OK;
	
}//verifyContents()


static HRESULT CreateAAFFile(aafWChar *  pFileName)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	
	aafProductIdentification_t ProductInfo;
	IAAFFile* pFile = NULL;
	IAAFHeader * pHeader = NULL;
	IAAFDictionary * pDict = NULL;
	
	try
	{
	    aafProductVersion_t v;
		v.major = 1;
		v.minor = 0;
		v.tertiary = 0;
		v.patchLevel = 0;
		v.type = kAAFVersionUnknown;

		ProductInfo.companyName = L"AAF Developers Desk";
		ProductInfo.productName = L"AAFTypeDefVariableArray Test";
		ProductInfo.productVersion = &v;
		ProductInfo.productVersionString = NULL;
		ProductInfo.productID = UnitTestProductID;
		ProductInfo.platform = NULL;
		
		//Do the usual ...
		RemoveTestFile (pFileName);
		checkResult (AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		assert (pFile);
		checkResult (pFile->GetHeader (&pHeader));
		assert (pHeader);

    aafProductVersion_t			testRev;
    checkResult(pHeader->GetRefImplVersion(&testRev));

		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		CAAFBuiltinDefs defs (pDict);
		
		///////////////////////
		
		checkResult(createVAType (pDict));
		
		checkResult(addVATypeToComponent (pDict));
		///////////

    if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
    { 	
  		checkResult(createTaggedValueStrongReferenceType (pDict));
  		checkResult(createTaggedValueVAType (pDict));

      checkResult(addTaggedValueVATypeToComponent (pDict));
    }
		///////////
		
		//Create a concrete subclass of mob 
		IAAFMobSP spMob;
		checkResult(defs.cdMasterMob()->
			CreateInstance(IID_IAAFMob, 
			(IUnknown **)&spMob));
		
		checkResult(spMob->SetMobID(TEST_MobID));
		checkResult(spMob->SetName(L"Some Mob"));
		
		//Add a slot - make it our Filler
		/// first, create  Filler 
		IAAFFillerSP  spFill;
		checkResult(defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown**)&spFill));
		checkResult(spFill->Initialize(defs.ddSound(), 10));
		
		checkResult(createVAFiller(pDict, spFill));

    if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
    { 	
      //Add tagged value attributes...
  		checkResult(createTaggedValueVAFiller(pDict, kComponentAttributesProperty1ID, spFill));
  		checkResult(createTaggedValueVAFiller(pDict, kComponentAttributesProperty2ID, spFill));
		}
		
		//seg
		//QI the filler for its segment
		IAAFSegmentSP spSeg;
		checkResult(spFill->QueryInterface (IID_IAAFSegment, (void **)&spSeg));
		
		//Create a concrete subclass of mob slot
		IAAFMobSlotSP spMobSlot;
		checkResult(defs.cdStaticMobSlot()->
			CreateInstance(IID_IAAFMobSlot, 
									   (IUnknown **)&spMobSlot));		
		
		//Add the segment to the mobslot
		checkResult(spMobSlot->SetSegment(spSeg));
		checkResult(spMobSlot->SetSlotID(TEST_SLOT_ID));
		checkResult(spMobSlot->SetName(TEST_SLOT_NAME));
		
		//Append the slot to the Mob ....
		checkResult(spMob->AppendSlot (spMobSlot));
		
		//FINALLY .... Add mob to header
		checkResult (pHeader->AddMob (spMob));
		
		//////////////////// done /!!!!!!!!!!!!!!!!!!!!!!
		
		//Verify results right away (during this creation process) ....
		checkResult(verifyContents (kAAFUnitTestReadWrite, pHeader, pDict, kAAFTrue));  //True => minimal testing 
		
	}
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	if (pDict) pDict->Release();
	if (pHeader) pHeader->Release();
	if (pFile)
	{
		stopGap(pFile->Save());
		stopGap(pFile->Close());
		pFile->Release();
	}
	
	return hr;
}//CreateAAFFile()


static HRESULT  ReadAAFFile(aafWChar *  pFileName, testMode_t testMode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	
	IAAFFile* pFile = NULL;
	IAAFHeader * pHeader = NULL;
	IAAFDictionary * pDict = NULL;
	aafBoolean_t  bFileOpen = kAAFFalse;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = kAAFTrue;
		
		checkResult (pFile->GetHeader (&pHeader));
		assert (pHeader);
		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		
		// Read the mob, slots, etc  to verify the contents ...
		checkResult(verifyContents (testMode, pHeader, pDict, kAAFFalse));  //False => NOT minimal testing; i.e. test everything 
		
		
	}//try
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	if (pDict) pDict->Release();
	if (pHeader) pHeader->Release();
	
	if (bFileOpen && pFile)
	{
		stopGap(pFile->Close());
		pFile->Release();
	}
	
	return hr;
	
}//ReadAAFFile()

extern "C" HRESULT CAAFTypeDefVariableArray_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDefVariableArray_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFTypeDefVariableArrayTest.aaf";
	
	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName, mode);		
		
		if(hr == AAFRESULT_SUCCESS)
			hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}//try
	catch (...)
	{
		cerr << "CAAFTypeDefVariableArray_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	return hr;
}

