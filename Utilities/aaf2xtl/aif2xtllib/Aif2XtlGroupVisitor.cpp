// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <Aif2XtlGroupVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxEssence.h>
#include <AxDefObject.h>

Aif2XtlGroupVisitor::Aif2XtlGroupVisitor( std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream )
{}

Aif2XtlGroupVisitor::~Aif2XtlGroupVisitor()
{}

void Aif2XtlGroupVisitor::PreOrderVisit( Aif2XtlCompMobNode& node )
{
	node.SetVisitChildren( false );

	std::vector<AifParseTreeNode*> audioGroup;
	std::vector<AifParseTreeNode*> videoGroup;
	std::vector<AifParseTreeNode*> unknownGroup;

	int numChildren = node.GetNumChildren();
	int i;
	for ( i = 0; i < numChildren; ++i ) {
		// This is not a bug!
		// Each time a child is released, the rest of the children
		// are shuffled down one position in the underlying vector.
		// Hence if we release child zero numChildren times, we have
		// released all the children.
		std::auto_ptr<AifParseTreeNode> pChildNode = node.ReleaseChild(0);


		if ( dynamic_cast<Aif2XtlTimelineMobSlot*>(pChildNode.get()) ){
			
			Aif2XtlTimelineMobSlot* pTimelineNode =
				dynamic_cast<Aif2XtlTimelineMobSlot*>(pChildNode.release());		

			AxTimelineMobSlot axTimelineMobSlot( pTimelineNode->GetAif2XtlAAFObject() );

			AxSegment axSeg( axTimelineMobSlot.GetSegment() );
			
			AxDataDef axDataDef( axSeg.GetDataDef() );

			if ( axDataDef.IsSoundKind() ) {
				audioGroup.push_back( pTimelineNode );
			}
			else if ( axDataDef.IsPictureKind() ) {
				videoGroup.push_back( pTimelineNode );
				}
			else {
				unknownGroup.push_back( pTimelineNode );
			}
			
		}
		else {
			unknownGroup.push_back( pChildNode.release() );
		}
	}

	// FIXME - The should probably be deleted.  They may cause
	// erroneous XTL to be generated.
	// Put the unknown nodes back in the order they were removed.
	std::vector<AifParseTreeNode*>::iterator iter;
	for ( iter = unknownGroup.begin();
		  iter != unknownGroup.end();
		  ++iter ) {
			  std::auto_ptr<AifParseTreeNode> pNode( *iter );
			  node.AddChild( pNode );
	}
		  
	// Create an audio group node and reparent the audio nodes
	if ( audioGroup.size() > 0 ) {
		std::auto_ptr<AifParseTreeNode> pAudioGroupNode( new Aif2XtlGroupNode );
		for ( iter = audioGroup.begin();
			  iter != audioGroup.end();
			++iter ) {
				  std::auto_ptr<AifParseTreeNode> pNode( *iter );
				pAudioGroupNode->AddChild( pNode );
		}
		std::auto_ptr<Aif2XtlGroupInfo> pAudioGroupInfo(	
					new Aif2XtlGroupInfo(Aif2XtlGroupInfo::GROUP_TYPE_AUDIO) );
		pAudioGroupNode->PushDecoration( pAudioGroupInfo );
		node.AddChild( pAudioGroupNode );
	}

	// Create a video group node and reparent the video nodes
	if ( videoGroup.size() > 0 ) {
		std::auto_ptr<AifParseTreeNode> pVideoGroupNode( new Aif2XtlGroupNode );
		for ( iter = videoGroup.begin();
			  iter != videoGroup.end();
			++iter ) {
				  std::auto_ptr<AifParseTreeNode> pNode( *iter );
				pVideoGroupNode->AddChild( pNode );
		}
		std::auto_ptr<Aif2XtlGroupInfo> pVideoGroupInfo(
			new Aif2XtlGroupInfo(Aif2XtlGroupInfo::GROUP_TYPE_VIDEO) );
		pVideoGroupNode->PushDecoration( pVideoGroupInfo );
		node.AddChild( pVideoGroupNode );
	}
}

void Aif2XtlGroupVisitor::PostOrderVisit( Aif2XtlCompMobNode& node )
{
	node.SetVisitChildren( true );
}
