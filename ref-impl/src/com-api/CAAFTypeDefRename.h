//@doc
//@class    AAFTypeDefRename | Implementation class for AAFTypeDefRename
#ifndef __CAAFTypeDefRename_h__
#define __CAAFTypeDefRename_h__

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


class CAAFTypeDefRename
  : public IAAFTypeDefRename,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefRename (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefRename ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to be an alias for the given type
  // (similar to a C typedef).
  // 
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pBaseType is a valid pointer.
  // - pTypeName is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  // 
  // AAFRESULT_NULL_PARAM
  //   - pBaseType or pTypeName is null.
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // type to which this is an alias
    /*[in]*/ IAAFTypeDef * pBaseType,

    // friendly name of this type definition
    /*[in, string]*/ aafCharacter_constptr  pTypeName);


  //***********************************************************
  //
  // GetBaseType()
  //
  // Returns the type definition to which this type def is an alias.
  //
  // Succeeds if:
  // - this object has been Initialize()d.
  // - The ppBaseType pointer is valid.
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
  //   - ppBaseType pointer is NULL.
  //
  STDMETHOD (GetBaseType) (
    // type definition for which this is an alias 
    /*[out]*/ IAAFTypeDef ** ppBaseType);


  //***********************************************************
  //
  // GetBaseValue()
  //
  // // Gets the property value of the base type from the given property
  // value of the typedef type and places a pointer to the base type's
  // property value into *ppOutPropVal.
  //
  // Succeeds if:
  // - The pInPropVal pointer is valid.
  // - The ppOutPropVal pointer is valid.
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
  //   - pInPropVal or ppOutPropVal is NULL.
  //
  STDMETHOD (GetBaseValue) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // pointer to property value represented by base type
    /*[out]*/ IAAFPropertyValue ** ppOutPropVal);

  //***********************************************************
  //
  // CreateValue()
  //
  // // Gets the property value of the typedef type from the given property
  // value of the base type and places a pointer to the base type's
  // property value into *ppOutPropVal.
  //
  // Succeeds if:
  // - The pInPropVal pointer is valid.
  // - The ppOutPropVal pointer is valid.
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
  //   - pInPropVal or ppOutPropVal is NULL.
  //
  STDMETHOD (CreateValue) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // pointer to property value represented by rename type
    /*[out]*/ IAAFPropertyValue ** ppOutPropVal);



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

#endif // ! __CAAFTypeDefRename_h__


