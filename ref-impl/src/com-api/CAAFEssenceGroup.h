//@doc
//@class    AAFEssenceGroup | Implementation class for AAFEssenceGroup
#ifndef __CAAFEssenceGroup_h__
#define __CAAFEssenceGroup_h__

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


class CAAFEssenceGroup
  : public IAAFEssenceGroup,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceGroup (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEssenceGroup ();

public:


  //***********************************************************
  //
  // SetStillFrame()
  //
  // This method sets the still frame property on a essence group to
  // be the source clip passed as the stillFrame argument.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pStillFrame is null.
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetStillFrame) (
    // Still Frame source clip  
    /*[in]*/ IAAFSourceClip * pStillFrame);
    

  //***********************************************************
  //
  // GetStillFrame()
  //
  // Gets the still frame property on a essence group.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppStillFrame is null.
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (GetStillFrame) (
    // Still Frame source clip  
    /*[out]*/ IAAFSourceClip ** ppStillFrame);


  //***********************************************************
  //
  // AppendChoice()
  //
  // Append another choice to this AAFEssenceGroup.
  // 
  // Succeeds if all of the following are true:
  // - the pChoice pointer is valid.
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
  // AAFRESULT_NULL_PARAM
  //   - pChoice is null.
  //
  STDMETHOD (AppendChoice) (
    // Source clip to add as a choice  
    /*[in]*/ IAAFSegment * pChoice);


  //***********************************************************
  //
  // PrependChoice()
  //
  // Prepend another choice to this AAFEssenceGroup.
  // 
  // Succeeds if all of the following are true:
  // - the pChoice pointer is valid.
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
  // AAFRESULT_NULL_PARAM
  //   - pChoice is null.
  //
  STDMETHOD (PrependChoice) (
    // Source clip to add as a choice 
    /*[in]*/ IAAFSegment * pChoice);


  //***********************************************************
  //
  // InsertChoiceAt()
  //
  // Insert another choice to this AAFEssenceGroup at the given
  // index.  Choices already existing at the given and higher indices
  // will be moved up by one index to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - the pChoice pointer is valid.
  // - index is less than or equal to the value returned by
  //   CountChoices().
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
  // AAFRESULT_NULL_PARAM
  //   - pChoice is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than the value returned by
  //     CountChoices().
  //
  STDMETHOD (InsertChoiceAt) (
    // place to insert new choice
    /*[in]*/ aafUInt32  index,

    // Source clip to add as a choice
    /*[in]*/ IAAFSegment * pChoice);


  //***********************************************************
  //
  // CountChoices()
  //
  // Returns the number of choices attached to this AAFEssenceGroup.,
  // excepting the still frame.
  // 
  // Succeeds if all of the following are true:
  // - the pCount pointer is valid.
  // 
  // If this method fails nothing is written to *pCount.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount is null.
  //
  STDMETHOD (CountChoices) (
    // The number of representations contained within. 
    /*[out]*/ aafUInt32*   pCount);


  //***********************************************************
  //
  // GetChoiceAt()
  //
  // Given an index, returns the corresponding source clip choice.
  // 
  // Succeeds if all of the following are true:
  // - the ppChoice pointer is valid.
  // 
  // If this method fails nothing will be written to *ppChoice.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_MISSING_INDEX
  //   - The given index value is not present.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppChoice arg is NULL.
  //
  STDMETHOD (GetChoiceAt) (
    // The 0-based index into the array
    /*[in]*/ aafUInt32  index,

    // The representation at that index
    /*[out]*/ IAAFSegment  ** ppChoice);


  //***********************************************************
  //
  // RemoveChoiceAt()
  //
  // Removes the choice at the given index.  Items already existing at
  // higher indices will be moved down by one index to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - the ppChoice pointer is valid.
  // - index is less than the value returned by CountChoices().
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
  // AAFRESULT_NULL_PARAM
  //   - ppChoice arg is NULL.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountChoices().
  //
  STDMETHOD (RemoveChoiceAt) (
    // The 0-based index into the array 
    /*[in]*/ aafUInt32  index);

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

#endif // ! __CAAFEssenceGroup_h__


