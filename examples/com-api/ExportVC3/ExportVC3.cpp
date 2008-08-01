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

#include "framebuffer.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

#define MAX_VC3_UNCOMPRESSED_FRAME (1080*1920*8)
#define MAX_VC3_COMPRESSED_FRAME (917504)

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

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

// settings derived from command line
int				ComprID=1253;				// arbitrary default = DNx36
aafRational_t	editRate = {30000,1001};	// default = NTSC idiocy
BufferLayout_t	layout = Layout_BGRAI;		// default = biggest possible

bool			UseDNX=false;				// default = VC3 web codec
aafUInt32		nCodecThreads=2;
aafUInt32		UseLegacyUIDs=0;			// default = SMPTE UIDs
bool			Atom=true;					// make an OP-Atom (the only choice for now)
bool			FrameGroup=false;			// make a frame-wrapped OP!A file (DISALLOWED until 1.1.3)
bool			FormatMXF = false;			// default = AAF
bool			FormatSS512 = false;		// default = 4k
unsigned		Duration = 10;				// default = 10 frames

static char*	input_video = NULL;

enum InputType	{ BarsUYVY, BarsRGB, Raw, UYVY, P_UYVY, UYVYI, UYVYM, BGRA, BGRAI };

enum InputType	input_type = BarsUYVY;			// no input file given means synthesize bars

// Local functions to map VCID to V H etc
//! Get BytesPerEditUnit
aafUInt32 GetBytesPerEditUnit(int ComprID)
{
	switch(ComprID)
	{
	case 1235:
	case 1238:
	case 1241:
	case 1243:
		return 917504;
		break;
	case 1237:
	case 1242:
	case 1244:
		return 606208;
		break;
	case 1250:
	case 1251:
		return 458752;
		break;
	case 1252:
		return 303104;
		break;
	case 1253:
		return 188416;
		break;
	default:
		return 0;
		break;
	}
}

//! Get LinesPerField for the selected CompressionID
int GetLinesPerField(int ComprID)
{
	switch(ComprID)
	{
	case 1235:
	case 1237:
	case 1238:
		return 1080;
		break;
	case 1241:
	case 1243:
	case 1242:
	case 1244:
		return 540;
		break;
	case 1250:
	case 1251:
	case 1252:
		return 720;
		break;
	case 1253:
		return 1080;
		break;
	default:
		return 0;
		break;
	}
}

//! Get PixelsPerLine for the selected CompressionID
int GetPixelsPerLine(int ComprID)
{
	switch(ComprID)
	{
	case 1235:
	case 1237:
	case 1238:
	case 1241:
	case 1243:
	case 1242:
		return 1920;
		break;
	case 1250:
	case 1251:
	case 1252:
		return 1280;
		break;
	case 1253:
		return 1920;
		break;
	case 1244:
		return 1440;
		break;
	default:
		return 0;
		break;
	}
}

//! Get Is10Bit for the selected CompressionID
bool Is10Bit(int ComprID)
{
	if( ComprID==1235 || ComprID==1241 || ComprID==1250)
		return true;
	return false;
}

//! Get IsInterlaced for the selected CompressionID
bool IsInterlaced(int ComprID)
{
	if(ComprID==1241 || ComprID==1242 || ComprID==1243 || ComprID==1244 )
		return true;
	return false;
}

//! Get Is720p for the selected CompressionID
bool Is720p(int ComprID)
{
	if(ComprID==1250 || ComprID==1251 || ComprID==1252 )
		return true;
	return false;
}

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
	ProductInfo.companyName = const_cast<wchar_t *>(L"none");
	ProductInfo.productName = const_cast<wchar_t *>(L"AAF SDK");
	ProductInfo.productVersion = &ver;
	ProductInfo.productVersionString = const_cast<wchar_t *>(L"1.0.0.0 Beta");
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;		// Set by SDK when saving

	// select the file kind
	const aafUID_t*			fileKind = &kAAFFileKind_DontCare;
	if( FormatMXF )			fileKind = &kAAFFileKind_AafKlvBinary;
	else if( FormatSS512 )	fileKind = &kAAFFileKind_Aaf512Binary;
	else					fileKind = &kAAFFileKind_Aaf4KBinary;

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
	check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));

	/* Create a Mastermob */

	// Get a Master MOB Interface
	check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));

	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));
	if (input_video == NULL)
	{
		check(pMob->SetName(L"DNX_color_bars"));
	}
	else
	{
		check(pMob->SetName(pFileName));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	check(pMasterMob->CreateEssence(
		1,			// Slot ID within MasterMob
		pPictureDef,			// MediaKind
		UseDNX? kAAFCodecDNxHD : kAAFCodecVC3,			// codecID
		editRate,				// edit rate
		editRate,				// sample rate
		comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable,
		NULL,					// No Locator used
		ContainerAAF,			// Essence embedded in AAF file
		&pEssenceAccess));		//

	// Set the codec flavour for desired video format
	switch(ComprID)
	{
	case 1235:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1235 );
		break;
	case 1238:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1238 );
		break;
	case 1237:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1237 );
		break;
	case 1241:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1241 );
		break;
	case 1243:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1243 );
		break;
	case 1244:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1244 );
		break;
	case 1242:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1242 );
		break;
	case 1250:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1250 );
		break;
	case 1251:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1251 );
		break;
	case 1252:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1252 );
		break;
	case 1253:
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_VC3_1253 );
		break;
	default:
		break;
	}

	switch( input_type )
	{
	case P_UYVY:
	case BarsUYVY:
	default:
		layout = UseDNX?Layout_P_UYVY:Layout_UYVYM;
		break;

	case UYVY:
		layout = UseDNX?Layout_UYVY:Layout_Unknown;
		break;

	case BGRAI:
	case BarsRGB:
		layout = UseDNX?Layout_BGRAI:Layout_Unknown;
		break;

	case BGRA:
		layout = UseDNX?Layout_BGRA:Layout_Unknown;
		break;
	}

	// Set up the information about the format of the data
	IAAFEssenceFormat *  fmtTemplate=NULL;
	check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
	
	if( UseLegacyUIDs )
		check(fmtTemplate->AddFormatSpecifier (kAAFLegacyAUIDs, sizeof(UseLegacyUIDs), (aafDataBuffer_t)&UseLegacyUIDs));

	if( UseDNX )
		check(fmtTemplate->AddFormatSpecifier (kAAFNumThreads, sizeof(nCodecThreads), (aafDataBuffer_t)&nCodecThreads));

	if( UseDNX )
		check(fmtTemplate->AddFormatSpecifier (kAAFBufferLayout, sizeof(layout), (aafDataBuffer_t)&layout));

	// codec will automatically SetEssenceKey() for MXF if !UseLegacyUIDs
	// codec will automatically SetEssenceKey() for Avid legacy if UseLegacyUIDs

	// future: if other kAAFEssenceElementKey, set it here

	// future: if other kAAFPhysicalTrackNum, set it here

	// Put the format specifiers into the codec
	check(pEssenceAccess->PutFileFormat( fmtTemplate ));

	// For fun, print the name of the selected codec flavour
	aafWChar codec_name[128] = L"";
	check(pEssenceAccess->GetCodecName(sizeof(codec_name), codec_name));
	printf("  using codec flavour \"%ls\"\n", codec_name);

	// Write the video samples
	int total_samples = 0;
	if( input_type==Raw )
	{
		// Read one frame of raw VC3 video at a time, repeat last frame until end
		FILE * ip=fopen( input_video,"rb" );
		if(!ip) return false;

		aafUInt32  nwant = GetBytesPerEditUnit(ComprID); 
		unsigned char* raw_buff = new unsigned char [ nwant ];

		size_t nread=0;

		while( Duration-- )
		{
			// keep on trying to read
			if( !feof( ip ) ) nread=fread( raw_buff, 1, nwant, ip );

			// abandon if partial frame (complete frame or zero is ok
			if ( nread!=nwant && nread!= 0 ) { perror(input_video); break; }

			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, raw_buff, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] raw_buff;
		fclose( ip );
	}
	else if( input_type==P_UYVY || input_type==UYVYI || input_type==UYVYM )
	{
		// Load a single frame of uncompressed
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*(GetPixelsPerLine(ComprID)/2)*(UseDNX?5:8); 
	
		unsigned char* video_buffer = new unsigned char [ nwant ];

		// int LoadXYUV( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_video, 
		//				 bool isLittleEndian=true, bool isBigEndian=false, bool isP_UYVY=false );

		LoadXYUV( UseDNX?Layout_P_UYVY:Layout_UYVYM, 
			      video_buffer, nwant, 
				  GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1), 
				  input_video,
				  input_type==UYVYI, input_type==UYVYM, input_type==P_UYVY );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}
	else if( input_type==UYVY )
	{
		// Load a single frame of uncompressed
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*(GetPixelsPerLine(ComprID)/2)*(UseDNX?4:8); 
	
		unsigned char* video_buffer = new unsigned char [ nwant ];

		// int LoadXYUV( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_video, 
		//				 bool isLittleEndian=true, bool isBigEndian=false, bool isP_UYVY=false );

		LoadXYUV( UseDNX?Layout_UYVY:Layout_UYVYM, 
			      video_buffer, nwant, 
				  GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1), 
				  input_video,
				  false, false, false );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}
	else if( input_type==BGRAI )
	{
		// never get here with VC3Codec

		// Load a single frame of uncompressed
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*GetPixelsPerLine(ComprID)*8; 
	
		unsigned char* video_buffer = new unsigned char [ nwant ];

		// int LoadXRGB( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_video, 
		//				 bool isLittleEndian=true, bool isBigEndian=false, bool hasA=false, bool isRGB=false );

		LoadXRGB( Layout_BGRAI, 
			      video_buffer, nwant, 
				  GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1), 
				  input_video,
				  input_type==BGRAI, false, true, false );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}
	else if( input_type==BGRA )
	{
		// never get here with VC3Codec

		// Load a single frame of uncompressed
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*GetPixelsPerLine(ComprID)*8; 
	
		unsigned char* video_buffer = new unsigned char [ nwant ];

		// int LoadXRGB( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_video, 
		//				 bool isLittleEndian=true, bool isBigEndian=false, bool hasA=false, bool isRGB=false );

		LoadXRGB( Layout_BGRA, 
			      video_buffer, nwant, 
				  GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1), 
				  input_video,
				  input_type==BGRAI, false, true, false );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}
	else if( input_type==BarsRGB )		// using generated rgb colo(u)r bars
	{
		// never get here with VC3Codec

		// Create a frame of colour bars
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*GetPixelsPerLine(ComprID)*8; 
		unsigned char* video_buffer = new unsigned char [ nwant ];

		LoadXBars( Layout_BGRAI, video_buffer, nwant, GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1) );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}
	else // if( input_type==BarsUYVY || input_video == NULL || others... )		// using generated component colo(u)r bars
	{
		// Create a frame of colour bars
		aafUInt32  nwant = GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1)*(GetPixelsPerLine(ComprID)/2)*(UseDNX?5:8); 
		unsigned char* video_buffer = new unsigned char [ nwant ];

		LoadXBars( UseDNX?Layout_P_UYVY:Layout_UYVYM, video_buffer, nwant, GetPixelsPerLine(ComprID), GetLinesPerField(ComprID)*(IsInterlaced(ComprID)?2:1) );

		while( Duration-- )
		{
			samplesWritten=0;
			check(pEssenceAccess->WriteSamples(1, nwant, video_buffer, &samplesWritten, &bytesWritten));
			total_samples += samplesWritten;
		}
		delete[] video_buffer;
	}

	printf("Completed Write\n");

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
	cout << "\tWith no arguments creates ExportVC3.aaf containing 10 VC3 frames" << endl;
	cout << "\tCurrently, AAFSDK only supports OP-Atom (-a), does not support OP1A" << endl;
	cout << endl;
	cout << "\t-o=<file>      write output to specified file instead of ExportVC3.aaf" << endl;
	cout << "\t-cbar=<dur>    Make bars using given compression format, starting from uyvy" << endl;
	cout << "\t-bars=<dur>    Make bars using given compression format, starting from rgb0" << endl;
	cout << "\t-raw=<file>    Specified input video is already compressed VC3" << endl;
 	cout << "\t-uyvy=<file>   Specified input video is uncompressed 8 bit UYVY format" << endl;
 	cout << "\t-p_uyvy=<file> Specified input video is uncompressed 10 bit UYVY, packed lsbs first in separate block" << endl;
 	cout << "\t-uyvyi=<file>  Specified input video is uncompressed 10 bit UYVY, little-endian, msb aligned" << endl;
 	cout << "\t-uyvym=<file>  Specified input video is uncompressed 10 bit UYVY, big-endian, msb aligned" << endl;
 	cout << "\t-bgra=<file>   Specified input video is uncompressed 8 bit BGRA" << endl;
 	cout << "\t-bgrai=<file>  Specified input video is uncompressed 16 bit BGRA, little-endian, msb aligned" << endl;
	cout << "\t-r=<n>/<m>     Specify Frame Rate, e.g. 24/1, 30000/1001" << endl;
	cout << "\t-dur=<n>       Specify duration of file (repeat single frame)" << endl;
	cout << "\t-DNX=<vcid>    Use DNX Codec to create essence with compression code n" << endl;
	cout << "\t-VC3=<vcid>    Use VC3 Codec to create essence with compression code n" << endl;
	cout << "\t-threads=<n>   Specify number of threads for the codec" << endl;
	cout << "\t-av            Use Avid legacy UIDs" << endl;
 	cout << "\t-legacy        Use legacy UIDs and properties" << endl;
	cout << "\t-a			  Make an OP-Atom file" << endl;
	cout << "\t-f			  Make a frame-wrapped OP1A file (DISABLED UNTIL 1.1.3)" << endl;
	cout << "\t-mxf           write output to KLV (MXF) file instead of SS (AAF)" << endl;
	cout << "\t-ss512         write output to AAF 512B sectors" << endl;
	cout << endl;
	cout << "\tvalues of options may be in next arg instead of after =" << endl;
}

char* opt( int& i, int argc, char** argv)
{
	char *colon=strchr( argv[i], '=' );
	if( colon )			{ i++; return colon+1; }
	else if( i<argc-1 ) { i+=2; return argv[i+1]; }
	else				{ i++; printUsage(argv[0]);	return NULL; }
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar 		pwFileName[FILENAME_MAX] = L"ExportVC3.aaf";
	bool			compressionEnable = true;		// supply synthetic component bars to codec by default

	int i = 1;
	if (argc > 1)
	{
		while (i < argc)
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			{
				printUsage(argv[0]);
				return 1;
			}
			else if (!strncmp(argv[i], "-o=",3))
			{
				mbstowcs(pwFileName, opt(i,argc,argv), sizeof(pwFileName));
			}
			else if (!strncmp(argv[i], "-cbar",5))
			{
				input_type = BarsUYVY;
				compressionEnable=true;
				Duration=10;	// default duration
				Duration=atoi( opt(i,argc,argv) );
			}
			else if (!strncmp(argv[i], "-bars",5))
			{
				input_type = BarsRGB;
				compressionEnable=true;
				Duration=10;	// default duration
				Duration=atoi( opt(i,argc,argv) );
			}
			else if (!strncmp(argv[i], "-raw",4))
			{
				input_type = Raw;
                compressionEnable = false;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-uyvyi",6))
			{
				input_type = UYVYI;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-uyvym",6))
			{
				input_type = UYVYM;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-uyvy",5))
			{
				input_type = UYVY;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-p_uyvy",7))
			{
				input_type = P_UYVY;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-bgrai",6))
			{
				input_type = BGRAI;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-bgra",5))
			{
				input_type = BGRA;
				compressionEnable = true;
				input_video = opt(i,argc,argv);
			}
			else if (!strncmp(argv[i], "-r",2))
			{
				int N, D; // Use ints in case Int32 is not the same size as "int" on this platform
				if( sscanf( opt(i,argc,argv),"%d/%d", &N, &D ) == 2)
				{
					editRate.numerator = N;
					editRate.denominator = D;
				}
				else
				{
					printUsage(argv[0]);
					return 1;
				}
			}
			else if (!strncmp(argv[i], "-dur",4))
			{
				Duration=atoi( opt(i,argc,argv) );
			}
			else if (!strncmp(argv[i], "-VC3",4) || !strncmp(argv[i], "-vc3",4))
			{
				UseDNX=false;
				ComprID=atoi( opt(i,argc,argv) );
			}
			else if (!strncmp(argv[i], "-DNX",4) || !strncmp(argv[i], "-dnx",4))
			{
				UseDNX=true;
				ComprID=atoi( opt(i,argc,argv) );
			}
			else if (!strncmp(argv[i], "-threads",8))
			{
				nCodecThreads=atoi( opt(i,argc,argv) );
			}
			else if (!strcmp(argv[i], "-legacy") || !strcmp(argv[i], "-av"))
			{
				UseLegacyUIDs=1;
				i++;
			}
			else if (!strcmp(argv[i], "-a"))
			{
				Atom = true;
				FrameGroup = false;
				i++;
			}
			else if (!strcmp(argv[i], "-f"))
			{
				FrameGroup = true;
				Atom = false;
				i++;
			}
			else if (!strcmp(argv[i], "-mxf"))
			{
				FormatMXF = true;
				i++;
			}
			else if (!strcmp(argv[i], "-ss512"))
			{
				FormatSS512 = true;
				i++;
			}
			else
			{
				printUsage(argv[0]);
				return 1;
			}
		}
	}

	// this version can only make OP-Atom
	if( !Atom || FrameGroup )
	{
		printUsage(argv[0]);
		return 1;
	}

	// VC3 Codec can only accept UYVYM
	if( !UseDNX )
	{
		if( input_type == BarsRGB || input_type == BGRA || input_type == BGRAI )
		{
			cout << "\tVC3 Codec can only accept UYVY" << endl;
			return 1;
		}
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());


	checkFatal(CreateAAFFile(pwFileName, compressionEnable));

	printf("DONE\n");

	return(0);
}
