// @doc INTERNAL
// @com This file implements the module test for CAAFFile
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/



#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"

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
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

static HRESULT checkModeFlag (aafUInt32 modeFlags,
							  HRESULT expectedResult)
{
  // Check mod flags
  aafProductIdentification_t	ProductInfo = { 0 };
  HRESULT temphr;
  IAAFFile * pFile = 0;
  temphr = AAFFileOpenNewModify(L"foo",
								modeFlags,
								&ProductInfo,
								&pFile);
  if (expectedResult != temphr)
	{
	  return AAFRESULT_TEST_FAILED;
	}
  if (pFile)
	{
	  return AAFRESULT_TEST_FAILED;
	}

  return AAFRESULT_SUCCESS;
}


static HRESULT checkModeFlags ()
{
  HRESULT temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_EAGER_LOADING,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_REVERTABLE,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_UNBUFFERED,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_RECLAIMABLE,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_USE_LARGE_SS_SECTORS,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_CLOSE_FAIL_DIRTY,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG0_ON,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG1_ON,
						  AAFRESULT_NOT_IMPLEMENTED);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  aafUInt32 i;
  for (i = 4; i < 28; i++)
	{
	  temphr = checkModeFlag ((1 << i),
							  AAFRESULT_BAD_FLAGS);
	  if (AAFRESULT_FAILED (temphr)) return temphr;
	}
  return AAFRESULT_SUCCESS;
}


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xfd3cc302, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFMob			*pMob = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFile Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;


  try 
  {
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);

	  // Check for illegal mode flags.
	  checkModeFlags ();

	  // Create the file.
	  checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	  bFileOpen = true;
  
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

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
  aafProductIdentification_t	ProductInfo;
  aafNumSlots_t				numMobs, n;
  HRESULT						hr = S_OK;
  aafWChar					name[500];
  aafMobID_t					mobID;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFFile Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.platform = NULL;
	  
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

extern "C" HRESULT CAAFFile_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFFileTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFMob_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  	// When all of the functionality of this class is tested, we can
	// return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
  return hr;
}
