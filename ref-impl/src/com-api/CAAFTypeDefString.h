//@doc
//@class    AAFTypeDefString | Implementation class for AAFTypeDefString
#ifndef __CAAFTypeDefString_h__
#define __CAAFTypeDefString_h__

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


class CAAFTypeDefString
  : public IAAFTypeDefString,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefString (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefString ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to contain elements of the given type.
  // Note that it is only possible to use certain types as the element
  // type.  Those permissible types include:
  //
  // - AAFTypeDefInt
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pID is a valid pointer.
  // - pTypeDef is a valid pointer.
  // - pTypeName is a valid pointer.
  // - element type is permissible for use in a String.
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
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // type of each element to be contained in this array
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // friendly name of this type definition
    /*[in]*/ aafCharacter_constptr  pTypeName);


  //***********************************************************
  //
  // GetType()
  //
  // Returns the type of elements in this string.
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
  // Returns number of elements contained in the referenced property
  // value.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pPropVal is a valid pointer.
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
  //   - Either pPropVal or pCount arg is NULL.
  //
  STDMETHOD (GetCount) (
    // property value of array
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // count of elements in the specified string property value
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // CreateValueFromCString()
  //
  // Creates a property value which contains a string type.  The
  // string in the property value are initialized from data in a C
  // string which is pointed to by pInitData.  Returns the
  // newly-created property value in ppPropVal.  The size of the
  // newly-created string property value will be determined by the
  // size of the initialization C array, as communicated by
  // initDataSize.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppPropVal pointer is valid.
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
  STDMETHOD (CreateValueFromCString) (
    // pointer to compile-time C string containing data to use
    /*[in, size_is(initDataSize)]*/ aafMemPtr_t  pInitData,

    // size of data in pInitData, in bytes
    /*[in]*/ aafUInt32  initDataSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // SetCString()
  //
  // Copies all the string data contained in the C array pointed to by
  // pData into the given property value, interpreting the data as a
  // string of this type.  If dataSize indicates an array size
  // different from the size currently in the indicated array property
  // value, that array property value will be resized.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
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
  STDMETHOD (SetCString) (
    // property value to write
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer from which C string data should be read
    /*[in, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);


  //***********************************************************
  //
  // AppendElements()
  //
  // Appends elements to the end of the array, setting them to the
  // values given in the pElements array.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pInPropVal pointer is valid.
  // - pElements pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pInPropVal or pElements arg is NULL.
  //
  STDMETHOD (AppendElements) (
    // property value corresponding to string to which elements are to
    // be appended
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // Null-terminated array of elements to be appended
    /*[in]*/ aafMemPtr_t  pElements);


  //***********************************************************
  //
  // GetElements()
  //
  // Gets the value of this property as a string and places it into
  // pBuffer.  bufferSize indicates the size of the buffer, in bytes.
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pInPropVal pointer is valid.
  // - bufferSize indicates that pBuffer is large enough to hold the
  //   data.
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
  //   - pInPropVal arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - bufferSize indicates that pBuffer is too small to hold the
  //     data.
  //
  STDMETHOD (GetElements) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // array of values that are read
    /*[out]*/ aafMemPtr_t  pBuffer,

    // size of pBuffer, in bytes
    /*[in]*/ aafUInt32  bufferSize);




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

#endif // ! __CAAFTypeDefString_h__


