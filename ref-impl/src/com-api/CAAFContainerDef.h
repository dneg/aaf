//@doc
//@class    AAFContainerDef | Implementation class for AAFContainerDef
#ifndef __CAAFContainerDef_h__
#define __CAAFContainerDef_h__

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


class CAAFContainerDef
  : public IAAFContainerDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFContainerDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFContainerDef ();

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
  // EssenceIsIdentified()
  //
  // Tells whether the given plugin is capable of supporting
  // authentication.  The methods for authenticating a plugin are
  // still <tbd>.
  // 
  // Succeeds if all of the following are true:
  // - the pEssenceIsIdentified pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pEssenceIsIdentified.
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
  //   - pEssenceIsIdentified arg is NULL.
  //
  STDMETHOD (EssenceIsIdentified) (
    // The EssenceIsIdentified 
    /*[out]*/ aafBoolean_t *  pEssenceIsIdentified);


  //***********************************************************
  //
  // SetEssenceIsIdentified()
  //
  // Tells whether the given plugin is capable of supporting
  // authentication.  The methods for authenticating a plugin are
  // still <tbd>.
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
  STDMETHOD (SetEssenceIsIdentified) (
    // The EssenceIsIdentified flag 
    /*[in]*/ aafBoolean_t  EssenceIsIdentified);

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

#endif // ! __CAAFContainerDef_h__


