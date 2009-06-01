//@doc
//@class    AAFEventMobSlot | Implementation class for AAFEventMobSlot
#ifndef __CAAFEventMobSlot_h__
#define __CAAFEventMobSlot_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
    public IAAFEventMobSlot2,
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
  /// 
  /// Succeeds if all of the following are true:
  /// - the pEditRate pointer is valid.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEditRate arg is NULL.
  //
  STDMETHOD (GetEditRate) (
    // Edit rate property value 
    /*[out,retval]*/ aafRational_t *  pEditRate);


  //***********************************************************
  //
  // SetEditRate()
  //
  // This method will get set edit rate for this mob slot.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pEditRate pointer is valid.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEditRate arg is NULL.
  //
  STDMETHOD (SetEditRate) (
    // Edit rate property value 
    /*[in]*/ aafRational_t *  pEditRate);





  //***********************************************************
  // METHOD NAME: GetEventSlotOrigin()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEventMobSlot2 | GetEventSlotOrigin |
  // This method will return the EventSlotOrigin of this mob slot.
  /// The EventSlotOrigin is an optional property.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pEventSlotOrigin pointer is valid.
  /// - the EventSlotOrigin property is present.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - The EventSlotOrigin property is not present.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEventSlotOrigin arg is NULL.
  // @end
  // 
  STDMETHOD (GetEventSlotOrigin)
   (
    // @parm [out,retval] aafPosition_t * | pEventSlotOrigin | EventSlotOrigin property value
    aafPosition_t *  pEventSlotOrigin
  );

  //***********************************************************
  // METHOD NAME: SetEventSlotOrigin()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEventMobSlot2 | SetEventSlotOrigin |
  // This method will set the EventSlotOrigin of this mob slot.
  /// EventSlotOrigin is an optional property.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.).
  // @end
  // 
  STDMETHOD (SetEventSlotOrigin)
   (
    // @parm [in] aafPosition_t | eventSlotOrigin | EventSlotOrigin property value
    aafPosition_t  eventSlotOrigin
  );



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

