//
// $Id$ $Name$
//
// eli2aaf.cpp -	Create an AAF file with embedded media from a lavtools
//					(mjpegtools) ELI format file.  Such a format is created
//					by Kino's "Save As ELI" menu option.
//
// Copyright (C) 2003 Stuart Cunningham <stuart_hc@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU General Public
// License as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


#ifdef _MSC_VER
#pragma warning( disable : 4786 )	// disable warning about debug info truncated
#endif

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <unistd.h>				// stat() under unix
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <AAF.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxMob.h>
#include <AxComponent.h>
#include <AxSmartPointer.h>
#include <AxEssence.h>
#include <AxMobSlot.h>
#include <AAFDataDefs.h>
#include <AAFContainerDefs.h>
#include <AAFCodecDefs.h>
#include <AAFClassDefUIDs.h>
#include <AAFTypeDefUIDs.h>
#include <AxMetaDef.h>
#include <AxPropertyValue.h>
#include <AAFResult.h>
#include <AAFFileKinds.h>
#include <AAFFileMode.h>

#define DV_PAL_FRAME_SIZE 144000
#define DV_NTSC_FRAME_SIZE 120000

using namespace std;

#if defined( _MSC_VER )
// MS VC++ cannot cope with a 64bit int passed to the ostream << operator
// so use this ugly workaround where it is converted to a string first.
static char str_int64_workaround[30];	// big enough for 2**64-1 as decimal
static const char *ostream_int64(aafInt64 value)
{
	sprintf(str_int64_workaround, "%I64d", value);
	return str_int64_workaround;
}
#else
#define ostream_int64
#endif
const aafMobID_t zerouid = {{0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,{0,0,0,{0,0,0,0,0,0,0,0}}};

const aafUID_t kAAFPropID_CDCIOffsetToFrameIndexes	= { 0x9d15fca3, 0x54c5, 0x11d3, { 0xa0, 0x29, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameIndexByteOrder	= { 0xb57e925d, 0x170d, 0x11d4, { 0xa0, 0x8f, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDResolutionID			= { 0xce2aca4d, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFirstFrameOffset		= { 0xce2aca4e, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameSampleSize		= { 0xce2aca50, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDImageSize				= { 0xce2aca4f, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

static bool writeNetworkLocator = false;
static bool useRawStorage = false;
static bool useLargeSectors = false;

// Add a property definition to the dictionary, after first checking if it
// needs to be added (used in CreateLegacyPropDefs() only)
#define ADD_PROPERTY_DEF(classdef, prop, type) \
	if(p##classdef->LookupPropertyDef(kAAFPropID_##classdef##prop, &pPropertyDef) != AAFRESULT_SUCCESS) \
	{ \
		CHECK_HRESULT(p_dict->LookupTypeDef(kAAFTypeID_##type, &pTypeDef)); \
		CHECK_HRESULT(p##classdef->RegisterOptionalPropertyDef (kAAFPropID_##classdef##prop, \
			L###prop, \
			pTypeDef, &pPropertyDef)); \
		pTypeDef->Release(); \
		pTypeDef = NULL; \
		pPropertyDef->Release(); \
		pPropertyDef = NULL; \
	}

static void CreateLegacyPropDefs(IAAFDictionary *p_dict)
{
	IAAFPropertyDef			*pPropertyDef = NULL;
	IAAFTypeDef				*pTypeDef = NULL;
	IAAFClassDef            *pCDCI = NULL;
	IAAFClassDef            *pDID = NULL;

	CHECK_HRESULT(p_dict->LookupClassDef(kAAFClassID_CDCIDescriptor, &pCDCI));
	CHECK_HRESULT(p_dict->LookupClassDef(kAAFClassID_DigitalImageDescriptor, &pDID));

	ADD_PROPERTY_DEF( CDCI, OffsetToFrameIndexes, Int32 );
	ADD_PROPERTY_DEF( DID, FrameIndexByteOrder, Int16 );
	ADD_PROPERTY_DEF( DID, ResolutionID, Int32 );
	ADD_PROPERTY_DEF( DID, FirstFrameOffset, Int32 );
	ADD_PROPERTY_DEF( DID, FrameSampleSize, Int32 );
	ADD_PROPERTY_DEF( DID, ImageSize, Int64 );

	if (pCDCI)
	{
		pCDCI->Release();
		pCDCI = 0;
	}
	if (pDID)
	{
		pDID->Release();
		pDID = 0;
	}
	if (pPropertyDef)
	{
		pPropertyDef->Release();
		pPropertyDef = 0;
	}
	if (pTypeDef)
	{
		pTypeDef->Release();
		pTypeDef = 0;
	}
}

// Strip leading slash and trailing extension (if any)
static string stripDirAndExt(const char *str)
{
		string noext;

		string basename = str;
		string::size_type strpos;
		strpos = basename.find_last_of("\\/");

		if (strpos != string::npos)
			noext = basename.substr(strpos + 1);
		else
			noext = basename;

		strpos = noext.find_last_of(".");
		if (strpos != string::npos)
			noext.resize( strpos );

		return noext;
}


static unsigned char WAVEHeader[44];
static int & WAVEDataLen = *reinterpret_cast<int *>(WAVEHeader + 40);
static int & WAVEFileLen = *reinterpret_cast<int *>(WAVEHeader + 4);

static void CreateWaveHeader(unsigned char * ptr)
{
	aafInt32 * ptr32;
	aafInt16 * ptr16;

		*ptr++ = 'R';		// Make sure that this is NOT byte-swapped
		*ptr++ = 'I';
		*ptr++ = 'F';
		*ptr++ = 'F';

		ptr += 4;			// Patch form length in later

		*ptr++ = 'W';
		*ptr++ = 'A';
		*ptr++ = 'V';
		*ptr++ = 'E';
		*ptr++ = 'f';
		*ptr++ = 'm';
		*ptr++ = 't';
		*ptr++ = ' ';
		ptr32 = reinterpret_cast<aafInt32 *>(ptr);
		*ptr32++ = 0x10;
		ptr16 = reinterpret_cast<aafInt16 *>(ptr32);
		*ptr16++ = 0x1;
		*ptr16++ = 0x1;
		ptr32 = reinterpret_cast<aafInt32 *>(ptr16);
		*ptr32++ = 48000;
		*ptr32++ = 96000;
		ptr16 = reinterpret_cast<aafInt16 *>(ptr32);
		*ptr16++ = 0x2;
		*ptr16++ = 16;
		ptr = reinterpret_cast<unsigned char *>(ptr16);
		*ptr++ = 'd';
		*ptr++ = 'a';
		*ptr++ = 't';
		*ptr++ = 'a';
}


#define DV_AUDIO_SAMPLES 1920

static int dv_audio_unshuffle_50[6][9] = {
  {  0, 18, 36, 13, 31, 49,  8, 26, 44 },
  {  3, 21, 39, 16, 34, 52, 11, 29, 47 },
  {  6, 24, 42,  1, 19, 37, 14, 32, 50 },
  {  9, 27, 45,  4, 22, 40, 17, 35, 53 },
  { 12, 30, 48,  7, 25, 43,  2, 20, 38 },
  { 15, 33, 51, 10, 28, 46,  5, 23, 41 },
};


// Deshuffle the audio samples from a PAL DV frame.
// See the SMPTE 314M-1999 specification, section 4.6.2.2.
static int deshuffle_audio_block(const unsigned char *inbuf, int dif_seq,
									int audio_dif, short *outbufs)
{
	if (dif_seq >= 6)
		dif_seq -= 6;			// hard-coded for 625/50

	int i_base = dv_audio_unshuffle_50[dif_seq][audio_dif];

	for (int bp = 8; bp < 80; bp+=2)
	{
		int i = i_base + (bp - 8)/2 * (6 * 9);
		outbufs[i] = (inbuf[bp] << 8) | inbuf[bp+1];
	}

	return 0 ;
}


// Return timecode as an integer number of frames from 0:00:00.00
// using the first DV frame from the specified DV file.
// This is based on the specification in SMPTE 314M-1999.
static int getFirstTimecodeFromDVFile(const char *file)
{
	int frames = 0;
	FILE *fp = fopen(file, "rb");
	if (fp == NULL)
	{
		perror(file);
		return -1;
	}

	// Seek to the first frame, Subcode section, past the subcode ID (3 bytes),
	// skip the first 3 SSYBs (3*8), then skip the SSYB ID (3 bytes),
	// leaving the file pointer at the start of the TC payload of 5 bytes
	fseek(fp, 0 + 80 + 3 + 3*8 + 3, SEEK_SET);

	// Read the SSYB data paylod of 5 bytes that store the timecode
	unsigned char buf[5];
	if (fread(buf, 1, sizeof(buf), fp) != sizeof(buf))
	{
		perror(file);
		fclose(fp);
		return -1;
	}

	// Treat all 0xff values as a special case and return 0
	// (the first byte buf[0] always contains 0x13 and does not store timecode)
	if (buf[1] == 0xff && buf[2] == 0xff && buf[3] == 0xff && buf[4] == 0xff)
	{
		fclose(fp);
		return 0;
	}

	int color_frame, drop_frame_flag;
	int frame_tens, frame_units, seconds_tens, seconds_units;
	int minutes_tens, minutes_units, hours_tens, hours_units;
	color_frame = (buf[1] >> 7) & 0x01;
	drop_frame_flag = (buf[1] >> 6) & 0x01;
	frame_tens = (buf[1] >> 4) & 0x03;
	frame_units = buf[1] & 0x0f;
	seconds_tens = (buf[2] >> 4) & 0x07;
	seconds_units = buf[2] & 0x0f;
	minutes_tens = (buf[3] >> 4) & 0x07;
	minutes_units = buf[3] & 0x0f;
	hours_tens = (buf[4] >> 4) & 0x03;
	hours_units = buf[4] & 0x0f;

	printf("1st timecode for %s = %02d:%02d:%02d.%02d (df = %d)\n", file,
					(hours_tens * 10 + hours_units),
					(minutes_tens * 10 + minutes_units),
					(seconds_tens * 10 + seconds_units),
					frame_tens * 10 + frame_units,
					drop_frame_flag);

	if (drop_frame_flag)
	{
		// FIXME: We round here instead of following SMPTE 12M-1999 sect 4.2.2
		// "To minimize the NTSC time error, the first two frame numbers
		// (00 and 01) shall be omitted from the count at the start of each
		// minute except minutes 00, 10, 20, 30, 40, and 50."
		double rate = 30000/1001;
		double tmp =
					frame_tens * 10 + frame_units +
					(seconds_tens * 10 + seconds_units) * rate +
					(minutes_tens * 10 + minutes_units) * rate*60 +
					(hours_tens * 10 + hours_units) * rate*60*60;
		frames = (int)(tmp + 0.5);		// round it
	}
	else
	{
		frames =	frame_tens * 10 + frame_units +
					(seconds_tens * 10 + seconds_units) * 25 +
					(minutes_tens * 10 + minutes_units) * 25*60 +
					(hours_tens * 10 + hours_units) * 25*60*60;
	}
	fclose(fp);
	return frames;
}

// Given a filepath to a DV file, return a WCHAR NetLoc string
// Returned wchar_t* needs to be freed
static wchar_t* getNetLocPath(const char *filepath, const char *audio_track)
{
	// RFC 1738 and RFC 1630 specify URIs and the file URL scheme
	// TODO: So, we should insert the full path after file:///
	// TODO: Should we have an option for MS drives? e.g. file:///C%3A/
	char c_str[FILENAME_MAX] = "file:///";
	strcat(c_str, filepath);

	// Treat referenced audio slots as if they were once external WAV files
	if (audio_track)
	{
		char *p = strrchr(c_str, '.');
		strcpy(p, audio_track);		// typically "A1.wav" or "A2.wav"
	}

	// Now convert to wchar
	size_t length = strlen(c_str);
	wchar_t* path = new wchar_t[length + 1];
	size_t status = mbstowcs(path, c_str, length + 1);
	return path;
}


// TODO: These should be command-line arguments
static bool create_tapemob = true;
static bool create_compmob = true;
static bool add_audio = true;

// Records format detected when parsing eli file
static bool formatPAL = false;

static aafRational_t PALrate = {25,1};
static aafRational_t NTSCrate = {30000,1001};
//static aafRational_t NTSCrate = {2997,100};


// Macro to make AxLib declaration look clearer.
// Relies on an AxDictionary dict being declared in the same scope.
#define DECLARE_AX(type, name) \
	Ax##type name( AxQueryInterface<IUnknown, IAAF##type>(dict.CreateInstance(AUID_AAF##type, IID_IAAF##type)) );


// addMasterMobForDVFile()
// Given a filepath to a DV file, this function adds to the Master Mob mmob
// three Source Mobs (Video, Audio1, Audio2), and adds them to the specified
// Header.  Legacy properties are added to dict to enable import into
// Avid Xpress DV 3.0.
//
// If p_tapemob is NULL, no tapemob is added.
// At present this function is hard-coded to expect PAL DV frames with two
// channels of 48 kHz audio.
// A limitation of the present AAF Specification creates the need to extract
// the audio samples from each DV frame and add them to a separate essence
// stream in WAV format.  A future revision of the Specification should avoid
// this problem.
static bool addMasterMobForDVFile(
					string filename,
					AxHeader &head, AxDictionary &dict,
					AxSourceMob *p_tapemob, AxMasterMob &mmob)
{
	// For Large File Support (>2GB) we must use the awkward stat() and
	// _stati64() calls below.  C++ standard library ifstream seekg and tellg
	// aren't 64bit capable using GCC 3.3 and MSVC 7 (GCC 3.4 fixes this).
#ifdef _MSC_VER
	struct _stati64 statbuf;
	_stati64(filename.c_str(), &statbuf);
#else
	struct stat statbuf;
	stat(filename.c_str(), &statbuf);
#endif
	aafLength_t eslength = statbuf.st_size;

	// Truncate frame count to ignore any partial frame at the end
	aafLength_t cliplength = eslength /
					( formatPAL ? DV_PAL_FRAME_SIZE : DV_NTSC_FRAME_SIZE );

	aafSourceRef_t srcref;		// tmp SourceRef used for multiple SetSourceReference()'s
	srcref.sourceID = zerouid;
	srcref.sourceSlotID = 0;
	srcref.startTime = 0;

	string name = stripDirAndExt(filename.c_str());
	mmob.SetName( AxStringUtil::mbtowc(name.c_str()) );

	IUnknownSP unk;

	// Establish PAL / NTSC versions of editrate
	aafRational_t editrate;
	if (formatPAL)
		editrate = PALrate;
	else
		editrate = NTSCrate;
	aafRational_t samplerate = editrate;

	// TimelineMobSlot for the Master Mob
	DECLARE_AX( TimelineMobSlot, tlslot );
	tlslot.SetEditRate(editrate);
	tlslot.SetSlotID(1);
	tlslot.SetPhysicalNum(1);
	tlslot.SetName(L"V1");

	// TimelineMobSlot for the source Mob
	DECLARE_AX( TimelineMobSlot, srctlslot );
	srctlslot.SetEditRate(editrate);
	srctlslot.SetSlotID(1);

	// SourceMob
	DECLARE_AX( SourceMob, smob );

	// Create the source clip for the Video File Source Mob
	DECLARE_AX( SourceClip, srcclip );
	srcclip.SetDataDef(dict.LookupDataDef(DDEF_Picture));
	srcclip.SetLength(cliplength);
	srcref.sourceID = zerouid;
	if (p_tapemob)
		srcref.sourceID = p_tapemob->GetMobID();
	srcref.sourceSlotID = 1;
	aafPosition_t start_timecode = getFirstTimecodeFromDVFile(filename.c_str());
	if (start_timecode == -1)
		srcref.startTime = 0;
	else
		srcref.startTime = start_timecode;
	srcclip.SetSourceReference(srcref);

	// Create the sourceclip for the Master Mob
	DECLARE_AX( SourceClip, msrcclip );
	msrcclip.SetDataDef(dict.LookupDataDef(DDEF_Picture));
	msrcclip.SetLength(cliplength);
	srcref.sourceID = smob.GetMobID();
	srcref.sourceSlotID = 1;
	srcref.startTime = 0;
	msrcclip.SetSourceReference(srcref);

	// Create CDCIDescriptor
	DECLARE_AX( CDCIDescriptor, cdci );
	cdci.SetFrameLayout(kAAFMixedFields);
	cdci.SetComponentWidth(8);
	if (formatPAL)
	{
		cdci.SetDisplayView(0x120,0x2d0,0,0);
		cdci.SetSampledView(0x120,0x2d0,0,0);
		cdci.SetStoredView(0x120,0x2d0);
		cdci.SetHorizontalSubsampling(2);	// 4:2:0 chroma format
		cdci.SetVerticalSubsampling(2);
		aafInt32 lines[2] = {0x17, 0x14f};
		cdci.SetVideoLineMap(2,lines);
	}
	else
	{
		cdci.SetDisplayView(0xf0,0x2d0,0,0);
		cdci.SetSampledView(0xf0,0x2d0,0,0);
		cdci.SetStoredView(0xf0,0x2d0);
		cdci.SetHorizontalSubsampling(4);	// 4:1:1 chroma format
		cdci.SetVerticalSubsampling(1);
		aafInt32 lines[2] = {0x17, 0x11d};
		cdci.SetVideoLineMap(2,lines);
	}
	cdci.SetBlackReferenceLevel(0x10);
	cdci.SetWhiteReferenceLevel(0xeb);
	cdci.SetColorRange(0xe1);
	aafRational_t aspect = {4,3};
	cdci.SetImageAspectRatio(aspect);
	cdci.SetContainerFormat(dict.LookupContainerDef(writeNetworkLocator ? ContainerFile : ContainerAAF));
	cdci.SetSampleRate(samplerate);
	cdci.SetLength(cliplength);
	cdci.SetCodecDef(dict.LookupCodecDef(kAAFCodecCDCI));
	aafUID_t aviddvcompression = { 0xedb35390, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
	cdci.SetCompression(aviddvcompression);
	AxClassDef cdciclass (dict.LookupClassDef(AUID_AAFCDCIDescriptor));
	AxClassDef didclass (dict.LookupClassDef(AUID_AAFDigitalImageDescriptor));

#define SET_PROPERTY_VALUE( name, type, value ) \
	{ \
		AxPropertyDef prop(cdciclass.LookupPropertyDef(kAAFPropID_##name)); \
		AxTypeDef valtype (prop.GetTypeDef()); \
		IAAFTypeDefInt * inttypep; \
		CHECK_HRESULT(static_cast<IUnknownSP>(valtype)->QueryInterface(IID_IAAFTypeDefInt, reinterpret_cast<void**>(&inttypep))); \
		IAAFTypeDefIntSP inttypesp; \
		*&inttypesp = inttypep; \
		AxTypeDefInt inttype(inttypesp); \
		type number = value; \
		AxPropertyValue val(inttype.CreateValue(reinterpret_cast<aafMemPtr_t>(&number), sizeof(number))); \
		CHECK_HRESULT(static_cast<IAAFObjectSP>(cdci)->SetPropertyValue( static_cast<IAAFPropertyDefSP>(prop), static_cast<IAAFPropertyValueSP>(val))); \
	}

	SET_PROPERTY_VALUE( CDCIOffsetToFrameIndexes, aafInt32, 0 );
	SET_PROPERTY_VALUE( DIDFrameIndexByteOrder, aafUInt16, 0x4949 );
	SET_PROPERTY_VALUE( DIDFirstFrameOffset, aafInt32, 0x0 );
	SET_PROPERTY_VALUE( DIDImageSize, aafInt64, eslength );
	if (formatPAL)
	{
		SET_PROPERTY_VALUE( DIDResolutionID, aafUInt32, 0x8d );
		SET_PROPERTY_VALUE( DIDFrameSampleSize, aafInt32, DV_PAL_FRAME_SIZE );
	}
	else
	{
		SET_PROPERTY_VALUE( DIDResolutionID, aafUInt32, 0x8c );
		SET_PROPERTY_VALUE( DIDFrameSampleSize, aafInt32, DV_NTSC_FRAME_SIZE );
	}

	// Order may be important from here.
	// Mobs must have slots before they are added to the header.
	// Slots must have a segment before they are added to mobs.
	// The source mob must have an essencedescriptor before it is added to the header.
	smob.SetEssenceDescriptor(cdci);
	if (writeNetworkLocator)
	{
		IAAFEssenceDescriptorSP mdes = smob.GetEssenceDescriptor();
		DECLARE_AX( NetworkLocator, netLoc );
		netLoc.Initialize();

		wchar_t* path = getNetLocPath(filename.c_str(), NULL);
		netLoc.SetPath(AxString(path));
		mdes->AppendLocator( static_cast<IAAFLocatorSP>(netLoc) );
		delete path;
	}
	tlslot.SetSegment(msrcclip);
	srctlslot.SetSegment(srcclip);
	mmob.AppendSlot(tlslot);
	smob.AppendSlot(srctlslot);
	head.AddMob(mmob);
	head.AddMob(smob);

	FILE *fp;
	if (!writeNetworkLocator)
	{
	IAAFEssenceDataSP essdata;
	unk = dict.CreateInstance(AUID_AAFEssenceData, IID_IAAFEssenceData);
	if(!AxIsA(unk, essdata))
	{
		throw AxEx(L"Bad essence data object.");
	}
	// The File mob must have been added to the file already. before it is
	// linked to the essence data.
	CHECK_HRESULT(essdata->SetFileMob(static_cast<IAAFSourceMobSP>(smob)));

	// You must add the essence data to the file before you write to the
	// essence data.
	CHECK_HRESULT(static_cast<IAAFHeaderSP>(head)->AddEssenceData(essdata));

	// Create the essence data and write to it.
	fp = fopen(filename.c_str(), "rb");
	if (!fp)
	{
		cerr << "Error Opening File " << filename << endl;
		return false;
	}

	aafInt64 total_written = 0;
	do
	{
		unsigned char buffer[8192];
		aafUInt32 byteswritten = 0;

		size_t bytesread = fread(buffer, 1, sizeof(buffer), fp);

		if (bytesread == 0)
			break;

		CHECK_HRESULT(essdata->Write(bytesread, buffer, &byteswritten));
		total_written += byteswritten;

		if (feof(fp))
		{
			break;
		}
	} while (true);
	fclose(fp);

	cout << filename << ": " << ostream_int64(total_written) << " bytes written for CDCI Essence" << endl;
	}

	/////////////////////////////////////////////////////////////////
	// Audio channel 1

	// TimelineMobSlot for channel 1 audio
	DECLARE_AX( TimelineMobSlot, tlslotA1 );
	tlslotA1.SetEditRate(editrate);
	tlslotA1.SetSlotID(2);
	tlslotA1.SetPhysicalNum(1);
	tlslotA1.SetName(L"A1");

	// TimelineMobSlot for the source Mob
	DECLARE_AX( TimelineMobSlot, srctlslotA1 );
	srctlslotA1.SetEditRate(editrate);
	srctlslotA1.SetSlotID(1);

	DECLARE_AX( SourceMob, spfmobA1 );

	// Create the source clip for the Source Mob
	DECLARE_AX( SourceClip, srcclipA1 );
	srcclipA1.SetDataDef(dict.LookupDataDef(DDEF_Sound));

	aafSourceRef_t srcrefA1;
	srcrefA1.sourceID = zerouid;
	srcrefA1.sourceSlotID = 0;
	if (p_tapemob)
	{
		srcrefA1.sourceID = p_tapemob->GetMobID();
		srcrefA1.sourceSlotID = 2;
	}
	srcrefA1.startTime = 0;
	srcclipA1.SetSourceReference(srcrefA1);

	DECLARE_AX( SourceMob, smobA1 );

	// Create the sourceclip for the Master Mob
	DECLARE_AX( SourceClip, msrcclipA1 );
	msrcclipA1.SetDataDef(dict.LookupDataDef(DDEF_Sound));
	srcrefA1.sourceID = smobA1.GetMobID();
	srcrefA1.sourceSlotID = 1;
	srcrefA1.startTime = 0;
	msrcclipA1.SetSourceReference(srcrefA1);

	// Create WaveDescriptor
	DECLARE_AX( WAVEDescriptor, waveA1 );
	waveA1.SetContainerFormat(dict.LookupContainerDef(writeNetworkLocator ? ContainerFile : ContainerAAF));
	waveA1.SetCodecDef(dict.LookupCodecDef(kAAFCodecWAVE));

	aafRational_t wav_samplerate = {48000, 1};		// 48kHz hard-coded in this version
	waveA1.SetSampleRate(wav_samplerate);

	smobA1.SetEssenceDescriptor(waveA1);
	if (writeNetworkLocator)
	{
		IAAFEssenceDescriptorSP mdesA1 = smobA1.GetEssenceDescriptor();
		DECLARE_AX( NetworkLocator, netLocA1 );
		netLocA1.Initialize();

		wchar_t* path = getNetLocPath(filename.c_str(), "A1.wav");
		netLocA1.SetPath(AxString(path));
		mdesA1->AppendLocator( static_cast<IAAFLocatorSP>(netLocA1) );
		delete path;
	}
	tlslotA1.SetSegment(msrcclipA1);
	srctlslotA1.SetSegment(srcclipA1);
	smobA1.AppendSlot(srctlslotA1);
	if (add_audio)
	{
		mmob.AppendSlot(tlslotA1);
		head.AddMob(smobA1);
	}

	/////////////////////////////////////////////////////////////////
	// Audio channel 2

	// TimelineMobSlot for channel 2 audio
	DECLARE_AX( TimelineMobSlot, tlslotA2 );
	tlslotA2.SetEditRate(editrate);
	tlslotA2.SetSlotID(3);
	tlslotA2.SetPhysicalNum(2);
	tlslotA2.SetName(L"A2");

	// TimelineMobSlot for the source Mob
	DECLARE_AX( TimelineMobSlot, srctlslotA2 );
	srctlslotA2.SetEditRate(editrate);
	srctlslotA2.SetSlotID(1);

	DECLARE_AX( SourceMob, smobA2 );

	// Create the source clip for the Source Mob
	DECLARE_AX( SourceClip, srcclipA2 );
	srcclipA2.SetDataDef(dict.LookupDataDef(DDEF_Sound));
	aafSourceRef_t srcrefA2;
	srcrefA2.sourceID = zerouid;
	srcrefA1.sourceSlotID = 0;
	if (p_tapemob)
	{
		srcrefA2.sourceID = p_tapemob->GetMobID();
		srcrefA2.sourceSlotID = 3;
	}
	srcrefA2.startTime = 0;
	srcclipA2.SetSourceReference(srcrefA2);

	// Create the sourceclip for the Master Mob
	DECLARE_AX( SourceClip, msrcclipA2 );
	msrcclipA2.SetDataDef(dict.LookupDataDef(DDEF_Sound));
	srcrefA2.sourceID = smobA2.GetMobID();
	srcrefA2.sourceSlotID = 1;
	srcrefA2.startTime = 0;
	msrcclipA2.SetSourceReference(srcrefA2);

	// Create WaveDescriptor
	DECLARE_AX( WAVEDescriptor, waveA2 );
	waveA2.SetContainerFormat(dict.LookupContainerDef(writeNetworkLocator ? ContainerFile : ContainerAAF));
	waveA2.SetCodecDef(dict.LookupCodecDef(kAAFCodecWAVE));
	waveA2.SetSampleRate(wav_samplerate);

	smobA2.SetEssenceDescriptor(waveA2);
	if (writeNetworkLocator)
	{
		IAAFEssenceDescriptorSP mdesA2 = smobA2.GetEssenceDescriptor();
		DECLARE_AX( NetworkLocator, netLocA2 );
		netLocA2.Initialize();

		wchar_t* path = getNetLocPath(filename.c_str(), "A2.wav");
		netLocA2.SetPath(AxString(path));
		mdesA2->AppendLocator( static_cast<IAAFLocatorSP>(netLocA2) );
		delete path;
	}
	tlslotA2.SetSegment(msrcclipA2);
	srctlslotA2.SetSegment(srcclipA2);
	smobA2.AppendSlot(srctlslotA2);
	if (add_audio)
	{
		mmob.AppendSlot(tlslotA2);
		head.AddMob(smobA2);
	}

	if (add_audio)
	{
#ifdef _MSC_VER
		struct _stati64 statbuf;
		_stati64(filename.c_str(), &statbuf);
#else
		struct stat statbuf;
		stat(filename.c_str(), &statbuf);
#endif

		// (hard-coded to 48kHz)
		// 48000 samples per second at 2 bytes per sample
		// so 1920 samples per frame
		// so 3840 bytes per frame.
		aafLength_t num_samples = (statbuf.st_size / DV_PAL_FRAME_SIZE) * 1920;
		waveA1.SetLength(num_samples);
		waveA2.SetLength(num_samples);

		// These lengths are in editrate
		// (lengths are always in units of the containing slot)
		aafInt64 length_in_er = num_samples / 1920;
		srcclipA1.SetLength(length_in_er);
		msrcclipA1.SetLength(length_in_er);
		srcclipA2.SetLength(length_in_er);
		msrcclipA2.SetLength(length_in_er);

		CreateWaveHeader(WAVEHeader);
		WAVEDataLen = num_samples * 2;
		WAVEFileLen = WAVEDataLen + 36;

		waveA1.SetSummary(sizeof(WAVEHeader), WAVEHeader);
		waveA2.SetSummary(sizeof(WAVEHeader), WAVEHeader);

		if (!writeNetworkLocator)
		{
			IAAFEssenceDataSP wavA1data;
			IAAFEssenceDataSP wavA2data;
			unk = dict.CreateInstance(AUID_AAFEssenceData, IID_IAAFEssenceData);
			if(!AxIsA(unk, wavA1data))
			{
				throw AxEx(L"Bad wavA1data data object.");
			}
			CHECK_HRESULT(wavA1data->SetFileMob(static_cast<IAAFSourceMobSP>(smobA1)));

			// You must add the essence data to the file before you write to the essence data.
			CHECK_HRESULT(static_cast<IAAFHeaderSP>(head)->AddEssenceData(wavA1data));
			unk = dict.CreateInstance(AUID_AAFEssenceData, IID_IAAFEssenceData);
			if(!AxIsA(unk, wavA2data))
			{
				throw AxEx(L"Bad wavA2data data object.");
			}
			CHECK_HRESULT(wavA2data->SetFileMob(static_cast<IAAFSourceMobSP>(smobA2)));

			// You must add the essence data to the file before you write to the essence data.
			CHECK_HRESULT(static_cast<IAAFHeaderSP>(head)->AddEssenceData(wavA2data));
			// Audio sample extraction from DV stream
			// FIXME: Audio sample extraction only works for PAL at the moment.
			short audio_buffers[2][DV_AUDIO_SAMPLES * sizeof(short)];

			aafUInt32 byteswritten = 0;
			for (int nchan = 0; nchan < 2; nchan++)
			{
				IAAFEssenceDataSP wavAdata;
				if (nchan == 0)
					wavAdata = wavA1data;
				else
					wavAdata = wavA2data;
				CHECK_HRESULT(wavAdata->Write(sizeof(WAVEHeader), WAVEHeader, &byteswritten));

				// Create the essence wav data and write to it for both A1 and A2
				fp = fopen(filename.c_str(), "rb");
				if (!fp)
				{
					cerr << "Error Opening File " << filename << endl;
					return false;
				}
				aafLength_t total_A = 0;
				for (aafLength_t i = 0; i < statbuf.st_size; i += DV_PAL_FRAME_SIZE)
				{
					unsigned char buf[DV_PAL_FRAME_SIZE];
					int r;
					if ((r = fread(buf, 1, sizeof(buf), fp)) != sizeof(buf))
					{
						cout << filename << " short read of DV frame (expected " << sizeof(buf) << ") read " << r << endl;
						break;
					}

					// There are 12 DIF sequences, each storing audio data, in PAL 625/50
					// Each DIF sequence is 12000 bytes long and has 9 DIF blocks containing
					// audio data which are not arranged contiguously [SMPTE 314M-1999]
					for (int dif_seq = 0; dif_seq < 12; dif_seq++)
					{
						// Audio data for one channel is scattered over 54 DIF blocks for
						// the 625/50 system (6 DIF sequences * 9 DIF blocks)
						for (int audio_dif = 0; audio_dif < 9; audio_dif++)
						{
							// channel 0 is in first 6 DIF sequences, channel 1 in 2nd 6
							int channel = (dif_seq < 6) ? 0 : 1;
							if (channel != nchan)
								continue;
							// skip first 6 DIF blocks (header, subcode, VAUX sections)
							// audio blocks occur every 16 DIF blocks after this point
							// each DIF block is 80 bytes in size
							int offset = (dif_seq * 150 + 6 + audio_dif * 16) * 80;
							deshuffle_audio_block(buf + offset, dif_seq, audio_dif,
													audio_buffers[channel] );
						}
					}
					aafDataValue_t dataA = (unsigned char *)audio_buffers[nchan];

					CHECK_HRESULT(wavAdata->Write(DV_AUDIO_SAMPLES*sizeof(short),
										dataA, &byteswritten));
					total_A += byteswritten;
				}
				fclose(fp);
				cout << filename << ": " << ostream_int64(total_A) << " bytes written for A" << nchan + 1 << " WAVE Essence" << endl;
			}
		}
	}
	return true;
}

static bool createAAFFileForEditDecisions(const char *output_aaf_file,
	vector<string> dvfile_by_idx,
	vector<int> start_by_idx,
	vector<int> end_by_idx)
{
	bool result = true;
	IAAFFile *pFile;

	try
	{
		{
			aafProductIdentification_t  ProductInfo;

			// Create a new file...
			aafProductVersion_t v;
			v.major = 0;
			v.minor = 6;
			v.tertiary = 4;
			v.patchLevel = 1;
			v.type = kAAFVersionReleased;

			ProductInfo.companyName = L"This property intentionally left blank";
			ProductInfo.productName = L"Kino";
			ProductInfo.productVersion = &v;
			ProductInfo.productVersionString = L"0.6.4 BBC R&D patch 1";
			ProductInfo.platform = L"i386-redhat-linux";		// has no effect

			// Remove existing file if any
			if (remove(output_aaf_file) == 0)
				cout << output_aaf_file << ": removed" << endl;

			// If file still exists, exit with error
			struct stat statbuf;
			if (stat(output_aaf_file, &statbuf) == 0)
			{
				cerr << output_aaf_file << " still exists - could not remove" << endl;
				return false;
			}

			// Create the new AAF file
			if (!useRawStorage)
			{
				int modeFlags = 0;
				if (useLargeSectors)
					modeFlags |= AAF_FILE_MODE_USE_LARGE_SS_SECTORS;

				CHECK_HRESULT(AAFFileOpenNewModify(
					AxStringUtil::mbtowc( output_aaf_file ).c_str(), modeFlags, &ProductInfo, &pFile));
			}
			else
			{
				// RawStorage with MS SS known to fail for 4k (works with 512)
				const aafUID_t* pFileKind;
				if (useLargeSectors)
					pFileKind = &aafFileKindAaf4KBinary; 
				else
					pFileKind = &aafFileKindAafSSBinary;

				IAAFRawStorage* pRawStorage = 0;
				CHECK_HRESULT (AAFCreateRawStorageDisk(
					AxStringUtil::mbtowc( output_aaf_file ).c_str(),
					kAAFFileExistence_new,
					kAAFFileAccess_modify,
					&pRawStorage));
				CHECK_HRESULT (AAFCreateAAFFileOnRawStorage (
					pRawStorage,
					kAAFFileExistence_new,
					kAAFFileAccess_modify,
					pFileKind,
					0,
					&ProductInfo,
					&pFile));
					pRawStorage->Release();

					CHECK_HRESULT(pFile->Open());
			}
		}

		IAAFHeaderSP headsp;
		CHECK_HRESULT(pFile->GetHeader(&headsp));
		AxHeader head(headsp);
		AxDictionary dict(head.GetDictionary());

		CreateLegacyPropDefs(static_cast<IAAFDictionarySP>(dict));

		// Establish PAL / NTSC versions of editrate
		int length_24hours;
		aafRational_t editrate;

		if (formatPAL)
		{
			editrate = PALrate;
			length_24hours = 24*60*60*25;
		}
		else
		{
			editrate = NTSCrate;
			length_24hours = (int)(24*60*60*(30000.0/1001));
		}

		aafSourceRef_t srcref;
		srcref.sourceID = zerouid;
		srcref.sourceSlotID = 0;
		srcref.startTime = 0;

		// Create Tape Mob
		DECLARE_AX( SourceMob, tapemob );

		// Variable scope to keep Tape variables tidy.
		{
			DECLARE_AX( TapeDescriptor, tapedes );
			// TODO: replace hard-coded tape SourceMob name with command arg.
			tapemob.SetName(L"DCAE");
			tapemob.SetEssenceDescriptor(tapedes);
			if (formatPAL)
				tapedes.SetSignalType(kAAFPALSignal);
			else
				tapedes.SetSignalType(kAAFNTSCSignal);

			DECLARE_AX( TimelineMobSlot, tapeslotvid );
			tapeslotvid.SetSlotID(1);
			tapeslotvid.SetEditRate(editrate);
			tapeslotvid.SetPhysicalNum(1);
			tapeslotvid.SetName(L"V1");
			DECLARE_AX( Filler, tapefiller );
			// Video track of Filler for 24 hours
			tapefiller.Initialize(dict.LookupDataDef(DDEF_Picture), length_24hours);

			DECLARE_AX( TimelineMobSlot, tapeslotaud1 );
			tapeslotaud1.SetSlotID(2);
			tapeslotaud1.SetEditRate(editrate);
			tapeslotaud1.SetPhysicalNum(1);
			tapeslotaud1.SetName(L"A1");
			DECLARE_AX( Filler, tapefiller_aud1 );
			// Audio track 1 of Filler for 24 hours
			tapefiller_aud1.Initialize(dict.LookupDataDef(DDEF_Sound), length_24hours);

			DECLARE_AX( TimelineMobSlot, tapeslotaud2 );
			tapeslotaud2.SetSlotID(3);
			tapeslotaud2.SetEditRate(editrate);
			tapeslotaud2.SetPhysicalNum(2);
			tapeslotaud2.SetName(L"A2");
			DECLARE_AX( Filler, tapefiller_aud2 );
			// Audio track 2 of Filler for 24 hours
			tapefiller_aud2.Initialize(dict.LookupDataDef(DDEF_Sound), length_24hours);

			DECLARE_AX( TimelineMobSlot, tapeslottc );
			tapeslottc.SetSlotID(4);
			tapeslottc.SetEditRate(editrate);
			tapeslottc.SetPhysicalNum(1);
			tapeslottc.SetName(L"TC1");

			DECLARE_AX( Timecode, tcseg );
			aafTimecode_t tc;
			if (formatPAL)
			{
				tc.drop = kAAFFalse;
				tc.fps = editrate.numerator/editrate.denominator;
			}
			else
			{
				tc.drop = kAAFTrue;
				tc.fps = 30;
			}
			tc.startFrame = 0;						// Start time of 0:00:00.00
			tcseg.Initialize(length_24hours, tc);		// 24 hours
			tcseg.SetDataDef(dict.LookupDataDef(DDEF_Timecode));

			tapeslotvid.SetSegment(tapefiller);
			tapemob.AppendSlot(tapeslotvid);
			if (add_audio)
			{
				tapeslotaud1.SetSegment(tapefiller_aud1);
				tapemob.AppendSlot(tapeslotaud1);
				tapeslotaud2.SetSegment(tapefiller_aud2);
				tapemob.AppendSlot(tapeslotaud2);
			}
			tapeslottc.SetSegment(tcseg);
			tapemob.AppendSlot(tapeslottc);
			if (create_tapemob)
			{
				cout << "Adding Tape Mob" << endl;
				head.AddMob(tapemob);
			}
		}

		// Create Codec Defininitions and register them in the dictionary.
		IAAFCodecDefSP cdciCodec;
		IUnknownSP unk0 = dict.CreateInstance(AUID_AAFCodecDef, IID_IAAFCodecDef);
		if(!AxIsA(unk0, cdciCodec))
		{
			throw AxEx(L"Bad codec def!!!");
		}
		CHECK_HRESULT(cdciCodec->Initialize(kAAFCodecCDCI, L"CDCI" ,L"CDCI video codec."));
		CHECK_HRESULT(cdciCodec->AddEssenceKind(dict.LookupDataDef(DDEF_Picture)));
		IAAFCDCIDescriptorSP spcdci;
		CHECK_HRESULT(cdciCodec->SetFileDescriptorClass(dict.LookupClassDef(AxAUID(&*spcdci))));

		IAAFCodecDefSP wavCodec;
		IUnknownSP unk = dict.CreateInstance(AUID_AAFCodecDef, IID_IAAFCodecDef);
		if(!AxIsA(unk, wavCodec))
		{
			throw AxEx(L"Bad codec def!!!");
		}
		CHECK_HRESULT(wavCodec->Initialize(kAAFCodecWAVE, L"Wave" ,L"Wave audio codec."));
		CHECK_HRESULT(wavCodec->AddEssenceKind(dict.LookupDataDef(DDEF_Sound)));
		IAAFWAVEDescriptorSP spwav;
		CHECK_HRESULT(wavCodec->SetFileDescriptorClass(dict.LookupClassDef(AxAUID(&*spwav))));

		dict.RegisterCodecDef(cdciCodec);
		dict.RegisterCodecDef(wavCodec);

		// Create a unique set of DV files
		// This set will be used to create one MasterMob + SourceClip + EssenceData
		// for each unique DV file
		map<string, aafMobID_t> mobID_by_dvfile;
		size_t i;
		for (i = 0; i < dvfile_by_idx.size(); i++)
		{
			mobID_by_dvfile[ dvfile_by_idx[i] ] = zerouid;
		}

		// For each clip, create a Master Mob (with Source Mob and Essence Data)
		// and record the generated MMobID
		map<string, aafMobID_t>::iterator pos;
		for (pos = mobID_by_dvfile.begin(); pos != mobID_by_dvfile.end(); pos++)
		{
			cout << "Adding Master Mob for " << pos->first << endl;

			DECLARE_AX( MasterMob, mmob );

			// Add MasterMob for DV file, plus necessary SourceMobs and Essence data
			if (create_tapemob)
				addMasterMobForDVFile(pos->first, head, dict, &tapemob, mmob);
			else
				addMasterMobForDVFile(pos->first, head, dict, NULL, mmob);

			mobID_by_dvfile[ pos->first ] = mmob.GetMobID();
		}

		// Create a Composition Mob and give it a name (derived from output filename)
		DECLARE_AX( CompositionMob, cmob );
		string compositionName = stripDirAndExt(output_aaf_file);
		cmob.SetName( AxStringUtil::mbtowc(compositionName.c_str()) );

		// TimelineMobSlots for TC1 V1 A1 A2
		DECLARE_AX( TimelineMobSlot, comp_tcslot );
		comp_tcslot.SetEditRate(editrate);
		comp_tcslot.SetSlotID(1);
		comp_tcslot.SetPhysicalNum(1);
		comp_tcslot.SetName(L"TC1");

		DECLARE_AX( TimelineMobSlot, comp_tlslot_V1 );
		comp_tlslot_V1.SetEditRate(editrate);
		comp_tlslot_V1.SetSlotID(2);
		comp_tlslot_V1.SetPhysicalNum(1);
		comp_tlslot_V1.SetName(L"V1");

		DECLARE_AX( TimelineMobSlot, ctlslotA1 );
		ctlslotA1.SetEditRate(editrate);
		ctlslotA1.SetSlotID(3);
		ctlslotA1.SetPhysicalNum(1);
		ctlslotA1.SetName(L"A1");

		DECLARE_AX( TimelineMobSlot, ctlslotA2 );
		ctlslotA2.SetEditRate(editrate);
		ctlslotA2.SetSlotID(4);
		ctlslotA2.SetPhysicalNum(2);
		ctlslotA2.SetName(L"A2");

		// Sequence segments for Composition

		DECLARE_AX( Sequence, sequenceV1 );
		sequenceV1.SetDataDef(dict.LookupDataDef(DDEF_Picture));
		comp_tlslot_V1.SetSegment(sequenceV1);

		DECLARE_AX( Sequence, sequenceA1 );
		sequenceA1.SetDataDef(dict.LookupDataDef(DDEF_Sound));
		ctlslotA1.SetSegment(sequenceA1);

		DECLARE_AX( Sequence, sequenceA2 );
		sequenceA2.SetDataDef(dict.LookupDataDef(DDEF_Sound));
		ctlslotA2.SetSegment(sequenceA2);

		// Loop over the clips, appending SourceClips to the sequence
		int comp_length = 0;
		for (i = 0; i < start_by_idx.size(); i++)
		{
			// In ELI format, the end index is last included frame,
			// so add 1 when calculating the length
			int length = end_by_idx[i] + 1 - start_by_idx[i];
			comp_length += length;

			// Create the source clip for the Composition Mob
			DECLARE_AX( SourceClip, comsrcclipV1 );
			comsrcclipV1.SetDataDef(dict.LookupDataDef(DDEF_Picture));
			comsrcclipV1.SetLength(length);
			srcref.sourceID = mobID_by_dvfile[ dvfile_by_idx[ i ] ];
			srcref.sourceSlotID = 1;
			srcref.startTime = start_by_idx[i];
			comsrcclipV1.SetSourceReference(srcref);

			sequenceV1.AppendComponent(comsrcclipV1);

			// A1
			DECLARE_AX( SourceClip, comsrcclipA1 );
			comsrcclipA1.SetDataDef(dict.LookupDataDef(DDEF_Sound));
			comsrcclipA1.SetLength(length);
			srcref.sourceSlotID = 2;
			comsrcclipA1.SetSourceReference(srcref);

			sequenceA1.AppendComponent(comsrcclipA1);

			// A2
			DECLARE_AX( SourceClip, comsrcclipA2 );
			comsrcclipA2.SetDataDef(dict.LookupDataDef(DDEF_Sound));
			comsrcclipA2.SetLength(length);
			srcref.sourceSlotID = 3;
			comsrcclipA2.SetSourceReference(srcref);

			sequenceA2.AppendComponent(comsrcclipA2);
		}

		// Setup timecode segment
		DECLARE_AX( Timecode, tcseg );
		aafTimecode_t tc;
		if (formatPAL)
		{
			tc.drop = kAAFFalse;
			tc.fps = editrate.numerator/editrate.denominator;
		}
		else
		{
			tc.drop = kAAFTrue;
			tc.fps = 30;
		}
		// TODO: make the composition start timecode a command arg.
		tc.startFrame = 0;							// Start time of 0:00:00.00
		tcseg.Initialize(comp_length, tc);
		tcseg.SetDataDef(dict.LookupDataDef(DDEF_Timecode));
		comp_tcslot.SetSegment(tcseg);

		cmob.AppendSlot(comp_tcslot);
		cmob.AppendSlot(comp_tlslot_V1);
		if (add_audio)
		{
			cmob.AppendSlot(ctlslotA1);
			cmob.AppendSlot(ctlslotA2);
		}
		if (create_compmob)
		{
			cout << "Adding Composition Mob for " << start_by_idx.size() << " Clips" << endl;
			head.AddMob(cmob);
		}
	}
	catch(AxEx & ex)
	{
		printf("%ls",ex.what());
		result = false;
	}
	try
	{
		CHECK_HRESULT(pFile->Save());
		CHECK_HRESULT(pFile->Close());
		CHECK_HRESULT(pFile->Release());
	}
	catch(AxEx & ex)
	{
		printf("Problem closing file.  Error: %ls", ex.what());
		return false;
	}
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "Usage: " << argv[0] << " [-netloc] [-useraw] [-largeSectors] infile outfile [essencedir]" << endl;
		return 1;
	}

	int i = 1;		// argv index
	while (true)
	{
		if (!strcmp(argv[i], "-netloc"))
		{
			// writeNetworkLocator flag is used to alternate between embedded essence
			// with ContainerAAF property and no embedded essence with ContainerFile property
			writeNetworkLocator = true;
			i++;
		}
		else if (!strcmp(argv[i], "-useraw"))
		{
			//create AAF file on raw storage
			useRawStorage = true;
			i++;
		}
		else if (!strcmp(argv[i], "-largeSectors"))
		{
			//create AAF file using 4k sectors
			useLargeSectors = true;
			i++;
		}
		else
			break;
	}
	const char *edit_list = argv[i++];
	const char *output_aaf_file = argv[i++];
	const char *essence_src_dir = NULL;
	if (argc >= 4)
	{
		int idxOffset = 3;

		if (writeNetworkLocator)
			idxOffset++;

		if (useRawStorage)
			idxOffset++;

		if (useLargeSectors)
			idxOffset++;

		if (idxOffset <= (argc - 1))
			essence_src_dir = argv[idxOffset];
	}

	FILE *edlfp = fopen(edit_list, "rb");
	if (!edlfp)
	{
		cerr << "Error Opening File " << edit_list << endl;
		return 1;
	}

	// Read Edit list and store in appropriate variables
	vector<string> dvfile_by_idx;
	vector<int> start_by_idx;
	vector<int> end_by_idx;
	char str[1024];

	fscanf(edlfp, "%[^\n]\n", str);		// skip first line
	fscanf(edlfp, "%[^\n]\n", str);		// PAL / NTSC specifier
	if (strcmp(str, "PAL") == 0)
	{
		formatPAL = true;
		add_audio = true;
	}
	else
	{
		formatPAL = false;
		add_audio = false;
	}
	int num_clips = 0;
	fscanf(edlfp, "%d\n", &num_clips);	// read total number of clips (not DV files)

	cout << "*** Start ELI ***" << endl;
	// read DV files and add to a map
	for (i = 0; i < num_clips; i++)
	{
		fscanf(edlfp, "%[^\n]\n", str);

		// look in essence_src_dir for DV files if specified
		string filename(str);
		if (essence_src_dir)
		{
			// Remove any path information for filename and
			// replace with essence_src_dir path
			string::size_type strpos = filename.find_last_of("\\/");
			if (strpos != string::npos)		// If found a slash
				filename = essence_src_dir + filename.substr(strpos);
			else
			{
				string slash("/");
				filename = essence_src_dir + slash + filename;
			}
		}

		dvfile_by_idx.push_back(filename);
		cout << "num_clips = " << num_clips << "  str=" << str << "  filename=" << filename << endl;
	}

	// read clip start and end points
	for (i = 0; i < num_clips; i++)
	{
		int index, start, end;
		fscanf(edlfp, "%d %d %d\n", &index, &start, &end);
		start_by_idx.push_back(start);
		end_by_idx.push_back(end);
		printf("index %d  start %d  end %d\n", index, start, end);
	}
	cout << "*** End ELI ***" << endl;

	if (createAAFFileForEditDecisions(output_aaf_file, dvfile_by_idx, start_by_idx, end_by_idx))
		return 0;
	else
		return 1;
}
