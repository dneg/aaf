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
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


