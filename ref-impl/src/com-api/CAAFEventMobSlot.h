//@doc
//@class    AAFEventMobSlot | Implementation class for AAFEventMobSlot
#ifndef __CAAFEventMobSlot_h__
#define __CAAFEventMobSlot_h__

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






#ifndef __CAAFMobSlot_h__
#include "CAAFMobSlot.h"
#endif


class CAAFEventMobSlot
  : public IAAFEventMobSlot,
    public CAAFMobSlot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEventMobSlot (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEventMobSlot ();

public:


  //***********************************************************
  //
  // GetEditRate()
  //
  // This method will get the edit rate for this mob slot.
  // 
  // Succeeds if all of the following are true:
  // - the pEditRate pointer is valid.
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
  //   - pEditRate arg is NULL.
  //
  STDMETHOD (GetEditRate) (
    // Edit rate property value 
    /*[out,retval]*/ aafRational_t *  pEditRate);


  //***********************************************************
  //
  // SetEditRate()
  //
  // This method will get set edit rate for this mob slot.
  // 
  // Succeeds if all of the following are true:
  // - the pEditRate pointer is valid.
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
  //   - pEditRate arg is NULL.
  //
  STDMETHOD (SetEditRate) (
    // Edit rate property value 
    /*[in]*/ aafRational_t *  pEditRate);




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

#endif // ! __CAAFEventMobSlot_h__


