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

#include "LoadPhase.h"

#include <TestGraph.h>
#include <TestLevelTestResult.h>

namespace {

using namespace aafanalyzer;

const wchar_t* PHASE_NAME = L"Load Phase";
const wchar_t* PHASE_DESC = L"Load an AAF file, reslove references, and ensure the graph is acyclic.";

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 	
{

LoadPhase::LoadPhase(std::wostream& os, const std::basic_string<wchar_t> AAFFile) 
  : TestPhase(os),
    _FileName(AAFFile)
{}

LoadPhase::~LoadPhase()
{}

boost::shared_ptr<TestGraph> LoadPhase::GetTestGraph()
{
  return _spTestGraph;
}

boost::shared_ptr<TestPhaseLevelTestResult> LoadPhase::Execute() 
{

  boost::shared_ptr<TestPhaseLevelTestResult> spLoadTest(
                            new TestPhaseLevelTestResult( PHASE_NAME,
                                                          PHASE_DESC,
                                                          L"", // explain
                                                          L"", // docref
                                                          TestResult::PASS ));

  //load the AAF file and create the graph
  boost::shared_ptr<FileLoad> load(new FileLoad(GetOutStream(), _FileName));
  boost::shared_ptr<const TestLevelTestResult> spTestResult( load->Execute() );
  spLoadTest->AppendSubtestResult(spTestResult);

  //get the TestGraph object we need for other tests
  _spTestGraph = load->GetTestGraph();
  //resolve all the references in the AAF graph
  boost::shared_ptr<RefResolver> ref(new RefResolver(GetOutStream(), _spTestGraph));
  spTestResult = ref->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  //ensure the AAF file graph is acyclic
  boost::shared_ptr<AcyclicAnalysis> acy(new AcyclicAnalysis(GetOutStream(), _spTestGraph));
  spTestResult = acy->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  spLoadTest->SetResult(spLoadTest->GetAggregateResult());

  return spLoadTest;
}

} // end of namespace diskstream
