//@doc
//@class    AAFPulldown | Implementation class for AAFPulldown
#ifndef __CAAFPulldown_h__
#define __CAAFPulldown_h__

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


class CAAFPulldown
  : public IAAFPulldown,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPulldown (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPulldown ();

public:


  //***********************************************************
  //
  // GetInputSegment()
  //
  // Places the input Segment object in this pulldown into the
  // *ppInputSegment argument.  If none exists yet, NULL is placed
  // into the *ppInputSegment argument.
  //
  // The returned segment object, if it exists, is AddRef()ed
  // before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppInputSegment pointer is valid.
  // - A valid segment exists.
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
  //   - ppInputSegment is null.
  //
  STDMETHOD (GetInputSegment) (
    // Returns the input Segment object 
    /*[out]*/ IAAFSegment ** ppInputSegment);


  //***********************************************************
  //
  // SetInputSegment()
  //
  // Sets the input segment .
  //
  // Succeeds if all of the following are true:
  // - the pInputSegment pointer is valid.
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
  //   - pInputSegment is null.
  //
  STDMETHOD (SetInputSegment) (
    // A Segment object 
    /*[in]*/ IAAFSegment * pInputSegment);


  //***********************************************************
  //
  // GetPulldownKind()
  //
  // Returns the pulldownKind property of this pulldown through the
  // *pPulldownKind argument.  The pulldown kinds include
  // kThreeTwoPD, kPalPD, kOneToOneNTSC, kOneToOnePAL, and
  // kVideoTapNTSC.
  //
  // Succeeds if all of the following are true:
  // - the pPulldownKind pointer is valid.
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
  //   - pPulldownKind is null.
  //
  STDMETHOD (GetPulldownKind) (
    // Returns the pulldownKind property 
    /*[out]*/ aafPulldownKind_t *  pPulldownKind);


  //***********************************************************
  //
  // SetPulldownKind()
  //
  // Sets the pulldown kind field of this pulldown object.  The
  // pulldown kinds include kThreeTwoPD, kPalPD, kOneToOneNTSC,
  // kOneToOnePAL, and kVideoTapNTSC.
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
  //   - pInputSegment is null.
  //
  STDMETHOD (SetPulldownKind) (
    // A Segment object 
    /*[in]*/ aafPulldownKind_t  pulldownKind);


  //***********************************************************
  //
  // GetPulldownDirection()
  //
  // Returns the pulldownDirection field of this pulldown through the
  // *pPulldownDirection argument.  The valid
  // pulldown directions are:
  //    kVideoToFilmSpeed -- The input segment is at videoSpeed, and
  //                         the pulldown object is on a mob slot at
  //                         film edit rate.
  //    kFilmToVideoSpeed -- The input segment is at film edit rate.
  //
  // The value kVideoToFilmSpeed is used when connecting 24fps file
  // mobs to tape mobs. 
  //
  // The value kFilmToVideoSpeed is used when connecting tape mobs to
  // film mobs.
  //
  // Succeeds if all of the following are true:
  // - the pPulldownDirection pointer is valid.
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
  //   - pPulldownDirection is null.
  //
  STDMETHOD (GetPulldownDirection) (
    // Returns the input Segment object 
    /*[out]*/ aafPulldownDir_t *  pPulldownDirection);


  //***********************************************************
  //
  // SetPulldownDirection()
  //
  // Sets the pulldown direction field of this pulldown object.  The
  // valid pulldown directions are:
  //    kVideoToFilmSpeed -- The input segment is at videoSpeed, and
  //                         the pulldown object is on a mob slot at
  //                         film edit rate.
  //    kFilmToVideoSpeed -- The input segment is at film edit rate.
  //
  // The value kVideoToFilmSpeed is used when connecting 24fps file
  // mobs to tape mobs.
  //
  // The value kFilmToVideoSpeed is used when connecting tape mobs to
  // film mobs.
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
  //   - pInputSegment is null.
  //
  STDMETHOD (SetPulldownDirection) (
    // A Segment object 
    /*[in]*/ aafPulldownDir_t  pulldownDirection);


  //***********************************************************
  //
  // GetPhaseFrame()
  //
  // Returns the phaseFrame field of this pulldown through the
  // *pPhaseFrame argument.  The phase frame field specifies the phase
  // within the repeating pattern used to map between the two edit
  // rates. A value of zero specifies that the pulldown object starts
  // at the begining of the pattern.
  //
  // Succeeds if all of the following are true:
  // - the pPhaseFrame pointer is valid.
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
  //   - pPhaseFrame is null.
  //
  STDMETHOD (GetPhaseFrame) (
    // Returns the input Segment object 
    /*[out]*/ aafPhaseFrame_t *  pPhaseFrame);


  //***********************************************************
  //
  // SetPhaseFrame()
  //
  // Sets the phase frame field of this pulldown object.  The phase
  // frame field specifies the phase within the repeating pattern used
  // to map between the two edit rates. A value of zero specifies that
  // the pulldown object starts at the begining of the pattern.
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
  //   - pInputSegment is null.
  //
  STDMETHOD (SetPhaseFrame) (
    // A Segment object 
    /*[in]*/ aafPhaseFrame_t  phaseFrame);







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

#endif // ! __CAAFPulldown_h__


