//@doc
//@class    AAFParameter | Implementation class for AAFParameter
#ifndef __CAAFParameter_h__
#define __CAAFParameter_h__

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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif



 


#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFParameter
  : public IAAFParameter,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFParameter (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFParameter ();

public:



  //***********************************************************
  //
  // GetParameterDefinition()
  //
  // Places the parameter definition of the operation parameter into
  // the *ppParmDef argument.  The length of an operation parameter is
  // in the same edit units and has the same value as the
  // IAAFOperationGroup enclosing this parameter.
  //
  // Succeeds if all of the following are true:
  // - the ppParmDef pointer is valid.
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
  //   - ppParmDef is null.
  //
  STDMETHOD (GetParameterDefinition) (
    // New parameter definition 
    /*[out,retval]*/ IAAFParameterDef ** ppParmDef);

  //***********************************************************
  //
  // GetTypeDefinition()
  //
  // Places the IAAFTypeDefinition of the data value inside this
  // parameter into the *ppTypeDef argument.  The data value is the
  // value of the parameter.  It is often an integer or rational, and
  // may change over time.  An example of a value would be the
  // "level" parameter of a video dissolve, which has control
  // points with a value of zero (0 percent B material) at the
  // start, to one (100 percent B material) at the end.  The data
  // value will actually be stored in either AAFConstantValue or one
  // of the AAFControlPoints inside of an AAFVaryingValue.
  //
  // The definition is stored in the base class because it should be
  // constant for all control points inside of a varying value.
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
  STDMETHOD (GetTypeDefinition) (
    // Type Definition of the data value inside of this object 
    /*[out,retval]*/ IAAFTypeDef ** ppTypeDef);

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

#endif // ! __CAAFParameter_h__


