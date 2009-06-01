//@doc
//@class    AAFDescriptiveMarker | Implementation class for AAFDescriptiveMarker
#ifndef __CAAFDescriptiveMarker_h__
#define __CAAFDescriptiveMarker_h__

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










#ifndef __CAAFCommentMarker_h__
#include "CAAFCommentMarker.h"
#endif


class CAAFDescriptiveMarker
  : public IAAFDescriptiveMarker,
    public CAAFCommentMarker
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDescriptiveMarker (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDescriptiveMarker ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated,
  /// IAAFDescriptiveFrame object.  This method must be called
  /// after allocation, and before any other method can be called.
  ///
  /// Return codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ALREADY_INITIALIZED
  ///   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // SetDescribedSlotIDs()
  //
  // Specifies the slot IDs in the Mob to which the DescriptiveFramework refers.
  /// 
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMonoSourceSlotIDs is null 
  ///
  /// This interface is not currently implemented.
  //
  STDMETHOD (SetDescribedSlotIDs) (
    // Number of elements in the pDescribedSlotIDs array
    /*[in]*/ aafUInt32  numberElements,

    // Array of slot IDs
    /*[in]*/ aafUInt32*  pDescribedSlotIDs);


  //***********************************************************
  //
  // GetDescribedSlotIDs()
  //
  // Get the described slot IDs that are referenced by this object.  Refer to
  /// SetDescribedSlotIDs for a description of pDescribedSlotIDs.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescribedSlotIDs is null
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present
  ///
  /// AAFRESULT_SMALLBUF
  ///   - pDescribedSlotIDs is too small
  ///
  /// This interface is not currently implemented.
  //
  STDMETHOD (GetDescribedSlotIDs) (
    // Number of elements in the pDescribedSlotIDs array
    /*[in]*/ aafUInt32  numberElements,

    // Array of channel IDs
    /*[in]*/ aafUInt32*  pDescribedSlotIDs);

  //***********************************************************
  //
  // GetDescribedSlotIDsSize()
  //
  // Get the number of slot IDs stored by this DescriptiveMarker.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescribedSlotIDs is null 
  //
  STDMETHOD (GetDescribedSlotIDsSize) (
    // Number of elements in the pDescribedSlotIDs array 
    /*[out]*/ aafUInt32 *  numberElements);

  //***********************************************************
  //
  // SetDescriptiveFramework()
  //
  // Sets the DescriptiveFramework that describes this DescriptiveMarker.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///	- succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///	- pDescriptorFramework is null
  //
  STDMETHOD (SetDescriptiveFramework) (
    // DescriptorFramework object to set 
    /*[in]*/ IAAFDescriptiveFramework * pDescriptiveFramework);

  //***********************************************************
  //
  // GetDescriptiveFramework()
  //
  // Gets the DescriptiveFramework that describes this DescriptiveMarker.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///	- succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///	- ppDescriptorFramework is null
  //
  STDMETHOD (GetDescriptiveFramework) (
    // DescriptorFramework object to return 
    /*[out,retval]*/ IAAFDescriptiveFramework ** ppDescriptiveFramework);


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

#endif // ! __CAAFDescriptiveMarker_h__


