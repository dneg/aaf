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
                                            boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPContainedTrackTest::~EPContainedTrackTest()
{}

boost::shared_ptr<TestLevelTestResult> EPContainedTrackTest::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  boost::shared_ptr<EPContainedTrackVisitor>
    spVisitor( new EPContainedTrackVisitor( GetOutStream(),
					    GetTestGraph()->GetEdgeMap(),
					    spTestResult ) );
  
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
    
  return spTestResult;
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
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
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
