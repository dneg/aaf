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

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#ifndef __ImplAAFTypeDefCharacter_h__
#include "ImplAAFTypeDefCharacter.h"
#endif

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>


extern "C" const aafClassID_t CLSID_AAFPropValData;

ImplAAFTypeDefString::ImplAAFTypeDefString ()
  : _ElementType  ( PID_TypeDefinitionString_ElementType,  "ElementType")
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefString::~ImplAAFTypeDefString ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::Initialize (
      const aafUID_t & id,
      ImplAAFTypeDef * pTypeDef,
      const aafCharacter * pTypeName)
{
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;

  assert (pTypeDef);
  if (! pTypeDef->IsStringable())
	return AAFRESULT_BAD_TYPE;

  aafUID_t typeId;
  AAFRESULT hr = pTypeDef->GetAUID(&typeId);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  return pvtInitialize (id, typeId, pTypeName);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::pvtInitialize (
      const aafUID_t & id,
	  const aafUID_t & typeId,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _ElementType = typeId;

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
	  hr = pDict->LookupTypeDef (_ElementType, &pNonConstThis->_cachedBaseType);
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
    ImplAAFTypeDefString::CreateValueFromCString (
	  aafMemPtr_t  pInitData,
	  aafUInt32  initDataSize,
	  ImplAAFPropertyValue ** ppPropVal)
{
  if (! pInitData)
	return AAFRESULT_NULL_PARAM;

  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  ImplAAFPropValDataSP pvd;
  ImplAAFPropValData * tmp;
  tmp = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!tmp) return AAFRESULT_NOMEMORY;
  pvd = tmp;
  // the pvd smart pointer will maintain a reference for us...
  aafUInt32 refCount;
  refCount = tmp->ReleaseReference ();
  // ...make sure it really does
  assert (1 == refCount);

  AAFRESULT hr;
  hr = SetCString (pvd, pInitData, initDataSize);
  if (AAFRESULT_FAILED (hr))
	return hr;

  assert (ppPropVal);
  *ppPropVal = pvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::SetCString (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t pData,
      aafUInt32 dataSize)
{
  if (! pPropVal)
	return AAFRESULT_NULL_PARAM;

  if (! pData)
	return AAFRESULT_NULL_PARAM;

  if (! IsRegistered ())
	return AAFRESULT_NOT_REGISTERED;

  AAFRESULT hr;
  ImplAAFTypeDefSP pBaseType;
  hr = GetType (&pBaseType);

  assert (pBaseType->IsFixedSize ());
  assert (pBaseType->IsRegistered ());
  // Size of individual elements
  aafUInt32 elemSize = pBaseType->NativeSize ();
  // number of elements in input data.  If this is not an integral
  // number, this will round down and the test below will fail.
  aafUInt32 elemCount = dataSize / elemSize;
  // The size of the new property, calculated from number of elements
  // and the size of each element.
  aafUInt32 propSize = elemSize * elemCount;

  // If the given dataSize was not an integral multiple of the size of
  // each element, then we'll signal an error.
  if (propSize != dataSize)
	return AAFRESULT_BAD_SIZE;

  ImplAAFPropValData * pvd = 0;
  assert (pPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pPropVal);
  assert (pvd);

  aafMemPtr_t pBits = 0;
  hr = pvd->AllocateBits (propSize, &pBits);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (pBits);

  memcpy (pBits, pData, propSize);
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

  aafUInt32 extElemSize = ptd->PropValSize ();
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

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->NativeSize ();
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
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

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->NativeSize ();
  // aafUInt32 intElemSize = ptd->NativeSize ();
  // aafUInt32 extElemSize = ptd->PropValSize ();
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

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->NativeSize ();
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
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

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->NativeSize ();
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
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
  return kAAFFalse;
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

  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  OMProperty * result = 0;


  ImplAAFTypeDefCharacter * ptdCharacter = 
	dynamic_cast<ImplAAFTypeDefCharacter*>((ImplAAFTypeDef*) ptd);
  if (ptdCharacter)
  {
    result = new OMWideStringProperty(pid, name);
  }
  else
  {
    ImplAAFTypeDefInt * ptdi = 
	  dynamic_cast<ImplAAFTypeDefInt*>((ImplAAFTypeDef*) ptd);
    assert (ptdi);
    if (ptdi)
	  {
	    // element is integral type
	    aafUInt32 intSize;
	    AAFRESULT hr;
	    hr = ptdi->GetSize (&intSize);
	    switch (intSize)
		  {
		  case 1:
		    result = new OMVariableSizeProperty<aafUInt8> (pid, name);
		    break;
		  case 2:
		    result = new OMVariableSizeProperty<aafUInt16> (pid, name);
		    break;
		  case 4:
		    result = new OMVariableSizeProperty<aafUInt32> (pid, name);
		    break;
		  case 8:
		    result = new OMVariableSizeProperty<aafInt64> (pid, name);
		    break;
		  default:
		    // We only support strings of those types.
		    assert (0);
		  }
	  }
  }

  // If result wasn't set above, we don't support the type.
  assert (result);
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}


bool ImplAAFTypeDefString::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefString::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefString::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefString::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefString::IsStringable () const
{ return false; }
