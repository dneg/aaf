
#if(0)

// @doc INTERNAL
// @com This file implements the module test for CAAFTextClip
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


#include <iostream>
using namespace std;
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

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


class TextClipTest
{
public:
  TextClipTest();
  ~TextClipTest();

  void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
  void Open(wchar_t *pFileName);
  void Close();

  void CreateTextClip();
  void OpenTextClip();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;
  aafMobID_t _referencedMobID; // save id for validation in the open test...
};


extern "C" HRESULT CAAFTextClip_test()
{
  HRESULT hr = S_OK;
  aafProductIdentification_t	ProductInfo = {0};
  aafWChar * pFileName = L"AAFTextClipTest.aaf";

  // Initialize the product info for this module test
  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTextClip Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  // Create an instance of our text clip test class and run the
  // tests...
  TextClipTest test;

  try
  {
    // Attempt to create a test file
    test.Create(pFileName, &ProductInfo);

    // Attempt to read the test file.
    test.Open(pFileName);
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
  }
  catch (...)
  {
    cerr << "CAAFTextClip_test..."
		 << "Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}


TextClipTest::TextClipTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
  memset(&_referencedMobID, 0, sizeof(_referencedMobID));
}

TextClipTest::~TextClipTest()
{
  Close();
}


void TextClipTest::Create(wchar_t *pFileName, aafProductIdentification_t* pinfo)
{
  assert(NULL == _pFile);

  // Remove the previous test file if any.
  RemoveTestFile(pFileName);
    
  // Create the file
  checkResult(AAFFileOpenNewModify(pFileName, 0, pinfo, &_pFile));
  _bWritableFile = true;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  checkResult(_pHeader->GetDictionary(&_pDictionary));

  // Make a text clip.
  CreateTextClip();

  // cleanup
  Close();
}


void TextClipTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the text clip.
  OpenTextClip();

  // cleanup
  Close();
}


void TextClipTest::Close()
{
  if (_pDictionary)
  {
    _pDictionary->Release();
    _pDictionary = NULL;
  }

  if (_pHeader)
  {
    _pHeader->Release();
    _pHeader = NULL;
  }
  
  if (_pFile)
  {
    if (_bWritableFile)
      checkResult(_pFile->Save());

    checkResult(_pFile->Close());

    _pFile->Release();
    _pFile = NULL;
  }  
}


void TextClipTest::CreateTextClip()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  IAAFMasterMob *pMasterMob = NULL;
  IAAFMob *pReferencedMob = NULL;
  IAAFTextClip *pTextClip = NULL;
  IAAFSourceReference *pSourceReference = NULL;
  IAAFCompositionMob *pCompositionMob = NULL;
  IAAFMob *pReferencingMob = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFTimelineMobSlot *pMobSlot = NULL;
	IAAFComponent*		pComponent = NULL;

  try
  {
	CAAFBuiltinDefs defs (_pDictionary);

	//Make the MOB to be referenced
	checkResult(defs.cdMasterMob()->
				CreateInstance(IID_IAAFMasterMob, 
							   (IUnknown **)&pMasterMob));
  
  checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pReferencedMob));
  checkResult(pReferencedMob->GetMobID(&_referencedMobID));
  checkResult(pReferencedMob->SetName(L"TextClipTest::ReferencedMob"));

  // Save the master mob.
  checkResult(_pHeader->AddMob(pReferencedMob));

  // Use EssenceAccess to write some text essence
    // Create a file mob for the text essence.
    // Create the corresponding text essence.
    // Write some text essence.


  // Create a concrete subclass of TextClip
  checkResult(defs.cdHTMLClip()->
			  CreateInstance(IID_IAAFTextClip, 
							 (IUnknown **)&pTextClip));
  checkResult(pTextClip->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
  checkResult(pComponent->SetDataDef(defs.ddPicture()));
  pComponent->Release();
  pComponent = NULL;

  // Initialize the source reference data.
  checkResult(pTextClip->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceReference));
  checkResult(pSourceReference->SetSourceID(_referencedMobID));
  checkResult(pSourceReference->SetSourceMobSlotID(0));

  // Create a composition mob to hold the text clip.
  checkResult(defs.cdCompositionMob()->
			  CreateInstance(IID_IAAFCompositionMob, 
							 (IUnknown **)&pCompositionMob));
  checkResult(pCompositionMob->QueryInterface(IID_IAAFMob, (void **)&pReferencingMob));
  checkResult(pReferencingMob->SetName(L"CompositionMob_TextClipTest"));
 
  
  checkResult(pTextClip->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
  IAAFMobSlot *pSlot = NULL;
  aafRational_t editRate = { 0, 1};
  checkResult(pReferencingMob->AppendNewTimelineSlot(editRate,
													 pSegment,
													 1,
													 L"TextClipTest",
													 0,
													 &pMobSlot));


  // Save the referencing mob.
  checkResult(_pHeader->AddMob(pReferencingMob));
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pMobSlot)
  {
    pMobSlot->Release();
    pMobSlot = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pComponent)
  {
    pComponent->Release();
    pComponent = NULL;
  }

  if (pReferencingMob)
  {
    pReferencingMob->Release();
    pReferencingMob = NULL;
  }

  if (pCompositionMob)
  {
    pCompositionMob->Release();
    pCompositionMob = NULL;
  }

  if (pSourceReference)
  {
    pSourceReference->Release();
    pSourceReference = NULL;
  }

  if (pTextClip)
  {
    pTextClip->Release();
    pTextClip = NULL;
  }

  if (pReferencedMob)
  {
    pReferencedMob->Release();
    pReferencedMob = NULL;
  }

  if (pMasterMob)
  {
    pMasterMob->Release();
    pMasterMob = NULL;
  }


  // Propogate the error if necessary.
  checkResult(hr);
}


void TextClipTest::OpenTextClip()
{
  assert(_pHeader);

  HRESULT hr = S_OK;
  aafNumSlots_t compositionMobs = 0;
  IEnumAAFMobs *pEnumMobs = NULL;
  IAAFMob *pReferencingMob = NULL;
  IAAFCompositionMob *pCompositionMob = NULL;
  aafNumSlots_t mobSlots = 0;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFTextClip *pTextClip = NULL;
  IAAFSourceReference *pSourceReference = NULL;
  aafMobID_t masterMobID = {0};
  IAAFMasterMob *pMasterMob = NULL;
  IAAFMob *pReferencedMob = NULL;


  try
  {
    // Get the number of composition mobs in the file (should be one)
    checkResult(_pHeader->CountMobs(kAAFCompMob, &compositionMobs));
    checkExpression(1 == compositionMobs, AAFRESULT_TEST_FAILED);

    // Get the composition mob. There should only be one.
    aafSearchCrit_t criteria;
    criteria.searchTag = kAAFByMobKind;
    criteria.tags.mobKind = kAAFCompMob;
    checkResult(_pHeader->GetMobs(&criteria, &pEnumMobs));
    checkResult(pEnumMobs->NextOne(&pReferencingMob));
    checkResult(pReferencingMob->QueryInterface(IID_IAAFCompositionMob, (void **)&pCompositionMob));

    // Get the text clip in the slot. There should be only one.
    checkResult(pReferencingMob->CountSlots(&mobSlots));
    checkExpression(1 == mobSlots, AAFRESULT_TEST_FAILED);
    checkResult(pReferencingMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFTextClip, (void **)&pTextClip));

    // Attempt to "resolve" the reference and find the master mob.
    checkResult(pTextClip->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceReference));  
    checkResult(pSourceReference->GetSourceID(&masterMobID));
    checkExpression(0 == memcmp(&masterMobID, &_referencedMobID, sizeof(masterMobID)), AAFRESULT_TEST_FAILED);
    checkResult(_pHeader->LookupMob(masterMobID, &pReferencedMob));
    checkResult(pReferencedMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

    
    // Use EssenceAccess to read some text essence
      // Find the file mob for the text essence.
      // Find the corresponding text essence.
      // Read some text essence.
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pMasterMob)
  {
    pMasterMob->Release();
    pMasterMob = NULL;
  }

  if (pReferencedMob)
  {
    pReferencedMob->Release();
    pReferencedMob = NULL;
  }

  if (pSourceReference)
  {
    pSourceReference->Release();
    pSourceReference = NULL;
  }

  if (pTextClip)
  {
    pTextClip->Release();
    pTextClip = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pMobSlot)
  {
    pMobSlot->Release();
    pMobSlot = NULL;
  }

  if (pEnumSlots)
  {
    pEnumSlots->Release();
    pEnumSlots = NULL;
  }

  if (pCompositionMob)
  {
    pCompositionMob->Release();
    pCompositionMob = NULL;
  }

  if (pReferencingMob)
  {
    pReferencingMob->Release();
    pReferencingMob = NULL;
  }

  if (pEnumMobs)
  {
    pEnumMobs->Release();
    pEnumMobs = NULL;
  }

  // Propogate the error if necessary.
  checkResult(hr);
}

#endif
