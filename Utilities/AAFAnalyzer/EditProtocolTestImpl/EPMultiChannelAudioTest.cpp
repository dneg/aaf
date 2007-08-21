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
#include "EPMultiChannelAudioTest.h"
#include "EPMultiChannelAudioVisitor.h"

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

const wchar_t* TEST_NAME = L"Edit Protocol Multi-Channel Audio Test";
const wchar_t* TEST_DESC = L"Verify that multi-channel audio is used correctly within the AAF file.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPMultiChannelAudioTest::EPMultiChannelAudioTest( wostream& log,
                                            shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPMultiChannelAudioTest::~EPMultiChannelAudioTest()
{}

shared_ptr<TestLevelTestResult> EPMultiChannelAudioTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  shared_ptr<EPMultiChannelAudioVisitor>
	  spVisitor( new EPMultiChannelAudioVisitor( GetOutStream(), spTestResult ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );

  return spTestResult;
}

AxString EPMultiChannelAudioTest::GetName() const
{
  return TEST_NAME;
}

AxString EPMultiChannelAudioTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPMultiChannelAudioTest::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"REQ_EP_110");     //Audio in a Composition or Master Mob
    return TestInfo(L"EPMultiChannelAudioTest", spReqIds);
}

} // end of namespace aafanalyzer
