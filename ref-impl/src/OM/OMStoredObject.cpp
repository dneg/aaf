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
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStoredObject
#include "OMStoredObject.h"

#include "OMUtilities.h"

  // @mfunc Destructor.
OMStoredObject::~OMStoredObject(void)
{
  TRACE("OMStoredObject::~OMStoredObject");
}

wchar_t* OMStoredObject::streamName(const wchar_t* propertyName,
                                    OMPropertyId pid)
{
  return referenceName(propertyName, pid);
}

wchar_t* OMStoredObject::referenceName(const wchar_t* propertyName,
                                       OMPropertyId pid)
{
  TRACE("OMStoredObject::referenceName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32);

  return result;
}

wchar_t* OMStoredObject::collectionName(const wchar_t* propertyName,
                                        OMPropertyId pid)
{
  TRACE("OMStoredObject::collectionName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32 - 10);

  return result;
}

wchar_t* OMStoredObject::elementName(const wchar_t* propertyName,
                                     OMPropertyId pid,
                                     OMUInt32 localKey)
{
  TRACE("OMStoredObject::elementName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32 - 10);

  size_t newSize = lengthOfWideString(result);
  concatenateWideString(result, L"{", 1);
  size_t keySize;
  if (localKey != 0) {
    keySize = stringSize(localKey);
    toWideString(localKey, &result[newSize + 1], keySize);
  } else {
    keySize = 1;
    concatenateWideString(result, L"0", 1);
  }
  concatenateWideString(result, L"}", 1);

  return result;
}

void OMStoredObject::mapCharacters(wchar_t* /* name */,
                                   size_t /* nameLength */)
{
  TRACE("OMStoredObject::mapCharacters");

#if 0
  for (size_t i = 0; i < nameLength; i++) {
    name[i] = map[name[i] & 0x7f];
  }
#endif
}

void OMStoredObject::mangleName(const wchar_t* clearName,
                                OMPropertyId pid,
                                wchar_t* mangledName,
                                size_t mangledNameSize)
{
  TRACE("OMStoredObject::mangleName");

  PRECONDITION("Valid pid", pid > 0);

  // Squeeze name to accommodate the pid as "-<pid>"
  //
  size_t maxSize = mangledNameSize - stringSize(pid) - 1 - 1;
  size_t newSize = squeezeWideString(clearName,
                                     lengthOfWideString(clearName),
                                     mangledName,
                                     maxSize);
  ASSERT("Consistent length", newSize == lengthOfWideString(mangledName));
  ASSERT("Consistent length",
                            lengthOfWideString(mangledName) < mangledNameSize);

  // Map out any illegal characters
  //
  mapCharacters(mangledName, newSize);

  // Append "-<pid>"
  //
  mangledName[newSize] = L'-';
  toWideString(pid, &mangledName[newSize+1], stringSize(pid));
}
