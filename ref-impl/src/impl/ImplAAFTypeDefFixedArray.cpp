/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefFixedArray_h__
#include "ImplAAFTypeDefFixedArray.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;


ImplAAFTypeDefFixedArray::ImplAAFTypeDefFixedArray ()
  : _ElementType  ( PID_TypeDefinitionFixedArray_ElementType,  "Element Type"),
	_ElementCount ( PID_TypeDefinitionFixedArray_ElementCount, "Element Count")
{
  _persistentProperties.put(_ElementType.address());
  _persistentProperties.put(_ElementCount.address());
}


ImplAAFTypeDefFixedArray::~ImplAAFTypeDefFixedArray ()
{}

 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefFixedArray::Initialize (
      aafUID_t *  pID,
      ImplAAFTypeDef * pTypeDef,
      aafUInt32  nElements,
      wchar_t *  pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
  if (! pID)       return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetIdentification (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _ElementType = pTypeDef;
  _ElementCount = nElements;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;
  *ppTypeDef = _ElementType;
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetCount (
      aafUInt32 *  pCount)
{
  if (! pCount) return AAFRESULT_NULL_PARAM;
  *pCount = _ElementCount;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::CreateValueFromValues (
      ImplAAFPropertyValue ** /*ppElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::CreateValueFromCArray (
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetElementValue (
      ImplAAFPropertyValue * pInPropVal,
      aafUInt32  index,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  if (index >= _ElementCount)
	return AAFRESULT_BADINDEX;

  AAFRESULT hr;
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
    ImplAAFTypeDefFixedArray::GetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::SetElementValue (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32  /*index*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::SetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


aafBool ImplAAFTypeDefFixedArray::IsFixedSize (void)
{
  return _ElementType->IsFixedSize();
}

size_t ImplAAFTypeDefFixedArray::PropValSize (void)
{
  return _ElementType->PropValSize() * _ElementCount;
}


OMDEFINE_STORABLE(ImplAAFTypeDefFixedArray, AUID_AAFTypeDefFixedArray);
