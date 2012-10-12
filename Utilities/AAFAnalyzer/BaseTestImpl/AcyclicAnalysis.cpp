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
#include "AcyclicAnalysis.h"
#include <AcyclicVisitor.h>

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

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

AcyclicAnalysis::AcyclicAnalysis(wostream& os, boost::shared_ptr<const TestGraph> spGraph)
  : Test(os, GetTestInfo()),
    _isCyclic(false)
{
  SetTestGraph(spGraph);
}

AcyclicAnalysis::~AcyclicAnalysis()
{}

boost::shared_ptr<TestLevelTestResult> AcyclicAnalysis::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = this->CreateTestResult();

  boost::shared_ptr<AcyclicVisitor> spVisitor( new AcyclicVisitor(GetOutStream(), spTestResult) );
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 

  _isCyclic = spVisitor->IsCycleDetected();
  
  return spTestResult;
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
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // Acyclic Derivation Chain
    spReqIds->push_back(L"REQ_EP_256");  

    return TestInfo(L"AcyclicAnalysis", spReqIds);
}

bool AcyclicAnalysis::IsCyclic() const
{
  return _isCyclic;
}

} // end of namespace aafanalyzer
