// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceAccess
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

//#include "AAFPluginManager.h"

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDS.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"




// This static variables are here so they can be referenced 
// thru out the whole program.

static aafSourceRef_t sourceRef; 


static aafBool	EqualAUID(aafUID_t *uid1, aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? AAFTrue : AAFFalse);
}

#define TEST_PATH	L"SomeFile.dat"

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


static void convert(char* cName, size_t length, const wchar_t* name)
{
	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
        cerr << ": Error : Conversion failed." << endl;
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

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola


static AAFByteOrder GetNativeByteOrder(void);
static void AAFByteSwap32(
				   aafInt32 *lp);	/* IN/OUT -- Byte swap this value */
static void AAFByteSwap16(
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

static HRESULT CreateAAFFile(aafWChar * pFileName, testDataFile_t *dataFile, testType_t testType)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceRawAccess*		pRawEssence = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IAAFEssenceFormat			*format = NULL;
	IAAFLocator					*pLocator = NULL;
	// !!!Previous revisions of this file contained variables here required to handle external essence
	aafUID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {44100, 1};
	aafRational_t				sampleRate = {44100, 1};
	FILE*						pWavFile = NULL;
	unsigned char				dataBuff[4096], *dataPtr;
	size_t						bytesRead;
	aafUInt32					bytesWritten, dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
	aafInt32			n, numSpecifiers;
	aafUID_t			essenceFormatCode, testContainer;
	// delete any previous test file before continuing...
	char chFileName[1000];

	try
	{

		convert(chFileName, sizeof(chFileName), pFileName);
		remove(chFileName);
		if(dataFile != NULL)
		{
			// delete any previous test file before continuing...
			char chFileName[1000];
			convert(chFileName, sizeof(chFileName), dataFile->dataFilename);
			remove(chFileName);
		}
		
		
		ProductInfo.companyName = L"AAF Developers Desk";
		ProductInfo.productName = L"Essence Data Test";
		ProductInfo.productVersion.major = 1;
		ProductInfo.productVersion.minor = 0;
		ProductInfo.productVersion.tertiary = 0;
		ProductInfo.productVersion.patchLevel = 0;
		ProductInfo.productVersion.type = kVersionUnknown;
		ProductInfo.productVersionString = NULL;
		ProductInfo.productID = NIL_UID;
		ProductInfo.platform = NULL;
		
		checkResult(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		// Get a Master MOB Interface
		checkResult(pDictionary->CreateInstance( &AUID_AAFMasterMob,
			IID_IAAFMasterMob, 
			(IUnknown **)&pMasterMob));
		// Get a Mob interface and set its variables.
		checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->GetMobID(&masterMobID));
		checkResult(pMob->SetName(L"A Master Mob"));
		
		// Add it to the file 
		checkResult(pHeader->AppendMob(pMob));
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		if(dataFile != NULL)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			checkResult(pDictionary->CreateInstance(&AUID_AAFNetworkLocator,
				IID_IAAFLocator, 
				(IUnknown **)&pLocator));		
			checkResult(pLocator->SetPath (dataFile->dataFilename));
			testContainer = dataFile->dataFormat;
		}
		else
		{
			pLocator = NULL;
			testContainer = ContainerAAF;
		}
		
		
		// open the Essence file to be included in this AAF file("Laser.wav")
		pWavFile = fopen("Laser.wav", "r");
		checkExpression(pWavFile != NULL, AAFRESULT_TEST_FAILED);
		// read in the essence data
		bytesRead = fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
		checkResult(loadWAVEHeader(dataBuff,
			&bitsPerSample,
			&numCh,
			&sampleRate,
			&dataOffset,
			&dataLen));
		if(testType != testRawCalls)
		{
			dataPtr = dataBuff + dataOffset;
		}
		else
		{	
			dataPtr = dataBuff;
			dataLen = bytesRead;
		}
		
		// now create the Essence data file
		checkResult(pMasterMob->CreateEssence(1,				// Slot ID
			DDEF_Sound,		// MediaKind
			CodecWave,		// codecID
			editRate,		// edit rate
			sampleRate,		// sample rate
			kSDKCompressionDisable,
			pLocator,	// In current file
			testContainer,	// In AAF Format
			&pEssenceAccess));// Compress disabled
		
		checkResult(pEssenceAccess->GetFileFormatParameterList (&format));
		checkResult(format->NumFormatSpecifiers (&numSpecifiers));
		for(n = 0; n < numSpecifiers; n++)
		{
			checkResult(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
		}
		format->Release();
		format = NULL;
		
		// Tell the AAFEssenceAccess what the format is.
		checkResult(pEssenceAccess->GetEmptyFileFormat (&pFormat));
		checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));
		
		aafInt32	sampleSize = bitsPerSample;
		checkResult(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
		checkResult(pEssenceAccess->PutFileFormat (pFormat));
		pFormat->Release();
		pFormat = NULL;
		// write out the data
		if(testType == testRawCalls)
		{
			checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceRawAccess, (void **)&pRawEssence));
			checkResult(pRawEssence->WriteRawData(	dataLen,	// Number of Samples
				dataPtr,	// THE Raw data
				sizeof(dataBuff)));// buffer size
			pRawEssence->Release();
			pRawEssence = NULL;
		}
		else if(testType == testStandardCalls)
		{
			checkResult(pEssenceAccess->WriteSamples(	dataLen,	//!!! hardcoded bytes/sample ==1// Number of Samples
				dataPtr,	// THE Raw data
				sizeof(dataBuff)));// buffer size
		}
		else if(testType == testFractionalCalls)
		{
			checkResult(pEssenceAccess->WriteFractionalSample(
				dataLen,			// number of bytes
				dataPtr,			// THE data	
				&bytesWritten));	// !!!checkResult this when it works
		}
		else if(testType == testMultiCalls)
		{
			aafmMultiXfer_t		xfer;
			aafmMultiResult_t	result;
			
			checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
			//!!!		xfer.subTrackNum = _channels[0].physicalOutChan;
			xfer.numSamples = dataLen;	//!!! hardcoded bytes/sample ==1
			xfer.buflen = sizeof(dataBuff);
			xfer.buffer = dataPtr;
			result.bytesXfered = 0;
			
			checkResult(pMultiEssence->WriteMultiSamples(1, &xfer, &result));
			pMultiEssence->Release();
			pMultiEssence = NULL;
		}
		
		// close essence data file
		fclose(pWavFile);
		pWavFile = NULL;
		
		// Finish writing the destination
		checkResult(pEssenceAccess->CompleteWrite());
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

		
		
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();
	
	if (pRawEssence)
		pRawEssence->Release();

	if(pFormat)
		pFormat->Release();

	if(format)
		format->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if (pWavFile)
		fclose(pWavFile);
	
	if(pLocator)
		pLocator->Release();

	if (pMob)
		pMob->Release();

	if (pMasterMob)
		pMasterMob->Release();

	if(pDictionary)
		pDictionary->Release();

	if(pHeader)
		pHeader->Release();

	if (pFile)
	{ // Preserve previous errors...
		HRESULT local_hr = pFile->Save();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceRawAccess*		pRawEssence = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafRational_t				readSampleRate;
	aafUID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char				AAFDataBuf[4096];
	aafUInt32					AAFBytesRead, samplesRead;
	FILE*						pWavFile = NULL;
	unsigned char				WAVDataBuf[4096], *dataPtr;
	size_t						WAVBytesRead;
	aafUInt32					dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
	
	try
	{
		checkResult(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		
		// Here we checkResult on the number of mobs in the file. 
		// Get the number of master mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs(kMasterMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kMasterMob;
		checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];
			
			
			checkResult(pMob->GetMobID (&mobID));
			checkResult(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			AUIDtoString(&mobID, mobIDstr);
			// Make sure we have one slot 
			checkResult(pMob->GetNumSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
			// The essence data is in SlotID 1
			// Get a Master Mob interface
			checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
			
			// Open the Essence Data
			checkResult(pMasterMob->OpenEssence(	1,						// SlotID 1
				NULL,				// mediaCriteria (Don't care)
				kMediaOpenReadOnly,	// Open mode
				kSDKCompressionDisable,// Compress disabled
				&pEssenceAccess));
			
			// Open and read the Wave file (for comparison)
			pWavFile = fopen("Laser.wav", "r");
			checkExpression(pWavFile != NULL, AAFRESULT_TEST_FAILED);
			
			// read in the essence data
			WAVBytesRead = fread(WAVDataBuf, sizeof(unsigned char), sizeof(WAVDataBuf), pWavFile);
			fclose(pWavFile);
			pWavFile = NULL;
			checkResult(loadWAVEHeader(WAVDataBuf,
				&bitsPerSample,
				&numCh,
				&readSampleRate,
				&dataOffset,
				&dataLen));
			if(testType != testRawCalls)
			{
				dataPtr = WAVDataBuf + dataOffset;
			}
			else
			{	
				dataPtr = WAVDataBuf;
				dataLen = WAVBytesRead;
			}
			
			aafUInt32			sampleBits;
			aafInt32			bytesRead;
			
			checkResult(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
			checkResult(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
			fmtTemplate->Release();
			fmtTemplate = NULL;
			
			checkResult(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleBits),
				(aafDataBuffer_t)&sampleBits, &bytesRead));
			pFormat->Release();
			pFormat = NULL;
			checkExpression(sampleBits == bitsPerSample, AAFRESULT_TEST_FAILED);
			
			// Read the Raw Data from the AAF file
			if(testType == testRawCalls)
			{
				checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceRawAccess, (void **)&pRawEssence));
				checkResult(pRawEssence->ReadRawData(	dataLen,		// Number of Samples 
					sizeof(AAFDataBuf),	// Maximum buffer size
					AAFDataBuf,			// Buffer for the data
					&AAFBytesRead,	// Actual number of bytes read
					&samplesRead));		// Actual number of samples read
				pRawEssence->Release();
				pRawEssence = NULL;
			}
			else if(testType == testStandardCalls)
			{
				checkResult(pEssenceAccess->ReadSamples(	dataLen,	//!!! Hardcoded	// Number of Samples 
					sizeof(AAFDataBuf),	// Maximum buffer size
					AAFDataBuf,			// Buffer for the data
					&samplesRead,		// Actual number of samples read
					&AAFBytesRead));	// Actual number of bytes read
			}
			else if(testType == testFractionalCalls)
			{
				checkResult(pEssenceAccess->ReadFractionalSample(dataLen,	// bytes to read
					AAFDataBuf,			// Buffer for the data
					&AAFBytesRead));	// Actual number of bytes read
			}
			else if(testType == testMultiCalls)
			{
				aafmMultiXfer_t		xfer;
				aafmMultiResult_t	result;
				
				checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
				xfer.numSamples = dataLen;	//!!! Hardcoded	// Number of Samples 
				xfer.buflen = sizeof(AAFDataBuf);
				xfer.buffer = AAFDataBuf;
				result.bytesXfered = 0;
				checkResult(pMultiEssence->ReadMultiSamples(1, &xfer, &result));
				samplesRead = result.samplesXfered;
				AAFBytesRead = result.bytesXfered;
				pMultiEssence->Release();
				pMultiEssence = NULL;
			}
			
			// Now compare the data read from the AAF file to the actual WAV file
			if (dataLen != AAFBytesRead)
			{
							cout << "***Wrong number of bytes read ( was "
									 << AAFBytesRead
									 << ", should be "
									 << WAVBytesRead
									 << ")"
									 << endl;
			}
			if (memcmp( dataPtr, AAFDataBuf, dataLen) != 0)
			{
							cout << "*** Data Read is different than the data in the WAV file ***" << endl;
			}
			
			// cleanup the master mob.
			pMasterMob->Release();
			pMasterMob = NULL;
			
			pMob->Release();
			pMob = NULL;
		}

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();

	if (pRawEssence)
		pRawEssence->Release();
	
	if (pFormat)
		pFormat->Release();
	
	if(fmtTemplate)
		fmtTemplate->Release();

	if (pWavFile)
		fclose(pWavFile);

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if (pMasterMob)
		pMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		HRESULT local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	
	return hr;
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
	aafBool				fmtFound = AAFFalse, dataFound = AAFFalse;
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
			fmtFound = AAFTrue;
		} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
		{
			*dataLen = chunkSize / bytesPerFrame;
			// Positioned at beginning of audio data
			*dataOffset = ptr - buf;
			
			dataFound = AAFTrue;
		}
		
		if((ptr-buf) > formSize)
			break;
		if (fmtFound && dataFound)	// Do we have all information yet?
			break;
		scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	}	
	
	return(AAFRESULT_SUCCESS);
}

extern "C" HRESULT CAAFEssenceAccess_test()
{
	CComInitialize comInit;
	AAFRESULT	hr;
	
	aafWChar *	rawData = L"AAFEssenceAccessExtRaw.wav";
	aafWChar *	externalAAF = L"AAFEssenceAccessExtAAF.aaf";
	testDataFile_t	dataFile;
	
    cout << "    Internal Essence:" << endl;
    cout << "        WriteRawData" << endl;
	hr = CreateAAFFile(L"AAFEssenceAccessRaw.aaf", NULL, testRawCalls);
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccessRaw.aaf", testRawCalls);
	}
	
	/**/
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "        WriteSamples" << endl;
		hr = CreateAAFFile(L"AAFEssenceAccess.aaf", NULL, testStandardCalls);
	}
	
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccess.aaf", testStandardCalls);
	}
	/**/
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "        WriteMultiSamples" << endl;
		checkResult(CreateAAFFile(L"AAFEssenceAccessMulti.aaf", NULL, testMultiCalls));
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccessMulti.aaf", testMultiCalls);
	}
	/**/
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "        WriteFractionalSample" << endl;
		hr = CreateAAFFile(L"AAFEssenceAccessFract.aaf", NULL, testFractionalCalls);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccessFract.aaf", testFractionalCalls);
	}
	/**/
	dataFile.dataFilename = rawData;
	dataFile.dataFormat = ContainerFile;
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "    External Essence:" << endl;
        cout << "        WriteSamples (External Raw Essence)" << endl;
		hr = CreateAAFFile(L"AAFEssenceAccessRawRef.aaf", &dataFile, testStandardCalls);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccessRawRef.aaf", testStandardCalls);
	}
	/**/
	dataFile.dataFilename = externalAAF;
	dataFile.dataFormat = ContainerAAF;
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "        WriteSamples (External AAF Essence)" << endl;
		hr = CreateAAFFile(L"AAFEssenceAccessRef.aaf", &dataFile, testStandardCalls);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		hr = ReadAAFFile(L"AAFEssenceAccessRef.aaf", testStandardCalls);
	}
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IEnumAAFEssenceAccess methods have not been tested:" << endl;       
		cout << "     GetSampleFrameSize" << endl; 
		cout << "     GetSampleCount" << endl; 
		cout << "     GetCodecName" << endl; 
		cout << "     GetCodecID" << endl; 
		cout << "     SetEssenceCodecFlavour" << endl; 
		cout << "     GetLargestSampleSize" << endl; 
		cout << "     SetTransformParameters" << endl; 
		cout << "     AddSampleIndexEntry" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}
	
	return(hr);
}
	
