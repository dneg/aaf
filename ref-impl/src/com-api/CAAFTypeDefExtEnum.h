//@doc
//@class    AAFTypeDefExtEnum | Implementation class for AAFTypeDefExtEnum
#ifndef __CAAFTypeDefExtEnum_h__
#define __CAAFTypeDefExtEnum_h__

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


class CAAFTypeDefExtEnum
  : public IAAFTypeDefExtEnum,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefExtEnum (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefExtEnum ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to be identified by the given guid.  No
  // element values are initially specified; they must be supplied
  // later using the AppendElement method.
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pID is a valid pointer.
  // - pTypeName is a valid pointer.
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
  //   - pTypeName arg is NULL.
  //
  // AAFRESULT_DUPLICATE
  //   - duplicate name or value is found.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // friendly name of this type definition
    /*[in]*/ aafCharacter_constptr  pTypeName);

  //***********************************************************
  //
  // CreateValueFromName()
  //
  // Creates a property value which contains an Enum type.  A
  // lookup on the Name is done to find a match in the list of 
  // legal enumerations for this type. If the Name specified is not
  // found to exist then an INVALID_PARAM error code is returned.
  // If it succeeds then a newly-created property value in ppPropVal
  // is returned.
  //
  // Succeeds if all of the following are true:
  // - the Name pointer is valid.
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
  // AAFRESULT_INVALID_PARAM
  //   - the Name specified is not in the list of legal Enumerations.
  //
  STDMETHOD (CreateValueFromName) (
    // the Name of a valid Enum symbol
    /*[in]*/ aafCharacter_constptr  Name,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);
  

  //***********************************************************
  //
  // CountElements()
  //
  // Returns number of enumeration elements contained.
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
  STDMETHOD (CountElements) (
    // count of elements within this enumeration 
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // GetElementValue()
  //
  // Gets the indexed element in this enumerated type.  The
  // value is written into the client-allocated *pOutValue.  Index
  // must be less than the value returned by CountElements().
  //
  // Succeeds if:
  // - pOutValue is a valid pointer.
  // - index is less than CountElements().
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
  //   - pOutValue arg is NULL.
  //
  // AAFRESULT_BADINDEX
  //   - index is not less than CountElements().
  //
  STDMETHOD (GetElementValue) (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // requested value
    /*[out]*/ aafUID_t *  pOutValue);


  //***********************************************************
  //
  // GetElementName()
  //
  // Gets the indexed element in this enumerated type.  The
  // Name is written into the client-allocated *pOutValue. The
  // bufSize argument should be large enough to hold the pOutValue.
  // Index must be less than the value returned by CountElements().
  //
  // Caller may call GetElementNameBufLen() to determine the
  // required buffer size.
  //  
  // Succeeds if:
  // - pOutValue is a valid pointer.
  // - index is less than CountElements().
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
  //   - pOutValue arg is NULL.
  //
  // AAFRESULT_BADINDEX
  //   - index is not less than CountElements().
  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  //
  STDMETHOD (GetElementName) (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // requested value
    /*[out, size_is(bufSize)]*/ aafCharacter *  pOutValue,

    // The size of the pOutValue buffer in bytes
    /*[in]*/ aafUInt32  bufSize);
  
  //***********************************************************
  //
  // GetElementNameBufLen()
  //
  // Returns the length of buffer required for the GetElementName()
  // method, in bytes.  The value is placed into the location
  // specified by pLen. The value will include space required for the
  // trailing null character.
  //
  // Succeeds if the following is true:
  // - the pLen pointer is valid.
  // - index is less than CountElements().
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  // AAFRESULT_BADINDEX
  //   - index is not less than CountElements().
  //
  //
  STDMETHOD (GetElementNameBufLen) (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // GetNameFromValue()
  //
  // Writes the human-legible tag associated with the given value in
  // this enumerated type.  The name is written, with a trailing null
  // character, into the pName buffer.  The buffer is allocated by
  // the caller.  The size of the buffer is given by bufSize.
  // 
  // Caller may call GetNameBufLenFromValue() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // - the pName pointer is valid.
  // - bufSize indicates the buffer is large enough to hold the name.
  // - the integer value is associated with an element of this
  //   enumerated type.
  // 
  // If this method fails nothing will be written to
  // *pName.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pValue or pName arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of
  //     this type.
  //
  STDMETHOD (GetNameFromValue) (
    // value of element to get
    /*[in]*/ IAAFPropertyValue * pValue,

    // buffer into which the element name is written
    /*[out, size_is(bufSize), string]*/ aafCharacter *  pName,

    // The size of the pName buffer, in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLenFromValue()
  //
  // Returns the length of buffer required for the GetNameFromValue()
  // method, in bytes.  The value is placed into the location
  // specified by pLen. The value will include space required for the
  // trailing null character.
  //
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // - the pLen pointer is valid.
  // - the value is associated with an element of this enumerated
  //   type.
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pValue or pLen arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of this
  //     type.
  //
  STDMETHOD (GetNameBufLenFromValue) (
    // value of element to get
    /*[in]*/ IAAFPropertyValue * pValue,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // GetNameFromAUID()
  //
  // Writes the human-legible tag associated with the given value in
  // this enumerated type.  The name is written, with a trailing null
  // character, into the pName buffer.  The buffer is allocated by
  // the caller.  The size of the buffer is given by bufSize.
  // 
  // Caller may call GetNameBufLenFromAUID() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pName pointer is valid.
  // - bufSize indicates the buffer is large enough to hold the name.
  // - the integer value is associated with an element of this
  //   enumerated type.
  // 
  // If this method fails nothing will be written to
  // *pName.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of
  //     this type.
  //
  STDMETHOD (GetNameFromAUID) (
    // value of element to get
    /*[in, ref]*/ aafUID_constref  value,

    // buffer into which the element name is written
    /*[out, size_is(bufSize), string]*/ aafCharacter *  pName,

    // The size of the pName buffer, in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLenFromAUID()
  //
  // Returns the length of buffer required for the GetNameFromInteger()
  // method, in bytes.  The value is placed into the location
  // specified by pLen. The value will include space required for the
  // trailing null character.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
  // - the value is associated with an element of this enumerated
  //   type.
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of this
  //     type.
  //
  STDMETHOD (GetNameBufLenFromAUID) (
    // value of element to get
    /*[in, ref]*/ aafUID_constref  value,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // GetAUIDValue()
  //
  // Gets the value from the given property value and writes it as an
  // AUID into *pValueOut.
  //
  // Succeeds if:
  // - The pPropValIn pointer is valid.
  // - The pValueOut pointer is valid.
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
  //   - Either pPropValIn or ppPropValOut arg is NULL.
  //
  STDMETHOD (GetAUIDValue) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pPropValIn,

    // value of the enum represented by the given input property value
    /*[out]*/ aafUID_t *  pValueOut);


  //***********************************************************
  //
  // SetAUIDValue()
  //
  // Sets the given property to the value given in valueIn.
  //
  // Succeeds if:
  // - The pPropValToSet pointer is valid.
  // - valueIn is a correct value for this enumerated type.
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
  //   - pPropValToSet arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - valueIn is not a correct value for this enumerated type.
  //   - pPropValIn's type doesn't match GetElementType()
  //
  STDMETHOD (SetAUIDValue) (
    // property value to set
    /*[in]*/ IAAFPropertyValue * pPropValToSet,

    // new value of the enum represented by the given property value
    /*[in, ref]*/ aafUID_constref  valueIn);


  //***********************************************************
  //
  // AppendElement()
  //
  // Appends a new element to this extendible enumeration.  The
  // element will have the given name and value.  It is not legal to
  // have elements with duplicate names or values.
  // 
  // Succeeds if:
  // - the pName pointer is valid.
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
  //   - pName arg is NULL.
  //
  // AAFRESULT_DUPLICATE
  //   - duplicate name or value is found.
  //
  STDMETHOD (AppendElement) (
    // value of appended element
    /*[in, ref]*/ aafUID_constref  value,

    // name of appended element
    /*[in]*/ aafCharacter_constptr  pName);




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

#endif // ! __CAAFTypeDefExtEnum_h__


