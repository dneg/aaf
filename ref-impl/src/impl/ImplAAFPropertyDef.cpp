/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/


#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFPropertyDef::ImplAAFPropertyDef ()
  : _Type(PID_PropertyDefinition_Type, "Type"),
    _IsOptional(PID_PropertyDefinition_IsOptional, "IsOptional"),
    _pid(PID_PropertyDefinition_LocalIdentification, "LocalIdentification")
{
  _persistentProperties.put (_Type.address());
  _persistentProperties.put (_IsOptional.address());
  _persistentProperties.put (_pid.address());
}


ImplAAFPropertyDef::~ImplAAFPropertyDef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::Initialize (
      const aafUID_t * pPropertyAuid,
      OMPropertyId omPid,
      wchar_t * pPropName,
      ImplAAFTypeDef * pTypeDef,
      aafBool isOptional)
{
  AAFRESULT hr;

  if (! pPropertyAuid)       return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
  if (! pPropName) return AAFRESULT_NULL_PARAM;

  hr = SetAUID (pPropertyAuid);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetName (pPropName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  assert (pTypeDef);
  aafUID_t typeId;
  hr = pTypeDef->GetAUID (&typeId);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  _Type = typeId;

  _pid = omPid;
  _IsOptional = isOptional;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetTypeDef (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  ImplAAFDictionary * pDict = 0;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr;

  hr = GetDictionary(&pDict);
  if (AAFRESULT_FAILED (hr)) return hr;
  assert (pDict);

  aafUID_t typeId = _Type;
  hr = pDict->LookupType (&typeId, &ptd);
  if (AAFRESULT_FAILED (hr))
    {
      assert (pDict);
      pDict->ReleaseReference ();
      pDict = 0;
      return hr;
    }
  assert (ptd);
  assert (ppTypeDef);
  *ppTypeDef = ptd;

  // release/acquire cancel each other
  // ptd->ReleaseReference ();
  // (*ppTypeDef)->AcquireReference ();

  assert (pDict);
  pDict->ReleaseReference ();
  pDict = 0;

  return AAFRESULT_SUCCESS;
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



OMPropertyId ImplAAFPropertyDef::OmPid (void) const
{
  return _pid;
}


OMDEFINE_STORABLE(ImplAAFPropertyDef, AUID_AAFPropertyDef);
