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

#include "AxEssence.h"

AxEssenceMultiAccess::AxEssenceMultiAccess( IAAFEssenceMultiAccessSP& sp )
:	_spIaafEssenceMultiAccess( sp )
{}

AxEssenceMultiAccess::~AxEssenceMultiAccess()
{}

//=---------------------------------------------------------------------=

AxEssenceAccess::AxEssenceAccess( IAAFEssenceAccessSP& sp )
:	AxEssenceMultiAccess( AxQueryInterface<IAAFEssenceAccess, IAAFEssenceMultiAccess>(
							sp, IID_IAAFEssenceMultiAccess ) ),
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
	
pair<aafUInt32,aafUInt32> AxEssenceAccess::WriteSamples( aafUInt32 nSamples,
							 aafUInt32 bufLength,
							 aafDataBuffer_t  buffer )
{
	pair<aafUInt32,aafUInt32> ret;

	CHECK_HRESULT( _spIaafEssenceAccess->WriteSamples( nSamples,
							   bufLength,
							   buffer,
							   &ret.first,
							   &ret.second) );
	return ret;
}

void AxEssenceAccess::CompleteWrite()
{
	CHECK_HRESULT( _spIaafEssenceAccess->CompleteWrite() );
}

//=---------------------------------------------------------------------=

AxLocator::AxLocator( IAAFLocatorSP& spIaafLocator )
:	AxObject( AxQueryInterface<IAAFLocator, IAAFObject>(
				spIaafLocator, IID_IAAFObject ) ),
	_spIaafLocator( spIaafLocator )
{}

AxLocator::~AxLocator()
{}

//=---------------------------------------------------------------------=

AxDefObject::AxDefObject( IAAFDefObjectSP& spIaafDefObject )
:	AxObject( AxQueryInterface<IAAFDefObject, IAAFObject>(
				spIaafDefObject, IID_IAAFObject) ),
	_spIaafDefObject( spIaafDefObject )
{}

AxDefObject::~AxDefObject()
{}

aafUID_t AxDefObject::GetAUID()
{
	aafUID_t uid;

	CHECK_HRESULT( _spIaafDefObject->GetAUID( &uid ) );

	return uid;
}

//=---------------------------------------------------------------------=

AxDataDef::AxDataDef( IAAFDataDefSP& spIaafDataDef )
:	AxDefObject( AxQueryInterface<IAAFDataDef, IAAFDefObject>(
					spIaafDataDef, IID_IAAFDefObject ) ),
	_spIaafDataDef( spIaafDataDef )
{}

AxDataDef::~AxDataDef()
{}


bool AxDataDef::IsSoundKind()
{
	aafBoolean_t isSoundKind;

	CHECK_HRESULT( _spIaafDataDef->IsSoundKind( &isSoundKind ) );

	return isSoundKind ? true : false;
}

bool AxDataDef::IsPictureKind()
{
	aafBoolean_t isPictureKind;

	CHECK_HRESULT( _spIaafDataDef->IsPictureKind( &isPictureKind ) );

	return isPictureKind ? true : false;
}

//=---------------------------------------------------------------------=

AxOperationDef::AxOperationDef( IAAFOperationDefSP& spIaafOperationDef )
:	AxDefObject( AxQueryInterface<IAAFOperationDef, IAAFDefObject>(
					spIaafOperationDef, IID_IAAFDefObject ) ),
	_spIaafOperationDef( spIaafOperationDef )
{}

AxOperationDef::~AxOperationDef()
{}

AxOperationDef::Initialize( const aafUID_t& uid,
			    const AxString& name,
			    const AxString& desc )
{
	CHECK_HRESULT( _spIaafOperationDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxOperationDef::SetDataDef( IAAFDataDefSP spIaafDataDef  )
{
	CHECK_HRESULT( _spIaafOperationDef->SetDataDef( spIaafDataDef ) );
}

void AxOperationDef::SetIsTimeWarp( aafBoolean_t isWarp )
{
	CHECK_HRESULT( _spIaafOperationDef->SetIsTimeWarp( isWarp ) );
}

void AxOperationDef::SetCategory( const aafUID_t& category_auid )
{
	CHECK_HRESULT( _spIaafOperationDef->SetCategory( category_auid ) );
}

void AxOperationDef::SetNumberInputs( aafInt32 numInputs )
{
	CHECK_HRESULT( _spIaafOperationDef->SetNumberInputs( numInputs ) );
}

void AxOperationDef::SetBypass( aafUInt32 bypassTrack )
{
	CHECK_HRESULT( _spIaafOperationDef->SetBypass( bypassTrack ) );
}

//=---------------------------------------------------------------------=

AxEssenceDescriptor::AxEssenceDescriptor( IAAFEssenceDescriptorSP& sp )
:	AxObject( AxQueryInterface<IAAFEssenceDescriptor, IAAFObject>(
				sp, IID_IAAFObject ) ),
	_spIaafEssenceDescriptor( sp )
{}

AxEssenceDescriptor::~AxEssenceDescriptor()
{}

//=---------------------------------------------------------------------=

AxFileDescriptor::AxFileDescriptor( IAAFFileDescriptorSP& sp )
:	AxEssenceDescriptor( AxQueryInterface<IAAFFileDescriptor, IAAFEssenceDescriptor>(
						sp, IID_IAAFEssenceDescriptor ) ),
	_spIaafFileDescriptor( sp )
{}

AxFileDescriptor::~AxFileDescriptor()
{}

//=---------------------------------------------------------------------=

AxWAVEDescriptor::AxWAVEDescriptor( IAAFWAVEDescriptorSP& sp )
:	AxFileDescriptor( AxQueryInterface<IAAFWAVEDescriptor, IAAFFileDescriptor>(
					sp, IID_IAAFFileDescriptor ) ),
	_spIaafWAVEDescriptor( sp )
{}

AxWAVEDescriptor::~AxWAVEDescriptor()
{}

void AxWAVEDescriptor::SetSummary( aafUInt32 size, aafDataValue_t  pSummary )
{
	CHECK_HRESULT( _spIaafWAVEDescriptor->SetSummary( size, pSummary ) );
}

//=---------------------------------------------------------------------=

AxDigitalImageDescriptor::AxDigitalImageDescriptor( IAAFDigitalImageDescriptorSP& sp )
:	AxFileDescriptor( AxQueryInterface<IAAFDigitalImageDescriptor, IAAFFileDescriptor>(
					sp, IID_IAAFFileDescriptor ) ),
	_spIaafDigitalImageDescriptor( sp )
{}

AxDigitalImageDescriptor::~AxDigitalImageDescriptor()
{}

void AxDigitalImageDescriptor::SetStoredView( aafUInt32 StoredHeight, aafUInt32 StoredWidth)
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetStoredView( StoredHeight, StoredWidth) );
}

void AxDigitalImageDescriptor::SetSampledView( aafUInt32 SampledHeight, aafUInt32 SampledWidth,
					       aafUInt32 SampledXOffset, aafUInt32 SampledYOffset )
{
	CHECK_HRESULT( _spIaafDigitalImageDescriptor->SetSampledView( SampledHeight, SampledWidth,
								      SampledXOffset, SampledYOffset ) );
}

void AxDigitalImageDescriptor::SetDisplayView( aafUInt32 DisplayedHeight, aafUInt32 DisplayedWidth,
					       aafUInt32 DisplayedXOffset, aafUInt32 DisplayedYOffset )
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

//=---------------------------------------------------------------------=

AxCDCIDescriptor::AxCDCIDescriptor( IAAFCDCIDescriptorSP& sp )
:	AxDigitalImageDescriptor( AxQueryInterface<IAAFCDCIDescriptor, IAAFDigitalImageDescriptor>(
				sp, IID_IAAFDigitalImageDescriptor ) ),
	_spIaafCDCIDescriptor( sp )
{}

AxCDCIDescriptor::~AxCDCIDescriptor()
{}

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

void AxCDCIDescriptor::SetColorRange( aafUInt32 ColorRange )
{
	CHECK_HRESULT( _spIaafCDCIDescriptor->SetColorRange( ColorRange ) );
}

//=---------------------------------------------------------------------=

AxEssenceFormat::AxEssenceFormat( IAAFEssenceFormatSP& sp )
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


//=---------------------------------------------------------------------=

