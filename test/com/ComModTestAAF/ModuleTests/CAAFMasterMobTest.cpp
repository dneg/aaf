// @doc INTERNAL
// @com This file implements the module test for CAAFMasterMob
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

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static aafWChar *		slotNames[NumMobSlots] = { L"VIDEO SLOT", L"AUDIO SLOT1", L"AUDIO SLOT2"};
static const aafUID_t *	slotDDefs[NumMobSlots] = {&DDEF_Picture, &DDEF_Sound, &DDEF_Sound};
static aafRational_t	slotRates[NumMobSlots] = { {297,1}, {44100, 1}, {44100, 1}};
static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kPALSignal;
static aafTapeFormatType_t TapeFormat = kVHSFormat;
static aafLength_t TapeLength = 3200 ;

static aafMobID_t		NewMobID;
#define TAPE_MOB_OFFSET	10
#define TAPE_MOB_LENGTH	60
#define TAPE_MOB_NAME	L"A Tape Mob"

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

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMasterMob Test";
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
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		
		// Create a Master Mob
		checkResult(pDictionary->CreateInstance(AUID_AAFMasterMob,
			IID_IAAFMob, 
			(IUnknown **)&pMob));
		
		// Set the IAAFMob properties
		checkResult(CoCreateGuid((GUID *)&NewMobID));
		aafMobID_t mobID;
		memcpy (&mobID, &NewMobID, sizeof (mobID));
		checkResult(pMob->SetMobID(mobID));
		checkResult(pMob->SetName(MobName));
		
		checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob));
		
		// Create source mob to associate with our MasterMob.
		checkResult(pDictionary->CreateInstance(AUID_AAFSourceMob,
			IID_IAAFSourceMob, 
			(IUnknown **)&pTapeMob));		
		hr = pDictionary->CreateInstance(AUID_AAFTapeDescriptor,
												IID_IAAFTapeDescriptor, 
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
			checkResult(pTapeMob->AddNilReference (test, TAPE_MOB_LENGTH, *slotDDefs[test], slotRates[test]));
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
			checkResult(pDictionary->CreateInstance(AUID_AAFSourceMob,
				IID_IAAFSourceMob, 
				(IUnknown **)&pSrcMob));		
			
			ref.sourceID = tapeMobID;
			ref.sourceSlotID = test;
			ref.startTime = TAPE_MOB_OFFSET;
			checkResult(pSrcMob->AppendPhysSourceRef (slotRates[test], test,
				*slotDDefs[test], ref, TAPE_MOB_LENGTH));
			
			checkResult(pDictionary->CreateInstance(AUID_AAFEssenceDescriptor,
				IID_IAAFEssenceDescriptor, 
				(IUnknown **)&pDesc));		
			checkResult(pSrcMob->SetEssenceDescriptor(pDesc));
			pDesc->Release();
			pDesc = NULL;
			
			// Append source MOB to header
			aafMobID_t				TempUID;
			checkResult(pSrcMob->QueryInterface(IID_IAAFMob, (void **) &pTempMob));
			checkResult(CoCreateGuid((GUID *)&TempUID));
			checkResult(pTempMob->SetMobID(TempUID));
			checkResult(pTempMob->SetName(L"source mob"));
			
			checkResult(pHeader->AddMob(pTempMob));
			pTempMob->Release();
			pTempMob = NULL;
			
			checkResult(pMasterMob->AddMasterSlot(*slotDDefs[test], test, pSrcMob, test+1, slotNames[test]));
			
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
	IAAFMasterMob*		pMasterMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot;
	aafWChar*			pTapeName = NULL;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr = S_OK;



  try
  {
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
    bFileOpen = true;

    // Validate that there is on one master mob in the test file.
	  checkResult(pHeader->CountMobs(kMasterMob, &numMobs));
    checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over Master MOBs
	  criteria.searchTag = kByMobKind;
	  criteria.tags.mobKind = kMasterMob;
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
		  checkExpression(0 == memcmp(&mobID, &NewMobID, sizeof(mobID)), AAFRESULT_TEST_FAILED);

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
				aafInt32			bufSize = 0;

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
 

extern "C" HRESULT CAAFMasterMob_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFMasterMobTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFMasterMob_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IAAFMasterMob tests have not been implemented:" << endl; 
		cout << "     GetRepresentationSourceClip" << endl; 
		cout << "     GetCriteriaSourceClip" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
























