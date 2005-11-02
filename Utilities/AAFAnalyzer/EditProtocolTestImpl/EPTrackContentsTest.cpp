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
    
    shared_ptr<EPTrackContentsVisitor> spVisitor(new EPTrackContentsVisitor( GetOutStream() ) );

    DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
    
    const shared_ptr<const Test> me = this->shared_from_this();
    Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
    shared_ptr<TestLevelTestResult> spResult( new TestLevelTestResult(TEST_NAME,
                                 TEST_DESC,
                                 L"-", // explain
                                 L"-",  // DOCREF REQUIRED
                                 TestResult::PASS,
                                 me, 
                                 spMyReqs ) );

    dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );

    spResult->AppendSubtestResult( spVisitor->GetResult() );

    spResult->SetResult( spResult->GetAggregateResult() );
    if ( spResult->GetResult() == TestResult::FAIL )
    {
        spResult->SetExplanation(L"Test Failed - See \"Edit Protocol Track Contents Visitor\" Visitor for details");
    }
    
    //Update the requirement status based upon the status of the requirements in
    //the visitor.
    for (int reqLevel = TestResult::PASS; reqLevel <= TestResult::FAIL; reqLevel++)
    {
      Requirement::RequirementMap childReqs = spVisitor->GetResult()->GetRequirements( (TestResult::Result)reqLevel );
      Requirement::RequirementMap::const_iterator iter;
      for( iter = childReqs.begin(); iter != childReqs.end(); ++iter )
      {
        spResult->SetRequirementStatus( (TestResult::Result)reqLevel, iter->second );
      }
    }

    return spResult;

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
    spReqIds->push_back(L"REQ_EP_037");     //Top-Level Composition
    spReqIds->push_back(L"REQ_EP_046");     //Tape Source
    return TestInfo(L"EPTrackContentsTest", spReqIds);
}

} // end of namespace aafanalyzer
