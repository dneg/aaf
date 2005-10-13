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
#include "FileLoad.h"

//Test/Result files
#include <TestLevelTestResult.h>

//AAF Analyzer Base files
#include <GraphBuilder.h>
#include <NodeFactoryImpl.h>
#include <AAFGraphInfo.h>

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

FileLoad::FileLoad(wostream& os, const basic_string<wchar_t> AAFFile)
  : Test(os, GetTestInfo()),
    _File(AAFFile)
{
}

FileLoad::~FileLoad()
{
}

shared_ptr<TestLevelTestResult> FileLoad::Execute()
{

  GraphBuilder graphBuild;
  shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());

  //output to screen
  //GetOutStream() << GetName() << endl << GetDescription() << endl << endl;

  //build the graph and initialize TestGraph 
  shared_ptr<const AAFGraphInfo> spGraphInfo( graphBuild.CreateGraph(_File, spFactory) );
  SetTestGraph(spGraphInfo->GetGraph());
  _spGraphInfo = spGraphInfo;
  
  // Set result properties.
  // If the graph builder did not through an exception, then
  // it succeeded, therefore, result is PASS.
  const shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  shared_ptr<TestLevelTestResult> spResult(
            new TestLevelTestResult( L"FileLoad",
                                     L"Loads and AAF file and builds the test graph.",
                                     L"File loaded correctly.",
                                     L"-", // DOCREF
                                     TestResult::PASS,
                                     me, 
                                     spMyReqs ) );
  return spResult;
}

AxString FileLoad::GetName() const
{
  AxString name = L"File Load Test";
  return name;
}

AxString FileLoad::GetDescription() const
{
  AxString description = L"Load an AAF file and build a graph of contained AAF objects.";
  return description;
}

const TestInfo FileLoad::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    //TODO: Push actual requirements.
//    spReqIds->push_back(L"Requirement Id");
    return TestInfo(L"FileLoad", spReqIds);
}

shared_ptr<const AAFGraphInfo> FileLoad::GetTestGraphInfo()
{
  return _spGraphInfo;
}

} // end of namespace diskstream
