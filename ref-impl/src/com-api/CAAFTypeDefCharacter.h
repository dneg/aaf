//@doc
//@class    AAFTypeDefCharacter | Implementation class for AAFTypeDefCharacter
#ifndef __CAAFTypeDefCharacter_h__
#define __CAAFTypeDefCharacter_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFTypeDef_h__
#include "CAAFTypeDef.h"
#endif


class CAAFTypeDefCharacter
  : public IAAFTypeDefCharacter,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefCharacter (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefCharacter ();

public:



  //***********************************************************
  //
  // CreateValueFromCharacter()
  //
  // Gets a property value corresponding to the character data. 
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pCharacterValue pointer is valid.
  // - The ppActualPropertyValue pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppCharacterValue is not a valid pointer.
  //
  //
  STDMETHOD (CreateValueFromCharacter) (
    // character data to write the the new value
    /*[in]*/ aafCharacter  character,

    // character property value with the given character
    /*[out]*/ IAAFPropertyValue ** ppCharacterValue);

  //***********************************************************
  //
  // GetCharacter()
  //
  // Gets a character from the property value. 
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pCharacterValue pointer is valid.
  // - The ppActualPropertyValue pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pCharacterValue is not a valid character property value.
  //
  //
  STDMETHOD (GetCharacter) (
    // character property value to read
    /*[in]*/ IAAFPropertyValue * pCharacterValue,

    // character that is read
    /*[out]*/ aafCharacter *  pCharacter);

  //***********************************************************
  //
  // SetCharacter()
  //
  // Sets a character into the given property value. 
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pCharacterValue pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pCharacterValue is not a valid character property value.
  //
  //
  STDMETHOD (SetCharacter) (
    // character property value to modify
    /*[in]*/ IAAFPropertyValue * pCharacterValue,

    // character that is written
    /*[in]*/ aafCharacter  character);




protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFTypeDefCharacter_h__


