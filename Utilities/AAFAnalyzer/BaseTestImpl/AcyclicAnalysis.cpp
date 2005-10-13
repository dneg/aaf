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
#include "AcyclicAnalysis.h"
#include <AcyclicVisitor.h>

//Test/Result files
#include <TestLevelTestResult.h>

//AAF Analyzer Base files
#include <TestGraph.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//STL files
#include <vector>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;
using namespace boost;

AcyclicAnalysis::AcyclicAnalysis(wostream& os, shared_ptr<const TestGraph> spGraph)
: Test(os, GetTestInfo())
{
  SetTestGraph(spGraph);
}

AcyclicAnalysis::~AcyclicAnalysis()
{
}

shared_ptr<TestLevelTestResult> AcyclicAnalysis::Execute()
{

  shared_ptr<AcyclicVisitor> spVisitor(new AcyclicVisitor(GetOutStream()));
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << endl << GetDescription() << endl << endl;

  //set result properties

  const shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  shared_ptr<TestLevelTestResult> spResult(new TestLevelTestResult(me, spMyReqs ));
  spResult->SetName(GetName());
  spResult->SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 
  
  shared_ptr<const DetailLevelTestResult> spVisitorResult( spVisitor->GetTestResult() );

  //Store sub results.
  spResult->AppendSubtestResult(spVisitorResult);
  spResult->SetResult( spResult->GetAggregateResult() );

  return spResult;
}

AxString AcyclicAnalysis::GetName() const
{
  AxString name = L"Acyclic Analysis Test";
  return name;
}

AxString AcyclicAnalysis::GetDescription() const
{
  AxString description = L"Verify that source references are not cyclic.";
  return description;
}

const TestInfo AcyclicAnalysis::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    //TODO: Push actual requirements.
//    spReqIds->push_back(L"Requirement Id");
    return TestInfo(L"AcyclicAnalysis", spReqIds);
}

} // end of namespace aafanalyzer
