// @com Executable example program by Stuart Cunningham, BBC R&D
// @com This is based upon ComEssenceDataTest.cpp.

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

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

// define uncompressedWAVE_Laser[] which is PCM 8bit mono 11025Hz
// raw PCM samples start at offset 44
#include "EssenceTestData.h"

const aafUInt8 *frame_DV = compressedDV_25_625;

typedef enum { NONE = 0, RawDV, RawUYVY, RawYUY2 } InputFormat;
InputFormat inpFormat = NONE;

// Use libdv API terminology for consistency with internal use of libdv API
// where "PAL" & "NTSC" mean 625/50 and 525/60 respectively
bool formatPAL = true;

const aafInt32 UNC_PAL_FRAME_SIZE = 720*576*2;
const aafInt32 UNC_NTSC_FRAME_SIZE = 720*480*2;
const aafInt32 DV_PAL_FRAME_SIZE = 144000;
const aafInt32 DV_NTSC_FRAME_SIZE = 120000;

bool useLegacyDV = false;

// DV Compression IDs supported (kLegacy_DV is found in AAF files output by some Avid products)
aafUID_t kLegacy_DV = { 0xedb35390, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
aafUID_t kIEC_DV_625_50 = { 0x04010202, 0x0201, 0x0200, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
aafUID_t kIEC_DV_525_60 = { 0x04010202, 0x0201, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };

#define aaf_assert(b, msg) \
	if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

static void LogError(HRESULT errcode, int line, const char *file)
{
	printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a) \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
		LogError(moduleErrorTmp, __LINE__, __FILE__);\
		exit(1); \
	} \
}

#define checkFatal(a) \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
		exit(1);\
}

static void convert(char* cName, size_t length, const wchar_t* name)
{
	aaf_assert((name && *name), "Valid input name");
	aaf_assert(cName != 0, "Valid output buffer");
	aaf_assert(length > 0, "Valid output buffer size");

	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		fprintf(stderr, ": Error : Conversion failed.\n\n");
		exit(1);
	}
}

static bool operator == (const aafUID_t a, const aafUID_t b)
{
	return memcmp(&a, &b, sizeof(a)) == 0;
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ExportPCM";

static char *input_video = NULL;

static aafUID_t container = kAAFContainerDef_RIFFWAVE;

static HRESULT CreateAAFFile(const aafWChar * pFileName)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IAAFLocator					*pLocator = NULL;
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {11025, 1};
	aafRational_t				sampleRate = {11025, 1};
	IAAFClassDef				*pCDMasterMob = NULL;
	IAAFDataDef					*pSoundDef = NULL;
	aafUInt32					samplesWritten, bytesWritten;

	// Delete any previous test file before continuing...
	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);

	aafProductVersion_t ver = {1, 0, 0, 0, kAAFVersionBeta};
	ProductInfo.companyName = companyName;
	ProductInfo.productName = productName;
	ProductInfo.productVersion = &ver;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;		// Set by SDK when saving

	// Create a new AAF file
	check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup class definitions for the objects we want to create. */
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pCDMasterMob));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(kAAFDataDef_Sound, &pSoundDef));

	/* Create a Mastermob */

	// Get a Master MOB Interface
	check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));

	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	if (input_video == NULL)
	{
		check(pMob->SetName(L"Laser"));
	}
	else
	{
		check(pMob->SetName(pFileName));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	// Locator needed for non-embedded essence
	IAAFClassDef *classDef = NULL;
	check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator, &classDef));
	check(classDef->CreateInstance(IID_IAAFLocator, (IUnknown **)&pLocator));
	classDef->Release();
	classDef = NULL;

	if (container == NIL_UID)
	{
		pLocator = NULL;
	}
	else if (container == ContainerAAF)
	{
		check(pLocator->SetPath(L"Laser.aaf"));
		remove("Laser.aaf");
	}
	else if (container == ContainerFile)
	{
		check(pLocator->SetPath(L"Laser.pcm"));
		remove("Laser.pcm");
	}
	else	// RIFFWAVE container
	{
		check(pLocator->SetPath(L"Laser.wav"));
		remove("Laser.wav");
	}

	// Get a pointer to video data for WriteSamples
	unsigned char *dataPtr, buf[4096];
	memcpy(buf, uncompressedWAVE_Laser+44, sizeof(uncompressedWAVE_Laser));
	dataPtr = buf;

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	check(pMasterMob->CreateEssence(1,			// Slot ID within MasterMob
						pSoundDef,				// MediaKind
						kAAFCodecPCM,			// codecID
						editRate,				// edit rate
						sampleRate,				// sample rate
						kAAFCompressionDisable,
						pLocator,				// Locator
						container,			// Container
						&pEssenceAccess));		//

	pEssenceAccess->SetEssenceCodecFlavour( kAAFNilCodecFlavour );

	// Set Format specifiers that describe the essence data
	// Sample rate is already specified in the CreateEssence() call
	aafUInt32 sampleBits = 8;
	aafUInt32 numChannels = 1;

	check(pEssenceAccess->GetEmptyFileFormat(&pFormat));
	check(pFormat->AddFormatSpecifier(kAAFAudioSampleBits, sizeof(sampleBits), (aafUInt8 *)&sampleBits));
	check(pFormat->AddFormatSpecifier(kAAFNumChannels, sizeof(numChannels), (aafUInt8 *)&numChannels));
	check(pEssenceAccess->PutFileFormat(pFormat));

	pFormat->Release();

	/* Write the samples */
	int total_samples = 0;
	if (input_video == NULL)		// using generated uncompressed video?
	{
		for (int i = 0; i < 1793; i++)		// Laser example has 1793 samples
		{
			check(pEssenceAccess->WriteSamples(
						1,					//
						sizeof(buf),		// buffer size
						dataPtr++,			// pointer to video frame
						&samplesWritten,
						&bytesWritten));
			total_samples += samplesWritten;
		}
	}
	printf("Wrote %d samples\n", total_samples);

	/* Set the essence to indicate that you have finished writing the samples */
	check(pEssenceAccess->CompleteWrite());

	pEssenceAccess->Release();
	pEssenceAccess = NULL;

	/* Release COM interfaces */

	pMob->Release();
	pMasterMob->Release();

	pSoundDef->Release();
	pCDMasterMob->Release();

	pDictionary->Release();
	pHeader->Release();

	/* Save the AAF file */
	pFile->Save();

	/* Close the AAF file */
	pFile->Close();
	pFile->Release();

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
			fprintf(stderr, "	Windows		- $PATH\n");
			fprintf(stderr, "	Unix/Linux	- $LD_LIBRARY_PATH\n");
			exit(hr);
		}
	}

	~CAAFInitialize()
	{
		AAFUnload();
	}
};


// Make sure all of our required plugins have been registered.
static HRESULT RegisterRequiredPlugins(void)
{
	IAAFPluginManager	*mgr = NULL;

	// Load the plugin manager
	check(AAFGetPluginManager(&mgr));

	// Attempt load and register all of the plugins
	// in the shared plugin directory.
	check(mgr->RegisterSharedPlugins());

	if (mgr)
		mgr->Release();

	return moduleErrorTmp;
}

void printUsage(const char *progname)
{
	cout << "Usage : " << progname << " [-File|-AAF]" << endl;
	cout << endl;
	cout << "\tWith no arguments creates ExportPCM.aaf containing Laser audio samples" << endl;
	cout << "\tand creates external BWF file Laser.wav" << endl;
	cout << endl;
	cout << "\t-File   use ContainerFile instead of kAAFContainerDef_RIFFWAVE" << endl;
	cout << "\t-AAF    use ContainerAAF instead of kAAFContainerDef_RIFFWAVE" << endl;
	cout << "\t-embed  store embedded essence in ExportPCM.aaf (no external file)" << endl;
	cout << endl;
}

extern int main(int argc, char *argv[])
{
	const aafWChar		*pwFileName	= L"ExportPCM.aaf";

	int i = 1;
	if (argc > 1)
	{
		while (i < argc)
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			{
				printUsage(argv[0]);
				return 0;
			}
			else if (!strcmp(argv[i], "-File"))
			{
				container = ContainerFile;
				i++;
			}
			else if (!strcmp(argv[i], "-AAF"))
			{
				container = ContainerAAF;
				i++;
			}
			else if (!strcmp(argv[i], "-embed"))
			{
				container = NIL_UID;
				i++;
			}
			else
			{
				printUsage(argv[0]);
				return 1;
			}
		}
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	checkFatal(CreateAAFFile(pwFileName));

	return(0);
}
