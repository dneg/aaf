/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFProperty_h__
#include "ImplAAFProperty.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;


ImplAAFProperty::ImplAAFProperty ()
  : _pPropDef (0),
	_pPropVal (0)
{}


ImplAAFProperty::~ImplAAFProperty ()
{
  if (_pPropVal)
	_pPropVal->ReleaseReference();
  if (_pPropDef)
	_pPropDef->ReleaseReference();
}

AAFRESULT ImplAAFProperty::Initialize
(
 ImplAAFPropertyDef * pPropDef
)
{
  if (!pPropDef)
	return AAFRESULT_NULL_PARAM;

  ImplAAFPropertyValue * pv = NULL;
  pv = (ImplAAFPropertyValue*) CreateImpl (CLSID_AAFPropertyValue);
  if (! pv)
	return E_FAIL;
  assert (pv);
  ImplAAFTypeDef * ptd = NULL;
  AAFRESULT hr;
  hr = pPropDef->GetTypeDef (&ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ptd);
  hr = pv->Initialize (ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  ptd->ReleaseReference ();
  _pPropVal = pv;
  _pPropDef = pPropDef;
  _pPropDef->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFProperty::GetDefinition (
      ImplAAFPropertyDef ** ppPropDef)
{
  if (!ppPropDef)
	return AAFRESULT_NULL_PARAM;

  assert (_pPropDef);
  *ppPropDef = _pPropDef;
  (*ppPropDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFProperty::GetValue (
      ImplAAFPropertyValue ** ppValue)
{
  if (!ppValue)
	return AAFRESULT_NULL_PARAM;

  assert (_pPropVal);
  *ppValue = _pPropVal;
  (*ppValue)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}
