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

#include <assert.h>
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
}


ImplAAFTypeDefFixedArray::~ImplAAFTypeDefFixedArray ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefFixedArray::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
	return AAFRESULT_NULL_PARAM;

   if(_ElementType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFTypeDef *pTypeDef = _ElementType;

  *ppTypeDef = pTypeDef;
  assert (*ppTypeDef);
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

  assert (pTypeDef);
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
ImplAAFTypeDefFixedArray::GetElements (
								ImplAAFPropertyValue * /*pInPropVal*/,
								ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
	return AAFRESULT_NOT_IN_CURRENT_VERSION;
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


ImplAAFTypeDefSP ImplAAFTypeDefFixedArray::BaseType (void) const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetType (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (result);
  return result;
}


void ImplAAFTypeDefFixedArray::reorder(OMByte* externalBytes,
									   size_t externalBytesSize) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefSP ptd = BaseType ();
  aafUInt32 elemSize = ptd->PropValSize ();
  aafInt32 numBytesLeft = externalBytesSize;

  for (elem = 0; elem < numElems; elem++)
	{
	  ptd->reorder (externalBytes, elemSize);
	  externalBytes += elemSize;
	  numBytesLeft -= elemSize;
	  assert (numBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefFixedArray::externalSize(OMByte* /*internalBytes*/,
											  size_t /*internalBytesSize*/) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd->IsFixedSize ());
  // size_t result = _ElementCount * ptd->externalSize (0, 0);
  size_t result = _ElementCount * ptd->PropValSize ();
  return result;
}


void ImplAAFTypeDefFixedArray::externalize(OMByte* internalBytes,
										   size_t internalBytesSize,
										   OMByte* externalBytes,
										   size_t externalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefSP ptd = BaseType ();
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
		  ptd->externalize (internalBytes,
							internalSize,
							externalBytes,
							externalSize,
							byteOrder);
		  internalBytes += internalSize;
		  externalBytes += externalSize;
		  internalBytesLeft -= internalSize;
		  externalBytesLeft -= externalSize;
		  assert (internalBytesLeft >= 0);
		  assert (externalBytesLeft >= 0);
		}
	}
}


size_t ImplAAFTypeDefFixedArray::internalSize(OMByte* /*externalBytes*/,
											  size_t /*externalBytesSize*/) const
{
  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd->IsFixedSize ());
  // size_t result = _ElementCount * ptd->internalSize (0, 0);
  size_t result = _ElementCount * ptd->ActualSize ();
  return result;
}


void ImplAAFTypeDefFixedArray::internalize(OMByte* externalBytes,
										   size_t externalBytesSize,
										   OMByte* internalBytes,
										   size_t internalBytesSize,
										   OMByteOrder byteOrder) const
{
  aafUInt32 numElems = _ElementCount;
  aafUInt32 elem = 0;

  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd->IsFixedSize ());
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
		  ptd->internalize (externalBytes,
							externalElemSize,
							internalBytes,
							internalElemSize,
							byteOrder);
		  internalBytes += internalElemSize;
		  externalBytes += externalElemSize;
		  internalBytesLeft -= internalElemSize;
		  externalBytesLeft -= externalElemSize;
		  assert (internalBytesLeft >= 0);
		  assert (externalBytesLeft >= 0);
		}
	}
}


aafBool ImplAAFTypeDefFixedArray::IsFixedSize (void) const
{
  return BaseType()->IsFixedSize();
}

size_t ImplAAFTypeDefFixedArray::PropValSize (void) const
{
  return (BaseType()->PropValSize()) * _ElementCount;
}


aafBool ImplAAFTypeDefFixedArray::IsRegistered (void) const
{
  return BaseType()->IsRegistered();
}


size_t ImplAAFTypeDefFixedArray::NativeSize (void) const
{
  ((ImplAAFTypeDefFixedArray*)this)->AttemptBuiltinRegistration ();
  assert (IsRegistered());

  size_t result;
  ImplAAFTypeDefSP elemType;
  elemType = BaseType ();
  assert (elemType);
  result = elemType->NativeSize() * _ElementCount;
  return result;
}


OMProperty * ImplAAFTypeDefFixedArray::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);

  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

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
	  assert (IsFixedSize());
	  aafUInt32 arraySize = NativeSize ();

	  // But even though elems are fixed size, the variable array is
	  // of variable size.  Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, arraySize);
	}

  assert (result);
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
