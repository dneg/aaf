/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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

  try
	{

	  _pPropDef = pPropDef;
	  // BobT: don't reference count the property def!
	  // _pPropDef->AcquireReference ();
	  AAFRESULT hr;
	  hr = pPropDef->GetTypeDef (&ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  assert (ptd);

#if defined(ALWAYS_USE_PROPVALDATA)
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
#else // #if defined(ALWAYS_USE_PROPVALDATA)
	  // Let the type definition create the correct subclass of property value.
	  hr = ptd->CreatePropertyValue(pOmProp, &_pPropVal);
#endif // #else // #if defined(ALWAYS_USE_PROPVALDATA)
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
