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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef _Aif2XtlSeqEditVisitor_h_
#define _Aif2XtlSeqEditVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

class Aif2XtlSeqEditVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlSeqEditVisitor( std::wostream& logStream );
	virtual ~Aif2XtlSeqEditVisitor();

	virtual void PreOrderVisit( Aif2XtlSequence& node );
	virtual void PostOrderVisit( Aif2XtlSequence& node );

	virtual void PreOrderVisit( Aif2XtlSourceClipNode& node );
	virtual void PostOrderVisit( Aif2XtlSourceClipNode& node );

private:
	
	void ProcessTransition( AifParseTreeNode& nodeA, IAAFSegmentSP spSegmentA,
				AifParseTreeNode& nodeB, IAAFTransitionSP spTransitionB,
				AifParseTreeNode& nodeC, IAAFSegmentSP spSegmentC );

	void ProcessCut( AifParseTreeNode& nodeA, IAAFSegmentSP spSegmentA,
			 AifParseTreeNode& nodeB, IAAFSegmentSP spSegmentB );

	void ProcessSingle( AifParseTreeNode& node, IAAFSegmentSP spSegment );

	// This is the position of a single segment in a sequence.
	// It is used as a temporary variable when processing the segments
	// in a single sequence.
	aafPosition_t _position;
};

#endif
