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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMCharacterStringProperty
#ifndef OMCHARACTERSTRINGPROPERTYT_H
#define OMCHARACTERSTRINGPROPERTYT_H

#include "OMAssertions.h"

// class OMCharacterStringProperty<CharacterType>

template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMVariableSizeProperty<CharacterType>(propertyId, name)
{
  TRACE("OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty");
}

template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::~OMCharacterStringProperty(void)
{
  TRACE(
       "OMCharacterStringProperty<CharacterType>::~OMCharacterStringProperty");
}

  // @mfunc Type conversion. Convert an
  //        <c OMCharacterStringProperty> into a
  //        string of <p CharacterType> characters.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p CharacterType>.
template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::operator const CharacterType*() const
{
  return reinterpret_cast<const CharacterType*>(_bits);
}

  // @mfunc Assign the string <p characterString> to this
  //        <c OMCharacterStringProperty>.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @parm The string of <p CharacterType> characters to assign.
template <typename CharacterType>
void OMCharacterStringProperty<CharacterType>::assign(
                                          const CharacterType* characterString)
{
  const CharacterType* source;
  const CharacterType empty = 0;

  if (characterString != 0) {
    source = characterString;
  } else {
    source = &empty;
  }
  setValue(source, (stringLength(source) + 1) * sizeof(CharacterType));
}

  // @mfunc The length of this <c OMCharacterStringProperty> in
  //        characters (not counting the null terminating character).
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @rdesc The length of this <c OMCharacterStringProperty>.
  //   @this const
template <typename CharacterType>
size_t OMCharacterStringProperty<CharacterType>::length(void) const
{
  return (size() - sizeof(CharacterType)) / sizeof(CharacterType);
}

  // @mfunc Utility function for computing the length, in characters,
  //        of the string of <p CharacterType> characters
  //        <p characterString>.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @parm  A  string of <p CharacterType> characters.
  //   @rdesc The length of the the string of <p CharacterType>
  //          characters <p characterString>.
template <typename CharacterType>
size_t OMCharacterStringProperty<CharacterType>::stringLength(
                                          const CharacterType* characterString)
{
  const CharacterType* p = characterString;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

#endif
