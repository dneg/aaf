// @doc INTERNAL
// @com This file implements the module test for CAAFPropertyDef
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
typedef IAAFSmartPointer<IAAFMetaDefinition>				IAAFMetaDefinitionSP;

//// Our TEST (gold) values ....

// {CADEF777-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t propDef_id = 
{ 0xcadef777, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

#define TEST_NAME L"Test: Property Def Name"
#define TEST_DESCRIPTION L"Test: Property Def Description"

//static const aafUInt8 TEST_INT_BYTE = 0xAB;
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
		ProductInfo.productName = L"AAFPropertyDef Test";
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
		
		
		//Look up the Type def based on Unsigned 8-bit integer
		IAAFTypeDefSP spTypeDef;
		checkResult(pDict->LookupTypeDef (TEST_UID, &spTypeDef));
		
		//Look up a Class based on a "sequence" or any other object for that matter
		IAAFClassDefSP spSomeClass;
		checkResult(pDict->LookupClassDef (AUID_AAFSequence, &spSomeClass));
		
		//OK, lets register (and get) a property def, give it a name + typedef, and
		//		get back a property def
		IAAFPropertyDefSP spPropDef;
		checkResult(spSomeClass->RegisterOptionalPropertyDef (propDef_id,
			TEST_NAME,
			spTypeDef,
			&spPropDef));
		
		
		//SetDescription
		checkResult(spPropDef->SetDescription(TEST_DESCRIPTION));
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
		
		//Now we have a prop def ... so deal with the Methods ...

		aafUInt32 bufSize = 0;
		
		//GetName	
		//GetNameBufLen
		checkResult( spPropDef->GetNameBufLen  (&bufSize) );
		aafCharacter *nameBuf = new aafCharacter[bufSize];
		checkResult( spPropDef->GetName(nameBuf, bufSize) );
		checkExpression( wcscmp(TEST_NAME, nameBuf) == 0, AAFRESULT_TEST_FAILED );
		delete [] nameBuf;
		
		//GetDescription	
		//GetDescriptionBufLen
		checkResult( spPropDef->GetDescriptionBufLen  (&bufSize) );
		aafCharacter *descBuf = new aafCharacter[bufSize];
		checkResult( spPropDef->GetDescription(descBuf, bufSize) );
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

extern "C" HRESULT CAAFPropertyDef_test(testMode_t mode);
extern "C" HRESULT CAAFPropertyDef_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFPropertyDefTest.aaf";
	
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
		cerr << "CAAFPropertyDef_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}
	
