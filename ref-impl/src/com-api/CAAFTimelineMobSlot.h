//@doc
//@class    AAFTimelineMobSlot | Implementation class for AAFTimelineMobSlot
#ifndef __CAAFTimelineMobSlot_h__
#define __CAAFTimelineMobSlot_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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


class CAAFTimelineMobSlot
  : public IAAFTimelineMobSlot,
    public IAAFTimelineMobSlot2,
    public CAAFMobSlot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTimelineMobSlot (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTimelineMobSlot ();

public:

  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  // IAAFTimelineMobSlot-supporting object.  This method must be
  // called after allocation, and before any other method can be
  // called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



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
  STDMETHOD (SetEditRate) (
    // Edit rate property value 
    /*[in]*/ aafRational_constref  editRate);


  //***********************************************************
  //
  // GetOrigin()
  //
  // This method will return the origin of this mob slot.
  // 
  // Succeeds if all of the following are true:
  // - the pOrigin pointer is valid.
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
  //   - pOrigin arg is NULL.
  //
  STDMETHOD (GetOrigin) (
    // Origin property value 
    /*[out,retval]*/ aafPosition_t *  pOrigin);


  //***********************************************************
  //
  // SetOrigin()
  //
  // This method will set the origin of this mob slot.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.).
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetOrigin) (
    // Origin property value 
    /*[in]*/ aafPosition_t  origin);
  //***********************************************************
  // METHOD NAME: SetMarkIn()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | SetMarkIn |
  // Sets the MarkIn property of this TimelineMobSlot.
  // This property is optional.
  //
  // Succeeds if all of the following are true:
  // - the object is initialized.
  //
  // If this method fails the MarkIn property will not be
  // changed.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  // @end
  // 
  STDMETHOD (SetMarkIn)
   (
    // @parm [in] aafPosition_t | value | MarkIn property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetMarkIn()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | GetMarkIn |
  // Gets the MarkIn property of this TimelineMobSlot.
  // This property is optional.
  // 
  // Succeeds if all of the following are true:
  // - the object is initialized.
  // - the result pointer is valid.
  // - the property is present.
  // 
  // If this method fails nothing will be written to *result.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMarkIn arg is NULL.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - the property is not present.
  // @end
  // 
  STDMETHOD (GetMarkIn)
   (
    // @parm [out,retval] aafPosition_t * | result | MarkIn property value
    aafPosition_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetMarkOut()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | SetMarkOut |
  // Sets the MarkOut property of this TimelineMobSlot.
  // This property is optional.
  //
  // Succeeds if all of the following are true:
  // - the object is initialized.
  //
  // If this method fails the MarkOut property will not be
  // changed.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  // @end
  // 
  STDMETHOD (SetMarkOut)
   (
    // @parm [in] aafPosition_t | value | MarkOut property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetMarkOut()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | GetMarkOut |
  // Gets the MarkOut property of this TimelineMobSlot.
  // This property is optional.
  // 
  // Succeeds if all of the following are true:
  // - the object is initialized.
  // - the result pointer is valid.
  // - the property is present.
  // 
  // If this method fails nothing will be written to *result.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMarkIn arg is NULL.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - the property is not present.
  // @end
  // 
  STDMETHOD (GetMarkOut)
   (
    // @parm [out,retval] aafPosition_t * | result | MarkOut property value
    aafPosition_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetUserPos()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | SetUserPos |
  // Sets the UserPos property of this TimelineMobSlot.
  // This property is optional.
  //
  // Succeeds if all of the following are true:
  // - the object is initialized.
  //
  // If this method fails the UserPos property will not be
  // changed.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  // @end
  // 
  STDMETHOD (SetUserPos)
   (
    // @parm [in] aafPosition_t | value | UserPos property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetUserPos()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFTimelineMobSlot2 | GetUserPos |
  // Gets the UserPos property of this TimelineMobSlot.
  // This property is optional.
  // 
  // Succeeds if all of the following are true:
  // - the object is initialized.
  // - the result pointer is valid.
  // - the property is present.
  // 
  // If this method fails nothing will be written to *result.
  //
  // This method will return the following codes:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - result arg is NULL.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - the property is not present.
  // @end
  // 
  STDMETHOD (GetUserPos)
   (
    // @parm [out,retval] aafPosition_t * | result | UserPos property value
    aafPosition_t *  result
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

#endif // ! __CAAFTimelineMobSlot_h__

