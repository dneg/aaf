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
#ifndef OMCHARACTERSTRINGPROPERTYT_H
#define OMCHARACTERSTRINGPROPERTYT_H

#include "OMAssertions.h"

// class OMCharacterStringProperty<CharacterType>

template<typename CharacterType>
OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMVariableSizeProperty<CharacterType>(propertyId, name)
{
  TRACE("OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty");
}

template<typename CharacterType>
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
template<typename CharacterType>
OMCharacterStringProperty<CharacterType>::operator const CharacterType* ()
{
  return reinterpret_cast<const CharacterType*>(_bits);
}

  // @mfunc Assign the string <p characterString> to this
  //        <c OMCharacterStringProperty>.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @parm The string of <p CharacterType> characters to assign.
template<typename CharacterType>
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
template<typename CharacterType>
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
template<typename CharacterType>
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
