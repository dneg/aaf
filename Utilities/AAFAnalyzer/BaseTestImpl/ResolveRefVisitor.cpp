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

//AAF files
#include <AAFResult.h>

//STL files
#include <set>

namespace {

using namespace aafanalyzer;
using namespace boost;
using namespace std;

class ResolveSlotRefVisitor : public TypedVisitor
{
 public:
 
  ResolveSlotRefVisitor(wostream& os, aafSlotID_t slotId)
    : _os(os),
      _slotId( slotId )
  {}

  virtual ~ResolveSlotRefVisitor()
  {}

  //Note: We cannot do a node cast in this situation since we need the node
  //      of the correct type to create an edge.
  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFTimelineMobSlot>& node)
  {
    AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();
    
    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }
  
  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFStaticMobSlot>& node)
  {
    AxStaticMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();
    
    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }
    
  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFEventMobSlot>& node)
  {
    AxEventMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();
    
    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFMobSlot>& node)
  {
    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
    aafSlotID_t slotId = axMobSlot.GetSlotID();
    
    if ( slotId == _slotId )
    {
        _spMobSlot = node.GetSharedPointerToNode();
        //The node was found, there is no need to traverse further.
        return false;
    }

    return true;
  }
  
  virtual bool EdgeVisit(AAFContainment& edge)
  {
    //Don't continue the traversal if the mob slot has been found.
    return !_spMobSlot;
  }
  
  shared_ptr<Node> GetMobSlot() const
  {
    return _spMobSlot;
  }

 private:
  wostream& _os;
  aafSlotID_t _slotId;
  shared_ptr<Node> _spMobSlot;

  // prohibited
  ResolveSlotRefVisitor();
  ResolveSlotRefVisitor( const ResolveSlotRefVisitor& );
  ResolveSlotRefVisitor& operator=( const ResolveSlotRefVisitor& );
};

class ResolveComponentRefVisitor : public TypedVisitor
{
 public:
 
  typedef set<shared_ptr<Node> > NodeSet;
  typedef shared_ptr<NodeSet> NodeSetSP;
 
  ResolveComponentRefVisitor(wostream& os)
    : _os(os),
      _spNodes( new NodeSet )
  {}

  virtual ~ResolveComponentRefVisitor()
  {}

  virtual bool PreOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node)
  {
    //Add the source clip to the set of source clips.
    _spNodes->insert( node.GetSharedPointerToNode() );
    
    //We have reached the source clip, there is no need to traverse any further.
    return false;
  }
  
  NodeSetSP GetReferencedNodes() const
  {
    return _spNodes;
  }

 private:
  wostream& _os;
  NodeSetSP _spNodes;

  // prohibited
  ResolveComponentRefVisitor();
  ResolveComponentRefVisitor( const ResolveComponentRefVisitor& );
  ResolveComponentRefVisitor& operator=( const ResolveComponentRefVisitor& );
};

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

using namespace std;
using namespace boost;

ResolveRefVisitor::ResolveRefVisitor(wostream& os, shared_ptr<EdgeMap> spEdgeMap )
: _os(os),
  _spEdgeMap(spEdgeMap),
  _spResult( new DetailLevelTestResult( 
                           L"ResolveRefVisitor",
                           L"Visit source clip objects and resolve references..",
                           L"-",
                           L"-",
                           TestResult::PASS,
                           TestRegistry::GetInstance().GetRequirementsForTest( RefResolver::GetTestInfo().GetName() )
           )                          )
{}

ResolveRefVisitor::~ResolveRefVisitor()
{
}

bool ResolveRefVisitor::PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node)
{
  AxSourceClip axSrcClp(node.GetAAFObjectOfType());

  aafSourceRef_t srcRef = axSrcClp.GetSourceReference();
  aafMobID_t mobid = srcRef.sourceID;

  if ( AxConstants::NULL_MOBID == mobid )
  {
    return true;
  }
    
  shared_ptr<Node> spNode;
  spNode = MobNodeMap::GetInstance().GetMobNode(mobid);
	
  //Assert it really is a mob. (i.e. to verify the MobNodeMap is delivering good data.)
  //Then resolve the reference.
  if(spNode)
  {
    //verify mob
    IAAFMobSP spVerify;
    shared_ptr<AAFObjNode> spObjNode;
    spObjNode = dynamic_pointer_cast<AAFObjNode>(spNode);
    AxObject axObj(spObjNode->GetAAFObject());
    assert(AxIsA(axObj, spVerify));
    
    //mob has been verified, proceed to the sourceclip
    shared_ptr<AAFTypedObjNode<IAAFSourceClip> > spSrcClp;
    shared_ptr<Node> temp = node.GetSharedPointerToNode();
    spSrcClp = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceClip> >( temp );
    
    //ensure we have the two proper nodes (spNode, spSrcClp), create a Mob Edge and add to Edgemap
    if(spSrcClp)
    {
      shared_ptr<AAFMobReference> spMobRefEdge(new AAFMobReference(spSrcClp, spNode)); 
      _spEdgeMap->AddEdge(spMobRefEdge);

      //now create a Slot Edge from the source clip to the mobslot and add to Edgemap
      shared_ptr<ResolveSlotRefVisitor> spSlotVisitor( new ResolveSlotRefVisitor( _os, srcRef.sourceSlotID ) );
      DepthFirstTraversal slotDFT( _spEdgeMap, spNode );
      slotDFT.TraverseDown( spSlotVisitor );
      shared_ptr<Node> spMobSlotNode = spSlotVisitor->GetMobSlot();
      
      if ( spMobSlotNode )
      {
        shared_ptr<AAFSlotReference> spSlotEdge( new AAFSlotReference(spSrcClp, spMobSlotNode) );
        _spEdgeMap->AddEdge( spSlotEdge );

        //Now make a reference from the source clip to all source clips
        //referenced from this mob slot.
        shared_ptr<ResolveComponentRefVisitor> spCompVisitor( new ResolveComponentRefVisitor( _os ) );
        DepthFirstTraversal dfs( _spEdgeMap, spMobSlotNode );
        dfs.TraverseDown( spCompVisitor );
        ResolveComponentRefVisitor::NodeSetSP spNodes = spCompVisitor->GetReferencedNodes();
        
        ResolveComponentRefVisitor::NodeSet::const_iterator iter;
        
        for ( iter = spNodes->begin(); iter != spNodes->end(); iter++ )
        {
          shared_ptr<AAFComponentReference> spCompEdge( new AAFComponentReference(spSrcClp, *iter) );
          _spEdgeMap->AddEdge( spCompEdge );
        }
        
      }
    }
  }
  //keep track of unresolved source clips
  else
  {
    AxString explain( L"Out-file mob referenced. ID = " );
    explain += AxStringUtil::mobid2Str( mobid );
    shared_ptr<const DetailLevelTestResult> spWarning( new DetailLevelTestResult( _spResult->GetName(),
							     L"-", // desc
							     explain,
							     L"-", // docref
							     TestResult::WARN,
                                 *(new Requirement::RequirementMapSP(new Requirement::RequirementMap())) ) );
    //TODO: Pass a real RequirementVectorSP
    _spResult->AppendSubtestResult( spWarning );
    _spResult->SetResult( _spResult->GetAggregateResult() );
  }

  return true;
}

shared_ptr<const DetailLevelTestResult> ResolveRefVisitor::GetTestResult() const
{
  return _spResult;
}

}
