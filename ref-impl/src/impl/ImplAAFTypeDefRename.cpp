//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFObjectCreation_h__
#include "ImplAAFObjectCreation.h"
#endif

#include "ImplAAFDictionary.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;


ImplAAFTypeDefRename::ImplAAFTypeDefRename ()
  : _RenamedType  ( PID_TypeDefinitionRename_RenamedType, 
                    L"RenamedType",
                    L"/MetaDictionary/TypeDefinitions",
                    PID_MetaDefinition_Identification)
{
  _persistentProperties.put(_RenamedType.address());
}


ImplAAFTypeDefRename::~ImplAAFTypeDefRename ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::Initialize (
      const aafUID_t & id,
      ImplAAFTypeDef * pBaseType,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;
  if (! pBaseType)  return AAFRESULT_NULL_PARAM;

  HRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  // Check if specified type definition is in the dictionary.
  if( !aafLookupTypeDef( this, pBaseType ) )
	return AAFRESULT_TYPE_NOT_FOUND;

  _RenamedType = pBaseType;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetBaseType (
      ImplAAFTypeDef ** ppBaseType) const
{
  if (! ppBaseType)
	return AAFRESULT_NULL_PARAM;

  if(_RenamedType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFTypeDef *pTypeDef = bootstrapTypeWeakReference(_RenamedType);

  *ppBaseType = pTypeDef;
  ASSERTU (*ppBaseType);
  (*ppBaseType)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetBaseValue (
      ImplAAFPropertyValue * pInPropVal,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  aafUInt32 inBitsSize;
  ImplAAFPropValDataSP pOutPVData;
  ImplAAFPropValDataSP pvd;
  ImplAAFTypeDefSP ptd;

  AAFRESULT hr;
  hr = GetBaseType (&ptd);
  if (AAFRESULT_FAILED (hr)) return hr;
  ASSERTU (ptd);
//  aafUInt32 elementSize = ptd->PropValSize();

  ASSERTU (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
  ASSERTU (pvd);

  hr = pvd->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (! pOutPVData) return AAFRESULT_NOMEMORY;

  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  pOutPVData->ReleaseReference ();

  ASSERTU (ptd);
  hr = pOutPVData->Initialize (ptd);
  if (AAFRESULT_FAILED(hr)) return hr;

  hr = pOutPVData->AllocateFromPropVal (pvd,
										0,
										inBitsSize,
										NULL);
  if (AAFRESULT_FAILED(hr)) return hr;

  ASSERTU (ppOutPropVal);
  *ppOutPropVal = pOutPVData;
  (*ppOutPropVal)->AcquireReference ();
  ASSERTU (*ppOutPropVal);
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::CreateValue (
      ImplAAFPropertyValue * pInPropVal,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;

  aafUInt32 inBitsSize;
  ImplAAFPropValDataSP pOutPVData;
  ImplAAFPropValDataSP pvd;
  AAFRESULT hr;

  ASSERTU (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
  ASSERTU (pvd);

  hr = pvd->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (! pOutPVData) return AAFRESULT_NOMEMORY;

  // SmartPointer operator= will automatically AddRef; CreateImpl *also* will 
  // addref, so we've got one too many.  Put us back to normal.
  pOutPVData->ReleaseReference ();

  hr = pOutPVData->Initialize (this);
  if (AAFRESULT_FAILED(hr)) return hr;

  hr = pOutPVData->AllocateFromPropVal (pvd,
										0,
										inBitsSize,
										NULL);
  if (AAFRESULT_FAILED(hr)) return hr;

  ASSERTU (ppOutPropVal);
  *ppOutPropVal = pOutPVData;
  (*ppOutPropVal)->AcquireReference ();
  ASSERTU (*ppOutPropVal);
  return AAFRESULT_SUCCESS;
}

ImplAAFTypeDefSP ImplAAFTypeDefRename::BaseType () const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetBaseType (&result);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  ASSERTU (result);
  return result;
}

void ImplAAFTypeDefRename::reorder(OMByte* externalBytes,
								   OMUInt32 externalBytesSize) const
{
  BaseType()->reorder (externalBytes, externalBytesSize);
}


OMUInt32 ImplAAFTypeDefRename::externalSize(const OMByte* internalBytes,
										  OMUInt32 internalBytesSize) const
{
  return BaseType()->externalSize (internalBytes, internalBytesSize);
}


void ImplAAFTypeDefRename::externalize(const OMByte* internalBytes,
									   OMUInt32 internalBytesSize,
									   OMByte* externalBytes,
									   OMUInt32 externalBytesSize,
									   OMByteOrder byteOrder) const
{
  BaseType()->externalize (internalBytes,
						   internalBytesSize,
						   externalBytes,
						   externalBytesSize,
						   byteOrder);
}


OMUInt32 ImplAAFTypeDefRename::internalSize(const OMByte* externalBytes,
										  OMUInt32 externalBytesSize) const
{
  return BaseType()->internalSize (externalBytes, externalBytesSize);
}


void ImplAAFTypeDefRename::internalize(const OMByte* externalBytes,
									   OMUInt32 externalBytesSize,
									   OMByte* internalBytes,
									   OMUInt32 internalBytesSize,
									   OMByteOrder byteOrder) const
{
  BaseType()->internalize (externalBytes,
						   externalBytesSize,
						   internalBytes,
						   internalBytesSize,
						   byteOrder);
}



aafBool ImplAAFTypeDefRename::IsFixedSize() const
{
  return BaseType()->IsFixedSize ();
}


OMUInt32 ImplAAFTypeDefRename::PropValSize() const
{
  return BaseType()->PropValSize ();
}


aafBool ImplAAFTypeDefRename::IsRegistered() const
{
  return BaseType()->IsRegistered ();
}


OMUInt32 ImplAAFTypeDefRename::NativeSize() const
{
  return BaseType()->NativeSize ();
}


OMProperty * ImplAAFTypeDefRename::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  return BaseType()->pvtCreateOMProperty (pid, name);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetTypeCategory (
      eAAFTypeCategory_t * pTid)
{
  if (! pTid)
	return AAFRESULT_NULL_PARAM;

  *pTid = kAAFTypeCatRename;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  return BaseType()->RawAccessType (ppRawTypeDef);
}


bool ImplAAFTypeDefRename::IsAggregatable () const
{ return BaseType()->IsAggregatable(); }

bool ImplAAFTypeDefRename::IsStreamable () const
{ return BaseType()->IsStreamable(); }

bool ImplAAFTypeDefRename::IsFixedArrayable () const
{ return BaseType()->IsFixedArrayable(); }

bool ImplAAFTypeDefRename::IsVariableArrayable () const
{ return BaseType()->IsVariableArrayable(); }

bool ImplAAFTypeDefRename::IsStringable () const
{ return BaseType()->IsStringable(); }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefRename::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefRename));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefRename::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefRename::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}
