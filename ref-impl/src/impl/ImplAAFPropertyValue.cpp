/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
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
  // BobT: type is not to be reference counted.
  // if (_pType)
  //  _pType->ReleaseReference ();
}


AAFRESULT ImplAAFPropertyValue::SetType (
	  ImplAAFTypeDef * pType)
{
  if (! pType) return AAFRESULT_NULL_PARAM;
  // make sure we haven't been init'd yet
  assert (! _pType);

  _pType = pType;
  // BobT: type is not to be reference counted.
  // _pType->AcquireReference ();
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


const ImplAAFTypeDef * ImplAAFPropertyValue::pvtGetType (void) const
{
  return _pType;
}
