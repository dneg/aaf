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
	~AxComponent();

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
	~AxSegment();

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
	~AxTransition();

	void Initialize( IAAFDataDefSP, aafLength_t, aafPosition_t, IAAFOperationGroupSP);

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
	~AxSequence();

	void Initialize( IAAFDataDefSP spIaafDataDef );
	
	void AppendComponent( IAAFComponentSP );

	IEnumAAFComponentsSP GetComponents();

private:
	AxSequence();
	AxSequence( const AxSequence& );
	AxSequence& operator=( const AxSequence& );

	IAAFSequenceSP _spIaafSequence;
};


//=---------------------------------------------------------------------=

class AxSourceReference : public AxSegment {

public:
	AxSourceReference( IAAFSourceReferenceSP spIaafSourceReference );
	~AxSourceReference();

	// FIXME - Too large to return by value?
	aafMobID_t GetSourceID();

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
	~AxSourceClip();

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
	~AxOperationGroup();

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

#endif
