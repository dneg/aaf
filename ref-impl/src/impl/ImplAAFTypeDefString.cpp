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

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#ifndef __ImplAAFTypeDefCharacter_h__
#include "ImplAAFTypeDefCharacter.h"
#endif

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
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

ImplAAFTypeDefString::ImplAAFTypeDefString ()
  : _ElementType  ( PID_TypeDefinitionString_ElementType,
                    L"ElementType",
                    L"/MetaDictionary/TypeDefinitions", 
                    PID_MetaDefinition_Identification)
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefString::~ImplAAFTypeDefString ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::Initialize (
      const aafUID_t & id,
      ImplAAFTypeDef * pTypeDef,
      const aafCharacter * pTypeName)
{
  if (! pTypeDef)  return AAFRESULT_NULL_PARAM;

  assert (pTypeDef);
  if (! pTypeDef->IsStringable())
	return AAFRESULT_BAD_TYPE;

  // Check if specified type definition is in the dictionary.
  if( !aafLookupTypeDef( this, pTypeDef ) )
	return AAFRESULT_TYPE_NOT_FOUND;

  return pvtInitialize (id, pTypeDef, pTypeName);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::pvtInitialize (
      const aafUID_t & id,
      const ImplAAFTypeDef * pTypeDef,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  HRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _ElementType = pTypeDef;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetType (
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
    ImplAAFTypeDefString::GetCount (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 *  pCount)
{
  ImplAAFTypeDefSP pIncomingType;
  ImplAAFTypeDefSP ptd;
  AAFRESULT hr;

  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pCount) return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  if( AAFRESULT_FAILED( pPropVal->GetType( &pIncomingType ) ) )
	return AAFRESULT_BAD_TYPE;
  assert (pIncomingType);
  if( (ImplAAFTypeDef *)pIncomingType != this )
	return AAFRESULT_BAD_TYPE;

  hr = GetType (&ptd);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ptd);
  assert (ptd->IsFixedSize());
  aafUInt32 elemSize = ptd->ActualSize();
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



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::CreateValueFromCString (
	  aafMemPtr_t  pInitData,
	  aafUInt32  initDataSize,
	  ImplAAFPropertyValue ** ppPropVal)
{
  if (! pInitData)
	return AAFRESULT_NULL_PARAM;

  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  if (! IsRegistered ())
	return AAFRESULT_NOT_REGISTERED;

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

  AAFRESULT hr;
  hr = pvd->Initialize(this);
  if (! AAFRESULT_SUCCEEDED (hr))
	return hr;

  hr = SetCString (pvd, pInitData, initDataSize);
  if (AAFRESULT_FAILED (hr))
	return hr;

  assert (ppPropVal);
  *ppPropVal = pvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::SetCString (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t pData,
      aafUInt32 dataSize)
{
  if (! pPropVal)
	return AAFRESULT_NULL_PARAM;

  if (! pData)
	return AAFRESULT_NULL_PARAM;

  if (! IsRegistered ())
	return AAFRESULT_NOT_REGISTERED;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pIncomingType;
  if( AAFRESULT_FAILED( pPropVal->GetType( &pIncomingType ) ) )
	return AAFRESULT_BAD_TYPE;
  assert (pIncomingType);
  if( (ImplAAFTypeDef *)pIncomingType != this )
	return AAFRESULT_BAD_TYPE;

  AAFRESULT hr;
  ImplAAFTypeDefSP pBaseType;
  hr = GetType (&pBaseType);

  assert (pBaseType->IsFixedSize ());
  pBaseType->AttemptBuiltinRegistration ();
  assert (pBaseType->IsRegistered ());
  // Size of individual elements
  aafUInt32 elemSize = pBaseType->NativeSize ();
  // number of elements in input data.  If this is not an integral
  // number, this will round down and the test below will fail.
  aafUInt32 elemCount = dataSize / elemSize;
  // The size of the new property, calculated from number of elements
  // and the size of each element.
  aafUInt32 propSize = elemSize * elemCount;

  // If the given dataSize was not an integral multiple of the size of
  // each element, then we'll signal an error.
  if (propSize != dataSize)
	return AAFRESULT_BAD_SIZE;

  ImplAAFPropValData * pvd = 0;
  assert (pPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*> (pPropVal);
  assert (pvd);

  aafMemPtr_t pBits = 0;
  hr = pvd->AllocateBits (propSize, &pBits);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (pBits);

  memcpy (pBits, pData, propSize);
  return AAFRESULT_SUCCESS;
}


template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IUnknown	*pUnk1=NULL, *pUnk2=NULL;
	
	pUnk1 = static_cast<IUnknown *> (cls1->GetContainer());
	assert (pUnk1);

	pUnk2 = static_cast<IUnknown *> (cls2->GetContainer());
	assert (pUnk2);
	
	if (pUnk1 == pUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;

}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::AppendElements (
      ImplAAFPropertyValue * pInPropVal,
      aafMemPtr_t  pElements)
{
	if (! pInPropVal)
		return AAFRESULT_NULL_PARAM;
	
	if (! pElements)
		return AAFRESULT_NULL_PARAM;
	
	if (! IsRegistered ())
		return AAFRESULT_NOT_REGISTERED;
	
	AAFRESULT hr;
	
	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP  pIncomingType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	assert (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;
	
	ImplAAFTypeDefSP  pBaseType;
	hr = GetType (&pBaseType);
	
	//do the size thing ...
	
	assert (pBaseType->IsFixedSize ());
	pBaseType->AttemptBuiltinRegistration ();
	assert (pBaseType->IsRegistered ());
	// Size of individual elements
	aafUInt32 elementSize = pBaseType->NativeSize ();
	
	// Get the current size of the property
    aafUInt32 originalDataSize;
	
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData *>(pInPropVal);
	assert (pvd);
	hr = pvd->GetBitsSize (&originalDataSize);
	
	//get the data
	aafMemPtr_t pOriginalData = NULL;
	hr = pvd->GetBits (&pOriginalData);
	assert(hr == AAFRESULT_SUCCESS);
		
	/////
	//Now, find out what additional size we need based on the new data coming in.
	
	//first, see how many elements we have
	aafMemPtr_t pNewData = pElements;
	
	aafUInt32 newElemCount =0;
	
	//outer loop of the entire memory buffer passed in ...
	while (pNewData)
	{
		aafUInt32 count_of_zeroes = 0;

		//inner loop - chunking in size of elementSize
		for (aafUInt32 i=0; i<elementSize; i++, pNewData++)
			if (*pNewData == 0)
				count_of_zeroes++;
		
		if (count_of_zeroes == elementSize)
			//we have a null! ... done!
			break;
		
		//otherwise, increment new element count, and move on
		newElemCount++;
		
	}//while
	
	
	//At this point, our newElemCount holds a count of new elements to be added 
	//and the new size of bits is:
	aafUInt32 newsize = (newElemCount+1/*don't forget EOS*/) * elementSize;
	
	//Add this "newsize" to the original originalDataSize to get the new Total buffer size
	aafUInt32 TotalSize = originalDataSize + newsize;
	
	//Save the orginal buffer, before we re-allocate
	aafMemPtr_t tmp_buffer = new aafUInt8[originalDataSize+1];
	memcpy(tmp_buffer, pOriginalData, originalDataSize);
	
	//Allocate the grand total # of bits (orginal + the new stuff) ...
	aafMemPtr_t pBits = 0;
	hr = pvd->AllocateBits (TotalSize, &pBits);
	if (AAFRESULT_FAILED (hr))
		return hr;
	assert (pBits);
	
	//copy over the first part
	memcpy (pBits, tmp_buffer, originalDataSize);
	pBits += originalDataSize;
	
	//copy over the second part
	memcpy (pBits, pElements, newsize);
	
	//delete our tmp_buffer
	delete [] tmp_buffer;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetElements (
      ImplAAFPropertyValue * pInPropVal,
      aafMemPtr_t pBuffer,
      aafUInt32 bufferSize)
{
  AAFRESULT hr;

  if (! pInPropVal) return AAFRESULT_NULL_PARAM;
  if (! pBuffer) return AAFRESULT_NULL_PARAM;

  if (! IsRegistered ())
	return AAFRESULT_NOT_REGISTERED;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pIncomingType;
  if( AAFRESULT_FAILED( pInPropVal->GetType( &pIncomingType ) ) )
	return AAFRESULT_BAD_TYPE;
  assert (pIncomingType);
  if( (ImplAAFTypeDef *)pIncomingType != this )
	return AAFRESULT_BAD_TYPE;

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pInPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  aafUInt32 propBitsSize;
  hr = pvd->GetBitsSize(&propBitsSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  if (bufferSize < propBitsSize)
	return AAFRESULT_SMALLBUF;

  aafMemPtr_t pBits = NULL;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pBits);

  assert (pBuffer);
  assert (propBitsSize <= bufferSize);
  memcpy (pBuffer, pBits, propBitsSize);

  return AAFRESULT_SUCCESS;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefString::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatString;
  return AAFRESULT_SUCCESS;
}


ImplAAFTypeDefSP ImplAAFTypeDefString::BaseType() const
{
  ImplAAFTypeDefSP result;
  AAFRESULT hr = GetType (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (result);
  return result;
}


void ImplAAFTypeDefString::reorder(OMByte* externalBytes,
								   size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

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


size_t ImplAAFTypeDefString::externalSize(const OMByte* /*internalBytes*/,
										  size_t internalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->ActualSize ();
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = internalBytesSize / intElemSize;
  return numElems * extElemSize;
}


void ImplAAFTypeDefString::externalize(const OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->ActualSize ();
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


size_t ImplAAFTypeDefString::internalSize(const OMByte* /*externalBytes*/,
										  size_t externalBytesSize) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->ActualSize ();
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
  assert (intElemSize);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  return numElems * intElemSize;
}


void ImplAAFTypeDefString::internalize(const OMByte* externalBytes,
									   size_t externalBytesSize,
									   OMByte* internalBytes,
									   size_t internalBytesSize,
									   OMByteOrder byteOrder) const
{
  ImplAAFTypeDefSP ptd = BaseType();
  assert (ptd);

  assert (ptd->IsFixedSize ());
  aafUInt32 extElemSize = ptd->PropValSize ();
  aafUInt32 intElemSize = ptd->ActualSize ();
  // aafUInt32 intElemSize = ptd->internalSize (0, 0);
  // aafUInt32 extElemSize = ptd->externalSize (0, 0);
  aafUInt32 numElems = externalBytesSize / extElemSize;
  aafInt32 intNumBytesLeft = internalBytesSize;
  aafInt32 extNumBytesLeft = externalBytesSize;
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


aafBool ImplAAFTypeDefString::IsFixedSize (void) const
{
  return kAAFFalse;
}


size_t ImplAAFTypeDefString::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefString::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefString::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


OMProperty * ImplAAFTypeDefString::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);

  ImplAAFTypeDefSP ptd = BaseType ();
  assert (ptd);

  OMProperty * result = 0;


  ImplAAFTypeDefCharacter * ptdCharacter = 
	dynamic_cast<ImplAAFTypeDefCharacter*>((ImplAAFTypeDef*) ptd);
  if (ptdCharacter)
  {
    result = new OMWideStringProperty(pid, name);
  }
  else
  {
    ImplAAFTypeDefInt * ptdi = 
	  dynamic_cast<ImplAAFTypeDefInt*>((ImplAAFTypeDef*) ptd);
    assert (ptdi);
    if (ptdi)
	  {
	    // element is integral type
	    aafUInt32 intSize;
	    ptdi->GetSize (&intSize);
	    switch (intSize)
		  {
		  case 1:
		    result = new OMVariableSizeProperty<aafUInt8> (pid, name);
		    break;
		  case 2:
		    result = new OMVariableSizeProperty<aafUInt16> (pid, name);
		    break;
		  case 4:
		    result = new OMVariableSizeProperty<aafUInt32> (pid, name);
		    break;
		  case 8:
		    result = new OMVariableSizeProperty<aafInt64> (pid, name);
		    break;
		  default:
		    // We only support strings of those types.
		    assert (0);
		  }
	  }
  }

  // If result wasn't set above, we don't support the type.
  assert (result);
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}


bool ImplAAFTypeDefString::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefString::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefString::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefString::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefString::IsStringable () const
{ return false; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefString::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefString));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefString::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefString::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}
