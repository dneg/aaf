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
