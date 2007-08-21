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

//Edit Protocol Test files
#include "EPParameterTest.h"
#include "EPParameterVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

//AAF Analyzer Base files
#include <TestGraph.h>

//Requirement files
#include <Requirement.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//STL files
#include <vector>

namespace {

const wchar_t* TEST_NAME = L"Edit Protocol Parameter Test";
const wchar_t* TEST_DESC = L"Verify that all parameters in the AAF file are valid.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPParameterTest::EPParameterTest( wostream& log,
                              shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPParameterTest::~EPParameterTest()
{}

shared_ptr<TestLevelTestResult> EPParameterTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  shared_ptr<EPParameterVisitor> spVisitor(new EPParameterVisitor( GetOutStream(),
								   GetTestGraph()->GetEdgeMap(),
								   spTestResult ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
  
  return spTestResult;
}

AxString EPParameterTest::GetName() const
{
  return TEST_NAME;
}

AxString EPParameterTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPParameterTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_174");     //Must be constant or varying.
    spReqIds->push_back(L"REQ_EP_175");     //Valid Interpolators.
    return TestInfo(L"EPParameterTest", spReqIds);
}

} // end of namespace aafanalyzer
