//@doc
//@class    AAFTypeDefSet | Implementation class for AAFTypeDefSet
#ifndef __CAAFTypeDefSet_h__
#define __CAAFTypeDefSet_h__

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


class CAAFTypeDefSet
  : public IAAFTypeDefSet,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefSet (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefSet ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to contain elements of the given type.
  //
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pTypeName is a valid pointer.
  //
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
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in]*/ aafUID_constref  id,

    // type of each element to be contained in this set
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // friendly name of this type definition
    /*[in,string]*/ aafCharacter_constptr  pTypeName);

  //***********************************************************
  //
  // GetElementType()
  //
  // Returns the type of elements in this set.
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
  STDMETHOD (GetElementType) (
    // type of elements in this array 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);

  //***********************************************************
  //
  // AddElement()
  //
  // Adds an element to the set, setting it to the
  // value given in pElementPropertyValue.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue pointer is valid.
  // - pElementPropertyValue pointer is valid.
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
  //   - either pSetPropertyValue or pElementPropertyValue arg is NULL.
  //
  STDMETHOD (AddElement) (
    // property value corresponding to set to which element is added
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // value to be added to this set
    /*[in]*/ IAAFPropertyValue * pElementPropertyValue);

  //***********************************************************
  //
  // RemoveElement()
  //
  // Removes an element from the set.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue pointer is valid.
  // - pElementPropertyValue pointer is valid.
  // - The given element was in the set.
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
  //   - either pSetPropertyValue or pElementPropertyValue arg is NULL.
  //
  STDMETHOD (RemoveElement) (
    // property value corresponding to set from which element is removed
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // value to be removed from this set
    /*[in]*/ IAAFPropertyValue * pElementPropertyValue);

  //***********************************************************
  //
  // ContainsElement()
  //
  // Tests if an element is in the set.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue pointer is valid.
  // - pElementPropertyValue pointer is valid.
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
  //   - either pSetPropertyValue or pElementPropertyValue arg is NULL.
  //
  STDMETHOD (ContainsElement) (
    // property value corresponding to set to which element is added
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // value whose presence is being tested in this set
    /*[in]*/ IAAFPropertyValue * pElementPropertyValue,

    // value to be added to this set
    /*[out]*/ aafBoolean_t*  pContainsElement);



  //***********************************************************
  //
  // GetCount()
  //
  // Returns number of elements in the referenced property value.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue is a valid pointer.
  // - pCount is a valid pointer.
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
  //   - Either pSetPropertyValue or pCount arg is NULL.
  //
  STDMETHOD (GetCount) (
    // property value of array
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // count of elements in the specified set property value
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // CreateKey()
  //
  // Returns number of elements in the referenced property value.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue is a valid pointer.
  // - pCount is a valid pointer.
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
  //   - Either pSetPropertyValue or pCount arg is NULL.
  //
  STDMETHOD (CreateKey) (
    // Pointer to the key value bytes
    /*[in,size_is(length)]*/ aafDataBuffer_t  pKeyPtr,

    // The size of the key in bytes
    /*[in]*/ aafUInt32  length,

    // An interface which may be passed to LookupElement() or ContainsKey()
    /*[out]*/ IAAFPropertyValue ** ppKey);


  //***********************************************************
  //
  // LookupElement()
  //
  // Looks up the given key (created by CreateKey()) and returns the propertyValue
  // of the entry, or an error if the entry is not present.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue is a valid pointer.
  // - pCount is a valid pointer.
  // - An entry with the correct key is present in the set.
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
  //   - Either pSetPropertyValue or pSetPropertyValue or ppElementPropertyValue arg is NULL.
  //
  STDMETHOD (LookupElement) (
    // property value of set
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // A key returned from CreateKey()
    /*[in]*/ IAAFPropertyValue * pKey,

    // The returned property value
    /*[out]*/ IAAFPropertyValue ** ppElementPropertyValue);

  //***********************************************************
  //
  // ContainsKey()
  //
  // Looks up the given key (created by CreateKey()) and returns AAFTrue if the entry
  // is present, or AAFFalse if the entry is not present.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pSetPropertyValue is a valid pointer.
  // - pCount is a valid pointer.
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
  //   - Either pSetPropertyValue or pContainsKey arg is NULL.
  //
  STDMETHOD (ContainsKey) (
    // property value of set
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

    // A key returned from CreateKey()
    /*[in]*/ IAAFPropertyValue * pKey,

    // Value returned is AAFTrue if an entry with the correct key is present
    /*[out]*/ aafBoolean_t*  pContainsKey);


  //***********************************************************
  //
  // GetElements()
  //
  // Returns an enumerator across elements in this set.
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pSetPropertyValue pointer is valid.
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
  //   - Either pSetPropertyValue or ppEnum arg is NULL.
  //
  STDMETHOD (GetElements) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pSetPropertyValue,

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

#endif // ! __CAAFTypeDefSet_h__


