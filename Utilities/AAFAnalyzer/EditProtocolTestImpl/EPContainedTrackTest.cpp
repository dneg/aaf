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
#include "EPContainedTrackTest.h"
#include "EPContainedTrackVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Mob Contained Tracks Test";
const wchar_t* TEST_DESC = L"Verify the correctness of track types within Edit Protocol mobs.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPContainedTrackTest::EPContainedTrackTest( wostream& log,
                                            shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPContainedTrackTest::~EPContainedTrackTest()
{}

shared_ptr<TestLevelTestResult> EPContainedTrackTest::Execute()
{

    shared_ptr<EPContainedTrackVisitor> spVisitor(new EPContainedTrackVisitor( GetOutStream(), GetTestGraph()->GetEdgeMap() ) );

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
        spResult->SetExplanation(L"Test Failed - See \"Edit Protocol Contained Track Visitor\" Visitor for details");
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

AxString EPContainedTrackTest::GetName() const
{
  return TEST_NAME;
}

AxString EPContainedTrackTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPContainedTrackTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_028");     //Top-Level Composition
    spReqIds->push_back(L"REQ_EP_037");     //Sub-Clip Composition
    spReqIds->push_back(L"REQ_EP_046");     //Adjusted Clip Composition
    spReqIds->push_back(L"REQ_EP_082");     //Tape Source
    spReqIds->push_back(L"REQ_EP_087");     //Film Source
    spReqIds->push_back(L"REQ_EP_131");     //Primary timecode track contents for Top-Level and Lower-Level Compositions.
    spReqIds->push_back(L"REQ_EP_136");     //Auxiliary Source
    return TestInfo(L"EPContainedTrackTest", spReqIds);
}

} // end of namespace aafanalyzer
