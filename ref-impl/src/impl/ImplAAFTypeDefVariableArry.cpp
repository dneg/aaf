/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;

ImplAAFTypeDefVariableArray::ImplAAFTypeDefVariableArray ()
  : _ElementType (0)
{}



ImplAAFTypeDefVariableArray::~ImplAAFTypeDefVariableArray ()
{
  if (_ElementType) _ElementType->ReleaseReference ();
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::Initialize (
      aafUID_t *  pID,
      ImplAAFTypeDef * pTypeDef,
      wchar_t *  pTypeName)
{
  if (! pID) return AAFRESULT_NULL_PARAM;
  if (! pTypeDef) return AAFRESULT_NULL_PARAM;
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  AAFRESULT hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  assert (! _ElementType);
  _ElementType = pTypeDef;
  if (_ElementType)
	_ElementType->AcquireReference ();

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;
  *ppTypeDef = _ElementType;
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;  
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetCount (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 *  pCount)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pCount) return AAFRESULT_NULL_PARAM;
  assert (_ElementType);
  assert (_ElementType->IsFixedSize());
  aafUInt32 elemSize = _ElementType->PropValSize();
  aafUInt32 propSize;
  AAFRESULT hr;
  assert (pPropVal);
  hr = pPropVal->GetBitsSize (&propSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pCount);
  *pCount = propSize / elemSize;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::AppendElement (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::CreateValueFromValues (
      ImplAAFPropertyValue ** /*pElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::CreateValueFromCArray (
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetElementValue (
      ImplAAFPropertyValue * pInPropVal,
      aafUInt32  index,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  aafUInt32 elemCount;
  AAFRESULT hr;
  hr = GetCount (pInPropVal, &elemCount);
  if (AAFRESULT_FAILED (hr)) return hr;

  if (index >= elemCount)
	return AAFRESULT_BADINDEX;

  aafUInt32 inBitsSize;
  aafUInt32 elementSize = _ElementType->PropValSize();

  hr = pInPropVal->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  assert (index * elementSize <= inBitsSize);

  ImplAAFPropertyValue * pv = NULL;
  pv = (ImplAAFPropertyValue *)CreateImpl(CLSID_AAFPropertyValue);
  if (! pv)
	return AAFRESULT_NOMEMORY;
  assert (_ElementType);
  hr = pv->Initialize (_ElementType);
  if (AAFRESULT_FAILED(hr))
	{
	  pv->ReleaseReference();
	  return hr;
	}

  hr = pv->AllocateFromPropVal (pInPropVal,
								index * elementSize,
								elementSize,
								NULL);
  if (AAFRESULT_FAILED(hr))
	{
	  pv->ReleaseReference ();
	  return hr;
	}

  assert (ppOutPropVal);
  *ppOutPropVal = pv;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::SetElementValue (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32  /*index*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::SetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatVariableArray;
  return AAFRESULT_SUCCESS;
}


aafBool ImplAAFTypeDefVariableArray::IsFixedSize (void)
{
  return AAFFalse;
}


size_t ImplAAFTypeDefVariableArray::PropValSize (void)
{
  assert (0);
  return 0; // not reached!
}


OMDEFINE_STORABLE(ImplAAFTypeDefVariableArray, AUID_AAFTypeDefVariableArray);
