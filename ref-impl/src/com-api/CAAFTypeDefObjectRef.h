//@doc
//@class    AAFTypeDefObjectRef | Implementation class for AAFTypeDefObjectRef
#ifndef __CAAFTypeDefObjectRef_h__
#define __CAAFTypeDefObjectRef_h__

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


class CAAFTypeDefObjectRef
  : public IAAFTypeDefObjectRef,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefObjectRef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefObjectRef ();

public:



  //***********************************************************
  //
  // GetObjectType()
  //
  // Returns the class def representing objects to which this type def
  // can refer.  The returned type def is the class definition of the
  // least-derived type which is permissible to be represented.
  //
  // Succeeds if:
  // - this object has been Initialize()d.
  // - The ppObjType pointer is valid.
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
  //   - ppObjType arg is NULL.
  //
  STDMETHOD (GetObjectType) (
    // class def of objects permitted to be referenced 
    /*[out]*/ IAAFClassDef ** ppObjType);

  //***********************************************************
  //
  // CreateValue()
  //
  // Creates a property value which contains an object reference.
  // Initializes it to refer to the given object, and returns the
  // newly-created property value in ppPropVal.
  //
  // Succeeds if:
  // - The pObj pointer is valid.
  // - The ppPropVal pointer is valid.
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
  //   - either pObj or ppPropVal arg is NULL.
  //
  STDMETHOD (CreateValue) (
    // object with which to initialize this object reference
    /*[in]*/ IUnknown * pObj,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);

  //***********************************************************
  //
  // GetObject()
  //
  // Returns the object contained in the named property value.
  // Requires that pPropVal be a strong object reference or a weak
  // object reference.
  //
  // Succeeds if:
  // - The pPropVal pointer is valid.
  // - The ppObject pointer is valid.
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
  //   - either pPropVal or ppObject arg is NULL.
  //
  STDMETHOD (GetObject) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // interface id of the required output
    /*[in]*/ REFIID  iid,

    // pointer to object value
    /*[out, iid_is(iid)]*/ IUnknown ** ppObject);


  //***********************************************************
  //
  // SetObject()
  //
  // Sets the named property value to refer to the named object.
  // Requires that pPropVal be a strong object reference or a weak
  // object reference.
  //
  // Succeeds if:
  // - The pPropVal pointer is valid.
  // - The ppObject pointer is valid.
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
  //   - either pPropVal or ppObject arg is NULL.
  //
  STDMETHOD (SetObject) (
    // property value from which value is to be read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // pointer to object value
    /*[in]*/ IUnknown * pObject);




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

#endif // ! __CAAFTypeDefObjectRef_h__


