// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#if defined(_MAC) || defined(macintosh)
#include <initguid.h> // define all of the AAF guids.
#include "AAF.h"
#else
#include "AAF.h"
// TODO: This should not be here, I added them for now to get a good link.
const CLSID CLSID_AAFComponent = { 0x38e6f8a0, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFCompositionMob = { 0x91920d64, 0x2a2e, 0x11D2, { 0xbf, 0xA3, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFFile = { 0x9346ACD2, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const CLSID CLSID_AAFFiller = { 0x38e6f8a8, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFHeader = { 0xB1A21383, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFIdentification = { 0xB1A2137F, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFLocator = { 0xe58a8565, 0x2a3e, 0x11D2, { 0xbf, 0xA4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFMasterMob = { 0xb1a21373, 0x1a7d, 0x11d2, { 0xbf, 0x78, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const CLSID CLSID_AAFEssenceDescriptor = { 0x94659784, 0x2a3c, 0x11D2, { 0xbf, 0xA4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFFileDescriptor = { 0xe58a8562, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFTapeDescriptor = { 0xe58a856a, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFMob = { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFMobSlot = { 0xB1A21377, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFNetworkLocator = { 0x2c1097b1, 0x69d6, 0x11d2, { 0x84, 0x1b, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFObject = { 0xB1A213AC, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFSegment = { 0x7a2f0571, 0x1ba3, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFSequence = { 0x66e855f1, 0x2bc2, 0x11d2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const CLSID CLSID_AAFSourceClip = { 0x38e6f8a5, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFSourceMob = { 0xB1A2137D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFSourceReference = { 0x92B88034, 0x2B2B, 0x11d2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFTimecode = { 0x92B88036, 0x2B2B, 0x11d2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFTimelineMobSlot = { 0xB1A21379, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFComponents = { 0xB1A21395, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFIdentifications = { 0xB1A21385, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFLocators = { 0xB1A2139F, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFMobSlots = { 0xB1A21389, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFMobs = { 0xB1A21387, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

#endif


static aafWChar *slotName = L"SLOT1";
static aafInt32 fadeInLen  = 1000;
static aafInt32 fadeOutLen = 2000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafFadeType_t fadeOutType = kFadeLinearPower;
static aafSourceRef_t sourceRef; 

static aafBool	EqualAUID(aafUID_t *uid1, aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? AAFTrue : AAFFalse);
}

#define TAPE_LENGTH			1L * 60L *60L * 30L
#define FILE1_LENGTH		60L * 30L
#define SEG_LENGTH			30L
#define FILL_LENGTH			10L


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFMob*					pMob;	// Shared, don't Release.
	IAAFMob*					pCompMob = NULL;
	IAAFEssenceDescriptor*		aDesc = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFSourceMob*				pFileMob = NULL;
	IAAFSourceMob*				pTapeMob = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFComponent*				aComponent = NULL;
	IAAFFileDescriptor*			pFileDesc = NULL;
	IAAFTapeDescriptor*			pTapeDesc = NULL;
	IAAFMobSlot*				newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFSourceClip*				fileSclp = NULL;
	IAAFSourceClip*				masterSclp = NULL;
	IAAFSourceClip*				compSclp = NULL;
	IAAFComponent*				compFill;
	aafRational_t				videoRate = { 30000, 1001 };
	aafUID_t					videoDef = DDEF_Video;
	aafUID_t					tapeMobID, fileMobID, masterMobID;
	aafTimecode_t				tapeTC = { 108000, kTcNonDrop, 30};

	aafProductIdentification_t	ProductInfo;
	HRESULT						hr;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	// We assume the following functions have been tested and they do work
	// The next 3 function calls create the AAF file
/*	hr = pSession->SetDefaultIdentification(&ProductInfo);	*/
	hr = CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile);

	hr = pFile->Initialize();
	hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
  	hr = pFile->GetHeader(&pHeader);

	//Make the Tape MOB
	hr = CoCreateInstance( CLSID_AAFSourceMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceMob, 
						   (void **)&pTapeMob);
	if (AAFRESULT_SUCCESS == hr)
	{
		hr = CoCreateInstance( CLSID_AAFTapeDescriptor,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFTapeDescriptor, 
						   (void **)&pTapeDesc);
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pTapeDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc);
		if (AAFRESULT_SUCCESS == hr)
			hr = pTapeMob->SetEssenceDescription(aDesc);
		aDesc->Release();
		aDesc = NULL;
	}
		
	if (AAFRESULT_SUCCESS == hr)
	{
		pTapeMob->AppendTimecodeSlot (videoRate, 0, tapeTC, TAPE_LENGTH);
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pTapeMob->AddNilReference (1,TAPE_LENGTH, &videoDef, videoRate);
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pTapeMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == hr)
			pMob->SetName (L"A Tape Mob");
		if (AAFRESULT_SUCCESS == hr)
			hr = pHeader->AppendMob(pMob);
		if (AAFRESULT_SUCCESS == hr)
			pMob->GetMobID (&tapeMobID);
		pMob->Release();
		pMob = NULL;
	}


	// Make a FileMob
	if (AAFRESULT_SUCCESS == hr)
	{
		hr = CoCreateInstance( CLSID_AAFSourceMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceMob, 
						   (void **)&pFileMob);
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = CoCreateInstance( CLSID_AAFFileDescriptor,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFileDescriptor, 
						   (void **)&pFileDesc);
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pFileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc);
		if (AAFRESULT_SUCCESS == hr)
			hr = pFileMob->SetEssenceDescription(aDesc);
		aDesc->Release();
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = CoCreateInstance( CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&fileSclp);		
	}

	if (AAFRESULT_SUCCESS == hr)
	{			
		sourceRef.sourceID = tapeMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		hr = fileSclp->SetRef(sourceRef);
		if (AAFRESULT_SUCCESS == hr)
		{
			aafLength_t	fileLen = FILE1_LENGTH;
			hr = fileSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent);
			if (AAFRESULT_SUCCESS == hr)
				hr = aComponent->SetLength (&fileLen);
			if (AAFRESULT_SUCCESS == hr)
				hr = fileSclp->QueryInterface (IID_IAAFSegment, (void **)&seg);
			if (AAFRESULT_SUCCESS == hr)
				hr = pFileMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
			if (AAFRESULT_SUCCESS == hr)
				hr = pMob->AppendNewSlot (seg, 1, slotName, &newSlot);
			if (AAFRESULT_SUCCESS == hr)
			{
				aComponent->Release();
				aComponent = NULL;
				pMob->Release();
				pMob = NULL;
			}
		}	
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pFileMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == hr)
			pMob->GetMobID (&fileMobID);
		if (AAFRESULT_SUCCESS == hr)
			hr = pHeader->AppendMob(pMob);
		pMob->Release();
	}

	//Make the Master MOB
	hr = CoCreateInstance( CLSID_AAFMasterMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMasterMob, 
						   (void **)&pMasterMob);


	if (AAFRESULT_SUCCESS == hr)
	{
		hr = CoCreateInstance( CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&masterSclp);		
	}

	if (AAFRESULT_SUCCESS == hr)
	{			
		sourceRef.sourceID = fileMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		hr = masterSclp->SetRef(sourceRef);
		if (AAFRESULT_SUCCESS == hr)
		{
			aafLength_t	fileLen = FILE1_LENGTH;
			hr = masterSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent);
			if (AAFRESULT_SUCCESS == hr)
				hr = aComponent->SetLength (&fileLen);
			if (AAFRESULT_SUCCESS == hr)
				hr = masterSclp->QueryInterface (IID_IAAFSegment, (void **)&seg);
			if (AAFRESULT_SUCCESS == hr)
				hr = pMob->AppendNewSlot (seg, 1, slotName, &newSlot);
			if (AAFRESULT_SUCCESS == hr)
			{
				aComponent->Release();
			}
		}	
	}

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pMasterMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == hr)
			pMob->GetMobID (&masterMobID);
		if (AAFRESULT_SUCCESS == hr)
			hr = pHeader->AppendMob(pMob);
		pMob->Release();
		pMob = NULL;
	}

	// Create a Composition Mob
	hr = CoCreateInstance( CLSID_AAFCompositionMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMob, 
						   (void **)&pCompMob);
	// Create a sequence
	hr = CoCreateInstance( CLSID_AAFSequence,
				  NULL, 
			   CLSCTX_INPROC_SERVER, 
			   IID_IAAFSequence, 
			   (void **)&pSequence);		
	if (AAFRESULT_SUCCESS == hr)
		hr = pSequence->QueryInterface (IID_IAAFSegment, (void **)&seg);
	if (AAFRESULT_SUCCESS == hr)
		hr = pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
	if (AAFRESULT_SUCCESS == hr)
		hr = pMob->AppendNewSlot (seg, 1, slotName, &newSlot);

	// Create a SourceClip
	hr = CoCreateInstance( CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&compSclp);		
 	if (AAFRESULT_SUCCESS == hr)
	{			
		sourceRef.sourceID = masterMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		hr = compSclp->SetRef(sourceRef);
		if (AAFRESULT_SUCCESS == hr)
		{
			hr = compSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent);
			if (AAFRESULT_SUCCESS == hr)
			{
				aafLength_t	segLen = SEG_LENGTH;

				if (AAFRESULT_SUCCESS == hr)
					hr = aComponent->SetLength (&segLen);
				if (AAFRESULT_SUCCESS == hr)
					hr = pSequence->AppendComponent (aComponent);
				if (AAFRESULT_SUCCESS == hr)
				{			
					// Create a filler
					hr = CoCreateInstance( CLSID_AAFFiller,
									   NULL, 
									   CLSCTX_INPROC_SERVER, 
									   IID_IAAFComponent, 
									   (void **)&compFill);		
					if (AAFRESULT_SUCCESS == hr)
					{
						aafLength_t	fillLen = FILL_LENGTH;

						if (AAFRESULT_SUCCESS == hr)
							hr = compFill->SetLength (&fillLen);
						if (AAFRESULT_SUCCESS == hr)
							hr = pSequence->AppendComponent (compFill);
					}
				}
				if (AAFRESULT_SUCCESS == hr)
				{
					seg->Release();
					seg = NULL;
					aComponent->Release();
					aComponent = NULL;
					pMob->Release();
					pMob = NULL;
					newSlot->Release();
					newSlot = NULL;
				}
			}
		}
	}

	if (AAFRESULT_SUCCESS == hr)
		hr = pHeader->AppendMob(pCompMob);

	// Cleanup and return
	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	if (pHeader)
		pHeader->Release();

	if (pMob)
		pMob->Release();

	if (newSlot)
		newSlot->Release();

	if (seg)
		seg->Release();

//	if (sclp)
//		sclp->Release();

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFComponent*				pComponent = NULL;
	IEnumAAFComponents*			pCompIter;
	IAAFFiller*					pFiller;

	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	aafInt32					numCompMobs;
	aafInt32					numTapeMobs, numFileMobs, numMasterMobs;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafUID_t					mobID;
	aafWChar					namebuf[1204];
	aafLength_t					length;

	  
	CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile);
//	check(hr); // display error message
	if (SUCCEEDED(hr))
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
/*		hr = pSession->SetDefaultIdentification(&ProductInfo); */
		hr = pFile->Initialize();
		hr = pFile->OpenExistingRead(pFileName, 0);
	  	hr = pFile->GetHeader(&pHeader);

		// Here we check on the number of tape mobs and file mobs in the file
		// If this was an importing application, then the file and tape mobs
		// often get converted first so that the compositions come in qithout
		// forward references.
		// Get the total number of mobs in the file (should be four)
		hr = pHeader->GetNumMobs(kAllMob, &numMobs);
		if (4 != numMobs )
		{
			printf("***Wrong number of mobs in the file (was %ld should be %ld)\n",
				numMobs, 4L);
		}

		wprintf(L"--------\n");
		// Get the number of tape mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kTapeMob, &numTapeMobs);
		if (1 == numTapeMobs )
		{
			printf("Found %ld Tape Mobs\n", numTapeMobs);
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kTapeMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			hr = pMobIter->NextOne(&pMob);
			while(AAFRESULT_SUCCESS == hr && pMobIter != NULL)
			{
				hr = pMob->GetMobID (&mobID);
				hr = pMob->GetName (namebuf, sizeof(namebuf));
				wprintf(L"    TapeName = %s (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
					namebuf, mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
					 mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
					  mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
				hr = pMobIter->NextOne(&pMob);
			}
		}
		else
		{
			printf("***Wrong number of tape mobs in the file (was %ld should be %ld)\n",
				numTapeMobs, 1L);
		}


		wprintf(L"--------\n");
		// Get the number of file mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kFileMob, &numFileMobs);
		if (1 == numFileMobs )
		{
			printf("Found %ld File Mobs\n", numFileMobs);
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kFileMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			hr = pMobIter->NextOne(&pMob);
			while(AAFRESULT_SUCCESS == hr && pMobIter != NULL)
			{
				hr = pMob->GetMobID (&mobID);
				wprintf(L"    (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
					mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
					 mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
					  mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
				hr = pMobIter->NextOne(&pMob);
			}
		}
		else
		{
			printf("***Wrong number of file mobs in the file (was %ld should be %ld)\n",
				numFileMobs, 1L);
		}

		wprintf(L"--------\n");
		// Get the number of master mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kMasterMob, &numMasterMobs);
		if (1 == numMasterMobs )
		{
			printf("Found %ld Master Mobs\n", numMasterMobs);
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kMasterMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			hr = pMobIter->NextOne(&pMob);
			while(AAFRESULT_SUCCESS == hr && pMobIter != NULL)
			{
				hr = pMob->GetMobID (&mobID);
				wprintf(L"    (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
					mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
					 mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
					  mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
				hr = pMobIter->NextOne(&pMob);
			}
		}
		else
		{
			printf("***Wrong number of master mobs in the file (was %ld should be %ld)\n",
				numMasterMobs, 1L);
		}

		wprintf(L"--------\n");
		// Get the number of composition mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kCompMob, &numCompMobs);
		if (1 == numCompMobs )
		{
			printf("Found %ld Composition Mobs\n", numCompMobs);

			// Enumerate over all Composition Mobs
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kCompMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			while (pMobIter && pMobIter->NextOne(&pMob) !=AAFRESULT_NO_MORE_MOBS)
			{
				hr = pMob->GetMobID (&mobID);
				wprintf(L"    (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
					mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
					 mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
					  mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);

				pMob->GetNumSlots(&numSlots);
				if (1 == numSlots)
				{
					hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
					while (pSlotIter && pSlotIter->NextOne(&pSlot) != AAFRESULT_NO_MORE_OBJECTS)
					{
						hr = pSlot->GetSegment(&pSegment);

						hr = pSegment->QueryInterface(IID_IAAFComponent, (void **) &pComponent);
						hr = pComponent->GetLength (&length);
						wprintf(L"Slot length = %ld\n", length);

						hr = pSegment->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
						if(AAFRESULT_SUCCESS == hr)
						{
							wprintf(L"Found source clip on slot\n");
							hr = pSourceClip->ResolveRef(&pReferencedMob);
							if(hr == AAFRESULT_SUCCESS)
							{
								pReferencedMob->GetMobID(&mobID);
								wprintf(L"    References mob (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
									mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
									mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
									mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
							}
						}
						else
						{
							hr = pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence);
							if(AAFRESULT_SUCCESS == hr)
							{
								aafInt32	numComponents;
						
								hr = pSequence->GetNumComponents (&numComponents);
								wprintf(L"Found Sequence on slot with %ld components\n",
									numComponents);
								hr = pSequence->EnumComponents (&pCompIter);
								while (pCompIter && pCompIter->NextOne(&pComponent) != AAFRESULT_NO_MORE_OBJECTS)
								{
									hr = pComponent->GetLength (&length);
									wprintf(L"   Component length = %ld\n", length);
									hr = pComponent->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
									if(AAFRESULT_SUCCESS == hr)
									{
										wprintf(L"    It's asource clip\n");
										hr = pSourceClip->ResolveRef(&pReferencedMob);
										if(hr == AAFRESULT_SUCCESS)
										{
											pReferencedMob->GetMobID(&mobID);
											wprintf(L"    Which references mob (mobID %08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x)\n",
												mobID.Data1, mobID.Data2, mobID.Data3, mobID.Data4[0],
												mobID.Data4[1], mobID.Data4[2], mobID.Data4[3], mobID.Data4[4],
												mobID.Data4[5], mobID.Data4[6], mobID.Data4[7]);
										}
									}
									hr = pComponent->QueryInterface(IID_IAAFFiller, (void **) &pFiller);
									if(AAFRESULT_SUCCESS == hr)
									{
										wprintf(L"    It's filler\n");
									}
								}
							}
							else
							{
								wprintf(L"Found unknown segment on slot\n");
							}
						}
					}
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
				}
			}
		}
		else
		{
			printf("***Wrong number of composition mobs in the file (was %ld should be %ld)\n",
				numCompMobs, 1L);
		}
	}

	// Cleanup and return
	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	if (pHeader)
		pHeader->Release();

	if (pMob)
		pMob->Release();

	if (pSlot)
		pSlot->Release();

	if (pSegment)
		pSegment->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pSlotIter)
		pSlotIter->Release();

	return hr;
}

// simple helper class to initialize and cleanup COM library.
struct CComInitialize
{
  CComInitialize()
  {
    CoInitialize(NULL);
  }

  ~CComInitialize()
  {
    CoUninitialize();
  }
};

main()
{
  CComInitialize comInit;
  aafWChar * pwFileName = L"CutsOnly.aaf";
  const char * pFileName = "CutsOnly .aaf";

  printf("***Creating file %s\n", pFileName);
  CreateAAFFile(pwFileName);
  printf("***Re-opening file %s\n", pFileName);
  ReadAAFFile(pwFileName);

  printf("Done\n");


  return(0);
}



















