// @doc INTERNAL
// @com This file implements the module test for CAAFTextClip
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
#include <assert.h>
#include <memory.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"


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
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTextClip Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
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
    cerr << "CAAFTextClip_test...Caught general C++ exception!" << endl;
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


  try
  {
  //Make the MOB to be referenced
  checkResult(_pDictionary->CreateInstance(AUID_AAFMasterMob,
					  IID_IAAFMasterMob, 
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


  // Create a TextClip
  checkResult(_pDictionary->CreateInstance(AUID_AAFTextClip,
					  IID_IAAFTextClip, 
					  (IUnknown **)&pTextClip));

  // Initialize the source reference data.
  checkResult(pTextClip->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceReference));
  checkResult(pSourceReference->SetSourceID(_referencedMobID));
  checkResult(pSourceReference->SetSourceMobSlotID(0));

  // Create a composition mob to hold the text clip.
  checkResult(_pDictionary->CreateInstance(AUID_AAFCompositionMob,
					  IID_IAAFCompositionMob, 
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
    checkResult(_pHeader->CountMobs(kCompMob, &compositionMobs));
    checkExpression(1 == compositionMobs, AAFRESULT_TEST_FAILED);

    // Get the composition mob. There should only be one.
    aafSearchCrit_t criteria;
    criteria.searchTag = kByMobKind;
    criteria.tags.mobKind = kCompMob;
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
