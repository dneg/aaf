// Copyright 2003 BBC Technology

// $Id$
// $Name$

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