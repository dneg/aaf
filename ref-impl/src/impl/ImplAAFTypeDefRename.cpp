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
      ImplAAFTypeDef ** ppBaseType) const
{
  if (! ppBaseType) return AAFRESULT_NULL_PARAM;

  if (!_cachedBaseType)
	{
	  ImplAAFHeaderSP pHead;
	  ImplAAFDictionarySP pDict;

	  AAFRESULT hr;
	  hr = MyHeadObject(&pHead);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pHead);
	  hr = (pHead->GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pDict);

	  ImplAAFTypeDefRename * pNonConstThis =
		  (ImplAAFTypeDefRename *) this;
	  aafUID_t id = _RenamedType;
	  hr = pDict->LookupType (&id, &pNonConstThis->_cachedBaseType);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (_cachedBaseType);
	}
  assert (ppBaseType);
  *ppBaseType = _cachedBaseType;
  assert (*ppBaseType);
  (*ppBaseType)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetValue (
      ImplAAFPropertyValue * pInPropVal,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  aafUInt32 inBitsSize;
  ImplAAFPropValDataSP pOutPVData;
  ImplAAFPropValDataSP pvd;
  ImplAAFTypeDefSP ptd;

  AAFRESULT hr;
  hr = GetBaseType (&ptd);
  if (AAFRESULT_FAILED (hr)) return hr;
  assert (ptd);
  aafUInt32 elementSize = ptd->PropValSize();

  assert (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
  assert (pvd);

  hr = pvd->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (! pOutPVData) return AAFRESULT_NOMEMORY;
  assert (ptd);
  hr = pOutPVData->Initialize (ptd);
  if (AAFRESULT_FAILED(hr)) return hr;

  hr = pOutPVData->AllocateFromPropVal (pvd,
										0,
										inBitsSize,
										NULL);
  if (AAFRESULT_FAILED(hr)) return hr;

  assert (ppOutPropVal);
  *ppOutPropVal = pOutPVData;
  (*ppOutPropVal)->AcquireReference ();
  assert (*ppOutPropVal);
  return AAFRESULT_SUCCESS;
}


ImplAAFTypeDefSP ImplAAFTypeDefRename::BaseType () const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetBaseType (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (result);
  return result;
}

void ImplAAFTypeDefRename::reorder(OMByte* externalBytes,
								   size_t externalBytesSize) const
{
  BaseType()->reorder (externalBytes, externalBytesSize);
}


size_t ImplAAFTypeDefRename::externalSize(OMByte* internalBytes,
										  size_t internalBytesSize) const
{
  return BaseType()->externalSize (internalBytes, internalBytesSize);
}


void ImplAAFTypeDefRename::externalize(OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByteOrder byteOrder) const
{
  BaseType()->externalize (internalBytes,
						   internalBytesSize,
						   externalBytes,
						   externalBytesSize,
						   byteOrder);
}


size_t ImplAAFTypeDefRename::internalSize(OMByte* externalBytes,
										  size_t externalBytesSize) const
{
  return BaseType()->internalSize (externalBytes, externalBytesSize);
}


void ImplAAFTypeDefRename::internalize(OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByteOrder byteOrder) const
{
  BaseType()->externalize (externalBytes,
						   externalBytesSize,
						   internalBytes,
						   internalBytesSize,
						   byteOrder);
}



aafBool ImplAAFTypeDefRename::IsFixedSize() const
{
  return BaseType()->IsFixedSize ();
}


size_t ImplAAFTypeDefRename::PropValSize() const
{
  return BaseType()->PropValSize ();
}


aafBool ImplAAFTypeDefRename::IsRegistered() const
{
  return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefRename::NativeSize() const
{
  return BaseType()->NativeSize ();
}


OMProperty * ImplAAFTypeDefRename::pvtCreateOMProperty
  (OMPropertyId pid,
   const aafCharacter * name) const
{
  return BaseType()->pvtCreateOMProperty (pid, name);
}



OMDEFINE_STORABLE(ImplAAFTypeDefRename, AUID_AAFTypeDefRename);
