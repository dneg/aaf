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
 * prior written permission of Avid Technology, Inc.
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

#include <assert.h>
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
  ImplAAFTypeDef *pTypeDef = _RenamedType;

  *ppBaseType = pTypeDef;
  assert (*ppBaseType);
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

  aafUInt32 inBitsSize;
  ImplAAFPropValDataSP pOutPVData;
  ImplAAFPropValDataSP pvd;
  ImplAAFTypeDefSP ptd;

  AAFRESULT hr;
  hr = GetBaseType (&ptd);
  if (AAFRESULT_FAILED (hr)) return hr;
  assert (ptd);
//  aafUInt32 elementSize = ptd->PropValSize();

  assert (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
  assert (pvd);

  hr = pvd->GetBitsSize (&inBitsSize);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (! pOutPVData) return AAFRESULT_NOMEMORY;

  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  pOutPVData->ReleaseReference ();

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
  AAFRESULT hr;

  assert (pInPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
  assert (pvd);

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
