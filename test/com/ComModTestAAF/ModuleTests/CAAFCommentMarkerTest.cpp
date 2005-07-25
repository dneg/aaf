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

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"

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


// {81831639-EDF4-11d3-A353-009027DFCA6A}
static const aafUID_t kAAFDataDef_Test = 
{ 0x81831639, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };


class CommentMarkerTest
{
public:
	CommentMarkerTest();
	~CommentMarkerTest();
	
	void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
	void Open(wchar_t *pFileName);
	void Close();
	
	void CreateEvent();
	void OpenEvent();
	
private:
	IAAFFile *_pFile;
	bool _bWritableFile;
	IAAFHeader *_pHeader;
	IAAFDictionary *_pDictionary;
	static const aafMobID_t _compositionMobID;
	
	// MobSlot static data
	static const wchar_t* _slotName;
	
	// EventMobSlot static data
	static const aafRational_t _editRate;
	// Event static data
	static const aafPosition_t _position;
	static const wchar_t* _eventComment;
};

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

extern "C" HRESULT CAAFCommentMarker_test(testMode_t mode);
extern "C" HRESULT CAAFCommentMarker_test(testMode_t mode)
{
	HRESULT hr = S_OK;
	aafProductIdentification_t	ProductInfo = {0};
	aafWChar * pFileName = L"AAFCommentMarkerTest.aaf";
	
	// Initialize the product info for this module test
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFCommentMarker Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	// Create an instance of our text clip test class and run the
	// tests...
	CommentMarkerTest test;
	
	try
	{
		// Attempt to create a test file
		if(mode == kAAFUnitTestReadWrite)
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
		cerr << "CAAFCommentMarker_test..."
			 << "Caught general C++ exception!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}
	
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFCommentMarker methods have not been tested:" << endl;       
//		cout << "     GetAnnotation" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}

// Test edit rate for the event mob slot.
const aafRational_t CommentMarkerTest::_editRate = { 2997, 100 };
const aafPosition_t CommentMarkerTest::_position = 0;
const wchar_t *CommentMarkerTest::_eventComment = L"Event::Comment:This is a test event";
const aafMobID_t CommentMarkerTest::_compositionMobID = 
	{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x3e2db3b8, 0x0a61, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


CommentMarkerTest::CommentMarkerTest() :
_pFile(NULL),
_bWritableFile(false),
_pHeader(NULL),
_pDictionary(NULL)
{
	//--cf no need ... memset(&_compositionMobID, 0, sizeof(_compositionMobID));
	;
}

CommentMarkerTest::~CommentMarkerTest()
{
	Close();
}


void CommentMarkerTest::Create(wchar_t *pFileName,
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
	CreateEvent();
	
	// cleanup
	Close();
}


void CommentMarkerTest::Open(wchar_t *pFileName)
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


void CommentMarkerTest::Close()
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


void CommentMarkerTest::CreateEvent()
{
	assert(_pHeader && _pDictionary);
	
	HRESULT hr = S_OK;
	IAAFEvent *pEvent = NULL;
	IAAFCommentMarker *pMarker = NULL;
	IAAFSourceReference *pClip = NULL;
	IAAFEventMobSlot *pEventMobSlot = NULL;
	IAAFSegment *pSegment = NULL;
	IAAFMobSlot *pMobSlot = NULL;
	IAAFMob *pMob = NULL;
	IAAFDataDef *pDataDef = NULL;
	IAAFComponent *pComp = NULL;
	
	
	CAAFBuiltinDefs defs (_pDictionary);

	try
	{
	  // not already in dictionary
		checkResult(defs.cdDataDef()->
					CreateInstance (IID_IAAFDataDef,
									(IUnknown **)&pDataDef));
	  hr = pDataDef->Initialize (kAAFDataDef_Test, L"Test", L"Test data");
	  hr = _pDictionary->RegisterDataDef (pDataDef);

	  // Create an event (note: this will be replaced by a concrete event in a
		// later version after such an event is implemented.)
		checkResult(defs.cdCommentMarker()->
					CreateInstance(IID_IAAFCommentMarker, 
								   (IUnknown **)&pMarker));
		checkResult(pMarker->QueryInterface(IID_IAAFComponent, (void **)&pComp));
		checkResult(pComp->SetDataDef(pDataDef));
		pComp->Release();
		pComp = NULL;

		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceReference, 
								   (IUnknown **)&pClip));
		checkResult(pClip->QueryInterface(IID_IAAFComponent, (void **)&pComp));
		checkResult(pComp->SetDataDef(pDataDef));
		pComp->Release();
		pComp = NULL;

		checkResult(pMarker->SetAnnotation(pClip));
		pClip->Release();
		pClip = NULL;
		checkResult(pMarker->QueryInterface(IID_IAAFEvent, (void **)&pEvent));
		pMarker->Release();
		pMarker = NULL;
		checkResult(pEvent->SetPosition(_position));
		checkResult(pEvent->SetComment(const_cast<wchar_t*>(_eventComment)));
		
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
		checkResult(pMob->SetMobID(_compositionMobID)); //--cf
		
		// Append event slot to the composition mob.
		checkResult(pMob->AppendSlot(pMobSlot));
		
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
	
	if (pClip)
	{
		pClip->Release();
		pClip = NULL;
	}
	
	if (pMarker)
	{
		pMarker->Release();
		pMarker = NULL;
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
	
	if (pComp)
	{
		pComp->Release();
		pComp = NULL;
	}
	
	if (pDataDef)
	{
		pDataDef->Release();
		pDataDef = NULL;
	}
	
	
	
	// Propogate the error if necessary.
	checkResult(hr);
}


void CommentMarkerTest::OpenEvent()
{
	assert(_pHeader);
	
	HRESULT hr = S_OK;
	IAAFMob *pMob = NULL;
	IEnumAAFMobSlots *pEnumSlots = NULL;
	IAAFCommentMarker *pMarker = NULL;
	IAAFMobSlot *pMobSlot = NULL;
	IAAFEventMobSlot *pEventMobSlot = NULL;
	IAAFSourceReference *pClip = NULL;
	aafRational_t editRate = {0};
	IAAFSegment *pSegment = NULL;
	IAAFEvent *pEvent = NULL;
	aafPosition_t position;
	wchar_t eventComment[128];
	
	
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
	
		checkResult(pEvent->QueryInterface(IID_IAAFCommentMarker, (void **)&pMarker));		
		checkResult(pMarker->GetAnnotation(&pClip));
//		checkResult(pClip->QueryInterface(IID_IAAFSourceClip, (void **)&pClip));		
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
	
	if (pMarker)
	{
		pMarker->Release();
		pMarker = NULL;
	}
	
	if (pSegment)
	{
		pSegment->Release();
		pSegment = NULL;
	}
	
	if (pClip)
	{
		pClip->Release();
		pClip = NULL;
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

