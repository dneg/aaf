//@doc
//@class    AAFTypeDefVariableArray | Implementation class for AAFTypeDefVariableArray
#ifndef __CAAFTypeDefVariableArray_h__
#define __CAAFTypeDefVariableArray_h__

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


class CAAFTypeDefVariableArray
  : public IAAFTypeDefVariableArray,
    public IAAFTypeDefVariableArrayEx,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefVariableArray (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefVariableArray ();

public:

  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to contain elements of the given type.
  // Note that it is only possible to use certain types as the element
  // type.  Those permissible types include:
  // - AAFTypeDefInt
  // - AAFTypeDefRecord
  // - AAFTypeDefEnum
  // - AAFTypeDefExtEnum
  // - AAFTypeDefFixedArray
  // - AAFTypeDefObjectRef
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pID is a valid pointer.
  // - pTypeDef is a valid pointer.
  // - pTypeName is a valid pointer.
  // - element type is permissible for use in a VariableArray.
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
  // Returns number of array elements in the referenced property
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

    // count of elements in the specified array property value
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // AppendElement()
  //
  // Appends an element to the end of the array, setting it to the
  // value given in pMemberPropVal.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pInPropVal pointer is valid.
  // - pMemberPropVal pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pInPropVal or pMemberPropVal arg is NULL.
  //
  STDMETHOD (AppendElement) (
    // property value corresponding to array to which element is appended
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // value to be appended to this array
    /*[in]*/ IAAFPropertyValue * pMemberPropVal);


  //***********************************************************
  //
  // CreateEmptyValue()
  //
  // Creates an empty property value which contains a variable array
  // type that initially is empty (that is, contains no elements
  // yet).
  //
  // Succeeds if all of the following are true:
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
  //   - ppPropVal arg is NULL.
  //
  STDMETHOD (CreateEmptyValue) (
    // newly-created empty property value 
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // CreateValueFromValues()
  //
  // Creates a property value which contains a variable array type.
  // The array elements in the property value are initialized to
  // contain the given values, passed in the pElementValues array.
  // numElements, which indicates the size of the pElementValues
  // array, determines the size of the array in the new array
  // property value.
  //
  // Succeeds if all of the following are true:
  // - the pElementValues pointer is valid.
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
  //   - either pElementValues or ppPropVal arg is NULL.
  //
  STDMETHOD (CreateValueFromValues) (
    // array of property values for elements of array value which
    // is to be created.
    /*[in, size_is(numElements)]*/ IAAFPropertyValue ** pElementValues,

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
  // property value in ppPropVal.  The size of the newly-created array
  // property value will be determined by the number of elements in
  // the initialization C array, as communicated by initDataSize.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppPropVal pointer is valid.
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
  // type.  If dataSize indicates an array size different from the
  // size currently in the indicated array property value, that array
  // property value will be resized.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
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

  //***********************************************************
  // METHOD NAME: PrependElement()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTypeDefVariableArrayEx | PrependElement |
  // Prepends an element to the end of the array, setting it to the
  // value given in pMemberPropVal.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pInPropVal pointer is valid.
  // - pMemberPropVal pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pInPropVal or pMemberPropVal arg is NULL.
  // @end
  // 
  STDMETHOD (PrependElement)
   (
    // @parm [in] AAFPropertyValue | pInPropVal | property value corresponding to array to which element is prepended
    IAAFPropertyValue * pInPropVal,

    // @parm [in] AAFPropertyValue | pMemberPropVal | value to be prepended to this array
    IAAFPropertyValue * pMemberPropVal
  );

  //***********************************************************
  // METHOD NAME: RemoveElement()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTypeDefVariableArrayEx | RemoveElement |
  // Remove an element from the Array, given an index.
  // Index is zero-based,
  // and must be less than the value returned by GetCount().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this array type def.
  // - The pInPropVal pointer is valid.
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
  // AAFRESULT_BADINDEX
  //   - The given index is out of range for this array type def.
  // @end
  // 
  STDMETHOD (RemoveElement)
   (
    // @parm [in] AAFPropertyValue | pInPropVal | property value corresponding to array
    IAAFPropertyValue * pInPropVal,

    // @parm [in] aafUInt32 | index | zero-based index into elements in this array type
    aafUInt32  index
  );

  //***********************************************************
  // METHOD NAME: InsertElement()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTypeDefVariableArrayEx | InsertElement |
  // Inserts the value of the single, indicated element of the fixed
  // array contained in pInPropVal, to the value contained in
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
  // AAFRESULT_BADINDEX
  //   - The given index is out of range for this array type def.
  // @end
  // 
  STDMETHOD (InsertElement)
   (
    // @parm [in] AAFPropertyValue | pInPropVal | property value corresponding to array
    IAAFPropertyValue * pInPropVal,

    // @parm [in] aafUInt32 | index | zero-based index into elements in this array type
    aafUInt32  index,

    // @parm [in] AAFPropertyValue | pMemberPropVal | value to be inserted into this array
    IAAFPropertyValue * pMemberPropVal
  );



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

#endif // ! __CAAFTypeDefVariableArray_h__

