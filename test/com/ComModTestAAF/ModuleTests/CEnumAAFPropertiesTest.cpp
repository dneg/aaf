// @doc INTERNAL
// @com This file implements the module test for CEnumAAFProperties
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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

//Define some smart pointers
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFTypeDefString>			IAAFTypeDefStringSP;
typedef IAAFSmartPointer<IAAFClassDef>				IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>			IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>			IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>			IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>					IUnknownSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFObject>				IAAFObjectSP;
typedef IAAFSmartPointer<IAAFMob>					IAAFMobSP;
typedef IAAFSmartPointer<IAAFSourceMob>				IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFFileDescriptor>    IAAFFileDescriptorSP;
typedef IAAFSmartPointer<IAAFSourceMob>         IAAFSourceMobSP;
typedef IAAFSmartPointer<IAAFFiller>            IAAFFillerSP;
typedef IAAFSmartPointer<IAAFSegment>           IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>   IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IEnumAAFMobSlots>      IEnumAAFMobSlotsSP;
typedef IAAFSmartPointer<IAAFProperty>           IAAFPropertySP;
typedef IAAFSmartPointer<IEnumAAFProperties>		IEnumAAFPropertiesSP;

typedef IAAFSmartPointer<IEnumAAFMobs>		IEnumAAFMobsSP;
typedef IAAFSmartPointer<IAAFCompositionMob>		IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFProperty>		IAAFPropertySP;

typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;


static const aafUID_t			TEST_PROP1_ID =
{ 0x9d92ba40, 0x3641, 0x11d4, { 0x8e, 0x50, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };
static const aafCharacter*		TEST_PROP1_NAME = L"Test Prop1: Name";

static const aafUID_t			TEST_PROP2_ID =
{ 0x9d92ba41, 0x3641, 0x11d4, { 0x8e, 0x50, 0x0, 0x90, 0x27, 0xdf, 0xcc, 0x26 } };
static const aafCharacter*		TEST_PROP2_NAME = L"Test Prop2: Name";

//say , 10 Property Names, each having 128 characters max.
#define PROPS_MAX_COUNT   10
#define TEST_BUF_MAX_SIZE  128
static aafCharacter  TEST_PROP_NAMES[PROPS_MAX_COUNT][TEST_BUF_MAX_SIZE];

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


static HRESULT verifyContents (IAAFHeader* const pHeader, IAAFDictionary* const /*pDict*/,
							   const aafBoolean_t /*bMinimalTesting*/)
							   
{
	//CAAFBuiltinDefs defs (pDict);
	IEnumAAFMobsSP				pMobIter;
	IAAFCompositionMobSP		pCMob;
	IAAFMobSP					pMob;
	aafNumSlots_t				numMobs;
	IAAFObjectSP					pObj;
	
	IEnumAAFPropertiesSP pEnum;
	IAAFPropertySP   pProp;
	
	IAAFPropertyDefSP  pPropDef;
	IAAFPropertyValueSP  pPropVal;
	
	IAAFTypeDefSP	pTypeDef;
	
	// Validate that there is only one composition mob.
	checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
	
	// Enumerate over all Composition Mobs
	aafSearchCrit_t				criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFCompMob;
	checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
	{		
		//Get the Composition mob
		checkResult (pMob->QueryInterface (IID_IAAFCompositionMob,
			(void **) &pCMob));
		
		//Get the Object
		checkResult (pCMob->QueryInterface (IID_IAAFObject,
			(void **) &pObj));
		
		//From the Object, we can get the properties ....
		aafUInt32 propCount = 0;
		checkResult (pObj->CountProperties (&propCount));
		//A composition should have at least 5 properties ...
		checkExpression(propCount>=5, AAFRESULT_TEST_FAILED);
		// TEST_PROP_NAMES array cannot hold more than PROPS_MAX_COUNT
		// elements.
		checkExpression(propCount<=PROPS_MAX_COUNT, 
			AAFRESULT_TEST_FAILED);
		
		//Get Enumeration over properties
		checkResult (pObj->GetProperties (&pEnum));
		checkExpression (pEnum != 0, AAFRESULT_TEST_FAILED);

		//first, try NextOne, and also use it to fill our TEST Prop Names
		aafUInt32 i=0;
		for (i=0; i < propCount; i++)
		{
			checkResult (pEnum->NextOne (&pProp));
			//1.  Get Definition
			checkResult( pProp->GetDefinition(&pPropDef) );
			//1.1. Get Name from def
			checkResult(pPropDef->GetName(TEST_PROP_NAMES[i], TEST_BUF_MAX_SIZE*sizeof(aafCharacter)));
		}

		//try the next one ... it should fail
		checkExpression(pEnum->NextOne(&pProp) == AAFRESULT_NO_MORE_OBJECTS, AAFRESULT_TEST_FAILED);
		
		aafCharacter name_buf[TEST_BUF_MAX_SIZE];
		/* Reset, and check the first element again*/
		checkResult(pEnum->Reset());
		checkResult(pEnum->NextOne (&pProp));
		checkResult( pProp->GetDefinition(&pPropDef) );
		checkResult(pPropDef->GetName (name_buf, TEST_BUF_MAX_SIZE));
		checkExpression (wcscmp(name_buf, TEST_PROP_NAMES[0]) == 0, AAFRESULT_TEST_FAILED);
		
		/* Reset, Skip, and check the second element*/
		checkResult(pEnum->Reset());
		checkResult(pEnum->Skip(1));
		checkResult(pEnum->NextOne (&pProp));  //this should be the 2nd element (index 1)
		checkResult( pProp->GetDefinition(&pPropDef) );
		checkResult(pPropDef->GetName (name_buf, TEST_BUF_MAX_SIZE));
		checkExpression (wcscmp(name_buf, TEST_PROP_NAMES[1]) == 0, AAFRESULT_TEST_FAILED);
		
		//Check the next 2 elements, using Next()
		//IAAFPropertySP		pArray[2];
		IAAFProperty*		pArray[2];
		aafUInt32 resultCount;
		
		checkResult(pEnum->Reset());
		checkResult(pEnum->Next (2, pArray, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		//check 1st element
		checkResult( pArray[0]->GetDefinition(&pPropDef) );
		checkResult(pPropDef->GetName (name_buf, TEST_BUF_MAX_SIZE));
		checkExpression (wcscmp(name_buf, TEST_PROP_NAMES[0]) == 0, AAFRESULT_TEST_FAILED);
		//check 2nd element
		checkResult( pArray[1]->GetDefinition(&pPropDef) );
		checkResult(pPropDef->GetName (name_buf, TEST_BUF_MAX_SIZE));
		checkExpression (wcscmp(name_buf, TEST_PROP_NAMES[1]) == 0, AAFRESULT_TEST_FAILED);
		//release the (non-smart) com pointers
		checkResult( pArray[0]->Release());
		checkResult( pArray[1]->Release());

		//Clone stuff ..... DEPENDS on the result of the previous (Next()) step
		IEnumAAFPropertiesSP spCloneEnum;
		checkResult(pEnum->Clone(&spCloneEnum));
		//we got next-2 from previous step, now lets skip one, and continue with clone ...
		//first, reset the original Enumerator to make sure we're not refering to it
		checkResult(pEnum->Reset());
		//NOw skip one with the CLONE ... (moving on the 4th element)
		checkResult(spCloneEnum->Skip(1));
		checkResult(spCloneEnum->NextOne (&pProp));  //this should be the 4th element (index 3)
		checkResult( pProp->GetDefinition(&pPropDef) );
		checkResult(pPropDef->GetName (name_buf, TEST_BUF_MAX_SIZE));
		//verify with index 3
		checkExpression (wcscmp(name_buf, TEST_PROP_NAMES[3]) == 0, AAFRESULT_TEST_FAILED);


		//Reset
		checkResult(pEnum->Reset());

		for (i=0; i < propCount; i++)
		{
		//Check for property validity
			checkResult (pEnum->NextOne (&pProp));
			checkExpression (pProp != NULL, AAFRESULT_TEST_FAILED);
			//Now call prop methods
			
			//1.  Get Definition
			checkResult( pProp->GetDefinition(&pPropDef) );
			//1.1. Get Name from def
			checkResult(pPropDef->GetName(TEST_PROP_NAMES[i],
				TEST_BUF_MAX_SIZE*sizeof(aafCharacter)));

			//2. Get Value
			checkResult( pProp->GetValue(&pPropVal) );
			
			//deal with prop value
			checkResult( pPropVal->GetType(&pTypeDef) );
			
			//From propval, get the typedef!!!!!!!!!!!!!!!!!!!!!
			IAAFTypeDefSP spTypeDef;
			checkResult( pPropVal->GetType(&spTypeDef) );
			
			//get category
			eAAFTypeCategory_t typeCat = kAAFTypeCatUnknown;
			checkResult (spTypeDef->GetTypeCategory(&typeCat));
			//make sure it is not unknown
			checkExpression(kAAFTypeCatUnknown != typeCat, AAFRESULT_TEST_FAILED);
			

			if (typeCat == kAAFTypeCatInt)
			{
				IAAFTypeDefIntSP  spTDInt;
				checkResult( spTypeDef->QueryInterface(IID_IAAFTypeDefInt, 
					(void**)&spTDInt) );
				
				aafUInt8 the_int = 0;

				checkResult(spTDInt->GetInteger(pPropVal, (aafMemPtr_t)&the_int, 1 /*1 byte*/));

				aafCharacter buf[128];
				checkResult(pPropDef->GetName(buf, 128*sizeof(aafCharacter)));

				char buf2[128];
				wcstombs(buf2, buf, 128);

			}//if Int

		}//while prop count
	}//while mob iter
	
	return AAFRESULT_SUCCESS;
	
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

static void  AddOptionalProperties(IAAFDictionary* const pDict,
									IAAFCompositionMobSP& spCMob)
{
	
		//first thing - lookup  a UInt8's  TypeDef
		
	    IAAFTypeDefSP spTD;
		IAAFTypeDefIntSP spIntTD;
		checkResult(pDict->LookupTypeDef (kAAFTypeID_UInt8, &spTD));

		// lookup the CD of a CMob
		IAAFClassDefSP spCD_CMob;
		checkResult(pDict->LookupClassDef (AUID_AAFCompositionMob, &spCD_CMob));	
		

		//Register our Test Property (1)  with CMob
		IAAFPropertyDefSP spPD1;
		checkResult(spCD_CMob->RegisterOptionalPropertyDef (TEST_PROP1_ID,
			TEST_PROP1_NAME,
			spTD,
			&spPD1));

	    //Register our Test Property (2)  with CMob
		IAAFPropertyDefSP spPD2;
		checkResult(spCD_CMob->RegisterOptionalPropertyDef (TEST_PROP2_ID,
			TEST_PROP2_NAME,
			spTD,
			&spPD2));

		//Create a composition ...

		CAAFBuiltinDefs defs (pDict);

		checkResult (defs.cdCompositionMob()->
			CreateInstance (IID_IAAFCompositionMob, (IUnknown **) &spCMob));
		checkResult (spCMob->Initialize (L"Some Name for our CMob"));
		
		//QI for Cmob's  object  (for Direct Access)
		IAAFObjectSP spObj_CMob;
		checkResult(spCMob->QueryInterface(IID_IAAFObject, (void**)&spObj_CMob));

		//Finally,  Create some values for our 2 new properties ....

		//create first value
		IAAFPropertyValueSP spIntVal;
		aafUInt8 some_int = 13;
		checkResult(spTD->QueryInterface(IID_IAAFTypeDefInt, (void **)&spIntTD));
		checkResult(spIntTD->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spIntVal));
		//Add it to CMob's PD1
		checkResult(spObj_CMob->SetPropertyValue(spPD1, spIntVal));

		//create second value
		some_int = 25;
		checkResult(spIntTD->CreateValue((aafMemPtr_t)&some_int, sizeof (some_int), &spIntVal));
		//Add it to CMob's PD2
		checkResult(spObj_CMob->SetPropertyValue(spPD2, spIntVal));

}//AddOptionalProperties()

static HRESULT CreateAAFFile(aafWChar *  pFileName )
{
	HRESULT hr = AAFRESULT_SUCCESS;
	
	aafProductIdentification_t ProductInfo;
	IAAFFile* pFile = NULL;
	IAAFHeader * pHeader = NULL;
	IAAFDictionary * pDict = NULL;
	IAAFMob * pMob = NULL;
	IAAFObject * pObj = NULL;
	
	try
	{
		aafProductVersion_t v;
		v.major = 1;
		v.minor = 0;
		v.tertiary = 0;
		v.patchLevel = 0;
		v.type = kAAFVersionUnknown;
		
		ProductInfo.companyName = L"AAF Developers Desk";
		ProductInfo.productName = L"EnumAAFProperties Test";
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
		
		IAAFCompositionMobSP spCMob;
		// Add optional properties .....
		AddOptionalProperties(pDict, spCMob);
		
		//... Get its mob, and add it to the header info
		checkResult (spCMob->QueryInterface (IID_IAAFMob,
										  (void **) &pMob));
		assert (pMob);
		checkResult (pHeader->AddMob (pMob));
		
		checkResult(verifyContents (pHeader, pDict, kAAFTrue));  //True => minimal testing
		
	}
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	if (pMob) pMob->Release();
	if (pObj) pObj->Release();
	if (pDict) pDict->Release();
	if (pHeader) pHeader->Release();
	if (pFile)
	{
		AAFRESULT temphr = pFile->Save();
		if (! SUCCEEDED (temphr)) return temphr;
		temphr = pFile->Close();
		if (! SUCCEEDED (temphr)) return temphr;
		pFile->Release();
	}
	
	return hr;
}


static HRESULT  ReadAAFFile(aafWChar *  pFileName )
{
	HRESULT hr = AAFRESULT_SUCCESS;
	
	IAAFFileSP				pFile;
	IAAFHeaderSP		pHeader;
	IAAFDictionarySP			pDict;
	bool bFileOpen = false;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult (pFile->GetHeader (&pHeader));
		assert (pHeader);
		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		
		checkResult(verifyContents (pHeader, pDict, kAAFFalse));  //False => NOT minimal testing
		
		
	}//try
	
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	
	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
	}
	
	return hr;
	
}

extern "C" HRESULT CEnumAAFProperties_test(testMode_t mode);
extern "C" HRESULT CEnumAAFProperties_test(testMode_t mode)
{
	
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"EnumAAFPropertiesTest.aaf";
	
	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );		
		
	}//try
	catch (...)
	{
		cerr << "CEnumAAFProperties_test...Caught general C++"
			<< " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;  
}
