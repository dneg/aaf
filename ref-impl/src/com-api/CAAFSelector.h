//@doc
//@class    AAFSelector | Implementation class for AAFSelector
#ifndef __CAAFSelector_h__
#define __CAAFSelector_h__

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


class CAAFSelector
  : public IAAFSelector,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSelector (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSelector ();

public:


  //***********************************************************
  //
  // GetSelectedSegment()
  //
  // Places the Selected Segment object in this Selector into the
  // *ppSelSegment argument.  If none exists yet, NULL is placed into
  // the *ppSelSegment argument.
  //
  // The returned essence descriptor object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppSelSegment pointer is valid.
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
  //   - ppSelSegment is null.
  //
  STDMETHOD (GetSelectedSegment) (
    // Returned the selected Segment object 
    /*[out]*/ IAAFSegment ** ppSelSegment);


  //***********************************************************
  //
  // SetSelectedSegment()
  //
  // Sets the Selected segment.
  //
  // Succeeds if all of the following are true:
  // - the pSelSegment pointer is valid.
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
  //   - pSelSegment is null.
  //
  STDMETHOD (SetSelectedSegment) (
    // A Segment object 
    /*[in]*/ IAAFSegment * pSelSegment);


  //***********************************************************
  //
  // AppendAlternateSegment()
  //
  // This function appends the input segment to the alternate segment
  // list, the alternate list of segments represents unused
  // alternative segments. 
  //
  // This method will AddRef() the segment if it succeeds.
  //
  // If the segment is successfully appended to the set of
  // alternates, the reference count of the segment is incremented.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pSegment pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSegment is null.
  //
  STDMETHOD (AppendAlternateSegment) (
    // Segment to append to the Alternate list of segments 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // GetNumAlternateSegments()
  //
  // This function returns the number of segments in the set.
  // 
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the pNumSegments pointer is valid.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumSegments is null.
  //
  STDMETHOD (GetNumAlternateSegments) (
    // Number of Alternate Segments 
    /*[out]*/ aafInt32 *  pNumSegments);


  //***********************************************************
  //
  // EnumAlternateSegments()
  //
  // Places an IEnumAAFSegments enumerator for the alterante segments
  // contained in the selector into the *ppEnum argument.
  // 
  // The returned enumerator is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  // E_FAIL
  //   - Failed to create the enumerator.
  //
  STDMETHOD (EnumAlternateSegments) (
    // Segment Enumeration 
    /*[out, retval]*/ IEnumAAFSegments ** ppEnum);

  //***********************************************************
  //
  // RemoveAlternateSegment()
  //
  // // Removes the given Segment from the selector.
  //
  // Succeeds if all of the following are true:
  // - the v pointer is valid.
  // - the given segment is currently in the collection.
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
  //   - pEssenceData is null.
  //
  // AAFRESULT_SEGMENT_NOT_FOUND
  //   - the given segment is not already contained.
  //
  STDMETHOD (RemoveAlternateSegment) (
    // Segment to remove 
    /*[in]*/ IAAFSegment * pSegment);


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

#endif // ! __CAAFSelector_h__


