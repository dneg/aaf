//@doc
//@class    AAFCodecDef | Implementation class for AAFCodecDef
#ifndef __CAAFCodecDef_h__
#define __CAAFCodecDef_h__

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


class CAAFCodecDef
  : public IAAFCodecDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCodecDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFCodecDef ();

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
    /*[in, string]*/ aafCharacter_constptr  pDescription);


  //***********************************************************
  //
  // IsEssenceKindSupported()
  //
  // Returns AAFTrue if the given codec support transfers to essence
  // of the given essence kind.
  // 
  // Succeeds if all of the following are true:
  // - the pEssenceKind pointer is valid.
  // - the pIsSupported pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsSupported.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceKind or pIsSupported is null.
  //
  STDMETHOD (IsEssenceKindSupported) (
    // The essence kind
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Is this type supported
    /*[out,retval]*/ aafBoolean_t*  pIsSupported);

 
  //***********************************************************
  //
  // AddEssenceKind()
  //
  // Appends the given essence kind to those supported by the codec.
  // This is dependant upon the format, not an incomplete implementation.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceKind is null.
  //
  // AAFRESULT_DUPLICATE_ESSENCE_KIND
  //   - the given essenceKind is already contained.
  //
  STDMETHOD (AddEssenceKind) (
    // The essence kind 
    /*[in]*/ IAAFDataDef * pEssenceKind);


  //***********************************************************
  //
  // RemoveEssenceKind()
  //
  // Removes the given essence kind from the list of those supported
  // by the codec.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceKind is null.
  //
  // AAFRESULT_OBJECT_NOT_FOUND
  //   - the given essenceKind is not already contained.
  //
  STDMETHOD (RemoveEssenceKind) (
    // The essence kind to remove 
    /*[in]*/ IAAFDataDef * pEssenceKind);


  //***********************************************************
  //
  // CountEssenceKinds()
  //
  // Places the number of supported essence kinds into *pResult.
  // 
  // If this method fails nothing will be written to *pResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is NULL.
  //
  STDMETHOD (CountEssenceKinds) (
    // The number of essence kinds 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetEssenceKinds()
  //
  // Places an IEnumAAFDataDefs enumerator for the essence kinds
  // contained in the sequence into the *ppEnum argument.
  // 
  // The returned enumerator is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  // E_FAIL
  //   - Failed to create the enumerator.
  //
  STDMETHOD (GetEssenceKinds) (
    // Essence Kind Enumeration 
    /*[out, retval]*/ IEnumAAFDataDefs ** ppEnum);


  //***********************************************************
  //
  // AreThereFlavours()
  //
  // Find out whether its worth iterating over flavours.  Flavours are
  // used when a single codec can support multiple formats.  An
  // example would be a codec which would accept a "resolution ID"
  // for a particular manufacturer and set up all of the parameters.
  // When a new resolution ID is released, then a new codec plugin
  // would give users the ability to use the new resolutions without
  // upgrading the application.
  //
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - if Result is null.
  //
  STDMETHOD (AreThereFlavours) (
    // True if there are flavours of this codec 
    /*[out,retval]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // GetFileDescriptorClass()
  //
  // Places the file descriptor class object associated with this
  // codec into the *ppClass argument.  If none exists yet, NULL is
  // placed into the*ppClass argument.
  //
  // The returned class object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppClass pointer is valid.
  // - A valid file descriptor class exists.
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
  //   - ppClass is null.
  //
  STDMETHOD (GetFileDescriptorClass) (
    // Returned file descriptor class object 
    /*[out]*/ IAAFClassDef ** ppClass);


  //***********************************************************
  //
  // SetFileDescriptorClass()
  //
  // Sets the file descriptor class associated with this codec to be
  // the given one. 
  //
  // Succeeds if all of the following are true:
  // - the pClass pointer is valid.
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
  //   - pClass is null.
  //
  STDMETHOD (SetFileDescriptorClass) (
    // File descriptor class object 
    /*[in]*/ IAAFClassDef * pClass);


  //***********************************************************
  //
  // EnumCodecFlavours()
  //
  // Places an enumerator for codec flavour into the *ppEnum argument.
  // The returned enumerator is AddRef()ed before it is returned.
  // 
  // Flavours are used when a single codec can support multiple
  // formats.  An example would be a codec which would accept a
  // "resolution ID" for a particular manufacturer and set up all of
  // the parameters.  When a new resolution ID is released, then a
  // new codec plugin would give users the ability to use the new
  // resolutions without upgrading the application.
  //
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - if ppEnum is null.
  //
  STDMETHOD (EnumCodecFlavours) (
    // Codec flavour Enumeration 
    /*[out,retval]*/ IEnumAAFCodecFlavours ** ppEnum);
  

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

#endif // ! __CAAFCodecDef_h__


