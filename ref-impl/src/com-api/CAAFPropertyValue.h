//@doc
//@class    AAFPropertyValue | Implementation class for AAFPropertyValue
#ifndef __CAAFPropertyValue_h__
#define __CAAFPropertyValue_h__

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


class CAAFPropertyValue
  : public IAAFPropertyValue,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPropertyValue (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPropertyValue ();

public:


  //***********************************************************
  //
  // GetType()
  //
  // Returns the type definition associated with this property value.
  // If this Property Value is not of a recognized type (such as from
  // a damaged or incorrectly construct file) this method will return
  // a raw access type which can be used to access the property data
  // in a raw manner.
  //
  // Succeeds if:
  // - The ppTypeDef pointer is valid.
  // - The associated type definition can be found in the dictionary.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef arg is NULL.
  //
  STDMETHOD (GetType) (
    // The type definition associated with this property value 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // IsDefinedType()
  //
  // Returns false if this property value's type is not (necessarily)
  // the one which was defined for it.  That may be the case if this
  // property value was read from a damaged file where type
  // information was not available; in that case GetType() will return
  // the raw access type for this value.
  //
  // If this property value's type is the one which was defined for
  // it, this method will return true and GetType() will return that
  // defined type for this value.
  //
  // Succeeds if:
  // - The pIsDefined pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsDefined arg is NULL.
  //
  STDMETHOD (IsDefinedType) (
    // result 
    /*[out]*/ aafBoolean_t *  pIsDefined);

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

#endif // ! __CAAFPropertyValue_h__


