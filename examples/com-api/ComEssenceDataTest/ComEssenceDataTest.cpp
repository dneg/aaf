/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/
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


//#include "AAFPluginManager.h"

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

// Include the AAF interface declarations.
#include "AAF.h"




// Include the defintions for the AAF Stored Object identifiers.
#define INIT_AUID
#include "AAFStoredObjectIDs.h"




// This static variables are here so they can be referenced 
// thru out the whole program.

static aafSourceRef_t sourceRef; 

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}


static aafBool	EqualAUID(aafUID_t *uid1, aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? AAFTrue : AAFFalse);
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

static void AUIDtoString(aafUID_t *uid, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}

typedef enum { testRawCalls, testStandardCalls, testMultiCalls, testFractionalCalls } testType_t;

#define DEFAULT_SAMPLE_SIZE	8
#define SAMPLE_SIZE_BYTES	((DEFAULT_SAMPLE_SIZE + 7) / 8)

/*// app work on all WAVE files, not just laser.wav.
AAFRESULT CAAFWaveCodec::loadWAVEHeader(void)
{
	aafInt32			offset, chunkStart, formSize;
	aafInt64			chunkStart64;
	aafInt16			pcm_format;
	aafUInt8            chunkID[4];
	aafUInt32			chunkSize, bytesRead;
	aafBool				fmtFound = AAFFalse, dataFound = AAFFalse;
 	aafInt32			junk32, rate;
	aafInt64			savePos;
#if DEBUG_READ
	aafUInt8			debugBuf[64];
#endif

	if(_headerLoaded)
		return AAFRESULT_SUCCESS;
//!!!	pdata->fmtOps[0].opcode = kOmfAFmtEnd;
//!!!	_interleaveBuf = NULL;
	
	XPROTECT()
	{
//!!!		CvtInt32toInt64(0, &pdata->formSizeOffset);
//!!!		CvtInt32toInt64(0, &pdata->dataSizeOffset);
//!!!		CvtInt32toInt64(0, &pdata->numSamplesOffset);
		CHECK(_stream->GetPosition (&savePos));
	
#if DEBUG_READ
		CHECK(_stream->Seek(0L));
		_stream->Read(64L, debugBuf, &bytesRead);
#endif
		CHECK(_stream->Seek(0L));
		CHECK(_stream->Read(4L, chunkID, &bytesRead));
		if (memcmp(&chunkID, "RIFF", (size_t) 4) != 0)
			RAISE(OM_ERR_BADWAVEDATA);
		CHECK(GetWAVEData(4L, (void *) &formSize));	
		CHECK(_stream->Read(4L, chunkID, &bytesRead));
		if (memcmp(&chunkID, "WAVE", (size_t) 4) != 0)
			RAISE(OM_ERR_BADWAVEDATA);
		CHECK(_stream->GetPosition(&chunkStart64));
		CHECK(TruncInt64toInt32(chunkStart64, &offset));	// OK - 32-bit format
	
		while ((offset < formSize) && _stream->Read(4L, chunkID, &bytesRead) == AAFRESULT_SUCCESS)
		{
			CHECK(GetWAVEData(4L, (void *) &chunkSize));	
			CHECK(_stream->GetPosition(&chunkStart64));
	
			if (memcmp(&chunkID, "fmt ", (size_t) 4) == 0)
			{
				// WAVE field: wFormatTag
				CHECK(GetWAVEData(2L, &pcm_format));
				if (pcm_format != 1)
					RAISE(OM_ERR_BADWAVEDATA);
	
				// WAVE field: wChannels
				CHECK(GetWAVEData(2L, &_numCh));
	
				// WAVE field: wSamplesPerSec
				CHECK(GetWAVEData(4L, &rate));
				_sampleRate.numerator = rate;
				_sampleRate.denominator = 1;
	
				// Skip WAVE field: avgBytesPerSec (4 bytes)
				CHECK(GetWAVEData(4L, &junk32));
	
				// WAVE field wBlockAlign
				CHECK(GetWAVEData(2L, &_bytesPerFrame));
	
				// WAVE field Sample Width
				CHECK(GetWAVEData(2L, &_bitsPerSample));
	
				_bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
	
				fmtFound = AAFTrue;
			} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
			{
				CvtInt32toInt64(chunkSize / _bytesPerFrame, &_sampleFrames);
				// Positioned at beginning of audio data
				CHECK(_stream->GetPosition(&_dataStartOffset));
	
				dataFound = AAFTrue;
			}
			CHECK(TruncInt64toInt32(chunkStart64, &chunkStart));	// OK - 32-bit format
			offset = chunkStart + chunkSize;
	
			if(offset > formSize)
				break;
			if (fmtFound && dataFound)	// Do we have all information yet?
				break;
			CHECK(_stream->Seek(offset));
		}
		_headerLoaded = AAFTrue;
		CHECK(_stream->Seek (savePos));
	}
	XEXCEPT
	{
		RERAISE(OM_ERR_BADWAVEDATA);
	}
	XEND
	
	
	return(AAFRESULT_SUCCESS);
}
*/

static HRESULT CreateAAFFile(aafWChar * pFileName, testType_t testType)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		aDesc = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat*			pFormat;
/*	
	//	This variables are needed if the Essence data is in a separate
	//	data file.

	IAAFLocator*				pLocator = NULL;
	IAAFNetworkLocator*			pNetLocator = NULL;
	IAAFFileDescriptor*			pFileDesc = NULL;
*/
	aafUID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {44100, 1};
	aafRational_t				sampleRate = {44100, 1};
	FILE*						pWavFile = NULL;
	unsigned char				dataBuff[4096], *dataPtr;
	size_t						bytesRead, dataLen;
	aafInt32					bytesLeft;
	aafUInt32					bytesWritten;

  
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);



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

	check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));

	// Create and open new AAF File
	check(pFile->Initialize());
	check(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
	check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));


/* This code is needed if the Essence data is to be saved
	in a separate file. For now it is commented out. 
	// Get a file locator Interface
	check(CoCreateInstance( CLSID_AAFFileDescriptor,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFileDescriptor, 
						   (void **)&pFileDesc));
	check(pFileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc));

	// Get a Locator Interface, and attach it to the EssenceDescriptor
	check(CoCreateInstance(CLSID_AAFNetworkLocator,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFNetworkLocator, 
							(void **)&pNetLocator));		
	check(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));
	check(pLocator->SetPath (TEST_PATH));	
	check(aDesc->AppendLocator(pLocator));
*/
	// Get a Master MOB Interface
	check(pDictionary->CreateInstance( &AUID_AAFMasterMob,
						   IID_IAAFMasterMob, 
						   (IUnknown **)&pMasterMob));
	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	check(pMob->SetName(L"A Master Mob"));
	
	// Add it to the file 
	check(pHeader->AppendMob(pMob));

/*
	since the data will be saved in the same file we do not need to make
	this call. Will be used when writing a separate Essence Data file.
	check(pEssenceAccess->SetEssenceDestination(pLocator, kAAFiMedia));
*/
	// now create the Essence data file
	check(pMasterMob->CreateEssence(1,				// Slot ID
									DDEF_Audio,		// MediaKind
									CodecWave,		// codecID
									editRate,		// edit rate
									sampleRate,		// sample rate
									kSDKCompressionDisable,
									NULL,			// In current file
									kAAFiMedia,		// In AAF Format
									&pEssenceAccess));// Compress disabled

	// open the Essence file to be included in this AAF file("Laser.wav")
	pWavFile = fopen("Laser.wav", "r");
	if (pWavFile)
	{
		// read in the essence data
		bytesRead = fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
		if(testType != testRawCalls)
		{
			dataPtr = dataBuff;
			bytesLeft = bytesRead;
			while(bytesLeft > 0)
			{
				while(bytesLeft > 0 && *dataPtr != 'd')
				{
					dataPtr++;
					bytesLeft--;
				}
				if(memcmp(dataPtr, "data", 4) == 0)
					break;
			}
						//!!! In development.  NOT industrial strength WAVE reader code.
						// Assumes wave chunk is the last chunk & doesn't parse all chunks
			dataPtr += 8;
			dataLen = bytesLeft - 8;
			// Assumes 8-bit WAVE data
		}
		else
		{	
			dataPtr = dataBuff;
			dataLen = bytesRead;
		}

		IAAFEssenceFormat	*format;
		aafInt32			n, numSpecifiers;
		aafUID_t			essenceFormatCode;
		check(pEssenceAccess->GetFileFormatParameterList (&format));
		check(format->NumFormatSpecifiers (&numSpecifiers));
		for(n = 0; n < numSpecifiers; n++)
		{
			check(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
		}

		// Tell the AAFEssenceAccess what the format is.
		check(pEssenceAccess->GetEmptyFileFormat (&pFormat));
		check(pFormat->NumFormatSpecifiers (&numSpecifiers));

		aafInt32	sampleSize = DEFAULT_SAMPLE_SIZE;
		check(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
		check(pEssenceAccess->PutFileFormat (pFormat));
		
		// write out the data
		if(testType == testRawCalls)
		{
			check(pEssenceAccess->WriteRawData(	dataLen,	// Number of Samples
											dataPtr,	// THE Raw data
											sizeof(dataBuff)));// buffer size
		}
		else if(testType == testStandardCalls)
		{
			check(pEssenceAccess->WriteSamples(	dataLen,	//!!! hardcoded bytes/sample ==1// Number of Samples
											dataPtr,	// THE Raw data
											sizeof(dataBuff)));// buffer size
		}
		else if(testType == testFractionalCalls)
		{
			check(pEssenceAccess->WriteFractionalSample(
											dataLen,			// number of bytes
											dataPtr,			// THE data	
											&bytesWritten));	// !!!Check this when it works
		}
		else if(testType == testMultiCalls)
		{
			aafmMultiXfer_t xfer;

//!!!		xfer.subTrackNum = _channels[0].physicalOutChan;
			xfer.numSamples = dataLen;	//!!! hardcoded bytes/sample ==1
			xfer.buflen = sizeof(dataBuff);
			xfer.buffer = dataPtr;
			xfer.bytesXfered = 0;
	
			check(pEssenceAccess->WriteMultiSamples(1, &xfer));
		}

		// close essence data file
		fclose(pWavFile);

		// Finish writing the destination
		pEssenceAccess->CompleteWrite();
	}
	else
	{
		printf("***Failed to open Wave file Laser.wav\n");
	}

	// Release all unnecesary interfaces
	pEssenceAccess->Release();
	pEssenceAccess= NULL;

	pMasterMob->Release();
	pMasterMob = NULL;
	pMob->Release();
	pMob = NULL;

  pDictionary->Release();
  pDictionary = NULL;
	pHeader->Release();
	pHeader = NULL;
	pFile->Close();
	pFile->Release();
	pFile = NULL;

cleanup:
	// Cleanup and return
	if (pEssenceAccess)
		pEssenceAccess->Release();
	
	if (pMasterMob)
		pMasterMob->Release();

	if (pMob)
		pMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
		pFile->Release();

	return moduleErrorTmp;
}

static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat*			pFormat = NULL;

	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafUID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char				AAFDataBuf[4096];
	aafUInt32					AAFBytesRead, samplesRead;
	FILE*						pWavFile = NULL;
	unsigned char				WAVDataBuf[4096], *dataPtr;
	size_t						WAVBytesRead, dataLen;
	aafInt32					bytesLeft;

	check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));
	check(pFile->Initialize());
	check(pFile->OpenExistingRead(pFileName, 0));
	check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));


	// Here we check on the number of mobs in the file. 
	// Get the number of master mobs in the file (should be one)
	check(pHeader->GetNumMobs(kMasterMob, &numMobs));
	if (1 == numMobs )
	{
		printf("Found %ld Master Mobs\n", numMobs);
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kMasterMob;
		check(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];


			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			AUIDtoString(&mobID, mobIDstr);
			printf("    MasterMob Name = '%s'\n", mobName);
			printf("        (mobID %s)\n", mobIDstr);
			// Make sure we have one slot 
			check(pMob->GetNumSlots(&numSlots));
			if (1 == numSlots)
			{
				// The essence data is in SlotID 1
				// Get a Master Mob interface
				check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

				// Open the Essence Data
					check(pMasterMob->OpenEssence(	1,						// SlotID 1
											NULL,				// mediaCriteria (Don't care)
											kMediaOpenReadOnly,	// Open mode
											kSDKCompressionDisable,// Compress disabled
											&pEssenceAccess));

				// Open and read the Wave file (for comparison)
				pWavFile = fopen("Laser.wav", "r");
				if (pWavFile)
				{
					// read in the essence data
					WAVBytesRead = fread(WAVDataBuf, sizeof(unsigned char), sizeof(WAVDataBuf), pWavFile);
					fclose(pWavFile);
					if(testType != testRawCalls)
					{
						dataPtr = WAVDataBuf;
						bytesLeft = WAVBytesRead;
						while(bytesLeft > 0)
						{
							while(bytesLeft > 0 && *dataPtr != 'd')
							{
								dataPtr++;
								bytesLeft--;
							}
							if(memcmp(dataPtr, "data", 4) == 0)
								break;
						}
						dataPtr += 8;
						bytesLeft -= 8;
						dataLen = bytesLeft;
					}
					else
					{	
						dataPtr = WAVDataBuf;
						dataLen = WAVBytesRead;
					}

					aafUInt32			sampleBits;
					aafInt32			bytesRead;
					IAAFEssenceFormat	*fmtTemplate;
					
					check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
					check(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
					check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
					check(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleBits),
                           (aafDataBuffer_t)&sampleBits, &bytesRead));
					if(sampleBits != DEFAULT_SAMPLE_SIZE)
					{
						printf("***Wrong sample size read ( was %ld , should be %ld)\n",
							sampleBits, DEFAULT_SAMPLE_SIZE);
					}

					// Read the Raw Data from the AAF file
					if(testType == testRawCalls)
					{
						check(pEssenceAccess->ReadRawData(	dataLen,		// Number of Samples 
														sizeof(AAFDataBuf),	// Maximum buffer size
														AAFDataBuf,			// Buffer for the data
														&AAFBytesRead,	// Actual number of bytes read
														&samplesRead));		// Actual number of samples read
					}
					else if(testType == testStandardCalls)
					{
						check(pEssenceAccess->ReadSamples(	dataLen,	//!!! Hardcoded	// Number of Samples 
														sizeof(AAFDataBuf),	// Maximum buffer size
														AAFDataBuf,			// Buffer for the data
														&samplesRead,		// Actual number of samples read
														&AAFBytesRead));	// Actual number of bytes read
					}
					else if(testType == testFractionalCalls)
					{
						check(pEssenceAccess->ReadFractionalSample(dataLen,	// bytes to read
														sizeof(AAFDataBuf),	// Maximum buffer size
														AAFDataBuf,			// Buffer for the data
														&AAFBytesRead));	// Actual number of bytes read
					}
					else if(testType == testMultiCalls)
					{
						aafmMultiXfer_t xfer;

						xfer.numSamples = dataLen;	//!!! Hardcoded	// Number of Samples 
						xfer.buflen = sizeof(AAFDataBuf);
						xfer.buffer = AAFDataBuf;
						xfer.bytesXfered = 0;
						check(pEssenceAccess->ReadMultiSamples(1, &xfer));
						samplesRead = xfer.samplesXfered;
						AAFBytesRead = xfer.bytesXfered;
					}

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
				}
				else
				{
					printf("***Failed to open Wave file Laser.wav for comparison\n");
				}
			}
			else
			{
				printf("***Wrong number of slots in the Master Mob (was %ld should be %ld)\n",
					numSlots, 1L);
			}
			if (pEssenceAccess)
			{
				pEssenceAccess->Release();
				pEssenceAccess = NULL;
			}
			if (pMasterMob)
			{
				pMasterMob->Release();
				pMasterMob = NULL;
			}

			pMob->Release();
			pMob = NULL;
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


	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	return moduleErrorTmp;
}


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
	aafWChar * pwFileName = L"EssenceTest.aaf";
	const char * pFileName = "EssenceTest.aaf";

//!!!	IAAFEssencePlugin *codecManager = NULL;
//	aafInt32		numCodecs;

	// This call to CoCreateInstance is here to allow the DLL to be quickly
	// loaded for debug.  It now serves no other function.
//!!!	CoCreateInstance(CLSID_AAFEssencePlugin,
//               NULL, 
  //             CLSCTX_INPROC_SERVER, 
 //              IID_IAAFEssencePlugin, 
 //              (void **)&codecManager);
//	codecManager->NumCodecsMatching (DDEF_Audio, kAAFRev1, &numCodecs);


	printf("***Creating file %s using writeRawData\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, testRawCalls));
	printf("***Re-opening file %s using readRawData\n", pFileName);
	ReadAAFFile(pwFileName, testRawCalls);
	printf("***Creating file %s using WriteSamples\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, testStandardCalls));
	printf("***Re-opening file %s using ReadSamples\n", pFileName);
	ReadAAFFile(pwFileName, testStandardCalls);
	printf("***Creating file %s using WriteMultiSamples\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, testMultiCalls));
	printf("***Re-opening file %s using ReadMultiSamples\n", pFileName);
	ReadAAFFile(pwFileName, testMultiCalls);
	printf("***Creating file %s using WriteFractionalSample\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, testFractionalCalls));
	printf("***Re-opening file %s using ReadFractionalSample\n", pFileName);
	ReadAAFFile(pwFileName, testFractionalCalls);

	printf("Done\n");


	return(0);
}




