/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

#ifndef __ImplAAFGPITrigger_h__
#include "ImplAAFGPITrigger.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include <AAFResult.h>


ImplAAFGPITrigger::ImplAAFGPITrigger () :
  _activeState(PID_GPITrigger_ActiveState, "ActiveState")
{
  _persistentProperties.put(_activeState.address());
}


ImplAAFGPITrigger::~ImplAAFGPITrigger ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGPITrigger::GetActiveState (
      aafBool *pActiveState)
{
  if (NULL == pActiveState)
    return AAFRESULT_NULL_PARAM;

  *pActiveState = _activeState;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFGPITrigger::SetActiveState (
      aafBool  ActiveState)
{
  _activeState = ActiveState;
  return AAFRESULT_SUCCESS;
}



OMDEFINE_STORABLE(ImplAAFGPITrigger, AUID_AAFGPITrigger);


