// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefVariableArray
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
#include "AAFStoredObjectIDs.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFClassDef>				IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>			IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>			IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>			IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>					IUnknownSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFTypeDefVariableArray>		IAAFTypeDefVariableArraySP;
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
	
//Variable Array
#define									TEST_VA_NAME	L"VA type Name"
static  aafUID_t						TEST_VA_TYPE_ID =  {0}; //use CoCreateGuid() 

static const aafUInt32					TEST_VA_COUNT =  5;
#define		 TEST_ELEM_t				aafInt16
static const aafUID_t					TEST_ELEM_TYPE_ID =	kAAFTypeID_Int16;
static const TEST_ELEM_t				TEST_VA_VALUES [TEST_VA_COUNT] = {-27, 3000, -50, 94, -8};

// Variable Array Property
#define									TEST_PROP_NAME	L"VA Property Name"
static  aafUID_t						TEST_PROP_ID = {0};  //use CoCreateGuid() 

// Mob id
static  aafMobID_t						TEST_MobID = {0};

// Slot id
static const aafSlotID_t				TEST_SLOT_ID = 7;
#define									TEST_SLOT_NAME	L"Slot containing our VA Segment"


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
	CAAFBuiltinDefs defs(pDict);
	
	//Look up the Type def based on 16-bit signed integer
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//Create a Variable Array
	IAAFTypeDefVariableArraySP spVA;
	checkResult(defs.cdTypeDefVariableArray()->
		CreateInstance (IID_IAAFTypeDefVariableArray, (IUnknown **) &spVA));
	
	//IAAFTypeDefVariableArray::Initialize
	checkResult(CoCreateGuid((GUID *)&TEST_VA_TYPE_ID)); // Could use a better/more accurate UID generator
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
	
	//Create a UID for the Property
	checkResult(CoCreateGuid((GUID *)&TEST_PROP_ID)); // Could use a better/more accurate UID generator
	
	//	Register the Property
	IAAFPropertyDefSP spPropDef;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (TEST_PROP_ID,
		TEST_PROP_NAME,
		spTD,
		&spPropDef));
	
	return S_OK;
}//addVATypeToComponent()


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




static HRESULT verifyContents (IAAFHeader* const pHeader, IAAFDictionary* const pDict,
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
	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );

	//IAAFTypeDefVariableArray::GetCArray()
	aafUInt32 i=0;
	checkResult(spVA->GetCArray(spPropVal, (aafMemPtr_t) check_va, sizeof(check_va)));
	//VERIFY values:
	for (i=0; i<TEST_VA_COUNT; i++)
		checkExpression( check_va[i] == TEST_VA_VALUES[i], AAFRESULT_TEST_FAILED );	

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
	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
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

	const TEST_ELEM_t  newSize_array[7] = {10, -20, 30, -44, 55, -360, 99};
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


	//Append an element, and retrieve/check the value from the array, etc ...

	CAAFBuiltinDefs defs (pDict);

	//first, create a new member, and add it to the array
	IAAFTypeDefIntSP spNewInt;
	checkResult(defs.cdTypeDefInt()->CreateInstance(IID_IAAFTypeDefInt, (IUnknown**)&spNewInt));
	aafUID_t  some_int_id = {0};
	checkResult(CoCreateGuid((GUID *)&some_int_id)); // Could use a better/more accurate UID generator
	checkResult (spNewInt->Initialize (some_int_id, sizeof(TEST_ELEM_t), kAAFTrue, L"Some Int Name"));
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
	checkExpression( AreUnksSame(spTestType, spTD_elem), AAFRESULT_TEST_FAILED );
	//now, test spSomeVal for integer 
	checkResult(spTestType->QueryInterface(IID_IAAFTypeDefInt, (void**)&spSomeInt));
	some_int = -1;
	checkResult(spSomeInt->GetInteger(spSomeVal, (aafMemPtr_t)&some_int, sizeof (some_int)));
	//the returned some_int should be the same as new_int
	checkExpression( some_int == new_int, AAFRESULT_TEST_FAILED );


	// how about some negative testing before we're done ?! ....
	
	hr = spVA->GetElementValue(spPropVal, newCount, &spSomeVal); 
	//0-based index,  so newCount should be out-of-range by 1 element
	checkExpression( hr == AAFRESULT_BADINDEX,    AAFRESULT_TEST_FAILED );

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
		ProductInfo.companyName = L"AAF Developers Desk";
		ProductInfo.productName = L"AAFTypeDefVariableArray Test";
		ProductInfo.productVersion.major = 1;
		ProductInfo.productVersion.minor = 0;
		ProductInfo.productVersion.tertiary = 0;
		ProductInfo.productVersion.patchLevel = 0;
		ProductInfo.productVersion.type = kAAFVersionUnknown;
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
		
		checkResult(createVAType (pDict));
		
		checkResult(addVATypeToComponent (pDict));
		///////////
		
		//Create a mob 
		IAAFMobSP spMob;
		checkResult(defs.cdMob()->
			CreateInstance(IID_IAAFMob, 
			(IUnknown **)&spMob));
		
		checkResult(CoCreateGuid((GUID *)&TEST_MobID)); // Could use a better/more accurate UID generator
		checkResult(spMob->SetMobID(TEST_MobID));
		checkResult(spMob->SetName(L"Some Mob"));
		
		//Add a slot - make it our Filler
		/// first, create  Filler 
		IAAFFillerSP  spFill;
		checkResult(defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown**)&spFill));
		checkResult(spFill->Initialize(defs.ddSound(), 10));
		
		checkResult(createVAFiller(pDict, spFill));
		
		//seg
		//QI the filler for its segment
		IAAFSegmentSP spSeg;
		checkResult(spFill->QueryInterface (IID_IAAFSegment, (void **)&spSeg));
		
		//Create a mob slot				
		IAAFMobSlotSP spMobSlot;
		checkResult(defs.cdMobSlot()->
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
	
	IEnumAAFMobs*				pMobIter = NULL;
	
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

extern "C" HRESULT CAAFTypeDefVariableArray_test()
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFTypeDefVariableArrayTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );

		if (SUCCEEDED(hr))
		{
			hr = AAFRESULT_TEST_PARTIAL_SUCCESS;			
				
			cout << "\t  SetElementValue  method is Not Implemented" << endl;
			cout << "\t  CreateValueFromValues  method is Not Implemented" << endl;
		}//if
		
		
	}//try
	catch (...)
	{
		cerr << "CAAFTypeDefVariableArray_test...Caught general C++"
			" exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}

