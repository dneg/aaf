//=------------------------------------------------ -*- tab-width:4 -*- =
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

#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "AAFWideString.h"
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"

#include "CAAFBuiltinDefs.h"

#include "CAAFFileTest.h"

typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFFile>        IAAFFileSP;

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

// Function to compare COM interface pointers, taken from
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
	checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
	checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
	if (spUnk1 == spUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;
}

// FileKInd helper function
static inline bool
equalUID(const aafUID_t& lhs, const aafUID_t& rhs)
{
	return (memcmp(&lhs, &rhs, sizeof(aafUID_t))==0);
}

static bool
isSSFileKind(const aafUID_t& fileKind)
{
	bool result=false;
	if (equalUID(fileKind, testFileKindDefault)
		|| equalUID(fileKind, kAAFFileKind_AafM512Binary)
		|| equalUID(fileKind, kAAFFileKind_AafS512Binary)
		|| equalUID(fileKind, kAAFFileKind_AafG512Binary)
		|| equalUID(fileKind, kAAFFileKind_AafM4KBinary)
		|| equalUID(fileKind, kAAFFileKind_AafS4KBinary)
		|| equalUID(fileKind, kAAFFileKind_AafG4KBinary))
	{
		result=true;
	}
	return result;
}

static aafUID_t
getSS4KFileKind(const aafUID_t& fileKind)
{
	aafUID_t ss4KFileKind=kAAFFileKind_Pathological;
	if (equalUID(fileKind, testFileKindDefault)) {
		ss4KFileKind=kAAFFileKind_Aaf4KBinary;
	} else if (equalUID(fileKind, kAAFFileKind_AafM4KBinary)) {
		ss4KFileKind=kAAFFileKind_AafM512Binary;
	} else if (equalUID(fileKind, kAAFFileKind_AafS4KBinary)) {
		ss4KFileKind=kAAFFileKind_AafS512Binary;
	} else if (equalUID(fileKind, kAAFFileKind_AafG4KBinary)) {
		ss4KFileKind=kAAFFileKind_AafS512Binary;
	}
	return ss4KFileKind;
}

static HRESULT checkModeFlag (
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID,
    aafUInt32 modeFlags,
    HRESULT expectedResult)
{
  // Check mod flags
  const aafWChar * pFileName = L"ModeTestF.ile";
  RemoveTestFile(pFileName);
  HRESULT temphr;
  IAAFFile * pFile = 0;
  temphr = CreateTestFile( pFileName,
                           fileKind,
                           rawStorageType,
                           modeFlags,
                           productID,
                           &pFile );

  if (temphr == AAFRESULT_SUCCESS) {
    RemoveTestFile(pFileName);
  }

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


static HRESULT checkModeFlags (
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  HRESULT temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_REVERTABLE,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_UNBUFFERED,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_RECLAIMABLE,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_CLOSE_FAIL_DIRTY,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_DEBUG0_ON,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  temphr = checkModeFlag (fileKind,
                          rawStorageType,
						  productID,
                          AAF_FILE_MODE_DEBUG1_ON,
                          AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr))
	  return temphr;

  aafUInt32 i;
  for (i = 4; i < 28; i++)
	{
	  temphr = checkModeFlag (fileKind,
                          rawStorageType,
                          productID,
                          (1 << i),
                          AAFRESULT_BAD_FLAGS);
	  if (AAFRESULT_FAILED (temphr))
		  return temphr;
	}
  return AAFRESULT_SUCCESS;
}


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


static HRESULT CreateAAFFile(
    const aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFMob			*pMob = NULL;
	HRESULT						hr = S_OK;


  try 
  {
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);

	  // Check for illegal mode flags.
	  checkResult(checkModeFlags (fileKind, rawStorageType, productID));

/* Premature Close() test inserted ******************************************/
	  // Create the file.
      checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  bFileOpen = true;
  
	  checkResult(pFile->Close());
	  bFileOpen = false;
	  HRESULT	hBad1 = pFile->GetHeader(&pHeader);
	  checkExpression( AAFRESULT_NOT_OPEN == hBad1, AAFRESULT_TEST_FAILED );
	  checkExpression( NULL == pHeader, AAFRESULT_TEST_FAILED );
	  pFile->Release();
	  pFile = NULL;
	  
	  // Remove the previous test file again.
	  RemoveTestFile(pFileName);

/* Actual Create file happening now! ******************************************/
	  // Create the file again.
      checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

      // Make sure the header returns us the same dictionary as the file
	  IAAFDictionarySP pDictionaryFromHeader;
	  checkResult(pHeader->GetDictionary(&pDictionaryFromHeader));
	  checkExpression(AreUnksSame(pDictionary,pDictionaryFromHeader)==kAAFTrue);

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

class TestProgress : public IAAFProgress 
    {
    // Defeat gcc warning about private ctor/dtor and no friends
    // Note that this dummy function cannot itself be called because
    // it requires a constructed TestProgress object.
    friend void dummyFriend(TestProgress);

    public:
        virtual HRESULT STDMETHODCALLTYPE ProgressCallback( void);
        virtual ULONG STDMETHODCALLTYPE AddRef( void);
        virtual ULONG STDMETHODCALLTYPE Release( void);
        virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
           /* [iid_is][out] */ void **ppvObject);
        
        static HRESULT Create(IAAFProgress** pProgress);

    private:

       TestProgress();

       virtual ~TestProgress();

       aafUInt32 _referenceCount;
    };
 
HRESULT STDMETHODCALLTYPE
    TestProgress::ProgressCallback ()
{
	return AAFRESULT_SUCCESS;
}

ULONG STDMETHODCALLTYPE
    TestProgress::AddRef ()
{
	return ++_referenceCount;
}

ULONG STDMETHODCALLTYPE
    TestProgress::Release ()
{
  aafUInt32 r = --_referenceCount;
  if (r == 0) {
    delete this;
  }
  return r;
}

HRESULT STDMETHODCALLTYPE
    TestProgress::QueryInterface (REFIID iid, void ** ppIfc)
{
  if (ppIfc == 0)
    return AAFRESULT_NULL_PARAM;

  if (memcmp(&iid, &IID_IUnknown, sizeof(IID)) == 0) {
    IUnknown* unk = (IUnknown*) this;
    *ppIfc = (void*) unk;
    AddRef ();
    return AAFRESULT_SUCCESS; 
  } else if (memcmp(&iid, &IID_IAAFProgress, sizeof(IID)) == 0) {
    IAAFProgress* cpa = this;
    *ppIfc = (void*) cpa;
    AddRef ();
    return AAFRESULT_SUCCESS;
  } else {
    return E_NOINTERFACE;
  }
}

HRESULT TestProgress::Create(IAAFProgress** ppProgress)
{
  if (ppProgress == 0)
    return AAFRESULT_NULL_PARAM;

  IAAFProgress* result = new TestProgress();
  if (result == 0)
    return AAFRESULT_NOMEMORY;

  result->AddRef();
  *ppProgress = result;
  return AAFRESULT_SUCCESS;
}

TestProgress::TestProgress()
: _referenceCount(0)
{
}

TestProgress::~TestProgress()
{
  assert(_referenceCount == 0);
}

static HRESULT ReadAAFFile(const aafWChar * pFileName)
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
  IAAFProgress					*progress;
	  
  try
  {
    // Open the file
	checkResult(TestProgress::Create(&progress));
	checkResult(AAFSetProgressCallback(progress));
    progress->Release();

    checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

	// Check mandatory Header properties are sane
	aafTimeStamp_t ts;
	checkResult(pHeader->GetLastModified(&ts));
	checkExpression(			// YYYY-MM-DD should not be 0000-00-00
		!( ts.date.year == 0 && ts.date.month == 0 && ts.date.day == 0),
		AAFRESULT_TEST_FAILED);

	// Check written mobs preserved
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
    checkExpression(kAAFRev2 == testRev, AAFRESULT_TEST_FAILED);

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

static int ExpectFail(HRESULT hr)
{
  int result = 0;
  // Any failure code (except for internal errors) means a successful test
  if (!AAFRESULT_FAILED(hr)) {
    result = 1; // Unexpected success for a negative test
  } else if (hr == AAFRESULT_ASSERTION_VIOLATION) {
    result = 1; // Internal error instead of failure
  } else if (hr == AAFRESULT_UNEXPECTED_EXCEPTION) {
    result = 1; // Internal error instead of failure
  } else if (hr == AAFRESULT_DLL_SYMBOL_NOT_FOUND) {
    // An older DLL, treat as expected failure of negative test
  }
  // else the negative test failed as expected
  return result;
}

static int ExpectFalse(HRESULT hr, aafBool b)
{
  // A passing test is a successful HRESULT with b == false

  int result = 0;

  if (FAILED(hr)) {
    result = 1;
  } else if (b) {
    result = 1;
  }

  return result;
}

static HRESULT CreateEmptyFile(const wchar_t* pFileName)
{
  HRESULT hr;
  const size_t size = wcslen(pFileName) + 1;
  char* name = new char[size];
  size_t status = wcstombs(name, pFileName, size);

  if (status != (size_t)-1) {
    FILE* f = fopen(name, "w");
    if (f != 0) {
      fclose(f);
      hr = AAFRESULT_SUCCESS;
    } else {
      hr = AAFRESULT_NOT_CREATABLE;
    }
  } else {
    hr = AAFRESULT_INTERNAL_ERROR;
  }
  delete [] name;
  return hr;
}

static HRESULT ComprehensiveOpenTest(testMode_t /* mode */,
									 aafUID_t fileKind,
                                     testRawStorageType_t rawStorageType,
                                     aafProductIdentification_t productID)
{
  // Summary of tests
  //    A) Error cases:
  //	   1. Not Initialized
  //	   2. Already Open
  //	   3. Already Closed
  //	   4. Later Version
  //
  //	B) Multiple Opens:
  //	   1. Already OpenExistingRead
  //	   2. Already OpenExistingModify
  //	   3. Already OpenNewModify
  //	   4. Already OpenTransient
  //	   5. Already CreateAAFFileOnRawStorage
  //
  //	C) Open Success:
  //	   1. Save after Open
  //	   2. Revert after Open
  //	   3. Close after Open
  //	   4. GetHeader after Open
  //
  //	D) Not Open:
  //	   1. Save before Open
  //	   2. Revert before Open
  //	   3. Close before Open
  //	   4. GetHeader before Open

  int f = 0; // Local count of failures
  IAAFFile* pFile = 0;
  
  // Open Test file present for functions that require one
  //
  const aafWChar * pFileName = L"OpenTestF.ile";
  RemoveTestFile(pFileName);

  /* **************************************************************************
	 A) Error cases:
		1. Not Initialized:
		   - Create object only new()-style,
		   - then Open()
		   - expect AAFRESULT_NOT_INITIALIZED
  ************************************************************************** */
  {
	  std::cout << "\tSkipping ComprehensiveOpenTest A1." << std::endl;
#if 0
	  // Can't be done, new-style methods not accesible at this level
	  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	  if (pFile)
		  checkExpression( AAFRESULT_NOT_INITIALIZED == pFile->Open(), AAFRESULT_TEST_FAILED );
	  else
		  f++;
	  if (pFile)
		  pFile->Release(), pFile = 0;
	  RemoveTestFile(pFileName);
#endif
  }

  /* **************************************************************************
	 A) Error cases:
		2. Already Open:
		   - Open(),
		   - then Open() again
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest A2." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 A) Error cases:
		3. Already Closed:
		   - Open(),
		   - then Close(),
		   - then Open() again
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest A3." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkExpression( AAFRESULT_SUCCESS == pFile->Close(), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 A) Error cases:
		4. Later Version:
		   - Create/setup AAF file with later version somehow
		   - Open(),
		   - expect AAFRESULT_FILEREV_DIFF
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest A4." << std::endl;
	  const aafWChar* pBadVersionFileName = L"./my_second_aaf_f.ile";
	  RemoveTestFile(pBadVersionFileName);

	  char cBadVersionFileName[FILENAME_MAX];
#ifdef _DEBUG
	  size_t status =
#endif
	  wcstombs( cBadVersionFileName, pBadVersionFileName, FILENAME_MAX );
	  assert( status != (size_t) - 1 );

	  FILE* fs = fopen( cBadVersionFileName, "wb" );
	  assert( fs );

	  int nbytes = sizeof(MY_SECOND_AAF_FILE) / sizeof(aafUInt8);
	  fwrite( MY_SECOND_AAF_FILE, sizeof(aafUInt8), nbytes, fs );
	  fclose( fs );

	  checkExpression( AAFRESULT_FILEREV_DIFF == AAFFileOpenExistingRead( pBadVersionFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );
	  RemoveTestFile( pBadVersionFileName );
  }

  /* **************************************************************************
	 B) Multiple Opens:
		1. Already OpenExistingRead:
		   - OpenExistingRead(),
		   - then Open(),
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest B1." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 B) Multiple Opens:
		2. Already OpenExistingModify:
		   - OpenExistingModify(),
		   - then Open(),
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest B2." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 B) Multiple Opens:
		3. Already OpenNewModify:
		   - OpenNewModify(),
		   - then Open(),
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest B3." << std::endl;
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenNewModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
	  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

	  // Cleanup for the next test
	  pFile->Close(), pFile->Release(), pFile = 0;
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 B) Multiple Opens:
		4. Already OpenTransient:
		   - OpenTransient(),
		   - then Open(),
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest B4." << std::endl;
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenTransient(&productID, &pFile ), AAFRESULT_TEST_FAILED );
	  checkExpression( AAFRESULT_ALREADY_OPEN == pFile->Open(), AAFRESULT_TEST_FAILED );

	  // Cleanup for the next test
	  pFile->Close(), pFile->Release(), pFile = 0;
  }

  /* **************************************************************************
	 B) Multiple Opens:
		5. Already CreateAAFFileOnRawStorage:
		   - CreateAAFFileOnRawStorage(),
		   - then Open(),
		   - expect AAFRESULT_ALREADY_OPEN
  ************************************************************************** */
{
	  std::cout << "\tSkipping ComprehensiveOpenTest B5." << std::endl;
#if 0
	  // Create a memory raw storage and a file on it, to be opened for
	  // reading.  We'll use this to get the SetFileBits to be tested.
	  IAAFRawStorageSP pRawStg;
	  checkResult( AAFCreateRawStorageMemory( kAAFFileAccess_read, &pRawStg ) );
	  IAAFFileSP pRawFile;
	  checkResult( AAFCreateAAFFileOnRawStorage( pRawStg,
												 kAAFFileExistence_existing,
												 kAAFFileAccess_read,
												 0,
												 0,
												 0,
												 &pRawFile ) );
	  assert (pRawFile);
	  checkExpression (0 != (IAAFFile*)pRawFile,
					   AAFRESULT_TEST_FAILED);

	  //Next line will fail.  Doesn't work, inside the code, the file is neither Open nor Closed.  Go Figure!
	  checkExpression( AAFRESULT_ALREADY_OPEN == pRawFile->Open(), AAFRESULT_TEST_FAILED );

	  // Cleanup for the next test
	  pRawFile->Close(), pRawFile->Release();
#endif
  }

  /* **************************************************************************
	 C) Open Success:
		1. Save after Open:
		   - Open(),
		   - then Save(),
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  // This test is too basic, every module test does approximately the same thing!
  //   We want the precious don't we.  Do it anyway!
  // Yes, yes the precious!  OK we will run this basic test and maybe she will leave it for us!
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest C1." << std::endl;
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenNewModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
	  checkExpression( AAFRESULT_SUCCESS == pFile->Save(), AAFRESULT_TEST_FAILED );

	  // Cleanup for the next test
	  pFile->Close(), pFile->Release(), pFile = 0;
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 C) Open Success:
		2. Revert after Open:
		   - Open(),
		   - then Revert(),
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tSkipping ComprehensiveOpenTest C2." << std::endl;
#if 0
	  // Can't be done, Revert is not exposed for IAAFFile objects
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_SUCCESS == pFile->Revert(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
#endif
  }

  /* **************************************************************************
	 C) Open Success:
		3. Close after Open:
		   - Open(),
		   - then Close(),
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  // This test is also too basic!  Forget it!
  //   You call me "storm crow" eh?
  // Whenever you show up war increases!
  //   Not this time, just test it and you'll see!
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest C3." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_SUCCESS == pFile->Close(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 C) Open Success:
		4. GetHeader after Open:
		   - Open(),
		   - then GetHeader(),
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  // One more basic test!
  //   Now let's go for it.  We may yet return!
  // It's no use Sam!
  //   We will run this test if I have to carry you up the mountain!
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest C4." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );

		  IAAFHeader *				pTestHeader = NULL;
		  checkExpression( AAFRESULT_SUCCESS == pFile->GetHeader(&pTestHeader), AAFRESULT_TEST_FAILED );
		  checkExpression( NULL != pTestHeader, AAFRESULT_TEST_FAILED );
	  
		  // Cleanup for the next test
		  pTestHeader->Release(), pTestHeader = 0;
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 D) Not Open:
		1. Save before Open:
		   - Save(),
		   - expect AAFRESULT_NOT_OPEN
		   - Open() and Close(),
		   - then Save(),
		   - expect AAFRESULT_NOT_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest D1." << std::endl;
	  // First two sub-steps can't be done.  I don't know of a way to create an IAAFFile object unopened.
#if 0
	  checkExpression( AAFRESULT_NOT_OPEN == pFile->Save(), AAFRESULT_TEST_FAILED );
#endif
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenNewModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );

	  // Cleanup for the next test
	  pFile->Close();
  	  checkExpression( AAFRESULT_NOT_OPEN == pFile->Save(), AAFRESULT_TEST_FAILED );
	  pFile->Release(), pFile = 0;
	  RemoveTestFile(pFileName);
  }

  /* **************************************************************************
	 D) Not Open:
		2. Revert before Open:
		   - Revert(),
		   - expect AAFRESULT_NOT_OPEN
		   - Open() and Close(),
		   - then Revert(),
		   - expect AAFRESULT_NOT_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tSkipping ComprehensiveOpenTest D2." << std::endl;
#if 0
	  // First two sub-steps can't be done.  I don't know of a way to create an IAAFFile object unopened.
	  // But that is immaterial because there is no Revert method in IAAFFile.
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

#if 0
		  checkExpression( AAFRESULT_NOT_OPEN == pFile->Revert(), AAFRESULT_TEST_FAILED );
#endif
		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );

		  pFile->Close();
  		  checkExpression( AAFRESULT_NOT_OPEN == pFile->Revert(), AAFRESULT_TEST_FAILED );
		
		  // Cleanup for the next test
		pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
#endif
  }

  /* **************************************************************************
	 D) Not Open:
		3. Close before Open:
		   - Close(),
		   - expect AAFRESULT_NOT_OPEN
		   - Open() and Close(),
		   - then Close(),
		   - expect AAFRESULT_NOT_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest D3." << std::endl;
	  // First two sub-steps can't be done.  I don't know of a way to create an IAAFFile object unopened.
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

#if 0
		  checkExpression( AAFRESULT_NOT_OPEN == pFile->Close(), AAFRESULT_TEST_FAILED );
#endif
		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_SUCCESS == pFile->Close(), AAFRESULT_TEST_FAILED );
		  checkExpression( AAFRESULT_NOT_OPEN == pFile->Close(), AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }
  /* **************************************************************************
	 D) Not Open:
		4. GetHeader before Open:
		   - GetHeader(),
		   - expect AAFRESULT_NOT_OPEN
		   - Open() and Close(),
		   - then GetHeader(),
		   - expect AAFRESULT_NOT_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveOpenTest D4." << std::endl;
	  // First two sub-steps can't be done.  I don't know of a way to create an IAAFFile object unopened.
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  IAAFHeader *				pTestHeader = NULL;
#if 0
		  checkExpression( AAFRESULT_NOT_OPEN == pFile->GetHeader(&pTestHeader), AAFRESULT_TEST_FAILED );
		  checkExpression( NULL == pTestHeader, AAFRESULT_TEST_FAILED );
#endif
		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );
		  pFile->Close();

		  checkExpression( AAFRESULT_NOT_OPEN == pFile->GetHeader(&pTestHeader), AAFRESULT_TEST_FAILED );
		  checkExpression( NULL == pTestHeader, AAFRESULT_TEST_FAILED );
	  
		  // Cleanup for the next test
		  pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
  }


  HRESULT hr;
  if (f == 0) {
    hr = AAFRESULT_SUCCESS;
  } else {
    hr = AAFRESULT_TEST_FAILED;
  }
  return hr;
}


static HRESULT ComprehensiveSaveCopyAsTest(testMode_t /* mode */,
					   aafUID_t fileKind,
					   testRawStorageType_t rawStorageType,
					   aafProductIdentification_t productID)
{
  // Summary of tests
  //    A) Error cases:
  //	   1. Already Exists
  //	   2. Already Closed
  //
  //	B) Save from Various Opens:
  //	   1. SaveCopyAs from OpenExistingRead
  //	   2. SaveCopyAs from OpenExistingModify
  //	   3. SaveCopyAs from OpenNewModify
  //
  //	C) Save Success:
  //	   1. GetHeader after SaveCopyAs

  //	D) Read Success:
  //	   1. Read after SaveCopyAs

  int f = 0; // Local count of failures
  IAAFFile* pFile = 0;
  IAAFFile* pCopy = 0;
  
  // SaveCopyAs Test file present for functions that require one
  //
  const aafWChar * pFileName = L"SaveCopyAsTestF.ile";
  const aafWChar * pCopyName = L"CopyTestF.ile";
  RemoveTestFile(pFileName);
  RemoveTestFile(pCopyName);

  /* **************************************************************************
	 A) Error cases:
		1. Already Exists:
		   - file.Open(), file.Save(), file.Close(), file.Open()
		   - copy.Open(), copy.Close()
		   - then file.SaveCopyAs(copy)
		   - expect AAFRESULT_NOT_OPEN
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest A1." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );

		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkResult( pCopy->Close() );
			  checkExpression( AAFRESULT_NOT_OPEN == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pCopy->Release(), pCopy = 0;
		  }
		  else
		  {
			  f++;
		  }

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
  }

  /* **************************************************************************
	 A) Error cases:
		2. Already Closed:
		   - file.Open(), file.Close()
		   - copy.Open()
		   - then file.SaveCopyAs(copy)
		   - expect AAFRESULT_UNEXPECTED_EXCEPTION (not sure about this one)
  ************************************************************************** */
  {
	  std::cout << "\tSkipping ComprehensiveSaveCopyAsTest A2." << std::endl;
#if 0
	  // 14224 Segmentation fault in Linux.
	  // create the copy throws exception that cannot be handled in
	  // Linux (works fine in Windows XP).
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkExpression( AAFRESULT_SUCCESS == pFile->Close(), AAFRESULT_TEST_FAILED );
		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkExpression( AAFRESULT_UNEXPECTED_EXCEPTION == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pCopy->Close(), pCopy->Release(), pCopy = 0;
		  }
		  else
		  {
			  f++;
		  }

		  // Cleanup for the next test
		  pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
#endif
  }

  /* **************************************************************************
	 B) Save from Various Opens:
		1. SaveCopyAs from OpenExistingRead:
		   - Create file
		   - file.OpenExistingRead(),
		   - copy.Open(),
		   - file.SaveCopyAs(copy)
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest B1." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );

		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkExpression( AAFRESULT_SUCCESS == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pCopy->Close(), pCopy->Release(), pCopy = 0;
		  }
		  else
		  {
			  f++;
		  }

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
  }

  /* **************************************************************************
	 B) Save from Various Opens:
		2. SaveCopyAs from OpenExistingModify:
		   - Create file
		   - file.OpenExistingModify(),
		   - copy.Open(),
		   - file.SaveCopyAs(copy)
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest B2." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );

		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkExpression( AAFRESULT_SUCCESS == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pCopy->Close(), pCopy->Release(), pCopy = 0;
		  }
		  else
		  {
			  f++;
		  }

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
  }

  /* **************************************************************************
	 B) Save from Various Opens:
		3. SaveCopyAs from OpenNewModify:
		   - file.OpenNewModify(),
		   - copy.Open(),
		   - file.SaveCopyAs(copy)
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest B3." << std::endl;
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenNewModify( pFileName, 0, &productID, &pFile ), AAFRESULT_TEST_FAILED );

	  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
	  if (pCopy)
	  {
		  checkExpression( AAFRESULT_SUCCESS == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );
		  
		  // Cleanup for the next test
		  pCopy->Close(), pCopy->Release(), pCopy = 0;
	  }
	  else
	  {
		  f++;
	  }

	  // Cleanup for the next test
	  pFile->Close(), pFile->Release(), pFile = 0;
	  RemoveTestFile(pFileName);
 	  RemoveTestFile(pCopyName);
  }

  /* **************************************************************************
	 C) Save Success:
		1. GetHeader after SaveCopyAs:
		   - Create file
		   - file.OpenExistingRead(),
		   - copy.Open(),
		   - file.SaveCopyAs(copy)
		   - then copy.GetHeader(),
		   - expect AAFRESULT_SUCCESS
		   - then file.GetHeader(),
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest C1." << std::endl;
	  checkResult(CreateTestFile(pFileName, fileKind, rawStorageType, productID, &pFile));
	  if (pFile)
	  {
		  checkResult(pFile->Save());
		  checkResult(pFile->Close());
		  pFile->Release(), pFile = 0;

		  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );

  		  IAAFHeader *				pTestHeader = NULL;

		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkExpression( AAFRESULT_SUCCESS == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  checkExpression( AAFRESULT_SUCCESS == pCopy->GetHeader(&pTestHeader), AAFRESULT_TEST_FAILED );
			  checkExpression( NULL != pTestHeader, AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pTestHeader->Release(), pTestHeader = 0;
			  pCopy->Close(), pCopy->Release(), pCopy = 0;
		  }
		  else
		  {
			  f++;
		  }

		  checkExpression( AAFRESULT_SUCCESS == pFile->GetHeader(&pTestHeader), AAFRESULT_TEST_FAILED );
		  checkExpression( NULL != pTestHeader, AAFRESULT_TEST_FAILED );

		  // Cleanup for the next test
		  pTestHeader->Release(), pTestHeader = 0;
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
  }

  /* **************************************************************************
	 D) Read Success:
		1. Read after SaveCopyAs:
		   - CreateAAFFile(file)
		   - file.OpenExistingRead(),
		   - copy.Open(),
		   - file.SaveCopyAs(copy)
		   - then ReadAAFFile(copy)
		   - expect AAFRESULT_SUCCESS
  ************************************************************************** */
  {
	  std::cout << "\tExecuting ComprehensiveSaveCopyAsTest D1." << std::endl;
	  checkExpression( AAFRESULT_SUCCESS == CreateAAFFile( pFileName, fileKind, rawStorageType, productID ) );
	  checkExpression( AAFRESULT_SUCCESS == AAFFileOpenExistingRead( pFileName, 0, &pFile ), AAFRESULT_TEST_FAILED );
	  if( pFile )
	  {
		  checkResult(CreateTestFile(pCopyName, fileKind, rawStorageType, productID, &pCopy));
		  if (pCopy)
		  {
			  checkExpression( AAFRESULT_SUCCESS == pFile->SaveCopyAs( pCopy ), AAFRESULT_TEST_FAILED );

			  // Cleanup for the next test
			  pCopy->Close(), pCopy->Release(), pCopy = 0;

			  checkExpression( AAFRESULT_SUCCESS == ReadAAFFile( pCopyName ), AAFRESULT_TEST_FAILED );
		  }
		  else
		  {
			  f++;
		  }

		  // Cleanup for the next test
		  pFile->Close(), pFile->Release(), pFile = 0;
	  }
	  else
	  {
		  f++;
	  }
	  RemoveTestFile(pFileName);
	  RemoveTestFile(pCopyName);
  }



  HRESULT hr;
  if (f == 0) {
    hr = AAFRESULT_SUCCESS;
  } else {
    hr = AAFRESULT_TEST_FAILED;
  }
  return hr;
}


static HRESULT NegativeTestPublicGlobalFunctions(
                                          testMode_t /* mode */,
                                          aafUID_t fileKind,
                                          testRawStorageType_t rawStorageType,
                                          aafProductIdentification_t productID)
{

  // Public global functions taking a file name
  // AAFFileOpenExistingRead
  // AAFFileOpenExistingModify
  // AAFFileOpenNewModify
  // AAFFileOpenNewModifyEx
  // AAFFileIsAAFFile
  // AAFFileIsAAFFileKind
  // AAFCreateRawStorageDisk
  // AAFCreateRawStorageCachedDisk

  int f = 0; // Local count of failures
  IAAFFile* pFile = 0;
  aafProductIdentification_t* id = &productID;
  aafUID_t kind = EffectiveTestFileEncoding(fileKind);
  aafUID_t k;
  aafBool b;
  HRESULT h;
//  IAAFRawStorage* pStg = 0;

  // no file present for functions that require one
  //
  const aafWChar* pFileName = L"NoSuchF.ile";

  /* **************************************************************************
     HACK ALERT -- temporarily remove non-existing file and already existing
     file tests until OMStream divergence fixed.

  RemoveTestFile(pFileName);

  f += ExpectFail(AAFFileOpenExistingRead(pFileName, 0, &pFile));
  f += ExpectFail(AAFFileOpenExistingModify(pFileName, 0, id, &pFile));
  //              AAFFileOpenNewModify - NA
  //              AAFFileOpenNewModifyEx - NA
  ************************************************************************** */

  f += ExpectFail(AAFFileIsAAFFile(pFileName, &k, &b));
  f += ExpectFail(AAFFileIsAAFFileKind(pFileName, &kind, &b));

  /* **************************************************************************
     HACK ALERT -- temporarily remove non-existing file and already existing
     file tests until OMStream divergence fixed.
   

  f += ExpectFail(AAFCreateRawStorageDisk(pFileName,
                                         kAAFFileExistence_existing,
                                         kAAFFileAccess_read,
                                         &pStg));
  f += ExpectFail(AAFCreateRawStorageCachedDisk(pFileName,
                                               kAAFFileExistence_existing,
                                               kAAFFileAccess_read,
                                               16,
                                               4096,
                                               &pStg));

  // file present for functions that require it to be absent
  //
  pFileName = L"ExistingF.ile";
  RemoveTestFile(pFileName); // !
  h = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
  if (AAFRESULT_FAILED(h))
	  return h;

  //              AAFFileOpenExistingRead - NA
  //              AAFFileOpenExistingModify - NA
  f += ExpectFail(AAFFileOpenNewModify(pFileName, 0, id, &pFile));
  f += ExpectFail(AAFFileOpenNewModifyEx(pFileName, &kind, 0, id, &pFile));
  //              AAFFileIsAAFFile - NA
  //              AAFFileIsAAFFileKind - NA
  f += ExpectFail(AAFCreateRawStorageDisk(pFileName,
                                         kAAFFileExistence_new,
                                         kAAFFileAccess_modify,
                                         &pStg));
  f += ExpectFail(AAFCreateRawStorageCachedDisk(pFileName,
                                               kAAFFileExistence_new,
                                               kAAFFileAccess_modify,
                                               16,
                                               4096,
                                               &pStg));
  ************************************************************************** */

  // Zero length file present
  //
  pFileName = L"EmptyF.ile";
  RemoveTestFile(pFileName);
  h = CreateEmptyFile(pFileName);
  if (AAFRESULT_FAILED(h))
	  return h;

  f += ExpectFail(AAFFileOpenExistingRead(pFileName, 0, &pFile));
  f += ExpectFail(AAFFileOpenExistingModify(pFileName, 0, id, &pFile));
  // AAFFileOpenNewModify - NA
  // AAFFileOpenNewModifyEx - NA
  h = AAFFileIsAAFFile(pFileName, &k, &b);
  f += ExpectFalse(h, b);
  h = AAFFileIsAAFFileKind(pFileName, &kind, &b);
  f += ExpectFalse(h, b);
  // AAFCreateRawStorageDisk - NA
  // AAFCreateRawStorageCachedDisk - NA

  // Clean up
  RemoveTestFile(pFileName);

  HRESULT hr;
  if (f == 0) {
    hr = AAFRESULT_SUCCESS;
  } else {
    hr = AAFRESULT_TEST_FAILED;
  }
  return hr;
}

extern "C" HRESULT CAAFFile_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	HRESULT hr4K = AAFRESULT_SUCCESS;

	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if (mode == kAAFUnitTestReadWrite) {
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		} else {
			hr = AAFRESULT_SUCCESS;
		}

		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );

		if (hr == AAFRESULT_SUCCESS) {
			if (mode == kAAFUnitTestReadWrite) {
				hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
			} else {
				hr = AAFRESULT_SUCCESS;
			}
		}

		if (hr == AAFRESULT_SUCCESS) {
			hr = ReadAAFFile( pFileName );
		}

		if (hr == AAFRESULT_SUCCESS) {
		    hr = NegativeTestPublicGlobalFunctions(mode,
                                                   fileKind,
                                                   rawStorageType,
                                                   productID);
		}

		if (equalUID(fileKind, testFileKindDefault) && isSSFileKind(fileKind)) {
			aafUID_t fileKind4K=getSS4KFileKind(fileKind);

			aafWChar pFileName4K[ fileNameBufLen ] = L"";
			GenerateTestFileName( productID.productName, fileKind4K, fileNameBufLen, pFileName4K );

			if (mode == kAAFUnitTestReadWrite) {
				hr4K = CreateAAFFile(pFileName4K, fileKind4K, rawStorageType, productID);
			} else {
				hr4K = AAFRESULT_SUCCESS;
			}

			if (hr4K == AAFRESULT_SUCCESS)
				hr4K = ReadAAFFile( pFileName4K );

			if (hr4K == AAFRESULT_SUCCESS) {
				if (mode == kAAFUnitTestReadWrite) {
					hr4K = CreateAAFFile(pFileName4K, fileKind4K, rawStorageType, productID);
				} else {
					hr4K = AAFRESULT_SUCCESS;
				}
			}
			if (hr4K == AAFRESULT_SUCCESS) {
				hr4K = ReadAAFFile( pFileName4K );
			}
			if (hr4K == AAFRESULT_SUCCESS) {
				hr4K = NegativeTestPublicGlobalFunctions(mode,
													fileKind4K,
													rawStorageType,
													productID);
			}
		}
		if (hr == AAFRESULT_SUCCESS) {
		    hr = ComprehensiveOpenTest(mode,
                                       fileKind,
                                       rawStorageType,
									   productID);
		}
		if (hr == AAFRESULT_SUCCESS) {
			hr = ComprehensiveSaveCopyAsTest(mode,
											 fileKind,
											 rawStorageType,
											 productID);
		}
	}
	catch (...)
	{
		std::wcout << L"CAAFFile_test...Caught general C++" << " exception!" << std::endl;
		hr = AAFRESULT_TEST_FAILED;
	}

	if (hr != AAFRESULT_SUCCESS) {
		return hr;
	} else if (hr4K != AAFRESULT_SUCCESS) {
		return hr4K;
    } else if (SUCCEEDED(hr) && SUCCEEDED(hr4K)) {
		// Open() and SaveCopyAs() method were not in the current version
		// of the toolkit at the time this module test was written.

		// TODO: Implement tests for Open() and SaveCopyAs() now that they
		// are implemented in ImplAAFFile.cpp
		return AAFRESULT_NOT_IN_CURRENT_VERSION;
	}

	return AAFRESULT_SUCCESS;
}
