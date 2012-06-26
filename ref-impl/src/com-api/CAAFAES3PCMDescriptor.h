//@doc
//@class    AAFAES3PCMDescriptor | Implementation class for AAFAES3PCMDescriptor
#ifndef __CAAFAES3PCMDescriptor_h__
#define __CAAFAES3PCMDescriptor_h__

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
// The Original Code of this file is Copyright 1998-2012, licensor of the
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





#ifndef __CAAFPCMDescriptor_h__
#include "CAAFPCMDescriptor.h"
#endif


class CAAFAES3PCMDescriptor
  : public IAAFAES3PCMDescriptor,
    public IAAFAES3PCMDescriptor2,
    public CAAFPCMDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFAES3PCMDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFAES3PCMDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, IAAFAES3PCMDescriptor-supporting
  /// object. This method must be called after allocation, and before
  /// any other method can be called.
  ///
  /// Succeeds if:
  /// - Initialize() has not yet been called on this object.
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
  //
  STDMETHOD (Initialize)
     ();


  //***********************************************************
  // METHOD NAME: GetEmphasis()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetEmphasis |
  // Gets the Emphasis type property.
  /// Succeeds if all of the following are true:
  /// - pEmphasisType is a valid pointer.
  ///
  /// If this method fails, *pEmphasisType will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEmphasisType arg is NULL.
  // @end
  // 
  STDMETHOD (GetEmphasis)
   (
    // @parm [out] aafEmphasisType_t * | pEmphasisType | Address to store the emphasis type.
    aafEmphasisType_t *  pEmphasisType
  );

  //***********************************************************
  // METHOD NAME: SetEmphasis()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetEmphasis |
  // Gets the Emphasis type property.
  /// Succeeds if all of the following are true:
  /// 
  ///
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  // @end
  // 
  STDMETHOD (SetEmphasis)
   (
    // @parm [in] aafEmphasisType_t | EmphasisType | Sets the Emphasis type property.
    aafEmphasisType_t  EmphasisType
  );

  //***********************************************************
  // METHOD NAME: GetBlockStartOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetBlockStartOffset |
  // Gets the BlockStartOffset.
  /// Succeeds if all of the following are true:
  /// - pBlockStartOffset is a valid pointer.
  ///
  /// If this method fails, *pBlockStartOffset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBlockStartOffset arg is NULL.
  // @end
  // 
  STDMETHOD (GetBlockStartOffset)
   (
    // @parm [out] aafUInt16 * | pBlockStartOffset | Address to store the BlockStartOffset.
    aafUInt16 *  pBlockStartOffset
  );

  //***********************************************************
  // METHOD NAME: SetBlockStartOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetBlockStartOffset |
  // Sets the BlockStartOffset.
  /// Succeeds if all of the following are true:
  /// 
  ///
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  // @end
  // 
  STDMETHOD (SetBlockStartOffset)
   (
    // @parm [in] aafUInt16 | BlockStartOffset | Sets the BlockStartOffset.
    aafUInt16  BlockStartOffset
  );

  //***********************************************************
  // METHOD NAME: GetAuxBitsMode()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetAuxBitsMode |
  // Gets the AuxBitsMode.
  /// Succeeds if all of the following are true:
  /// - pAuxBitsMode is a valid pointer.
  ///
  /// If this method fails, *pAuxBitsMode will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pAuxBitsMode arg is NULL.
  // @end
  // 
  STDMETHOD (GetAuxBitsMode)
   (
    // @parm [out] aafAuxBitsModeType_t * | pAuxBitsMode | Address to store the AuxBitsMode.
    aafAuxBitsModeType_t *  pAuxBitsMode
  );

  //***********************************************************
  // METHOD NAME: SetAuxBitsMode()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetAuxBitsMode |
  // Sets the AuxBitsMode.
  /// Succeeds if all of the following are true:
  /// 
  ///
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  // @end
  // 
  STDMETHOD (SetAuxBitsMode)
   (
    // @parm [in] aafAuxBitsModeType_t | AuxBitsMode | Sets the AuxBitsMode.
    aafAuxBitsModeType_t  AuxBitsMode
  );

  //***********************************************************
  // METHOD NAME: GetChannelStatusModeAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetChannelStatusModeAt |
  // Gets the ChannelStatusMode for the channel referenced by the index
	/// Succeeds if all of the following are true:
	///	
	/// - pChannelStatusMode is a valid pointer
	/// - index is a valid index with the array of channels
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pChannelStatusMode arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
  // @end
  // 
  STDMETHOD (GetChannelStatusModeAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [out] aafChannelStatusModeType_t * | pChannelStatusMode | Address to store ChannelStatusMode for the index given.
    aafChannelStatusModeType_t *  pChannelStatusMode
  );

  //***********************************************************
  // METHOD NAME: SetChannelStatusModeAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetChannelStatusModeAt |
  // Sets the ChannelStatusMode for the channel referenced by the index
	/// Succeeds if all of the following are true:
	///	
	/// - index is a valid index with the array of channels
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
  // @end
  // 
  STDMETHOD (SetChannelStatusModeAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafChannelStatusModeType_t | ChannelStatusMode | Set ChannelStatusMode for the index given.
    aafChannelStatusModeType_t  ChannelStatusMode
  );

  //***********************************************************
  // METHOD NAME: GetFixedChannelStatusDataAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetFixedChannelStatusDataAt |
  // Gets the FixedChannelStatusData for the channel referenced by the index
	///		each channel's status is a 24 byte array that gets copied into the 
	///		the address pointed to by pFixedChannelStatusData.
	/// Succeeds if all of the following are true:
	///	
	/// - pFixedChannelStatusData is a valid pointer
	/// - index is a valid index with the array of channels
	/// - sizeOfBuffer must be 24
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pFixedChannelStatusData arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
	///
	/// AAFRESULT_WRONG_SIZE
	///  - buffer size is not correct
  // @end
  // 
  STDMETHOD (GetFixedChannelStatusDataAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafUInt32 | sizeOfBuffer | Size of pFixedChannelStatusData buffer in bytes.
    aafUInt32  sizeOfBuffer,

    // @parm [out] aafUInt8 * | pFixedChannelStatusData | Address to store FixedChannelStatusData for the index given.
    aafUInt8 *  pFixedChannelStatusData
  );

  //***********************************************************
  // METHOD NAME: SetFixedChannelStatusDataAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetFixedChannelStatusDataAt |
  // Sets the FixedChannelStatusData for the channel referenced by the index
	///		each channel's status is a 24 byte array that gets copied from the 
	///		the address pointed to by pFixedChannelStatusData .
	/// Succeeds if all of the following are true:
	///	
	/// - pFixedChannelStatusData is a valid pointer
	/// - index is a valid index with the array of channels
	/// - sizeOfBuffer must be 24
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pFixedChannelStatusData arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
	///
	/// AAFRESULT_WRONG_SIZE
	///  - buffer size is not correct
  // @end
  // 
  STDMETHOD (SetFixedChannelStatusDataAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafUInt32 | sizeOfBuffer | Size of pFixedChannelStatusData buffer in bytes.
    aafUInt32  sizeOfBuffer,

    // @parm [in] aafUInt8 * | pFixedChannelStatusData | Set FixedChannelStatusData for the index given.
    aafUInt8 *  pFixedChannelStatusData
  );

  //***********************************************************
  // METHOD NAME: GetUserDataModeAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetUserDataModeAt |
  // Gets the UserDataMode for the channel referenced by the index
	/// Succeeds if all of the following are true:
	///	
	/// - pUserDataMode is a valid pointer
	/// - index is a valid index with the array of channels
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pUserDataMode arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
  // @end
  // 
  STDMETHOD (GetUserDataModeAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [out] aafUserDataModeType_t * | pUserDataMode | Address to store UserDataMode for the index given.
    aafUserDataModeType_t *  pUserDataMode
  );

  //***********************************************************
  // METHOD NAME: SetUserDataModeAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetUserDataModeAt |
  // Sets the UserDataMode for the channel referenced by the index
	/// Succeeds if all of the following are true:
	///	
	/// - index is a valid index with the array of channels
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
  // @end
  // 
  STDMETHOD (SetUserDataModeAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafUserDataModeType_t | UserDataMode | Set UserDataMode for the index given.
    aafUserDataModeType_t  UserDataMode
  );

  //***********************************************************
  // METHOD NAME: GetFixedUserDataAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | GetFixedUserDataAt |
  // Gets the FixedUserData for the channel referenced by the index
	///		each channel's status is a 24 byte array that gets copied into the 
	///		the address pointed to by pFixedUserData.
	/// Succeeds if all of the following are true:
	///	
	/// - pFixedUserData is a valid pointer
	/// - index is a valid index with the array of channels
	/// - sizeOfBuffer must be 24
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pFixedUserData arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
	///
	/// AAFRESULT_WRONG_SIZE
	///  - buffer size is not correct
  // @end
  // 
  STDMETHOD (GetFixedUserDataAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafUInt32 | sizeOfBuffer | Size of pFixedUserData buffer in bytes.
    aafUInt32  sizeOfBuffer,

    // @parm [out] aafUInt8 * | pFixedUserData | Address to store FixedUserData for the index given.
    aafUInt8 *  pFixedUserData
  );

  //***********************************************************
  // METHOD NAME: SetFixedUserDataAt()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFAES3PCMDescriptor2 | SetFixedUserDataAt |
  // Sets the FixedUserData for the channel referenced by the index
	///		each channel's status is a 24 byte array that gets copied from the 
	///		the address pointed to by pFixedUserData .
	/// Succeeds if all of the following are true:
	///	
	/// - pFixedUserData is a valid pointer
	/// - index is a valid index with the array of channels
	/// - sizeOfBuffer must be 24
	///
	/// this method will return the following codes
	///
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NOT_INITIALIZED
	///   - This object has not yet had Initialize() called on it.
	///
	/// AAFRESULT_NULL_PARAM
	///	 - pFixedUserData arg is NULL
	///
	///	AAFRESULT_BADINDEX
	///	 - index is not a valid index
	///
	/// AAFRESULT_WRONG_SIZE
	///  - buffer size is not correct
  // @end
  // 
  STDMETHOD (SetFixedUserDataAt)
   (
    // @parm [in] aafUInt32 | index | Index of the channel desired.
    aafUInt32  index,

    // @parm [in] aafUInt32 | sizeOfBuffer | Size of pFixedUserData buffer in bytes.
    aafUInt32  sizeOfBuffer,

    // @parm [in] aafUInt8 * | pFixedUserData | Set FixedUserData for the index given.
    aafUInt8 *  pFixedUserData
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

#endif // ! __CAAFAES3PCMDescriptor_h__

