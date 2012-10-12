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
                    boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPNameTest::~EPNameTest()
{}

boost::shared_ptr<TestLevelTestResult> EPNameTest::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  boost::shared_ptr<EPNameVisitor> spVisitor( new EPNameVisitor( GetOutStream(),
									                      GetTestGraph()->GetEdgeMap(),
														  spTestResult ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
  spVisitor->CheckForUniqueNames();  
  
  return spTestResult;
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
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_027");     //Top-Level Composition
    spReqIds->push_back(L"REQ_EP_032");     //Lower-Level Composition
    spReqIds->push_back(L"REQ_EP_038");     //Sub-Clip Composition
    spReqIds->push_back(L"REQ_EP_047");     //Adjusted Clip Composition
    spReqIds->push_back(L"REQ_EP_052");     //Template Clip
    spReqIds->push_back(L"REQ_EP_057");     //Clip
    spReqIds->push_back(L"REQ_EP_073");     //Recording Source
    spReqIds->push_back(L"REQ_EP_081");     //Tape Source
    spReqIds->push_back(L"REQ_EP_086");     //Film Source
    spReqIds->push_back(L"REQ_EP_101");     //Essence Tracks
    spReqIds->push_back(L"REQ_EP_161");     //Definition Objects
    return TestInfo(L"EPNameTest", spReqIds);
}

} // end of namespace aafanalyzer
