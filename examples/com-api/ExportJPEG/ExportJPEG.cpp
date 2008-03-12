// @com Executable example program by Stuart Cunningham, BBC R&D
// @com This is based upon ComEssenceDataTest.cpp.

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


// Use libdv API terminology for consistency with internal use of libdv API
// where "PAL" & "NTSC" mean 625/50 and 525/60 respectively
bool formatPAL = true;

const aafInt32 UNC_PAL_FRAME_SIZE = 720*576*2;
const aafInt32 UNC_NTSC_FRAME_SIZE = 720*480*2;

bool useLegacySS = true;

aafUID_t kAAFOpDef_Atom = { 0x0d010201, 0x1000, 0x0000, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01}};

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

// Represent the colour and position of a colour bar
typedef struct {
	double			position;
	unsigned char	colour[4];
} bar_colour_t;

// Generate a video buffer containing uncompressed UYVY video representing
// the familiar colour bars test signal (or YUY2 video if specified).
static void create_colour_bars(unsigned char *video_buffer, int width, int height, int convert_to_YUY2)
{
	int				i,j,b;
	bar_colour_t	UYVY_table[] = {
				{52/720.0,	{0x80,0xEB,0x80,0xEB}},	// white
				{140/720.0,	{0x10,0xD2,0x92,0xD2}},	// yellow
				{228/720.0,	{0xA5,0xA9,0x10,0xA9}},	// cyan
				{316/720.0,	{0x35,0x90,0x22,0x90}},	// green
				{404/720.0,	{0xCA,0x6A,0xDD,0x6A}},	// magenta
				{492/720.0,	{0x5A,0x51,0xF0,0x51}},	// red
				{580/720.0,	{0xf0,0x29,0x6d,0x29}},	// blue
				{668/720.0,	{0x80,0x10,0x80,0x10}},	// black
				{720/720.0,	{0x80,0xEB,0x80,0xEB}}	// white
			};

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i+=2)
		{
			for (b = 0; b < 9; b++)
			{
				if ((i / ((double)width)) < UYVY_table[b].position)
				{
					if (convert_to_YUY2) {
						// YUY2 packing
						video_buffer[j*width*2 + i*2 + 1] = UYVY_table[b].colour[0];
						video_buffer[j*width*2 + i*2 + 0] = UYVY_table[b].colour[1];
						video_buffer[j*width*2 + i*2 + 3] = UYVY_table[b].colour[2];
						video_buffer[j*width*2 + i*2 + 2] = UYVY_table[b].colour[3];
					}
					else {
						// UYVY packing
						video_buffer[j*width*2 + i*2 + 0] = UYVY_table[b].colour[0];
						video_buffer[j*width*2 + i*2 + 1] = UYVY_table[b].colour[1];
						video_buffer[j*width*2 + i*2 + 2] = UYVY_table[b].colour[2];
						video_buffer[j*width*2 + i*2 + 3] = UYVY_table[b].colour[3];
					}
					break;
				}
			}
		}
	}
}

// Copy lines from interleaved UYVY frame into separated fields of UYVY
// field_height is provided as the height of a single field
static void separate_fields(aafUInt8 *src, aafUInt8 *dest, int width, int field_height)
{
	aafUInt8 *dest_top = dest;
	aafUInt8 *dest_bot = dest + width * field_height * 2;

	// Copy top field
	int o_line = 0;
	for (int i = 0; i < field_height * 2; i += 2)
	{
		memcpy(&dest_top[o_line * width*2], &src[i * width*2], width*2);
		memcpy(&dest_bot[o_line * width*2], &src[(i+1) * width*2], width*2);
		o_line++;
	}
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ExportJPEG";

static char *input_video = NULL;
static FILE *input;
static int res_num = 15;			// default is 15:1 Resolution
static int frame_limit = -1;		// number of frames to embed

static HRESULT CreateAAFFile(aafWChar * pFileName, bool comp_enable)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	aafMobID_t					masterMobID;
	aafProductIdentification_t	ProductInfo;
	aafRational_t				editRate = {25, 1};
	aafRational_t				sampleRate = {25, 1};
	IAAFClassDef				*pCDMasterMob = NULL;
	IAAFDataDef					*pPictureDef = NULL;
	aafUInt32					samplesWritten, bytesWritten;

	// Delete any previous test file before continuing...
	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);

	cout << "Creating file " << cFileName << " using WriteSamples with ResolutionId " <<
		res_num << ":1 and " <<
		(comp_enable ? "CompressionEnable" : "CompressionDisable") << endl;

	aafProductVersion_t ver = {1, 0, 0, 0, kAAFVersionBeta};
	ProductInfo.companyName = companyName;
	ProductInfo.productName = productName;
	ProductInfo.productVersion = &ver;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;		// Set by SDK when saving


	const aafUID_t *fileKind = 0;
	// Large sectors for new files, small sectors for legacy files
    if (useLegacySS) {
	    fileKind = &kAAFFileKind_Aaf512Binary;
    } else {
        fileKind = &kAAFFileKind_Aaf4KBinary;
	}

	// Create a new AAF file
	check(AAFFileOpenNewModifyEx(pFileName, fileKind, 0, &ProductInfo, &pFile));
	check(pFile->GetHeader(&pHeader));


	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup class definitions for the objects we want to create. */
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pCDMasterMob));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(kAAFDataDef_LegacyPicture, &pPictureDef));
	//check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));

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
		aafWChar 		mob_name[FILENAME_MAX];
		mbstowcs(mob_name, input_video, sizeof(mob_name));
		check(pMob->SetName(mob_name));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	aafInt32 frameSize = UNC_PAL_FRAME_SIZE;
	if (! formatPAL) {
		editRate.numerator = 2997;
		editRate.denominator = 100;
		sampleRate.numerator = 2997;
		sampleRate.denominator = 100;
		frameSize = UNC_NTSC_FRAME_SIZE;
	}

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	check(pMasterMob->CreateEssence(1,			// Slot ID within MasterMob
						pPictureDef,			// MediaKind
						kAAFCodecJPEG,			// codecID
						editRate,				// edit rate
						sampleRate,				// sample rate
						comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable,
						NULL,					// No Locator used
						ContainerAAF,			// Essence embedded in AAF file
						&pEssenceAccess));		//

	// Set Format specifiers that describe the essence data
	// Default settings are for "15:1s"
	aafFrameLayout_t frame_layout = kAAFOneField;
	aafInt32 YUV_pixel = kAAFColorSpaceYUV;
	aafInt32 horiz_sub_sampling = 2;
	aafRect_t stored_rect = {0, 0, 352, 296};
	aafRect_t sampled_rect = {0, 0, 352, 296};
	aafRect_t display_rect = {0, 8, 352, 288};	// doesn't display VBI (2 x 8 = 16 lines)
	aafUID_t compression = kAAFCompressionDef_AAF_CMPR_FULL_JPEG;
	aafInt32 line_map[2] = {15, 0};
	aafColorSiting_t siting = kAAFRec601;
	int lines_of_VBI = 8;
	int frame_height = 576;

	if (! formatPAL) {
		frame_height = 480;
		stored_rect.ySize = 248;
		sampled_rect.ySize = 248;
		// Avid must have display hieght of 243 and display y offset of 5 otherwise it
		// will fail to import with "Warning: NO WRONG FORMAT Media found" in the console
		display_rect.ySize = 243;
		display_rect.yOffset = 5;
		line_map[0] = 16;
	}

	aafInt32 resolution_id;
	switch (res_num) {
		case 2:
			resolution_id = 0x4c;
			break;
		case 3:
			resolution_id = 0x4d;
			break;
		case 10:
			resolution_id = 0x4b;
			break;
		case 15:
			resolution_id = 0x4e;
			break;
		case 20:
			resolution_id = 0x52;
			break;
	}
	if (res_num != 15) {
		// All resolutions are SeparateFields except "15:1s"
		frame_layout = kAAFSeparateFields;
		lines_of_VBI = 16;
		if (formatPAL) {
			stored_rect.xSize = 720;
			stored_rect.ySize = 296;
			sampled_rect.xSize = 720;
			sampled_rect.ySize = 296;
			display_rect.xSize = 720;
			display_rect.ySize = 288;
			line_map[0] = 15;
			line_map[1] = 328;
		}
		else	// NTSC
		{
			stored_rect.xSize = 720;
			stored_rect.ySize = 248;
			sampled_rect.xSize = 720;
			sampled_rect.ySize = 248;
			display_rect.xSize = 720;
			display_rect.ySize = 243;
			line_map[0] = 16;		// TODO: fix line_map guesses
			line_map[1] = 328;
		}
	}

	IAAFEssenceFormat*			pFormat = NULL;
	check(pEssenceAccess->GetEmptyFileFormat(&pFormat));
	check(pFormat->AddFormatSpecifier(kAAFPixelFormat, 4, (unsigned char *) &YUV_pixel));
	check(pFormat->AddFormatSpecifier(kAAFFrameLayout, 4, (unsigned char *) &frame_layout));
	check(pFormat->AddFormatSpecifier(kAAFCDCIHorizSubsampling, 4 ,(unsigned char *) &horiz_sub_sampling));
	check(pFormat->AddFormatSpecifier(kAAFStoredRect, 16, (unsigned char *) &stored_rect));
	check(pFormat->AddFormatSpecifier(kAAFSampledRect, 16, (unsigned char *) &sampled_rect));
	check(pFormat->AddFormatSpecifier(kAAFDisplayRect, 16, (unsigned char *) &display_rect));
	check(pFormat->AddFormatSpecifier(kAAFVideoLineMap, 8, (unsigned char *) &line_map));
	check(pFormat->AddFormatSpecifier(kAAFCDCIColorSiting, 4, (unsigned char *) &siting));
	check(pFormat->AddFormatSpecifier(kAAFCompression, 16, (unsigned char *) &compression));
	check(pFormat->AddFormatSpecifier(kAAFResolutionID, 4, (unsigned char *) &resolution_id));
	check(pEssenceAccess->PutFileFormat(pFormat));

	pFormat->Release();

	// Setup uncompressed video buffer with room for 16 lines of black
	int writeFrameSize = 720*(frame_height+16)*2;
	aafUInt8 *video_buf = new aafUInt8[writeFrameSize];
	aafUInt8 *buf_separate_fields = new aafUInt8[writeFrameSize];

	// Set first 16 lines to black to represent empty VBI
	for (int i = 0; i < 720*16*2; i+=4) {	// 16 lines by 720 pixels wide (2 bytes per pixel)
		video_buf[i+0] = 0x80;
		video_buf[i+1] = 0x10;
		video_buf[i+2] = 0x80;
		video_buf[i+3] = 0x10;
	}
	unsigned char *dataPtr = video_buf;

	// Write the video samples
	int total_samples = 0;
	if (input_video == NULL)		// use generated uncompressed video with compression enabled
	{
		create_colour_bars(video_buf + lines_of_VBI * stored_rect.xSize*2,	// skip 8 or 16 lines of black
							stored_rect.xSize, stored_rect.ySize*2 - lines_of_VBI, false);

		// Rearrange interleaved frame into separate fields in one frame
		separate_fields(dataPtr, buf_separate_fields, stored_rect.xSize, stored_rect.ySize);
		dataPtr = buf_separate_fields;

		int frames_to_write = (frame_limit == -1) ? 10 : frame_limit;		// 10 frames by default
		for (int i = 0; i < frames_to_write; i++)
		{
			check(pEssenceAccess->WriteSamples(	1,					//
												writeFrameSize,		// buffer size
												dataPtr,			// pointer to video frame
												&samplesWritten,
												&bytesWritten));
			total_samples += samplesWritten;
		}
	}
	else if (comp_enable)	// using uncompressed video and compressing using codec
	{
		if ((input = fopen(input_video, "rb")) == NULL)
		{
			fprintf(stderr, "Could not open %s\n", input_video);
			perror("fopen");
		}


		aafUInt8 *buf_scaled = new aafUInt8[stored_rect.xSize * (frame_height+16) * 2 * 8];

		// Read and pass each frame of video to codec
		while (1)
		{
			// Read frame into position 16 lines after blank VBI
			if ( fread(video_buf + 16*720*2, frameSize, 1, input) != 1 )
			{
				if (feof(input))
					break;
				fprintf(stderr, "Could not read UYVY frame\n");
				perror("fread");
				exit(1);
			}
			if (res_num == 15)
			{
				// scale image horizontally from 720 to stored width (E.g. 352) pixels
				int tgt_width = stored_rect.xSize;
				double scale = 720.0 / tgt_width;
				for (int y = 0; y < frame_height+16; y++) {
					// For one line, read two macropixels at a time and scale down to target width
					for (int x = 0; x < stored_rect.xSize/2; x++) {
						int src = (int)(x*scale + 0.5);		// source macropixel rounded to integer
						buf_scaled[y*tgt_width*2 + x*4 + 0] = video_buf[y*720*2 + src*4 + 0];
						buf_scaled[y*tgt_width*2 + x*4 + 1] = video_buf[y*720*2 + src*4 + 1];
						buf_scaled[y*tgt_width*2 + x*4 + 2] = video_buf[y*720*2 + src*4 + 2];
						buf_scaled[y*tgt_width*2 + x*4 + 3] = video_buf[y*720*2 + src*4 + 3];
					}					
				}
				dataPtr = buf_scaled;
				writeFrameSize = tgt_width * (frame_height+16) * 2;
			}

			// Rearrange interleaved frame into separate fields in one frame
			separate_fields(dataPtr, buf_separate_fields, stored_rect.xSize, stored_rect.ySize);
			dataPtr = buf_separate_fields;

			// Now pass UYVY frame over API, which contains separated fields
			check(pEssenceAccess->WriteSamples(
									1,					// number of samples
									writeFrameSize,		// buffer size
									dataPtr,			// samples buffer
									&samplesWritten,
									&bytesWritten));
			total_samples += samplesWritten;

		}
		delete [] buf_scaled;
	}
	else 			// use pre-compressed JPEG pictures (compression disabled)
	{
		// open JPEG stream file
		if ((input = fopen(input_video, "rb")) == NULL)
		{
			fprintf(stderr, "Could not open %s\n", input_video);
			perror("fopen");
			exit(1);
		}

		// buffer for storing one compressed JPEG image read from file
		int max_jpeg_size = 1000000;
		aafUInt8 *jpeg_buf = new aafUInt8[max_jpeg_size];

		while (true)
		{
			// find JPEG image boundary i.e. FFD8...FFD9 FFD8...FFD9
			aafUInt8	*p = jpeg_buf;
			int			c, last_c = -1;
			while ((c = fgetc(input)) != EOF)
            {
				if (p - jpeg_buf > max_jpeg_size) {
					fprintf(stderr, "JPEG image exceeded expected maximum of %d\n", max_jpeg_size);
					exit(1);
				}
				*p++ = c;
                if (last_c == 0xFF && c == 0xD9)
					break;
				last_c = c;
            }
			if (c == EOF)		// exhausted input file
				break;

			int img_size = p - jpeg_buf;
			dataPtr = jpeg_buf;

			// Write the already-compressed JPEG image (compression disabled)
			check(pEssenceAccess->WriteSamples(
									1,					// number of samples
									img_size,			// buffer size
									dataPtr,			// samples buffer
									&samplesWritten,
									&bytesWritten));
			total_samples += samplesWritten;
		}
		delete [] jpeg_buf;
	}
	delete [] video_buf;
	delete [] buf_separate_fields;
	printf("Wrote %d samples\n", total_samples);

	// Set the essence to indicate that you have finished writing the samples
	check(pEssenceAccess->CompleteWrite());

	pEssenceAccess->Release();

	pMob->Release();
	pMasterMob->Release();

	pPictureDef->Release();
	pCDMasterMob->Release();

	pDictionary->Release();
	pHeader->Release();

	pFile->Save();
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
	cout << "Usage : " << progname << " [options]" << endl;
	cout << endl;
	cout << "\tWith no arguments creates ExportJPEG.aaf in legacy 512B sectors" << endl;
	cout << "\tcontaining 10 JPEG PAL colorbar \"frames\" at 15:1s" << endl;
	cout << endl;
	cout << "\t-ntsc          treat inputs and colorbars as 720x480 30fps instead of" << endl;
	cout << "\t               the default 720x576 25fps" << endl;
	cout << "\t-res <id>      resolution: 2:1, 3:1, 10:1, 15:1, 20:1" << endl;
	cout << "\t-noLegacySS    use 4kB sectors instead of legacy 512B sectors" << endl;
	cout << "\t-o file        write output to specified file instead of ExportJPEG.aaf" << endl;
	cout << "\t-rawUYVY file  input UYVY video file to JPEG compress and embed" << endl;
	cout << "\t-rawJPEG file  specified input contains already compressed JPEG pictures" << endl;
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar 		pwFileName[FILENAME_MAX] = L"ExportJPEG.aaf";
	bool			compressionEnable = true;		// convert from uncomp to JPEG by default

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
			else if (!strcmp(argv[i], "-ntsc")) 	 
			{ 	 
				formatPAL = false; 	 
				i++; 	 
			}
			else if (!strcmp(argv[i], "-noLegacySS")) 	 
			{ 	 
				useLegacySS = false; 	 
				i++; 	 
			}
			else if (!strcmp(argv[i], "-res"))
			{
				if (i >= argc-1)
				{
					printUsage(argv[0]);
					return 1;
				}
				if (strcmp(argv[i+1], "2:1") == 0) {
					res_num = 2;
				}
				else if (strcmp(argv[i+1], "3:1") == 0) {
					res_num = 3;
				}
				else if (strcmp(argv[i+1], "10:1") == 0) {
					res_num = 10;
				}
				else if (strcmp(argv[i+1], "15:1") == 0) {
					res_num = 15;
				}
				else if (strcmp(argv[i+1], "20:1") == 0) {
					res_num = 20;
				}
				else {
					printUsage(argv[0]);
					return 1;
				}
				i += 2;
			}
			else if (!strcmp(argv[i], "-n"))
			{
				if (i >= argc-1)		// -n requires integer arg
				{
					printUsage(argv[0]);
					return 1;
				}
				frame_limit = atol(argv[i+1]);
				i += 2;
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
			else if (!strcmp(argv[i], "-rawUYVY"))
			{
				if (i >= argc-1)
				{
					printUsage(argv[0]);
					return 1;
				}
				compressionEnable = true;
				input_video = argv[i+1];
				i+=2;
			}
			else if (!strcmp(argv[i], "-rawJPEG"))
			{
				if (i >= argc-1)
				{
					printUsage(argv[0]);
					return 1;
				}
				compressionEnable = false;
				input_video = argv[i+1];
				i+=2;
			}
			else
			{
				break;
			}
		}
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	checkFatal(CreateAAFFile(pwFileName, compressionEnable));

	printf("DONE\n");

	return(0);
}
