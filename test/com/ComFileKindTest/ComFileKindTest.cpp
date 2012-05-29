// @doc INTERNAL
// @com This file implements tests for various file kinds
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
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream>
#include <iomanip>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#ifndef _MSC_VER
#include <unistd.h>		// POSIX pathconf()
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"

static void RemoveTestFile(const aafWChar* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1) {
    remove(cFileName);
  }
}

inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

static const   aafMobID_t  TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafProductIdentification_t TestProductID;
static aafProductVersion_t TestVersion = { 1, 1, 0, 0, kAAFVersionUnknown };

static bool deleteFiles = false;

// List of File APIs used for reading and writing
typedef enum _api_t
{
	APIunknown,
	FileOpenNewModify,
	FileOpenNewModifyEx,
	FileOpenExistingRead,
	FileOpenExistingModify,
	RawStorage
} api_t;

typedef struct _api_info_t
{
	api_t api;
	char name[16];
} api_info_t;

api_info_t write_api[] = {
		{ FileOpenNewModify, "NM" },
		{ FileOpenNewModifyEx, "Ex" },
		{ RawStorage, "RS" },
		{ APIunknown, ""} };

api_info_t read_api[] = {
		{ FileOpenExistingRead, "ER" },
		{ FileOpenExistingModify, "EM" },
		{ RawStorage, "RS" },
		{ APIunknown, ""} };

// List of file kinds to test
typedef struct _kind_info_t
{
	const aafUID_t* kind;
	char name[16];
} kind_info_t;

kind_info_t filekind[] = {
	{ &kAAFFileKind_Aaf4KBinary, "4K" },
	{ &kAAFFileKind_Aaf512Binary, "512" },
	{ &kAAFFileKind_AafS4KBinary, "S4K" },
	{ &kAAFFileKind_AafS512Binary, "S512" },
	{ &kAAFFileKind_AafM4KBinary, "M4K" },
	{ &kAAFFileKind_AafM512Binary, "M512" },
	{ &kAAFFileKind_AafG4KBinary, "G4K" },
	{ &kAAFFileKind_AafG512Binary, "G512" },
	{ NULL, ""} };

// OpenNewModify has a filemode argument to exercise
typedef struct _modeflag_info_t
{
	int flag;
	const aafUID_t* kind;
	char name[16];
} modeflag_info_t;

// Specify the file mode flag and the compatible RawStorage kind
modeflag_info_t modeNewModify[] = {
	{ AAF_FILE_MODE_USE_LARGE_SS_SECTORS, &kAAFFileKind_Aaf4KBinary, "4K" },
	{ 0, &kAAFFileKind_Aaf512Binary,  "512" },
	{ -1, NULL, ""} };


static HRESULT WriteAAFFile(IAAFFile* pFile)
{
  IAAFHeader* pHeader = NULL;
  IAAFDictionary* pDictionary = NULL;
  IAAFMob *pMob = NULL;
  HRESULT hr = S_OK;

  try 
  {

    // Get the header
    checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary
    checkResult(pHeader->GetDictionary(&pDictionary));

    // Create a Mob
	IAAFClassDef *classDef = NULL;
    checkResult(pDictionary->LookupClassDef(AUID_AAFMasterMob, &classDef));
    checkResult(classDef->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
	classDef->Release();

    // Initialize the Mob
    checkResult(pMob->SetMobID(TEST_MobID));
    checkResult(pMob->SetName(MOB_NAME_TEST));

    // Add the mob to the file
    checkResult(pHeader->AddMob(pMob));

    // Save the file
    checkResult(pFile->Save());

    // Close the file
    checkResult(pFile->Close());

    // Release the AAF file's resources
	// (especially important for files written using RawStorage and FileOpenNewModifyEx) 
    checkResult(pFile->Release());
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
	cout << "*** WriteAAFFile: caught error hr=0x" << hex << hr << dec << endl;
  }

  // Clean up
  if (pMob)
    pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();

  return hr;
}

static HRESULT ReadAAFContents(IAAFFile *pFile)
{
	IAAFHeader* pHeader = NULL;
	IEnumAAFMobs* mobIter = NULL;
	IAAFMob* pMob = NULL;
	aafNumSlots_t numMobs, n;
	aafWChar name[500];
	aafMobID_t mobID;
	aafFileRev_t testRev;
	HRESULT hr = S_OK;

	try
	{
		// Get the header
		checkResult(pFile->GetHeader(&pHeader));

		// Expect to find a single Mob
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			checkResult(mobIter->NextOne (&pMob));
			checkResult(pMob->GetName (name, sizeof(name)));
			checkResult(pMob->GetMobID (&mobID));
			// Check that the properties are as we wrote them
			checkExpression(wcscmp( name, MOB_NAME_TEST) == 0,
                      AAFRESULT_TEST_FAILED);
			checkExpression(memcmp(&mobID, &TEST_MobID, sizeof(mobID)) == 0,
                      AAFRESULT_TEST_FAILED);

			pMob->Release();
			pMob = NULL;
    	}

		mobIter->Release();
		mobIter = NULL;

		checkResult(pFile->GetRevision(&testRev));
		checkExpression(kAAFRev2 == testRev, AAFRESULT_TEST_FAILED);

		checkResult(pFile->Close());
    	checkResult(pFile->Release());
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ReadAAFContents: caught error hr=0x" << hex << hr << dec << endl;
	}

	// Clean up
	if (mobIter)
		mobIter->Release();

	if (pMob)
		pMob->Release();

	if (pHeader)
		pHeader->Release();

	printf("   ok   ");
	return hr;
}

static HRESULT OpenAAFFile(const aafUID_t *written_kind, aafWChar * pFileName)
{
	IAAFFile* pFile = NULL;
	HRESULT hr = S_OK;

	try
	{
		for (int i = 0; read_api[i].api != APIunknown; i++)
		{
			// Determine the method to use to open the file
			if (read_api[i].api == FileOpenExistingRead)
			{
				checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

				checkResult(ReadAAFContents(pFile));
			}
			else if (read_api[i].api == FileOpenExistingModify)
			{
				checkResult(AAFFileOpenExistingModify(pFileName, 0, &TestProductID, &pFile));

				checkResult(ReadAAFContents(pFile));
			}
			else if (read_api[i].api == RawStorage)
			{
				for (int j = 0; filekind[j].kind != NULL; j++)
				{
					HRESULT r = S_OK;
	
					IAAFRawStorage* pRawStorage = 0;
					checkResult(AAFCreateRawStorageDisk(
									pFileName,
									kAAFFileExistence_existing,
									kAAFFileAccess_read,
									&pRawStorage));
					r = AAFCreateAAFFileOnRawStorage (
									pRawStorage,
									kAAFFileExistence_existing,
									kAAFFileAccess_read,
									filekind[j].kind,
									0,
									0,
									&pFile);
					pRawStorage->Release();
					if (r == AAFRESULT_FILEKIND_NOT_REGISTERED)
					{
						printf("    x   ");
						continue;
					}

					// For RawStorage some combinations are designed not to work
					// such as reading a 4K file using a 512 implementation
					// or reading a 512 file using a 4K implementation.
					if ((filekind[j].kind == &kAAFFileKind_Aaf512Binary ||
						filekind[j].kind == &kAAFFileKind_AafS512Binary ||
						filekind[j].kind == &kAAFFileKind_AafM512Binary ||
						filekind[j].kind == &kAAFFileKind_AafG512Binary) &&
							(
							written_kind == &kAAFFileKind_Aaf4KBinary ||
							written_kind == &kAAFFileKind_AafS4KBinary ||
							written_kind == &kAAFFileKind_AafM4KBinary ||
							written_kind == &kAAFFileKind_AafG4KBinary))
					{
						printf("   ic   ");
						continue;
					}
					if ((filekind[j].kind == &kAAFFileKind_Aaf4KBinary ||
						filekind[j].kind == &kAAFFileKind_AafS4KBinary ||
						filekind[j].kind == &kAAFFileKind_AafM4KBinary ||
						filekind[j].kind == &kAAFFileKind_AafG4KBinary) &&
							(written_kind == NULL ||	// NULL (meaning NM default) is 512
							written_kind == &kAAFFileKind_Aaf512Binary ||
							written_kind == &kAAFFileKind_AafS512Binary ||
							written_kind == &kAAFFileKind_AafM512Binary ||
							written_kind == &kAAFFileKind_AafG512Binary))
					{
						printf("   ic   ");
						continue;
					}

					checkResult(pFile->Open());

					checkResult(ReadAAFContents(pFile));
				}
			}
			else
			{
				printf(" *** Did not find valid api to open file %ls i=%d\n", pFileName, i);
				checkExpression(false, AAFRESULT_TEST_FAILED);
			}

			// ReadAAFContents() closes and releases pFile
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** OpenAAFFile: caught error hr=0x" << hex << hr << dec << endl;
	}
	return hr;
}

static void SetFilename(aafWChar *filename, const char *api, const char *kind, bool largename)
{
	aafWChar tmp[FILENAME_MAX];
	wcscpy(filename, L"CFKT-");
	mbstowcs(tmp, api, strlen(api)+1);
	wcscat(filename, tmp);
	if (kind)
	{
		wcscat(filename, L"-");
		mbstowcs(tmp, kind, strlen(kind)+1);
		wcscat(filename, tmp);
	}

	if (largename)
	{
		// POSIX defines NAME_MAX for max bytes in a filename (excluding null term.)
		// MSVC instead defines _MAX_FNAME.
		// For max bytes in a full pathname, POSIX and MSVC define FILENAME_MAX.
		// However under WIN32, FILENAME_MAX=260 and _MAX_FNAME=256 so
		// there is little scope to test big filepaths larger than 256.

#ifdef _WIN32
		aafWChar full[FILENAME_MAX] = L"";
		_wfullpath(full, filename, FILENAME_MAX);
		// Calculate filepath space remaining in current path
		// leaving space for terminating null.
		size_t unused_chars = FILENAME_MAX - wcslen(full) - 1;
#else

#ifdef NAME_MAX
		size_t unused_chars = NAME_MAX - wcslen(filename);
#else
		// Get the runtime value for NAME_MAX on the current filesystem
		size_t unused_chars = pathconf(".", _PC_NAME_MAX) - wcslen(filename);
#endif

#endif
		size_t padlen = unused_chars;
		if (padlen >= 4)	// Preserve space for ".aaf" (4 chars)
			padlen -= 4;

		for (size_t i = 0; i < padlen; i++)
			wcscat(filename, L"X");
	}
	wcscat(filename, L".aaf");
}

static HRESULT CreateAAFFile(api_info_t info, bool testLargeNames)
{

	HRESULT hr = S_OK;
	IAAFFile* pFile = NULL;
	aafWChar filename[FILENAME_MAX];

	try
	{
		if (info.api == FileOpenNewModify)
		{
			for (int i = 0; modeNewModify[i].flag != -1; i++)
			{
				SetFilename(filename, info.name, modeNewModify[i].name, testLargeNames);
				RemoveTestFile(filename);

				printf("%-2s-%-4s |", info.name, modeNewModify[i].name);

				checkResult(AAFFileOpenNewModify(
									filename,
									modeNewModify[i].flag,
									&TestProductID,
									&pFile));

				checkResult(WriteAAFFile(pFile));

				checkResult(OpenAAFFile( modeNewModify[i].kind, filename));
				printf("\n");

				if (deleteFiles)
					RemoveTestFile(filename);
			}
		}
		else if (info.api == FileOpenNewModifyEx)
		{
			for (int i = 0; filekind[i].kind != NULL; i++)
			{
				HRESULT r = S_OK;
				SetFilename(filename, info.name, filekind[i].name, testLargeNames);
				RemoveTestFile(filename);

				printf("%2s-%-4s |", info.name, filekind[i].name);

				r = AAFFileOpenNewModifyEx(
								filename,
								filekind[i].kind,
								0,
								&TestProductID,
								&pFile);
				if (r == AAFRESULT_FILEKIND_NOT_REGISTERED)
				{
					printf("    x       x       x       x       x       x       x       x       x       x\n");
					RemoveTestFile(filename);
					continue;
				}
				else
					checkResult(r);

				checkResult(WriteAAFFile(pFile));

				checkResult(OpenAAFFile(filekind[i].kind, filename));
				printf("\n");

				if (deleteFiles)
					RemoveTestFile(filename);
			}
		}
		else
		{
			// RawStorage
    		checkExpression (info.api == RawStorage, AAFRESULT_TEST_FAILED);

			for (int i = 0; filekind[i].kind != NULL; i++)
			{
				HRESULT r = S_OK;
				SetFilename(filename, info.name, filekind[i].name, testLargeNames);
				RemoveTestFile(filename);

				printf("%2s-%-4s |", info.name, filekind[i].name);

				IAAFRawStorage* pRawStorage = 0;
				checkResult(AAFCreateRawStorageDisk(
								filename,
								kAAFFileExistence_new,
								kAAFFileAccess_modify,
								&pRawStorage));
				r = AAFCreateAAFFileOnRawStorage (
								pRawStorage,
								kAAFFileExistence_new,
								kAAFFileAccess_modify,
								filekind[i].kind,
								0,
								&TestProductID,
								&pFile);

				if (r == AAFRESULT_FILEKIND_NOT_REGISTERED)
				{
					printf("    x       x       x       x       x       x       x       x       x       x\n");
					pRawStorage->Release();
					RemoveTestFile(filename);
					continue;
				}
				else
					checkResult(r);

				pRawStorage->Release();
				checkResult(pFile->Open());

				checkResult(WriteAAFFile(pFile));

				checkResult(OpenAAFFile(filekind[i].kind, filename));
				printf("\n");

				if (deleteFiles)
					RemoveTestFile(filename);
			}
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	return hr;
}

void printUsage(const char *progname)
{
	cout << "Usage : " << progname << " [-d]" << endl;
	cout << endl;
	cout << "\tTests all supported AAF write and AAF read APIs" << endl;
	cout << endl;
	cout << "\t-d             Delete test files upon completion" << endl;
}

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ComFileKindTest";

int main(int argc, char *argv[])
{
	TestProductID.companyName = companyName;
	TestProductID.productName = productName;
	TestProductID.productVersion = &TestVersion;
	TestProductID.productVersionString = NULL;
	TestProductID.productID = UnitTestProductID;
	TestProductID.platform = NULL;

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
				deleteFiles = true;
				i++;
			}
			else
			{
				printUsage(argv[0]);
			}
		}
	}

	try
	{
		printf("Legend:  ER - FileOpenExistingRead      ok - success\n");
		printf("         EM - FileOpenExistingModify    ic - incompatible FileKinds\n");
		printf("         RS - RawStorage                 x - FileKind not registered\n");
		printf("         NM - FileOpenNewModify\n");
		printf("         Ex - FileOpenNewModifyEx\n");

		// Run all tests twice: once with small filenames, once with large filenames
		bool testLongNames = false;
		bool allTested = false;
		while (! allTested)
		{
			printf("\n");
			printf("Testing with %s filenames\n", testLongNames ? "long" : "short");
			printf("write   |                                read method                                     \n");
			printf("method  |   ER     EM     RS-4K   RS-512  RS-S4K  RS-S512 RS-M4K  RS-M512 RS-G4K  RS-G512\n");
			printf("--------+--------------------------------------------------------------------------------\n");
			for (int i = 0; write_api[i].api != APIunknown; i++)
			{
				checkResult(CreateAAFFile(write_api[i], testLongNames));
			}
			allTested = testLongNames;
			testLongNames = true;
		}
	}
	catch (HRESULT& r)
	{
		cerr << "Error : Caught HRESULT 0x" << hex << r << endl;
		return 1;
	}

	return 0;
}
