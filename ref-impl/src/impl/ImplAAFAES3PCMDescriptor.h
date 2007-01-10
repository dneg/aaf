#ifndef __ImplAAFAES3PCMDescriptor_h__
#define __ImplAAFAES3PCMDescriptor_h__

//=---------------------------------------------------------------------=
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


#include "ImplAAFPCMDescriptor.h"
#include "OMArrayProperty.h"



class ImplAAFAES3PCMDescriptor : public ImplAAFPCMDescriptor
{
public:
  //****************
  // Constructor
  //
  ImplAAFAES3PCMDescriptor ();

protected:
  //****************
  // Desctructor
  //
  virtual ~ImplAAFAES3PCMDescriptor ();

public:
  //********************************
  // AAF SDK exported methods start
  //********************************

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();

  //****************
  // GetEmphasis()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEmphasis
		// @parm [out] Address to store the emphasis.
        (aafEmphasisType_t * pEmphasisType);

  //****************
  // SetEmphasis()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEmphasis
		// @parm [in] emphasis type.
        (aafEmphasisType_t EmphasisType);

  //****************
  // GetBlockStartOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBlockStartOffset
		// @parm [out] Address to store the BlockStartOffset.
        (aafUInt16 * pBlockStartOffset);

  //****************
  // SetBlockStartOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBlockStartOffset
		// @parm [in] BlockStartOffset.
        (aafUInt16 BlockStartOffset);

  //****************
  // GetAuxBitsMode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  GetAuxBitsMode
	  // @parm [out] Address to store the AuxBitsMode.
	  (aafAuxBitsModeType_t * pAuxBitsMode);

  //****************
  // SetAuxBitsMode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  SetAuxBitsMode
	  // @parm [in] AuxBitsMode.
	  (aafAuxBitsModeType_t AuxBitsMode);

  //****************
  // GetChannelStatusModeAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  GetChannelStatusModeAt
	  // @parm [out] Address to store the ChannelStatusMode.
	  (aafUInt32 index,
	  aafChannelStatusModeType_t * pChannelStatusMode);

  //****************
  // SetChannelStatusModeAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  SetChannelStatusModeAt
	  // @parm [in] ChannelStatusModeAt.
	  (aafUInt32 index,
	  aafChannelStatusModeType_t ChannelStatusMode);

  //****************
  // GetFixedChannelStatusData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  GetFixedChannelStatusDataAt
	  // @parm [out] Address to store the FixedChannelStatusData.
	  (aafUInt32 index,
	  aafUInt8 * pFixedChannelStatusData);

  //****************
  // SetFixedChannelStatusData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  SetFixedChannelStatusDataAt
	  // @parm [in] FixedChannelStatusData.
	  (aafUInt32 index,
	  aafUInt8 * pFixedChannelStatusData);

  //****************
  // GetUserDataModeAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetUserDataModeAt
  // @parm [out] Address to store the UserDataMode.
  (aafUInt32 index,
  aafUserDataModeType_t * pUserDataMode);

  //****************
  // SetUserDataModeAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetUserDataModeAt
  // @parm [in] UserDataModeAt.
  (aafUInt32 index,
  aafUserDataModeType_t UserDataMode);

  //****************
  // GetFixedUserData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  GetFixedUserDataAt
	  // @parm [out] Address to store the FixedUserData.
	  (aafUInt32 index,
	  aafUInt8 * pFixedUserData);

  //****************
  // SetFixedUserData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	  SetFixedUserDataAt
	  // @parm [in] FixedUserData.
	  (aafUInt32 index,
	  aafUInt8 * pFixedUserData);


private:

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE 
	GetValue(T *pValueStorage, const U & storage) const;

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE 
	SetValue(const T &value, U &storage);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE 
	ExpandArray(aafUInt32 newMinimumSize, T value, U & array);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE 
	GetModeAt(aafUInt32 index, T * pMode, const U & array);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE
	SetModeAt(aafUInt32 index, T Mode, U & array, const T & defValue);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE
	VerifyDataArraySize(aafUInt32 index, aafUInt32 SizeOfData, 
						U &array, T * pData,
						bool expand = false, const T defValue = 0);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE 
	GetDataAt(aafUInt32 index, T * pData, U & array,
				   aafUInt32 SizeOfData);

	template<class T, class U> AAFRESULT STDMETHODCALLTYPE
	SetDataAt(aafUInt32 index, T * pData, U & array, 
				   aafUInt32 SizeOfData, const T & defValue);

	
	OMFixedSizeProperty<aafEmphasisType_t>		_emphasis;
	OMFixedSizeProperty<aafUInt16>				_blockStartOffset;
	OMFixedSizeProperty<aafAuxBitsModeType_t>	_auxBitsMode;
	OMArrayProperty<aafChannelStatusModeType_t>	_channelStatusModeArray;
	OMArrayProperty<aafUInt8>				_fixedChannelStatusDataArray;
	OMArrayProperty<aafUserDataModeType_t>		_userDataModeArray;
	OMArrayProperty<aafUInt8>					_fixedUserDataArray;

};

#endif // ! __ImplAAFAES3PCMDescriptor_h__
