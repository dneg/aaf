//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
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

extern "C" const aafClassID_t CLSID_AAFPropValData;


#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFProperty::ImplAAFProperty ()
  : _pPropDef (0),
	_pPropVal (0),
	_pid(0)
{}


ImplAAFProperty::~ImplAAFProperty ()
{
  RELEASE_IF_SET (_pPropVal);
  // BobT: don't reference count the property def!
  // RELEASE_IF_SET (_pPropDef);
}


AAFRESULT ImplAAFProperty::Initialize
(
 ImplAAFPropertyDef * pPropDef,
 OMProperty * pOmProp
)
{
  ImplAAFPropValData * pvd = NULL;
  ImplAAFTypeDef * ptd = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;

  if (!pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (!pOmProp)
	return AAFRESULT_NULL_PARAM;

  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;

  try
	{
    _pid = pPropDef->OmPid(); // save local key.
	  _pPropDef = pPropDef;
	  // BobT: don't reference count the property def!
	  // _pPropDef->AcquireReference ();
	  AAFRESULT hr;
	  hr = pPropDef->GetTypeDef (&ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  assert (ptd);
	  // Let the type definition create the correct subclass of property value.
	  hr = ptd->CreatePropertyValue(pOmProp, &_pPropVal);

    if (AAFRESULT_SUCCEEDED(hr))
    {
      // This instance is now fully initialized.
      setInitialized();
    }
	}
  catch (HRESULT &rCaught)
	{
	  rReturned = rCaught;

	  // BobT: don't reference count the property def!
	  // RELEASE_IF_SET (_pPropDef);
	  _pPropDef = 0;

	  RELEASE_IF_SET (pvd);
	}

  RELEASE_IF_SET (ptd);
  return rReturned;
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



AAFRESULT
    ImplAAFProperty::pvtSetValue (
      ImplAAFPropertyValue * pValue)
{
  if (!pValue)
	return AAFRESULT_NULL_PARAM;

  if (_pPropVal)
	_pPropVal->ReleaseReference ();

  _pPropVal = pValue;
  _pPropVal->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


    
// non-public unique identification
const OMPropertyId& ImplAAFProperty::identification(void) const
{
  return _pid;
}
