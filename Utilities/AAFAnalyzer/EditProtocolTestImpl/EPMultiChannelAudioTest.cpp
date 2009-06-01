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

#include <iostream>

namespace {

using namespace std;
using namespace boost;
using namespace aafanalyzer;

const wchar_t* TEST_NAME = L"Edit Protocol Multi-Channel Audio Test";
const wchar_t* TEST_DESC = L"Verify that multi-channel audio is used correctly within the AAF file.";

//======================================================================

class Analyzer
{
public:
  Analyzer( wostream& os,
            shared_ptr<const TestGraph> spGraph,
            shared_ptr<TestLevelTestResult> spTestResult )
    : _os( os ),
      _spGraph( spGraph ),
      _spTestResult( spTestResult )
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spRootComposition )
  {
    shared_ptr<EPMultiChannelAudioVisitor>
      spVisitor( new EPMultiChannelAudioVisitor( _os, _spTestResult ) );

    DepthFirstTraversal dfs( _spGraph->GetEdgeMap(), spRootComposition );
    dfs.TraverseDown( spVisitor );
  }

private:
  wostream& _os;
  shared_ptr<const TestGraph> _spGraph;
  shared_ptr<TestLevelTestResult> _spTestResult;
  
  // prohibited
  Analyzer();
  Analyzer& operator=( const Analyzer& );
};


} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPMultiChannelAudioTest::EPMultiChannelAudioTest( wostream& log,
						  shared_ptr<const TestGraph> spGraph,
						  CompMobDependency::CompMobNodeVectorSP spTopLevelCompMobs )
  : Test( log, GetTestInfo() ),
    _spTopLevelCompMobs( spTopLevelCompMobs )
{
  SetTestGraph(spGraph);
}

EPMultiChannelAudioTest::~EPMultiChannelAudioTest()
{}

shared_ptr<TestLevelTestResult> EPMultiChannelAudioTest::Execute()
{
  shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  Analyzer analyzer( GetOutStream(), GetTestGraph(), spTestResult );

  for_each( _spTopLevelCompMobs->begin(), _spTopLevelCompMobs->end(), analyzer );

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
