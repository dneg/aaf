// @doc INTERNAL
// @com This file implements the module test for CAAFKLVData
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

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFPropertyDefs.h"

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFObject> IAAFObjectSP;
typedef IAAFSmartPointer<IAAFClassDef> IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef> IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef> IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefOpaque> IAAFTypeDefOpaqueSP;
typedef IAAFSmartPointer<IAAFKLVData> IAAFKLVDataSP;

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
static const aafUID_t *	slotDDefs[5] = {&DDEF_Picture, &DDEF_Sound, &DDEF_Sound, &DDEF_Picture, &DDEF_Picture};
static aafLength_t	slotsLength[5] = { 297, 44100, 44100, 44100, 44100};

static aafSourceRef_t sourceRef; 

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x2DFB8BAF, 0x0972, 0x11d4, 0xA3, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6A}};

static const 	aafMobID_t	TEST_referencedMobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x2DFB8BB0, 0x0972, 0x11d4, 0xA3, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6A}};

// {2DFB8BB2-0972-11d4-A357-009027DFCA6A}
static const aafUID_t TEST_KLV = 
{ 0x2dfb8bb2, 0x972, 0x11d4, { 0xa3, 0x57, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

static const char KLVsmiley[] =        /* 16x16 smiley face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "****************"
  "****************"
  " ** ******** ** "
  " *** ****** *** "
  "  ***  **  ***  "
  "   ****  ****   "
  "    ********    "
  "      ****     ";

static const char KLVfrowney[] =        /* 16x16 frowney face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "*******  *******"
  "*****  **  *****"
  " *** ****** *** "
  " ** ******** ** "
  "  ************  "
  "   **********   "
  "    ********    "
  "      ****     ";

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

// Use the direct access interfaces to extract an opaque
// handle from the given KLVData object.
static void CreateOpaqueHandleFromKLVData(
  IAAFKLVData* pData,
  aafDataBuffer_t &opaqueHandle,
  aafUInt32 &handleSize)
{
  checkExpression (NULL != pData, E_INVALIDARG);

  IAAFObjectSP pObject;
  checkResult(pData->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));

  IAAFPropertyDefSP pPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_KLVData_Value, &pPropertyDef));

  IAAFPropertyValueSP pPropertyValue;
  checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));

  IAAFTypeDefSP pTypeDef;
  checkResult(pPropertyDef->GetTypeDef(&pTypeDef));

  IAAFTypeDefOpaqueSP pTypeDefOpaque;
  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&pTypeDefOpaque));

  //
  // Get the expected size of the handle.
  //
  checkResult(pTypeDefOpaque->GetHandleBufLen(pPropertyValue, &handleSize));

  //
  // Allocate the handle. NOTE: The caller must delete this memory!
  //
  opaqueHandle = new aafUInt8[handleSize];
  checkExpression(NULL != opaqueHandle, AAFRESULT_NOMEMORY);

  aafUInt32 bytesRead = 0;
  checkResult(pTypeDefOpaque->GetHandle(pPropertyValue, handleSize, opaqueHandle, &bytesRead));
  checkExpression(handleSize == bytesRead, AAFRESULT_TEST_FAILED);
}


// Use the direct access interfaces to write an opaque
// handle to the given KLVData object.
static void InitializeKLVDataFromOpaqueHandle(
  IAAFKLVData *pData,
  aafDataBuffer_t opaqueHandle,
  aafUInt32 handleSize)
{
  checkExpression (NULL != opaqueHandle && NULL != pData && 0 < handleSize, E_INVALIDARG);


  IAAFObjectSP pObject;
  checkResult(pData->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));

  IAAFPropertyDefSP pPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_KLVData_Value, &pPropertyDef));

  IAAFPropertyValueSP pPropertyValue;
  checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));

  IAAFTypeDefSP pTypeDef;
  checkResult(pPropertyDef->GetTypeDef(&pTypeDef));

  IAAFTypeDefOpaqueSP pTypeDefOpaque;
  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&pTypeDefOpaque));

  checkResult(pTypeDefOpaque->SetHandle(pPropertyValue, handleSize, opaqueHandle));
  checkResult(pObject->SetPropertyValue(pPropertyDef, pPropertyValue));
}



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *			pFile = NULL;
	bool				bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob				*pMob = NULL;
	IAAFMob*			pRMob = NULL;
	IAAFKLVData*		pData = NULL;
	IAAFTypeDef*		pBaseType = NULL;
	IAAFMasterMob*		pReferencedMob = NULL;
	IAAFCompositionMob* pCompMob = NULL;
	IAAFTimelineMobSlot	*newSlot = NULL;
	IAAFSegment			*seg = NULL;
	IAAFComponent		*pComponent = NULL;
	IAAFSourceClip		*sclp = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT				hr = S_OK;
	aafUInt32			numComments;
  aafDataBuffer_t opaqueHandle = NULL;
  aafUInt32 opaqueHandleSize = 0;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFKLVData Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		checkResult(pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &pBaseType));
		checkResult(pDictionary->RegisterKLVDataKey(TEST_KLV, pBaseType));

		//Make the first mob
		long	test;

		// Create a  Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));

		checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFKLVDataTest"));

		// append a KLVData to this mob
		checkResult(defs.cdKLVData()->
					CreateInstance(IID_IAAFKLVData, 
								   (IUnknown **)&pData));
		checkResult(pData->Initialize(TEST_KLV, sizeof(KLVfrowney), (unsigned char *)KLVfrowney));
		checkResult(pData->SetValue(sizeof(KLVsmiley), (unsigned char *)KLVsmiley));
		checkResult(pMob->AppendKLVData (pData));

    // Test extracting an opaque handle from the KLVData value property.
    // NOTE: This should be a "smiley" value.
    CreateOpaqueHandleFromKLVData(pData, opaqueHandle, opaqueHandleSize);  
		pData->Release();
		pData = NULL;

    // append a second KLVData to this mob, count, delete it, count again
		checkResult(defs.cdKLVData()->
					CreateInstance(IID_IAAFKLVData, 
								   (IUnknown **)&pData));
		checkResult(pData->Initialize(TEST_KLV, sizeof(KLVfrowney), (unsigned char *)KLVfrowney));
		checkResult(pData->SetValue(sizeof(KLVsmiley), (unsigned char *)KLVsmiley));
		checkResult(pMob->AppendKLVData (pData));
		checkResult(pMob->CountKLVData(&numComments));
		checkExpression(2 == numComments, AAFRESULT_TEST_FAILED);
		checkResult(pMob->RemoveKLVData (pData));
		checkResult(pMob->CountKLVData(&numComments));
		checkExpression(1 == numComments, AAFRESULT_TEST_FAILED);
    pData->Release();
		pData = NULL;
    
    // Create a new "frowney" KLVData value and then reinitialize it with the
    // "smiley" opaque handle created above.
		checkResult(defs.cdKLVData()->
					CreateInstance(IID_IAAFKLVData, 
								   (IUnknown **)&pData));
		checkResult(pData->Initialize(TEST_KLV, sizeof(KLVfrowney), (unsigned char *)KLVfrowney));
 		checkResult(pMob->AppendKLVData (pData));
    InitializeKLVDataFromOpaqueHandle(pData, opaqueHandle, opaqueHandleSize);    
    pData->Release();
		pData = NULL;
		checkResult(pMob->CountKLVData(&numComments));
	  checkExpression(2 == numComments, AAFRESULT_TEST_FAILED);

		// Create a master mob to be referenced
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, 
								   (IUnknown **)&pReferencedMob));

		checkResult(pReferencedMob->QueryInterface(IID_IAAFMob, (void **)&pRMob));
		checkResult(pRMob->SetMobID(TEST_referencedMobID));
		checkResult(pRMob->SetName(L"AAFKLVDataTest::ReferencedMob"));

		// Add some slots
		for(test = 0; test < 5; test++)
		{
 			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&sclp));		
			// Set the properties for the SourceClip
			sourceRef.sourceID = TEST_referencedMobID;
			sourceRef.sourceSlotID = 0;
			sourceRef.startTime = 0;
			IAAFDataDefSP pDataDef;
			checkResult(pDictionary->LookupDataDef(*slotDDefs[test], &pDataDef));
			checkResult(sclp->Initialize(pDataDef, slotsLength[test], sourceRef));

			checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));

			checkResult(sclp->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(defs.cdKLVData()->
						CreateInstance(IID_IAAFKLVData, 
								   (IUnknown **)&pData));
			checkResult(pData->Initialize(TEST_KLV, sizeof(KLVfrowney), (unsigned char *)KLVfrowney));
			checkResult(pComponent->AppendKLVData (pData));
			pData->Release();
			pData = NULL;
			// append a second KLVData to this component, count, delete it, count again
			checkResult(defs.cdKLVData()->
					CreateInstance(IID_IAAFKLVData, 
								   (IUnknown **)&pData));
			checkResult(pData->Initialize(TEST_KLV, sizeof(KLVfrowney), (unsigned char *)KLVfrowney));
			checkResult(pData->SetValue(sizeof(KLVsmiley), (unsigned char *)KLVsmiley));
			checkResult(pComponent->AppendKLVData (pData));
			checkResult(pComponent->CountKLVData(&numComments));
			checkExpression(2 == numComments, AAFRESULT_TEST_FAILED);
			checkResult(pComponent->RemoveKLVData (pData));
			checkResult(pComponent->CountKLVData(&numComments));
			checkExpression(1 == numComments, AAFRESULT_TEST_FAILED);
			pData->Release();
			pData = NULL;

			//
			pComponent->Release();
			pComponent = NULL;
			
			aafRational_t editRate = { 0, 1};
			checkResult(pMob->AppendNewTimelineSlot (editRate,
													 seg,
													 test+1,
													 slotNames[test],
													 0,
													 &newSlot));

			newSlot->Release();
			newSlot = NULL;

			seg->Release();
			seg = NULL;

			sclp->Release();
			sclp = NULL;
		}

		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pRMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (opaqueHandle)
    delete [] opaqueHandle;

  if (newSlot)
    newSlot->Release();

  if (seg)
    seg->Release();

  if (sclp)
    sclp->Release();

  if (pCompMob)
	  pCompMob->Release();

  if (pMob)
    pMob->Release();

  if (pComponent)
    pComponent->Release();

  if (pData)
    pData->Release();
  if (pBaseType)
    pBaseType->Release();

  if (pReferencedMob)
	  pReferencedMob->Release();

  if (pRMob)
	  pRMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
	  {
		  pFile->Save();
		  pFile->Close();
	  }
    pFile->Release();
  }

  return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFSegment *				pSegment = NULL;
	IAAFComponent *				pComponent = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IEnumAAFKLVData* pKLVIterator = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFTypeDef*		pBaseType = NULL;
	IAAFKLVData*		pKLVData = NULL;
	aafUInt32			testKLVLen;
	aafUID_t			testKey;

	IAAFMobSlot		*slot = NULL;
	aafNumSlots_t	numMobs, n, slt;
	aafUInt32		numComments, bytesRead, com;
	HRESULT						hr = S_OK;
	char			Value[sizeof(KLVsmiley)];
	aafSearchCrit_t	criteria;


	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
  		checkResult(pFile->GetHeader(&pHeader));

		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;

		checkResult(pHeader->GetDictionary(&pDictionary));
		checkResult(pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &pBaseType));
		checkResult(pDictionary->RegisterKLVDataKey(TEST_KLV, pBaseType));

		checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);


		checkResult(pHeader->GetMobs (&criteria, &mobIter));

		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;

			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));

			// Check for comments
			checkResult(aMob->CountKLVData(&numComments));
			checkExpression(2 == numComments, AAFRESULT_TEST_FAILED);
			checkResult(aMob->GetKLVData(&pKLVIterator));
			for(com = 0; com < numComments; com++)
			{
				checkResult(pKLVIterator->NextOne(&pKLVData));
				checkResult(pKLVData->GetValueBufLen(&testKLVLen));
				checkExpression(sizeof(KLVsmiley) == testKLVLen, AAFRESULT_TEST_FAILED);
				checkResult(pKLVData->GetValue( sizeof(Value), (unsigned char *)Value, &bytesRead));
				checkExpression(memcmp(Value, KLVsmiley, sizeof(KLVsmiley)) == 0, AAFRESULT_TEST_FAILED);
				checkResult(pKLVData->GetKey(&testKey));
				checkExpression(memcmp(&testKey, &TEST_KLV, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
				pKLVData->Release();
				pKLVData = NULL;
			}
			pKLVIterator->Release();
			pKLVIterator = NULL;

			checkResult(aMob->CountSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);

			checkResult(aMob->GetSlots(&slotIter));

			for(slt = 0; slt < numSlots; slt++)
			{
				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetName (slotName, sizeof(slotName)));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (wcscmp(slotName, slotNames[slt]) == 0, AAFRESULT_TEST_FAILED);
				checkResult(slot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
				checkResult(pComponent->CountKLVData(&numComments));
				checkExpression(1 == numComments, AAFRESULT_TEST_FAILED);
				checkResult(pComponent->GetKLVData(&pKLVIterator));
				for(com = 0; com < numComments; com++)
				{
					checkResult(pKLVIterator->NextOne(&pKLVData));
					checkResult(pKLVData->GetValueBufLen(&testKLVLen));
					checkExpression(sizeof(KLVfrowney) == testKLVLen, AAFRESULT_TEST_FAILED);
					checkResult(pKLVData->GetValue( sizeof(Value), (unsigned char *)Value, &bytesRead));
					checkExpression(memcmp(Value, KLVfrowney, sizeof(KLVfrowney)) == 0, AAFRESULT_TEST_FAILED);
					pKLVData->Release();
					pKLVData = NULL;
				}
				pKLVIterator->Release();
				pKLVIterator = NULL;

				slot->Release();
				slot = NULL;
				pSegment->Release();
				pSegment = NULL;
				pComponent->Release();
				pComponent = NULL;
			}

			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
  if (pKLVData)
    pKLVData->Release();

  if (pKLVIterator)
    pKLVIterator->Release();

  if (slot)
    slot->Release();

  if (slotIter)
    slotIter->Release();

  if (pSegment)
    pSegment->Release();

  if (pComponent)
    pComponent->Release();

  if (aMob)
    aMob->Release();

  if (mobIter)
    mobIter->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pBaseType)
    pBaseType->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
      pFile->Close();
     pFile->Release();
  }

  return hr;
}
 

extern "C" HRESULT CAAFKLVData_test(testMode_t mode);
extern "C" HRESULT CAAFKLVData_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFKLVDataTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CKLVData_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following AAFKLVDataTest tests have not been implemented:" << endl; 
//		cout << "     Component::RemoveKLVData" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}
  return hr;
}


    
    
    
    
    

