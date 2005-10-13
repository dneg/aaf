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
#include "LoadPhase.h"

//Test/Result files
#include <TestGraph.h>
#include <TestPhaseLevelTestResult.h>
#include <TestLevelTestResult.h>

//AAF Analyzer Base files
#include <AAFGraphInfo.h>

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
    
using namespace std;
using namespace boost;

LoadPhase::LoadPhase(wostream& os, const basic_string<wchar_t> AAFFile) 
  : TestPhase(os),
    _FileName(AAFFile)
{}

LoadPhase::~LoadPhase()
{}

shared_ptr<const AAFGraphInfo> LoadPhase::GetTestGraphInfo()
{
  return _spGraphInfo;
}

shared_ptr<TestPhaseLevelTestResult> LoadPhase::Execute() 
{

  shared_ptr<TestPhaseLevelTestResult> spLoadTest(
                            new TestPhaseLevelTestResult( PHASE_NAME,
                                                          PHASE_DESC,
                                                          L"", // explain
                                                          L"", // docref
                                                          TestResult::PASS ));

  //load the AAF file and create the graph
  shared_ptr<FileLoad> load(new FileLoad(GetOutStream(), _FileName));
  shared_ptr<const TestLevelTestResult> spTestResult( load->Execute() );
  spLoadTest->AppendSubtestResult(spTestResult);

  //get the TestGraph object we need for other tests
  _spGraphInfo = load->GetTestGraphInfo();
  //resolve all the references in the AAF graph
  shared_ptr<RefResolver> ref(new RefResolver(GetOutStream(), _spGraphInfo->GetGraph()));
  spTestResult = ref->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  //ensure the AAF file graph is acyclic
  shared_ptr<AcyclicAnalysis> acy(new AcyclicAnalysis(GetOutStream(), _spGraphInfo->GetGraph()));
  spTestResult = acy->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  spLoadTest->SetResult(spLoadTest->GetAggregateResult());

  return spLoadTest;
}

} // end of namespace diskstream
