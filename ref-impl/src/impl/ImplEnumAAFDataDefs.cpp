/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif







#ifndef __ImplEnumAAFDataDefs_h__
#include "ImplEnumAAFDataDefs.h"
#endif

#include <assert.h>
#include <string.h>


ImplEnumAAFDataDefs::ImplEnumAAFDataDefs ()
{}


ImplEnumAAFDataDefs::~ImplEnumAAFDataDefs ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::NextOne (
      ImplAAFDataDef ** /*ppDataDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Next (
      aafUInt32  /*count*/,
      ImplAAFDataDef ** /*ppDataDef*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Reset ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Clone (
      ImplEnumAAFDataDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



