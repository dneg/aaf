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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStoredObject
#include "OMStoredObject.h"

#include "OMUtilities.h"
#include "OMDataStream.h"

#include <stdio.h>

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
