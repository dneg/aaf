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

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFTypeDefString::ImplAAFTypeDefString ()
  : _ElementType  ( PID_TypeDefinitionString_ElementType,  "Element Type")
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefString::~ImplAAFTypeDefString ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::Initialize (
      const aafUID_t * pID,
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

  aafUID_t id;
  assert (pTypeDef);
  hr = pTypeDef->GetAUID(&id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  _ElementType = id;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (!_cachedBaseType)
	{
	  ImplAAFDictionarySP pDict;

	  AAFRESULT hr;
	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pDict);

	  ImplAAFTypeDefString * pNonConstThis =
		  (ImplAAFTypeDefString *) this;
	  aafUID_t id = _ElementType;
	  hr = pDict->LookupType (&id, &pNonConstThis->_cachedBaseType);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (_cachedBaseType);
	}
  assert (ppTypeDef);
  *ppTypeDef = _cachedBaseType;
  assert (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetCount (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 *  pCount)
{
  ImplAAFTypeDefSP ptd;
  AAFRESULT hr;

  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pCount) return AAFRESULT_NULL_PARAM;
  hr = GetType (&ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ptd);
  assert (ptd->IsFixedSize());
  aafUInt32 elemSize = ptd->PropValSize();
  aafUInt32 propSize;
  assert (pPropVal);

  ImplAAFPropValDataSP pvd;
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
  AAFRESULT hr;

  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! pBuffer) return AAFRESULT_NULL_PARAM;

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pInPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  aafUInt32 propBitsSize;
  hr = pvd->GetBitsSize(&propBitsSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  if (bufferSize < propBitsSize)
	return AAFRESULT_SMALLBUF;

  aafMemPtr_t pBits = NULL;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pBits);

  assert (pBuffer);
  assert (propBitsSize <= bufferSize);
  memcpy (pBuffer, pBits, propBitsSize);

  return AAFRESULT_SUCCESS;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefString::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatString;
  return AAFRESULT_SUCCESS;
}


ImplAAFTypeDefSP ImplAAFTypeDefString::BaseType() const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetType (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (result);
  return result;
}


void ImplAAFTypeDefString::reorder(OMByte* externalBytes,
								   size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  aafUInt32 extElemSize = PropValSize ();
  aafUInt32 numElems = externalBytesSize / extElemSize;
  aafInt32 numBytesLeft = externalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->reorder (externalBytes, extElemSize);
	  externalBytes += extElemSize;
	  numBytesLeft -= extElemSize;
	  assert (numBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefString::externalSize(OMByte* internalBytes,
										  size_t internalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  // aafUInt32 extElemSize = ptd->PropValSize ();
  // aafUInt32 intElemSize = ptd->NativeSize ();
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = internalBytesSize / intElemSize;
  return numElems * extElemSize;
}


void ImplAAFTypeDefString::externalize(OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  aafUInt32 intElemSize = ptd->NativeSize ();
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 numElems = internalBytesSize / intElemSize;
  aafInt32 intNumBytesLeft = externalBytesSize;
  aafInt32 extNumBytesLeft = internalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->externalize (internalBytes,
						intElemSize,
						externalBytes,
						extElemSize,
						byteOrder);
	  internalBytes += intElemSize;
	  externalBytes += extElemSize;
	  intNumBytesLeft -= intElemSize;
	  extNumBytesLeft -= extElemSize;
	  assert (intNumBytesLeft >= 0);
	  assert (extNumBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefString::internalSize(OMByte* externalBytes,
										  size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  // aafUInt32 extElemSize = ptd->PropValSize ();
  // aafUInt32 intElemSize = ptd->NativeSize ();
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  return numElems * intElemSize;
}


void ImplAAFTypeDefString::internalize(OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  aafUInt32 intElemSize = ptd->internalSize (0, 0);
  aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  aafInt32 intNumBytesLeft = externalBytesSize;
  aafInt32 extNumBytesLeft = internalBytesSize;
  aafUInt32 elem = 0;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->internalize (externalBytes,
						extElemSize,
						internalBytes,
						intElemSize,
						byteOrder);
	  internalBytes += intElemSize;
	  externalBytes += extElemSize;
	  intNumBytesLeft -= intElemSize;
	  extNumBytesLeft -= extElemSize;
	  assert (intNumBytesLeft >= 0);
	  assert (extNumBytesLeft >= 0);
	}
}


aafBool ImplAAFTypeDefString::IsFixedSize (void) const
{
  return AAFFalse;
}


size_t ImplAAFTypeDefString::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefString::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefString::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


OMProperty * ImplAAFTypeDefString::pvtCreateOMPropertyMBS
  (OMPropertyId pid,
   const char * name) const
{
  assert (name);
  // Don't specify size for variably-sized properties
  OMProperty * result = new OMSimpleProperty (pid, name);
  assert (result);
  return result;
}


OMDEFINE_STORABLE(ImplAAFTypeDefString, AUID_AAFTypeDefString);
