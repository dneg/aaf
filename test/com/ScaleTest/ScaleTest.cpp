// @com Executable example program by Stuart Cunningham, BBC R&D
// @com This is based upon ComEssenceDataTest.cpp.

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

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <unistd.h>				// stat() under unix
#endif

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
#include "AAFFileKinds.h"
#include "AAF.h"
#include "AAFStoredObjectIDs.h"
#include "CAAFBuiltinDefs.h"

// Guess the toolkit version (found by experiment)
#if defined(__IAAFImportDescriptor_INTERFACE_DEFINED__)
	// V1.1 or greater toolkit
#else
#define AAF_TOOLKIT_V1_0
#endif

#ifdef AAF_TOOLKIT_V1_0
// using v1.0.x toolkit
const aafUID_t kAAFCompression_IEC_DV_525_60 = { 0x04010202, 0x0201, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 
0x01, 0x01, 0x01 } };
const aafUID_t *filekind_4K = &aafFileKindAaf4KBinary;
#else
// using v1.0.x toolkit
#include "AAFCompressionDefs.h"
const aafUID_t *filekind_4K = &kAAFFileKind_Aaf4KBinary;
#endif


// Provide a sample compressed DV frame
#include "../ComModTestAAF/ModuleTests/EssenceTestData.h"


#define SIZE_2GB AAFCONSTINT64(2147483648)

const aafInt32 UNC_PAL_FRAME_SIZE = 720*576*2;
const aafInt32 UNC_NTSC_FRAME_SIZE = 720*480*2;
const aafInt32 DV_PAL_FRAME_SIZE = 144000;
const aafInt32 DV_NTSC_FRAME_SIZE = 120000;

bool useLegacyDV = false;		// LegacyDV implies 512 sector files (limited to 2GB)
bool verbose = true;

#define aaf_assert(b, msg) \
	if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

static void LogError(HRESULT errcode, int line, char *file)
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

bool operator ==( const aafUID_t uid1, const aafUID_t uid2 )
{
    bool    are_equal = true;
    int     i = 0;

    for( i=0; i<8; i++ )
    {
        if( uid1.Data4[i] != uid2.Data4[i] )
        {
            are_equal = false;
            break;
        }
    }

    if( are_equal == true )
    {
        if( uid1.Data1 != uid2.Data1  ||
            uid1.Data2 != uid2.Data2  ||
            uid1.Data3 != uid2.Data3 )
        {
            are_equal = false;
        }
    }

    return are_equal;
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

static aafLength_t getFilesize(const char *file)
{
	// For Large File Support (>2GB) we must use the awkward stat() and
	// _stati64() calls below.  C++ standard library ifstream seekg and tellg
	// aren't 64bit capable using GCC 3.3 and MSVC 7 (GCC 3.4 fixes this).
    int rc;
#ifdef _MSC_VER
	struct _stati64 statbuf;
	rc = _stati64(file, &statbuf);
#else
	struct stat statbuf;
	rc = stat(file, &statbuf);
#endif
	if ( rc != 0 )
	{
	  perror("stat:");
	  return -1;
	}

	return statbuf.st_size;
}

// Generate a video buffer containing uncompressed UYVY video representing
// the familiar colour bars test signal (or YUY2 video if specified).
static void create_colour_bars(unsigned char *video_buffer, bool convert_to_YUY2)
{
	int		i,j,b;
	int		UYVY_table[][5] = {
				{ 52, 0x80,0xEB,0x80,0xEB },	// white
				{ 140, 0x10,0xD2,0x92,0xD2 },	// yellow
				{ 228, 0xA5,0xA9,0x10,0xA9 },	// cyan
				{ 316, 0x35,0x90,0x22,0x90 },	// green
				{ 404, 0xCA,0x6A,0xDD,0x6A },	// magenta
				{ 492, 0x5A,0x51,0xF0,0x51 },	// red
				{ 580, 0xf0,0x29,0x6d,0x29 },	// blue
				{ 668, 0x80,0x10,0x80,0x10 },	// black
				{ 720, 0x80,0xEB,0x80,0xEB },	// white
			};

	for (j = 0; j < 576; j++)
	{
		for (i = 0; i < 720*2; i+=4)
		{
			for (b = 0; b < 9; b++)
			{
				if (i < UYVY_table[b][0] * 2)
				{
					video_buffer[j*720*2 + i + 0] = UYVY_table[b][1];
					video_buffer[j*720*2 + i + 1] = UYVY_table[b][2];
					video_buffer[j*720*2 + i + 2] = UYVY_table[b][3];
					video_buffer[j*720*2 + i + 3] = UYVY_table[b][4];
					break;
				}
			}
		}
	}

	if (convert_to_YUY2)
	{
		// Convert from UYVY -> YUY2
		for (i = 0; i < 720 * 576 * 2; i+=2)
		{
			unsigned char tmp = video_buffer[i+1];
			video_buffer[i+1] = video_buffer[i];
			video_buffer[i] = tmp;
		}
	}
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static const   aafMobID_t  TEST_MobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};
static const   aafMobID_t  TEST_SourceMobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc402, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafProductVersion_t TestVersion = { 1, 1, 0, 0, kAAFVersionUnknown };
static aafProductIdentification_t TestProductID;

static HRESULT CreateAAFFileEssenceData(aafWChar *pFileName, int frame_limit,
									aafLength_t *p_bytes)
{
	TestProductID.companyName = L"AAF Association";
	TestProductID.productName = L"ScaleTest";
	TestProductID.productVersionString = NULL;
	TestProductID.productID = UnitTestProductID;
	TestProductID.platform = NULL;
	TestProductID.productVersion = &TestVersion;

	HRESULT			hr = S_OK;

	try
	{
		// Delete any previous test file before continuing...
		char cFileName[FILENAME_MAX];
		convert(cFileName, sizeof(cFileName), pFileName);
		remove(cFileName);

		// Open new file
		IAAFFile		*pFile = NULL;
		TestProductID.productVersionString = L"ScaleTest EssenceData";
		check( AAFFileOpenNewModifyEx(
						pFileName,
						filekind_4K,
						0,
						&TestProductID,
						&pFile) );

		// Get the header & dictionary
		IAAFHeader		*pHeader = NULL;
		IAAFDictionary	*pDictionary = NULL;
		check(pFile->GetHeader(&pHeader));
		check(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs(pDictionary);

		// Create a MasterMob
		IAAFMob			*pMob = NULL;
		check(defs.cdMasterMob()->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
		check(pMob->SetMobID(TEST_MobID));
		check(pMob->SetName(L"CreateAAFFile - MasterMob"));
		check(pHeader->AddMob(pMob));
		pMob->Release();

		// Create a SourceMob 
		IAAFSourceMob			*pSourceMob = NULL;
		check(defs.cdSourceMob()->CreateInstance(IID_IAAFSourceMob, (IUnknown **)&pSourceMob));
		check(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		check(pMob->SetMobID(TEST_SourceMobID));
		check(pMob->SetName(L"CreateAAFFile - SourceMob"));
		check(pHeader->AddMob(pMob));

		// Add an EssenceDescriptor to the SourceMob
		IAAFEssenceDescriptor		*edesc = NULL;
		IAAFCDCIDescriptor			*pCDCIDesc = NULL;
		IAAFDigitalImageDescriptor	*pDIDesc = NULL;
		check(defs.cdCDCIDescriptor()->CreateInstance(IID_IAAFEssenceDescriptor, (IUnknown **)&edesc));
		check(edesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc));
		check(edesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDIDesc));
		check(pDIDesc->SetCompression(kAAFCompressionDef_IEC_DV_525_60));
		check(pDIDesc->SetStoredView(576/2, 720));
		check(pDIDesc->SetFrameLayout(kAAFSeparateFields));
		check(pCDCIDesc->SetHorizontalSubsampling(2));
		check(pCDCIDesc->SetVerticalSubsampling(2));
		check(pSourceMob->SetEssenceDescriptor(edesc));
		pCDCIDesc->Release();
		edesc->Release();

		// Add an EssenceData object containing DV frames
		IAAFEssenceData			*pEssenceData = NULL;
		aafUInt32				bytesWritten = 0;
		check(defs.cdEssenceData()->CreateInstance(
					IID_IAAFEssenceData, (IUnknown **)&pEssenceData));
		check(pEssenceData->SetFileMob(pSourceMob));
		check(pHeader->AddEssenceData(pEssenceData));

		*p_bytes = 0;
		for (int i = 0; i < frame_limit; i++)
		{
			check(pEssenceData->Write(
									sizeof(compressedDV_25_625),
									(aafUInt8*)compressedDV_25_625,
									&bytesWritten));
			*p_bytes += bytesWritten;
			if (verbose && (i+1) % 25 == 0)
			{
				printf("%6d frames %6.2f%%\r", i+1, (i+1) * 100.0 / frame_limit);
				fflush(stdout);
			}
		}
		if (verbose)
			printf("%6d frames %6.2f%%  bytes=%"AAFFMT64"d\n",
					frame_limit, frame_limit * 100.0 / frame_limit, *p_bytes);
		pEssenceData->Release();

		pSourceMob->Release();
		pMob->Release();

		pDictionary->Release();
		pHeader->Release();

		// Save & close the file
		check(pFile->Save());
		check(pFile->Close());
		check(pFile->Release());

		aafLength_t filesize = getFilesize(cFileName);
		printf("Wrote %s (EssenceData API) filesize=%"AAFFMT64"d (%s)\n",
					cFileName, filesize, (filesize > SIZE_2GB) ? "> 2GB" : "<= 2GB");
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** CreateAAFFileEssenceData: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

static HRESULT ReadAAFFileEssenceData(aafWChar *pFileName, int frame_limit, aafLength_t bytes)
{
	HRESULT			hr = S_OK;

	try
	{
		// Delete any previous test file before continuing...
		char cFileName[FILENAME_MAX];
		convert(cFileName, sizeof(cFileName), pFileName);

		// Open new file
		IAAFFile		*pFile = NULL;

		hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
		if (hr == S_OK)
		{
			printf("FileOpenExistingRead for %s succeeded\n", cFileName);
		}
		else
		{
			printf("FileOpenExistingRead for %s failed hr=0x%08x\n", cFileName, hr);
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ReadAAFFileEssenceData: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

#ifndef AAF_TOOLKIT_V1_0
static HRESULT CreateAAFFileCodec(aafWChar * pFileName, bool comp_enable,
								aafUID_t codec, aafUID_t container,
								int frame_limit, aafLength_t *p_bytes)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				pictureEditRate = {25, 1}, pictureSampleRate = {25, 1};
	aafRational_t				soundEditRate = {48000, 1}, soundSampleRate = {48000, 1};
	aafRational_t				editRate, sampleRate;
	IAAFClassDef				*pCDMasterMob = NULL;
	IAAFDataDef					*pPictureDef = NULL, *pSoundDef;
	IAAFDataDef					*media_kind = NULL;
	aafUInt32					samplesWritten, bytesWritten;
	aafInt32					frameSize = 0;

	// Delete any previous test file before continuing...
	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);

	cout << "Creating file " << cFileName << " using WriteSamples with " <<
		(comp_enable ? "CompressionEnable" : "CompressionDisable") << endl;

	aafProductVersion_t ver = {1, 0, 0, 0, kAAFVersionBeta};
	ProductInfo.companyName = L"none";
	ProductInfo.productName = L"AAF SDK";
	ProductInfo.productVersion = &ver;
	ProductInfo.productVersionString = L"1.0.0.0 Beta";
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;		// Set by SDK when saving

	// Large sectors for new files, small sectors for legacy files
	const aafUID_t *fileKind = useLegacyDV ? &kAAFFileKind_Aaf512Binary : &kAAFFileKind_Aaf4KBinary;

	// Create a new AAF file
	check(AAFFileOpenNewModifyEx(pFileName, fileKind, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup class definitions for the objects we want to create. */
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pCDMasterMob));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));
	check(pDictionary->LookupDataDef(kAAFDataDef_Sound, &pSoundDef));

	/* Create a Mastermob */

	// Get a Master MOB Interface
	check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));

	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	if (comp_enable)
	{
		check(pMob->SetName(L"colour_bars"));
	}
	else
	{
		check(pMob->SetName(L"flower"));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	// Prepare parameters based on codec selected
	if (codec == kAAFCodecDef_PCM)
	{
		media_kind = pSoundDef;
		editRate = soundEditRate;
		sampleRate = soundSampleRate;
	}
	else if (codec == kAAFCodecDef_CDCI)
	{
		media_kind = pPictureDef;
		editRate = pictureEditRate;
		sampleRate = pictureSampleRate;
	}

	// Create a frame of colour bars
	// experiment shows 4 bytes per pixel needed to avoid SMALLBUF using WriteSamples
	// with kAAFMixedFields and 720x576
	// 2 bytes per pixel work fine with kAAFSeparateFields and 720x288
	unsigned char video_buf[UNC_PAL_FRAME_SIZE];
	if (comp_enable)
	{
		create_colour_bars(video_buf, true);
	}

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	check(pMasterMob->CreateEssence(1,			// Slot ID within MasterMob
						media_kind,				// MediaKind
						codec,					// E.g. CDCI, WAVE, PCM
						editRate,				// edit rate
						sampleRate,				// sample rate
						comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable,
						NULL,					// No Locator used
						container,				// Essence embedded or external?
						&pEssenceAccess));		//

	// Set the codec flavour for desired video format
	if (codec == kAAFCodecDef_CDCI)
	{
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_IEC_DV_625_50 );

		frameSize = (comp_enable ? UNC_PAL_FRAME_SIZE : DV_PAL_FRAME_SIZE);
	}

	// For fun, print the name of the selected codec flavour
	aafWChar codec_name[128] = L"";
	check(pEssenceAccess->GetCodecName(sizeof(codec_name), codec_name));
	printf("  using codec flavour \"%ls\"\n", codec_name);

	// Get a pointer to video data for WriteSamples
	unsigned char *dataPtr;
	dataPtr = video_buf;

	// Write the video samples
	int total_samples = 0;

	if (comp_enable)				// using generated uncompressed video?
	{
		for (int i = 0; i < frame_limit; i++)
		{
			check(pEssenceAccess->WriteSamples(	1,					//
												sizeof(video_buf),	// buffer size
												dataPtr,			// pointer to video frame
												&samplesWritten,
												&bytesWritten));
			total_samples += samplesWritten;
		}
	}
	else // using pre-compressed DV frames
	{
		memcpy(video_buf, compressedDV_25_625, sizeof(compressedDV_25_625));

		for (int i = 0; i < frame_limit; i++)
		{
			check(pEssenceAccess->WriteSamples(
									1,					// number of samples
									frameSize,			// buffer size
									dataPtr,			// samples buffer
									&samplesWritten,
									&bytesWritten));
			total_samples += samplesWritten;
			if (verbose && total_samples % 25 == 0)
			{
				printf("%6d frames %6.2f%%\r", total_samples, total_samples * 100.0 / frame_limit);
				fflush(stdout);
			}
		}
	}
	if (verbose)
		printf("%6d frames %6.2f%%\n", total_samples, total_samples * 100.0 / frame_limit);

	/* Set the essence to indicate that you have finished writing the samples */
	check(pEssenceAccess->CompleteWrite());

	pEssenceAccess->Release();

	pMob->Release();
	pMasterMob->Release();

	pPictureDef->Release();
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
#endif

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
#ifndef AAF_TOOLKIT_V1_0
	cout << "Usage : " << progname << " [-q] [-c] [-n frames]" << endl;
	cout << endl;
	cout << "\tWith no arguments creates TestDV.aaf containing 30 minutes of DV frames" << endl;
	cout << endl;
	cout << "\t-n frames   number of DV frames to write" << endl;
#else
	cout << "\t-c          Test the codecs only (not EssenceData API)" << endl;
	cout << "\t-compress   Test using uncompressed video passing through codec's compressor" << endl;
#endif
	cout << "\t-q          Quiet (no status messages)" << endl;
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar *		pwFileName	= L"TestDV.aaf";
	bool			compressionEnable = false;		// use compressed frames by default
	bool			testEssenceData = true;
	bool			testCodecs = false;

	//int				frame_limit = 25 * 60 * 30;		// 30 minutes DV is approx. 6GB
	//int				frame_limit = 14897;		// just under 2GB by 77824 bytes
	int				frame_limit = 14898;		// just over 2GB by 69632 bytes

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
			else if (!strcmp(argv[i], "-legacyDV"))
			{
				useLegacyDV = true;
				i++;
			}
			else if (!strcmp(argv[i], "-n"))
			{
				frame_limit = strtol(argv[i+1], NULL, 10);
				if (frame_limit < 1)
				{
					printUsage(argv[0]);
					return 1;
				}
				i+=2;
			}
			else if (!strcmp(argv[i], "-q"))
			{
				verbose = false;
				i++;
			}
			else if (!strcmp(argv[i], "-c"))
			{
				testEssenceData = false;
				testCodecs = true;
				i++;
			}
#ifndef AAF_TOOLKIT_V1_0
			else if (!strcmp(argv[i], "-compress"))
			{
				compressionEnable = true;
				i++;
			}
#endif
			else
			{
				break;
			}
		}
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	if (testEssenceData)
	{
		aafLength_t bytes = 0;
		checkFatal(CreateAAFFileEssenceData(pwFileName, frame_limit, &bytes));

		checkFatal(ReadAAFFileEssenceData(pwFileName, frame_limit, bytes));
	}

#ifndef AAF_TOOLKIT_V1_0
	// Codec tests are not run under v1.0 toolkit due to 2GB limits in codecs
	if (testCodecs)
	{
		// 2005-03-21
		// Codecs that should support > 2GB files
		//	CDCI codec
		//		internal ContainerAAF
		//		external ContainerAAF, ContainerFile
		//	PCM codec
		//		internal ContainerAAF
		//		external ContainerAAF, ContainerFile
		//		(note internal/external ContainerRIFFWAVE must be 2GB limited)

		aafLength_t bytes = 0;

		checkFatal(CreateAAFFileCodec(pwFileName, compressionEnable, kAAFCodecCDCI, ContainerAAF,
									frame_limit, &bytes));
		checkFatal(ReadAAFFileEssenceData(pwFileName, frame_limit, bytes));

		checkFatal(CreateAAFFileCodec(pwFileName, compressionEnable, kAAFCodecPCM, ContainerAAF,
									frame_limit, &bytes));
		checkFatal(ReadAAFFileEssenceData(pwFileName, frame_limit, bytes));

		//checkFatal(ReadAAFFileCodec(pwFileName, frame_limit));
	}
#endif
	return(0);
}
