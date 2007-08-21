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
#include "EPLocatorTest.h"
#include "EPLocatorVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Locator Test";
const wchar_t* TEST_DESC = L"Verify that valid Locators are present where needed.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPLocatorTest::EPLocatorTest( wostream& log,
                              shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPLocatorTest::~EPLocatorTest()
{}

shared_ptr<TestLevelTestResult> EPLocatorTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  shared_ptr<EPLocatorVisitor>
    spVisitor(new EPLocatorVisitor( GetOutStream(),
				    spTestResult ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
  
  return spTestResult;
}

AxString EPLocatorTest::GetName() const
{
  return TEST_NAME;
}

AxString EPLocatorTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPLocatorTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_059");
    spReqIds->push_back(L"REQ_EP_060");
    spReqIds->push_back(L"REQ_EP_061");
    spReqIds->push_back(L"REQ_EP_075");
    return TestInfo(L"EPLocatorTest", spReqIds);
}

} // end of namespace aafanalyzer
