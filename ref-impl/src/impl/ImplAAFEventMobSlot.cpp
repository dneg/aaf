/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFEventMobSlot_h__
#include "ImplAAFEventMobSlot.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFEventMobSlot::ImplAAFEventMobSlot ()
{}


ImplAAFEventMobSlot::~ImplAAFEventMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::GetEditRate (
      aafRational_t *  /*pEditRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::SetEditRate (
      aafRational_t *  /*pEditRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}






  // Override from AAFMobSlot
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFEventMobSlot::SetSegment (/*[in]*/ ImplAAFSegment * /*pSegment*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }



OMDEFINE_STORABLE(ImplAAFEventMobSlot, AUID_AAFEventMobSlot);


