// @com Executable test program by Chris Morgan, intern for Avid Technology, Tewksbury 
// @com This is used for scalability testing of AAF code.  Last modified on 7/23/99.

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

//#include "AAFPluginManager.h"

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

#if USE_TIMER_LIB
// Include this for the purpose of timing the data read/write
#include "UtlConsole.h"
#endif

#if defined( OS_MACOS )
#include "DataInput.h"
#endif

// This static variables are here so they can be referenced 
// throughout the whole program.

static aafSourceRef_t sourceRef; 

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
	sprintf(buf, "%02x%02x%02x%02x%02x%02x%02x%02x--%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
		(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], (int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3], 
		(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], (int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7], 
		(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[8], (int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11], 
		(int)uid.length, (int)uid.instanceHigh, (int)uid.instanceMid, (int)uid.instanceLow, 
		uid.material.Data1, uid.material.Data2, uid.material.Data3, (int)uid.material.Data4[0],
		(int)uid.material.Data4[1], (int)uid.material.Data4[2], (int)uid.material.Data4[3],
		(int)uid.material.Data4[4],
		(int)uid.material.Data4[5], (int)uid.material.Data4[6], (int)uid.material.Data4[7]);
}

typedef enum { testStandardCalls, testMultiCalls } testType_t;

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
} testDataFile_t;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

//  These fudgy bits are needed to get correct output...
char* externalkind = "";
char* kind = "";
char* location = "";


//  Note: function modified to take N as an argument
static HRESULT CreateAAFFile(aafWChar * pFileName, testDataFile_t *dataFile, testType_t testType, long int N)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;

	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IAAFEssenceFormat			*format = NULL;
	IAAFLocator					*pLocator = NULL;
	IAAFClassDef                *pCDMasterMob = 0;
	IAAFClassDef                *pCDNetworkLocator = 0;
	IAAFDataDef                 *pDdefSound = 0;
	// !!!Previous revisions of this file contained variables here required to handle external essence
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {44100, 1};
	aafRational_t				sampleRate = {44100, 1};
	FILE*						pWavFile = NULL;
	unsigned char				dataBuff[4096], *dataPtr;
	size_t						bytesRead;
//	aafUInt32					bytesWritten;
	aafUInt32					dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
		aafInt32			n, numSpecifiers;
		aafUID_t			essenceFormatCode, testContainer;
  aafUInt32 samplesWritten, bytesWritten;

		// delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);
  if(dataFile != NULL)
  {
	// delete any previous test file before continuing...
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
	ProductInfo.companyName = L"AAF-East Avid@Tewksbury";
	ProductInfo.productName = L"Essence Data Transfer Rate Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;

	check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary so that we can create valid AAF objects.
	check(pHeader->GetDictionary(&pDictionary));

	check(pDictionary->LookupClassDef(AUID_AAFMasterMob,
									  &pCDMasterMob));
	check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator,
									  &pCDNetworkLocator));
	check(pDictionary->LookupDataDef(DDEF_Sound,
									 &pDdefSound));

	// !!!Previous revisions of this file contained code here required to handle external essence

  // Get a Master MOB Interface
	check(pCDMasterMob->
		  CreateInstance(IID_IAAFMasterMob, 
						 (IUnknown **)&pMasterMob));
	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	check(pMob->SetName(L"A Master Mob"));
	
	// Add it to the file 
	check(pHeader->AddMob(pMob));

	// !!!Previous revisions of this file contained code here required to handle external essence

	if(dataFile != NULL)
	{
		// Make a locator, and attach it to the EssenceDescriptor
		check(pCDNetworkLocator->
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

	//  yet more fudge to fix the output
	
	if (dataFile == 0)
		location = "internal";
	else
		location = "external";
	
	
	// open the Essence file to be included in this AAF file("Laser.wav")
	pWavFile = fopen("Laser.wav", "r");
	if (pWavFile)
	{
		// read in the essence data
		bytesRead = fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
		check(loadWAVEHeader(dataBuff,
										&bitsPerSample,
										&numCh,
										&sampleRate,
										&dataOffset,
										&dataLen));
		dataPtr = dataBuff + dataOffset;

		// now create the Essence data file
		check(pMasterMob->CreateEssence(1,				// Slot ID
									pDdefSound,		// MediaKind
									kAAFCodecWAVE,		// codecID
									editRate,		// edit rate
									sampleRate,		// sample rate
									kAAFCompressionDisable,
									pLocator,	// In current file
									testContainer,	// In AAF Format
									&pEssenceAccess));// Compress disabled

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

		aafInt32	sampleSize = bitsPerSample;
		check(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
		check(pEssenceAccess->PutFileFormat (pFormat));
		pFormat->Release();
		pFormat = NULL;
	 
#if USE_TIMER_LIB
		//  start the write timer here
		aafUInt32 timerID, elapsedtime;
		moduleErrorTmp = UTLStartPeriod(&timerID);
#endif

		aafUInt32 dataWritten =0;
		aafUInt32 dataWriteRate = 0;
		
		// the loop for creating files of cli-set size is below

		for (long i=0; i<N; i++)
		{

			// write out the data
			if(testType == testStandardCalls)
			{
				check(pEssenceAccess->WriteSamples(	dataLen,		//!!! hardcoded bytes/sample ==1// Number of Samples
												sizeof(dataBuff), 	// buffer size
												dataPtr,			// THE  data
												&samplesWritten,
												&bytesWritten));
				kind = "standard calls";
			}
			else if(testType == testMultiCalls)
			{
				aafmMultiXfer_t		xfer;
				aafmMultiResult_t	result;	

				check(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
	//!!!		xfer.subTrackNum = _channels[0].physicalOutChan;
				xfer.numSamples = dataLen;	//!!! hardcoded bytes/sample ==1
				xfer.buflen = sizeof(dataBuff);
				xfer.buffer = dataPtr;
				result.bytesXfered = 0;
		
				check(pMultiEssence->WriteMultiSamples(1, &xfer, &result));
				pMultiEssence->Release();
				pMultiEssence = NULL;


				samplesWritten = result.samplesXfered;
				bytesWritten = result.bytesXfered;
				
				kind  = "multi calls";
			}

		//accumulate the number of essence bytes written
		dataWritten = dataWritten + dataLen;
		//  end of the loop
		}

#if USE_TIMER_LIB
		//  stop the timer here
		moduleErrorTmp = UTLEndPeriod(timerID, &elapsedtime);

		//  now return the desired statistics 
		if (elapsedtime!=0)	dataWriteRate = ((dataWritten/elapsedtime)*1000)/1024;
		else dataWriteRate = 0;
	
		//  check output is of correct form for grep
		if (location == "internal")
			externalkind = "";

		printf("Write rate (%s%s %s) = %ld kb/sec KBytes = %ld\n",externalkind, location, kind, dataWriteRate,dataWritten/1024);
#endif
		
		// close essence data file
		fclose(pWavFile);
    pWavFile = NULL;

		// Finish writing the destination
		check(pEssenceAccess->CompleteWrite());

	}
	else
	{
		printf("***Failed to open Wave file Laser.wav\n");
	}

	// Release all unnecesary interfaces

	if(pMasterMob)
		pMasterMob->Release();
	pMasterMob = NULL;
	if(pMob)
		pMob->Release();
	pMob = NULL;

	if(pDictionary)
		pDictionary->Release();
	pDictionary = NULL;
	if(pHeader)
		pHeader->Release();
	pHeader = NULL;
	//!!!DebugOnly
	pFile->Save();
	pFile->Close();
	pFile->Release();
	pFile = NULL;
	if (pEssenceAccess)
	{	
		pEssenceAccess->Release();
		pEssenceAccess= NULL;
	}

cleanup:
	// Cleanup and return
  if (pWavFile)
    fclose(pWavFile);

  if(pFormat)
		pFormat->Release();

	if(format)
		format->Release();

	if(pLocator)
		pLocator->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();
	
	if (pMultiEssence)
		pMultiEssence->Release();
	
	if (pMasterMob)
		pMasterMob->Release();

	if (pMob)
		pMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pCDMasterMob)
	  {
		pCDMasterMob->Release();
		pCDMasterMob = 0;
	  }

	if (pCDNetworkLocator)
	  {
		pCDNetworkLocator->Release();
		pCDNetworkLocator = 0;
	  }

	if (pDdefSound)
	  {
		pDdefSound->Release();
		pDdefSound = 0;
	  }

	if (pFile)
  {
    pFile->Close();
		pFile->Release();
  }


	return moduleErrorTmp;
}

static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IAAFDataDef*				pddSound = 0;

	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char						*AAFDataBuf = NULL;
	aafUInt32					AAFBytesRead, samplesRead;
	FILE*						pWavFile = NULL;
//	size_t						WAVBytesRead;
	aafUInt32					/* dataOffset, */samplesToRead, bytesToRead;
	aafLength_t				sampleCount;

	check(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary so that we can create valid AAF objects.
	check(pHeader->GetDictionary(&pDictionary));

	// Here we check on the number of mobs in the file. 
	// Get the number of master mobs in the file (should be one)
	check(pHeader->CountMobs(kAAFMasterMob, &numMobs));
	if (1 == numMobs )
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
			// Make sure we have one slot 
			check(pMob->CountSlots(&numSlots));
			if (1 == numSlots)
			{
				// The essence data is in SlotID 1
				// Get a Master Mob interface
				check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

				// Open the Essence Data
					check(pMasterMob->OpenEssence(	1,						// SlotID 1
											NULL,				// mediaCriteria (Don't care)
											kAAFMediaOpenReadOnly,	// Open mode
											kAAFCompressionDisable,// Compress disabled
											&pEssenceAccess));

#if 0
					// Open and read the Wave file (for comparison)
				pWavFile = fopen("Laser.wav", "r");
				if (pWavFile)
				{
					// read in the essence data
					WAVBytesRead = fread(WAVDataBuf, sizeof(unsigned char), sizeof(WAVDataBuf), pWavFile);
					fclose(pWavFile);
          pWavFile = NULL
					check(loadWAVEHeader(WAVDataBuf,
										&bitsPerSample,
										&numCh,
										&readSampleRate,
										&dataOffset,
										&dataLen));
					dataPtr = WAVDataBuf + dataOffset;
					
#else
					check(pDictionary->LookupDataDef(DDEF_Sound, &pddSound));
					check(pEssenceAccess->CountSamples(pddSound, &sampleCount));
					samplesToRead = (aafInt32)sampleCount;		// !!! Possible loss of data if > 4gig
#endif
					aafUInt32			sampleBits;
					aafInt32			bytesRead;

					check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
					check(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
					check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
					fmtTemplate->Release();
					fmtTemplate = NULL;
					
					check(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleBits),
                           (aafDataBuffer_t)&sampleBits, &bytesRead));
					pFormat->Release();
					pFormat = NULL;
#if 0
					if(sampleBits != bitsPerSample)
					{
						printf("***Wrong sample size read ( was %ld , should be %ld)\n",
							sampleBits, bitsPerSample);
					}
#endif
					bytesToRead = samplesToRead * (sampleBits+7)/8;
					AAFDataBuf = (unsigned char *)new char[bytesToRead];

#if USE_TIMER_LIB
					// start the read timer here

					aafUInt32 timerID, elapsedtime;
					moduleErrorTmp = UTLStartPeriod(&timerID);

					aafUInt32 dataRead =0;
					aafUInt32 dataReadRate = 0;
#endif

					// Read the Data from the AAF file
					if(testType == testStandardCalls)
					{
						check(pEssenceAccess->ReadSamples(	samplesToRead,	//!!! Hardcoded	// Number of Samples 
														bytesToRead,	// Maximum buffer size
														AAFDataBuf,			// Buffer for the data
														&samplesRead,		// Actual number of samples read
														&AAFBytesRead));	// Actual number of bytes read
					}
					else if(testType == testMultiCalls)
					{
						aafmMultiXfer_t		xfer;
						aafmMultiResult_t	result;

						check(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
						xfer.numSamples = samplesToRead;	//!!! Hardcoded	// Number of Samples 
						xfer.buflen = bytesToRead;
						xfer.buffer = AAFDataBuf;
						result.bytesXfered = 0;
						check(pMultiEssence->ReadMultiSamples(1, &xfer, &result));
						samplesRead = result.samplesXfered;
						AAFBytesRead = result.bytesXfered;
						pMultiEssence->Release();
						pMultiEssence = NULL;
					}

						
#if USE_TIMER_LIB
					//  stop the timer here
					moduleErrorTmp = UTLEndPeriod(timerID, &elapsedtime);
					
					//  fill up the dataRead variable
					dataRead=AAFBytesRead;
					//  now return the desired statistics 
					if (elapsedtime!=0)	dataReadRate = ((dataRead/elapsedtime)*1000)/1024;
					else dataReadRate = 0;

					//  convert pFileName to a printable form
					char niceFileName[80];
					wcstombs(niceFileName,pFileName,80);
									
					printf("File = %s KBytes = %ld Time(ms) = %ld Rate = %ld kb/sec\n",niceFileName,dataRead/1024,elapsedtime,dataReadRate);
#endif

#if 0
					// Now compare the data read from the AAF file to the actual WAV file
					if (dataLen != AAFBytesRead)
					{
						printf("***Wrong number of bytes read ( was %ld , should be %ld)\n",
							AAFBytesRead, WAVBytesRead);
					}
					if (memcmp( dataPtr, AAFDataBuf, dataLen) != 0)
					{
						printf("*** Data Read is different than the data in the WAV file ***\n");
					}
#endif
					delete [] AAFDataBuf;
          AAFDataBuf = NULL;

          if (pWavFile)
          { // close essence data file
		        fclose(pWavFile);
            pWavFile = NULL;
          }
#if 0
				}
				else
				{
					printf("***Failed to open Wave file Laser.wav for comparison\n");
				}
#endif
			}
			else
			{
				printf("***Wrong number of slots in the Master Mob (was %ld should be %ld)\n",
					numSlots, 1L);
			}
			if (pMasterMob)
			{
				pMasterMob->Release();
				pMasterMob = NULL;
			}

			pMob->Release();
			pMob = NULL;
			if (pEssenceAccess)
			{
				pEssenceAccess->Release();
				pEssenceAccess = NULL;
			}
		}

		pMobIter->Release();
		pMobIter = NULL;

	}
	else
	{
		printf("***Wrong number of Master mobs in the file (was %ld should be %ld)\n",
			numMobs, 1L);
	}

	printf("--------\n");

cleanup:
	// Cleanup and return
  if (AAFDataBuf)
    delete [] AAFDataBuf;

  if (pWavFile)
    fclose(pWavFile);

	if (pMultiEssence)
		pMultiEssence->Release();
	if(fmtTemplate)
	{
		fmtTemplate->Release();
		fmtTemplate = NULL;
	}
	if (pEssenceAccess)
	{
		pEssenceAccess->Release();
		pEssenceAccess = NULL;
	}
	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();
	if (pMobIter)
		pMobIter->Release();
	if (pFormat)
		pFormat->Release();

	if (pddSound)
	  {
		pddSound->Release();
		pddSound = 0;
	  }

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

  if (byte == 0x12) {
    result = MOTOROLA_ORDER;
  } else {
    result = INTEL_ORDER;
  }
  return result;
}

void AAFByteSwap32(
			aafInt32 *lp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) lp;
	int             t;

	t = cp[3];
	cp[3] = cp[0];
	cp[0] = t;
	t = cp[2];
	cp[2] = cp[1];
	cp[1] = t;
}

void AAFByteSwap16(
			aafInt16 * wp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) wp;
	int             t;

	t = cp[1];
	cp[1] = cp[0];
	cp[0] = t;
}

// app work on all WAVE files, not just laser.wav.
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
  HRESULT hr = S_OK;
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
static void usage(void)
{
	printf ("Usage:\n EssenceAccess.exe <Number of copies of Laser.wav in AAF file>\n");
	printf (" Number is required to be integer greater than zero.\n\n");
}

//  Main adapted to use command-line arguments with argument checking
//  NOTE:  defining [0] program name; [1] Number N of wave files
int main(int argumentCount, char *argumentVector[])
{
	//  First checking for correct number of arguments 

	if (argumentCount != 2)
	{
		usage();
		return 0;
	}

	//  Processing the second argument to be stored as global variable N
	char* Ns = argumentVector[1];
	char* expectedEnd = &Ns[strlen(Ns)];
	char* end = 0;
	long int N = strtoul(Ns, &end, 10);
	
	//  Testing for correct second argument (less than one)
	if ((end != expectedEnd) || (N < 1))
	{ 
		printf("The first argument was of the incorrect form. [%s]\n\n",argumentVector[1]);
		usage();
		return 0;
	}
	
#if USE_TIMER_LIB
	//  Initialise timers
	UTLInitTimers(1000);
#endif

	CAAFInitialize aafInit;

  // Make sure all of our required plugins have been registered.
  checkFatal(RegisterRequiredPlugins());


	//  The new, non-interleaved code to fix the caching issues relating to statistic gathering

	//  first, for the write calls, creating 6 different files, one for each test type

	aafWChar *		pwFileName = L"";
	char *	pFileName = "";
	aafWChar *	externalAAF = L"ExternalAAFEssence.aaf";
	testDataFile_t	dataFile;
	
	
	pwFileName = L"InternalStandard.aaf";
	pFileName = "InternalStandard.aaf";
	printf("***Creating file %s using WriteSamples (Internal Media)\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, NULL, testStandardCalls, N));
	
	pwFileName = L"InternalMulti.aaf";
	pFileName = "InternalMulti.aaf";
	printf("***Creating file %s using WriteMultiSamples (Internal Media)\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, NULL, testMultiCalls, N));
			
	pwFileName = L"ExternalStandardAAF.aaf";
	pFileName = "ExternalStandardAAF.aaf";
	dataFile.dataFilename = externalAAF;
	dataFile.dataFormat = ContainerAAF;
	externalkind = "AAF ";
	printf("***Creating file %s using WriteSamples (External AAF Media)\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, &dataFile, testStandardCalls, N));
	
	//  now, for the read calls, reading the 6 different files created
		
	pwFileName = L"InternalStandard.aaf";
	pFileName = "InternalStandard.aaf";
	printf("***Re-opening file %s using ReadSamples\n", pFileName);
	ReadAAFFile(pwFileName, testStandardCalls);
	
	pwFileName = L"InternalMulti.aaf";
	pFileName = "InternalMulti.aaf";
	printf("***Re-opening file %s using ReadMultiSamples\n", pFileName);
	ReadAAFFile(pwFileName, testMultiCalls);
			
	pwFileName = L"ExternalStandardAAF.aaf";
	pFileName = "ExternalStandardAAF.aaf";
	dataFile.dataFilename = externalAAF;
	dataFile.dataFormat = ContainerAAF;
	externalkind = "AAF ";
	printf("***Re-opening file %s using ReadSamples\n", pFileName);
	ReadAAFFile(pwFileName, testStandardCalls);
	
	//  and now, we're all done...
	printf("Done\n");

	return(0);
	
}




