//=---------------------------------------------------------------------=
//
// $Id$
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
//=---------------------------------------------------------------------=

//Base Test files
#include "ResolveRefVisitor.h"
#include "RefResolver.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>

//AAF Analyzer Base files
#include <AAFMobReference.h>
#include <AAFSlotReference.h>
#include <AAFComponentReference.h>
#include <MobNodeMap.h>

//Analyzer Base files
#include <EdgeMap.h>
#include <DepthFirstTraversal.h>

//Ax files
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxUtil.h>
#include <AxMob.h>

//AAF files
#include <AAFResult.h>
#include <AAFClassDefUIDs.h>

#include <sstream>

namespace {

using namespace aafanalyzer;
using namespace boost;
using namespace std;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

using namespace std;
using namespace boost;

ResolveRefVisitor::ResolveRefVisitor( wostream& os,
				      shared_ptr<EdgeMap> spEdgeMap,
				      shared_ptr<TestLevelTestResult> spTestLevelResult )
: _os(os),
  _spEdgeMap(spEdgeMap),
  _spTestLevelResult( spTestLevelResult )
{}

ResolveRefVisitor::~ResolveRefVisitor()
{}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node)
{
  bool fullyResolvedFlag = false;

  // This is the tag we use for all the graph edges we create in order
  // to fully resolve this source reference.
  Node::LID tag = node.GetLID();

  AxSourceClip axSrcClp(node.GetAAFObjectOfType());

  aafSourceRef_t srcRef = axSrcClp.GetSourceReference();
  aafMobID_t mobid = srcRef.sourceID;

  if ( AxConstants::NULL_MOBID == mobid )
  {
    return true;
  }

  shared_ptr<Node> spMobNode;
  spMobNode = MobNodeMap::GetInstance().GetMobNode(mobid);

  // Assert it really is a mob. (i.e. to verify the MobNodeMap is
  // delivering good data.)  Then resolve the reference.

  if(spMobNode)
  {
    //verify mob
    IAAFMobSP spIaafMob;
    shared_ptr<AAFObjNode> spObjNode;
    spObjNode = dynamic_pointer_cast<AAFObjNode>(spMobNode);
    AxObject axObj(spObjNode->GetAAFObject());
    if ( !AxIsA(axObj, spIaafMob) )
    {
      // something is seriously messed up
      assert(0);
    }

    //mob has been verified, proceed to the sourceclip
    shared_ptr<AAFTypedObjNode<IAAFSourceClip> > spSrcClp;
    shared_ptr<Node> temp = node.GetSharedPointerToNode();
    spSrcClp = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceClip> >( temp );

    if(spSrcClp)
    {
      // Now find the referenced slot (a child of the referenced mob)
      // and extend the graph by adding a slot edge from the
      // referenced mob to the mobslot and add to the edge map.
      //
      // src clip -> referenced mob -> referenced slot
      shared_ptr<Node> spMobSlotNode = ResolveChildSlotNode( _spEdgeMap,
                                                             spMobNode, spIaafMob,
                                                             srcRef.sourceSlotID );

      if ( spMobSlotNode )
      {
        // Now make a reference from the src clip to the all the
        // components in the referenced mob that are within the range
        // of the reference.  We add references edges to the graph as
        // follows:
        //
        // src clip -> referenced mob -> referenced slot -> sequence +-> component one
        //                                                           |
        //                                                           +-> component two
        //                                                           |
        //                                                           etc...
        //

        // JPT REVIEW - If the components existed in a sequence this
        // analysis leaves out the sequence.  It is easier to
        // interpret dumps if the sequence is left in the chain of
        // references.  It also may be necessary, at some point, in
        // order to implement a tests.

        // Find all components below the MobSlot that need to be
        // included as part of the source reference.
        shared_ptr<vector<shared_ptr<Node> > >  componentsToReference =
          GetChildComponents( spMobSlotNode );

        for ( size_t i = 0 ; i < componentsToReference->size(); ++i )
        {
          fullyResolvedFlag = true;
          shared_ptr<AAFComponentReference> spCompEdge( new AAFComponentReference(spMobSlotNode,
                                                                                  (*componentsToReference)[i],
                                                                                  tag, true) );
          _spEdgeMap->AddEdge( spCompEdge );
        }

        // Add the remaining edges that link the source clip node to
        // the mob node to the slot node, after we know that something
        // was resolved.

        if ( fullyResolvedFlag )
        {
          shared_ptr<AAFMobReference> spMobRefEdge( new AAFMobReference(spSrcClp, spMobNode, tag) );
          _spEdgeMap->AddEdge( spMobRefEdge );

          shared_ptr<AAFSlotReference> spSlotEdge( new AAFSlotReference(spMobNode, spMobSlotNode,tag) );
          _spEdgeMap->AddEdge( spSlotEdge );
        }

      } // if (spMobSlotNode)
    } // if (spSrcClip)
  } // if (spMobNode)

  // Else, the mob was not found. Issue a warning.
  else
  {
    AxString explain( L"Out of file mob referenced. ID = " );
    explain += AxStringUtil::mobid2Str( mobid );

    _spTestLevelResult->AddSingleResult( L"REQ_EP_016",
					 TestResult::WARN,
					 L"Mob Resolve",
					 L"Resolve source clip mob reference.",
					 explain );
  }

  return true;
}

shared_ptr<vector<shared_ptr<Node> > > ResolveRefVisitor::GetChildComponents( shared_ptr<Node> spMobSlotNode )
{
  shared_ptr<vector<shared_ptr<Node> > > spChildren( new vector<shared_ptr<Node> > );

  EdgeMap::ConstEdgeVectorSP spChildEdges = _spEdgeMap->GetChildren(spMobSlotNode);


  for ( EdgeMap::EdgeVector::const_iterator iter = spChildEdges->begin();
        iter != spChildEdges->end();
        ++iter )
  {
    spChildren->push_back( (*iter)->GetChildNode() );
  }

  return spChildren;
}

// spParentMobNode is the parent of the slot we want to
// resolve. spParentIaafMob is the underlying aaf object. It is passed
// for convenience so avoid have to pull it out of spParentMobNode.
// slotId is the reference slot in that mob.

shared_ptr<Node> ResolveRefVisitor::ResolveChildSlotNode( shared_ptr<EdgeMap> spEdgeMap,
                                                          shared_ptr<Node> spParentMobNode,
                                                          IAAFMobSP spParentIaafMob,
                                                          const aafSlotID_t slotId )
{
  EdgeMap::ConstEdgeVectorSP spChildEdges = spEdgeMap->GetChildren( spParentMobNode );
  for( EdgeMap::EdgeVector::const_iterator iter = spChildEdges->begin();
       iter != spChildEdges->end();
       ++iter )
  {
    // Recover the AAFMobSlot object from the child node and compare
    // its id.
    shared_ptr<AAFObjNode> spChildNode = dynamic_pointer_cast<AAFObjNode>( (*iter)->GetChildNode() );
    AxObject axObj(spChildNode->GetAAFObject());

    IAAFMobSlotSP spMobSlot;
    if ( AxIsA( axObj, spMobSlot ) )
    {
      AxMobSlot axMobSlot( spMobSlot );
      if ( slotId == axMobSlot.GetSlotID() )
      {
        return spChildNode;
      }
    }
  }
  
  // It wasn't resolved. Fail the related requirement and return a
  // null. Note, the mal-terminated derivation chain will lead to
  // derivation chain analysis failures when that runs.

  AxMob axParentMob( spParentIaafMob );

  wstringstream ss;
  ss << L"Unable to resolve slot id " << slotId
     << L" in the mob named \"" << axParentMob.GetName( L"<unnamed>" ) << L"\""
     << L" with id " << AxStringUtil::mobid2Str( axParentMob.GetMobID() );

  _spTestLevelResult->AddSingleResult( L"REQ_EP_257",
				       TestResult::FAIL,
				       L"Slot Resolve",
				       L"Lookup slot in resolved mob.",
				       ss.str() );

  return shared_ptr<Node>();
}

}
