/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

 


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropertyDef::ImplAAFPropertyDef ()
  : _pTypeDef (0)
{}


ImplAAFPropertyDef::~ImplAAFPropertyDef ()
{
  if (_pTypeDef)
	_pTypeDef->ReleaseReference ();
  _pTypeDef = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::Initialize (
      aafUID_t * pID,
      ImplAAFTypeDef * pTypeDef,
      wchar_t * pTypeName)
{
  AAFRESULT hr;

  if (! pID)       return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  hr = SetIdentification (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _pTypeDef = pTypeDef;
  _pTypeDef->AcquireReference();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetTypeDef (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;
  if (! _pTypeDef) return AAFRESULT_NOT_INITIALIZED;
  assert (_pTypeDef);
  assert (ppTypeDef);
  *ppTypeDef = _pTypeDef;
  (*ppTypeDef)->AcquireReference();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetMaxVersion (
      aafVersionType_t *  /*pMaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetMaxVersion (
      aafVersionType_t *  /*pMaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetIsSearchable (
      aafBool *  /*pIsSearchable*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetIsSearchable (
      aafBool  /*IsSearchable*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetDefaultValue (
      ImplAAFPropertyValue ** /*ppDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetDefaultValue (
      ImplAAFPropertyValue * /*pDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetRefValues (
      ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFPropertyDef, AUID_AAFPropertyDef);
