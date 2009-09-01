//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefFixedArray_h__
#include "ImplAAFTypeDefFixedArray.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "ImplEnumAAFPropertyValues.h"

#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

#include "OMTypeVisitor.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;


ImplAAFTypeDefFixedArray::ImplAAFTypeDefFixedArray ()
  : _ElementType  ( PID_TypeDefinitionFixedArray_ElementType,  
                    L"ElementType", 
                    L"/MetaDictionary/TypeDefinitions", 
                    PID_MetaDefinition_Identification),
    _ElementCount ( PID_TypeDefinitionFixedArray_ElementCount, 
                    L"ElementCount")
{
  _persistentProperties.put(_ElementType.address());
  _persistentProperties.put(_ElementCount.address());
  _baseTypeIsCached = false;
}


ImplAAFTypeDefFixedArray::~ImplAAFTypeDefFixedArray ()
{
	if(_baseTypeIsCached)
	{
		_cachedBaseType->ReleaseReference();
		_cachedBaseType = NULL;
		_baseTypeIsCached = false;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
	return AAFRESULT_NULL_PARAM;

   if(_ElementType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFTypeDef* pTypeDef = bootstrapTypeWeakReference(_ElementType);

  *ppTypeDef = pTypeDef;
  ASSERTU (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefFixedArray::Initialize (
      const aafUID_t & id,
      ImplAAFTypeDef * pTypeDef,
      aafUInt32  nElements,
      const aafCharacter * pTypeName)
{
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;

  ASSERTU (pTypeDef);
  if (! pTypeDef->IsFixedArrayable())
	return AAFRESULT_BAD_TYPE;

  // Check if specified type definition is in the dictionary.
  if( !aafLookupTypeDef( this, pTypeDef ) )
	return AAFRESULT_TYPE_NOT_FOUND;

  return pvtInitialize (id, pTypeDef, nElements, pTypeName);
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefFixedArray::pvtInitialize (
      const aafUID_t & id,
      const ImplAAFTypeDef * pTypeDef,
      aafUInt32  nElements,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  HRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _ElementType = pTypeDef;
  _ElementCount = nElements;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetCount (
      aafUInt32 *  pCount) const
{
  if (! pCount) return AAFRESULT_NULL_PARAM;
  *pCount = _ElementCount;
  return AAFRESULT_SUCCESS;
}

AAFRESULT
ImplAAFTypeDefFixedArray::GetElements ( ImplAAFPropertyValue * pInPropVal,
				        ImplEnumAAFPropertyValues ** ppEnum)
{
  if (NULL == pInPropVal) {
    return AAFRESULT_NULL_PARAM;
  }

  if (NULL == ppEnum) {
    return AAFRESULT_NULL_PARAM;
  }

  ImplEnumAAFPropertyValues* pEnum = 
    static_cast<ImplEnumAAFPropertyValues*>( ::CreateImpl(CLSID_EnumAAFPropertyValues) );

  if ( !pEnum ) {
    return AAFRESULT_NOMEMORY;
  }

  AAFRESULT hr = pEnum->Initialize( this, pInPropVal );
  if ( AAFRESULT_FAILED( hr ) ) {
    return hr;
  }
  if ( AAFRESULT_SUCCEEDED( hr ) ) {
    pEnum->AcquireReference();
    *ppEnum = pEnum;
  }

  pEnum->ReleaseReference();
  pEnum = NULL;
  
  return hr;
}

// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefFixedArray::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatFixedArray;
  return AAFRESULT_SUCCESS;
}


aafUInt32 ImplAAFTypeDefFixedArray::pvtCount
(
 ImplAAFPropertyValue * /*pInPropVal*/ /*arg unused*/
) const
{
  return _ElementCount;
}


// This pointer is not reference counted when returned, DO NOT KEEP A COPY
ImplAAFTypeDef* ImplAAFTypeDefFixedArray::NonRefCountedBaseType (void) const
{
	if(_baseTypeIsCached)
	{
		return _cachedBaseType;
	}
	else
	{	
		ImplAAFTypeDef* result;
 		AAFRESULT hr = GetType (&result);
 		ASSERTU (AAFRESULT_SUCCEEDED (hr));
 		ASSERTU (result);
 		((ImplAAFTypeDefFixedArray*)this)->_cachedBaseType = result;
 		((ImplAAFTypeDefFixedArray*)this)->_baseTypeIsCached = true;
  		return result;
  	}
}


bool ImplAAFTypeDefFixedArray::isFixedSize(void) const
{
  bool result = false;
  if (IsFixedSize() == kAAFTrue) {
    result = true;
  }
  return result;
}


void ImplAAFTypeDefFixedArray::reorder(OMByte* externalBytes,
									   OMUInt32 externalBytesSize) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  aafUInt32 elemSize = ptd->PropValSize ();
  aafInt32 numBytesLeft = externalBytesSize;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->type()->reorder (externalBytes, elemSize);
	  externalBytes += elemSize;
	  numBytesLeft -= elemSize;
	  ASSERTU (numBytesLeft >= 0);
	}
}


OMUInt32 ImplAAFTypeDefFixedArray::externalSize(const OMByte* /*internalBytes*/,
												OMUInt32 /*internalBytesSize*/) const
{
  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  ASSERTU (ptd->IsFixedSize ());
  // OMUInt32 result = _ElementCount * ptd->externalSize (0, 0);
  OMUInt32 result = _ElementCount * ptd->PropValSize ();
  return result;
}


OMUInt32 ImplAAFTypeDefFixedArray::externalSize(void) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}


void ImplAAFTypeDefFixedArray::externalize(const OMByte* internalBytes,
										   OMUInt32 internalBytesSize,
										   OMByte* externalBytes,
										   OMUInt32 externalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  aafUInt32 internalSize = ptd->ActualSize ();
  aafUInt32 externalSize = ptd->PropValSize ();
  if (internalSize == externalSize)
	{
	  if (numElems > 0)
	  {
		copy (internalBytes,
			  externalBytes,
			  externalBytesSize);
	  }
	}
  else
	{
	  aafInt32 internalBytesLeft = internalBytesSize;
	  aafInt32 externalBytesLeft = externalBytesSize;

	  for (elem = 0; elem < numElems; elem++)
		{
		  ptd->type()->externalize (internalBytes,
							internalSize,
							externalBytes,
							externalSize,
							byteOrder);
		  internalBytes += internalSize;
		  externalBytes += externalSize;
		  internalBytesLeft -= internalSize;
		  externalBytesLeft -= externalSize;
		  ASSERTU (internalBytesLeft >= 0);
		  ASSERTU (externalBytesLeft >= 0);
		}
	}
}


OMUInt32 ImplAAFTypeDefFixedArray::internalSize(const OMByte* /*externalBytes*/,
												OMUInt32 /*externalBytesSize*/) const
{
  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  ASSERTU (ptd->IsFixedSize ());
  // OMUInt32 result = _ElementCount * ptd->internalSize (0, 0);
  OMUInt32 result = _ElementCount * ptd->ActualSize ();
  return result;
}


OMUInt32 ImplAAFTypeDefFixedArray::internalSize(void) const
{
  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  ASSERTU (ptd->IsFixedSize ());
  OMUInt32 result = _ElementCount * ptd->ActualSize ();
  return result;
}


void ImplAAFTypeDefFixedArray::internalize(const OMByte* externalBytes,
										   OMUInt32 externalBytesSize,
										   OMByte* internalBytes,
										   OMUInt32 internalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  ASSERTU (ptd->IsFixedSize ());
  aafUInt32 internalElemSize = ptd->ActualSize ();
  aafUInt32 externalElemSize = ptd->PropValSize ();
  if (internalElemSize == externalElemSize)
	{
	  if (numElems > 0)
	  {
		copy (externalBytes,
			  internalBytes,
			  internalBytesSize);
	  }
	}
  else
	{
	  aafInt32 internalBytesLeft = internalBytesSize;
	  aafInt32 externalBytesLeft = externalBytesSize;

	  for (elem = 0; elem < numElems; elem++)
		{
		  ptd->type()->internalize (externalBytes,
							externalElemSize,
							internalBytes,
							internalElemSize,
							byteOrder);
		  internalBytes += internalElemSize;
		  externalBytes += externalElemSize;
		  internalBytesLeft -= internalElemSize;
		  externalBytesLeft -= externalElemSize;
		  ASSERTU (internalBytesLeft >= 0);
		  ASSERTU (externalBytesLeft >= 0);
		}
	}
}

OMType* ImplAAFTypeDefFixedArray::elementType(void) const
{
  ImplAAFTypeDef* type = NonRefCountedBaseType();
  return type->type();
}

void ImplAAFTypeDefFixedArray::accept(OMTypeVisitor& visitor) const
{
  visitor.visitFixedArrayType(this);
  elementType()->accept(visitor);
}

OMUInt32 ImplAAFTypeDefFixedArray::elementCount(void) const
{
  return _ElementCount;
}


aafBool ImplAAFTypeDefFixedArray::IsFixedSize (void) const
{
  return NonRefCountedBaseType()->IsFixedSize();
}

OMUInt32 ImplAAFTypeDefFixedArray::PropValSize (void) const
{
  return (NonRefCountedBaseType()->PropValSize()) * _ElementCount;
}


aafBool ImplAAFTypeDefFixedArray::IsRegistered (void) const
{
  return NonRefCountedBaseType()->IsRegistered();
}


OMUInt32 ImplAAFTypeDefFixedArray::NativeSize (void) const
{
  ((ImplAAFTypeDefFixedArray*)this)->AttemptBuiltinRegistration ();
  ASSERTU (IsRegistered());

  OMUInt32 result;
  ImplAAFTypeDef* elemType;
  elemType = NonRefCountedBaseType ();
  ASSERTU (elemType);
  result = elemType->NativeSize() * _ElementCount;
  return result;
}


OMProperty * ImplAAFTypeDefFixedArray::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  ASSERTU (name);

  ImplAAFTypeDef* ptd = NonRefCountedBaseType ();
  ASSERTU (ptd);

  OMProperty * result = 0;

  if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is strong ref
	  result = new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
	}
  else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is weak ref, hence implemented as AUID array.
	  // Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, sizeof (aafUID_t));
	}

  else
	{
	  // We don't support variable arrays of variably-sized properties.
	  ASSERTU (IsFixedSize());
	  aafUInt32 as = NativeSize ();
	  ASSERTU(as <= OMPROPERTYSIZE_MAX);
	  OMPropertySize arraySize = static_cast<OMPropertySize>(as);

	  // But even though elems are fixed size, the variable array is
	  // of variable size.  Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, arraySize);
	}

  ASSERTU (result);
  return result;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefFixedArray::ValidateInputParams (
												  ImplAAFPropertyValue ** ppElementValues,
												  aafUInt32  numElements)
												  
{
	//first call base impl.
	HRESULT hr;
	hr = ImplAAFTypeDefArray::ValidateInputParams(ppElementValues, numElements);
	if (AAFRESULT_FAILED (hr)) 
		return hr;

	//Next, do some additional specific checking for Fixed Array ...

	if (!ppElementValues)
		return AAFRESULT_NULL_PARAM;

	//verify count
	aafUInt32  internalCount = 0;
	hr = GetCount(&internalCount);
	if (AAFRESULT_FAILED (hr)) 
		return hr;
	if (numElements != internalCount)
		return AAFRESULT_DATA_SIZE;
	

	return AAFRESULT_SUCCESS;

}//ValidateInputParams()


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}


bool ImplAAFTypeDefFixedArray::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefFixedArray::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefFixedArray::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefFixedArray::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefFixedArray::IsStringable () const
{ return false; }

bool ImplAAFTypeDefFixedArray::IsArrayable(ImplAAFTypeDef * pSourceTypeDef) const
{ return pSourceTypeDef->IsFixedArrayable(); }





// override from OMStorable.
const OMClassId& ImplAAFTypeDefFixedArray::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefFixedArray));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefFixedArray::onSave(void* clientContext) const
{
  ImplAAFTypeDefArray::onSave(clientContext);
}

void ImplAAFTypeDefFixedArray::onRestore(void* clientContext) const
{
  ImplAAFTypeDefArray::onRestore(clientContext);
}

const OMUniqueObjectIdentification& ImplAAFTypeDefFixedArray::identification(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefFixedArray::name(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefFixedArray::hasDescription(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefFixedArray::description(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefFixedArray::isPredefined(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::isPredefined();
}

