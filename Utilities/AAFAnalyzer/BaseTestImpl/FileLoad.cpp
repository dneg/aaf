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

FileLoad::FileLoad(wostream& os, const basic_string<wchar_t> AAFFile)
  : Test(os, GetTestInfo()),
    _File(AAFFile)
{
}

FileLoad::~FileLoad()
{
}

boost::shared_ptr<TestLevelTestResult> FileLoad::Execute()
{

  GraphBuilder graphBuild;
  boost::shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());

  // Build the graph and initialize TestGraph.
  boost::shared_ptr<const AAFGraphInfo> spGraphInfo( graphBuild.CreateGraph(_File, spFactory) );
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
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // There are no requirements to associate with this test.  Either
    // it passes, and the program continues, or the graph builder
    // throws an exception that stops the program.

    return TestInfo(L"FileLoad", spReqIds);
}

boost::shared_ptr<const AAFGraphInfo> FileLoad::GetTestGraphInfo()
{
  return _spGraphInfo;
}

} // end of namespace diskstream
