//@doc
//@class    AAFTypeDefFixedArray | Implementation class for AAFTypeDefFixedArray
#ifndef __CAAFTypeDefFixedArray_h__
#define __CAAFTypeDefFixedArray_h__

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


class CAAFTypeDefFixedArray
  : public IAAFTypeDefFixedArray,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefFixedArray (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefFixedArray ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to contain elements of the given type,
  // and have the given fixed size.  Note that it is only possible to
  // use certain types as the element type.  Those permissible types
  // include:
  // - AAFTypeDefInt
  // - AAFTypeDefRecord
  // - AAFTypeDefEnum
  // - AAFTypeDefExtEnum
  // - AAFTypeDefFixedArray
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pID is a valid pointer.
  // - pTypeDef is a valid pointer.
  // - pTypeName is a valid pointer.
  // - element type is permissible for use in a FixedArray.
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
  // AAFRESULT_NULL_PARAM
  //   - either pTypeDef or pTypeName arg is NULL.
  //
  // AAFRESULT_BAD_TYPE
  //   - an illegal element type was given.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // type of each element to be contained in this array
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // number of elements to be in this array
    /*[in]*/ aafUInt32  nElements,

    // friendly name of this type definition
    /*[in]*/ aafCharacter_constptr  pTypeName);


  //***********************************************************
  //
  // GetType()
  //
  // Returns the type of elements in this array.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - ppTypeDef is a valid pointer.
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
  //   - ppTypeDef arg is NULL.
  //
  STDMETHOD (GetType) (
    // type of elements in this array 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetCount()
  //
  // Returns number of elements in this array.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pCount is a valid pointer.
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
  //   - pCount arg is NULL.
  //
  STDMETHOD (GetCount) (
    // count of elements in this array 
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // CreateValueFromValues()
  //
  // Creates a property value which contains a fixed array type.  The
  // array elements in the property value are initialized to contain
  // the given values, passed in the pElementValues array.
  // numElements, which indicates the size of the pElementValues
  // array, must match the value returned by GetCount().  Returns
  // the newly-created property value in ppPropVal.
  //
  // Succeeds if all of the following are true:
  // - the pElementValues pointer is valid.
  // - the ppPropVal pointer is valid.
  // - numElements matches the number of elements in this array
  //
  // If this method fails nothing will be written to *ppPropVal.
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
  //   - either pElementValues or ppPropVal arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - numElements does not match GetCount().
  //
  STDMETHOD (CreateValueFromValues) (
    // array of property values for elements of array value which
    // is to be created.
    /*[in, size_is(numElements)]*/ IAAFPropertyValue ** ppElementValues,

    // size of pElementValues array.
    /*[in]*/ aafUInt32  numElements,

    // newly-created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // CreateValueFromCArray()
  //
  // Creates a property value which contains a fixed array type.  The
  // array elements in the property value are initialized from data in
  // a C array which is pointed to by pInitData.  Requires that any
  // structures declared within this array typedef have had their
  // offsets registered with that type.  Returns the newly-created
  // property value in ppPropVal.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppPropVal pointer is valid.
  // - initDataSize indicates pInitData is the correct size.
  // - compile-time struct has had its member offests registered.
  //
  // If this method fails nothing will be written to *ppPropVal.
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
  //   - either pInitData or ppPropVal arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - any contained struct offsets have not yet been registered for
  //    that typedef.
  //
  STDMETHOD (CreateValueFromCArray) (
    // pointer to compile-time C array containing data to use
    /*[in, size_is(initDataSize)]*/ aafMemPtr_t  pInitData,

    // size of data in pInitData, in bytes
    /*[in]*/ aafUInt32  initDataSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // GetElementValue()
  //
  // Gets a single property value corresponding to the indexed array
  // element.  Places a property value representing the array element
  // identified by the index into ppOutPropval.  Index is zero-based,
  // and must be less than the value returned by GetCount().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this array type def.
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
  //   - Either pInPropVal or ppOutPropVal arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - The given index is out of range for this array type def.
  //
  STDMETHOD (GetElementValue) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // zero-based index into elements in this array type
    /*[in]*/ aafUInt32  index,

    // value that is read
    /*[out]*/ IAAFPropertyValue ** ppOutPropVal);


  //***********************************************************
  //
  // GetCArray()
  //
  // Copies all the array data contained in the given property value,
  // interpreted as a fixed array of this type, into the C array
  // pointed to by pData.  Requires that any structures declared
  // within this array typedef have had their offsets registered with
  // that type.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
  // - dataSize indicates pData is large enough to hold the data.
  // - compile-time struct has had its member offests registered.
  //
  // If this method fails nothing will be written to *ppPropVal.
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
  //   - either pPropVal or pData arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - dataSize indicates pData is too small.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (GetCArray) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer into which C array data should be written
    /*[out, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);


  //***********************************************************
  //
  // SetElementValue()
  //
  // Sets the value of the single, indicated element of the fixed
  // array contained in pPropVal, to the value contained in
  // pMemberPropVal.  Index is zero-based, and must be less than the
  // value returned by GetCount().  Property value must be of the
  // same type as returned by GetType().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this array type def.
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
  //   - either pInPropVal or ppOutPropVal arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - The given index is out of range for this array type def.
  //
  STDMETHOD (SetElementValue) (
    // property value to write
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // zero-based index into members in this array type
    /*[in]*/ aafUInt32  index,

    // value to be placed into this array
    /*[in]*/ IAAFPropertyValue * pMemberPropVal);


  //***********************************************************
  //
  // SetCArray()
  //
  // Copies all the array data contained in the C array pointed to by
  // pData into the given property value, interpreting the data as a
  // fixed array of this type.  Requires that any structures declared
  // within this typedef have had their offsets registered with that
  // type.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
  // - dataSize indicates pData contains the correct amount of data.
  // - any contained compile-time struct has had its member offests
  //   registered.
  //
  // If this method fails nothing will be written to *ppPropVal.
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
  //   - either pPropVal or pData arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //  - dataSize indicates pData is not the correct size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - offsets of any contained struct have not yet been registered.
  //
  STDMETHOD (SetCArray) (
    // property value to write
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer from which C array data should be read
    /*[in, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);




  //***********************************************************
  //
  // GetElements()
  //
  // Returns an enumerator across elements in this set.
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The PSetPropVal pointer is valid.
  // - The ppEnum pointer is valid.
  //
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
  //   - Either PSetPropVal or ppEnum arg is NULL.
  //
  STDMETHOD (GetElements) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * PSetPropVal,

    // enumerator across property values
    /*[out]*/ IEnumAAFPropertyValues ** ppEnum);


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

#endif // ! __CAAFTypeDefFixedArray_h__


