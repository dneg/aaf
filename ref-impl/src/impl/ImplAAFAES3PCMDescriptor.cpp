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

#include "ImplAAFAES3PCMDescriptor.h"
#include "AAFPropertyIDs.h"

const aafUInt32 SizeOfFixedChannelStatusData = 24;
const aafUInt32 SizeOfFixedUserData = 24;

ImplAAFAES3PCMDescriptor::ImplAAFAES3PCMDescriptor() :
	_emphasis(PID_AES3PCMDescriptor_Emphasis, L"Emphasis"),
	_blockStartOffset(PID_AES3PCMDescriptor_BlockStartOffset, 
					  L"BlockStartOffset"),
	_auxBitsMode(PID_AES3PCMDescriptor_AuxBitsMode, L"AuxBitsMode"),
	_channelStatusModeArray(PID_AES3PCMDescriptor_ChannelStatusMode,
							L"ChannelStatusMode"),
	_fixedChannelStatusDataArray(PID_AES3PCMDescriptor_FixedChannelStatusData,
								 L"FixedChannelStatusData"),
	_userDataModeArray(PID_AES3PCMDescriptor_UserDataMode,
					   L"UserDataMode"),
	_fixedUserDataArray(PID_AES3PCMDescriptor_FixedUserData,
						L"FixedUserData")
{
	_persistentProperties.put(_emphasis.address());
	_persistentProperties.put(_blockStartOffset.address());
	_persistentProperties.put(_auxBitsMode.address());
	_persistentProperties.put(_channelStatusModeArray.address());
	_persistentProperties.put(_fixedChannelStatusDataArray.address());
	_persistentProperties.put(_userDataModeArray.address());
	_persistentProperties.put(_fixedUserDataArray.address());
}



ImplAAFAES3PCMDescriptor::~ImplAAFAES3PCMDescriptor ()
{
}



AAFRESULT STDMETHODCALLTYPE ImplAAFAES3PCMDescriptor::Initialize()
{
    if(isInitialized())
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }


    // Call parent class' Initialize.
    AAFRESULT  ar = ImplAAFPCMDescriptor::Initialize();

    // Initialize this class required properties
    // and set the object initialized.
    if(ar == AAFRESULT_SUCCESS)
    {
        setInitialized();
    }


    return ar;
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFAES3PCMDescriptor::GetValue(T *pValueStorage, const U &storage) const
{
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if( !storage.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}

	if(pValueStorage == NULL) {
		return(AAFRESULT_NULL_PARAM);
	}

	*pValueStorage = storage;

	return(AAFRESULT_SUCCESS);
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::SetValue(const T &value, U &storage)
{
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	storage = value;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetEmphasis(aafEmphasisType_t *pEmphasisType)
{
	return(GetValue<aafEmphasisType_t, OMFixedSizeProperty<aafEmphasisType_t> >
					(pEmphasisType, _emphasis));
}

AAFRESULT STDMETHODCALLTYPE 
  ImplAAFAES3PCMDescriptor::SetEmphasis(aafEmphasisType_t EmphasisType)
{
	return(SetValue<aafEmphasisType_t, OMFixedSizeProperty<aafEmphasisType_t> >
					(EmphasisType, _emphasis));
}

AAFRESULT STDMETHODCALLTYPE 
  ImplAAFAES3PCMDescriptor::GetBlockStartOffset(aafUInt16 *pBlockStartOffset)
{
	return(GetValue<aafUInt16, OMFixedSizeProperty<aafUInt16> >
					(pBlockStartOffset, _blockStartOffset));
}

AAFRESULT STDMETHODCALLTYPE 
  ImplAAFAES3PCMDescriptor::SetBlockStartOffset(aafUInt16 BlockStartOffset)
{
	return(SetValue<aafUInt16, OMFixedSizeProperty<aafUInt16> >
					(BlockStartOffset, _blockStartOffset));
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetAuxBitsMode(aafAuxBitsModeType_t * pAuxBitsMode)
{
	return(GetValue<aafAuxBitsModeType_t, 
					OMFixedSizeProperty<aafAuxBitsModeType_t> >
					(pAuxBitsMode, _auxBitsMode));
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::SetAuxBitsMode(aafAuxBitsModeType_t AuxBitsMode)
{
	return(SetValue<aafAuxBitsModeType_t, 
					OMFixedSizeProperty<aafAuxBitsModeType_t> >
					(AuxBitsMode, _auxBitsMode));
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::ExpandArray(aafUInt32 newMinimumSize,
									  T value, U & array)
{
	if((newMinimumSize * sizeof(T)) > OMPROPERTYSIZE_MAX) {
		return(AAFRESULT_BAD_SIZE);
	}
	for(aafUInt32 count = array.count(); count < newMinimumSize; ++count) {
		array.append(value);
	}
	return(AAFRESULT_SUCCESS);
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFAES3PCMDescriptor::GetModeAt(aafUInt32 index, T * pMode,
									  const U & array)
{
	TRACE("ImplAAFAES3PCMDescriptor::GetModeAt");
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if( !array.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}

	aafUInt32 currChannelCount;
	GetChannelCount(&currChannelCount);
	if((index >= currChannelCount) || (index >= array.count()))
	{
		return(AAFRESULT_BADINDEX);
	}
	if(pMode == NULL) {
		return(AAFRESULT_NULL_PARAM);
	}
	array.getValueAt(pMode, index);
	return(AAFRESULT_SUCCESS);
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE
  ImplAAFAES3PCMDescriptor::SetModeAt(aafUInt32 index, T Mode,
									  U & array, const T & defValue)
{
	TRACE("ImplAAFAES3PCMDescriptor::SetModeAt");
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	aafUInt32 currChannelCount;
	GetChannelCount(&currChannelCount);
	if((index >= currChannelCount))
	{
		return(AAFRESULT_BADINDEX);
	}
	AAFRESULT res = AAFRESULT_SUCCESS;
	if(index >= array.count()) {
		res = ExpandArray<T, U>(index + 1, defValue, array);
	}
	if(res == AAFRESULT_SUCCESS) {
		array.setAt(Mode, index);
	}
	return(res);
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetChannelStatusModeAt(aafUInt32 index,
						aafChannelStatusModeType_t * pChannelStatusMode)
{
	return(GetModeAt<aafChannelStatusModeType_t, 
						  OMArrayProperty<aafChannelStatusModeType_t> > 
						  (index, pChannelStatusMode, 
						   _channelStatusModeArray));
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::SetChannelStatusModeAt(aafUInt32 index,
								aafChannelStatusModeType_t ChannelStatusMode)
{
	return(SetModeAt<aafChannelStatusModeType_t,
						  OMArrayProperty<aafChannelStatusModeType_t> >
						  (index, ChannelStatusMode, _channelStatusModeArray,
						   kAAFChannelStatusMode_None));
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetUserDataModeAt(aafUInt32 index,
										aafUserDataModeType_t * pUserDataMode)
{
	return(GetModeAt<aafUserDataModeType_t, 
						  OMArrayProperty<aafUserDataModeType_t> > 
						 (index, pUserDataMode, _userDataModeArray));
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::SetUserDataModeAt(aafUInt32 index,
										aafUserDataModeType_t UserDataMode)
{
	return(SetModeAt<aafUserDataModeType_t,
						  OMArrayProperty<aafUserDataModeType_t> >
						 (index, UserDataMode, _userDataModeArray,
						  kAAFUserDataMode_NotDefined));
}

template<class T, class U> AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::VerifyDataArraySize(aafUInt32 index, 
											  aafUInt32 SizeOfData, 
											  U &array, T * pData,
											  bool expand, const T defValue)
{
	TRACE("ImplAAFAES3PCMDescriptor::VerifyDataArraySize");
	aafUInt32 currChannelCount;
	GetChannelCount(&currChannelCount);
	aafUInt32 neededArraySize = (index + 1) * SizeOfData;
	if((index >= currChannelCount) || 
		(!expand && (neededArraySize > array.count())))
	{
		return(AAFRESULT_BADINDEX);
	}
	if(pData == NULL) {
		return(AAFRESULT_NULL_PARAM);
	}
	AAFRESULT res = AAFRESULT_SUCCESS;
	if(expand && (neededArraySize > array.count())) {
		res = ExpandArray<T, U>(neededArraySize, defValue, array);
	}
	return(res);
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetDataAt(aafUInt32 index,
										 T * pData, U & array,
										 aafUInt32 SizeOfData, 
										 aafUInt32 sizeOfBuffer)
{
	TRACE("ImplAAFAES3PCMDescriptor::GetDataAt");
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if( !array.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}

	if(SizeOfData != sizeOfBuffer) {
		return AAFRESULT_WRONG_SIZE;
	}

	AAFRESULT res = VerifyDataArraySize<T, U>(index, SizeOfData, array, pData);
	if(res != AAFRESULT_SUCCESS) {
		return(res);
	}
	aafUInt32 beginIndex = index * SizeOfData;
	aafUInt32 endIndex = (index + 1) * SizeOfData;
	for(aafUInt32 myIndex = beginIndex, outputIndex = 0;
		myIndex < endIndex;
		++myIndex, ++outputIndex)
	{
		array.getValueAt(&(pData[outputIndex]), myIndex);
	}
	
	return(AAFRESULT_SUCCESS);
}

template<class T, class U>
AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::SetDataAt(aafUInt32 index,
									T * pData, U & array,
									aafUInt32 SizeOfData, 
									aafUInt32 sizeOfBuffer, 
									const T & defValue)
{
	TRACE("ImplAAFAES3PCMDescriptor::SetDataAt");
	if( !isInitialized() )
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	AAFRESULT res = VerifyDataArraySize<T, U>(index, SizeOfData, array, pData,
											  true, defValue);
	if(res != AAFRESULT_SUCCESS) {
		return(res);
	}

	if(SizeOfData != sizeOfBuffer) {
		return AAFRESULT_WRONG_SIZE;
	}

	aafUInt32 beginIndex = index * SizeOfData;
	aafUInt32 endIndex = (index + 1) * SizeOfData;
	for(aafUInt32 myIndex = beginIndex, outputIndex = 0;
		myIndex < endIndex;
		++myIndex, ++outputIndex)
	{
		array.setAt(pData[outputIndex], myIndex);
	}
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetFixedChannelStatusDataAt(aafUInt32 index,
													  aafUInt32 sizeOfBuffer,
											aafUInt8 * pFixedChannelStatusData)
{
	TRACE("ImplAAFAES3PCMDescriptor::GetFixedChannelStatusDataAt");
	return(GetDataAt<aafUInt8, OMArrayProperty<aafUInt8> >
				(index, pFixedChannelStatusData, _fixedChannelStatusDataArray,
				 SizeOfFixedChannelStatusData, sizeOfBuffer));
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::SetFixedChannelStatusDataAt(aafUInt32 index,
													  aafUInt32 sizeOfBuffer,
											aafUInt8 * pFixedChannelStatusData)
{
	TRACE("ImplAAFAES3PCMDescriptor::SetFixedChannelStatusDataAt");
	return(SetDataAt<aafUInt8, OMArrayProperty<aafUInt8> >
				(index, pFixedChannelStatusData, _fixedChannelStatusDataArray,
				 SizeOfFixedChannelStatusData, sizeOfBuffer, 0));
}

AAFRESULT STDMETHODCALLTYPE 
ImplAAFAES3PCMDescriptor::GetFixedUserDataAt(aafUInt32 index,
											 aafUInt32 sizeOfBuffer,
											 aafUInt8 * pFixedUserData)
{
	TRACE("ImplAAFAES3PCMDescriptor::GetFixedUserDataAt");
	return(GetDataAt<aafUInt8, OMArrayProperty<aafUInt8> >
		(index, pFixedUserData, _fixedUserDataArray,
		SizeOfFixedUserData, sizeOfBuffer));
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFAES3PCMDescriptor::SetFixedUserDataAt(aafUInt32 index,
											 aafUInt32 sizeOfBuffer,
											 aafUInt8 * pFixedUserData)
{
	TRACE("ImplAAFAES3PCMDescriptor::SetFixedUserDataAt");
	return(SetDataAt<aafUInt8, OMArrayProperty<aafUInt8> >
		(index, pFixedUserData, _fixedUserDataArray,
		SizeOfFixedUserData, sizeOfBuffer, 0));
}
