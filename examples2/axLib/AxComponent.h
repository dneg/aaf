#ifndef __AxComponent_h__
#define __AxComponent_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxComponent : public AxObject {

public:
	AxComponent( IAAFComponentSP spIaafComponent );
	virtual ~AxComponent();

	aafLength_t GetLength();
	void SetLength( const aafLength_t& len );


	void SetDataDef( IAAFDataDefSP );
	IAAFDataDefSP GetDataDef();

	operator IAAFComponentSP ()
	{ return _spIaafComponent; };

private:
	AxComponent();
	AxComponent( const AxComponent& );
	AxComponent& operator=( const AxComponent& );

	IAAFComponentSP _spIaafComponent;
};

//=---------------------------------------------------------------------=

class AxSegment : public AxComponent {

public:
	AxSegment( IAAFSegmentSP spIaafSegment );
	virtual ~AxSegment();

	operator IAAFSegmentSP ()
	{ return _spIaafSegment; }

private:
	AxSegment();
	AxSegment( const AxSegment& );
	AxSegment& operator=( const AxSegment& );

	IAAFSegmentSP _spIaafSegment;
};

//=---------------------------------------------------------------------=

class AxTransition : public AxComponent {

public:
	AxTransition( IAAFTransitionSP spIaafTransition );
	virtual ~AxTransition();

	void Initialize( IAAFDataDefSP, aafLength_t, aafPosition_t, IAAFOperationGroupSP);

	aafPosition_t GetCutPoint();
	void SetCutPoint( aafPosition_t cutPoint );
	
	operator IAAFTransitionSP ()
	{ return _spIaafTransition; }

private:
	AxTransition();
	AxTransition( const AxTransition& );
	AxTransition& operator=( const AxTransition& );

	IAAFTransitionSP _spIaafTransition;
};

//=---------------------------------------------------------------------=

class AxSequence : public AxSegment {

public:
	AxSequence( IAAFSequenceSP spIaafSequence );
	virtual ~AxSequence();

	void Initialize( IAAFDataDefSP spIaafDataDef );
	
	void AppendComponent( IAAFComponentSP );

	IEnumAAFComponentsSP GetComponents();

	aafUInt32 CountComponents();

private:
	AxSequence();
	AxSequence( const AxSequence& );
	AxSequence& operator=( const AxSequence& );

	IAAFSequenceSP _spIaafSequence;
};

//=---------------------------------------------------------------------=

class AxTimecode : public AxSegment {

public:
	AxTimecode( IAAFTimecodeSP spIaafTimecode );
	virtual ~AxTimecode();

    void Initialize( aafLength_t length, const aafTimecode_t& timecode );

	aafTimecode_t GetTimecode();
	void SetTimecode( const aafTimecode_t& timecode );

private:
	AxTimecode();
	AxTimecode( const AxTimecode& );
	AxTimecode& operator=( const AxTimecode& );

	IAAFTimecodeSP _spIaafTimecode;
};

//=---------------------------------------------------------------------=

class AxFiller : public AxSegment {

public:
	AxFiller( IAAFFillerSP spIaafFiller );
	virtual ~AxFiller();

    void Initialize( IAAFDataDefSP spDataDef, aafLength_t length );

private:
	AxFiller();
	AxFiller( const AxFiller& );
	AxFiller& operator=( const AxFiller& );

	IAAFFillerSP _spIaafFiller;
};

//=---------------------------------------------------------------------=

class AxSourceReference : public AxSegment {

public:
	AxSourceReference( IAAFSourceReferenceSP spIaafSourceReference );
	virtual ~AxSourceReference();

	aafMobID_t GetSourceID();
	void SetSourceID( const aafMobID_t& id );

	aafSlotID_t GetSourceMobSlotID();
	void SetSourceMobSlotID( const aafSlotID_t& slotid );

private:
	AxSourceReference();
	AxSourceReference( const AxSourceReference& );
	AxSourceReference& operator=( const AxSourceReference& );

	IAAFSourceReferenceSP _spIaafSourceReference;
};

//=---------------------------------------------------------------------=

class AxSourceClip : public AxSourceReference {

public:
	AxSourceClip( IAAFSourceClipSP spIaafSourceClip );
	virtual ~AxSourceClip();

	void Initialize( IAAFDataDefSP, const aafLength_t&, const aafSourceRef_t& );

	void SetSourceReference( const aafSourceRef_t& ref );
	
	operator IAAFSourceClipSP () 
	{ return _spIaafSourceClip; }

private:
	AxSourceClip();
	AxSourceClip( const AxSourceClip& );
	AxSourceClip& operator=( const AxSourceClip& );

	IAAFSourceClipSP _spIaafSourceClip;
};

//=---------------------------------------------------------------------=

class AxOperationGroup : public AxSegment {

public:
	AxOperationGroup( IAAFOperationGroupSP spIaafOperationGroup );
	virtual ~AxOperationGroup();

	void Initialize( IAAFDataDefSP, aafLength_t, IAAFOperationDefSP );

	void AppendInputSegment( IAAFSegmentSP );
	
	operator IAAFOperationGroupSP () 
	{ return _spIaafOperationGroup; }

private:
	AxOperationGroup();
	AxOperationGroup( const AxOperationGroup& );
	AxOperationGroup& operator=( const AxOperationGroup& );

	IAAFOperationGroupSP _spIaafOperationGroup;
};

//=---------------------------------------------------------------------=

class AxEvent : public AxSegment {

public:
	AxEvent( IAAFEventSP spIaafEvent );
	virtual ~AxEvent();

	aafPosition_t GetPosition();
	void SetPosition( aafPosition_t pos );

	void SetComment( const AxString& comment );
	AxString GetComment();

private:
	AxEvent();
	AxEvent( const AxEvent& );
	AxEvent& operator=( const AxEvent& );

	IAAFEventSP _spIaafEvent;
};

//=---------------------------------------------------------------------=

class AxCommentMarker : public AxEvent {

public:
	AxCommentMarker( IAAFCommentMarkerSP spIaafCommentMarker );
	virtual ~AxCommentMarker();

	IAAFSourceReferenceSP GetAnnotation();

	void SetAnnotation( IAAFSourceReferenceSP spIaafSourceReference );

private:
	AxCommentMarker();
	AxCommentMarker( const AxEvent& );
	AxCommentMarker& operator=( const AxCommentMarker& );

	IAAFCommentMarkerSP _spIaafCommentMarker;
};

//=---------------------------------------------------------------------=

#endif
