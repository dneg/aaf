// @doc INTERNAL
// @com This file implements the module test for CAAFProperty
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
	IAAFCompositionMob * pCMob = NULL;
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
		ProductInfo.productName = L"AAFProperty Test";
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

extern "C" HRESULT CAAFProperty_test()
{
	HRESULT hr = AAFRESULT_SUCCESS;
	aafWChar * pFileName = L"AAFPropertyTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );

	}
	catch (...)
	{
		cerr << "CAAFProperty_test...Caught general C++"
			" exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	
	return hr;
}

