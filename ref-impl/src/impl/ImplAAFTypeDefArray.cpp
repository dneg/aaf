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

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;
extern "C" const aafClassID_t CLSID_AAFPropValObjVectElem;

ImplAAFTypeDefArray::ImplAAFTypeDefArray ()
{}


ImplAAFTypeDefArray::~ImplAAFTypeDefArray ()
{}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::CreateValueFromValues (
      ImplAAFPropertyValue ** /*ppElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::CreateValueFromCArray (
      aafMemPtr_t pInitData,
      aafUInt32 initDataSize,
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
  hr = SetCArray (pvd, pInitData, initDataSize);
  if (AAFRESULT_FAILED (hr))
	return hr;

  assert (ppPropVal);
  *ppPropVal = pvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::GetElementValue (
      ImplAAFPropertyValue * pInPropVal,
      aafUInt32  index,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  if (index >= pvtCount (pInPropVal))
	return AAFRESULT_BADINDEX;

  aafUInt32 inBitsSize;
  ImplAAFPropValDataSP pOutPVData;
  ImplAAFPropValDataSP pvd;
  ImplAAFTypeDefSP ptd;

  AAFRESULT hr;
  hr = GetType (&ptd);
  if (AAFRESULT_FAILED (hr)) return hr;
  aafUInt32 elementSize = ptd->PropValSize();

  assert (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);

  hr = pvd->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  assert ((index+1) * elementSize <= inBitsSize);

  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (! pOutPVData) return AAFRESULT_NOMEMORY;
  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  pOutPVData->ReleaseReference ();

  hr = pOutPVData->Initialize (ptd);
  if (AAFRESULT_FAILED(hr)) return hr;

  hr = pOutPVData->AllocateFromPropVal (pvd,
										index * elementSize,
										elementSize,
										NULL);
  if (AAFRESULT_FAILED(hr)) return hr;

  assert (ppOutPropVal);
  *ppOutPropVal = pOutPVData;
  assert (*ppOutPropVal);
  (*ppOutPropVal)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::GetCArray (
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
  aafUInt32 elemSize = pBaseType->NativeSize ();
  aafUInt32 elemCount = pvtCount (pPropVal);
  aafUInt32 propSize = elemSize * elemCount;

  if (dataSize < propSize)
	return AAFRESULT_BAD_SIZE;

  ImplAAFPropValData * pvd = 0;
  assert (pPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pPropVal);
  assert (pvd);
  
  aafUInt32 bitsSize;
  hr = pvd->GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (bitsSize >= propSize);

  aafMemPtr_t pBits = 0;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (pBits);

  memcpy (pData, pBits, propSize);
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::SetElementValue (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32  /*index*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefArray::SetCArray (
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

  // In case of fixed-size arrays, we'll also have to see if the data
  // size matches what we're expecting.
  if (IsFixedSize ())
	{
	  aafUInt32 nativeSize = NativeSize ();
	  if (nativeSize != dataSize)
		return AAFRESULT_BAD_SIZE;
	}

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
