/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif




#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropertyValue::ImplAAFPropertyValue ()
  : _pType (0)
{}


ImplAAFPropertyValue::~ImplAAFPropertyValue ()
{
  if (_pType)
	_pType->ReleaseReference ();
}


AAFRESULT ImplAAFPropertyValue::SetType (
	  ImplAAFTypeDef * pType)
{
  if (! pType) return AAFRESULT_NULL_PARAM;
  // make sure we haven't been init'd yet
  assert (! _pType);

  _pType = pType;
  _pType->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  assert (_pType);
  _pType->AcquireReference ();
  *ppTypeDef = _pType;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::IsDefinedType (
      aafBool *  /*pIsDefined*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}
