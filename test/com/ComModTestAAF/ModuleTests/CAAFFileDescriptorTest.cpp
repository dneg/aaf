// @doc INTERNAL
// @com This file implements the module test for CAAFFileDescriptor
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "AAF.h"

#include <stdio.h>
#include <iostream.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFContainerDefs.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static aafRational_t	checkSampleRate = { 2997, 100 };
static aafUID_t			checkContainer = ContainerFile;
static aafLength_t		checkLength = 42;

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



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	IAAFFileDescriptor *pFileDesc = NULL;
	
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = S_OK;
	
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFileDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		//Make the first mob
		long			test;
		aafUID_t		ddef = DDEF_Sound;
		
		aafRational_t	audioRate = { 44100, 1 };
		
		// Create a Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
			IID_IAAFSourceMob, 
			(IUnknown **)&pSourceMob));
		
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
		checkResult(CoCreateGuid((GUID *)&newUID));
		checkResult(pMob->SetMobID(&newUID));
		checkResult(pMob->SetName(L"FileDescriptorTest"));
		
		// Add some slots
		for(test = 0; test < 2; test++)
		{
			checkResult(pSourceMob->AddNilReference (test+1, 0, &ddef, audioRate));
		}
		
		checkResult(pDictionary->CreateInstance(&AUID_AAFFileDescriptor,
			IID_IAAFEssenceDescriptor, 
			(IUnknown **)&edesc));		
		checkResult(edesc->QueryInterface(IID_IAAFFileDescriptor, (void **) &pFileDesc));
		checkResult(pFileDesc->SetSampleRate (&checkSampleRate));
		checkResult(pFileDesc->SetContainerFormat (&checkContainer));
		checkResult(pFileDesc->SetLength (checkLength));
		checkResult(pFileDesc->SetIsInContainer (AAFTrue));
		
		checkResult(pSourceMob->SetEssenceDescriptor (edesc));
		
		checkResult(pHeader->AppendMob(pMob));
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	if (pFileDesc)
		pFileDesc->Release();
	if (edesc)
		edesc->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pSourceMob)
		pSourceMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile) 
	{
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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot		*slot = NULL;
	IAAFFileDescriptor *pFileDesc = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n, s;
	HRESULT						hr = S_OK;
	aafRational_t				testSampleRate;
	aafUID_t					testContainer;
	aafLength_t					testLength;
	aafBool						testBool;
	
	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"AAFFileDescriptor Test. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	
	
	
	try
	{ 
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		checkResult(pHeader->EnumAAFAllMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafNumSlots_t	numSlots;
			aafUID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->GetNumSlots (&numSlots));
			if (2 != numSlots)
				return AAFRESULT_TEST_FAILED;
			if(numSlots != 0)
			{
				checkResult(aMob->EnumAAFAllMobSlots(&slotIter));
				
				for(s = 0; s < numSlots; s++)
				{
					checkResult(slotIter->NextOne (&slot));
					checkResult(slot->GetSlotID(&trackID));
					
					slot->Release();
					slot = NULL;
				}
			}
			checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));
			checkResult(pEdesc->QueryInterface(IID_IAAFFileDescriptor, (void **) &pFileDesc));
			checkResult(pFileDesc->GetSampleRate (&testSampleRate));
			checkExpression(testSampleRate.numerator == checkSampleRate.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testSampleRate.denominator == checkSampleRate.denominator, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->GetContainerFormat (&testContainer));
			checkExpression(memcmp(&testContainer, &checkContainer, sizeof(testContainer)) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->GetLength (&testLength));
			checkExpression(checkLength == testLength, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->GetIsInContainer (&testBool));
			checkExpression(testBool == AAFTrue, AAFRESULT_TEST_FAILED);
			
			pEdesc->Release();
			pEdesc = NULL;
			pFileDesc->Release();
			pFileDesc = NULL;
			pSourceMob->Release();
			pSourceMob = NULL;
			
			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (slot)
		slot->Release();
	
	if (pFileDesc)
		pFileDesc->Release();
			
	if (slotIter)
		slotIter->Release();
	
	if (pEdesc)
		pEdesc->Release();
	
	if (pSourceMob)
		pSourceMob->Release();
	
	if (aMob)
		aMob->Release();
	
	if(mobIter)
		mobIter->Release();
	
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

extern "C" HRESULT CAAFFileDescriptor_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFFileDescriptorTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFFileDescriptor_test...Caught general C++"
			" exception!" << endl; 
	}
	
	return hr;
}


