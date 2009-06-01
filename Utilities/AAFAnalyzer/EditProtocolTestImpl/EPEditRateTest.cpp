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
#include "EPEditRateTest.h"
#include "EPEditRateVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Edit Rate Test";
const wchar_t* TEST_DESC = L"Verify that all essence tracks have valid edit rates.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPEditRateTest::EPEditRateTest( wostream& log,
                              shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPEditRateTest::~EPEditRateTest()
{}

shared_ptr<TestLevelTestResult> EPEditRateTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();
    
  shared_ptr<EPEditRateVisitor>
    spVisitor( new EPEditRateVisitor( GetOutStream(),
	                                  GetTestGraph()->GetEdgeMap(),
									  spTestResult ) );
  
  DepthFirstTraversal dfs( GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode() );
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
  spVisitor->CheckAudioSampleRates();
  
  return spTestResult;
}

AxString EPEditRateTest::GetName() const
{
  return TEST_NAME;
}

AxString EPEditRateTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPEditRateTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_091");     //In table
    spReqIds->push_back(L"REQ_EP_092");     //Numerator/Denominator > 0
    spReqIds->push_back(L"REQ_EP_099");     //Audio Tracks
    spReqIds->push_back(L"REQ_EP_100");     //Video Tracks
    return TestInfo(L"EPEditRateTest", spReqIds);
}

} // end of namespace aafanalyzer
