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
const CLSID CLSID_AAFEssenceAccess = { 0xaed97eb1, 0x2bc8, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };

#endif

const CLSID CLSID_AAFWAVECodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

// This static variables are here so they can be referenced 
// thru out the whole program.

static aafSourceRef_t sourceRef; 

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

static void AUIDtoString(aafUID_t *uid, aafWChar *buf)
{
	wsprintf(buf, L"%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		aDesc = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;

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
	unsigned char				dataBuff[4096];
	size_t						bytesRead;


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
	check(CoCreateInstance( CLSID_AAFMasterMob,
						   NULL, 			  
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMasterMob, 
						   (void **)&pMasterMob));
	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	check(pMob->SetName(L"A Master Mob"));
	
	// Add it to the file 
	check(pHeader->AppendMob(pMob));

	// Get an EssenceAccess Interface
	check(CoCreateInstance(CLSID_AAFEssenceAccess,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFEssenceAccess, 
							(void **)&pEssenceAccess));		
/*
	since the data will be saved in the same file we do not need to make
	this call. Will be used when writing a separate Essence Data file.
	check(pEssenceAccess->SetEssenceDestination(pLocator, kAAFiMedia));
*/
	// now create the Essence data file
	check(pEssenceAccess->Create(	pMasterMob,		// The MasteMob
									1,				// Slot ID
									DDEF_Audio,		// MediaKind
									CodecWave,		// codecID
									editRate,		// edit rate
									sampleRate,		// sample rate
									kSDKCompressionDisable));// Compress disabled

	// open the Essence file to be included in this AAF file("Laser.wav")
	pWavFile = fopen("Laser.wav", "r+b");
	if (pWavFile)
	{
		// read in the essence data
		bytesRead = fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
		// write out the data
		check(pEssenceAccess->WriteRawData(	bytesRead,	// Number of Samples
											dataBuff,	// THE Raw data
											sizeof(dataBuff)));// buffer size

		// close essence data file
		fclose(pWavFile);
	}
	else
	{
		printf("***Failed to open Wave file Laser.wav\n");
	}

	// Close the EssenceData
	check(pEssenceAccess->Close());
	// Release all unnecesary interfaces
	pEssenceAccess->Release();
	pEssenceAccess= NULL;

	pMasterMob->Release();
	pMasterMob = NULL;
	pMob->Release();
	pMob = NULL;

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

	if (pHeader)
		pHeader->Release();

	if (pFile)
		pFile->Release();

	return moduleErrorTmp;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;

	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafUID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char				AAFDataBuf[4096];
	aafUInt32					AAFBytesRead, samplesRead;
	FILE*						pWavFile = NULL;
	unsigned char				WAVDataBuf[4096];
	size_t						WAVBytesRead;

	check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));
	check(pFile->Initialize());
	check(pFile->OpenExistingRead(pFileName, 0));
	check(pFile->GetHeader(&pHeader));

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
			aafWChar	buf[256];

			check(pMob->GetMobID (&mobID));
			check(pMob->GetName (namebuf, sizeof(namebuf)));
			AUIDtoString(&mobID, buf);
			wprintf(L"    MasterMob Name = '%s'\n", namebuf);
			wprintf(L"        (mobID %s)\n", buf);
			// Make sure we have one slot 
			check(pMob->GetNumSlots(&numSlots));
			if (1 == numSlots)
			{
				// The essence data is in SlotID 1
				// Get a Master Mob interface
				check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

				// Get an EssenceAccess Interface
				check(CoCreateInstance(CLSID_AAFEssenceAccess,
										NULL, 
										CLSCTX_INPROC_SERVER, 
										IID_IAAFEssenceAccess, 
										(void **)&pEssenceAccess));		
				// Open the Essence Data
				check(pEssenceAccess->Open(	pMasterMob,				//Master Mob pointer
											1,						// SlotID 1
											NULL,				// mediaCriteria (Don't care)
											kMediaOpenReadOnly,	// Open mode
											kSDKCompressionDisable));// Compress disabled

				// Open and read the Wave file (for comparison)
				pWavFile = fopen("Laser.wav", "r+b");
				if (pWavFile)
				{
					// read in the essence data
					WAVBytesRead = fread(WAVDataBuf, sizeof(unsigned char), sizeof(WAVDataBuf), pWavFile);
					fclose(pWavFile);

					// Read the Raw Data from the AAF file
					check(pEssenceAccess->ReadRawData(	sizeof(AAFDataBuf),	// Number of Samples 
														sizeof(AAFDataBuf),	// Maximum buffer size
														AAFDataBuf,			// Buffer for the data
														&AAFBytesRead,		// Actual number of bytes read
														&samplesRead));		// Actual number of samples read

					// Now compare the data read from the AAF file to the actual WAV file
					if (WAVBytesRead != AAFBytesRead)
					{
						printf("***Wrong number of bytes read ( was %ld , should be %ld)\n",
							AAFBytesRead, WAVBytesRead);
					}
					if (memcmp( WAVDataBuf, AAFDataBuf, WAVBytesRead) != 0)
					{
						printf("*** Data Read is different than the data in the WAV file ***\n");
					}
				}
				else
				{
					printf("***Failed to open Wave file Laser.wav for comparison\n");
				}

				// Close the EssenceData
				check(pEssenceAccess->Close());

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

	wprintf(L"--------\n");

cleanup:
	// Cleanup and return

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

const aafUID_t CLSID_AAFDefaultCodec = { 0xDC089C31, 0x9527, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const CLSID CLSID_AAFEssencePlugin = { 0xAF98DE41, 0x952D, 0x11D2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

main()
{
	CComInitialize comInit;
	aafWChar * pwFileName = L"EssenceTest.aaf";
	const char * pFileName = "EssenceTest .aaf";

	IAAFEssencePlugin *codecManager = NULL;
//	aafInt32		numCodecs;

	// This call to CoCreateInstance is here to allow the DLL to be quickly
	// loaded for debug.  It now serves no other function.
	CoCreateInstance(CLSID_AAFEssencePlugin,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFEssencePlugin, 
               (void **)&codecManager);
//	codecManager->NumCodecsMatching (DDEF_Audio, kAAFRev1, &numCodecs);


	printf("***Creating file %s\n", pFileName);
	checkFatal(CreateAAFFile(pwFileName));
	printf("***Re-opening file %s\n", pFileName);
	ReadAAFFile(pwFileName);

	printf("Done\n");


	return(0);
}




