//@doc
//@class    AAFTypeDef | Implementation class for AAFTypeDef
#ifndef __CAAFTypeDef_h__
#define __CAAFTypeDef_h__

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


class CAAFTypeDef
  : public IAAFTypeDef,
    public CAAFMetaDefinition
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDef ();

public:


  //***********************************************************
  //
  // GetTypeCategory()
  //
  // Returns the type category to which this type definition belongs.
  // 
  // Succeeds if:
  // - The pTid argument is valid
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTid arg is NULL.
  //
  STDMETHOD (GetTypeCategory) (
    // Returned type category 
    /*[out]*/ eAAFTypeCategory_t *  pTid);


  //***********************************************************
  //
  // RawAccessType()
  //
  // This method returns the type def through which values of this
  // type may be accessed if the client wishes to access the value as
  // as raw data.
    // NOTE! Stub only.   Implementation not yet added.
  //

  // 
  // Succeeds if:
  // - The ppRawTYPEDEF argument is valid
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppRawTypeDef arg is NULL.
  //
  STDMETHOD (RawAccessType) (
    // the raw access type definition 
    /*[out]*/ IAAFTypeDef ** ppRawTypeDef);



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

#endif // ! __CAAFTypeDef_h__


