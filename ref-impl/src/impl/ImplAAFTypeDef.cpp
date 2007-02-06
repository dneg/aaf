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


#include "ImplAAFTypeDef.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#include "ImplAAFObjectCreation.h"

#include "OMAssertions.h"
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


aafBool ImplAAFTypeDef::IsFixedSize (void) const
{
  // Should be implemented in derived class.
  ASSERTU (0);
  return kAAFFalse; // not reached!
}


OMUInt32 ImplAAFTypeDef::PropValSize (void) const
{
  // Should be implemented in derived class.
  ASSERTU (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDef::IsRegistered (void) const
{
  // Should be implemented in derived class.
  ASSERTU (0);
  return kAAFFalse; // not reached!
}

void ImplAAFTypeDef::AttemptBuiltinRegistration (void)
{
  // Default: will not attempt to register.
}


OMUInt32 ImplAAFTypeDef::NativeSize (void) const
{
  // Should be implemented in derived class.
  ASSERTU (0);
  return 0; // not reached!
}


OMUInt32 ImplAAFTypeDef::ActualSize (void) const
{
  if (IsRegistered())
    return NativeSize();
  else
    return PropValSize();
}


AAFRESULT ImplAAFTypeDef::MergeTo( ImplAAFDictionary* pDstDictionary )
{
  ASSERTU( pDstDictionary );


  AAFRESULT hr = AAFRESULT_SUCCESS;

  aafUID_t  typeID;
  GetAUID( &typeID );

  ImplAAFTypeDef* pDstTypeDef = 0;
  if( AAFRESULT_FAILED( pDstDictionary->LookupTypeDef( typeID, &pDstTypeDef ) ) )
  {
    OMClassFactory* pDstFactory =
        dynamic_cast<OMClassFactory*>( pDstDictionary->metaDictionary() );
    OMStorable* pDstStorable = shallowCopy( pDstFactory );
    ImplAAFTypeDef* pDstTypeDef =
        dynamic_cast<ImplAAFTypeDef*>( pDstStorable );
    ASSERTU( pDstTypeDef );

    hr = pDstDictionary->RegisterTypeDef( pDstTypeDef );
    if( AAFRESULT_SUCCEEDED(hr) )
    {
      pDstTypeDef->onCopy( 0 );
      deepCopyTo( pDstTypeDef, 0 );
    }
  }
  else
  {
      pDstTypeDef->ReleaseReference();
      pDstTypeDef = 0;
  }


  return hr;
}

const OMType* ImplAAFTypeDef::type() const
{
  const OMType* pType = dynamic_cast<const OMType*>(this);
  ASSERTU (pType);

  return pType;
}

OMType* ImplAAFTypeDef::type()
{
  OMType* pType = dynamic_cast<OMType*>(this);
  ASSERTU (pType);

  return pType;
}

OMProperty * ImplAAFTypeDef::pvtCreateOMProperty
  (OMPropertyId /*pid*/,
   const wchar_t * /*name*/) const
{
  // Should be implemented in derived class.
  ASSERTU (0);
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
  ASSERTU (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  ASSERTU (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  ASSERTU (type);
  ImplAAFTypeDef *ptd = const_cast<ImplAAFTypeDef *>
                          (dynamic_cast<const ImplAAFTypeDef *>(type));
  ASSERTU (ptd);
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
    OMUInt32 bitsSize;
    ASSERTU (property);
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
          ASSERTU (pBits);
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


/*static*/ size_t ImplAAFTypeDef::stringArrayStringCount(
    const wchar_t* buffer,
    size_t bufferSize )
{
  TRACE("ImplAAFTypeDefEnum::stringArrayStringCount");
  PRECONDITION("Valid string array", buffer);
  PRECONDITION("Valid string array size", bufferSize > 0);
  PRECONDITION("Is NULL-terminated string", !(buffer[bufferSize-1]));

  size_t count = 0;
  for( size_t i=0; i<bufferSize; i++ )
  {
    if( !(buffer[i]) )
    {
        count++;
    }
  }

  return count;
}


/*static*/ void ImplAAFTypeDef::getStringArrayStrings(
    const wchar_t* buffer,
    size_t ANAME(bufferSize),
    const wchar_t** strings,
    size_t stringCount )
{
  TRACE("ImplAAFTypeDefEnum::parseStringArray");
  PRECONDITION("Valid string array", buffer);
  PRECONDITION("Valid string array size", bufferSize > 0);
  PRECONDITION("Is NULL-terminated string", !(buffer[bufferSize-1]));
  PRECONDITION("Valid string count",
               stringCount > 0 && stringCount == stringArrayStringCount(buffer, bufferSize));

  const wchar_t* p = buffer;
  for( size_t i=0; i<stringCount; i++ )
  {
    strings[i] = p;
    while( *(p++) )
    {
    }
  }
}


// These all should be pure virtual, but if we allow client extension
// of behavior, clients may have to instantiate these.
bool ImplAAFTypeDef::IsAggregatable () const
{ ASSERTU (0); return false; }

bool ImplAAFTypeDef::IsStreamable () const
{ ASSERTU (0); return false; }

bool ImplAAFTypeDef::IsFixedArrayable () const
{ ASSERTU (0); return false; }

bool ImplAAFTypeDef::IsVariableArrayable () const
{ ASSERTU (0); return false; }

bool ImplAAFTypeDef::IsStringable () const
{ ASSERTU (0); return false; }






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

void ImplAAFTypeDef::onCopy(void* clientContext) const
{
  ImplAAFMetaDefinition::onCopy(clientContext);

  ImplAAFTypeDef* pNonConstThis = const_cast<ImplAAFTypeDef*>(this);
  pNonConstThis->setInitialized();
}

const OMUniqueObjectIdentification& ImplAAFTypeDef::identification(void) const
{
  // tjb - to prevent ambiguity
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDef::name(void) const
{
  // tjb - to prevent ambiguity
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDef::hasDescription(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDef::description(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDef::isPredefined(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::isPredefined();
}
