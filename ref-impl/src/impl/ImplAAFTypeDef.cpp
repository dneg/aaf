//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "ImplAAFTypeDef.h"
#include "ImplAAFDictionary.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>
#include <wchar.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;


ImplAAFTypeDef::ImplAAFTypeDef ()
{}


ImplAAFTypeDef::~ImplAAFTypeDef ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetTypeCategory (
      eAAFTypeCategory_t *  /*pTid*/)
{
  // Should be implemented in derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::RawAccessType (
      ImplAAFTypeDef ** /*ppRawTypeDef*/)
{
  // Should be implemented in derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



void ImplAAFTypeDef::reorder(OMByte* /*bytes*/,
							 size_t /*bytesSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


size_t ImplAAFTypeDef::externalSize(OMByte* /*internalBytes*/,
									size_t /*internalBytesSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::externalize(OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


size_t ImplAAFTypeDef::internalSize(OMByte* /*externalBytes*/,
									size_t /*externalSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::internalize(OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


aafBool ImplAAFTypeDef::IsFixedSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return kAAFFalse; // not reached!
}


size_t ImplAAFTypeDef::PropValSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDef::IsRegistered (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return kAAFFalse; // not reached!
}

void ImplAAFTypeDef::AttemptBuiltinRegistration (void)
{
  // Default: will not attempt to register.
}


size_t ImplAAFTypeDef::NativeSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}


size_t ImplAAFTypeDef::ActualSize (void) const
{
  if (IsRegistered())
    return NativeSize();
  else
    return PropValSize();
}


OMProperty * ImplAAFTypeDef::pvtCreateOMProperty
  (OMPropertyId /*pid*/,
   const wchar_t * /*name*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}




// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDef::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  assert (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  assert (type);
  ImplAAFTypeDef *ptd = const_cast<ImplAAFTypeDef *>
                          (dynamic_cast<const ImplAAFTypeDef *>(type));
  assert (ptd);
  if (NULL == ptd)
    return AAFRESULT_INVALID_PARAM;
 
  ImplAAFPropValData *pvd = NULL;
  pvd = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pvd) 
    return AAFRESULT_NOMEMORY;

  result = pvd->Initialize (ptd);
  if (AAFRESULT_SUCCEEDED(result))
  {
    // set the storage in the prop value
    size_t bitsSize;
    assert (property);
    bitsSize = property->bitsSize ();
    aafMemPtr_t pBits = NULL;
    // Bobt hack! This should be removed once we have proper
    // integration with OM property def support.
    if (! property->isOptional() || property->isPresent ())
    {
      result = pvd->AllocateBits (bitsSize, &pBits);
      if (AAFRESULT_SUCCEEDED (result))
      {
	if (bitsSize)
        {
          assert (pBits);
          property->getBits (pBits, bitsSize);
        }
      }
    }
  }

  if (AAFRESULT_SUCCEEDED(result))
  {
    *ppPropertyValue = pvd; // ref count is already 1.
    pvd = NULL;
  }
  else
  {
    pvd->ReleaseReference(); // delete the new object.
  }

  return (result) ;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::pvtGetUInt8Array8Type (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  if (! ppRawTypeDef)
	return AAFRESULT_NULL_PARAM;
  
  ImplAAFDictionarySP pDict;
  AAFRESULT hr = GetDictionary(&pDict);
  if (AAFRESULT_FAILED (hr)) return hr;
  
  return pDict->LookupTypeDef (kAAFTypeID_UInt8Array, ppRawTypeDef);
}


// These all should be pure virtual, but if we allow client extension
// of behavior, clients may have to instantiate these.
bool ImplAAFTypeDef::IsAggregatable () const
{ assert (0); return false; }

bool ImplAAFTypeDef::IsStreamable () const
{ assert (0); return false; }

bool ImplAAFTypeDef::IsFixedArrayable () const
{ assert (0); return false; }

bool ImplAAFTypeDef::IsVariableArrayable () const
{ assert (0); return false; }

bool ImplAAFTypeDef::IsStringable () const
{ assert (0); return false; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDef::classId(void) const
{
  // This should be overridden by each specific type definition.
  return ImplAAFMetaDefinition::classId();
}

// Override callbacks from OMStorable
void ImplAAFTypeDef::onSave(void* clientContext) const
{
  ImplAAFMetaDefinition::onSave(clientContext);
}

void ImplAAFTypeDef::onRestore(void* clientContext) const
{
  ImplAAFMetaDefinition::onRestore(clientContext);
}
