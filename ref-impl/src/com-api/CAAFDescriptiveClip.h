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
  // SetSourceTrackIDs()
  //
  // Specifies the Source Track IDs in the Mob.
  /// 
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSourceTrackIDs is null 
  ///
  /// This interface is not currently implemented.
  //
  STDMETHOD (SetSourceTrackIDs) (
    // Number of elements in the pSourceTrackIDs array
    /*[in]*/ aafUInt32  numberElements,

    // Array of source track IDs
    /*[in]*/ aafUInt32*  pSourceTrackIDs);


  //***********************************************************
  //
  // GetSourceTrackIDs()
  //
  // Get the source track IDs that are referenced by this object. 
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSourceTrackIDs is null
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present
  ///
  /// AAFRESULT_SMALLBUF
  ///   - pSourceTrackIDs is too small
  ///
  /// This interface is not currently implemented.
  //
  STDMETHOD (GetSourceTrackIDs) (
    // Number of elements in the pSourceTrackIDs array
    /*[in]*/ aafUInt32  numberElements,

    // Array of channel IDs
    /*[in]*/ aafUInt32*  pSourceTrackIDs);

  //***********************************************************
  //
  // GetSourceTrackIDsSize()
  //
  // Get the number of source track IDs stored by this DescriptiveClip.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSourceTrackIDs is null 
  //
  STDMETHOD (GetSourceTrackIDsSize) (
    // Number of elements in the pSourceTrackIDs array 
    /*[out]*/ aafUInt32 *  numberElements);


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


