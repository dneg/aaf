//@doc
//@class    AAFTaggedValue | Implementation class for AAFTaggedValue
#ifndef __CAAFTaggedValue_h__
#define __CAAFTaggedValue_h__

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






#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFTaggedValue
  : public IAAFTaggedValue,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTaggedValue (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTaggedValue ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a new tagged value object to be identified with the
  // given the given type, and with the given human-legible name.
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pName is a valid pointer.
  // - pTypeDef is a valid pointer and registered
  // - pValue is a valid pointer
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
  //   - pName or pTypeDef or pValue arg is NULL.
  // 
  //
  STDMETHOD (Initialize) (
    // User defined name (tag) of this tagged value object
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Type Definition of the following value data
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // buffer containing value
    /*[in, size_is(valueSize)]*/ aafDataBuffer_t  pValue);


  //***********************************************************
  //
  // GetName()
  //
  // Gets the Name string property.
  // 
  // Writes the Name property, with a trailing null
  // character, into the pName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Name property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pName.
  // 
  // Succeeds if:
  // - The pName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Name.
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
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Name.
  //
  STDMETHOD (GetName) (
    // buffer into which Name is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pName,

    // size of *pName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetName().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetTypeDefinition()
  //
  // Returns the type definition for this invocation.
  // 
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppTypeDef.
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
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef arg is NULL.
  //
  STDMETHOD (GetTypeDefinition) (
    // Type definition object 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetValue()
  //
  // Writes the value into the pValue buffer.  The buffer is allocated
  // by the caller, and the size of the buffer is given by
  // valueSize.
  // 
  // Caller may call GetValueBufLen() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // - valueSize indicates the buffer is large enough to hold the name.
  // 
  // If this method fails nothing will be written to *pValue.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - valueSize indicates the buffer is too small to hold the value.
  //
  STDMETHOD (GetValue) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // Preallocated buffer to hold value
    /*[out, size_is(valueSize), length_is(*bytesRead)]*/ aafDataBuffer_t  pValue,

    // Number of actual bytes read
    /*[out]*/ aafUInt32 *  bytesRead);


  //***********************************************************
  //
  // GetValueBufLen()
  //
  // Returns the length of buffer required for the GetValue() method.
  // The value is placed into the location specified by pLen.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
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
  STDMETHOD (GetValueBufLen) (
    // Pointer to an variable used to return the length 
    /*[out]*/ aafUInt32 *  pLen);

  //***********************************************************
  //
  // SetValue()
  //
  // The data value is set from a buffer of size valueSize and type.
  //
  // Succeeds if all of the following are true:
  // - pTypeDef is valid and registered with the object's dictionary.
  // - the pValue pointer is valid.
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
  //   - ppTypeDef is null.
  //
  STDMETHOD (SetValue) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // buffer containing value
    /*[in, size_is(valueSize)]*/ aafDataBuffer_t  pValue);


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

#endif // ! __CAAFTaggedValue_h__


