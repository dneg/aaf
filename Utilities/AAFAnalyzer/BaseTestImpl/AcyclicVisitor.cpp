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

AcyclicVisitor::AcyclicVisitor( wostream& os,
                                boost::shared_ptr<TestLevelTestResult> spTestResult )
  : Visitor(FOLLOW_REFERENCES),
    _os(os),
    _spTestResult( spTestResult ),
    _level(0),
    _cycleDetected(false)
{}

AcyclicVisitor::~AcyclicVisitor()
{}

void AcyclicVisitor::CreateCycleTestResult()
{
  // Else, a cycle was detected, add a failure result and add details
  // showing the location in the chain of references.
  boost::shared_ptr<DetailLevelTestResult>
    spCycleResult = _spTestResult->AddUnassociatedSingleResult( L"REQ_EP_256",
								L"A cycle was detected amoung the referenced objects in the file.",
								TestResult::FAIL );

  spCycleResult->AddDetail( L"Nodes involved in the cycle:" );

  for(unsigned int i = 0; i < _nodeVector.size(); i++)
  {
    wstring detail;

    detail = wstring(L"Node: ") +
             wstring(AxStringUtil::int2Str(_nodeVector.at(i)->GetLID())) +
             wstring(L"  Object - ") +
             wstring( _nodeVector.at(i)->GetName());
    spCycleResult->AddDetail( detail );

    boost::shared_ptr<AAFObjNode> spAafNode = boost::dynamic_pointer_cast<AAFObjNode>( _nodeVector.at(i) );
    
    if ( spAafNode )
    {
      AxObject axObj( spAafNode->GetAAFObject() );
    
      IAAFMobSP spMob;
      IAAFMobSlotSP spMobS;
      IAAFSourceReferenceSP spSR;
      
      if ( AxIsA(axObj, spMobS) )
      {
	AxMobSlot axMobS(spMobS);
	
	detail = wstring(L"\tSlotID - ") +
	  AxStringUtil::int2Str(axMobS.GetSlotID());

	spCycleResult->AddDetail( detail );
      }
      else if( AxIsA(axObj, spMob) )
      {
	AxMob axMob(spMob);
	
	detail = wstring( L"\tName - ") +
	  wstring( axMob.GetName() );
	spCycleResult->AddDetail( detail );

	detail = wstring( L"\tMobID - " ) +
	  AxStringUtil::mobid2Str(axMob.GetMobID());
	spCycleResult->AddDetail( detail );
      }
      else if ( AxIsA(axObj,spSR) )
      {
	AxSourceReference axSR(spSR);

	detail = wstring(L"\tSourceID - ") +
	  AxStringUtil::mobid2Str( axSR.GetSourceID() );
	spCycleResult->AddDetail( detail );
      }
    }
  }
}

bool AcyclicVisitor::PreOrderVisit(Node& node)
{
  // Don't bother continuing if a cycle has already been detected
  // (This also has the benifit of silencing redudant reporting of
  // errors invovling other paths into same cycle.)
  if ( _cycleDetected )
  {
    return false;
  }

  _level++;

  // JPT REVIEW - This alogrithm is not adequate.  A source clip can
  // legally reference another slot in it's parent mob. (Also, it can
  // possible reference a different region on the same slot if it is a
  // timeline.)  That will cause the parent mob to be pre-order
  // visited twice during a travesal down a branch.  This case,
  // however, isn't currently handled (resolved) by the reference
  // resolver so it is necessary to handle it here.  Identification of
  // the cycle requires examanination of the node, and the reason we
  // are visiting it (i.e. the edge).

  _nodeVector.push_back(node.GetSharedPointerToNode());

  Node::LID lid = node.GetLID();

  // std::set.insert() returns a pair.second value that is a bool
  // indicating whether the value was already exists in the set. Its
  // presence in the set indicates that the node has already been
  // pre-visited during the traversal down the branch. If it has
  // already been visited then we have identified a cycle.
  if ( _nodeSet.insert(lid).second )
  {
    return true;
  }
  else
  {
    CreateCycleTestResult();

    // Don't continue traversing down this branch because it is part
    // of a cycle, and set flag that will stop all further traversals.
    _nodeVector.pop_back();
    _cycleDetected = true;
    return false;
  }
}

bool AcyclicVisitor::PostOrderVisit(Node& node)
{
  Node::LID lid = node.GetLID();

  // precondition = not is in set
  assert( _nodeSet.find( lid ) != _nodeSet.end() );

  _nodeVector.pop_back();
  _nodeSet.erase(lid);

  _level--;

  return true;
}

bool AcyclicVisitor::IsCycleDetected() const
{
  return _cycleDetected;
}

} // end of namespace aafanalyzer
