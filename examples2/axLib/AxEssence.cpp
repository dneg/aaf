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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#include "AxEssence.h"

#include "AxUtil.h"

#include <AAFResult.h>

AxEssenceMultiAccess::AxEssenceMultiAccess( IAAFEssenceMultiAccessSP sp )
:	_spIaafEssenceMultiAccess( sp )
{}

AxEssenceMultiAccess::~AxEssenceMultiAccess()
{}

//=---------------------------------------------------------------------=

AxEssenceAccess::AxEssenceAccess( IAAFEssenceAccessSP sp )
:	AxEssenceMultiAccess( AxQueryInterface<IAAFEssenceAccess, IAAFEssenceMultiAccess>( sp ) ),
	_spIaafEssenceAccess( sp )
{}

AxEssenceAccess::~AxEssenceAccess()
{}

IAAFEssenceFormatSP AxEssenceAccess::GetEmptyFileFormat()
{
	IAAFEssenceFormatSP spIaafEssenceFormat;

	CHECK_HRESULT( _spIaafEssenceAccess->GetEmptyFileFormat( &spIaafEssenceFormat ) );

	return spIaafEssenceFormat;
}

IAAFEssenceFormatSP AxEssenceAccess::GetFileFormat( IAAFEssenceFormatSP spTemplate )
{
	IAAFEssenceFormatSP spIaafEssenceFormat;

	CHECK_HRESULT( _spIaafEssenceAccess->GetFileFormat( spTemplate, &spIaafEssenceFormat ) );

	return spIaafEssenceFormat;
}

IAAFEssenceFormatSP AxEssenceAccess::GetFileFormatParameterList()
{
	IAAFEssenceFormatSP spIaafEssenceFormat;

	CHECK_HRESULT( _spIaafEssenceAccess->GetFileFormatParameterList( &spIaafEssenceFormat ) );

	return spIaafEssenceFormat;
}

void AxEssenceAccess::PutFileFormat( IAAFEssenceFormatSP spIaafEssenceFormat )
{
	CHECK_HRESULT( _spIaafEssenceAccess->PutFileFormat( spIaafEssenceFormat ) );
}

aafLength_t AxEssenceAccess::CountSamples( IAAFDataDefSP spDataDef )
{
	aafLength_t count;

	CHECK_HRESULT( _spIaafEssenceAccess->CountSamples( spDataDef, &count ) );

	return count;
}
	
AxEssenceAccess::WriteResult AxEssenceAccess::WriteSamples( aafUInt32 nSamples,
													        aafUInt32 bufLength,
							                                aafDataBuffer_t  buffer )
{
	WriteResult ret = { AAFRESULT_SUCCESS, 0, 0 };

	ret.hr = _spIaafEssenceAccess->WriteSamples( nSamples,
								   bufLength,
								   buffer,
								   &ret.samplesWritten,
							       &ret.bytesWritten );

	// We permit AAFRESULT_EOF.  Any other error throws and exceptions.
	if ( ret.hr != AAFRESULT_EOF ) {
		CHECK_HRESULT( ret.hr );
	}

	return ret;
}

void AxEssenceAccess::CompleteWrite()
{
	CHECK_HRESULT( _spIaafEssenceAccess->CompleteWrite() );
}

AxString AxEssenceAccess::GetCodecName()
{
	// There is no "GetCodecNameLength()" method, hence, assume the name will be
	// less than 80 characters.
	const aafUInt32 bufLength = 80;
	aafCharacter* buf = new aafCharacter[bufLength];

	CHECK_HRESULT( _spIaafEssenceAccess->GetCodecName( bufLength, buf ) );

	return AxString( buf );
}

aafLength_t AxEssenceAccess::GetLargestSampleSize( IAAFDataDefSP spIaafDataDef )
{
	aafLength_t length;

	CHECK_HRESULT( _spIaafEssenceAccess->GetLargestSampleSize( spIaafDataDef, &length ) );

	return length;
}

AxEssenceAccess::ReadResult AxEssenceAccess::ReadSamples( aafUInt32 nSamples,
			 					 					      aafUInt32 buflen,
												 	      aafDataBuffer_t buffer )
{
	ReadResult result;

	CHECK_HRESULT( _spIaafEssenceAccess->ReadSamples( nSamples, buflen, buffer,
					&result.samplesRead, &result.bytesRead ) );

	return result;
}

//=---------------------------------------------------------------------=

AxLocator::AxLocator( IAAFLocatorSP spIaafLocator )
:	AxObject( AxQueryInterface<IAAFLocator, IAAFObject>(spIaafLocator) ),
	_spIaafLocator( spIaafLocator )
{}

AxLocator::~AxLocator()
{}

void AxLocator::SetPath( const AxString& path )
{
	CHECK_HRESULT( _spIaafLocator->SetPath( path.c_str() ) );
}

AxString AxLocator::GetPath()
{
	aafUInt32 sizeInBytes = 0;

	CHECK_HRESULT( _spIaafLocator->GetPathBufLen( &sizeInBytes ) );

	// size is in bytes!  Divide by sizeof(aafCharacter) to allocate correctly
	// sized aafCharacter array.  Add one to account for possible rounding.

	int sizeInChars = (sizeInBytes /sizeof(aafCharacter)) + 1;
	std::vector< aafCharacter > buf( sizeInChars );

	CHECK_HRESULT( _spIaafLocator->GetPath( &buf[0], sizeInChars*sizeof(aafCharacter) ) );
	
	AxString name( &buf[0] );

	return name;
}


//=---------------------------------------------------------------------=

AxNetworkLocator::AxNetworkLocator( IAAFNetworkLocatorSP spIaafNetworkLocator )
:	AxLocator( AxQueryInterface<IAAFNetworkLocator, IAAFLocator>(spIaafNetworkLocator) ),
	_spIaafNetworkLocator( spIaafNetworkLocator )
{}

AxNetworkLocator::~AxNetworkLocator()
{}

void AxNetworkLocator::Initialize()
{
	CHECK_HRESULT( _spIaafNetworkLocator->Initialize() );
}

//=---------------------------------------------------------------------=

AxEssenceDescriptor::AxEssenceDescriptor( IAAFEssenceDescriptorSP sp )
:	AxObject( AxQueryInterface<IAAFEssenceDescriptor, IAAFObject>(sp) ),
	_spIaafEssenceDescriptor( sp )
{}

AxEssenceDescriptor::~AxEssenceDescriptor()
{}

aafUInt32 AxEssenceDescriptor::CountLocators()
{
	aafUInt32 numlocators;
	CHECK_HRESULT(_spIaafEssenceDescriptor->CountLocators(&numlocators));
	return numlocators;
}

void AxEssenceDescriptor::AppendLocator( IAAFLocatorSP spLocator )
{
	CHECK_HRESULT( _spIaafEssenceDescriptor->AppendLocator( spLocator ) );
}

IEnumAAFLocatorsSP AxEssenceDescriptor::GetLocators()
{
	IEnumAAFLocatorsSP spIEnumAAFLocators;

	CHECK_HRESULT( _spIaafEssenceDescriptor->GetLocators( &spIEnumAAFLocators ) );

	return spIEnumAAFLocators;
}


//=---------------------------------------------------------------------=

AxFileDescriptor::AxFileDescriptor( IAAFFileDescriptorSP sp )
:	AxEssenceDescriptor( AxQueryInterface<IAAFFileDescriptor, IAAFEssenceDescriptor>(sp) ),
	_spIaafFileDescriptor( sp )
{}

AxFileDescriptor::~AxFileDescriptor()
{}

aafLength_t AxFileDescriptor::GetLength()
{
	aafLength_t length;

	CHECK_HRESULT( _spIaafFileDescriptor->GetLength(&length) );

	return length;
}

aafRational_t AxFileDescriptor::GetSampleRate()
{
	aafRational_t rate;

	CHECK_HRESULT(_spIaafFileDescriptor->GetSampleRate(&rate));

	return rate;
}

void AxFileDescriptor::SetCodecDef(IAAFCodecDefSP codec)
{
	CHECK_HRESULT(_spIaafFileDescriptor->SetCodecDef(codec));
}

IAAFContainerDefSP AxFileDescriptor::GetContainerFormat()
{
  IAAFContainerDefSP spContainerDef;
  CHECK_HRESULT( _spIaafFileDescriptor->GetContainerFormat( &spContainerDef ) );
  return spContainerDef;
}

void AxFileDescriptor::SetContainerFormat(IAAFContainerDefSP container)
{
	CHECK_HRESULT(_spIaafFileDescriptor->SetContainerFormat(container));
}

void AxFileDescriptor::SetLength(aafLength_t length)
{
	CHECK_HRESULT(_spIaafFileDescriptor->SetLength(length));
}

void AxFileDescriptor::SetSampleRate(aafRational_constref rate)
{
	CHECK_HRESULT(_spIaafFileDescriptor->SetSampleRate(rate));
}

//=---------------------------------------------------------------------=

AxWAVEDescriptor::AxWAVEDescriptor( IAAFWAVEDescriptorSP sp )
:	AxFileDescriptor( AxQueryInterface<IAAFWAVEDescriptor, IAAFFileDescriptor>(sp) ),
	_spIaafWAVEDescriptor( sp )
{}

AxWAVEDescriptor::~AxWAVEDescriptor()
{}

void AxWAVEDescriptor::SetSummary( aafUInt32 size, aafDataValue_t  pSummary )
{
	CHECK_HRESULT( _spIaafWAVEDescriptor->SetSummary( size, pSummary ) );
}

AxBuffer<aafUInt8> AxWAVEDescriptor::GetSummary()
{
	using namespace std;
	
	aafUInt32 size;

	CHECK_HRESULT( _spIaafWAVEDescriptor->GetSummaryBufferSize( &size ) );

	aafUInt8* buffer = new aafUInt8[ size ];

	CHECK_HRESULT( _spIaafWAVEDescriptor->GetSummary( size, buffer ) );

	return AxBuffer<aafUInt8>( auto_ptr<aafUInt8>( buffer ), size );
}

//=---------------------------------------------------------------------=

AxDigitalImageDescriptor::AxDigitalImageDescriptor( IAAFDigitalImageDescriptorSP sp )
:	AxFileDescriptor( AxQueryInterface<IAAFDigitalImageDescriptor, IAAFFileDescriptor>(sp) ),
	_spIaafDigitalImageDescriptor( sp )
{}

AxDigitalImageDescriptor::~AxDigitalImageDescriptor()
{}

void AxDigitalImageDescriptor::SetCompression( const aafUID_t& compression )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetCompression( compression ) );
}
		
aafUID_t AxDigitalImageDescriptor::GetCompression()
{
	aafUID_t compression;

	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetCompression( &compression ) );

	return compression;
}

void AxDigitalImageDescriptor::SetStoredView( aafUInt32 StoredHeight, aafUInt32 StoredWidth)
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetStoredView( StoredHeight, StoredWidth) );
}

void AxDigitalImageDescriptor::SetSampledView( aafUInt32 SampledHeight, aafUInt32 SampledWidth,
					       aafInt32 SampledXOffset, aafInt32 SampledYOffset )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetSampledView( SampledHeight, SampledWidth,
								      SampledXOffset, SampledYOffset ) );
}

void AxDigitalImageDescriptor::SetDisplayView( aafUInt32 DisplayedHeight, aafUInt32 DisplayedWidth,
					       aafInt32 DisplayedXOffset, aafInt32 DisplayedYOffset )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetDisplayView( DisplayedHeight, DisplayedWidth,
								      DisplayedXOffset, DisplayedYOffset ) );
}

void AxDigitalImageDescriptor::SetFrameLayout( aafFrameLayout_t FrameLayout )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetFrameLayout( FrameLayout ) );
}

void AxDigitalImageDescriptor::SetVideoLineMap( aafUInt32  numberElements, const aafInt32*  pVideoLineMap )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetVideoLineMap( numberElements,
			                                            const_cast<aafInt32*>(pVideoLineMap) ) );
}

void AxDigitalImageDescriptor::SetImageAspectRatio( const aafRational_t & aspect)
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetImageAspectRatio( aspect ) );
}


void AxDigitalImageDescriptor::SetAlphaTransparency( const aafAlphaTransparency_t& alphaTransparency )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetAlphaTransparency( alphaTransparency ) );
}


void AxDigitalImageDescriptor::SetGamma( const aafUID_t& gamma )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetGamma( gamma ) );
}

void AxDigitalImageDescriptor::SetImageAlignmentFactor( aafUInt32 ImageAlignmentFactor)
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetImageAlignmentFactor( ImageAlignmentFactor ) );
}


void AxDigitalImageDescriptor::GetStoredView( aafUInt32& StoredHeight, aafUInt32& StoredWidth)
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetStoredView( &StoredHeight, &StoredWidth) );
}

void AxDigitalImageDescriptor::GetSampledView( aafUInt32& SampledHeight, aafUInt32& SampledWidth,
					       aafInt32& SampledXOffset, aafInt32& SampledYOffset )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetSampledView( &SampledHeight, &SampledWidth,
								      &SampledXOffset, &SampledYOffset ) );
}

void AxDigitalImageDescriptor::GetDisplayView( aafUInt32& DisplayedHeight, aafUInt32& DisplayedWidth,
					       aafInt32& DisplayedXOffset, aafInt32& DisplayedYOffset )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetDisplayView( &DisplayedHeight, &DisplayedWidth,
								      &DisplayedXOffset, &DisplayedYOffset ) );
}

aafRational_t AxDigitalImageDescriptor::GetImageAspectRatio()
{
	aafRational_t aspect;

	CHECK_HRESULT(_spIaafDigitalImageDescriptor->GetImageAspectRatio(&aspect));

	return aspect;
}

aafFrameLayout_t AxDigitalImageDescriptor::GetFrameLayout()
{
	aafFrameLayout_t layout;
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetFrameLayout( &layout ) );
	return layout;
}

void AxDigitalImageDescriptor::GetVideoLineMap( aafUInt32  numberElements, aafInt32*  pVideoLineMap )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetVideoLineMap( numberElements, pVideoLineMap ) );
}


aafAlphaTransparency_t AxDigitalImageDescriptor::GetAlphaTransparency()
{
	aafAlphaTransparency_t alpha;

	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetAlphaTransparency( &alpha ) );

	return alpha;
}

aafUID_t AxDigitalImageDescriptor::GetGamma()
{
	aafUID_t gamma;

	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetGamma( &gamma ) );

	return gamma;
}

aafUInt32 AxDigitalImageDescriptor::GetImageAlignmentFactor()
{
	aafUInt32 alignment;

	CHECK_HRESULT( _spIaafDigitalImageDescriptor->GetImageAlignmentFactor( &alignment ) );

	return alignment;
}

//=---------------------------------------------------------------------=

AxCDCIDescriptor::AxCDCIDescriptor( IAAFCDCIDescriptorSP sp )
:	AxDigitalImageDescriptor( AxQueryInterface<IAAFCDCIDescriptor, IAAFDigitalImageDescriptor>(sp) ),
	_spIaafCDCIDescriptor( sp )
{}

AxCDCIDescriptor::~AxCDCIDescriptor()
{}

void AxCDCIDescriptor::SetPaddingBits( aafInt16 PaddingBits )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetPaddingBits( PaddingBits ) );
}

void AxCDCIDescriptor::SetComponentWidth( aafInt32 ComponentWidth )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetComponentWidth( ComponentWidth ) );
}

void AxCDCIDescriptor::SetHorizontalSubsampling( aafUInt32  HorizontalSubsampling )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetHorizontalSubsampling( HorizontalSubsampling ) );
}

void AxCDCIDescriptor::SetVerticalSubsampling( aafUInt32 VerticalSubsampling )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetVerticalSubsampling( VerticalSubsampling ) );
}

void AxCDCIDescriptor::SetColorSiting( const aafColorSiting_t& ColorSiting )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetColorSiting( ColorSiting ) );
}

void AxCDCIDescriptor::SetBlackReferenceLevel( aafUInt32 BlackReferenceLevel )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetBlackReferenceLevel( BlackReferenceLevel ) );
}

void AxCDCIDescriptor::SetWhiteReferenceLevel( aafUInt32 WhiteReferenceLevel )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetWhiteReferenceLevel( WhiteReferenceLevel ) );
}

void AxCDCIDescriptor::SetColorRange( aafUInt32 ColorRange )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetColorRange( ColorRange ) );
}

aafInt16 AxCDCIDescriptor::GetPaddingBits()
{
	aafInt16 paddingBits;
	CHECK_HRESULT( _spIaafCDCIDescriptor->GetPaddingBits( &paddingBits ) );
	return paddingBits;
}

aafInt32 AxCDCIDescriptor::GetComponentWidth()
{
  aafInt32 componentWidth;
  CHECK_HRESULT( _spIaafCDCIDescriptor->GetComponentWidth( &componentWidth ) );
  return componentWidth;
}

aafUInt32 AxCDCIDescriptor::GetHorizontalSubsampling()
{
  aafUInt32  horizontalSubsampling;
  CHECK_HRESULT( _spIaafCDCIDescriptor->GetHorizontalSubsampling( &horizontalSubsampling ) );
  return horizontalSubsampling;
}

aafUInt32 AxCDCIDescriptor::GetVerticalSubsampling()
{
  aafUInt32 verticalSubsampling;
  CHECK_HRESULT( _spIaafCDCIDescriptor->GetVerticalSubsampling( &verticalSubsampling ) );
  return verticalSubsampling;
}

aafColorSiting_t AxCDCIDescriptor::GetColorSiting()
{
	aafColorSiting_t siting;
	CHECK_HRESULT( _spIaafCDCIDescriptor->GetColorSiting( &siting ) );
	return siting;
}

aafUInt32 AxCDCIDescriptor::GetBlackReferenceLevel()
{
	aafUInt32 level;
	CHECK_HRESULT( _spIaafCDCIDescriptor->GetBlackReferenceLevel( &level ) );
	return level;
}

aafUInt32 AxCDCIDescriptor::GetWhiteReferenceLevel()
{
	aafUInt32 level;
	CHECK_HRESULT( _spIaafCDCIDescriptor->GetWhiteReferenceLevel( &level ) );
	return level;
}

aafUInt32 AxCDCIDescriptor::GetColorRange()
{
  aafUInt32 colorRange;
  CHECK_HRESULT( _spIaafCDCIDescriptor->GetColorRange( &colorRange ) );
  return colorRange;
}

//=---------------------------------------------------------------------=

AxTapeDescriptor::AxTapeDescriptor( IAAFTapeDescriptorSP sp )
:	AxEssenceDescriptor( AxQueryInterface<IAAFTapeDescriptor, IAAFEssenceDescriptor>(sp) ),
	_spIaafTapeDescriptor( sp )
{}

AxTapeDescriptor::~AxTapeDescriptor()
{}

void AxTapeDescriptor::Initialize()
{
	// noop
}

void AxTapeDescriptor::SetTapeManufacturer( const AxString& manufacturer )
{
	CHECK_HRESULT(
		_spIaafTapeDescriptor->SetTapeManufacturer( manufacturer.c_str() )
    );
}

AxString AxTapeDescriptor::GetTapeManufacturer()
{
	AxString manufacturer;

	AX_ANY_TO_STRING( manufacturer,
					  _spIaafTapeDescriptor,
					  GetTapeManufacturerBufLen,
					  GetTapeManufacturer );

	return manufacturer;
}

void AxTapeDescriptor::SetTapeModel( const AxString& mode )
{
	CHECK_HRESULT(
		_spIaafTapeDescriptor->SetTapeModel( mode.c_str() )
    );
}

AxString AxTapeDescriptor::GetTapeModel()
{
	AxString tapeModel;

	AX_ANY_TO_STRING( tapeModel,
					  _spIaafTapeDescriptor,
					  GetTapeModelBufLen,
					  GetTapeModel );

	return tapeModel;
}

void AxTapeDescriptor::SetTapeFormFactor( aafTapeCaseType_t formFactor )
{
	CHECK_HRESULT(
		_spIaafTapeDescriptor->SetTapeFormFactor( formFactor )
    );
}

aafTapeCaseType_t AxTapeDescriptor::GetTapeFormFactor()
{
	aafTapeCaseType_t formFactor;
	CHECK_HRESULT(
		_spIaafTapeDescriptor->GetTapeFormFactor( &formFactor )	);
	return formFactor;
}

void AxTapeDescriptor::SetSignalType( aafVideoSignalType_t signalType )
{
	CHECK_HRESULT(
		_spIaafTapeDescriptor->SetSignalType( signalType )
	);
}

aafVideoSignalType_t AxTapeDescriptor::GetSignalType()
{
	aafVideoSignalType_t signalType;
	CHECK_HRESULT( _spIaafTapeDescriptor->GetSignalType( &signalType ) );
	return signalType;
}

void AxTapeDescriptor::SetTapeFormat( aafTapeFormatType_t tapeFormat )
{
	CHECK_HRESULT( _spIaafTapeDescriptor->SetTapeFormat( tapeFormat ) );
}

aafTapeFormatType_t AxTapeDescriptor::GetTapeFormat()
{
	aafTapeFormatType_t tapeFormat;
	CHECK_HRESULT( _spIaafTapeDescriptor->GetTapeFormat( &tapeFormat ) );
	return tapeFormat;
}

void AxTapeDescriptor::SetTapeLength( aafUInt32 length )
{
	CHECK_HRESULT( _spIaafTapeDescriptor->SetTapeLength( length ) );
}

aafUInt32 AxTapeDescriptor::GetTapeLength()
{
	aafUInt32 length;
	CHECK_HRESULT( _spIaafTapeDescriptor->GetTapeLength( &length ) );
	return length;
}

//=---------------------------------------------------------------------=

AxFilmDescriptor::AxFilmDescriptor( IAAFFilmDescriptorSP spIaafFilmDescriptor )
:   AxEssenceDescriptor( AxQueryInterface<IAAFFilmDescriptor, IAAFEssenceDescriptor>(spIaafFilmDescriptor) ),
    _spIaafFilmDescriptor( spIaafFilmDescriptor )
{}

AxFilmDescriptor::~AxFilmDescriptor()
{}

void AxFilmDescriptor::SetFilmManufacturer( const AxString& manufacturer )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetFilmManufacturer( manufacturer.c_str() )
    );
}

AxString AxFilmDescriptor::GetFilmManufacturer()
{
    AxString manufacturer;

    AX_ANY_TO_STRING( manufacturer,
                      _spIaafFilmDescriptor,
                      GetFilmManufacturerBufLen,
                      GetFilmManufacturer );

    return manufacturer;
}

void AxFilmDescriptor::SetFilmModel( const AxString& model )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetFilmModel( model.c_str() )
    );
}

AxString AxFilmDescriptor::GetFilmModel()
{
    AxString filmModel;

    AX_ANY_TO_STRING( filmModel,
                      _spIaafFilmDescriptor,
                      GetFilmModelBufLen,
                      GetFilmModel );

    return filmModel;
}

void AxFilmDescriptor::SetFilmFormat( aafFilmType_t filmFormat )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetFilmFormat( filmFormat )
    );
}

aafFilmType_t AxFilmDescriptor::GetFilmFormat()
{
    aafFilmType_t filmFormat;
    CHECK_HRESULT(
        _spIaafFilmDescriptor->GetFilmFormat( &filmFormat ) );
    return filmFormat;
}

void AxFilmDescriptor::SetFrameRate( aafUInt32 rate )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetFrameRate( rate )
    );
}

aafUInt32 AxFilmDescriptor::GetFrameRate()
{
    aafUInt32 frameRate;
    CHECK_HRESULT(
        _spIaafFilmDescriptor->GetFrameRate( &frameRate ) );
    return frameRate;
}

void AxFilmDescriptor::SetPerfPerFrame( aafUInt8 perfPerFrame )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetPerfPerFrame( perfPerFrame )
    );
}

aafUInt8 AxFilmDescriptor::GetPerfPerFrame()
{
    aafUInt8 perfPerFrame;
    CHECK_HRESULT(
        _spIaafFilmDescriptor->GetPerfPerFrame( &perfPerFrame ) );
    return perfPerFrame;
}

void AxFilmDescriptor::SetFilmAspectRatio( aafRational_t aspectRatio )
{
    CHECK_HRESULT(
        _spIaafFilmDescriptor->SetFilmAspectRatio( aspectRatio )
    );
}

aafRational_t AxFilmDescriptor::GetFilmAspectRatio()
{
    aafRational_t aspectRatio;
    CHECK_HRESULT(
        _spIaafFilmDescriptor->GetFilmAspectRatio( &aspectRatio ) );
    return aspectRatio;
}

//=---------------------------------------------------------------------=

AxPhysicalDescriptor::AxPhysicalDescriptor( IAAFPhysicalDescriptorSP spIaafPhysicalDescriptor )
:   AxEssenceDescriptor( AxQueryInterface<IAAFPhysicalDescriptor, IAAFEssenceDescriptor>(spIaafPhysicalDescriptor) ),
    _spIaafPhysicalDescriptor( spIaafPhysicalDescriptor )
{}

AxPhysicalDescriptor::~AxPhysicalDescriptor()
{}

//=---------------------------------------------------------------------=

AxImportDescriptor::AxImportDescriptor( IAAFImportDescriptorSP spIaafImportDescriptor )
:   AxPhysicalDescriptor( AxQueryInterface<IAAFImportDescriptor, IAAFPhysicalDescriptor>(spIaafImportDescriptor) ),
    _spIaafImportDescriptor( spIaafImportDescriptor )
{}

AxImportDescriptor::~AxImportDescriptor()
{}

void AxImportDescriptor::Initialize()
{
    // noop
}

//=---------------------------------------------------------------------=

AxRecordingDescriptor::AxRecordingDescriptor( IAAFRecordingDescriptorSP spIaafRecordingDescriptor )
:   AxPhysicalDescriptor( AxQueryInterface<IAAFRecordingDescriptor, IAAFPhysicalDescriptor>(spIaafRecordingDescriptor) ),
    _spIaafRecordingDescriptor( spIaafRecordingDescriptor )
{}

AxRecordingDescriptor::~AxRecordingDescriptor()
{}

void AxRecordingDescriptor::Initialize()
{
    // noop
}

//=---------------------------------------------------------------------=

AxEssenceFormat::AxEssenceFormat( IAAFEssenceFormatSP sp )
:	_spIaafEssenceFormat( sp )
{}

AxEssenceFormat::~AxEssenceFormat()
{}

void AxEssenceFormat::AddFormatSpecifier( const aafUID_t& essenceFormatCode,
					  aafInt32 valueSize,
					  aafDataBuffer_t pValue )
{
	CHECK_HRESULT( _spIaafEssenceFormat->AddFormatSpecifier( essenceFormatCode,
								 valueSize,
								 pValue ) );
}

void AxEssenceFormat::GetFormatSpecifier( const aafUID_t& essenceFormatCode,
										  aafInt32 valueSize,
										  aafDataBuffer_t pValue,
										  aafInt32& bytesRead )
{
	CHECK_HRESULT( _spIaafEssenceFormat->GetFormatSpecifier( essenceFormatCode,
					 valueSize,
					 pValue,
					 &bytesRead ) );
}

//=---------------------------------------------------------------------=

