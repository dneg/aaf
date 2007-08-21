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
using namespace boost;

DumpPhase::DumpPhase(wostream& os, shared_ptr<const TestGraph> spGraph)
  : TestPhase(os),
    _spGraph(spGraph),
    _spRoot(spGraph->GetRootNode()),
    _followReferences(false)
{}

DumpPhase::DumpPhase(wostream& os, shared_ptr<const TestGraph> spGraph, shared_ptr<Node> spRoot)
  : TestPhase(os),
    _spGraph(spGraph),
    _spRoot(spRoot),
    _followReferences(true)
{}

DumpPhase::~DumpPhase()
{}

shared_ptr<const TestGraph> DumpPhase::GetTestGraph()
{
  return _spGraph;
}

shared_ptr<TestPhaseLevelTestResult> DumpPhase::Execute()
{
  shared_ptr<TestPhaseLevelTestResult> spLoadTest(new TestPhaseLevelTestResult());
  spLoadTest->SetName(L"DumpPhase");
  spLoadTest->SetDescription(L"Output the contents of the AAF parse tree.");

  //dump the aaf file graph to screen
  shared_ptr<FileDumper> dumper(new FileDumper(GetOutStream(), GetTestGraph(), _spRoot, _followReferences));
  
  shared_ptr<TestLevelTestResult> spTestResult( dumper->Execute() );
  spLoadTest->AppendSubtestResult(spTestResult);

  return spLoadTest;
}

} // end of namespace diskstream
