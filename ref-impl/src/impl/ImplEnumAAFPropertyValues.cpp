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



#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#include "ImplAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefVariableArray.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyValues;

ImplEnumAAFPropertyValues::ImplEnumAAFPropertyValues ()
  : _count(0),
    _current(0),
    _pDef(0),
    _pVal(0)
{}

ImplEnumAAFPropertyValues::~ImplEnumAAFPropertyValues ()
{
  if ( _pDef ) {
    _pDef->ReleaseReference();
    _pDef = 0;
  }

  if ( _pVal ) {
    _pVal->ReleaseReference();
    _pVal = 0;
  }
}

AAFRESULT STDMETHODCALLTYPE ImplEnumAAFPropertyValues::Initialize( ImplAAFTypeDefArray* pDef,
								   ImplAAFPropertyValue* pVal )
{
  if ( !pDef ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !pVal ) {
    return AAFRESULT_NULL_PARAM;
  }

  // Must resolve the array type in order to get the array size.
  ImplAAFTypeDefFixedArray *pFixedArray = dynamic_cast<ImplAAFTypeDefFixedArray*>(pDef);
  ImplAAFTypeDefVariableArray *pVariableArray = dynamic_cast<ImplAAFTypeDefVariableArray*>(pDef);

  // Intentional assignment in predicate (both cases below).
  AAFRESULT hr = AAFRESULT_SUCCESS;
  if ( pFixedArray  ) {
    hr = pFixedArray->GetCount( &_count );
  }
  else if ( pVariableArray  ) {
    hr = pVariableArray->GetCount( pVal, &_count );
  }
  else {
    hr = AAFRESULT_BAD_TYPE;
  }

  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  _current = 0;

  _pDef = pDef;
  _pDef->AcquireReference();

  _pVal = pVal;
  _pVal->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::NextOne (
      ImplAAFPropertyValue ** ppPropertyValue )
{
  if ( !_pDef || !_pVal ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  if ( !ppPropertyValue ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( _count == _current ) {
    return AAFRESULT_NO_MORE_OBJECTS;
  }

  // _current should never exceed _count if the implementation is correct
  assert( _current < _count );

  HRESULT hr = _pDef->GetElementValue( _pVal, _current, ppPropertyValue );

  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  _current++;
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Next (
      aafUInt32  count,
      ImplAAFPropertyValue ** ppItems,
      aafUInt32 *  pFetched )
{
  if ( !_pDef || !_pVal ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  // Credits: This was nabbed from ImplAAFEnumerator.

  aafUInt32			numItems;
  AAFRESULT			ar=AAFRESULT_SUCCESS;

  if(ppItems==NULL||pFetched==NULL)
    return(AAFRESULT_NULL_PARAM);
	
  if(count==0)
    return(AAFRESULT_INVALID_PARAM);

  for (numItems = 0; numItems < count; numItems++)
    {
      ar = NextOne(&ppItems[numItems]);
      if (FAILED(ar))
	break;
    }
  
  if (pFetched)
    *pFetched=numItems;
  
  return(ar);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Skip (
      aafUInt32  count )
{
  if ( !_pDef || !_pVal ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  _current += count;

  if ( _current > _count ) {
    _current = _count;
    return AAFRESULT_NO_MORE_OBJECTS;
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Reset ()
{
  if ( !_pDef || !_pVal ) {
    return AAFRESULT_NOT_INITIALIZED;
  }
  
  _current = 0;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Clone (
      ImplEnumAAFPropertyValues ** ppEnum)
{
  ImplEnumAAFPropertyValues* pEnum = 
    static_cast<ImplEnumAAFPropertyValues*>( ::CreateImpl(CLSID_EnumAAFPropertyValues) );

  AAFRESULT hr = pEnum->Initialize( _pDef, _pVal );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  // Initialize() sets _current to zero.  It must match the position of
  // this iterator.
  pEnum->_current = _current;

  pEnum->AcquireReference();
  *ppEnum = pEnum;

  return AAFRESULT_SUCCESS;
}



