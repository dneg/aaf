// @com Executable example program by Stuart Cunningham, BBC R&D

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
#include "AAFCompressionDefs.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"


typedef enum {
	formatYUV422,
	formatYUV420,
	formatYUV411
} videoFormat_t;

// Use libdv API terminology for consistency with internal use of libdv API
// where "PAL" & "NTSC" mean 625/50 and 525/60 respectively
const aafInt32 comprSizePALDV25 = 144000;
const aafInt32 comprSizePALDV50 = 288000;
const aafInt32 comprSizeNTSCDV25 = 120000;
const aafInt32 comprSizeNTSCDV50 = 240000;

const aafRational_t sampleRate25 = {25, 1};
const aafRational_t sampleRate30 = {30000, 1001};
const aafRational_t sampleRate50 = {50, 1};
const aafRational_t sampleRate60 = {60000, 1001};

// Default output is IEC_DV_625_50 DV 25Mbps wrapped in AAF
aafRational_t sample_rate = sampleRate25;
aafUID_t flavour = kAAFCodecFlavour_IEC_DV_625_50;
aafInt32 comprSize = comprSizePALDV25;
aafInt32 frameWidth = 720;
aafInt32 frameHeight = 576;
bool useLegacyDV = false;
videoFormat_t vidFmt = formatYUV420;
bool formatMXF = false;

aafUID_t kAAFOpDef_Atom = { 0x0d010201, 0x1000, 0x0000, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

inline bool operator == (const aafRational_t a, const aafRational_t b)
{
	return memcmp(&a, &b, sizeof(a)) == 0;
}

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

// Generate a video buffer containing uncompressed YUV420, YUV411 or YUV422
// with a representation of the colour bars test pattern.
static void create_colour_bars(unsigned char *video_buffer, int width, int height, videoFormat_t format)
{
	int		i,j,b;
	int		UYVY_table[9][4] = {
				{ 52,  0xEB,0x80,0x80 },	// white
				{ 140, 0xD2,0x10,0x92 },	// yellow
				{ 228, 0xA9,0xA5,0x10 },	// cyan
				{ 316, 0x90,0x35,0x22 },	// green
				{ 404, 0x6A,0xCA,0xDD },	// magenta
				{ 492, 0x51,0x5A,0xF0 },	// red
				{ 580, 0x29,0xf0,0x6d },	// blue
				{ 668, 0x10,0x80,0x80 },	// black
				{ 720, 0xEB,0x80,0x80 },	// white
			};

	unsigned char *y, *u, *v;

	y = video_buffer;
	u = y + width*height;
	if (format == formatYUV422)
		v = u + width*height/2;
	else
		v = u + width*height/4;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			for (b = 0; b < 9; b++)
			{
				if (i < (UYVY_table[b][0] * (width / 720.0)))
				{
					*y++ = UYVY_table[b][1];

					if (format == formatYUV411 && i % 4 != 0)	// horiz subsample by 4
						break;
					if (format == formatYUV420 && j % 2 == 1)	// vert subsample by 2
						break;
					if (i % 2 == 1)								// horiz subsample by 2
						break;

					*u++ = UYVY_table[b][2];
					*v++ = UYVY_table[b][3];
					break;
				}
			}
		}
	}
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ExportDV";

static char *input_video = NULL;
static FILE *input;

static HRESULT CreateAAFFile(aafWChar * pFileName, bool comp_enable)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFHeader2*				pHeader2 = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	IAAFClassDef				*pCDMasterMob = NULL;
	IAAFDataDef					*pPictureDef = NULL;
	aafUInt32					samplesWritten, bytesWritten;

	// Delete any previous test file before continuing...
	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);

	cout << "Creating file " << cFileName << " using WriteSamples with " <<
		(comp_enable ? "CompressionEnable" : "CompressionDisable") << endl;

	aafProductVersion_t ver = {1, 0, 0, 0, kAAFVersionBeta};
	ProductInfo.companyName = companyName;
	ProductInfo.productName = productName;
	ProductInfo.productVersion = &ver;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;		// Set by SDK when saving


	const aafUID_t *fileKind = 0;
	if (formatMXF) {
		fileKind = &kAAFFileKind_AafKlvBinary;
	} else {
		// Large sectors for new files, small sectors for legacy files
		if (useLegacyDV) {
			fileKind = &kAAFFileKind_Aaf512Binary;
		} else {
			fileKind = &kAAFFileKind_Aaf4KBinary;
		}
	}

	// Create a new AAF file
	check(AAFFileOpenNewModifyEx(pFileName, fileKind, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Set the operational pattern
	check(pHeader->QueryInterface(IID_IAAFHeader2, (void **)&pHeader2));
	check(pHeader2->SetOperationalPattern(kAAFOpDef_Atom));

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup class definitions for the objects we want to create. */
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pCDMasterMob));

	/* Lookup any necessary data definitions. */
	if (useLegacyDV) {
		check(pDictionary->LookupDataDef(kAAFDataDef_LegacyPicture, &pPictureDef));
	} else {
		check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));
	}

	/* Create a Mastermob */

	// Get a Master MOB Interface
	check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));

	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	if (input_video == NULL)
	{
		check(pMob->SetName(L"colour_bars"));
	}
	else
	{
		check(pMob->SetName(pFileName));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	// Create a frame of colour bars
	aafInt32 frameSize = frameWidth * frameHeight * 3/2;		// YUV411 or YUV420
	if (vidFmt == formatYUV422)
		frameSize = frameWidth * frameHeight * 2;
	unsigned char *video_buf = (unsigned char *)malloc(frameSize);
	if (input_video == NULL)
	{
		create_colour_bars(video_buf, frameWidth, frameHeight, vidFmt);
	}

	// Get a pointer to video data for WriteSamples
	unsigned char *dataPtr;
	dataPtr = video_buf;

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	AAFRESULT res = pMasterMob->CreateEssence(
						1,						// Slot ID within MasterMob
						pPictureDef,			// MediaKind
						kAAFCodecCDCI,			// codecID
						sample_rate,			// edit rate
						sample_rate,			// sample rate
						comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable,
						NULL,					// No Locator used
						ContainerAAF,			// Essence embedded in AAF file
						&pEssenceAccess);		// object returned
	if (res == AAFRESULT_INVALID_OP_CODEC) {
		printf("Error AAFRESULT_INVALID_OP_CODEC: Plugin codec not compiled with compression support\n");
		exit(1);
	}

	// Set the codec flavour for desired video format
	check(pEssenceAccess->SetEssenceCodecFlavour(flavour));

	// For fun, print the name of the selected codec flavour
	// and use it to add to the Mob Name property.
	aafWChar codec_name[128] = L"", mob_name[256] = L"";
	check(pEssenceAccess->GetCodecName(sizeof(codec_name), codec_name));
	check(pMob->GetName(mob_name, 256));
	wcscat(mob_name, L" - ");
	wcscat(mob_name, codec_name);
	check(pMob->SetName(mob_name));
	printf("  using codec flavour \"%ls\"\n", codec_name);

	// Write the video samples
	int total_samples = 0;
	aafInt64 total_bytes_written = 0;

	// If compression is enabled, pass uncompressed data, otherwise pass pre-compressed data
	aafInt32 dataSize = comp_enable ? frameSize : comprSize;

	if (input_video == NULL)
	{
		// use generated uncompressed video
		for (int i = 0; i < 10; i++)	// 10 frames will do
		{
			check(pEssenceAccess->WriteSamples(	1,					// nSamples
												dataSize,			// buffer size
												dataPtr,			// pointer to video frame
												&samplesWritten,
												&bytesWritten));
			total_samples += samplesWritten;
			total_bytes_written += bytesWritten;
		}
	}
	else
	{
		while (1)
		{
			// Read video, either uncompressed or compressed, from file
			if ( fread(video_buf, dataSize, 1, input) != 1 )
			{
				if (feof(input))
					break;
				perror(input_video);
				fprintf(stderr, "Probably incomplete last frame in input file\n");
				break;
			}

			check(pEssenceAccess->WriteSamples(
									1,					// number of samples
									dataSize,			// buffer size
									dataPtr,			// samples buffer
									&samplesWritten,
									&bytesWritten));
			total_samples += samplesWritten;
			total_bytes_written += bytesWritten;
		}
	}
	printf("Wrote %d samples (size of essence=%lld)\n", total_samples, total_bytes_written);

	/* Set the essence to indicate that you have finished writing the samples */
	check(pEssenceAccess->CompleteWrite());
	check(pHeader2->UpdateEssenceContainers());

	pEssenceAccess->Release();

	pMob->Release();
	pMasterMob->Release();

	pPictureDef->Release();
	pCDMasterMob->Release();

	pDictionary->Release();
	pHeader->Release();
	pHeader2->Release();

	/* Save the AAF file */
	pFile->Save();

	/* Close the AAF file */
	pFile->Close();
	pFile->Release();

	// cleanup local data
	free(video_buf);

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
	cout << "Usage : " << progname << " [options] [input_filename]" << endl;
	cout << endl;
	cout << "  With no arguments, creates ExportDV.aaf containing 10 DV frames" << endl;
	cout << "  using IEC_DV_625_50 flavour." << endl;
	cout << endl;
	cout << "  -mxf           store file as KLV encoded file (OP Atom) rather than AAF" << endl;
	cout << "  -o file        write output to specified file instead of ExportDV.aaf" << endl;
	cout << "  -raw file      wrap, don't compress, already compressed input file" << endl;
	cout << "                 (use along with -f <flavour>)" << endl;
	cout << "  -f <flavour>   Use the specified codec flavour for wrapping or encoding:" << endl;
	cout << "      LegacyDV_625_50            - DV 25Mbps 4:1:1 with 6 ext legacy props" << endl;
	cout << "      LegacyDV_525_60            - DV 25Mbps 4:1:1 with 6 ext legacy props" << endl;
	cout << "      IEC_DV_625_50              - DV 25Mbps 4:2:0 IEC 61834 RP224 label" << endl;
	cout << "      IEC_DV_525_60              - DV 25Mbps 4:1:1 IEC 61834 RP224 label" << endl;
	cout << "      DV_Based_25Mbps_625_50     - DV 25Mbps 4:1:1 SMPTE 314M RP244 label" << endl;
	cout << "      DV_Based_25Mbps_525_60     - DV 25Mbps 4:1:1 SMPTE 314M RP244 label" << endl;
	cout << "      DV_Based_50Mbps_625_50     - DV 50Mbps 4:2:2 SMPTE 314M RP244 label" << endl;
	cout << "      DV_Based_50Mbps_525_60     - DV 50Mbps 4:2:2 SMPTE 314M RP244 label" << endl;
	cout << "      DVbased_1080x50I_100Mbps   - DV 100Mbps 4:2:2 SMPTE 370M RP244 label" << endl;
	cout << "      DVbased_1080x5994I_100Mbps - DV 100Mbps 4:2:2 SMPTE 370M RP244 label" << endl;
	cout << "      DVbased_720x50P_100Mbps    - DV 100Mbps 4:2:2 SMPTE 370M RP244 label" << endl;
	cout << "      DVbased_720x5994P_100Mbps  - DV 100Mbps 4:2:2 SMPTE 370M RP244 label" << endl;
	cout << "      SMPTE_D10_625x50I_50Mbps   - IMX 50 4:2:2 SMPTE D-10" << endl;
	cout << "      SMPTE_D10_525x5994I_50Mbps - IMX 50 4:2:2 SMPTE D-10" << endl;
	cout << "      SMPTE_D10_625x50I_40Mbps   - IMX 40 4:2:2 SMPTE D-10" << endl;
	cout << "      SMPTE_D10_525x5994I_40Mbps - IMX 40 4:2:2 SMPTE D-10" << endl;
	cout << "      SMPTE_D10_625x50I_30Mbps   - IMX 30 4:2:2 SMPTE D-10" << endl;
	cout << "      SMPTE_D10_525x5994I_30Mbps - IMX 30 4:2:2 SMPTE D-10" << endl;
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar 		pwFileName[FILENAME_MAX] = L"ExportDV.aaf";
	bool			compressionEnable = true;		// convert from uncomp to DV by default

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
			else if (!strcmp(argv[i], "-o"))
			{
				if (i >= argc-1)		// -o requires filename arg
				{
					printUsage(argv[0]);
					return 1;
				}
				mbstowcs(pwFileName, argv[i+1], sizeof(pwFileName));
				i += 2;
			}
			else if (!strcmp(argv[i], "-f"))
			{
				if (i >= argc-1)		// -f requires flavour arg
				{
					printUsage(argv[0]);
					return 1;
				}
				const char *fstr = argv[i+1];
				// 625/50 flavours
				if (!strcmp(fstr, "LegacyDV_625_50")) {
					flavour = kAAFCodecFlavour_LegacyDV_625_50;
					useLegacyDV = true;
					comprSize = comprSizePALDV25;
					vidFmt = formatYUV420;
				}
				else if (!strcmp(fstr, "IEC_DV_625_50")) {
					flavour = kAAFCodecFlavour_IEC_DV_625_50;
					comprSize = comprSizePALDV25;
					vidFmt = formatYUV411;
				}
				else if (!strcmp(fstr, "DV_Based_25Mbps_625_50")) {
					flavour = kAAFCodecFlavour_DV_Based_25Mbps_625_50;
					comprSize = comprSizePALDV25;
					vidFmt = formatYUV411;
				}
				else if (!strcmp(fstr, "DV_Based_50Mbps_625_50")) {
					flavour = kAAFCodecFlavour_DV_Based_50Mbps_625_50;
					comprSize = comprSizePALDV50;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_625x50I_50Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_625x50I_50Mbps;
					comprSize = 250000;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_625x50I_40Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_625x50I_40Mbps;
					comprSize = 200000;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_625x50I_30Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_625x50I_30Mbps;
					comprSize = 150000;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "DVbased_1080x50I_100Mbps")) {
					flavour = kAAFCodecFlavour_DVbased_1080x50I_100Mbps;
					comprSize = 576000;
					vidFmt = formatYUV422;
					frameWidth = 1440;
					frameHeight = 1080;
				}
				else if (!strcmp(fstr, "DVbased_720x50P_100Mbps")) {
					flavour = kAAFCodecFlavour_DVbased_720x50P_100Mbps;
					sample_rate = sampleRate50;
					comprSize = 288000;
					vidFmt = formatYUV422;
					frameWidth = 960;
					frameHeight = 720;
				}
				// 525/60 flavours
				else if (!strcmp(fstr, "LegacyDV_525_60")) {
					flavour = kAAFCodecFlavour_LegacyDV_525_60;
					useLegacyDV = true;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = comprSizeNTSCDV25;
					vidFmt = formatYUV411;
				}
				else if (!strcmp(fstr, "IEC_DV_525_60")) {
					flavour = kAAFCodecFlavour_IEC_DV_525_60;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = comprSizeNTSCDV25;
					vidFmt = formatYUV411;
				}
				else if (!strcmp(fstr, "DV_Based_25Mbps_525_60")) {
					flavour = kAAFCodecFlavour_DV_Based_25Mbps_525_60;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = comprSizeNTSCDV25;
					vidFmt = formatYUV411;
				}
				else if (!strcmp(fstr, "DV_Based_50Mbps_525_60")) {
					flavour = kAAFCodecFlavour_DV_Based_50Mbps_525_60;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = comprSizeNTSCDV50;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_525x5994I_50Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_525x5994I_50Mbps;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = 208541;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_525x5994I_40Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_525x5994I_40Mbps;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = 166833;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "SMPTE_D10_525x5994I_30Mbps")) {
					flavour = kAAFCodecFlavour_SMPTE_D10_525x5994I_30Mbps;
					sample_rate = sampleRate30;
					frameHeight = 480;
					comprSize = 125125;
					vidFmt = formatYUV422;
				}
				else if (!strcmp(fstr, "DVbased_1080x5994I_100Mbps")) {
					flavour = kAAFCodecFlavour_DVbased_1080x5994I_100Mbps;
					sample_rate = sampleRate30;
					comprSize = 480000;
					vidFmt = formatYUV422;
					frameWidth = 1280;
					frameHeight = 1080;
				}
				else if (!strcmp(fstr, "DVbased_720x5994P_100Mbps")) {
					flavour = kAAFCodecFlavour_DVbased_720x5994P_100Mbps;
					sample_rate = sampleRate60;
					comprSize = 240000;
					vidFmt = formatYUV422;
					frameWidth = 960;
					frameHeight = 720;
				}
				else {
					printf("Flavour %s not supported\n", fstr);
					exit(1);
				}
				i += 2;
			}
			else if (!strcmp(argv[i], "-raw"))
			{
				compressionEnable = false;
				i++;
			}
			else if (!strcmp(argv[i], "-mxf"))
			{
				formatMXF = true;
				i++;
			}
			else
			{
				break;
			}
		}
	}

	if (i < argc)
	{
		input_video = argv[i];		// source of uncompressed video

		if ( (input = fopen(input_video, "rb")) == NULL)
		{
			perror(input_video);
			return 1;
		}
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	checkFatal(CreateAAFFile(pwFileName, compressionEnable));

	printf("DONE\n");

	return(0);
}
