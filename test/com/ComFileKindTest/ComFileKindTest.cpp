// @doc INTERNAL
// @com This file implements the module test for CAAFFile
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



#include "AAF.h"

#include <iostream.h>
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


// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

// convenient error handlers.
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

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xfd3cc302, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


static HRESULT CreateAAFFile(aafWChar * pFileName,
                             const aafUID_t* pFileKind,
                             IAAFFile** ppFile)
{
	aafProductIdentification_t	ProductInfo;
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
	HRESULT						hr = S_OK;
 
    try {
     // Remove the previous test file if any.
      RemoveTestFile(pFileName);

#if 0
      checkResult(AAFFileOpenNewModify(
        pFileName,
        0,
        &ProductInfo,
        ppFile));
#else
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
#endif
	} catch (HRESULT& rResult) {
	  hr = rResult;
    }
	return hr;
}

static HRESULT WriteAAFFile(IAAFFile* pFile)
{
	bool bFileOpen = true;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFMob			*pMob = NULL;
	HRESULT						hr = S_OK;

  try 
  {

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

      // Make sure the header returns us the same dictionary as the file
	  IAAFDictionarySP pDictionaryFromHeader;
	  checkResult(pHeader->GetDictionary(&pDictionaryFromHeader));

	  CAAFBuiltinDefs defs (pDictionary);
 	  
	  // Create a concrete subclass of Mob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));
    
	  // Initialize the Mob properties
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(MOB_NAME_TEST));

	  // Add the source mob into the tree
	  checkResult(pHeader->AddMob(pMob));

	  // Attempt to save the file.
	  checkResult(pFile->Save());

	  // Attempt to close the file.
	  checkResult(pFile->Close());
	  bFileOpen = false;
    }
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


	// Cleanup and return
	if (pMob)
		pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
 		pFile->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  IEnumAAFMobs *mobIter = NULL;
  IAAFMob			*pMob = NULL;
  aafNumSlots_t				numMobs, n;
  HRESULT						hr = S_OK;
  aafWChar					name[500];
  aafMobID_t					mobID;
  aafFileRev_t					testRev;

	  
  try
  {
    // Open the file
    checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

    checkResult(pHeader->GetMobs (NULL, &mobIter));
    for(n = 0; n < numMobs; n++)
	  {
		  checkResult(mobIter->NextOne (&pMob));
		  checkResult(pMob->GetName (name, sizeof(name)));
		  checkResult(pMob->GetMobID (&mobID));
		  checkExpression(wcscmp( name, MOB_NAME_TEST) == 0, AAFRESULT_TEST_FAILED);
		  checkExpression(memcmp(&mobID, &TEST_MobID, sizeof(mobID)) == 0, AAFRESULT_TEST_FAILED);

		  pMob->Release();
		  pMob = NULL;
	  }

	  mobIter->Release();
	  mobIter = NULL;

	checkResult(pFile->GetRevision(&testRev));
    checkExpression(kAAFRev1 == testRev, AAFRESULT_TEST_FAILED);

	checkResult(pFile->Close());
    bFileOpen = false;

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


	// Cleanup and return
  if (mobIter)
    mobIter->Release();

	if (pMob)
		pMob->Release();

  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	return hr;
}

struct {
  const aafUID_t* kind;
  wchar_t* name;
  bool read; // temporary - some kinds cannot be read
} fileinfo[] = {
  {
    &aafFileKindAafSSBinary,
    L"ComFileKindTest.aaf",
    true
  },
  {
    &aafFileKindAafXmlText,
    L"ComFileKindTest.xml",
    false
  }
};

int main(void)
{
  try {
  IAAFFile* pFile = 0;
    for (int i = 0; i < sizeof(fileinfo)/sizeof(fileinfo[0]); i++) {
      checkResult(CreateAAFFile(fileinfo[i].name, fileinfo[i].kind, &pFile));
      checkResult(WriteAAFFile(pFile));
      aafUID_t k = {0};
      aafBool b = kAAFFalse;
      checkResult(AAFFileIsAAFFile(fileinfo[i].name, &k, &b));
      if (!b) {
        cerr << "AAFFileIsAAFFile() reports file is not an AAF file." << endl;
        throw AAFRESULT_TEST_FAILED;
      }
      if (memcmp(&k, fileinfo[i].kind, sizeof(aafUID_t)) != 0) {
        cerr << "Incorrect file kind." << endl;
        throw AAFRESULT_TEST_FAILED;
      }
      if (fileinfo[i].read) {
        checkResult(ReadAAFFile(fileinfo[i].name));
      }
    }
  } catch(HRESULT& r) {
    cerr << "Caught HRESULT 0x" << hex << r << endl;
  }
  return 0;
}
