// @doc INTERNAL
// @com This file implements the module test for CAAFPropertyDef
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

//// Our TEST (gold) values ....

// {CADEF777-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t propDef_id = 
{ 0xcadef777, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

#define TEST_NAME L"Test: Property Def Name"
#define TEST_DESCRIPTION L"Test: Property Def Description"

static const aafUInt8 TEST_INT_BYTE = 0xAB;
static const aafUID_t TEST_UID = kAAFTypeID_UInt8;

//Initialize our Non-Implemented flags to False ...
static aafBoolean_t	 bSetDefaultValue_NI = kAAFFalse;       
static aafBoolean_t  bSetIsSearchable_NI = kAAFFalse;       
static aafBoolean_t  bGetDefaultValue_NI = kAAFFalse;       
static aafBoolean_t  bGetIsSearchable_NI = kAAFFalse;       


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
		ProductInfo.companyName = L"AAF Developers Desk";
		ProductInfo.productName = L"AAFPropertyDef Test";
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
		
		//SetDefaultValue
		//	//first, create a value for integer
		IAAFTypeDefIntSP  spTypeDefInt;
		checkResult(spTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void **) &spTypeDefInt));
		IAAFPropertyValueSP  spPropVal;
		checkResult(spTypeDefInt->CreateValue((aafMemPtr_t)&TEST_INT_BYTE, 1, &spPropVal));
		//	//now, set the default value
		hr = spPropDef->SetDefaultValue(spPropVal);
		if (FAILED(hr)) 
		{
			if (hr == AAFRESULT_NOT_IMPLEMENTED)
			{
				//If not implemented, then mark as NI and move on ...
				//Flag this NI
				bSetDefaultValue_NI = kAAFTrue;
				//Reset the hr
				hr = AAFRESULT_SUCCESS;
				//Move on
			}
			else
				//If it failed, but was not an NI, then "what the hell is this problem ??!!"
				throw hr;
		}//if Failed hr
		

		
		
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
		IAAFDefObjectSP  spDefObj;
		aafUID_t checkUID = {0};
		checkResult(spTypeDef->QueryInterface(IID_IAAFDefObject, (void**)&spDefObj));
		checkResult(spDefObj->GetAUID (&checkUID));
		checkExpression(memcmp(&checkUID, &TEST_UID, sizeof(checkUID)) == 0, AAFRESULT_TEST_FAILED);
		
		//check Category of the typedef - WE HAPPEN TO KNOW THAT ITS AN "INTEGER" ... 
		//			.....  REMOVE if we ever change the type to something other than INTEGER
		eAAFTypeCategory_t typeCat = kAAFTypeCatUnknown;
		checkResult(spTypeDef->GetTypeCategory(&typeCat));
		checkExpression( typeCat == kAAFTypeCatInt, AAFRESULT_TEST_FAILED );
		
		//NOt implemented yet ... GetDefaultValue() and GetIsSearchable()
		
		//GetDefaultValue
		IAAFPropertyValueSP  spPropVal;
		IAAFTypeDefSP  spTypeDef2;
		hr = spPropDef->GetDefaultValue(&spPropVal);
		if (FAILED(hr)) 
		{
			if (hr == AAFRESULT_NOT_IMPLEMENTED)
			{
				//If not implemented, then mark as NI and move on ...
				//Flag this NI
				bGetDefaultValue_NI = kAAFTrue;
				//Reset the hr
				hr = AAFRESULT_SUCCESS;
				//Move on
				goto _skip_GetDefaultValue;
			}
			else
				//If it failed, but was not an NI, then "what the hell is this problem ??!!"
				throw hr;
		}//if Failed hr
		checkResult(spPropVal->GetType(&spTypeDef2));
		//Compare the two TypeDef's ...
		checkResult(spTypeDef->QueryInterface(IID_IUnknown, (void **)&pUnk1));
		checkResult(spTypeDef2->QueryInterface(IID_IUnknown, (void **)&pUnk2));
		checkExpression( pUnk1 == pUnk2, AAFRESULT_TEST_FAILED );
_skip_GetDefaultValue:
		;

		
		
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

extern "C" HRESULT CAAFPropertyDef_test()
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFPropertyDefTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
		
		//Check for partial success
		if (bSetDefaultValue_NI || bSetIsSearchable_NI || bGetDefaultValue_NI || bGetIsSearchable_NI )
		{
			//if any one of these are set, we have only partial success ... damn! ...
			hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
			
			//print out the appropriate NI message(s) ...
			if (bSetDefaultValue_NI)
				cout << "\t  SetDefaultValue  method is Not Implemented" << endl;
			if (bSetIsSearchable_NI)
				cout << "\t  SetIsSearchable  method is Not Implemented" << endl;
			if (bGetDefaultValue_NI)
				cout << "\t  GetDefaultValue  method is Not Implemented" << endl;
			if (bGetIsSearchable_NI)
				cout << "\t  GetIsSearchable  method is Not Implemented" << endl;
			
		}//if partial success stuff

	}//try
	catch (...)
	{
		cerr << "CAAFPropertyDef_test...Caught general C++"
			" exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}
	
