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


IEnumAAFComponentsSP AxSequence::GetComponents()
{
	IEnumAAFComponentsSP spIaafComponents;

	CHECK_HRESULT( _spIaafSequence->GetComponents( &spIaafComponents ) );

	return spIaafComponents;
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
