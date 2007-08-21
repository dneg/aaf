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

  // Build the graph and initialize TestGraph.
  shared_ptr<const AAFGraphInfo> spGraphInfo( graphBuild.CreateGraph(_File, spFactory) );
  SetTestGraph(spGraphInfo->GetGraph());
  _spGraphInfo = spGraphInfo;
  
  // If the graph builder did not through an exception, then it
  // succeeded, therefore, result is PASS.
  return CreateTestResult( L"File loaded correctly.", TestResult::PASS );
}

AxString FileLoad::GetName() const
{
  AxString name = L"File Load";
  return name;
}

AxString FileLoad::GetDescription() const
{
  AxString description = L"Load all objects in the AAF file.";
  return description;
}

const TestInfo FileLoad::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // There are no requirements to associate with this test.  Either
    // it passes, and the program continues, or the graph builder
    // throws an exception that stops the program.

    return TestInfo(L"FileLoad", spReqIds);
}

shared_ptr<const AAFGraphInfo> FileLoad::GetTestGraphInfo()
{
  return _spGraphInfo;
}

} // end of namespace diskstream
