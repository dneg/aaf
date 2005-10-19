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
#include "EPNameTest.h"
#include "EPNameVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Naming Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob names.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPNameTest::EPNameTest( wostream& log,
                    shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPNameTest::~EPNameTest()
{}

shared_ptr<TestLevelTestResult> EPNameTest::Execute()
{
    
    shared_ptr<EPNameVisitor> spVisitor(new EPNameVisitor( GetOutStream() ) );

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
    spVisitor->CheckForUniqueNames();  

    spResult->AppendSubtestResult( spVisitor->GetResult() );

    spResult->SetResult( spResult->GetAggregateResult() );
    if ( spResult->GetResult() == TestResult::FAIL )
    {
        spResult->SetExplanation(L"Test Failed - See \"Edit Protocol Naming Visitor\" Visitor for details");
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

AxString EPNameTest::GetName() const
{
  return TEST_NAME;
}

AxString EPNameTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPNameTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_027");     //Top-Level Composition
    spReqIds->push_back(L"REQ_EP_032");     //Lower-Level Composition
    spReqIds->push_back(L"REQ_EP_038");     //Sub-Clip Composition
    spReqIds->push_back(L"REQ_EP_047");     //Adjusted Clip Composition
    spReqIds->push_back(L"REQ_EP_052");     //Template Clip
    spReqIds->push_back(L"REQ_EP_057");     //Clip
    spReqIds->push_back(L"REQ_EP_073");     //Recording Source
    spReqIds->push_back(L"REQ_EP_081");     //Tape Source
    spReqIds->push_back(L"REQ_EP_086");     //Film Source
    return TestInfo(L"EPNameTest", spReqIds);
}

} // end of namespace aafanalyzer
