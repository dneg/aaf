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

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


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
    ImplAAFTypeDefFixedArray::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  ImplAAFHeader * pHead = NULL;
  ImplAAFDictionary * pDict = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;
	  hr = MyHeadObject(&pHead);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pHead);
	  hr = (pHead->GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pDict);

	  ImplAAFTypeDef * ptd = NULL;
	  aafUID_t id = _ElementType;
	  hr = pDict->LookupType (&id, &ptd);
	  if (AAFRESULT_FAILED(hr))
		throw hr;

	  *ppTypeDef = ptd;
	  (*ppTypeDef)->AcquireReference ();
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pHead);
  RELEASE_IF_SET (pDict);

  return rReturned;
}


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

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  aafUID_t id;
  assert (pTypeDef);
  hr = pTypeDef->GetAUID(&id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  _ElementType = id;

  _ElementCount = nElements;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetCount (
      aafUInt32 *  pCount) const
{
  if (! pCount) return AAFRESULT_NULL_PARAM;
  *pCount = _ElementCount;
  return AAFRESULT_SUCCESS;
}


// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefFixedArray::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatFixedArray;
  return AAFRESULT_SUCCESS;
}


aafUInt32 ImplAAFTypeDefFixedArray::pvtCount
(
 ImplAAFPropertyValue * /*pInPropVal*/ /*arg unused*/
) const
{
  return _ElementCount;
}


ImplAAFTypeDef * ImplAAFTypeDefFixedArray::GetBaseType (void)
{
  AAFRESULT hr;
  ImplAAFTypeDef * pBaseType = 0;
  hr = GetType (&pBaseType);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (pBaseType);
  return pBaseType;
}


void ImplAAFTypeDefFixedArray::reorder(OMByte* externalBytes,
									   size_t externalBytesSize) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefFixedArray * pNonConstThis =
	(ImplAAFTypeDefFixedArray *) this;

  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  aafUInt32 elemSize = ptd->NativeSize ();
  aafInt32 numBytesLeft = externalBytesSize;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->reorder (externalBytes, elemSize);
	  externalBytes += elemSize;
	  numBytesLeft -= elemSize;
	  assert (numBytesLeft >= 0);
	}
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefFixedArray::externalSize(OMByte* /*internalBytes*/,
											  size_t /*internalBytesSize*/) const
{
  return PropValSize();
}


void ImplAAFTypeDefFixedArray::externalize(OMByte* internalBytes,
										   size_t internalBytesSize,
										   OMByte* externalBytes,
										   size_t externalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefFixedArray * pNonConstThis =
	(ImplAAFTypeDefFixedArray *) this;

  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  aafUInt32 internalSize = ptd->NativeSize ();
  aafUInt32 externalSize = ptd->PropValSize ();
  aafInt32 internalBytesLeft = internalBytesSize;
  aafInt32 externalBytesLeft = externalBytesSize;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->externalize (internalBytes,
						internalSize,
						externalBytes,
						externalBytesSize,
						byteOrder);
	  internalBytes += internalSize;
	  externalBytes += externalSize;
	  internalBytesLeft -= internalSize;
	  externalBytesLeft -= externalSize;
	  assert (internalBytesLeft >= 0);
	  assert (externalBytesLeft >= 0);
	}
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefFixedArray::internalSize(OMByte* /*externalBytes*/,
											  size_t /*externalBytesSize*/) const
{
  return NativeSize ();
}


void ImplAAFTypeDefFixedArray::internalize(OMByte* externalBytes,
										   size_t externalBytesSize,
										   OMByte* internalBytes,
										   size_t internalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefFixedArray * pNonConstThis =
	(ImplAAFTypeDefFixedArray *) this;

  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  aafUInt32 internalSize = ptd->NativeSize ();
  aafUInt32 externalSize = ptd->PropValSize ();
  aafInt32 internalBytesLeft = internalBytesSize;
  aafInt32 externalBytesLeft = externalBytesSize;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->internalize (externalBytes,
						externalBytesSize,
						internalBytes,
						internalSize,
						byteOrder);
	  internalBytes += internalSize;
	  externalBytes += externalSize;
	  internalBytesLeft -= internalSize;
	  externalBytesLeft -= externalSize;
	  assert (internalBytesLeft >= 0);
	  assert (externalBytesLeft >= 0);
	}
  ptd->ReleaseReference ();
}


aafBool ImplAAFTypeDefFixedArray::IsFixedSize (void) const
{
  aafBool result;
  ImplAAFTypeDef * elemType = NULL;
  AAFRESULT hr;
  // BobT semi-hack: make non-const 'this' ptr so we can call
  // GetType().  We know we're not munging the typedef we get back, so
  // we know it'll be OK...
  ImplAAFTypeDefArray * pNonConstThis =
	(ImplAAFTypeDefArray *) this;
  hr = pNonConstThis->GetType (&elemType);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (elemType);
  result = elemType->IsFixedSize();
  elemType->ReleaseReference();
  return result;
}

size_t ImplAAFTypeDefFixedArray::PropValSize (void) const
{
  size_t result;
  ImplAAFTypeDef * elemType = NULL;
  AAFRESULT hr;
  // BobT semi-hack: make non-const 'this' ptr so we can call
  // GetType().  We know we're not munging the typedef we get back, so
  // we know it'll be OK...
  ImplAAFTypeDefArray * pNonConstThis =
	(ImplAAFTypeDefArray *) this;
  hr = pNonConstThis->GetType (&elemType);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (elemType);
  result = elemType->PropValSize() * _ElementCount;
  elemType->ReleaseReference();
  return result;
}


aafBool ImplAAFTypeDefFixedArray::IsRegistered (void) const
{
  // true if underlying type is registered
  ImplAAFTypeDef * elemType = NULL;
  AAFRESULT hr;
  // BobT semi-hack: make non-const 'this' ptr so we can call
  // GetType().  We know we're not munging the typedef we get back, so
  // we know it'll be OK...
  ImplAAFTypeDefArray * pNonConstThis =
	(ImplAAFTypeDefArray *) this;
  hr = pNonConstThis->GetType (&elemType);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (elemType);
  aafBool result = elemType->IsRegistered();
  elemType->ReleaseReference ();
  return result;
}


size_t ImplAAFTypeDefFixedArray::NativeSize (void) const
{
  assert (IsRegistered());

  size_t result;
  ImplAAFTypeDef * elemType = NULL;
  AAFRESULT hr;
  // BobT semi-hack: make non-const 'this' ptr so we can call
  // GetType().  We know we're not munging the typedef we get back, so
  // we know it'll be OK...
  ImplAAFTypeDefArray * pNonConstThis =
	(ImplAAFTypeDefArray *) this;
  hr = pNonConstThis->GetType (&elemType);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (elemType);
  result = elemType->NativeSize() * _ElementCount;
  elemType->ReleaseReference();
  return result;
}


OMDEFINE_STORABLE(ImplAAFTypeDefFixedArray, AUID_AAFTypeDefFixedArray);
