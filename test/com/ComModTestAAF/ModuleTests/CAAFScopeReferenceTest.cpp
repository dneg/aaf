// @doc INTERNAL
// @com This file implements the module test for CAAFScopeReference

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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFSmartPointer.h"
#include "AAFStoredObjectIDs.h"
#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"

typedef IAAFSmartPointer<IAAFComponent> IAAFComponentSP;
typedef IAAFSmartPointer<IAAFCompositionMob> IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFFile> IAAFFileSP;
typedef IAAFSmartPointer<IAAFFiller> IAAFFillerSP;
typedef IAAFSmartPointer<IAAFHeader> IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFMob> IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot> IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFNestedScope> IAAFNestedScopeSP;
typedef IAAFSmartPointer<IAAFScopeReference> IAAFScopeReferenceSP;
typedef IAAFSmartPointer<IAAFSegment> IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot> IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IEnumAAFMobs> IEnumAAFMobsSP;
typedef IAAFSmartPointer<IEnumAAFMobSlots> IEnumAAFMobSlotsSP;

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}

inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

// These two functions fill in the product version and product info structures,
// respectively, for the AAF files we will create
static void FillInProductVersion(aafProductVersion_t& v)
{
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
}

static void FillInProductInfo(aafProductIdentification_t& ProductInfo,
	aafProductVersion_t& v)
{
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFScopeReference Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	ProductInfo.productID = UnitTestProductID;
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

static const aafMobID_t	kTestMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xf41d3e80, 0x4939, 0x11d4, 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d }};

static const aafUInt32 kTestRelativeScope=0,kTestRelativeSlot=1;

static void CreateScopeReferenceFile(aafWChar *pFilename)
{
	aafProductVersion_t v;
	aafProductIdentification_t	ProductInfo;
	FillInProductVersion(v);
	FillInProductInfo(ProductInfo,v);

	// Remove the previous test file, if any.
	RemoveTestFile(pFilename);

	// Create new AAF file.
	IAAFFileSP pFile;
	checkResult(AAFFileOpenNewModify(pFilename,0,&ProductInfo, &pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	CAAFBuiltinDefs defs(pDictionary);

	// Create a Composition Mob
	IAAFCompositionMobSP pCompositionMob;
	checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFCompositionMob, 
		(IUnknown **)&pCompositionMob));

	IAAFMobSP pMob;
	checkResult(pCompositionMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	checkResult(pMob->SetMobID(kTestMobID));	

	// Create a scope reference via Create(), SetDataDef()
	IAAFScopeReferenceSP pScopeRef1;
	checkResult(defs.cdScopeReference()->CreateInstance(IID_IAAFScopeReference,
		(IUnknown**)&pScopeRef1));
	checkResult(pScopeRef1->Create(kTestRelativeScope,kTestRelativeSlot));
	IAAFComponentSP pComponent;
	checkResult(pScopeRef1->QueryInterface(IID_IAAFComponent,(void**)&pComponent));
	checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));

	// Verify that GetRelativeScope() and GetRelativeSlot() return correct values
	aafUInt32 returnedRelativeScope=0xff,returnedRelativeSlot=0xff;
	checkResult(pScopeRef1->GetRelativeScope(&returnedRelativeScope));
	checkResult(pScopeRef1->GetRelativeSlot(&returnedRelativeSlot));
	checkExpression(returnedRelativeScope==kTestRelativeScope);
	checkExpression(returnedRelativeSlot==kTestRelativeSlot);

	// Create a scope reference via Initialize()
	IAAFScopeReferenceSP pScopeRef2;
	checkResult(defs.cdScopeReference()->CreateInstance(IID_IAAFScopeReference,
		(IUnknown**)&pScopeRef2));
	checkResult(pScopeRef2->Initialize(defs.ddkAAFPicture(),kTestRelativeScope,
		kTestRelativeSlot));

	// Verify that GetRelativeScope() and GetRelativeSlot() return correct values
	returnedRelativeScope=0xff;
	checkResult(pScopeRef2->GetRelativeScope(&returnedRelativeScope));
	returnedRelativeSlot=0xff;
	checkResult(pScopeRef2->GetRelativeSlot(&returnedRelativeSlot));
	checkExpression(returnedRelativeScope==kTestRelativeScope);
	checkExpression(returnedRelativeSlot==kTestRelativeSlot);

	// Create a filler 
	IAAFFillerSP pFiller;
	checkResult(defs.cdFiller()->CreateInstance(IID_IAAFFiller, 
		(IUnknown **)&pFiller));
	checkResult(pFiller->Initialize(defs.ddkAAFPicture(),3665));

	// Add filler to composition Mob
	IAAFSegmentSP pSegment;
	checkResult(pFiller->QueryInterface(IID_IAAFSegment,
		(void **)&pSegment));
	IAAFTimelineMobSlotSP pTimelineMobSlot;
	aafRational_t editRate = { 0, 1};
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1460,
		L"Filler",0,&pTimelineMobSlot)); 

	// Add scope references to composition Mob
	checkResult(pScopeRef1->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,5671,
		L"ScopeReference",0,&pTimelineMobSlot)); 

	checkResult(pScopeRef2->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,5671,
		L"ScopeReference",0,&pTimelineMobSlot)); 

	// Add composition mob to file
	pHeader->AddMob(pMob);

	// Save & close file
	checkResult(pFile->Save());
	checkResult(pFile->Close());
}

static void ReadScopeReferenceFile(aafWChar *pFilename)
{
	IAAFFileSP pFile;
	checkResult(AAFFileOpenExistingRead(pFilename,0,&pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	// Make sure there is only one composition Mob
	aafUInt32 numMobs;
	checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	checkExpression(numMobs==1);

	// Get first composition Mob
	aafSearchCrit_t	criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFCompMob;
	IEnumAAFMobsSP pMobIter;
	checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	IAAFMobSP pMob;
	checkResult(pMobIter->NextOne(&pMob));

	// Enumerate over Mob slots
	IEnumAAFMobSlotsSP pSlotIter;
	checkResult(pMob->GetSlots(&pSlotIter));
	IAAFMobSlotSP pSlot;
	while (pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
	{
		// Check if current slot is a scope reference
		IAAFScopeReferenceSP pScopeRef;
		if(pSlotIter->QueryInterface(IID_IAAFScopeReference,
			(void**)&pScopeRef)==AAFRESULT_SUCCESS)
		{
			// Make sure scope reference is the same as when we created it
			aafUInt32 returnedRelativeScope,returnedRelativeSlot;
			checkResult(pScopeRef->GetRelativeScope(&returnedRelativeScope));
			checkResult(pScopeRef->GetRelativeSlot(&returnedRelativeSlot));
			checkExpression(returnedRelativeScope==kTestRelativeScope);
			checkExpression(returnedRelativeSlot==kTestRelativeSlot);
		}
	}

	pFile->Close();
}

extern "C" HRESULT CAAFScopeReference_test(testMode_t mode);
extern "C" HRESULT CAAFScopeReference_test(testMode_t mode)
{
	aafWChar *pTestFilename=L"ScopeReferenceTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			CreateScopeReferenceFile(pTestFilename);
		ReadScopeReferenceFile(pTestFilename);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}










