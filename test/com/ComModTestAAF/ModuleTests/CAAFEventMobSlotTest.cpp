// @doc INTERNAL
// @com This file implements the module test for CAAFEventMobSlot
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
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
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

static const aafUID_t DDEF_TEST = 
{ 0x81831639, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

class EventMobSlotTest
{
public:
  EventMobSlotTest();
  ~EventMobSlotTest();

  void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
  void Open(wchar_t *pFileName);
  void Close();

  IAAFEvent *CreateAnEvent(aafPosition_t* position, 
                           wchar_t *comment, 
                           IAAFSequence *pSequence = NULL);

  void CreateEventMobSlot();
  void OpenEventMobSlot();

  void CreateEventSequenceMobSlot();
  void OpenEventSequenceMobSlot();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;
  aafMobID_t _eventMobID1;
  aafMobID_t _eventMobID2;

  // EventMobSlot static data
  static const aafRational_t _editRate;
  // Event static data
  static const aafPosition_t _position;
  static const wchar_t* _slotName;
};


extern "C" HRESULT CAAFEventMobSlot_test()
{
  HRESULT hr = S_OK;
  aafProductIdentification_t	ProductInfo = {0};
  aafWChar * pFileName = L"AAFEventMobSlotTest.aaf";

  // Initialize the product info for this module test
  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFEventMobSlot Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
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
    cerr << "CAAFEventMobSlot_test..."
		 << "Caught general C++ exception!" << endl;
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
  memset(&_eventMobID1, 0, sizeof(_eventMobID1));
  memset(&_eventMobID2, 0, sizeof(_eventMobID1));
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

  // Make a simple event mob slot
  CreateEventMobSlot();

  // Make a more complex mob slot.
  CreateEventSequenceMobSlot();

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

  // Open and validate a mob slot with a sequence of events.
  OpenEventSequenceMobSlot();

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


IAAFEvent *EventMobSlotTest::CreateAnEvent(aafPosition_t* position,
                                           wchar_t *comment,
                                           IAAFSequence *pSequence)
{
  assert(_pHeader && _pDictionary);
  assert(position && comment);

  IAAFEvent *pEvent = NULL;
  IAAFComponent *pComponent = NULL;
  IAAFDataDef *pDataDef = NULL;
  IAAFComponent *pComp = NULL;
  AAFRESULT	hr;

  CAAFBuiltinDefs defs (_pDictionary);

  try
  {
	  // not already in dictionary
		checkResult(defs.cdDataDef()->
					CreateInstance (IID_IAAFDataDef,
									(IUnknown **)&pDataDef));
	  hr = pDataDef->Initialize (DDEF_TEST, L"Test", L"Test data");
	  hr = _pDictionary->RegisterDataDef (pDataDef);
	// Create a concrete subclass of event
    checkResult(defs.cdCommentMarker()->
				CreateInstance(IID_IAAFEvent, 
							   (IUnknown **)&pEvent));
    checkResult(pEvent->SetPosition(*position));
    checkResult(pEvent->SetComment(L"Event::Comment:This is a test event"));
	checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pComp));
	checkResult(pComp->SetDataDef(defs.ddPicture()));
	pComp->Release();
	pComp = NULL;

    if (NULL != pSequence)
    {
      // Get the segment inteface to add to the mob slot
      checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
	  CAAFBuiltinDefs defs (_pDictionary);
	  checkResult(pComponent->SetDataDef(defs.ddPicture()));

      // Add the event to the sequence.
      checkResult(pSequence->AppendComponent(pComponent));
      pComponent->Release();
      pComponent = NULL;      
    }
  }
  catch (...)
  {
    if (pComponent)
    {
      pComponent->Release();
      pComponent = NULL;
    }

    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }
    
    throw;
  }
  
  return pEvent;
}

void EventMobSlotTest::CreateEventMobSlot()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  aafPosition_t position;
  IAAFEvent *pEvent = NULL;
  IAAFComponent *pComponent = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFMob *pMob = NULL;

  CAAFBuiltinDefs defs (_pDictionary);

  try
  {
    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 0;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event");

    // Get the segment inteface to add to the mob slot
    checkResult(pEvent->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
    pEvent->Release();
    pEvent = NULL;

    // Create and initialize an EventMobSlot
    checkResult(defs.cdEventMobSlot()->
				CreateInstance(IID_IAAFEventMobSlot, 
							   (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
    pEventMobSlot->Release();
    pEventMobSlot = NULL;

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));
    pSegment->Release();
    pSegment = NULL;

    // Create the mob to hold the new event mob slot.
    checkResult(defs.cdCompositionMob()->
				CreateInstance(IID_IAAFMob, 
							   (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));
    pMobSlot->Release();
    pMobSlot = NULL;

    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->GetMobID(&_eventMobID1));
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
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
 
    // Propogate the error.
    throw;
  }
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
    checkResult(_pHeader->LookupMob(_eventMobID1, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFEvent, (void **)&pEvent));

    pEvent->Release();
    pEvent = NULL;
    pSegment->Release();
    pSegment = NULL;
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
    pMobSlot->Release();
    pMobSlot = NULL;
    pEnumSlots->Release();
    pEnumSlots = NULL;
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
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
 
    // Propogate the error.
    throw; 
  }
}


void EventMobSlotTest::CreateEventSequenceMobSlot()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  aafPosition_t position;
  IAAFSequence *pSequence = NULL;
  IAAFEvent *pEvent = NULL;
  IAAFComponent *pComponent = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFMob *pMob = NULL;

  try
  {
	CAAFBuiltinDefs defs (_pDictionary);

    // Create a sequence to hold our list of events.
    checkResult(defs.cdSequence()->
				CreateInstance(IID_IAAFSequence, 
							   (IUnknown **)&pSequence));
     checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent));

	 checkResult(pComponent->SetDataDef(defs.ddPicture()));
	 pComponent->Release();
	 pComponent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 0;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 0", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 1;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 1", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 1;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 2", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Get the segment inteface to add to the mob slot
    checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
    pSequence->Release();
    pSequence = NULL;

    // Create and initialize an EventMobSlot
    checkResult(defs.cdEventMobSlot()->
				CreateInstance(IID_IAAFEventMobSlot, 
							   (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
    pEventMobSlot->Release();
    pEventMobSlot = NULL;

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));
    pSegment->Release();
    pSegment = NULL;

    // Create the mob to hold the new event mob slot.
    checkResult(defs.cdCompositionMob()->
				CreateInstance(IID_IAAFMob, 
							   (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));
    pMobSlot->Release();
    pMobSlot = NULL;

    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->GetMobID(&_eventMobID2));
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
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

    if (pComponent)
    {
      pComponent->Release();
      pComponent = NULL;
    }

    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }

    if (pSequence)
    {
      pSequence->Release();
      pSequence = NULL;
    }
 
    // Propogate the error.
    throw;
  }
}


void EventMobSlotTest::OpenEventSequenceMobSlot()
{
  assert(_pHeader);

  HRESULT hr = S_OK;
  IAAFMob *pMob = NULL;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  aafRational_t editRate = {0};
  IAAFSegment *pSegment = NULL;
  IAAFSequence *pSequence = NULL;


  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(_eventMobID2, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **)&pSequence));


    pSequence->Release();
    pSequence = NULL;
    pSegment->Release();
    pSegment = NULL;
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
    pMobSlot->Release();
    pMobSlot = NULL;
    pEnumSlots->Release();
    pEnumSlots = NULL;
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
    // Cleanup local references
    if (pSequence)
    {
      pSequence->Release();
      pSequence = NULL;
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
 
    // Propogate the error.
    throw; 
  }
}






