//@doc
//@class    AAFParameterDef | Implementation class for AAFParameterDef
#ifndef __CAAFParameterDef_h__
#define __CAAFParameterDef_h__

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



 


#ifndef __CAAFDefObject_h__
#include "CAAFDefObject.h"
#endif


class CAAFParameterDef
  : public IAAFParameterDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFParameterDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFParameterDef ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Init all fields of a definition object.
  //
  STDMETHOD (Initialize) (
    // AUID for new DeObject
    /*[in, ref]*/ aafUID_constref  id,

    // Name for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Description for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pDescription,

    // The type definition for all parameters that use this definition
    /*[in]*/ IAAFTypeDef * pType);


  //***********************************************************
  //
  // GetTypeDefinition()
  //
  // Places the AAFTypeDef object attached to this IAAFParameterDef
  // into the *ppTypeDef argument.  If none exists yet, NULL is
  // placed into the *ppTypeDef argument.
  //
  // The returned AAFTypeDef object, if it exists, is AddRef()ed
  // before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // - A valid AAFTypeDef exists.
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
  // AAFRESULT_NO_ESSENCE_DESC
  //   - There is no AAFTypeDef.  There has to be one of some
  //     kind for this to be a valid operation definition.
  //
  STDMETHOD (GetTypeDefinition) (
    // Pointer to a type definition 
    /*[retval,out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // SetDisplayUnits()
  //
  // Sets the Display Units string property.
  //
  // Set the DisplayUnits property to the value specified in
  // pDisplayUnits.  A copy is made of the data so the caller
  // retains ownership of the *pDisplayUnits buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pDisplayUnits pointer is valid.
  // 
  // If this method fails the DisplayUnits property will not be
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
  //   - pDisplayUnits arg is NULL.
  //
  STDMETHOD (SetDisplayUnits) (
    // buffer from which DisplayUnits is to be read 
    /*[in, string]*/ aafCharacter_constptr  pDisplayUnits);


  //***********************************************************
  //
  // GetDisplayUnits()
  //
  // Gets the Display Units string property.
  // 
  // Writes the DisplayUnits property, with a trailing null
  // character, into the pDisplayUnits buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the DisplayUnits property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetDisplayUnitsBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pDisplayUnits.
  // 
  // Succeeds if:
  // - The pDisplayUnits pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   DisplayUnits.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDisplayUnits arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold DisplayUnits.
  //
  STDMETHOD (GetDisplayUnits) (
    // buffer into which DisplayUnits is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pDisplayUnits,

    // size of *pDisplayUnits buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetDisplayUnitsBufLen()
  //
  // Returns size of buffer (in bytes) required for GetDisplayUnits().
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
  STDMETHOD (GetDisplayUnitsBufLen) (
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

#endif // ! __CAAFParameterDef_h__


