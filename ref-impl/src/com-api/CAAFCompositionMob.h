//@doc
//@class    AAFCompositionMob | Implementation class for AAFCompositionMob
#ifndef __CAAFCompositionMob_h__
#define __CAAFCompositionMob_h__

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





#ifndef __CAAFMob_h__
#include "CAAFMob.h"
#endif


class CAAFCompositionMob
  : public IAAFCompositionMob,
    public CAAFMob
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCompositionMob (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFCompositionMob ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this object with the given name.
  // 
  // Succeeds if all of the following are true:
  // - this object has not yet been initialized.
  // - the pName pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pName argument is NULL.
  //
  STDMETHOD (Initialize) (
    // Mob name [optional] 
    /*[in, string]*/ aafCharacter_constptr  pName);


  //***********************************************************
  //
  // GetDefaultFade()
  //
  // Get the default fade for this composition.  If there is no
  // default fade, this function returns with no error, but the VALID
  // field of the structure is false.  This allows you to pass this
  // struct to SourceClip::GetFade() in all cases.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pResult pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult argument is NULL.
  //
  STDMETHOD (GetDefaultFade) (
    // a default fade struct 
    /*[out]*/ aafDefaultFade_t *  pResult);
	

  //***********************************************************
  //
  // SetDefaultFade()
  //
  // Adds the default crossfade properties to the Mob.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_BAD_TYPE
  //   - invalid fadeType.
  //
  // AAFRESULT_BAD_LENGTH
  //   - invalid fadeLength.
  //
  STDMETHOD (SetDefaultFade) (
    // Default fade length
    /*[in]*/ aafLength_t  fadeLength,

    // default fade type
    /*[in]*/ aafFadeType_t  fadeType,

    // default fade edit unit
    /*[in]*/ aafRational_t  fadeEditUnit);

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

#endif // ! __CAAFCompositionMob_h__


