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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFEventMobSlot_h__
#include "ImplAAFEventMobSlot.h"
#endif

#ifndef __ImplAAFSequence_h__
#include "ImplAAFSequence.h"
#endif

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "AAFUtils.h"

#include <string.h>

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;


ImplAAFEventMobSlot::ImplAAFEventMobSlot ():
  _editRate(PID_EventMobSlot_EditRate,	L"EditRate"),
  _eventSlotOrigin(PID_EventMobSlot_EventSlotOrigin,	L"EventSlotOrigin")
{
  _persistentProperties.put(_editRate.address());
  _persistentProperties.put(_eventSlotOrigin.address());
}


ImplAAFEventMobSlot::~ImplAAFEventMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::GetEditRate (
      aafRational_t *  pEditRate)
{
  if (NULL == pEditRate)
    return AAFRESULT_NULL_PARAM;

  *pEditRate = _editRate;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::SetEditRate (
      aafRational_t *  pEditRate)
{
  if (NULL == pEditRate)
    return AAFRESULT_NULL_PARAM;

  _editRate = *pEditRate;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::GetEventSlotOrigin (aafPosition_t *pEventSlotOrigin)
{
  if ( !_eventSlotOrigin.isPresent() )
    return AAFRESULT_PROP_NOT_PRESENT;

  if ( pEventSlotOrigin == NULL )
    return AAFRESULT_NULL_PARAM;

  *pEventSlotOrigin = _eventSlotOrigin;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::SetEventSlotOrigin (aafPosition_t eventSlotOrigin)
{
  _eventSlotOrigin = eventSlotOrigin;

  return AAFRESULT_SUCCESS;
}




// Override from AAFMobSlot
AAFRESULT STDMETHODCALLTYPE
ImplAAFEventMobSlot::SetSegment (/*[in]*/ ImplAAFSegment * pSegment)
{
  ImplAAFSequence *pSequence = NULL;
  ImplAAFEvent *pEvent = NULL;
  ImplAAFComponent *pComponent = NULL;
  ImplAAFDictionary *pDict = NULL;
  ImplAAFDataDefSP pComponentDataDef;
  aafBool	willConvert;

  if (NULL == pSegment)
    return AAFRESULT_NULL_PARAM;

  // Do even attempt to set the given segment if has already been
  // attached to another persistent object.
  if (pSegment->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;


  XPROTECT()
  {
    // Validate the following constraints if the segment is a sequence:
    // A) all events must have the same Event subclass and 
    // B) all events must have the same data kind.
    // C) datakind of the each event must be the same as the sequence.
    // D) the positions must be increasing.
    //
    // Otherwise the segment must be an event.
    //
    pSequence = dynamic_cast<ImplAAFSequence *>(pSegment);
    if (NULL != pSequence)
    {
      aafUInt32 i;
	  aafUInt32 numberOfComponents = 0;
      aafPosition_t previousPosition;
      
      
      // Save the sequences data definition guid.
	  ImplAAFDataDefSP pSequDataDef;
      CHECK(pSequence->GetDataDef(&pSequDataDef));

      // There must be at least one component in the sequence.
      CHECK(pSequence->CountComponents(&numberOfComponents));
      if (0 >= numberOfComponents)
        RAISE(AAFRESULT_OBJECT_SEMANTIC);

      // Get the first component so that we can extract information
      // to compare to any remaining elements.
      CHECK(pSequence->GetNthComponent(0, &pComponent));
      
      // The component must have the same data definition [id] as the sequence.
      CHECK(pComponent->GetDataDef(&pComponentDataDef));
	  CHECK(pComponentDataDef->DoesDataDefConvertTo(pSequDataDef,
													&willConvert));
	  if (willConvert == kAAFFalse)
		  RAISE(AAFRESULT_OBJECT_SEMANTIC);

      // Get the runtime type info for validation.
      aafUID_t firstComponentType;
      CHECK(pComponent->GetObjectClass(&firstComponentType));

      // The component MUST be an event.
      pEvent = dynamic_cast<ImplAAFEvent *>(pComponent);
      if (NULL == pEvent)
        RAISE(AAFRESULT_OBJECT_SEMANTIC);

      // Save the position of the first event so that we can use this 
      // as our starting value...
      CHECK(pEvent->GetPosition(&previousPosition));
      
      // Release reference before reusing variable.
      pComponent->ReleaseReference();
      pComponent = NULL;

      // Make sure that all components in the sequence have the same
      // data definition and represent the same event class.
      for (i = 1; i < numberOfComponents; ++i)
      {
        CHECK(pSequence->GetNthComponent(i, &pComponent));

        // The component must have the same data definition [id] as the
        // sequence.
		CHECK(pComponentDataDef->DoesDataDefConvertTo(pSequDataDef,
													  &willConvert));
		if (willConvert == kAAFFalse)
			RAISE(AAFRESULT_INVALID_DATADEF);

        // Validate that this event is the "same" type of event as the
        // first component of the sequence.
        aafUID_t  componentType;
        CHECK(pComponent->GetObjectClass(&componentType));
        if (0 != memcmp(&firstComponentType, &componentType, sizeof(firstComponentType)))
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // The component MUST be an event.
        pEvent = dynamic_cast<ImplAAFEvent *>(pComponent);
        if (NULL == pEvent)
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // Make sure the position of this event is no earlier then the 
        // previous event.
        aafPosition_t currentPosition;
        CHECK(pEvent->GetPosition(&currentPosition));
        if (currentPosition < previousPosition)
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // Save the current position to compare to the next event.
        previousPosition = currentPosition;

        pComponent->ReleaseReference();
        pComponent = NULL;
      }
    }
    else
    {
      // The segment must be an event.
      pEvent = dynamic_cast<ImplAAFEvent *>(pSegment);
      if (NULL == pEvent)
        RAISE(AAFRESULT_OBJECT_SEMANTIC);
    }

    // Call the parent class to set the segment...
    CHECK(ImplAAFMobSlot::SetSegment(pSegment));
  }
  XEXCEPT
  {
    if (NULL != pComponent)
      pComponent->ReleaseReference();
	pComponent = 0;
    if (NULL != pDict)
      pDict->ReleaseReference();
	pDict = 0;
  }
  XEND;

  return(AAFRESULT_SUCCESS);
}
