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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//
//=---------------------------------------------------------------------=

#include "ImplAAFTaggedValueUtil.h"

#include "ImplAAFTaggedValue.h"
#include "ImplEnumAAFTaggedValues.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFRoot.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
#include "aafErr.h"

#include "AAFResult.h"

#include <wchar.h>
#include <assert.h>

extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;


AAFRESULT ImplAAFTaggedValueUtil::AppendNameValuePair(
  ImplAAFObject* pContainingObject,  
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  aafCharacter_constptr  pName,
  aafCharacter_constptr  pValue )
{
  if ( pName  == NULL ||
       pValue == NULL ) {
    return AAFRESULT_NULL_PARAM;
  }


  XPROTECT()
  {
    ImplAAFSmartPointer<ImplAAFDictionary> spDict;
    CHECK( pContainingObject->GetDictionary(&spDict) );

    // Get a type def for the tagged value.  GetBuiltinDefs passes an
    // unowned pointer.  Don't release it (hence, a bare pointer).
    ImplAAFTypeDef* pTaggedValType = spDict->GetBuiltinDefs()->tdString();
    assert( pTaggedValType );


    // The TaggedValueDef returned by GetBuildinDefs is not reference
    // counted - don't release it.
    // pTaggedDef is the object we will return, so it is not put in a smart pointer
    // either.
    ImplAAFTaggedValue* pTaggedVal;
    ImplAAFClassDef* pTaggedValDef = spDict->GetBuiltinDefs()->cdTaggedValue();
    if ( !pTaggedValDef ) {
      RAISE( E_FAIL );
    }


    // Create the tagged value.
    CHECK( pTaggedValDef->CreateInstance( reinterpret_cast<ImplAAFObject**>(&pTaggedVal) ) );
    assert( pTaggedVal );

    // Init the tagged value.
    CHECK( pTaggedVal->Initialize( pName,
				   pTaggedValType,
				   (wcslen(pValue)+1)*sizeof(aafCharacter),
				   reinterpret_cast<aafDataBuffer_t>(const_cast<aafCharacter*>(pValue)) ) );

    taggedValVector.appendValue( pTaggedVal );

  }
  XEXCEPT
  {}
  XEND;


  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTaggedValueUtil::CountEntries(
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  aafUInt32* pNumEntries )
{
  if ( NULL == pNumEntries ) {
    return AAFRESULT_NULL_PARAM;
  }

  // The convention is to return zero if the property is not present.
  if ( !taggedValVector.isPresent() ) {
    *pNumEntries = 0;
  }
  else {
    *pNumEntries = taggedValVector.count();
  }

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFTaggedValueUtil::GetEnumerator(
  ImplAAFObject* pContainingObject,  
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  ImplEnumAAFTaggedValues ** ppEnum )
{
  if ( NULL == ppEnum ) {
    return AAFRESULT_NULL_PARAM;
  }

  ImplEnumAAFTaggedValues* pEnum = 
    reinterpret_cast<ImplEnumAAFTaggedValues*>( CreateImpl(CLSID_EnumAAFTaggedValues) );
  if ( NULL == pEnum ) {
    return E_FAIL;
  }

  XPROTECT()
  {
    OMStrongReferenceVectorIterator<ImplAAFTaggedValue>* iter =
      new OMStrongReferenceVectorIterator<ImplAAFTaggedValue>(taggedValVector);
    if ( NULL == iter ) {
      RAISE( AAFRESULT_NOMEMORY );
    }

    CHECK(pEnum->Initialize( &CLSID_EnumAAFTaggedValues, pContainingObject, iter ) );
    *ppEnum = pEnum;

  }
  XEXCEPT
  {
    if ( pEnum ) {
      pEnum->ReleaseReference();
    }
  }
  XEND;

  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTaggedValueUtil::RemoveEntry(
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  ImplAAFTaggedValue* pEntry )
{
  if (NULL == pEntry) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !pEntry->attached () ) {
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  }

  if ( !taggedValVector.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  size_t index;
  if ( taggedValVector.findIndex (pEntry, index) ) {
    taggedValVector.removeAt(index);
    pEntry->ReleaseReference();
  }
  else {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

  return AAFRESULT_SUCCESS;
}


