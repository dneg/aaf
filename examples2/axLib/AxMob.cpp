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

#include "AxMob.h"

#include "AxUtil.h"

//=---------------------------------------------------------------------=

AxMob::AxMob( IAAFMobSP spIaafMob )
: AxObject( AxQueryInterface<IAAFMob, IAAFObject>( spIaafMob ) ),
	_spIaafMob( AxQueryInterface<IAAFMob, IAAFMob2>( spIaafMob ) )
{}

AxMob::AxMob( IAAFMob2SP spIaafMob )
: AxObject( AxQueryInterface<IAAFMob2, IAAFObject>( spIaafMob ) ),
	_spIaafMob( spIaafMob )
{}

AxMob::~AxMob()
{}

aafNumSlots_t AxMob::CountSlots()
{
	aafNumSlots_t numSlots;
	
	CHECK_HRESULT( _spIaafMob->CountSlots( &numSlots ) );

	return numSlots;
}

void AxMob::AppendSlot( IAAFMobSlotSP spSlot )
{
	CHECK_HRESULT( _spIaafMob->AppendSlot( spSlot ) );
}

aafMobID_t AxMob::GetMobID()
{
	aafMobID_t id;

	CHECK_HRESULT( _spIaafMob->GetMobID( &id ) );

	return id;
}

void AxMob::SetMobID( const aafMobID_t& mobID )
{
	CHECK_HRESULT( _spIaafMob->SetMobID( mobID ) );
}
	
AxString AxMob::GetName()
{
	return AxNameToString<IAAFMob2>( _spIaafMob );	
}

AxString AxMob::GetName( const AxString& defaul )
{
	return AxNameToString<IAAFMob2>( _spIaafMob, defaul );
}

void AxMob::SetName( const AxString& name )
{
	CHECK_HRESULT( _spIaafMob->SetName( name.c_str() ) );
}

void AxMob::AppendComment( const AxString& category,
				  		   const AxString& comment )
{
	CHECK_HRESULT( _spIaafMob->AppendComment(
		const_cast<aafCharacter*>( category.c_str() ),
		comment.c_str() ) );
}

void AxMob::AppendKLVData( const IAAFKLVDataSP& spIaafKLVData )
{
	CHECK_HRESULT( _spIaafMob->AppendKLVData( spIaafKLVData ) );
}


IAAFTimelineMobSlotSP AxMob::AppendNewTimelineSlot(
	aafRational_t  editRate,
	IAAFSegmentSP spIaafSegment,
	aafSlotID_t  slotID,
	const AxString& slotName,
	aafPosition_t  origin )
{
	IAAFTimelineMobSlotSP spSlot;

	CHECK_HRESULT( _spIaafMob->AppendNewTimelineSlot( editRate,
							  spIaafSegment,
							  slotID,
							  slotName.c_str(),
							  origin,
							  &spSlot ) );

	return spSlot;
}

IEnumAAFMobSlotsSP AxMob::GetSlots()
{
	IEnumAAFMobSlotsSP spIEnumAAFMobSlots;

	CHECK_HRESULT( _spIaafMob->GetSlots( &spIEnumAAFMobSlots ) );

	return spIEnumAAFMobSlots;
}

IAAFMobSlotSP AxMob::LookupSlot( aafSlotID_t slotId )
{
	IAAFMobSlotSP spIAAFMobSlot;

	CHECK_HRESULT( _spIaafMob->LookupSlot( slotId, &spIAAFMobSlot ) );

	return spIAAFMobSlot;
}

void AxMob::SetUsageCode( const aafUID_t& usageCode )
{
  CHECK_HRESULT( _spIaafMob->SetUsageCode( usageCode ) );
}

aafUID_t AxMob::GetUsageCode()
{
  aafUID_t usageCode;

  CHECK_HRESULT( _spIaafMob->GetUsageCode( &usageCode ) );

  return usageCode;
}

aafUID_t AxMob::GetUsageCode( const aafUID_t& defaul )
{
  aafUID_t usageCode;

  const AAFRESULT hr = _spIaafMob->GetUsageCode( &usageCode );

  if ( AAFRESULT_PROP_NOT_PRESENT == hr )
  {
    return defaul;
  }

  CHECK_HRESULT(hr);

  return usageCode;
}

std::pair<bool,aafUID_t> AxMob::ExistsUsageCode()
{
  std::pair<bool,aafUID_t> usageCode(false, AxConstants::NULL_UID);

  const AAFRESULT hr = _spIaafMob->GetUsageCode( &usageCode.second );

  if ( AAFRESULT_PROP_NOT_PRESENT == hr )
  {
    return usageCode;
  }

  CHECK_HRESULT(hr);

  usageCode.first = true;

  return usageCode;
}

IAAFMobSP AxMob::Copy( const AxString& newMobName )
{
	IAAFMobSP spNewMob;

	CHECK_HRESULT( _spIaafMob->Copy( newMobName.c_str(), &spNewMob ) );

	return spNewMob;
}

IAAFMobSP AxMob::CloneExternal( aafDepend_t resolveDependencies,
								aafIncMedia_t includeMedia,
								IAAFFileSP spDestFile )
{
	IAAFMobSP spDstMob;

	CHECK_HRESULT( _spIaafMob->CloneExternal( resolveDependencies,
											  includeMedia,
											  spDestFile,
											  &spDstMob ) );

	return spDstMob;
}

//=---------------------------------------------------------------------=

AxSearchSource::AxSearchSource( IAAFSearchSourceSP spIaafSearchSource )
: _spIaafSearchSource( spIaafSearchSource )
{}

AxSearchSource::~AxSearchSource()
{}

IAAFFindSourceInfoSP AxSearchSource::SearchSource (aafSlotID_t slotID,
				aafPosition_t  offset, aafMobKind_t  mobKind,
				aafMediaCriteria_t *  pMediaCrit,
				aafOperationChoice_t *  pOperationChoice)
{
	IAAFFindSourceInfoSP spIAAFFindSourceInfo;

	CHECK_HRESULT( _spIaafSearchSource->SearchSource(slotID, offset,
					mobKind, pMediaCrit, pOperationChoice,
					&spIAAFFindSourceInfo));

	return spIAAFFindSourceInfo;
}

//=---------------------------------------------------------------------=

AxMasterMob::AxMasterMob( IAAFMasterMobSP spIaafMasterMob )
:	AxMob( AxQueryInterface<IAAFMasterMob,IAAFMob2>( spIaafMasterMob ) ),
    AxSearchSource( AxQueryInterface<IAAFMasterMob, IAAFSearchSource>(spIaafMasterMob) ),
	_spIaafMasterMob( AxQueryInterface<IAAFMasterMob,IAAFMasterMob2>(spIaafMasterMob) )
{}

AxMasterMob::AxMasterMob( IAAFMasterMob2SP spIaafMasterMob )
:	AxMob( AxQueryInterface<IAAFMasterMob2,IAAFMob2>( spIaafMasterMob ) ),
    AxSearchSource( AxQueryInterface<IAAFMasterMob2, IAAFSearchSource>(spIaafMasterMob) ),
	_spIaafMasterMob( spIaafMasterMob )
{}


AxMasterMob::~AxMasterMob()
{}

void AxMasterMob::Initialize()
{
	CHECK_HRESULT( _spIaafMasterMob->Initialize() );
}

IAAFEssenceAccessSP AxMasterMob::CreateEssence(	aafSlotID_t  masterSlotID,
						IAAFDataDefSP pMediaKind,
						aafUID_constref  codecID,
						aafRational_t  editRate,
						aafRational_t  samplerate,
						aafCompressEnable_t  Enable,
						IAAFLocatorSP destination,
						aafUID_constref fileFormat )
{
	IAAFEssenceAccessSP spIaafEssenceAccess;

	CHECK_HRESULT( _spIaafMasterMob->CreateEssence( masterSlotID, pMediaKind, codecID,
							editRate, samplerate, Enable,
							destination, fileFormat,
							&spIaafEssenceAccess ) );

	return spIaafEssenceAccess;
}
	
void AxMasterMob::AddMasterSlot(
		IAAFDataDefSP		spDataDef,
		aafSlotID_t			sourceSlotID,
		IAAFSourceMobSP		spSourceMob,
		aafSlotID_t			masterSlotID,
		const AxString&		slotName )
{
	CHECK_HRESULT( _spIaafMasterMob->AddMasterSlot( spDataDef, sourceSlotID, spSourceMob, masterSlotID, slotName.c_str() ) );
}

//=---------------------------------------------------------------------=

AxMasterMobEx::AxMasterMobEx( IAAFMasterMobExSP spIaafMasterMobEx )
:	AxMasterMob( AxQueryInterface<IAAFMasterMobEx, IAAFMasterMob>(spIaafMasterMobEx) ),
	_spIaafMasterMobEx( spIaafMasterMobEx )
{}

AxMasterMobEx::~AxMasterMobEx()
{}

IAAFEssenceAccessSP AxMasterMobEx::ExtendEssence( aafSlotID_t  masterSlotID,
						  IAAFDataDefSP pMediaKind,
						  aafUID_constref  codecID,
						  aafRational_t  editRate,
						  aafRational_t  samplerate,
						  aafCompressEnable_t  Enable,
						  IAAFLocatorSP destination,
						  aafUID_constref fileFormat )
{
	IAAFEssenceAccessSP spIaafEssenceAccess;

	CHECK_HRESULT( _spIaafMasterMobEx->ExtendEssence( masterSlotID, pMediaKind, codecID,
							  editRate, samplerate, Enable,
							  destination, fileFormat,
							  &spIaafEssenceAccess ) );

	return spIaafEssenceAccess;
}
							 
//=---------------------------------------------------------------------=

IAAFEssenceAccessSP AxMasterMob::OpenEssence( aafSlotID_t		slotId,
					      aafMediaCriteria_t*	mediaCrit,	
					      aafMediaOpenMode_t	openMode,
					      aafCompressEnable_t	compEnable )
{
	IAAFEssenceAccessSP spIaafEssenceAccess;

	CHECK_HRESULT( _spIaafMasterMob->OpenEssence( slotId, mediaCrit,
						      openMode, compEnable,
						      &spIaafEssenceAccess ) );

	return spIaafEssenceAccess;
}



AxCompositionMob::AxCompositionMob( IAAFCompositionMobSP spIaafCompositionMob )
:	AxMob( AxQueryInterface<IAAFCompositionMob,IAAFMob2>(spIaafCompositionMob) ),
 	_spIaafCompositionMob( AxQueryInterface<IAAFCompositionMob,IAAFCompositionMob2>(spIaafCompositionMob) )
{}


AxCompositionMob::AxCompositionMob( IAAFCompositionMob2SP spIaafCompositionMob )
:	AxMob( AxQueryInterface<IAAFCompositionMob2,IAAFMob2>(spIaafCompositionMob) ),
 	_spIaafCompositionMob( spIaafCompositionMob )
{}

AxCompositionMob::~AxCompositionMob()
{}

void AxCompositionMob::Initialize( const AxString& name )
{
  CHECK_HRESULT( _spIaafCompositionMob->Initialize( name.c_str() ) );
}

//=---------------------------------------------------------------------=


AxSourceMob::AxSourceMob( IAAFSourceMobSP spIaafSourceMob )
:	AxMob( AxQueryInterface<IAAFSourceMob,IAAFMob>(spIaafSourceMob) ),
    AxSearchSource( AxQueryInterface<IAAFSourceMob, IAAFSearchSource>(spIaafSourceMob) ),
	_spIaafSourceMob( spIaafSourceMob )
{}

AxSourceMob::~AxSourceMob()
{}

void AxSourceMob::Initialize()
{
	CHECK_HRESULT( _spIaafSourceMob->Initialize() );
}

void AxSourceMob::SetEssenceDescriptor( IAAFEssenceDescriptorSP sp )
{
	CHECK_HRESULT( _spIaafSourceMob->SetEssenceDescriptor( sp ) );
}

IAAFEssenceDescriptorSP AxSourceMob::GetEssenceDescriptor()
{
	IAAFEssenceDescriptorSP spIaafEssenceDescriptor;

	CHECK_HRESULT( _spIaafSourceMob->GetEssenceDescriptor( &spIaafEssenceDescriptor ) );

	return spIaafEssenceDescriptor;
}

void AxSourceMob::AddNilReference( aafSlotID_t slotId,
								   aafLength_t len,
								   IAAFDataDefSP spDataDef,
								   const aafRational_t& editRate  )
{
	CHECK_HRESULT( _spIaafSourceMob->AddNilReference( slotId, len, spDataDef, editRate ) );
}

//=---------------------------------------------------------------------=
