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

#include "AxComponent.h"

//=---------------------------------------------------------------------=

AxComponent::AxComponent( IAAFComponentSP spIaafComponent )
:	AxObject( AxQueryInterface<IAAFComponent, IAAFObject>( spIaafComponent ) ),
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
