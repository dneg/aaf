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
#include "AcyclicVisitor.h"
#include "AcyclicAnalysis.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>

//Analyzer Base files
#include <Node.h>

//AAF Analyzer Base files
#include <AAFObjNode.h>

//Axlib files
#include <AxObject.h>
#include <AxMob.h>
#include <AxUtil.h>
#include <AxMobSlot.h>
#include <AxComponent.h>

//STL files
#include <ostream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace std;
using namespace boost;

AcyclicVisitor::AcyclicVisitor( wostream& os,
                                shared_ptr<TestLevelTestResult> spTestResult )
  : _os(os),
    _spTestResult( spTestResult )
{}

AcyclicVisitor::~AcyclicVisitor()
{}

bool AcyclicVisitor::IsPresent(unsigned int lid)
{
  return ( _nodeSet.find( lid ) != _nodeSet.end() );
}

bool AcyclicVisitor::PreOrderVisit(Node& node)
{
  Node::LID lid = node.GetLID();

  // JPT REVIEW - This alogrithm is not adequate.  A source clip can
  // legally reference another slot in it's parent mob. (Also, it can
  // possible reference a different region on the same slot if it is a
  // timeline.)  That will cause the parent mob to be pre-order
  // visited twice during a travesal down a branch.  This case,
  // however, isn't currently handled (resolved) by the reference
  // resolve so it is necessary to handle it here.  Identification of
  // the cycle requires examanination of the node, and the reason we
  // are visiting it (i.e. the edge).

  // std::set.insert() returns a pair.second value that is a bool
  // indicating whether the value was already exists in the set. Its
  // precense in the set indicates that the node has been pre-visited
  // during the traversal down the branch. If it has been visited then
  // we cycled back to this node and we reject it.

  if ( _nodeSet.insert(lid).second )
  {
    // JPT REVIEW - This is a valid optimization, but it's confusing
    // for maintainers.  We are not shooting for all out performance
    // here so I'm disabling it and will remove later when I confirm
    // removing it has no side effect.  Also, review the HaveVisited
    // and RecordVisited calls. Those are an optimization that I
    // believe can be removed to make this easier to understand.

    // The node was properly inserted into the set, therefore, a cycle
    // has not yet been detected.  If this node has already been
    // visited, that means that there are no cycles down this
    // sub-branch, so the node can be removed from the set and the
    // traversal of this sub-branch can be halted.

    if ( this->HaveVisited( lid ) )
    {
        _nodeSet.erase( lid );
        return false;
    }
    
    // Add node into the vector since it was visited on this
    // sub-branch's traversal
    _nodeVector.push_back(node.GetSharedPointerToNode());

    this->RecordVisit( lid );

    return true;
  }

  // A cycle was detected, add failure result and add details showing
  // the location in the chain of references.

  shared_ptr<DetailLevelTestResult>
    spCycleResult = _spTestResult->AddSingleResult( L"REQ_EP_256",
                                                    TestResult::FAIL,
                                                    L"Reference Cycle Detected",
                                                    L"",
                                                    L"A cycle was detected in the derivation chain." );
  
  spCycleResult->AddDetail( L"Nodes involved in the cycle:" );

  _nodeVector.push_back(node.GetSharedPointerToNode());
  
  for(unsigned int i = 0; i < _nodeVector.size(); i++)
  {
    wstring detail;

    detail = wstring(L"Node: ") +
             wstring(AxStringUtil::int2Str(_nodeVector.at(i)->GetLID())) +
             wstring(L"  Object - ") +
             wstring( _nodeVector.at(i)->GetName());

    spCycleResult->AddDetail( detail );

    AAFObjNode& aafNode = dynamic_cast<AAFObjNode&>(*(_nodeVector.at(i)));
    AxObject axObj( aafNode.GetAAFObject() );
    
    IAAFMobSP spMob;
    IAAFMobSlotSP spMobS;
    IAAFSourceReferenceSP spSR;
    
    if ( AxIsA(axObj, spMobS) )
    {
      AxMobSlot axMobS(spMobS);

      detail = wstring(L"         SlotID - ") +
               AxStringUtil::int2Str(axMobS.GetSlotID());

      spCycleResult->AddDetail( detail );
    }
    else if( AxIsA(axObj, spMob) )
    {
      AxMob axMob(spMob);

      detail = wstring( L"           Name - ") +
               wstring( axMob.GetName() );

      spCycleResult->AddDetail( detail );
      detail = wstring( L"          MobID - " ) +
               AxStringUtil::mobid2Str(axMob.GetMobID());

      spCycleResult->AddDetail( detail );
    }
    else if ( AxIsA(axObj,spSR) )
    {
      AxSourceReference axSR(spSR);

      detail = wstring(L"       SourceID - ") +
               AxStringUtil::mobid2Str( axSR.GetSourceID() );

      spCycleResult->AddDetail( detail );
    }
  }
  
  _nodeVector.pop_back();
  
  return false;
}

bool AcyclicVisitor::PostOrderVisit(Node& node)
{
  Node::LID lid = node.GetLID();

  if(IsPresent(lid))
  {
    //remove the node from vector since we have finished travelling down branch
    Erase(lid);
    return true;
  }

  // Should never arrive here if everything is working correctly.
  assert(0);  

  return false;
}

void AcyclicVisitor::Erase(unsigned int lid)
{
  _nodeVector.pop_back();
  _nodeSet.erase( lid );
}

} // end of namespace aafanalyzer
