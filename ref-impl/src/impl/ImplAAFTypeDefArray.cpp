/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


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

  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  aafUInt32 inBitsSize;
  ImplAAFPropValData * pOutPVData = NULL;
  ImplAAFPropValData * pvd = NULL;
  ImplAAFTypeDef * ptd = NULL;

  try
	{
	  AAFRESULT hr;
	  hr = GetType (&ptd);
	  if (AAFRESULT_FAILED (hr)) throw hr;
	  assert (ptd);
	  aafUInt32 elementSize = ptd->PropValSize();

	  assert (pInPropVal);
	  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	  assert (pvd);

	  hr = pvd->GetBitsSize (&inBitsSize);
	  if (! AAFRESULT_SUCCEEDED (hr)) throw hr;
	  assert ((index+1) * elementSize <= inBitsSize);

	  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
	  if (! pOutPVData) throw AAFRESULT_NOMEMORY;
	  assert (ptd);
	  hr = pOutPVData->Initialize (ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;


	  hr = pOutPVData->AllocateFromPropVal (pvd,
											index * elementSize,
											elementSize,
											NULL);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  assert (ppOutPropVal);
	  *ppOutPropVal = pOutPVData;
	  assert (*ppOutPropVal);
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  // Don't release this!  It is simply a dynamic_cast<>ed pInPropVal
  // RELEASE_IF_SET (pvd);
  RELEASE_IF_SET (ptd);

  return rReturned;
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
