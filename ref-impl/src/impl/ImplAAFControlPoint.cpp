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

#ifndef __ImplAAFControlPoint_h__
#include "ImplAAFControlPoint.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFControlPoint::ImplAAFControlPoint ()
{}


ImplAAFControlPoint::~ImplAAFControlPoint ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTime (
      aafRational_t *  /*pTime*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetEditHint (
      aafEditHint_t *  /*pEditHint*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetDataDefID (
      aafUID_t *  /*pDatadefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValueBufLen (
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValue (
      aafUInt32  /*valueSize*/,
      aafDataBuffer_t  /*pValue*/,
      aafUInt32*  /*bytesRead*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetTime (
      aafRational_t  /*pTime*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetEditHint (
      aafEditHint_t  /*editHint*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetDataDefID (
       aafUID_t * /*pDatadefID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetValue (
      aafUInt32  /*valueSize*/,
      aafDataBuffer_t  /*pValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFControlPoint, AUID_AAFControlPoint);



