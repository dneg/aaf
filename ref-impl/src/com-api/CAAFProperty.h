//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __CAAFProperty_h__
#define __CAAFProperty_h__

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








#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFProperty
  : public IAAFProperty,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFProperty (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFProperty ();

public:


  //***********************************************************
  //
  // GetDefinition()
  //
  // Returns the definition of this property.
  //
  // Succeeds if:
  // - This object has already been Initialize()d.
  // - The ppPropDef pointer is valid.
  // - The associated property definition can be found in the
  //   dictionary.
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
  //   - ppPropDef arg is NULL.
  //
  // AAFRESULT_BAD_PROP
  //   - The definition for this property could not be found in the
  //     dictionary.
  //
  STDMETHOD (GetDefinition) (
    // This property's definition 
    /*[out]*/ IAAFPropertyDef ** ppPropDef);


  //***********************************************************
  //
  // GetValue()
  //
  // Returns the Property Value object associated with this property.
  //
  // Succeeds if:
  // - This object has already been Initialize()d.
  // - The ppPval pointer is valid.
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
  //   - ppValue arg is NULL.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - The given property is optional and not present.
  //
  STDMETHOD (GetValue) (
    // The Property Value object associated with this property. 
    /*[out]*/ IAAFPropertyValue ** ppValue);

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

#endif // ! __CAAFProperty_h__


