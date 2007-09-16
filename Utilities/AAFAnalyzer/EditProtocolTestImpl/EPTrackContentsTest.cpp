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
#include "EPTrackContentsTest.h"
#include "EPTrackContentsVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Track Contents Test";
const wchar_t* TEST_DESC = L"Verify the correctness of data within the tracks of Edit Protocol mobs.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPTrackContentsTest::EPTrackContentsTest( wostream& log,
                                            shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPTrackContentsTest::~EPTrackContentsTest()
{}

shared_ptr<TestLevelTestResult> EPTrackContentsTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  shared_ptr<EPTrackContentsVisitor>
    spVisitor(new EPTrackContentsVisitor( GetOutStream(),
					  GetTestGraph()->GetEdgeMap(),
					  spTestResult ) );
  
  DepthFirstTraversal dfs( GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode() );
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
  
  return spTestResult;
}

AxString EPTrackContentsTest::GetName() const
{
  return TEST_NAME;
}

AxString EPTrackContentsTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPTrackContentsTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_103");       // Physical Track Numbers
    spReqIds->push_back(L"REQ_EP_108.1");     // Marked IN exists
    spReqIds->push_back(L"REQ_EP_108.2");     // Marked OUT exists
    spReqIds->push_back(L"REQ_EP_108.3");     // assert mark OUT > IN 
    spReqIds->push_back(L"REQ_EP_108.4");     // UsePosition exists
    return TestInfo(L"EPTrackContentsTest", spReqIds);
}

} // end of namespace aafanalyzer
