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

// There are differences in the microsoft and other compilers in the "Length" specifier
// used in printf for 64bit integers.
//
// NOTE: If your compiler does not support 64 bit integers then this example will NOT
// print out the correct lengths.
#ifdef _MSC_VER
#define L64 "I64"
#else
#define L64 "ll"
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
#define TEST_PATH	L"AnotherFile.aaf"

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

static void     LogError(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}

#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     exit(1);\
}

static void AUIDtoString(aafUID_t *uid, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}


static HRESULT convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n\n");
    return -1; 
  }
  else
  	return S_OK;
}


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFMob*					pMob = NULL;
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
	IAAFComponent*				compFill = NULL;
	IAAFLocator*				pLocator = NULL;
	IAAFNetworkLocator*			pNetLocator = NULL;
	aafRational_t				videoRate = { 30000, 1001 };
	aafUID_t					videoDef = DDEF_Video;
	aafUID_t					tapeMobID, fileMobID, masterMobID;
	aafTimecode_t				tapeTC = { 108000, kTcNonDrop, 30};
	aafLength_t					fileLen = FILE1_LENGTH;
	aafLength_t					fillLen = FILL_LENGTH;
	aafLength_t					segLen = SEG_LENGTH;

	aafProductIdentification_t	ProductInfo;

	ProductInfo.companyName = L"Company Name";
	ProductInfo.productName = L"Cuts Only Composition Example";
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
	check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));

	check(pFile->Initialize());
	check(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
	check(pFile->GetHeader(&pHeader));

	//Make the Tape MOB
	check(CoCreateInstance( CLSID_AAFSourceMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceMob, 
						   (void **)&pTapeMob));
	check(CoCreateInstance( CLSID_AAFTapeDescriptor,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFTapeDescriptor, 
						   (void **)&pTapeDesc));
	check(pTapeDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc));
	check(pTapeMob->SetEssenceDescriptor(aDesc));
	aDesc->Release();
	aDesc = NULL;
		
	check(pTapeMob->AppendTimecodeSlot (videoRate, 0, tapeTC, TAPE_LENGTH));
	check(pTapeMob->AddNilReference (1,TAPE_LENGTH, &videoDef, videoRate));
	check(pTapeMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	check(pMob->SetName (L"A Tape Mob"));
	check(pHeader->AppendMob(pMob));
	check(pMob->GetMobID (&tapeMobID));
	pMob->Release();
	pMob = NULL;


	// Make a FileMob
	check(CoCreateInstance( CLSID_AAFSourceMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceMob, 
						   (void **)&pFileMob));
	check(CoCreateInstance( CLSID_AAFFileDescriptor,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFileDescriptor, 
						   (void **)&pFileDesc));
	check(pFileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc));

	// Make a locator, and attach it to the EssenceDescriptor
	check(CoCreateInstance(CLSID_AAFNetworkLocator,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFNetworkLocator, 
							(void **)&pNetLocator));		
	check(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));
	check(pLocator->SetPath (TEST_PATH));	
	check(aDesc->AppendLocator(pLocator));

	check(pFileMob->SetEssenceDescriptor(aDesc));
	aDesc->Release();
	aDesc = NULL;
	pFileDesc->Release();
	pFileDesc = NULL;

	sourceRef.sourceID = tapeMobID;
	sourceRef.sourceSlotID = 1;
	sourceRef.startTime = 0;
	check(pFileMob->NewPhysSourceRef (videoRate,
                           1, &videoDef, sourceRef, fileLen));

	check(pFileMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID (&fileMobID));
	check(pHeader->AppendMob(pMob));
	pMob->Release();
	pMob = NULL;

	//Make the Master MOB
	check(CoCreateInstance( CLSID_AAFMasterMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMasterMob, 
						   (void **)&pMasterMob));

	sourceRef.sourceID = fileMobID;
	sourceRef.sourceSlotID = 1;
	sourceRef.startTime = 0;
	check(pMasterMob->NewPhysSourceRef (videoRate, 1, &videoDef, sourceRef, fileLen));
	check(pMasterMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID (&masterMobID));
	check(pMob->SetName (L"A Master Mob"));
	check(pHeader->AppendMob(pMob));
	pMob->Release();
	pMob = NULL;

	// Create a Composition Mob
	check(CoCreateInstance( CLSID_AAFCompositionMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMob, 
						   (void **)&pCompMob));
	// Create a sequence
	check(CoCreateInstance( CLSID_AAFSequence,
				  NULL, 
			   CLSCTX_INPROC_SERVER, 
			   IID_IAAFSequence, 
			   (void **)&pSequence));		
	check(pSequence->QueryInterface (IID_IAAFSegment, (void **)&seg));

	check(pSequence->QueryInterface(IID_IAAFComponent, (void **)&aComponent));

	check(aComponent->SetDataDef(&videoDef));
	check(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	check(pMob->AppendNewSlot (seg, 1, slotName, &newSlot));

	// This variable is about to be overwritten so we need to 

	// release the old interface.

	aComponent->Release();

	aComponent = NULL;


	// Create a SourceClip
	check(CoCreateInstance( CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&compSclp));		

	sourceRef.sourceID = masterMobID;
	sourceRef.sourceSlotID = 1;
	sourceRef.startTime = 0;
	check(compSclp->SetSourceReference (sourceRef));
	check(compSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent));
	check(aComponent->SetDataDef(&videoDef));
	check(aComponent->SetLength (&segLen));
	check(pSequence->AppendComponent (aComponent));

	// Create a filler - Get the component interface only (IID_IAAFComponent)
	check(CoCreateInstance( CLSID_AAFFiller,
									   NULL, 
									   CLSCTX_INPROC_SERVER, 
									   IID_IAAFComponent, 
									   (void **)&compFill));		

	check(compFill->SetLength (&fillLen));

	check(compFill->SetDataDef(&videoDef));
	check(pSequence->AppendComponent (compFill));

	seg->Release();
	seg = NULL;
	aComponent->Release();
	aComponent = NULL;
	pMob->Release();
	pMob = NULL;
	newSlot->Release();
	newSlot = NULL;

	check(pHeader->AppendMob(pCompMob));

cleanup:
	// Cleanup and return
	if (pNetLocator)
		pNetLocator->Release();

	if (pLocator)
		pLocator->Release();

	if (compFill)
		compFill->Release();

	if (compSclp)
		compSclp->Release();

	if (masterSclp)
		masterSclp->Release();

	if (fileSclp)
		fileSclp->Release();

	if (pTapeDesc)
		pTapeDesc->Release();

	if (pFileDesc)
		pFileDesc->Release();

	if (aComponent)
		aComponent->Release();

	if (pSequence)
		pSequence->Release();

	if (pTapeMob)
		pTapeMob->Release();

	if (pFileMob)
		pFileMob->Release();

	if (pMasterMob)
		pMasterMob->Release();

	if (aDesc)
		aDesc->Release();

	if (pCompMob)
		pCompMob->Release();

	if (seg)
		seg->Release();

	if (newSlot)
		newSlot->Release();

	if (pMob)
		pMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	
	return moduleErrorTmp;
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
	IEnumAAFComponents*			pCompIter = NULL;
	IEnumAAFLocators*			pLocEnum = NULL;
	IAAFLocator*				pLocator = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFEssenceDescriptor*		pEdesc = NULL;
	IAAFSourceMob*				pSourceMob = NULL;

	IAAFFindSourceInfo*			info = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	aafInt32					numCompMobs;
	aafInt32					numTapeMobs, numFileMobs, numMasterMobs;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafUID_t					mobID;
	aafWChar					bufW[1204];
	char						bufA[2408];
	aafLength_t					length;

	  
	check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));
	check(pFile->Initialize());
	check(pFile->OpenExistingRead(pFileName, 0));
	check(pFile->GetHeader(&pHeader));

	// Here we check on the number of tape mobs and file mobs in the file
	// If this was an importing application, then the file and tape mobs
	// often get converted first so that the compositions come in without
	// forward references.
	// Get the total number of mobs in the file (should be four)
	check(pHeader->GetNumMobs(kAllMob, &numMobs));
	if (4 != numMobs )
	{
		printf("***Wrong number of mobs in the file (was %ld should be %ld)\n",
			numMobs, 4L);
	}

	printf("--------\n");
	// Get the number of tape mobs in the file (should be one)
	check(pHeader->GetNumMobs(kTapeMob, &numTapeMobs));
	if (1 == numTapeMobs )
	{
		printf("Found %ld Tape Mobs\n", numTapeMobs);
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kTapeMob;
		check(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (bufW, sizeof(bufW)));
			check(convert(bufA, sizeof(bufA), bufW));
			printf("    TapeName = '%s'\n", bufA);
			AUIDtoString(&mobID, bufA);
			printf("        (mobID %s)\n", bufA);

			pMob->Release();
			pMob = NULL;
		}

		pMobIter->Release();
		pMobIter = NULL;
	}
	else
	{
		printf("***Wrong number of tape mobs in the file (was %ld should be %ld)\n",
			numTapeMobs, 1L);
	}


	printf("--------\n");
	// Get the number of file mobs in the file (should be one)
	check(pHeader->GetNumMobs(kFileMob, &numFileMobs));
	if (1 == numFileMobs )
	{
		printf("Found %ld File Mobs\n", numFileMobs);
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kFileMob;
		check(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			check(pMob->GetMobID (&mobID));
			AUIDtoString(&mobID, bufA);
			printf("    (mobID %s)\n", bufA);

			check(pMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			check(pSourceMob->GetEssenceDescriptor (&pEdesc));
			check(pEdesc->EnumAAFAllLocators(&pLocEnum));

			// This should read the one real locator
			if(pLocEnum->NextOne(&pLocator) == AAFRESULT_SUCCESS)
			{
				check(pLocator->GetPath (bufW, sizeof(bufW)));
				check(convert(bufA, sizeof(bufA), bufW));
				printf("        There is one locator pointing to '%s'\n", bufA);

				pLocator->Release();
				pLocator = NULL;
			}
			else
				printf("        There are no locators on this file mob.\n");


			pLocEnum->Release();
			pLocEnum = NULL;

			pEdesc->Release();
			pEdesc = NULL;

			pSourceMob->Release();
			pSourceMob = NULL;

			pMob->Release();
			pMob = NULL;
		}

		pMobIter->Release();
		pMobIter = NULL;
	}
	else
	{
		printf("***Wrong number of file mobs in the file (was %ld should be %ld)\n",
			numFileMobs, 1L);
	}

	printf("--------\n");
	// Get the number of master mobs in the file (should be one)
	check(pHeader->GetNumMobs(kMasterMob, &numMasterMobs));
	if (1 == numMasterMobs )
	{
		printf("Found %ld Master Mobs\n", numMasterMobs);
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kMasterMob;
		check(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (bufW, sizeof(bufW)));
			check(convert(bufA, sizeof(bufA), bufW));
			printf("    MasterMob Name = '%s'\n", bufA);
			AUIDtoString(&mobID, bufA);
			printf("        (mobID %s)\n", bufA);
			

			pMob->Release();
			pMob = NULL;
		}

		pMobIter->Release();
		pMobIter = NULL;
	}
	else
	{
		printf("***Wrong number of master mobs in the file (was %ld should be %ld)\n",
			numMasterMobs, 1L);
	}

	printf("--------\n");
	// Get the number of composition mobs in the file (should be one)
	check(pHeader->GetNumMobs(kCompMob, &numCompMobs));
	if (1 == numCompMobs )
	{
		printf("Found %ld Composition Mobs\n", numCompMobs);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		check(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (pMobIter && AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			check(pMob->GetMobID (&mobID));
			AUIDtoString(&mobID, bufA);
			printf("    (mobID %s)\n", bufA);
			pMob->GetNumSlots(&numSlots);
			if (1 == numSlots)
			{
				check(pMob->EnumAAFAllMobSlots(&pSlotIter));
				while (pSlotIter && AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
				{
					check(pSlot->GetSegment(&pSegment));

					// Get the length of the segment: access through the component interface.
					check(pSegment->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
					check(pComponent->GetLength (&length));
					pComponent->Release();
					pComponent = NULL;

					hr = pSegment->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
					if(AAFRESULT_SUCCESS == hr)
					{
						printf("    Found source clip on slot\n");
						printf("        It has length %" L64 "d\n", length);

						hr = pSourceClip->ResolveRef(&pReferencedMob);
						if(hr == AAFRESULT_SUCCESS)
						{
							check(pReferencedMob->GetMobID(&mobID));
							check(pReferencedMob->GetName (bufW, sizeof(bufW)));
							check(convert(bufA, sizeof(bufA), bufW));
							printf("        References mob = '%s'\n", bufW);
							AUIDtoString(&mobID, bufA);
							printf("            (mobID %s)\n", bufA);

							pReferencedMob->Release();
							pReferencedMob = NULL;
						}


						pSourceClip->Release();
						pSourceClip = NULL;
					}
					else
					{
						hr = pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence);
						if(AAFRESULT_SUCCESS == hr)
						{
							aafInt32	numComponents, item = 0;
					
							check(pSequence->GetNumComponents (&numComponents));
							printf("    Found Sequence on slot with %ld components\n",
								numComponents);
							printf("        It has length %" L64 "d\n", length);
							check(pSequence->EnumComponents (&pCompIter));
							while (pCompIter && AAFRESULT_SUCCESS == pCompIter->NextOne(&pComponent))
							{
								item++;
								check(pComponent->GetLength (&length));
								hr = pComponent->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
								if(AAFRESULT_SUCCESS == hr)
								{
									aafSourceRef_t		ref;

									check(pSourceClip->GetSourceReference (&ref));
									printf("        %ld) A length %" L64 "d source clip\n", item, length);
									check(pSourceClip->ResolveRef(&pReferencedMob));
									check(pReferencedMob->GetMobID(&mobID));
									check(pReferencedMob->GetName (bufW, sizeof(bufW)));
									check(convert(bufA, sizeof(bufA), bufW));
									printf("            References mob = '%s'\n", bufA);
									AUIDtoString(&mobID, bufA);
									printf("                (mobID %s)\n", bufA);

									hr = pReferencedMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob);
 									if(AAFRESULT_SUCCESS == hr)
									{
										check(pMasterMob->GetTapeName (ref.sourceSlotID,
											bufW, sizeof(bufW)));
										check(convert(bufA, sizeof(bufA), bufW));
										printf("            Derived from tape = '%s'\n", bufA);


										pMasterMob->Release();

										pMasterMob = NULL;
									}


									pSourceClip->Release();

									pSourceClip = NULL;
								}
								hr = pComponent->QueryInterface(IID_IAAFFiller, (void **) &pFiller);
								if(AAFRESULT_SUCCESS == hr)
								{
									printf("        %ld) A length %" L64 "d filler\n", item, length);

									pFiller->Release();
									pFiller = NULL;
								}

								pComponent->Release();
								pComponent = NULL;
							}

							pCompIter->Release();
							pCompIter = NULL;

							pSequence->Release();
							pSequence = NULL;
						}
						else
						{
							printf("    Found unknown segment on slot\n");
						}
					}

					pSegment->Release();
					pSegment = NULL;

					pSlot->Release();
					pSlot = NULL;
				}

				pSlotIter->Release();
				pSlotIter = NULL;
			}
			else
			{
				printf("***Wrong number of slots in the composition mob (was %ld should be %ld)\n",
					numSlots, 1L);
			}

			pMob->Release();
			pMob = NULL;
		}

		pMobIter->Release();
		pMobIter = NULL;

	}
	else
	{
		printf("***Wrong number of composition mobs in the file (was %ld should be %ld)\n",
			numCompMobs, 1L);
	}



cleanup:
	// Cleanup and return
	if (info)

		info->Release();


	if (pMasterMob)

		pMasterMob->Release();


	if(pSourceMob)
		pSourceMob->Release();

	if(pEdesc)
		pEdesc->Release();
	
	if(pLocator)
		pLocator->Release();
	
	if(pLocEnum)
		pLocEnum->Release();
	
	if (pFiller)
		pFiller->Release();

	if (pCompIter)
		pCompIter->Release();

	if (pComponent)
		pComponent->Release();

	if (pSequence)
		pSequence->Release();

	if (pReferencedMob)
		pReferencedMob->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pSegment)
		pSegment->Release();

	if (pSlot)
		pSlot->Release();

	if (pSlotIter)
		pSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	return moduleErrorTmp;
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
  checkFatal(CreateAAFFile(pwFileName));
  printf("***Re-opening file %s\n", pFileName);
  ReadAAFFile(pwFileName);

  printf("Done\n");


  return(0);
}



















