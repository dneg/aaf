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
	_pPropVal (0)
{}


ImplAAFProperty::~ImplAAFProperty ()
{
  RELEASE_IF_SET (_pPropVal);
  RELEASE_IF_SET (_pPropDef);
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

  try
	{

	  _pPropDef = pPropDef;
	  _pPropDef->AcquireReference ();

	  AAFRESULT hr;
	  hr = pPropDef->GetTypeDef (&ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  assert (ptd);

	  pvd = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
	  if (! pvd) throw AAFRESULT_NOMEMORY;

	  hr = pvd->Initialize (ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  // set the storage in the prop value
	  size_t bitsSize;
	  assert (pOmProp);
	  bitsSize = pOmProp->bitsSize ();
	  aafMemPtr_t pBits = NULL;
	  // Bobt hack! This should be removed once we have proper
	  // integration with OM property def support.
	  if (! pOmProp->isOptional() || pOmProp->isPresent ())
		  {
			hr = pvd->AllocateBits (bitsSize, &pBits);
			if (! AAFRESULT_SUCCEEDED (hr)) throw hr;
			if (bitsSize)
			  {
				assert (pBits);
				pOmProp->getBits (pBits, bitsSize);
			  }
		  }
	  _pPropVal = pvd;
	}
  catch (HRESULT &rCaught)
	{
	  rReturned = rCaught;

	  RELEASE_IF_SET (_pPropDef);
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
