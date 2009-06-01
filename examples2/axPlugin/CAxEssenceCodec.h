#ifndef __CAxEssenceCodec_h__
#define __CAxEssenceCodec_h__

//=---------------------------------------------------------------------=
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
//=---------------------------------------------------------------------=

#include "AxPluginUtil.h"

#include <AAFPlugin.h>
#include <AAFResult.h>


#include <memory>

template <class T>
class CAxEssenceCodec : public IAAFEssenceCodec
{
public:

	CAxEssenceCodec( std::auto_ptr<T> impl )
		: _impl( impl )
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

	HRESULT STDMETHODCALLTYPE SetEssenceAccess (
		IAAFEssenceAccess * pEssenceAccess )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pEssenceAccess );
		AX_PLUGIN_TRY( _impl, SetEssenceAccess, (pEssenceAccess) );
	}

	HRESULT STDMETHODCALLTYPE CountFlavours (
		aafUInt32*  pCount )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pCount );
		AX_PLUGIN_TRY( _impl, CountFlavours, (pCount) );
	}

	HRESULT STDMETHODCALLTYPE GetIndexedFlavourID (
		aafUInt32  index,
		aafUID_t *  pVariant )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pVariant );
		AX_PLUGIN_TRY( _impl, GetIndexedFlavourID, (index, pVariant) );
	}
 
	HRESULT STDMETHODCALLTYPE CountDataDefinitions (
		aafUInt32*  pCount )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pCount );
		AX_PLUGIN_TRY( _impl, CountDataDefinitions, (pCount) );
	}

	HRESULT STDMETHODCALLTYPE GetIndexedDataDefinition (
		aafUInt32  index,
		aafUID_t *  pDataDefID )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pDataDefID );
		AX_PLUGIN_TRY( _impl, GetIndexedDataDefinition, (index, pDataDefID) );
	}

	HRESULT STDMETHODCALLTYPE GetMaxCodecDisplayNameLength (
		aafUInt32*  pBufSize )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pBufSize );
		AX_PLUGIN_TRY( _impl, GetMaxCodecDisplayNameLength, (pBufSize) );
	}		

	HRESULT STDMETHODCALLTYPE GetCodecDisplayName (
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pName );
		AX_PLUGIN_TRY( _impl, GetCodecDisplayName, (flavour, pName, bufSize) );
	}
	
	HRESULT STDMETHODCALLTYPE CountChannels (
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels )
	{	
		AX_PLUGIN_CHECK_NULL_PARAM( fileMob );
		AX_PLUGIN_CHECK_NULL_PARAM( stream );
		AX_PLUGIN_CHECK_NULL_PARAM( pNumChannels );
		AX_PLUGIN_TRY( _impl, CountChannels, (fileMob, essenceKind, stream, pNumChannels) );
	}

	HRESULT STDMETHODCALLTYPE GetSelectInfo (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( fileMob );
		AX_PLUGIN_CHECK_NULL_PARAM( stream );
		AX_PLUGIN_CHECK_NULL_PARAM( pSelectInfo );
		AX_PLUGIN_TRY( _impl, GetSelectInfo, (fileMob, stream, pSelectInfo) );
	}

	HRESULT STDMETHODCALLTYPE ValidateEssence (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( fileMob );
		AX_PLUGIN_CHECK_NULL_PARAM( stream );
		AX_PLUGIN_CHECK_NULL_PARAM( pErrorText );
		AX_PLUGIN_CHECK_NULL_PARAM( pBytesRead );
		AX_PLUGIN_TRY( _impl, ValidateEssence, (fileMob, stream, verbose, outputWarnings, bufSize, pErrorText, pBytesRead) );
	}

	HRESULT STDMETHODCALLTYPE Create (
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
	{	
		AX_PLUGIN_CHECK_NULL_PARAM( fileMob );
		AX_PLUGIN_CHECK_NULL_PARAM( stream );
		AX_PLUGIN_TRY( _impl, Create, (fileMob, flavour, essenceKind, sampleRate, stream, compEnable) );
	}	

	HRESULT STDMETHODCALLTYPE Open (
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( fileMob );
		AX_PLUGIN_CHECK_NULL_PARAM( stream );
		AX_PLUGIN_TRY( _impl, Open, (fileMob, openMode, stream, compEnable) );
	}
	
	HRESULT STDMETHODCALLTYPE CountSamples (
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pNumSamples );
		AX_PLUGIN_TRY( _impl, CountSamples, (essenceKind, pNumSamples) );
	}

	HRESULT STDMETHODCALLTYPE WriteSamples (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( buffer );
		AX_PLUGIN_CHECK_NULL_PARAM( samplesWritten );
		AX_PLUGIN_CHECK_NULL_PARAM( bytesWritten );
		AX_PLUGIN_TRY( _impl, WriteSamples, (nSamples, buflen, buffer, samplesWritten, bytesWritten) );
	}

	HRESULT STDMETHODCALLTYPE ReadSamples (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( buffer );
		AX_PLUGIN_CHECK_NULL_PARAM( samplesRead );
		AX_PLUGIN_CHECK_NULL_PARAM( bytesRead );
		AX_PLUGIN_TRY( _impl, ReadSamples, (nSamples, buflen, buffer, samplesRead, bytesRead) );
	}

	HRESULT STDMETHODCALLTYPE Seek (
		aafPosition_t  sampleFrame)
	{
		AX_PLUGIN_TRY( _impl, Seek, (sampleFrame) );
	}


	HRESULT STDMETHODCALLTYPE CompleteWrite (
		IAAFSourceMob * pFileMob)
	{
		// No check for null pFileMob.  It is optional, and may have a null value.
		AX_PLUGIN_TRY( _impl, CompleteWrite, (pFileMob) );
	}

	HRESULT STDMETHODCALLTYPE CreateDescriptorFromStream (
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pStream );
		AX_PLUGIN_CHECK_NULL_PARAM( pSourceMob );
		AX_PLUGIN_TRY( _impl, CreateDescriptorFromStream, (pStream, pSourceMob) );
	}

	HRESULT STDMETHODCALLTYPE GetCurrentEssenceStream (
		IAAFEssenceStream ** ppStream)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( ppStream );
		AX_PLUGIN_TRY( _impl, GetCurrentEssenceStream, (ppStream) );
	}

	HRESULT STDMETHODCALLTYPE PutEssenceFormat (
		IAAFEssenceFormat * pFormat)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pFormat );
		AX_PLUGIN_TRY( _impl, PutEssenceFormat, (pFormat) );
	}
	
	HRESULT STDMETHODCALLTYPE GetEssenceFormat (
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pFormatTemplate );
		AX_PLUGIN_CHECK_NULL_PARAM( ppNewFormat );
		AX_PLUGIN_TRY( _impl, GetEssenceFormat, (pFormatTemplate, ppNewFormat) );
	}

	HRESULT STDMETHODCALLTYPE GetDefaultEssenceFormat (
		IAAFEssenceFormat ** ppNewFormat)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( ppNewFormat );
		AX_PLUGIN_TRY( _impl, GetDefaultEssenceFormat, (ppNewFormat) );
	}

	HRESULT STDMETHODCALLTYPE GetEssenceDescriptorID (
		aafUID_t *  pDescriptorID)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pDescriptorID );
		AX_PLUGIN_TRY( _impl, GetEssenceDescriptorID, (pDescriptorID) );
	}

	HRESULT STDMETHODCALLTYPE GetEssenceDataID (
		aafUID_t *  pEssenceDataID)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pEssenceDataID );
		AX_PLUGIN_TRY( _impl, GetEssenceDataID, (pEssenceDataID) );
	}
	
	HRESULT STDMETHODCALLTYPE GetIndexedSampleSize (
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pLength );
		AX_PLUGIN_TRY( _impl, GetIndexedSampleSize, (essenceDefID, sampleOffset, pLength) );
	}

	HRESULT STDMETHODCALLTYPE GetLargestSampleSize (
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength)
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pLength );
		AX_PLUGIN_TRY( _impl, GetLargestSampleSize, (essenceDefID, pLength) );
	}
	
private:

	std::auto_ptr<T> _impl;
};

#endif

