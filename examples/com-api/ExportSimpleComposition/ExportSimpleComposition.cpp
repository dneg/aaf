// @com Executable example program by Chris Morgan, intern for Avid Technology, Tewksbury 
// @com This is based upon ComEssenceDataTest.cpp  Last modified on 8/11/99.

// This program creates a new aaf file in memory, using code from my ExportAudioExample program.
// The file is then processed so that composition mobs are made as per the documentation.

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(macintosh) || defined(_MAC)
#include "DataInput.h"
#endif

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

// The static variables are here so they can be referenced throughout the whole program.

// Filename variables
static aafWChar *	pwFileName; 
static char *		pFileName; 

static aafSourceRef_t sourceRef; 

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}


static aafBool	EqualAUID(aafUID_t *uid1, aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? kAAFTrue : kAAFFalse);
}

#define TEST_PATH	L"SomeFile.dat"

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

static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n\n");
    exit(1);  
  }
}

static void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}


typedef enum { testStandardCalls, testMultiCalls, testFractionalCalls } testType_t;

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola


AAFByteOrder GetNativeByteOrder(void);
void AAFByteSwap32(
			aafInt32 *lp);	/* IN/OUT -- Byte swap this value */
void AAFByteSwap16(
			aafInt16 * wp);	/* IN/OUT -- Byte swap this value */
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
AAFRESULT loadWAVEHeader(aafUInt8 *buf,
										aafUInt16 *bitsPerSample,
										aafUInt16 *numCh,
										aafRational_t *sampleRate,
										aafUInt32 *dataOffset,
										aafUInt32 *dataLen);

typedef struct
{
	aafWChar	*dataFilename;
	aafUID_t	dataFormat;
}

testDataFile_t;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT CreateAAFFile(aafWChar * pFileName, testDataFile_t *dataFile, testType_t testType,
							 IAAFFile ** ppFile)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IAAFEssenceFormat			*format = NULL;
	IAAFLocator					*pLocator = NULL;
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {44100, 1};
	aafRational_t				sampleRate = {44100, 1};
	FILE*						pWavFile = NULL;
	unsigned char				dataBuff[4096], *dataPtr;
	aafUInt32					dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
	aafInt32					n, numSpecifiers;
	aafUID_t					essenceFormatCode, testContainer;
	int							i;
  IAAFClassDef *pMasterMobDef = NULL;
  IAAFClassDef *pNetworkLocatorDef = NULL;
  IAAFDataDef *pSoundDef = NULL;
  aafUInt32 samplesWritten, bytesWritten;

  
  // Delete any previous test file before continuing...
	char chFileName[1000];
	convert(chFileName, sizeof(chFileName), pFileName);
	remove(chFileName);
	if(dataFile != NULL)
	{
		// Delete any previous test file before continuing...
		char chFileName[1000];
		convert(chFileName, sizeof(chFileName), dataFile->dataFilename);
		remove(chFileName);
	}

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF-East: Avid Technology";
	ProductInfo.productName = L"Export Simple Composition";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;

	/* Create a new AAF file IN MEMORY */

	check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, ppFile));
	pFile=*ppFile;
	check(pFile->GetHeader(&pHeader));
	
	/* Get the AAF Dictionary from the file */
	check(pHeader->GetDictionary(&pDictionary));
	
  /* Lookup class definitions for the objects we want to create. */
  check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pMasterMobDef));
  check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator, &pNetworkLocatorDef));

  /* Lookup any necessary data definitions. */
  check(pDictionary->LookupDataDef(DDEF_Sound, &pSoundDef));
	
	/* Create a Mastermob */

	// note that there is only one iteration at present because of
	// a bug in essenceaccess which prevents this functioning...
	for (i = 0; i<1; i++)
	{
		// Get a Master MOB Interface
		check(pMasterMobDef->
			  CreateInstance(IID_IAAFMasterMob, 
							 (IUnknown **)&pMasterMob));
		// Get a Mob interface and set its variables.
		check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		check(pMob->GetMobID(&masterMobID));
		check(pMob->SetName(L"A Master Mob"));
		
		// Add it to the file 
		check(pHeader->AddMob(pMob));
		
		if(dataFile != NULL)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			check(pNetworkLocatorDef->
				  CreateInstance(IID_IAAFLocator, 
								 (IUnknown **)&pLocator));		
			check(pLocator->SetPath (dataFile->dataFilename));
			testContainer = dataFile->dataFormat;
		}
		else
		{
			pLocator = NULL;
			testContainer = ContainerAAF;
		}
		
		
		// Open the Essence file to be included in this AAF file("Laser.wav")
		pWavFile = fopen("Laser.wav", "r");
		if (pWavFile)
		{
			// Read in the essence data
			fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
			check(loadWAVEHeader(dataBuff,
				&bitsPerSample,
				&numCh,
				&sampleRate,
				&dataOffset,
				&dataLen));
			
			//  The testtype is for Internal Standard calls so set dataPtr
			dataPtr = dataBuff + dataOffset;
			
			/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
			check(pMasterMob->CreateEssence(1,				// Slot ID
				pSoundDef,			// MediaKind
				kAAFCodecWAVE,		// codecID
				editRate,			// edit rate
				sampleRate,			// sample rate
				kAAFCompressionDisable,
				pLocator,			// In current file
				testContainer,		// In AAF Format
				&pEssenceAccess));	// Compress disabled
			
			check(pEssenceAccess->GetFileFormatParameterList (&format));
			check(format->NumFormatSpecifiers (&numSpecifiers));
			for(n = 0; n < numSpecifiers; n++)
			{
				check(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
			}
			format->Release();
			format = NULL;
			
			// Tell the AAFEssenceAccess what the format is.
			check(pEssenceAccess->GetEmptyFileFormat (&pFormat));
			check(pFormat->NumFormatSpecifiers (&numSpecifiers));
			
			/* Set the properties that describe the essence data, such as audio sample size */
			
			aafInt32	sampleSize = bitsPerSample;
			check(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
			check(pEssenceAccess->PutFileFormat (pFormat));
			pFormat->Release();
			pFormat = NULL;
			
			/* Write the samples */
			
			aafInt32 totalNumberSamples = dataLen * 5;  //  arbitrarily set to write 5 copies of audio sequentially
			aafInt32 samplesLeft = totalNumberSamples;
			
			while (samplesLeft >0)
			{
				check(pEssenceAccess->WriteSamples(	dataLen,	//!!! hardcoded bytes/sample ==1// Number of Samples
					sizeof(dataBuff), // buffer size
					dataPtr,	// THE data
					&samplesWritten,
					&bytesWritten));
				samplesLeft=samplesLeft-dataLen;
			}
			
			// Close the essence data file
			fclose(pWavFile);
			
			/* Set the essence to indicate that you have finished writing the samples */
			check(pEssenceAccess->CompleteWrite());

      pEssenceAccess->Release();
      pEssenceAccess = NULL;
		}
		else
		{
			printf("***Failed to open Wave file Laser.wav\n");
		}
	
	/* Release COM interfaces */

	  pMob->Release();
	  pMob = NULL;
	  pMasterMob->Release();
	  pMasterMob = NULL;
	}


  pSoundDef->Release();
  pSoundDef = NULL;
  pNetworkLocatorDef->Release();
  pNetworkLocatorDef = NULL;
  pMasterMobDef->Release();
  pMasterMobDef = NULL;

	if(pDictionary)
		pDictionary->Release();
	pDictionary = NULL;
	if(pHeader)
		pHeader->Release();
	pHeader = NULL;
	
cleanup:
	// Cleanup and return
	if(pFormat)
		pFormat->Release();

	if(format)
		format->Release();

	if(pLocator)
		pLocator->Release();

  if (pEssenceAccess)
		pEssenceAccess->Release();
	
	if (pMasterMob)
		pMasterMob->Release();

	if (pMob)
		pMob->Release();

  if (pSoundDef)
    pSoundDef->Release();

  if (pNetworkLocatorDef)
    pNetworkLocatorDef->Release();

  if (pMasterMobDef)
    pMasterMobDef->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	return moduleErrorTmp;
}

static HRESULT ProcessAAFFile(aafWChar * pFileName, testType_t testType)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	aafWChar*					slotName = L"A slot in Composition Mob";
	IAAFComponent*				pComponent = NULL;
	IAAFComponent*				aComponent = NULL;
	IEnumAAFMobSlots*			pMobSlotIter = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFTimelineMobSlot*		newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFMob*					pCompMob = NULL;
	IAAFMob*					pMob = NULL;
	aafPosition_t				zeroPos = 0;
	IAAFSequence*				pAudioSequence = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	aafLength_t					duration;
	IAAFTimelineMobSlot* pTimelineMobSlot = NULL;
  IAAFClassDef *pCompositionMobDef = NULL;
  IAAFClassDef *pSequenceDef = NULL;
  IAAFClassDef *pSourceClipDef = NULL;
  IAAFDataDef *pSoundDef = NULL;
  IAAFDataDef *pDataDef = NULL;
	

	// Set the edit rate information
	aafRational_t				editRate;
	editRate.numerator = 48000;
	editRate.denominator = 1;

	// Set search condition to true
	bool lookingForAudio = true;

	// Call the routine (from ExportAudioExample) to make the file for processing
	check(CreateAAFFile(pwFileName, NULL, testStandardCalls, &pFile));

	/* Get the Header and iterate through the Master Mobs in the existing file */
	check(pFile->GetHeader(&pHeader));
	check(pHeader->GetDictionary(&pDictionary));

	
  /* Lookup class definitions for the objects we want to create. */
  check(pDictionary->LookupClassDef(AUID_AAFCompositionMob, &pCompositionMobDef));
  check(pDictionary->LookupClassDef(AUID_AAFSequence, &pSequenceDef));
  check(pDictionary->LookupClassDef(AUID_AAFSourceClip, &pSourceClipDef));

  /* Lookup any necessary data definitions. */
  check(pDictionary->LookupDataDef(DDEF_Sound, &pSoundDef));
  

	// Get the number of master mobs in the existing file (must not be zero)
	check(pHeader->CountMobs(kAAFMasterMob, &numMobs));
	if (numMobs != 0)
	{
		printf("Found %ld Master Mobs\n", numMobs);
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		check(pHeader->GetMobs(&criteria, &pMobIter));

		/* Create a Composition Mob */
		check(pCompositionMobDef->
			  CreateInstance(IID_IAAFMob,
							 (IUnknown **)&pCompMob));
		/* Append the Mob to the Header */
		check(pHeader->AddMob(pCompMob));
 
		/* Create a TimelineMobSlot with an audio sequence */
		check(pSequenceDef->
			  CreateInstance(IID_IAAFSequence,
							 (IUnknown **)&pAudioSequence));
		check(pAudioSequence->QueryInterface(IID_IAAFSegment, (void **)&seg));

		check(pAudioSequence->QueryInterface(IID_IAAFComponent,
											 (void **)&aComponent));

		check(aComponent->SetDataDef(pSoundDef));
		check(pCompMob->AppendNewTimelineSlot(editRate, seg, 1, slotName, zeroPos, &newSlot));
    seg->Release();
    seg = NULL;
    newSlot->Release();
    newSlot = NULL;

		// This variable is about to be overwritten so we need to release the old interface
		aComponent->Release();
		aComponent = NULL;

		while((AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob)))
		{
			//  Print out information about the Mob
			char mobIDstr[256];
			char mobName[256];

			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			MobIDtoString(mobID, mobIDstr);
			printf("    MasterMob Name = '%s'\n", mobName);
			printf("        (mobID %s)\n", mobIDstr);
			
			// Add a Source Clip for each Master Mob to the audio sequence by iterating
			check(pMob->GetSlots(&pMobSlotIter));
			
			/* Iterating through all Mob Slots */
			// Get the number of slots
			check(pMob->CountSlots(&numSlots));
			
			while (lookingForAudio && (AAFRESULT_SUCCESS == pMobSlotIter->NextOne(&pMobSlot)));
			{
				/* Check to see if it is an Audio Timeline Mob Slot */
				HRESULT hr;
				
				hr=pMobSlot->QueryInterface(IID_IAAFTimelineMobSlot,(void **) &pTimelineMobSlot);
				if (SUCCEEDED(hr))
				{
					printf("Found a timeline mob slot\n");
					check(pMobSlot->GetDataDef(&pDataDef));
					
					// Check that we have a sound file by examining its data definition
          aafBool bIsSoundKind = kAAFFalse;
          check(pDataDef->IsSoundKind(&bIsSoundKind));

          if (kAAFTrue == bIsSoundKind)
					{
						printf("Found a sound file\n");

						// We are no longer looking for audio data so set boolean	
						lookingForAudio = false;

						/* Get the information for the new source clip */
						check(pMob->GetMobID(&sourceRef.sourceID));
						check(pMobSlot->GetSlotID(&sourceRef.sourceSlotID));
						check(pTimelineMobSlot->GetOrigin(&sourceRef.startTime));
						check(pMobSlot->GetSegment(&pSegment));
						check(pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
						check(pComponent->GetLength(&duration));
            pComponent->Release();
            pComponent = NULL;
            pSegment->Release();
            pSegment = NULL;
						
						// this loop is to be removed upon fixing of the bug
						// in essenceaccess relating to codec definitions...
						int j = 0;
						for (j=0; j<10; j++)
						{
							/* Create a new Source Clip */
							check(pSourceClipDef->
								  CreateInstance(IID_IAAFSourceClip,
												 (IUnknown **)&pSourceClip));
							// Initialize the Source Clip
							check(pSourceClip->Initialize( pSoundDef, duration, sourceRef));
							check(pSourceClip->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
							check(pAudioSequence->AppendComponent(pComponent));
              pComponent->Release();
              pComponent = NULL;
							pSourceClip->Release();
							pSourceClip = NULL;
						}
					}
					pTimelineMobSlot->Release();
					pTimelineMobSlot = NULL;

          pDataDef->Release();
          pDataDef = NULL;
				}	
        
        pMobSlot->Release();
        pMobSlot = NULL;
			}

      pMobSlotIter->Release();
      pMobSlotIter = NULL;
		  pMob->Release();
		  pMob = NULL;
		}
		

    pAudioSequence->Release();
    pAudioSequence = NULL;

    pCompMob->Release();
    pCompMob = NULL;

		pMobIter->Release();
		pMobIter = NULL;
	}
	else
	{
		printf("Error with file: File has no Master mobs.\n");
	}

cleanup:
	// Cleanup and return
  if (pSourceClip)
    pSourceClip->Release();

  if (pComponent)
    pComponent->Release();

  if (pSegment)
    pSegment->Release();

  if (pTimelineMobSlot)
    pTimelineMobSlot->Release();

  if (pMobSlotIter)
    pMobSlotIter->Release();

  if (pMob)
    pMob->Release();

  if (newSlot)
    newSlot->Release();

  if (aComponent)
    aComponent->Release();

  if (seg)
    seg->Release();

  if (pAudioSequence)
    pAudioSequence->Release();

  if (pCompMob)
    pCompMob->Release();

  if (pMobIter)
		pMobIter->Release();

  if (pDataDef)
    pDataDef->Release();

  if (pSoundDef)
    pSoundDef->Release();

  if (pSourceClipDef)
    pSourceClipDef->Release();

  if (pSequenceDef)
    pSequenceDef->Release();

  if (pCompositionMobDef)
    pCompositionMobDef->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
	  /* Save the AAF file */
    pFile->Save();
	  /* Close the AAF file */
		pFile->Close();
		pFile->Release();
	}

	return moduleErrorTmp;
}

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = NULL)
  {
  	printf("Attempting to load the AAF dll...\n");
    HRESULT hr = AAFLoad(dllname);
    (SUCCEEDED(hr)) ? printf("DONE\n\n") : printf("FAILED\n\n");
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};

//**********************
// Extra code required to scan the original WAVE headers and extract metadata parameters & data offset
AAFByteOrder GetNativeByteOrder(void)
{
  aafInt16 word = 0x1234;
  aafInt8  byte = *((aafInt8*)&word);
  AAFByteOrder result;

//  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) 
	  result = MOTOROLA_ORDER;
  else
	  result = INTEL_ORDER;
  
  return result;
}

void AAFByteSwap32(aafInt32 *lp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) lp;
	int t;
	t		= cp[3];
	cp[3]	= cp[0];
	cp[0]	= t;
	t		= cp[2];
	cp[2]	= cp[1];
	cp[1]	= t;
}

void AAFByteSwap16(
			aafInt16 * wp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) wp;
	int t;
	t		= cp[1];
	cp[1]	= cp[0];
	cp[0]	= t;
}

// Program works on all WAVE files, not just laser.wav.
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
}

void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
	AAFByteOrder	nativeByteOrder = GetNativeByteOrder();
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
	
	if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != nativeByteOrder))
			AAFByteSwap32((aafInt32 *) data);
	else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != nativeByteOrder))
			AAFByteSwap16((aafInt16 *) data);
}

AAFRESULT loadWAVEHeader(aafUInt8 *buf,
					 	 aafUInt16 *bitsPerSample,
						 aafUInt16 *numCh,
						 aafRational_t *sampleRate,
						 aafUInt32 *dataOffset,
						 aafUInt32 *dataLen)
{
	aafInt32			formSize;
	aafInt16			pcm_format, junk16;
	aafUInt32			chunkSize;
	aafBool				fmtFound = kAAFFalse, dataFound = kAAFFalse;
	aafUInt8			chunkID[4];
 	aafInt32			junk32, rate, bytesPerFrame;
	aafUInt8			*ptr;

	ptr = buf;
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	if (memcmp(&chunkID, "RIFF", (size_t) 4) != 0)
		return(AAFRESULT_BADWAVEDATA);
	scanSwappedWAVEData(&ptr, sizeof(formSize), &formSize);	
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	if (memcmp(&chunkID, "WAVE", (size_t) 4) != 0)
		return(AAFRESULT_BADWAVEDATA);
	
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	while ((ptr-buf) < formSize)
	{
		scanSwappedWAVEData(&ptr, sizeof(chunkSize), &chunkSize);	

		if (memcmp(&chunkID, "fmt ", (size_t) 4) == 0)
		{
			// WAVE field: wFormatTag
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&pcm_format);
			if (pcm_format != 1)
				return(AAFRESULT_BADWAVEDATA);
	
			// WAVE field: wChannels
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)numCh);
	
			// WAVE field: wSamplesPerSec
			scanSwappedWAVEData(&ptr, sizeof(aafUInt32), (aafUInt8 *)&rate);
			sampleRate->numerator = rate;
			sampleRate->denominator = 1;
	
			// Skip WAVE field: avgBytesPerSec (4 bytes)
			scanSwappedWAVEData(&ptr, sizeof(junk32), (aafUInt8 *)&junk32);

			// WAVE field wBlockAlign
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&junk16);
	
			// WAVE field Sample Width
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)bitsPerSample);
			bytesPerFrame = (((*bitsPerSample) + 7) / 8) * (*numCh);
			fmtFound = kAAFTrue;
		} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
		{
			*dataLen = chunkSize / bytesPerFrame;
			// Positioned at beginning of audio data
			*dataOffset = ptr - buf;
	
			dataFound = kAAFTrue;
		}
	
		if((ptr-buf) > formSize)
			break;
		if (fmtFound && dataFound)	// Do we have all information yet?
			break;
		scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	}	
	
	return(AAFRESULT_SUCCESS);
}

// Make sure all of our required plugins have been registered.
static HRESULT RegisterRequiredPlugins(void)
{
	IAAFPluginManager	*mgr = NULL;

  // Load the plugin manager 
  check(AAFGetPluginManager(&mgr));

  // Attempt load and register all of the plugins
  // in the shared plugin directory.
  check(mgr->RegisterSharedPlugins());

  // Attempt to register all of the plugin files
  // in the given directorys:
  //check(mgr->RegisterPluginDirectory(directory1));
  //check(mgr->RegisterPluginDirectory(directory2));


  // Attempt to register all of the plugins in any
  // of the given files:
  //check(mgr->RegisterPluginFile(file1));
  //check(mgr->RegisterPluginFile(file2));
  //...

cleanup:
  if (mgr)
    mgr->Release();

	return moduleErrorTmp;
}


//  A new usage function to make program more friendly
void usage(void)
{
	printf("Usage:\n ExportSimpleComposition.exe <file name>.aaf \n");
	printf(" If no filename is given, the filename defaults to ExportSimpleComposition.aaf\n\n");
}

//  Main adapted to use command-line arguments with argument checking
//  NOTE:  defining [0] program name; [1] filename.aaf; 
//  Specifying that use file ExportSimpleComposition.aaf as default
//  The specified filename is the name of the file that is created by the program.
int main(int argumentCount, char* argumentVector[])
{
	CAAFInitialize aafInit;

	
    // Make sure all of our required plugins have been registered.
    checkFatal(RegisterRequiredPlugins());
  

	if (argumentCount ==1)
	{
		// Initialise filename variables to default settings and inform user
		pwFileName = L"ExportSimpleComposition.aaf";
		pFileName = "ExportSimpleComposition.aaf";

		printf("No file specified => defaulting to ExportSimpleComposition.aaf\n\n");
	}
	else if (argumentCount ==2)
	{
		// Set the variables to do the specified case
		static char* niceFileName = argumentVector[1];
		
		// It is a design issue whether argument must end with ".aaf" or not
		// If one chooses that files are specified without extension, 
		// then uncomment the following line, which adds the ".aaf" extension
		//strcat (niceFileName,".aaf");
		
		aafWChar FileNameBuffer[80];
		mbstowcs(FileNameBuffer,niceFileName,80);
		pwFileName = FileNameBuffer;
		pFileName = niceFileName;
	}
	else
	{
		usage();
		return 0;
	}
	// Access the AAF file with name set from argument or lack thereof
	printf("Working on file %s using ReadSamples\n", pFileName);
	ProcessAAFFile(pwFileName, testStandardCalls);

	printf("DONE\n\n");

	return(0);
}




