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
#ifndef OMCHARACTERSTRINGPROPERTY_H
#define OMCHARACTERSTRINGPROPERTY_H

#include "OMVariableSizeProperty.h"

  // @class Abstract base class for persistent character string
  //        properties supported by the Object Manager.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @base public | <c OMVariableSizeProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename CharacterType>
class OMCharacterStringProperty :
                                 public OMVariableSizeProperty<CharacterType> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCharacterStringProperty(const OMPropertyId propertyId,
                            const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMCharacterStringProperty(void);

    // @cmember Type conversion. Convert an
    //          <c OMCharacterStringProperty> into a
    //          string of <p CharacterType> characters.
  operator const CharacterType* () const;

    // @cmember Assign the string <p characterString> to this
    //          <c OMCharacterStringProperty>.
  void assign(const CharacterType* characterString);

    // @cmember The length of this <c OMCharacterStringProperty> in
    //          characters (not counting the null terminating character).
  size_t length(void) const;

    // @cmember Utility function for computing the length, in
    //          characters, of the string of <p CharacterType>
    //          characters <p characterString>.
  static size_t stringLength(const CharacterType* characterString);

private:
  // hide, declare but don't define
  operator CharacterType* (void);
};

#include "OMCharacterStringPropertyT.h"

#endif
