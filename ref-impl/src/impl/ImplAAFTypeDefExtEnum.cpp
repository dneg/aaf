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
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTypeDefExtEnum_h__
#include "ImplAAFTypeDefExtEnum.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "AAFUtils.h"

#include <assert.h>
#include <string.h>
#include <wchar.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;

ImplAAFTypeDefExtEnum::ImplAAFTypeDefExtEnum ()
:	_ElementNames  ( PID_TypeDefinitionExtendibleEnumeration_ElementNames,  L"ElementNames"),
_ElementValues ( PID_TypeDefinitionExtendibleEnumeration_ElementValues, L"ElementValues")
{
	_persistentProperties.put(_ElementNames.address());
	_persistentProperties.put(_ElementValues.address());
}

//some macros
#define check_hr(expr)\
{\
	HRESULT  the_hresult = (expr);\
	if (FAILED(the_hresult))\
	   return the_hresult;\
}

ImplAAFTypeDefExtEnum::~ImplAAFTypeDefExtEnum ()
{}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::Initialize (
								   const aafUID_t & id,
								   const aafCharacter * pTypeName)
{
	if (!pTypeName)
		return AAFRESULT_NULL_PARAM;
	
	AAFRESULT hr;
	
	hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::CountElements (
									  aafUInt32 * pCount)
{
	if (!pCount) return AAFRESULT_NULL_PARAM;
	*pCount = _ElementValues.count();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetElementValue (
										aafUInt32 index,
										aafUID_t * pOutValue)
{
	if (! pOutValue)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountElements (&count);
	if (AAFRESULT_FAILED(hr))
		return hr;
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	aafUID_t val;
	_ElementValues.getValueAt (&val, index);
	assert (pOutValue);
	*pOutValue = val;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::CreateValueFromName (
											/*[in]*/ aafCharacter_constptr  Name,
											/*[out]*/ ImplAAFPropertyValue ** ppPropVal)
{
	if (! ppPropVal )
		return AAFRESULT_NULL_PARAM;
	
	if (! Name )
		return AAFRESULT_NULL_PARAM;
	
	if (!IsRegistered())
		return AAFRESULT_NOT_INITIALIZED;
	
	
	//Now try to do a Name lookup
	aafUInt32 i=0;
	aafUInt32 count=0;
	aafBoolean_t  bFound = kAAFFalse;
	aafCharacter Name_buf[256]; 
	aafUInt32 bufSize = 256;
	aafUID_t the_value = {0};
	
	check_hr ( CountElements(&count) );
	while ( (i<count) && !bFound)
	{
		check_hr ( GetElementName (i, Name_buf, bufSize) );
		if ( wcscmp (Name_buf, Name) == 0 ) //matched
		{
			bFound = kAAFTrue;
			
			check_hr (GetElementValue(i, &the_value));
			break;
			
		}//if
		i++;
	}//while
	
	if (!bFound)
		return AAFRESULT_INVALID_PARAM;
	
	//else FOUND
	
	
	//Now allocate a New PV based on the local INT size ....
	
	ImplAAFTypeDefSP ptd;
	ImplAAFTypeDefRecordSP ptdAuid;
	
	ptd = BaseType ();
	assert (ptd);
	
	ptdAuid = dynamic_cast<ImplAAFTypeDefRecord*> ((ImplAAFTypeDef*) ptd);
	assert (ptdAuid);
	
	HRESULT hr = ptdAuid->CreateValueFromStruct ((aafMemPtr_t) &the_value, sizeof (aafUID_t),
		ppPropVal);
	
	return hr;
}

// Note: The type of input pValue is validated in GetAUIDValue().
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetNameFromValue (
										 ImplAAFPropertyValue * pValue,
										 wchar_t *  pName,
										 aafUInt32  bufSize)
{
	if (! pValue)
		return AAFRESULT_NULL_PARAM;
	
	if (! pName)
		return AAFRESULT_NULL_PARAM;
	
	//Extract the AUID value from the PV;  use existing method on AUID!
	// pValue type is validated in GetAUIDValue().
	aafUID_t val = {0};
	HRESULT	 hr = GetAUIDValue(pValue, &val);
	if( AAFRESULT_FAILED( hr ) )
	    return hr;
	
	//Use an existing method on AUID, to find out the name!!! 
	return (GetNameFromAUID(val, pName, bufSize));	
}



// Note: The type of input pValue is validated in GetAUIDValue().
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetNameBufLenFromValue (
											   ImplAAFPropertyValue * pValue,
											   aafUInt32 *  pLen)
{
	if (! pValue)
		return AAFRESULT_NULL_PARAM;
	
	if (! pLen)
		return AAFRESULT_NULL_PARAM;
	
	//Extract the AUID value from the PV;  use existing method on AUID!
	// pValue type is validated in GetAUIDValue().
	aafUID_t val = {0};
	HRESULT	 hr = GetAUIDValue(pValue, &val);
	if( AAFRESULT_FAILED( hr ) )
	    return hr;
	
	//Use an existing method on AUID, to find out the Buffer Length!!! 
	return (GetNameBufLenFromAUID(val, pLen));
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetNameFromAUID (
										const aafUID_t & value,
										wchar_t * pName,
										aafUInt32 bufSize)
{
	if (! pName)
		return AAFRESULT_NULL_PARAM;
	
	AAFRESULT hr;
	aafUInt32 len;
	// following call may return AAFRESULT_ILLEGAL_VALUE if value isn't
	// recognized
	hr = GetNameBufLenFromAUID (value, &len);
	if (AAFRESULT_FAILED(hr))
		return hr;
	
	// len includes space for trailing null
	if (bufSize < len)
		return AAFRESULT_SMALLBUF;
	
	aafUInt32 i;
	aafUInt32 count;
	hr = CountElements(&count);
	if (AAFRESULT_FAILED(hr))
		return hr;
	for (i = 0; i < count; i++)
	{
		aafUID_t val;
		hr = GetElementValue (i, &val);
		if (AAFRESULT_FAILED(hr))
			return hr;
		if (EqualAUID (&val, &value))
		{
			// given integer value matches value of "i"th element.
			hr = GetElementName(i, pName, bufSize);
			if (AAFRESULT_FAILED(hr))
				return hr;
			return AAFRESULT_SUCCESS;
		}
	}
	// fell out of for() loop, so we didn't find it.
	// redundant, since GetNameBufLenFromInteger() should have already
	// found it.
	return AAFRESULT_ILLEGAL_VALUE;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetNameBufLenFromAUID (
											  const aafUID_t & value,
											  aafUInt32 * pLen)
{
	if (! pLen)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 i;
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountElements(&count);
	if (AAFRESULT_FAILED(hr))
		return hr;
	for (i = 0; i < count; i++)
	{
		aafUID_t val;
		hr = GetElementValue (i, &val);
		if (AAFRESULT_FAILED(hr))
			return hr;
		if (EqualAUID (&val, &value))
		{
			aafUInt32 len;
			hr = GetElementNameBufLen(i, &len);
			if (AAFRESULT_FAILED(hr))
				return hr;
			assert (pLen);
			*pLen = len;
			return AAFRESULT_SUCCESS;
		}
	}
	// fell out of for() loop, so we didn't find it.
	return AAFRESULT_ILLEGAL_VALUE;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetAUIDValue (
									 ImplAAFPropertyValue * pPropValIn,
									 aafUID_t * pValueOut)
{
	ImplAAFTypeDefSP ptd;
	ImplAAFTypeDefRecordSP ptAuid;
	
	if (! pPropValIn)
		return AAFRESULT_NULL_PARAM;
	if (! pValueOut)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	spPropType;
	if( AAFRESULT_FAILED( pPropValIn->GetType( &spPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	assert (spPropType);
	if( (ImplAAFTypeDef *)spPropType != this )
		return AAFRESULT_BAD_TYPE;

	AAFRESULT hr;
	aafUID_t retval;
	
	ptd = BaseType ();
	assert (ptd);
	
	ptAuid = dynamic_cast<ImplAAFTypeDefRecord*> ((ImplAAFTypeDef*) ptd);
	assert (ptAuid);
	
	hr = ptAuid->GetStruct (pPropValIn, (aafMemPtr_t) &retval, sizeof (retval));
	if (AAFRESULT_FAILED(hr))
		return hr;
	
	assert (pValueOut);
	*pValueOut = retval;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::SetAUIDValue (
									 ImplAAFPropertyValue * pPropValToSet,
									 const aafUID_t & valueIn)
{
	ImplAAFTypeDefSP ptd;
	ImplAAFTypeDefRecordSP ptAuid;
	
	if (! pPropValToSet)
		return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the local type.
	ImplAAFTypeDefSP	spPropType;
	if( AAFRESULT_FAILED( pPropValToSet->GetType( &spPropType ) ) )
		return AAFRESULT_BAD_TYPE;
	assert (spPropType);
	if( (ImplAAFTypeDef *)spPropType != this )
		return AAFRESULT_BAD_TYPE;


	AAFRESULT hr;
	
	// Call this method to find out if this is a legal value
	aafUInt32 tmp; // unused
	hr = GetNameBufLenFromAUID (valueIn, &tmp);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	ptd = BaseType ();
	assert (ptd);
	
	ptAuid = dynamic_cast<ImplAAFTypeDefRecord*> ((ImplAAFTypeDef*) ptd);
	assert (ptAuid);
	
	hr = ptAuid->SetStruct (pPropValToSet, (aafMemPtr_t) &valueIn, sizeof (aafUID_t));
	if (AAFRESULT_FAILED(hr))
		return hr;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::AppendElement (
									  const aafUID_t & value,
									  const aafCharacter * pName)
{
	if (! pName)
		return AAFRESULT_NULL_PARAM;
	
	AAFRESULT hr;
	aafUInt32 origNumElems = 0;
	hr = CountElements(&origNumElems);
	if (AAFRESULT_FAILED(hr))
		return hr;
	
	aafWChar * namesBuf = 0;
	aafUID_t * valsBuf = 0;
	
	AAFRESULT rReturned = AAFRESULT_SUCCESS;
	try
	{
		//
		// First, calculate new names
		//
		
		aafUInt32 origNameCharCount = 0;
		aafUInt32 newNameCharCount = 0;
		
		// _ElementNames.count() includes final trailing null
		origNameCharCount = _ElementNames.count();
		assert (pName);
		
		// Add length for name to be appended.  Don't forget to add one
		// character for new name's trailing null
		newNameCharCount = origNameCharCount + wcslen (pName) + 1;
		namesBuf = new aafWChar[newNameCharCount];
		if (origNameCharCount)
			_ElementNames.getValue (namesBuf, origNameCharCount*sizeof(aafWChar));
		
		// Append new name to end of buffer.  Don't forget that original
		// buffer may have embedded nulls, so start copying at desired
		// point immediately (don't use strcat or equiv).
		wcscpy (namesBuf+origNameCharCount, pName);
		
		//
		// now, calculate the new value
		//
		
		// add one for new element to be appended
		valsBuf = new aafUID_t[origNumElems+1];
		if (origNumElems)
			_ElementValues.getValue (valsBuf, origNumElems*sizeof(aafUID_t));
		valsBuf[origNumElems] = value;
		
		
		// Copy newly-appended name and value buffers out.  Do these
		// together, last so that if any errors occurred during the
		// calculations we won't leave the property half-appended.
		_ElementNames.setValue (namesBuf, newNameCharCount*sizeof(aafWChar));
		_ElementValues.setValue (valsBuf, (origNumElems+1)*sizeof (aafUID_t));
	}
	catch (AAFRESULT &rCaught)
	{
		rReturned = rCaught;
	}
	
	if (namesBuf) delete[] namesBuf;
	if (valsBuf) delete[] valsBuf;
	
	return rReturned;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
	if (! pTid)
		return AAFRESULT_NULL_PARAM;
	
	assert (pTid);
	*pTid = kAAFTypeCatExtEnum;
	return AAFRESULT_SUCCESS;
}


//
// private method
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetElementName (
									   aafUInt32 index,
									   wchar_t * pName,
									   aafUInt32  bufSize)
{
	AAFRESULT hr;
	aafUInt32 count;
	aafUInt32 indexIntoProp;
	aafUInt32 currentIndex;
	
	if (!pName) return AAFRESULT_NULL_PARAM;
	
	hr = CountElements(&count);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	if (index >= count) return AAFRESULT_ILLEGAL_VALUE;
	
	wchar_t c;
	size_t numChars = _ElementNames.count();
	indexIntoProp = 0;
	currentIndex = 0;
	if (0 != index)
	{
		for (size_t i = 0; i < numChars; i++)
		{
			indexIntoProp++;
			_ElementNames.getValueAt(&c, i);
			if (0 == c)
			{
				// We've found the null just before the string we want.
				// We'll increment the indexIntoProp to the start of the
				// string and break out of the loop, but first make sure
				// there's more string there to index into.
				assert (i < numChars);
				currentIndex++;
				if (index == currentIndex)
					break;
			}
		}
		// Make sure we didn't terminate the loop by dropping out before
		// the correct index was found.
		assert (indexIntoProp < numChars);
	}
	
	// indexIntoProp now indicates the starting char we want.  Copy it
	// into the client's buffer.
	do
	{
		if (! bufSize) return AAFRESULT_SMALLBUF;
		_ElementNames.getValueAt(&c, indexIntoProp++);
		// BobT Note!!! We're cheating here, modifying client data
		// before we're sure this method will succeed.
		*pName++ = c;
		bufSize--;
	}
	while (c);
	return AAFRESULT_SUCCESS;
}



//
// private method
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefExtEnum::GetElementNameBufLen (
											 aafUInt32  index,
											 aafUInt32 * pLen)
{
	AAFRESULT hr;
	aafUInt32 count;
	aafUInt32 indexIntoProp;
	aafUInt32 currentIndex;
	
	if (!pLen) return AAFRESULT_NULL_PARAM;
	
	hr = CountElements(&count);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	if (index >= count) return AAFRESULT_ILLEGAL_VALUE;
	
	wchar_t c;
	size_t numChars = _ElementNames.count();
	indexIntoProp = 0;
	currentIndex = 0;
	if (0 != index)
	{
		for (size_t i = 0; i < numChars; i++)
		{
			indexIntoProp++;
			_ElementNames.getValueAt(&c, i);
			if (0 == c)
			{
				// We've found the null just before the string we want.
				// We'll increment the indexIntoProp to the start of the
				// string and break out of the loop, but first make sure
				// there's more string there to index into.
				assert (i < numChars);
				currentIndex++;
				if (index == currentIndex)
					break;
			}
		}
		// Make sure we didn't terminate the loop by dropping out before
		// the correct index was found.
		assert (indexIntoProp < numChars);
	}
	
	// indexIntoProp now indicates the starting char we want.  Start
	// counting until we get to the next null.
	aafUInt32 nameLength = 0;
	do
	{
		_ElementNames.getValueAt(&c, indexIntoProp++);
		if (c) nameLength += sizeof(wchar_t);
	}
	while (c);
	
	// increment once more for trailing null
	nameLength += sizeof (wchar_t);
	
	assert (pLen);
	*pLen = nameLength;
	return AAFRESULT_SUCCESS;
}


ImplAAFTypeDefSP ImplAAFTypeDefExtEnum::BaseType () const
{
	if (! _cachedBaseType)
	{
		AAFRESULT hr;
		ImplAAFDictionarySP pDict;
		hr = GetDictionary (&pDict);
		assert (AAFRESULT_SUCCEEDED(hr));
		assert (pDict);
		
		hr = pDict->LookupTypeDef (kAAFTypeID_AUID, &((ImplAAFTypeDefExtEnum*)this)->_cachedBaseType);
		assert (AAFRESULT_SUCCEEDED(hr));
		assert (_cachedBaseType);
	}
	return _cachedBaseType;
}


void ImplAAFTypeDefExtEnum::reorder(OMByte* bytes,
									size_t bytesSize) const
{
	BaseType()->reorder (bytes, bytesSize);
}


size_t ImplAAFTypeDefExtEnum::externalSize(OMByte* internalBytes,
										   size_t internalBytesSize) const
{
	return BaseType()->externalSize (internalBytes, internalBytesSize);
}


void ImplAAFTypeDefExtEnum::externalize(OMByte* internalBytes,
										size_t internalBytesSize,
										OMByte* externalBytes,
										size_t externalBytesSize,
										OMByteOrder byteOrder) const
{
	BaseType()->externalize (internalBytes,
		internalBytesSize,
		externalBytes,
		externalBytesSize,
		byteOrder);
}


size_t ImplAAFTypeDefExtEnum::internalSize(OMByte* externalBytes,
										   size_t externalBytesSize) const
{
	return BaseType()->internalSize (externalBytes, externalBytesSize);
}


void ImplAAFTypeDefExtEnum::internalize(OMByte* externalBytes,
										size_t externalBytesSize,
										OMByte* internalBytes,
										size_t internalBytesSize,
										OMByteOrder byteOrder) const
{
	BaseType()->internalize (externalBytes,
		externalBytesSize,
		internalBytes,
		internalBytesSize,
		byteOrder);
}


aafBool ImplAAFTypeDefExtEnum::IsFixedSize (void) const
{
	return kAAFTrue;
}


size_t ImplAAFTypeDefExtEnum::PropValSize (void) const
{
	return BaseType()->PropValSize ();
}


aafBool ImplAAFTypeDefExtEnum::IsRegistered (void) const
{
	return BaseType()->IsRegistered ();
}


size_t ImplAAFTypeDefExtEnum::NativeSize (void) const
{
	return BaseType()->NativeSize ();
}


OMProperty * ImplAAFTypeDefExtEnum::pvtCreateOMProperty
(OMPropertyId pid,
 const wchar_t * name) const
{
	assert (name);
	size_t elemSize = PropValSize ();
	OMProperty * result = new OMSimpleProperty (pid, name, elemSize);
	assert (result);
	return result;
}


bool ImplAAFTypeDefExtEnum::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefExtEnum::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefExtEnum::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefExtEnum::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefExtEnum::IsStringable () const
{ return true; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefExtEnum::classId(void) const
{
	return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefExtEnum));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefExtEnum::onSave(void* clientContext) const
{
	ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefExtEnum::onRestore(void* clientContext) const
{
	ImplAAFTypeDef::onRestore(clientContext);
}
