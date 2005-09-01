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

#include "AAF.h"					// AAF interface declarations
#include "AAFResult.h"				// AAF error codes
#include "AAFCompressionDefs.h"		// Compression IDs

static bool verbose = false;

static void LogError(HRESULT errcode, int line, char *file)
{
	fprintf(stderr, "Error '%0x' returned at line %d in %s\n", errcode, line, file);
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

#define aaf_assert(b, msg) \
	if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

static void convert(char* cName, size_t length, const wchar_t* name)
{
	aaf_assert((name), "non-NULL input name");
	aaf_assert(cName != 0, "Valid output buffer");
	aaf_assert(length > 0, "Valid output buffer size");

	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		fprintf(stderr, ": Error : Conversion failed.\n\n");
		exit(1);
	}
}

static void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}

static bool operator == (const aafUID_t a, const aafUID_t b)
{
	return memcmp(&a, &b, sizeof(a)) == 0;
}

static void
extensionByDescriptor(IAAFSourceMob *pSourceMob, char *ext, size_t size)
{
	const aafUID_t Legacy_MPEG2 = { 0xedb35388, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
	IAAFEssenceDescriptor *pDesc = NULL;
	IAAFCDCIDescriptor* pCDCIDesc = NULL;
	IAAFRGBADescriptor* pRGBADesc = NULL;
	IAAFWAVEDescriptor* pWAVEDesc = NULL;
	IAAFAIFCDescriptor* pAIFCDesc = NULL;
	IAAFTIFFDescriptor* pTIFFDesc = NULL;
	IAAFSoundDescriptor* pSoundDesc = NULL;
	IAAFPCMDescriptor* pPCMDesc = NULL;

	check(pSourceMob->GetEssenceDescriptor(&pDesc));

	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFCDCIDescriptor,
			(void **)&pCDCIDesc)))
	{
		IAAFDigitalImageDescriptor *pDID = NULL;
		check(pCDCIDesc->QueryInterface(IID_IAAFDigitalImageDescriptor,
			(void **)&pDID));

		aafUID_t compression = {0}, null_id = {0};
		if (SUCCEEDED(pDID->GetCompression(&compression)))
		{
			if (compression == kAAFCompressionDef_LegacyDV ||
				compression == kAAFCompressionDef_IEC_DV_625_50 ||
				compression == kAAFCompressionDef_IEC_DV_525_60)
			{
				strncpy(ext, "dv", size);
				return;
			}
			if (compression == Legacy_MPEG2)
			{
				strncpy(ext, "m2v", size);
				return;
			}
			if (compression == null_id)
			{
				strncpy(ext, "unc.cdci", size);
				return;
			}
			strncpy(ext, "cmpr.cdci", size);
			return;
		}
		strncpy(ext, "unc.cdci", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFRGBADescriptor,
			(void **)&pRGBADesc)))
	{
		strncpy(ext, "rgba", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFWAVEDescriptor,
			(void **)&pWAVEDesc)))
	{
		strncpy(ext, "wav", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFAIFCDescriptor,
			(void **)&pAIFCDesc)))
	{
		strncpy(ext, "aif", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFTIFFDescriptor,
			(void **)&pTIFFDesc)))
	{
		strncpy(ext, "tif", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFSoundDescriptor,
			(void **)&pSoundDesc)))
	{
		strncpy(ext, "pcm", size);
		return;
	}
	if (SUCCEEDED(pDesc->QueryInterface(IID_IAAFPCMDescriptor,
			(void **)&pPCMDesc)))
	{
		strncpy(ext, "pcm", size);
		return;
	}

	strncpy(ext, "unknown", size);
	return;
}

static int DumpEssence(const char *file, bool mobIDfilename)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	aafWChar					pwFileName[FILENAME_MAX*4];
	IEnumAAFEssenceData *pEnumEssenceData = NULL;
	IAAFEssenceData *pEssenceData = NULL;

	if ((mbstowcs(pwFileName, file, sizeof(pwFileName))) == (size_t)(-1))
	{
		fprintf(stderr, "%s: mbstowcs conversion failed\n", file);
		return 1;
	}

	// Open an AAF file
	check(AAFFileOpenExistingRead(pwFileName, 0, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	// Check whether there are any EssenceData objects
	aafUInt32	ess_objects = 0;
	check(pHeader->CountEssenceData(&ess_objects));
	if (! ess_objects)
	{
		fprintf(stderr, "No EssenceData objects found\n");
		return 0;
	}

	// For each EssenceData object get its associated File SourceMob
	check(pHeader->EnumEssenceData(&pEnumEssenceData));
	while (pEnumEssenceData->NextOne(&pEssenceData) == AAFRESULT_SUCCESS)
	{
		aafMobID_t				mobID;
		IAAFMob*				pMob = NULL;
		aafWChar				namebuf[1024] = L"";
		char					mobName[1024], mobIDstr[1024];
		char					*name = "Unknown";
		int						kcount = 0;
		aafNumSlots_t			numSlots;
		IAAFSourceMob			*pSourceMob;

		check(pEssenceData->GetFileMob(&pSourceMob));
		check(pSourceMob->QueryInterface(IID_IAAFMob,(void **) &pMob));

		// Get MobID and (optional) Name property
		check(pMob->GetMobID(&mobID));
		pMob->GetName(namebuf, sizeof(namebuf));		// Name is optional
		convert(mobName, sizeof(mobName), namebuf);
		MobIDtoString(mobID, mobIDstr);
		if (verbose)
			printf("mobID=%s\n", mobIDstr);


		IEnumAAFMobSlots* pMobSlotIter = NULL;

		// Get the number of slots
		check(pMob->CountSlots(&numSlots));
		
		// Iterating through all Mob Slots to determine a user-friendly name
		check(pMob->GetSlots(&pMobSlotIter));
		while(pMobSlotIter->NextOne(&pMobSlot) == AAFRESULT_SUCCESS)
		{
			IAAFDataDef		*pDataDef = NULL;
			IAAFDataDef3	*pDataDef3 = NULL;
	
			aafUInt32 MobSlotID;
			check(pMobSlot->GetSlotID(&MobSlotID));
			if (verbose)
				printf("    SlotID=%d", MobSlotID);

			check(pMobSlot->GetDataDef(&pDataDef));
			check(pDataDef->QueryInterface(IID_IAAFDataDef3,(void **) &pDataDef3));

			// Determine what DataDef kind the TimelineMobSlot is
			aafBool bKind = kAAFFalse;
			check(pDataDef3->IsPictureKind(&bKind));
			if (bKind) { static int c = 0; name = "Picture"; kcount = c++; }
			check(pDataDef3->IsSoundKind(&bKind));
			if (bKind) { static int c = 0; name = "Sound"; kcount = c++; }
			check(pDataDef3->IsTimecodeKind(&bKind));
			if (bKind) { static int c = 0; name = "Timecode"; kcount = c++; }
			check(pDataDef3->IsEdgecodeKind(&bKind));
			if (bKind) { static int c = 0; name = "Edgecode"; kcount = c++; }
			check(pDataDef3->IsDescriptiveMetadataKind(&bKind));
			if (bKind) { static int c = 0; name = "DescriptiveMetadata"; kcount = c++; }

			if (verbose)
				printf("    Kind=%s", name);

			// TODO: Search through all MasterMobs for the MasterMob which
			// references this slot in this SourceMob.  MasterMobs more often
			// have a useful Name property which we could use as part of
			// the filename.

			pMobSlot->Release();
			pMobSlot = NULL;
		}
		if (verbose)
		{
			if (*mobName)
				printf("  Name=\"%s\"", mobName);
			printf("\n");
		}

		// Get the EssenceDescriptor (required for SourceMob) and look for
		// common types of essence to determine the filename extension.
		char ext[FILENAME_MAX];
		extensionByDescriptor(pSourceMob, ext, sizeof(ext));

		aafLength_t essenceSize = 0;
		check(pEssenceData->GetSize(&essenceSize));

		FILE *fp = NULL;
		char rawfile[FILENAME_MAX];

		if (mobIDfilename)
			sprintf(rawfile, "%s.%s", mobIDstr, ext);
		else
			sprintf(rawfile, "%s_%d.%s", name, kcount, ext);

		if ((fp = fopen(rawfile, "wb")) == NULL)
		{
			perror(rawfile);
			break;
		}

		if (verbose)
			printf("      %s", rawfile);
		else
			printf("%s\n", rawfile);

		fflush(stdout);
		while (true)
		{
			aafUInt8 buf[128*1024];
			aafUInt32 bytesRead = 0;

			HRESULT hr = pEssenceData->Read(sizeof(buf), buf, &bytesRead);
			if (hr == AAFRESULT_END_OF_DATA)	// empty stream is OK
				break;
			check(hr);

			if (fwrite(buf, bytesRead, 1, fp) != 1)
			{
				perror(rawfile);
				break;
			}

			if (bytesRead != sizeof(buf))
				break;
		}
		fclose(fp);
		if (verbose)
			printf(" written, size=%"AAFFMT64"d\n\n", essenceSize);

		pSourceMob->Release();
		pSourceMob = NULL;
		
		pEssenceData->Release();
		pEssenceData = NULL;

		pMobSlotIter->Release();
		pMobSlotIter = NULL;
	
		pMob->Release();
		pMob = NULL;
	}
	pEnumEssenceData->Release();
	pEnumEssenceData = NULL;
	
	if (moduleErrorTmp != 0)
		fprintf(stderr, "ERROR: 0x%08X\n", moduleErrorTmp);
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
	cout << "Usage : " << progname << " aaf_filename" << endl;
	cout << endl;
	cout << "\tDumps all EssenceData streams found in AAF file." << endl;
	cout << "\tFiles are named after their DataDef and EssenceDescriptor properties." << endl;
	cout << endl;
	cout << "\t-mobid    Use SourceMob MobID for filename" << endl;
	cout << "\t-v        print detailed information on extracted EssenceData streams." << endl;
	cout << endl;
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;
	const char *input_aaf = NULL;
	bool useMobIDfilename = false;

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
			else if (!strcmp(argv[i], "-v"))
			{
				verbose = true;
				i++;
			}
			else if (!strcmp(argv[i], "-mobid"))
			{
				useMobIDfilename = true;
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
		input_aaf = argv[i];
	}
	else
	{
		printUsage(argv[0]);
		return 1;
	}

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	return DumpEssence(input_aaf, useMobIDfilename);
}
