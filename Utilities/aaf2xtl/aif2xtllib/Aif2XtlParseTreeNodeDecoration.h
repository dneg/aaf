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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef _Aif2XtlParseTreeNodeDecoration_h_
#define _Aif2XtlParseTreeNodeDecoration_h_

#include <AifParseTreeNode.h>

// For aafRational_t
#include <AAFTypes.h>

class Aif2XtlGroupInfo : public AifParseTreeNodeDecoration {
public:
	enum GroupEnum {
		GROUP_TYPE_AUDIO,
		GROUP_TYPE_VIDEO
	};
	
	Aif2XtlGroupInfo( GroupEnum group )
	:	_group(group)
	{}

	virtual ~Aif2XtlGroupInfo()
	{}

	enum GroupEnum GetGroupType()
	{ return _group; }

private:

	GroupEnum _group;
};


class Aif2XtlTransitionInfo : public AifParseTreeNodeDecoration {
public:
	Aif2XtlTransitionInfo( bool supported, aafLength_t length, aafPosition_t cutPoint, const aafUID_t effectUid )
	:	_supported( supported ),
		_length( length ),
		_cutPoint( cutPoint ),
		_effectUid( effectUid ),
		_pReferencedNode(0)
	{}
	
	virtual ~Aif2XtlTransitionInfo()
	{}

	bool IsSupported()
	{ return _supported; }

	aafLength_t GetLength()
	{ return _length; }

	aafPosition_t GetCutPoint()
	{ return _cutPoint; }

	aafUID_t GetEffectUID()
	{ return _effectUid; }

	// FIXME - Tempoary solution to transition param gen location problem.
	void SetNode( AifParseTreeNode& referencedNode )
	{ _pReferencedNode = &referencedNode; }
	AifParseTreeNode& GetNode()
	{ assert( _pReferencedNode ); return *_pReferencedNode; }
private:
	AifParseTreeNode* _pReferencedNode;
	// end FIXME

private:
	// prohibited
	Aif2XtlTransitionInfo();

	bool _supported;
	aafLength_t  _length;
	aafPosition_t _cutPoint;
	aafUID_t _effectUid;
};

class Aif2XtlSequenceEditInfo : public AifParseTreeNodeDecoration {
public:
	Aif2XtlSequenceEditInfo( aafPosition_t startPosition, aafPosition_t endPosition,
				 aafLength_t segmentOffset, const aafRational_t& rate )
	: _startPosition( startPosition ),
	  _endPosition( endPosition ),
	  _segmentOffset( segmentOffset )
	{
		_rate = rate;
	}
	
	virtual ~Aif2XtlSequenceEditInfo()
	{}

	aafPosition_t GetStartPosition()
	{ return _startPosition; };

	void OffsetPositions( aafLength_t offset )
	{
		_startPosition += offset;
		_endPosition += offset;
	}

	aafPosition_t GetEndPosition()
	{ return _endPosition; };

	aafLength_t GetSegmentOffset()
	{ return _segmentOffset; }

	aafRational_t GetEditRate()
	{ return _rate; }

private:

	// These are the start/end positions within a single sequence.
	aafPosition_t _startPosition;
	aafPosition_t _endPosition;

	// This is the offset into the referenced media.
	aafLength_t _segmentOffset;

	// This is the rate that is used to convert edit units to time.
	aafRational_t _rate;
};

class Aif2XtlTransitionSequenceEditInfo : public Aif2XtlSequenceEditInfo
{
public:
	Aif2XtlTransitionSequenceEditInfo( aafPosition_t startPosition, aafPosition_t endPosition,
									   aafLength_t segmentOffset, const aafRational_t& rate )
      : Aif2XtlSequenceEditInfo( startPosition, endPosition, segmentOffset, rate ) 
	{}

	virtual ~Aif2XtlTransitionSequenceEditInfo()
	{}
};

class Aif2XtlExtractedEssenceInfo : public AifParseTreeNodeDecoration {
public:
	Aif2XtlExtractedEssenceInfo( const AxString fileName )
		: _fileName( fileName )
	{}
	~Aif2XtlExtractedEssenceInfo()
	{}

	AxString GetFileName()
	{ return _fileName; }
private:
	AxString _fileName;
};

class Aif2XtlEssenceInfo : public AifParseTreeNodeDecoration {
public:
	Aif2XtlEssenceInfo( const AxString fileRef )
		: _fileRef( fileRef )
	{}
	~Aif2XtlEssenceInfo()
	{}

	AxString GetFileRef()
	{ return _fileRef; }
private:
	AxString _fileRef;
};

// This class is used to mark unmasterd source clips so that they can
// be easily distinguished from other source clips.
class Aif2XtlUnMasteredSourceClipDecoration : public AifParseTreeNodeDecoration {
public:
	Aif2XtlUnMasteredSourceClipDecoration()
	{}

	~Aif2XtlUnMasteredSourceClipDecoration()
	{}
private:
};

#endif
