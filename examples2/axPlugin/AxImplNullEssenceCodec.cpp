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

#include "AxImplNullEssenceCodec.h"

#include <AxSmartPointer.h>
#include <AxDictionary.h>
#include <AxEssence.h>
#include <AxMetaDef.h>
#include <AxMob.h>

#include <AAFCodecDefs.h>
#include <AAFDataDefs.h>
#include <AAFResult.h>

#include <memory>

#if 0
#include <iostream>
#define TRACE std::cout << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define TRACE
#endif

//=---------------------------------------------------------------------=

AxImplNullEssenceCodec::AxImplNullEssenceCodec()
:	_descriptorAUID( AUID_AAFCDCIDescriptor ),
	_dataDefID( DDEF_Picture ),
	_codecName( L"AxNullEssenceCodec" ),
	_codecDesc( L"AAF Example Codec" ),
	_essenceDataAUID( AUID_AAFEssenceData ),
	_essenceDataDefID( DDEF_Picture ),
	_categoryClassAUID( AUID_AAFCodecDef ),
	_pAccess( 0 )
{
	// Init the flavours vector
	// kAAFNilCodecFlavour must be supported
	_flavours.push_back( kAAFNilCodecFlavour );

	// Init the display name map.  This maps flavour ids to taste buds.
	// Each entry in the _flavours vector should be represented in this map.
	// kAAFNilCodecFlavour must alway be supported.
	_flavourNames[ kAAFNilCodecFlavour ] = L"AAF Example Codec";
		
	InitFormatSpecifiers();
}

AxImplNullEssenceCodec::~AxImplNullEssenceCodec()
{
	FiniFormatSpecifiers();
}
	
void AxImplNullEssenceCodec::InitFormatSpecifiers()
{

#define _INIT( SPECIFIER, UID ) \
	SPECIFIER.SetUID( UID ); \
	_specifierMap[ UID ] = &SPECIFIER;

#define _INIT_RO( SPECIFIER, UID ) \
	_INIT( SPECIFIER, UID ) \
	SPECIFIER.SetReadOnly( true );

	_INIT( _storedRect,			   kAAFStoredRect );
	_INIT( _compression,           kAAFCompression );
	_INIT( _colorSpace,            kAAFPixelFormat );
	_INIT( _frameLayout,           kAAFFrameLayout );
	_INIT( _fieldDominance,        kAAFFieldDominance );
	_INIT( _storedRect,            kAAFStoredRect );
	_INIT( _sampledRect,           kAAFSampledRect );
	_INIT( _displayRect,           kAAFDisplayRect );
	_INIT( _aspectRatio,           kAAFAspectRatio );
	_INIT( _alphaTransparency,     kAAFAlphaTransparency );
	_INIT( _gamma,                 kAAFGamma );
	_INIT( _imageAlignmentFactor,  kAAFImageAlignmentFactor );
	_INIT( _videoLineMap,          kAAFVideoLineMap );
	_INIT( _componentWidth,        kAAFCDCICompWidth );
	_INIT( _horizSubsampling,      kAAFCDCIHorizSubsampling );
    _INIT( _colorSiting,           kAAFCDCIColorSiting );
	_INIT( _blackLevel,            kAAFCDCIBlackLevel );	
	_INIT( _whiteLevel,            kAAFCDCIWhiteLevel );
	_INIT( _colorRange,            kAAFCDCIColorRange );
	_INIT( _padBits,               kAAFCDCIPadBits );
	_INIT( _padBytesPerRow,        kAAFPadBytesPerRow );
	_INIT( _fieldStartOffset,      kAAFFieldStartOffset );
	_INIT( _fieldEndOffset,        kAAFFieldEndOffset );
	_INIT( _sampleRate,            kAAFSampleRate );

	// kAAFCDCIVertSubsampling is not currently defined in AAFEssenceFormats.h
	// _INIT( _vertSubsampling,       kAAFCDCIVertSubsampling );
	
	// Read only format specifiers.
	_INIT_RO( _numChannels,        kAAFNumChannels );
	_INIT_RO( _pixelSize,          kAAFPixelSize );
	_INIT_RO( _maxSampleBytes,     kAAFMaxSampleBytes );
	_INIT_RO( _willTransferLines,  kAAFWillTransferLines );
	_INIT_RO( _isCompressed,       kAAFIsCompressed );
	
#undef _INIT
#undef _INIT_RO

	// Finally, the oddball legacy specifier.
	// This does not go in the map. It gets special treatment.
	_legacyFlag.SetUID( kAAFLegacyCDCI );
	_legacyFlag = false;

	// Figure out the maximum specifier value buffer size.
	_maxValBufSize = _specifierMap.GetMaxValBufSize();
	if ( _legacyFlag.GetValBufSize() > _maxValBufSize ) {
		_maxValBufSize = _legacyFlag.GetValBufSize();
	}

	// Constant read only format specifiers.
	_numChannels = 1;
	_willTransferLines = false;
	_isCompressed = false;

	// Computed read only format specifiers.
	_maxSampleBytes = 0;
	_pixelSize = 24;
}

void AxImplNullEssenceCodec::FiniFormatSpecifiers()
{
	// Intentional noop.
	// The _specifierMap does not own the pointers it stores.  No need
	// to delete them.
}


const aafUID_t& AxImplNullEssenceCodec::GetDescriptorAUID()
{
	return _descriptorAUID;
}

const aafUID_t& AxImplNullEssenceCodec::GetDataDefID()
{
	return _dataDefID;
}

const aafUID_t& AxImplNullEssenceCodec::GetCategoryClassAUID()
{
	return _categoryClassAUID;
}

const AxString& AxImplNullEssenceCodec::GetName()
{
	return _codecName;
}

const AxString& AxImplNullEssenceCodec::GetDesc()
{
	return _codecDesc;
}

IAAFCDCIDescriptorSP AxImplNullEssenceCodec::GetEssenceDescriptor( IAAFSourceMobSP spSourceMob )
{
	AxSourceMob axSourceMob( spSourceMob );

	IAAFEssenceDescriptorSP spEssenceDesc = axSourceMob.GetEssenceDescriptor();
	IAAFCDCIDescriptorSP spCDCIDescriptor;
	AxQueryInterface( spEssenceDesc, spCDCIDescriptor );

	return spCDCIDescriptor;
}

void AxImplNullEssenceCodec::UpdateEssenceDescriptor( IAAFSourceMobSP spSourceMob )
{
	AxCDCIDescriptor axDesc( GetEssenceDescriptor( spSourceMob ) );
	
	// Update FileDescriptor properties
	axDesc.SetLength( _numSamples );
	axDesc.SetSampleRate( _sampleRate );

	// Update DigitalImageDescriptor properties
	axDesc.SetCompression( _compression );
	axDesc.SetStoredView(  _storedRect.GetVal().ySize,
						   _storedRect.GetVal().xSize );
	axDesc.SetSampledView( _sampledRect.GetVal().ySize,
						   _sampledRect.GetVal().xSize,
						   _sampledRect.GetVal().xOffset,
						   _sampledRect.GetVal().yOffset );
	axDesc.SetDisplayView( _displayRect.GetVal().ySize,
						   _displayRect.GetVal().xSize,
						   _displayRect.GetVal().xOffset,
						   _displayRect.GetVal().yOffset );
	axDesc.SetFrameLayout( _frameLayout );
	axDesc.SetVideoLineMap( _videoLineMap.GetValBufSize()/sizeof(_videoLineMap.GetVal()[0]),
							_videoLineMap );
	axDesc.SetImageAspectRatio( _aspectRatio );
	axDesc.SetAlphaTransparency( _alphaTransparency );
	axDesc.SetGamma( _gamma );
	axDesc.SetImageAlignmentFactor( _imageAlignmentFactor );

	// FIXME - These are optional properties.  They would have to be added to
	// the descriptor, as is done in plugin/CAAFCDescriptorHelper.cpp
	// Seems like a bad idea, in general, should it be emulated here?
	//axDesc.SetFieldDominance( _fieldDominance );
	//axDesc.SetFieldStartOffset( _fieldStartOffset );
	//axDesc.SetFieldEndOffset( _fieldEndOffset );

	// CDCIDescriptor methods:
	axDesc.SetComponentWidth( _componentWidth );
	axDesc.SetHorizontalSubsampling( _horizSubsampling );
	axDesc.SetColorSiting( _colorSiting );
	axDesc.SetBlackReferenceLevel( _blackLevel );
	axDesc.SetWhiteReferenceLevel( _whiteLevel );
	axDesc.SetColorRange( _colorRange );
	axDesc.SetPaddingBits( _padBits );
}

int AxImplNullEssenceCodec::GetFrameSize()
{
	// Verify minimal supported essence descriptor values.
	// FIXME - Test this when the values are set.
	if ( 2 != _horizSubsampling || kAAFColorSpaceYUV != _colorSpace ) {
		// Of course, a full blown codec implemention would not enforce
		// such restrictions.
		CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
	}

	// Assumes 8 bit per component yuv.
	return  _storedRect.GetVal().xSize * _storedRect.GetVal().ySize * 2;
}

//
//  IAAFEssenceCodec methods
//

void AxImplNullEssenceCodec::SetEssenceAccess(
		IAAFEssenceAccess * pEssenceAccess )
{
	_pAccess = pEssenceAccess;
}

void AxImplNullEssenceCodec::CountFlavours(
		aafUInt32*  pCount )
{
	TRACE

	*pCount = _flavours.size();
}

void AxImplNullEssenceCodec::GetIndexedFlavourID(
		aafUInt32  index,
		aafUID_t *  pVariant )
{
	TRACE

	if ( index >= _flavours.size() ) {
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	*pVariant = _flavours[index];
}
 
void AxImplNullEssenceCodec::CountDataDefinitions(
		aafUInt32*  pCount )
{
	TRACE

	// FIXME, the CDCI, JPEG, AIFC, and WAVE all return
	// the value one, but then return AAFRESULT_NOT_IMPLEMENTED
	// in GetIndexedDataDefinition(). Hence, zero seems more
	// logical.
	*pCount = 0;
}

void AxImplNullEssenceCodec::GetIndexedDataDefinition(
		aafUInt32  index,
		aafUID_t *  pDataDefID )
{
	if ( index >= 0 ) {
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	// FIXME - What should be done here?  The CDCI, JPEG, AIFC, and WAVE
	// codecs all return AAFRESULT_NOT_IMPLEMENTED.
	// The bad index exception is returned in leiu of "bad implementation".
}

void AxImplNullEssenceCodec::GetMaxCodecDisplayNameLength(
		aafUInt32*  pBufSize )
{
	TRACE

	*pBufSize = _flavourNames.GetMaxCBufferSize();
}

void AxImplNullEssenceCodec::GetCodecDisplayName(
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize )
{
	TRACE

	DisplayNameIterType iter;
	iter = _flavourNames.find( flavour );
	if ( iter == _flavourNames.end() ) {
		// FIXME - This requires a more meaningful error code.
		// e.g. "not found", "unknown", or "not supported".
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	if ( bufSize < ((iter->second.size()+1) * sizeof(DisplayNameCharType)) ) {
		CHECK_HRESULT( AAFRESULT_SMALLBUF );
	}
		
	int i;
	for (i = 0;
		 i < iter->second.size();
		 i++ ) {
		pName[i] = (iter->second)[i];
	}

	pName[i] = 0;
}
	
void AxImplNullEssenceCodec::CountChannels(
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels )
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::GetSelectInfo(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo )
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}


void AxImplNullEssenceCodec::ValidateEssence(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}


void AxImplNullEssenceCodec::Create(
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  /*compEnable*/ )
{
	TRACE

	if ( !_flavourNames.IsFound( flavour ) ) {
		// FIXME - The CDCI and JPEG codecs return AAFRESULT_NULL_PARAMS.
		// This seems like a better alternative, but remains less than ideal.
		CHECK_HRESULT( AAFRESULT_NO_MORE_FLAVOURS );
	}

	// If multiple flavours are supported, the data def would likely
	// be a function of the flavour id.
	if ( essenceKind != _dataDefID ) {
		CHECK_HRESULT( AAFRESULT_INVALID_DATADEF );
	}

	// Reset specifier values as required.
	_sampleRate = sampleRate;
	
	// Save the stream pointer.
	IAAFEssenceStreamSP spStream(stream);
	_spStream = spStream;

	IAAFSourceMobSP spSrcMob( fileMob );
	UpdateEssenceDescriptor( spSrcMob );
}

void AxImplNullEssenceCodec::Open(
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}
	
void AxImplNullEssenceCodec::CountSamples(
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples)
{
	if ( essenceKind != _dataDefID ) {
		CHECK_HRESULT( AAFRESULT_INVALID_DATADEF );
	}

	*pNumSamples = _numSamples;
}

void AxImplNullEssenceCodec::WriteSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten)
{
	TRACE

	if ( buflen < GetFrameSize() ) {
		CHECK_HRESULT( AAFRESULT_SMALLBUF );
	}
	
	// Write sample to _stream, aligning as necessary.

	*samplesWritten = 1;
	*bytesWritten = GetFrameSize();

	_numSamples += 1;
}

void AxImplNullEssenceCodec::ReadSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead)
{
	TRACE

	// Read samples from _stream, aligned as necessary.
	
	*samplesRead = 1;
	*bytesRead = GetFrameSize();
}

void AxImplNullEssenceCodec::Seek(
		aafPosition_t  sampleFrame)
{
	TRACE
	
	// Seek to the correct location in _stream to read frame number
	// sampleFrame.

	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::CompleteWrite(
		IAAFSourceMob * pFileMob)
{
	TRACE

	// Perform final processing on _spStream.

	// Update the essence descriptor.
	IAAFSourceMobSP spSrcMob( pFileMob );
	UpdateEssenceDescriptor( spSrcMob );
		
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::CreateDescriptorFromStream(
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::GetCurrentEssenceStream(
		IAAFEssenceStream ** ppStream)
{
	// FIXME - Mixed use of smart/dump pointer.
	// Is there a better way to deal with this?

	*ppStream = _spStream;
	(*ppStream)->AddRef();
}

void AxImplNullEssenceCodec::PutEssenceFormat(
		IAAFEssenceFormat * pFormat)
{
	aafInt32 numSpecifiers;
	int i;

	CHECK_HRESULT( pFormat->NumFormatSpecifiers( &numSpecifiers ) );

	for (i = 0; i < numSpecifiers; i++ ) {

		aafUID_t uid;
		std::auto_ptr<aafUInt8> valBuf( new aafUInt8( _maxValBufSize ) );
		aafInt32 bytesRead;

		CHECK_HRESULT(
			pFormat->GetIndexedFormatSpecifier( i, &uid,
												_maxValBufSize,
												valBuf.get(),
												&bytesRead ) );

		// First, is this the oddball legacy specifier?
		if ( _legacyFlag.IsEqual( uid ) ) {
			// Do something oddball.
		}
		else {

			// If not in the specifier map then it is not supported.
			if ( !_specifierMap.IsFound( uid ) ) {
				CHECK_HRESULT( AAFRESULT_INVALID_OP_CODEC );
			}

			
			// If not read only, then assign the value;
			if ( !_specifierMap[uid]->IsReadOnly() ) {
				_specifierMap[uid]->SetValBuf( valBuf.get(), bytesRead );
			}
		}
	}
}
	
void AxImplNullEssenceCodec::GetEssenceFormat(
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat)
{
	aafInt32 numSpecifiers;
	int i;

	CHECK_HRESULT( pFormatTemplate->NumFormatSpecifiers( &numSpecifiers ) );

	IAAFSmartPointer<IAAFEssenceFormat> pFormat;
	CHECK_HRESULT( _pAccess->GetEmptyFileFormat( &pFormat ) );

	for (i = 0; i < numSpecifiers; i++ ) {

		aafUID_t uid;
		std::auto_ptr<aafUInt8> valBuf( new aafUInt8( _maxValBufSize ) );
		aafInt32 bytesRead;

		CHECK_HRESULT(
			pFormatTemplate->GetIndexedFormatSpecifier( i, &uid,
												_maxValBufSize,
												valBuf.get(),
												&bytesRead ) );
		
		// If not in the specifier map then it is not supported.
		if ( !_specifierMap.IsFound( uid ) ) {
			CHECK_HRESULT( AAFRESULT_INVALID_OP_CODEC );
		}
		
		CHECK_HRESULT( pFormat->AddFormatSpecifier( uid,
													_specifierMap[uid]->GetValBufSize(),
													_specifierMap[uid]->GetValBuf() ) );
	}

	*ppNewFormat = pFormat;
}

void AxImplNullEssenceCodec::GetDefaultEssenceFormat(
		IAAFEssenceFormat ** ppNewFormat)
{
	IAAFSmartPointer<IAAFEssenceFormat> pFormat;
	CHECK_HRESULT( _pAccess->GetEmptyFileFormat( &pFormat ) );

	AxPluginSpecifierMap::IterType iter;

	for( iter = _specifierMap.begin(); iter != _specifierMap.end(); ++iter ) {

		CHECK_HRESULT( pFormat->AddFormatSpecifier( iter->second->GetUID(),
													iter->second->GetValBufSize(),
													iter->second->GetValBuf() ) );
	}

	*ppNewFormat = pFormat;
}

void AxImplNullEssenceCodec::GetEssenceDescriptorID(
		aafUID_t *  pDescriptorID)
{
	TRACE
	*pDescriptorID = _descriptorAUID;
}

void AxImplNullEssenceCodec::GetEssenceDataID(
		aafUID_t *  pEssenceDataID)
{
	TRACE
	*pEssenceDataID = _essenceDataAUID;
}
	
void AxImplNullEssenceCodec::GetIndexedSampleSize(
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength)
{
	TRACE
	
	if ( _dataDefID != essenceDefID ) {
		CHECK_HRESULT( AAFRESULT_CODEC_CHANNELS );
	}

	GetLargestSampleSize( essenceDefID, pLength );
}

void AxImplNullEssenceCodec::GetLargestSampleSize(
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength)
{
	TRACE
		
	if ( _dataDefID != essenceDefID ) {
		CHECK_HRESULT( AAFRESULT_CODEC_CHANNELS );
	}

	*pLength = GetFrameSize();
}
