/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying 
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted, 
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
*  prior written permission of Avid Technology, Inc.
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

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;
extern "C" const aafClassID_t CLSID_AAFStrongRefArrayValue;
extern "C" const aafClassID_t CLSID_AAFWeakRefArrayValue;


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
	ImplAAFTypeDef *pTypeDef = _ElementType;
	
	*ppTypeDef = pTypeDef;
	assert (*ppTypeDef);
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
	
	assert (pTypeDef);
	if (! pTypeDef->IsVariableArrayable())
		return AAFRESULT_BAD_TYPE;
	
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
	assert (ptd);
	assert (ptd->IsFixedSize());
	aafUInt32 elemSize = ptd->PropValSize();
	aafUInt32 propSize;
	assert (pPropVal);
	
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData *>(pPropVal);
	
	assert (pvd);
	hr = pvd->GetBitsSize (&propSize);
	if (AAFRESULT_FAILED(hr)) return hr;
	assert (pCount);
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
	
	AAFRESULT hr;
	
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->AppendElement(pMemberPropVal);
  }

	ImplAAFPropValData* inPvd =
		dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	assert (inPvd);
	
	ImplAAFPropValData* memPvd =
		dynamic_cast<ImplAAFPropValData*> (pMemberPropVal);
	assert (memPvd);
	
	aafUInt32 oldSize = 0;
	hr = inPvd->GetBitsSize (&oldSize);
	assert (AAFRESULT_SUCCEEDED (hr));
	
	aafUInt32 newElemSize = 0;
	hr = memPvd->GetBitsSize (&newElemSize);
	assert (AAFRESULT_SUCCEEDED (hr));
	
	aafUInt32 newSize = oldSize + newElemSize;
	assert (newSize);
	// sizeof (*buf) must be 1
	aafUInt8* buf = new aafUInt8[newSize];
	assert (buf);
	
	aafMemPtr_t pBits = 0;
	
	// Copy old bits into our buffer
	if (oldSize)
	{
		pBits = 0;
		hr = inPvd->GetBits (&pBits);
		assert (AAFRESULT_SUCCEEDED (hr));
		assert (pBits);
		memcpy (buf, pBits, oldSize);
	}
	
	// Append new prop val onto end of our buffer
	pBits = 0;
	hr = memPvd->GetBits (&pBits);
	assert (AAFRESULT_SUCCEEDED (hr));
	assert (pBits);
	// Following ptr addition depends on buf being a byte pointer
	memcpy (buf+oldSize, pBits, newElemSize);
	
	// Re-allocate prop val bits to hold newly expanded data
	pBits = 0;
	hr = inPvd->AllocateBits (newSize, &pBits);
	assert (AAFRESULT_SUCCEEDED (hr));
	assert (pBits);
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
  *ppEnum = NULL;
  
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->GetElements(ppEnum);
  }

	return AAFRESULT_NOT_IN_CURRENT_VERSION;
}

ImplAAFTypeDefSP ImplAAFTypeDefVariableArray::BaseType() const
{
	ImplAAFTypeDefSP result;
	AAFRESULT hr = GetType (&result);
	assert (AAFRESULT_SUCCEEDED (hr));
	assert (result);
	return result;
}


void ImplAAFTypeDefVariableArray::reorder(OMByte* externalBytes,
										  size_t externalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
	assert (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 numElems = externalBytesSize / extElemSize;
	aafInt32 numBytesLeft = externalBytesSize;
	aafUInt32 elem = 0;
	
	for (elem = 0; elem < numElems; elem++)
	{
		ptd->reorder (externalBytes, extElemSize);
		externalBytes += extElemSize;
		numBytesLeft -= extElemSize;
		assert (numBytesLeft >= 0);
	}
}


size_t ImplAAFTypeDefVariableArray::externalSize(OMByte* /*internalBytes*/,
												 size_t internalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
	assert (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize;
	if (ptd->IsRegistered())
		intElemSize = ptd->NativeSize ();
	else
		intElemSize = extElemSize;
	
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	assert (intElemSize);
	aafUInt32 numElems = internalBytesSize / intElemSize;
	return numElems * extElemSize;
}


void ImplAAFTypeDefVariableArray::externalize(OMByte* internalBytes,
											  size_t internalBytesSize,
											  OMByte* externalBytes,
											  size_t externalBytesSize,
											  OMByteOrder byteOrder) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
	assert (ptd->IsFixedSize ());
	aafUInt32 intElemSize = ptd->NativeSize ();
	aafUInt32 extElemSize = ptd->PropValSize ();
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	if (intElemSize == extElemSize)
	{
		copy (internalBytes,
			externalBytes,
			externalBytesSize);
	}
	else
	{
		aafUInt32 numElems = internalBytesSize / intElemSize;
		aafInt32 intNumBytesLeft = internalBytesSize;
		aafInt32 extNumBytesLeft = externalBytesSize;
		aafUInt32 elem = 0;
		
		for (elem = 0; elem < numElems; elem++)
		{
			ptd->externalize (internalBytes,
				intElemSize,
				externalBytes,
				extElemSize,
				byteOrder);
			internalBytes += intElemSize;
			externalBytes += extElemSize;
			intNumBytesLeft -= intElemSize;
			extNumBytesLeft -= extElemSize;
			assert (intNumBytesLeft >= 0);
			assert (extNumBytesLeft >= 0);
		}
	}
}


size_t ImplAAFTypeDefVariableArray::internalSize(OMByte* /*externalBytes*/,
												 size_t externalBytesSize) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
	assert (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize;
	if (ptd->IsRegistered())
		intElemSize = ptd->NativeSize ();
	else
		intElemSize = extElemSize;
	
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	assert (intElemSize);
	aafUInt32 numElems = externalBytesSize / extElemSize;
	return numElems * intElemSize;
}


void ImplAAFTypeDefVariableArray::internalize(OMByte* externalBytes,
											  size_t externalBytesSize,
											  OMByte* internalBytes,
											  size_t internalBytesSize,
											  OMByteOrder byteOrder) const
{
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
	assert (ptd->IsFixedSize ());
	aafUInt32 extElemSize = ptd->PropValSize ();
	aafUInt32 intElemSize = ptd->NativeSize ();
	// aafUInt32 intElemSize = ptd->internalSize (0, 0);
	// aafUInt32 extElemSize = ptd->externalSize (0, 0);
	if (intElemSize == extElemSize)
	{
		assert (externalBytesSize <= internalBytesSize);
		copy (externalBytes,
			internalBytes,
			externalBytesSize);
	}
	else
	{
		aafUInt32 numElems = externalBytesSize / extElemSize;
		aafInt32 intNumBytesLeft = externalBytesSize;
		aafInt32 extNumBytesLeft = internalBytesSize;
		aafUInt32 elem = 0;
		
		for (elem = 0; elem < numElems; elem++)
		{
			ptd->internalize (externalBytes,
				extElemSize,
				internalBytes,
				intElemSize,
				byteOrder);
			internalBytes += intElemSize;
			externalBytes += extElemSize;
			intNumBytesLeft -= intElemSize;
			extNumBytesLeft -= extElemSize;
			assert (intNumBytesLeft >= 0);
			assert (extNumBytesLeft >= 0);
		}
	}
}


aafUInt32 ImplAAFTypeDefVariableArray::pvtCount
(
 ImplAAFPropertyValue * pInPropVal
 ) const
{
	assert (pInPropVal);
	AAFRESULT hr;
	aafUInt32 retval;
	hr = GetCount (pInPropVal, &retval);
	assert (AAFRESULT_SUCCEEDED(hr));
	return retval;
}


aafBool ImplAAFTypeDefVariableArray::IsFixedSize (void) const
{
	return kAAFFalse;
}


size_t ImplAAFTypeDefVariableArray::PropValSize (void) const
{
	assert (0);
	return 0; // not reached!
}


aafBool ImplAAFTypeDefVariableArray::IsRegistered (void) const
{
	// Only depends on registration of basic type.
	return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefVariableArray::NativeSize (void) const
{
	assert (0);
	return 0; // not reached!
}


OMProperty * ImplAAFTypeDefVariableArray::pvtCreateOMProperty
(OMPropertyId pid,
 const wchar_t * name) const
{
	assert (name);
	
	ImplAAFTypeDefSP ptd = BaseType ();
	assert (ptd);
	
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
          result = new OMWeakReferenceVectorProperty<ImplAAFMetaDefinition>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
      
        case PID_DefinitionObject_Identification:
          result = new OMWeakReferenceVectorProperty<ImplAAFDefObject>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
    
//			  case PID_Mob_MobID:
//          result = new OMWeakReferenceVectorProperty<ImplAAFMob>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
//          break;
//
//			  case PID_EssenceData_MobID:
//          result = new OMWeakReferenceVectorProperty<ImplAAFEssenceData>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
//          break;
    
        default:
          // No support for other "key properties"
          assert (0);
          break;
      }
    }
#endif // #else // #if defined(USE_SIMPLEPROPERTY)
	}
	
	else
	{
		// We don't support variable arrays of variably-sized properties.
		assert (ptd->IsFixedSize());
		
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
		result = new OMVariableSizeProperty<aafUInt8> (pid, name);
		
	}
	
	assert (result);
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
  assert (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  
  OMReferenceVectorProperty* pReferenceVectorProperty = dynamic_cast<OMReferenceVectorProperty*>(property);
  if (NULL != pReferenceVectorProperty)
  {
    assert (property->definition());
    if (NULL == property->definition())
      return AAFRESULT_INVALID_PARAM;
    const OMType *type = property->definition()->type();
    assert (type);
    ImplAAFTypeDefVariableArray *ptd = const_cast<ImplAAFTypeDefVariableArray *>
                            (dynamic_cast<const ImplAAFTypeDefVariableArray *>(type));
    assert (ptd);
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
//      assert (NULL != *ppPropertyValue);     
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
	
	aafUInt32 elemSize = BaseType()->NativeSize();
	
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
	
