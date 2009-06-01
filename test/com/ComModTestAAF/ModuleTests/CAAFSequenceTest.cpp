//=---------------------------------------------------------------------=
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x75529074, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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

#define COMPONENT_TEST_LENGTH 10

#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

void InstantiateComponent(CAAFBuiltinDefs *defs, 
						 aafLength_t length, 
						 IAAFComponent*& pComponent)
{
	checkResult(defs->cdFiller()->CreateInstance(IID_IAAFComponent, 
		(IUnknown **)&pComponent));
	checkResult(pComponent->SetDataDef(defs->ddkAAFSound()));
	checkResult(pComponent->SetLength(length)); 
}

void InstantiateComponentWithoutLength(CAAFBuiltinDefs *defs, IAAFComponent*& pComponent)
{
	checkResult(defs->cdFiller()->CreateInstance(IID_IAAFComponent, 
		(IUnknown **)&pComponent));
	checkResult(pComponent->SetDataDef(defs->ddkAAFSound()));
}

void InstantiateEvent(CAAFBuiltinDefs *defs,
					  aafPosition_t position,
					  IAAFComponent*& pEventASpComp)
{
	IAAFEvent *pEvent = NULL;

	checkResult(defs->cdCommentMarker()->
		CreateInstance(IID_IAAFEvent, 
			(IUnknown **)&pEvent));
	checkResult(pEvent->SetPosition(position));
	checkResult(pEvent->SetComment(L"A comment"));
	checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pEventASpComp));
	checkResult(pEventASpComp->SetDataDef(defs->ddkAAFSound()));

	pEvent->Release();
	pEvent = NULL;
}

void InstantiateEventWithLength(CAAFBuiltinDefs *defs,
								  aafLength_t length,
								  aafPosition_t position,
								  IAAFComponent*& pEventASpComp)
{
	IAAFEvent *pEvent = NULL;
	InstantiateEvent(defs, position, pEventASpComp);
	checkResult(pEventASpComp->QueryInterface(IID_IAAFEvent, (void **)&pEvent));
	checkResult(pEventASpComp->SetLength(length));
	pEvent->Release();
	pEvent = NULL;
}


void TestComponentLength(IAAFComponent *pCompLengthCheck,
						 aafLength_t expectedLength)
{
	aafLength_t actualLength = 0;
	checkResult(pCompLengthCheck->GetLength(&actualLength));
	checkExpression(expectedLength == actualLength, AAFRESULT_TEST_FAILED);
}

void InstantiateTransition(CAAFBuiltinDefs *defs,
						   aafLength_t transitionLength,
						   IAAFOperationDef* pOperationDef,
						   IAAFComponent*& pComponent)
  {  
	IAAFOperationGroup  *pOperationGroup = 0;
	IAAFTransition* pTransition = 0;
	aafPosition_t cutPoint = 0;

	checkResult(defs->cdOperationGroup()->
				CreateInstance(IID_IAAFOperationGroup,
								(IUnknown **)&pOperationGroup));
	checkResult(pOperationGroup->Initialize(defs->ddkAAFSound(), transitionLength, pOperationDef));

	checkResult(defs->cdTransition()->
						CreateInstance(IID_IAAFTransition, 
									(IUnknown **)&pTransition));
	checkResult(pTransition->Initialize (defs->ddkAAFSound(),
												transitionLength,
												cutPoint,
												pOperationGroup));
	checkResult(pTransition->QueryInterface (IID_IAAFComponent,
											(void **)&pComponent));
	
	pTransition->Release();
	pTransition  = NULL;
	pOperationGroup->Release();
	pOperationGroup = NULL;
}

HRESULT TestDefinitions(CAAFBuiltinDefs *defs)
{
	HRESULT			hr = S_OK;

	IAAFSequence*	pSequence = NULL; 
	IAAFComponent*	pComponent = NULL;

	// Add mob slot w/ sequence
 	  checkResult(defs->cdSequence()->
				  CreateInstance(IID_IAAFSequence, 
								 (IUnknown **)&pSequence));		//Instantiate the sequence

	  /*
		This is a charaterization test.
		This test confirms the erroneus behavior that inserting components into an uninitialized sequence succeeds.
		The insert will succeed and leave the sequence uninitialized.
		This behaviour is wrong but legacy code probably depends on it.
		The correct behaviour would be to return AAFRESULT_NOT_INITIALIZED on any attempt to manipulate the sequence before it is initialized.
	  */
	  //Attempt to insert a component before the sequence datadef is set
	  InstantiateComponent(defs, COMPONENT_TEST_LENGTH, pComponent);
	  checkExpression(AAFRESULT_SUCCESS == pSequence->PrependComponent(pComponent), AAFRESULT_TEST_FAILED);
	  pComponent->Release();
	  pComponent = NULL;

	  pSequence->Release();
	  pSequence = 0;

	  return hr;
}

 		
HRESULT TestComponents(CAAFBuiltinDefs *defs, 
					   IAAFDictionary*  pDictionary, 
					   IAAFMob* pMob)
{
	HRESULT			hr = S_OK;

	IAAFSequence*	pSequence = NULL; 
	IAAFComponent *pCompLengthCheck = NULL; //To test sequence length.
	IAAFComponent*	pComponent = NULL;
	aafUInt32		numComponents;
	aafUID_t	   effectID = kTestEffectID;
	IAAFSegment*	pSegment = NULL;
	IAAFTimelineMobSlot*	pMobSlot = NULL;
	  
	  // Add mob slot w/ sequence
 	  checkResult(defs->cdSequence()->
				  CreateInstance(IID_IAAFSequence, 
								 (IUnknown **)&pSequence));		
	  checkResult(pSequence->Initialize(defs->ddkAAFSound()));

	  //Get Component interface to test sequence lenght:
	  checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pCompLengthCheck));

	  
	  aafLength_t testLength[4];
	  aafLength_t len;
	  IAAFComponent *pEventASpComp = NULL;
	  aafPosition_t position;
	  
	  InstantiateComponentWithoutLength(defs, pComponent);
	  //Inserting a component without length should fail:
	  checkExpression(AAFRESULT_PROP_NOT_PRESENT==pSequence->AppendComponent(pComponent), AAFRESULT_TEST_FAILED);
	  pComponent->Release();
	  pComponent = NULL;

	  int i = 0;
	  // Append two components onto sequence
	  for(i = 0; i < 2; i++)
	  {
		  InstantiateComponent(defs, COMPONENT_TEST_LENGTH+i+2, pComponent);
		  if(i<1)
		  {
			  checkResult(pSequence->PrependComponent(pComponent));
		  }
		  else
		  {
		    checkResult(pSequence->AppendComponent(pComponent));
		  }
		  if(i > 0)
		  {
			  testLength[i] = testLength[i-1]+(COMPONENT_TEST_LENGTH+i+2);
		  }
		  else
		  {
			  testLength[i] = COMPONENT_TEST_LENGTH+i+2;
		  }
		  TestComponentLength(pCompLengthCheck, testLength[i]);
		  pComponent->Release();
		  pComponent = NULL;
	  }
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(2 == numComponents, AAFRESULT_TEST_FAILED);
		checkResult(pSequence->RemoveComponentAt(1));
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(1 == numComponents, AAFRESULT_TEST_FAILED);
		len = 0;//Component at index 1 had length 13, after removing it, the sequence length should be 12.
		checkResult(pCompLengthCheck->GetLength(&len));
		checkExpression((COMPONENT_TEST_LENGTH+2) == len, AAFRESULT_TEST_FAILED);



		// Now prepend a component
		InstantiateComponent(defs, COMPONENT_TEST_LENGTH, pComponent);//10+12 = 22
		checkResult(pSequence->PrependComponent(pComponent));
		pComponent->Release();
		pComponent = NULL;
		TestComponentLength(pCompLengthCheck, (COMPONENT_TEST_LENGTH+2+COMPONENT_TEST_LENGTH));

		
		// Now insert a component in the middle
		InstantiateComponent(defs, COMPONENT_TEST_LENGTH+1, pComponent);//22+11 = 33 
		checkResult(pSequence->InsertComponentAt(1,pComponent));
		pComponent->Release();
		pComponent = NULL;
		TestComponentLength(pCompLengthCheck, (3*COMPONENT_TEST_LENGTH+3));
		aafLength_t oldLen = (3*COMPONENT_TEST_LENGTH+3);
		// Component count should now be 3
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(3 == numComponents, AAFRESULT_TEST_FAILED);

		//Inserting an event in a non event sequence should fail
		position = 0;
		InstantiateEventWithLength(defs, COMPONENT_TEST_LENGTH, position, pEventASpComp);
		//Insert event should fail 
		checkExpression(AAFRESULT_EVENT_SEMANTICS==pSequence->AppendComponent(pEventASpComp), AAFRESULT_TEST_FAILED);
		pEventASpComp->Release();
		pEventASpComp = NULL;


		/////////////////////Trannsition testing/////////////////////////////
		aafLength_t					transitionLength=1;
		IAAFOperationDef*			pOperationDef = NULL;

		// Create the effect and parameter definitions
		checkResult(defs->cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
    

		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));

		checkResult(pOperationDef->SetDataDef (defs->ddkAAFSound()));
		
		checkResult(pOperationDef->SetNumberInputs (0));

		//Create a transition 
		InstantiateTransition(defs, transitionLength, pOperationDef, pComponent);
		//Prepending the transition should fail
		checkExpression(AAFRESULT_LEADING_TRAN == pSequence->InsertComponentAt(0,pComponent), AAFRESULT_TEST_FAILED);
		// now append the transition
		checkResult(pSequence->AppendComponent(pComponent));
		TestComponentLength(pCompLengthCheck, (oldLen-transitionLength));
		// Component count should now be 4
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(4 == numComponents, AAFRESULT_TEST_FAILED);
		// Release the component - because we need to reuse the pointer 
		pComponent->Release();
		pComponent = NULL;
		
		//Make a second transition
		InstantiateTransition(defs, transitionLength, pOperationDef, pComponent);
		// now append the second transition and it should fail due to adjecent transitions
		checkExpression(AAFRESULT_ADJACENT_TRAN == pSequence->AppendComponent(pComponent), AAFRESULT_TEST_FAILED);
		// Component count should now still be 4
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(4 == numComponents, AAFRESULT_TEST_FAILED);
		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		pOperationDef->Release();
		pOperationDef = NULL;

		//Remove the transition.
		checkResult(pSequence->RemoveComponentAt(3)); 
		checkResult(pSequence->CountComponents (&numComponents));
		checkExpression(3 == numComponents, AAFRESULT_TEST_FAILED);
		oldLen = (oldLen-transitionLength);
		len = 0;
		checkResult(pCompLengthCheck->GetLength(&len));
		checkExpression((oldLen+transitionLength) == len, AAFRESULT_TEST_FAILED);
		///End Transition testing////////////////////////////////////////////

		checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));

		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"AAF Test Sequence",
												0,
												&pMobSlot));
		
		pSegment->Release();
    pSegment = NULL;

    pMobSlot->Release();
    pMobSlot = NULL;


		if (pSequence)
			{
				pSequence->Release();
				pSequence = 0;
			}

		if (pComponent)
			pComponent->Release();

		if (pCompLengthCheck)
			pCompLengthCheck->Release();


	return hr;
}

HRESULT TestEvents(CAAFBuiltinDefs *defs)
{
	HRESULT			hr = S_OK;
	IAAFComponent*	pComponent = NULL;
	IAAFSequence*	pEventsSequence = NULL;
	IAAFComponent *pEventLengthCheck = NULL; //To test events sequence length.

	// Add mob slot w/ events sequence.
 	  checkResult(defs->cdSequence()->
				  CreateInstance(IID_IAAFSequence, 
								 (IUnknown **)&pEventsSequence));		
	  checkResult(pEventsSequence->Initialize(defs->ddkAAFSound()));

	  //Get Component interface to test sequence lenght:
	  checkResult(pEventsSequence->QueryInterface(IID_IAAFComponent, (void **)&pEventLengthCheck));
	  aafLength_t testLength[4];
	  aafLength_t len;

	  IAAFComponent *pEventASpComp = NULL;
	  aafPosition_t position;


		//Create and append a few events with NO lenght property defined
		position = 0;
		InstantiateEvent(defs, position, pEventASpComp);
		//Append event
		checkResult(pEventsSequence->AppendComponent(pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		//The sequence should also not have length defined because all its elements don't.
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == pEventLengthCheck->GetLength(&len), AAFRESULT_TEST_FAILED);

		position = 1;
		InstantiateEvent(defs, position, pEventASpComp);
		//Append event
		checkResult(pEventsSequence->AppendComponent(pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == pEventLengthCheck->GetLength(&len), AAFRESULT_TEST_FAILED);

		//Remove elements
		checkResult(pEventsSequence->RemoveComponentAt(1));
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == pEventLengthCheck->GetLength(&len), AAFRESULT_TEST_FAILED);
		checkResult(pEventsSequence->RemoveComponentAt(0));
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == pEventLengthCheck->GetLength(&len), AAFRESULT_TEST_FAILED);

		int i;
		for(i = 0; i < 3; i++)
		{
			position = 10+i;
			// Create a concrete subclass of event
			InstantiateEventWithLength(defs, (COMPONENT_TEST_LENGTH+i), position, pEventASpComp);
			if(i==0)
			{
				testLength[i] = COMPONENT_TEST_LENGTH+i;
			}
			else
			{
				testLength[i]=(position+COMPONENT_TEST_LENGTH+i)-10;
  }
			// Add the event to the sequence.
			if(i<1)
			{
				checkResult(pEventsSequence->PrependComponent(pEventASpComp));
			}
			else
			{
				checkResult(pEventsSequence->AppendComponent(pEventASpComp));
			}
			TestComponentLength(pEventLengthCheck, testLength[i]);

			if(pEventASpComp)
  {
				pEventASpComp->Release();
				pEventASpComp = NULL;
			}
  }
		//Remove the middle event, the sequence length should not be affected:
		checkResult(pEventsSequence->RemoveComponentAt(1));
		TestComponentLength(pEventLengthCheck, 14);

		//Remove the last element with the farthest end point, sequence length must be recalculated in O(n) time.
		checkResult(pEventsSequence->RemoveComponentAt(1));
		TestComponentLength(pEventLengthCheck, COMPONENT_TEST_LENGTH);

		// Now prepend an event
		position = 0;
		InstantiateEventWithLength(defs, COMPONENT_TEST_LENGTH, position, pEventASpComp);
		checkResult(pEventsSequence->PrependComponent(pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck, (COMPONENT_TEST_LENGTH*2));

		//Remove the end element
		checkResult(pEventsSequence->RemoveComponentAt(1));
		TestComponentLength(pEventLengthCheck, COMPONENT_TEST_LENGTH);

		//Sequence now has length 10 with an elemnent starting at 0 and having lenght 10.
		//Insert an element starting at 5 with length 10 and the sequence should end up with a length of 15.
		position = 5;
		InstantiateEventWithLength(defs, COMPONENT_TEST_LENGTH, position, pEventASpComp);
		//Inserting events out of order (5 > 0) should fail 
		checkExpression(AAFRESULT_EVENT_SEMANTICS == pEventsSequence->PrependComponent(pEventASpComp), AAFRESULT_TEST_FAILED);
		//Append event
		checkResult(pEventsSequence->AppendComponent(pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck, 15);

		//Remove both elements. ( "clear" is part of some super class? )
		checkResult(pEventsSequence->RemoveComponentAt(1));
		TestComponentLength(pEventLengthCheck,COMPONENT_TEST_LENGTH);

		checkResult(pEventsSequence->RemoveComponentAt(0));
		TestComponentLength(pEventLengthCheck,0);

		//Last test with 3 nested events.
		position = 40;
		InstantiateEventWithLength(defs, (4*COMPONENT_TEST_LENGTH), position, pEventASpComp);
		//Inserting at invalid index should fail
		checkExpression(AAFRESULT_BADINDEX == pEventsSequence->InsertComponentAt(3,pEventASpComp), AAFRESULT_TEST_FAILED);
		//Insert first event 
		checkResult(pEventsSequence->InsertComponentAt(0,pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck,4*COMPONENT_TEST_LENGTH);

		//Insert second event, which should not add to the length.
		position = 50;
		InstantiateEventWithLength(defs, (2*COMPONENT_TEST_LENGTH), position, pEventASpComp);
		//Insert second event 
		checkResult(pEventsSequence->InsertComponentAt(1,pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck,4*COMPONENT_TEST_LENGTH);

		//Third event adds to the length
		position = 60;
		InstantiateEventWithLength(defs, (3*COMPONENT_TEST_LENGTH), position, pEventASpComp);
		//Just like append and prepend, insert should not allow out of order events.
		checkExpression(AAFRESULT_EVENT_SEMANTICS == pEventsSequence->InsertComponentAt(1,pEventASpComp), AAFRESULT_TEST_FAILED);
		//Insert third event 
		checkResult(pEventsSequence->InsertComponentAt(2,pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck,5*COMPONENT_TEST_LENGTH);

		//Inset last event with first position at index 0 and length longer then curent sequence length.
		position = 0;
		InstantiateEventWithLength(defs, (10*COMPONENT_TEST_LENGTH), position, pEventASpComp);
		//Insert last event 
		checkResult(pEventsSequence->InsertComponentAt(0,pEventASpComp));
		pEventASpComp->Release();
		pEventASpComp = NULL;
		TestComponentLength(pEventLengthCheck,10*COMPONENT_TEST_LENGTH);

		//Inserting a component in an events sequence should fail.
		InstantiateComponent(defs, COMPONENT_TEST_LENGTH, pComponent);
		checkExpression(AAFRESULT_EVENT_SEMANTICS == pEventsSequence->AppendComponent(pComponent), AAFRESULT_TEST_FAILED);
    pComponent->Release();
		pComponent = NULL;

		if(pEventsSequence)
			pEventsSequence->Release();

		if(pEventLengthCheck)
			pEventLengthCheck->Release();

	return hr;
}

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFMob*		pMob = NULL;

	HRESULT			hr = S_OK;
	


  try
  {  
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);


	  // Create the AAF file
	  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));

	  // Get the AAF file header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);
 		
	  // Create a Composition Mob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"AAFSequenceTest"));

	  TestDefinitions(&defs);
	  TestEvents(&defs);
	  TestComponents(&defs,pDictionary, pMob);
	  
		// Add the master mob to the file and cleanup
		pHeader->AddMob(pMob);

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


	if (pMob)
    pMob->Release();

	if (pDictionary)
    pDictionary->Release();

	if (pHeader)
    pHeader->Release();

	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFComponent*		pComp = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFSequence*		pSequence = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	IAAFDataDef*        pDataDef = 0;
	IAAFDefObject*      pDefObj = 0;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr = S_OK;


  try
  {
	  // Open the AAF file
	  checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

	  // Get the AAF file header.
	  checkResult(pFile->GetHeader(&pHeader));

    // Validate that there is only one composition mob.
	  checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over Composition MOBs
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;
    checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
	  {
		  aafNumSlots_t		numSlots = 0;

		  checkResult(pMob->CountSlots(&numSlots));
		  checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

      // Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafUInt32			numCpnts;

				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));

				checkResult(pSequence->CountComponents(&numCpnts));
				checkExpression(numCpnts == 3, AAFRESULT_TEST_FAILED);

				// Verify components using enumerator
			    checkResult(pSequence->GetComponents(&pCompIter));
				numCpnts = 0;
				aafLength_t	len;
				aafUID_t	dataDef;
				while (pCompIter && pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
				{
					numCpnts++;

					checkResult(pComp->GetDataDef(&pDataDef));
					checkResult(pDataDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObj));
					pDataDef->Release();
					pDataDef = 0;
					checkResult(pDefObj->GetAUID(&dataDef));
					pDefObj->Release();
					pDefObj = 0;
					checkExpression(memcmp(&kAAFDataDef_Sound, &dataDef, sizeof(aafUID_t)) == 0,
					                AAFRESULT_TEST_FAILED);

					checkResult(pComp->GetLength(&len));
					checkExpression(len == COMPONENT_TEST_LENGTH+numCpnts-1);

					pComp->Release();
					pComp = NULL;
				}
				checkExpression(numCpnts == 3);

				// Verify components using GetComponentAt()
				for(aafUInt32 n=0;n<3;n++)
				{
					checkResult(pSequence->GetComponentAt(n,&pComp));
					checkResult(pComp->GetDataDef(&pDataDef));
					checkResult(pDataDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObj));
					pDataDef->Release();
					pDataDef = 0;
					checkResult(pDefObj->GetAUID(&dataDef));
					pDefObj->Release();
					pDefObj = 0;
					checkExpression(memcmp(&kAAFDataDef_Sound, &dataDef, sizeof(aafUID_t)) == 0,
					                AAFRESULT_TEST_FAILED);

					checkResult(pComp->GetLength(&len));
					checkExpression(len == COMPONENT_TEST_LENGTH+n);

					pComp->Release();
					pComp = NULL;
				}

				pCompIter->Release();
        pCompIter = NULL;

        pSequence->Release();
        pSequence = NULL;

				pSegment->Release();
        pSegment = NULL;

        pSlot->Release();
        pSlot = NULL;
			}

			pSlotIter->Release();
      pSlotIter = NULL;

		  pMob->Release();
      pMob = NULL;
		}


	}
	catch (HRESULT& rResult)
	{
	  hr = rResult;
	}

	// Cleanup object references
	if (pComp)
	  {
		pComp->Release();
		pComp = 0;
	  }

	if (pCompIter)
	  {
		pCompIter->Release();
		pCompIter = 0;
	  }

	if (pSequence)
	  {
		pSequence->Release();
		pSequence = 0;
	  }

	if (pSegment)
	  {
		pSegment->Release();
		pSegment = 0;
	  }

	if (pSlot)
	  {
		pSlot->Release();
		pSlot = 0;
	  }

	if (pSlotIter)
	  {
		pSlotIter->Release();
		pSlotIter = 0;
	  }

	if (pMob)
	  {
		pMob->Release();
		pMob = 0;
	  }

	if (pMobIter)
	  {
		pMobIter->Release();
		pMobIter = 0;
	  }

	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release();
		pDefObj = 0;
	  }

	if (pFile)
	  {
		pFile->Close();
		pFile->Release();
		pFile = 0;
	  }

	return 	hr;
}

extern "C" HRESULT CAAFSequence_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFSequence_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFSequence_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}



















