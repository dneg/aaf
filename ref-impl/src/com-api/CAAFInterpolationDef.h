//@doc
//@class    AAFInterpolationDef | Implementation class for AAFInterpolationDef
#ifndef __CAAFInterpolationDef_h__
#define __CAAFInterpolationDef_h__

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


class CAAFInterpolationDef
  : public IAAFInterpolationDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFInterpolationDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFInterpolationDef ();

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

#endif // ! __CAAFInterpolationDef_h__


