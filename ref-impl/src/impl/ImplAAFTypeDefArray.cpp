/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

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


// OMDEFINE_STORABLE(ImplAAFTypeDefArray, AUID_AAFTypeDefArray);
