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
:	AxObject( AxQueryInterface<IAAFComponent, IAAFObject>(
				spIaafComponent, IID_IAAFObject) ),
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
:	AxComponent( AxQueryInterface<IAAFSegment, IAAFComponent>(
					spIaafSegment, IID_IAAFComponent) ),
	_spIaafSegment( spIaafSegment )
{}

AxSegment::~AxSegment()
{}

//=---------------------------------------------------------------------=

AxTransition::AxTransition( IAAFTransitionSP spIaafTransition )
:	AxComponent( AxQueryInterface<IAAFTransition, IAAFComponent>(
					spIaafTransition, IID_IAAFComponent) ),
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
:	AxSegment( AxQueryInterface<IAAFSequence, IAAFSegment>(
					spIaafSequence, IID_IAAFSegment) ),
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

//=---------------------------------------------------------------------=

AxSourceReference::AxSourceReference( IAAFSourceReferenceSP spIaafSourceReference )
:	AxSegment( AxQueryInterface<IAAFSourceReference, IAAFSegment>(
					spIaafSourceReference, IID_IAAFSegment) ),
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

//=---------------------------------------------------------------------=

AxSourceClip::AxSourceClip( IAAFSourceClipSP spIaafSourceClip )
:	AxSourceReference( AxQueryInterface<IAAFSourceClip, IAAFSourceReference>(
						spIaafSourceClip, IID_IAAFSourceReference) ),
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

//=---------------------------------------------------------------------=

AxOperationGroup::AxOperationGroup( IAAFOperationGroupSP spIaafOperationGroup )
:	AxSegment( AxQueryInterface<IAAFOperationGroup, IAAFSegment>(
					spIaafOperationGroup, IID_IAAFSegment) ),
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
