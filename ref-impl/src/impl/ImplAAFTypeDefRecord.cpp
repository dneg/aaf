/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

#if defined(macintosh) || defined(_MAC)
#include <wstring.h>
#endif

extern "C" const aafClassID_t CLSID_AAFPropValData;

#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


ImplAAFTypeDefRecord::ImplAAFTypeDefRecord ()
  : _memberTypes ( PID_TypeDefinitionRecord_MemberTypes, "Member Types"),
	_memberNames ( PID_TypeDefinitionRecord_MemberNames, "Member Names"),
	_registeredOffsets (0),
	_registeredSize (0)
{
  _persistentProperties.put(_memberTypes.address());
  _persistentProperties.put(_memberNames.address());
}


ImplAAFTypeDefRecord::~ImplAAFTypeDefRecord ()
{
  if (_registeredOffsets)
	delete[] _registeredOffsets;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::Initialize (
      aafUID_t * pID,
      ImplAAFTypeDef ** ppMemberTypes,
      aafString_t * pMemberNames,
      aafUInt32 numMembers,
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

  aafUInt32 i;
  aafUInt32 totalNameSize = 0;
  for (i = 0; i < numMembers; i++)
	{
	  if ( !pMemberNames[i])
		return AAFRESULT_NULL_PARAM;
	  if ( !ppMemberTypes[i])
		return AAFRESULT_NULL_PARAM;

	  totalNameSize += (wcslen (pMemberNames[i]) + 1);
	}

  wchar_t * namesBuf = new wchar_t[totalNameSize];
  if (!namesBuf)
	return AAFRESULT_NOMEMORY;
  // make it an empty string
  *namesBuf = 0;
  wchar_t * tmpNamePtr = namesBuf;

  assert (0 == _memberTypes.count());
  aafUID_t * buf = new aafUID_t[numMembers*sizeof(aafUID_t)];
  for (i = 0; i < numMembers; i++)
	{
	  assert (ppMemberTypes[i]);
	  aafUID_t typeUID;
	  AAFRESULT hr = ppMemberTypes[i]->GetAUID(&typeUID);
	  assert (AAFRESULT_SUCCEEDED(hr));
	  buf[i] = typeUID;

	  assert (pMemberNames[i]);
	  wcscpy(tmpNamePtr, pMemberNames[i]);
	  // +1 to go past embedded null
	  tmpNamePtr += wcslen (pMemberNames[i]) + 1;
	}
  _memberTypes.setValue(buf, numMembers*sizeof(aafUID_t));
  delete[] buf;
  _memberNames.setValue (namesBuf, totalNameSize * sizeof(wchar_t));
  delete[] namesBuf;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetMemberType (
      aafUInt32 index,
      ImplAAFTypeDef ** ppTypeDef)
{
  AAFRESULT hr;
  aafUInt32 count;
  ImplAAFTypeDef * pMemberType = NULL;
  aafUID_t memberUID;

  if (!ppTypeDef) return AAFRESULT_NULL_PARAM;
  
  hr = GetCount(&count);
  if (AAFRESULT_FAILED(hr)) return hr;

  if (index >= count) return AAFRESULT_ILLEGAL_VALUE;

  _memberTypes.getValueAt (&memberUID, index);
  hr = GetDict()->LookupType(&memberUID, &pMemberType);
  assert (AAFRESULT_SUCCEEDED(hr));
  assert (pMemberType);
  pMemberType->AcquireReference();
  assert (ppTypeDef);
  *ppTypeDef = pMemberType;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetMemberName (
      aafUInt32 index,
      wchar_t * pName,
      aafUInt32  bufSize)
{
  AAFRESULT hr;
  aafUInt32 count;
  aafUInt32 indexIntoProp;
  aafUInt32 currentIndex;

  if (!pName) return AAFRESULT_NULL_PARAM;
  
  hr = GetCount(&count);
  if (AAFRESULT_FAILED(hr)) return hr;

  if (index >= count) return AAFRESULT_ILLEGAL_VALUE;

  wchar_t c;
  size_t numChars = _memberNames.count();
  indexIntoProp = 0;
  currentIndex = 0;
  if (0 != index)
	{
	  for (size_t i = 0; i < numChars; i++)
		{
		  indexIntoProp++;
		  _memberNames.getValueAt(&c, i);
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
	  _memberNames.getValueAt(&c, indexIntoProp++);
	  // BobT Note!!! We're cheating here, modifying client data
	  // before we're sure this method will succeed.
	  *pName++ = c;
	  bufSize--;
	}
  while (c);
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetMemberNameBufLen (
      aafUInt32  index,
      aafUInt32 * pLen)
{
  AAFRESULT hr;
  aafUInt32 count;
  aafUInt32 indexIntoProp;
  aafUInt32 currentIndex;

  if (!pLen) return AAFRESULT_NULL_PARAM;
  
  hr = GetCount(&count);
  if (AAFRESULT_FAILED(hr)) return hr;

  if (index >= count) return AAFRESULT_ILLEGAL_VALUE;

  wchar_t c;
  size_t numChars = _memberNames.count();
  indexIntoProp = 0;
  currentIndex = 0;
  if (0 != index)
	{
	  for (size_t i = 0; i < numChars; i++)
		{
		  indexIntoProp++;
		  _memberNames.getValueAt(&c, i);
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
	  _memberNames.getValueAt(&c, indexIntoProp++);
	  if (c) nameLength++;
	}
  while (c);

  // increment once more for trailing null
  nameLength++;

  assert (pLen);
  *pLen = nameLength;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::CreateValueFromValues (
      ImplAAFPropertyValue ** pMemberValues,
      aafUInt32  numMembers,
      ImplAAFPropertyValue ** ppPropVal)
{
  aafUInt32 count;
  AAFRESULT hr;

  if (!pMemberValues) return AAFRESULT_NULL_PARAM;
  if (!ppPropVal) return AAFRESULT_NULL_PARAM;

  hr = GetCount (&count);
  if (AAFRESULT_FAILED(hr)) return hr;
  if (numMembers != count) return AAFRESULT_ILLEGAL_VALUE;

  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  ImplAAFPropValData * pvd = NULL;

  aafMemPtr_t pBits = NULL;
  ImplAAFTypeDef * ptd = NULL;
  try
	{
	  // Get total property value size from sum of all member prop vals
	  assert (pMemberValues);
	  aafUInt32 size = 0;
	  aafUInt32 i;
	  for (i = 0; i < count ; i++)
		{
		  hr = pMemberValues[i]->GetType (&ptd);
		  if (AAFRESULT_FAILED(hr)) throw hr;
		  assert (ptd);
		  assert (ptd->IsFixedSize());
		  size += ptd->PropValSize();
		  ptd->ReleaseReference();
		  ptd = NULL;
		}
	  assert (PropValSize() == size);

	  AAFRESULT hr;

	  // create new ueber-prop-val and set it up
	  pvd = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
	  if (!pvd) throw AAFRESULT_NOMEMORY;

	  hr = pvd->Initialize (this);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  hr = pvd->AllocateBits (size, &pBits);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  // copy bits of each subordinate prop val into correct place in
	  // the ueber one
	  size_t curOffset = 0;
	  for (i = 0; i < count; i++)
		{
		  aafUInt32 curSize;
		  hr = pMemberValues[i]->GetType (&ptd);
		  if (AAFRESULT_FAILED(hr)) throw hr;
		  assert (ptd);
		  curSize = ptd->PropValSize();
		  ptd->ReleaseReference ();
		  ptd = NULL;
		  aafMemPtr_t inBits = NULL;
		  assert (dynamic_cast<ImplAAFPropValData*>(pMemberValues[i]));
		  hr = dynamic_cast<ImplAAFPropValData*>(pMemberValues[i])
			->GetBits (&inBits);
		  if (AAFRESULT_FAILED(hr)) throw hr;
		  assert (inBits);
		  memcpy (pBits+curOffset, inBits, curSize);
		  curOffset += curSize;
		  assert (curOffset <= size);
		}
	  assert (ppPropVal);
	  *ppPropVal = pvd;
	  pvd = NULL;
	}
  catch (HRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  RELEASE_IF_SET (pvd);
  RELEASE_IF_SET (ptd);
  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::CreateValueFromStruct (
      aafMemPtr_t * /*pInitData*/,
      aafUInt32 /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetValue (
      ImplAAFPropertyValue * pInPropVal,
      aafUInt32 index,
      ImplAAFPropertyValue ** ppOutPropVal)
{
  aafUInt32 count;
  AAFRESULT hr;

  if (!pInPropVal)   return AAFRESULT_NULL_PARAM;
  if (!ppOutPropVal) return AAFRESULT_NULL_PARAM;

  hr = GetCount (&count);
  if (AAFRESULT_FAILED(hr)) return hr;
  if (index >= count) return AAFRESULT_ILLEGAL_VALUE;

  ImplAAFPropValData * pvdIn = NULL;
  ImplAAFPropValData * pvdOut = NULL;
  ImplAAFTypeDef * ptd = NULL;
  aafUID_t memberUID;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  try
	{
	  aafUInt32 offset = 0;
	  // add up offsets of previous items
	  for (aafUInt32 i = 0; i < index; i++)
		{
		  _memberTypes.getValueAt (&memberUID, i);
		  hr = GetDict()->LookupType (&memberUID, &ptd);
		  assert (AAFRESULT_SUCCEEDED(hr));
		  assert (ptd);
		  ptd->AcquireReference ();
		  offset += ptd->PropValSize();
		  ptd->ReleaseReference ();
		  ptd = NULL;
		}

	  // offset now points into prop storage

	  pvdOut = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
	  if (!pvdOut) throw AAFRESULT_NOMEMORY;

	  _memberTypes.getValueAt (&memberUID, index);
	  hr = GetDict()->LookupType (&memberUID, &ptd);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (ptd);
	  ptd->AcquireReference();

	  hr = pvdOut->Initialize (ptd);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  assert (pInPropVal);
	  pvdIn = dynamic_cast<ImplAAFPropValData*>(pInPropVal);
	  assert (pvdIn);

	  hr = pvdOut->AllocateFromPropVal (pvdIn,
										offset,
										ptd->PropValSize(),
										NULL);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  ptd->ReleaseReference();
	  ptd = NULL;
	  assert (ppOutPropVal);
	  *ppOutPropVal = pvdOut;
	  pvdOut = NULL;  // don't release reference; output param now
					  // owns it
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  // Don't release this!  It is simply a dynamic_cast<>ed pInPropVal
  // RELEASE_IF_SET (pvd);
  RELEASE_IF_SET (pvdOut);
  RELEASE_IF_SET (ptd);
  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetStruct (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t pData,
      aafUInt32 dataSize)
{
  if (! pPropVal)
	return AAFRESULT_NULL_PARAM;
  if (! pData)
	return AAFRESULT_NULL_PARAM;

  // Bobt hack implementation! Not platform-independent!
  aafUInt32 bitsSize = 0;
  ImplAAFPropValData * pvd = 0;
  assert (pPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);
  AAFRESULT hr;
  hr = pvd->GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED(hr))
	return hr;
  if (dataSize < bitsSize)
	return AAFRESULT_ILLEGAL_VALUE;

  aafMemPtr_t pBits;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr))
	return hr;

  // Bobt hack!!! should be registered size, not bitsSize.
  memcpy (pData, pBits, bitsSize);
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::SetValue (
      ImplAAFPropertyValue * pPropVal,
      aafUInt32 index,
      ImplAAFPropertyValue * pMemberPropVal)
{
  aafUInt32 count;
  AAFRESULT hr;

  if (!pPropVal)   return AAFRESULT_NULL_PARAM;
  if (!pMemberPropVal) return AAFRESULT_NULL_PARAM;

  hr = GetCount (&count);
  if (AAFRESULT_FAILED(hr)) return hr;
  if (index <= count) return AAFRESULT_ILLEGAL_VALUE;

  ImplAAFPropValData * pvdIn = NULL;
  ImplAAFPropValData * pvdOut = NULL;
  ImplAAFTypeDef * ptd = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;
  aafUID_t memberUID;
  try
	{
	  aafUInt32 offset = 0;
	  // add up offsets of previous items
	  for (aafUInt32 i = 0; i < index; i++)
		{
		  _memberTypes.getValueAt (&memberUID, i);
		  hr = GetDict()->LookupType (&memberUID, &ptd);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (ptd);
		  ptd->AcquireReference ();
		  assert (ptd);
		  offset += ptd->PropValSize();
		  ptd->ReleaseReference ();
		  ptd = NULL;
		}

	  // offset now points into prop storage

	  _memberTypes.getValueAt (&memberUID, index);
	  hr = GetDict()->LookupType (&memberUID, &ptd);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (ptd);
	  ptd->AcquireReference();

	  assert (pMemberPropVal);
	  pvdIn = dynamic_cast<ImplAAFPropValData*>(pMemberPropVal);
	  assert (pvdIn);

	  assert (pPropVal);
	  pvdOut = dynamic_cast<ImplAAFPropValData*>(pPropVal);
	  assert (pvdOut);

	  assert ((offset+ptd->PropValSize()) <= this->PropValSize());

	  aafMemPtr_t pInBits = NULL;
	  aafMemPtr_t pOutBits = NULL;

	  hr = pvdIn->GetBits (&pInBits);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  hr = pvdOut->GetBits (&pOutBits);
	  if (AAFRESULT_FAILED(hr)) throw hr;

	  memcpy (pOutBits+offset, pInBits, ptd->PropValSize());
	  ptd->ReleaseReference();
	  ptd = NULL;
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  // Don't release this!  It is simply a dynamic_cast<>ed pPropVal
  // RELEASE_IF_SET (pvdIn);
  // RELEASE_IF_SET (pvdOut);
  RELEASE_IF_SET (ptd);
  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::SetStruct (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t pData,
      aafUInt32 dataSize)
{
  if (! pPropVal)
	return AAFRESULT_NULL_PARAM;
  if (! pData)
	return AAFRESULT_NULL_PARAM;

  // Bobt hack implementation! Not platform-independent!
  aafUInt32 bitsSize = 0;
  ImplAAFPropValData * pvd = 0;
  AAFRESULT hr;
  assert (pPropVal);
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);
  hr = pvd->GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED(hr))
	return hr;
  if (dataSize > bitsSize)
	return AAFRESULT_ILLEGAL_VALUE;

  aafMemPtr_t pBits;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr))
	return hr;

  // Bobt hack!!! should be registered size, not bitsSize.
  memcpy (pBits, pData, bitsSize);
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::GetCount (
      aafUInt32 *  pCount)
{
  if (!pCount) return AAFRESULT_NULL_PARAM;
  *pCount = _memberTypes.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRecord::RegisterMembers (
      aafUInt32 * pOffsets,
      aafUInt32 numMembers,
      aafUInt32 structSize)
{
  aafUInt32 count;
  AAFRESULT hr;

  if (! pOffsets) return AAFRESULT_NULL_PARAM;

  hr = GetCount (&count);
  if (AAFRESULT_FAILED(hr)) return hr;

  if (numMembers != count) return AAFRESULT_ILLEGAL_VALUE;

  if (_registeredOffsets) delete[] _registeredOffsets;
  _registeredOffsets = new aafUInt32[numMembers];
  if (! _registeredOffsets) return AAFRESULT_NOMEMORY;

  for (aafUInt32 i = 0; i < numMembers; i++)
	{
	  _registeredOffsets[i] = pOffsets[i];
	}

  _registeredSize = structSize;
  return AAFRESULT_SUCCESS;
}


// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefRecord::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatRecord;
  return AAFRESULT_SUCCESS;
}


aafBool ImplAAFTypeDefRecord::IsFixedSize (void)
{
  return AAFTrue;
}


size_t ImplAAFTypeDefRecord::PropValSize (void)
{
  aafUInt32 count;
  size_t totalSize = 0;
  AAFRESULT hr;

  hr = GetCount (&count);
  if (AAFRESULT_FAILED(hr)) return hr;

  for (aafUInt32 i = 0; i < count; i++)
	{
	  ImplAAFTypeDef * pMemType;
	  pMemType = 0;
	  hr = GetMemberType (i, &pMemType);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (pMemType);
	  assert (pMemType->IsFixedSize());
	  totalSize += pMemType->PropValSize();
	}
  return totalSize;
}


aafBool ImplAAFTypeDefRecord::IsRegistered (void)
{
  return (_registeredOffsets ? AAFTrue : AAFFalse);
}


size_t ImplAAFTypeDefRecord::NativeSize (void)
{
  assert (IsRegistered());
  return _registeredSize;
}


OMDEFINE_STORABLE(ImplAAFTypeDefRecord, AUID_AAFTypeDefRecord);
