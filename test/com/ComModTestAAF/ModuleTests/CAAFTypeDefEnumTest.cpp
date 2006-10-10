//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
typedef IAAFSmartPointer<IAAFTypeDefString>		IAAFTypeDefStringSP;
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
//Enum
typedef IAAFSmartPointer<IAAFTypeDefEnum>		IAAFTypeDefEnumSP;

//// Our TEST values ...

//Enum TYPE id
#define									TEST_ENUM_TYPE_NAME	 L"Enum type Name"
static  const aafUID_t					TEST_ENUM_TYPE_ID =
{ 0x96b7ba43, 0x4945, 0x11d4, { 0x8e, 0x57, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };


//Elements in the Enum ...
#define 								TEST_ENUM_COUNT         7
#define									TEST_ELEM_t				aafInt32
static const aafUID_t					TEST_ELEM_TYPE_ID =	kAAFTypeID_Int32; 

//The Enum table

// kAAFFriday is included to verify that CreateValueFromName()
// operates correctly when given a name without the "kAAF" prefix.

#define TEST_KAAF_SYMBOL L"Friday"
#define TEST_NONKAAF_SYMBOL L"Saturday"

static aafString_t				TEST_ENUM_NAMES[TEST_ENUM_COUNT] = 
						{L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday",
						 L"kAAF" TEST_KAAF_SYMBOL, TEST_NONKAAF_SYMBOL };
static aafInt64					TEST_ENUM_VALUES[TEST_ENUM_COUNT] = 
											{-1, 15, -20, 33, -2, 0xaaf };

static const aafString_t				TEST_ENUM_SOME_SYMBOL = TEST_ENUM_NAMES[3];
static const aafInt64					TEST_ENUM_SOME_VALUE = TEST_ENUM_VALUES[3];

// Enum PROPERTY id
#define									TEST_PROP_NAME	L"Enum Property Name"
static  const aafUID_t					TEST_PROP_ID =
{ 0x96b7ba44, 0x4945, 0x11d4, { 0x8e, 0x57, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };

///////////////////////////
// Mob id
static  const aafMobID_t						TEST_MobID = 
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x6cc93ed4, 0x4dfc, 0x11d4, {0x8e, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xcc, 0x26}}};

// Slot id
static const aafSlotID_t				TEST_SLOT_ID = 7;
#define									TEST_SLOT_NAME	L"Slot containing our ENUM Segment"


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

static HRESULT  createENUMType (IAAFDictionary * const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	//Look up the Type def based on 16-bit signed integer
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//Create a String type
	IAAFTypeDefEnumSP spENUM;
	//checkResult(defs.cdTypeDefEnum()->
		//CreateInstance (IID_IAAFTypeDefEnum, (IUnknown **) &spENUM));
	pDict->CreateMetaInstance(AUID_AAFTypeDefEnum, IID_IAAFTypeDefEnum,
		(IUnknown **) &spENUM) ;

	
	//IAAFTypeDefEnum::Initialize
	checkResult(spENUM->Initialize(TEST_ENUM_TYPE_ID, //The Enum Type ID
		spTD_elem,  //type of elements 
		&TEST_ENUM_VALUES[0], //values of elems
		&TEST_ENUM_NAMES[0],  //names of elems
		TEST_ENUM_COUNT,  // the count of elems
		TEST_ENUM_TYPE_NAME)   );  //The Enum TypeName

	//Register size
	//checkResult(spENUM->RegisterSize(sizeof(TEST_ELEM_t)));
	
	//  Register our new ENUM type def :
	IAAFTypeDefSP spTD;
	checkResult(spENUM->QueryInterface(IID_IAAFTypeDef, (void**)&spTD));
	checkResult(pDict->RegisterTypeDef(spTD));
	
	return S_OK;
}//createENUMType()


static HRESULT addENUMTypeToComponent(IAAFDictionary * const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	///////////////////////////////////////////////////////////////////////
	//Look up a Class based on a component? or some meaningful class
	IAAFClassDefSP spSomeClass;
	checkResult(pDict->LookupClassDef (AUID_AAFComponent, &spSomeClass));	
	//OK, lets register (and get) a property def, give it a name + typedef, and
	
	//Lookup the TEST_ENUM_TYPE_ID we just created!
	IAAFTypeDefSP spTD;
	checkResult(pDict->LookupTypeDef (TEST_ENUM_TYPE_ID, &spTD));
	
	//	Register the Property
	IAAFPropertyDefSP spPropDef;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (TEST_PROP_ID,
		TEST_PROP_NAME,
		spTD,
		&spPropDef));
	
	return S_OK;
}//addENUMTypeToComponent()


static HRESULT createENUMFiller(IAAFDictionary* const pDict, IAAFFillerSP& spFill)
{
	
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::ENUM
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
	//Lookup the ENUM type
	IAAFTypeDefSP spTypeDef;
	checkResult(pDict->LookupTypeDef(TEST_ENUM_TYPE_ID, &spTypeDef));
	
	//Get the ENUM typedef
	IAAFTypeDefEnumSP  spENUM;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefEnum, (void**)&spENUM));
	
	
	//Set the array up
	IAAFPropertyValueSP spPropVal;
	checkResult( spENUM->CreateValueFromName (
		TEST_ENUM_SOME_SYMBOL,
		&spPropVal)	);
	
	
	//Set the value ENUM to the Object *****************************************
	checkResult(spObj->SetPropertyValue(spPD_comp, spPropVal));
	
	//Verify that the optional property is now present in the object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue,  AAFRESULT_TEST_FAILED);

	// Test accessing a "kAAF" prefixed name without the "kAAF" prefix.
	IAAFPropertyValueSP spPropValFriday;
	checkResult( spENUM->CreateValueFromName( TEST_KAAF_SYMBOL, &spPropValFriday ) );

	// Test access to a non "kAAF" prefixed name, but, this time
	// with the "kAAF" prefix.
	IAAFPropertyValueSP spPropValSaturday;
	checkResult( spENUM->CreateValueFromName( L"kAAF" TEST_NONKAAF_SYMBOL, &spPropValSaturday ) );
	
	return S_OK;
}//createENUMFiller()




static HRESULT verifyContents (IAAFHeader* const pHeader, IAAFDictionary* const pDict,
							   const aafBoolean_t bMinimalTesting)
							   
{
	//CAAFBuiltinDefs defs (pDict);
	
	HRESULT hr = 0;
	
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
	checkResult(spMobSlot->GetName(buf, 128*sizeof(aafCharacter)));
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
	
	//Get the  property def for Component::ENUM
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
	checkResult(spObj->GetPropertyValue(spPD_comp, &spPropVal)); //Our ENUMING PropVal
	
	//Get the Fixed Array typedef
	//first, get the Type Def from the  Property def
	IAAFTypeDefSP spTypeDef;
	checkResult(spPD_comp->GetTypeDef(&spTypeDef));
	//now get the ENUM intf
	IAAFTypeDefEnumSP spENUM;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefEnum, (void**)&spENUM));
	

	//IAAFTypeDefEnum::GetCount()
	aafUInt32 check_count = 0;
	checkResult(spENUM->CountElements(&check_count));
	checkExpression( check_count == TEST_ENUM_COUNT, AAFRESULT_TEST_FAILED );	
	

	
	//At this point, the test is succesful for both the CREATE and READ (unscrambling of .aaf file) routines
	if (bMinimalTesting)
		// so,  bail if we're called from CREATE 
		return S_OK;
	
	/////  READ routine .... continue with more tests ....................

	aafUInt32 bufsize =  128;
	aafCharacter * somebuf = new aafCharacter [bufsize];
	//Get the Name [BufLen] from the PV
	aafUInt32 actual_len = 0;
	checkResult(spENUM->GetNameBufLenFromValue(spPropVal, &actual_len));
	//try -ve test
	hr = spENUM->GetNameFromValue(spPropVal, somebuf, actual_len-1); 
	//verify -ve result 
	checkExpression( AAFRESULT_SMALLBUF == hr, AAFRESULT_TEST_FAILED );	
	//try +ve test
	checkResult(spENUM->GetNameFromValue(spPropVal, somebuf, actual_len));
	//verify
	checkExpression( wcscmp(TEST_ENUM_SOME_SYMBOL, somebuf) == 0, AAFRESULT_TEST_FAILED );
	
	//GetINtegerValue
	aafInt64 someval=0;
	hr = spENUM->GetIntegerValue(spPropVal, &someval);
	checkExpression( someval == TEST_ENUM_SOME_VALUE, AAFRESULT_TEST_FAILED );
	

	aafUInt32 index = 0;
	while (index < TEST_ENUM_COUNT )
	{
		//GEtElementValue()
		checkResult(spENUM->GetElementValue(index, &someval));
		checkExpression( someval == TEST_ENUM_VALUES[index], AAFRESULT_TEST_FAILED );	

		//GetElementName()
		actual_len = 0; //reset
		checkResult(spENUM->GetElementNameBufLen(index, &actual_len));
		assert(actual_len);
		checkResult(spENUM->GetElementName(index, somebuf, bufsize));
		checkExpression( wcscmp(TEST_ENUM_NAMES[index], somebuf) == 0, AAFRESULT_TEST_FAILED );	

		//GetName [BufLen] - from integer
		actual_len = 0; //reset
		checkResult(spENUM->GetNameBufLenFromInteger(someval, &actual_len));
		assert(actual_len);
		checkResult(spENUM->GetNameFromInteger(someval, somebuf, actual_len));
		checkExpression( wcscmp(TEST_ENUM_NAMES[index], somebuf) == 0, AAFRESULT_TEST_FAILED );	

		index++;
	}//while

	//SEt/Get Integer value

	//try a -ve Set test
	someval = -111;  //illegal value!
	hr = spENUM->SetIntegerValue(spPropVal, someval); //-ve test!
	checkExpression( hr == AAFRESULT_ILLEGAL_VALUE, AAFRESULT_TEST_FAILED );	

	//try a +ve Set test
	const aafInt32 Test_val = 15;
	someval = Test_val;  //2nd value
	checkResult ( spENUM->SetIntegerValue(spPropVal, someval) );
	//Get it back
	someval = 0; //reset
	hr = spENUM->GetIntegerValue(spPropVal, &someval);
	checkExpression( someval == Test_val, AAFRESULT_TEST_FAILED );	

	delete [] somebuf; //done

	return S_OK;
	
}//verifyContents()


static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	
	IAAFFile* pFile = NULL;
	IAAFHeader * pHeader = NULL;
	IAAFDictionary * pDict = NULL;
	
	try
	{
		//Do the usual ...
		RemoveTestFile (pFileName);
		checkResult (CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		assert (pFile);
		checkResult (pFile->GetHeader (&pHeader));
		assert (pHeader);
		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		CAAFBuiltinDefs defs (pDict);
		
		///////////////////////
		
		checkResult(createENUMType (pDict));
		
		checkResult(addENUMTypeToComponent (pDict));
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
		checkResult(spFill->Initialize(defs.ddkAAFSound(), 10));
		
		checkResult(createENUMFiller(pDict, spFill));
		
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

extern "C" HRESULT CAAFTypeDefEnum_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTypeDefEnum_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
			
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );	
		
	}//try
	catch (...)
	{
		cerr << "CAAFTypeDefEnum_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}


