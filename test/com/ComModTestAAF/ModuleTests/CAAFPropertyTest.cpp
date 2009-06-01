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

#include "CAAFBuiltinDefs.h"

//Define some smart pointers
typedef IAAFSmartPointer<IAAFTypeDef>				IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFDefObject>				IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFTypeDefString>			IAAFTypeDefStringSP;

#define TEST_NAME   L"Test: Some Name"

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
	IAAFCompositionMob * pCMob = NULL;
	IAAFMob * pMob = NULL;
	IAAFObject * pObj = NULL;
	
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
		
		//Create a composition ...
		checkResult (defs.cdCompositionMob()->
			CreateInstance (IID_IAAFCompositionMob,
			(IUnknown **) &pCMob));
		assert (pCMob);
		checkResult (pCMob->Initialize (TEST_NAME));
		
		//... Get its mob, and add it to the header info
		checkResult (pCMob->QueryInterface (IID_IAAFMob,
										  (void **) &pMob));
		assert (pMob);
		checkResult (pHeader->AddMob (pMob));
		
	}
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	if (pCMob) pCMob->Release();
	if (pMob) pCMob->Release();
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
	
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFCompositionMob *		pCMob = NULL;
	IAAFMob*					pMob = NULL;
	bool bFileOpen = false;
	aafNumSlots_t				numMobs;
	IAAFObject*					pObj = NULL;
	
	IEnumAAFProperties * pEnum = NULL;
	IAAFProperty *pProp = NULL;
	
	IAAFPropertyDef* pPropDef = NULL;
	IAAFPropertyValue* pPropVal = NULL;

	IAAFTypeDef*	pTypeDef  = NULL;

	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
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
			
			//Get Enumeration over properties
			checkResult (pObj->GetProperties (&pEnum));
			checkExpression (pEnum != 0, AAFRESULT_TEST_FAILED);
			while (propCount--)
			{
				//Check for property validity
				checkResult (pEnum->NextOne (&pProp));
				checkExpression (pProp != NULL, AAFRESULT_TEST_FAILED);
				//Now call prop methods
				
				//1.  Get Definition
				checkResult( pProp->GetDefinition(&pPropDef) );
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
				
				//Deal with our Name  String .... 
				if (typeCat == kAAFTypeCatString)
				{
					IAAFTypeDefStringSP  spTypeDefString;
					checkResult( spTypeDef->QueryInterface(IID_IAAFTypeDefString, 
												(void**)&spTypeDefString) );

					aafUInt32 bufSize = 
						//don't forget the NULL char, and factoring in the size of a wchar
						   (wcslen(TEST_NAME)+1) * sizeof(aafCharacter); 

					aafCharacter *nameBuf = new aafCharacter[bufSize];
					checkResult( spTypeDefString->GetElements(pPropVal, (aafMemPtr_t)nameBuf, bufSize) );
					checkExpression( wcscmp(TEST_NAME, nameBuf) == 0, AAFRESULT_TEST_FAILED );
					delete [] nameBuf;
				}

				//Done ...........

				//release typedef
				pTypeDef->Release();

				//release defintion + value
				pPropDef->Release();
				pPropVal->Release();
				
				//release this prop
				pProp->Release();
				pProp = NULL;

			}//while prop count
		}//while mob iter
	}//try
	
	catch (HRESULT & rResult)
	{
		hr = rResult;
	}
	
	//Release the rest ...
	if (pEnum) 
		pEnum->Release();
	
	if (pObj) 
		pObj->Release();
	if (pCMob) 
		pCMob->Release();
	if (pMob)
		pMob->Release();
	if (pMobIter)
		pMobIter->Release();
	if (pHeader)
		pHeader->Release();
	
	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
	
}

extern "C" HRESULT CAAFProperty_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFProperty_test(
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

	}
	catch (...)
	{
		cerr << "CAAFProperty_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}

