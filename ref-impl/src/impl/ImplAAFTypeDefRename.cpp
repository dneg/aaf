/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation      *
*                                               *
\***********************************************/

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFObjectCreation_h__
#include "ImplAAFObjectCreation.h"
#endif

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFTypeDefRename::ImplAAFTypeDefRename ()
  : _RenamedType  ( PID_TypeDefinitionRename_RenamedType, "RenamedType")
{
  _persistentProperties.put(_RenamedType.address());
}


ImplAAFTypeDefRename::~ImplAAFTypeDefRename ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::Initialize (
      const aafUID_t * pID,
      ImplAAFTypeDef * pBaseType,
      wchar_t * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  if (! pBaseType)  return AAFRESULT_NULL_PARAM;
  if (! pID)       return AAFRESULT_NULL_PARAM;

  HRESULT hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  aafUID_t id;
  assert (pBaseType);
  hr = pBaseType->GetAUID(&id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  _RenamedType = id;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetBaseType (
      ImplAAFTypeDef ** ppBaseType)
{
  if (! ppBaseType) return AAFRESULT_NULL_PARAM;

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
	  aafUID_t id = _RenamedType;
	  hr = pDict->LookupType (&id, &ptd);
	  if (AAFRESULT_FAILED(hr))
		throw hr;

	  *ppBaseType = ptd;
	  (*ppBaseType)->AcquireReference ();
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
    ImplAAFTypeDefRename::GetValue (
      ImplAAFPropertyValue * pInPropVal,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  aafUInt32 inBitsSize;
  ImplAAFPropValData * pOutPVData = NULL;
  ImplAAFPropValData * pvd = NULL;
  ImplAAFTypeDef * ptd = NULL;

  try
	{
	  AAFRESULT hr;
	  hr = GetBaseType (&ptd);
	  if (AAFRESULT_FAILED (hr)) throw hr;
	  assert (ptd);
	  aafUInt32 elementSize = ptd->PropValSize();

	  assert (pInPropVal);
	  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	  assert (pvd);

	  hr = pvd->GetBitsSize (&inBitsSize);
	  if (! AAFRESULT_SUCCEEDED (hr)) throw hr;

	  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
	  if (! pOutPVData) throw AAFRESULT_NOMEMORY;
	  assert (ptd);
	  hr = pOutPVData->Initialize (ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  hr = pOutPVData->AllocateFromPropVal (pvd,
											0,
											inBitsSize,
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


void ImplAAFTypeDefRename::reorder(OMByte* externalBytes,
								   size_t externalBytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefRename * pNonConstThis =
	(ImplAAFTypeDefRename *) this;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr = pNonConstThis->GetBaseType (&ptd);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);
  ptd->reorder (externalBytes, externalBytesSize);
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefRename::externalSize(OMByte* internalBytes,
										  size_t internalBytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefRename * pNonConstThis =
	(ImplAAFTypeDefRename *) this;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr = pNonConstThis->GetBaseType (&ptd);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);
  size_t result = ptd->externalSize (internalBytes, internalBytesSize);
  ptd->ReleaseReference ();
  return result;
}


void ImplAAFTypeDefRename::externalize(OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByteOrder byteOrder) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefRename * pNonConstThis =
	(ImplAAFTypeDefRename *) this;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr = pNonConstThis->GetBaseType (&ptd);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);
  ptd->externalize (internalBytes,
					internalBytesSize,
					externalBytes,
					externalBytesSize,
					byteOrder);
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefRename::internalSize(OMByte* externalBytes,
										  size_t externalBytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefRename * pNonConstThis =
	(ImplAAFTypeDefRename *) this;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr = pNonConstThis->GetBaseType (&ptd);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);
  size_t result = ptd->internalSize (externalBytes, externalBytesSize);
  ptd->ReleaseReference ();
  return result;
}


void ImplAAFTypeDefRename::internalize(OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByteOrder byteOrder) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefRename * pNonConstThis =
	(ImplAAFTypeDefRename *) this;
  ImplAAFTypeDef * ptd = 0;
  AAFRESULT hr = pNonConstThis->GetBaseType (&ptd);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);
  ptd->externalize (externalBytes,
					externalBytesSize,
					internalBytes,
					internalBytesSize,
					byteOrder);
  ptd->ReleaseReference ();
}



OMDEFINE_STORABLE(ImplAAFTypeDefRename, AUID_AAFTypeDefRename);
