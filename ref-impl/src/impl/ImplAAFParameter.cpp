/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


 


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFParameter::ImplAAFParameter ()
{}


ImplAAFParameter::~ImplAAFParameter ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::SetLength (
      aafLength_t  /*length*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetLength (
      aafLength_t *  /*pLength*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	

AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::SetDataDefID (
      aafUID_t  /*pDataDefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetDataDefID (
      aafUID_t *  /*pDataDefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFParameter, AUID_AAFParameter);




