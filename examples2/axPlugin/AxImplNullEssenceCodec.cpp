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
	_categoryClassAUID( AUID_AAFCodecDef )
{
	// Init the flavours vector
	// kAAFNilCodecFlavour must be supported
	_flavours.push_back( kAAFNilCodecFlavour );

	// Init the display name map.  This maps flavour ids to taste buds.
	// Each entry in the _flavours vector should be represented in this map.
	// kAAFNilCodecFlavour must alway be supported.
	_displayNames[ kAAFNilCodecFlavour ] = L"AAF Example Codec";
		
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

//
//  IAAFEssenceCodec methods
//

void AxImplNullEssenceCodec::SetEssenceAccess(
		IAAFEssenceAccess * pEssenceAccess )
{
	_access = pEssenceAccess;
}

void AxImplNullEssenceCodec::CountFlavours(
		aafUInt32*  pCount )
{
	TRACE

	if ( !pCount ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	*pCount = _flavours.size();
}

void AxImplNullEssenceCodec::GetIndexedFlavourID(
		aafUInt32  index,
		aafUID_t *  pVariant )
{
	TRACE

	if ( !pVariant ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}
	
	if ( index >= _flavours.size() ) {
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	*pVariant = _flavours[index];
}
 
void AxImplNullEssenceCodec::CountDataDefinitions(
		aafUInt32*  pCount )
{
	TRACE

	if ( !pCount ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

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
	if ( !pDataDefID ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}
	
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

	if ( !pBufSize ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	// FIXME - This could be done once at init time.  Also, could be in
	// a little class that derives from std::map and adds a method to computes
	// the max length is done with the specifier map.
	int maxSize = 0;
	DisplayNameIterType iter;
	for( iter = _displayNames.begin();  iter != _displayNames.end(); ++iter ) {
		if ( iter->second.size() > maxSize ) {
			maxSize = iter->second.size();
		}
	}

	*pBufSize = (maxSize+1) * sizeof(DisplayNameCharType);
}

void AxImplNullEssenceCodec::GetCodecDisplayName(
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize )
{
	TRACE

	if ( !pName ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	DisplayNameIterType iter;
	iter = _displayNames.find( flavour );
	if ( iter == _displayNames.end() ) {
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
		aafCompressEnable_t  compEnable)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
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
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::WriteSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::ReadSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::Seek(
		aafPosition_t  sampleFrame)
{
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::CompleteWrite(
		IAAFSourceMob * pFileMob)
{
	TRACE
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
	TRACE
	CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
}

void AxImplNullEssenceCodec::PutEssenceFormat(
		IAAFEssenceFormat * pFormat)
{
	if ( !pFormat ) {
		CHECK_HRESULT(AAFRESULT_NULL_PARAM);
	}

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
	if ( !pFormatTemplate ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	if ( !ppNewFormat ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	aafInt32 numSpecifiers;
	int i;

	CHECK_HRESULT( pFormatTemplate->NumFormatSpecifiers( &numSpecifiers ) );

	IAAFSmartPointer<IAAFEssenceFormat> pFormat;
	CHECK_HRESULT( _access->GetEmptyFileFormat( &pFormat ) );

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
	if ( !ppNewFormat ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	IAAFSmartPointer<IAAFEssenceFormat> pFormat;
	CHECK_HRESULT( _access->GetEmptyFileFormat( &pFormat ) );

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

	if ( !pDescriptorID ) {
		CHECK_HRESULT(AAFRESULT_NULL_PARAM);
	}

	*pDescriptorID = _descriptorAUID;
}

void AxImplNullEssenceCodec::GetEssenceDataID(
		aafUID_t *  pEssenceDataID)
{
	TRACE
	
	if ( !pEssenceDataID ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}

	*pEssenceDataID = _essenceDataAUID;
}
	
void AxImplNullEssenceCodec::GetIndexedSampleSize(
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength)
{
	TRACE
	
	if ( !pLength ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}
		
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

	if ( !pLength ) {
		CHECK_HRESULT( AAFRESULT_NULL_PARAM );
	}
		
		
	if ( _dataDefID != essenceDefID ) {
		CHECK_HRESULT( AAFRESULT_CODEC_CHANNELS );
	}

	// Verify minimal supported essence descriptor values.
	// FIXME - Test this when the values are set.
	if ( 2 != _horizSubsampling || kAAFColorSpaceYUV != _colorSpace ) {
		// Of course, a full blown codec implemention would not enforce
		// such restrictions.
		CHECK_HRESULT( AAFRESULT_NOT_IMPLEMENTED );
	}

	// Assumes 8 bit per component yuv.
	*pLength = _storedRect.GetVal().xSize * _storedRect.GetVal().ySize * 2;
}
