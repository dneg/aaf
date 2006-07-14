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
  // CountSourceTrackIDs()
  //
  // Gets the total number of source track IDs present in
  /// the list of source track IDs referenced by this DescriptiveClip.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid;
  /// - the SourceTrackIDs property is present.
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
  ///   - the SourceTrackIDs property is not present.
  //
  STDMETHOD (CountSourceTrackIDs) (
    // Number of source track IDs 
    /*[out, retval]*/ aafUInt32*  pCount);


  //***********************************************************
  //
  // GetSourceTrackIDs()
  //
  // Gets all the source track IDs in the list of
  /// source track IDs referenced by this DescriptiveClip.
  ///
  /// The values are written to the array specified by pSourceTrackIDs,
  /// which is of size maxSourceTrackIDCount. The required size may be found
  /// by calling CountSourceTrackIDs().
  /// 
  /// Succeeds if all of the following are true:
  /// - pSourceTrackIDs is a valid pointer;
  /// - maxSourceTrackIDCount indicates the array is large enough to hold the
  ///   data;
  /// - the SourceTrackIDs property is present.
  /// 
  /// If this method fails, the property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSourceTrackIDs is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - maxSourceTrackIDCount indicates that the array is too small to hold
  ///     all the referenced source track IDs.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the SourceTrackIDs property is not present.
  //
  STDMETHOD (GetSourceTrackIDs) (
    // The size of the given pSourceTrackIDs buffer
    /*[in]*/ aafUInt32  maxSourceTrackIDCount,

    // Array to hold the source track IDs
    /*[out, size_is(maxSourceTrackIDsCount)]*/ aafUInt32 *  pSourceTrackIDs);


  //***********************************************************
  //
  // IsSourceTrackIDPresent()
  //
  // Determines if the given source track ID is present in
  /// the list of source track IDs referenced by this DescriptiveClip.
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - the pIsPresent pointer is valid;
  /// - the SourceTrackIDs property is present.
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
  ///   - the SourceTrackIDs property is not present.
  //
  STDMETHOD (IsSourceTrackIDPresent) (
    // Source track ID whose presence is to be queried
    /*[in, ref]*/ aafUInt32  sourceTrackID,

    // True if sourceTrackID is present
    /*[out,retval]*/ aafBoolean_t*  pIsPresent);


  //***********************************************************
  //
  // AddSourceTrackID()
  //
  // Appends the given source track ID to the list of
  /// source track IDs referenced by this DescriptiveClip.
  ///
  /// Succeeds if all of the following are true:
  /// - the given source track ID is not already contained in the list
  ///   of source track IDs referenced by this DescriptiveClip.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given source track ID is already contained in the list
  ///     of source track IDs referenced by this DescriptiveClip.
  //
  STDMETHOD (AddSourceTrackID) (
    // Source track ID to add. 
    /*[in]*/ aafUInt32  sourceTrackID);


  //***********************************************************
  //
  // RemoveSourceTrackID()
  //
  // Removes the given source track ID from
  /// the list of source track IDs referenced by this DescriptiveClip.
  ///
  /// If the removed ID was the last ID in the list, the list is removed
  /// from this DescriptiveClip (the property is removed).
  ///
  /// Succeeds if all of the following are true:
  /// - the SourceTrackIDs property is present;
  /// - the given source track ID is present in the list
  ///   of source track IDs referenced by this DescriptiveClip.
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
  ///   - The given source track ID is not present in the list
  ///     of source track IDs referenced by this DescriptiveClip.
  //
  STDMETHOD (RemoveSourceTrackID) (
    // Source track ID to remove. 
    /*[in]*/ aafUInt32  sourceTrackID);



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


