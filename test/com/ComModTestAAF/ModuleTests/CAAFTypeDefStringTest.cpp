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
#include <string.h>

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

//// Our TEST values ...

//Fixed Array
#define									TEST_STR_NAME	L"STR type Name"
static  const aafUID_t					TEST_STR_TYPE_ID =
{ 0xb9da6c9e, 0x2b3c, 0x11d4, { 0x8e, 0x50, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };


static const aafUInt32					TEST_STR_COUNT =  21;
#define		 TEST_ELEM_t				aafCharacter
static const aafUID_t					TEST_ELEM_TYPE_ID =	kAAFTypeID_Character;
//static const TEST_ELEM_t				TEST_STR_VALUES [TEST_STR_COUNT] = {L"This is a TEST String"};
static const aafCharacter *			TEST_STR_VALUE = L"This is a TEST String";

// String Property
#define									TEST_PROP_NAME	L"STR Property Name"
static  const aafUID_t					TEST_PROP_ID =
{ 0xb9da6c9f, 0x2b3c, 0x11d4, { 0x8e, 0x50, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };


// Mob id
static  aafMobID_t						TEST_MobID = 
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xabbe6200, 0x2b3c, 0x11d4, {0x8e, 0x50, 0x00, 0x90, 0x27, 0xdf, 0xcc, 0x26}}};

// Slot id
static const aafSlotID_t				TEST_SLOT_ID = 7;
#define									TEST_SLOT_NAME	L"Slot containing our STR Segment"


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

static HRESULT  createSTRType (IAAFDictionary * const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	//Look up the Type def based on 16-bit signed integer
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//Create a String type
	IAAFTypeDefStringSP spSTR;
	//checkResult(defs.cdTypeDefString()->
		//CreateInstance (IID_IAAFTypeDefString, (IUnknown **) &spSTR));
	pDict->CreateMetaInstance(AUID_AAFTypeDefString, IID_IAAFTypeDefString,
		(IUnknown **) &spSTR) ;

	
	//IAAFTypeDefString::Initialize
	checkResult(spSTR->Initialize(TEST_STR_TYPE_ID, spTD_elem, TEST_STR_NAME));
	
	//  Register our new STR type def :
	IAAFTypeDefSP spTD_str;
	checkResult(spSTR->QueryInterface(IID_IAAFTypeDef, (void**)&spTD_str));
	checkResult(pDict->RegisterTypeDef(spTD_str));
	
	return S_OK;
}//createSTRType()


static HRESULT addSTRTypeToComponent(IAAFDictionary * const pDict)
{
	CAAFBuiltinDefs defs(pDict);
	
	///////////////////////////////////////////////////////////////////////
	//Look up a Class based on a component? or some meaningful class
	IAAFClassDefSP spSomeClass;
	checkResult(pDict->LookupClassDef (AUID_AAFComponent, &spSomeClass));	
	//OK, lets register (and get) a property def, give it a name + typedef, and
	
	//Lookup the TEST_STR_TYPE_ID we just created!
	IAAFTypeDefSP spTD;
	checkResult(pDict->LookupTypeDef (TEST_STR_TYPE_ID, &spTD));
	
	//	Register the Property
	IAAFPropertyDefSP spPropDef;
	checkResult(spSomeClass->RegisterOptionalPropertyDef (TEST_PROP_ID,
		TEST_PROP_NAME,
		spTD,
		&spPropDef));
	
	return S_OK;
}//addSTRTypeToComponent()


static HRESULT createSTRFiller(IAAFDictionary* const pDict, IAAFFillerSP& spFill)
{
	
	//handy - QI filler for  Object intf.		
	IAAFObjectSP spObj;
	checkResult(spFill->QueryInterface(IID_IAAFObject, (void**)&spObj));
	
	//Get the  property def for Component::STR
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
	//Lookup the STR type
	IAAFTypeDefSP spTypeDef;
	checkResult(pDict->LookupTypeDef(TEST_STR_TYPE_ID, &spTypeDef));
	
	//Get the STR typedef
	IAAFTypeDefStringSP  spSTR;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefString, (void**)&spSTR));
	
	
	//Set the array up
	IAAFPropertyValueSP spPropVal;
	checkResult( spSTR->CreateValueFromCString (
		(aafMemPtr_t)  TEST_STR_VALUE,
		wcslen(TEST_STR_VALUE) * sizeof(aafCharacter),
		&spPropVal)	);
	
	
	//Set the value STR to the Object *****************************************
	checkResult(spObj->SetPropertyValue(spPD_comp, spPropVal));
	
	//Verify that the optional property is now present in the object
	checkResult(spObj->IsPropertyPresent(spPD_comp, &bIsPresent));
	checkExpression(bIsPresent == kAAFTrue,  AAFRESULT_TEST_FAILED);
	
	return S_OK;
}//createSTRFiller()




static HRESULT verifyContents (IAAFHeader* const pHeader, IAAFDictionary* const pDict,
							   const aafBoolean_t bMinimalTesting)
							   
{
	//CAAFBuiltinDefs defs (pDict);
	
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
	
	//Get the  property def for Component::STR
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
	checkResult(spObj->GetPropertyValue(spPD_comp, &spPropVal)); //Our STRING PropVal
	
	//Get the Fixed Array typedef
	//first, get the Type Def from the  Property def
	IAAFTypeDefSP spTypeDef;
	checkResult(spPD_comp->GetTypeDef(&spTypeDef));
	//now get the STR intf
	IAAFTypeDefStringSP spSTR;
	checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefString, (void**)&spSTR));
	
	//get the array out of it ...
	TEST_ELEM_t	check_str [128] = {0};  //init a checking variable; select a reasonable size buffer
	
	//IAAFTypeDefString::GetCount()
	aafUInt32 check_count = 0;
	checkResult(spSTR->GetCount(spPropVal, &check_count));
	checkExpression( check_count == TEST_STR_COUNT, AAFRESULT_TEST_FAILED );	
	
	//IAAFTypeDefString::GetType()
	IAAFTypeDefSP spTestType;
	checkResult(spSTR->GetType(&spTestType));
	//Look up our elem Type def 
	IAAFTypeDefSP spTD_elem;
	checkResult(pDict->LookupTypeDef (TEST_ELEM_TYPE_ID, &spTD_elem));
	
	//IAAFTypeDefString::GetElements()
	aafUInt32 i=0;
	checkResult(spSTR->GetElements(spPropVal, (aafMemPtr_t) check_str, sizeof(check_str)));
	//VERIFY values:
	for (i=0; i<TEST_STR_COUNT; i++)
		checkExpression( check_str[i] == TEST_STR_VALUE[i], AAFRESULT_TEST_FAILED );	
	
	
	//At this point, the test is succesful for both the CREATE and READ (unscrambling of .aaf file) routines
	if (bMinimalTesting)
		// so,  bail if we're called from CREATE 
		return S_OK;
	
	/////  READ routine .... continue with more tests ....................

	//::SetCString ...
	// Set it
	const aafCharacter* const NEW_STR = L"Another One";
	wcscpy(check_str, NEW_STR);
	aafUInt32 cstr_size = wcslen(check_str) * sizeof(aafCharacter);
	spSTR->SetCString(spPropVal, (aafMemPtr_t) check_str, cstr_size);
	//Read it back
	memset(check_str, 0, sizeof(check_str)); //initialize 
	checkResult(spSTR->GetElements(spPropVal, (aafMemPtr_t) check_str, sizeof(check_str)));
	//VERIFY values:
	checkExpression( wcsncmp(check_str, NEW_STR, wcslen(NEW_STR)) == 0, AAFRESULT_TEST_FAILED );

	//Append elements
	const aafCharacter* const APPEND_STR = L" Bites the Dust";
	wcscpy(check_str, APPEND_STR);
	cstr_size = wcslen(check_str) * sizeof(aafCharacter);
	spSTR->AppendElements(spPropVal, (aafMemPtr_t) check_str); //, cstr_size);
	//Read it back
	memset(check_str, 0, sizeof(check_str)); //initialize 
	checkResult(spSTR->GetElements(spPropVal, (aafMemPtr_t) check_str, sizeof(check_str)));
	//VERIFY values:
	//verify first part of the string
	checkExpression( wcsncmp(check_str, NEW_STR, wcslen(NEW_STR)) == 0, AAFRESULT_TEST_FAILED );
	cstr_size = wcslen(NEW_STR); //point to the "append" part of the string
	//verify second part of the string
	checkExpression( wcsncmp(&check_str[cstr_size], APPEND_STR, wcslen(APPEND_STR)) == 0, AAFRESULT_TEST_FAILED );


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
		
		checkResult(createSTRType (pDict));
		
		checkResult(addSTRTypeToComponent (pDict));
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
		
		checkResult(createSTRFiller(pDict, spFill));
		
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

extern "C" HRESULT CAAFTypeDefString_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTypeDefString_test(
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
		cerr << "CAAFTypeDefString_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}




















