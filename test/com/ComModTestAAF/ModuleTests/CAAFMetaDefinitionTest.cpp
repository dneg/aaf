
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
typedef IAAFSmartPointer<IAAFMetaDefinition>				IAAFMetaDefinitionSP;

//// Our TEST (gold) values ....

// {CADEF777-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t propDef_id = 
{ 0xcadef777, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

#define TEST_NAME L"Test: Property Def Name"
#define TEST_DESCRIPTION L"Test: Property Def Description"

static const aafUID_t TEST_UID = kAAFTypeID_UInt8;

//Initialize our Non-Implemented flags to False ...
//static aafBoolean_t	 bSetDefaultValue_NI = kAAFFalse;       
//static aafBoolean_t  bSetIsSearchable_NI = kAAFFalse;       
//static aafBoolean_t  bGetDefaultValue_NI = kAAFFalse;       
//static aafBoolean_t  bGetIsSearchable_NI = kAAFFalse;       


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
		
		
		//Look up the Type def based on Unsigned 8-bit integer
		IAAFTypeDefSP spTypeDef;
		checkResult(pDict->LookupTypeDef (TEST_UID, &spTypeDef));
		
		//Look up a Class based on a "sequence" or any other object for that matter
		IAAFClassDefSP spSomeClass;
		checkResult(pDict->LookupClassDef (AUID_AAFSequence, &spSomeClass));
		
		//OK, lets register (and get) a property def, give it a name + typedef, and
		//		get back a property def
		IAAFPropertyDefSP spPropDef;
		IAAFMetaDefinitionSP spMetaDef;
		checkResult(spSomeClass->RegisterOptionalPropertyDef (propDef_id,
			TEST_NAME,
			spTypeDef,
			&spPropDef));
		
		
	   checkResult(spPropDef->QueryInterface(IID_IAAFMetaDefinition, (void **) &spMetaDef));
		//Initialize
		checkResult(spMetaDef->Initialize(propDef_id, TEST_NAME, TEST_DESCRIPTION));
		//SetName
		checkResult(spMetaDef->SetName(TEST_NAME));
		//SetDescription
		checkResult(spMetaDef->SetDescription(TEST_DESCRIPTION));
	}
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
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
	
	IAAFFile* pFile = NULL;
	IAAFHeader * pHeader = NULL;
	IAAFDictionary * pDict = NULL;
	aafBoolean_t  bFileOpen = kAAFFalse;
	
	IUnknownSP    pUnk1, pUnk2;		
	
	try
	{
		
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = kAAFTrue;
		
		checkResult (pFile->GetHeader (&pHeader));
		assert (pHeader);
		checkResult (pHeader->GetDictionary (&pDict));
		assert (pDict);
		CAAFBuiltinDefs defs (pDict);
		
		//Now, get our (extended) Sequence class
		IAAFClassDefSP spSomeClass;
		checkResult(pDict->LookupClassDef (AUID_AAFSequence, &spSomeClass));
		
		IAAFPropertyDefSP spPropDef;
		checkResult(spSomeClass->LookupPropertyDef(propDef_id, &spPropDef));
		
		IAAFMetaDefinitionSP spMetaDef;
	   checkResult(spPropDef->QueryInterface(IID_IAAFMetaDefinition, (void **) &spMetaDef));

		//Now we have a prop def ... so deal with the Methods ...

		aafUInt32 bufSize = 0;
		
		//GetName	
		//GetNameBufLen
		checkResult( spMetaDef->GetNameBufLen  (&bufSize) );
		aafCharacter *nameBuf = new aafCharacter[bufSize];
		checkResult( spMetaDef->GetName(nameBuf, bufSize) );
		checkExpression( wcscmp(TEST_NAME, nameBuf) == 0, AAFRESULT_TEST_FAILED );
		delete [] nameBuf;
		
		//GetDescription	
		//GetDescriptionBufLen
		checkResult( spMetaDef->GetDescriptionBufLen  (&bufSize) );
		aafCharacter *descBuf = new aafCharacter[bufSize];
		checkResult( spMetaDef->GetDescription(descBuf, bufSize) );
		checkExpression( wcscmp(TEST_DESCRIPTION, descBuf) == 0, AAFRESULT_TEST_FAILED );
		delete [] descBuf;
		
		
		//GetIsOptional
		aafBoolean_t bIsOptional = kAAFFalse;
		checkResult( spPropDef->GetIsOptional(&bIsOptional) );
		checkExpression( bIsOptional == kAAFTrue, AAFRESULT_TEST_FAILED );
		
		//GetTypeDef
		IAAFTypeDefSP  spTypeDef;
		checkResult( spPropDef->GetTypeDef(&spTypeDef) );

		//check AUID
		IAAFMetaDefinitionSP  spMetaDefinition;
		aafUID_t checkUID = {0};
		checkResult(spTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void**)&spMetaDefinition));
		checkResult(spMetaDefinition->GetAUID (&checkUID));
		checkExpression(memcmp(&checkUID, &TEST_UID, sizeof(checkUID)) == 0, AAFRESULT_TEST_FAILED);
		
		//check Category of the typedef - WE HAPPEN TO KNOW THAT ITS AN "INTEGER" ... 
		//			.....  REMOVE if we ever change the type to something other than INTEGER
		eAAFTypeCategory_t typeCat = kAAFTypeCatUnknown;
		checkResult(spTypeDef->GetTypeCategory(&typeCat));
		checkExpression( typeCat == kAAFTypeCatInt, AAFRESULT_TEST_FAILED );
	}//try
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	if (pDict) pDict->Release();
	if (pHeader) pHeader->Release();
	
	if (bFileOpen && pFile)
	{
		AAFRESULT temphr = pFile->Close();
		if (FAILED(temphr)) 
			return temphr;
		pFile->Release();
	}
	
	return hr;
	
}

extern "C" HRESULT CAAFMetaDefinition_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFMetaDefinition_test(
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
		cerr << "CAAFMetaDefinition_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFMasterMob tests have not been implemented:" << endl; 
//		cout << "     Initialize" << endl; 
//		cout << "     SetName" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}
	
	return hr;
}
