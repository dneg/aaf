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


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include <AAFResult.h>


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
  if (NULL == pSegment)
    return AAFRESULT_NULL_PARAM;

  // Do even attempt to set the given segment if has already been
  // attached to another persistent object.
  if (pSegment->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  // TODO:
  // Validate the following constraints if the segment is a sequence:
  // A) all events must have the same Event subclass and 
  // B) all events must have the same data kind.
  //
  ImplAAFSequence *pSequence = dynamic_cast<ImplAAFSequence *>(pSegment);
  if (NULL != pSequence)
  {
  }


  // Call the parent class to set the segment...
  return ImplAAFMobSlot::SetSegment(pSegment);
}



OMDEFINE_STORABLE(ImplAAFEventMobSlot, AUID_AAFEventMobSlot);


