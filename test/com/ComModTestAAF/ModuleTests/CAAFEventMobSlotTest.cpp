// @doc INTERNAL
// @com This file implements the module test for CAAFEventMobSlot
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFEventMobSlot.h"
#include "CAAFEventMobSlot.h"
#ifndef __CAAFEventMobSlot_h__
#error - improperly defined include guard
#endif


#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"


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


class EventMobSlotTest
{
public:
  EventMobSlotTest();
  ~EventMobSlotTest();

  void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
  void Open(wchar_t *pFileName);
  void Close();

  void CreateEventMobSlot();
  void OpenEventMobSlot();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;
  aafUID_t _compositionMobID;

  // EventMobSlot static data
  static const aafRational_t _editRate;
  // Event static data
  static const aafPosition_t _position;
  static const wchar_t* _slotName;
};


HRESULT CAAFEventMobSlot::test()
{
  HRESULT hr = S_OK;
  aafProductIdentification_t	ProductInfo = {0};
  aafWChar * pFileName = L"EventMobSlotTest.aaf";

  // Initialize the product info for this module test
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFEventMobSlot Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = -1;
  ProductInfo.platform = NULL;

  // Create an instance of our text clip test class and run the
  // tests...
  EventMobSlotTest test;

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
    cerr << "CAAFEventMobSlot::test...Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Test edit rate for the event mob slot.
const aafRational_t EventMobSlotTest::_editRate = { 2997, 100 };
const aafPosition_t _position = 0;
const wchar_t *_eventComment = L"Event::Comment:This is a test event";

EventMobSlotTest::EventMobSlotTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
  memset(&_compositionMobID, 0, sizeof(_compositionMobID));
}

EventMobSlotTest::~EventMobSlotTest()
{
  Close();
}


void EventMobSlotTest::Create(wchar_t *pFileName,
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
  CreateEventMobSlot();

  // cleanup
  Close();
}


void EventMobSlotTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the text clip.
  OpenEventMobSlot();

  // cleanup
  Close();
}


void EventMobSlotTest::Close()
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


void EventMobSlotTest::CreateEventMobSlot()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  IAAFEvent *pEvent = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFMob *pMob = NULL;


  try
  {
    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    checkResult(_pDictionary->CreateInstance(&AUID_AAFEvent,
                                             IID_IAAFEvent, 
                                             (IUnknown **)&pEvent));
    checkResult(pEvent->SetPosition(0));
    checkResult(pEvent->SetComment(L"Event::Comment:This is a test event"));

    // Get the segment inteface to add to the mob slot
    checkResult(pEvent->QueryInterface(IID_IAAFSegment, (void **)&pSegment));

    // Create and initialize an EventMobSlot
    checkResult(_pDictionary->CreateInstance(&AUID_AAFEventMobSlot,
                                             IID_IAAFEventMobSlot, 
                                             (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));

    // Create the mob to hold the new event mob slot.
    checkResult(_pDictionary->CreateInstance(&AUID_AAFCompositionMob,
                                             IID_IAAFMob, 
                                             (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));

    // Attach the mob to the header...
    checkResult(_pHeader->AppendMob(pMob));

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


void EventMobSlotTest::OpenEventMobSlot()
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


  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(&_compositionMobID, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->EnumAAFAllMobSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFEvent, (void **)&pEvent));


    
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











