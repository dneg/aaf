// @com Executable example program by Chris Morgan, intern for Avid Technology, Tewksbury 
// @com This is based upon ComEssenceDataTest.cpp  Last modified on 8/4/99.

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
aafWChar *	pwFileName; 
char *	pFileName; 

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}



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

void doSomethingWithAudioFile()
{
 // Code here would depend upon required functionality
}

//const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	IAAFTimelineMobSlot* pTimelineMobSlot = NULL;
	IEnumAAFMobSlots* pMobSlotIter = NULL;
	IAAFMobSlot* pMobSlot = NULL;
	IAAFDataDef *pSoundDef = NULL;
	IAAFDataDef *pDataDef = NULL;
	unsigned char *dataBuff = NULL;


	/* Open an AAF file */
	check(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
	
	/* Get the Header and iterate through the Master Mobs */
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary so that we can create valid AAF objects.
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(DDEF_Sound, &pSoundDef));

	// Here we check on the number of mobs in the file. 
	// Get the number of master mobs in the file (must not be zero)
	check(pHeader->CountMobs(kAAFMasterMob, &numMobs));
	if (numMobs != 0)
	{
		printf("Found %ld Master Mobs\n", numMobs);
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		check(pHeader->GetMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];

			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			MobIDtoString(mobID, mobIDstr);
			printf("    MasterMob Name = '%s'\n", mobName);
			printf("        (mobID %s)\n", mobIDstr);
			
			// Get the number of slots
			check(pMob->CountSlots(&numSlots));
			
			/* Iterating through all Mob Slots */

			check(pMob->GetSlots(&pMobSlotIter));
			while(AAFRESULT_SUCCESS == pMobSlotIter->NextOne(&pMobSlot))
			{
				/* Check to see if it is an Audio Timeline Mob Slot */
				HRESULT hr;
				aafUInt32 MobSlotID;

				hr=pMobSlot->QueryInterface(IID_IAAFTimelineMobSlot,(void **) &pTimelineMobSlot);
				if (SUCCEEDED(hr))
				{
					check(pMobSlot->GetDataDef(&pDataDef));

					// Check that we have a sound file by examining its data definition
					aafBool bIsSoundKind = kAAFFalse;
					check(pDataDef->IsSoundKind(&bIsSoundKind));

					if (kAAFTrue == bIsSoundKind)
					{
						// Prepare to get audio data: first get MobSlotID
						check(pMobSlot->GetSlotID(&MobSlotID));

						// Then get a Master Mob interface
						check(pMob->QueryInterface(IID_IAAFMasterMob, 
							(void **)&pMasterMob));
						
						// Open the Essence Data
						check(pMasterMob->OpenEssence(MobSlotID,
												 	  NULL,	
													  kAAFMediaOpenReadOnly,
													  kAAFCompressionDisable, 
													  &pEssenceAccess));
						
						// Get the information about the format of the audio data
						check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
						check(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
						check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
						fmtTemplate->Release();
						fmtTemplate = NULL;
						
						// Work out how much data is read 
						aafUInt32 sampleBits;
						aafInt32 bytesRead;

						check(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, 
							sizeof(sampleBits),
							(aafDataBuffer_t)&sampleBits, &bytesRead));
		
						// Output the size so we can see what's going on
						printf("File has %ld Sample Bits.\n",sampleBits);
		
						pFormat->Release();
						pFormat = NULL;
		
						aafUInt32 dataLen, bytesLeft, samplesRead, actualBytesRead;

						if (sampleBits == 8) 
							dataLen = 1;
												
						// Get the sample count
						aafLength_t sampleCount;
						check(pEssenceAccess->CountSamples(pSoundDef, &sampleCount));
						
						// Read the samples of audio data
						bytesLeft = (aafUInt32) sampleCount * dataLen;

						// Set a suitable buffer size						
						dataBuff = new unsigned char[bytesLeft];
						if (NULL == dataBuff)
							check(AAFRESULT_NOMEMORY);

						
						while (bytesLeft != 0)
						{
							check(pEssenceAccess->ReadSamples(dataLen, 
													sizeof(dataBuff),	// Maximum buffer size
													dataBuff,			// Buffer for the data
													&samplesRead,		// Actual number of samples read
													&actualBytesRead));	// Actual number of bytes read
							if (actualBytesRead!=0)
							{ 
								doSomethingWithAudioFile();
							} 
							bytesLeft = bytesLeft - actualBytesRead;
						}
						
						delete [] dataBuff;
						dataBuff = NULL;
						
						pEssenceAccess->Release();
						pEssenceAccess = NULL;
						pMasterMob->Release();
						pMasterMob = NULL;

						// Inform the user that the process has been successful
						printf("Essence was read from file.\n");
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

		pMobIter->Release();
		pMobIter = NULL;
	}
	else
	{
		printf("Error with file: File has no Master mobs.\n");
	}

cleanup:
  // Cleanup and return
  if (dataBuff)
    delete [] dataBuff;
    
  if (pFormat)
    pFormat->Release();

  if (fmtTemplate)
    fmtTemplate->Release();

  if (pEssenceAccess)
    pEssenceAccess->Release();

  if (pMasterMob)
    pMasterMob->Release();

  if (pTimelineMobSlot)
    pTimelineMobSlot->Release();

  if (pMobSlot)
    pMobSlot->Release();

  if (pMobSlotIter)
    pMobSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

  if (pSoundDef)
    pSoundDef->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	/* Close the AAF file */
	if (pFile) 
	{
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
    HRESULT hr = AAFLoad(dllname);
	if (!AAFRESULT_SUCCEEDED(hr)) {
	  fprintf(stderr, "Error : Failed to load the AAF library, ");
	  fprintf(stderr, "check environment variables -\n");
	  fprintf(stderr, "  Windows    - $PATH\n");
	  fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
	  exit(hr);
	}
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
	AAFByteOrder	nativeByteOrder = GetNativeByteOrder()
		;
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
	printf("Usage:\n ImportAudioExample.exe <file name>.aaf \n");
	printf(" If no filename is given, the filename defaults to ImportAudioExample.aaf\n\n");
}

//  Main adapted to use command-line arguments with argument checking
//  NOTE:  defining [0] program name; [1] filename.aaf; 
//  Specifying that use file ImportAudioExample.aaf as default
int main(int argumentCount, char* argumentVector[])
{
	CAAFInitialize aafInit;
	
	
	
	if (argumentCount ==1)
	{
		// Initialise filename variables to default settings and inform user
		pwFileName = L"ExportAudioExample.aaf";
		pFileName = "ExportAudioExample.aaf";

		printf("No file specified => defaulting to ExportAudioExample.aaf\n\n");
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

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	// Access the AAF file with name set from argument or lack thereof
	printf("Opening file %s using ReadSamples\n", pFileName);
	ReadAAFFile(pwFileName, testStandardCalls);

	printf("DONE\n\n");

	return(0);
}




