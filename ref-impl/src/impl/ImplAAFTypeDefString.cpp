/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFTypeDefString::ImplAAFTypeDefString ()
  : _ElementType  ( PID_TypeDefinitionString_ElementType,  "Element Type")
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefString::~ImplAAFTypeDefString ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::Initialize (
      aafUID_t * pID,
      ImplAAFTypeDef * pTypeDef,
      wchar_t * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
  if (! pID)       return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _ElementType = pTypeDef;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;
  *ppTypeDef = _ElementType;
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetCount (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 *  pCount)
{
  ImplAAFTypeDef * ptd = NULL;
  AAFRESULT hr;

  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pCount) return AAFRESULT_NULL_PARAM;
  hr = GetType (&ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ptd);
  assert (ptd->IsFixedSize());
  aafUInt32 elemSize = ptd->PropValSize();
  ptd->ReleaseReference ();
  aafUInt32 propSize;
  assert (pPropVal);

  ImplAAFPropValData * pvd = NULL;
  pvd = dynamic_cast<ImplAAFPropValData *>(pPropVal);

  assert (pvd);
  hr = pvd->GetBitsSize (&propSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pCount);
  *pCount = propSize / elemSize;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::AppendElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      aafMemPtr_t  /*pElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetElements (
      ImplAAFPropertyValue * pInPropVal,
      aafMemPtr_t pBuffer,
      aafUInt32 bufferSize)
{
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  ImplAAFTypeDef * ptd = NULL;

  try
	{
	  AAFRESULT hr;

	  if (! pInPropVal) throw AAFRESULT_NULL_PARAM;
	  if (! pBuffer) throw AAFRESULT_NULL_PARAM;

	  ImplAAFPropValData * pvd = NULL;
	  pvd = dynamic_cast<ImplAAFPropValData*>(pInPropVal);
	  if (!pvd) throw AAFRESULT_BAD_TYPE;

	  aafUInt32 propBitsSize;
	  hr = pvd->GetBitsSize(&propBitsSize);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  if (bufferSize < propBitsSize)
		throw AAFRESULT_SMALLBUF;

	  aafMemPtr_t pBits = NULL;
	  hr = pvd->GetBits (&pBits);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (pBits);

	  assert (pBuffer);
	  assert (propBitsSize <= bufferSize);
	  memcpy (pBuffer, pBits, propBitsSize);
	}
  catch (HRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (ptd);
  // Don't release this! It's a dynamically-casted pInPropVal.
  // RELEASE_IF_SET (pvd);

  return rReturned;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefString::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatString;
  return AAFRESULT_SUCCESS;
}


OMDEFINE_STORABLE(ImplAAFTypeDefString, AUID_AAFTypeDefString);
