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
#include "FileDumper.h"
#include "TestVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <TestGraph.h>

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

FileDumper::FileDumper(wostream& os, shared_ptr<const TestGraph> spGraph)
: Test(os, GetTestInfo())
{
  SetTestGraph(spGraph);
}

FileDumper::~FileDumper()
{
}

shared_ptr<TestLevelTestResult> FileDumper::Execute()
{
  shared_ptr<TestVisitor> spVisitor(new TestVisitor());
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << endl << GetDescription() << endl << endl;

  const shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  shared_ptr<TestLevelTestResult> spResult(new TestLevelTestResult(me, spMyReqs ));

  //set result properties
  spResult->SetName(GetName());
  spResult->SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 
  return spResult;
}

AxString FileDumper::GetName() const
{
  AxString name = L"File Dump";
  return name;
}

AxString FileDumper::GetDescription() const
{
  AxString description = L"Dump an dobject graph..";
  return description;
}

const TestInfo FileDumper::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    //TODO: Push actual requirements.
//    spReqIds->push_back(L"Requirement Id");
    return TestInfo(L"FileDumper", spReqIds);
}

} // end of namespace diskstream
