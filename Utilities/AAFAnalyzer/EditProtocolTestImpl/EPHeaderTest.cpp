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
#include "EPHeaderTest.h"
#include "EPHeaderVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

//AAF Analyzer Base files
#include <TestGraph.h>

//Requirement files
#include <Requirement.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

namespace {

const wchar_t* TEST_NAME = L"Edit Protocol Header Test";
const wchar_t* TEST_DESC = L"Verify the AAF file header is valid.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPHeaderTest::EPHeaderTest( wostream& log, boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPHeaderTest::~EPHeaderTest()
{}

boost::shared_ptr<TestLevelTestResult> EPHeaderTest::Execute()
{
   boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();
    
   boost::shared_ptr<EPHeaderVisitor> spVisitor(new EPHeaderVisitor( GetOutStream(), spTestResult ) );
   
   DepthFirstTraversal dfs( GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode() );
   dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );
   
   return spTestResult;
}

AxString EPHeaderTest::GetName() const
{
  return TEST_NAME;
}

AxString EPHeaderTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPHeaderTest::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_255");     //Operational Pattern
    return TestInfo(L"EPHeaderTest", spReqIds);
}

} // end of namespace aafanalyzer
