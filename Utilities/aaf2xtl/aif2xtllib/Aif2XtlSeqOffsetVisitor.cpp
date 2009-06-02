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

#include <Aif2XtlSeqOffsetVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

#include <assert.h>


Aif2XtlSeqOffsetVisitor::Aif2XtlSeqOffsetVisitor( std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream ),
  _offset(0)
{}

Aif2XtlSeqOffsetVisitor::~Aif2XtlSeqOffsetVisitor()
{}

void Aif2XtlSeqOffsetVisitor::PreOrderVisit( Aif2XtlSourceClipNode& node )
{
	Aif2XtlSequenceEditInfo* pEditInfo;
	if ( node.IsDecorated( pEditInfo ) ) {
		// Start positions are relative to the start of they sequence that contains them.
		// Accumulate the start positions to figure out the total offset.
		_offset += pEditInfo->GetStartPosition();
	}
}

void Aif2XtlSeqOffsetVisitor::PostOrderVisit( Aif2XtlSourceClipNode& node )
{
	Aif2XtlSequenceEditInfo* pEditInfo;
	if ( node.IsDecorated( pEditInfo ) ) {
		// Remove this nodes offset.
		_offset -= pEditInfo->GetStartPosition();

		// Add in the remainder to make the edit info absolute rather than relative the
		// start of the sequence.
		// FIXME - Is this best?  Maybe it would be better to simply store the offset?
		pEditInfo->OffsetPositions( _offset );

	    Aif2XtlTransitionSequenceEditInfo* pTransEditInfo;
	    if ( node.IsDecorated( pTransEditInfo ) ) {
			pTransEditInfo->OffsetPositions( _offset );
	    }

	}
}
