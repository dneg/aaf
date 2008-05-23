//@doc
//@class    AAFDescriptiveClip | Implementation class for AAFDescriptiveClip
#ifndef __CAAFDescriptiveClip_h__
#define __CAAFDescriptiveClip_h__

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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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







#ifndef __CAAFSourceClip_h__
#include "CAAFSourceClip.h"
#endif


class CAAFDescriptiveClip
  : public IAAFDescriptiveClip,
    public CAAFSourceClip
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDescriptiveClip (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDescriptiveClip ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // This method initializes a source clip object with
  /// the given properties.  Only required properties are set.
  /// Optional properties are added with separate functions.
  ///
  /// Succeds if:
  /// - This object has not already been Initialize()d.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ALREADY_INITIALIZED
  ///   - Initialize() has already been called on this object.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDataDef argument is NULL.
  //
  STDMETHOD (Initialize) (
    // Data Definition object
    /*[in]*/ IAAFDataDef * pDataDef,

    // Length property value
    /*[in]*/ aafLength_constref  length,

    // Source Reference
    /*[in]*/ aafSourceRef_t  sourceRef);


  //***********************************************************
  //
  // CountDescribedSlotIDs()
  //
  // Gets the total number of described slot IDs present in
  /// the list of described slot IDs referenced by this DescriptiveClip.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid;
  /// - the DescribedSlotIDs property is present.
  ///
  /// If this method fails nothing will be written to *pCount.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the DescribedSlotIDs property is not present.
  //
  STDMETHOD (CountDescribedSlotIDs) (
    // Number of described slot IDs 
    /*[out, retval]*/ aafUInt32*  pCount);


  //***********************************************************
  //
  // GetDescribedSlotIDs()
  //
  // Gets all the described slot IDs in the list of
  /// described slot IDs referenced by this DescriptiveClip.
  ///
  /// The values are written to the array specified by pDescribedSlotIDs,
  /// which is of size maxDescribedSlotIDCount. The required size may be found
  /// by calling CountDescribedSlotIDs().
  /// 
  /// Succeeds if all of the following are true:
  /// - pDescribedSlotIDs is a valid pointer;
  /// - maxDescribedSlotIDCount indicates the array is large enough to hold the
  ///   data;
  /// - the DescribedSlotIDs property is present.
  /// 
  /// If this method fails, the property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescribedSlotIDs is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - maxDescribedSlotIDCount indicates that the array is too small to hold
  ///     all the referenced described slot IDs.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the DescribedSlotIDs property is not present.
  //
  STDMETHOD (GetDescribedSlotIDs) (
    // The size of the given pDescribedSlotIDs buffer
    /*[in]*/ aafUInt32  maxDescribedSlotIDCount,

    // Array to hold the described slot IDs
    /*[out, size_is(maxDescribedSlotIDCount)]*/ aafUInt32 *  pDescribedSlotIDs);


  //***********************************************************
  //
  // IsDescribedSlotIDPresent()
  //
  // Determines if the given described slot ID is present in
  /// the list of described slot IDs referenced by this DescriptiveClip.
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - the pIsPresent pointer is valid;
  /// - the DescribedSlotIDs property is present.
  ///
  /// If this method fails nothing will be written to *pIsPresent.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIsPresent arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the DescribedSlotIDs property is not present.
  //
  STDMETHOD (IsDescribedSlotIDPresent) (
    // described slot ID whose presence is to be queried
    /*[in]*/ aafUInt32  DescribedSlotID,

    // True if DescribedSlotID is present
    /*[out,retval]*/ aafBoolean_t*  pIsPresent);


  //***********************************************************
  //
  // AddDescribedSlotID()
  //
  // Appends the given described slot ID to the list of
  /// described slot IDs referenced by this DescriptiveClip.
  ///
  /// Succeeds if all of the following are true:
  /// - the given described slot ID is not already contained in the list
  ///   of described slot IDs referenced by this DescriptiveClip.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given described slot ID is already contained in the list
  ///     of described slot IDs referenced by this DescriptiveClip.
  //
  STDMETHOD (AddDescribedSlotID) (
    // described slot ID to add. 
    /*[in]*/ aafUInt32  DescribedSlotID);


  //***********************************************************
  //
  // RemoveDescribedSlotID()
  //
  // Removes the given described slot ID from
  /// the list of described slot IDs referenced by this DescriptiveClip.
  ///
  /// If the removed ID was the last ID in the list, the list is removed
  /// from this DescriptiveClip (the property is removed).
  ///
  /// Succeeds if all of the following are true:
  /// - the DescribedSlotIDs property is present;
  /// - the given described slot ID is present in the list
  ///   of described slot IDs referenced by this DescriptiveClip.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given described slot ID is not present in the list
  ///     of described slot IDs referenced by this DescriptiveClip.
  //
  STDMETHOD (RemoveDescribedSlotID) (
    // described slot ID to remove. 
    /*[in]*/ aafUInt32  DescribedSlotID);



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

#endif // ! __CAAFDescriptiveClip_h__


