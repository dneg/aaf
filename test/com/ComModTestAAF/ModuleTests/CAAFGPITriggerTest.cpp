// @doc INTERNAL
// @com This file implements the module test for CAAFGPITrigger
/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
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
#include <stdlib.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
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

static const aafUID_t DDEF_TEST = 
{ 0x81831639, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };


class GPITriggerTest
{
public:
  GPITriggerTest();
  ~GPITriggerTest();

  void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
  void Open(wchar_t *pFileName);
  void Close();

  void CreateGPITrigger();
  void OpenGPITrigger();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;
  aafMobID_t _compositionMobID;

  // MobSlot static data
  static const wchar_t* _slotName;

  // EventMobSlot static data
  static const aafRational_t _editRate;
  // GPITrigger static data
  static const aafPosition_t _position;
  static const wchar_t* _eventComment;
  static const aafBool _triggerState;
};

const aafUID_t NIL_UID = { 0 };

extern "C" HRESULT CAAFGPITrigger_test()
{
  HRESULT hr = S_OK;
  aafProductIdentification_t	ProductInfo = {0};
  aafWChar * pFileName = L"AAFGPITriggerTest.aaf";

  // Initialize the product info for this module test
  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFGPITrigger Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  // Create an instance of our text clip test class and run the
  // tests...
  GPITriggerTest test;

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
    cerr << "CAAFGPITriggerMobSlot_test..."
		 << "Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Test edit rate for the event mob slot.
const aafRational_t GPITriggerTest::_editRate = { 2997, 100 };
const aafPosition_t GPITriggerTest::_position = 0;
const wchar_t *GPITriggerTest::_eventComment = L"GPITrigger::Comment:This is a test event";

GPITriggerTest::GPITriggerTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
  memset(&_compositionMobID, 0, sizeof(_compositionMobID));
}

GPITriggerTest::~GPITriggerTest()
{
  Close();
}


void GPITriggerTest::Create(wchar_t *pFileName,
                              aafProductIdentification_t* pinfo)
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
  CreateGPITrigger();

  // cleanup
  Close();
}


void GPITriggerTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the text clip.
  OpenGPITrigger();

  // cleanup
  Close();
}


void GPITriggerTest::Close()
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


void GPITriggerTest::CreateGPITrigger()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  IAAFGPITrigger *pTrigger = NULL;
  IAAFEvent		 *pEvent = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFDataDef *pDataDef = NULL;
  IAAFComponent *pComp = NULL;
  IAAFMob *pMob = NULL;

  CAAFBuiltinDefs defs (_pDictionary);

  try
  {
	  // not already in dictionary
		checkResult(defs.cdDataDef()->
					CreateInstance (IID_IAAFDataDef,
									(IUnknown **)&pDataDef));
	  hr = pDataDef->Initialize (DDEF_TEST, L"Test", L"Test data");
	  hr = _pDictionary->RegisterDataDef (pDataDef);

	// Create a GPITrigger
    checkResult(defs.cdGPITrigger()->
				CreateInstance(IID_IAAFGPITrigger, 
							   (IUnknown **)&pTrigger));
	checkResult(pTrigger->SetActiveState (kAAFTrue));
	checkResult(pTrigger->QueryInterface(IID_IAAFEvent, (void **)&pEvent));
    checkResult(pEvent->SetPosition(_position));
    checkResult(pEvent->SetComment(const_cast<wchar_t*>(_eventComment)));
	checkResult(pTrigger->QueryInterface(IID_IAAFComponent, (void **)&pComp));
	checkResult(pComp->SetDataDef(pDataDef));
	pComp->Release();
	pComp = NULL;

    // Get the segment inteface to the trigger to install into the mob slot.
    checkResult(pTrigger->QueryInterface(IID_IAAFSegment, (void **)&pSegment));

    // Create and initialize an EventMobSlot
    checkResult(defs.cdEventMobSlot()->
				CreateInstance(IID_IAAFEventMobSlot, 
							   (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));

    // Create the mob to hold the new event mob slot.
    checkResult(defs.cdCompositionMob()->
				CreateInstance(IID_IAAFMob, 
							   (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));

    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->GetMobID(&_compositionMobID));
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pMob)
  {
    pMob->Release();
    pMob = NULL;
  }

  if (pDataDef)
  {
    pDataDef->Release();
    pDataDef = NULL;
  }

  if (pComp)
  {
    pComp->Release();
    pComp = NULL;
  }

  if (pMobSlot)
  {
    pMobSlot->Release();
    pMobSlot = NULL;
  }

  if (pEventMobSlot)
  {
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pTrigger)
  {
    pTrigger->Release();
    pTrigger = NULL;
  }



  // Propogate the error if necessary.
  checkResult(hr);
}


void GPITriggerTest::OpenGPITrigger()
{
  assert(_pHeader);

  HRESULT hr = S_OK;
  IAAFMob *pMob = NULL;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  aafRational_t editRate = {0};
  IAAFSegment *pSegment = NULL;
  IAAFGPITrigger *pTrigger = NULL;
  IAAFEvent		 *pEvent = NULL;
  aafPosition_t position;
  wchar_t eventComment[128];
  aafBool		testTrigger;

  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(_compositionMobID, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFEvent, (void **)&pEvent));

    // Now validate the event property values we thought were written to the
    // file.

    // Validate the position
    checkResult(pEvent->GetPosition(&position));
    checkExpression(0 == memcmp(&position, &_position, sizeof(position)), AAFRESULT_TEST_FAILED);

    // Validate the comment buffer size.
    aafUInt32 expectedLen = wcslen(_eventComment) + 1;
    aafUInt32 expectedSize = expectedLen * sizeof(wchar_t);
    aafUInt32 commentBufSize = 0;
    checkResult(pEvent->GetCommentBufLen(&commentBufSize));
    checkExpression(commentBufSize == expectedSize, AAFRESULT_TEST_FAILED);
    
    // Validate the event comment.
    checkExpression(commentBufSize <= sizeof(eventComment), AAFRESULT_TEST_FAILED);
    checkResult(pEvent->GetComment(eventComment, commentBufSize));
    checkExpression(0 == memcmp(eventComment, _eventComment, commentBufSize), AAFRESULT_TEST_FAILED); 

	// Validate the trigger state
    checkResult(pSegment->QueryInterface(IID_IAAFGPITrigger, (void **)&pTrigger));
    checkResult(pTrigger->GetActiveState(&testTrigger));
    checkExpression(kAAFTrue == testTrigger, AAFRESULT_TEST_FAILED); 
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pEvent)
  {
    pEvent->Release();
    pEvent = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pEventMobSlot)
  {
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
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

  if (pMob)
  {
    pMob->Release();
    pMob = NULL;
  }



  // Propogate the error if necessary.
  checkResult(hr);
}

