//@doc
//@class    AAFClassDef | Implementation class for AAFClassDef
#ifndef __CAAFClassDef_h__
#define __CAAFClassDef_h__

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








#ifndef __AAFObjectModel_h__
#include "AAFObjectModel.h"
#endif





#ifndef __CAAFMetaDefinition_h__
#include "CAAFMetaDefinition.h"
#endif


class CAAFClassDef
  : public IAAFClassDef,
    public CAAFMetaDefinition
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFClassDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFClassDef ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this class definition object to inherit from the
  // given parent class.  If isConcrete is set to AAFTrue, objects of
  // this class can be instantiated; otherwise, it will be illegal to
  // instantiate such objects.
  // 
  // Succeeds if:
  // - The pID pointer is valid.
  // - The pParentClass pointer is valid.
  // - The pTypeName pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pID, pParentClass, or pTypeName arg is NULL.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  classID,

    // existing class from which this one inherits
    /*[in]*/ IAAFClassDef * pParentClass,

    // friendly name of this type definition
    /*[in, string]*/ aafCharacter_constptr  pClassName,

    // can objects of this class be instantiated
    /*[in]*/ aafBoolean_t  isConcrete);


  //***********************************************************
  //
  // GetPropertyDefs()
  //
  // Returns an enumerator over all of the aaf property definitions
  // attached to the current class.
  // 
  // Succeeds if:
  // - The ppEnum pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetPropertyDefs) (
    // Property Definition enumeration 
    /*[out, retval]*/ IEnumAAFPropertyDefs ** ppEnum);


  //***********************************************************
  //
  // CountPropertyDefs()
  //
  // Returns number of property definitions in this class.
  // 
  // Succeeds if:
  // - The pCount pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount arg is NULL.
  //
  STDMETHOD (CountPropertyDefs) (
    // number of properties contained in this class definition 
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // RegisterNewPropertyDef()
  //
  // Creates a new property definition and registers it in this class
  // definition.  If ppPropDef is non-NULL, will return the new
  // property definition in ppPropDef.
  // 
  // Note that it is illegal to add mandatory properties to an
  // existing (registered) class.  This method will allow adding
  // either optional or mandatory properties to a class, but they must
  // be added to a class which has not yet been registered in the
  // dictionary.  If this class has already been registered, it is
  // possible to add optional properties, but not through this
  // method.  Optional properties added to an existing (registered)
  // class may be added through the RegisterOptionalPropertyDef()
  // method.
  // 
  // Succeeds if:
  // - The pID pointer is valid.
  // - The pName pointer is valid.
  // - The pTypeDef pointer is valid.
  // - This class has not already been registered in the dictionary.
  // - The auid specified by pID has not already been registered.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pName or pTypeDef arg is NULL.
  //
  // AAFRESULT_OBJECT_ALREADY_ATTACHED
  //   - This class has already been registered.
  //
  // AAFRESULT_ALREADY_UNIQUELY_INDENTIFED
  //   - This class already has a unique indentifier property.
  //
  // AAFRESULT_BAD_PARAM
  //   - The given ID has already been registered.
  //
  STDMETHOD (RegisterNewPropertyDef) (
    // auid to be used to identify this property
    /*[in, ref]*/ aafUID_constref  id,

    // name of the new property
    /*[in, string]*/ aafCharacter_constptr  pName,

    // type of the new property
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // true if new property is to be optional
    /*[in]*/ aafBoolean_t  isOptional,

    // true if new property is to be the unique identifier of the class
    /*[in]*/ aafBoolean_t  isUniqueIdentifier,

    // return pointer to newly created property def
    /*[out]*/ IAAFPropertyDef ** ppPropDef);


  //***********************************************************
  //
  // RegisterOptionalPropertyDef()
  //
  // Creates a new property definition and registers it in this class
  // definition.  If ppPropDef is non-NULL, will return the new
  // property definition in ppPropDef.
  //
  // Note that it is illegal to add mandatory properties to an already
  // existing (registered) class.  It is assumed that this property is
  // being added to a class which is already registered.  If so, it
  // must be optional and this method will declare it so.  If it is
  // wished to add a mandatory property, that may be done through the
  // RegisterNewPropertyDef() method, but that must be called on a class
  // which is not yet registered.
  // 
  // Succeeds if:
  // - The pID pointer is valid.
  // - The pName pointer is valid.
  // - The pTypeDef pointer is valid.
  // - The auid specified by pID has not already been registered.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either pName or pTypeDef arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - The given ID has already been registered.
  //
  STDMETHOD (RegisterOptionalPropertyDef) (
    // auid to be used to identify this property
    /*[in, ref]*/ aafUID_constref  id,

    // name of the new property
    /*[in, string]*/ aafCharacter_constptr  pName,

    // type of the new property
    /*[in]*/ IAAFTypeDef * pTypeDef,

    // return pointer to newly created property def
    /*[out]*/ IAAFPropertyDef ** ppPropDef);


  //***********************************************************
  //
  // LookupPropertyDef()
  //
  // Looks up the property definition corresponding to the named auid
  // and returns a pointer to that property definition in ppPropDef.
  // 
  // Succeeds if:
  // - The pPropID pointer is valid.
  // - The ppPropDef pointer is valid.
  // - the auid specified by pID has been registered as a property
  //   definition for this class definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppPropDef arg is NULL.
  //
  // AAFRESULT_BAD_PARAM
  //   - The given ID has not been registered as a property
  //     definition.
  //
  STDMETHOD (LookupPropertyDef) (
    // auid reprepresenting property to look up
    /*[in, ref]*/ aafUID_constref  propID,

    // resulting property definition
    /*[out]*/ IAAFPropertyDef ** ppPropDef);


  //***********************************************************
  //
  // GetName()
  //
  // Gets Accesses a human-readable name for the class.  This name is not
  // meant to be a way for programs to refer to the class, as it is
  // of undetermined length, and is not checked to guarantee
  // uniqueness.
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
  // GetParent()
  //
  // Gets the Parent class for this object.  If there is no parent,
  // returns the result AAFRESULT_NO_PARENT.  The only class which has no parent will
  // be AAFObject.
  // 
  // Succeeds if:
  // - The ppClassDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pClassDef arg is NULL.
  //
  STDMETHOD (GetParent) (
    // parent class definition 
    /*[out, retval]*/ IAAFClassDef ** ppClassDef);


  //***********************************************************
  //
  // IsConcrete()
  //
  // Returns true if this class can be instantiated; returns false
  // otherwise. 
  // 
  // Succeeds if:
  // - The ppClassDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  STDMETHOD (IsConcrete) (
    // Can this class be instantiated 
    /*[out, retval]*/ aafBoolean_t*  pResult);


  //***********************************************************
  //
  // IsRoot()
  //
  // Returns true if this class is the base of the inheritance
  // hierarchy; returns false otherwise.
  // 
  // Succeeds if:
  // - The ppClassDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - isRoot arg is NULL.
  //
  STDMETHOD (IsRoot) (
    // Is this a root (base) class 
    /*[out, retval]*/ aafBoolean_t*  isRoot);


  //***********************************************************
  //
  // IsUniquelyIdentified()
  //
  // Returns true if this class is uniquely identified.
  // 
  // Succeeds if:
  // - The pIsUniquelyIdentified pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsUniquelyIdentified arg is NULL.
  //
  STDMETHOD (IsUniquelyIdentified) (
    // Is this a UniquelyIdentified class 
    /*[out, retval]*/ aafBoolean_t*  pIsUniquelyIdentified);


  //***********************************************************
  //
  // GetUniqueIdentifier()
  //
  // Returns the property definition corresponding to this class'
  // unique identifier.
  // 
  // Succeeds if:
  // - The ppUniqueIdentifier pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppUniqueIdentifier arg is NULL.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - If this class definition does not have a unique identifier
  //     property definition.
  //
  STDMETHOD (GetUniqueIdentifier) (
    // The unique identifier 
    /*[out, retval]*/ IAAFPropertyDef ** ppUniqueIdentifier);


  // Creates an object of this class, and returns it by reference in
  // the location specified by the ppObject argument.
  // 
  // Succeeds if:
  // - The ppObject pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppObject arg is NULL. 
  STDMETHOD (CreateInstance)
   (// Reference to the identifier of the interface
    /*[in, ref]*/ REFIID riid,

    // Address of output variable that receives the 
    // interface pointer requested in riid
    /*[out, iid_is(riid)]*/ IUnknown ** ppvObject);


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

#endif // ! __CAAFClassDef_h__


