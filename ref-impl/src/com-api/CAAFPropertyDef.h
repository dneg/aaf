//@doc
//@class    AAFPropertyDef | Implementation class for AAFPropertyDef
#ifndef __CAAFPropertyDef_h__
#define __CAAFPropertyDef_h__

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





 


#ifndef __CAAFMetaDefinition_h__
#include "CAAFMetaDefinition.h"
#endif


class CAAFPropertyDef
  : public IAAFPropertyDef,
    public CAAFMetaDefinition
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPropertyDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPropertyDef ();

public:


  //***********************************************************
  //
  // GetTypeDef()
  //
  // Returns a reference to this property's type definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  //
  STDMETHOD (GetTypeDef) (
    // definition of type contained by this property 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetName()
  //
  // Gets the human-legible name.
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
  // GetIsOptional()
  //
  // Sets *pIsOptional to AAFTrue for properties that are optional.
  // Sets it to AAFFalse for properties that are mandatory.
  //
  // Succeeds if:
  // - The pIsOptional pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
    // NOTE! Stub only.   Implementation not yet added.
  //

  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - The pIsOptional pointer is NULL.
  //
  STDMETHOD (GetIsOptional) (
    // pointer to the result 
    /*[out, retval]*/ aafBoolean_t *  pIsOptional);

  //***********************************************************
  //
  // GetIsUniqueIdentifier()
  //
  // Sets *pIsUniqueIdentifier to AAFTrue for properties that are optional.
  // Sets it to AAFFalse for properties that are mandatory.
  //
  // Succeeds if:
  // - The pIsUniqueIdentifier pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
    // NOTE! Stub only.   Implementation not yet added.
  //

  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - The pIsUniqueIdentifier pointer is NULL.
  //
  STDMETHOD (GetIsUniqueIdentifier) (
    // pointer to the result 
    /*[out, retval]*/ aafBoolean_t *  pIsUniqueIdentifier);




  //***********************************************************
  //
  // SetDescription()
  //
  // Sets the description of the property definition.
  //
  // Set the Description property to the value specified in
  // pDescription.  A copy is made of the data so the caller
  // retains ownership of the *pDescription buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pDescription pointer is valid.
  // 
  // If this method fails the Description property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescription arg is NULL.
  //
  STDMETHOD (SetDescription) (
    // buffer from which Description is to be read 
    /*[in, string]*/ aafCharacter_constptr  pDescription);


  //***********************************************************
  //
  // GetDescription()
  //
  // Gets the description of the property definition.
  // 
  // Writes the Description property, with a trailing null
  // character, into the pDescription buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Description property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetDescriptionBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pDescription.
  // 
  // Succeeds if:
  // - The pDescription pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Description.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescription arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Description.
  //
  STDMETHOD (GetDescription) (
    // buffer into which Description is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pDescription,

    // size of *pDescription buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetDescriptionBufLen()
  //
  // Returns size of buffer (in bytes) required for GetDescription().
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
  STDMETHOD (GetDescriptionBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


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

#endif // ! __CAAFPropertyDef_h__


