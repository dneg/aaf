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
// The Original Code of this file is Copyright 1998-2008, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFStrongRefArrayValue_h__
#include "ImplAAFStrongRefArrayValue.h"
#endif

#ifndef __ImplAAFWeakRefArrayValue_h__
#include "ImplAAFWeakRefArrayValue.h"
#endif


#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#include "ImplEnumAAFPropertyValues.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include "OMArrayProperty.h"
#include "OMTypeVisitor.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;
extern "C" const aafClassID_t CLSID_AAFStrongRefArrayValue;
extern "C" const aafClassID_t CLSID_AAFWeakRefArrayValue;
extern "C" const aafClassID_t CLSID_EnumAAFPropertyValues;


ImplAAFTypeDefVariableArray::ImplAAFTypeDefVariableArray ()
: _ElementType  ( PID_TypeDefinitionVariableArray_ElementType,
				 L"ElementType", 
				 L"/MetaDictionary/TypeDefinitions", 
				 PID_MetaDefinition_Identification)
{
	_persistentProperties.put(_ElementType.address());
}



ImplAAFTypeDefVariableArray::~ImplAAFTypeDefVariableArray ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::GetType (
									  ImplAAFTypeDef ** ppTypeDef) const
{
	if (! ppTypeDef)
		return AAFRESULT_NULL_PARAM;
	
	if(_ElementType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
	ImplAAFTypeDef *pTypeDef = bootstrapTypeWeakReference(_ElementType);
	
	*ppTypeDef = pTypeDef;
	ASSERTU (*ppTypeDef);
	(*ppTypeDef)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::Initialize (
										 const aafUID_t & id,
										 ImplAAFTypeDef * pTypeDef,
										 const aafCharacter * pTypeName)
{
	if (! pTypeDef)  return AAFRESULT_NULL_PARAM;
	
	ASSERTU (pTypeDef);
	if (! pTypeDef->IsVariableArrayable())
		return AAFRESULT_BAD_TYPE;
	
	// Check if specified type definition is in the dictionary.
	if( !aafLookupTypeDef( this, pTypeDef ) )
		return AAFRESULT_TYPE_NOT_FOUND;

	return pvtInitialize (id, pTypeDef, pTypeName);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::pvtInitialize (
											const aafUID_t & id,
											const ImplAAFTypeDef *pType,
											const aafCharacter * pTypeName)
{
	if (! pTypeName) return AAFRESULT_NULL_PARAM;
	
	HRESULT hr;
	
	hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	_ElementType = pType;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::GetCount (
									   ImplAAFPropertyValue * pPropVal,
									   aafUInt32 *  pCount) const
{
	ImplAAFTypeDefSP ptd;
	AAFRESULT hr;
	
	if (! pPropVal) return AAFRESULT_NULL_PARAM;
	if (! pCount) return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	pInPropType;
	if( AAFRESULT_FAILED( pPropVal->GetType( &pInPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pInPropType);
	if( (ImplAAFTypeDef *)pInPropType != (ImplAAFTypeDef *)this )
		return AAFRESULT_BAD_TYPE;

  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->Count(pCount);
  }

	// Bobt semi-hack: need non-const this in order to call
	// non-const GetType. We know we aren't mangling it, so it
	// technically is OK...
	ImplAAFTypeDefVariableArray * pNonConstThis =
		(ImplAAFTypeDefVariableArray *) this;
	hr = pNonConstThis->GetType (&ptd);
	if (AAFRESULT_FAILED(hr)) return hr;
	ASSERTU (ptd);
	ASSERTU (ptd->IsFixedSize());
	aafUInt32 elemSize = ptd->ActualSize();
	aafUInt32 propSize;
	ASSERTU (pPropVal);
	
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData *>(pPropVal);
	
	ASSERTU (pvd);
	hr = pvd->GetBitsSize (&propSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	ASSERTU (pCount);
	*pCount = propSize / elemSize;
	
	return AAFRESULT_SUCCESS;
}


// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
	if (!pTid) return AAFRESULT_NULL_PARAM;
	*pTid = kAAFTypeCatVariableArray;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::AppendElement
(
 ImplAAFPropertyValue * pInPropVal,
 ImplAAFPropertyValue * pMemberPropVal
 )
{
	if (!pInPropVal)
		return AAFRESULT_NULL_PARAM;
	if (!pMemberPropVal)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	pInPropType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pInPropType);
	if( (ImplAAFTypeDef *)pInPropType != this )
		return AAFRESULT_BAD_TYPE;

	AAFRESULT hr;
	
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->AppendElement(pMemberPropVal);
  }

	ImplAAFPropValData* inPvd =
		dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	ASSERTU (inPvd);
	
	ImplAAFPropValData* memPvd =
		dynamic_cast<ImplAAFPropValData*> (pMemberPropVal);
	ASSERTU (memPvd);
	
	aafUInt32 oldSize = 0;
	hr = inPvd->GetBitsSize (&oldSize);
	ASSERTU (AAFRESULT_SUCCEEDED (hr));
	
	aafUInt32 newElemSize = 0;
	hr = memPvd->GetBitsSize (&newElemSize);
	ASSERTU (AAFRESULT_SUCCEEDED (hr));
	
	aafUInt32 newSize = oldSize + newElemSize;
	ASSERTU (newSize);
	// sizeof (*buf) must be 1
	aafUInt8* buf = new aafUInt8[newSize];
	ASSERTU (buf);
	
	aafMemPtr_t pBits = 0;
	
	// Copy old bits into our buffer
	if (oldSize)
	{
		pBits = 0;
		hr = inPvd->GetBits (&pBits);
		ASSERTU (AAFRESULT_SUCCEEDED (hr));
		ASSERTU (pBits);
		memcpy (buf, pBits, oldSize);
	}
	
	// Append new prop val onto end of our buffer
	pBits = 0;
	hr = memPvd->GetBits (&pBits);
	ASSERTU (AAFRESULT_SUCCEEDED (hr));
	ASSERTU (pBits);
	// Following ptr addition depends on buf being a byte pointer
	memcpy (buf+oldSize, pBits, newElemSize);
	
	// Re-allocate prop val bits to hold newly expanded data
	pBits = 0;
	hr = inPvd->AllocateBits (newSize, &pBits);
	ASSERTU (AAFRESULT_SUCCEEDED (hr));
	ASSERTU (pBits);
	memcpy (pBits, buf, newSize);
	
	delete[] buf;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::ValidateInputParams (
												  ImplAAFPropertyValue ** ppElementValues,
												  aafUInt32  numElements)
{
	//first call base impl.
	HRESULT hr;
	hr = ImplAAFTypeDefArray::ValidateInputParams(ppElementValues, numElements);
	if (AAFRESULT_FAILED (hr)) 
		return hr;
	
	//Next, do some additional specific checking for Fixed Array ...
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFTypeDefVariableArray::GetElements (
										  ImplAAFPropertyValue *pInPropVal,
										  ImplEnumAAFPropertyValues **ppEnum)
{
  if (NULL == pInPropVal || NULL == ppEnum)
	  return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the local type.
  ImplAAFTypeDefSP	pInPropType;
  if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) )  )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  *ppEnum = NULL;
  
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->GetElements(ppEnum);
  }
  else {
    // The property value is of the "old" variety.

    ImplEnumAAFPropertyValues* pEnum = 
      static_cast<ImplEnumAAFPropertyValues*>( ::CreateImpl(CLSID_EnumAAFPropertyValues) );

    if ( !pEnum ) {
      return AAFRESULT_NOMEMORY;
    }

    AAFRESULT hr = pEnum->Initialize( this, pInPropVal );
    if ( AAFRESULT_SUCCEEDED( hr ) ) {
      pEnum->AcquireReference();
      *ppEnum = pEnum;
    }

    pEnum->ReleaseReference();
    pEnum = NULL;

    return hr;
  }
}

ImplAAFTypeDefSP ImplAAFTypeDefVariableArray::BaseType() const
{
	ImplAAFTypeDefSP result;
	AAFRESULT hr = GetType (&result);
	ASSERTU (AAFRESULT_SUCCEEDED (hr));
	ASSERTU (result);
	return result;
}


bool ImplAAFTypeDefVariableArray::isFixedSize(void) const
{
  bool result = false;
  if (IsFixedSize() == kAAFTrue) {
    result = true;
  }
  return result;
}


void ImplAAFTypeDefVariableArray::reorder(OMByte* externalBytes,
										  OMUInt32 externalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
	ASSERTU (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 numElems = externalBytesSize / extElemSize;
	aafInt32 numBytesLeft = externalBytesSize;
	aafUInt32 elem = 0;
	
	for (elem = 0; elem < numElems; elem++)
	{
		ptd->type()->reorder (externalBytes, extElemSize);
		externalBytes += extElemSize;
		numBytesLeft -= extElemSize;
		ASSERTU (numBytesLeft >= 0);
	}
}


OMUInt32 ImplAAFTypeDefVariableArray::externalSize(const OMByte* /*internalBytes*/,
												   OMUInt32 internalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
	ASSERTU (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize = ptd->ActualSize ();
	
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	ASSERTU (intElemSize);
	aafUInt32 numElems = internalBytesSize / intElemSize;
	return numElems * extElemSize;
}


OMUInt32 ImplAAFTypeDefVariableArray::externalSize(void) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}


void ImplAAFTypeDefVariableArray::externalize(const OMByte* internalBytes,
											  OMUInt32 internalBytesSize,
											  OMByte* externalBytes,
											  OMUInt32 externalBytesSize,
											  OMByteOrder byteOrder) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
	ASSERTU (ptd->IsFixedSize ());
	aafUInt32 intElemSize = ptd->ActualSize ();
	aafUInt32 extElemSize = ptd->PropValSize ();
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	aafUInt32 numElems = internalBytesSize / intElemSize;
	if (intElemSize == extElemSize)
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
		aafInt32 intNumBytesLeft = internalBytesSize;
		aafInt32 extNumBytesLeft = externalBytesSize;
		aafUInt32 elem = 0;
		
		for (elem = 0; elem < numElems; elem++)
		{
			ptd->type()->externalize (internalBytes,
				intElemSize,
				externalBytes,
				extElemSize,
				byteOrder);
			internalBytes += intElemSize;
			externalBytes += extElemSize;
			intNumBytesLeft -= intElemSize;
			extNumBytesLeft -= extElemSize;
			ASSERTU (intNumBytesLeft >= 0);
			ASSERTU (extNumBytesLeft >= 0);
		}
	}
}


OMUInt32 ImplAAFTypeDefVariableArray::internalSize(const OMByte* /*externalBytes*/,
												   OMUInt32 externalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
	ASSERTU (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize = ptd->ActualSize ();;
	
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	ASSERTU (intElemSize);
	aafUInt32 numElems = externalBytesSize / extElemSize;
	return numElems * intElemSize;
}


OMUInt32 ImplAAFTypeDefVariableArray::internalSize(void) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}


void ImplAAFTypeDefVariableArray::internalize(const OMByte* externalBytes,
											  OMUInt32 externalBytesSize,
											  OMByte* internalBytes,
											  OMUInt32 internalBytesSize,
											  OMByteOrder byteOrder) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
	ASSERTU (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize = ptd->ActualSize ();
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	aafUInt32 numElems = externalBytesSize / extElemSize;
	if (intElemSize == extElemSize)
	{
		ASSERTU (externalBytesSize <= internalBytesSize);
		if (numElems > 0)
		{
		  copy (externalBytes,
			internalBytes,
			internalBytesSize);
		}
	}
	else
	{
		aafInt32 intNumBytesLeft = internalBytesSize;
		aafInt32 extNumBytesLeft = externalBytesSize;
		aafUInt32 elem = 0;
		
		for (elem = 0; elem < numElems; elem++)
		{
			ptd->type()->internalize (externalBytes,
				extElemSize,
				internalBytes,
				intElemSize,
				byteOrder);
			internalBytes += intElemSize;
			externalBytes += extElemSize;
			intNumBytesLeft -= intElemSize;
			extNumBytesLeft -= extElemSize;
			ASSERTU (intNumBytesLeft >= 0);
			ASSERTU (extNumBytesLeft >= 0);
		}
	}
}

OMType* ImplAAFTypeDefVariableArray::elementType(void) const
{
  ImplAAFTypeDef* type = BaseType();
  return type->type();
}

void ImplAAFTypeDefVariableArray::accept(OMTypeVisitor& visitor) const
{
  visitor.visitVaryingArrayType(this);
  elementType()->accept(visitor);
}


aafUInt32 ImplAAFTypeDefVariableArray::pvtCount
(
 ImplAAFPropertyValue * pInPropVal
 ) const
{
	ASSERTU (pInPropVal);
	AAFRESULT hr;
	aafUInt32 retval;
	hr = GetCount (pInPropVal, &retval);
	ASSERTU (AAFRESULT_SUCCEEDED(hr));
	return retval;
}


aafBool ImplAAFTypeDefVariableArray::IsFixedSize (void) const
{
	return kAAFFalse;
}


OMUInt32 ImplAAFTypeDefVariableArray::PropValSize (void) const
{
	ASSERTU (0);
	return 0; // not reached!
}


aafBool ImplAAFTypeDefVariableArray::IsRegistered (void) const
{
	// Only depends on registration of basic type.
	return BaseType()->IsRegistered ();
}


OMUInt32 ImplAAFTypeDefVariableArray::NativeSize (void) const
{
	ASSERTU (0);
	return 0; // not reached!
}


OMProperty * ImplAAFTypeDefVariableArray::pvtCreateOMProperty
(OMPropertyId pid,
 const wchar_t * name) const
{
	ASSERTU (name);
	
	ImplAAFTypeDefSP ptd = BaseType ();
	ASSERTU (ptd);
	
  OMProperty * result = 0;
  ImplAAFTypeDefWeakObjRef *pWeakRefType = NULL;
	
	if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) ptd))
	{
		// element is strong ref
		result = new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
	}
	else if (NULL != (pWeakRefType = dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) ptd)))
	{
#if defined(USE_SIMPLEPROPERTY)
		// element is weak ref, hence implemented as AUID array.
		// Specify a size of one element.
		result = new OMSimpleProperty (pid, name, sizeof (aafUID_t));
#else // #if defined(USE_SIMPLEPROPERTY)
    
    if (pWeakRefType->GetTargetPids())
    {
      
      switch (pWeakRefType->GetUniqueIdentifierPid())
      {
        case PID_MetaDefinition_Identification:
          result = new OMWeakReferenceVectorProperty<OMUniqueObjectIdentification, ImplAAFMetaDefinition>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
      
        case PID_DefinitionObject_Identification:
          result = new OMWeakReferenceVectorProperty<OMUniqueObjectIdentification, ImplAAFDefObject>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
    
        case PID_Mob_MobID:
          result = new OMWeakReferenceVectorProperty<OMUniqueMaterialIdentification, ImplAAFMob>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;

        case PID_EssenceData_MobID:
          result = new OMWeakReferenceVectorProperty<OMUniqueMaterialIdentification, ImplAAFEssenceData>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
    
        default:
          // No support for other "key properties"
          ASSERTU (0);
          break;
      }
    }
#endif // #else // #if defined(USE_SIMPLEPROPERTY)
	}
	
	else
	{
		// We don't support variable arrays of variably-sized properties.
		ASSERTU (ptd->IsFixedSize());
		
		// aafUInt32 elemSize = ptd->NativeSize ();
		// BobT, 2000-02-14: Hack to handle unpersisting objects which have
		// properties of client-defined types which have not yet had
		// their types registered.
		//
		// Was: aafUInt32 elemSize = ptd->NativeSize ();
		//
		//	  aafUInt32 elemSize;
		//	  if (ptd->IsRegistered())
		//		elemSize = ptd->NativeSize ();
		//	  else
		//		elemSize = ptd->PropValSize ();
		
		// But even though elems are fixed size, the variable array is
		// of variable size.  Specify a size of one element.
		//	  result = new OMSimpleProperty (pid, name, elemSize);
		
		// Use a variable sized property so that we can allow a property value
		// size of 0 (i.e. no elements in the array). (transdel 2000-MAR-14)

		// Oliver/Ian 2004-10-08: reinstate code to accept objects with
		// properties of client-defined types that have not yet had
		// their types registered.

		// aafUInt32 elemSize = ptd->NativeSize();
		aafUInt32 elemSize; 
		if (ptd->IsRegistered()) 
			elemSize = ptd->NativeSize (); 
		else 
			elemSize = ptd->PropValSize (); 
		switch (elemSize) {
		case 1:
			result = new OMArrayProperty<aafUInt8> (pid, name);
			break;
		case 2:
			result = new OMArrayProperty<aafUInt16> (pid, name);
			break;
		case 4:
			result = new OMArrayProperty<aafUInt32> (pid, name);
			break;
		case 8:
			result = new OMArrayProperty<aafUInt64> (pid, name);
			break;
		case 16:
			result = new OMArrayProperty<aafUID_t> (pid, name);
			break;
		default:
			result = new OMVariableSizeProperty<aafUInt8> (pid, name);
			break;
		}
		
	}
	
	ASSERTU (result);
	return result;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::RawAccessType (
											ImplAAFTypeDef ** ppRawTypeDef)
{
	// Return variable array of unsigned char
	return pvtGetUInt8Array8Type (ppRawTypeDef);
}


// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefVariableArray::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ASSERTU (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  
  OMReferenceVectorProperty* pReferenceVectorProperty = dynamic_cast<OMReferenceVectorProperty*>(property);
  if (NULL != pReferenceVectorProperty)
  {
    ASSERTU (property->definition());
    if (NULL == property->definition())
      return AAFRESULT_INVALID_PARAM;
    const OMType *type = property->definition()->type();
    ASSERTU (type);
    ImplAAFTypeDefVariableArray *ptd = const_cast<ImplAAFTypeDefVariableArray *>
                            (dynamic_cast<const ImplAAFTypeDefVariableArray *>(type));
    ASSERTU (ptd);
    if (NULL == ptd)
      return AAFRESULT_INVALID_PARAM;
      
    ImplAAFTypeDefSP pElementType;
    result = GetType(&pElementType);
    if (AAFRESULT_FAILED(result))
      return result;
      
    if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is strong ref
      ImplAAFStrongRefArrayValue* pStrongRefArray = NULL;
      pStrongRefArray = (ImplAAFStrongRefArrayValue*) CreateImpl (CLSID_AAFStrongRefArrayValue);
      if (!pStrongRefArray) 
        return AAFRESULT_NOMEMORY;
      result = pStrongRefArray->Initialize(this, property, false /* !fixed */);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pStrongRefArray;
      }
      else
      {
        pStrongRefArray->ReleaseReference();
      }
    }
    else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is weak ref
      ImplAAFWeakRefArrayValue* pWeakRefArray = NULL;
      pWeakRefArray = (ImplAAFWeakRefArrayValue*) CreateImpl (CLSID_AAFWeakRefArrayValue);
      if (!pWeakRefArray) 
        return AAFRESULT_NOMEMORY;
      result = pWeakRefArray->Initialize(this, property, false /* !fixed */);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pWeakRefArray;
      }
      else
      {
        pWeakRefArray->ReleaseReference();
      }
    }
    else
    { 
//      ASSERTU (NULL != *ppPropertyValue);     
//      return AAFRESULT_INVALID_PARAM;

      // TEMPORARY HACK!
      // Weak reference arrays are still implemented as an array of records!
      
      // If the property is not a reference vector then use the "old" method
      // for creating a variable array property value.
      result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
    }
  }
  else
  {
    // If the property is not a reference vector then use the "old" method
    // for creating a variable array property value.
    result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
  }
  return result;
}


bool ImplAAFTypeDefVariableArray::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefVariableArray::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefVariableArray::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefVariableArray::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefVariableArray::IsStringable () const
{ return false; }


//see if source TD is arrayable or not
bool ImplAAFTypeDefVariableArray::IsArrayable(ImplAAFTypeDef * pSourceTypeDef) const
{ return pSourceTypeDef->IsVariableArrayable(); }


const OMUniqueObjectIdentification& ImplAAFTypeDefVariableArray::identification(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefVariableArray::name(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefVariableArray::hasDescription(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefVariableArray::description(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefVariableArray::isPredefined(void) const
{
  // Re-implement pure virtual method inherited from OMArrayType
  return ImplAAFMetaDefinition::isPredefined();
}

// override from OMStorable.
const OMClassId& ImplAAFTypeDefVariableArray::classId(void) const
{
	return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefVariableArray));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefVariableArray::onSave(void* clientContext) const
{
	ImplAAFTypeDefArray::onSave(clientContext);
}

void ImplAAFTypeDefVariableArray::onRestore(void* clientContext) const
{
	ImplAAFTypeDefArray::onRestore(clientContext);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::PrependElement(
											// property value corresponding to array to which element is prepended, [in]
											ImplAAFPropertyValue * pInPropVal, 
											// value to be prepended to this array,  [in]
											ImplAAFPropertyValue * pMemberPropVal )
{
	if (!pInPropVal)
		return AAFRESULT_NULL_PARAM;
	if (!pMemberPropVal)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	pInPropType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pInPropType);
	if( (ImplAAFTypeDef *)pInPropType != this )
		return AAFRESULT_BAD_TYPE;

	AAFRESULT hr;
	
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->PrependElement(pMemberPropVal);
  }

	ImplAAFPropValData* inPvd =
		dynamic_cast<ImplAAFPropValData*> (pInPropVal);	
	if (!inPvd)
		return AAFRESULT_BAD_TYPE;

	ImplAAFPropValData* memPvd =
		dynamic_cast<ImplAAFPropValData*> (pMemberPropVal);
	if (!memPvd)
		return AAFRESULT_BAD_TYPE;
	
	aafUInt32 oldSize = 0;
	hr = inPvd->GetBitsSize (&oldSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	aafUInt32 newElemSize = 0;
	hr = memPvd->GetBitsSize (&newElemSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	aafUInt32 newSize = oldSize + newElemSize;
	
	// sizeof (*buf) must be 1
	aafUInt8* buf = new aafUInt8[newSize];
	
	aafMemPtr_t pBits = 0;
	
	// Prepend new prop val onto end of our buffer
	pBits = 0;
	hr = memPvd->GetBits (&pBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	// Following ptr addition depends on buf being a byte pointer
	memcpy (buf, pBits, newElemSize);
	
	// Copy old bits into our buffer
	if (oldSize)
	{
		pBits = 0;
		hr = inPvd->GetBits (&pBits);
		if (AAFRESULT_FAILED(hr)) return hr;
		memcpy (buf+newElemSize, pBits, oldSize);
	}
	
	
	// Re-allocate prop val bits to hold newly expanded data
	pBits = 0;
	hr = inPvd->AllocateBits (newSize, &pBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	memcpy (pBits, buf, newSize);
	
	delete[] buf;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::RemoveElement(
										   // property value corresponding to array;  [in] 
										   ImplAAFPropertyValue * pInPropVal,
										   // zero-based index into elements in this array type; [in] 
										   aafUInt32  index)
{
	if (!pInPropVal)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	pInPropType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pInPropType);
	if( (ImplAAFTypeDef *)pInPropType != this )
		return AAFRESULT_BAD_TYPE;

  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->RemoveObjectAt(index);
  }

	HRESULT hr = 0;
	aafUInt32  count;
	hr = GetCount (pInPropVal, &count);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	//CASE 1 -  invalid Index
	if (index > (count-1) )
		return AAFRESULT_BADINDEX;
	
	ImplAAFPropValData* inPvd =
		dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	if (!inPvd)
		return AAFRESULT_BAD_TYPE;
	
	aafUInt32 elemSize = BaseType()->ActualSize();
	
	aafUInt32 oldSize = 0;
	hr = inPvd->GetBitsSize (&oldSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	//Ensure the whole size is a multiple of elemSize
	if ( (oldSize % elemSize) != 0 )
		return AAFRESULT_BAD_SIZE;

	aafUInt32 newSize = oldSize - elemSize; //The new size will be 1 less
	
	
	//Ready for battle ..............
	
	aafUInt8* buf = new aafUInt8[newSize];
	
	aafMemPtr_t pInBits = 0;
	
	///  OK ... Two parts: Pre-Index, Post-Index  (we skip the elem-to-delete)
	
	// First part: copy upto Pre-Index into buffer
	hr = inPvd->GetBits (&pInBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	aafUInt32  preindex_size = (index) * elemSize;
	memcpy (buf, pInBits, preindex_size);
	//incr. pInBits to skip the elem-to-delete
	pInBits+= (preindex_size + elemSize);
	
	//Second part: copy Post-Index into buffer
	//		don't forget to point incr. the buf ptr with the pre-index + the new Elem
	aafUInt32 postindex_size = newSize - preindex_size; 
	memcpy (buf + preindex_size,   pInBits,  postindex_size);
	
	
	// Re-allocate prop val bits to hold newly expanded data
	aafMemPtr_t  pBits = 0;
	hr = inPvd->AllocateBits (newSize, &pBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	memcpy (pBits, buf, newSize);
	
	delete[] buf;
	return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefVariableArray::InsertElement(
										   // property value corresponding to array; [in] 
										   ImplAAFPropertyValue * pInPropVal,
										   // zero-based index into elements in this array type;  [in]
										   aafUInt32  index,
										   // value to be inserted into this array; [in]
										   ImplAAFPropertyValue * pMemberPropVal)
{
	if (!pInPropVal)
		return AAFRESULT_NULL_PARAM;
	if (!pMemberPropVal)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	pInPropType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pInPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pInPropType);
	if( (ImplAAFTypeDef *)pInPropType != this )
		return AAFRESULT_BAD_TYPE;
		
	//CASE 1 -- if the Insert is at "0" postition - this implies a prepend, 
	//			SO - delegate to PrependElement() routine
	if (index == 0)
		return PrependElement(pInPropVal, pMemberPropVal);
	
	
	
	HRESULT hr = 0;
	
	aafUInt32  count;
	hr = GetCount (pInPropVal, &count);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	//CASE 2 -- if the Insert is at LAST position - this implies an Append,
	//			SO - delegate to AppendElement() routine
	if (index == count)
		return AppendElement(pInPropVal, pMemberPropVal);
	
	//CASE 3 -  invalid Index
	if (index > count)
		return AAFRESULT_BADINDEX;
	
	//All other VALID Cases follow ..............
	//   (index lying within (1) and (count-1) ) ...

	//Tom's  RefArray stuff:
	ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
	if (NULL != pRefArray)
	{
		return pRefArray->InsertElementAt(pMemberPropVal, index);
	}

	//regular insert stuff follows ....
	ImplAAFPropValData* inPvd =
		dynamic_cast<ImplAAFPropValData*> (pInPropVal);	
	if (!inPvd)
		return AAFRESULT_BAD_TYPE;

	ImplAAFPropValData* memPvd =
		dynamic_cast<ImplAAFPropValData*> (pMemberPropVal);
	if (!memPvd)
		return AAFRESULT_BAD_TYPE;
	
	aafUInt32 oldSize = 0;
	hr = inPvd->GetBitsSize (&oldSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	aafUInt32 newElemSize = 0;
	hr = memPvd->GetBitsSize (&newElemSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	aafUInt32 newSize = oldSize + newElemSize;
	// sizeof (*buf) must be 1
	aafUInt8* buf = new aafUInt8[newSize];
	
	aafMemPtr_t pBits = 0;
	aafMemPtr_t pInBits = 0;
	
	
	///  OK ... three parts: Pre-Index, Index, Post-Index
	
	// First part: copy upto Pre-Index into buffer
	hr = inPvd->GetBits (&pInBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	aafUInt32  preindex_size = (index-1) * newElemSize;
	memcpy (buf, pInBits, preindex_size);
	//incr. pInBits to remaining part
	pInBits+=preindex_size;
	
	//Second part: copy the Index (newElement)
	pBits = 0;
	hr = memPvd->GetBits (&pBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	// Make sure to incr. the buf ptr by the pre-index  size
	memcpy (buf + preindex_size, pBits, newElemSize);
	
	//Third part: Finally, copy Post-Index into buffer
	//		don't forget to point incr. the buf ptr with the pre-index + the new Elem
	aafUInt32 postincr = (preindex_size + newElemSize);
	aafUInt32 postindex_size = newSize - postincr; 
	memcpy (buf + postincr,   pInBits,  postindex_size);
	
	
	// Re-allocate prop val bits to hold newly expanded data
	pBits = 0;
	hr = inPvd->AllocateBits (newSize, &pBits);
	if (AAFRESULT_FAILED(hr)) return hr;
	memcpy (pBits, buf, newSize);
	
	delete[] buf;
	return AAFRESULT_SUCCESS;
}
	
