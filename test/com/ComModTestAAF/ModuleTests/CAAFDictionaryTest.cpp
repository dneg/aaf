// @doc INTERNAL
// @com This file implements the module test for CAAFDictionary
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
 *  prior written permission of Avid Technology, Inc.
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

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#ifndef __AAFSmartPointer_h__
#include "AAFSmartPointer.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"

#define kNumComponents	5


typedef IAAFSmartPointer<IAAFClassDef>      IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFComponent>     IAAFComponentSP;
typedef IAAFSmartPointer<IAAFFiller>        IAAFFillerSP;
typedef IAAFSmartPointer<IAAFObject>        IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef>   IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>       IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>    IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>          IUnknownSP;

// {69E9DEB3-4130-11d3-843E-00600832ACB8}
static aafUID_t kClassAUID_NewFill = 
{ 0x69e9deb3, 0x4130, 0x11d3, { 0x84, 0x3e, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {69E9DEB4-4130-11d3-843E-00600832ACB8}
static const aafUID_t kPropAUID_NewFill_Odor = 
{ 0x69e9deb4, 0x4130, 0x11d3, { 0x84, 0x3e, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


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



static void RegisterNewClass (IAAFDictionary * pDictionary)
{
  //
  // Create a new kind of filler with no new properties.
  //

  // Look up parent class
  IAAFClassDefSP pFillClass;
  checkResult (pDictionary->LookupClass (&AUID_AAFFiller, &pFillClass));
  assert (pFillClass);

  // Create new object for our new filler class, and initialize it.
  IAAFClassDefSP pNewFillClass;
  checkResult (pDictionary->CreateInstance(&AUID_AAFClassDef,
										   IID_IAAFClassDef,
										   (IUnknown **)&pNewFillClass));
  checkResult (pNewFillClass->Initialize ((aafUID_t*)&kClassAUID_NewFill,
										  pFillClass,
										  L"New Filler"));

  // Get type def for uint32
  IAAFTypeDefSP ptd;
  checkResult (pDictionary->LookupType ((aafUID_t*)&kAAFTypeID_UInt32,
										&ptd));
  assert (ptd);

  // Initialize new property
  checkResult
	(pNewFillClass->AppendNewPropertyDef ((aafUID_t*)&kPropAUID_NewFill_Odor,
										  L"Odor",
										  ptd,
										  AAFFalse,  // mandatory
										  0));

  // Register it in the dictionary.
  checkResult (pDictionary->RegisterClass (pNewFillClass));
}
										  


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFDictionary Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*		pMob = NULL;
	IAAFMobSlot*	pMobSlot = NULL;
	IAAFSequence*	pSequence = NULL;
	IAAFSegment*	pSegment = NULL;
	IAAFComponentSP	pComponent;
	aafUID_t		NewMobID;
	int				i;
	HRESULT			hr = S_OK;
	
	
	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// Create a new class, and register it in the dictionary.
		RegisterNewClass (pDictionary);

		// Create a Composition Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
			IID_IAAFMob, 
			(IUnknown **)&pMob));
		
		checkResult(CoCreateGuid((GUID *)&NewMobID));
		checkResult(pMob->SetMobID(&NewMobID));
		checkResult(pMob->SetName(L"AAFDictionaryTest"));
		
		// Add mob slot w/ Sequence
		checkResult(pDictionary->CreateInstance(&AUID_AAFSequence,
			IID_IAAFSequence, 
			(IUnknown **)&pSequence));		
		checkResult(pSequence->Initialize((aafUID_t*)&DDEF_Picture));
		
		//
		//	Add some segments.  Need to test failure conditions
		//	(i.e. starting/ending w/ transition, two trans back
		//	to back).
		//
		for(i = 0; i < kNumComponents; i++)
		{
			aafLength_t		len = 10;
			
			// For the first component, make it our extended filler.
			if(i == 0)
			{
			  checkResult
				(pDictionary->CreateInstance((aafUID_t*)&kClassAUID_NewFill,
											 IID_IAAFComponent, 
											 (IUnknown**)&pComponent));
			  checkResult(pComponent->SetDataDef((aafUID_t*)&DDEF_PictureWithMatte));
			}
			else
			{
			  checkResult
				(pDictionary->CreateInstance(&AUID_AAFFiller,
											 IID_IAAFComponent, 
											 (IUnknown**)&pComponent));

				checkResult(pComponent->SetDataDef((aafUID_t*)&DDEF_Picture));
			}

			checkResult(pComponent->SetLength(&len));
			checkResult(pSequence->AppendComponent(pComponent));
			
			// For our first component, set the 'odor' value.  Must be
			// done after the component has been inserted in sequence.
			if (i == 0)
			  {
				// Set the odor value.
				//
				// 1) Get type def for uint32
				IAAFTypeDefSP ptd;
				checkResult (pDictionary->LookupType ((aafUID_t*)&kAAFTypeID_UInt32,
													  &ptd));
				assert (ptd);
				IAAFTypeDefIntSP pTDUint32;
				checkResult(ptd->QueryInterface (IID_IAAFTypeDefInt,
												 (void **)&pTDUint32));
				assert (pTDUint32);

				// 2) Create a property value for the odor property, and
				//    set it to 42.
				IAAFPropertyValueSP pVal;
				const aafUInt32 odorValue = 42;
				checkResult (pTDUint32->CreateValue ((aafMemPtr_t) &odorValue,
													 sizeof (odorValue),
													 &pVal));

				// 3) Look up the property def for the odor property in
				//    the new fill class.
				IAAFClassDefSP pNewFillClass;
				checkResult (pDictionary->LookupClass ((aafUID_t*)&kClassAUID_NewFill,
													   &pNewFillClass));
				IAAFPropertyDefSP pPropDef;
				checkResult (pNewFillClass->LookupPropertyDef ((aafUID_t*)&kPropAUID_NewFill_Odor,
															   &pPropDef));

				// 4) Get IAAFObject interface for new fill object, and
				//    set the odor property.
				IAAFObjectSP pObj;
				checkResult(pComponent->QueryInterface (IID_IAAFObject,
														(void **)&pObj));
				checkResult (pObj->SetPropertyValue (pPropDef, pVal));
			  }
		}
		checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		
		checkResult(pMob->AppendNewSlot(pSegment, 1, L"AAF Test Sequence", &pMobSlot));
		
		pMobSlot->Release();
		pMobSlot = NULL;
		
		pSegment->Release();
		pSegment = NULL;
		
		// Add the master mob to the file and cleanup
		pHeader->AppendMob(pMob);
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFComponent*		pComp = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFDataDef*		pDataDef = NULL;
	IAAFSequence*		pSequence = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	aafNumSlots_t		numMobs;
	aafSearchCrit_t		criteria;
	HRESULT				hr = S_OK;
	
	
	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		
		// Validate that there is only one composition mob.
		checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// Check a data definition from a composition MOB in order to test weak references
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
					
		// Enumerate the first MOB slot for this MOB
		checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
		checkResult(pSlotIter->NextOne(&pSlot));
				
		checkResult(pSlot->GetSegment(&pSegment));
		checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
		checkResult(pSequence->EnumComponents(&pCompIter));
		checkResult(pCompIter->NextOne(&pComp));
		
		aafUID_t	dataDef, pwmID = DDEF_PictureWithMatte;
		aafBool		testBool;
				
		checkResult(pComp->GetDataDef(&dataDef));
		checkResult(pDictionary->LookupDataDefintion(&dataDef, &pDataDef));
		checkResult(pDataDef->IsSoundKind(&testBool));
		checkExpression(testBool == AAFFalse, AAFRESULT_TEST_FAILED);
		
		checkResult(pDataDef->IsDataDefOf(&pwmID, &testBool));
		checkExpression(testBool == AAFTrue, AAFRESULT_TEST_FAILED);
		
		// Make sure first component is a filler, and is our extended
		// class.  To do that, we'll compare the class def we looked
		// up in the dict, with the one we got from the new object.
		//
		// First get the class from the object.
		IAAFFillerSP pFill;
		checkResult(pComp->QueryInterface(IID_IAAFFiller,
										  (void **) &pFill));
		assert (pFill);

		IAAFObjectSP pObj;
		checkResult(pFill->QueryInterface(IID_IAAFObject,
										 (void **) &pObj));
		assert (pObj);
		IAAFClassDefSP pClassFromObj;
		checkResult (pObj->GetDefinition (&pClassFromObj));
		assert (pClassFromObj);
		IUnknownSP pUnkFromObj;
		checkResult(pClassFromObj->QueryInterface(IID_IUnknown,
												  (void **) &pUnkFromObj));
		
		// Now get the class from the dict
		IAAFClassDefSP pClassFromDict;
		checkResult (pDictionary->LookupClass ((aafUID_t*)&kClassAUID_NewFill,
											   &pClassFromDict));
		assert (pClassFromDict);
		IUnknownSP pUnkFromDict;
		checkResult(pClassFromDict->QueryInterface(IID_IUnknown,
												   (void **) &pUnkFromDict));

		// Compare class from object with class from dict.  Compare
		// using IUnknown pointers.
		assert (((IUnknown*)pUnkFromObj) ==
				((IUnknown*)pUnkFromDict));


		// Get the 'odor' property from our new fill clip.  Make sure
		// it is set to the value we think it should be ('42').
		//
		// First get the property def from the class.
		IAAFPropertyDefSP pPropDef;
		checkResult (pClassFromObj->LookupPropertyDef ((aafUID_t*)&kPropAUID_NewFill_Odor,
													   &pPropDef));
		//
		// Get the property value from the object
		IAAFPropertyValueSP pPropVal;
		checkResult (pObj->GetPropertyValue (pPropDef, &pPropVal));
		// 
		// We know the property is int32; get the int32 type def
		IAAFTypeDefSP ptd;
		checkResult (pDictionary->LookupType ((aafUID_t*)&kAAFTypeID_UInt32,
											  &ptd));
		IAAFTypeDefIntSP pTDUint32;
		checkResult(ptd->QueryInterface(IID_IAAFTypeDefInt,
										(void **) &pTDUint32));
		assert (pTDUint32);
		//
		// Ask the typedef to interpret this property value for us.
		aafUInt32 odorValue = 0;
		checkResult (pTDUint32->GetInteger (pPropVal,
											(aafMemPtr_t) &odorValue,
											sizeof (odorValue)));
		//
		// make sure it's what we expect.
		assert (42 == odorValue);

		pComp->Release();
		pComp = NULL;
		pDataDef->Release();
		pDataDef = NULL;
		
		pCompIter->Release();
		pCompIter = NULL;
		
		pSequence->Release();
		pSequence = NULL;
		
		pSegment->Release();
		pSegment = NULL;
		
		pSlot->Release();
		pSlot = NULL;
		
		
		pSlotIter->Release();
		pSlotIter = NULL;
		
		pMob->Release();
		pMob = NULL;
		
		
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (pComp)
		pComp->Release();
	
	if (pCompIter)
		pCompIter->Release();
	
	if (pDataDef)
		pDataDef->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pSlot)
		pSlot->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pSlotIter)
		pSlotIter->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pMobIter)
		pMobIter->Release();
	
	if (pHeader) pHeader->Release();
	
	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}
	
	
	return 	hr;
}

extern "C" HRESULT CAAFDictionary_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFDictionaryTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
		  hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFDictionary_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFDictionary tests have not been implemented:" << endl; 
		cout << "     RegisterClass" << endl; 
		cout << "     LookupClass" << endl; 
		cout << "     GetClassDefinitions" << endl; 
		cout << "     RegisterType" << endl; 
		cout << "     LookupType" << endl; 
		cout << "     GetTypeDefinitions" << endl; 
		cout << "     RegisterDataDefintion" << endl; 
		cout << "     GetDataDefinitions" << endl; 
		cout << "     RegisterOperationDefinition" << endl; 
		cout << "     LookupOperationDefinition" << endl; 
		cout << "     GetOperationDefinitions" << endl; 
		cout << "     RegisterParameterDefinition" << endl; 
		cout << "     LookupParameterDefinition" << endl; 
		cout << "     GetParameterDefinitions" << endl; 
		cout << "     RegisterCodecDefinition" << endl; 
		cout << "     LookupCodecDefinition" << endl; 
		cout << "     GetCodecDefinitions" << endl; 
		cout << "     RegisterContainerDefinition" << endl; 
		cout << "     LookupContainerDefinition" << endl; 
		cout << "     GetContainerDefinitions" << endl; 
		cout << "     RegisterInterpolationDefinition" << endl; 
		cout << "     LookupInterpolationDefinition" << endl; 
		cout << "     GetInterpolationDefinitions" << endl; 
		cout << "     RegisterPluginDescriptor" << endl; 
		cout << "     LookupPluginDescriptor" << endl; 
		cout << "     GetPluginDescriptors" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
