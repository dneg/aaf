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

#include <Aif2XtlSeqEditVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

#include <AxComponent.h>
#include <AxMobSlot.h>

#include <assert.h>

namespace {

aafRational_t GetEditRateFromParent( AifParseTreeNode& node )
{
	AifParseTreeNode* pParent;

	pParent = node.GetParent();
	assert( pParent );

	Aif2XtlTimelineMobSlot* pTimelineSlotNode = dynamic_cast<Aif2XtlTimelineMobSlot*>(pParent);

	if ( pTimelineSlotNode ) {
		AxTimelineMobSlot axTimelineSlot( pTimelineSlotNode->GetAif2XtlAAFObject() );
		return axTimelineSlot.GetEditRate();

		// FIXME - SHOULD GET ORIGIN HERE AS WELL.
	}
	else {
		return GetEditRateFromParent( *pParent );
	}
}

} // end of namespace


Aif2XtlSeqEditVisitor::Aif2XtlSeqEditVisitor( std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream ),
  _position(0)
{}

Aif2XtlSeqEditVisitor::~Aif2XtlSeqEditVisitor()
{}

void Aif2XtlSeqEditVisitor::PreOrderVisit( Aif2XtlSequence& node )
{
	int i;
	int numChildren = node.GetNumChildren();
	unsigned int sampleCount = 0;

	_position = 0;

	if ( 1 == numChildren ) {

		AifParseTreeNode& childNode = node.GetChild(0);
		AxObject axObject( childNode.GetAAFObject() );
		IAAFSegmentSP spSegment;
	
		if ( AxIsA( axObject, spSegment ) ) {
			ProcessSingle( childNode, spSegment );
		}

	}
	else {

		for( i = 0; i < numChildren-1; /*noop*/ ) {

			AifParseTreeNode& childNodeA = node.GetChild(i);
			AxObject axObjectA( childNodeA.GetAAFObject() );
			IAAFSegmentSP spSegmentA;
	
			AifParseTreeNode& childNodeB = node.GetChild(i+1);
			AxObject axObjectB( childNodeB.GetAAFObject() );
			IAAFSegmentSP spSegmentB;
			IAAFTransitionSP spTransitionB;

			if ( AxIsA( axObjectA, spSegmentA ) &&  AxIsA( axObjectB, spSegmentB ) ) {
				// This is cut between two segments.

				ProcessCut( childNodeA, spSegmentA,
						    childNodeB, spSegmentB );

				i++;

			}
			else if ( AxIsA( axObjectA, spSegmentA ) &&  AxIsA( axObjectB, spTransitionB ) ) {
				// This is a transition between two segments.  The component after
				// TransitionB *must* be a segment.
	
				// Ensure we do not attempt to access beyond end of the child list.
				if ( !(i+2 < numChildren) ) {
					assert(0);
				}

				AifParseTreeNode& childNodeC = node.GetChild(i+2);
				AxObject axObjectC( childNodeC.GetAAFObject() );
				IAAFSegmentSP spSegmentC;

				if ( !AxIsA( axObjectC, spSegmentC ) ) {
					assert(0);
				}

				ProcessTransition( childNodeA, spSegmentA,
								   childNodeB, spTransitionB,
								   childNodeC, spSegmentC );
			
				i += 2;
			}			
			else {
				assert(0);
			}
		}
	}
}

void Aif2XtlSeqEditVisitor::PostOrderVisit( Aif2XtlSequence& node )
{}


void Aif2XtlSeqEditVisitor::PreOrderVisit( Aif2XtlSourceClipNode& node )
{}

void Aif2XtlSeqEditVisitor::PostOrderVisit( Aif2XtlSourceClipNode& )
{}

void Aif2XtlSeqEditVisitor::ProcessTransition(
		AifParseTreeNode& nodeA, IAAFSegmentSP spSegmentA,
		AifParseTreeNode& nodeB, IAAFTransitionSP spTransitionB,
		AifParseTreeNode& nodeC, IAAFSegmentSP spSegmentC )
{
	Aif2XtlTransitionInfo* pTransInfo;

	nodeB.GetDecoration( pTransInfo );

	if ( pTransInfo->IsSupported() ) {
		// Transition is supported.  The segments are edits as follows:
		// [     Segment A        ]
		//             [Transition B]
		//			   [       Segment C        ]
		//

		AxSegment    axSegmentA( spSegmentA );
		AxTransition axTransitionB( spTransitionB );
		AxSegment    axSegmentC( spSegmentC );

		
		// FIXME - Verify that the edit rates of the nodes match.
		// FIXME - Verify that the transition is longer than the segments.
		
		aafPosition_t nodeAStart = _position;
		aafPosition_t nodeAEnd   = nodeAStart + axSegmentA.GetLength();

		aafPosition_t nodeBStart = nodeAEnd - axTransitionB.GetLength();
		aafPosition_t nodeBEnd   = nodeAEnd;

		aafPosition_t nodeCStart = nodeBStart;
		aafPosition_t nodeCEnd   = nodeCStart + axSegmentC.GetLength();

		// Pick one of the nodes to get the edit rate.
		aafRational_t editRate = GetEditRateFromParent( nodeA );
		
		// Node A may already be decorated with sequence edit info if it is
		// part of a sequence as follows:
		// [Segment Y] [Transition Z] [Segment A] [Transition B] [Segment C]
		// Seqment A will acquire edit info when Y-Z-A is processed.
		// It is incorrect to add new edit info - the Aif2XtlXmlGenVisitor
		// expects either one SeqEditInfo decoration or a SequenceEditInfo
		// and a TransitionInfo decoration.

		Aif2XtlSequenceEditInfo* pSeqInfoAUnowned;
		if ( !nodeA.IsDecorated( pSeqInfoAUnowned ) ) {
			std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoA(
				new Aif2XtlSequenceEditInfo(nodeAStart, nodeAEnd, 0, editRate ) );
			nodeA.PushDecoration( pSeqInfoA );
		}
		
        std::auto_ptr<Aif2XtlTransitionSequenceEditInfo> pSeqInfoB(
				new Aif2XtlTransitionSequenceEditInfo(nodeBStart, nodeBEnd, 0, editRate ) );


		std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoC(
				new Aif2XtlSequenceEditInfo(nodeCStart, nodeCEnd, 0, editRate ) );


		// Xtl wants the transition to be represented as follows:
		// <track> [ Segment A ] </track>
		// <track> [ Segment C ] [ Transition B ] </track>
		// i.e. the transition comes after segment C.
		// This ordering differs from that used in the AAF file, and represeted by the
		// ordering of the children of a sequence.  To accomadate this, transition B's
		// Aif2XtlTransitionInfo and Aif2XtlSequenceEditInfo are pushed onto Segment C.
		// The Xml generation visitor expects this.

		std::auto_ptr<Aif2XtlTransitionInfo> pTransInfoB;
		nodeB.PopDecoration( pTransInfoB );

		nodeC.PushDecoration( pSeqInfoB );
		nodeC.PushDecoration( pTransInfoB );
		nodeC.PushDecoration( pSeqInfoC );
		
		_position = nodeBStart;
	}
	else {
		// Transition not supported.  Edit segments A and B at the transition's cut point.

		AxSegment axSegmentA( spSegmentA );
		AxSegment axSegmentC( spSegmentC );
		
		// FIXME - Verify the edit rates of the nodes match.
		aafPosition_t nodeAStart = _position;
		aafPosition_t nodeAEnd   = nodeAStart + axSegmentA.GetLength() - (pTransInfo->GetLength() - pTransInfo->GetCutPoint());

		aafPosition_t nodeCStart = nodeAEnd;
		aafPosition_t nodeCEnd   = nodeCStart + axSegmentC.GetLength() - pTransInfo->GetCutPoint();

		// Pick one of the nodes to get the edit rate.
		aafRational_t editRate = GetEditRateFromParent( nodeA );

		Aif2XtlSequenceEditInfo* pSeqInfoAUnowned;
		if ( !nodeA.IsDecorated( pSeqInfoAUnowned ) ) {
			std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoA(
				new Aif2XtlSequenceEditInfo(nodeAStart, nodeAEnd, 0, editRate ) );
			nodeA.PushDecoration( pSeqInfoA );
		}

		std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoC(
				new Aif2XtlSequenceEditInfo(nodeCStart, nodeCEnd, pTransInfo->GetCutPoint(), editRate ) );

		nodeC.PushDecoration( pSeqInfoC );

		_position = nodeAEnd;
	}

}

void Aif2XtlSeqEditVisitor::ProcessCut(
		AifParseTreeNode& nodeA, IAAFSegmentSP spSegmentA,
		AifParseTreeNode& nodeB, IAAFSegmentSP spSegmentB )
{
	AxSegment axSegmentA( spSegmentA );
	AxSegment axSegmentB( spSegmentB );

		
	// FIXME - Verify the edit rates of the nodes match.
	aafPosition_t nodeAStart = _position;
	aafPosition_t nodeAEnd   = nodeAStart + axSegmentA.GetLength();


	aafPosition_t nodeBStart = nodeAEnd;
	aafPosition_t nodeBEnd   = nodeBStart + axSegmentB.GetLength();

	// Pick one of the nodes to get the edit rate.
	aafRational_t editRate = GetEditRateFromParent( nodeA );

	Aif2XtlSequenceEditInfo* pSeqInfoAUnowned;
	if ( !nodeA.IsDecorated( pSeqInfoAUnowned ) ) {
		std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoA(
			new Aif2XtlSequenceEditInfo(nodeAStart, nodeAEnd, 0, editRate ) );

		nodeA.PushDecoration( pSeqInfoA );
	}

	std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfoB(
			new Aif2XtlSequenceEditInfo(nodeBStart, nodeBEnd, 0, editRate ) );

	nodeB.PushDecoration( pSeqInfoB );

	_position = nodeAEnd;
}

void Aif2XtlSeqEditVisitor::ProcessSingle( 
		AifParseTreeNode& node, IAAFSegmentSP spSegment )
{
	AxSegment axSegment( spSegment );
		
	aafPosition_t nodeStart = _position;
	aafPosition_t nodeEnd   = nodeStart + axSegment.GetLength();

	// Pick one of the nodes to get the edit rate.
	aafRational_t editRate = GetEditRateFromParent( node );

	std::auto_ptr<Aif2XtlSequenceEditInfo> pSeqInfo(
			new Aif2XtlSequenceEditInfo(nodeStart, nodeEnd, 0, editRate ) );

	node.PushDecoration( pSeqInfo );

	_position = nodeEnd;
}

