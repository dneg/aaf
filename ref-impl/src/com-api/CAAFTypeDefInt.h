//@doc
//@class    AAFTypeDefInt | Implementation class for AAFTypeDefInt
#ifndef __CAAFTypeDefInt_h__
#define __CAAFTypeDefInt_h__

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


class CAAFTypeDefInt
  : public IAAFTypeDefInt,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefInt (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefInt ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to be identified by the given guid, to
  // have the given size in bytes, and to be signed or unsigned.
  //
  // The implementation of this method may only allow certain values
  // for intSize.  It *will* allow at least 1, 2, 4, and 8-byte
  // integers; some implementations may allow more than that.
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - intSize is a valid value.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - This object has already had Initialize() called on it.
  //
  // AAFRESULT_BAD_SIZE
  //  - intSize is not a valid value.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // the size of this integer type in bytes
    /*[in]*/ aafUInt8  intSize,

    // true if this integer type is signed; false for unsigned
    /*[in]*/ aafBoolean_t  isSigned,

    // friendly name of this type definition
    /*[in, string]*/ aafCharacter_constptr  pTypeName);


  //***********************************************************
  //
  // CreateValue()
  //
  // Creates a property value which contains an integer.  Initializes
  // it to contain the given integer value.  The initialization value
  // is passed through pVal; the size of the initialzation value is
  // given in valSize.
  //
  // valSize may be smaller than GetSize() for this typedef; if so,
  // the value is lsb-justified and sign-extended (for signed) or
  // zero-filled (for unsigned).
  //
  // The implementation of this method may only allow certain values
  // for valSize.  It *will* allow at least 1, 2, 4, and 8-byte
  // integers; some implementations may allow more than that.
  //
  // Succeeds if:
  // - The pVal pointer is valid.
  // - The ppPropVal pointer is valid.
  // - valSize is no larger than GetSize() for this typedef.
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
  //   - either pVal or ppPropVal arg is NULL.
  //
  // AAFRESULT_BAD_SIZE
  //   - valSize is larger than GetSize() for this typedef, or
  //     valSize is not a supported value.
  //
  STDMETHOD (CreateValue) (
    // pointer to integer initialization value
    /*[in, size_is(valSize)]*/ aafMemPtr_t  pVal,

    // size of integer, in bytes, in pVal
    /*[in]*/ aafUInt32  valSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // GetInteger()
  //
  // Returns the integer value of this property.  The value
  // to set is passed through pVal.  The size of the pVal buffer is
  // given in valSize.  valSize may be larger than GetSize(); if so,
  // the value is lsb-justified and sign-extended (for signed) or
  // zero-filled (for unsigned).
  //
  // valSize may be smaller than GetSize() for this typedef; if so,
  // the value is lsb-justified and sign-extended (for signed) or
  // zero-filled (for unsigned).
  //
  // The implementation of this method may only allow certain values
  // for valSize.  It *will* allow at least 1, 2, 4, and 8-byte
  // integers; some implementations may allow more than that.
  //
  // Succeeds if:
  // - The pPropVal pointer is valid.
  // - The pVal pointer is valid.
  // - valSize indicates that pVal is large enough to hold the value.
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
  //   - either pPropVal or pVal arg is NULL.
  //
  // AAFRESULT_BAD_SIZE
  //   - valSize is smaller than GetSize() for this typedef, or
  //     valSize is not a supported value.
  //
  // AAFRESULT_BAD_TYPE
  //   - The type associated with pPropVal cannot be read as an
  //     integral type, or the int size of pPropVal is larger than the
  //     int size of this type.
  //
  STDMETHOD (GetInteger) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer into which value is written
    /*[out, size_is(valSize)]*/ aafMemPtr_t  pVal,

    // size of pVal buffer in bytes
    /*[in]*/ aafUInt32  valSize);


  //***********************************************************
  //
  // SetInteger()
  //
  // Sets this property value to the given integer value.  The value
  // to set is passed in pVal, and the size of the value in pVal is
  // given in valSize.  valSize may be smaller than GetSize(); if so,
  // the value is lsb-justified and sign-extended (for signed) or
  // zero-filled (for unsigned).
  //
  // Succeeds if:
  // - The pPropVal pointer is valid.
  // - The pVal pointer is valid.
  // - valSize indicates that pVal is large enough to hold the value.
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
  //   - either pPropVal or pVal arg is NULL.
  //
  // AAFRESULT_BAD_SIZE
  //   - valSize is larger than GetSize() for this typedef.
  //
  STDMETHOD (SetInteger) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer from which value is read
    /*[in, size_is(valSize)]*/ aafMemPtr_t  pVal,

    // size of pVal buffer in bytes
    /*[in]*/ aafUInt32  valSize);


  //***********************************************************
  //
  // GetSize()
  //
  // Returns the size of the integral value defined by this type
  // definition. 
  //
  // Succeeds if:
  // - The pSize pointer is valid.
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
  //   - pSize arg is NULL.
  //
  STDMETHOD (GetSize) (
    // the returned size of this integral value, in bytes 
    /*[out]*/ aafUInt32 *  pSize);


  //***********************************************************
  //
  // IsSigned()
  //
  // Puts true in *pSigned if property values defined with this type
  // def are signed; puts false there for unsigned property values.
  //
  // Succeeds if:
  // - The pSigned pointer is valid.
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
  //   - pSigned arg is NULL.
  //
  STDMETHOD (IsSigned) (
    // set to true if this integer type is signed; set to false for
    // unsigned 
    /*[out]*/ aafBoolean_t *  pSigned);




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

#endif // ! __CAAFTypeDefInt_h__


