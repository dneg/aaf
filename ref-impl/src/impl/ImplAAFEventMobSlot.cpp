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
#include "aafCvt.h"
#include "aafUtils.h"



#include <assert.h>
#include <string.h>


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
  ImplAAFDictionary *pDict = NULL;
  ImplAAFDataDef *pDef = NULL;
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
 
	  CHECK(GetDictionary(&pDict));
	  CHECK(pDict->LookupDataDefintion(&componentDataDef, &pDef));
	  pDict->ReleaseReference();
	  pDict = NULL;
	  CHECK(pDef->DoesDataDefConvertTo(&sequDataDef, &willConvert));
	  pDef->ReleaseReference();
	  pDef = NULL;

	  if (willConvert == AAFFalse)
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
		CHECK(GetDictionary(&pDict));
		CHECK(pDict->LookupDataDefintion(&componentDataDef, &pDef));
		pDict->ReleaseReference();
		pDict = NULL;
		CHECK(pDef->DoesDataDefConvertTo(&sequDataDef, &willConvert));
		pDef->ReleaseReference();
		pDef = NULL;

		if (willConvert == AAFFalse)
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
	pComponent = 0;
    if (NULL != pDict)
      pDict->ReleaseReference();
	pDict = 0;
    if (NULL != pDef)
      pDef->ReleaseReference();
	pDef = 0;
  }
  XEND;

  return(AAFRESULT_SUCCESS);
}



OMDEFINE_STORABLE(ImplAAFEventMobSlot, AUID_AAFEventMobSlot);


