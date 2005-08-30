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

#include "DumpPhase.h"

#include "FileDumper.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 	
{

DumpPhase::DumpPhase(std::ostream& os, boost::shared_ptr<TestGraph> spTestGraph) 
: TestPhase(os),
  _spTestGraph(spTestGraph)
{
}

DumpPhase::~DumpPhase()
{
}

boost::shared_ptr<TestGraph> DumpPhase::GetTestGraph()
{
  return _spTestGraph;
}

boost::shared_ptr<TestResult> DumpPhase::Execute()
{
  boost::shared_ptr<TestResult> spLoadTest(new TestResult());
  spLoadTest->SetName(L"DumpPhase");
  spLoadTest->SetDescription(L"Output the contents of the AAF parse tree.");

  //dump the aaf file graph to screen
  FileDumper dumper(GetOutStream(), GetTestGraph());
  spLoadTest->AppendSubtestResult(dumper.Execute());
  spLoadTest->SetResult(spLoadTest->GetAggregateResult());

  return spLoadTest;
}

} // end of namespace diskstream
