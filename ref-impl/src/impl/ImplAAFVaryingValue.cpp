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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFVaryingValue_h__
#include "ImplAAFVaryingValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFVaryingValue::ImplAAFVaryingValue ()
{}


ImplAAFVaryingValue::~ImplAAFVaryingValue ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::AddPoint (
      ImplAAFControlPoint * /*pPoint*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetControlPoints (
      ImplEnumAAFControlPoints ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFVaryingValue::GetValueBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFVaryingValue::GetInterpolatedValue (
      aafRational_t  /*inputValue*/,
      aafInt32  /*valueSize*/,
      aafDataBuffer_t  /*pValue*/,
      aafInt32*  /*bytesRead*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFVaryingValue, AUID_AAFVaryingValue);



