#ifndef __CAxEssenceCodec_h__
#define __CAxEssenceCodec_h__

//=---------------------------------------------------------------------=
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
//=---------------------------------------------------------------------=

#include <AAFResult.h>
#include <AAFPlugin.h>

#include <memory>

template <class T>
class CAxEssenceCodec : public IAAFEssenceCodec
{
public:

	CAxEssenceCodec( std::auto_ptr<T> impl )
	{}

	virtual ~CAxEssenceCodec()
	{}

	T& GetEssenceCodecImpl()
	{
		return *_impl;
	}

	//
	// IAAFEssenceCodec interface methods
	//

	STDMETHOD(SetEssenceAccess)(
		IAAFEssenceAccess * pEssenceAccess )
	{
		AX_PLUGIN_TRY( _impl, SetEssenceAccess, (pEssenceAccess) );
	}

	STDMETHOD(CountFlavours)(
		aafUInt32*  pCount )
	{
		AX_PLUGIN_TRY( _impl, CountFlavours, (pCount) );
	}

	STDMETHOD(GetIndexedFlavourID)(
		aafUInt32  index,
		aafUID_t *  pVariant )
	{
		AX_PLUGIN_TRY( _impl, GetIndexedFlavourID, (index, pVariant) );
	}
 
	STDMETHOD(CountDataDefinitions)(
		aafUInt32*  pCount )
	{
		AX_PLUGIN_TRY( _impl, CountDataDefinitions, (pCount) );
	}

	STDMETHOD(GetIndexedDataDefinition) (
		aafUInt32  index,
		aafUID_t *  pDataDefID )
	{
		AX_PLUGIN_TRY( _impl, GetIndexedDataDefinition, (index, pDataDefID) );
	}

	STDMETHOD(GetMaxCodecDisplayNameLength) (
		aafUInt32*  pBufSize )
	{
		AX_PLUGIN_TRY( _impl, GetMaxCodecDisplayNameLength, (pBufSize) );
	}		

	STDMETHOD(GetCodecDisplayName) (
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize )
	{
		AX_PLUGIN_TRY( _impl, GetCodecDisplayName, (flavour, pName, bufSize) );
	}
	
	STDMETHOD(CountChannels) (
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels )
	{	
		AX_PLUGIN_TRY( _impl, CountChannels, (fileMob, essenceKind, stream, pNumChannels) );
	}

	STDMETHOD(GetSelectInfo) (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo )
	{
		AX_PLUGIN_TRY( _impl, GetSelectInfo, (fileMob, stream, pSelectInfo) );
	}

	STDMETHOD(ValidateEssence) (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead)
	{
		AX_PLUGIN_TRY( _impl, ValidateEssence, (fileMob, stream, verbose, outputWarnings, bufSize, pErrorText, pBytesRead) );
	}

	STDMETHOD(Create) (
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
	{	
		AX_PLUGIN_TRY( _impl, Create, (fileMob, flavour, essenceKind, sampleRate, stream, compEnable) );
	}	

	STDMETHOD(Open) (
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
	{
		AX_PLUGIN_TRY( _impl, Open, (fileMob, openMode, stream, compEnable) );
	}
	
	STDMETHOD(CountSamples) (
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples)
	{
		AX_PLUGIN_TRY( _impl, CountSamples, (essenceKind, pNumSamples) );
	}

	STDMETHOD(WriteSamples) (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten)
	{
		AX_PLUGIN_TRY( _impl, WriteSamples, (nSamples, buflen, buffer, samplesWritten, bytesWritten) );
	}

	STDMETHOD(ReadSamples) (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead)
	{
		AX_PLUGIN_TRY( _impl, ReadSamples, (nSamples, buflen, buffer, samplesRead, bytesRead) );
	}

	STDMETHOD(Seek) (
		aafPosition_t  sampleFrame)
	{
		AX_PLUGIN_TRY( _impl, Seek, (sampleFrame) );
	}


	STDMETHOD(CompleteWrite) (
		IAAFSourceMob * pFileMob)
	{
		AX_PLUGIN_TRY( _impl, CompleteWrite, (pFileMob) );
	}

	STDMETHOD(CreateDescriptorFromStream) (
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob)
	{
		AX_PLUGIN_TRY( _impl, CreateDescriptorFromStream, (pStream, pSourceMob) );
	}

	STDMETHOD(GetCurrentEssenceStream) (
		IAAFEssenceStream ** ppStream)
	{
		AX_PLUGIN_TRY( _impl, GetCurrentEssenceStream, (ppStream) );
	}

	STDMETHOD(PutEssenceFormat) (
		IAAFEssenceFormat * pFormat)
	{
		AX_PLUGIN_TRY( _impl, PutEssenceFormat, (pFormat) );
	}
	
	STDMETHOD(GetEssenceFormat) (
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat)
	{
		AX_PLUGIN_TRY( _impl, GetEssenceFormat, (pFormatTemplate, ppNewFormat) );
	}

	STDMETHOD(GetDefaultEssenceFormat) (
		IAAFEssenceFormat ** ppNewFormat)
	{
		AX_PLUGIN_TRY( _impl, GetDefaultEssenceFormat, (ppNewFormat) );
	}

	STDMETHOD(GetEssenceDescriptorID) (
		aafUID_t *  pDescriptorID)
	{
		AX_PLUGIN_TRY( _impl, GetEssenceDescriptorID, (pDescriptorID) );
	}

	STDMETHOD(GetEssenceDataID) (
		aafUID_t *  pEssenceDataID)
	{
		AX_PLUGIN_TRY( _impl, GetEssenceDataID, (pEssenceDataID) );
	}
	
	STDMETHOD(GetIndexedSampleSize) (
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength)
	{
		AX_PLUGIN_TRY( _impl, GetIndexedSampleSize, (essenceDefID, sampleOffset, pLength) );
	}

	STDMETHOD(GetLargestSampleSize) (
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength)
	{
		AX_PLUGIN_TRY( _impl, GetLargestSampleSize, (essenceDefID, pLength) );
	}
	
private:

	std::auto_ptr<T> _impl;
};

#endif