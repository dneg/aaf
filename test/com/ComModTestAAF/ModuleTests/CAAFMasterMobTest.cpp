// @doc INTERNAL
// @com This file implements the module test for CAAFMasterMob
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

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static aafWChar *		slotNames[NumMobSlots] = { L"VIDEO SLOT", L"AUDIO SLOT1", L"AUDIO SLOT2"};
static const aafUID_t *	slotDDefs[NumMobSlots] = {&DDEF_Picture, &DDEF_Sound, &DDEF_Sound};
static aafRational_t	slotRates[NumMobSlots] = { {297,1}, {44100, 1}, {44100, 1}};
static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kAAFVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kAAFPALSignal;
static aafTapeFormatType_t TapeFormat = kAAFVHSFormat;
static aafUInt32 TapeLength = 3200 ;

#define TAPE_MOB_OFFSET	10
#define TAPE_MOB_LENGTH	60
#define TAPE_MOB_NAME	L"A Tape Mob"

//--cf  This will require some work!!! 
static const 	aafMobID_t	TEST_Master_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x07c01e92, 0x0403, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

static const 	aafMobID_t	TEST_Source_MobIDs[NumMobSlots] =
{	//start mobid block
	
	//first id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
		0x13, 0x00, 0x00, 0x00,
	{0x11ee08d4, 0x0403, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}},
	
	//second id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x7f0b27bc, 0x0403, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}},
	
	//third id
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x8d6c568c, 0x0403, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}
	
};	//end mobid block




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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMasterMob Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
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
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*		pMob = NULL;
	IAAFMasterMob*	pMasterMob = NULL;
	IAAFSourceMob* pSrcMob = NULL;
	IAAFSourceMob* pTapeMob = NULL;
	IAAFEssenceDescriptor*	pDesc = NULL;
	IAAFMob*				pTempMob = NULL;
	HRESULT			hr = S_OK;
	long			test;
	aafSourceRef_t	ref;
	aafMobID_t		tapeMobID;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFTapeDescriptor*			pTapeDesc = NULL;

	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));

		CAAFBuiltinDefs defs (pDictionary);
				
		// Create a Master Mob
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		// Set the IAAFMob properties
		checkResult(pMob->SetMobID(TEST_Master_MobID));
		checkResult(pMob->SetName(MobName));
		
		checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob));
		checkResult(pMasterMob->Initialize());
		
		// Create source mob to associate with our MasterMob.
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
			(IUnknown **)&pTapeMob));		
		hr = defs.cdTapeDescriptor()->
		  CreateInstance(IID_IAAFTapeDescriptor, 
						 (IUnknown **)&pTapeDesc);		
		if (AAFRESULT_SUCCESS == hr)
		{
			hr = pTapeDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pTapeMob->SetEssenceDescriptor(pEssDesc);
				if (AAFRESULT_SUCCESS == hr)
				{
					hr = pTapeDesc->SetTapeManufacturer( Manufacturer );
					if (AAFRESULT_SUCCESS == hr )
					{
						hr = pTapeDesc->SetTapeModel( Model );
						if (AAFRESULT_SUCCESS == hr )
						{
							hr = pTapeDesc->SetTapeFormFactor( FormFactor );
							if (AAFRESULT_SUCCESS == hr )
							{
								hr = pTapeDesc->SetSignalType( VideoSignalType );
								if (AAFRESULT_SUCCESS == hr )
								{
									hr = pTapeDesc->SetTapeFormat( TapeFormat );
									if (AAFRESULT_SUCCESS == hr )
										hr = pTapeDesc->SetTapeLength( TapeLength );
								}
							}
						}
					}
				}
				pEssDesc->Release();
				pEssDesc = NULL;
			}
			pTapeDesc->Release();
			pTapeDesc = NULL;
		}
		for (test = 0; test < NumMobSlots; test++)
		{
		  IAAFDataDefSP pDataDef;
		  checkResult (pDictionary->LookupDataDef (*slotDDefs[test], &pDataDef));
		  checkResult(pTapeMob->AddNilReference (test, TAPE_MOB_LENGTH, pDataDef, slotRates[test]));
		}
		checkResult(pTapeMob->QueryInterface(IID_IAAFMob, (void **) &pTempMob));
		checkResult(pTempMob->SetName(TAPE_MOB_NAME));
		checkResult(pTempMob->GetMobID(&tapeMobID));
		checkResult(pHeader->AddMob(pTempMob));
		pTempMob->Release();
		pTempMob = NULL;
		
		// Add some slots
		for (test = 0; test < NumMobSlots; test++)
		{
			// Create source mob to associate with our MasterMob.
			checkResult(defs.cdSourceMob()->
						CreateInstance(IID_IAAFSourceMob, 
									   (IUnknown **)&pSrcMob));		
			
			ref.sourceID = tapeMobID;
			ref.sourceSlotID = test;
			ref.startTime = TAPE_MOB_OFFSET;
			IAAFDataDefSP pDDef;
			checkResult(pDictionary->LookupDataDef(*slotDDefs[test], &pDDef));
			if(test == 0)
			{
				checkResult(pSrcMob->NewPhysSourceRef (slotRates[test],
													  test,
													  pDDef,
													  ref,
													  TAPE_MOB_LENGTH));
			}
			else
			{
				checkResult(pSrcMob->AppendPhysSourceRef (slotRates[test],
													  test,
													  pDDef,
													  ref,
													  TAPE_MOB_LENGTH));
			}
			
			// Create concrete subclass of EssenceDescriptor
			checkResult(defs.cdAIFCDescriptor()->
						CreateInstance(IID_IAAFEssenceDescriptor, 
									   (IUnknown **)&pDesc));	

			IAAFAIFCDescriptor*			pAIFCDesc = NULL;
			checkResult(pDesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
			checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
			pAIFCDesc->Release();
			pAIFCDesc = NULL;

			checkResult(pSrcMob->SetEssenceDescriptor(pDesc));
			pDesc->Release();
			pDesc = NULL;
			
			// Append source MOB to header
			checkResult(pSrcMob->QueryInterface(IID_IAAFMob, (void **) &pTempMob));
			checkResult(pTempMob->SetMobID(TEST_Source_MobIDs[test]));
			checkResult(pTempMob->SetName(L"source mob"));
			
			checkResult(pHeader->AddMob(pTempMob));
			pTempMob->Release();
			pTempMob = NULL;
			
			IAAFDataDefSP pDataDef;
			checkResult (pDictionary->LookupDataDef (*slotDDefs[test], &pDataDef));
			checkResult(pMasterMob->AddMasterSlot(pDataDef, test, pSrcMob, test+1, slotNames[test]));
			
			pSrcMob->Release();
			pSrcMob = NULL;
		}
		
		// Add the master mob to the file and cleanup
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pTempMob)
		pTempMob->Release();
	
	if (pEssDesc)
		pEssDesc->Release();
	
	if (pTapeDesc)
		pTapeDesc->Release();
	

	if (pDesc)
		pDesc->Release();
	
	if (pSrcMob)
		pSrcMob->Release();
	
	if (pTapeMob)
		pTapeMob->Release();
	
	if (pMasterMob)
		pMasterMob->Release();
	
	if (pMob)
		pMob->Release();
	
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

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
  bool bFileOpen = false;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob = NULL;
	IAAFSegment*		pSeg = NULL;
	IAAFMasterMob*		pMasterMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot;
	aafWChar*			pTapeName = NULL;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr = S_OK;
	aafMediaCriteria_t	mediaCriteria;


  try
  {
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
    bFileOpen = true;

    // Validate that there is on one master mob in the test file.
	  checkResult(pHeader->CountMobs(kAAFMasterMob, &numMobs));
    checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over Master MOBs
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFMasterMob;
    checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
	  {
		  aafWChar			name[500];
		  aafNumSlots_t		numSlots = 0;
		  aafMobID_t				mobID;

		  // TODO: Test Master MOB specific methods here
		  checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob));

		  checkResult(pMob->GetName(name, sizeof(name)));
		  checkExpression(wcscmp(name, MobName) == 0, AAFRESULT_TEST_FAILED);

		  checkResult(pMob->GetMobID(&mobID));
		  checkExpression(0 == memcmp(&mobID, &TEST_Master_MobID, sizeof(mobID)), AAFRESULT_TEST_FAILED);

		  checkResult(pMob->CountSlots(&numSlots));
		  checkExpression(NumMobSlots == numSlots, AAFRESULT_TEST_FAILED);

//AAFRESULT STDMETHODCALLTYPE
 //   ImplAAFMasterMob::GetTapeNameBufLen (0,
//										 aafInt32*  pLen)
			unsigned long	s = 0;

			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafWChar			slotName[500];
				aafSlotID_t			slotID;
				aafNumSlots_t		numReps;
				aafUInt32			bufSize = 0;

        // Validate the slot name
				checkResult(pSlot->GetName(slotName, sizeof(slotName)));
				checkExpression(wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
        
        // Validate the slot id.
				checkResult(pSlot->GetSlotID(&slotID));
				checkExpression(slotID == s+1, AAFRESULT_TEST_FAILED);

				checkResult(pMasterMob->GetTapeNameBufLen(slotID, &bufSize));
				if (bufSize)
				{
					pTapeName = new aafWChar [bufSize];
					checkExpression(NULL != pTapeName, E_OUTOFMEMORY);

					checkResult(pMasterMob->GetTapeName(slotID, pTapeName, bufSize));
					delete [] pTapeName;
				}

				checkResult(pMasterMob->GetNumRepresentations(slotID, &numReps));
				checkExpression (numReps == 1, AAFRESULT_TEST_FAILED);

				checkResult(pMasterMob->GetRepresentation (slotID, 0, &pSeg));
				pSeg->Release();
				pSeg = NULL;

				mediaCriteria.type = kAAFAnyRepresentation;
				checkResult(pMasterMob->GetCriteriaSegment (slotID, &mediaCriteria, &pSeg));

				pSeg->Release();
				pSeg = NULL;
				
				pSlot->Release();
				pSlot = NULL;
				s++;
			}

	   pSlotIter->Release();
      pSlotIter = NULL;
	   pMob->Release();
      pMob = NULL;
	   pMasterMob->Release();
      pMasterMob = NULL;
	}

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
//  if (pTapeName)
//		delete [] pTapeName;

  if (pSlot)
    pSlot->Release();

  if (pMasterMob)
    pMasterMob->Release();

  if (pMob)
    pMob->Release();

  if (pMobIter)
    pMobIter->Release();

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
 

extern "C" HRESULT CAAFMasterMob_test(testMode_t mode);
extern "C" HRESULT CAAFMasterMob_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFMasterMobTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFMasterMob_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFMasterMob tests have not been implemented:" << endl; 
//		cout << "     GetCriteriaSegment" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}














