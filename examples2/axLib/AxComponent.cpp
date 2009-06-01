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

#include "AxComponent.h"

//=---------------------------------------------------------------------=

AxComponent::AxComponent( IAAFComponentSP spIaafComponent )
: AxObject( AxQueryInterface<IAAFComponent, IAAFObject>( spIaafComponent ) ),
    _spIaafComponent( AxQueryInterface<IAAFComponent, IAAFComponent2>( spIaafComponent ) )
{}

AxComponent::AxComponent( IAAFComponent2SP spIaafComponent )
: AxObject( AxQueryInterface<IAAFComponent2, IAAFObject>( spIaafComponent ) ),
    _spIaafComponent( spIaafComponent )
{}

AxComponent::~AxComponent()
{}

aafLength_t AxComponent::GetLength()
{
	aafLength_t len;

	CHECK_HRESULT( _spIaafComponent->GetLength( &len ) );

	return len;
}

std::pair<bool,aafLength_t> AxComponent::ExistsLength()
{
  std::pair<bool,aafLength_t> len(true,0);

  AAFRESULT hresult = _spIaafComponent->GetLength( &len.second );
  if ( hresult == AAFRESULT_PROP_NOT_PRESENT )
  {
    len.first = false;
  }
  else
  {
    CHECK_HRESULT( hresult );
  }

  return len;
}

void AxComponent::SetLength( const aafLength_t& len )
{
	CHECK_HRESULT( _spIaafComponent->SetLength( len ) );
}

void AxComponent::SetDataDef( IAAFDataDefSP spIaafDataDef )
{
	CHECK_HRESULT( _spIaafComponent->SetDataDef( spIaafDataDef ) );
}

IAAFDataDefSP AxComponent::GetDataDef()
{
	IAAFDataDefSP spIaafDataDef;

	CHECK_HRESULT( _spIaafComponent->GetDataDef( &spIaafDataDef ) );

	return spIaafDataDef;
}

void AxComponent::AppendKLVData( IAAFKLVDataSP klvData )
{
    CHECK_HRESULT( _spIaafComponent->AppendKLVData( klvData ) );
}

void AxComponent::AppendComment( const AxString& name, const AxString& value )
{
    CHECK_HRESULT( _spIaafComponent->AppendComment( name.c_str(), value.c_str() ) );
}

void AxComponent::AppendAttribute( const AxString& name, const AxString& value )
{
    CHECK_HRESULT( _spIaafComponent->AppendAttribute( name.c_str(), value.c_str() ) );
}

aafUInt32 AxComponent::CountKLVData()
{
    aafUInt32 numKLVData;

    CHECK_HRESULT( _spIaafComponent->CountKLVData( &numKLVData ) );

    return numKLVData;
}

aafUInt32 AxComponent::CountComments()
{
    aafUInt32 numComments;

    CHECK_HRESULT( _spIaafComponent->CountComments( &numComments ) );

    return numComments;
}

aafUInt32 AxComponent::CountAttributes()
{
    aafUInt32 numAttribs;

    CHECK_HRESULT( _spIaafComponent->CountAttributes( &numAttribs ) );

    return numAttribs;
}

IEnumAAFKLVDataSP AxComponent::GetKLVData()
{
    IEnumAAFKLVDataSP spIaafKLVData;

    CHECK_HRESULT( _spIaafComponent->GetKLVData( &spIaafKLVData ) );

    return spIaafKLVData;
}

IEnumAAFTaggedValuesSP AxComponent::GetComments()
{
    IEnumAAFTaggedValuesSP spIaafTaggedValues;

    CHECK_HRESULT( _spIaafComponent->GetComments( &spIaafTaggedValues ) );

    return spIaafTaggedValues;
}

IEnumAAFTaggedValuesSP AxComponent::GetAttributes()
{
    IEnumAAFTaggedValuesSP spIaafTaggedValues;

    CHECK_HRESULT( _spIaafComponent->GetAttributes( &spIaafTaggedValues ) );

    return spIaafTaggedValues;
}

void AxComponent::RemoveKLVData( IAAFKLVDataSP klvpData )
{
    CHECK_HRESULT( _spIaafComponent->RemoveKLVData( klvpData ) );
}

void AxComponent::RemoveComment( IAAFTaggedValueSP comment )
{
    CHECK_HRESULT( _spIaafComponent->RemoveComment( comment ) );
}

void AxComponent::RemoveAttribute( IAAFTaggedValueSP attribute )
{
    CHECK_HRESULT( _spIaafComponent->RemoveAttribute( attribute ) );
}

//=---------------------------------------------------------------------=

AxSegment::AxSegment( IAAFSegmentSP spIaafSegment )
:	AxComponent( AxQueryInterface<IAAFSegment, IAAFComponent>(spIaafSegment) ),
	_spIaafSegment( spIaafSegment )
{}

AxSegment::~AxSegment()
{}

//=---------------------------------------------------------------------=

AxTransition::AxTransition( IAAFTransitionSP spIaafTransition )
:	AxComponent( AxQueryInterface<IAAFTransition, IAAFComponent>( spIaafTransition ) ),
	_spIaafTransition( spIaafTransition )
{}

AxTransition::~AxTransition()
{}

void AxTransition::Initialize( IAAFDataDefSP spDataDef,
			       aafLength_t length,
			       aafPosition_t cutPoint,
			       IAAFOperationGroupSP spIaafOpGroup )
{
	CHECK_HRESULT(
		_spIaafTransition->Initialize(
			spDataDef, length, cutPoint, spIaafOpGroup ) );
}

aafPosition_t AxTransition::GetCutPoint()
{
	aafPosition_t cutPoint;
	CHECK_HRESULT( _spIaafTransition->GetCutPoint( &cutPoint ) );
	return cutPoint;
}

void AxTransition::SetCutPoint( aafPosition_t cutPoint )
{
	CHECK_HRESULT( _spIaafTransition->SetCutPoint( cutPoint ) );
}

IAAFOperationGroupSP AxTransition::GetOperationGroup()
{
  IAAFOperationGroupSP spOpGroup;
  CHECK_HRESULT( _spIaafTransition->GetOperationGroup( &spOpGroup ) );
  return spOpGroup;
}

void AxTransition::SetOperationGroup( IAAFOperationGroupSP spOpGroup )
{
  CHECK_HRESULT( _spIaafTransition->SetOperationGroup( spOpGroup ) );
}

//=---------------------------------------------------------------------=

AxSequence::AxSequence( IAAFSequenceSP spIaafSequence )
:	AxSegment( AxQueryInterface<IAAFSequence, IAAFSegment>( spIaafSequence ) ),
	_spIaafSequence( spIaafSequence )
{}

AxSequence::~AxSequence()
{}

void AxSequence::Initialize( IAAFDataDefSP spIaafDataDef )
{
	CHECK_HRESULT( _spIaafSequence->Initialize( spIaafDataDef ) );
}

void AxSequence::AppendComponent( IAAFComponentSP spIaafComponent )
{
	CHECK_HRESULT( _spIaafSequence->AppendComponent( spIaafComponent ) );
}

void AxSequence::PrependComponent( IAAFComponentSP spIaafComponent )
{
	CHECK_HRESULT( _spIaafSequence->PrependComponent( spIaafComponent ) );
}

IEnumAAFComponentsSP AxSequence::GetComponents()
{
	IEnumAAFComponentsSP spIaafComponents;

	CHECK_HRESULT( _spIaafSequence->GetComponents( &spIaafComponents ) );

	return spIaafComponents;
}

IAAFComponentSP AxSequence::GetComponentAt( aafUInt32 index )
{
	IAAFComponentSP spIaafComponent;

	CHECK_HRESULT( _spIaafSequence->GetComponentAt( index, &spIaafComponent ) );

	return spIaafComponent;
}

void AxSequence::InsertComponentAt( aafUInt32 index,
				    IAAFComponentSP spIaafComponent )
{
	CHECK_HRESULT( _spIaafSequence->InsertComponentAt( index, spIaafComponent ) );
}

void AxSequence::RemoveComponentAt( aafUInt32 index )
{
	CHECK_HRESULT( _spIaafSequence->RemoveComponentAt( index ) );
}

aafUInt32 AxSequence::CountComponents()
{
	aafUInt32 numComponents;

	CHECK_HRESULT( _spIaafSequence->CountComponents( &numComponents ) );

	return numComponents;
}

//=---------------------------------------------------------------------=

AxTimecode::AxTimecode( IAAFTimecodeSP spIaafTimecode )
:	AxSegment( AxQueryInterface<IAAFTimecode, IAAFSegment>( spIaafTimecode ) ),
	_spIaafTimecode( spIaafTimecode )
{}

AxTimecode::~AxTimecode()
{}

void AxTimecode::Initialize( aafLength_t length, const aafTimecode_t& timecode )
{
	CHECK_HRESULT(
		_spIaafTimecode->Initialize( length,
									 const_cast<aafTimecode_t*>(&timecode) )
	);
}

aafTimecode_t AxTimecode::GetTimecode()
{
	aafTimecode_t timecode;

	CHECK_HRESULT( _spIaafTimecode->GetTimecode(&timecode) );

	return timecode;
}

void AxTimecode::SetTimecode( const aafTimecode_t& timecode )
{
	CHECK_HRESULT( _spIaafTimecode->SetTimecode( const_cast<aafTimecode_t*>(&timecode) ) );
}

//=---------------------------------------------------------------------=

AxFiller::AxFiller( IAAFFillerSP spIaafFiller )
:	AxSegment( AxQueryInterface<IAAFFiller, IAAFSegment>( spIaafFiller ) ),
	_spIaafFiller( spIaafFiller )
{}

AxFiller::~AxFiller()
{}

void AxFiller::Initialize( IAAFDataDefSP spDataDef, aafLength_t length )
{
	CHECK_HRESULT( _spIaafFiller->Initialize( spDataDef, length ) );
}

//=---------------------------------------------------------------------=

AxPulldown::AxPulldown( IAAFPulldownSP spIaafPulldown )
:	AxSegment( AxQueryInterface<IAAFPulldown, IAAFSegment>( spIaafPulldown ) ),
	_spIaafPulldown( spIaafPulldown )
{}

AxPulldown::~AxPulldown()
{}

IAAFSegmentSP AxPulldown::GetInputSegment()
{
	IAAFSegmentSP spIaafSegment;

	CHECK_HRESULT(_spIaafPulldown->GetInputSegment(&spIaafSegment));

	return spIaafSegment;
}
aafPulldownKind_t AxPulldown::GetPulldownKind()
{
	aafPulldownKind_t pdkind;

	CHECK_HRESULT(_spIaafPulldown->GetPulldownKind(&pdkind));

	return pdkind;
}
aafPulldownDir_t AxPulldown::GetPulldownDirection()
{
	aafPulldownDir_t pddir;

	CHECK_HRESULT(_spIaafPulldown->GetPulldownDirection(&pddir));

	return pddir;
}
aafPhaseFrame_t AxPulldown::GetPhaseFrame()
{
	aafPhaseFrame_t pdphase;

	CHECK_HRESULT(_spIaafPulldown->GetPhaseFrame(&pdphase));

	return pdphase;
}
void AxPulldown::SetInputSegment( IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT(_spIaafPulldown->SetInputSegment(spIaafSegment));
}
void AxPulldown::SetPulldownKind( aafPulldownKind_t pdkind )
{
	CHECK_HRESULT(_spIaafPulldown->SetPulldownKind(pdkind));
}
void AxPulldown::SetPulldownDirection( aafPulldownDir_t pddir )
{
	CHECK_HRESULT(_spIaafPulldown->SetPulldownDirection(pddir));
}
void AxPulldown::SetPhaseFrame( aafPhaseFrame_t pdphase)
{
	CHECK_HRESULT(_spIaafPulldown->SetPhaseFrame(pdphase));
}

//=---------------------------------------------------------------------=

AxSourceReference::AxSourceReference( IAAFSourceReferenceSP spIaafSourceReference )
:	AxSegment( AxQueryInterface<IAAFSourceReference, IAAFSegment>( spIaafSourceReference ) ),
	_spIaafSourceReference( spIaafSourceReference )
{}

AxSourceReference::~AxSourceReference()
{}

aafMobID_t AxSourceReference::GetSourceID()
{
	aafMobID_t uid;

	CHECK_HRESULT( _spIaafSourceReference->GetSourceID( &uid ) );

	return uid;
}

void AxSourceReference::SetSourceID( const aafMobID_t& id )
{
	CHECK_HRESULT( _spIaafSourceReference->SetSourceID( id ) );
}

aafSlotID_t AxSourceReference::GetSourceMobSlotID()
{
	aafSlotID_t slotid;

	CHECK_HRESULT( _spIaafSourceReference->GetSourceMobSlotID( &slotid ) );

	return slotid;
}

void AxSourceReference::SetSourceMobSlotID( const aafSlotID_t& slotid )
{
	CHECK_HRESULT( _spIaafSourceReference->SetSourceMobSlotID( slotid ) );
}

//=---------------------------------------------------------------------=

AxSourceClip::AxSourceClip( IAAFSourceClipSP spIaafSourceClip )
:	AxSourceReference( AxQueryInterface<IAAFSourceClip, IAAFSourceReference>(spIaafSourceClip) ),
	_spIaafSourceClip( spIaafSourceClip )
{}

AxSourceClip::~AxSourceClip()
{}

void AxSourceClip::Initialize( IAAFDataDefSP spIaafDataDef,
			       const aafLength_t& length,
			       const aafSourceRef_t& sourceRef )
{
	CHECK_HRESULT( _spIaafSourceClip->Initialize(
			spIaafDataDef, length, sourceRef ) );
}

void AxSourceClip::SetSourceReference( const aafSourceRef_t& ref )
{
  CHECK_HRESULT( _spIaafSourceClip->SetSourceReference( ref ) );
}

aafSourceRef_t AxSourceClip::GetSourceReference()
{
	aafSourceRef_t ref;

	CHECK_HRESULT( _spIaafSourceClip->GetSourceReference( &ref ) );

	return ref;
}

IAAFMobSP AxSourceClip::ResolveRef()
{
	IAAFMobSP mob;

	CHECK_HRESULT(_spIaafSourceClip->ResolveRef(&mob));

	return mob;
}

//=---------------------------------------------------------------------=

AxOperationGroup::AxOperationGroup( IAAFOperationGroupSP spIaafOperationGroup )
:	AxSegment( AxQueryInterface<IAAFOperationGroup, IAAFSegment>(spIaafOperationGroup) ),
	_spIaafOperationGroup( spIaafOperationGroup )
{}

AxOperationGroup::~AxOperationGroup()
{}

void AxOperationGroup::Initialize( IAAFDataDefSP spIaafDataDef,
				   aafLength_t length,
				   IAAFOperationDefSP spIaafOperationDef )
{
	CHECK_HRESULT( _spIaafOperationGroup->Initialize( spIaafDataDef,
							  length,
							  spIaafOperationDef ) );
}

void AxOperationGroup::AppendInputSegment( IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafOperationGroup->AppendInputSegment( spIaafSegment ) );
}

void AxOperationGroup::AddParameter( IAAFParameterSP spIaafParameter )
{
    	CHECK_HRESULT( _spIaafOperationGroup->AddParameter( spIaafParameter ) );
}

void AxOperationGroup::PrependInputSegment( IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafOperationGroup->PrependInputSegment( spIaafSegment ) );
}

void AxOperationGroup::InsertInputSegmentAt( aafUInt32 index,
					     IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafOperationGroup->InsertInputSegmentAt( index,
								    spIaafSegment ) );
}

void AxOperationGroup::RemoveInputSegmentAt( aafUInt32 index )
{
	CHECK_HRESULT( _spIaafOperationGroup->RemoveInputSegmentAt( index ));
}

IAAFOperationDefSP AxOperationGroup::GetOperationDef()
{
	IAAFOperationDefSP operationDef;

	CHECK_HRESULT( _spIaafOperationGroup->GetOperationDefinition(&operationDef));

	return operationDef;
}

aafUInt32 AxOperationGroup::CountSourceSegments()
{
	aafUInt32 numSegments;

	CHECK_HRESULT( _spIaafOperationGroup->CountSourceSegments(&numSegments));

	return numSegments;
}

IAAFSegmentSP AxOperationGroup::GetInputSegmentAt(aafUInt32 segIndex)
{
	IAAFSegmentSP inputSeg;

	CHECK_HRESULT( _spIaafOperationGroup->GetInputSegmentAt(segIndex,
								&inputSeg));

	return inputSeg;
}

IAAFSourceReferenceSP AxOperationGroup::GetRender()
{
	IAAFSourceReferenceSP sourceRef;

	CHECK_HRESULT( _spIaafOperationGroup->GetRender(&sourceRef));

	return sourceRef;
}

void AxOperationGroup::SetRender( IAAFSourceReferenceSP spIaafSourceReference )
{
	CHECK_HRESULT( _spIaafOperationGroup->SetRender(spIaafSourceReference));
}

aafBoolean_t AxOperationGroup::IsATimeWarp()
{
	aafBoolean_t isATW;

	CHECK_HRESULT( _spIaafOperationGroup->IsATimeWarp(&isATW));

	return isATW;
}

aafBoolean_t AxOperationGroup::IsValidTranOperation()
{
	aafBoolean_t validTranOp;

	CHECK_HRESULT( _spIaafOperationGroup->IsValidTranOperation(&validTranOp));
	
	return validTranOp;
}

aafUInt32 AxOperationGroup::GetBypassOverride()
{
	aafUInt32 bypassOR;

	CHECK_HRESULT( _spIaafOperationGroup->GetBypassOverride(&bypassOR));

	return bypassOR;
}

void AxOperationGroup::SetBypassOverride( aafUInt32 bypassOR )
{
	CHECK_HRESULT( _spIaafOperationGroup->SetBypassOverride(bypassOR));
}

aafUInt32 AxOperationGroup::CountParameters()
{
	aafUInt32 numParams;

	CHECK_HRESULT( _spIaafOperationGroup->CountParameters(&numParams));

	return numParams;
}

IEnumAAFParametersSP AxOperationGroup::GetParameters()
{
	IEnumAAFParametersSP spIaafParameters;

	CHECK_HRESULT( _spIaafOperationGroup->GetParameters(&spIaafParameters));
	return spIaafParameters;
}

IAAFParameterSP AxOperationGroup::LookupParameter( aafArgIDType_constref argID )
{
	IAAFParameterSP spIaafParameter;

	CHECK_HRESULT( _spIaafOperationGroup->LookupParameter(argID, &spIaafParameter));

	return spIaafParameter;
}

void AxOperationGroup::SetOperationDef( IAAFOperationDefSP spIaafOperationDef )
{
  CHECK_HRESULT( _spIaafOperationGroup->SetOperationDefinition(spIaafOperationDef));
}

//=---------------------------------------------------------------------=

AxNestedScope::AxNestedScope( IAAFNestedScopeSP spIaafNestedScope )
:	AxSegment( AxQueryInterface<IAAFNestedScope, IAAFSegment>(spIaafNestedScope) ),
	_spIaafNestedScope( spIaafNestedScope )
{}

AxNestedScope::~AxNestedScope()
{}

void AxNestedScope::AppendSegment( IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafNestedScope->AppendSegment( spIaafSegment ) );
}

void AxNestedScope::PrependSegment( IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafNestedScope->PrependSegment( spIaafSegment ) );
}

void AxNestedScope::InsertSegmentAt( aafUInt32 index, IAAFSegmentSP spIaafSegment )
{
	CHECK_HRESULT( _spIaafNestedScope->InsertSegmentAt( index, spIaafSegment ) );
}

void AxNestedScope::RemoveSegmentAt( aafUInt32 index )
{
	CHECK_HRESULT( _spIaafNestedScope->RemoveSegmentAt( index ) );
}

aafUInt32 AxNestedScope::CountSegments()
{
	aafUInt32 numSegments;

	CHECK_HRESULT( _spIaafNestedScope->CountSegments(&numSegments));

	return numSegments;
}

IAAFSegmentSP AxNestedScope::GetSegmentAt( aafUInt32 index )
{
	IAAFSegmentSP segment;

	CHECK_HRESULT( _spIaafNestedScope->GetSegmentAt( index, &segment ) );

	return segment;
}

IEnumAAFSegmentsSP AxNestedScope::GetSegments()
{
	IEnumAAFSegmentsSP segEnum;

	CHECK_HRESULT( _spIaafNestedScope->GetSegments( &segEnum ) );

	return segEnum;
}

//=---------------------------------------------------------------------=

AxScopeReference::AxScopeReference( IAAFScopeReferenceSP spIaafScopeReference )
:	AxSegment( AxQueryInterface<IAAFScopeReference, IAAFSegment>(spIaafScopeReference) ),
	_spIaafScopeReference( spIaafScopeReference )
{}

AxScopeReference::~AxScopeReference()
{}

void AxScopeReference::Initialize( IAAFDataDefSP spIaafDataDef,
				   aafUInt32 relScope,
				   aafUInt32 relSlot )
{
  CHECK_HRESULT( _spIaafScopeReference->Initialize( spIaafDataDef,
						    relScope,
						    relSlot ) );
}

aafUInt32 AxScopeReference::GetRelativeScope()
{
	aafUInt32 relScope;

	CHECK_HRESULT( _spIaafScopeReference->GetRelativeScope( &relScope ) );

	return relScope;
}

aafUInt32 AxScopeReference::GetRelativeSlot()
{
	aafUInt32 relSlot;

	CHECK_HRESULT( _spIaafScopeReference->GetRelativeSlot( &relSlot ) );

	return relSlot;
}

//=---------------------------------------------------------------------=

AxEssenceGroup::AxEssenceGroup( IAAFEssenceGroupSP spIaafEssenceGroup )
:	AxSegment( AxQueryInterface<IAAFEssenceGroup, IAAFSegment>(spIaafEssenceGroup) ),
	_spIaafEssenceGroup( spIaafEssenceGroup )
{}

AxEssenceGroup::~AxEssenceGroup()
{}

IAAFSourceClipSP AxEssenceGroup::GetStillFrame()
{
  IAAFSourceClipSP spStillFrame;

  CHECK_HRESULT( _spIaafEssenceGroup->GetStillFrame( &spStillFrame ) );

  return spStillFrame;
}

aafUInt32 AxEssenceGroup::CountChoices()
{
  aafUInt32 count = 0;

  CHECK_HRESULT( _spIaafEssenceGroup->CountChoices( &count ) );

  return count;
}

IAAFSegmentSP AxEssenceGroup::GetChoiceAt( aafUInt32 index )
{
  IAAFSegmentSP spChoice;

  CHECK_HRESULT( _spIaafEssenceGroup->GetChoiceAt( index, &spChoice ) );

  return spChoice;
}

//=---------------------------------------------------------------------=

AxSelector::AxSelector( IAAFSelectorSP spIaafSelector )
:	AxSegment( AxQueryInterface<IAAFSelector, IAAFSegment>(spIaafSelector) ),
	_spIaafSelector( spIaafSelector )
{}

AxSelector::~AxSelector()
{}

IAAFSegmentSP AxSelector::GetSelectedSegment()
{
  IAAFSegmentSP spSelectedSegment;

  CHECK_HRESULT( _spIaafSelector->GetSelectedSegment( &spSelectedSegment ) );

  return spSelectedSegment;
}

IEnumAAFSegmentsSP AxSelector::EnumAlternateSegments()
{
  IEnumAAFSegmentsSP spEnumSegments;

  CHECK_HRESULT( _spIaafSelector->EnumAlternateSegments( &spEnumSegments  ) );

  return spEnumSegments;
}

//=---------------------------------------------------------------------=

AxEdgecode::AxEdgecode( IAAFEdgecodeSP spIaafEdgecode )
:	AxSegment( AxQueryInterface<IAAFEdgecode, IAAFSegment>(spIaafEdgecode
) ),
	_spIaafEdgecode( spIaafEdgecode )
{}

AxEdgecode::~AxEdgecode()
{}

void AxEdgecode::Initialize( aafLength_t length,
			     aafEdgecode_t edgecode )
{
	CHECK_HRESULT( _spIaafEdgecode->Initialize( length, edgecode ) );
}

aafEdgecode_t AxEdgecode::GetEdgecode()
{
	aafEdgecode_t edgecode;

	CHECK_HRESULT( _spIaafEdgecode->GetEdgecode( &edgecode ) );

	return edgecode;
}

//=---------------------------------------------------------------------=

AxEvent::AxEvent( IAAFEventSP spIaafEvent )
:	AxSegment( AxQueryInterface<IAAFEvent, IAAFSegment>(spIaafEvent) ),
	_spIaafEvent( spIaafEvent )
{}

AxEvent::~AxEvent()
{}

aafPosition_t AxEvent::GetPosition()
{
	aafPosition_t pos;

	CHECK_HRESULT( _spIaafEvent->GetPosition( &pos ) );

	return pos;
}

void AxEvent::SetPosition( aafPosition_t pos )
{
	CHECK_HRESULT( _spIaafEvent->SetPosition( pos ) );
}

void AxEvent::SetComment( const AxString& comment )
{
	CHECK_HRESULT( _spIaafEvent->SetComment( comment.c_str() ) );
}

AxString AxEvent::GetComment()
{
	AxString comment;

	// Yet another get string method with a "custom" name.
	AX_ANY_TO_STRING( comment, _spIaafEvent, GetCommentBufLen, GetComment );

	return comment;
}

//=---------------------------------------------------------------------=

AxCommentMarker::AxCommentMarker( IAAFCommentMarkerSP spIaafCommentMarker )
:	AxEvent( AxQueryInterface<IAAFCommentMarker, IAAFEvent>(spIaafCommentMarker) ),
	_spIaafCommentMarker( spIaafCommentMarker )
{}

AxCommentMarker::~AxCommentMarker()
{}

IAAFSourceReferenceSP AxCommentMarker::GetAnnotation()
{
	IAAFSourceReferenceSP spRef;

	CHECK_HRESULT( _spIaafCommentMarker->GetAnnotation( &spRef ) );

	return spRef;
}

void AxCommentMarker::SetAnnotation( IAAFSourceReferenceSP spIaafSourceReference )
{
	CHECK_HRESULT( _spIaafCommentMarker->SetAnnotation( spIaafSourceReference ) );
}

//=---------------------------------------------------------------------=

AxGPITrigger::AxGPITrigger( IAAFGPITriggerSP spIaafGPITrigger )
:	AxEvent( AxQueryInterface<IAAFGPITrigger, IAAFEvent>(spIaafGPITrigger) ),
	_spIaafGPITrigger( spIaafGPITrigger )
{}
    
AxGPITrigger::~AxGPITrigger()
{}

aafBoolean_t AxGPITrigger::GetActiveState()
{
	aafBoolean_t activeState;

	CHECK_HRESULT( _spIaafGPITrigger->GetActiveState( &activeState ) );

	return activeState;
}

void AxGPITrigger::SetActiveState( aafBoolean_t activeState )
{
	CHECK_HRESULT( _spIaafGPITrigger->SetActiveState( activeState ) );
}

//=---------------------------------------------------------------------=

AxTimecodeStream::AxTimecodeStream( IAAFTimecodeStreamSP spIaafTimecodeStream )
:   AxSegment( AxQueryInterface<IAAFTimecodeStream, IAAFSegment>(spIaafTimecodeStream) ),
    _spIaafTimecodeStream( spIaafTimecodeStream )
{}

AxTimecodeStream::~AxTimecodeStream()
{}

aafTimecode_t AxTimecodeStream::GetPositionTimecode ( aafPosition_t position )
{
    aafTimecode_t timecode;

    CHECK_HRESULT( _spIaafTimecodeStream->GetPositionTimecode( position, &timecode ) );

    return timecode;
}

aafInt32 AxTimecodeStream::GetUserDataLength ()
{
    aafInt32 length;

    CHECK_HRESULT( _spIaafTimecodeStream->GetUserDataLength( &length ) );

    return length;
}

aafRational_t AxTimecodeStream::GetSampleRate ()
{
    aafRational_t sampleRate;

    CHECK_HRESULT( _spIaafTimecodeStream->GetSampleRate( &sampleRate ) );

    return sampleRate;
}

aafTimecodeSourceType_t AxTimecodeStream::GetSourceType ()
{
    aafTimecodeSourceType_t sourceType;

    CHECK_HRESULT( _spIaafTimecodeStream->GetSourceType( &sourceType ) );

    return sourceType;
}

aafUInt32 AxTimecodeStream::GetSampleSize ()
{
    aafUInt32 sampleSize;

    CHECK_HRESULT( _spIaafTimecodeStream->GetSampleSize( &sampleSize ) );

    return sampleSize;
}

aafUInt32 AxTimecodeStream::GetSourceBufLen ()
{
    aafUInt32 sourceBufLen;

    CHECK_HRESULT( _spIaafTimecodeStream->GetSourceBufLen( &sourceBufLen ) );

    return sourceBufLen;
}

void AxTimecodeStream::GetUserDataAtPosition ( aafPosition_t position, aafInt32 buflen, aafDataBuffer_t buffer )
{
    CHECK_HRESULT( _spIaafTimecodeStream->GetUserDataAtPosition( position, buflen, buffer ) );
}

void AxTimecodeStream::GetSource( aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead )
{
    CHECK_HRESULT( _spIaafTimecodeStream->GetSource( valueSize, pValue, bytesRead ) );
}

void AxTimecodeStream::SetPositionTimecode ( aafPosition_t position, aafTimecode_t timecode)
{
    CHECK_HRESULT( _spIaafTimecodeStream->SetPositionTimecode( position, timecode ) );
}

void AxTimecodeStream::SetUserDataAtPosition ( aafPosition_t position, aafInt32 buflen, aafDataBuffer_t buffer )
{
    CHECK_HRESULT( _spIaafTimecodeStream->SetUserDataAtPosition( position, buflen, buffer ) );
}

void AxTimecodeStream::SetSampleRate ( aafRational_t sampleRate )
{
    CHECK_HRESULT( _spIaafTimecodeStream->SetSampleRate( sampleRate ) );
}

void AxTimecodeStream::SetSourceType ( aafTimecodeSourceType_t sourceType )
{
    CHECK_HRESULT( _spIaafTimecodeStream->SetSourceType( sourceType ) );
}

void AxTimecodeStream::SetSource (aafUInt32 valueSize, aafDataBuffer_t pValue)
{
    CHECK_HRESULT( _spIaafTimecodeStream->SetSource( valueSize, pValue ) );
}

//=---------------------------------------------------------------------=

AxTimecodeStream12M::AxTimecodeStream12M( IAAFTimecodeStream12MSP spIaafTimecodeStream12M )
:   AxTimecodeStream( AxQueryInterface<IAAFTimecodeStream12M, IAAFTimecodeStream>(spIaafTimecodeStream12M) ),
    _spIaafTimecodeStream12M( spIaafTimecodeStream12M )
{}

AxTimecodeStream12M::~AxTimecodeStream12M()
{}

//=---------------------------------------------------------------------=

AxDescriptiveMarker::AxDescriptiveMarker( IAAFDescriptiveMarkerSP spIaafDescriptiveMarker )
:   AxCommentMarker( AxQueryInterface<IAAFDescriptiveMarker, IAAFCommentMarker>(spIaafDescriptiveMarker) ),
    _spIaafDescriptiveMarker( spIaafDescriptiveMarker )
{}

AxDescriptiveMarker::~AxDescriptiveMarker()
{}

void AxDescriptiveMarker::Initialize()
{
    // noop
}

std::vector<aafUInt32> AxDescriptiveMarker::GetDescribedSlotIDs()
{
    aafUInt32 numberOfElements = 0;

    CHECK_HRESULT( _spIaafDescriptiveMarker->GetDescribedSlotIDsSize( &numberOfElements ) );
    
    std::vector<aafUInt32> describedSlotIds; 

    CHECK_HRESULT( _spIaafDescriptiveMarker->GetDescribedSlotIDs( numberOfElements, &(describedSlotIds[0]) ) ); 
    
    return describedSlotIds;
}

IAAFDescriptiveFrameworkSP AxDescriptiveMarker::GetDescriptiveFramework()
{
    IAAFDescriptiveFrameworkSP descriptiveFramework;

    CHECK_HRESULT( _spIaafDescriptiveMarker->GetDescriptiveFramework( &descriptiveFramework ) );

    return descriptiveFramework;
}

void AxDescriptiveMarker::SetDescribedSlotIDs( aafUInt32 numberElements, aafUInt32* describedSlotIDs )
{
    CHECK_HRESULT( _spIaafDescriptiveMarker->SetDescribedSlotIDs( numberElements, describedSlotIDs ) );
}

void AxDescriptiveMarker::SetDescriptiveFramework( IAAFDescriptiveFrameworkSP descriptiveFramework )
{
    CHECK_HRESULT( _spIaafDescriptiveMarker->SetDescriptiveFramework( descriptiveFramework ) );
}

//=---------------------------------------------------------------------=
