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

AAFRESULT loadWAVEHeader(aafUInt8		*buf,
						 aafUInt16		*bitsPerSample,
						 aafUInt16		*numCh,
						 aafRational_t  *sampleRate,
						 aafUInt32		*dataOffset,
						 aafUInt32		*dataLen);

typedef struct
{
	aafWChar	*dataFilename;
	aafUID_t	dataFormat;
} 

testDataFile_t;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT CreateAAFFile(aafWChar * pFileName, testDataFile_t *dataFile, testType_t testType)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
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
	ProductInfo.productName = L"Export Audio Example";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;

	/* Create a new AAF file */

	check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));
	
	/* Get the AAF Dictionary from the file */
	check(pHeader->GetDictionary(&pDictionary));
	
  /* Lookup class definitions for the objects we want to create. */
  check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pMasterMobDef));
  check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator, &pNetworkLocatorDef));

  /* Lookup any necessary data definitions. */
  check(pDictionary->LookupDataDef(DDEF_Sound, &pSoundDef));

	/* Create a Mastermob */
	
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
									kAAFCodecWAVE,			// codecID
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

  pSoundDef->Release();
  pSoundDef = NULL;
  pNetworkLocatorDef->Release();
  pNetworkLocatorDef = NULL;
  pMasterMobDef->Release();
  pMasterMobDef = NULL;

	pDictionary->Release();
	pDictionary = NULL;
	pHeader->Release();
	pHeader = NULL;
	
	/* Save the AAF file */
	pFile->Save();
	
	/* Close the AAF file */
	pFile->Close();
	pFile->Release();
	pFile = NULL;


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

// application works on all WAVE files, not just laser.wav.
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
		} 
		else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
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


int main()
{
	CAAFInitialize aafInit;
	
	aafWChar *		pwFileName	= L"ExportAudioExample.aaf";
	const char *	pFileName	= "ExportAudioExample.aaf";
	
  // Make sure all of our required plugins have been registered.
  checkFatal(RegisterRequiredPlugins());

  printf("Creating file %s using WriteSamples (Internal Media)...\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName, NULL, testStandardCalls));

	printf("DONE\n\n");

	return(0);
}
