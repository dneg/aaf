// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefFixedArray
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>

#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFClassDef>				IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>			IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>			IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>			IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>					IUnknownSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFTypeDefFixedArray>		IAAFTypeDefFixedArraySP;
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

//// Our TEST values ...

//Fixed Array
#define									TEST_FA_NAME	L"FA type Name"
static  const aafUID_t					TEST_FA_TYPE_ID =
{ 0x47240c2c, 0x19d, 0x11d4, { 0x8e, 0x3d, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x7c } };


static const aafUInt32					TEST_FA_COUNT =  5;
#define		 TEST_ELEM_t				aafInt16
static const aafUID_t					TEST_ELEM_TYPE_ID =	kAAFTypeID_Int16;
static const TEST_ELEM_t				TEST_FA_VALUES [TEST_FA_COUNT] = {-27, 3000, -50, 94, -8};

// Fixed Array Property
#define									TEST_PROP_NAME	L"FA Property Name"
static  const aafUID_t					TEST_PROP_ID =
{ 0x47240c2d, 0x19d, 0x11d4, { 0x8e, 0x3d, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x7c } };


// Mob id
static  aafMobID_t						TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xaea357b8, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

// Slot id
static const aafSlotID_t				TEST_SLOT_ID = 7;
#define									TEST_SLOT_NAME	L"Slot containing our FA Segment"


// i find this convenient to compare the IUNK's of two interfaces :
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
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


static HRESULT  createFAType (IAAFDictionary * const pDict)
{
	//Look up the Type def based on 16-bit signed integer
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//Create a Fixed Array
	IAAFTypeDefFixedArraySP spFA;
	checkResult(pDict->CreateMetaInstance (AUID_AAFTypeDefFixedArray, IID_IAAFTypeDefFixedArray, (IUnknown **) &spFA));
	
	//IAAFTypeDefFixedArray::Initialize
	checkResult(spFA->Initialize(TEST_FA_TYPE_ID, spTD_elem, TEST_FA_COUNT, TEST_FA_NAME));
	
	//  Register our new FA type def :
	IAAFTypeDefSP spTD_fa;
	checkResult(spFA->QueryInterface(IID_IAAFTypeDef, (void**)&spTD_fa));
	checkResult(pDict->RegisterTypeDef(spTD_fa));
	
	return S_OK;
}//createFAType()


static HRESULT addFATypeToComponent(IAAFDictionary * const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	///////////////////////////////////////////////////////////////////////
	//Look up a Class based on a component? or some meaningful class
	IAAFClassDefSP spSomeClass;
	checkResult(pDict->LookupClassDef (AUID_AAFComponent, &spSomeClass));	
	//OK, lets register (and get) a property def, give it a name + typedef, and
	
	//Lookup the TEST_FA_TYPE_ID we just created!
	IAAFTypeDefSP spTD;
	checkResult(pDict->LookupTypeDef (TEST_FA_TYPE_ID, &spTD));
	
	//	Register the Property
	IAAFPropertyDefSP spPropDef;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (TEST_PROP_ID,
		TEST_PROP_NAME,
		spTD,
		&spPropDef));
	
	return S_OK;
}//addFATypeToComponent()


static HRESULT createFAFiller(IAAFDictionary* const pDict, IAAFFillerSP& spFill)
{
	
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::FA
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
	//Lookup the FA type
	IAAFTypeDefSP spTypeDef;
	checkResult(pDict->LookupTypeDef(TEST_FA_TYPE_ID, &spTypeDef));
	
	//Get the FA typedef
	IAAFTypeDefFixedArraySP  spFA;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefFixedArray, (void**)&spFA));
	
	
	//Set the array up
	IAAFPropertyValueSP spPropVal;
	checkResult( spFA->CreateValueFromCArray (
		(aafMemPtr_t)  TEST_FA_VALUES,
		sizeof(TEST_FA_VALUES),
		&spPropVal)	);
	
	
	//Set the value FA to the Object *****************************************
	checkResult(spObj->SetPropertyValue(spPD_comp, spPropVal));
	
	//Verify that the optional property is now present in the object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue,  AAFRESULT_TEST_FAILED);
	
	return S_OK;
}//createFAFiller()




static HRESULT verifyContents (IAAFHeader* const pHeader, IAAFDictionary* const pDict,
							   const aafBoolean_t bMinimalTesting)
							   
{
	//CAAFBuiltinDefs defs (pDict);
	
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
	
	//Get the  property def for Component::FA
	IAAFClassDefSP spCD_comp;
	checkResult(pDict->LookupClassDef(AUID_AAFComponent, &spCD_comp));
	
	//From Class Def, get the Property Def
	IAAFPropertyDefSP spPD_comp;
	checkResult(spCD_comp->LookupPropertyDef(TEST_PROP_ID, &spPD_comp));
	
	//Verify that optional property is not yet present in object
	aafBoolean_t  bIsPresent = kAAFFalse;
	
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue, AAFRESULT_TEST_FAILED);
	
	//////////////////////////////////////
	//get the value
	IAAFPropertyValueSP spPropVal;
	checkResult(spObj->GetPropertyValue(spPD_comp, &spPropVal));
	
	//Get the Fixed Array typedef
	//first, get the Type Def from the  Property def
	IAAFTypeDefSP spTypeDef;
	checkResult(spPD_comp->GetTypeDef(&spTypeDef));
	//now get the FA intf
	IAAFTypeDefFixedArraySP spFA;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefFixedArray, (void**)&spFA));
	
	//get the array out of it ...
	TEST_ELEM_t	check_fa [TEST_FA_COUNT] = {0};  //init a checking variable
	
	//IAAFTypeDefFixedArray::GetCount()
	aafUInt32 check_count = 0;
	checkResult(spFA->GetCount(&check_count));
	checkExpression( check_count == TEST_FA_COUNT, AAFRESULT_TEST_FAILED );	
	
	//IAAFTypeDefFixedArray::GetType()
	IAAFTypeDefSP spTestType;
	checkResult(spFA->GetType(&spTestType));
	//Look up our elem Type def 
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
//!!!	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
	
	//IAAFTypeDefFixedArray::GetCArray()
	aafUInt32 i=0;
	checkResult(spFA->GetCArray(spPropVal, (aafMemPtr_t) check_fa, sizeof(check_fa)));
	//VERIFY values:
	for (i=0; i<TEST_FA_COUNT; i++)
		checkExpression( check_fa[i] == TEST_FA_VALUES[i], AAFRESULT_TEST_FAILED );	
	
	
	//At this point, the test is succesful for both the CREATE and READ (unscrambling of .aaf file) routines
	if (bMinimalTesting)
		// so,  bail if we're called from CREATE 
		return S_OK;
	
	/////  READ routine .... continue with more tests ....................
	
	
	//IAAFTypeDefFixedArray::GetElementValue 
	//Get 3rd index out of array
	aafUInt32  test_index = 2;
	IAAFPropertyValueSP spSomeVal;
	checkResult(spFA->GetElementValue(spPropVal, test_index, &spSomeVal));
	//Make sure both have same types ...
	checkResult(spSomeVal->GetType(&spTestType));
//!!!	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
	//now, test spSomeVal for integer
	IAAFTypeDefIntSP spSomeInt;
	checkResult(spTestType->QueryInterface(IID_IAAFTypeDefInt, (void**)&spSomeInt));
	TEST_ELEM_t some_int = -1;
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	checkExpression( some_int == TEST_FA_VALUES[test_index], AAFRESULT_TEST_FAILED );
	
	//IAAFTypeDefFixedArray::SetCArray  
	const TEST_ELEM_t  newArray[TEST_FA_COUNT] = {99, -99, 22, -22, 120};
	checkResult(spFA->SetCArray (spPropVal, (aafMemPtr_t) newArray,  sizeof(newArray)));
	//Verify that the new values are set .... 
	// .... call GetCArray
	checkResult(spFA->GetCArray(spPropVal, (aafMemPtr_t) check_fa, sizeof(check_fa)));
	for (i=0; i<TEST_FA_COUNT; i++)
		checkExpression( check_fa[i] == newArray[i], AAFRESULT_TEST_FAILED );	
	

	//Test IAAFTypeDefFixedArray::IAAFTypeDefArray::SetElementValue() ....

	some_int = -13;
	checkResult(spSomeInt->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spSomeVal));
	//try to set the value out-of-bounds
	hr = spFA->SetElementValue(spPropVal, 5, spSomeVal); //5 is one elem out of bounds
	checkExpression( hr == AAFRESULT_BADINDEX, AAFRESULT_TEST_FAILED );
	//Set the element to last index ...
	checkResult(spFA->SetElementValue(spPropVal, 4, spSomeVal));  //4 is last index position
	//Now get back the element ...
	checkResult(spFA->GetElementValue(spPropVal, 4, &spSomeVal));
	some_int = 0; //reset value
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	//verify retrieved integer
	checkExpression( -13 == some_int  , AAFRESULT_TEST_FAILED );
	
	////
    //IAAFTypeDefArray::CreateValueFromValues ()
	const TEST_ELEM_t  newsize_array[7] = {10, -20, 30, -44, 55, -360, 11};

	aafUInt32 bad_count = 7;
	IAAFPropertyValue * pSourceValArr[7];
	//setup
	for (i=0; i<bad_count; i++)
		checkResult(spSomeInt->CreateValue((aafMemPtr_t)&newsize_array[i], 
						sizeof (some_int), &pSourceValArr[i]));
	IAAFPropertyValueSP spTargetValArr;
	hr = spFA->CreateValueFromValues(pSourceValArr, bad_count, &spTargetValArr );
	checkExpression(AAFRESULT_DATA_SIZE == hr, AAFRESULT_TEST_FAILED);
	//ok try again, with the correct count
	aafUInt32 good_count = 5;
	checkResult(spFA->CreateValueFromValues(pSourceValArr, good_count, &spTargetValArr ));

	//done with creating Target ValArr;  release Source ValArr elements
	for (i=0; i<bad_count; i++)
		pSourceValArr[i]->Release();

	//verify spPropValArr values
	TEST_ELEM_t	check2_fa [5] = {0};  //init a checking variable
	checkResult(spFA->GetCArray(spTargetValArr, (aafMemPtr_t) check2_fa, sizeof(check2_fa)));
	for (i=0; i<good_count; i++)
		checkExpression( check2_fa[i] == newsize_array[i], AAFRESULT_TEST_FAILED );	

	
	//another  negative test ...
	
	//check for bad size request : like, say 6 elements
	const TEST_ELEM_t  badSize_array[TEST_FA_COUNT+1] = {99, -2, 78, -12, 77, -55};
	hr = spFA->SetCArray (spPropVal, (aafMemPtr_t) badSize_array,  sizeof(badSize_array));
	//we should have got back a BAD SIZE!!!! 
	checkExpression( hr == AAFRESULT_BAD_SIZE, AAFRESULT_TEST_FAILED );
	
	
	return S_OK;
	
}//verifyContents()


static HRESULT CreateAAFFile(aafWChar *  pFileName )
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
		ProductInfo.productName = L"AAFTypeDefFixedArray Test";
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
		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		CAAFBuiltinDefs defs (pDict);
		
		///////////////////////
		
		checkResult(createFAType (pDict));
		
		checkResult(addFATypeToComponent (pDict));
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
		
		checkResult(createFAFiller(pDict, spFill));
		
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
		checkResult(verifyContents (pHeader, pDict, kAAFTrue));  //True => minimal testing 
		
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


static HRESULT  ReadAAFFile(aafWChar *  pFileName )
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
		checkResult(verifyContents (pHeader, pDict, kAAFFalse));  //False => NOT minimal testing; i.e. test everything 
		
		
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

extern "C" HRESULT CAAFTypeDefFixedArray_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDefFixedArray_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFTypeDefFixedArrayTest.aaf";
	
	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );		
		
		
		if(hr == AAFRESULT_SUCCESS)
			hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}//try
	catch (...)
	{
		cerr << "CAAFTypeDefFixedArray_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}

