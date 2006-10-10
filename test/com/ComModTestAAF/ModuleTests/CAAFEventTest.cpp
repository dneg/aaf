
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
#include <stdlib.h>
#include <wchar.h>
#include "AAFDefUIDs.h"

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "CAAFBuiltinDefs.h"
//{060c2b340205110101001000-13-00-00-00-{f5fedc56-8d6f-11d4-a380-009027dfca6a}}

static const aafMobID_t gMobID = {

{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 

0x13, 0x00, 0x00, 0x00, 

{0xf5fedc56, 0x8d6f, 0x11d4, {0xa3, 0x80, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}}};


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

static const aafUID_t kAAFDataDef_Test = 
{ 0x81831639, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };


class EventTest
{
public:
  EventTest();
  ~EventTest();

  void Create(wchar_t *pFileName,
              aafUID_constref fileKind,
              testRawStorageType_t rawStorageType,
              aafProductIdentification_constref productID);
  void Open(wchar_t *pFileName);
  void Close();

  void CreateEvent();
  void OpenEvent();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;

  // MobSlot static data
  static const wchar_t* _slotName;

  // EventMobSlot static data
  static const aafRational_t _editRate;
  // Event static data
  static const aafPosition_t _position;
  static const wchar_t* _eventComment;
};

const aafUID_t NIL_UID = { 0 };

extern "C" HRESULT CAAFEvent_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFEvent_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = S_OK;
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  // Create an instance of our text clip test class and run the
  // tests...
  EventTest test;

  try
  {
    // Attempt to create a test file
	if(mode == kAAFUnitTestReadWrite)
	    test.Create(pFileName, fileKind, rawStorageType, productID);

    // Attempt to read the test file.
    test.Open(pFileName);
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
  }
  catch (...)
  {
    cerr << "CAAFEventMobSlot_test..."
		 << "Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Test edit rate for the event mob slot.
const aafRational_t EventTest::_editRate = { 2997, 100 };
const aafPosition_t EventTest::_position = 0;
const wchar_t *EventTest::_eventComment = L"Event::Comment:This is a test event";

EventTest::EventTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
}

EventTest::~EventTest()
{
  Close();
}


void EventTest::Create(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  assert(NULL == _pFile);

  // Remove the previous test file if any.
  RemoveTestFile(pFileName);
    
  // Create the file
  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &_pFile ));
  _bWritableFile = true;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  checkResult(_pHeader->GetDictionary(&_pDictionary));

  // Make a text clip.
  CreateEvent();

  // cleanup
  Close();
}


void EventTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the text clip.
  OpenEvent();

  // cleanup
  Close();
}


void EventTest::Close()
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


void EventTest::CreateEvent()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  IAAFEvent *pEvent = NULL;
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
	  hr = pDataDef->Initialize (kAAFDataDef_Test, L"Test", L"Test data");
	  hr = _pDictionary->RegisterDataDef (pDataDef);

	// Create a concrete subclass of event
    checkResult(defs.cdCommentMarker()->
				CreateInstance(IID_IAAFEvent, 
							   (IUnknown **)&pEvent));
    checkResult(pEvent->SetPosition(_position));
    checkResult(pEvent->SetComment(const_cast<wchar_t*>(_eventComment)));
	checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pComp));
	checkResult(pComp->SetDataDef(pDataDef));
	pComp->Release();
	pComp = NULL;

    // Get the segment inteface to the event to install into the mob slot.
    checkResult(pEvent->QueryInterface(IID_IAAFSegment, (void **)&pSegment));

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

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->SetMobID(gMobID));
    
    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

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

  if (pEvent)
  {
    pEvent->Release();
    pEvent = NULL;
  }



  // Propogate the error if necessary.
  checkResult(hr);
}


void EventTest::OpenEvent()
{
  assert(_pHeader);

  HRESULT hr = S_OK;
  IAAFMob *pMob = NULL;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  aafRational_t editRate = {0};
  IAAFSegment *pSegment = NULL;
  IAAFEvent *pEvent = NULL;
  aafPosition_t position;
  wchar_t eventComment[128];


  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(gMobID, &pMob));

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

