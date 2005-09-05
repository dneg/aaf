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

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 	
{

LoadPhase::LoadPhase(std::ostream& os, const std::basic_string<wchar_t> AAFFile) 
: TestPhase(os),
  _FileName(AAFFile)
{
}

LoadPhase::~LoadPhase()
{
}

boost::shared_ptr<TestGraph> LoadPhase::GetTestGraph()
{
  return _spTestGraph;
}

boost::shared_ptr<TestResult> LoadPhase::Execute() 
{

  boost::shared_ptr<TestResult> spLoadTest(new TestResult());
  spLoadTest->SetName(L"LoadPhase");
  spLoadTest->SetDescription(L"Load an AAF file, reslove references, and ensure the graph is acyclic.");

  //load the AAF file and create the graph
  FileLoad load(GetOutStream(), _FileName);
  spLoadTest->AppendSubtestResult(load.Execute());

  //get the TestGraph object we need for other tests
  _spTestGraph = load.GetTestGraph();

  //resolve all the references in the AAF graph
  RefResolver ref(GetOutStream(), _spTestGraph);
  spLoadTest->AppendSubtestResult(ref.Execute());

  //ensure the AAF file graph is acyclic
  AcyclicAnalysis acy(GetOutStream(), _spTestGraph);
  spLoadTest->AppendSubtestResult(acy.Execute());
  spLoadTest->SetResult(spLoadTest->GetAggregateResult());

  return spLoadTest;
}

} // end of namespace diskstream
