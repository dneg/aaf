//@doc
//@class    AAFSourceReference | Implementation class for AAFSourceReference
#ifndef __CAAFSourceReference_h__
#define __CAAFSourceReference_h__

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





#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFSourceReference
  : public IAAFSourceReference,
    public IAAFSourceReference2,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSourceReference (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSourceReference ();

public:

  //***********************************************************
  //
  // GetSourceID()
  //
  // Gets the SourceID and places it into the pSourceID argument.
  ///
  /// Succeeds if all of the following are true:
  /// - the pSourceID pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pSourceID.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSourceID arg is NULL.
  //
  STDMETHOD (GetSourceID) (
    // Place to put source ID 
    /*[retval][out]*/ aafMobID_t *  pSourceID);


  //***********************************************************
  //
  // SetSourceID()
  //
  // Sets the SourceID using the sourceID argument.
  /// 
  /// Always succeeds.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetSourceID) (
    // Source ID to set 
    /*[in]*/ aafMobID_constref   sourceID);


  //***********************************************************
  //
  // GetSourceMobSlotID()
  //
  // Gets the Mob Slot ID and places it into the pMobSlotID argument.
  ///
  /// Succeeds if all of the following are true:
  /// - the pMobSlotID pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pMobSlotID.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMobSlotID arg is NULL.
  //
  STDMETHOD (GetSourceMobSlotID) (
    // Place to put source mob slot ID 
    /*[retval][out]*/ aafSlotID_t *  pMobSlotID);


  //***********************************************************
  //
  // SetSourceMobSlotID()
  //
  // Sets the mob slot ID using the mobSlotID argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - (preconditions here)
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// (other error codes here.)
  //
  STDMETHOD (SetSourceMobSlotID) (
    // Source Mob ID to set 
    /*[in]*/ aafSlotID_t   mobSlotID);


  //***********************************************************
  // METHOD NAME: SetChannelIDs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | SetChannelIDs |
  // Specify the channels in a slot that are referenced. The first channel has
/// and ID of 1, the N'th channel has an ID of N.  The number of channel IDs
/// shall equal the number of channels being described the bht MobSlot containing
/// the SourceReference, e.g. 1 element for a mono audio slot, 6 elements for a 5.1
/// multi-channel audio slot.
/// 
/// Return codes:
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pChannelIDs is null 
  // @end
  // 
  STDMETHOD (SetChannelIDs)
   (
    // @parm [in] aafUInt32 | numberElements | Number of elements in the pChannelIDs array
    aafUInt32  numberElements,

    // @parm [in] aafUInt32* | pChannelIDs | Array of channel IDs
    aafUInt32*  pChannelIDs
  );

  //***********************************************************
  // METHOD NAME: GetChannelIDs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | GetChannelIDs |
  // /// Get the channels in a slot that are referenced.  Refer to
/// SetChannelIDs for channel IDs description.
/// Return codes:
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pChannelIDs is null
///
/// AAFRESULT_PROP_NOT_PRESENT
///   - the property is not present
///
/// AAFRESULT_SMALLBUF
///   - pChannelIDs is too small
  // @end
  // 
  STDMETHOD (GetChannelIDs)
   (
    // @parm [in] aafUInt32 | numberElements | Number of elements in the pChannelIDs array
    aafUInt32  numberElements,

    // @parm [in] aafUInt32* | pChannelIDs | Array of channel IDs
    aafUInt32*  pChannelIDs
  );

  //***********************************************************
  // METHOD NAME: GetChannelIDsSize()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | GetChannelIDsSize |
  // /// Get the number of channel IDs stored by this SourceReference.
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pChannelIDs is null
  // @end
  // 
  STDMETHOD (GetChannelIDsSize)
   (
    // @parm [out] aafUInt32 * | numberElements | Number of elements in the pChannelIDs array
    aafUInt32 *  numberElements
  );

  //***********************************************************
  // METHOD NAME: SetMonoSourceSlotIDs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | SetMonoSourceSlotIDs |
  // For reference from a multi-channel MobSlot to multiple mono MobSlots.
/// pMonoSourceSlotIDs identifies the mono slots referenced by this SourceReference
/// object.

/// 
/// Return codes:
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pMonoSourceSlotIDs is null 
  // @end
  // 
  STDMETHOD (SetMonoSourceSlotIDs)
   (
    // @parm [in] aafUInt32 | numberElements | Number of elements in the pMonoSourceSlotIDs array
    aafUInt32  numberElements,

    // @parm [in] aafUInt32* | pMonoSourceSlotIDs | Array of slot IDs
    aafUInt32*  pMonoSourceSlotIDs
  );

  //***********************************************************
  // METHOD NAME: GetMonoSourceSlotIDs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | GetMonoSourceSlotIDs |
  // /// Get the mono slot IDs that are referenced by this object.  Refer to
/// SetMonoSourceSlotIDs for a description of pMonoSourceSlotIDs.
/// Return codes:
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pMonoSourceSlotIDs is null
///
/// AAFRESULT_PROP_NOT_PRESENT
///   - the property is not present
///
/// AAFRESULT_SMALLBUF
///   - pMonoSourceSlotIDs is too small
  // @end
  // 
  STDMETHOD (GetMonoSourceSlotIDs)
   (
    // @parm [in] aafUInt32 | numberElements | Number of elements in the pMonoSourceSlotIDs array
    aafUInt32  numberElements,

    // @parm [in] aafUInt32* | pMonoSourceSlotIDs | Array of channel IDs
    aafUInt32*  pMonoSourceSlotIDs
  );

  //***********************************************************
  // METHOD NAME: GetMonoSourceSlotIDsSize()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSourceReference2 | GetMonoSourceSlotIDsSize |
  // /// Get the number of mono slot IDs stored by this SourceReference.
///
/// AAFRESULT_SUCCESS
///   - succeeded
///
/// AAFRESULT_NULL_PARAM
///   - pMonoSourceSlotIDs is null
  // @end
  // 
  STDMETHOD (GetMonoSourceSlotIDsSize)
   (
    // @parm [out] aafUInt32 * | numberElements | Number of elements in the pMonoSourceSlotIDs array
    aafUInt32 *  numberElements
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

#endif // ! __CAAFSourceReference_h__

