/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTypeDefEnum_h__
#include "ImplAAFTypeDefEnum.h"
#endif

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif

#include <assert.h>
#include <string.h>

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif


extern "C" const aafClassID_t CLSID_AAFPropValData;

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFTypeDefEnum::ImplAAFTypeDefEnum ()
  : _ElementType   ( PID_TypeDefinitionEnumeration_ElementType,   "Element Type"),
	_ElementNames  ( PID_TypeDefinitionEnumeration_ElementNames,  "Element Names"),
	_ElementValues ( PID_TypeDefinitionEnumeration_ElementValues, "Element Values")
{
  _persistentProperties.put(_ElementType.address());
  _persistentProperties.put(_ElementNames.address());
  _persistentProperties.put(_ElementValues.address());
}


ImplAAFTypeDefEnum::~ImplAAFTypeDefEnum ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::Initialize (
      aafUID_t * pID,
      ImplAAFTypeDef * pType,
      aafInt64 * pElementValues,
      aafString_t * pElementNames,
      aafUInt32 numElements,
      wchar_t * pTypeName)
{
  if (!pID)
	return AAFRESULT_NULL_PARAM;
  if (!pTypeName)
    return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  eAAFTypeCategory_t baseTypeCat;  
  assert (pType);
  hr = pType->GetTypeCategory(&baseTypeCat);
  if (AAFRESULT_FAILED(hr))
	return hr;
  if (kAAFTypeCatInt != baseTypeCat)
	return AAFRESULT_BAD_TYPE;

  aafUInt32 i;
  aafUInt32 totalNameSize = 0;
  for (i = 0; i < numElements; i++)
	{
	  if ( !pElementNames[i])
		return AAFRESULT_NULL_PARAM;

	  totalNameSize += (wcslen (pElementNames[i]) + 1);
	}

  wchar_t * namesBuf = new wchar_t[totalNameSize];
  if (!namesBuf)
	return AAFRESULT_NOMEMORY;
  // make it an empty string
  *namesBuf = 0;
  wchar_t * tmpNamePtr = namesBuf;

  assert (0 == _ElementValues.count());
  for (i = 0; i < numElements; i++)
	{
	  assert (pElementNames[i]);
	  wcscpy(tmpNamePtr, pElementNames[i]);
	  // +1 to go past embedded null
	  tmpNamePtr += wcslen (pElementNames[i]) + 1;
	}

  aafUID_t typeUID;
  assert (pType);
  hr = pType->GetAUID(&typeUID);
  assert (AAFRESULT_SUCCEEDED(hr));
  _ElementType = typeUID;

  _ElementNames.setValue (namesBuf, totalNameSize * sizeof(wchar_t));
  delete[] namesBuf;
  _ElementValues.setValue (pElementValues, numElements*sizeof(aafInt64));
  assert (numElements == _ElementValues.count());
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetElementType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  ImplAAFHeader * pHead = NULL;
  ImplAAFDictionary * pDict = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;
	  hr = MyHeadObject(&pHead);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pHead);
	  hr = (pHead->GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pDict);

	  ImplAAFTypeDef * ptd = NULL;
	  aafUID_t id = _ElementType;
	  hr = pDict->LookupType (&id, &ptd);
	  if (AAFRESULT_FAILED(hr))
		throw hr;

	  *ppTypeDef = ptd;
	  (*ppTypeDef)->AcquireReference ();
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pHead);
  RELEASE_IF_SET (pDict);

  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::CountElements (
      aafUInt32 * pCount)
{
  if (!pCount) return AAFRESULT_NULL_PARAM;
  *pCount = _ElementValues.count();
  return AAFRESULT_SUCCESS;

}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetElementValue (
      aafUInt32 index,
      aafInt64 * pOutValue)
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

  aafInt64 val;
  _ElementValues.getValueAt (&val, index);
  assert (pOutValue);
  *pOutValue = val;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetNameFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetNameBufLenFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetNameFromInteger (
      aafInt64 value,
      wchar_t * pName,
      aafUInt32 bufSize)
{
  if (! pName)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  aafUInt32 len;
  // following call may return AAFRESULT_ILLEGAL_VALUE if value isn't
  // recognized
  hr = GetNameBufLenFromInteger (value, &len);
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
	  aafInt64 val;
	  hr = GetElementValue (i, &val);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  if (val == value)
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
ImplAAFTypeDefEnum::GetNameBufLenFromInteger (
      aafInt64 value,
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
	  aafInt64 val;
	  hr = GetElementValue (i, &val);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  if (val == value)
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
ImplAAFTypeDefEnum::GetIntegerValue (
      ImplAAFPropertyValue * pPropValIn,
      aafInt64 * pValueOut)
{
  if (! pPropValIn)
	return AAFRESULT_NULL_PARAM;
  if (! pValueOut)
	return AAFRESULT_NULL_PARAM;

  ImplAAFTypeDef * pBaseType = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;
	  hr = GetElementType (&pBaseType);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pBaseType);

	  eAAFTypeCategory_t baseTypeCat;
	  hr = pBaseType->GetTypeCategory(&baseTypeCat);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert(kAAFTypeCatInt == baseTypeCat);
	  ImplAAFTypeDefInt * ptdi = 0;
	  // This cast should succeed since we've already checked the type
	  // category
	  ptdi = dynamic_cast<ImplAAFTypeDefInt*>(pBaseType);
	  assert (ptdi);

  // Get the size of the base integer type
	  aafUInt32 baseIntSize;
	  hr = ptdi->GetSize (&baseIntSize);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pPropValIn);
	  aafInt64 retval;
	  switch (baseIntSize)
		{
		case 1:
		  aafUInt8 ui8Val;
		  hr = ptdi->GetInteger (pPropValIn,
								 (aafMemPtr_t) &ui8Val,
								 sizeof (ui8Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  retval = ui8Val;
		  break;

		case 2:
		  aafUInt16 ui16Val;
		  hr = ptdi->GetInteger (pPropValIn,
								 (aafMemPtr_t) &ui16Val,
								 sizeof (ui16Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  retval = ui16Val;
		  break;

		case 4:
		  aafUInt32 ui32Val;
		  hr = ptdi->GetInteger (pPropValIn,
								 (aafMemPtr_t) &ui32Val,
								 sizeof (ui32Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  retval = ui32Val;
		  break;

		case 8:
		  aafInt64 i64Val;
		  hr = ptdi->GetInteger (pPropValIn,
								 (aafMemPtr_t) &i64Val,
								 sizeof (i64Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  retval = i64Val;
		  break;

		default:
		  assert (0);
		}

	  assert (pValueOut);
	  *pValueOut = retval;
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pBaseType);

  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::SetIntegerValue (
      ImplAAFPropertyValue * pPropValToSet,
      aafInt64 valueIn)
{
  if (! pPropValToSet)
	return AAFRESULT_NULL_PARAM;

  ImplAAFTypeDef * pBaseType = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;
	  hr = GetElementType (&pBaseType);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pBaseType);

	  eAAFTypeCategory_t baseTypeCat;
	  hr = pBaseType->GetTypeCategory(&baseTypeCat);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert(kAAFTypeCatInt == baseTypeCat);
	  ImplAAFTypeDefInt * ptdi = 0;
	  // This cast should succeed since we've already checked the type
	  // category
	  ptdi = dynamic_cast<ImplAAFTypeDefInt*>(pBaseType);
	  assert (ptdi);

  // Get the size of the base integer type
	  aafUInt32 baseIntSize;
	  hr = ptdi->GetSize (&baseIntSize);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pPropValToSet);
	  switch (baseIntSize)
		{
		case 1:
		  if (valueIn > ((1 << 8)-1))
			throw AAFRESULT_ILLEGAL_VALUE;
		  if (valueIn < -(1<<8))
			throw AAFRESULT_ILLEGAL_VALUE;

		  aafUInt8 ui8Val;
		  ui8Val = (aafUInt8) valueIn;
		  hr = ptdi->SetInteger (pPropValToSet,
								 (aafMemPtr_t) &ui8Val,
								 sizeof (ui8Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  break;

		case 2:
		  if (valueIn > ((1 << 16)-1))
			throw AAFRESULT_ILLEGAL_VALUE;
		  if (valueIn < -(1<<16))
			throw AAFRESULT_ILLEGAL_VALUE;

		  aafUInt16 ui16Val;
		  ui16Val = (aafUInt16) valueIn;
		  hr = ptdi->SetInteger (pPropValToSet,
								 (aafMemPtr_t) &ui16Val,
								 sizeof (ui16Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  break;

		case 4:
		  if (valueIn > ((1 << 32)-1))
			throw AAFRESULT_ILLEGAL_VALUE;
		  if (valueIn < -(1<<32))
			throw AAFRESULT_ILLEGAL_VALUE;

		  aafUInt32 ui32Val;
		  ui32Val = (aafUInt32) valueIn;
		  hr = ptdi->SetInteger (pPropValToSet,
								 (aafMemPtr_t) &ui32Val,
								 sizeof (ui32Val));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  break;

		case 8:
		  hr = ptdi->SetInteger (pPropValToSet,
								 (aafMemPtr_t) &valueIn,
								 sizeof (valueIn));
		  if (AAFRESULT_FAILED(hr))
			throw hr;
		  break;

		default:
		  assert (0);
		}
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pBaseType);

  return rReturned;
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetTypeCategory (eAAFTypeCategory_t * pTid)
{
  if (! pTid)
	return AAFRESULT_NULL_PARAM;

  assert (pTid);
  *pTid = kAAFTypeCatEnum;
  return AAFRESULT_SUCCESS;
}


//
// private method
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefEnum::GetElementName (
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
ImplAAFTypeDefEnum::GetElementNameBufLen (
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


OMDEFINE_STORABLE(ImplAAFTypeDefEnum, AUID_AAFTypeDefEnum);
