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

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;


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
ImplAAFTypeDefVariableArray::CreateEmptyValue
(ImplAAFPropertyValue ** ppPropVal)
{
  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  ImplAAFPropValDataSP pvd;
  ImplAAFPropValData * tmp;
  tmp = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!tmp) return AAFRESULT_NOMEMORY;
  pvd = tmp;
  // the pvd smart pointer will maintain a reference for us...
  aafUInt32 refCount;
  refCount = tmp->ReleaseReference ();
  // ...make sure it really does
  assert (1 == refCount);

  assert (ppPropVal);
  *ppPropVal = pvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::CreateValueFromValues (
      ImplAAFPropertyValue ** ppElementValues,
      aafUInt32  numElements,
      ImplAAFPropertyValue ** ppPropVal)
{
	AAFRESULT hr;

	//first validate params + basic stuff ...
	if (!ppElementValues || !ppPropVal)
		return AAFRESULT_NULL_PARAM;

	//verify that all the individual elem types are the same as each other,
	// AND that each of them is VARIABLE  Arrayable

	//get Base TD and size
	ImplAAFTypeDefSP spTargetTD;
	hr = GetType(&spTargetTD); //gets base elem type
	if (AAFRESULT_FAILED (hr)) 
		return hr;
	aafUInt32 targetElemSize = spTargetTD->NativeSize();

	for (aafUInt32 i=0; i<numElements; i++)
	{
		//get  source size
		ImplAAFTypeDefSP  spSourceTD;
		hr = ppElementValues[i]->GetType (&spSourceTD);
		if (AAFRESULT_FAILED (hr)) 
			return hr;

		//verify FIXED Arrayable
		if (! spSourceTD->IsVariableArrayable())
			return AAFRESULT_BAD_TYPE;

		//verify that spTargetTD == spSourceTD
		if (spSourceTD != spTargetTD )
			return AAFRESULT_BAD_TYPE;

		//verify that the target elem size is equal to that of source 
		aafUInt32 sourceSize = spSourceTD->NativeSize();	
		if (sourceSize != targetElemSize )
			return AAFRESULT_BAD_SIZE;

	}//for each elem

	// All params validated; proceed ....

	//... just defer to Base-Class Array implementation:
	return ImplAAFTypeDefArray::CreateValueFromValues(ppElementValues, numElements, ppPropVal);
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


size_t ImplAAFTypeDefVariableArray::externalSize(OMByte* internalBytes,
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


size_t ImplAAFTypeDefVariableArray::internalSize(OMByte* externalBytes,
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
	  copy (externalBytes,
			internalBytes,
			internalBytesSize);
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
