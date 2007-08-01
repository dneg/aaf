// @doc INTERNAL
// @com This file implements tests for variour file kinds
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology, 
// with parts Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

//This test is based ont he ComFileKindsTest and tests reading and writing
//filenames with unicode characters, though the named and raw interfaces.

#include "AAF.h"

#include <iostream>
#include <iomanip>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"

#include "utf8.h"

static void CoutTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstou8s(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1) {
    cout << endl << cFileName << endl;
  }
}

static void RemoveTestFile(const wchar_t* pFileName)
{

#ifdef OS_WINDOWS
	int ret=_wremove(pFileName);
#else
	const size_t kMaxFileName = 512;
	char cFileName[kMaxFileName];

	size_t status = wcstou8s(cFileName, pFileName, kMaxFileName);
	if (status != (size_t)-1) {
		remove(cFileName);
	}
#endif
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
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"UTF8FileNameTest";


static HRESULT CreateAAFFile(aafWChar * pFileName,
			     bool UseRaw,
			     bool UseEx,
                             const aafUID_t* pFileKind,
                             IAAFFile** ppFile)
{
  aafProductIdentification_t  ProductInfo;
  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = companyName;
  ProductInfo.productName = productName;
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;
  HRESULT hr = S_OK;
 
  try {
		RemoveTestFile(pFileName);

		if( UseEx )
		{
			checkResult(AAFFileOpenNewModifyEx(
				pFileName,
				pFileKind,
				0,
				&ProductInfo,
				ppFile));
		}
		else if (UseRaw)
		{
			IAAFRawStorage* pRawStorage = 0;
			checkResult(AAFCreateRawStorageDisk(
				pFileName,
				kAAFFileExistence_new,
				kAAFFileAccess_modify,
				&pRawStorage));
			checkResult(AAFCreateAAFFileOnRawStorage (
				pRawStorage,
				kAAFFileExistence_new,
				kAAFFileAccess_modify,
				pFileKind,
				0,
				&ProductInfo,
				ppFile));
			pRawStorage->Release();
			checkResult((*ppFile)->Open());
		}
		else
		{
			checkResult(AAFFileOpenNewModify(
				pFileName,
				0,
				&ProductInfo,
				ppFile));
		}
  }
	catch (HRESULT& rResult)
	{
    hr = rResult;
  }
  return hr;
}

typedef IAAFSmartPointer<IAAFEssenceFormat> IAAFEssenceFormatSP;

static HRESULT WriteAAFFile(IAAFFile* pFile, const aafWChar *pExternalFilename)
{
  IAAFHeader* pHeader = NULL;
  IAAFDictionary* pDictionary = NULL;
  IAAFMob *pMob = NULL;
  IAAFMasterMob* pMasterMob = NULL;
  HRESULT hr = S_OK;

  try 
  {

    // Get the header
    checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary
    checkResult(pHeader->GetDictionary(&pDictionary));
    CAAFBuiltinDefs defs (pDictionary);
     
    // Create a Mob
    checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));
	checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
    
    // Initialize the Mob (these values are tested when the file is read)
    checkResult(pMob->SetMobID(TEST_MobID));
    checkResult(pMob->SetName(MOB_NAME_TEST));

    // Add the mob to the file
    checkResult(pHeader->AddMob(pMob));

	// Test external essence which will create an external file via the Locator
	IAAFEssenceAccess*	pEssenceAccess = NULL;
	aafUID_t			testContainer = ContainerFile;
	IAAFLocator			*pLocator = NULL;
	aafRational_t		testRate = {48000, 1};
	IAAFEssenceFormatSP	pFormat;


	RemoveTestFile(pExternalFilename);		// avoid AAFRESULT_FILE_EXISTS

	checkResult(defs.cdNetworkLocator()->CreateInstance(IID_IAAFLocator, (IUnknown **)&pLocator));
	checkResult(pLocator->SetPath(pExternalFilename));
	
	checkResult(pMasterMob->CreateEssence(
						1,
						defs.ddkAAFSound(),
						kAAFCodecWAVE,
						testRate,
						testRate,
						kAAFCompressionDisable,
						pLocator,
						testContainer,
						&pEssenceAccess));

	aafUInt32 sampleBits = 8;
	checkResult(pEssenceAccess->GetEmptyFileFormat(&pFormat));
	checkResult(pFormat->AddFormatSpecifier(kAAFAudioSampleBits, sizeof(sampleBits), (aafUInt8 *)&sampleBits));
	checkResult(pEssenceAccess->PutFileFormat(pFormat));

	aafUInt8 audioBuf[] = { 0x01, 0x02, 0x03, 0x04 };
	aafUInt32 samplesWritten, bytesWritten;

	checkResult(pEssenceAccess->WriteSamples(
						4,
						sizeof(audioBuf),
						audioBuf,
						&samplesWritten,
						&bytesWritten));

	checkResult(pEssenceAccess->CompleteWrite());

	if (pMasterMob)
		pMasterMob->Release();

	if (pMob)
		pMob->Release();

    // Save & close the file
    checkResult(pFile->Save());
    checkResult(pFile->Close());
    checkResult(pFile->Release());

	pEssenceAccess->Release();
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
	cout << "*** WriteAAFFile: caught error hr=0x" << hex << hr << dec << endl;
  }

  return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName,
			   bool UseRaw = false,
                           const aafUID_t* pFileKind = &kAAFFileKind_DontCare)
{
  IAAFFile* pFile = NULL;
  IAAFHeader* pHeader = NULL;
  IEnumAAFMobs* mobIter = NULL;
  IAAFMob* pMob = NULL;
  aafNumSlots_t numMobs, n;
  HRESULT hr = S_OK;
  aafWChar name[500];
  aafMobID_t mobID;
  aafFileRev_t testRev;

  try
  {

    // Open the file
		if( !UseRaw )
		{
			checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		}
		else
		{
			IAAFRawStorage* pRawStorage = 0;
			checkResult(AAFCreateRawStorageDisk(
				pFileName,
				kAAFFileExistence_existing,
				kAAFFileAccess_read,
				&pRawStorage));
			checkResult(AAFCreateAAFFileOnRawStorage (
				pRawStorage,
				kAAFFileExistence_existing,
				kAAFFileAccess_read,
				pFileKind,
				0,
				0,
				&pFile));
			pRawStorage->Release();
			checkResult(pFile->Open());
		}

    // Get the header
    checkResult(pFile->GetHeader(&pHeader));

    // Expect to find a single Mob
    checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
    checkExpression (2 == numMobs, AAFRESULT_TEST_FAILED);

    checkResult(pHeader->GetMobs (NULL, &mobIter));
    for(n = 0; n < numMobs; n++)
	{
		IAAFSourceMob				*pSourceMob = NULL;

		checkResult(mobIter->NextOne (&pMob));

		// Is this a source mob?
		if (AAFRESULT_SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob)))
		{
			pSourceMob->Release();
		}
		else
		{
			// Check that the properties are as we wrote them to the MasterMob
			checkResult(pMob->GetName (name, sizeof(name)));
			checkExpression(wcscmp( name, MOB_NAME_TEST) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pMob->GetMobID (&mobID));
			checkExpression(memcmp(&mobID, &TEST_MobID, sizeof(mobID)) == 0, AAFRESULT_TEST_FAILED);
		}

		pMob->Release();
		pMob = NULL;
    }

    mobIter->Release();
    mobIter = NULL;

    checkResult(pFile->GetRevision(&testRev));
    checkExpression(kAAFRev2 == testRev, AAFRESULT_TEST_FAILED);

    checkResult(pFile->Close());
    checkResult(pFile->Release());

  } catch (HRESULT& rResult) {
    hr = rResult;
	cout << "*** ReadAAFFile: caught error hr=0x" << hex << hr << dec << endl;
  }

  // Clean up
  if (mobIter)
    mobIter->Release();

  if (pMob)
    pMob->Release();

  if (pHeader)
    pHeader->Release();
      
  return hr;
}

struct _fileinfo_t {
  const wchar_t* name;
  bool createex;
  bool createraw;
  const aafUID_t* kind;
  const char* type;
  bool read;
  const aafUID_t* rkind;
  const char* rtype;
} fileinfo[] = {
  {
    L"CFKT-S512.aaf",
		true, // use Ex
		true,
    &kAAFFileKind_AafS512Binary,
    "S512",
    true,
    &kAAFFileKind_AafS512Binary,
    "S512"
  },
  {
    L"CFKT-S4K.aaf",
		true, // use Ex
		true,
    &kAAFFileKind_AafS4KBinary,
    "S4K",
    true,
    &kAAFFileKind_AafS4KBinary,
    "S4K"
  }
#ifdef OS_WINDOWS
  , {
    L"CFKT-M512.aaf",
		true, // use Ex
		true,
    &kAAFFileKind_AafM512Binary,
    "M512",
    true,
	&kAAFFileKind_AafM512Binary,
	"M512"
  },
  {
    L"CFKT-M4K.aaf",
		true, // use Ex
		true,
    &kAAFFileKind_AafM4KBinary,
    "M4K",
    true,
    &kAAFFileKind_AafM4KBinary,
    "M4K"
  }
#endif
  , {
    L"CFKT-S512.aaf",
		false,
		true, // use raw
    &kAAFFileKind_AafS512Binary,
    "S512",
    true,
    &kAAFFileKind_AafS512Binary,
    "S512"
  },
  {
    L"CFKT-S4K.aaf",
		false,
		true, // use raw
    &kAAFFileKind_AafS4KBinary,
    "S4K",
    true,
    &kAAFFileKind_AafS4KBinary,
    "S4K"
  }
#ifdef OS_WINDOWS
  , {
    L"CFKT-M512.aaf",
		false,
		true, // use raw
    &kAAFFileKind_AafM512Binary,
    "M512",
    true,
	&kAAFFileKind_AafM512Binary,
	"M512"
  },
  {
    L"CFKT-M4K.aaf",
		false,
		true, // use raw
    &kAAFFileKind_AafM4KBinary,
    "M4K",
    true,
    &kAAFFileKind_AafM4KBinary,
    "M4K"
  }
#endif
  , {
    L"CFKT-Default-512.aaf",
		false,
		false, // use AAFFileOpenNewModify
    &kAAFFileKind_Aaf512Binary,
    "512",
    true,
    &kAAFFileKind_Aaf512Binary,
    "512"
  }
};

// Make sure all of our required plugins have been registered.
static void RegisterRequiredPlugins(void)
{
  IAAFPluginManager	*mgr = NULL;

  // Load the plugin manager 
  checkResult(AAFGetPluginManager(&mgr));

  // Attempt load and register all of the plugins
  // in the shared plugin directory.
  checkResult(mgr->RegisterSharedPlugins());

  if (mgr)
    mgr->Release();
}

int main(int argc, char* argv[])
{
	bool simpleFileIO = false;

	// By default all file I/O, including external essence, is tested with UTF-8 names
	// -s arg will test only the simple file I/O operations using UTF-8 names which are:
	//   AAFFileOpenNewModify() AAFCreateAAFFileOnRawStorage()
	if (argc == 2 && (strcmp(argv[1], "-s") == 0))
		simpleFileIO = true;


  try {
	RegisterRequiredPlugins();

	IAAFFile* pFile = 0;
	wchar_t UnicodeFileName[30], UnicodeExtName[30];

	wcscpy(UnicodeFileName, L"1x2x3xN.aaf");		// filename of AAF file
	wcscpy(UnicodeExtName, L"1x2x3xN.ess");		// filename of external essence
	UnicodeFileName[1]=0x61; // "a" symbol, UTF-8 encoding is 1 byte
	UnicodeFileName[3]=0xa9; // copyright symbol, UTF-8 encoding is 2 byte
	UnicodeFileName[5]=0x2260; // not equal symbol, UTF-8 encoding is 3 byte

	UnicodeExtName[1]=0x61; // "a" symbol, UTF-8 encoding is 1 byte
	UnicodeExtName[3]=0xa9; // copyright symbol, UTF-8 encoding is 2 byte
	UnicodeExtName[5]=0x2260; // not equal symbol, UTF-8 encoding is 3 byte

    for (unsigned i = 0; i < sizeof(fileinfo)/sizeof(fileinfo[0]); i++)
	{
		// Increment seventh character of filename to give different names per iteration
		UnicodeFileName[6] = UnicodeExtName[6] = '0'+i;
		CoutTestFile( UnicodeFileName );

		// Create the file
		cout << "  Creating " << fileinfo[i].type << " with ";
		if( fileinfo[i].createex )
			cout << "AAFFileOpenNewModifyEx()" << endl;
		else if( fileinfo[i].createraw )
			cout << "AAFCreateRawStorageDisk(), AAFCreateAAFFileOnRawStorage()" << endl;
		else
			cout << "AAFFileOpenNewModify()" << endl;
		checkResult(CreateAAFFile(UnicodeFileName, fileinfo[i].createraw, fileinfo[i].createex,
			fileinfo[i].kind, &pFile));

		// Write the AAF file and the external essence file
		cout << "  Writing" << endl;
		if (simpleFileIO)
			checkResult(WriteAAFFile(pFile, L"external.wav"));
		else
			checkResult(WriteAAFFile(pFile, UnicodeExtName));

		// Check that we made an AAF file with the correct encoding
		cout << "  Checking with AAFFileIsAAFFile()" << endl;
		aafUID_t k = {0};
		aafBool b = kAAFFalse;

		checkResult(AAFFileIsAAFFile(UnicodeFileName, &k, &b));
		if (!b) {
			cerr << "Error : AAFFileIsAAFFile() reports file is not an AAF file." << endl;
			throw AAFRESULT_TEST_FAILED;
		}

		// Read the file
		cout << "  Reading with AAFFileOpenExistingRead()" << endl;
		checkResult(ReadAAFFile(UnicodeFileName));
		cout << "  Reading " << fileinfo[i].rtype
			<< " with AAFCreateRawStorageDisk(), AAFCreateAAFFileOnRawStorage()" << endl;
		checkResult(ReadAAFFile(UnicodeFileName, true, fileinfo[i].rkind ));
	}
  }
	catch(HRESULT& r)
	{
    cerr << "Error : Caught HRESULT 0x" << hex << r << endl;
	return 1;
  }
  return 0;
}
