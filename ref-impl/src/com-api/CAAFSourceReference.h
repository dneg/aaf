//@doc
//@class    AAFSourceReference | Implementation class for AAFSourceReference
#ifndef __CAAFSourceReference_h__
#define __CAAFSourceReference_h__

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





#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFSourceReference
  : public IAAFSourceReference,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSourceReference (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSourceReference ();

public:


  //***********************************************************
  //
  // GetSourceID()
  //
  // Gets the SourceID and places it into the pSourceID argument.
  //
  // Succeeds if all of the following are true:
  // - the pSourceID pointer is valid.
  // 
  // If this method fails nothing will be written to *pSourceID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSourceID arg is NULL.
  //
  STDMETHOD (GetSourceID) (
    // Place to put source ID 
    /*[retval][out]*/ aafMobID_t *  pSourceID);


  //***********************************************************
  //
  // SetSourceID()
  //
  // Sets the SourceID using the sourceID argument.
  // 
  // Always succeeds.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetSourceID) (
    // Source ID to set 
    /*[in]*/ aafMobID_constref   sourceID);


  //***********************************************************
  //
  // GetSourceMobSlotID()
  //
  // Gets the Mob Slot ID and places it into the pMobSlotID argument.
  //
  // Succeeds if all of the following are true:
  // - the pMobSlotID pointer is valid.
  // 
  // If this method fails nothing will be written to *pMobSlotID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMobSlotID arg is NULL.
  //
  STDMETHOD (GetSourceMobSlotID) (
    // Place to put source mob slot ID 
    /*[retval][out]*/ aafSlotID_t *  pMobSlotID);


  //***********************************************************
  //
  // SetSourceMobSlotID()
  //
  // Sets the mob slot ID using the mobSlotID argument.
  // 
  // Succeeds if all of the following are true:
  // - (preconditions here)
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
  // (other error codes here.)
  //
  STDMETHOD (SetSourceMobSlotID) (
    // Source Mob ID to set 
    /*[in]*/ aafSlotID_t   mobSlotID);

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

#endif // ! __CAAFSourceReference_h__


