//@doc
//@class    AAFTransition | Implementation class for AAFTransition
#ifndef __CAAFTransition_h__
#define __CAAFTransition_h__

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








#ifndef __CAAFComponent_h__
#include "CAAFComponent.h"
#endif


class CAAFTransition
  : public IAAFTransition,
    public CAAFComponent
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTransition (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTransition ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Constructor which allows specification of starting values.
  //
  STDMETHOD (Initialize) (
    // Data Definition Object
    /*[in]*/ IAAFDataDef * pDataDef,

    // Length property value
    /*[in]*/ aafLength_t  length,

    // The point at which a cut would be inserted if the transition
	// were removed
    /*[in]*/ aafPosition_t  cutPoint,

    // A reference to an operation group object
    /*[in]*/ IAAFOperationGroup * op);	 


  //***********************************************************
  //
  // GetCutPoint()
  //
  // Gets the point at which a cut would be inserted if the transition
  // were removed.
  //
  STDMETHOD (GetCutPoint) (
    // Cut Point 
    /*[out]*/ aafPosition_t *  cutPoint);


  //***********************************************************
  //
  // GetOperationGroup()
  //
  // Gets the OperationGroup associated with the transition.
  //
  STDMETHOD (GetOperationGroup) (
    // OperationGroup used by transition 
    /*[out]*/ IAAFOperationGroup ** groupObj);


  //***********************************************************
  //
  // SetCutPoint()
  //
  // Sets the point at which a cut would be inserted if the transition
  // were removed.
  //
  STDMETHOD (SetCutPoint) (
    // Cut Point 
    /*[in]*/ aafPosition_t  cutPoint);


  //***********************************************************
  //
  // SetOperationGroup()
  //
  // Sets the operation group associated with the transition.
  //
  STDMETHOD (SetOperationGroup) (
    // Operation group used by transition 
    /*[in]*/ IAAFOperationGroup * opgroup);

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

#endif // ! __CAAFTransition_h__


