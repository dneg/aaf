//@doc
//@class    AAFScopeReference | Implementation class for AAFScopeReference
#ifndef __CAAFScopeReference_h__
#define __CAAFScopeReference_h__

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




#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFScopeReference
  : public IAAFScopeReference,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFScopeReference (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFScopeReference ();

public:


  //***********************************************************
  //
  // Create()
  //
  // Constructs a Scope Reference object.
  // 
  // If this method fails the length property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (Create) (
    // Number of nested scopes to pass to find the Nested Scope slot
    /*[in]*/ aafUInt32  RelativeScope,

    // Number of slots to look backwards from the slot containing the
    // Scope Reference
    /*[in]*/ aafUInt32  RelativeSlot);

  //***********************************************************
  //
  // Initialize()
  //
  // Constructs a Scope Reference object.
  // 
  // If this method fails the length property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (Initialize) (
    // Data definition for referenced slot
    /*[in]*/ IAAFDataDef * pDataDef,

    // Number of nested scopes to pass to find the Nested Scope slot
    /*[in]*/ aafUInt32  RelativeScope,

    // Number of slots to look backwards from the slot containing the
    // Scope Reference
    /*[in]*/ aafUInt32  RelativeSlot);

  //***********************************************************
  //
  // GetRelativeScope()
  //
  // Gets the number of nested scopes to pass to find the Nested Scope
  // slot.
  //	
  // Succeeds if all of the following are true:
  // - the pnRelativeScope pointer is valid.
  // 
  // If this method fails nothing will be written to *pnRelativeScope.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pnRelativeScope arg is NULL.
  //
  STDMETHOD (GetRelativeScope) (
    // Pointer to a Relative Scope 
    /*[retval][out]*/ aafUInt32 *  pnRelativeScope);


  //***********************************************************
  //
  // GetRelativeSlot()
  //
  // Gets the number of slots that preced the slot containing the
  // Scope Reference. 
  //	
  // Succeeds if all of the following are true:
  // - the pnRelativeScope pointer is valid.
  // 
  // If this method fails nothing will be written to *pnRelativeSlot.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pnRelativeSlot arg is NULL.
  //
  STDMETHOD (GetRelativeSlot) (
    // Pointer to a Relative Slot 
    /*[retval][out]*/ aafUInt32 *  pnRelativeSlot);

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

#endif // ! __CAAFScopeReference_h__


