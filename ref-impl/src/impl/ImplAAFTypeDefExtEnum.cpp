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

#ifndef __ImplAAFTypeDefExtEnum_h__
#include "ImplAAFTypeDefExtEnum.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "AAFUtils.h"

#include <assert.h>
#include <string.h>


#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

extern "C" const aafClassID_t CLSID_AAFPropValData;

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFTypeDefExtEnum::ImplAAFTypeDefExtEnum ()
  :	_ElementNames  ( PID_TypeDefinitionExtendibleEnumeration_ElementNames,  "Element Names"),
	_ElementValues ( PID_TypeDefinitionExtendibleEnumeration_ElementValues, "Element Values")
{
  _persistentProperties.put(_ElementNames.address());
  _persistentProperties.put(_ElementValues.address());
}


ImplAAFTypeDefExtEnum::~ImplAAFTypeDefExtEnum ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::Initialize (
      const aafUID_t * pID,
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
    ImplAAFTypeDefExtEnum::GetNameFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameBufLenFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameFromAUID (
      aafUID_t * pValue,
      wchar_t * pName,
      aafUInt32 bufSize)
{
  if (! pName)
	return AAFRESULT_NULL_PARAM;

  if (! pValue)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  aafUInt32 len;
  // following call may return AAFRESULT_ILLEGAL_VALUE if value isn't
  // recognized
  hr = GetNameBufLenFromAUID (pValue, &len);
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
	  if (EqualAUID (&val, pValue))
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
      aafUID_t * pValue,
      aafUInt32 * pLen)
{
  if (! pLen)
	return AAFRESULT_NULL_PARAM;

  if (! pValue)
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
	  if (EqualAUID (&val, pValue))
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
  ImplAAFDictionary * pDict = 0;
  ImplAAFTypeDef * ptd = 0;
  ImplAAFTypeDefRecord * ptAuid = 0;

  if (! pPropValIn)
	return AAFRESULT_NULL_PARAM;
  if (! pValueOut)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;
	  aafUID_t retval;

	  hr = GetDictionary (&pDict);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pDict);

	  hr = pDict->LookupType (&kAAFTypeID_AUID ,&ptd);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (ptd);

	  ptAuid = dynamic_cast<ImplAAFTypeDefRecord*> (ptd);
	  assert (ptAuid);

	  hr = ptAuid->GetStruct (pPropValIn, (aafMemPtr_t) &retval, sizeof (retval));
	  if (AAFRESULT_FAILED(hr))
		throw hr;

	  assert (pValueOut);
	  *pValueOut = retval;
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pDict);
  RELEASE_IF_SET (ptd);

  // Don't release this! It was dynamically-cast, not QI'd (and
  // therefore not ref counted).
  //RELEASE_IF_SET (ptAuid);

  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::SetAUIDValue (
      ImplAAFPropertyValue * pPropValToSet,
      aafUID_t * pValueIn)
{
  ImplAAFDictionary * pDict = 0;
  ImplAAFTypeDef * ptd = 0;
  ImplAAFTypeDefRecord * ptAuid = 0;

  if (! pPropValToSet)
	return AAFRESULT_NULL_PARAM;
  if (! pValueIn)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  AAFRESULT hr;

	  hr = GetDictionary (&pDict);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (pDict);

	  hr = pDict->LookupType (&kAAFTypeID_AUID ,&ptd);
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	  assert (ptd);

	  ptAuid = dynamic_cast<ImplAAFTypeDefRecord*> (ptd);
	  assert (ptAuid);

	  hr = ptAuid->SetStruct (pPropValToSet, (aafMemPtr_t) pValueIn, sizeof (aafUID_t));
	  if (AAFRESULT_FAILED(hr))
		throw hr;
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}
  RELEASE_IF_SET (pDict);
  RELEASE_IF_SET (ptd);

  // Don't release this! It was dynamically-cast, not QI'd (and
  // therefore not ref counted).
  //RELEASE_IF_SET (ptAuid);

  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::AppendElement (
      aafUID_t * pValue,
      wchar_t * pName)
{
  if (! pValue)
	return AAFRESULT_NULL_PARAM;
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
	  _ElementNames.getValue (namesBuf, origNameCharCount);

	  // Append new name to end of buffer.  Don't forget that original
	  // buffer may have embedded nulls, so start copying at desired
	  // point immediately (don't use strcat or equiv).
	  wcscpy (namesBuf+origNameCharCount, pName);

	  //
	  // now, calculate the new value
	  //

	  // add one for new element to be appended
	  valsBuf = new aafUID_t[origNumElems+1];
	  _ElementValues.getValue (valsBuf, origNumElems);
	  assert (pValue);
	  valsBuf[origNumElems] = *pValue;


	  // Copy newly-appended name and value buffers out.  Do these
	  // together, last so that if any errors occurred during the
	  // calculations we won't leave the property half-appended.
	  _ElementNames.setValue (namesBuf, newNameCharCount);
	  _ElementValues.setValue (valsBuf, origNumElems+1);
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


ImplAAFTypeDef * ImplAAFTypeDefExtEnum::GetBaseType ()
{
  AAFRESULT hr;
  ImplAAFDictionary * pDict = 0;
  hr = GetDictionary (&pDict);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (pDict);

  ImplAAFTypeDef * ptd = 0;
  hr = pDict->LookupType (&kAAFTypeID_AUID ,&ptd);
  pDict->ReleaseReference ();
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (ptd);

  return ptd;
}


void ImplAAFTypeDefExtEnum::reorder(OMByte* bytes,
									size_t bytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefExtEnum * pNonConstThis =
	(ImplAAFTypeDefExtEnum *) this;
  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  ptd->reorder (bytes, bytesSize);
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefExtEnum::externalSize(OMByte* internalBytes,
										   size_t internalBytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefExtEnum * pNonConstThis =
	(ImplAAFTypeDefExtEnum *) this;
  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  size_t result = ptd->externalSize (internalBytes, internalBytesSize);
  ptd->ReleaseReference ();
  return result;
}


void ImplAAFTypeDefExtEnum::externalize(OMByte* internalBytes,
										size_t internalBytesSize,
										OMByte* externalBytes,
										size_t externalBytesSize,
										OMByteOrder byteOrder) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefExtEnum * pNonConstThis =
	(ImplAAFTypeDefExtEnum *) this;
  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  ptd->externalize (internalBytes,
					internalBytesSize,
					externalBytes,
					externalBytesSize,
					byteOrder);
  ptd->ReleaseReference ();
}


size_t ImplAAFTypeDefExtEnum::internalSize(OMByte* externalBytes,
										   size_t externalBytesSize) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefExtEnum * pNonConstThis =
	(ImplAAFTypeDefExtEnum *) this;
  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  size_t result = ptd->internalSize (externalBytes, externalBytesSize);
  ptd->ReleaseReference ();
  return result;
}


void ImplAAFTypeDefExtEnum::internalize(OMByte* externalBytes,
										size_t externalBytesSize,
										OMByte* internalBytes,
										size_t internalBytesSize,
										OMByteOrder byteOrder) const
{
  // BobT hack: need non-const this pointer in order to call
  // GetBaseType(), and to do ReleaseReference() later.  Since we know
  // we're not changing this object for real, we don't *really* mind
  // cheating a bit on const-ness...
  ImplAAFTypeDefExtEnum * pNonConstThis =
	(ImplAAFTypeDefExtEnum *) this;
  ImplAAFTypeDef * ptd = pNonConstThis->GetBaseType ();
  ptd->internalize (internalBytes,
					internalBytesSize,
					externalBytes,
					externalBytesSize,
					byteOrder);
  ptd->ReleaseReference ();
}


OMDEFINE_STORABLE(ImplAAFTypeDefExtEnum, AUID_AAFTypeDefExtEnum);
