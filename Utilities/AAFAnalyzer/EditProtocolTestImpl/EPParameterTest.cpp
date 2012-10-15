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
                              boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPParameterTest::~EPParameterTest()
{}

boost::shared_ptr<TestLevelTestResult> EPParameterTest::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  boost::shared_ptr<EPParameterVisitor> spVisitor(new EPParameterVisitor( GetOutStream(),
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
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_174");     //Must be constant or varying.
    spReqIds->push_back(L"REQ_EP_175");     //Valid Interpolators.
    return TestInfo(L"EPParameterTest", spReqIds);
}

} // end of namespace aafanalyzer
