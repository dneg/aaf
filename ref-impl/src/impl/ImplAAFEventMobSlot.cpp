/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFEventMobSlot_h__
#include "ImplAAFEventMobSlot.h"
#endif

#ifndef __ImplAAFSequence_h__
#include "ImplAAFSequence.h"
#endif

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"




#include <assert.h>
#include <string.h>
#include <typeinfo.h>


ImplAAFEventMobSlot::ImplAAFEventMobSlot ():
  _editRate(PID_EventMobSlot_EditRate,	"EditRate")
{
  _persistentProperties.put(_editRate.address());
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






// Override from AAFMobSlot
AAFRESULT STDMETHODCALLTYPE
ImplAAFEventMobSlot::SetSegment (/*[in]*/ ImplAAFSegment * pSegment)
{
  ImplAAFSequence *pSequence = NULL;
  ImplAAFEvent *pEvent = NULL;
  ImplAAFComponent *pComponent = NULL;


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
      aafUID_t sequDataDef, componentDataDef;
      aafInt32 i, numberOfComponents = 0;
      aafPosition_t previousPosition;
      
      
      // Save the sequences data definition guid.
      CHECK(pSequence->GetDataDef(&sequDataDef));

      // There must be at least one component in the sequence.
      CHECK(pSequence->GetNumComponents(&numberOfComponents));
      if (0 >= numberOfComponents)
        RAISE(AAFRESULT_OBJECT_SEMANTIC);

      // Get the first component so that we can extract information
      // to compare to any remaining elements.
      CHECK(pSequence->GetNthComponent(0, &pComponent));
      
      // The component must have the same data definition [id] as the sequence.
      CHECK(pComponent->GetDataDef(&componentDataDef));
      if (0 != memcmp(&sequDataDef, &componentDataDef, sizeof(sequDataDef)))
        RAISE(AAFRESULT_OBJECT_SEMANTIC);

      // Get the runtime type info for validation.
      const type_info& firstComponentType = typeid(*pComponent);

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
        CHECK(pComponent->GetDataDef(&componentDataDef));
        if (0 != memcmp(&sequDataDef, &componentDataDef, sizeof(sequDataDef)))
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // Validate that this event is the "same" type of event as the
        // first component of the sequence.
        const type_info& componentType = typeid(*pEvent);
        if (componentType != firstComponentType)
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // The component MUST be an event.
        pEvent = dynamic_cast<ImplAAFEvent *>(pComponent);
        if (NULL == pEvent)
          RAISE(AAFRESULT_OBJECT_SEMANTIC);

        // Make sure the position of this event is no earlier then the 
        // previous event.
        aafPosition_t currentPosition;
        CHECK(pEvent->GetPosition(&currentPosition));
        if (Int64Less(currentPosition, previousPosition))
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
  }
  XEND;

  return(AAFRESULT_SUCCESS);
}



OMDEFINE_STORABLE(ImplAAFEventMobSlot, AUID_AAFEventMobSlot);


