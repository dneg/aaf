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

#include "AcyclicAnalysis.h"

#include <DepthFirstTraversal.h>
#include <AcyclicVisitor.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

AcyclicAnalysis::AcyclicAnalysis(std::wostream& os, boost::shared_ptr<TestGraph> spTestGraph)
: Test(os)
{
  SetTestGraph(spTestGraph);
}

AcyclicAnalysis::~AcyclicAnalysis()
{
}

boost::shared_ptr<TestResult> AcyclicAnalysis::Execute()
{

  boost::shared_ptr<AcyclicVisitor> spVisitor(new AcyclicVisitor(GetOutStream()));
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << std::endl << GetDescription() << std::endl << std::endl;

  //set result properties
  boost::shared_ptr<TestResult> spResult(new TestResult());
  spResult->SetName(GetName());
  spResult->SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 

  boost::shared_ptr<const TestResult> spVisitorResult(spVisitor->GetTestResult());

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

} // end of namespace aafanalyzer
