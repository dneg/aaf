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
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>

//Analyzer Base files
#include <Node.h>

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

AcyclicVisitor::  AcyclicVisitor(wostream& os)
  : _os(os),
    _spResult( new DetailLevelTestResult(L"AcyclicVisitor",
                                         L"Detects cycles in an AAF object graph.",
                                         L"No cycles found.",
                                         L"", // DOCREF REQUIRED
                                         TestResult::PASS,
                                         //*(new Requirement::RequirementMapSP(new Requirement::RequirementMap()))
                                         TestRegistry::GetInstance().GetRequirementsForTest( AcyclicAnalysis::GetTestInfo().GetName() )
             )                         )
{}

AcyclicVisitor::~AcyclicVisitor()
{
}

bool AcyclicVisitor::IsPresent(unsigned int lid)
{
  return ( _Set.find( lid ) != _Set.end() );
}

bool AcyclicVisitor::PreOrderVisit(Node& node)
{
  Node::LID lid = node.GetLID();

  if ( _Set.insert(lid).second )
  {
    
    //The node was properly inserted into the set, therefore, there a cycle has
    //not yet been detected.  If this node has already been visited, that means
    //that there are no cycles down this sub-branch, so the node can be removed
    //from the set and the traversal of this sub-branch can be halted.
    if ( this->HaveVisited( lid ) )
    {
        _Set.erase( lid );
    }
    
    //add node into the vector since it was visited on this sub-branch's traversal
    _Vector.push_back(lid);
    this->RecordVisit( lid );
    return true;
  }
 
  // a cycle was detected
  _spResult->SetExplanation(L"Cycle detected.!");
  _spResult->SetResult(TestResult::FAIL);

  //TODO: Dump something more useful than the vector of LIDs.
  _os << "Nodes of the cycle:" << endl;
  for(unsigned int i = 0; i < _Vector.size(); i++)
  {
    _os << _Vector.at(i) << endl;
  }
  _os << lid << endl;

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

  //an unkown error occured
  _spResult->SetExplanation(L"Error: Unknown error occured during postorder visit!");
  _spResult->SetResult(TestResult::FAIL);

  return false;
}

void AcyclicVisitor::Erase(unsigned int lid)
{
  _Vector.pop_back();
  _Set.erase( lid );
}

shared_ptr<const DetailLevelTestResult> AcyclicVisitor::GetTestResult() const
{
  return _spResult; 
}

} // end of namespace aafanalyzer
