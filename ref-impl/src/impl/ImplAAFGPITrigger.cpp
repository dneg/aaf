/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFGPITrigger_h__
#include "ImplAAFGPITrigger.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFGPITrigger::ImplAAFGPITrigger ()
{}


ImplAAFGPITrigger::~ImplAAFGPITrigger ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGPITrigger::GetActiveState (
      aafBool *  /*pActiveState*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFGPITrigger::SetActiveState (
      aafBool  /*ActiveState*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFGPITrigger, AUID_AAFGPITrigger);


