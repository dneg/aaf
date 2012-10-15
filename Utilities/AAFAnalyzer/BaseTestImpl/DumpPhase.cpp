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

//Base Test files
#include "DumpPhase.h"
#include "TestGraph.h"

//Test/Result files
#include <TestPhaseLevelTestResult.h>
#include <TestLevelTestResult.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 	
{
    
using namespace std;

DumpPhase::DumpPhase(wostream& os, boost::shared_ptr<const TestGraph> spGraph)
  : TestPhase(os),
    _spGraph(spGraph),
    _spRoot(spGraph->GetRootNode()),
    _followReferences(false)
{}

DumpPhase::DumpPhase(wostream& os, boost::shared_ptr<const TestGraph> spGraph, boost::shared_ptr<Node> spRoot)
  : TestPhase(os),
    _spGraph(spGraph),
    _spRoot(spRoot),
    _followReferences(true)
{}

DumpPhase::~DumpPhase()
{}

boost::shared_ptr<const TestGraph> DumpPhase::GetTestGraph()
{
  return _spGraph;
}

boost::shared_ptr<TestPhaseLevelTestResult> DumpPhase::Execute()
{
  boost::shared_ptr<TestPhaseLevelTestResult> spLoadTest(new TestPhaseLevelTestResult());
  spLoadTest->SetName(L"DumpPhase");
  spLoadTest->SetDescription(L"Output the contents of the AAF parse tree.");

  //dump the aaf file graph to screen
  boost::shared_ptr<FileDumper> dumper(new FileDumper(GetOutStream(), GetTestGraph(), _spRoot, _followReferences));
  
  boost::shared_ptr<TestLevelTestResult> spTestResult( dumper->Execute() );
  spLoadTest->AppendSubtestResult(spTestResult);

  return spLoadTest;
}

} // end of namespace diskstream
