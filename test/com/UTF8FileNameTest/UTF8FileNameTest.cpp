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
// Licensor of the AAF Association is Avid Technology.
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
	_wremove(pFileName);
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
{0xfd3cc302, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


static HRESULT CreateAAFFile(aafWChar * pFileName,
			     bool UseRaw,
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
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"ComFileKindTest";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;
  HRESULT hr = S_OK;
 
  try {
    RemoveTestFile(pFileName);

		if( !UseRaw )
		{
			checkResult(AAFFileOpenNewModify(
				pFileName,
				0,
				&ProductInfo,
				ppFile));
		}
		else
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
  }
	catch (HRESULT& rResult)
	{
    hr = rResult;
  }
  return hr;
}

static HRESULT WriteAAFFile(IAAFFile* pFile)
{
  bool bFileOpen = true;
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
    CAAFBuiltinDefs defs (pDictionary);
     
    // Create a Mob
    checkResult(defs.cdMasterMob()->CreateInstance(IID_IAAFMob, 
                                                   (IUnknown **)&pMob));
    
    // Initialize the Mob
    checkResult(pMob->SetMobID(TEST_MobID));
    checkResult(pMob->SetName(MOB_NAME_TEST));

    // Add the mob to the file
    checkResult(pHeader->AddMob(pMob));

    // Save the file
    checkResult(pFile->Save());

    // Close the file
    checkResult(pFile->Close());
    bFileOpen = false;

  } catch (HRESULT& rResult) {
    hr = rResult;
  }

  // Clean up
  if (pMob)
    pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile) {
    if (bFileOpen) {
      // Save and close the file if an error left it open
      pFile->Save();
      pFile->Close();
    }
    pFile->Release();
  }

  return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName,
			   bool UseRaw = false,
                           const aafUID_t* pFileKind = &aafFileKindDontCare)
{
  IAAFRawStorage* pRawStorage = 0;
  IAAFFile* pFile = NULL;
  bool bFileOpen = false;
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
    bFileOpen = true;

    // Get the header
    checkResult(pFile->GetHeader(&pHeader));

    // Expect to find a single Mob
    checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
    checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

    checkResult(pHeader->GetMobs (NULL, &mobIter));
    for(n = 0; n < numMobs; n++) {
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
    checkExpression(kAAFRev1 == testRev, AAFRESULT_TEST_FAILED);

    checkResult(pFile->Close());
    bFileOpen = false;

  } catch (HRESULT& rResult) {
    hr = rResult;
  }

  // Clean up
  if (mobIter)
    mobIter->Release();

  if (pMob)
    pMob->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile) {
    if (bFileOpen) {
      // Close the file if an error left it open
      pFile->Close();
    }
    pFile->Release();
  }

  return hr;
}

struct {
  wchar_t* name;
	bool create;
	bool createraw;
  const aafUID_t* kind;
  char* type;
  bool read;
  const aafUID_t* rkind;
  char* rtype;
} fileinfo[] = {
#ifdef OS_WINDOWS

  {
    L"CFKT-Default-2.aaf",
		true,
		false,
    &aafFileKindAafS4KBinary,
    "SSS",
    true,
    &aafFileKindAafS4KBinary,
    "SSS"
  },
  {
    L"CFKT-S512.aaf",
		true,
		true,
    &aafFileKindAafS512Binary,
    "SSS",
    true,
    &aafFileKindAafS512Binary,
    "SSS"
  },
  {
    L"CFKT-S4k.aaf",
		true,
		true,
    &aafFileKindAafS4KBinary,
    "S4K",
    true,
    &aafFileKindAafS4KBinary,
    "S4K"
  },

  {
    L"CFKT-M4K.aaf",
		true,
		true,
    &aafFileKindAafM4KBinary,
    "M4K",
    true,
	&aafFileKindAafM4KBinary,
	"M4K"
  }
#else
  {
    L"CFKT-Default.aaf",
		true,
		true,
    &aafFileKindAaf4KBinary,
    "SSS",
    true,
    &aafFileKindAaf4KBinary,
    "SSS"
  },
  {
    L"CFKT-Default-2.aaf",
		true,
		false,
    &aafFileKindAafS4KBinary,
    "SSS",
    true,
    &aafFileKindAafS4KBinary,
    "SSS"
  },
  {
    L"CFKT-S512.aaf",
		true,
		true,
    &aafFileKindAafS512Binary,
    "SSS",
    true,
    &aafFileKindAafS512Binary,
    "SSS"
  },
  {
    L"CFKT-S4k.aaf",
		true,
		true,
    &aafFileKindAafS4KBinary,
    "S4K",
    true,
    &aafFileKindAafS4KBinary,
    "S4K"
  },

  {
    L"CFKT-M4K.aaf",
		true,
		true,
    &aafFileKindAafM4KBinary,
    "M4K",
    true,
	&aafFileKindAafM4KBinary,
	"M4K"
  }
#endif
};


int main(void)
{
  try {
		IAAFFile* pFile = 0;
		wchar_t UnicodeFileName[30];

		wcscpy(UnicodeFileName, L"zz12345z.aaf");
		UnicodeFileName[2]=0x2200;
		UnicodeFileName[3]=0x2297;
		UnicodeFileName[4]=0x222F;
		UnicodeFileName[5]=0x2282;
		UnicodeFileName[6]=0x221e;


    for (int i = 0; i < sizeof(fileinfo)/sizeof(fileinfo[0]); i++)
		{
			UnicodeFileName[1]=i+'1'; //change the filename for each test, second character is incremented 1,2,3 etc.
			CoutTestFile( UnicodeFileName );
      // Create the file

			cout << "  Creating " ;
			if( fileinfo[i].createraw ) cout << "Raw ";
			cout << fileinfo[i].type << endl;
			checkResult(CreateAAFFile(UnicodeFileName, fileinfo[i].createraw, fileinfo[i].kind, &pFile));

			// Write the file contents
			cout << "  Writing" << endl;
			checkResult(WriteAAFFile(pFile));


      // Check that we made an AAF file with the correct encoding
      cout << "  Checking " << fileinfo[i].type << endl;
      aafUID_t k = {0};
      aafBool b = kAAFFalse;

      checkResult(AAFFileIsAAFFile(UnicodeFileName, &k, &b));
      if (!b) {
        cerr << "Error : AAFFileIsAAFFile() reports file is not an AAF file."
             << endl;
        throw AAFRESULT_TEST_FAILED;
      }

      // Read the file
        cout << "  Reading (default)" << endl;
        checkResult(ReadAAFFile(UnicodeFileName));
        cout << "  Reading Raw " << fileinfo[i].rtype << endl;
        checkResult(ReadAAFFile(UnicodeFileName, true, fileinfo[i].rkind ));
    }
  }
	catch(HRESULT& r)
	{
    cerr << "Error : Caught HRESULT 0x" << hex << r << endl;
  }
  return 0;
}
