//
// $Id$ $Name$
//
// aafembed.cpp -	Edits AAF file in place adding essence for each FileMob
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
#include "AAFClassDefUIDs.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

// Guess the toolkit version (found by experiment)
#if defined(__IAAFImportDescriptor_INTERFACE_DEFINED__)
        // V1.1 or greater toolkit
#else
#define AAF_TOOLKIT_V1_0
#define kAAFCodecDef_CDCI kAAFCodecCDCI
#define kAAFCodecDef_WAVE kAAFCodecWAVE
#if defined(_MSC_VER)
#define AAFFMT64 "I64"
#else
#define AAFFMT64 "ll"
#endif
#endif

#ifdef _MSC_VER
extern "C" {
#include "avilib.h"
}
#else
#include "avilib.h"
#endif

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

static char *EssenceDir = NULL;
static bool embed_essence = true;

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

/*
 *	Routines to support the URL and URI utility functions.
 */

// These routines rely upon URLs as defined in RFC 1738:
//
// <scheme>:<scheme-specific-part>
// ; the scheme is in lower case; interpreters should use case-ignore
//	scheme		= 1*[ lowalpha | digit | "+" | "-" | "." ]
//
// For file scheme:
//	fileurl		= "file://" [ host | "localhost" ] "/" fpath
//
// NB. ':' is acceptable unescaped in the fpath component

#ifdef _MSC_VER			// MS VC++ dosen't provide POSIX strncasecmp
#define strncasecmp(s1, s2, n) strnicmp(s1, s2, n)
#else
#include <strings.h>	// strncasecmp()
#endif

static char asciiHexToChar (char c)
{
    return  c >= '0' && c <= '9' ?  c - '0' 
    	    : c >= 'A' && c <= 'F'? c - 'A' + 10
    	    : c - 'a' + 10;	/* accept lowercase letters too */
}

static void unescapeURI(char *str)
{
    char *p = str;
    char *q = str;

    while (*p)
	{
        if (*p == '%')		// URI hex escape char
		{
			p++;
			if (*p)
				*q = asciiHexToChar(*p++) * 16;
			if (*p)
				*q = *q + asciiHexToChar(*p);
			p++;
			q++;
		}
		else
		    *q++ = *p++; 
    }
    *q++ = 0;
}

/************************
 * Function: wcsconvertURLtoFilepath
 *
 *	Converts a file scheme URL into an absolute filepath.  An invalid URL is
 *	returned unmodified.
 *
 * Argument Notes:
 *		filepath must have space allocated by the caller
 *
 * ReturnValue:
 *		void
 *
 * Possible Errors:
 *		none
 */
void wcsconvertURLtoFilepath(wchar_t *url, wchar_t *filepath)
{
	// Convert to char* for ease of processing.
	// (wcsncasecmp and similiar are not available everywhere)
	//unsigned tlen = wcslen(url);
	unsigned tlen = (unsigned) wcslen(url);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstombs(tmp, url, tlen+1);

	// If no file scheme is found, assume a simple filepath and not a URI.
	// Note that other schemes such as http and ftp are not supported.
	if (strncasecmp(tmp, "file://", strlen("file://")) != 0 &&
		strncasecmp(tmp, "file%3A//", strlen("file%3A//")) != 0)
	{
		wcscpy(filepath, url);
		delete [] tmp;
		return;
	}

	// Skip over the file://[host]/ to point to the fpath.
	char *fpath = tmp + strlen("file://");
	if (strncasecmp(tmp, "file%3A//", strlen("file%3A//")) == 0)
		fpath = tmp + strlen("file%3A//");
	while (*fpath && *fpath != '/')
		fpath++;

#ifdef _WIN32
	// WIN32 filepaths must start with a drive letter, so remove the
	// initial '/' from the URL.
	if (*fpath == '/')
		fpath++;
#endif

	unescapeURI(fpath);

	mbstowcs(filepath, fpath, strlen(fpath)+1);		// convert back to wcs
	delete [] tmp;
}

// Edit url wchar string in place, fixing incorrect and unnecessary
// escaping of colon in URLs. E.g.
//   file%3A///C%3A/boat.avi  ->  file:///C:/boat.avi
static void fixURL(aafWChar *url)
{
	// Convert to char* for ease of processing.
	unsigned tlen = (unsigned) wcslen(url);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstombs(tmp, url, tlen+1);

	char *p;
	while (	(p = strstr(tmp, "%3A")) != NULL ||
			(p = strstr(tmp, "%3a")) != NULL)
	{
		char *q = p + 3;
		*p++ = ':';
		while (*q)
			*p++ = *q++;
		*p = '\0';
	}
	mbstowcs(url, tmp, strlen(tmp)+1);		// convert back to wcs
	delete [] tmp;
}

static void storeUInt16_LE(aafUInt8 *p, aafUInt16 value)
{
	p[0] = (value & 0x00ff) >> 0;
	p[1] = (value & 0xff00) >> 8;
}

static void storeUInt32_LE(aafUInt8 *p, aafUInt32 value)
{
	p[0] = (aafUInt8)((value & 0x000000ff) >> 0);
	p[1] = (aafUInt8)((value & 0x0000ff00) >> 8);
	p[2] = (aafUInt8)((value & 0x00ff0000) >> 16);
	p[3] = (aafUInt8)((value & 0xff000000) >> 24);
}

static HRESULT addEssenceDataForAVI(
		IAAFEssenceData *pEssenceData,
		char			*avifile,
		aafBool			isPicture,
		int				track_num,
		aafUInt8		*summary,
		aafLength_t		*pLength,
		int				*pRate)
{
	// Using a large array on the stack will cause "Stack overflow"
	// errors under the MSVC compiler.  E.g.
	//  aafUInt8	buf[1024 * 768 * 3];	// always fails
	//  aafUInt8	buf[1024 * 1024];		// fails sometimes
	aafUInt8	*buf = new aafUInt8[1024 * 768 * 3];
	aafUInt32	bytesWritten = 0;
	int			i, frames;

	// AVI processing
	avi_t *avi = NULL;
	avi = AVI_open_input_file(avifile, 1);		// 1 means use index
	if (avi == NULL)
	{
		fprintf(stderr, "Could not open AVI file %s: ", avifile);
		perror("");
		return E_FAIL;
	}

	if (isPicture)
	{
		printf("  width=%d\n  height=%d\n  frames=%ld\n  fps=%f\n  codec=\"%s\"\n",
            AVI_video_width(avi), AVI_video_height(avi),
            AVI_video_frames(avi), AVI_frame_rate(avi),
            AVI_video_compressor(avi));

		frames = AVI_video_frames(avi);
		aafLength_t totalBytes = 0;
		for (i = 0; i < frames; i++)
		{
			int key = 0;
			int nread = AVI_read_frame(avi, (char *)buf, &key);
			if (nread < 0) return E_FAIL;

			if (nread == 0)
				break;

			if (pEssenceData->Write(nread, buf, &bytesWritten) != S_OK)
				return E_FAIL;
			totalBytes += bytesWritten;
		}
		printf("  total bytes written=%"AAFFMT64"d\n", totalBytes);
	}
	else		// isSound
	{
		int bits_per_sample = AVI_audio_bits(avi);
		int channels = AVI_audio_channels(avi);
		int rate = AVI_audio_rate(avi);
		int block_align = (1 * bits_per_sample) / 8;	// hard-coded to mono
		int avgBPS = (rate * bits_per_sample) / 8;
		int bytes_per_sample = (bits_per_sample * channels) / 8;
		int stream_length = AVI_audio_bytes(avi);

		// Build up the RIFF header and data header
		// (together these make up the Summary property)
		aafUInt8 riff_fmt[12 + 24] = "RIFF....WAVEfmt ";
		aafUInt8 data_hdr[8] = "data";

		int datasize = stream_length / channels;
		int riffsize = datasize + 44;

		// fmt chunk
		storeUInt16_LE(&riff_fmt[4], riffsize);				// size of riff chunk (filesize - 8)
		storeUInt16_LE(&riff_fmt[12+4], 16);				// size is always 16
		storeUInt16_LE(&riff_fmt[12+8], 0x0001);			// wFormatTag WAVE_FORMAT_PCM
		storeUInt16_LE(&riff_fmt[12+10], 1);				// nchannels (hard-coded to mono)
		storeUInt32_LE(&riff_fmt[12+12], rate);				// nSamplesPerSec
		storeUInt32_LE(&riff_fmt[12+16], avgBPS);			// nAvgBytesPerSec
		storeUInt16_LE(&riff_fmt[12+20], block_align);		// nBlockAlign
		storeUInt16_LE(&riff_fmt[12+22], bits_per_sample);	// nBitsPerSample
		// data chunk
		storeUInt32_LE(&data_hdr[4], datasize);				// size of data (not including data_hdr)

		// Write WAVE header both to EssenceData and Summary property
		if (pEssenceData->Write(sizeof(riff_fmt), riff_fmt, &bytesWritten) != S_OK)
			return E_FAIL;
		if (pEssenceData->Write(sizeof(data_hdr), data_hdr, &bytesWritten) != S_OK)
			return E_FAIL;

		memcpy(summary, riff_fmt, sizeof(riff_fmt));
		memcpy(summary+sizeof(riff_fmt), data_hdr, sizeof(data_hdr));

		// Write Length & Rate properties
		*pLength = datasize / (bits_per_sample / 8);
		*pRate = rate;

		printf("  Input audio:\n  channels=%d\n  bits=%d\n  rate=%ld\n  format=%d\n  total bytes in stream=%ld\n",
                AVI_audio_channels(avi), AVI_audio_bits(avi),
                AVI_audio_rate(avi), AVI_audio_format(avi),
                AVI_audio_bytes(avi));

		int nread = 0;
		while (true)
		{
			nread = AVI_read_audio(avi, (char *)buf, bytes_per_sample);
			if (nread < 0) return E_FAIL;
			if (nread == 0)
				break;

			if (track_num == 1)
			{
				if (pEssenceData->Write(block_align, buf, &bytesWritten) != S_OK)
					return E_FAIL;
			}
			else // track 2
			{
				if (pEssenceData->Write(block_align, buf+block_align, &bytesWritten) != S_OK)
					return E_FAIL;
			}
		}
	}
	AVI_close(avi);
	delete [] buf;

	return S_OK;
}

static aafBoolean_t hasEssenceForFileMob(IAAFHeader *pHeader, IAAFMob *pFileMob)
{
	IAAFContentStorage	*pStorage = NULL;

	check(pHeader->GetContentStorage(&pStorage));

	aafMobID_t mobid;
	check(pFileMob->GetMobID(&mobid));

	aafBoolean_t present = false;
	check(pStorage->IsEssenceDataPresent(mobid, kAAFEssence, &present));

	return present;
}

static HRESULT RelinkAAFFile(aafWChar * pFileName)
{
	IAAFFile					*pFile = NULL;
	IAAFHeader					*pHeader = NULL;
	IAAFDictionary				*pDictionary = NULL;

	aafProductVersion_t productVersion;
	productVersion.major = 0;
	productVersion.minor = 1;
	productVersion.tertiary = 0;
	productVersion.patchLevel = 0;
	productVersion.type = kAAFVersionUnknown;

	aafProductIdentification_t productInfo;
	productInfo.companyName = (aafCharacter*)L"BBC R&D";
	productInfo.productName = (aafCharacter*)L"Relink AVI Essence";
	productInfo.productVersion = &productVersion;
	productInfo.productVersionString = (aafCharacter*)L"0.1.0.0";
	productInfo.platform = (aafCharacter*)L"GNU/Hurd";


	aafUInt32 modeFlags = 0;
	check(AAFFileOpenExistingModify(pFileName, modeFlags, &productInfo, &pFile));

	check(pFile->GetHeader(&pHeader));
	check(pHeader->GetDictionary(&pDictionary));

	IAAFMob			*pFileMob = NULL;
	IEnumAAFMobs	*pFileMobIter = NULL;
	aafSearchCrit_t				criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFFileMob;		// Search by File Mob

	check(pHeader->GetMobs(&criteria, &pFileMobIter));

	// For each FileMob ...
	while (AAFRESULT_SUCCESS == pFileMobIter->NextOne(&pFileMob))
	{
		IAAFSourceMob			*pSourceMob = NULL;
		IAAFEssenceDescriptor	*pEdesc = NULL;
		IAAFAIFCDescriptor		*pAIFCDesc = NULL;
		IAAFCDCIDescriptor		*pCDCIDesc = NULL;
		IAAFFileDescriptor		*pSoundFileDesc = NULL;
		IAAFWAVEDescriptor		*pWAVEDesc = NULL;
		bool					isAIFC = false, isCDCI = false;

		// Check for AIFC or CDCI descriptors
		check(pFileMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		check(pSourceMob->GetEssenceDescriptor(&pEdesc));

		// Get the Locator
		IEnumAAFLocators	*pEnum = NULL;
		IAAFLocator			*pLocator = NULL;
		check(pEdesc->GetLocators(&pEnum));
		if ( AAFRESULT_FAILED( pEnum->NextOne(&pLocator) ) )
		{
			printf("FileMob has no locator - skipped\n");
			continue;
		}

		if (SUCCEEDED(pEdesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc)))
		{
			isAIFC = true;
		}
		else if (SUCCEEDED(pEdesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc)))
		{
			isCDCI = true;
		}
		else
		{
			printf("FileMob Essence Descriptor not CDCI or AIFC - skipped\n");
			continue;
		}

		// Read the contents of the Locator
		aafUInt32			readLen = 0;
		check(pLocator->GetPathBufLen(&readLen));
		aafWChar *URLbuf = new aafWChar[readLen];
		check(pLocator->GetPath(URLbuf, readLen));

		// Fix the URL if needed
		fixURL(URLbuf);
		check(pLocator->SetPath(URLbuf));

		// Skip FileMob if no embedding requested or
		// FileMob already has embedded essence.
		if (! embed_essence || hasEssenceForFileMob(pHeader, pFileMob))
		{
			pEdesc->Release();
			pSourceMob->Release();
			continue;
		}

		// Convert URL to filepath
		aafWChar filepath[FILENAME_MAX];
		wcsconvertURLtoFilepath(URLbuf, filepath);

		// Make sure there exists a slot with a SourceClip of SoundKind or PictureKind
		IEnumAAFMobSlots	*slotIter = NULL;
		IAAFMobSlot			*slot = NULL;
		IAAFClassDef		*classDef = NULL;
		IAAFDataDef			*pDataDef = NULL;
		aafUInt32			phys_num = 0;
		check(pFileMob->GetSlots(&slotIter));
		check(slotIter->NextOne(&slot));
		check(slot->GetDataDef(&pDataDef));
		aafBool isPicture = false;
		aafBool isSound = false;
		pDataDef->IsPictureKind(&isPicture);
		pDataDef->IsSoundKind(&isSound);
		check(slot->GetPhysicalNum(&phys_num));

		// Replace the AIFCDescriptor with WAVEDescriptor
		IAAFEssenceDescriptor	*pNewAudioEdesc = NULL;
		if (isAIFC)
		{
			check(pDictionary->LookupClassDef(AUID_AAFWAVEDescriptor, &classDef));
			check(classDef->CreateInstance(IID_IAAFFileDescriptor, (IUnknown **)&pSoundFileDesc));
			classDef->Release();
			check(pSoundFileDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc));
			check(pSoundFileDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pNewAudioEdesc));
			check(pSourceMob->SetEssenceDescriptor(pNewAudioEdesc));

			printf("FileMob == AIFCDescriptor - replacing with WAVEDescriptor\n");
		}
		if (isCDCI)
		{
			printf("FileMob == CDCIDescriptor - fixing\n");
		}

		// Create and add EssenceData object
		IAAFEssenceData *pEssenceData;
		check(pDictionary->LookupClassDef(AUID_AAFEssenceData, &classDef));
		check(classDef->CreateInstance(IID_IAAFEssenceData, (IUnknown **)&pEssenceData));
		classDef->Release();

		check(pEssenceData->SetFileMob(pSourceMob));
		check(pHeader->AddEssenceData(pEssenceData));

		aafUInt8	summaryBuf[44] = {0};
		aafLength_t	len = 0;
		int			rate = 0;

		// filepath processing
		char avifile[FILENAME_MAX];
		convert(avifile, sizeof(avifile), filepath);
		if (EssenceDir)
		{
			// Find the essence files in specified EssenceDir
			// strip filepath and build up new path using EssenceDir
			char basename[FILENAME_MAX];
			strncpy(basename, avifile, sizeof(basename));
			char *pb = strrchr(basename, '/');
			if (*(pb+1))
				pb++;
			strncpy(avifile, EssenceDir, sizeof(avifile));
			avifile[sizeof(avifile)-1] = '\0';
			strncat(avifile, "/", sizeof(avifile));
			avifile[sizeof(avifile)-1] = '\0';
			strncat(avifile, pb, sizeof(avifile));
			avifile[sizeof(avifile)-1] = '\0';
		}
		printf("  Using path=%s\n", avifile);

		check(addEssenceDataForAVI(
				pEssenceData,	// to call Write()
				avifile,
				isPicture,
				phys_num,		// Track number for audio mob
				summaryBuf,
				&len,
				&rate
				));
		pEssenceData->Release();

		if (isCDCI)
		{
			IAAFFileDescriptor				*pFileDesc = NULL;
			IAAFDigitalImageDescriptor		*pDIDDesc = NULL;
			check(pEdesc->QueryInterface(IID_IAAFFileDescriptor, (void **)&pFileDesc));
			check(pEdesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDIDDesc));

			aafRational_t					rate = {0,0};
			check(pFileDesc->GetSampleRate(&rate));
			bool sourcePAL = (rate.numerator == 25) ? true : false;

			aafUID_t aviddvcompression = { 0xedb35390, 0x6d30, 0x11d3,
									{ 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
			pDIDDesc->SetCompression(aviddvcompression);
			pDIDDesc->SetFrameLayout(kAAFSeparateFields);

			// Set the CodecDef property so that OpenEssence will work
			IAAFCodecDef		*codecDef = NULL;
			if (pDictionary->LookupCodecDef(kAAFCodecDef_CDCI, &codecDef) != AAFRESULT_SUCCESS)
			{
				// Need to register missing CodecDef
				printf("Need to register missing CDCI CodecDef\n");
			}
			else
			{
				pFileDesc->SetCodecDef(codecDef);
				codecDef->Release();
				//codecDef->ReleaseReference();
			}

			// Set PAL / NTSC properties
			if (sourcePAL)
			{
				aafInt32	linemap[2] = {0x17, 0x14f};
				pDIDDesc->SetVideoLineMap(2, linemap);
				pCDCIDesc->SetHorizontalSubsampling(2);		// 4:2:0
				pCDCIDesc->SetVerticalSubsampling(2);
			}
			else
			{
				aafInt32	linemap[2] = {0x17, 0x11d};
				pDIDDesc->SetVideoLineMap(2, linemap);
				pCDCIDesc->SetHorizontalSubsampling(4);		// 4:1:1
				pCDCIDesc->SetVerticalSubsampling(1);
			}
			printf("  editrate=%d/%d\n", rate.numerator, rate.denominator);

			pDIDDesc->Release();
			pFileDesc->Release();
		}
		if (isAIFC)
		{
			// Set the CodecDef property so that OpenEssence will work
			IAAFCodecDef		*codecDef = NULL;
			if (pDictionary->LookupCodecDef(kAAFCodecDef_WAVE, &codecDef) != AAFRESULT_SUCCESS)
			{
				// Need to register missing CodecDef
				IAAFDataDef *pDefSound = NULL;

				check(pDictionary->LookupClassDef(AUID_AAFCodecDefinition, &classDef));
				check(classDef->CreateInstance(IID_IAAFCodecDef, (IUnknown **)&codecDef));
				classDef->Release();
				check(codecDef->Initialize(kAAFCodecDef_WAVE, L"Wave", L"Wave audio codec."));
#ifdef AAF_TOOLKIT_V1_0
				check(pDictionary->LookupDataDef(DDEF_Sound, &pDefSound));
				check(codecDef->AddEssenceKind(pDefSound));
#else
				check(pDictionary->LookupDataDef(kAAFDataDef_Sound, &pDefSound));
				check(codecDef->AddEssenceKind(pDefSound));
#endif
				pDefSound->Release();
				check(pDictionary->RegisterCodecDef(codecDef));
				check(pDictionary->LookupClassDef(kAAFClassID_WAVEDescriptor, &classDef));
				check(codecDef->SetFileDescriptorClass(classDef));
				classDef->Release();
			}
			pSoundFileDesc->SetCodecDef(codecDef);
			codecDef->Release();

			// Add network locator
			IAAFNetworkLocator	*pNetLocator = NULL;
			IAAFLocator			*pLocator = NULL;
			check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator, &classDef));
			check(classDef->CreateInstance(IID_IAAFNetworkLocator, (IUnknown **)&pNetLocator));
			classDef->Release();
			check(pNetLocator->QueryInterface(IID_IAAFLocator, (void **)&pLocator));
			check(pLocator->SetPath(URLbuf));
			check(pNewAudioEdesc->AppendLocator(pLocator));

			// Set audio parameters and Summary
			aafRational_t wav_samplerate = {rate, 1};
			check(pSoundFileDesc->SetSampleRate(wav_samplerate));
			check(pSoundFileDesc->SetLength(len));
			check(pWAVEDesc->SetSummary(sizeof(summaryBuf), summaryBuf));
			pWAVEDesc->Release();
			pSoundFileDesc->Release();
			pNewAudioEdesc->Release();
		}
		printf("\n");

		if (pCDCIDesc) pCDCIDesc->Release();
		if (pAIFCDesc) pAIFCDesc->Release();

		pEdesc->Release();
		pSourceMob->Release();
	} // For each FileMob ...

	check(pFile->Save());
	check(pFile->Close());

	pDictionary->Release();
	pHeader->Release();
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

void printUsage(const char *progname)
{
	fprintf(stderr, "Usage:\n\t%s [options] file_to_fix.aaf [more_files_to_fix.aaf]\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "\t-d   Directory to search for referenced essence files\n");
	fprintf(stderr, "\t     (leading directory components are first removed from URL)\n");
	fprintf(stderr, "\t-n   No essence will be embedded but fixes will be made\n");
}

int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar 		pwFileName[FILENAME_MAX];

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
			else if (!strcmp(argv[i], "-d"))
			{
				EssenceDir = argv[i+1];
				i += 2;
			}
			else if (!strcmp(argv[i], "-n"))
			{
				embed_essence = false;
				i++;
			}
			else
			{
				break;
			}
		}
	}

	if (i >= argc)
	{
		printUsage(argv[0]);
		return 1;
	}

	// loop over
	for (int idx = i; idx < argc; idx++)
	{
		if ((mbstowcs(pwFileName, argv[i], sizeof(pwFileName))) == (size_t)(-1))
		{
			fprintf(stderr, "%s: mbstowcs conversion failed\n", argv[i]);
			return 1;
		}

		checkFatal(RelinkAAFFile(pwFileName));
	}

	return 0;
}
